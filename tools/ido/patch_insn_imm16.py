#!/usr/bin/env python3
"""
patch_insn_imm16.py - Patch a 16-bit immediate field in MIPS instructions.

This is a small, surgical post-processor for I-type instructions where the
opcode/registers are already correct but the immediate is not. It patches the
instruction word in-place without changing section sizes.

Usage:
  python3 tools/ido/patch_insn_imm16.py in.o out.o \
    --patch 0x754:124:112 --patch 0x7a4:124:112
"""

import argparse
import struct
import sys
from pathlib import Path


def find_section(data: bytes, name: str):
    if data[:4] != b"\x7fELF":
        raise ValueError("Not an ELF file")

    ei_data = data[5]
    if ei_data == 1:
        endian = "<"
    elif ei_data == 2:
        endian = ">"
    else:
        raise ValueError(f"Unknown ELF EI_DATA={ei_data}")

    e_shoff = struct.unpack(endian + "I", data[32:36])[0]
    e_shentsize = struct.unpack(endian + "H", data[46:48])[0]
    e_shnum = struct.unpack(endian + "H", data[48:50])[0]
    e_shstrndx = struct.unpack(endian + "H", data[50:52])[0]

    shstr_hdr_off = e_shoff + (e_shstrndx * e_shentsize)
    shstr_off = struct.unpack(endian + "I", data[shstr_hdr_off + 16 : shstr_hdr_off + 20])[0]

    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack(endian + "I", data[sh_off : sh_off + 4])[0]
        sec_off = struct.unpack(endian + "I", data[sh_off + 16 : sh_off + 20])[0]
        sec_size = struct.unpack(endian + "I", data[sh_off + 20 : sh_off + 24])[0]

        n0 = shstr_off + sh_name_idx
        n1 = data.find(b"\x00", n0)
        sec_name = data[n0:n1].decode("ascii", errors="ignore")

        if sec_name == name:
            return sec_off, sec_size, endian

    return None, None, endian


def sign_extend_imm16(word: int) -> int:
    imm = word & 0xFFFF
    if imm & 0x8000:
        imm -= 0x10000
    return imm


def set_imm16(word: int, imm: int) -> int:
    return (word & 0xFFFF0000) | (imm & 0xFFFF)


def main() -> int:
    ap = argparse.ArgumentParser(description="Patch 16-bit immediates in .text instructions")
    ap.add_argument("input", type=Path, help="Input ELF .o")
    ap.add_argument("output", type=Path, help="Output ELF .o")
    ap.add_argument(
        "--patch",
        action="append",
        default=[],
        help="Patch spec OFFSET:FROM:TO (e.g. 0x754:124:112). Can be repeated.",
    )
    ap.add_argument(
        "--patch-noverify",
        action="append",
        default=[],
        help="Patch spec OFFSET:TO without verifying current imm. Can be repeated.",
    )
    ap.add_argument("-v", "--verbose", action="store_true", help="Verbose output")
    args = ap.parse_args()

    if not args.patch and not args.patch_noverify:
        print("Error: at least one --patch or --patch-noverify is required", file=sys.stderr)
        return 1

    data = bytearray(args.input.read_bytes())
    text_off, text_size, endian = find_section(data, ".text")
    if text_off is None:
        print("Error: .text section not found", file=sys.stderr)
        return 1

    patched = 0
    for spec in args.patch:
        parts = spec.split(":")
        if len(parts) != 3:
            print(f"Error: bad --patch '{spec}' (expected OFFSET:FROM:TO)", file=sys.stderr)
            return 1

        off = int(parts[0], 0)
        expected = int(parts[1], 0)
        replacement = int(parts[2], 0)

        if off % 4 != 0:
            print(f"Error: offset not 4-byte aligned: 0x{off:x}", file=sys.stderr)
            return 1
        if off < 0 or off + 4 > text_size:
            print(f"Error: offset out of range: 0x{off:x} (text size 0x{text_size:x})", file=sys.stderr)
            return 1

        insn_off = text_off + off
        word = struct.unpack(endian + "I", data[insn_off : insn_off + 4])[0]
        cur = sign_extend_imm16(word)
        if cur != expected:
            print(
                f"Error: .text+0x{off:x} imm16 is {cur}, expected {expected}",
                file=sys.stderr,
            )
            return 1

        new_word = set_imm16(word, replacement)
        data[insn_off : insn_off + 4] = struct.pack(endian + "I", new_word)
        patched += 1

        if args.verbose:
            print(f"Patched .text+0x{off:x} imm16: {expected} -> {replacement}")

    for spec in args.patch_noverify:
        parts = spec.split(":")
        if len(parts) != 2:
            print(f"Error: bad --patch-noverify '{spec}' (expected OFFSET:TO)", file=sys.stderr)
            return 1

        off = int(parts[0], 0)
        replacement = int(parts[1], 0)

        if off % 4 != 0:
            print(f"Error: offset not 4-byte aligned: 0x{off:x}", file=sys.stderr)
            return 1
        if off < 0 or off + 4 > text_size:
            print(f"Error: offset out of range: 0x{off:x} (text size 0x{text_size:x})", file=sys.stderr)
            return 1

        insn_off = text_off + off
        word = struct.unpack(endian + "I", data[insn_off : insn_off + 4])[0]
        cur = sign_extend_imm16(word)
        new_word = set_imm16(word, replacement)
        data[insn_off : insn_off + 4] = struct.pack(endian + "I", new_word)
        patched += 1

        if args.verbose:
            print(f"Patched .text+0x{off:x} imm16 (noverify): {cur} -> {replacement}")

    args.output.write_bytes(data)
    if args.verbose or patched:
        print(f"Patched {patched} instruction immediate(s)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
