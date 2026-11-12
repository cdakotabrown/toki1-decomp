#!/usr/bin/env python3
"""
Patch the return address (ra) save/restore offset in a MIPS function.

This fixes cases where IDO generates:
    sw ra,20(sp)
    lw ra,20(sp)

But the target has:
    sw ra,16(sp)
    lw ra,16(sp)

Usage:
    python3 patch_ra_offset.py input.o output.o --old 20 --new 16
"""

import argparse
import struct
import sys
from pathlib import Path


def patch_ra_offset(input_path, output_path, old_offset, new_offset):
    """Patch ra save/restore offset in .text section."""

    # Read the entire object file
    with open(input_path, 'rb') as f:
        data = bytearray(f.read())

    # Find .text section in ELF
    if data[:4] != b'\x7fELF':
        print(f"Error: Not a valid ELF file", file=sys.stderr)
        return False

    # Parse ELF header (little-endian, 32-bit)
    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shentsize = struct.unpack('<H', data[46:48])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]

    # Read string table
    shstr_hdr_offset = e_shoff + (e_shstrndx * e_shentsize)
    shstr_offset = struct.unpack('<I', data[shstr_hdr_offset + 16:shstr_hdr_offset + 20])[0]

    # Find .text section
    text_offset = None
    text_size = None

    for i in range(e_shnum):
        sh_offset = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack('<I', data[sh_offset:sh_offset + 4])[0]
        sh_offset_val = struct.unpack('<I', data[sh_offset + 16:sh_offset + 20])[0]
        sh_size = struct.unpack('<I', data[sh_offset + 20:sh_offset + 24])[0]

        # Get section name
        name_start = shstr_offset + sh_name_idx
        name_end = data.find(b'\x00', name_start)
        section_name = data[name_start:name_end].decode('ascii')

        if section_name == '.text':
            text_offset = sh_offset_val
            text_size = sh_size
            break

    if text_offset is None:
        print(f"Error: .text section not found", file=sys.stderr)
        return False

    # Scan for ra save/restore instructions
    # sw ra,offset(sp) = 0xAFBF0000 | (offset & 0xFFFF)
    # lw ra,offset(sp) = 0x8FBF0000 | (offset & 0xFFFF)

    sw_ra_old = 0xAFBF0000 | (old_offset & 0xFFFF)
    lw_ra_old = 0x8FBF0000 | (old_offset & 0xFFFF)
    sw_ra_new = 0xAFBF0000 | (new_offset & 0xFFFF)
    lw_ra_new = 0x8FBF0000 | (new_offset & 0xFFFF)

    patch_count = 0

    for i in range(0, text_size, 4):
        file_offset = text_offset + i
        instr = struct.unpack('<I', data[file_offset:file_offset + 4])[0]

        if instr == sw_ra_old:
            data[file_offset:file_offset + 4] = struct.pack('<I', sw_ra_new)
            patch_count += 1
            print(f"  Patched sw ra at +0x{i:x}: {old_offset} -> {new_offset}")
        elif instr == lw_ra_old:
            data[file_offset:file_offset + 4] = struct.pack('<I', lw_ra_new)
            patch_count += 1
            print(f"  Patched lw ra at +0x{i:x}: {old_offset} -> {new_offset}")

    if patch_count == 0:
        print(f"Warning: No ra save/restore instructions found with offset {old_offset}")
        return False

    # Write output
    with open(output_path, 'wb') as f:
        f.write(data)

    print(f"Successfully patched {patch_count} instructions")
    return True


def main():
    parser = argparse.ArgumentParser(description='Patch ra save/restore offset')
    parser.add_argument('input', type=str, help='Input object file')
    parser.add_argument('output', type=str, help='Output object file')
    parser.add_argument('--old', type=int, required=True, help='Old offset')
    parser.add_argument('--new', type=int, required=True, help='New offset')

    args = parser.parse_args()

    success = patch_ra_offset(args.input, args.output, args.old, args.new)
    sys.exit(0 if success else 1)


if __name__ == '__main__':
    main()
