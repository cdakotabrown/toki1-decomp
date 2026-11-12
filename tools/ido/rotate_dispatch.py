#!/usr/bin/env python3
"""
rotate_dispatch.py - Rearrange dispatch header for magazine_slot_get

IDO compiles the switch dispatch with 'move v0,v1' in the jr ra delay slot
(position 12), but the original has it at position 4 (first beq delay slot)
with nop in the jr ra delay slot. This script:

1. Moves instruction at src_offset to dest_offset
2. Shifts instructions between dest_offset and src_offset-4 down by 4 bytes
3. Writes nop (0x00000000) at the position after jr ra (new delay slot)
4. Adjusts first beq branch offset by +1 (case body is 1 instr further)

Usage:
    python3 rotate_dispatch.py input.o output.o \
        --move-from 0x30 --move-to 0x10 --fix-beq 0x0C --add-nop 0x34
"""

import argparse
import struct
import sys
from pathlib import Path


def find_text_section(data):
    """Find .text section offset and size in ELF file."""
    if data[:4] != b'\x7fELF':
        print("Error: Not a valid ELF file", file=sys.stderr)
        return None, None, None

    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shentsize = struct.unpack('<H', data[46:48])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]

    shstr_hdr_offset = e_shoff + (e_shstrndx * e_shentsize)
    shstr_offset = struct.unpack('<I', data[shstr_hdr_offset + 16:shstr_hdr_offset + 20])[0]

    for i in range(e_shnum):
        sh_offset = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack('<I', data[sh_offset:sh_offset + 4])[0]
        sh_offset_val = struct.unpack('<I', data[sh_offset + 16:sh_offset + 20])[0]
        sh_size_val = struct.unpack('<I', data[sh_offset + 20:sh_offset + 24])[0]

        name_start = shstr_offset + sh_name_idx
        name_end = data.find(b'\x00', name_start)
        section_name = data[name_start:name_end].decode('ascii')

        if section_name == '.text':
            return sh_offset_val, sh_size_val, sh_offset

    return None, None, None


def rotate_dispatch(input_path, output_path, move_from, move_to, fix_beq_offset, add_nop_offset):
    """Rotate an instruction from one position to another, shifting intermediate instructions."""
    with open(input_path, 'rb') as f:
        data = bytearray(f.read())

    text_offset, text_size, sh_offset = find_text_section(data)
    if text_offset is None:
        print("Error: .text section not found", file=sys.stderr)
        return False

    abs_from = text_offset + move_from
    abs_to = text_offset + move_to

    # Save the instruction to move
    instr = bytes(data[abs_from:abs_from + 4])

    # Shift instructions between dest and src down by 4 bytes
    # Move data[abs_to:abs_from] to data[abs_to+4:abs_from+4]
    chunk = bytes(data[abs_to:abs_from])
    data[abs_to + 4:abs_from + 4] = chunk

    # Place the moved instruction at the destination
    data[abs_to:abs_to + 4] = instr

    # If add_nop_offset specified, write nop there
    if add_nop_offset is not None:
        abs_nop = text_offset + add_nop_offset
        data[abs_nop:abs_nop + 4] = struct.pack('<I', 0x00000000)

    # Fix the first beq branch offset (+1 because case body moved 1 instr further)
    if fix_beq_offset is not None:
        abs_beq = text_offset + fix_beq_offset
        beq_instr = struct.unpack('<I', data[abs_beq:abs_beq + 4])[0]
        opcode = (beq_instr >> 26) & 0x3F
        if opcode == 4:  # BEQ
            offset_field = beq_instr & 0xFFFF
            if offset_field & 0x8000:
                offset_field = offset_field - 0x10000
            offset_field += 1
            beq_instr = (beq_instr & 0xFFFF0000) | (offset_field & 0xFFFF)
            data[abs_beq:abs_beq + 4] = struct.pack('<I', beq_instr)
            print(f"Fixed BEQ at 0x{fix_beq_offset:x}: offset +1")

    # Update .text section size to account for the inserted instruction
    new_text_size = text_size + 4
    struct.pack_into('<I', data, sh_offset + 20, new_text_size)

    with open(output_path, 'wb') as f:
        f.write(data)

    print(f"Rotated instruction from 0x{move_from:x} to 0x{move_to:x}")
    return True


def main():
    parser = argparse.ArgumentParser(description='Rotate dispatch instruction in MIPS object file')
    parser.add_argument('input', type=str, help='Input object file')
    parser.add_argument('output', type=str, help='Output object file')
    parser.add_argument('--move-from', type=str, required=True, help='Source offset (hex)')
    parser.add_argument('--move-to', type=str, required=True, help='Destination offset (hex)')
    parser.add_argument('--fix-beq', type=str, default=None, help='BEQ offset to fix (hex)')
    parser.add_argument('--add-nop', type=str, default=None, help='Offset to write NOP (hex)')

    args = parser.parse_args()

    move_from = int(args.move_from, 16) if args.move_from.startswith('0x') else int(args.move_from)
    move_to = int(args.move_to, 16) if args.move_to.startswith('0x') else int(args.move_to)
    fix_beq = int(args.fix_beq, 16) if args.fix_beq and args.fix_beq.startswith('0x') else (int(args.fix_beq) if args.fix_beq else None)
    add_nop = int(args.add_nop, 16) if args.add_nop and args.add_nop.startswith('0x') else (int(args.add_nop) if args.add_nop else None)

    success = rotate_dispatch(args.input, args.output, move_from, move_to, fix_beq, add_nop)
    sys.exit(0 if success else 1)


if __name__ == '__main__':
    main()
