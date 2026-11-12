#!/usr/bin/env python3
"""
strip_and_patch.py - Strip leading instructions and patch register at offset

Custom post-processing for functions where cross-prologue hoisting produces
an extra lui instruction that needs to be removed.

Usage:
    python3 strip_and_patch.py input.o output.o --strip N --patch-reg OFFSET:OLD:NEW

    --strip N: Remove the first N instructions (N*4 bytes) from .text
    --patch-reg OFFSET:OLD_REG:NEW_REG: Patch rt field at byte OFFSET
"""

import argparse
import struct
import subprocess
import tempfile
import shutil
import os
import sys

REGS = {
    'zero': 0, 'at': 1, 'v0': 2, 'v1': 3,
    'a0': 4, 'a1': 5, 'a2': 6, 'a3': 7,
    't0': 8, 't1': 9, 't2': 10, 't3': 11,
    't4': 12, 't5': 13, 't6': 14, 't7': 15,
    's0': 16, 's1': 17, 's2': 18, 's3': 19,
    's4': 20, 's5': 21, 's6': 22, 's7': 23,
    't8': 24, 't9': 25, 'k0': 26, 'k1': 27,
    'gp': 28, 'sp': 29, 'fp': 30, 'ra': 31,
}


def main():
    parser = argparse.ArgumentParser(description='Strip and patch post-processor')
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output ELF object file')
    parser.add_argument('--strip', type=int, default=0,
                        help='Number of instructions to strip from start')
    parser.add_argument('--patch-reg', action='append', default=[],
                        help='OFFSET:OLD_REG:NEW_REG - patch rt field at byte offset')
    args = parser.parse_args()

    with tempfile.TemporaryDirectory() as tmpdir:
        bin_path = os.path.join(tmpdir, 'text.bin')

        # Extract .text section
        subprocess.run([
            'mipsel-linux-gnu-objcopy', '-O', 'binary', '-j', '.text',
            args.input, bin_path
        ], check=True)

        with open(bin_path, 'rb') as f:
            text = bytearray(f.read())

        # Strip leading instructions
        if args.strip > 0:
            strip_bytes = args.strip * 4
            if strip_bytes >= len(text):
                print(f"Error: cannot strip {args.strip} instructions from {len(text)} byte text",
                      file=sys.stderr)
                sys.exit(1)
            text = text[strip_bytes:]
            print(f"Stripped {args.strip} instruction(s) ({strip_bytes} bytes)")

        # Patch registers
        for patch_spec in args.patch_reg:
            parts = patch_spec.split(':')
            if len(parts) != 3:
                print(f"Error: --patch-reg format is OFFSET:OLD_REG:NEW_REG", file=sys.stderr)
                sys.exit(1)
            offset = int(parts[0], 0)
            old_reg = REGS.get(parts[1])
            if old_reg is None:
                old_reg = int(parts[1])
            new_reg = REGS.get(parts[2])
            if new_reg is None:
                new_reg = int(parts[2])

            if offset + 4 > len(text):
                print(f"Error: offset {offset} out of range", file=sys.stderr)
                sys.exit(1)

            word = struct.unpack('<I', text[offset:offset+4])[0]
            rt = (word >> 16) & 0x1f
            if rt != old_reg:
                print(f"Warning: rt at offset {offset} is {rt}, expected {old_reg}",
                      file=sys.stderr)
            word = (word & 0xFFE0FFFF) | (new_reg << 16)
            struct.pack_into('<I', text, offset, word)
            print(f"Patched rt at offset {offset}: {parts[1]}({old_reg}) -> {parts[2]}({new_reg})")

        # Write back
        with open(bin_path, 'wb') as f:
            f.write(text)

        # Create new ELF with the modified text
        # First copy the input
        shutil.copy2(args.input, args.output)

        # Then replace the text section content
        subprocess.run([
            'mipsel-linux-gnu-objcopy',
            '--update-section', f'.text={bin_path}',
            args.output
        ], check=True)

    print(f"Output: {args.output} ({len(text)} bytes)")


if __name__ == '__main__':
    main()
