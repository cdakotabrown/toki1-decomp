#!/usr/bin/env python3
"""
Hidden Register Parameter Detection Tool

Detects functions where a non-argument register is READ before being WRITTEN,
indicating a "hidden parameter" that cannot be expressed in C.

Common patterns in Tokimeki Memorial:
- t6: Loop index, pointer seed, computed offset
- v1: Switch dispatch variable, return value carrier
- t7/t8: Extended argument passing

These functions require assembly - don't waste permuter cycles on them.

Usage:
    python3 detect_hidden_regs.py                    # Analyze all WIP functions
    python3 detect_hidden_regs.py --address 0x80041234
    python3 detect_hidden_regs.py --classify         # Output classification JSON
"""

import os
import re
import sys
import struct
import json
import argparse
from pathlib import Path

# Constants
EXE_PATH = "disks/SLPS_000.65"
TEXT_START = 0x800  # PS-X EXE header size
BASE_ADDR = 0x80041000
CODE_END = 0x800C0000

# MIPS register names
REG_NAMES = {
    0: 'zero', 1: 'at', 2: 'v0', 3: 'v1',
    4: 'a0', 5: 'a1', 6: 'a2', 7: 'a3',
    8: 't0', 9: 't1', 10: 't2', 11: 't3',
    12: 't4', 13: 't5', 14: 't6', 15: 't7',
    16: 's0', 17: 's1', 18: 's2', 19: 's3',
    20: 's4', 21: 's5', 22: 's6', 23: 's7',
    24: 't8', 25: 't9', 26: 'k0', 27: 'k1',
    28: 'gp', 29: 'sp', 30: 'fp', 31: 'ra'
}

# Argument registers (not hidden params)
ARG_REGS = {4, 5, 6, 7}  # a0-a3

# Registers that indicate hidden params when read before write
HIDDEN_CANDIDATES = {
    3: 'v1',   # Often dispatch variable
    14: 't6',  # Often loop/pointer
    15: 't7',  # Extended arg
    24: 't8',  # Extended arg
    1: 'at',   # Assembler temp (rare)
}


def decode_instruction(word):
    """Decode MIPS instruction into (op, rs, rt, rd, shamt, funct, imm)."""
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    shamt = (word >> 6) & 0x1F
    funct = word & 0x3F
    imm = word & 0xFFFF
    return op, rs, rt, rd, shamt, funct, imm


def get_read_regs(word):
    """Get list of registers READ by this instruction."""
    op, rs, rt, rd, shamt, funct, imm = decode_instruction(word)
    reads = set()

    if op == 0:  # R-type
        if funct in (0x08, 0x09):  # jr, jalr
            reads.add(rs)
        elif funct in (0x00, 0x02, 0x03):  # sll, srl, sra (shift by imm)
            reads.add(rt)
        elif funct in (0x04, 0x06, 0x07):  # sllv, srlv, srav
            reads.add(rt)
            reads.add(rs)
        elif funct in (0x10, 0x12):  # mfhi, mflo
            pass  # Read from HI/LO, not GPR
        elif funct in (0x11, 0x13):  # mthi, mtlo
            reads.add(rs)
        elif funct in (0x18, 0x19, 0x1A, 0x1B):  # mult, multu, div, divu
            reads.add(rs)
            reads.add(rt)
        else:  # Most R-type: read rs, rt
            reads.add(rs)
            reads.add(rt)
    elif op in (0x04, 0x05, 0x06, 0x07):  # beq, bne, blez, bgtz
        reads.add(rs)
        if op in (0x04, 0x05):  # beq, bne also read rt
            reads.add(rt)
    elif op == 0x01:  # REGIMM (bltz, bgez, etc.)
        reads.add(rs)
    elif op in (0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F):
        # addi, addiu, slti, sltiu, andi, ori, xori, lui
        if op != 0x0F:  # lui doesn't read rs
            reads.add(rs)
    elif op in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26):
        # lb, lh, lwl, lw, lbu, lhu, lwr - read base
        reads.add(rs)
    elif op in (0x28, 0x29, 0x2A, 0x2B, 0x2E):
        # sb, sh, swl, sw, swr - read base and rt
        reads.add(rs)
        reads.add(rt)
    elif op == 0x02 or op == 0x03:  # j, jal
        pass  # No register reads
    elif op == 0x10:  # COP0
        pass  # Ignore coprocessor

    reads.discard(0)  # $zero doesn't count
    return reads


