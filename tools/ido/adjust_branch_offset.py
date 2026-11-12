#!/usr/bin/env python3
"""
Adjust the branch immediate (offset) of a branch instruction at a specific
offset in a MIPS ELF .o file's .text section.

This is used when swap_instructions moves a branch to a different position,
breaking its PC-relative offset. Unlike patch_instruction, this tool:
  - Only modifies the lower 16 bits (immediate field)
  - Verifies the instruction is actually a branch (BEQ/BNE/BLEZ/BGTZ/etc.)
  - Leaves opcode and register fields untouched

MIPS I-type branch format (32 bits):
    bits 31-26: opcode
    bits 25-21: rs
    bits 20-16: rt
    bits 15-0:  immediate (signed offset in words)

Branch opcodes:
    0x04 = BEQ, 0x05 = BNE, 0x06 = BLEZ, 0x07 = BGTZ
    0x01 with rt=0 = BLTZ, 0x01 with rt=1 = BGEZ

Usage:
    python3 adjust_branch_offset.py input.o output.o --offset 0x28 --imm 0x0f
"""

import sys
import struct
import argparse

BRANCH_OPCODES = {0x04: 'beq', 0x05: 'bne', 0x06: 'blez', 0x07: 'bgtz'}
REGIMM_OPCODE = 0x01  # BLTZ/BGEZ family


def is_branch(word):
    """Check if instruction is a branch. Returns opcode name or None."""
    op = (word >> 26) & 0x3f
    if op in BRANCH_OPCODES:
        return BRANCH_OPCODES[op]
    if op == REGIMM_OPCODE:
        rt = (word >> 16) & 0x1f
        if rt == 0:
            return 'bltz'
        elif rt == 1:
            return 'bgez'
    return None


def find_text_section(elf_data):
    """Find .text section offset and size in ELF file."""
    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

    e_shoff = struct.unpack(endian + 'I', elf_data[32:36])[0]
    e_shentsize = struct.unpack(endian + 'H', elf_data[46:48])[0]
    e_shnum = struct.unpack(endian + 'H', elf_data[48:50])[0]
    e_shstrndx = struct.unpack(endian + 'H', elf_data[50:52])[0]

    shstrtab_off = e_shoff + e_shstrndx * e_shentsize
    shstrtab_addr = struct.unpack(endian + 'I',
                                  elf_data[shstrtab_off+16:shstrtab_off+20])[0]
    shstrtab_size = struct.unpack(endian + 'I',
                                  elf_data[shstrtab_off+20:shstrtab_off+24])[0]
    shstrtab = elf_data[shstrtab_addr:shstrtab_addr+shstrtab_size]

    for i in range(e_shnum):
        sh_off = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack(endian + 'I',
                                    elf_data[sh_off:sh_off+4])[0]
        sh_offset_data = struct.unpack(endian + 'I',
                                       elf_data[sh_off+16:sh_off+20])[0]
        sh_size = struct.unpack(endian + 'I',
                                elf_data[sh_off+20:sh_off+24])[0]

        name_end = shstrtab.find(b'\x00', sh_name_idx)
        name = shstrtab[sh_name_idx:name_end].decode('ascii')

        if name == '.text':
            return sh_offset_data, sh_size, endian

    raise ValueError(".text section not found")


def main():
    parser = argparse.ArgumentParser(
        description='Adjust branch immediate at a specific offset in ELF .text'
    )
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('--offset', type=lambda x: int(x, 0), required=True,
                        help='Offset in .text of the branch instruction')
    parser.add_argument('--imm', type=lambda x: int(x, 0), required=True,
                        help='New immediate value (branch offset in words)')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Print changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = bytearray(f.read())

    text_offset, text_size, endian = find_text_section(elf_data)

    if args.offset >= text_size or args.offset % 4 != 0:
        print(f"Error: invalid offset 0x{args.offset:x}", file=sys.stderr)
        sys.exit(1)

    file_off = text_offset + args.offset
    word = struct.unpack(endian + 'I', elf_data[file_off:file_off+4])[0]

    branch_name = is_branch(word)
    if branch_name is None:
        print(f"Error: instruction at 0x{args.offset:x} (0x{word:08x}) "
              f"is not a branch", file=sys.stderr)
        sys.exit(1)

    old_imm = word & 0xffff
    new_word = (word & 0xffff0000) | (args.imm & 0xffff)

    if args.verbose:
        print(f"  0x{args.offset:04x}: {branch_name} imm 0x{old_imm:04x} -> "
              f"0x{args.imm & 0xffff:04x} (0x{word:08x} -> 0x{new_word:08x})")

    struct.pack_into(endian + 'I', elf_data, file_off, new_word)

    with open(args.output, 'wb') as f:
        f.write(bytes(elf_data))

    print(f"Adjusted branch at 0x{args.offset:x}: imm 0x{old_imm:04x} -> "
          f"0x{args.imm & 0xffff:04x}")


if __name__ == '__main__':
    main()
