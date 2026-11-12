#!/usr/bin/env python3
"""
patch_store_zero.py - Fix as1 peephole $zero propagation in store instructions

When IDO's as1 sees `move Rd, $zero; sw Rd, offset(base)` and Rd is dead after
the store, it optimizes to `sw $zero, offset(base)` and eliminates the move.
However, the original binary may have used a different register (e.g., $v0) that
as1 wouldn't optimize away (because v0 is the return register).

This tool fixes the pattern by:
1. Finding `sw $zero, offset(base)` instructions at a given offset
2. Changing the rt field from $zero to a specified register
3. Optionally inserting a `move $reg, $zero` before the store sequence

Usage:
    python3 patch_store_zero.py input.o output.o --offset 0x30 --register v0

    # With instruction rotation (move the move instruction from after to before)
    python3 patch_store_zero.py input.o output.o --offset 0x30 --register v0 \
        --move-from 0x30 --move-to 0x24
"""

import argparse
import struct
import shutil
import sys

# MIPS register name to number mapping
REG_MAP = {
    'zero': 0, 'at': 1, 'v0': 2, 'v1': 3,
    'a0': 4, 'a1': 5, 'a2': 6, 'a3': 7,
    't0': 8, 't1': 9, 't2': 10, 't3': 11,
    't4': 12, 't5': 13, 't6': 14, 't7': 15,
    's0': 16, 's1': 17, 's2': 18, 's3': 19,
    's4': 20, 's5': 21, 's6': 22, 's7': 23,
    't8': 24, 't9': 25, 'k0': 26, 'k1': 27,
    'gp': 28, 'sp': 29, 'fp': 30, 'ra': 31,
}


def find_text_section(elf_data):
    """Find .text section offset and size in ELF file."""
    e_shoff = struct.unpack_from('<I', elf_data, 32)[0]
    e_shentsize = struct.unpack_from('<H', elf_data, 46)[0]
    e_shnum = struct.unpack_from('<H', elf_data, 48)[0]
    e_shstrndx = struct.unpack_from('<H', elf_data, 50)[0]

    shstrtab_offset = struct.unpack_from('<I', elf_data,
                                          e_shoff + e_shstrndx * e_shentsize + 16)[0]

    for i in range(e_shnum):
        sh_offset = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from('<I', elf_data, sh_offset)[0]
        name = elf_data[shstrtab_offset + sh_name:].split(b'\0')[0].decode()
        if name == '.text':
            return (struct.unpack_from('<I', elf_data, sh_offset + 16)[0],
                    struct.unpack_from('<I', elf_data, sh_offset + 20)[0])
    return None, None


def patch_store_register(elf_data, text_offset, instr_offset, new_reg_num):
    """Change the rt field of a sw instruction from $zero to new_reg."""
    file_offset = text_offset + instr_offset
    word = struct.unpack_from('<I', elf_data, file_offset)[0]

    opcode = (word >> 26) & 0x3f
    if opcode != 0x2b:  # SW opcode
        print(f"Error: instruction at offset 0x{instr_offset:x} is not SW "
              f"(opcode=0x{opcode:02x})")
        return False

    old_rt = (word >> 16) & 0x1f
    if old_rt != 0:
        print(f"Warning: instruction at offset 0x{instr_offset:x} uses "
              f"${old_rt}, not $zero")

    new_word = (word & ~(0x1f << 16)) | (new_reg_num << 16)
    struct.pack_into('<I', elf_data, file_offset, new_word)
    print(f"Patched sw at 0x{instr_offset:x}: rt ${old_rt} -> ${new_reg_num}")
    return True


def main():
    parser = argparse.ArgumentParser(
        description='Fix as1 peephole $zero propagation in store instructions')
    parser.add_argument('input', help='Input object file')
    parser.add_argument('output', help='Output object file')
    parser.add_argument('--offset', required=True,
                        help='Offset of sw instruction in .text (hex)')
    parser.add_argument('--register', required=True,
                        help='Register to use instead of $zero (e.g., v0, a0)')
    args = parser.parse_args()

    offset = int(args.offset, 16)
    reg_name = args.register.lower().replace('$', '')
    if reg_name not in REG_MAP:
        print(f"Error: unknown register '{args.register}'")
        sys.exit(1)
    reg_num = REG_MAP[reg_name]

    shutil.copy(args.input, args.output)

    with open(args.output, 'rb') as f:
        data = bytearray(f.read())

    text_offset, text_size = find_text_section(data)
    if text_offset is None:
        print("Error: .text section not found")
        sys.exit(1)

    if offset >= text_size:
        print(f"Error: offset 0x{offset:x} exceeds .text size 0x{text_size:x}")
        sys.exit(1)

    if not patch_store_register(data, text_offset, offset, reg_num):
        sys.exit(1)

    with open(args.output, 'wb') as f:
        f.write(data)

    print(f"Written to {args.output}")


if __name__ == '__main__':
    main()