def get_write_regs(word):
    """Get list of registers WRITTEN by this instruction."""
    op, rs, rt, rd, shamt, funct, imm = decode_instruction(word)
    writes = set()

    if op == 0:  # R-type
        if funct in (0x08,):  # jr
            pass
        elif funct == 0x09:  # jalr
            writes.add(rd if rd != 0 else 31)
        elif funct in (0x10,):  # mfhi
            writes.add(rd)
        elif funct in (0x12,):  # mflo
            writes.add(rd)
        elif funct in (0x11, 0x13, 0x18, 0x19, 0x1A, 0x1B):
            # mthi, mtlo, mult, multu, div, divu - write HI/LO not GPR
            pass
        else:
            writes.add(rd)
    elif op in (0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F):
        # I-type ALU: write rt
        writes.add(rt)
    elif op in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26):
        # Load instructions: write rt
        writes.add(rt)
    elif op == 0x03:  # jal
        writes.add(31)  # ra

    writes.discard(0)  # Can't write $zero
    return writes


def find_function_end(exe_data, start_offset, max_size=4096):
    """Find end of function (jr ra followed by delay slot, or next prologue)."""
    for i in range(0, max_size, 4):
        offset = start_offset + i
        if offset + 8 > len(exe_data):
            break

        word = struct.unpack('<I', exe_data[offset:offset+4])[0]
        next_word = struct.unpack('<I', exe_data[offset+4:offset+8])[0]

        # jr ra (0x03e00008) followed by anything = function end
        if word == 0x03E00008:
            return i + 8  # Include delay slot

        # Next prologue (addiu sp,sp,-N)
        if i > 0 and (word & 0xFFFF0000) == 0x27BD0000:
            imm = word & 0xFFFF
            if imm >= 0x8000:  # Negative = stack allocation
                return i

    return max_size


def analyze_function(exe_data, addr, verbose=False):
    """
    Analyze a function for hidden register parameters.

    Returns:
        dict with 'hidden_regs', 'classification', 'details'
    """
    if addr < BASE_ADDR or addr >= CODE_END:
        return {'error': 'Address out of range'}

    file_offset = TEXT_START + (addr - BASE_ADDR)

    # Check for prologue
    if file_offset + 4 > len(exe_data):
        return {'error': 'Invalid offset'}

    prologue = struct.unpack('<I', exe_data[file_offset:file_offset+4])[0]

    # Optional: check if there's an instruction BEFORE the prologue
    # that sets up a hidden register
    pre_prologue_hidden = set()
    if file_offset >= TEXT_START + 4:
        prev_word = struct.unpack('<I', exe_data[file_offset-4:file_offset])[0]
        writes = get_write_regs(prev_word)
        for reg in writes:
            if reg in HIDDEN_CANDIDATES:
                pre_prologue_hidden.add(reg)

    # Find function size
    func_size = find_function_end(exe_data, file_offset)

    # Track register state: None=unset, 'r'=read first, 'w'=written first
    reg_state = {}
    hidden_regs = set()
    details = []

    # Analyze instructions
    for i in range(0, func_size, 4):
        offset = file_offset + i
        if offset + 4 > len(exe_data):
            break

        word = struct.unpack('<I', exe_data[offset:offset+4])[0]
        reads = get_read_regs(word)
        writes = get_write_regs(word)

        # Check for reads of hidden candidate regs before write
        for reg in reads:
            if reg in HIDDEN_CANDIDATES and reg not in reg_state:
                reg_state[reg] = 'r'
                hidden_regs.add(reg)
                details.append(f"0x{addr+i:08x}: {HIDDEN_CANDIDATES[reg]} read before write")

        # Mark writes
        for reg in writes:
            if reg not in reg_state:
                reg_state[reg] = 'w'

    # Classify
    classification = 'c_matchable'
    if hidden_regs or pre_prologue_hidden:
        all_hidden = hidden_regs | pre_prologue_hidden
        if 3 in all_hidden and 14 in all_hidden:
            classification = 'hidden_v1_t6'  # Dispatcher pattern
        elif 3 in all_hidden:
            classification = 'hidden_v1'
        elif 14 in all_hidden:
            classification = 'hidden_t6'
        elif 15 in all_hidden or 24 in all_hidden:
            classification = 'hidden_t7_t8'
        else:
            classification = 'hidden_other'

    return {
        'address': f'0x{addr:08x}',
        'size': func_size,
        'hidden_regs': [HIDDEN_CANDIDATES.get(r, f'r{r}') for r in sorted(hidden_regs)],
        'pre_prologue_setup': [HIDDEN_CANDIDATES.get(r, f'r{r}') for r in sorted(pre_prologue_hidden)],
        'classification': classification,
        'asm_required': classification != 'c_matchable',
        'details': details if verbose else []
    }


