#!/usr/bin/env python3
"""
Strip unnecessary argument save/load pairs from IDO -O1 object files.

IDO -O1 generates patterns like:
    sw    a0,24(sp)    # Save argument
    lw    a0,24(sp)    # Load it right back (no-op)

This tool removes such pairs by NOP'ing them out.
"""

import sys
import struct
import argparse

def decode_instruction(word):
    """Decode basic MIPS instruction fields."""
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    imm = word & 0xFFFF
    if imm & 0x8000:
        imm = imm - 0x10000
    return op, rs, rt, imm

def is_sw_sp(word):
    """Check if instruction is sw rt, offset(sp)"""
    op, rs, rt, imm = decode_instruction(word)
    return op == 0x2B and rs == 29  # sw with $sp base

def is_lw_sp(word):
    """Check if instruction is lw rt, offset(sp)"""
    op, rs, rt, imm = decode_instruction(word)
    return op == 0x23 and rs == 29  # lw with $sp base

def find_text_section(elf_data):
    """Find .text section in ELF file."""
    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    e_shoff = struct.unpack('<I', elf_data[32:36])[0]
    e_shentsize = struct.unpack('<H', elf_data[46:48])[0]
    e_shnum = struct.unpack('<H', elf_data[48:50])[0]
    e_shstrndx = struct.unpack('<H', elf_data[50:52])[0]

    shstrtab_off = e_shoff + e_shstrndx * e_shentsize
    shstrtab_addr = struct.unpack('<I', elf_data[shstrtab_off+16:shstrtab_off+20])[0]
    shstrtab_size = struct.unpack('<I', elf_data[shstrtab_off+20:shstrtab_off+24])[0]
    shstrtab = elf_data[shstrtab_addr:shstrtab_addr+shstrtab_size]

    for i in range(e_shnum):
        sh_offset = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack('<I', elf_data[sh_offset:sh_offset+4])[0]
        sh_offset_data = struct.unpack('<I', elf_data[sh_offset+16:sh_offset+20])[0]
        sh_size = struct.unpack('<I', elf_data[sh_offset+20:sh_offset+24])[0]

        name_end = shstrtab.find(b'\x00', sh_name_idx)
        name = shstrtab[sh_name_idx:name_end].decode('ascii')

        if name == '.text':
            return sh_offset_data, sh_size

    raise ValueError(".text section not found")

def strip_arg_saves(elf_data, verbose=False):
    """Find and NOP out sw/lw pairs that save then immediately load the same register."""
    patched = bytearray(elf_data)
    text_offset, text_size = find_text_section(elf_data)

    changes = []
    NOP = 0x00000000

    i = 0
    while i < text_size - 4:
        file_off = text_offset + i
        word1 = struct.unpack('<I', elf_data[file_off:file_off+4])[0]
        word2 = struct.unpack('<I', elf_data[file_off+4:file_off+8])[0]

        # Check for sw followed by lw with same register and offset
        if is_sw_sp(word1) and is_lw_sp(word2):
            op1, rs1, rt1, imm1 = decode_instruction(word1)
            op2, rs2, rt2, imm2 = decode_instruction(word2)

            # Same register, same offset = no-op pair
            if rt1 == rt2 and imm1 == imm2:
                # Don't remove if it's RA save (that's intentional)
                if rt1 != 31:  # $ra = 31
                    struct.pack_into('<I', patched, file_off, NOP)
                    struct.pack_into('<I', patched, file_off+4, NOP)
                    reg_name = f"${rt1}" if rt1 < 32 else "?"
                    changes.append((i, f"Removed sw/lw pair for {reg_name} at offset {imm1}"))
                    i += 8
                    continue

        i += 4

    if verbose:
        for offset, desc in changes:
            print(f"  0x{offset:04x}: {desc}")

    return bytes(patched), len(changes)

def main():
    parser = argparse.ArgumentParser(description='Strip unnecessary sw/lw pairs from IDO output')
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('-v', '--verbose', action='store_true', help='Print changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, count = strip_arg_saves(elf_data, args.verbose)

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Stripped {count} argument save/load pairs")

if __name__ == '__main__':
    main()
