#!/usr/bin/env python3
"""
analyze_mismatches.py - Analyze overlay instruction count mismatches

Categorizes mismatches by diff value to identify fixable patterns.
"""

import os
import re
import sys
import subprocess
from pathlib import Path
from collections import defaultdict

ROOT = Path(__file__).parent.parent.parent
SRC_DIR = ROOT / "src" / "overlay"
ASM_DIR = ROOT / "asm" / "overlay"
IDO_CC = ROOT / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
INCLUDE_DIR = ROOT / "include"

sys.path.insert(0, str(ROOT / "tools" / "build"))
from ido_flags import base_flags, pic_flags

IDO_FLAGS = base_flags([INCLUDE_DIR])


def get_target_insn_count(asm_file: Path) -> int:
    try:
        with open(asm_file, 'r') as f:
            content = f.read(500)
        match = re.search(r'Size: \d+ bytes \((\d+) instructions?\)', content)
        if match:
            return int(match.group(1))
    except Exception:
        pass
    return 0


def get_compiled_insn_count(c_file: Path) -> int:
    obj_file = Path("/tmp/analyze_mismatch.o")
    cmd = ([str(IDO_CC)] + IDO_FLAGS + pic_flags(c_file)
           + ["-c", str(c_file), "-o", str(obj_file)])
    try:
        result = subprocess.run(cmd, capture_output=True, timeout=30)
        if result.returncode != 0:
            return -1  # Compile error
        result = subprocess.run(
            ["mipsel-linux-gnu-objdump", "-d", str(obj_file)],
            capture_output=True, text=True
        )
        return len(re.findall(r'^\s+[0-9a-f]+:', result.stdout, re.MULTILINE))
    except Exception:
        return -1


def check_cross_prologue(asm_file: Path) -> bool:
    """Check if target ASM shows cross-prologue scheduling."""
    try:
        with open(asm_file, 'r') as f:
            lines = f.readlines()
        # Look for pattern: global load (lui) before stack setup (addiu sp)
        found_lui_before_sp = False
        found_sp_setup = False
        for line in lines[:30]:
            if 'addiu' in line and '$sp' in line and not found_sp_setup:
                found_sp_setup = True
            if 'lui' in line and not found_sp_setup:
                # lui before sp setup = cross-prologue
                found_lui_before_sp = True
        return found_lui_before_sp and found_sp_setup
    except Exception:
        return False


def check_hidden_param(c_file: Path) -> bool:
    """Check if C file mentions hidden parameters."""
    try:
        with open(c_file, 'r') as f:
            content = f.read(2000)
        return 'hidden' in content.lower() or 'Hidden' in content
    except Exception:
        return False


def main():
    by_diff = defaultdict(list)
    matched = 0
    total = 0
    compile_fail = 0
    cross_prologue_count = 0
    hidden_param_count = 0

    print("Analyzing overlay mismatches...")

    for overlay_dir in sorted(SRC_DIR.iterdir()):
        if not overlay_dir.is_dir():
            continue
        overlay_name = overlay_dir.name
        asm_overlay_dir = ASM_DIR / overlay_name

        for c_file in sorted(overlay_dir.glob("*.c")):
            func_name = c_file.stem
            asm_file = asm_overlay_dir / f"{func_name}.s"
            total += 1

            if not asm_file.exists():
                continue

            target = get_target_insn_count(asm_file)
            if target == 0:
                continue

            compiled = get_compiled_insn_count(c_file)
            if compiled < 0:
                compile_fail += 1
                continue

            diff = compiled - target
            if diff == 0:
                matched += 1
            else:
                has_cross = check_cross_prologue(asm_file)
                has_hidden = check_hidden_param(c_file)
                by_diff[diff].append({
                    'overlay': overlay_name,
                    'func': func_name,
                    'target': target,
                    'compiled': compiled,
                    'cross_prologue': has_cross,
                    'hidden_param': has_hidden
                })
                if has_cross:
                    cross_prologue_count += 1
                if has_hidden:
                    hidden_param_count += 1

            if total % 500 == 0:
                print(f"  Analyzed {total} functions...")

    print(f"\n{'='*60}")
    print(f"ANALYSIS SUMMARY")
    print(f"{'='*60}")
    print(f"Total functions:    {total}")
    print(f"Matched:            {matched} ({matched*100/total:.1f}%)")
    print(f"Mismatched:         {total - matched - compile_fail}")
    print(f"Compile failures:   {compile_fail}")
    print(f"Cross-prologue:     {cross_prologue_count}")
    print(f"Hidden params:      {hidden_param_count}")

    print(f"\n{'='*60}")
    print(f"MISMATCH DISTRIBUTION BY DIFF")
    print(f"{'='*60}")

    for diff in sorted(by_diff.keys()):
        funcs = by_diff[diff]
        sign = '+' if diff > 0 else ''
        cross = sum(1 for f in funcs if f['cross_prologue'])
        hidden = sum(1 for f in funcs if f['hidden_param'])
        print(f"  {sign}{diff:3d}: {len(funcs):4d} functions (cross-prologue: {cross}, hidden: {hidden})")

    # Show fixable categories
    print(f"\n{'='*60}")
    print(f"FIXABLE CATEGORIES")
    print(f"{'='*60}")

    # +1 remaining (should have been fixed by strip_trailing_nop)
    plus_one = by_diff.get(1, [])
    if plus_one:
        print(f"\n+1 (trailing nop): {len(plus_one)} functions")
        for f in plus_one[:5]:
            print(f"  - {f['overlay']}/{f['func']}")

    # +2 to +5 (might be cross-prologue or hidden params)
    small_positive = []
    for diff in range(2, 6):
        small_positive.extend(by_diff.get(diff, []))
    if small_positive:
        print(f"\n+2 to +5 (likely cross-prologue/hidden): {len(small_positive)} functions")
        cross_here = sum(1 for f in small_positive if f['cross_prologue'])
        hidden_here = sum(1 for f in small_positive if f['hidden_param'])
        print(f"  With cross-prologue: {cross_here}")
        print(f"  With hidden params:  {hidden_here}")

    # -1 to -5 (IDO optimizing more)
    small_negative = []
    for diff in range(-5, 0):
        small_negative.extend(by_diff.get(diff, []))
    if small_negative:
        print(f"\n-1 to -5 (IDO optimizing more): {len(small_negative)} functions")
        print(f"  These need code changes to de-optimize IDO output")

    # Large diffs
    large_positive = []
    large_negative = []
    for diff, funcs in by_diff.items():
        if diff > 5:
            large_positive.extend(funcs)
        elif diff < -5:
            large_negative.extend(funcs)
    if large_positive:
        print(f"\n+6 or more (structural): {len(large_positive)} functions")
    if large_negative:
        print(f"\n-6 or less (major optimization): {len(large_negative)} functions")


if __name__ == "__main__":
    main()
