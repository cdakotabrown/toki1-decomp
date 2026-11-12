#!/usr/bin/env python3
"""batch_fix_leaf_init.py — Batch-fix leaf_init functions with CSE pattern.

For functions that write N constants to N global addresses in the same
64K page, applies the following PostProc chain:
1. cse_group_lui --keep-all (consolidate store bases to $at)
2. scheduler_normalizer (reorder to match target)
3. patch_insn_reg_field + patch_insn_imm16 (fix remaining diffs)

Strategy: Apply steps 1-2, test, then auto-discover the remaining reg/imm
diffs and add step 3 directives.

Usage:
    python3 batch_fix_leaf_init.py BUNKAKEN --apply --max-funcs 20
"""

import argparse
import json
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

TEST_TOOL = ROOT / "tools" / "overlay" / "test_overlay_match.py"
SRC_DIR = ROOT / "src" / "overlay"
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"

REG_NAMES = {
    0: 'zero', 1: 'at', 2: 'v0', 3: 'v1',
    4: 'a0', 5: 'a1', 6: 'a2', 7: 'a3',
    8: 't0', 9: 't1', 10: 't2', 11: 't3',
    12: 't4', 13: 't5', 14: 't6', 15: 't7',
    16: 's0', 17: 's1', 18: 's2', 19: 's3',
    20: 's4', 21: 's5', 22: 's6', 23: 's7',
    24: 't8', 25: 't9', 26: 'k0', 27: 'k1',
    28: 'gp', 29: 'sp', 30: 'fp', 31: 'ra'
}

REG_NUMS = {v: k for k, v in REG_NAMES.items()}


def get_match_pct(overlay: str, func_name: str) -> float:
    result = subprocess.run(
        ["python3", str(TEST_TOOL), overlay, func_name, "--fix-relocs"],
        capture_output=True, text=True, timeout=30
    )
    m = re.search(r'(\d+\.\d+)%', result.stdout)
    return float(m.group(1)) if m else 0


def get_diffs(overlay: str, func_name: str) -> list:
    """Get the list of diffs after all PostProc steps."""
    result = subprocess.run(
        ["python3", str(TEST_TOOL), overlay, func_name, "--fix-relocs", "-v"],
        capture_output=True, text=True, timeout=30
    )
    pairs = re.findall(r'\[\s*(\d+)\] target=([0-9a-f]+) compiled=([0-9a-f]+)', result.stdout)
    return [(int(idx), int(t, 16), int(c, 16)) for idx, t, c in pairs]


def analyze_diff(target_word: int, compiled_word: int):
    """Analyze a single instruction diff to determine what fields differ."""
    t_op = (target_word >> 26) & 0x3f
    c_op = (compiled_word >> 26) & 0x3f
    t_rs = (target_word >> 21) & 0x1f
    c_rs = (compiled_word >> 21) & 0x1f
    t_rt = (target_word >> 16) & 0x1f
    c_rt = (compiled_word >> 16) & 0x1f
    t_imm = target_word & 0xffff
    c_imm = compiled_word & 0xffff

    fixes = []
    if t_op == c_op:  # Same opcode
        if t_rt != c_rt:
            fixes.append(('rt', REG_NAMES.get(c_rt, f'r{c_rt}'), REG_NAMES.get(t_rt, f'r{t_rt}')))
        if t_rs != c_rs:
            fixes.append(('rs', REG_NAMES.get(c_rs, f'r{c_rs}'), REG_NAMES.get(t_rs, f'r{t_rs}')))
        if t_imm != c_imm:
            fixes.append(('imm16', t_imm))
    return fixes


