#!/usr/bin/env python3
"""
fix_sign_test_pattern.py - Convert IDO 5.2 sign-test pattern to IDO 5.1 style

IDO 5.2 at -O2 optimizes `x & 0x80000000` to `sll+bgez` (peephole), and
`(u32)x >= 0x80000000U` to `lui at,0x8000; sltu at,x,at; bnez at` (comparison).

Some library functions were compiled with IDO 5.1 which instead generates:
    lui  at, 0x8000
    and  rD, rS, at
    beqz rD, target

This tool converts the IDO 5.2 comparison pattern to the IDO 5.1 bitwise pattern:
    lui  at, 0x8000          ->  lui  at, 0x8000    (unchanged)
    sltu at, rS, at          ->  and  rD, rS, at    (replace sltu with and, change rd)
    bnez at, target          ->  beqz rD, target    (invert branch, change rs to rD)

Usage:
    python3 fix_sign_test_pattern.py input.o output.o --dest-reg t8

The --dest-reg specifies what register the `and` result goes into (default: t8).
"""

import argparse
import struct
import sys
from pathlib import Path

REGS = {
    "zero": 0, "at": 1, "v0": 2, "v1": 3,
    "a0": 4, "a1": 5, "a2": 6, "a3": 7,
    "t0": 8, "t1": 9, "t2": 10, "t3": 11,
    "t4": 12, "t5": 13, "t6": 14, "t7": 15,
    "s0": 16, "s1": 17, "s2": 18, "s3": 19,
    "s4": 20, "s5": 21, "s6": 22, "s7": 23,
    "t8": 24, "t9": 25, "k0": 26, "k1": 27,
    "gp": 28, "sp": 29, "fp": 30, "ra": 31,
}


def find_text_section(data):
    """Return (file_offset, size) of .text section."""
    if data[:4] != b"\x7fELF":
        raise ValueError("Not an ELF file")
    e_shoff = struct.unpack("<I", data[32:36])[0]
    e_shentsize = struct.unpack("<H", data[46:48])[0]
    e_shnum = struct.unpack("<H", data[48:50])[0]
    e_shstrndx = struct.unpack("<H", data[50:52])[0]
    shstr_hdr = e_shoff + e_shstrndx * e_shentsize
    shstr_off = struct.unpack("<I", data[shstr_hdr + 16:shstr_hdr + 20])[0]
    for i in range(e_shnum):
        sh_off = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack("<I", data[sh_off:sh_off + 4])[0]
        n0 = shstr_off + sh_name_idx
        n1 = data.find(b"\x00", n0)
        name = data[n0:n1].decode("ascii", errors="ignore")
        if name == ".text":
            sec_off = struct.unpack("<I", data[sh_off + 16:sh_off + 20])[0]
            sec_size = struct.unpack("<I", data[sh_off + 20:sh_off + 24])[0]
            return sec_off, sec_size
    return None, None


def main():
    ap = argparse.ArgumentParser(
        description="Convert sltu+bnez sign-test pattern to and+beqz (IDO 5.1 style)")
    ap.add_argument("input", type=Path, help="Input ELF .o")
    ap.add_argument("output", type=Path, help="Output ELF .o")
    ap.add_argument("--dest-reg", default="t8",
                    help="Destination register for AND result (default: t8)")
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()

    dest_reg = REGS.get(args.dest_reg)
    if dest_reg is None:
        print(f"Error: unknown register '{args.dest_reg}'", file=sys.stderr)
        return 1

    data = bytearray(args.input.read_bytes())
    text_off, text_size = find_text_section(data)
    if text_off is None:
        print("Error: .text section not found", file=sys.stderr)
        return 1

    n_insns = text_size // 4
    patched = 0

    for i in range(n_insns - 2):
        off0 = text_off + i * 4
        off1 = off0 + 4
        off2 = off1 + 4

        w0 = struct.unpack("<I", data[off0:off0 + 4])[0]
        w1 = struct.unpack("<I", data[off1:off1 + 4])[0]
        w2 = struct.unpack("<I", data[off2:off2 + 4])[0]

        # Check pattern: lui at, 0x8000
        op0 = (w0 >> 26) & 0x3F
        rt0 = (w0 >> 16) & 0x1F
        imm0 = w0 & 0xFFFF
        if op0 != 0x0F or rt0 != REGS["at"] or imm0 != 0x8000:
            continue

        # Check: sltu at, rS, at  (R-type, func=0x2b)
        op1 = (w1 >> 26) & 0x3F
        rs1 = (w1 >> 21) & 0x1F
        rt1 = (w1 >> 16) & 0x1F
        rd1 = (w1 >> 11) & 0x1F
        func1 = w1 & 0x3F
        if op1 != 0 or func1 != 0x2B or rt1 != REGS["at"] or rd1 != REGS["at"]:
            continue

        # rs1 is the register being tested (e.g., v0)
        src_reg = rs1

        # Check: bnez at, target  (bne at, zero, offset)
        op2 = (w2 >> 26) & 0x3F
        rs2 = (w2 >> 21) & 0x1F
        rt2 = (w2 >> 16) & 0x1F
        imm2 = w2 & 0xFFFF
        if op2 != 0x05 or rs2 != REGS["at"] or rt2 != REGS["zero"]:
            continue

        if args.verbose:
            print(f"  Found sltu+bnez pattern at .text+0x{i*4:x}")
            print(f"    src_reg={src_reg}, dest_reg={dest_reg}")

        # Replace sltu with and: and dest_reg, src_reg, at
        # R-type: op=0, rs=src_reg, rt=at, rd=dest_reg, sa=0, func=0x24(and)
        new_w1 = ((src_reg << 21) | (REGS["at"] << 16) |
                  (dest_reg << 11) | 0x24)
        struct.pack_into("<I", data, off1, new_w1)

        # Replace bnez at with beqz dest_reg (same offset)
        # beq dest_reg, zero, offset: op=0x04, rs=dest_reg, rt=zero
        new_w2 = (0x04 << 26) | (dest_reg << 21) | (REGS["zero"] << 16) | imm2
        struct.pack_into("<I", data, off2, new_w2)

        patched += 1

    if args.verbose:
        print(f"  Patched {patched} sign-test pattern(s)")

    args.output.write_bytes(data)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
