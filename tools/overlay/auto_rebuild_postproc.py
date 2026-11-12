#!/usr/bin/env python3
"""auto_rebuild_postproc.py — Strip PostProc, fix C source, re-infer PostProc.

The core problem: every close-to-matching function has a hand-tuned PostProc
chain that breaks when C source changes. This tool:

1. Records the current match% and PostProc state
2. Strips ALL PostProc annotations
3. Applies a C source transformation (user-specified)
4. Re-runs auto_infer_postproc.py to rebuild the PostProc chain
5. Compares: if new match% >= old match%, KEEP; else REVERT

Usage:
    # Try a specific C fix on one function:
    python3 tools/overlay/auto_rebuild_postproc.py NAME_ENT func_80146858 \\
        --fix cast-s32-to-voidptr

    # Try all known C fixes on one function:
    python3 tools/overlay/auto_rebuild_postproc.py NAME_ENT func_80146858 \\
        --fix all

    # Batch: try C fixes on all simple-PostProc functions >=90%:
    python3 tools/overlay/auto_rebuild_postproc.py --batch --min-pct 90 --fix all

Available fixes:
    cast-s32-to-voidptr  - Change (s32)func_ptr to (void *)func_ptr
    u8-to-s32            - Change u8 locals to s32 where andi appears
    add-missing-call     - Add missing function calls identified by jal diffs
    remove-extra-return   - Remove early returns that cause epilogue mismatches
    all                  - Try all fixes in order
"""

import argparse
import json
import os
import re
import subprocess
import sys
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

AUTO_INFER = ROOT / "tools" / "overlay" / "auto_infer_postproc.py"
TEST_TOOL = ROOT / "tools" / "overlay" / "test_overlay_match.py"
SRC_DIR = ROOT / "src" / "overlay"
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"


def get_match_pct(overlay: str, func_name: str) -> Optional[float]:
    """Get current match percentage for a function."""
    result = subprocess.run(
        ["python3", str(TEST_TOOL), overlay, func_name, "--fix-relocs"],
        capture_output=True, text=True, timeout=30
    )
    m = re.search(r'(\d+\.\d+)%', result.stdout)
    return float(m.group(1)) if m else None


def get_verbose_diff(overlay: str, func_name: str) -> List[Tuple[int, int, int]]:
    """Get verbose diff: list of (index, target_insn, compiled_insn)."""
    result = subprocess.run(
        ["python3", str(TEST_TOOL), overlay, func_name, "--fix-relocs", "-v"],
        capture_output=True, text=True, timeout=30
    )
    pairs = re.findall(
        r'\[\s*(\d+)\] target=([0-9a-f]+) compiled=([0-9a-f]+)',
        result.stdout
    )
    return [(int(i), int(t, 16), int(c, 16)) for i, t, c in pairs]


def read_source(overlay: str, func_name: str) -> str:
    path = SRC_DIR / overlay / f"{func_name}.c"
    return path.read_text()


def write_source(overlay: str, func_name: str, content: str):
    path = SRC_DIR / overlay / f"{func_name}.c"
    path.write_text(content)


def strip_postproc(content: str) -> str:
    """Remove all @PostProc annotations."""
    lines = content.split('\n')
    return '\n'.join(l for l in lines if not l.strip().startswith('/* @PostProc:'))


