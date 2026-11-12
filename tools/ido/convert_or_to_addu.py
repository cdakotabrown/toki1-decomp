#!/usr/bin/env python3
"""
Convert OR-encoded move instructions to ADDU encoding in ELF object files.

GNU AS encodes 'move $dst,$src' as 'or $dst,$src,$zero' (funct=0x25)
ASPSX/IDO encodes it as 'addu $dst,$src,$zero' (funct=0x21)

This tool patches the funct field from 0x25 to 0x21 for all such instructions.
"""

import sys
import struct
import argparse

def is_or_move(word):
    """
    Check if instruction is 'or $rd, $rs, $zero' (move via OR).
    Encoding: op=0, rt=0, funct=0x25
    """
    op = (word >> 26) & 0x3f
    rt = (word >> 16) & 0x1f
    funct = word & 0x3f
    return op == 0 and rt == 0 and funct == 0x25

def convert_or_to_addu(word):
    """Convert OR-encoded move to ADDU encoding."""
    # Just change funct from 0x25 to 0x21
    return (word & 0xFFFFFFC0) | 0x21

def patch_or_to_addu(elf_data, verbose=False, exclude_offsets=None):
    """Patch all OR-encoded moves to ADDU encoding in .text section."""
    if exclude_offsets is None:
        exclude_offsets = set()
    patched = bytearray(elf_data)

    # Check ELF and determine endianness
    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

    if verbose:
        print(f"ELF endianness: {'big' if is_big_endian else 'little'}")

    # Parse ELF header
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

    changes = 0
    for i in range(0, text_size, 4):
        file_off = text_offset + i
        word = struct.unpack(endian + 'I', elf_data[file_off:file_off+4])[0]

        if is_or_move(word):
            if i in exclude_offsets:
                if verbose:
                    print(f"  0x{i:04x}: EXCLUDED from conversion")
                continue
            new_word = convert_or_to_addu(word)
            struct.pack_into(endian + 'I', patched, file_off, new_word)
            changes += 1
            if verbose:
                rs = (word >> 21) & 0x1f
                rd = (word >> 11) & 0x1f
                print(f"  0x{i:04x}: 0x{word:08x} -> 0x{new_word:08x} (move ${rd},${rs})")

    return bytes(patched), changes

def main():
    parser = argparse.ArgumentParser(
        description='Convert OR-encoded moves to ADDU encoding in ELF files'
    )
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('--exclude-offsets',
                       help='Comma-separated .text offsets to skip (e.g., "0x4c,0x50")')
    parser.add_argument('-v', '--verbose', action='store_true', help='Print changes')

    args = parser.parse_args()

    exclude_offsets = set()
    if args.exclude_offsets:
        for offset_str in args.exclude_offsets.split(','):
            offset_str = offset_str.strip()
            if offset_str:
                exclude_offsets.add(int(offset_str, 0))

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, count = patch_or_to_addu(elf_data, args.verbose, exclude_offsets)

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Converted {count} OR-encoded moves to ADDU encoding")

if __name__ == '__main__':
    main()
