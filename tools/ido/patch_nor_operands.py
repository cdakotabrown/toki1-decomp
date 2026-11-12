#!/usr/bin/env python3
"""
Swap rs and rt operands of NOR instructions in a MIPS ELF .o file's .text section.

IDO 5.2 compiles ~x as NOR v0, v0, zero (rs=v0, rt=zero -> 0x00401027).
But the target binary has NOR v0, zero, v0 (rs=zero, rt=v0 -> 0x00021027).

This tool swaps the rs and rt fields for all NOR instructions in the .text section.

MIPS R-type instruction format (32 bits):
    bits 31-26: opcode (000000 for SPECIAL)
    bits 25-21: rs
    bits 20-16: rt
    bits 15-11: rd
    bits 10-6:  sa
    bits 5-0:   function (100111 = 0x27 for NOR)

Usage:
    python3 patch_nor_operands.py input.o output.o [-v]
"""

import sys
import struct
import argparse

# Register names for verbose output
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


def is_nor(word):
    """Check if instruction is a NOR (SPECIAL function 0x27).

    Returns (rs, rt, rd) or None.
    """
    op = (word >> 26) & 0x3f
    funct = word & 0x3f
    if op == 0 and funct == 0x27:
        rs = (word >> 21) & 0x1f
        rt = (word >> 16) & 0x1f
        rd = (word >> 11) & 0x1f
        return (rs, rt, rd)
    return None


def swap_nor_operands(word):
    """Swap rs and rt fields in a NOR instruction."""
    rs = (word >> 21) & 0x1f
    rt = (word >> 16) & 0x1f
    # Clear rs and rt fields, then set them swapped
    word = word & ~(0x1f << 21) & ~(0x1f << 16)
    word = word | (rt << 21) | (rs << 16)
    return word


def patch_nor_operands(elf_data, verbose=False):
    """Swap rs and rt operands of all NOR instructions in .text section.

    Returns (patched_data, count_of_nors_patched).
    """
    patched = bytearray(elf_data)

    # Check ELF magic
    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

    if verbose:
        print(f"ELF endianness: {'big' if is_big_endian else 'little'}")

    # Parse ELF header to find .text section
    e_shoff = struct.unpack(endian + 'I', elf_data[32:36])[0]
    e_shentsize = struct.unpack(endian + 'H', elf_data[46:48])[0]
    e_shnum = struct.unpack(endian + 'H', elf_data[48:50])[0]
    e_shstrndx = struct.unpack(endian + 'H', elf_data[50:52])[0]

    # Get section header string table
    shstrtab_off = e_shoff + e_shstrndx * e_shentsize
    shstrtab_addr = struct.unpack(endian + 'I', elf_data[shstrtab_off+16:shstrtab_off+20])[0]
    shstrtab_size = struct.unpack(endian + 'I', elf_data[shstrtab_off+20:shstrtab_off+24])[0]
    shstrtab = elf_data[shstrtab_addr:shstrtab_addr+shstrtab_size]

    text_offset = None
    text_size = None
    for i in range(e_shnum):
        sh_offset = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack(endian + 'I', elf_data[sh_offset:sh_offset+4])[0]
        sh_offset_data = struct.unpack(endian + 'I', elf_data[sh_offset+16:sh_offset+20])[0]
        sh_size = struct.unpack(endian + 'I', elf_data[sh_offset+20:sh_offset+24])[0]

        name_end = shstrtab.find(b'\x00', sh_name_idx)
        name = shstrtab[sh_name_idx:name_end].decode('ascii')

        if name == '.text':
            text_offset = sh_offset_data
            text_size = sh_size
            break

    if text_offset is None:
        raise ValueError(".text section not found")

    if verbose:
        print(f".text at offset 0x{text_offset:x}, size 0x{text_size:x}")

    # Scan for NOR instructions and swap rs/rt
    changes = 0
    for i in range(0, text_size, 4):
        file_off = text_offset + i
        if file_off + 4 > len(elf_data):
            break

        word = struct.unpack(endian + 'I', elf_data[file_off:file_off+4])[0]

        nor_info = is_nor(word)
        if nor_info is not None:
            rs, rt, rd = nor_info
            new_word = swap_nor_operands(word)
            struct.pack_into(endian + 'I', patched, file_off, new_word)
            changes += 1

            if verbose:
                rs_name = REG_NAMES.get(rs, str(rs))
                rt_name = REG_NAMES.get(rt, str(rt))
                rd_name = REG_NAMES.get(rd, str(rd))
                print(f"  0x{i:04x}: nor ${rd_name},${rs_name},${rt_name}"
                      f" -> nor ${rd_name},${rt_name},${rs_name}"
                      f" (0x{word:08x} -> 0x{new_word:08x})")

    return bytes(patched), changes


def main():
    parser = argparse.ArgumentParser(
        description='Swap rs and rt operands of NOR instructions in ELF .text section'
    )
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('-v', '--verbose', action='store_true', help='Print changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, count = patch_nor_operands(elf_data, args.verbose)

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Swapped operands in {count} NOR instruction(s)")


if __name__ == '__main__':
    main()
