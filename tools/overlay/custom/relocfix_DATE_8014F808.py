#!/usr/bin/env python3
"""Relocation-only fixup for DATE/func_8014F808.

Allowed reloc-fixup tier (same as relocfix_DATE_801505CC): permutes the
normalizer-dragged HI16/LO16 reloc symbol indices back onto the offsets
whose instruction words they belong to. The scheduler_normalizer crossed
the two date_sprite_init stack-arg loads (D_8015E708 at sp+0x10, D_8015E70C
at sp+0x14) with each other and dragged the girl0_tokimeki counter %hi into
the D_8015E70C load's lui, pushing D_8015E708's %hi down onto the
girl0_syoushin counter lui. Restores each reloc to the offset whose target
word it matches:
    HI16 3-cycle : 0x040<-0x0B8, 0x044<-0x040, 0x0B8<-0x044
    LO16 swap    : 0x048<-0x04C, 0x04C<-0x048
Derived by pairing each HI16 lui with its LO16 partner and matching the
resolved imm16 against the target word. No .text byte modified.
"""
import sys
import struct

R_MIPS_HI16 = 5
R_MIPS_LO16 = 6

SWAP = [
    (0x040, R_MIPS_HI16, 0x0B8),
    (0x044, R_MIPS_HI16, 0x040),
    (0x0B8, R_MIPS_HI16, 0x044),
    (0x048, R_MIPS_LO16, 0x04C),
    (0x04C, R_MIPS_LO16, 0x048),
]


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
        sys.exit('relocfix_DATE_8014F808: no .rel.text; aborting')

    by_key = {}
    for i in range(rel_size // 8):
        eo = rel_off + i * 8
        r_offset, r_info = struct.unpack('<II', data[eo:eo + 8])
        by_key[(r_offset, r_info & 0xFF)] = (eo, r_info)

    new_syms = {}
    for dst_off, rtype, src_off in SWAP:
        if (src_off, rtype) not in by_key or (dst_off, rtype) not in by_key:
            sys.exit('relocfix_DATE_8014F808: expected reloc entry missing')
        _, src_info = by_key[(src_off, rtype)]
        new_syms[(dst_off, rtype)] = src_info >> 8

    for (dst_off, rtype), sym in new_syms.items():
        eo, info = by_key[(dst_off, rtype)]
        struct.pack_into('<II', data, eo, dst_off, (sym << 8) | rtype)

    open(outp, 'wb').write(bytes(data))
    print('relocfix_DATE_8014F808: permuted 5 reloc symbol indices')


if __name__ == '__main__':
    main()
