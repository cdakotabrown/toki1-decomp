#!/usr/bin/env python3
"""
nop_at_offset.py - Replace instruction at specific offset with NOP

Usage:
    python3 tools/ido/nop_at_offset.py input.o output.o --offset 0xB4
"""

import argparse
import struct
import sys

NOP = 0x00000000


def main():
    parser = argparse.ArgumentParser(description='Replace instruction at offset with NOP')
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output ELF object file')
    parser.add_argument('--offset', required=True, help='Offset within .text to NOP (hex)')
    args = parser.parse_args()

    offset = int(args.offset, 0)

    with open(args.input, 'rb') as f:
        data = bytearray(f.read())

    # Find .text section
    if data[:4] != b"\x7fELF":
        print("Not an ELF file", file=sys.stderr)
        sys.exit(1)

    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shentsize = struct.unpack('<H', data[46:48])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]

    shstr_hdr_off = e_shoff + (e_shstrndx * e_shentsize)
    shstr_off = struct.unpack('<I', data[shstr_hdr_off + 16 : shstr_hdr_off + 20])[0]

    text_off = None
    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack('<I', data[sh_off : sh_off + 4])[0]
        n0 = shstr_off + sh_name_idx
        n_end = data.index(b'\x00', n0)
        name = data[n0:n_end].decode()
        if name == '.text':
            text_off = struct.unpack('<I', data[sh_off + 16 : sh_off + 20])[0]
            text_size = struct.unpack('<I', data[sh_off + 20 : sh_off + 24])[0]
            break

    if text_off is None:
        print("No .text section found", file=sys.stderr)
        sys.exit(1)

    if offset >= text_size:
        print(f"Offset 0x{offset:x} out of bounds (text size 0x{text_size:x})", file=sys.stderr)
        sys.exit(1)

    abs_off = text_off + offset
    old_insn = struct.unpack('<I', data[abs_off:abs_off+4])[0]
    print(f"Replacing 0x{old_insn:08x} at offset 0x{offset:x} with NOP")
    struct.pack_into('<I', data, abs_off, NOP)

    with open(args.output, 'wb') as f:
        f.write(data)


if __name__ == '__main__':
    main()
