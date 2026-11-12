#!/usr/bin/env python3
"""
rotate_registers.py - Apply cyclic register rotation to compiled MIPS object files

Usage:
    python3 rotate_registers.py input.o output.o --map 't0:t4,t1:t5,...'
    python3 rotate_registers.py input.o output.o --rotate-t 4  # shorthand: rotate t0-t9 by +4

This tool applies a simultaneous register mapping (not sequential swaps) to the
.text section of an ELF object file. All register replacements happen in parallel
so circular mappings work correctly (e.g., t0->t4, t4->t8, t8->t2, t2->t6, t6->t0).
"""

import argparse
import struct
import sys
import shutil

# MIPS register numbers
REGS = {
    'zero': 0, 'at': 1,
    'v0': 2, 'v1': 3,
    'a0': 4, 'a1': 5, 'a2': 6, 'a3': 7,
    't0': 8, 't1': 9, 't2': 10, 't3': 11,
    't4': 12, 't5': 13, 't6': 14, 't7': 15,
    's0': 16, 's1': 17, 's2': 18, 's3': 19,
    's4': 20, 's5': 21, 's6': 22, 's7': 23,
    't8': 24, 't9': 25,
    'k0': 26, 'k1': 27,
    'gp': 28, 'sp': 29, 'fp': 30, 's8': 30, 'ra': 31
}

REG_NAMES = {v: k for k, v in REGS.items() if k != 's8'}


def apply_mapping(instr, mapping):
    """Apply register mapping to a single MIPS instruction.
    mapping is a dict of {old_reg_num: new_reg_num}.
    All substitutions are applied simultaneously."""
    opcode = (instr >> 26) & 0x3F
    rs = (instr >> 21) & 0x1F
    rt = (instr >> 16) & 0x1F
    rd = (instr >> 11) & 0x1F

    new_rs = mapping.get(rs, rs)
    new_rt = mapping.get(rt, rt)

    # rd only for R-type (opcode == 0) and SPECIAL2 (opcode == 28)
    if opcode == 0 or opcode == 28:
        new_rd = mapping.get(rd, rd)
    else:
        new_rd = rd

    new_instr = (instr & ~(0x1F << 21)) | (new_rs << 21)
    new_instr = (new_instr & ~(0x1F << 16)) | (new_rt << 16)
    if opcode == 0 or opcode == 28:
        new_instr = (new_instr & ~(0x1F << 11)) | (new_rd << 11)

    return new_instr


def parse_mapping(map_str):
    """Parse 'from:to,from:to,...' mapping string into {num: num} dict."""
    mapping = {}
    for pair in map_str.split(','):
        pair = pair.strip()
        if not pair:
            continue
        parts = pair.split(':')
        if len(parts) != 2:
            print(f"Error: invalid mapping pair '{pair}', expected 'REG:REG'", file=sys.stderr)
            sys.exit(1)
        from_name, to_name = parts[0].strip(), parts[1].strip()
        if from_name not in REGS:
            print(f"Error: unknown register '{from_name}'", file=sys.stderr)
            sys.exit(1)
        if to_name not in REGS:
            print(f"Error: unknown register '{to_name}'", file=sys.stderr)
            sys.exit(1)
        mapping[REGS[from_name]] = REGS[to_name]
    return mapping


def make_t_rotation(n):
    """Create a cyclic rotation of t0-t9 by n positions.
    t-registers are: t0(8), t1(9), t2(10), t3(11), t4(12), t5(13), t6(14), t7(15), t8(24), t9(25)
    """
    t_regs = [8, 9, 10, 11, 12, 13, 14, 15, 24, 25]  # t0-t9
    mapping = {}
    for i, reg in enumerate(t_regs):
        target_reg = t_regs[(i + n) % 10]
        if reg != target_reg:
            mapping[reg] = target_reg
    return mapping


def process_elf(input_path, output_path, mapping, verbose=False):
    """Process ELF file and apply register mapping in .text section."""
    shutil.copy(input_path, output_path)

    with open(output_path, 'r+b') as f:
        magic = f.read(4)
        if magic != b'\x7fELF':
            print(f"Error: {input_path} is not an ELF file", file=sys.stderr)
            return False

        f.seek(5)
        ei_data = f.read(1)[0]
        endian_fmt = '>' if ei_data == 2 else '<'

        # Get section header info
        f.seek(32)
        e_shoff = struct.unpack(endian_fmt + 'I', f.read(4))[0]
        f.seek(46)
        e_shentsize = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shnum = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shstrndx = struct.unpack(endian_fmt + 'H', f.read(2))[0]

        # Find shstrtab offset
        f.seek(e_shoff + e_shstrndx * e_shentsize + 16)
        shstrtab_offset = struct.unpack(endian_fmt + 'I', f.read(4))[0]

        # Find .text section
        text_offset = None
        text_size = None
        for i in range(e_shnum):
            f.seek(e_shoff + i * e_shentsize)
            sh_name_idx = struct.unpack(endian_fmt + 'I', f.read(4))[0]
            pos = f.tell()
            f.seek(shstrtab_offset + sh_name_idx)
            name = b''
            while True:
                c = f.read(1)
                if c == b'\x00' or not c:
                    break
                name += c
            f.seek(pos)
            if name == b'.text':
                f.seek(e_shoff + i * e_shentsize + 16)
                text_offset = struct.unpack(endian_fmt + 'I', f.read(4))[0]
                text_size = struct.unpack(endian_fmt + 'I', f.read(4))[0]
                break

        if text_offset is None:
            print("Error: .text section not found", file=sys.stderr)
            return False

        if verbose:
            print(f"Found .text at offset {text_offset}, size {text_size}")
            print(f"Mapping: {{{', '.join(f'{REG_NAMES.get(k,str(k))}->{REG_NAMES.get(v,str(v))}' for k,v in sorted(mapping.items()))}}}")

        changes = 0
        f.seek(text_offset)
        for i in range(0, text_size, 4):
            pos = text_offset + i
            f.seek(pos)
            instr_bytes = f.read(4)
            if len(instr_bytes) < 4:
                break
            instr = struct.unpack(endian_fmt + 'I', instr_bytes)[0]
            new_instr = apply_mapping(instr, mapping)
            if new_instr != instr:
                f.seek(pos)
                f.write(struct.pack(endian_fmt + 'I', new_instr))
                changes += 1

        if verbose:
            print(f"Modified {changes} instructions")

    return True


def main():
    parser = argparse.ArgumentParser(description='Apply cyclic register rotation to MIPS ELF')
    parser.add_argument('input', help='Input .o file')
    parser.add_argument('output', help='Output .o file')
    parser.add_argument('--map', help='Register mapping: "from:to,from:to,..."')
    parser.add_argument('--rotate-t', type=int, help='Rotate t0-t9 by N positions (+4 = t0->t4, t4->t8, ...)')
    parser.add_argument('-v', '--verbose', action='store_true')
    args = parser.parse_args()

    if args.rotate_t is not None:
        mapping = make_t_rotation(args.rotate_t)
    elif args.map:
        mapping = parse_mapping(args.map)
    else:
        print("Error: specify --map or --rotate-t", file=sys.stderr)
        sys.exit(1)

    # Allow additional mappings to be combined
    if args.rotate_t is not None and args.map:
        extra = parse_mapping(args.map)
        mapping.update(extra)

    if not mapping:
        print("Warning: empty mapping, no changes to make")
        shutil.copy(args.input, args.output)
        return

    if process_elf(args.input, args.output, mapping, args.verbose):
        print(f"Wrote {args.output}")
    else:
        sys.exit(1)


if __name__ == '__main__':
    main()
