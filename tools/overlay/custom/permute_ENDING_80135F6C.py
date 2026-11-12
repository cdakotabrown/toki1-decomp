#!/usr/bin/env python3
"""Instruction-position permutation for ENDING/func_80135F6C (2026-07-04).

REORDER-ONLY custom script (allowed class: "custom_script for register
patches / reorders / reloc fixup ONLY").  Every word written is a word
READ from the compiled object itself, moved to a different position; no
instruction word is hardcoded, constructed, or read from the target.

Why: our uopt build speculatively hoists independent stores above the
div-by-variable break-check branch groups and floats the flag-byte
stores up into the multu latency shadow, while the original compile
kept source order (checks first, stores after; flag byte in the loop
branch-delay slot).  scheduler_normalizer --block-aware cannot restore
this (the moves cross the check-branch basic blocks, and its attempt
corrupted the check CFG), so the count-neutral position permutation is
applied here explicitly.

Three windows (word indices = .text-word offsets of the function):

  W1 [124..147]  loop1 (3-entry sprite init): swap slti/addiu pair,
                 sink the screen_w[i+4277]=13 byte store from the
                 multu shadow into the bnez delay slot (everything
                 between shifts up one).
  W2 [371..431]  loop3 (12-entry sprite init): move the two div-check
                 groups (9 words each, kept intact) above the field
                 stores they guard; sink the flag addu/sb pair to its
                 original positions; reorder the mfhi/mflo tail.
  W3 [760..780]  switch default block: interleave the three
                 volatile-pinned copy loads/stores and the a3..a0 arg
                 loads the way the original scheduler did.  Reloc
                 r_offsets move with their words; .rel.text TABLE ORDER
                 is untouched so HI16/LO16 pairing is preserved.

Branch instructions inside the windows either keep their index or move
together with their entire branch-target region, so every branch
immediate is unchanged (verified: bnez a0/bne a0/bne s1 pairs keep
their +2/+4/+2 skip shapes; the loop back-edges bnez at 146/430 keep
their indices).  CFG equivalence: the moved stores are independent of
the checks; both orders execute the same instruction set before the
loop tail.
"""
import struct
import sys

from elftools.elf.elffile import ELFFile

# current word index -> new word index (bijections within each window)
W1 = {124: 125, 125: 124, 126: 147, 146: 146, 147: 145}
W1.update({i: i - 1 for i in range(127, 146)})  # 127..145 -> 126..144

W2 = {
    371: 405, 372: 419,
    373: 371, 374: 372, 375: 373, 376: 374, 377: 375, 378: 376, 379: 377,
    380: 387, 381: 388, 382: 389, 383: 390, 384: 391, 385: 392, 386: 393,
    387: 394, 388: 395, 389: 396,
    390: 378, 391: 379, 392: 380, 393: 381, 394: 382, 395: 383, 396: 384,
    397: 385, 398: 386,
    399: 397, 400: 398, 401: 399, 402: 400, 403: 401, 404: 402, 405: 403,
    406: 404, 407: 407, 408: 406,
    409: 408, 410: 409, 411: 410, 412: 411, 413: 412, 414: 413, 415: 414,
    416: 415, 417: 416, 418: 417, 419: 418,
    420: 423, 421: 425, 422: 428, 423: 420, 424: 422, 425: 421, 426: 424,
    427: 426, 428: 427, 429: 429, 430: 430, 431: 431,
}

W3 = {
    760: 760, 761: 761, 762: 763, 763: 764, 764: 767, 765: 768, 766: 762,
    767: 772, 768: 775, 769: 771, 770: 765, 771: 766, 772: 776, 773: 770,
    774: 777, 775: 769, 776: 773, 777: 774, 778: 778, 779: 779, 780: 780,
}

MAPPING = {}
for w in (W1, W2, W3):
    assert sorted(w.keys()) == sorted(w.values()), "not a bijection"
    MAPPING.update(w)


def main():
    inp, outp = sys.argv[1], sys.argv[2]
    data = bytearray(open(inp, 'rb').read())
    elf = ELFFile(open(inp, 'rb'))

    text = elf.get_section_by_name('.text')
    text_off = text['sh_offset']

    # function start inside .text (types.h emits leading thunk words)
    func_base = None
    symtab = elf.get_section_by_name('.symtab')
    if symtab is not None:
        for sym in symtab.iter_symbols():
            # accept both spellings: the address name pre-landing and the
            # landed name (the 2026-10-17 renames changed the member; this
            # lookup is the "paired fix" that keeps the permute alive)
            if sym.name in ('func_80135F6C', 'ending_sprites_init'):
                func_base = sym['st_value']
                break
    if func_base is None:
        print('permute_ENDING_80135F6C: func symbol not found')
        sys.exit(1)
    base_w = func_base // 4

    # sanity: prologue word at the function start
    first = struct.unpack_from('<I', data, text_off + func_base)[0]
    if (first >> 16) != 0x27bd:
        print('permute_ENDING_80135F6C: unexpected first word %08x' % first)
        sys.exit(1)

    # permute .text words (read all sources first, then write)
    src_words = {i: struct.unpack_from(
        '<I', data, text_off + 4 * (base_w + i))[0] for i in MAPPING}
    for i, w in src_words.items():
        struct.pack_into('<I', data, text_off + 4 * (base_w + MAPPING[i]), w)

    # move reloc r_offsets with their words; table order untouched
    rel = elf.get_section_by_name('.rel.text')
    moved = 0
    if rel is not None:
        rel_off = rel['sh_offset']
        nrel = rel['sh_size'] // 8
        for k in range(nrel):
            r_offset, r_info = struct.unpack_from('<II', data,
                                                  rel_off + k * 8)
            widx = r_offset // 4 - base_w
            if widx in MAPPING and MAPPING[widx] != widx:
                struct.pack_into('<II', data, rel_off + k * 8,
                                 (base_w + MAPPING[widx]) * 4, r_info)
                moved += 1

    open(outp, 'wb').write(data)
    print('permute_ENDING_80135F6C: permuted %d words, moved %d relocs'
          % (len(MAPPING), moved))


if __name__ == '__main__':
    main()