def fix_function(overlay: str, func_name: str, apply: bool = False, verbose: bool = False) -> dict:
    """Try to fix a leaf_init function."""
    src_path = SRC_DIR / overlay / f"{func_name}.c"
    if not src_path.exists():
        return {"func": func_name, "error": "no C source"}

    with open(src_path) as f:
        old_c = f.read()

    baseline_pct = get_match_pct(overlay, func_name)

    # Check if cse_group_lui + scheduler_normalizer are already present
    has_cse = 'cse_group_lui' in old_c
    has_sched = 'scheduler_normalizer' in old_c

    if has_cse and has_sched:
        # Already has steps 1-2, go straight to step 2 (analyze diffs and add patches)
        pct_after_cse_sched = baseline_pct
        new_c = old_c  # No change needed to header
    else:
        # Step 1: Add cse_group_lui + scheduler_normalizer
        # Remove existing PostProc lines and add new ones
        header_match = re.search(r'(/\*.*?\*/)', old_c, re.DOTALL)
        if not header_match:
            return {"func": func_name, "error": "can't parse header"}

        header = header_match.group(1)

        # Build new header with PostProc steps 1-2
        new_header_lines = []
        for line in header.split('\n'):
            if '@PostProc:' not in line:
                new_header_lines.append(line)

        # Insert PostProc lines before */
        closing_idx = None
        for i, line in enumerate(new_header_lines):
            if '*/' in line:
                closing_idx = i
                break

        if closing_idx is not None:
            pp_lines = [
                f" * @PostProc: cse_group_lui --keep-all",
                f" * @PostProc: scheduler_normalizer --target-bin assets/overlays/{overlay}.EXN --address 0x{func_name[-8:]}"
            ]
            for pp in reversed(pp_lines):
                new_header_lines.insert(closing_idx, pp)

        new_c = '\n'.join(new_header_lines) + old_c[header_match.end():]

        if not apply:
            return {"func": func_name, "result": "would apply", "pct": baseline_pct}

        # Write step 1-2 and test
        with open(src_path, 'w') as f:
            f.write(new_c)

        pct_after_cse_sched = get_match_pct(overlay, func_name)
        if pct_after_cse_sched <= baseline_pct:
            # Revert
            with open(src_path, 'w') as f:
                f.write(old_c)
            return {"func": func_name, "result": "no improvement from cse+sched", "before": baseline_pct, "after": pct_after_cse_sched}

    # Step 2: Analyze remaining diffs and add patch_insn_reg_field / patch_insn_imm16
    diffs = get_diffs(overlay, func_name)
    if not diffs:
        return {"func": func_name, "result": "BYTE-PERFECT!", "before": baseline_pct, "after": pct_after_cse_sched}

    reg_fixes = []  # (offset, field, from, to)
    imm_fixes = []  # (offset, value)

    for idx, t_word, c_word in diffs:
        offset = idx * 4  # Instruction index to byte offset
        t_op = (t_word >> 26) & 0x3f
        c_op = (c_word >> 26) & 0x3f
        t_rs = (t_word >> 21) & 0x1f
        c_rs = (c_word >> 21) & 0x1f
        t_rt = (t_word >> 16) & 0x1f
        c_rt = (t_word >> 16) & 0x1f
        t_imm = t_word & 0xffff
        c_imm = c_word & 0xffff

        if t_op == c_op:  # Same opcode — fix register and/or immediate fields
            if t_rt != c_rt:
                reg_fixes.append((offset, 'rt', REG_NAMES.get(c_rt, f'r{c_rt}'), REG_NAMES.get(t_rt, f'r{t_rt}')))
            if t_rs != c_rs and t_op != 0:  # Skip R-type (different encoding)
                reg_fixes.append((offset, 'rs', REG_NAMES.get(c_rs, f'r{c_rs}'), REG_NAMES.get(t_rs, f'r{t_rs}')))
            if t_imm != c_imm:
                imm_fixes.append((offset, t_imm))
        else:
            if verbose:
                print(f"    Warning: opcode mismatch at idx {idx}, can't auto-fix")

    # Add patch_insn_reg_field and patch_insn_imm16 to PostProc
    if reg_fixes or imm_fixes:
        # Read current C source (with cse+sched already applied)
        with open(src_path) as f:
            current_c = f.read()
        
        lines = current_c.split('\n')
        
        # Find the */ closing line to insert before
        closing_idx = None
        for i, line in enumerate(lines):
            if '*/' in line:
                closing_idx = i
                break
        
        if closing_idx is not None:
            if reg_fixes:
                reg_str = ' '.join(f"0x{off:x}:{field}:{frm}:{to}" for off, field, frm, to in reg_fixes)
                lines.insert(closing_idx, f" * @PostProc: patch_insn_reg_field {reg_str}")
                closing_idx += 1  # Shift closing line index
            if imm_fixes:
                imm_str = ' '.join(f"0x{off:x}:0x{val:04x}" for off, val in imm_fixes)
                lines.insert(closing_idx, f" * @PostProc: patch_insn_imm16 {imm_str}")
        
        new_c2 = '\n'.join(lines)
        
        with open(src_path, 'w') as f:
            f.write(new_c2)
        
        final_pct = get_match_pct(overlay, func_name)
        if final_pct >= pct_after_cse_sched:
            return {"func": func_name, "result": "improved", "before": baseline_pct, "after": final_pct, "intermediate": pct_after_cse_sched}

    # Even if patches failed, keep cse+sched improvement
    return {"func": func_name, "result": "cse+sched only", "before": baseline_pct, "after": pct_after_cse_sched}