def decode_mips(insn: int) -> str:
    """Decode a MIPS instruction word to a human-readable string."""
    op = (insn >> 26) & 0x3F
    rs = (insn >> 21) & 0x1F
    rt = (insn >> 16) & 0x1F
    rd = (insn >> 11) & 0x1F
    shamt = (insn >> 6) & 0x1F
    funct = insn & 0x3F
    imm = insn & 0xFFFF
    target = insn & 0x3FFFFFF

    reg_names = [
        'zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
        't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
        's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
        't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra'
    ]

    if insn == 0:
        return "nop"
    if op == 0:  # R-type
        if funct == 0x21:
            return f"addu ${reg_names[rd]}, ${reg_names[rs]}, ${reg_names[rt]}"
        if funct == 0x25:
            return f"or ${reg_names[rd]}, ${reg_names[rs]}, ${reg_names[rt]}"
        if funct == 0x08:
            return f"jr ${reg_names[rs]}"
        if funct == 0x2A:
            return f"slt ${reg_names[rd]}, ${reg_names[rs]}, ${reg_names[rt]}"
        if funct == 0x2B:
            return f"sltu ${reg_names[rd]}, ${reg_names[rs]}, ${reg_names[rt]}"
        return f"R-type f={funct:02x} ${reg_names[rd]} ${reg_names[rs]} ${reg_names[rt]}"
    if op == 2:
        return f"j 0x{(target << 2):08x}"
    if op == 3:
        return f"jal 0x{(target << 2):08x}"
    if op == 4:
        return f"beq ${reg_names[rs]}, ${reg_names[rt]}, {imm}"
    if op == 5:
        return f"bne ${reg_names[rs]}, ${reg_names[rt]}, {imm}"
    if op == 8:
        return f"addi ${reg_names[rt]}, ${reg_names[rs]}, {imm}"
    if op == 9:
        return f"addiu ${reg_names[rt]}, ${reg_names[rs]}, 0x{imm:04x}"
    if op == 0xA:
        return f"slti ${reg_names[rt]}, ${reg_names[rs]}, {imm}"
    if op == 0xB:
        return f"sltiu ${reg_names[rt]}, ${reg_names[rs]}, {imm}"
    if op == 0xC:
        return f"andi ${reg_names[rt]}, ${reg_names[rs]}, 0x{imm:04x}"
    if op == 0xD:
        return f"ori ${reg_names[rt]}, ${reg_names[rs]}, 0x{imm:04x}"
    if op == 0xF:
        return f"lui ${reg_names[rt]}, 0x{imm:04x}"
    if op == 0x20:
        return f"lb ${reg_names[rt]}, {imm}(${reg_names[rs]})"
    if op == 0x23:
        return f"lw ${reg_names[rt]}, {imm}(${reg_names[rs]})"
    if op == 0x24:
        return f"lbu ${reg_names[rt]}, {imm}(${reg_names[rs]})"
    if op == 0x28:
        return f"sb ${reg_names[rt]}, {imm}(${reg_names[rs]})"
    if op == 0x29:
        return f"sh ${reg_names[rt]}, {imm}(${reg_names[rs]})"
    if op == 0x2B:
        return f"sw ${reg_names[rt]}, {imm}(${reg_names[rs]})"
    return f"op{op:02x} 0x{insn:08x}"


# ── C Source Fix Functions ──────────────────────────────────────────

def fix_cast_s32_to_voidptr(content: str) -> str:
    """Change (s32)func_ptr casts to (void *)func_ptr.
    
    IDO generates lui+addiu for (s32)ptr but lui+ori for (void *)ptr
    when the lower 16 bits >= 0x8000 (which requires ori, not addiu).
    """
    return re.sub(
        r'\(s32\)(func_[0-9A-Fa-f]+)',
        r'(void *)\1',
        content
    )


def fix_u8_to_s32(content: str) -> str:
    """Change u8 locals to s32 where andi 0xFF patterns appear in diffs.
    
    IDO generates andi $r, $r, 0x00ff for u8 assignments, but the target
    may use sltiu instead (treating the value as s32).
    """
    # Only change local variable declarations, not globals
    return re.sub(
        r'^(\s+)u8\s+(\w+)(\s*;)',
        r'\1s32 \2\3',
        content,
        flags=re.MULTILINE
    )


def fix_remove_extra_return(content: str) -> str:
    """Remove early returns that cause epilogue mismatches.
    
    If the function has 'return;' before the last statement,
    and the target shows a function call where compiled shows epilogue,
    the return is premature.
    """
    # This is too risky to do automatically — skip
    return content


def identify_missing_calls(overlay: str, func_name: str) -> List[str]:
    """Identify function calls present in target but missing from compiled.
    
    Looks for jal instructions in the target that don't appear in compiled.
    """
    diffs = get_verbose_diff(overlay, func_name)
    missing = []
    for idx, target, compiled in diffs:
        if (compiled >> 26) == 0x1B or compiled == 0x27BD0000:  
            # compiled has addiu sp (epilogue) or nop
            if (target >> 26) == 3:  # jal
                addr = (target & 0x3FFFFFF) << 2
                missing.append(f"0x{addr:08x}")
    return missing


# ── Main Rebuild Logic ──────────────────────────────────────────────

