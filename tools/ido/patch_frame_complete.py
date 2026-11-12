#!/usr/bin/env python3
"""
Completely patch stack frame size in an ELF object file.

Changes:
  - Prologue: addiu sp, sp, -old  ->  addiu sp, sp, -new
  - Epilogue: addiu sp, sp, old   ->  addiu sp, sp, new
  - Arg saves/loads: sw/lw reg, old(sp)  ->  sw/lw reg, new(sp)

This handles the common GCC-region pattern where:
  - Target has 40-byte frame
  - IDO compiles to 24-byte frame
  - Arguments are saved/loaded at frame_size(sp)
"""

import sys
import struct
import argparse

def patch_frame_complete(elf_data, old_size, new_size, verbose=False):
    """Patch all frame-related instructions."""
    patched = bytearray(elf_data)

    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

    # Parse ELF header to find .text section
    e_shoff = struct.unpack(endian + 'I', elf_data[32:36])[0]
    e_shentsize = struct.unpack(endian + 'H', elf_data[46:48])[0]
    e_shnum = struct.unpack(endian + 'H', elf_data[48:50])[0]
    e_shstrndx = struct.unpack(endian + 'H', elf_data[50:52])[0]

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

    changes = 0

    for i in range(0, text_size, 4):
        file_off = text_offset + i
        word = struct.unpack(endian + 'I', elf_data[file_off:file_off+4])[0]

        opcode = (word >> 26) & 0x3F
        rs = (word >> 21) & 0x1F
        rt = (word >> 16) & 0x1F
        imm = word & 0xFFFF

        # Sign-extend immediate
        if imm & 0x8000:
            imm_signed = imm - 0x10000
        else:
            imm_signed = imm

        new_word = None
        reason = None

        # addiu (opcode 9) with sp as both rs and rt
        if opcode == 9 and rs == 29 and rt == 29:
            if imm_signed == -old_size:
                # Prologue
                new_imm = (-new_size) & 0xFFFF
                new_word = (word & 0xFFFF0000) | new_imm
                reason = "prologue"
            elif imm_signed == old_size:
                # Epilogue
                new_word = (word & 0xFFFF0000) | (new_size & 0xFFFF)
                reason = "epilogue"

        # sw/lw with sp (rs=29) and offset = old_size
        # sw: opcode = 43 (0x2b)
        # lw: opcode = 35 (0x23)
        if opcode in (35, 43) and rs == 29 and imm_signed == old_size:
            new_word = (word & 0xFFFF0000) | (new_size & 0xFFFF)
            reason = f"{'lw' if opcode == 35 else 'sw'} at offset {old_size}"

        if new_word is not None:
            struct.pack_into(endian + 'I', patched, file_off, new_word)
            changes += 1
            if verbose:
                print(f"  0x{i:04x}: {reason} ({word:08x} -> {new_word:08x})")

    return bytes(patched), changes

def main():
    parser = argparse.ArgumentParser(description='Complete stack frame size patching')
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('--old-size', type=int, default=24, help='Old frame size (default: 24)')
    parser.add_argument('--new-size', type=int, default=40, help='New frame size (default: 40)')
    parser.add_argument('-v', '--verbose', action='store_true', help='Print changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, count = patch_frame_complete(elf_data, args.old_size, args.new_size, args.verbose)

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Patched {count} instructions")

if __name__ == '__main__':
    main()
