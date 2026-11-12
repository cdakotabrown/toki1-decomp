#!/usr/bin/env python3
"""
Strip interior NOPs from .text section.

Removes NOP instructions that appear between non-NOP instructions
(not delay slot NOPs at the end after jr ra).

This tool:
1. Finds NOPs that are not delay slots
2. Removes them from .text
3. Adjusts all relocations that point after removed NOPs
4. Adjusts symbol values and sizes
"""

import sys
import struct
import argparse

def is_branch_or_jump(word):
    """Check if instruction is a branch or jump (has delay slot)."""
    op = (word >> 26) & 0x3f
    # J-type: j (2), jal (3)
    if op in (2, 3):
        return True
    # I-type branches: beq (4), bne (5), blez (6), bgtz (7)
    if op in (4, 5, 6, 7):
        return True
    # REGIMM: bltz, bgez, etc. (1)
    if op == 1:
        return True
    # R-type: jr (8), jalr (9)
    if op == 0:
        funct = word & 0x3f
        if funct in (8, 9):
            return True
    return False

def strip_interior_nops(elf_data, verbose=False):
    """Strip interior NOPs from .text section."""
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

    # Find .text, .rel.text, and .symtab sections
    text_offset = None
    text_size = None
    text_section_idx = None
    rel_text_offset = None
    rel_text_size = None
    symtab_offset = None
    symtab_size = None

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
            text_section_idx = i
            text_section_header_off = sh_offset
        elif name == '.rel.text':
            rel_text_offset = sh_offset_data
            rel_text_size = sh_size
        elif name == '.symtab':
            symtab_offset = sh_offset_data
            symtab_size = sh_size

    if text_offset is None:
        raise ValueError(".text section not found")

    if verbose:
        print(f".text at offset 0x{text_offset:x}, size 0x{text_size:x}")

    # Read all instructions and identify interior NOPs
    instructions = []
    for i in range(0, text_size, 4):
        file_off = text_offset + i
        word = struct.unpack(endian + 'I', elf_data[file_off:file_off+4])[0]
        instructions.append((i, word))

    # Find interior NOPs (NOPs not in delay slots of branches/jumps)
    interior_nops = []
    for idx, (off, word) in enumerate(instructions):
        if word != 0:  # Not a NOP
            continue

        # Check if this is a delay slot
        if idx > 0:
            prev_word = instructions[idx-1][1]
            if is_branch_or_jump(prev_word):
                continue  # This is a delay slot NOP, keep it

        # This is an interior NOP
        interior_nops.append(off)

    if verbose:
        print(f"Found {len(interior_nops)} interior NOPs at offsets: {[hex(o) for o in interior_nops]}")

    if not interior_nops:
        return bytes(patched), 0

    # Build a mapping of old offset -> new offset after NOP removal
    # We'll remove NOPs and shift code

    # For simplicity, we'll work with a new text section
    new_text = bytearray()
    offset_map = {}  # old_offset -> new_offset

    current_new_off = 0
    for old_off, word in instructions:
        if old_off in interior_nops:
            # Skip this NOP
            continue
        offset_map[old_off] = current_new_off
        new_text.extend(struct.pack(endian + 'I', word))
        current_new_off += 4

    removed_count = len(interior_nops)
    removed_bytes = removed_count * 4
    new_text_size = len(new_text)

    if verbose:
        print(f"New .text size: 0x{new_text_size:x} (removed {removed_bytes} bytes)")

    # Replace text section in patched
    # For simplicity, pad to original size
    padded_text = new_text + b'\x00' * (text_size - new_text_size)
    patched[text_offset:text_offset+text_size] = padded_text

    # Update .text section size in section header
    struct.pack_into(endian + 'I', patched, text_section_header_off + 20, new_text_size)

    # Adjust relocations in .rel.text
    if rel_text_offset and rel_text_size:
        num_relocs = rel_text_size // 8
        for i in range(num_relocs):
            rel_off = rel_text_offset + i * 8
            r_offset = struct.unpack(endian + 'I', patched[rel_off:rel_off+4])[0]

            # Find how many NOPs were removed before this offset
            adjustment = sum(4 for nop_off in interior_nops if nop_off < r_offset)
            new_r_offset = r_offset - adjustment

            if verbose and adjustment > 0:
                print(f"  Reloc at 0x{r_offset:x} -> 0x{new_r_offset:x}")

            struct.pack_into(endian + 'I', patched, rel_off, new_r_offset)

    # Adjust symbols in .symtab
    if symtab_offset and symtab_size:
        num_syms = symtab_size // 16
        for i in range(num_syms):
            sym_off = symtab_offset + i * 16
            st_value = struct.unpack(endian + 'I', patched[sym_off+4:sym_off+8])[0]
            st_size = struct.unpack(endian + 'I', patched[sym_off+8:sym_off+12])[0]
            st_shndx = struct.unpack(endian + 'H', patched[sym_off+14:sym_off+16])[0]

            if st_shndx != text_section_idx:
                continue

            # Find how many NOPs were removed before this symbol
            adjustment = sum(4 for nop_off in interior_nops if nop_off < st_value)
            new_value = st_value - adjustment

            # Adjust size if NOPs removed within symbol
            removed_within = sum(4 for nop_off in interior_nops
                                if st_value <= nop_off < st_value + st_size)
            new_size = st_size - removed_within

            struct.pack_into(endian + 'I', patched, sym_off+4, new_value)
            struct.pack_into(endian + 'I', patched, sym_off+8, new_size)

    return bytes(patched), removed_count

def main():
    parser = argparse.ArgumentParser(
        description='Strip interior NOPs from .text section'
    )
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('-v', '--verbose', action='store_true', help='Print changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, count = strip_interior_nops(elf_data, args.verbose)

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Removed {count} interior NOPs ({count * 4} bytes)")

if __name__ == '__main__':
    main()