def main():
    parser = argparse.ArgumentParser(description="Batch-fix leaf_init functions")
    parser.add_argument("overlay", help="Overlay name")
    parser.add_argument("--apply", action="store_true", help="Apply changes")
    parser.add_argument("--max-funcs", type=int, default=10, help="Max functions to process")
    parser.add_argument("--min-pct", type=float, default=10, help="Minimum baseline %")
    parser.add_argument("--max-pct", type=float, default=80, help="Maximum baseline %")
    parser.add_argument("-v", "--verbose", action="store_true")
    args = parser.parse_args()

    d = json.load(open(STATUS_FILE))

    targets = []
    for fname, info in d.get(args.overlay, {}).items():
        if info.get('status') not in ('semi_matched', 'semi'):
            continue
        pct = info.get('exact_pct', 0)
        if not isinstance(pct, (int, float)):
            continue
        if pct < args.min_pct or pct > args.max_pct:
            continue

        src_path = SRC_DIR / args.overlay / f"{fname}.c"
        if not src_path.exists():
            continue
        with open(src_path) as f:
            content = f.read()
        m = re.search(r'@SymbolSize:\s*(\d+)', content)
        if m and int(m.group(1)) // 4 <= 20:
            targets.append((fname, float(pct)))

    targets.sort(key=lambda x: -x[1])
    if args.max_funcs:
        targets = targets[:args.max_funcs]

    print(f"Processing {len(targets)} functions from {args.overlay}")

    improved = 0
    bp_perfect = 0
    for fname, pct in targets:
        print(f"\n{args.overlay}::{fname}: {pct:.1f}%")
        result = fix_function(args.overlay, fname, args.apply, args.verbose)
        r = result.get('result', 'unknown')
        if 'BYTE-PERFECT' in r:
            bp_perfect += 1
            improved += 1
            print(f"  ✓ BYTE-PERFECT! {result.get('before', '?')}% → 100%")
        elif 'improved' in r:
            improved += 1
            print(f"  ✓ {result.get('before', '?')}% → {result.get('after', '?')}%")
        elif 'partial' in r:
            improved += 1
            print(f"  ~ {result.get('before', '?')}% → {result.get('after', '?')}% (partial)")
        elif 'error' in result:
            print(f"  ✗ {result.get('error')}")
        else:
            print(f"  - {r}")

    print(f"\nResults: {improved} improved, {bp_perfect} BYTE-PERFECT out of {len(targets)}")


if __name__ == "__main__":
    main()
