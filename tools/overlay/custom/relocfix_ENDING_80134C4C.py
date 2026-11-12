#!/usr/bin/env python3
"""Relocation-only fixup for ENDING/func_80134C4C (2026-07-04).

The store block writes three sprite-alpha bytes (back_clear_b/797/796) and two
decremented byte counters (cw97_bright/B37) as five interleaved RMW groups.
scheduler_normalizer interleaves the identical-shaped `sb $r, %lo(D)($at)`
words into the target positions, but leaves the three trailing store LO16
reloc symbols in compile order while the matching `lui $at` HI16 words are
already in target order.  The result: words 22/24/27 (r_offset 0x68/0x70/0x7c)
carry each other's D_ store symbols in a 3-cycle.

  word 22 (`sb`) should reference cw97_bright  (currently back_clear_r)
  word 24 (`sb`) should reference back_clear_r  (currently cal_cw_bright)
  word 27 (`sb`) should reference cal_cw_bright  (currently cw97_bright)

Each destination's preceding `lui $at` HI16 already matches the target, so the
permutation restores a byte-perfect AND semantically-consistent hi/lo pairing.
This script permutes ONLY .rel.text r_info symbol indices between those three
LO16 entries.  No .text byte is read from the target or modified — allowed
reloc-fixup class (same tier as relocfix_DATE2_801350A8.py).
"""
import sys
import struct
from elftools.elf.elffile import ELFFile

R_MIPS_LO16 = 6

# (dst_word, rtype) <- (src_word): dst entry takes src entry's symbol.
# r_offset = 0x10 + 4*word.
MAPPING = [
    (22, R_MIPS_LO16, 27),
    (24, R_MIPS_LO16, 22),
    (27, R_MIPS_LO16, 24),
]


def main():
    inp, outp = sys.argv[1], sys.argv[2]
    data = bytearray(open(inp, 'rb').read())
    elf = ELFFile(open(inp, 'rb'))
    rel = elf.get_section_by_name('.rel.text')
    if rel is None:
        print('relocfix_ENDING_80134C4C: no .rel.text; aborting')
        sys.exit(1)
    rel_off = rel['sh_offset']
    nrel = rel['sh_size'] // 8

    entries = []            # [entry_index, r_offset, sym, rtype]
    for i in range(nrel):
        r_offset, r_info = struct.unpack_from('<II', data, rel_off + i * 8)
        entries.append([i, r_offset, r_info >> 8, r_info & 0xFF])

    def entry(word_idx, rtype):
        off = 0x10 + 4 * word_idx
        cands = [e for e in entries if e[1] == off and e[3] == rtype]
        if len(cands) != 1:
            print('relocfix_ENDING_80134C4C: no unique rtype-%d entry at '
                  'word %d (found %d)' % (rtype, word_idx, len(cands)))
            sys.exit(1)
        return cands[0]

    # capture source symbols before rewriting
    src_sym = {(d, t): entry(s, t)[2] for d, t, s in MAPPING}
    for d, t, _s in MAPPING:
        entry(d, t)[2] = src_sym[(d, t)]

    for i, r_offset, sym, rtype in entries:
        struct.pack_into('<II', data, rel_off + i * 8, r_offset,
                         (sym << 8) | rtype)

    open(outp, 'wb').write(data)
    print('relocfix_ENDING_80134C4C: permuted %d reloc entries' % len(MAPPING))


if __name__ == '__main__':
    main()
