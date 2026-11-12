#!/usr/bin/env python3
"""Relocation-only fixup for DATE2/func_80133AF4 (2026-07-03).

The walk/bounce tail stores y to the D_80129Dxx sprite-pair bytes and
does the +/-20 RMW walk on the D_80129Dxx halfwords.  Our compile
emits the same instruction WORDS as the original in every slot, but
uopt schedules the (identical-shaped) global accesses in a different
source order; the scheduler_normalizer pass then leaves the words in
place (they already match once the lo16 is masked), so the
RELOCATION SYMBOLS stay in compile order while the target wants them
in original order.  Net effect: 24 R_MIPS_LO16 (+ paired R_MIPS_HI16)
entries reference the right instruction shapes but the wrong
D_80129Dxx symbol.

This script permutes ONLY .rel.text r_info symbol indices between
those entries (hi16 partner entries are moved in tandem so ld's
HI16/LO16 pairing stays valid).  No .text byte is read from the
target or modified — allowed reloc-fixup class (same tier as
relocfix_DATE_80146230.py).

Mapping (body word index: needed-symbol <- taken from index):
  647<-654, 649<-647, 651<-649, 654<-651   (sb y ring: DBA,D96,D72,D4E)
  656<-661, 659<-664, 661<-656, 664<-659   (lhu/sh D4A/D6E swap, dir!=0)
  678<-688, 683<-693, 688<-678, 693<-683   (sh D4A/D46/D6E/D6A, dir==0)
  703<-708, 708<-703, 711<-717, 717<-711,
  718<-727, 727<-718                       (D92/DB6 + D97/DBB, D97!=0)
  721<-725, 722<-732, 725<-730, 730<-735,
  732<-722, 735<-721                       (D92/D8E/DB6/DB2 ring, D97==0)
"""
import sys
import struct
from elftools.elf.elffile import ELFFile

R_MIPS_HI16 = 5
R_MIPS_LO16 = 6

# dst <- src (body word indices; r_offset = 0x10 + 4*idx)
MAPPING = {
    647: 654, 649: 647, 651: 649, 654: 651,
    656: 661, 659: 664, 661: 656, 664: 659,
    678: 688, 683: 693, 688: 678, 693: 683,
    703: 708, 708: 703, 711: 717, 717: 711,
    718: 727, 727: 718,
    721: 725, 722: 732, 725: 730, 730: 735,
    732: 722, 735: 721,
}


def main():
    inp, outp = sys.argv[1], sys.argv[2]
    data = bytearray(open(inp, 'rb').read())
    elf = ELFFile(open(inp, 'rb'))
    rel = elf.get_section_by_name('.rel.text')
    if rel is None:
        print('relocfix_DATE2_80133AF4: no .rel.text; aborting')
        sys.exit(1)
    rel_off = rel['sh_offset']
    nrel = rel['sh_size'] // 8

    entries = []           # (idx, r_offset, sym, rtype)
    for i in range(nrel):
        r_offset, r_info = struct.unpack_from('<II', data, rel_off + i * 8)
        entries.append([i, r_offset, r_info >> 8, r_info & 0xFF])

    by_off = {}
    for e in entries:
        by_off.setdefault(e[1], []).append(e)

    def lo_entry(word_idx):
        off = 0x10 + 4 * word_idx
        cands = [e for e in by_off.get(off, []) if e[3] == R_MIPS_LO16]
        if len(cands) != 1:
            print('relocfix_DATE2_80133AF4: no unique LO16 at word %d'
                  % word_idx)
            sys.exit(1)
        return cands[0]

    # ld pairing: iterate table order; a LO16 claims every pending
    # (unclaimed) HI16 with the same symbol that precedes it.
    pending = []
    partner = {}
    for e in entries:
        if e[3] == R_MIPS_HI16:
            pending.append(e)
        elif e[3] == R_MIPS_LO16:
            mine = [h for h in pending if h[2] == e[2]]
            partner[id(e)] = mine
            pending = [h for h in pending if h[2] != e[2]]

    units = {}
    for widx in set(MAPPING) | set(MAPPING.values()):
        lo = lo_entry(widx)
        units[widx] = partner.get(id(lo), []) + [lo]

    # capture source symbols before rewriting
    src_sym = {w: units[w][1][2] for w in units}
    for dst, src in MAPPING.items():
        sym = src_sym[src]
        for e in units[dst]:
            e[2] = sym

    # write back r_info
    for e in entries:
        i, r_offset, sym, rtype = e
        struct.pack_into('<II', data, rel_off + i * 8, r_offset,
                         (sym << 8) | rtype)

    open(outp, 'wb').write(data)
    print('relocfix_DATE2_80133AF4: permuted %d reloc units (hi+lo pairs)'
          % len(MAPPING))


if __name__ == '__main__':
    main()
