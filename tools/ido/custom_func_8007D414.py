#!/usr/bin/env python3
"""
Custom post-processing for wipe_tile_disp.
Pipeline:
1. swap_registers s2<->s3 (bidirectional)
2. remap_stack_slots 52<->56 (fix s-reg save offsets after swap)
3. swap_instructions at epilogue (lw order)
4. patch_insn_reg_field for 4 addu operand orders
5. scheduler_normalizer for prologue scheduling
"""
import struct
import subprocess
import sys
import os

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o", file=sys.stderr)
        sys.exit(1)

    inp = sys.argv[1]
    out = sys.argv[2]
    py = sys.executable
    tools = os.path.join(ROOT_DIR, "tools")

    # Step 1: swap_registers s2<->s3
    tmp1 = out + ".tmp1"
    subprocess.check_call([
        py, os.path.join(tools, "ido", "swap_registers.py"),
        inp, tmp1, "--from", "s2", "--to", "s3", "--bidirectional"
    ])

    # Step 2: remap_stack_slots 52<->56
    tmp2 = out + ".tmp2"
    subprocess.check_call([
        py, os.path.join(tools, "ido", "remap_stack_slots.py"),
        tmp1, tmp2, "--map", "52:56,56:52"
    ])

    # Step 3: swap epilogue lw instructions at offsets 0x398 and 0x39c
    tmp3 = out + ".tmp3"
    subprocess.check_call([
        py, os.path.join(tools, "ido", "swap_instructions.py"),
        tmp2, tmp3, "--offset1", "0x398", "--offset2", "0x39c"
    ])

    # Step 4: patch addu operand order (swap rs and rt at 4 offsets)
    tmp4 = out + ".tmp4"
    subprocess.check_call([
        py, os.path.join(tools, "ido", "patch_insn_reg_field.py"),
        tmp3, tmp4,
        "--patch", "0x98:rs:t9:s1", "--patch", "0x98:rt:s1:t9",
        "--patch", "0x134:rs:t1:s1", "--patch", "0x134:rt:s1:t1",
        "--patch", "0x240:rs:t9:s1", "--patch", "0x240:rt:s1:t9",
        "--patch", "0x2DC:rs:t2:s1", "--patch", "0x2DC:rt:s1:t2",
    ])

    # Step 5: scheduler_normalizer for prologue scheduling
    subprocess.check_call([
        py, os.path.join(tools, "postproc", "scheduler_normalizer.py"),
        tmp4, "--target", os.path.join(ROOT_DIR, "disks", "SLPS_000.65"),
        "--address", "0x8007D414", "-o", out
    ])

    # Cleanup temp files
    for tmp in [tmp1, tmp2, tmp3, tmp4]:
        try:
            os.unlink(tmp)
        except OSError:
            pass


if __name__ == "__main__":
    main()
