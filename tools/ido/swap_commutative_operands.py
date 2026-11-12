#!/usr/bin/env python3
"""
Swap rs and rt operands of commutative R-type instructions at specific offsets
in a MIPS ELF .o file's .text section.

For commutative instructions (addu, add, and, or, xor, nor), the result is
identical regardless of operand order: addu rd,rs,rt == addu rd,rt,rs.
IDO sometimes generates the operands in a different order than the target binary.

MIPS R-type instruction format (32 bits):
    bits 31-26: opcode (000000 for SPECIAL)
    bits 25-21: rs
    bits 20-16: rt
    bits 15-11: rd
    bits 10-6:  sa
    bits 5-0:   function

Commutative SPECIAL functions:
    0x20 = add, 0x21 = addu, 0x24 = and, 0x25 = or, 0x26 = xor, 0x27 = nor

Usage:
    python3 swap_commutative_operands.py input.o output.o --offset 0x54
    python3 swap_commutative_operands.py input.o output.o --offset 0x54 --offset 0x68
"""

import sys
import struct
import argparse

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

COMMUTATIVE_FUNCTS = {
    0x20: 'add',
    0x21: 'addu',
    0x24: 'and',
    0x25: 'or',
    0x26: 'xor',
    0x27: 'nor',
}


def is_commutative_rtype(word):
    """Check if instruction is a commutative R-type.

    Returns (name, rs, rt, rd) or None.
    """
    op = (word >> 26) & 0x3f
    funct = word & 0x3f
    if op == 0 and funct in COMMUTATIVE_FUNCTS:
        rs = (word >> 21) & 0x1f
        rt = (word >> 16) & 0x1f
        rd = (word >> 11) & 0x1f
        return (COMMUTATIVE_FUNCTS[funct], rs, rt, rd)
    return None


def swap_rs_rt(word):
    """Swap rs and rt fields in an R-type instruction."""
    rs = (word >> 21) & 0x1f
    rt = (word >> 16) & 0x1f
    word = word & ~(0x1f << 21) & ~(0x1f << 16)
    word = word | (rt << 21) | (rs << 16)
    return word


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
        sh_offset = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack(endian + 'I',
                                    elf_data[sh_offset:sh_offset+4])[0]
        sh_offset_data = struct.unpack(endian + 'I',
                                       elf_data[sh_offset+16:sh_offset+20])[0]
        sh_size = struct.unpack(endian + 'I',
                                elf_data[sh_offset+20:sh_offset+24])[0]

        name_end = shstrtab.find(b'\x00', sh_name_idx)
        name = shstrtab[sh_name_idx:name_end].decode('ascii')

        if name == '.text':
            return sh_offset_data, sh_size, endian

    raise ValueError(".text section not found")


def swap_commutative_operands(elf_data, offsets, verbose=False):
    """Swap rs/rt operands at specified offsets in .text section.

    Returns (patched_data, count_of_swaps).
    """
    patched = bytearray(elf_data)
    text_offset, text_size, endian = find_text_section(elf_data)

    if verbose:
        print(f".text at file offset 0x{text_offset:x}, size 0x{text_size:x}")

    changes = 0
    for off in offsets:
        if off >= text_size:
            print(f"Warning: offset 0x{off:x} is beyond .text size 0x{text_size:x}",
                  file=sys.stderr)
            continue
        if off % 4 != 0:
            print(f"Warning: offset 0x{off:x} is not 4-byte aligned",
                  file=sys.stderr)
            continue

        file_off = text_offset + off
        word = struct.unpack(endian + 'I', elf_data[file_off:file_off+4])[0]

        info = is_commutative_rtype(word)
        if info is None:
            print(f"Warning: instruction at 0x{off:x} (0x{word:08x}) "
                  f"is not a commutative R-type", file=sys.stderr)
            continue

        name, rs, rt, rd = info
        new_word = swap_rs_rt(word)
        struct.pack_into(endian + 'I', patched, file_off, new_word)
        changes += 1

        if verbose:
            rs_name = REG_NAMES.get(rs, str(rs))
            rt_name = REG_NAMES.get(rt, str(rt))
            rd_name = REG_NAMES.get(rd, str(rd))
            print(f"  0x{off:04x}: {name} ${rd_name},${rs_name},${rt_name}"
                  f" -> {name} ${rd_name},${rt_name},${rs_name}"
                  f" (0x{word:08x} -> 0x{new_word:08x})")

    return bytes(patched), changes


def main():
    parser = argparse.ArgumentParser(
        description='Swap rs/rt operands of commutative R-type instructions '
                    'at specific offsets in ELF .text section'
    )
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('--offset', type=lambda x: int(x, 0), action='append',
                        required=True,
                        help='Offset in .text to swap (hex, e.g. 0x54)')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Print changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, count = swap_commutative_operands(elf_data, args.offset,
                                               args.verbose)

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Swapped operands at {count} offset(s)")


if __name__ == '__main__':
    main()
