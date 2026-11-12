#!/usr/bin/env python3
"""Relocation-only fixup for DATE/func_80154154.

In the second precondition group the compiler emits the two byte-load
globals (now_month and now_day) with their %lo16 relocations in the
opposite source order from the original: our `lbu $a0` and `lbu $v1`
(object words 34 and 35) reference each other's symbol.  The register
fields already match the target after patch_insn_reg_field; only the two
R_MIPS_LO16 symbol indices are crossed.

This script swaps ONLY those two .rel.text LO16 symbol indices.  No .text
byte is read from the target or modified -- allowed reloc-fixup class
(same tier as relocfix_DATE2_801350A8.py).
"""
import sys
import struct
from elftools.elf.elffile import ELFFile

R_MIPS_LO16 = 6

# (dst_word, rtype) <- (src_word): dst entry takes src entry's symbol.
# r_offset = 0x10 + 4*word.
MAPPING = [
    (34, R_MIPS_LO16, 35),
    (35, R_MIPS_LO16, 34),
]


def main():
    inp, outp = sys.argv[1], sys.argv[2]
    data = bytearray(open(inp, 'rb').read())
    elf = ELFFile(open(inp, 'rb'))
    rel = elf.get_section_by_name('.rel.text')
    if rel is None:
        print('relocfix_DATE_80154154: no .rel.text; aborting')
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
            print('relocfix_DATE_80154154: no unique rtype-%d entry at '
                  'word %d (found %d)' % (rtype, word_idx, len(cands)))
            sys.exit(1)
        return cands[0]

    # capture source symbols before rewriting
    src_sym = {(d, t): entry(s, t)[2] for d, t, s in MAPPING}
    for d, t, _s in MAPPING:
        entry(d, t)[2] = src_sym[(d, t)]

    for i, r_offset, sym, rtype in entries:
        r_info = (sym << 8) | rtype
        struct.pack_into('<II', data, rel_off + i * 8, r_offset, r_info)

    with open(outp, 'wb') as f:
        f.write(data)


if __name__ == '__main__':
    main()
