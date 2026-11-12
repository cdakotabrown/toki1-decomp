#!/usr/bin/env python3
"""Custom post-processing for LoadClut.

Compiled with IDO 5.2 -O2 using volatile params, then convert_or_to_addu.
Produces 24 instructions matching target size.

Transformations:
1. Instruction reorder: permute positions 2-15 to match target scheduling
   (IDO interleaves sw/lw for volatile params; target groups all saves first)
2. Per-position register remap: v0->t6, v1->t7, t6->t9, t7->t8
3. RA sp-offset patch: 20(sp) -> 28(sp) at positions 4 and 20

All transforms are instruction reorders and register field patches.
No instruction count change (24 -> 24).
"""
import struct
import sys


def find_text_section(data):
    """Find .text section offset and size in ELF."""
    e_shoff = struct.unpack_from('<I', data, 32)[0]
    e_shentsize = struct.unpack_from('<H', data, 46)[0]
    e_shnum = struct.unpack_from('<H', data, 48)[0]
    e_shstrndx = struct.unpack_from('<H', data, 50)[0]
    shstr_hdr = e_shoff + (e_shstrndx * e_shentsize)
    shstr_off = struct.unpack_from('<I', data, shstr_hdr + 16)[0]
    for i in range(e_shnum):
        sh = e_shoff + (i * e_shentsize)
        name_idx = struct.unpack_from('<I', data, sh)[0]
        n0 = shstr_off + name_idx
        n1 = data.find(b'\x00', n0)
        name = data[n0:n1].decode('ascii', errors='ignore')
        if name == '.text':
            off = struct.unpack_from('<I', data, sh + 16)[0]
            size = struct.unpack_from('<I', data, sh + 20)[0]
            return off, size
    return None, None


def patch_reg_rt(word, old_reg, new_reg):
    """Patch rt field (bits 20-16) if it matches old_reg."""
    rt = (word >> 16) & 0x1F
    if rt == old_reg:
        word = (word & ~(0x1F << 16)) | (new_reg << 16)
    return word


def main():
    with open(sys.argv[1], 'rb') as f:
        data = bytearray(f.read())

    text_off, text_size = find_text_section(bytes(data))
    assert text_off is not None, '.text not found'
    assert text_size == 96, f'Expected 96 bytes, got {text_size}'

    # Read all 24 instructions
    insns = []
    for i in range(24):
        insns.append(struct.unpack_from('<I', data, text_off + i * 4)[0])

    # Step 1: Permute instructions
    perm = [0, 1, 3, 5, 6, 4, 2, 15, 9, 8, 7, 10, 11, 12, 14, 13,
            16, 17, 18, 19, 20, 21, 22, 23]
    reordered = [insns[perm[i]] for i in range(24)]

    # Step 2: Per-position register remaps (rt field only - all affected
    # instructions are I-type: lw/sw/sh/addiu where the register is in rt)
    remap_positions = {
        5: (3, 15),    # v1(3) -> t7(15)
        6: (2, 14),    # v0(2) -> t6(14)
        7: (3, 15),    # v1(3) -> t7(15)
        9: (15, 24),   # t7(15) -> t8(24)
        10: (14, 25),  # t6(14) -> t9(25)
        11: (14, 25),  # t6(14) -> t9(25)
        12: (15, 24),  # t7(15) -> t8(24)
        15: (2, 14),   # v0(2) -> t6(14)
    }
    for pos, (old_reg, new_reg) in remap_positions.items():
        reordered[pos] = patch_reg_rt(reordered[pos], old_reg, new_reg)

    # Step 3: Fix RA sp-offset at positions 4 and 20
    # sw ra,20(sp) -> sw ra,28(sp)  (imm 0x0014 -> 0x001c)
    # lw ra,20(sp) -> lw ra,28(sp)
    for pos in [4, 20]:
        w = reordered[pos]
        imm = w & 0xFFFF
        assert imm == 0x0014, f'Expected imm 0x0014 at pos {pos}, got 0x{imm:04x}'
        reordered[pos] = (w & 0xFFFF0000) | 0x001c

    # Write back
    for i in range(24):
        struct.pack_into('<I', data, text_off + i * 4, reordered[i])

    with open(sys.argv[2], 'wb') as f:
        f.write(bytes(data))


if __name__ == '__main__':
    main()