def get_wip_addresses():
    """Get addresses of all WIP C files."""
    wip_dir = Path("src/main/wip")
    addresses = []

    if not wip_dir.exists():
        return addresses

    pattern = re.compile(r'func_([0-9a-fA-F]+)\.c')
    for f in wip_dir.iterdir():
        match = pattern.match(f.name)
        if match:
            addr = int(match.group(1), 16)
            addresses.append(addr)

    return sorted(addresses)


def main():
    parser = argparse.ArgumentParser(
        description='Detect hidden register parameters in functions',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Classification meanings:
  c_matchable    - No hidden regs detected, try C matching
  hidden_v1      - v1 used before write (dispatcher pattern)
  hidden_t6      - t6 used before write (loop/pointer pattern)
  hidden_v1_t6   - Both v1 and t6 (complex dispatcher)
  hidden_t7_t8   - t7/t8 extended argument passing
  hidden_other   - Other hidden register usage

Examples:
    %(prog)s                          # Analyze all WIP functions
    %(prog)s --address 0x800739E4     # Analyze specific function
    %(prog)s --classify --json        # Output JSON classification
"""
    )
    parser.add_argument('--address', type=lambda x: int(x, 0),
                        help='Analyze specific function address')
    parser.add_argument('--classify', action='store_true',
                        help='Classify all WIP functions')
    parser.add_argument('--json', action='store_true',
                        help='Output as JSON')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Show detailed analysis')

    args = parser.parse_args()

    # Read executable
    if not os.path.exists(EXE_PATH):
        print(f"Error: {EXE_PATH} not found", file=sys.stderr)
        sys.exit(1)

    with open(EXE_PATH, 'rb') as f:
        exe_data = f.read()

    if args.address:
        # Single function analysis
        result = analyze_function(exe_data, args.address, args.verbose)
        if args.json:
            print(json.dumps(result, indent=2))
        else:
            print(f"Function at {result.get('address', 'unknown')}:")
            print(f"  Size: {result.get('size', 0)} bytes")
            print(f"  Hidden regs: {result.get('hidden_regs', [])}")
            print(f"  Pre-prologue setup: {result.get('pre_prologue_setup', [])}")
            print(f"  Classification: {result.get('classification', 'unknown')}")
            print(f"  ASM required: {result.get('asm_required', False)}")
            if result.get('details'):
                print("  Details:")
                for d in result['details']:
                    print(f"    {d}")
    else:
        # Analyze all WIP functions
        addresses = get_wip_addresses()
        if not addresses:
            print("No WIP functions found in src/main/wip/")
            sys.exit(1)

        results = []
        stats = {
            'c_matchable': 0,
            'hidden_v1': 0,
            'hidden_t6': 0,
            'hidden_v1_t6': 0,
            'hidden_t7_t8': 0,
            'hidden_other': 0,
        }

        for addr in addresses:
            result = analyze_function(exe_data, addr, args.verbose)
            results.append(result)
            cls = result.get('classification', 'unknown')
            if cls in stats:
                stats[cls] += 1

        if args.json:
            output = {
                'total': len(results),
                'stats': stats,
                'functions': results
            }
            print(json.dumps(output, indent=2))
        else:
            print(f"Analyzed {len(results)} WIP functions:\n")
            print("Classification breakdown:")
            for cls, count in sorted(stats.items(), key=lambda x: -x[1]):
                pct = 100.0 * count / len(results) if results else 0
                marker = "  " if cls == 'c_matchable' else "* "
                print(f"  {marker}{cls}: {count} ({pct:.1f}%)")

            print("\nASM required functions:")
            asm_funcs = [r for r in results if r.get('asm_required')]
            for r in asm_funcs[:20]:  # Show first 20
                regs = r.get('hidden_regs', []) + r.get('pre_prologue_setup', [])
                print(f"  {r['address']}: {r['classification']} ({', '.join(regs)})")
            if len(asm_funcs) > 20:
                print(f"  ... and {len(asm_funcs) - 20} more")


if __name__ == '__main__':
    main()
