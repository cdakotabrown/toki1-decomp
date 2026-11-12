#!/usr/bin/env python3
"""
remap_stack_slots.py - Remap stack slot offsets in compiled MIPS object files

Usage:
    python3 remap_stack_slots.py input.o output.o --map '84:88,96:92'

Changes immediate offsets in lw/sw/lb/sb/lh/sh/lbu/lhu instructions that
reference $sp. Only modifies instructions where the base register is $sp (29).

The map is applied SIMULTANEOUSLY (it is a dict consulted once per instruction),
so `--map '88:100,100:88'` really is a swap and needs no scratch offset.

--at restricts the rewrite to the listed .text byte offsets.  This is the
SLOT-SPLIT case, which a whole-function map cannot express: uopt recycles one
dead slot for a second, non-overlapping value where retail parks that value in a
different free slot, so only SOME references to an offset must move.  A blanket
map rewrites the other references too and breaks them.  Splitting two provably
disjoint live ranges across two slots changes no value and no instruction count
-- but you must CHECK the ranges are disjoint before using this, because the
tool cannot: list the offsets only after reading the def-use chains.
"""

import argparse
import struct
import sys
import shutil

SP_REG = 29


def remap_stack_in_instruction(instr, slot_map, endian_fmt, keep_frame=False):
    """Remap stack slot offset in a single MIPS instruction.
    Only modifies I-type load/store instructions with base register = sp."""
    opcode = (instr >> 26) & 0x3F
    rs = (instr >> 21) & 0x1F  # base register

    # Process load/store I-type with base = sp, plus addiu with rs = sp
    # lw=0x23, sw=0x2b, lb=0x20, sb=0x28, lh=0x21, sh=0x29, lbu=0x24, lhu=0x25
    # addiu=0x09 (for stack address computations like addiu reg, sp, offset)
    load_store_ops = {0x09, 0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2b}

    if opcode not in load_store_ops or rs != SP_REG:
        return instr, False

    # A frame adjustment (`addiu $sp,$sp,N`) is not a stack SLOT reference: its
    # destination IS the stack pointer.  Remapping it silently rewrites the
    # prologue or epilogue and leaves the frame unbalanced.  Off by default so
    # that no existing map changes behaviour; pass --keep-frame when a slot
    # offset happens to collide with the frame size.
    if keep_frame and opcode == 0x09 and ((instr >> 16) & 0x1F) == SP_REG:
        return instr, False

    # Extract signed 16-bit immediate
    imm = instr & 0xFFFF
    if imm & 0x8000:
        imm_signed = imm - 0x10000
    else:
        imm_signed = imm

    if imm_signed in slot_map:
        new_imm = slot_map[imm_signed]
        new_instr = (instr & 0xFFFF0000) | (new_imm & 0xFFFF)
        return new_instr, True

    return instr, False


def parse_slot_map(map_str):
    """Parse 'old:new,old:new,...' mapping string into {int: int} dict."""
    mapping = {}
    for pair in map_str.split(','):
        pair = pair.strip()
        if not pair:
            continue
        parts = pair.split(':')
        if len(parts) != 2:
            print(f"Error: invalid mapping pair '{pair}'", file=sys.stderr)
            sys.exit(1)
        old_off = int(parts[0].strip())
        new_off = int(parts[1].strip())
        mapping[old_off] = new_off
    return mapping


def process_elf(input_path, output_path, slot_map, verbose=False, keep_frame=False,
                only_at=None):
    """Process ELF file and remap stack slots in .text section."""
    shutil.copy(input_path, output_path)

    with open(output_path, 'r+b') as f:
        magic = f.read(4)
        if magic != b'\x7fELF':
            print(f"Error: {input_path} is not an ELF file", file=sys.stderr)
            return False

        f.seek(5)
        ei_data = f.read(1)[0]
        endian_fmt = '>' if ei_data == 2 else '<'

        f.seek(32)
        e_shoff = struct.unpack(endian_fmt + 'I', f.read(4))[0]
        f.seek(46)
        e_shentsize = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shnum = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shstrndx = struct.unpack(endian_fmt + 'H', f.read(2))[0]

        f.seek(e_shoff + e_shstrndx * e_shentsize + 16)
        shstrtab_offset = struct.unpack(endian_fmt + 'I', f.read(4))[0]

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
            print(f"Slot mapping: {slot_map}")
            if only_at is not None:
                print("Restricted to .text offsets: "
                      + ",".join(hex(o) for o in sorted(only_at)))

        changes = 0
        f.seek(text_offset)
        for i in range(0, text_size, 4):
            pos = text_offset + i
            f.seek(pos)
            instr_bytes = f.read(4)
            if len(instr_bytes) < 4:
                break
            instr = struct.unpack(endian_fmt + 'I', instr_bytes)[0]
            if only_at is not None and i not in only_at:
                continue
            new_instr, changed = remap_stack_in_instruction(instr, slot_map, endian_fmt, keep_frame=keep_frame)
            if changed:
                if verbose:
                    print(f"  0x{i:04x}: 0x{instr:08x} -> 0x{new_instr:08x}")
                f.seek(pos)
                f.write(struct.pack(endian_fmt + 'I', new_instr))
                changes += 1

        if verbose:
            print(f"Modified {changes} instructions")
        if only_at is not None and changes != len(only_at):
            print(f"remap_stack_slots: ERROR --at named {len(only_at)} offset(s) "
                  f"but {changes} instruction(s) matched the map. An --at offset that\n"
                  f"  does not hold an $sp reference with a mapped immediate is a stale\n"
                  f"  directive, not a no-op -- refusing.", file=sys.stderr)
            return False

    return True


def main():
    parser = argparse.ArgumentParser(description='Remap stack slot offsets in MIPS ELF')
    parser.add_argument('input', help='Input .o file')
    parser.add_argument('output', help='Output .o file')
    parser.add_argument('--map', required=True, help='Slot mapping: "old:new,old:new,..."')
    parser.add_argument('--keep-frame', action='store_true',
                        help='do not rewrite `addiu $sp,$sp,N` frame adjustments '
                             'even when N is a key in the map')
    parser.add_argument('--at', default=None,
                        help='restrict the rewrite to these .text BYTE offsets '
                             '(comma-separated, 0x-prefixed allowed). Use for a '
                             'slot SPLIT, where only some references to an offset move.')
    parser.add_argument('-v', '--verbose', action='store_true')
    args = parser.parse_args()

    slot_map = parse_slot_map(args.map)
    if not slot_map:
        print("Warning: empty mapping")
        shutil.copy(args.input, args.output)
        return

    only_at = None
    if args.at:
        only_at = set()
        for tok in args.at.split(','):
            tok = tok.strip()
            if tok:
                only_at.add(int(tok, 0))

    if process_elf(args.input, args.output, slot_map, args.verbose,
                   keep_frame=args.keep_frame, only_at=only_at):
        print(f"Wrote {args.output}")
    else:
        sys.exit(1)


if __name__ == '__main__':
    main()
