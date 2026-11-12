#!/usr/bin/env python3
"""
swap_registers.py - Swap register usage in compiled MIPS object files

Usage:
    python3 swap_registers.py input.o output.o --from at --to v0

This tool swaps all occurrences of one register with another in the
.text section of an ELF object file. Useful for fixing register
allocation differences between compilers.
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
    'gp': 28, 'sp': 29, 'fp': 30, 'ra': 31
}

def swap_register_in_instruction(instr, from_reg, to_reg, big_endian=True, bidirectional=False):
    """Swap register in a single MIPS instruction.

    If bidirectional=False (default): only replaces from_reg -> to_reg.
    If bidirectional=True: swaps both directions (from_reg <-> to_reg) using
    snapshot semantics (reads original values, then writes all at once).
    """
    changed = False

    opcode = (instr >> 26) & 0x3F
    rs = (instr >> 21) & 0x1F
    rt = (instr >> 16) & 0x1F
    rd = (instr >> 11) & 0x1F

    new_rs, new_rt, new_rd = rs, rt, rd

    # Swap rs
    if rs == from_reg:
        new_rs = to_reg
        changed = True
    elif bidirectional and rs == to_reg:
        new_rs = from_reg
        changed = True

    # Swap rt
    if rt == from_reg:
        new_rt = to_reg
        changed = True
    elif bidirectional and rt == to_reg:
        new_rt = from_reg
        changed = True

    # Swap rd (only for R-type instructions)
    if opcode == 0:
        if rd == from_reg:
            new_rd = to_reg
            changed = True
        elif bidirectional and rd == to_reg:
            new_rd = from_reg
            changed = True

    if changed:
        instr = (instr & ~(0x1F << 21)) | (new_rs << 21)
        instr = (instr & ~(0x1F << 16)) | (new_rt << 16)
        if opcode == 0:
            instr = (instr & ~(0x1F << 11)) | (new_rd << 11)

    return instr, changed

def process_elf(input_path, output_path, from_reg, to_reg, verbose=False, exclude_offsets=None, only_offsets=None, bidirectional=False):
    """Process ELF file and swap registers in .text section"""
    if exclude_offsets is None:
        exclude_offsets = set()
    if only_offsets is not None:
        only_offsets = set(only_offsets)

    # Copy file
    shutil.copy(input_path, output_path)
    
    with open(output_path, 'r+b') as f:
        # Read ELF header
        magic = f.read(4)
        if magic != b'\x7fELF':
            print(f"Error: {input_path} is not an ELF file", file=sys.stderr)
            return False
        
        # Check endianness
        f.seek(5)
        ei_data = f.read(1)[0]
        big_endian = (ei_data == 2)
        endian_fmt = '>' if big_endian else '<'
        
        # Get section header info
        f.seek(32 if ei_data else 40)
        e_shoff = struct.unpack(endian_fmt + 'I', f.read(4))[0]
        f.seek(46 if ei_data else 48)
        e_shentsize = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shnum = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shstrndx = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        
        # Find .text section
        f.seek(e_shoff + e_shstrndx * e_shentsize)
        f.seek(16, 1)  # Skip to sh_offset
        shstrtab_offset = struct.unpack(endian_fmt + 'I', f.read(4))[0]
        
        text_offset = None
        text_size = None
        
        for i in range(e_shnum):
            f.seek(e_shoff + i * e_shentsize)
            sh_name_idx = struct.unpack(endian_fmt + 'I', f.read(4))[0]
            
            # Read section name
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
        
        # Process instructions
        changes = 0
        f.seek(text_offset)
        for i in range(0, text_size, 4):
            pos = text_offset + i
            f.seek(pos)
            instr_bytes = f.read(4)
            if len(instr_bytes) < 4:
                break

            if only_offsets is not None and i not in only_offsets:
                continue

            if i in exclude_offsets:
                if verbose:
                    print(f"  Offset {i:04x}: EXCLUDED from swap")
                continue

            instr = struct.unpack(endian_fmt + 'I', instr_bytes)[0]
            new_instr, changed = swap_register_in_instruction(instr, from_reg, to_reg, big_endian, bidirectional)
            
            if changed:
                if verbose:
                    print(f"  Offset {i:04x}: {instr:08x} -> {new_instr:08x}")
                f.seek(pos)
                f.write(struct.pack(endian_fmt + 'I', new_instr))
                changes += 1
        
        if verbose or changes > 0:
            print(f"Swapped {changes} register occurrences (${REGS.get(from_reg, from_reg)} -> ${REGS.get(to_reg, to_reg)})")
    
    return True

def main():
    parser = argparse.ArgumentParser(description='Swap registers in MIPS ELF object files')
    parser.add_argument('input', help='Input .o file')
    parser.add_argument('output', help='Output .o file')
    parser.add_argument('--from', dest='from_reg', required=True, help='Source register (name or number)')
    parser.add_argument('--to', dest='to_reg', required=True, help='Target register (name or number)')
    parser.add_argument('--exclude-offsets',
                       help='Comma-separated .text offsets to skip (e.g., "0xFC,0x10")')
    parser.add_argument('--only',
                       help='Comma-separated .text offsets to apply swap ONLY at (e.g., "0x04,0x10")')
    parser.add_argument('--bidirectional', action='store_true',
                       help='Swap both directions (from<->to) instead of just from->to')
    parser.add_argument('-v', '--verbose', action='store_true', help='Verbose output')

    args = parser.parse_args()
    
    # Convert register names to numbers
    from_reg = REGS.get(args.from_reg, None)
    if from_reg is None:
        try:
            from_reg = int(args.from_reg)
        except ValueError:
            print(f"Unknown register: {args.from_reg}", file=sys.stderr)
            sys.exit(1)
    
    to_reg = REGS.get(args.to_reg, None)
    if to_reg is None:
        try:
            to_reg = int(args.to_reg)
        except ValueError:
            print(f"Unknown register: {args.to_reg}", file=sys.stderr)
            sys.exit(1)
    
    exclude_offsets = set()
    if args.exclude_offsets:
        for offset_str in args.exclude_offsets.split(','):
            offset_str = offset_str.strip()
            if offset_str:
                exclude_offsets.add(int(offset_str, 0))

    only_offsets = None
    if args.only:
        only_offsets = set()
        for offset_str in args.only.split(','):
            offset_str = offset_str.strip()
            if offset_str:
                only_offsets.add(int(offset_str, 0))

    if process_elf(args.input, args.output, from_reg, to_reg, args.verbose, exclude_offsets, only_offsets, args.bidirectional):
        sys.exit(0)
    else:
        sys.exit(1)

if __name__ == '__main__':
    main()
