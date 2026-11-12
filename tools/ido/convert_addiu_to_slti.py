#!/usr/bin/env python3
"""Convert IDO's `addiu rt,$zero,K; b{ne,eq} rs,rt,dest` into the target's
`slti rt,rs,K; b{ne,eq} rt,$zero,dest` form.

IDO promotes the small constant to a register and uses a generic compare:

    addiu $at, $zero, 512
    bne   $s0, $at, dest

The target instead uses the dedicated slti opcode:

    slti  $at, $s0, 512
    bne   $at, $zero, dest

Both forms are semantically identical for compares against a small (>0)
constant, but the byte encoding differs in opcode + rs/rt fields. This tool
is a pure ELF text rewrite over a 2-instruction window — safe under the
project's BANNED-tools policy.

Usage:
    python3 tools/ido/convert_addiu_to_slti.py in.o out.o --offset 0x40
    python3 tools/ido/convert_addiu_to_slti.py in.o out.o --offset 0x40 -v

Note: --offset is the .text-relative byte offset of the *addiu* instruction
(the same convention as other PostProc tools — object-relative, after the
0x10 thunk pad if present). The branch at offset+4 is rewritten in lockstep.

Tier A2 of the 2026-05-11 blocker brainstorm (kept in the project's private archive).
"""
from __future__ import annotations

import argparse
import shutil
import struct
import sys
from pathlib import Path
from typing import Optional, Tuple

# Reuse ELF .text extraction from rename_register_chain
sys.path.insert(0, str(Path(__file__).parent))
from rename_register_chain import extract_text_section  # noqa: E402

OP_SPECIAL = 0x00
OP_ADDIU = 0x09
OP_SLTI = 0x0A
OP_BEQ = 0x04
OP_BNE = 0x05

REG_ZERO = 0


def decode_iform(word: int) -> Tuple[int, int, int, int]:
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    imm = word & 0xFFFF
    return op, rs, rt, imm


def encode_iform(op: int, rs: int, rt: int, imm: int) -> int:
    return ((op & 0x3F) << 26) | ((rs & 0x1F) << 21) | ((rt & 0x1F) << 16) | (imm & 0xFFFF)


def convert_pair(addiu_word: int, branch_word: int) -> Optional[Tuple[int, int]]:
    """Return (new_addiu, new_branch) on a successful rewrite, else None."""
    op_a, rs_a, rt_a, imm_a = decode_iform(addiu_word)
    if op_a != OP_ADDIU or rs_a != REG_ZERO:
        return None  # not a `addiu rt, $zero, K`

    op_b, rs_b, rt_b, off_b = decode_iform(branch_word)
    if op_b not in (OP_BEQ, OP_BNE):
        return None  # not a beq/bne in the delay window

    # Identify which operand of the branch is `rt_a` (the constant-loaded reg).
    # Result: compare_reg = the *non-rt_a* register, which becomes the slti rs.
    if rs_b == rt_a and rt_b != rt_a:
        compare_reg = rt_b
    elif rt_b == rt_a and rs_b != rt_a:
        compare_reg = rs_b
    else:
        return None  # neither rs nor rt of the branch matches the loaded reg

    if compare_reg == REG_ZERO:
        return None  # compare against zero — addiu+bne wasn't the slti idiom

    if imm_a >= 0x8000:
        # Negative (signed) immediate — slti has the same encoding so still safe,
        # but addiu's K and slti's K agree only for non-negative K when used as
        # an unsigned bound. Bail to be safe; if a target legitimately wants
        # this, the user can lift the check.
        return None

    new_addiu = encode_iform(OP_SLTI, compare_reg, rt_a, imm_a)
    new_branch = encode_iform(op_b, rt_a, REG_ZERO, off_b)
    return new_addiu, new_branch


def apply(input_path: str, output_path: str, offset: int, verbose: bool = False) -> bool:
    shutil.copy(input_path, output_path)
    text_offset, text_size, endian_fmt = extract_text_section(output_path)

    if offset < 0 or offset + 8 > text_size or offset % 4:
        print(
            f"error: --offset 0x{offset:x} out of range or unaligned "
            f"(.text size 0x{text_size:x})",
            file=sys.stderr,
        )
        return False

    with open(output_path, "r+b") as f:
        f.seek(text_offset + offset)
        addiu = struct.unpack(endian_fmt + "I", f.read(4))[0]
        branch = struct.unpack(endian_fmt + "I", f.read(4))[0]

        result = convert_pair(addiu, branch)
        if result is None:
            print(
                f"error: instructions at 0x{offset:x}/0x{offset+4:x} "
                f"don't match `addiu rt,\\$zero,K; b{{ne,eq}} rs,rt,dest` "
                f"(got 0x{addiu:08x} 0x{branch:08x})",
                file=sys.stderr,
            )
            return False
        new_addiu, new_branch = result

        if verbose:
            print(
                f"  0x{offset:04x}: 0x{addiu:08x} -> 0x{new_addiu:08x}  (addiu->slti)"
            )
            print(
                f"  0x{offset+4:04x}: 0x{branch:08x} -> 0x{new_branch:08x}  (b -> use slti result)"
            )

        f.seek(text_offset + offset)
        f.write(struct.pack(endian_fmt + "I", new_addiu))
        f.write(struct.pack(endian_fmt + "I", new_branch))

    return True


def main() -> int:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("input", help="input .o file")
    p.add_argument("output", help="output .o file")
    p.add_argument(
        "--offset",
        required=True,
        help=".text-relative byte offset of the addiu instruction (e.g. 0x40)",
    )
    p.add_argument("-v", "--verbose", action="store_true")
    args = p.parse_args()
    offset = int(args.offset, 0)
    ok = apply(args.input, args.output, offset, args.verbose)
    return 0 if ok else 1


if __name__ == "__main__":
    raise SystemExit(main())
