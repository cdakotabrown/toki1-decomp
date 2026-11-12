#!/usr/bin/env python3
"""cse_group_lui.py — Group same-valued lui instructions into a single $at CSE.

IDO generates `lui $at, HI` for each store to the same 64K page, then
the uopt pass CSE-eliminates redundant ones. But GCC/our compiler doesn't
do this, generating individual `lui $tN, HI` for each store instead.

This tool:
1. Finds all lui instructions with the same HI value (used as store bases)
2. Renames them all to $at (register 1)
3. Renames all store instructions' rs field from $tN to $at
4. NOPs out redundant lui $at instructions (keeping only the first per group)

This matches IDO's CSE pattern without modifying the C source.

Usage:
    python3 cse_group_lui.py input.o output.o [--verbose]
"""

import argparse
import struct
import sys
from pathlib import Path
from collections import defaultdict


MIPS_REG_AT = 1  # $at register number

OP_LUI = 0x0F
OP_ADDIU = 0x09
OP_ORI = 0x0D
OP_SW = 0x2B
OP_SH = 0x29
OP_SB = 0x28
OP_LW = 0x23
OP_LH = 0x21
OP_LB = 0x20
OP_LBU = 0x24
OP_LHU = 0x25

STORE_OPS = {OP_SW, OP_SH, OP_SB}
LOAD_OPS = {OP_LW, OP_LH, OP_LB, OP_LBU, OP_LHU}


def find_text_section(data: bytearray):
    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shentsize = struct.unpack('<H', data[46:48])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]

    shstr_hdr_off = e_shoff + (e_shstrndx * e_shentsize)
    shstr_off = struct.unpack('<I', data[shstr_hdr_off + 16:shstr_hdr_off + 20])[0]

    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack('<I', data[sh_off:sh_off + 4])[0]
        sec_off = struct.unpack('<I', data[sh_off + 16:sh_off + 20])[0]
        sec_size = struct.unpack('<I', data[sh_off + 20:sh_off + 24])[0]

        n0 = shstr_off + sh_name_idx
        n1 = data.find(b'\x00', n0)
        sec_name = data[n0:n1].decode('ascii', errors='ignore')

        if sec_name == '.text':
            return sec_off, sec_size

    raise ValueError("No .text section found")


def decode(word):
    op = (word >> 26) & 0x3f
    rs = (word >> 21) & 0x1f
    rt = (word >> 16) & 0x1f
    rd = (word >> 11) & 0x1f
    imm = word & 0xffff
    return op, rs, rt, rd, imm


def encode_lui(rt, hi):
    return (OP_LUI << 26) | (rt << 16) | (hi & 0xffff)


def encode_store(op, rt, base, offset):
    return (op << 26) | (base << 21) | (rt << 16) | (offset & 0xffff)


