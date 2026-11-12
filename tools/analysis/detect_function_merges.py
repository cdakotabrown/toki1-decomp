#!/usr/bin/env python3
"""
Detect functions that should be merged (labels within larger functions).

Many "unmatched" functions are actually jump targets within larger functions.
This tool identifies parent-child relationships by analyzing:
1. Functions that use registers not initialized within their scope
2. Functions at addresses inside known symbol ranges
3. Branch targets from other functions

Usage:
    python3 tools/analysis/detect_function_merges.py --exe disks/SLPS_000.65
    python3 tools/analysis/detect_function_merges.py --check 0x8007F064
"""

import argparse
import json
import struct
from pathlib import Path

# Known function symbols from o.bin
SYMBOLS_PATH = Path("build/slps00065/symbols/symbol_map.json")


def load_symbols():
    """Load symbol map."""
    if not SYMBOLS_PATH.exists():
        return {}

    with open(SYMBOLS_PATH) as f:
        data = json.load(f)

    symbols = {}
    for sym in data.get('symbols', []):
        addr = int(sym['addr'], 16)
        symbols[addr] = sym['name']

    return symbols


def get_function_at(symbols, addr):
    """Find which named function contains an address."""
    # Sort symbols by address
    sorted_addrs = sorted(symbols.keys())

    # Find the symbol just before this address
    containing = None
    for sym_addr in sorted_addrs:
        if sym_addr <= addr:
            containing = (sym_addr, symbols[sym_addr])
        else:
            break

    return containing


def analyze_function_registers(exe_path, addr, size):
    """Analyze which registers a function uses without initializing."""
    with open(exe_path, 'rb') as f:
        load_addr = 0x80041000
        offset = 0x800 + (addr - load_addr)
        f.seek(offset)
        data = f.read(size)

    # Track register states
    # 0 = unknown, 1 = read before write, 2 = written first
    reg_state = {i: 0 for i in range(32)}

    # Registers that are always valid inputs
    valid_inputs = {0, 29, 31}  # $zero, $sp, $ra
    # Standard argument registers
    arg_regs = {4, 5, 6, 7}  # $a0-$a3

    suspicious_reads = []

    for i in range(0, min(size, 20), 4):  # Check first 5 instructions
        word = struct.unpack('<I', data[i:i+4])[0]

        op = (word >> 26) & 0x3f
        rs = (word >> 21) & 0x1f
        rt = (word >> 16) & 0x1f
        rd = (word >> 11) & 0x1f

        # Check for reads of uninitialized registers
        if op == 0:  # R-type
            funct = word & 0x3f
            # rs and rt are read (for most R-type ops)
            if funct not in [0, 2, 3]:  # Not shift-by-immediate
                if rs not in valid_inputs and rs not in arg_regs:
                    if reg_state[rs] == 0:
                        suspicious_reads.append(('$' + get_reg_name(rs), addr + i))
                        reg_state[rs] = 1
            # rd is written
            reg_state[rd] = 2
        elif op in [4, 5]:  # beq, bne
            if rs not in valid_inputs and rs not in arg_regs:
                if reg_state[rs] == 0:
                    suspicious_reads.append(('$' + get_reg_name(rs), addr + i))
                    reg_state[rs] = 1
        elif op == 0x0f:  # lui
            reg_state[rt] = 2
        elif op in [0x20, 0x21, 0x23, 0x24, 0x25]:  # loads
            reg_state[rt] = 2
        elif op in [0x08, 0x09, 0x0c, 0x0d]:  # immediate ops
            if rs not in valid_inputs and rs not in arg_regs:
                if reg_state[rs] == 0:
                    suspicious_reads.append(('$' + get_reg_name(rs), addr + i))
            reg_state[rt] = 2

    return suspicious_reads


def get_reg_name(num):
    """Get register name from number."""
    names = [
        'zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
        't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
        's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
        't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra'
    ]
    return names[num] if num < 32 else f'r{num}'


