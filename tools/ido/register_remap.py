#!/usr/bin/env python3
"""
register_remap.py - Attempt to remap registers in compiled MIPS object files

This tool was developed as part of research into matching "hidden register parameter"
functions. These functions expect registers like t6, t7, t8 to be set by the caller
before the function is called.

IMPORTANT: This tool is provided for RESEARCH/EXPERIMENTATION only.
Analysis shows that simple register remapping does NOT work for most functions
because:
1. The compiler makes different instruction scheduling decisions
2. Temporary register allocation cascades through the function
3. Instruction count often differs between compiled and target

See /tmp/register_remap_analysis.md for detailed findings.

Usage:
    python3 register_remap.py input.o output.o --map a0:t6 a1:t1 t6:t9

Example:
    # Remap a0->t6 for functions that expect t6 as hidden param
    python3 register_remap.py func.o func_remapped.o --map a0:t6
"""

import struct
import sys
import argparse
from pathlib import Path


# MIPS register name to number mapping
REG_NAMES = {
    'zero': 0, 'at': 1, 'v0': 2, 'v1': 3,
    'a0': 4, 'a1': 5, 'a2': 6, 'a3': 7,
    't0': 8, 't1': 9, 't2': 10, 't3': 11,
    't4': 12, 't5': 13, 't6': 14, 't7': 15,
    's0': 16, 's1': 17, 's2': 18, 's3': 19,
    's4': 20, 's5': 21, 's6': 22, 's7': 23,
    't8': 24, 't9': 25, 'k0': 26, 'k1': 27,
    'gp': 28, 'sp': 29, 'fp': 30, 'ra': 31,
}

REG_NUMS = {v: k for k, v in REG_NAMES.items()}


def parse_reg(s):
    """Parse register name or number to register number."""
    s = s.lower().strip()
    if s.startswith('$'):
        s = s[1:]
    if s.isdigit():
        return int(s)
    return REG_NAMES.get(s)


def remap_instruction(word, reg_map, verbose=False):
    """
    Remap registers in a single MIPS instruction.

    Returns: (remapped_word, changed)
    """
    original = word
    op = (word >> 26) & 0x3F

    # Different instruction formats use different register fields
    if op == 0:  # R-type: opcode(6) | rs(5) | rt(5) | rd(5) | shamt(5) | funct(6)
        rs = (word >> 21) & 0x1F
        rt = (word >> 16) & 0x1F
        rd = (word >> 11) & 0x1F

        new_rs = reg_map.get(rs, rs)
        new_rt = reg_map.get(rt, rt)
        new_rd = reg_map.get(rd, rd)

        word = (word & 0xFC0007FF) | (new_rs << 21) | (new_rt << 16) | (new_rd << 11)

    elif op in [0x02, 0x03]:  # J/JAL: opcode(6) | target(26) - no registers
        pass

    elif op in [0x04, 0x05, 0x06, 0x07]:  # BEQ/BNE/BLEZ/BGTZ: rs, rt, offset
        rs = (word >> 21) & 0x1F
        rt = (word >> 16) & 0x1F

        new_rs = reg_map.get(rs, rs)
        new_rt = reg_map.get(rt, rt)

        word = (word & 0xFC00FFFF) | (new_rs << 21) | (new_rt << 16)

    elif op == 0x01:  # REGIMM (BLTZ, BGEZ, etc.): rs, sub, offset
        rs = (word >> 21) & 0x1F
        new_rs = reg_map.get(rs, rs)
        word = (word & 0xFC1FFFFF) | (new_rs << 21)

    elif op in [0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F]:
        # ADDI/ADDIU/SLTI/SLTIU/ANDI/ORI/XORI/LUI: rs, rt, imm
        rs = (word >> 21) & 0x1F
        rt = (word >> 16) & 0x1F

        new_rs = reg_map.get(rs, rs)
        new_rt = reg_map.get(rt, rt)

        word = (word & 0xFC00FFFF) | (new_rs << 21) | (new_rt << 16)

    elif op in [0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26]:
        # LB/LH/LWL/LW/LBU/LHU/LWR: base(rs), rt, offset
        rs = (word >> 21) & 0x1F
        rt = (word >> 16) & 0x1F

        new_rs = reg_map.get(rs, rs)
        new_rt = reg_map.get(rt, rt)

        word = (word & 0xFC00FFFF) | (new_rs << 21) | (new_rt << 16)

    elif op in [0x28, 0x29, 0x2A, 0x2B, 0x2E]:
        # SB/SH/SWL/SW/SWR: base(rs), rt, offset
        rs = (word >> 21) & 0x1F
        rt = (word >> 16) & 0x1F

        new_rs = reg_map.get(rs, rs)
        new_rt = reg_map.get(rt, rt)

        word = (word & 0xFC00FFFF) | (new_rs << 21) | (new_rt << 16)

    changed = word != original

    if verbose and changed:
        print(f"  Remapped: {original:08X} -> {word:08X}")

    return word, changed


