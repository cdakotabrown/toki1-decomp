#!/usr/bin/env python3
"""
Fix the ANDI pattern to use in-place register.

Converts:
  andi $t6, $a0, 0xffff
  move $a0, $t6

To:
  andi $a0, $a0, 0xffff
  nop (or remove the move entirely)

This is needed when IDO 5.2 generates andi with a temp register
but the target binary does it in-place.
"""

import sys
import struct
import argparse

def patch_andi_pattern(elf_data, verbose=False):
    """Patch ANDI pattern to use in-place register."""
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

    # Find all ANDI and MOVE instructions
    andi_list = []  # List of (offset, word, rs, rt, imm) for ANDI instructions
    move_list = []  # List of (offset, word, rs, rd) for MOVE instructions (or $rd, $rs, $0)

    for i in range(0, text_size, 4):
        file_off = text_offset + i
        if file_off + 4 > len(elf_data):
            break

        word = struct.unpack(endian + 'I', elf_data[file_off:file_off+4])[0]

        op = (word >> 26) & 0x3f
        rs = (word >> 21) & 0x1f
        rt = (word >> 16) & 0x1f
        rd = (word >> 11) & 0x1f
        shamt = (word >> 6) & 0x1f
        funct = word & 0x3f
        imm = word & 0xffff

        # ANDI: op=0xC (12), format: andi rt, rs, imm
        if op == 0xC:
            andi_list.append((i, file_off, word, rs, rt, imm))

        # MOVE as OR: op=0, rs=src, rt=0, rd=dst, funct=0x25
        # or $rd, $rs, $zero  (move $rd, $rs)
        if op == 0 and rt == 0 and shamt == 0 and funct == 0x25:
            move_list.append((i, file_off, word, rs, rd))

    # Match ANDI with MOVE where:
    # - ANDI: $temp = $src & imm  (rt=temp, rs=src)
    # - MOVE: $dst = $temp        (rd=dst, rs=temp)
    # - temp register is the same
    # - We want to convert to in-place: $src = $src & imm; nop

    for andi_offset, andi_file_off, andi_word, andi_rs, andi_rt, andi_imm in andi_list:
        # Only consider andi with temp register (not in-place already)
        if andi_rs == andi_rt:
            continue

        # Find matching MOVE that uses the temp from ANDI
        for move_offset, move_file_off, move_word, move_rs, move_rd in move_list:
            # MOVE must come after ANDI
            if move_offset <= andi_offset:
                continue

            # MOVE source should match ANDI dest (temp register)
            if move_rs != andi_rt:
                continue

            # MOVE dest should match ANDI source (the original register)
            if move_rd != andi_rs:
                continue

            # Found a match! Convert to in-place
            src = andi_rs

            # New ANDI: andi $src, $src, imm
            new_andi = (0xC << 26) | (src << 21) | (src << 16) | andi_imm

            # NOP for the move
            new_nop = 0x00000000

            struct.pack_into(endian + 'I', patched, andi_file_off, new_andi)
            struct.pack_into(endian + 'I', patched, move_file_off, new_nop)
            changes += 1

            if verbose:
                print(f"  0x{andi_offset:04x}: andi ${andi_rt},${andi_rs},0x{andi_imm:x} -> andi ${src},${src},0x{andi_imm:x}")
                print(f"  0x{move_offset:04x}: move ${move_rd},${move_rs} -> nop")

            # Only match once per ANDI
            break

    return bytes(patched), changes

def main():
    parser = argparse.ArgumentParser(
        description='Fix ANDI pattern to use in-place register'
    )
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('-v', '--verbose', action='store_true', help='Print changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, count = patch_andi_pattern(elf_data, args.verbose)

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Fixed {count} ANDI patterns")

if __name__ == '__main__':
    main()
