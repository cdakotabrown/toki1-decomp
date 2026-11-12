#!/usr/bin/env python3
"""Custom post-processing for kuchi_level_set.

Pipeline:
1. patch_stack_complete --old 24 --new 40 (applied before this script)
2. Per-block register remapping via remap_all_regs_per_block
3. Strip trailing nop (108 -> 107 instructions)
"""
import subprocess
import sys
import os
import shutil


def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} input.o output.o", file=sys.stderr)
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    tools_dir = os.path.dirname(os.path.abspath(__file__))
    remap_script = os.path.join(tools_dir, 'remap_all_regs_per_block.py')
    strip_script = os.path.join(tools_dir, 'strip_trailing_instructions.py')

    # Find target exe
    repo_root = os.path.dirname(os.path.dirname(tools_dir))
    target_exe = os.path.join(repo_root, 'disks', 'SLPS_000.65')

    # Step 1: Apply per-block register remapping
    tmp_path = input_path + '.remap_tmp'
    result = subprocess.run(
        [sys.executable, remap_script,
         '--target-exe', target_exe,
         '--address', '0x80067D58',
         input_path, tmp_path],
        capture_output=True, text=True
    )

    if result.returncode != 0:
        print(f"remap_all_regs_per_block failed: {result.stderr}", file=sys.stderr)
        shutil.copy(input_path, output_path)
        sys.exit(1)

    # Step 2: Strip trailing nop
    result = subprocess.run(
        [sys.executable, strip_script,
         '--count', '1', '--nop-only',
         tmp_path, output_path],
        capture_output=True, text=True
    )

    # Clean up temp file
    try:
        os.unlink(tmp_path)
    except OSError:
        pass

    if result.returncode != 0:
        print(f"strip_trailing_instructions failed: {result.stderr}", file=sys.stderr)
        shutil.copy(tmp_path, output_path)
        sys.exit(1)

    print(f"Custom post-processing applied for kuchi_level_set")


if __name__ == '__main__':
    main()
