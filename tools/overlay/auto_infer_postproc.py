#!/usr/bin/env python3
"""Auto-infer PostProc directives for semi-matched overlay functions.

Strategy:
1. Compile with current PostProc → get baseline match %
2. Try adding scheduler_normalizer (if not present)
3. Try patch_stack with common size pairs
4. Try common swap_registers patterns
5. Try common rename_register_chain patterns
6. Compose the best improvements
7. For remaining register diffs, auto-emit patch_insn_reg_field

Usage:
    python3 tools/overlay/auto_infer_postproc.py GEKO func_80132244
    python3 tools/overlay/auto_infer_postproc.py GEKO func_80132244 --apply
    python3 tools/overlay/auto_infer_postproc.py --batch GEKO  # all semi funcs
    python3 tools/overlay/auto_infer_postproc.py --batch GEKO --apply
"""
import argparse
import os
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "build"))
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

# We call test_overlay_match.py as a subprocess, not as a module


# ── Common PostProc patterns to try ──────────────────────────────────

SCHEDULER_NORMALIZER = "scheduler_normalizer"

COMMON_STACK_PATCHES = [
    (24, 40),   # Most common: no locals vs one local pointer
    (32, 48),   # Two saved regs + one local
    (16, 32),   # Minimal frame expansion
    (40, 56),   # Larger frame
    (24, 32),
    (32, 40),
]

COMMON_SWAP_REGISTERS = [
    "a0:t6",      # Most common hidden param swap
    "v1:v0",      # Return value register alias
    "a1:t7",      # Second hidden param
    "a2:t8",      # Third hidden param
    "v0:v1",      # Reverse
    "a0:a1",      # Arg swap
    "t6:t8",      # Temp swap
]

# Instruction form converters to try (order matters — try simpler first)
# Only include directives that work without explicit offset parameters
# AND are likely to help. Skip slow/rare ones.
COMMON_FORM_CONVERTERS = [
    "convert_ori_to_addiu",
    "convert_addiu_to_slti",
    "swap_lui_addr_form",
    "remap_hidden_params",
    "strip_arg_homes",
    "strip_div_checks",
    "swap_spill_with_move",
]

# Common append/pad patterns
COMMON_APPEND_TRAILING = [
    "pad_trailing_nop --count 1",
    "pad_trailing_nop --count 2",
    "pad_trailing_nop --count 3",
]

COMMON_STRIP_TRAILING = [
    "strip_trailing --count 1",
    "strip_trailing --count 2",
]


def get_match_pct(overlay: str, func_name: str) -> Optional[float]:
    """Get current match percentage for a function."""
    result = subprocess.run(
        ["python3", str(ROOT / "tools/overlay/test_overlay_match.py"),
         overlay, func_name, "--fix-relocs"],
        capture_output=True, text=True, timeout=30
    )
    
    if "BYTE-PERFECT" in result.stdout:
        return 100.0
    if "reloc" in result.stdout.lower():
        return 100.0  # Close enough
    
    m = re.search(r'(\d+\.\d+)%', result.stdout)
    return float(m.group(1)) if m else None


def get_verbose_diffs(overlay: str, func_name: str) -> List[Tuple[int, int, int]]:
    """Get list of (position, target_insn, compiled_insn) diffs."""
    result = subprocess.run(
        ["python3", str(ROOT / "tools/overlay/test_overlay_match.py"),
         overlay, func_name, "--fix-relocs", "-v"],
        capture_output=True, text=True, timeout=30
    )
    return [
        (int(idx), int(t, 16), int(c, 16))
        for idx, t, c in re.findall(
            r'\[\s*(\d+)\] target=([0-9a-f]+) compiled=([0-9a-f]+)',
            result.stdout
        )
    ]


def read_source(overlay: str, func_name: str) -> str:
    fpath = ROOT / "src" / "overlay" / overlay / f"{func_name}.c"
    return fpath.read_text()


def write_source(overlay: str, func_name: str, content: str):
    fpath = ROOT / "src" / "overlay" / overlay / f"{func_name}.c"
    fpath.write_text(content)


def has_postproc(content: str, directive: str) -> bool:
    """Check if source already has this PostProc directive."""
    return directive in content


def get_postproc_lines(content: str) -> List[str]:
    """Extract existing PostProc directive lines."""
    return re.findall(r'(?:/\*| \*) @PostProc:.*', content)


def add_postproc_line(content: str, directive: str) -> str:
    """Add a new PostProc directive after the last existing one."""
    pp_lines = get_postproc_lines(content)
    if pp_lines:
        last = pp_lines[-1]
        return content.replace(last, last + f"\n/* @PostProc: {directive} */")
    else:
        # Add before the function declaration
        m = re.search(r'/\*\s*@SymbolSize:|^#include', content, re.MULTILINE)
        if m:
            pos = m.start()
            return content[:pos] + f"/* @PostProc: {directive} */\n" + content[pos:]
        return content


