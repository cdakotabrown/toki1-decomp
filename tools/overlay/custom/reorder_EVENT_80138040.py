#!/usr/bin/env python3
"""Reorder-only fixup for EVENT/func_80138040.

The original IDO build hoists the then/else D_800EB06F address `lui`s into
the ev_flag14 load shadow / beq delay slot, interleaves the three byte
loads with the store-base luis in the flag block, and materializes the
tail-block constants inside their branch delay slots.  Our compile emits
the same words in the mirror placements.  This script applies the exact
position permutation (new[i] = old[SRC[i]]); relocations ride with their
words.  Branch words whose skip distances change are fixed by
adjust_branch_offset directives in the .c header (allowed class).

The 2-word residual at body 61/62 (branch imm +0xf vs +0x10, nop vs the
duplicated `lw ra` epilogue prefetch in the beq delay) is a genuine
codegen difference and is NOT touched: our compile restores ra at the
shared epilogue instead of duplicating the restore into the delay slot,
so the branch must keep targeting the `lw ra`.

Policy: reorder-only custom_script (ALLOWED class - permutes existing
compiled words only, relocations ride with their words; no hardcoded
words, no opcode changes, no insert/delete).
"""
import sys
import struct
from elftools.elf.elffile import ELFFile

SRC = {
    27: 29, 29: 72,
    32: 35, 34: 36, 35: 32, 36: 34,
    62: 27, 63: 62, 64: 63, 65: 64, 66: 65,
    70: 66, 71: 70, 72: 71,
}
THUNK = 4


def main():
    inp, outp = sys.argv[1], sys.argv[2]
    data = bytearray(open(inp, 'rb').read())
    elf = ELFFile(open(inp, 'rb'))
    text = elf.get_section_by_name('.text')
    text_off = text['sh_offset']
    nwords = text['sh_size'] // 4

    assert set(SRC.keys()) == set(SRC.values()), 'not a permutation'

    old = [struct.unpack_from('<I', data, text_off + i * 4)[0]
           for i in range(nwords)]

    def src_of(i):
        b = i - THUNK
        return SRC.get(b, b) + THUNK

    for i in range(nwords):
        struct.pack_into('<I', data, text_off + i * 4, old[src_of(i)])

    rel = elf.get_section_by_name('.rel.text')
    if rel is not None:
        rel_off = rel['sh_offset']
        nrel = rel['sh_size'] // 8
        dst_of = {}
        for i in range(nwords):
            dst_of[src_of(i)] = i
        for i in range(nrel):
            r_off, r_info = struct.unpack_from('<II', data, rel_off + i * 8)
            w = r_off // 4
            nw = dst_of.get(w, w)
            struct.pack_into('<II', data, rel_off + i * 8, nw * 4, r_info)

    open(outp, 'wb').write(data)
    print('reorder_EVENT_80138040: permuted %d body words' % len(SRC))


main()
