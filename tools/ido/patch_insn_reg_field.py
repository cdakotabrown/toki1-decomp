#!/usr/bin/env python3
"""
patch_insn_reg_field.py - Patch a register field in a MIPS instruction at given .text offsets

This is a small, surgical post-processor for cases where the compiled C is
structurally identical to the target, but one or two instructions use the wrong
register due to a non-standard calling convention (e.g. a hidden v1 parameter).

It patches the instruction word in-place, without changing section sizes.

Supported fields:
  - rs: bits 21..25
  - rt: bits 16..20
  - rd: bits 11..15 (R-type only; still patched as bits)

Usage:
  python3 tools/ido/patch_insn_reg_field.py in.o out.o \\
    --patch 0x4:rs:a0:v1 --patch 0x10:rs:a0:v1
"""

import argparse
import struct
import sys
from pathlib import Path


REGS = {
    "zero": 0,
    "at": 1,
    "v0": 2,
    "v1": 3,
    "a0": 4,
    "a1": 5,
    "a2": 6,
    "a3": 7,
    "t0": 8,
    "t1": 9,
    "t2": 10,
    "t3": 11,
    "t4": 12,
    "t5": 13,
    "t6": 14,
    "t7": 15,
    "s0": 16,
    "s1": 17,
    "s2": 18,
    "s3": 19,
    "s4": 20,
    "s5": 21,
    "s6": 22,
    "s7": 23,
    "t8": 24,
    "t9": 25,
    "k0": 26,
    "k1": 27,
    "gp": 28,
    "sp": 29,
    "fp": 30,
    "ra": 31,
}


def parse_reg(s: str) -> int:
    s = s.strip().lower()
    if s.startswith("$"):
        s = s[1:]
    if s in REGS:
        return REGS[s]
    return int(s, 0)


def find_section(data: bytes, name: str):
    """Return (section_header_offset, section_file_offset, section_size)."""
    if data[:4] != b"\x7fELF":
        raise ValueError("Not an ELF file")

    # ELF32 endianness from e_ident[5]
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
            return sh_off, sec_off, sec_size, endian

    return None, None, None, endian


def get_field(word: int, field: str) -> int:
    if field == "rs":
        return (word >> 21) & 0x1F
    if field == "rt":
        return (word >> 16) & 0x1F
    if field == "rd":
        return (word >> 11) & 0x1F
    raise ValueError(f"Unknown field '{field}' (expected rs/rt/rd)")


def set_field(word: int, field: str, value: int) -> int:
    if value < 0 or value > 31:
        raise ValueError(f"Register value out of range: {value}")
    if field == "rs":
        return (word & ~(0x1F << 21)) | (value << 21)
    if field == "rt":
        return (word & ~(0x1F << 16)) | (value << 16)
    if field == "rd":
        return (word & ~(0x1F << 11)) | (value << 11)
    raise ValueError(f"Unknown field '{field}' (expected rs/rt/rd)")


def main() -> int:
    ap = argparse.ArgumentParser(description="Patch MIPS instruction register fields at .text offsets")
    ap.add_argument("input", type=Path, help="Input ELF .o")
    ap.add_argument("output", type=Path, help="Output ELF .o")
    ap.add_argument(
        "--patch",
        action="append",
        default=[],
        help="Patch spec OFFSET:FIELD:FROM:TO (e.g. 0x4:rs:a0:v1). Can be repeated.",
    )
    ap.add_argument("-v", "--verbose", action="store_true", help="Verbose output")
    args = ap.parse_args()

    if not args.patch:
        print("Error: at least one --patch is required", file=sys.stderr)
        return 1

    data = bytearray(args.input.read_bytes())

    _, text_off, text_size, endian = find_section(data, ".text")
    if text_off is None:
        print("Error: .text section not found", file=sys.stderr)
        return 1

    patched = 0
    for spec in args.patch:
        parts = spec.split(":")
        if len(parts) != 4:
            print(f"Error: bad --patch '{spec}' (expected OFFSET:FIELD:FROM:TO)", file=sys.stderr)
            return 1

        off = int(parts[0], 0)
        field = parts[1].strip().lower()
        from_reg = parse_reg(parts[2])
        to_reg = parse_reg(parts[3])

        if off % 4 != 0:
            print(f"Error: offset not 4-byte aligned: 0x{off:x}", file=sys.stderr)
            return 1
        if off < 0 or off + 4 > text_size:
            print(f"Error: offset out of range: 0x{off:x} (text size 0x{text_size:x})", file=sys.stderr)
            return 1

        insn_off = text_off + off
        word = struct.unpack(endian + "I", data[insn_off : insn_off + 4])[0]
        cur = get_field(word, field)
        if cur != from_reg:
            print(
                f"Error: .text+0x{off:x} field {field} is {cur}, expected {from_reg}",
                file=sys.stderr,
            )
            return 1

        new_word = set_field(word, field, to_reg)
        data[insn_off : insn_off + 4] = struct.pack(endian + "I", new_word)
        patched += 1

        if args.verbose:
            print(f"Patched .text+0x{off:x} {field}: {from_reg} -> {to_reg}")

    args.output.write_bytes(data)
    if args.verbose or patched:
        print(f"Patched {patched} instruction field(s)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

