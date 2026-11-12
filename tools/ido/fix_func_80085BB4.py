#!/usr/bin/env python3
"""
Custom post-processing script for serifu_run_step.

Cross-prologue thunk with hidden params t9/t0 stored to globals.
IDO compiles the two global stores using a0/a1, but target uses t9/t0.

After strip_leading_instructions(1), strip_arg_homes(0x14), patch_stack_complete:
  Compiled idx 0: sw a0,0(at)      -> target idx 2: sw t9,1600(at)
  Compiled idx 1: addiu sp,sp,-40  -> target idx 0
  Compiled idx 2: lui at,0x0       -> target idx 4: lui at,0x800d
  Compiled idx 3: lui a0,0x0       -> target idx 1: lui a0,0x8013
  Compiled idx 4: sw ra,20(sp)     -> target idx 3
  Compiled idx 5: sw a1,0(at)      -> target idx 9: sw t0,1604(at)
  Compiled idx 6: lbu a0,0(a0)     -> target idx 5
  Compiled idx 7: li a1,1          -> target idx 6
  Compiled idx 8: jal ...          -> target idx 8
  Compiled idx 9: move a2,zero     -> target idx 7
  10-18: already match

Transforms:
  1. Reorder first 10 instructions: [1,3,0,4,2,6,7,9,8,5]
  2. Patch sw at idx 2 (after reorder): a0(rt) -> t9(rt)
  3. Patch sw at idx 9 (after reorder): a1(rt) -> t0(rt)

Usage: python3 fix_func_80085BB4.py input.o output.o
"""

import struct
import sys


def read_elf_sections(data):
    e_shoff = struct.unpack_from('<I', data, 32)[0]
    e_shentsize = struct.unpack_from('<H', data, 46)[0]
    e_shnum = struct.unpack_from('<H', data, 48)[0]
    e_shstrndx = struct.unpack_from('<H', data, 50)[0]

    shstr_shoff = e_shoff + e_shstrndx * e_shentsize
    shstr_offset = struct.unpack_from('<I', data, shstr_shoff + 16)[0]

    sections = {}
    for i in range(e_shnum):
        shoff = e_shoff + i * e_shentsize
        sh_name_off = struct.unpack_from('<I', data, shoff)[0]
        sh_offset = struct.unpack_from('<I', data, shoff + 16)[0]
        sh_size = struct.unpack_from('<I', data, shoff + 20)[0]
        sh_entsize = struct.unpack_from('<I', data, shoff + 36)[0]

        name_end = data.index(0, shstr_offset + sh_name_off)
        name = data[shstr_offset + sh_name_off:name_end].decode()
        sections[name] = {
            'offset': sh_offset, 'size': sh_size,
            'entsize': sh_entsize, 'elf_offset': shoff
        }
    return sections


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.o output.o", file=sys.stderr)
        sys.exit(1)

    with open(sys.argv[1], 'rb') as f:
        data = bytearray(f.read())

    sections = read_elf_sections(data)
    text = sections['.text']
    text_off = text['offset']

    def instr_at(idx):
        return struct.unpack_from('<I', data, text_off + idx * 4)[0]

    def set_instr(idx, val):
        struct.pack_into('<I', data, text_off + idx * 4, val)

    # Read first 10 instructions
    orig = [instr_at(i) for i in range(10)]

    # Reorder: compiled [0,1,2,3,4,5,6,7,8,9] -> target order [1,3,0,4,2,6,7,9,8,5]
    reorder = [1, 3, 0, 4, 2, 6, 7, 9, 8, 5]
    reordered = [orig[i] for i in reorder]

    for i, insn in enumerate(reordered):
        set_instr(i, insn)

    # Now patch register fields:
    # idx 2: sw a0,0(at) -> sw t9,0(at)  (rt field: a0=4 -> t9=25)
    w = instr_at(2)
    # sw format: 101011 rs(5) rt(5) imm(16), rt is bits 20:16
    rt = (w >> 16) & 0x1f
    assert rt == 4, f"Expected a0(4) at idx 2 rt, got {rt}"
    w = (w & ~(0x1f << 16)) | (25 << 16)  # t9 = 25
    set_instr(2, w)

    # idx 9: sw a1,0(at) -> sw t0,0(at)  (rt field: a1=5 -> t0=8)
    w = instr_at(9)
    rt = (w >> 16) & 0x1f
    assert rt == 5, f"Expected a1(5) at idx 9 rt, got {rt}"
    w = (w & ~(0x1f << 16)) | (8 << 16)  # t0 = 8
    set_instr(9, w)

    # Fix relocations: reorder entries that reference the first 10 instructions
    if '.rel.text' in sections:
        rel = sections['.rel.text']
        rel_off = rel['offset']
        rel_size = rel['size']
        num_rels = rel_size // 8

        # Build offset mapping: old_byte_offset -> new_byte_offset
        offset_map = {}
        for new_idx, old_idx in enumerate(reorder):
            offset_map[old_idx * 4] = new_idx * 4

        for i in range(num_rels):
            r_offset = struct.unpack_from('<I', data, rel_off + i * 8)[0]
            if r_offset in offset_map:
                new_offset = offset_map[r_offset]
                struct.pack_into('<I', data, rel_off + i * 8, new_offset)

    with open(sys.argv[2], 'wb') as f:
        f.write(data)

    print("fix_func_80085BB4.py: patches applied successfully", file=sys.stderr)


if __name__ == '__main__':
    main()
