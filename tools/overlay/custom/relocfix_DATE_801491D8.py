#!/usr/bin/env python3
"""Relocation-only fixup for DATE/func_801491D8.

Same class as relocfix_DATE_80136F98.py (allowed reloc-fixup tier): IDO
assigns the two loop base addresses in the opposite registers from the
target (ours: a1=screen_w, a3=Sprites; target: a1=Sprites,
a3=screen_w). patch_insn_reg_field paints the register fields, but the
%hi/%lo relocation entries then ride on the swapped symbols. This script
permutes ONLY the .rel.text r_info symbol indices of the two HI16 and two
LO16 entries. No .text byte is modified.

  word 7  (lui  a3,%hi) : HI16  ours Sprites <-> target screen_w
  word 8  (lui  a1,%hi) : HI16  ours screen_w <-> target Sprites
  word 9  (addiu a1,%lo): LO16  ours screen_w <-> target Sprites
  word 10 (addiu a3,%lo): LO16  ours Sprites <-> target screen_w

  r_offset = 0x10 + 4*word  (0x10 = 4-word overlay thunk prefix)
"""
import sys
import struct

R_MIPS_HI16 = 5
R_MIPS_LO16 = 6

SWAP = [(0x10 + 4 * 7, R_MIPS_HI16, 0x10 + 4 * 8),
        (0x10 + 4 * 8, R_MIPS_HI16, 0x10 + 4 * 7),
        (0x10 + 4 * 9, R_MIPS_LO16, 0x10 + 4 * 10),
        (0x10 + 4 * 10, R_MIPS_LO16, 0x10 + 4 * 9)]


def find_section(data, want):
    e_shoff = struct.unpack_from('<I', data, 0x20)[0]
    e_shentsize = struct.unpack_from('<H', data, 0x2E)[0]
    e_shnum = struct.unpack_from('<H', data, 0x30)[0]
    e_shstrndx = struct.unpack_from('<H', data, 0x32)[0]
    sh = e_shoff + e_shstrndx * e_shentsize
    shstr_off = struct.unpack_from('<I', data, sh + 16)[0]
    for i in range(e_shnum):
        so = e_shoff + i * e_shentsize
        name_idx = struct.unpack_from('<I', data, so)[0]
        end = data.find(b'\x00', shstr_off + name_idx)
        name = data[shstr_off + name_idx:end].decode('ascii', 'replace')
        if name == want:
            off = struct.unpack_from('<I', data, so + 16)[0]
            size = struct.unpack_from('<I', data, so + 20)[0]
            return off, size
    return None, None


def main():
    inp, outp = sys.argv[1], sys.argv[2]
    data = bytearray(open(inp, 'rb').read())

    rel_off, rel_size = find_section(data, '.rel.text')
    if rel_off is None:
        sys.exit('relocfix_DATE_801491D8: no .rel.text; aborting')

    by_key = {}
    for i in range(rel_size // 8):
        eo = rel_off + i * 8
        r_offset, r_info = struct.unpack('<II', data[eo:eo + 8])
        by_key[(r_offset, r_info & 0xFF)] = (eo, r_info)

    new_syms = {}
    for dst_off, rtype, src_off in SWAP:
        if (src_off, rtype) not in by_key or (dst_off, rtype) not in by_key:
            sys.exit('relocfix_DATE_801491D8: expected reloc entry missing')
        _, src_info = by_key[(src_off, rtype)]
        new_syms[(dst_off, rtype)] = src_info >> 8

    for (dst_off, rtype), sym in new_syms.items():
        eo, info = by_key[(dst_off, rtype)]
        struct.pack_into('<II', data, eo, dst_off, (sym << 8) | rtype)

    open(outp, 'wb').write(bytes(data))
    print('relocfix_DATE_801491D8: permuted 4 reloc symbol indices')


if __name__ == '__main__':
    main()
