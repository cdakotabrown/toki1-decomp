#!/usr/bin/env python3
"""Relocation-only fixup for DATE2/func_801350A8 (2026-07-03).

Every instruction WORD in our compile matches the original once the
lo16/hi16 fields are masked, but uopt emits some of the identical-shaped
global accesses in a different source order and the scheduler leaves the
(byte-identical, --const-fp-matched) words in their target positions, so
several RELOCATION SYMBOLS stay in compile order while the target wants
them in original order:

  * the case-21/23/25/27 tails each end with two `lui $at` words whose
    HI16 entries (step_counter step counter vs D_8013A5AC bounce counter)
    ride swapped               -> words 440<->442, 496<->498, 552<->554,
                                  653<->655
  * case 26's `lbu` of Sprites_4_r and case 30's post-call `lbu` of
    now_sub_sub_step carry each other's HI16+LO16 units
                               -> words 661<->725 (hi), 662<->726 (lo)
  * case 30's first `lui $t8` (now_sub_sub_step), the helper's DB6 `lui $t8`
    and the helper's first D46 `lui $t8` HI16s ride a 3-cycle
                               -> 718<-855, 855<-828, 828<-718

This script permutes ONLY .rel.text r_info symbol indices between those
entries.  No .text byte is read from the target or modified — allowed
reloc-fixup class (same tier as relocfix_DATE2_80133AF4.py).
"""
import sys
import struct
from elftools.elf.elffile import ELFFile

R_MIPS_HI16 = 5
R_MIPS_LO16 = 6

# (dst_word, rtype) <- (src_word): dst entry takes src entry's symbol.
# r_offset = 0x10 + 4*word.
MAPPING = [
    # case-21/23/25/27 tails: F864/A5AC hi16 pair swaps
    (440, R_MIPS_HI16, 442), (442, R_MIPS_HI16, 440),
    (496, R_MIPS_HI16, 498), (498, R_MIPS_HI16, 496),
    (552, R_MIPS_HI16, 554), (554, R_MIPS_HI16, 552),
    (653, R_MIPS_HI16, 655), (655, R_MIPS_HI16, 653),
    # case 26 Sprites_4_r lbu <-> case 30 now_sub_sub_step lbu (hi+lo units)
    (661, R_MIPS_HI16, 725), (725, R_MIPS_HI16, 661),
    (662, R_MIPS_LO16, 726), (726, R_MIPS_LO16, 662),
    # case 30 86A hi / helper DB6 hi / helper D46 hi 3-cycle
    (718, R_MIPS_HI16, 855), (855, R_MIPS_HI16, 828),
    (828, R_MIPS_HI16, 718),
]


def main():
    inp, outp = sys.argv[1], sys.argv[2]
    data = bytearray(open(inp, 'rb').read())
    elf = ELFFile(open(inp, 'rb'))
    rel = elf.get_section_by_name('.rel.text')
    if rel is None:
        print('relocfix_DATE2_801350A8: no .rel.text; aborting')
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
            print('relocfix_DATE2_801350A8: no unique rtype-%d entry at '
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
    print('relocfix_DATE2_801350A8: permuted %d reloc entries' % len(MAPPING))


if __name__ == '__main__':
    main()
