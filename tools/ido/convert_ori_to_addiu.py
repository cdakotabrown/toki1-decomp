#!/usr/bin/env python3
"""
Convert ORI instructions to ADDIU encoding in ELF object files.

Some functions have ORI where the target uses ADDIU for loading constants.
For small positive immediates with rs == rt, these are functionally equivalent:
  ori  $rt, $rs, imm  -> rt = rs | imm
  addiu $rt, $rs, imm -> rt = rs + imm (sign-extended)

When rs == rt and imm is small positive, both produce the same result.

This tool patches the opcode field from 0x0D (ori) to 0x09 (addiu).

Usage:
  python3 convert_ori_to_addiu.py input.o output.o [--offsets 0x10,0x20]

PostProc annotation:
  @PostProc: convert_ori_to_addiu --offsets 59,62
"""

import sys
import struct
import argparse


def is_ori_with_same_reg(word):
    """Check if instruction is 'ori $rt, $rs, imm' where rs == rt."""
    op = (word >> 26) & 0x3f
    rs = (word >> 21) & 0x1f
    rt = (word >> 16) & 0x1f
    return op == 0x0D and rs == rt


def convert_ori_to_addiu(word):
    """Convert ORI to ADDIU - just change opcode from 0x0D to 0x09."""
    return (word & 0x03FFFFFF) | (0x09 << 26)


def patch_ori_to_addiu(elf_data, verbose=False, offsets=None):
    """Patch specified ORI instructions to ADDIU encoding in .text section."""
    patched = bytearray(elf_data)

    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

    e_shoff = struct.unpack(endian + 'I', elf_data[32:36])[0]
    e_shentsize = struct.unpack(endian + 'H', elf_data[46:48])[0]
    e_shnum = struct.unpack(endian + 'H', elf_data[48:50])[0]
    e_shstrndx = struct.unpack(endian + 'H', elf_data[50:52])[0]

    shstr_hdr_offset = e_shoff + e_shstrndx * e_shentsize
    shstr_offset = struct.unpack(endian + 'I', elf_data[shstr_hdr_offset + 16:shstr_hdr_offset + 20])[0]
    shstr_size = struct.unpack(endian + 'I', elf_data[shstr_hdr_offset + 20:shstr_hdr_offset + 24])[0]
    shstrtab = elf_data[shstr_offset:shstr_offset + shstr_size]

    text_offset = None
    text_size = None

    for i in range(e_shnum):
        sh_offset = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack(endian + 'I', elf_data[sh_offset:sh_offset + 4])[0]
        name_end = shstrtab.find(b'\x00', sh_name_idx)
        if name_end == -1:
            name_end = len(shstrtab)
        name = shstrtab[sh_name_idx:name_end].decode('ascii', errors='ignore')

        if name == '.text':
            text_offset = struct.unpack(endian + 'I', elf_data[sh_offset + 16:sh_offset + 20])[0]
            text_size = struct.unpack(endian + 'I', elf_data[sh_offset + 20:sh_offset + 24])[0]
            break

    if text_offset is None:
        raise ValueError("No .text section found")

    if verbose:
        print(f".text section: offset=0x{text_offset:x}, size={text_size}")

    count = 0
    for i in range(0, text_size, 4):
        word_offset = text_offset + i
        word = struct.unpack(endian + 'I', patched[word_offset:word_offset + 4])[0]
        word_idx = i // 4

        if offsets is not None:
            if word_idx not in offsets:
                continue
            if not is_ori_with_same_reg(word):
                if verbose:
                    print(f"  Warning: offset {word_idx} is not ori with rs==rt")
                continue
        else:
            if not is_ori_with_same_reg(word):
                continue

        new_word = convert_ori_to_addiu(word)
        patched[word_offset:word_offset + 4] = struct.pack(endian + 'I', new_word)
        count += 1

        if verbose:
            print(f"  Patched offset {word_idx}: 0x{word:08x} -> 0x{new_word:08x}")

    if verbose:
        print(f"Patched {count} instruction(s)")

    return bytes(patched)


def main():
    parser = argparse.ArgumentParser(description='Convert ORI to ADDIU in ELF objects')
    parser.add_argument('input', help='Input ELF file')
    parser.add_argument('output', help='Output ELF file')
    parser.add_argument('--offsets', help='Comma-separated instruction offsets (0-based word indices)')
    parser.add_argument('-v', '--verbose', action='store_true')
    args = parser.parse_args()

    offsets = None
    if args.offsets:
        offsets = set(int(x.strip()) for x in args.offsets.split(','))

    with open(args.input, 'rb') as f:
        data = f.read()

    patched = patch_ori_to_addiu(data, verbose=args.verbose, offsets=offsets)

    with open(args.output, 'wb') as f:
        f.write(patched)


if __name__ == '__main__':
    main()
