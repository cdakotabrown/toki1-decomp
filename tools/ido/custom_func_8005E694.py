#!/usr/bin/env python3
"""Custom post-processing for system_config_icon_mode_init.

Pipeline:
1. patch_stack_complete --old 104 --new 112 (expand frame by 8 bytes)
2. swap_registers v1<->a1 in loop (offsets 0x24,0x2c,0x4c,0x54,0x5c,0x60)
3. scheduler_normalizer (fix loop instruction ordering)
"""

import subprocess
import sys
import os
import tempfile

TOOLS_DIR = sys.path[0] if sys.path[0] else "tools/ido"
ROOT = os.path.dirname(os.path.dirname(TOOLS_DIR))


def run(cmd):
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"FAILED: {' '.join(cmd)}", file=sys.stderr)
        print(result.stderr, file=sys.stderr)
        sys.exit(1)


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o")
        sys.exit(1)

    inp = sys.argv[1]
    out = sys.argv[2]

    tools_ido = os.path.join(ROOT, "tools", "ido")
    tools_postproc = os.path.join(ROOT, "tools", "postproc")
    target_exe = os.path.join(ROOT, "disks", "SLPS_000.65")

    tmp1 = tempfile.mktemp(suffix=".o")
    tmp2 = tempfile.mktemp(suffix=".o")

    try:
        # Step 1: patch frame 104 -> 112
        run(["python3", os.path.join(tools_ido, "patch_stack_complete.py"),
             inp, tmp1, "--old", "104", "--new", "112"])

        # Step 2: swap v1<->a1 in loop only
        run(["python3", os.path.join(tools_ido, "swap_registers.py"),
             tmp1, tmp2, "--from", "v1", "--to", "a1", "--bidirectional",
             "--only", "0x24,0x2c,0x4c,0x54,0x5c,0x60"])

        # Step 3: scheduler normalizer
        run(["python3", os.path.join(tools_postproc, "scheduler_normalizer.py"),
             tmp2, "--target", target_exe, "--address", "0x8005E694",
             "-o", out])

    finally:
        for f in [tmp1, tmp2]:
            if os.path.exists(f):
                os.unlink(f)


if __name__ == "__main__":
    main()
