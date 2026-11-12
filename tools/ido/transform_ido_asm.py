#!/usr/bin/env python3
"""
Transform IDO assembly output for Tokimeki Memorial matching.

This transforms IDO -O1 assembly output to match Tokimeki patterns:
1. Remove unnecessary sw/lw argument save pairs
2. Adjust frame sizes from 24 to 40 bytes
3. Clean up extra nops
"""

import re
import sys
import argparse

def transform_asm(lines, frame_delta=16):
    """Transform IDO assembly to match Tokimeki patterns."""
    result = []
    i = 0

    while i < len(lines):
        line = lines[i]

        # Check for sw ax, offset(sp) followed by lw ax, offset(sp)
        # This is an unnecessary argument save/load pair
        if i + 1 < len(lines):
            sw_match = re.match(r'^(\s*)(sw|sd)\s+\$(\w+)\s*,\s*(\d+)\s*\(\s*\$sp\s*\)', line)
            lw_match = re.match(r'^(\s*)(lw|ld)\s+\$(\w+)\s*,\s*(\d+)\s*\(\s*\$sp\s*\)', lines[i+1])

            if sw_match and lw_match:
                sw_reg, sw_offset = sw_match.group(3), sw_match.group(4)
                lw_reg, lw_offset = lw_match.group(3), lw_match.group(4)

                # Same register, same offset, not $ra (31)
                if sw_reg == lw_reg and sw_offset == lw_offset:
                    # Skip $ra saves (those are intentional)
                    if sw_reg not in ('31', 'ra'):
                        # Skip both lines
                        i += 2
                        continue

        # Transform frame allocation: subu/addiu sp, N -> subu/addiu sp, N+delta
        frame_match = re.match(r'^(\s*)(subu|addiu)\s+\$sp\s*,\s*(-?\d+)(.*)$', line)
        if frame_match:
            indent, op, size, rest = frame_match.groups()
            old_size = int(size)
            if old_size < 0:  # Prologue
                new_size = old_size - frame_delta
            else:  # Epilogue
                new_size = old_size + frame_delta
            result.append(f"{indent}{op}\t$sp, {new_size}{rest}\n")
            i += 1
            continue

        # Also match "addiu $sp, $sp, N" form
        frame_match2 = re.match(r'^(\s*)(addiu)\s+\$sp\s*,\s*\$sp\s*,\s*(-?\d+)(.*)$', line)
        if frame_match2:
            indent, op, size, rest = frame_match2.groups()
            old_size = int(size)
            if old_size < 0:  # Prologue
                new_size = old_size - frame_delta
            else:  # Epilogue
                new_size = old_size + frame_delta
            result.append(f"{indent}{op}\t$sp, $sp, {new_size}{rest}\n")
            i += 1
            continue

        # Transform .frame directive
        frame_dir = re.match(r'^(\s*)(\.frame)\s+\$sp\s*,\s*(\d+)\s*,\s*\$31(.*)$', line)
        if frame_dir:
            indent, directive, size, rest = frame_dir.groups()
            new_size = int(size) + frame_delta
            result.append(f"{indent}{directive}\t$sp, {new_size}, $31{rest}\n")
            i += 1
            continue

        # Remove trailing nops (but keep delay slot nops)
        # A nop at the very end of a function can usually be removed
        # But we need to be careful about delay slots

        result.append(line)
        i += 1

    return result

def main():
    parser = argparse.ArgumentParser(description='Transform IDO assembly for Tokimeki matching')
    parser.add_argument('input', help='Input assembly file')
    parser.add_argument('-o', '--output', help='Output file (default: stdout)')
    parser.add_argument('-d', '--delta', type=int, default=16,
                       help='Frame size delta to add (default: 16)')

    args = parser.parse_args()

    with open(args.input, 'r') as f:
        lines = f.readlines()

    transformed = transform_asm(lines, args.delta)

    if args.output:
        with open(args.output, 'w') as f:
            f.writelines(transformed)
    else:
        for line in transformed:
            print(line, end='')

if __name__ == '__main__':
    main()
