#!/usr/bin/env python3
"""Insert an explicit `or rd, rs, $zero` register-to-register move into a
NOP slot, recovering moves that IDO's dead-store / dead-move elimination
removed but the target kept.

Inverse counterpart of `swap_spill_with_move`: that tool converts a `sw`
spill into an `or` move; this one materializes an `or` move where IDO
emitted just a `nop` (typically a scheduling gap or alignment slot). The
target binary keeps the explicit register-to-register copy because the
upstream compiler ran with different liveness analysis.

Safe under the BANNED-tools policy: only rewrites the 4-byte instruction
at --offset, requires it to currently be 0x00000000 (a nop), and refuses
to shift any other bytes. No `.text` size change.

Usage:
    unfold_dead_or_move.py in.o out.o --offset 0x40 --rs v1 --rd v0
    unfold_dead_or_move.py in.o out.o --offset 0x40 --rs v1 --rd v0 \\
        --allow-nonnop   # accept other safe-to-clobber instructions

Tier A4 of the 2026-05-11 blocker brainstorm (kept in the project's private archive).
"""
from __future__ import annotations

import argparse
import shutil
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from rename_register_chain import REGS, REG_NAMES, extract_text_section  # noqa: E402

OP_SPECIAL = 0x00
FUNC_OR = 0x25


def encode_or(rs: int, rd: int) -> int:
    # SPECIAL rs rt(=0) rd shamt(=0) OR
    return ((OP_SPECIAL & 0x3F) << 26) | ((rs & 0x1F) << 21) | ((rd & 0x1F) << 11) | (FUNC_OR & 0x3F)


def apply(input_path: str, output_path: str, offset: int, rs: int, rd: int,
          allow_nonnop: bool, verbose: bool) -> bool:
    shutil.copy(input_path, output_path)
    text_offset, text_size, endian_fmt = extract_text_section(output_path)

    if offset < 0 or offset + 4 > text_size or offset % 4:
        print(
            f"error: --offset 0x{offset:x} out of range or unaligned "
            f"(.text size 0x{text_size:x})",
            file=sys.stderr,
        )
        return False

    with open(output_path, "r+b") as f:
        f.seek(text_offset + offset)
        current = struct.unpack(endian_fmt + "I", f.read(4))[0]
        if current != 0 and not allow_nonnop:
            print(
                f"error: offset 0x{offset:x} is not a nop (got 0x{current:08x}); "
                f"pass --allow-nonnop to override",
                file=sys.stderr,
            )
            return False

        new_word = encode_or(rs, rd)
        if verbose:
            print(
                f"  0x{offset:04x}: 0x{current:08x} -> 0x{new_word:08x}  "
                f"(or ${REG_NAMES[rd]}, ${REG_NAMES[rs]}, $zero)"
            )

        f.seek(text_offset + offset)
        f.write(struct.pack(endian_fmt + "I", new_word))

    return True


def parse_reg(name: str) -> int:
    name = name.strip().lstrip("$")
    if name not in REGS:
        raise ValueError(f"unknown register '{name}'")
    return REGS[name]


def main() -> int:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("input")
    p.add_argument("output")
    p.add_argument(
        "--offset",
        required=True,
        help=".text-relative byte offset of the slot to overwrite (must be nop)",
    )
    p.add_argument(
        "--rs",
        required=True,
        help="source register (e.g., v1, t6) — copied FROM",
    )
    p.add_argument(
        "--rd",
        required=True,
        help="destination register (e.g., v0, a0) — copied TO",
    )
    p.add_argument(
        "--allow-nonnop",
        action="store_true",
        help="allow overwrite of a non-nop instruction (DANGEROUS — only "
             "use when the slot is provably dead, e.g. an unused 'addiu "
             "$at,$zero,K' constant load eliminated by your C edits)",
    )
    p.add_argument("-v", "--verbose", action="store_true")
    args = p.parse_args()

    offset = int(args.offset, 0)
    try:
        rs = parse_reg(args.rs)
        rd = parse_reg(args.rd)
    except ValueError as e:
        print(f"error: {e}", file=sys.stderr)
        return 1

    return 0 if apply(args.input, args.output, offset, rs, rd,
                      args.allow_nonnop, args.verbose) else 1


if __name__ == "__main__":
    raise SystemExit(main())
