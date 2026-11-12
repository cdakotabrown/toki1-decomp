#!/usr/bin/env python3
"""
Transform IDO 5.3 assembly output to match Tokimeki Memorial frame sizes.

IDO 5.3 produces 24-byte frames, Tokimeki has 40-byte frames.
The extra 16 bytes are added above the return address.

Transformations:
- subu $sp, 24 → subu $sp, 40
- addiu $sp, $sp, -24 → addiu $sp, $sp, -40
- .frame $sp, 24, $31 → .frame $sp, 40, $31
- addu $sp, 24 → addu $sp, 40
- addiu $sp, $sp, 24 → addiu $sp, $sp, 40

RA offset stays at 20 (unchanged).
"""

import re
import sys
import argparse

def transform_line(line, frame_delta=16):
    """Transform a single assembly line to adjust frame size."""

    # Match frame prologue: subu $sp, N or addiu $sp, $sp, -N
    prologue_subu = re.match(r'^(\s*)(subu)\s+\$sp\s*,\s*(\d+)(.*)$', line)
    if prologue_subu:
        indent, op, size, rest = prologue_subu.groups()
        new_size = int(size) + frame_delta
        return f"{indent}{op}\t$sp, {new_size}{rest}\n"

    prologue_addiu = re.match(r'^(\s*)(addiu)\s+\$sp\s*,\s*\$sp\s*,\s*-(\d+)(.*)$', line)
    if prologue_addiu:
        indent, op, size, rest = prologue_addiu.groups()
        new_size = int(size) + frame_delta
        return f"{indent}{op}\t$sp, $sp, -{new_size}{rest}\n"

    # Match frame epilogue: addu $sp, N or addiu $sp, $sp, N (positive)
    epilogue_addu = re.match(r'^(\s*)(addu)\s+\$sp\s*,\s*(\d+)(.*)$', line)
    if epilogue_addu:
        indent, op, size, rest = epilogue_addu.groups()
        new_size = int(size) + frame_delta
        return f"{indent}{op}\t$sp, {new_size}{rest}\n"

    epilogue_addiu = re.match(r'^(\s*)(addiu)\s+\$sp\s*,\s*\$sp\s*,\s*(\d+)(.*)$', line)
    if epilogue_addiu:
        indent, op, size, rest = epilogue_addiu.groups()
        old_size = int(size)
        if old_size > 0:  # Positive = epilogue
            new_size = old_size + frame_delta
            return f"{indent}{op}\t$sp, $sp, {new_size}{rest}\n"

    # Match .frame directive
    frame_dir = re.match(r'^(\s*)(\.frame)\s+\$sp\s*,\s*(\d+)\s*,\s*\$31(.*)$', line)
    if frame_dir:
        indent, directive, size, rest = frame_dir.groups()
        new_size = int(size) + frame_delta
        return f"{indent}{directive}\t$sp, {new_size}, $31{rest}\n"

    # No transformation needed
    return line

def transform_file(input_path, output_path=None, frame_delta=16):
    """Transform an entire assembly file."""

    with open(input_path, 'r') as f:
        lines = f.readlines()

    transformed = []
    for line in lines:
        transformed.append(transform_line(line, frame_delta))

    if output_path:
        with open(output_path, 'w') as f:
            f.writelines(transformed)
    else:
        for line in transformed:
            print(line, end='')

def main():
    parser = argparse.ArgumentParser(description='Transform IDO assembly frame sizes')
    parser.add_argument('input', help='Input assembly file')
    parser.add_argument('-o', '--output', help='Output file (default: stdout)')
    parser.add_argument('-d', '--delta', type=int, default=16,
                       help='Frame size delta to add (default: 16)')

    args = parser.parse_args()
    transform_file(args.input, args.output, args.delta)

if __name__ == '__main__':
    main()
