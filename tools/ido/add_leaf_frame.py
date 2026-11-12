#!/usr/bin/env python3
"""
Add a stack frame to a leaf function in a MIPS ELF object file.

IDO at -O2 eliminates stack frames for leaf functions (no function calls).
Some original binaries have small frames (e.g., 8 bytes) on leaf functions.
This tool adds the prologue/epilogue to match.

Transformations:
1. Insert 'addiu sp,sp,-N' at the START of .text (shifts all code by 4 bytes)
2. Replace the 'nop' in the 'jr ra' delay slot with 'addiu sp,sp,N'

Branch offsets are NOT adjusted because MIPS branches use PC-relative offsets,
and inserting at the very start shifts all instructions uniformly, preserving
all relative branch distances within the function.

Usage:
    python3 add_leaf_frame.py input.o output.o --frame-size 8
"""

import argparse
import struct
import sys


def find_text_section(data):
    """Find .text section offset and size in ELF file."""
    if data[:4] != b'\x7fELF':
        raise ValueError("Not a valid ELF file")

    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shentsize = struct.unpack('<H', data[46:48])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]

    shstr_hdr_offset = e_shoff + (e_shstrndx * e_shentsize)
    shstr_offset = struct.unpack('<I', data[shstr_hdr_offset + 16:shstr_hdr_offset + 20])[0]

    for i in range(e_shnum):
        sh_offset = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack('<I', data[sh_offset:sh_offset + 4])[0]
        sh_file_offset = struct.unpack('<I', data[sh_offset + 16:sh_offset + 20])[0]
        sh_size = struct.unpack('<I', data[sh_offset + 20:sh_offset + 24])[0]

        name_start = shstr_offset + sh_name_idx
        name_end = data.find(b'\x00', name_start)
        section_name = data[name_start:name_end].decode('ascii')

        if section_name == '.text':
            return sh_offset, sh_file_offset, sh_size, i

    raise ValueError(".text section not found")


def add_leaf_frame(input_path, output_path, frame_size):
    """Add leaf frame prologue/epilogue to object file."""

    with open(input_path, 'rb') as f:
        data = bytearray(f.read())

    sh_offset, text_file_offset, text_size, section_idx = find_text_section(data)

    # Encode prologue and epilogue instructions
    # addiu sp, sp, -frame_size
    prologue = struct.pack('<I', 0x27bd0000 | ((-frame_size) & 0xFFFF))
    # addiu sp, sp, frame_size
    epilogue = struct.pack('<I', 0x27bd0000 | (frame_size & 0xFFFF))

    # Find 'jr ra' (0x03e00008) followed by 'nop' (0x00000000) in .text
    jr_ra_offset = None
    for i in range(0, text_size - 4, 4):
        word = struct.unpack('<I', data[text_file_offset + i:text_file_offset + i + 4])[0]
        if word == 0x03e00008:  # jr ra
            next_word = struct.unpack('<I', data[text_file_offset + i + 4:text_file_offset + i + 8])[0]
            if next_word == 0x00000000:  # nop in delay slot
                jr_ra_offset = i
                break

    if jr_ra_offset is None:
        print("Error: Could not find 'jr ra' followed by 'nop' in .text", file=sys.stderr)
        return False

    # Step 1: Replace nop in jr ra delay slot with epilogue
    epilogue_file_offset = text_file_offset + jr_ra_offset + 4
    data[epilogue_file_offset:epilogue_file_offset + 4] = epilogue

    # Step 2: Insert prologue at the start of .text
    # This shifts all subsequent data by 4 bytes
    insert_point = text_file_offset
    data[insert_point:insert_point] = prologue  # insert 4 bytes

    # Step 3: Update .text section size in section header
    # The section header is at sh_offset, size field is at offset 20
    # But first we need to recalculate sh_offset since we inserted bytes
    # If sh_offset > text_file_offset, it shifted by 4
    e_shoff = struct.unpack('<I', data[32:36])[0]
    if e_shoff > text_file_offset:
        # Section header table shifted, update ELF header
        struct.pack_into('<I', data, 32, e_shoff + 4)
        e_shoff += 4

    # Recalculate section header offset for .text section
    e_shentsize = struct.unpack('<H', data[46:48])[0]
    new_sh_offset = e_shoff + (section_idx * e_shentsize)

    # Update section size (+4 for the inserted instruction)
    new_text_size = text_size + 4
    struct.pack_into('<I', data, new_sh_offset + 20, new_text_size)

    # Update file offsets for all sections that come after .text
    e_shnum = struct.unpack('<H', data[48:50])[0]
    for i in range(e_shnum):
        sec_hdr = e_shoff + (i * e_shentsize)
        sec_file_offset = struct.unpack('<I', data[sec_hdr + 16:sec_hdr + 20])[0]
        if sec_file_offset > text_file_offset and i != section_idx:
            struct.pack_into('<I', data, sec_hdr + 16, sec_file_offset + 4)

    with open(output_path, 'wb') as f:
        f.write(data)

    print(f"Added leaf frame: addiu sp,sp,-{frame_size} / addiu sp,sp,{frame_size}")
    print(f"  Inserted prologue at .text+0x0")
    print(f"  Replaced nop with epilogue at .text+0x{jr_ra_offset + 4 + 4:x}")
    print(f"  New .text size: 0x{new_text_size:x} (was 0x{text_size:x})")
    return True


def main():
    parser = argparse.ArgumentParser(description='Add leaf frame to MIPS object file')
    parser.add_argument('input', type=str, help='Input object file')
    parser.add_argument('output', type=str, help='Output object file')
    parser.add_argument('--frame-size', type=int, required=True,
                        help='Frame size in bytes (e.g., 8)')

    args = parser.parse_args()
    success = add_leaf_frame(args.input, args.output, args.frame_size)
    sys.exit(0 if success else 1)


if __name__ == '__main__':
    main()