def main():
    parser = argparse.ArgumentParser(description="Group same-valued lui into $at CSE")
    parser.add_argument("input", type=Path, help="Input ELF object file")
    parser.add_argument("output", type=Path, help="Output ELF object file")
    parser.add_argument("--keep-all", action="store_true",
                        help="Keep all lui $at (just rename, don't NOP redundant ones)")
    parser.add_argument("-v", "--verbose", action="store_true")
    args = parser.parse_args()

    data = bytearray(args.input.read_bytes())
    text_off, text_size = find_text_section(data)

    # First pass: find all lui instructions and track their HI values
    lui_positions = []  # (offset, rt, hi)
    for off in range(0, text_size, 4):
        abs_off = text_off + off
        word = struct.unpack('<I', data[abs_off:abs_off + 4])[0]
        op, rs, rt, rd, imm = decode(word)
        if op == OP_LUI and rt != 0:  # Skip lui $zero
            lui_positions.append((off, rt, imm))

    # Build register usage map: for each register, what instructions use it as base
    store_positions = []  # (offset, op, rt, base_reg, offset_imm)
    for off in range(0, text_size, 4):
        abs_off = text_off + off
        word = struct.unpack('<I', data[abs_off:abs_off + 4])[0]
        op, rs, rt, rd, imm = decode(word)
        if op in STORE_OPS or op in LOAD_OPS:
            store_positions.append((off, op, rt, rs, imm))

    # Find which lui registers are used as store/load bases
    # A lui is a "base lui" if its destination register is used as rs in a store/load
    base_lui_regs = set()
    for lui_off, lui_rt, lui_hi in lui_positions:
        for s_off, s_op, s_rt, s_rs, s_imm in store_positions:
            if s_rs == lui_rt:
                base_lui_regs.add(lui_rt)
                break

    # Group base lui by HI value
    hi_groups = defaultdict(list)  # hi -> [(offset, rt)]
    for lui_off, lui_rt, lui_hi in lui_positions:
        if lui_rt in base_lui_regs:
            hi_groups[lui_hi].append((lui_off, lui_rt))

    if args.verbose:
        print(f"Found {len(lui_positions)} lui instructions")
        print(f"Base lui regs: {base_lui_regs}")
        print(f"HI groups: {dict(hi_groups)}")

    # For each group with >1 member, consolidate to $at
    changed = 0
    for hi, members in hi_groups.items():
        if len(members) < 2:
            continue

        if args.verbose:
            print(f"  Consolidating {len(members)} lui with hi=0x{hi:04x} → $at")

        # Check if $at is free (not used by any other instruction)
        at_used = False
        for off in range(0, text_size, 4):
            abs_off = text_off + off
            word = struct.unpack('<I', data[abs_off:abs_off + 4])[0]
            op, rs, rt, rd, imm = decode(word)
            # $at is reg 1
            if rs == MIPS_REG_AT or rt == MIPS_REG_AT or rd == MIPS_REG_AT:
                # Skip the lui $at instructions we're about to create
                if not (op == OP_LUI and rt == MIPS_REG_AT and imm == hi):
                    at_used = True
                    break

        if at_used:
            if args.verbose:
                print(f"    $at already in use, skipping group")
            continue

        # Track which registers are being consolidated
        old_regs = set(rt for _, rt in members)

        # For the first lui, change rt to $at
        first_off, first_rt = members[0]
        abs_off = text_off + first_off
        old_word = struct.unpack('<I', data[abs_off:abs_off + 4])[0]
        # Clear rt field (bits 20:16) and set to $at
        new_word = (old_word & 0xffe0ffff) | (MIPS_REG_AT << 16)
        struct.pack_into('<I', data, abs_off, new_word)
        if args.verbose:
            print(f"    Offset 0x{first_off:x}: lui ${first_rt} → lui $at")
        changed += 1

        # For remaining lui, either NOP or rename to $at
        for lui_off, lui_rt in members[1:]:
            abs_off = text_off + lui_off
            old_word = struct.unpack('<I', data[abs_off:abs_off + 4])[0]
            if args.keep_all:
                # Keep but rename to $at
                new_word = (old_word & 0xffe0ffff) | (MIPS_REG_AT << 16)
                struct.pack_into('<I', data, abs_off, new_word)
                if args.verbose:
                    print(f"    Offset 0x{lui_off:x}: lui ${lui_rt} → lui $at (kept)")
            else:
                # NOP out
                struct.pack_into('<I', data, abs_off, 0)  # NOP
                if args.verbose:
                    print(f"    Offset 0x{lui_off:x}: lui ${lui_rt} → NOP")
            changed += 1

        # Rename store/load rs from old_regs to $at
        for s_off, s_op, s_rt, s_rs, s_imm in store_positions:
            if s_rs in old_regs:
                abs_off = text_off + s_off
                old_word = struct.unpack('<I', data[abs_off:abs_off + 4])[0]
                # Clear rs field (bits 25:21) and set to $at
                new_word = (old_word & 0xfc1fffff) | (MIPS_REG_AT << 21)
                struct.pack_into('<I', data, abs_off, new_word)
                if args.verbose:
                    print(f"    Offset 0x{s_off:x}: store rs ${s_rs} → $at")
                changed += 1

    args.output.write_bytes(data)
    print(f"  cse_group_lui: {changed} changes made")
    # A no-op is a SUCCESSFUL no-op: the (unchanged) output object was written
    # and is valid. Exiting non-zero here made apply_postproc abort the whole
    # directive chain — silently dropping every later step — whenever the CSE
    # grouping was already present or inapplicable. Reserve non-zero for real
    # errors (handled above); "0 changes" is exit 0.
    return 0


if __name__ == "__main__":
    sys.exit(main())
