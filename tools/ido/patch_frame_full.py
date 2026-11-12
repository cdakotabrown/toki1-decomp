#!/usr/bin/env python3
"""
Full stack frame patching for ELF object files.

This tool patches:
1. addiu sp,sp,-N (prologue) and addiu sp,sp,N (epilogue)
2. sw ra,offset(sp) and lw ra,offset(sp)
3. sw sX,offset(sp) and lw sX,offset(sp) for callee-saved s0-s7

Based on research from SM64/OOT decomp projects: frame size changes
require updating ALL stack-relative offsets in the save/restore area.

Usage:
    python3 patch_frame_full.py input.o output.o --old 24 --new 40
"""

import sys
import struct
import argparse


def decode_mips_insn(word):
    """Decode a MIPS instruction into components."""
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    imm = word & 0xFFFF

    # Sign-extend immediate
    if imm & 0x8000:
        imm_signed = imm - 0x10000
    else:
        imm_signed = imm

    return op, rs, rt, rd, imm, imm_signed


def encode_mips_load_store(op, base, rt, offset):
    """Encode a MIPS load/store instruction."""
    return (op << 26) | (base << 21) | (rt << 16) | (offset & 0xFFFF)


def encode_mips_addiu(rt, rs, imm):
    """Encode an addiu instruction."""
    return (0x09 << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


# MIPS register numbers
REG_SP = 29
REG_RA = 31
REG_S0 = 16  # s0-s7 are registers 16-23

# MIPS opcodes
OP_LW = 0x23
OP_SW = 0x2B
OP_ADDIU = 0x09


def patch_frame_full(elf_data, old_size, new_size, verbose=False):
    """
    Patch stack frame size and all related offsets.

    Args:
        elf_data: Raw ELF file bytes
        old_size: Original frame size (e.g., 24)
        new_size: Target frame size (e.g., 40)
        verbose: Print detailed changes

    Returns:
        (patched_data, stats_dict)
    """
    patched = bytearray(elf_data)
    delta = new_size - old_size

    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

    # Parse ELF header
    e_shoff = struct.unpack(endian + 'I', elf_data[32:36])[0]
    e_shentsize = struct.unpack(endian + 'H', elf_data[46:48])[0]
    e_shnum = struct.unpack(endian + 'H', elf_data[48:50])[0]
    e_shstrndx = struct.unpack(endian + 'H', elf_data[50:52])[0]

    # Get section header string table
    shstrtab_off = e_shoff + e_shstrndx * e_shentsize
    shstrtab_addr = struct.unpack(endian + 'I', elf_data[shstrtab_off+16:shstrtab_off+20])[0]
    shstrtab_size = struct.unpack(endian + 'I', elf_data[shstrtab_off+20:shstrtab_off+24])[0]
    shstrtab = elf_data[shstrtab_addr:shstrtab_addr+shstrtab_size]

    text_offset = None
    text_size = None
    for i in range(e_shnum):
        sh_offset = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack(endian + 'I', elf_data[sh_offset:sh_offset+4])[0]
        sh_offset_data = struct.unpack(endian + 'I', elf_data[sh_offset+16:sh_offset+20])[0]
        sh_size = struct.unpack(endian + 'I', elf_data[sh_offset+20:sh_offset+24])[0]

        name_end = shstrtab.find(b'\x00', sh_name_idx)
        name = shstrtab[sh_name_idx:name_end].decode('ascii')

        if name == '.text':
            text_offset = sh_offset_data
            text_size = sh_size
            break

    if text_offset is None:
        raise ValueError(".text section not found")

    stats = {
        'prologue': 0,
        'epilogue': 0,
        'ra_save': 0,
        'ra_restore': 0,
        's_save': 0,
        's_restore': 0,
    }

    if verbose:
        print(f"Frame size: {old_size} -> {new_size} (delta={delta})")
        print(f".text at 0x{text_offset:x}, size 0x{text_size:x}")

    # First pass: find all function prologues to understand save area
    # Second pass: patch instructions

    for i in range(0, text_size, 4):
        file_off = text_offset + i
        word = struct.unpack(endian + 'I', elf_data[file_off:file_off+4])[0]
        op, rs, rt, rd, imm, imm_signed = decode_mips_insn(word)

        # Check for addiu sp,sp,N
        if op == OP_ADDIU and rs == REG_SP and rt == REG_SP:
            if imm_signed == -old_size:
                # Prologue: addiu sp,sp,-old_size
                new_word = encode_mips_addiu(REG_SP, REG_SP, -new_size)
                struct.pack_into(endian + 'I', patched, file_off, new_word)
                stats['prologue'] += 1
                if verbose:
                    print(f"  0x{i:04x}: prologue addiu sp,sp,{imm_signed} -> {-new_size}")
            elif imm_signed == old_size:
                # Epilogue: addiu sp,sp,old_size
                new_word = encode_mips_addiu(REG_SP, REG_SP, new_size)
                struct.pack_into(endian + 'I', patched, file_off, new_word)
                stats['epilogue'] += 1
                if verbose:
                    print(f"  0x{i:04x}: epilogue addiu sp,sp,{imm_signed} -> {new_size}")

        # Check for sw/lw with sp base
        elif op in (OP_SW, OP_LW) and rs == REG_SP:
            # Only patch offsets in the save area (typically >= frame_size - 8 for RA)
            # We patch if offset >= old_size - 8 (conservative: patch RA and below)
            # Actually, patch all positive offsets relative to old frame

            # The save area is at the TOP of the frame
            # For old_size=24: RA at 20(sp), locals below
            # For old_size=40: RA at 36(sp) after patch

            # Heuristic: patch offsets that are in the old save area
            # Typically RA is at old_size-4, s-regs below that

            # More conservative: patch if this looks like a callee-saved reg
            is_ra = (rt == REG_RA)
            is_s_reg = (REG_S0 <= rt <= REG_S0 + 7)  # s0-s7

            # Patch RA and s-reg saves/restores
            if is_ra or is_s_reg:
                new_offset = imm_signed + delta
                new_word = encode_mips_load_store(op, REG_SP, rt, new_offset)
                struct.pack_into(endian + 'I', patched, file_off, new_word)

                if op == OP_SW:
                    if is_ra:
                        stats['ra_save'] += 1
                    else:
                        stats['s_save'] += 1
                else:
                    if is_ra:
                        stats['ra_restore'] += 1
                    else:
                        stats['s_restore'] += 1

                if verbose:
                    reg_name = 'ra' if is_ra else f's{rt - REG_S0}'
                    op_name = 'sw' if op == OP_SW else 'lw'
                    print(f"  0x{i:04x}: {op_name} {reg_name},{imm_signed}(sp) -> {new_offset}(sp)")

    return bytes(patched), stats


def main():
    parser = argparse.ArgumentParser(
        description='Full stack frame patching (frame size + save/restore offsets)',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
    # Patch from IDO's 24-byte to game's 40-byte frame
    %(prog)s input.o output.o --old 24 --new 40

    # Verbose output
    %(prog)s input.o output.o --old 24 --new 40 -v
"""
    )
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('--old', '--old-size', type=int, default=24,
                        dest='old_size', help='Old frame size (default: 24)')
    parser.add_argument('--new', '--new-size', type=int, default=40,
                        dest='new_size', help='New frame size (default: 40)')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Print detailed changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, stats = patch_frame_full(elf_data, args.old_size, args.new_size, args.verbose)

    with open(args.output, 'wb') as f:
        f.write(patched)

    total = sum(stats.values())
    print(f"Patched {total} instructions:")
    print(f"  Prologue: {stats['prologue']}")
    print(f"  Epilogue: {stats['epilogue']}")
    print(f"  RA save/restore: {stats['ra_save']}/{stats['ra_restore']}")
    print(f"  S-reg save/restore: {stats['s_save']}/{stats['s_restore']}")


if __name__ == '__main__':
    main()
