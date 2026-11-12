#!/usr/bin/env python3
"""Relocation-only fixup for DATE/func_80146230 (2026-07-03).

The walk/bounce tail stores y to the D_80129Dxx sprite-pair bytes and
does the +/-20 RMW walk on the D_80129Dxx halfwords.  Our compile
emits the same instruction WORDS as the original in every slot, but
uopt schedules the (identical-shaped) global accesses in a different
source order; the scheduler_normalizer pass then leaves the words in
place (they already match once the lo16 is masked), so the
RELOCATION SYMBOLS stay in compile order while the target wants them
in original order.  Net effect: 18 R_MIPS_LO16 (+ paired R_MIPS_HI16)
entries reference the right instruction shapes but the wrong
D_80129Dxx symbol.

This script permutes ONLY .rel.text r_info symbol indices between
those entries (hi16 partner entries are moved in tandem so ld's
HI16/LO16 pairing stays valid).  No .text byte is read from the
target or modified — allowed reloc-fixup class (same tier as
reorder_TEL_80136620_v2.py).

Mapping (body word index: needed-symbol <- taken from index):
  706<-713, 708<-706, 710<-708, 713<-710   (sb y ring: DBA,D96,D72,D4E)
  715<-720, 720<-715                        (lhu D4A/D6E swap)
  737<-747, 742<-737, 747<-742              (sh D4A/D46/D6E rotate)
  759<-764, 764<-759, 762<-767, 767<-762    (D92/DB6 swaps)
  777<-784, 780<-786, 784<-789, 786<-777, 789<-780  (D92/D8E/DB6 ring)
"""
import sys
import struct
from elftools.elf.elffile import ELFFile

R_MIPS_HI16 = 5
R_MIPS_LO16 = 6

# dst <- src (body word indices; r_offset = 0x10 + 4*idx)
# 2026-08-23: the 4-word cross-prologue head func_80146220 was absorbed into
# func_80146230, so the body now starts 4 words later in the object and every
# index below moved +4 (was 702..785, now 706..789).
MAPPING = {
    706: 713, 708: 706, 710: 708, 713: 710,
    715: 720, 720: 715,
    737: 747, 742: 737, 747: 742,
    759: 764, 764: 759, 762: 767, 767: 762,
    777: 784, 780: 786, 784: 789, 786: 777, 789: 780,
}


def main():
    inp, outp = sys.argv[1], sys.argv[2]
    data = bytearray(open(inp, 'rb').read())
    elf = ELFFile(open(inp, 'rb'))
    rel = elf.get_section_by_name('.rel.text')
    if rel is None:
        print('relocfix_DATE_80146230: no .rel.text; aborting')
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
            print('relocfix_DATE_80146230: no unique LO16 at word %d'
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
    print('relocfix_DATE_80146230: permuted %d reloc units (hi+lo pairs)'
          % len(MAPPING))


if __name__ == '__main__':
    main()