def find_merge_candidates(exe_path, wip_functions):
    """Find functions that are likely labels within larger functions."""
    symbols = load_symbols()
    candidates = []

    for func_addr, func_size in wip_functions:
        # Check if this address falls within a named function
        containing = get_function_at(symbols, func_addr)
        if containing:
            parent_addr, parent_name = containing
            if parent_addr != func_addr:
                offset = func_addr - parent_addr
                candidates.append({
                    'address': func_addr,
                    'size': func_size,
                    'parent_address': parent_addr,
                    'parent_name': parent_name,
                    'offset': offset,
                    'reason': 'inside_named_function'
                })
                continue

        # Check for suspicious register reads
        suspicious = analyze_function_registers(exe_path, func_addr, func_size)
        if suspicious:
            candidates.append({
                'address': func_addr,
                'size': func_size,
                'suspicious_regs': suspicious,
                'reason': 'uninitialized_register_reads'
            })

    return candidates


def scan_wip_functions(wip_dir):
    """Scan WIP directory for function addresses and sizes."""
    functions = []
    wip_path = Path(wip_dir)

    for c_file in wip_path.glob("func_*.c"):
        # Extract address from filename
        name = c_file.stem
        try:
            addr = int(name.replace('func_', ''), 16)
            # Estimate size from file (rough)
            size = 64  # Default
            functions.append((addr, size))
        except ValueError:
            continue

    return functions


def main():
    parser = argparse.ArgumentParser(description='Detect function merge candidates')
    parser.add_argument('--exe', default='disks/SLPS_000.65', help='Executable path')
    parser.add_argument('--wip', default='src/main/wip', help='WIP directory')
    parser.add_argument('--check', type=lambda x: int(x, 0), help='Check specific address')
    parser.add_argument('--output', help='Output JSON file')

    args = parser.parse_args()

    if args.check:
        symbols = load_symbols()
        containing = get_function_at(symbols, args.check)
        if containing:
            parent_addr, parent_name = containing
            if parent_addr != args.check:
                print(f"Address 0x{args.check:08X} is INSIDE {parent_name} (0x{parent_addr:08X})")
                print(f"Offset: +{args.check - parent_addr} bytes")
            else:
                print(f"Address 0x{args.check:08X} IS {parent_name}")
        else:
            print(f"No containing function found for 0x{args.check:08X}")

        # Check registers
        suspicious = analyze_function_registers(args.exe, args.check, 64)
        if suspicious:
            print(f"\nSuspicious register reads (used before initialized):")
            for reg, instr_addr in suspicious:
                print(f"  {reg} at 0x{instr_addr:08X}")
        return

    print("Scanning WIP functions for merge candidates...")
    wip_funcs = scan_wip_functions(args.wip)
    print(f"Found {len(wip_funcs)} WIP functions")

    candidates = find_merge_candidates(args.exe, wip_funcs)

    print(f"\n=== Merge Candidates ({len(candidates)}) ===\n")

    inside_parent = [c for c in candidates if c.get('reason') == 'inside_named_function']
    uninitialized = [c for c in candidates if c.get('reason') == 'uninitialized_register_reads']

    if inside_parent:
        print("Functions inside named parent functions:")
        for c in inside_parent[:20]:
            print(f"  0x{c['address']:08X} inside {c['parent_name']} (+{c['offset']})")
        if len(inside_parent) > 20:
            print(f"  ... and {len(inside_parent) - 20} more")

    if uninitialized:
        print("\nFunctions with uninitialized register reads:")
        for c in uninitialized[:20]:
            regs = ', '.join(r for r, _ in c['suspicious_regs'])
            print(f"  0x{c['address']:08X}: reads {regs}")
        if len(uninitialized) > 20:
            print(f"  ... and {len(uninitialized) - 20} more")

    if args.output:
        with open(args.output, 'w') as f:
            json.dump(candidates, f, indent=2)
        print(f"\nSaved to {args.output}")


if __name__ == '__main__':
    main()
