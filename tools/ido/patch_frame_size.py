#!/usr/bin/env python3
"""
Patch stack frame size in an ELF object file.

Changes:
  addiu sp, sp, -24  ->  addiu sp, sp, -40
  addiu sp, sp, 24   ->  addiu sp, sp, 40
"""

import sys
import struct
import argparse

def patch_frame_size(elf_data, old_size, new_size, verbose=False):
    """Patch addiu sp,sp,N instructions to change frame size."""
    patched = bytearray(elf_data)

    # Check ELF and determine endianness
    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    # ELF header byte 5: 1=little endian, 2=big endian
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

    changes = 0

    # MIPS instructions are always big-endian regardless of data endianness
    # But we need to read them in the file's endianness
    # addiu sp, sp, -old_size  (prologue: subtract from sp)
    # Encoding: 001001 11101 11101 imm16  = 0x27bd????
    prologue_old = 0x27bd0000 | ((-old_size) & 0xFFFF)
    prologue_new = 0x27bd0000 | ((-new_size) & 0xFFFF)

    # addiu sp, sp, old_size  (epilogue: add to sp)
    epilogue_old = 0x27bd0000 | (old_size & 0xFFFF)
    epilogue_new = 0x27bd0000 | (new_size & 0xFFFF)

    if verbose:
        print(f"Looking for prologue: {prologue_old:08x} -> {prologue_new:08x}")
        print(f"Looking for epilogue: {epilogue_old:08x} -> {epilogue_new:08x}")
        print(f".text at offset 0x{text_offset:x}, size 0x{text_size:x}")

    for i in range(0, text_size, 4):
        file_off = text_offset + i
        word = struct.unpack(endian + 'I', elf_data[file_off:file_off+4])[0]

        if word == prologue_old:
            struct.pack_into(endian + 'I', patched, file_off, prologue_new)
            changes += 1
            if verbose:
                print(f"  Patched prologue at offset 0x{i:04x}")
        elif word == epilogue_old:
            struct.pack_into(endian + 'I', patched, file_off, epilogue_new)
            changes += 1
            if verbose:
                print(f"  Patched epilogue at offset 0x{i:04x}")

    return bytes(patched), changes

def main():
    parser = argparse.ArgumentParser(description='Patch stack frame size in ELF')
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('--old-size', type=int, default=24, help='Old frame size (default: 24)')
    parser.add_argument('--new-size', type=int, default=40, help='New frame size (default: 40)')
    parser.add_argument('-v', '--verbose', action='store_true', help='Print changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, count = patch_frame_size(elf_data, args.old_size, args.new_size, args.verbose)

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Patched {count} instructions")

if __name__ == '__main__':
    main()
