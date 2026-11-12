#!/usr/bin/env python3
"""
Strip epilogue padding code from IDO-compiled functions.

This tool removes instructions between the jal delay slot and the epilogue
(lw ra, N(sp)), specifically targeting patterns like:
    lw    tX, N(sp)     # Reload saved value
    addiu tY, sp, M     # Calculate address
    sw    tX, 0(tY)     # Store to pad

These patterns are generated when using volatile padding to force frame sizes
but aren't needed for matching the original binary.
"""

import sys
import struct
import argparse

def decode_instruction(word):
    """Decode basic MIPS instruction fields."""
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    imm = word & 0xFFFF
    if imm & 0x8000:
        imm = imm - 0x10000
    return op, rs, rt, rd, imm

def is_lw_sp(word):
    """Check if instruction is lw rt, offset(sp)"""
    op, rs, rt, rd, imm = decode_instruction(word)
    return op == 0x23 and rs == 29  # lw with $sp base

def is_sw(word):
    """Check if instruction is sw rt, offset(rs)"""
    op = (word >> 26) & 0x3F
    return op == 0x2B  # sw

def is_addiu(word):
    """Check if instruction is addiu"""
    op = (word >> 26) & 0x3F
    return op == 0x09  # addiu

def is_lui(word):
    """Check if instruction is lui"""
    op = (word >> 26) & 0x3F
    return op == 0x0F  # lui

def is_lw_ra_sp(word):
    """Check if instruction is lw ra, offset(sp)"""
    op, rs, rt, rd, imm = decode_instruction(word)
    return op == 0x23 and rs == 29 and rt == 31  # lw $ra, N($sp)

def is_jal(word):
    """Check if instruction is jal"""
    op = (word >> 26) & 0x3F
    return op == 0x03  # jal

def find_text_section(elf_data):
    """Find .text section in ELF file."""
    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    e_shoff = struct.unpack('>I', elf_data[32:36])[0]
    e_shentsize = struct.unpack('>H', elf_data[46:48])[0]
    e_shnum = struct.unpack('>H', elf_data[48:50])[0]
    e_shstrndx = struct.unpack('>H', elf_data[50:52])[0]

    shstrtab_off = e_shoff + e_shstrndx * e_shentsize
    shstrtab_addr = struct.unpack('>I', elf_data[shstrtab_off+16:shstrtab_off+20])[0]
    shstrtab_size = struct.unpack('>I', elf_data[shstrtab_off+20:shstrtab_off+24])[0]
    shstrtab = elf_data[shstrtab_addr:shstrtab_addr+shstrtab_size]

    for i in range(e_shnum):
        sh_offset = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack('>I', elf_data[sh_offset:sh_offset+4])[0]
        sh_offset_data = struct.unpack('>I', elf_data[sh_offset+16:sh_offset+20])[0]
        sh_size = struct.unpack('>I', elf_data[sh_offset+20:sh_offset+24])[0]

        name_end = shstrtab.find(b'\x00', sh_name_idx)
        name = shstrtab[sh_name_idx:name_end].decode('ascii')

        if name == '.text':
            return sh_offset_data, sh_size

    raise ValueError(".text section not found")

def strip_epilogue_padding(elf_data, verbose=False, compact=False):
    """
    Find and NOP out epilogue padding instructions.

    Target pattern after jal delay slot:
        lw    tX, N(sp)     # Reload (not ra)
        addiu tY, sp, M     # Calculate pad address
        sw    tX, 0(tY)     # Store to pad

    Keep:
        lw    ra, N(sp)     # Restore return address
    """
    patched = bytearray(elf_data)
    text_offset, text_size = find_text_section(elf_data)

    changes = []
    nop_ranges = []  # Track NOP ranges for compaction
    NOP = 0x00000000

    # Find jal instructions, then look for epilogue padding after them
    i = 0
    while i < text_size - 24:  # Need at least 6 instructions to check
        file_off = text_offset + i
        word = struct.unpack('>I', elf_data[file_off:file_off+4])[0]

        if is_jal(word):
            # Skip delay slot (i+4), then look for pattern
            # Pattern: lw tX, N(sp) / addiu tY, sp, M / sw tX, 0(tY) / lw ra, N(sp)

            # Start checking after delay slot
            check_off = file_off + 8  # After jal + delay slot

            # Read next 4 instructions
            instrs = []
            for j in range(4):
                if check_off + j*4 + 4 <= text_offset + text_size:
                    w = struct.unpack('>I', elf_data[check_off + j*4:check_off + j*4 + 4])[0]
                    instrs.append(w)

            if len(instrs) >= 4:
                # Check for pattern: lw(not ra) / addiu / sw / lw ra
                # or: lui / lw / addiu sp / jr ra

                # Pattern 1: lw tX / addiu tY,sp / sw tX / lw ra
                if (is_lw_sp(instrs[0]) and not is_lw_ra_sp(instrs[0]) and
                    is_addiu(instrs[1]) and
                    is_sw(instrs[2]) and
                    is_lw_ra_sp(instrs[3])):

                    # NOP out the first 3 instructions
                    for j in range(3):
                        struct.pack_into('>I', patched, check_off + j*4, NOP)
                        changes.append((i + 8 + j*4, f"NOP'd instruction at +{8 + j*4}"))
                    nop_ranges.append((check_off, 3))

                # Pattern 2: addiu tX,sp / lw tY / addiu tZ,sp / sw ... (for global store)
                elif (is_addiu(instrs[0]) and
                      is_lw_sp(instrs[1]) and not is_lw_ra_sp(instrs[1])):

                    # Check if this is followed by lw ra somewhere
                    # NOP first 2-3 epilogue instructions before lw ra
                    nop_count = 0
                    for j in range(min(5, len(instrs))):
                        if is_lw_ra_sp(instrs[j]):
                            break
                        if not is_lw_ra_sp(instrs[j]):
                            struct.pack_into('>I', patched, check_off + j*4, NOP)
                            nop_count += 1
                    if nop_count > 0:
                        changes.append((i + 8, f"NOP'd {nop_count} epilogue instructions"))
                        nop_ranges.append((check_off, nop_count))

        i += 4

    if verbose:
        for offset, desc in changes:
            print(f"  0x{offset:04x}: {desc}")

    # Compact: shift code to remove NOP gaps
    if compact and nop_ranges:
        if verbose:
            print("  Compacting code...")
        for nop_start, nop_count in sorted(nop_ranges, reverse=True):
            nop_bytes = nop_count * 4
            # Shift everything after the NOPs up
            end_pos = text_offset + text_size
            patched[nop_start:end_pos - nop_bytes] = patched[nop_start + nop_bytes:end_pos]
            # Fill the end with NOPs
            for j in range(nop_count):
                struct.pack_into('>I', patched, end_pos - nop_bytes + j*4, NOP)

    return bytes(patched), len(changes)

def main():
    parser = argparse.ArgumentParser(description='Strip epilogue padding from IDO output')
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('-v', '--verbose', action='store_true', help='Print changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, count = strip_epilogue_padding(elf_data, args.verbose)

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Stripped {count} epilogue padding patterns")

if __name__ == '__main__':
    main()