def find_text_section(elf_data):
    """
    Find the .text section in an ELF file.
    Returns (offset, size) or None if not found.
    """
    # Check ELF magic
    if elf_data[:4] != b'\x7fELF':
        return None

    # ELF32 header
    e_shoff = struct.unpack('>I', elf_data[0x20:0x24])[0]  # Section header offset
    e_shentsize = struct.unpack('>H', elf_data[0x2E:0x30])[0]  # Section header entry size
    e_shnum = struct.unpack('>H', elf_data[0x30:0x32])[0]  # Number of section headers
    e_shstrndx = struct.unpack('>H', elf_data[0x32:0x34])[0]  # Section name string table index

    # Read section name string table
    shstr_off = e_shoff + e_shstrndx * e_shentsize
    shstr_entry = elf_data[shstr_off:shstr_off + e_shentsize]
    shstr_sh_offset = struct.unpack('>I', shstr_entry[0x10:0x14])[0]

    # Find .text section
    for i in range(e_shnum):
        sh_off = e_shoff + i * e_shentsize
        sh_entry = elf_data[sh_off:sh_off + e_shentsize]

        sh_name = struct.unpack('>I', sh_entry[0x00:0x04])[0]
        sh_type = struct.unpack('>I', sh_entry[0x04:0x08])[0]
        sh_offset = struct.unpack('>I', sh_entry[0x10:0x14])[0]
        sh_size = struct.unpack('>I', sh_entry[0x14:0x18])[0]

        # Get section name
        name_end = elf_data.find(b'\x00', shstr_sh_offset + sh_name)
        name = elf_data[shstr_sh_offset + sh_name:name_end].decode('ascii', errors='ignore')

        if name == '.text':
            return sh_offset, sh_size

    return None


def remap_elf(input_path, output_path, reg_map, verbose=False):
    """
    Remap registers in the .text section of an ELF file.
    """
    with open(input_path, 'rb') as f:
        data = bytearray(f.read())

    text_info = find_text_section(data)
    if text_info is None:
        print(f"Error: Could not find .text section in {input_path}")
        return False

    text_offset, text_size = text_info
    print(f"Found .text section at offset 0x{text_offset:X}, size {text_size} bytes")

    # Remap each instruction
    changed_count = 0
    for i in range(0, text_size, 4):
        offset = text_offset + i
        word = struct.unpack('>I', data[offset:offset+4])[0]

        new_word, changed = remap_instruction(word, reg_map, verbose)

        if changed:
            changed_count += 1
            data[offset:offset+4] = struct.pack('>I', new_word)

    print(f"Remapped {changed_count} instructions")

    with open(output_path, 'wb') as f:
        f.write(data)

    return True


def main():
    parser = argparse.ArgumentParser(
        description='Remap registers in MIPS ELF object file',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
    # Remap a0->t6 for hidden parameter functions
    %(prog)s func.o func_remapped.o --map a0:t6

    # Multiple remappings
    %(prog)s func.o func_remapped.o --map a0:t6 a1:t1 t6:t9 t7:t2

WARNING: This tool is experimental. Simple register remapping usually does NOT
produce matching code because the compiler makes different scheduling decisions.
See the analysis documentation for details.
"""
    )
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output ELF object file')
    parser.add_argument('--map', nargs='+', required=True,
                       help='Register mappings in format FROM:TO (e.g., a0:t6)')
    parser.add_argument('-v', '--verbose', action='store_true',
                       help='Show each remapped instruction')

    args = parser.parse_args()

    # Parse register mappings
    reg_map = {}
    for mapping in args.map:
        if ':' not in mapping:
            print(f"Error: Invalid mapping format '{mapping}', expected FROM:TO")
            sys.exit(1)

        from_reg, to_reg = mapping.split(':')
        from_num = parse_reg(from_reg)
        to_num = parse_reg(to_reg)

        if from_num is None:
            print(f"Error: Unknown register '{from_reg}'")
            sys.exit(1)
        if to_num is None:
            print(f"Error: Unknown register '{to_reg}'")
            sys.exit(1)

        reg_map[from_num] = to_num
        print(f"Mapping: {REG_NUMS[from_num]} (${from_num}) -> {REG_NUMS[to_num]} (${to_num})")

    # Check for conflicts
    used_targets = set(reg_map.values())
    for from_reg, to_reg in reg_map.items():
        if from_reg in used_targets and from_reg not in reg_map.values():
            print(f"Warning: Source register {REG_NUMS[from_reg]} is also a target, may cause issues")

    if not remap_elf(args.input, args.output, reg_map, args.verbose):
        sys.exit(1)

    print(f"\nOutput written to {args.output}")
    print("\nNOTE: Verify the output manually - register remapping alone")
    print("usually does NOT produce matching code due to instruction scheduling")
    print("differences between compiled and target code.")


if __name__ == '__main__':
    main()
