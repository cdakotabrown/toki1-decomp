#!/usr/bin/env python3
"""Relocation-only fixup for DATE/func_80136F98.

The function hoists three loop-invariant array bases (kao2 dest,
imp src, ans trans) into t0/t1/t4. The target allocates them
in address order (t0=kao2, t1=imp); IDO here allocates
t0=imp, t1=kao2 -- a stable base-register-allocation coin-flip
that no C reference-ordering steers. Every instruction WORD already matches
once the two setup addiu LO16 fields are put in the target's order (the
paired body `addu` register roles are handled by patch_insn_reg_field in the
header).

Concretely, the two setup LO16 relocs ride swapped:
  * word 5  (addiu t0,t0,%lo) : ours imp  <->  target kao2
  * word 6  (addiu t1,t1,%lo) : ours kao2  <->  target imp
The paired HI16 luis need no change (both %hi = 0x800D, no carry, identical
bytes). This script permutes ONLY the .rel.text r_info symbol indices of
those two LO16 entries. No .text byte is read from the target or modified --
allowed reloc-fixup class (same tier as relocfix_DATE2_801350A8.py).

  r_offset = 0x10 + 4*word  (0x10 = 4-word overlay thunk prefix)
"""
import sys
import struct
from elftools.elf.elffile import ELFFile

R_MIPS_LO16 = 6

# word 5 <-> word 6 (both R_MIPS_LO16): each entry takes the other's symbol.
SWAP = [(0x10 + 4 * 5, R_MIPS_LO16, 0x10 + 4 * 6),
        (0x10 + 4 * 6, R_MIPS_LO16, 0x10 + 4 * 5)]


def main():
    inp, outp = sys.argv[1], sys.argv[2]
    data = bytearray(open(inp, 'rb').read())
    elf = ELFFile(open(inp, 'rb'))
    rel = elf.get_section_by_name('.rel.text')
    if rel is None:
        print('relocfix_DATE_80136F98: no .rel.text; aborting')
        sys.exit(1)
    rel_off = rel['sh_offset']
    nrel = rel['sh_size'] // 8

    # index entries by (offset, type)
    by_key = {}
    for i in range(nrel):
        eo = rel_off + i * 8
        r_offset, r_info = struct.unpack('<II', data[eo:eo + 8])
        rtype = r_info & 0xFF
        by_key[(r_offset, rtype)] = (eo, r_info)

    # read source symbol indices first, then write (avoid clobber)
    new_syms = {}
    for dst_off, rtype, src_off in SWAP:
        if (src_off, rtype) not in by_key or (dst_off, rtype) not in by_key:
            print('relocfix_DATE_80136F98: expected reloc entry missing; aborting')
            sys.exit(1)
        _, src_info = by_key[(src_off, rtype)]
        new_syms[(dst_off, rtype)] = src_info >> 8

    for (dst_off, rtype), sym in new_syms.items():
        eo, r_info = by_key[(dst_off, rtype)]
        r_info = (sym << 8) | (r_info & 0xFF)
        struct.pack_into('<I', data, eo + 4, r_info)

    open(outp, 'wb').write(data)


if __name__ == '__main__':
    main()
