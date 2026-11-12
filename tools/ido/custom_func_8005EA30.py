#!/usr/bin/env python3
"""Custom post-processing for system_config_mouse_menu_init.

Pipeline:
1. patch_stack_complete --old 96 --new 112 (expand frame, no local offset changes)
2. remap_stack_slots (shift all array offsets +8)
3. swap_registers v1:a1 at specific loop offsets
4. scheduler_normalizer (fix instruction ordering)
"""

import subprocess
import sys
import shutil

TOOLS_DIR = sys.path[0] if sys.path[0] else "tools/ido"
import os
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

    import tempfile
    tmp1 = tempfile.mktemp(suffix=".o")
    tmp2 = tempfile.mktemp(suffix=".o")
    tmp3 = tempfile.mktemp(suffix=".o")

    try:
        # Step 1: patch frame 96 -> 112
        run(["python3", os.path.join(tools_ido, "patch_stack_complete.py"),
             inp, tmp1, "--old", "96", "--new", "112"])

        # Step 2: remap stack slots (+8 for all array offsets)
        run(["python3", os.path.join(tools_ido, "remap_stack_slots.py"),
             tmp1, tmp2, "--map",
             "40:48,46:54,48:56,50:58,52:60,54:62,56:64,58:66,64:72,70:78,72:80,80:88,82:90,84:92,88:96,90:98,92:100"])

        # Step 3: swap v1<->a1 in loop only
        run(["python3", os.path.join(tools_ido, "swap_registers.py"),
             tmp2, tmp3, "--from", "v1", "--to", "a1", "--bidirectional",
             "--only", "0x24,0x2c,0x4c,0x54,0x5c,0x60"])

        # Step 4: scheduler normalizer
        run(["python3", os.path.join(tools_postproc, "scheduler_normalizer.py"),
             tmp3, "--target", target_exe, "--address", "0x8005EA30",
             "-o", out])

    finally:
        for f in [tmp1, tmp2, tmp3]:
            if os.path.exists(f):
                os.unlink(f)

if __name__ == "__main__":
    main()
