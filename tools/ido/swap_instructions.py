#!/usr/bin/env python3
"""
Swap two instructions at specified offsets in a MIPS object file.

Usage:
    python3 swap_instructions.py input.o output.o --offset1 0x10 --offset2 0x14
"""

import argparse
import struct
import sys
from pathlib import Path


def swap_instructions(input_path, output_path, offset1, offset2):
    """Swap instructions at two offsets in the .text section.

    Also swaps any relocations targeting the two offsets so that
    relocations follow their instructions to the new positions.
    """

    # Read the entire object file
    with open(input_path, 'rb') as f:
        data = bytearray(f.read())

    if data[:4] != b'\x7fELF':
        print(f"Error: Not a valid ELF file", file=sys.stderr)
        return False

    # Parse ELF header (little-endian, 32-bit)
    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shentsize = struct.unpack('<H', data[46:48])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]

    # Read string table section header
    shstr_hdr_offset = e_shoff + (e_shstrndx * e_shentsize)
    shstr_offset = struct.unpack('<I', data[shstr_hdr_offset + 16:shstr_hdr_offset + 20])[0]

    # Find .text and .rel.text sections
    text_offset = None
    text_size = None
    rel_offset = None
    rel_size = None

    for i in range(e_shnum):
        sh_offset = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack('<I', data[sh_offset:sh_offset + 4])[0]
        sh_offset_val = struct.unpack('<I', data[sh_offset + 16:sh_offset + 20])[0]
        sh_size = struct.unpack('<I', data[sh_offset + 20:sh_offset + 24])[0]

        name_start = shstr_offset + sh_name_idx
        name_end = data.find(b'\x00', name_start)
        section_name = data[name_start:name_end].decode('ascii')

        if section_name == '.text':
            text_offset = sh_offset_val
            text_size = sh_size
        elif section_name == '.rel.text':
            rel_offset = sh_offset_val
            rel_size = sh_size

    if text_offset is None:
        print(f"Error: .text section not found", file=sys.stderr)
        return False

    # Validate offsets
    if offset1 < 0 or offset1 + 4 > text_size:
        print(f"Error: offset1 0x{offset1:x} out of range (text size: 0x{text_size:x})", file=sys.stderr)
        return False

    if offset2 < 0 or offset2 + 4 > text_size:
        print(f"Error: offset2 0x{offset2:x} out of range (text size: 0x{text_size:x})", file=sys.stderr)
        return False

    # Calculate absolute file offsets
    file_offset1 = text_offset + offset1
    file_offset2 = text_offset + offset2

    # Read instructions
    instr1 = data[file_offset1:file_offset1 + 4]
    instr2 = data[file_offset2:file_offset2 + 4]

    # Swap instructions
    data[file_offset1:file_offset1 + 4] = instr2
    data[file_offset2:file_offset2 + 4] = instr1

    # Swap relocations targeting these offsets
    relocs_swapped = 0
    if rel_offset is not None and rel_size is not None:
        num_rels = rel_size // 8
        for i in range(num_rels):
            ro = rel_offset + i * 8
            r_off = struct.unpack('<I', data[ro:ro + 4])[0]
            if r_off == offset1:
                struct.pack_into('<I', data, ro, offset2)
                relocs_swapped += 1
            elif r_off == offset2:
                struct.pack_into('<I', data, ro, offset1)
                relocs_swapped += 1

    # Write output
    with open(output_path, 'wb') as f:
        f.write(data)

    msg = f"Swapped instructions at 0x{offset1:x} and 0x{offset2:x}"
    if relocs_swapped > 0:
        msg += f" ({relocs_swapped} relocations also swapped)"
    print(msg)
    return True


def main():
    parser = argparse.ArgumentParser(description='Swap two instructions in MIPS object file')
    parser.add_argument('input', type=str, help='Input object file')
    parser.add_argument('output', type=str, help='Output object file')
    parser.add_argument('--offset1', type=str, required=True, help='First instruction offset (hex)')
    parser.add_argument('--offset2', type=str, required=True, help='Second instruction offset (hex)')

    args = parser.parse_args()

    # Parse hex offsets
    offset1 = int(args.offset1, 16) if args.offset1.startswith('0x') else int(args.offset1)
    offset2 = int(args.offset2, 16) if args.offset2.startswith('0x') else int(args.offset2)

    success = swap_instructions(args.input, args.output, offset1, offset2)
    sys.exit(0 if success else 1)


if __name__ == '__main__':
    main()