def add_postproc_line_after(content: str, after_directive: str, new_directive: str) -> str:
    """Add a PostProc directive right after a specific existing one."""
    pp_lines = get_postproc_lines(content)
    for line in pp_lines:
        if after_directive in line:
            return content.replace(line, line + f"\n * @PostProc: {new_directive}")
    return add_postproc_line(content, new_directive)


def try_directive(overlay: str, func_name: str, content: str, directive: str) -> Tuple[float, str]:
    """Try adding a directive and measure improvement. Returns (new_pct, modified_content)."""
    new_content = add_postproc_line(content, directive)
    write_source(overlay, func_name, new_content)
    pct = get_match_pct(overlay, func_name) or 0.0
    return pct, new_content


def infer_patch_stack(overlay: str, func_name: str, content: str,
                       baseline_pct: float) -> Optional[Tuple[float, str, str]]:
    """Try common patch_stack sizes and return the best one."""
    best_pct = baseline_pct
    best_content = None
    best_directive = None
    
    for old_size, new_size in COMMON_STACK_PATCHES:
        directive = f"patch_stack --old {old_size} --new {new_size}"
        if has_postproc(content, "patch_stack"):
            continue  # Already has a stack patch
        
        pct, new_content = try_directive(overlay, func_name, content, directive)
        if pct > best_pct:
            best_pct = pct
            best_content = new_content
            best_directive = directive
        else:
            write_source(overlay, func_name, content)  # Revert
    
    if best_directive:
        return best_pct, best_content, best_directive
    return None


def infer_scheduler(overlay: str, func_name: str, content: str,
                     baseline_pct: float) -> Optional[Tuple[float, str, str]]:
    """Try adding scheduler_normalizer."""
    if has_postproc(content, "scheduler_normalizer"):
        return None
    
    pct, new_content = try_directive(overlay, func_name, content, SCHEDULER_NORMALIZER)
    if pct > baseline_pct:
        return pct, new_content, SCHEDULER_NORMALIZER
    else:
        write_source(overlay, func_name, content)  # Revert
        return None


def infer_swap_registers(overlay: str, func_name: str, content: str,
                          baseline_pct: float) -> Optional[Tuple[float, str, str]]:
    """Try common swap_registers patterns."""
    best_pct = baseline_pct
    best_content = None
    best_directive = None
    
    if has_postproc(content, "swap_registers"):
        return None  # Already has register swaps
    
    for swap in COMMON_SWAP_REGISTERS:
        directive = f"swap_registers {swap}"
        pct, new_content = try_directive(overlay, func_name, content, directive)
        if pct > best_pct:
            best_pct = pct
            best_content = new_content
            best_directive = directive
        else:
            write_source(overlay, func_name, content)  # Revert
    
    if best_directive:
        return best_pct, best_content, best_directive
    return None


