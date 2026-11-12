#!/usr/bin/env python3
"""
fix_trailing_nop.py - Add @PostProc annotations to +1 diff functions

Finds all overlay functions with +1 instruction count diff and adds
@PostProc: strip_trailing_nop annotation to fix them.
"""

import os
import re
import sys
import subprocess
from pathlib import Path

ROOT = Path(__file__).parent.parent.parent
SRC_DIR = ROOT / "src" / "overlay"
ASM_DIR = ROOT / "asm" / "overlay"
IDO_CC = ROOT / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
INCLUDE_DIR = ROOT / "include"

sys.path.insert(0, str(ROOT / "tools" / "build"))
from ido_flags import base_flags, pic_flags

IDO_FLAGS = base_flags([INCLUDE_DIR])


def get_target_insn_count(asm_file: Path) -> int:
    """Extract target instruction count from ASM file header."""
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
    """Compile and count instructions."""
    obj_file = Path("/tmp/fix_trailing_test.o")
    cmd = ([str(IDO_CC)] + IDO_FLAGS + pic_flags(c_file)
           + ["-c", str(c_file), "-o", str(obj_file)])
    try:
        result = subprocess.run(cmd, capture_output=True, timeout=30)
        if result.returncode != 0:
            return 0
        result = subprocess.run(
            ["mipsel-linux-gnu-objdump", "-d", str(obj_file)],
            capture_output=True, text=True
        )
        return len(re.findall(r'^\s+[0-9a-f]+:', result.stdout, re.MULTILINE))
    except Exception:
        return 0


def has_postproc(c_file: Path) -> bool:
    """Check if file already has @PostProc annotation."""
    try:
        with open(c_file, 'r') as f:
            return '@PostProc' in f.read(3000)
    except Exception:
        return False


def add_postproc_annotation(c_file: Path) -> bool:
    """Add @PostProc: strip_trailing_nop annotation to C file."""
    try:
        with open(c_file, 'r') as f:
            content = f.read()

        if '@PostProc: strip_trailing_nop' in content:
            return False

        # Add after first line (the header comment)
        lines = content.split('\n', 1)
        if len(lines) == 2:
            new_content = lines[0] + '\n/* @PostProc: strip_trailing_nop */\n' + lines[1]
            with open(c_file, 'w') as f:
                f.write(new_content)
            return True
    except Exception:
        pass
    return False


def main():
    dry_run = '--dry-run' in sys.argv
    verbose = '--verbose' in sys.argv or '-v' in sys.argv

    plus_one_funcs = []
    other_diffs = []
    total_checked = 0
    already_annotated = 0

    print("Scanning overlay functions for +1 diffs...")

    for overlay_dir in sorted(SRC_DIR.iterdir()):
        if not overlay_dir.is_dir():
            continue
        overlay_name = overlay_dir.name
        asm_overlay_dir = ASM_DIR / overlay_name

        for c_file in sorted(overlay_dir.glob("*.c")):
            func_name = c_file.stem
            asm_file = asm_overlay_dir / f"{func_name}.s"
            total_checked += 1

            if not asm_file.exists():
                continue

            # Skip if already has PostProc
            if has_postproc(c_file):
                already_annotated += 1
                continue

            target = get_target_insn_count(asm_file)
            if target == 0:
                continue

            compiled = get_compiled_insn_count(c_file)
            if compiled == 0:
                continue

            diff = compiled - target
            if diff == 1:
                plus_one_funcs.append((overlay_name, func_name, c_file))
                if verbose:
                    print(f"  +1: {overlay_name}/{func_name}")
            elif diff != 0:
                other_diffs.append((overlay_name, func_name, diff))

            if total_checked % 100 == 0:
                print(f"  Checked {total_checked} functions...")

    print(f"\nFound {len(plus_one_funcs)} functions with +1 diff")
    print(f"Already annotated: {already_annotated}")
    print(f"Other diffs: {len(other_diffs)}")

    if dry_run:
        print("\n[DRY RUN] Would add annotations to:")
        for overlay, func, _ in plus_one_funcs[:20]:
            print(f"  {overlay}/{func}")
        if len(plus_one_funcs) > 20:
            print(f"  ... and {len(plus_one_funcs) - 20} more")
        return

    # Add annotations
    added = 0
    for overlay, func, c_file in plus_one_funcs:
        if add_postproc_annotation(c_file):
            added += 1
            if verbose:
                print(f"  Added: {overlay}/{func}")

    print(f"\nAdded @PostProc annotations to {added} files")

    # Show other diffs summary
    if other_diffs and verbose:
        print(f"\nOther diffs (not +1):")
        by_diff = {}
        for overlay, func, diff in other_diffs:
            if diff not in by_diff:
                by_diff[diff] = []
            by_diff[diff].append(f"{overlay}/{func}")
        for diff in sorted(by_diff.keys()):
            sign = '+' if diff > 0 else ''
            print(f"  {sign}{diff}: {len(by_diff[diff])} functions")


if __name__ == "__main__":
    main()
