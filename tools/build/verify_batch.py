#!/usr/bin/env python3
"""
Verify compiled C code against target binary.
Returns match percentage and shows differences.
"""

import sys
import struct
import subprocess
import tempfile
import os
import argparse

def verify_function(c_file, target_addr, target_size, frame_old=24, frame_new=40):
    """Compile C file and verify against target."""

    TEXT_BASE = 0x80041000
    HEADER_SIZE = 2048

    # Compile C file
    with tempfile.NamedTemporaryFile(suffix='.o', delete=False) as tf:
        obj_file = tf.name

    try:
        result = subprocess.run([
            './tools/ido-static-recomp/build/5.2/out/cc',
            '-O2', '-G0', '-mips1', '-EL', '-non_shared',
            '-I', 'include',
            '-c', c_file, '-o', obj_file
        ], capture_output=True, text=True)

        if result.returncode != 0:
            print(f"Compile error: {result.stderr}")
            return None

        # Patch frame size
        patched_file = obj_file + '.patched'
        subprocess.run([
            'python3', 'tools/ido/patch_frame_complete.py',
            obj_file, patched_file,
            '--old-size', str(frame_old),
            '--new-size', str(frame_new)
        ], capture_output=True)

        # Read patched object
        with open(patched_file, 'rb') as f:
            elf_data = f.read()
        os.unlink(patched_file)

        # Find .text section by looking for prologue
        prologue = struct.pack('<I', 0x27bd0000 | ((-frame_new) & 0xFFFF))
        text_start = elf_data.find(prologue)
        if text_start == -1:
            # Try without frame patch
            prologue = struct.pack('<I', 0x27bd0000 | ((-frame_old) & 0xFFFF))
            text_start = elf_data.find(prologue)

        if text_start == -1:
            print("Cannot find .text section")
            return None

        compiled_bytes = elf_data[text_start:text_start+target_size]

    finally:
        if os.path.exists(obj_file):
            os.unlink(obj_file)

    # Read target bytes
    file_offset = HEADER_SIZE + (target_addr - TEXT_BASE)
    with open('disks/SLPS_000.65', 'rb') as f:
        f.seek(file_offset)
        target_bytes = f.read(target_size)

    # Compare
    match = 0
    reloc = 0
    diff_list = []

    for i in range(0, target_size, 4):
        tw = struct.unpack('<I', target_bytes[i:i+4])[0]
        cw = struct.unpack('<I', compiled_bytes[i:i+4])[0]

        if tw == cw:
            match += 1
            continue

        c_opcode = (cw >> 26) & 0x3F
        t_opcode = (tw >> 26) & 0x3F

        # Check if relocation
        if c_opcode == t_opcode:
            upper_match = (cw & 0xFFFF0000) == (tw & 0xFFFF0000)
            is_lui = c_opcode == 15
            is_jal = c_opcode == 3

            if upper_match or is_lui or is_jal:
                reloc += 1
                continue

        diff_list.append((i, tw, cw))

    total = target_size // 4
    match_pct = (match + reloc) / total * 100

    return {
        'match': match,
        'reloc': reloc,
        'diff': len(diff_list),
        'total': total,
        'match_pct': match_pct,
        'diffs': diff_list
    }

def main():
    parser = argparse.ArgumentParser(description='Verify compiled C against target')
    parser.add_argument('c_file', help='C source file')
    parser.add_argument('address', help='Target address (hex)')
    parser.add_argument('size', type=int, help='Target size in bytes')
    parser.add_argument('--frame-old', type=int, default=24, help='Old frame size')
    parser.add_argument('--frame-new', type=int, default=40, help='New frame size')

    args = parser.parse_args()

    addr = int(args.address, 16) if args.address.startswith('0x') else int(args.address)

    result = verify_function(args.c_file, addr, args.size, args.frame_old, args.frame_new)

    if result:
        print(f"Match: {result['match']}/{result['total']} exact")
        print(f"Reloc: {result['reloc']}/{result['total']}")
        print(f"Diff:  {result['diff']}/{result['total']}")
        print(f"Rate:  {result['match_pct']:.1f}%")

        if result['diffs']:
            print("\nDifferences:")
            for off, tw, cw in result['diffs'][:10]:
                print(f"  0x{off:04x}: {tw:08x} vs {cw:08x}")
            if len(result['diffs']) > 10:
                print(f"  ... and {len(result['diffs']) - 10} more")

if __name__ == '__main__':
    main()