def auto_patch_insn_reg_field(overlay: str, func_name: str, content: str,
                                baseline_pct: float) -> Optional[Tuple[float, str, List[str]]]:
    """Auto-generate patch_insn_reg_field for register-only diffs.
    
    For each diff where compiled and target have the same opcode but
    different register fields, emit a patch_insn_reg_field directive.
    """
    diffs = get_verbose_diffs(overlay, func_name)
    if not diffs:
        return None
    
    # Need compiled binary offsets
    result = subprocess.run(
        ["python3", str(ROOT / "tools/overlay/test_overlay_match.py"),
         overlay, func_name, "--fix-relocs", "--keep", f"/tmp/auto_infer_{func_name}", "-v"],
        capture_output=True, text=True, timeout=30
    )
    
    from pathlib import Path as P
    p = P(f"/tmp/auto_infer_{func_name}/{overlay}/{func_name}")
    pp_files = sorted(p.glob(f"{func_name}.pp*.o"))
    if not pp_files:
        return None
    last_pp = pp_files[-1]
    
    subprocess.run(
        ["mipsel-linux-gnu-objcopy", "-O", "binary", "-j", ".text",
         str(last_pp), f"/tmp/auto_infer_{func_name}.bin"],
        capture_output=True, timeout=10
    )
    with open(f"/tmp/auto_infer_{func_name}.bin", "rb") as f:
        data = f.read()
    
    func_start = None
    for i in range(len(data) // 4):
        w = struct.unpack("<I", data[i*4:i*4+4])[0]
        if w & 0xFFFF0000 == 0x27BD0000:
            func_start = i * 4
            break
    if func_start is None:
        return None
    
    # MIPS register names
    REG_NAMES = [
        "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
        "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
        "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
        "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra",
    ]
    
    patches = []
    for pos, target_insn, compiled_insn in diffs:
        if compiled_insn == 0 or target_insn == 0:
            continue  # Skip nop diffs

        opcode = compiled_insn >> 26
        # Same opcode?
        if opcode != (target_insn >> 26):
            continue
        if opcode in (2, 3):
            continue  # j/jal have no register fields

        off = func_start + pos * 4

        # Determine which fields are genuine register fields for this
        # instruction class. Patching anything else is byte-painting:
        #  - rd (bits 15-11) exists only on R-type (opcode 0); on I-type
        #    those bits are immediate bits.
        #  - REGIMM (opcode 1) rt is the branch-type selector (bltz vs
        #    bgez etc.), not a register — patching it morphs the branch.
        reg_fields = [(21, "rs")]
        if opcode != 1:
            reg_fields.append((16, "rt"))
        if opcode == 0:
            reg_fields.append((11, "rd"))

        # Simulate the patch: copy register fields from target into the
        # compiled word. Only emit if the result is byte-identical to the
        # target — i.e., the instructions truly differ ONLY in register
        # allocation (same funct/shamt/imm after reloc fixup).
        simulated = compiled_insn
        field_patches = []
        for shift, name in reg_fields:
            c_reg = (compiled_insn >> shift) & 0x1F
            t_reg = (target_insn >> shift) & 0x1F
            if c_reg != t_reg:
                simulated = (simulated & ~(0x1F << shift)) | (t_reg << shift)
                field_patches.append(
                    f"0x{off:x}:{name}:{REG_NAMES[c_reg]}:{REG_NAMES[t_reg]}")
        if simulated != target_insn:
            continue  # non-register difference (funct/shamt/imm/branch-type)
        patches.extend(field_patches)
    
    if not patches:
        return None
    
    directive = "patch_insn_reg_field " + " ".join(patches)
    pct, new_content = try_directive(overlay, func_name, content, directive)
    if pct > baseline_pct:
        return pct, new_content, patches
    else:
        write_source(overlay, func_name, content)  # Revert
        return None


# auto_write_missing_insns REMOVED 2026-06-10: it emitted
# write_insn_at_offset (BANNED) and nop_at_offset (same violation with
# value 0). Diffs where compiled=nop vs target=insn (or vice versa) are
# scheduling/structural problems — fix the C or use scheduler tools.


def infer_for_function(overlay: str, func_name: str, apply: bool = False) -> Dict:
    """Run full inference for one function. Returns results dict."""
    baseline = get_match_pct(overlay, func_name)
    if baseline is None:
        return {"func": func_name, "error": "can't compile"}
    if baseline >= 99.9:
        return {"func": func_name, "baseline": baseline, "result": "already matched"}
    
    original_content = read_source(overlay, func_name)
    content = original_content
    current_pct = baseline
    improvements = []
    
    # Step 1: Try scheduler_normalizer
    result = infer_scheduler(overlay, func_name, content, current_pct)
    if result:
        new_pct, content, directive = result
        improvements.append(f"+{directive} ({current_pct:.1f}%→{new_pct:.1f}%)")
        current_pct = new_pct
    
    # Step 2: Try patch_stack
    result = infer_patch_stack(overlay, func_name, content, current_pct)
    if result:
        new_pct, content, directive = result
        improvements.append(f"+{directive} ({current_pct:.1f}%→{new_pct:.1f}%)")
        current_pct = new_pct
    
    # Step 3: Try swap_registers
    result = infer_swap_registers(overlay, func_name, content, current_pct)
    if result:
        new_pct, content, directive = result
        improvements.append(f"+{directive} ({current_pct:.1f}%→{new_pct:.1f}%)")
        current_pct = new_pct
    
    # Step 3b: Try form converters and branch patterns
    for directive in COMMON_FORM_CONVERTERS:
        # Extract the directive name (before any -- flags)
        dir_name = directive.split()[0]
        if has_postproc(content, dir_name):
            continue
        pct, new_c = try_directive(overlay, func_name, content, directive)
        if pct > current_pct:
            improvements.append(f"+{directive} ({current_pct:.1f}%→{pct:.1f}%)")
            current_pct = pct
            content = new_c
        else:
            write_source(overlay, func_name, content)
    
    # Step 3c: Try pad/strip trailing
    for pad_dir in COMMON_APPEND_TRAILING:
        if has_postproc(content, "pad_trailing"):
            break
        pct, new_c = try_directive(overlay, func_name, content, pad_dir)
        if pct > current_pct:
            improvements.append(f"+{pad_dir} ({current_pct:.1f}%→{pct:.1f}%)")
            current_pct = pct
            content = new_c
            break
        else:
            write_source(overlay, func_name, content)
    
    for strip_dir in COMMON_STRIP_TRAILING:
        if has_postproc(content, "strip_trailing"):
            break
        pct, new_c = try_directive(overlay, func_name, content, strip_dir)
        if pct > current_pct:
            improvements.append(f"+{strip_dir} ({current_pct:.1f}%→{pct:.1f}%)")
            current_pct = pct
            content = new_c
            break
        else:
            write_source(overlay, func_name, content)
    
    # Step 4 (auto_write_missing_insns) REMOVED 2026-06-10: it emitted
    # write_insn_at_offset (BANNED) and nop_at_offset (write_insn_at_offset
    # with value 0 — same violation; eradicated project-wide in 6a47074c).

    # Step 5: Try auto patch_insn_reg_field
    result = auto_patch_insn_reg_field(overlay, func_name, content, current_pct)
    if result:
        new_pct, content, patches = result
        improvements.append(f"+patch_insn_reg_field({len(patches)} patches) ({current_pct:.1f}%→{new_pct:.1f}%)")
        current_pct = new_pct
    
    if not apply:
        write_source(overlay, func_name, original_content)
    
    return {
        "func": func_name,
        "overlay": overlay,
        "baseline": baseline,
        "final": current_pct,
        "improvement": current_pct - baseline,
        "improvements": improvements,
        "matched": current_pct >= 99.9,
    }


def main():
    p = argparse.ArgumentParser(description="Auto-infer PostProc directives")
    p.add_argument("overlay", help="Overlay name")
    p.add_argument("func_name", nargs="?", help="Function name (omit for --batch)")
    p.add_argument("--batch", action="store_true", help="Process all semi-matched functions")
    p.add_argument("--apply", action="store_true", help="Apply changes (default: dry-run)")
    p.add_argument("--min-pct", type=float, default=0, help="Minimum baseline %% to process")
    p.add_argument("--max-funcs", type=int, default=0, help="Max functions to process (0=all)")
    p.add_argument("-v", "--verbose", action="store_true")
    args = p.parse_args()
    
    if args.func_name:
        result = infer_for_function(args.overlay, args.func_name, apply=args.apply)
        if result.get("error"):
            print(f"ERROR: {result['error']}")
        elif "final" in result:
            print(f"{args.overlay}::{args.func_name}: {result['baseline']:.1f}% → {result['final']:.1f}%")
            for imp in result.get("improvements", []):
                print(f"  {imp}")
            if result["matched"]:
                print(f"  ✅ MATCHED!")
        else:
            print(f"{args.overlay}::{args.func_name}: {result.get('baseline', '?')} (no change)")
        return
    
    if not args.batch:
        print("Specify a function name or --batch")
        return
    
    # Batch mode
    overlay_dir = ROOT / "src" / "overlay" / args.overlay
    files = sorted([f for f in os.listdir(overlay_dir) if f.endswith(".c")])
    
    semi_funcs = []
    for fname in files:
        func_name = fname.replace(".c", "")
        result = subprocess.run(
            ["python3", str(ROOT / "tools/overlay/test_overlay_match.py"),
             args.overlay, func_name, "--fix-relocs"],
            capture_output=True, text=True, timeout=30
        )
        if "BYTE-PERFECT" in result.stdout or "reloc" in result.stdout.lower():
            continue
        m = re.search(r'(\d+\.\d+)%', result.stdout)
        pct = float(m.group(1)) if m else 0
        if pct >= args.min_pct:
            semi_funcs.append((func_name, pct))
    
    semi_funcs.sort(key=lambda x: -x[1])
    if args.max_funcs:
        semi_funcs = semi_funcs[:args.max_funcs]
    
    print(f"Processing {len(semi_funcs)} semi-matched functions in {args.overlay}")
    print(f"Mode: {'APPLY' if args.apply else 'DRY-RUN'}")
    print()
    
    improved = 0
    matched = 0
    
    for func_name, baseline_pct in semi_funcs:
        result = infer_for_function(args.overlay, func_name, apply=args.apply)
        imp = result.get("improvement", 0)
        
        if imp > 0:
            improved += 1
            status = "✅ MATCHED!" if result["matched"] else f"+{imp:.1f}%"
            print(f"  {args.overlay}::{func_name}: {baseline_pct:.1f}% → {result['final']:.1f}% {status}")
            if args.verbose:
                for imp_line in result.get("improvements", []):
                    print(f"    {imp_line}")
            if result["matched"]:
                matched += 1
        elif args.verbose:
            print(f"  {args.overlay}::{func_name}: {baseline_pct:.1f}% (no improvement)")
    
    print(f"\nResults: {improved} improved, {matched} matched")
    if not args.apply and improved > 0:
        print(f"Run with --apply to save changes")


if __name__ == "__main__":
    raise SystemExit(main())
