#!/usr/bin/env python3
"""
swap_branch_operands.py - Swap rs and rt fields in branch instructions

MIPS beq/bne are commutative: beq $s0,$t6,offset == beq $t6,$s0,offset
but they encode differently. IDO sometimes picks the opposite operand order.

This swaps the rs (bits 25-21) and rt (bits 20-16) fields at specified offsets.
Only operates on beq (0x04) and bne (0x05) since they're the only commutative branches.

Usage:
    python3 swap_branch_operands.py input.o output.o --offsets 0x28
"""

import argparse
import struct
import sys
from pathlib import Path


def find_text_section(data):
    """Return (file_offset, size) of .text section."""
    if data[:4] != b"\x7fELF":
        raise ValueError("Not an ELF file")

    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shentsize = struct.unpack('<H', data[46:48])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]

    shstr_hdr = e_shoff + e_shstrndx * e_shentsize
    shstr_off = struct.unpack('<I', data[shstr_hdr + 16:shstr_hdr + 20])[0]

    for i in range(e_shnum):
        sh_off = e_shoff + i * e_shentsize
        nm_idx = struct.unpack('<I', data[sh_off:sh_off + 4])[0]
        end = data.find(b'\x00', shstr_off + nm_idx)
        name = data[shstr_off + nm_idx:end].decode('ascii', errors='ignore')
        if name == '.text':
            off = struct.unpack('<I', data[sh_off + 16:sh_off + 20])[0]
            sz = struct.unpack('<I', data[sh_off + 20:sh_off + 24])[0]
            return off, sz
    return None, None


def main():
    ap = argparse.ArgumentParser(
        description="Swap rs and rt operands in beq/bne instructions"
    )
    ap.add_argument("input", type=Path, help="Input ELF .o")
    ap.add_argument("output", type=Path, help="Output ELF .o")
    ap.add_argument(
        "--offsets",
        required=True,
        nargs="+",
        type=lambda s: int(s, 0),
        help=".text offsets of branch instructions to swap (hex)",
    )
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()

    data = bytearray(args.input.read_bytes())
    text_off, text_size = find_text_section(data)
    if text_off is None:
        print("Error: .text section not found", file=sys.stderr)
        return 1

    for off in args.offsets:
        if off < 0 or off + 4 > text_size:
            print(f"Error: offset 0x{off:x} out of range (text size 0x{text_size:x})",
                  file=sys.stderr)
            return 1
        if off % 4 != 0:
            print(f"Error: offset 0x{off:x} not 4-byte aligned", file=sys.stderr)
            return 1

        abs_off = text_off + off
        word = struct.unpack('<I', data[abs_off:abs_off + 4])[0]
        opcode = (word >> 26) & 0x3F

        if opcode not in (0x04, 0x05):
            op_name = {0x04: "beq", 0x05: "bne"}.get(opcode, f"opcode={opcode:#x}")
            print(f"Error: instruction at 0x{off:x} is not beq/bne "
                  f"(opcode=0x{opcode:02x})", file=sys.stderr)
            return 1

        rs = (word >> 21) & 0x1F
        rt = (word >> 16) & 0x1F
        imm = word & 0xFFFF

        new_word = (opcode << 26) | (rt << 21) | (rs << 16) | imm
        struct.pack_into('<I', data, abs_off, new_word)

        if args.verbose:
            op_name = "beq" if opcode == 0x04 else "bne"
            REG_NAMES = {
                0: 'zero', 1: 'at', 2: 'v0', 3: 'v1',
                4: 'a0', 5: 'a1', 6: 'a2', 7: 'a3',
                8: 't0', 9: 't1', 10: 't2', 11: 't3',
                12: 't4', 13: 't5', 14: 't6', 15: 't7',
                16: 's0', 17: 's1', 18: 's2', 19: 's3',
                20: 's4', 21: 's5', 22: 's6', 23: 's7',
                24: 't8', 25: 't9', 26: 'k0', 27: 'k1',
                28: 'gp', 29: 'sp', 30: 'fp', 31: 'ra',
            }
            rs_name = REG_NAMES.get(rs, f"${rs}")
            rt_name = REG_NAMES.get(rt, f"${rt}")
            print(f"  0x{off:x}: {op_name} {rs_name},{rt_name} -> "
                  f"{op_name} {rt_name},{rs_name}")

    args.output.write_bytes(data)
    if args.verbose:
        print(f"Wrote {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
