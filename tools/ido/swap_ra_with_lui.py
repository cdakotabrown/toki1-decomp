#!/usr/bin/env python3
"""
Swap the position of 'sw ra' with 'lui/ori' sequence to match
specific instruction scheduling patterns.

Transforms:
  addiu sp,sp,-N
  sw ra,OFFSET(sp)      <- move this down
  lui a0,HIGH
  ori a0,a0,LOW
  jal target
  ...

To:
  addiu sp,sp,-N
  lui a0,HIGH
  ori a0,a0,LOW
  sw ra,OFFSET(sp)      <- to here
  jal target
  ...

Also adjusts 'lw ra' position if needed.
"""

import sys
import struct
import argparse

def get_opcode(word):
    """Get the 6-bit opcode field."""
    return (word >> 26) & 0x3f

def is_addiu_sp(word):
    """Check if instruction is addiu sp,sp,X"""
    # addiu: opcode=9, rs=sp(29), rt=sp(29)
    op = get_opcode(word)
    rs = (word >> 21) & 0x1f
    rt = (word >> 16) & 0x1f
    return op == 9 and rs == 29 and rt == 29

def is_sw_ra_sp(word):
    """Check if instruction is sw ra,X(sp)"""
    # sw: opcode=0x2b, base=sp(29), rt=ra(31)
    op = get_opcode(word)
    base = (word >> 21) & 0x1f
    rt = (word >> 16) & 0x1f
    return op == 0x2b and base == 29 and rt == 31

def is_lw_ra_sp(word):
    """Check if instruction is lw ra,X(sp)"""
    # lw: opcode=0x23, base=sp(29), rt=ra(31)
    op = get_opcode(word)
    base = (word >> 21) & 0x1f
    rt = (word >> 16) & 0x1f
    return op == 0x23 and base == 29 and rt == 31

def is_lui(word):
    """Check if instruction is lui rt,imm"""
    return get_opcode(word) == 0xf

def is_ori(word):
    """Check if instruction is ori rt,rs,imm"""
    return get_opcode(word) == 0xd

def swap_ra_with_lui(elf_data, verbose=False):
    """Find and swap sw ra position with lui/ori sequence."""
    patched = bytearray(elf_data)

    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

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
    i = 0
    while i < text_size - 16:  # Need at least 4 instructions
        file_off = text_offset + i

        # Read 4 consecutive instructions
        w0 = struct.unpack(endian + 'I', elf_data[file_off:file_off+4])[0]
        w1 = struct.unpack(endian + 'I', elf_data[file_off+4:file_off+8])[0]
        w2 = struct.unpack(endian + 'I', elf_data[file_off+8:file_off+12])[0]
        w3 = struct.unpack(endian + 'I', elf_data[file_off+12:file_off+16])[0]

        # Look for pattern: addiu sp, sw ra, lui, ori
        if is_addiu_sp(w0) and is_sw_ra_sp(w1) and is_lui(w2) and is_ori(w3):
            if verbose:
                print(f"  Found pattern at offset 0x{i:04x}")
                print(f"    0x{i:04x}: {w0:08x} (addiu sp)")
                print(f"    0x{i+4:04x}: {w1:08x} (sw ra) <- will move to 0x{i+12:04x}")
                print(f"    0x{i+8:04x}: {w2:08x} (lui) <- will move to 0x{i+4:04x}")
                print(f"    0x{i+12:04x}: {w3:08x} (ori) <- will move to 0x{i+8:04x}")

            # Reorder: addiu, lui, ori, sw ra
            struct.pack_into(endian + 'I', patched, file_off+4, w2)   # lui moves to +4
            struct.pack_into(endian + 'I', patched, file_off+8, w3)   # ori moves to +8
            struct.pack_into(endian + 'I', patched, file_off+12, w1)  # sw ra moves to +12
            changes += 1
            i += 16  # Skip past the pattern we just modified
        else:
            i += 4

    return bytes(patched), changes

def main():
    parser = argparse.ArgumentParser(
        description='Swap sw ra position with lui/ori sequence in ELF files'
    )
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('-v', '--verbose', action='store_true', help='Print changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, count = swap_ra_with_lui(elf_data, args.verbose)

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Swapped {count} instruction sequences")

if __name__ == '__main__':
    main()
