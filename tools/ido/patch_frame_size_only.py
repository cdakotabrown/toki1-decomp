#!/usr/bin/env python3
"""
Simple frame size patching for ELF object files.

Unlike patch_frame_full.py, this ONLY patches the stack frame allocation
and deallocation instructions without modifying RA or s-reg offsets.

Use this when:
- Compiled code uses smaller frame than target
- RA offset is the same in both (e.g., both at offset 28)
- Only the stack frame size differs

Usage:
    python3 patch_frame_size_only.py input.o output.o --old 32 --new 48
"""

import sys
import struct
import argparse


def patch_frame_size(input_path, output_path, old_size, new_size, verbose=False):
    """Patch stack frame size without changing other offsets."""

    with open(input_path, 'rb') as f:
        data = bytearray(f.read())

    # Calculate expected instruction encodings
    # addiu sp,sp,-N (prologue): 27bd + 16-bit signed immediate
    old_neg = (-old_size) & 0xFFFF
    new_neg = (-new_size) & 0xFFFF
    prologue_old = 0x27bd0000 | old_neg
    prologue_new = 0x27bd0000 | new_neg

    # addiu sp,sp,+N (epilogue): same opcode, positive value
    epilogue_old = 0x27bd0000 | old_size
    epilogue_new = 0x27bd0000 | new_size

    if verbose:
        print(f"Frame size: {old_size} -> {new_size}")
        print(f"Prologue: 0x{prologue_old:08x} -> 0x{prologue_new:08x}")
        print(f"Epilogue: 0x{epilogue_old:08x} -> 0x{epilogue_new:08x}")

    # Find and patch prologue (little-endian)
    prologue_bytes_old = struct.pack('<I', prologue_old)
    prologue_offset = data.find(prologue_bytes_old)

    if prologue_offset == -1:
        print(f"Error: Could not find prologue addiu sp,sp,-{old_size}")
        return False

    if verbose:
        print(f"Found prologue at offset 0x{prologue_offset:x}")

    data[prologue_offset:prologue_offset+4] = struct.pack('<I', prologue_new)

    # Find and patch epilogue
    epilogue_bytes_old = struct.pack('<I', epilogue_old)
    epilogue_offset = data.find(epilogue_bytes_old, prologue_offset)

    if epilogue_offset == -1:
        print(f"Warning: Could not find epilogue addiu sp,sp,{old_size}")
    else:
        if verbose:
            print(f"Found epilogue at offset 0x{epilogue_offset:x}")
        data[epilogue_offset:epilogue_offset+4] = struct.pack('<I', epilogue_new)

    with open(output_path, 'wb') as f:
        f.write(data)

    print(f"Patched frame size: {old_size} -> {new_size}")
    return True


def main():
    parser = argparse.ArgumentParser(
        description='Patch stack frame size in ELF object file (frame size only, no offset changes)'
    )
    parser.add_argument('input', help='Input object file')
    parser.add_argument('output', help='Output object file')
    parser.add_argument('--old', type=int, required=True, help='Old frame size')
    parser.add_argument('--new', type=int, required=True, help='New frame size')
    parser.add_argument('-v', '--verbose', action='store_true', help='Verbose output')

    args = parser.parse_args()

    success = patch_frame_size(args.input, args.output, args.old, args.new, args.verbose)
    sys.exit(0 if success else 1)


if __name__ == '__main__':
    main()