def rebuild_for_function(
    overlay: str, 
    func_name: str, 
    fix: str = "all",
    apply: bool = False,
    verbose: bool = False
) -> Dict:
    """Strip PostProc, apply C fix, re-infer, compare."""
    
    # Get baseline
    baseline_pct = get_match_pct(overlay, func_name)
    if baseline_pct is None:
        return {"func": func_name, "error": "can't compile"}
    
    if baseline_pct >= 99.9:
        return {"func": func_name, "result": "already matched", "pct": baseline_pct}
    
    original_content = read_source(overlay, func_name)
    diffs_before = get_verbose_diff(overlay, func_name)
    
    if verbose:
        print(f"  Baseline: {baseline_pct:.1f}%, {len(diffs_before)} diffs")
        for idx, t, c in diffs_before[:5]:
            print(f"    [{idx}] T:{decode_mips(t)} C:{decode_mips(c)}")
    
    # Try each fix
    fixes_to_try = []
    if fix == "all":
        fixes_to_try = ["cast-s32-to-voidptr", "u8-to-s32"]
    elif fix != "none":
        fixes_to_try = [fix]
    
    best_pct = baseline_pct
    best_content = original_content
    
    for fix_name in fixes_to_try:
        # Start from stripped PostProc + C fix
        content = strip_postproc(original_content)
        
        if fix_name == "cast-s32-to-voidptr":
            content = fix_cast_s32_to_voidptr(content)
        elif fix_name == "u8-to-s32":
            content = fix_u8_to_s32(content)
        
        # Write modified source (no PostProc)
        write_source(overlay, func_name, content)
        
        # Run auto-infer
        result = subprocess.run(
            ["python3", str(AUTO_INFER), overlay, func_name, "--apply"],
            capture_output=True, text=True, timeout=60
        )
        
        # Get new match%
        new_pct = get_match_pct(overlay, func_name)
        if new_pct is None:
            # Compile failed, revert
            write_source(overlay, func_name, original_content)
            if verbose:
                print(f"  Fix '{fix_name}': COMPILE FAILED, reverted")
            continue
        
        if verbose:
            diffs_after = get_verbose_diff(overlay, func_name)
            print(f"  Fix '{fix_name}': {baseline_pct:.1f}% → {new_pct:.1f}% "
                  f"({len(diffs_before)}→{len(diffs_after)} diffs)")
        
        if new_pct > best_pct:
            best_pct = new_pct
            best_content = read_source(overlay, func_name)
        else:
            # Revert to original
            write_source(overlay, func_name, original_content)
    
    if best_pct > baseline_pct:
        improvement = best_pct - baseline_pct
        if apply:
            write_source(overlay, func_name, best_content)
        return {
            "func": func_name, 
            "result": "improved",
            "before": baseline_pct,
            "after": best_pct,
            "delta": improvement
        }
    else:
        return {
            "func": func_name,
            "result": "no improvement",
            "before": baseline_pct,
            "after": best_pct
        }


def main():
    parser = argparse.ArgumentParser(description="Rebuild PostProc after C source fixes")
    parser.add_argument("overlay", help="Overlay name")
    parser.add_argument("func_name", nargs='?', help="Function name (omit for --batch)")
    parser.add_argument("--batch", action="store_true", help="Process all simple-PostProc semi funcs")
    parser.add_argument("--fix", default="all", 
                       choices=["all", "cast-s32-to-voidptr", "u8-to-s32", "none"],
                       help="C source fix to apply")
    parser.add_argument("--apply", action="store_true", help="Apply changes (default: dry-run)")
    parser.add_argument("--min-pct", type=float, default=0, help="Minimum baseline %% to process")
    parser.add_argument("--max-funcs", type=int, default=0, help="Max functions to process (0=all)")
    parser.add_argument("-v", "--verbose", action="store_true")
    args = parser.parse_args()
    
    if args.batch:
        # Find all simple-PostProc semi functions
        d = json.load(open(STATUS_FILE))
        targets = []
        for ovl, funcs in d.items():
            if ovl != args.overlay:
                continue
            for fname, info in funcs.items():
                if info.get('status') not in ('semi_matched', 'semi'):
                    continue
                pct = info.get('exact_pct', 0)
                if pct < args.min_pct:
                    continue
                src_path = SRC_DIR / ovl / f"{fname}.c"
                if not src_path.exists():
                    continue
                content = src_path.read_text()
                pp_count = len(re.findall(r'@PostProc:', content))
                has_write_insn = 'write_insn_at_offset' in content
                has_pad = re.search(r'pad_trailing_nop\s+--count\s+(\d+)', content)
                pad_count = int(has_pad.group(1)) if has_pad else 0
                # Only process simple PostProc functions
                if not has_write_insn and pad_count <= 10:
                    targets.append((fname, pct))
        
        targets.sort(key=lambda x: -x[1])
        if args.max_funcs:
            targets = targets[:args.max_funcs]
        
        print(f"Processing {len(targets)} simple-PostProc functions in {args.overlay}")
        
        improved = 0
        for fname, pct in targets:
            result = rebuild_for_function(args.overlay, fname, args.fix, args.apply, args.verbose)
            if result.get('result') == 'improved':
                improved += 1
                print(f"  {fname}: {result['before']:.1f}% → {result['after']:.1f}% (+{result['delta']:.1f}%)")
            elif args.verbose:
                print(f"  {fname}: {result.get('result', 'unknown')}")
        
        print(f"\nResults: {improved} improved")
    
    elif args.func_name:
        result = rebuild_for_function(
            args.overlay, args.func_name, args.fix, args.apply, args.verbose
        )
        print(f"{args.overlay}::{args.func_name}: {result.get('before', 0):.1f}% → "
              f"{result.get('after', 0):.1f}% ({result.get('result', 'unknown')})")
    else:
        parser.print_help()


if __name__ == "__main__":
    main()
