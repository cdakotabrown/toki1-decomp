#!/usr/bin/env python3
"""Analyze dual_entry_thunk functions to categorize them properly."""

import json
import struct
import sys
from pathlib import Path

def read_word(f, addr, base=0x80041000, file_offset=0x800):
    """Read a 32-bit word from the binary at given address."""
    file_pos = file_offset + (addr - base)
    f.seek(file_pos)
    return struct.unpack('<I', f.read(4))[0]

def decode_opcode(word):
    """Decode instruction opcode."""
    opcode = (word >> 26) & 0x3F
    funct = word & 0x3F

    # Check for jr/jalr
    if opcode == 0x00 and funct == 0x08:
        return 'jr'
    if opcode == 0x00 and funct == 0x09:
        return 'jalr'

    # Check for j/jal
    if opcode == 0x02:
        return 'j'
    if opcode == 0x03:
        return 'jal'

    # Check for branches
    if opcode in [0x04, 0x05, 0x06, 0x07]:  # beq, bne, blez, bgtz
        return 'branch'
    if opcode == 0x01:  # bltz, bgez, etc.
        return 'branch'

    # Check for lui
    if opcode == 0x0F:
        return 'lui'

    # Check for loads
    if opcode in [0x20, 0x21, 0x23, 0x24, 0x25, 0x30, 0x31]:  # lb, lh, lw, lbu, lhu
        return 'load'

    # Check for stores
    if opcode in [0x28, 0x29, 0x2B]:  # sb, sh, sw
        return 'store'

    return 'other'

def analyze_thunk(binary_path, addr):
    """Analyze a single thunk function."""
    with open(binary_path, 'rb') as f:
        # Read instruction at thunk entry (4 bytes before symbol entry)
        thunk_addr = addr
        prev_addr = addr - 4
        prev_prev_addr = addr - 8

        thunk_word = read_word(f, thunk_addr)
        prev_word = read_word(f, prev_addr)
        prev_prev_word = read_word(f, prev_prev_addr)

        # Check if previous instruction is a jump/branch
        prev_op = decode_opcode(prev_word)
        prev_prev_op = decode_opcode(prev_prev_word)
        thunk_op = decode_opcode(thunk_word)

        # Classify
        if prev_op in ['jr', 'jalr', 'j', 'jal', 'branch']:
            return 'delay_slot_artifact', prev_op
        elif prev_prev_op in ['jr', 'jalr', 'j', 'jal', 'branch']:
            # Check if prev is nop
            if prev_word == 0:
                return 'genuine_cross_prologue', thunk_op
            else:
                # Might be two hoisted instructions
                return 'genuine_cross_prologue_2insn', f"{prev_op}+{thunk_op}"
        elif thunk_op == 'lui':
            return 'genuine_cross_prologue_lui', 'lui'
        elif thunk_op in ['load', 'store']:
            return 'genuine_cross_prologue_ldst', thunk_op
        else:
            return 'unknown', thunk_op

def main():
    # Load classification data
    # the repository root: tools/analysis/<this file> -> two levels up
    script_dir = Path(__file__).resolve().parents[2]

    config_path = script_dir / 'config' / 'function_classification.json'
    binary_path = script_dir / 'disks' / 'SLPS_000.65'

    with open(config_path) as f:
        data = json.load(f)

    # Get all dual_entry_thunk functions
    thunks = [f for f in data['functions'] if 'dual_entry_thunk' in f['categories']]

    print(f"Analyzing {len(thunks)} dual_entry_thunk functions...")
    print()

    # Analyze each
    results = {}
    for func in thunks:
        addr = int(func['address'], 16)
        category, detail = analyze_thunk(binary_path, addr)

        if category not in results:
            results[category] = []
        results[category].append((func['address'], func['name'], detail))

    # Print summary
    print("Classification Results:")
    print("=" * 70)
    for category, funcs in sorted(results.items(), key=lambda x: -len(x[1])):
        print(f"\n{category}: {len(funcs)} functions")
        print("-" * 70)
        for addr, name, detail in funcs[:5]:
            print(f"  {addr} {name} ({detail})")
        if len(funcs) > 5:
            print(f"  ... and {len(funcs) - 5} more")

    print()
    print("Summary:")
    print("-" * 70)
    total = len(thunks)
    for category, funcs in sorted(results.items(), key=lambda x: -len(x[1])):
        pct = 100.0 * len(funcs) / total
        print(f"{category:30s}: {len(funcs):3d} ({pct:5.1f}%)")

if __name__ == '__main__':
    main()
