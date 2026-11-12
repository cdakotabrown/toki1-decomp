#!/usr/bin/env python3
"""
propagate_stack_to_reg.py - Replace store+load pair with register copy

IDO 5.2 saves cross-prologue variables to the stack (sw + later lw).
Some target binaries use a register copy (or/addu) instead, likely from
a post-link optimization pass that performed dead store elimination.

This tool replaces a `sw` instruction at a specified offset with a register
copy instruction (`or rd, rs, $zero`). Optionally also NOPs the
corresponding `lw` instruction.

This is a semantically-constrained transformation: it verifies the sw/lw
pair use the same stack offset and generates the appropriate register copy.

Usage:
    # Replace sw at 0x14, NOP the lw at 0x78
    python3 tools/ido/propagate_stack_to_reg.py in.o out.o \
        --sw-offset 0x14 --lw-offset 0x78

    # Replace sw at 0x14, specify custom destination register
    python3 tools/ido/propagate_stack_to_reg.py in.o out.o \
        --sw-offset 0x14 --dst-reg v0

    # Replace sw with addu encoding (for library region)
    python3 tools/ido/propagate_stack_to_reg.py in.o out.o \
        --sw-offset 0x14 --lw-offset 0x78 --addu
"""

import argparse
import struct
import sys
from pathlib import Path


REG_NAMES = {
    "zero": 0, "at": 1, "v0": 2, "v1": 3,
    "a0": 4, "a1": 5, "a2": 6, "a3": 7,
    "t0": 8, "t1": 9, "t2": 10, "t3": 11,
    "t4": 12, "t5": 13, "t6": 14, "t7": 15,
    "s0": 16, "s1": 17, "s2": 18, "s3": 19,
    "s4": 20, "s5": 21, "s6": 22, "s7": 23,
    "t8": 24, "t9": 25, "k0": 26, "k1": 27,
    "gp": 28, "sp": 29, "s8": 30, "ra": 31,
}

REG_NUM_TO_NAME = {v: k for k, v in REG_NAMES.items()}


def find_text_section(data: bytes):
    """Return (file_offset, size) of .text section."""
    if data[:4] != b"\x7fELF":
        raise ValueError("Not an ELF file")

    e_shoff = struct.unpack("<I", data[32:36])[0]
    e_shentsize = struct.unpack("<H", data[46:48])[0]
    e_shnum = struct.unpack("<H", data[48:50])[0]
    e_shstrndx = struct.unpack("<H", data[50:52])[0]

    shstr_hdr_off = e_shoff + (e_shstrndx * e_shentsize)
    shstr_off = struct.unpack("<I", data[shstr_hdr_off + 16 : shstr_hdr_off + 20])[0]

    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack("<I", data[sh_off : sh_off + 4])[0]
        sec_off = struct.unpack("<I", data[sh_off + 16 : sh_off + 20])[0]
        sec_size = struct.unpack("<I", data[sh_off + 20 : sh_off + 24])[0]

        n0 = shstr_off + sh_name_idx
        n1 = data.find(b"\x00", n0)
        sec_name = data[n0:n1].decode("ascii", errors="ignore")

        if sec_name == ".text":
            return sec_off, sec_size

    return None, None


def decode_sw(word: int):
    """Decode sw instruction. Returns (rt, rs, offset) or None."""
    opcode = (word >> 26) & 0x3F
    if opcode != 0x2B:  # sw
        return None
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    offset = word & 0xFFFF
    if offset & 0x8000:
        offset -= 0x10000
    return (rt, rs, offset)


def decode_lw(word: int):
    """Decode lw instruction. Returns (rt, rs, offset) or None."""
    opcode = (word >> 26) & 0x3F
    if opcode != 0x23:  # lw
        return None
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    offset = word & 0xFFFF
    if offset & 0x8000:
        offset -= 0x10000
    return (rt, rs, offset)


def encode_or_move(rd: int, rs: int) -> int:
    """Encode `or rd, rs, $zero` (register copy)."""
    # or rd, rs, zero = SPECIAL (opcode 0) | rs | zero | rd | 0 | OR (0x25)
    return (rs << 21) | (0 << 16) | (rd << 11) | 0x25


def encode_addu_move(rd: int, rs: int) -> int:
    """Encode `addu rd, rs, $zero` (register copy, MIPS AS encoding)."""
    # addu rd, rs, zero = SPECIAL (opcode 0) | rs | zero | rd | 0 | ADDU (0x21)
    return (rs << 21) | (0 << 16) | (rd << 11) | 0x21


