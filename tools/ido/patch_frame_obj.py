#!/usr/bin/env python3
"""
Patch MIPS ELF object files to adjust stack frame sizes.

This modifies the .text section in-place to change:
- addiu $sp, $sp, -N  to  addiu $sp, $sp, -(N+delta)
- addiu $sp, $sp, N   to  addiu $sp, $sp, (N+delta)

Frame delta is added to all frame allocations while keeping RA offset unchanged.
"""

import sys
import struct
import argparse

def decode_addiu(word):
    """Decode MIPS addiu instruction."""
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    imm = word & 0xFFFF
    # Sign-extend immediate
    if imm & 0x8000:
        imm = imm - 0x10000
    return op, rs, rt, imm

def encode_addiu(rs, rt, imm):
    """Encode MIPS addiu instruction."""
    # Handle negative immediates
    if imm < 0:
        imm = imm & 0xFFFF
    op = 0x09  # addiu opcode
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)

def patch_text_section(data, delta=16):
    """Patch all stack frame operations in binary data."""
    patched = bytearray(data)
    changes = []

    for i in range(0, len(data) - 3, 4):
        word = struct.unpack('<I', data[i:i+4])[0]
        op, rs, rt, imm = decode_addiu(word)

        # Check for addiu $sp, $sp, imm (opcode=9, rs=29, rt=29)
        if op == 0x09 and rs == 29 and rt == 29:
            if imm < 0:
                # Prologue: addiu $sp, $sp, -N
                new_imm = imm - delta
                new_word = encode_addiu(rs, rt, new_imm)
                struct.pack_into('<I', patched, i, new_word)
                changes.append((i, f"addiu $sp,$sp,{imm} -> addiu $sp,$sp,{new_imm}"))
            elif imm > 0:
                # Epilogue: addiu $sp, $sp, N
                new_imm = imm + delta
                new_word = encode_addiu(rs, rt, new_imm)
                struct.pack_into('<I', patched, i, new_word)
                changes.append((i, f"addiu $sp,$sp,{imm} -> addiu $sp,$sp,{new_imm}"))

    return bytes(patched), changes

def find_text_section(elf_data):
    """Find .text section in ELF file."""
    # Parse ELF header
    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    e_shoff = struct.unpack('<I', elf_data[32:36])[0]
    e_shentsize = struct.unpack('<H', elf_data[46:48])[0]
    e_shnum = struct.unpack('<H', elf_data[48:50])[0]
    e_shstrndx = struct.unpack('<H', elf_data[50:52])[0]

    # Get section header string table
    shstrtab_off = e_shoff + e_shstrndx * e_shentsize
    shstrtab_addr = struct.unpack('<I', elf_data[shstrtab_off+16:shstrtab_off+20])[0]
    shstrtab_size = struct.unpack('<I', elf_data[shstrtab_off+20:shstrtab_off+24])[0]
    shstrtab = elf_data[shstrtab_addr:shstrtab_addr+shstrtab_size]

    # Find .text section
    for i in range(e_shnum):
        sh_offset = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack('<I', elf_data[sh_offset:sh_offset+4])[0]
        sh_offset_data = struct.unpack('<I', elf_data[sh_offset+16:sh_offset+20])[0]
        sh_size = struct.unpack('<I', elf_data[sh_offset+20:sh_offset+24])[0]

        # Get section name
        name_end = shstrtab.find(b'\x00', sh_name_idx)
        name = shstrtab[sh_name_idx:name_end].decode('ascii')

        if name == '.text':
            return sh_offset_data, sh_size

    raise ValueError(".text section not found")

def patch_elf(input_path, output_path, delta=16, verbose=False):
    """Patch an ELF object file."""
    with open(input_path, 'rb') as f:
        elf_data = bytearray(f.read())

    text_offset, text_size = find_text_section(elf_data)
    text_data = bytes(elf_data[text_offset:text_offset+text_size])

    patched_text, changes = patch_text_section(text_data, delta)

    elf_data[text_offset:text_offset+text_size] = patched_text

    with open(output_path, 'wb') as f:
        f.write(elf_data)

    if verbose:
        print(f"Patched {len(changes)} instructions:")
        for offset, change in changes:
            print(f"  0x{text_offset + offset:04x}: {change}")

    return len(changes)

def main():
    parser = argparse.ArgumentParser(description='Patch MIPS ELF frame sizes')
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('-d', '--delta', type=int, default=16,
                       help='Frame size delta to add (default: 16)')
    parser.add_argument('-v', '--verbose', action='store_true',
                       help='Print changes')

    args = parser.parse_args()
    count = patch_elf(args.input, args.output, args.delta, args.verbose)
    print(f"Patched {count} frame instructions")

if __name__ == '__main__':
    main()
