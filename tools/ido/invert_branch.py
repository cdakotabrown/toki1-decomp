#!/usr/bin/env python3
"""
invert_branch.py - Invert branch condition (beq<->bne, blez<->bgtz, bltz<->bgez)

IDO sometimes generates the opposite branch condition from the target binary,
typically when insert_instruction_and_shift adds an early-return path that
requires the preceding branch to be inverted.

Usage:
    python3 invert_branch.py input.o output.o --offsets 0x10 0x28
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


# Inversion pairs: opcode -> inverted opcode
INVERT_OPCODE = {
    0x04: 0x05,  # beq -> bne
    0x05: 0x04,  # bne -> beq
    0x06: 0x07,  # blez -> bgtz
    0x07: 0x06,  # bgtz -> blez
}

# REGIMM (opcode 0x01) inversion: rt field
INVERT_REGIMM = {
    0x00: 0x01,  # bltz -> bgez
    0x01: 0x00,  # bgez -> bltz
    0x10: 0x11,  # bltzal -> bgezal
    0x11: 0x10,  # bgezal -> bltzal
}


def main():
    ap = argparse.ArgumentParser(
        description="Invert branch conditions (beq<->bne, etc.)"
    )
    ap.add_argument("input", type=Path, help="Input ELF .o")
    ap.add_argument("output", type=Path, help="Output ELF .o")
    ap.add_argument(
        "--offsets",
        required=True,
        nargs="+",
        type=lambda s: int(s, 0),
        help=".text offsets of branch instructions to invert (hex)",
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

        if opcode in INVERT_OPCODE:
            new_opcode = INVERT_OPCODE[opcode]
            new_word = (new_opcode << 26) | (word & 0x03FFFFFF)
            struct.pack_into('<I', data, abs_off, new_word)

            if args.verbose:
                names = {0x04: 'beq', 0x05: 'bne', 0x06: 'blez', 0x07: 'bgtz'}
                print(f"  0x{off:x}: {names[opcode]} -> {names[new_opcode]}")

        elif opcode == 0x01:
            rt = (word >> 16) & 0x1F
            if rt in INVERT_REGIMM:
                new_rt = INVERT_REGIMM[rt]
                new_word = (word & 0xFFE0FFFF) | (new_rt << 16)
                struct.pack_into('<I', data, abs_off, new_word)

                if args.verbose:
                    names = {0x00: 'bltz', 0x01: 'bgez',
                             0x10: 'bltzal', 0x11: 'bgezal'}
                    print(f"  0x{off:x}: {names[rt]} -> {names[new_rt]}")
            else:
                print(f"Error: REGIMM instruction at 0x{off:x} has rt=0x{rt:02x}, "
                      f"not a recognized branch", file=sys.stderr)
                return 1
        else:
            print(f"Error: instruction at 0x{off:x} is not a branch "
                  f"(opcode=0x{opcode:02x})", file=sys.stderr)
            return 1

    args.output.write_bytes(data)
    if args.verbose:
        print(f"Wrote {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