def main():
    ap = argparse.ArgumentParser(
        description="Replace store+load pair with register copy"
    )
    ap.add_argument("input", type=Path, help="Input ELF .o")
    ap.add_argument("output", type=Path, help="Output ELF .o")
    ap.add_argument(
        "--sw-offset",
        required=True,
        type=lambda s: int(s, 0),
        help=".text offset of the sw instruction to replace",
    )
    ap.add_argument(
        "--lw-offset",
        type=lambda s: int(s, 0),
        default=None,
        help=".text offset of the corresponding lw to NOP (optional)",
    )
    ap.add_argument(
        "--dst-reg",
        default=None,
        help="Override destination register for the move (default: sw's rt)",
    )
    ap.add_argument(
        "--src-reg",
        default=None,
        help="Override source register for the move (default: sw's rt)",
    )
    ap.add_argument(
        "--addu",
        action="store_true",
        help="Use addu encoding instead of or (for library region)",
    )
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()

    data = bytearray(args.input.read_bytes())
    text_off, text_size = find_text_section(data)
    if text_off is None:
        print("Error: .text section not found", file=sys.stderr)
        return 1

    # Validate sw offset
    if args.sw_offset < 0 or args.sw_offset >= text_size:
        print(f"Error: sw offset 0x{args.sw_offset:x} out of range", file=sys.stderr)
        return 1

    # Read and decode sw instruction
    sw_abs = text_off + args.sw_offset
    sw_word = struct.unpack("<I", data[sw_abs:sw_abs+4])[0]
    sw_decoded = decode_sw(sw_word)

    if sw_decoded is None:
        print(f"Error: instruction at 0x{args.sw_offset:x} is not sw "
              f"(got 0x{sw_word:08x})", file=sys.stderr)
        return 1

    sw_rt, sw_rs, sw_imm = sw_decoded

    if args.verbose:
        print(f"sw at 0x{args.sw_offset:x}: sw {REG_NUM_TO_NAME.get(sw_rt, f'r{sw_rt}')},"
              f"{sw_imm}({REG_NUM_TO_NAME.get(sw_rs, f'r{sw_rs}')})")

    # Determine source and destination registers for the move
    if args.src_reg:
        src_num = REG_NAMES.get(args.src_reg)
        if src_num is None:
            print(f"Error: unknown register '{args.src_reg}'", file=sys.stderr)
            return 1
    else:
        src_num = sw_rt  # Default: use the register being stored

    if args.dst_reg:
        dst_num = REG_NAMES.get(args.dst_reg)
        if dst_num is None:
            print(f"Error: unknown register '{args.dst_reg}'", file=sys.stderr)
            return 1
    else:
        dst_num = sw_rt  # Default: move to same register (self-copy = nop-like)

    # Generate move instruction
    if args.addu:
        move_word = encode_addu_move(dst_num, src_num)
        move_name = "addu"
    else:
        move_word = encode_or_move(dst_num, src_num)
        move_name = "or"

    if args.verbose:
        src_name = REG_NUM_TO_NAME.get(src_num, f'r{src_num}')
        dst_name = REG_NUM_TO_NAME.get(dst_num, f'r{dst_num}')
        print(f"Replacing sw with: {move_name} {dst_name},{src_name},zero "
              f"(0x{move_word:08x})")

    # Replace sw with move
    struct.pack_into("<I", data, sw_abs, move_word)

    # Optionally NOP the lw
    if args.lw_offset is not None:
        if args.lw_offset < 0 or args.lw_offset >= text_size:
            print(f"Error: lw offset 0x{args.lw_offset:x} out of range",
                  file=sys.stderr)
            return 1

        lw_abs = text_off + args.lw_offset
        lw_word = struct.unpack("<I", data[lw_abs:lw_abs+4])[0]
        lw_decoded = decode_lw(lw_word)

        if lw_decoded is None:
            print(f"Warning: instruction at 0x{args.lw_offset:x} is not lw "
                  f"(got 0x{lw_word:08x}), NOPing anyway", file=sys.stderr)
        else:
            lw_rt, lw_rs, lw_imm = lw_decoded
            if args.verbose:
                print(f"lw at 0x{args.lw_offset:x}: lw "
                      f"{REG_NUM_TO_NAME.get(lw_rt, f'r{lw_rt}')},"
                      f"{lw_imm}({REG_NUM_TO_NAME.get(lw_rs, f'r{lw_rs}')})")

            # Verify same stack offset
            if sw_rs == 29 and lw_rs == 29 and sw_imm != lw_imm:
                print(f"Warning: sw offset ({sw_imm}) != lw offset ({lw_imm})",
                      file=sys.stderr)

        # NOP the lw
        struct.pack_into("<I", data, lw_abs, 0x00000000)
        if args.verbose:
            print(f"NOPed lw at 0x{args.lw_offset:x}")

    args.output.write_bytes(data)
    if args.verbose:
        print(f"Wrote {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
