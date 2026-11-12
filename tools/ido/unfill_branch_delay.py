#!/usr/bin/env python3
"""Undo an as1 branch-delay-slot COPY in a MIPS ELF .text section.

as1's branch optimisation copies the first instruction of a branch's target
into the branch's (empty) delay slot and bumps the branch displacement by one
word, so the copy executes in the slot and the original stays where it was.
This tool undoes exactly that: at the branch at --offset it decrements the
displacement by one (the branch targets the original instruction again) and
zeroes the delay slot.

It does NOT move or create anything.  It therefore refuses (exit 1, input
untouched) unless the word it zeroes is a GENUINE as1 copy:

  * the instruction at --offset is a conditional branch (beq/bne/blez/bgtz/
    REGIMM);
  * the delay-slot word is not already a nop;
  * the word at the NEW target (old target - 4), which stays in the object,
    is bit-identical to the delay-slot word being removed;
  * no relocation in .rel.text applies to the delay slot (zeroing a relocated
    word would leave the linker patching a nop).

Before 2026-10-04 none of this was checked: the slot was zeroed whatever it
held, so the step could silently DELETE a real instruction (count-neutral, in
place -- the class the count test cannot see).

Self-test (synthetic ELF objects, no compiler needed):
    python3 tools/ido/unfill_branch_delay.py --self-test
"""
import argparse
import struct
import sys

BRANCH_OPS = (0x01, 0x04, 0x05, 0x06, 0x07)
SHT_REL = 9


class Refused(Exception):
    pass


def _sections(data):
    e_shoff = struct.unpack_from('<I', data, 32)[0]
    e_shnum = struct.unpack_from('<H', data, 48)[0]
    e_shstrndx = struct.unpack_from('<H', data, 50)[0]
    secs = [struct.unpack_from('<10I', data, e_shoff + i * 40)
            for i in range(e_shnum)]
    str_off = secs[e_shstrndx][4]

    def name(idx):
        end = data.index(b'\x00', str_off + idx)
        return data[str_off + idx:end].decode()
    return secs, name


def unfill(data, branch_off):
    """Apply in place to bytearray `data`.  Returns (old_simm, new_simm,
    removed_word).  Raises Refused if the slot is not a genuine as1 copy."""
    secs, name = _sections(data)
    text_idx = next((i for i, sh in enumerate(secs) if name(sh[0]) == '.text'),
                    None)
    if text_idx is None:
        raise Refused('no .text section found')
    text_off, text_size = secs[text_idx][4], secs[text_idx][5]

    if branch_off % 4 or not 0 <= branch_off <= text_size - 8:
        raise Refused('offset 0x%x outside .text (size 0x%x)'
                      % (branch_off, text_size))
    boff = text_off + branch_off
    branch = struct.unpack_from('<I', data, boff)[0]
    op = branch >> 26
    if op not in BRANCH_OPS:
        raise Refused('instruction at 0x%x is not a conditional branch '
                      '(op=0x%02x)' % (branch_off, op))
    simm = branch & 0xFFFF
    simm = simm - 0x10000 if simm & 0x8000 else simm
    new_simm = simm - 1

    delay_off = branch_off + 4
    removed = struct.unpack_from('<I', data, text_off + delay_off)[0]
    if removed == 0:
        raise Refused('delay slot at 0x%x is already a nop' % delay_off)

    new_target = branch_off + 4 + 4 * new_simm
    if not 0 <= new_target <= text_size - 4:
        raise Refused('new branch target 0x%x outside .text' % new_target)
    if new_target == delay_off:
        raise Refused('new target is the delay slot itself')
    kept = struct.unpack_from('<I', data, text_off + new_target)[0]
    if kept != removed:
        raise Refused('delay-slot word 0x%08x at 0x%x is NOT a copy of the '
                      'word at the new target 0x%x (0x%08x); zeroing it would '
                      'delete a real instruction'
                      % (removed, delay_off, new_target, kept))

    for sh in secs:
        if sh[1] == SHT_REL and sh[7] == text_idx:
            for k in range(sh[5] // 8):
                r_offset = struct.unpack_from('<I', data, sh[4] + 8 * k)[0]
                if r_offset == delay_off:
                    raise Refused('delay slot at 0x%x carries a relocation'
                                  % delay_off)

    struct.pack_into('<I', data, boff, (branch & 0xFFFF0000) | (new_simm & 0xFFFF))
    struct.pack_into('<I', data, text_off + delay_off, 0)
    return simm, new_simm, removed


# ── self-test ─────────────────────────────────────────────────────────

def make_elf(words, rel_offsets=()):
    """Minimal little-endian ELF32 relocatable: .text (+ .rel.text)."""
    shstr = b'\x00.text\x00.rel.text\x00.shstrtab\x00'
    text = b''.join(struct.pack('<I', w) for w in words)
    rel = b''.join(struct.pack('<II', o, (1 << 8) | 5) for o in rel_offsets)
    text_off = 52
    rel_off = text_off + len(text)
    str_off = rel_off + len(rel)
    sh_off = (str_off + len(shstr) + 3) & ~3
    blob = bytearray(sh_off + 4 * 40)
    hdr = b'\x7fELF\x01\x01\x01' + b'\x00' * 9
    struct.pack_into('<16sHHIIIIIHHHHHH', blob, 0, hdr, 1, 8, 1, 0, 0,
                     sh_off, 0, 52, 0, 0, 40, 4, 3)
    blob[text_off:text_off + len(text)] = text
    blob[rel_off:rel_off + len(rel)] = rel
    blob[str_off:str_off + len(shstr)] = shstr
    shs = [(0,) * 10,
           (1, 1, 6, 0, text_off, len(text), 0, 0, 4, 0),
           (7, SHT_REL, 0, 0, rel_off, len(rel), 0, 1, 4, 8),
           (17, 3, 0, 0, str_off, len(shstr), 0, 0, 1, 0)]
    for i, sh in enumerate(shs):
        struct.pack_into('<10I', blob, sh_off + 40 * i, *sh)
    return blob


def self_test():
    LW = 0x8C820004          # lw v0,4(a0)
    ADDIU = 0x24420001       # addiu v0,v0,1
    JR_RA = 0x03E00008
    # beqz a1 at word 0; as1 copied word 3 (LW) into the slot and bumped the
    # displacement 2 -> 3 (target word 4).
    good = [0x10A00003, LW, ADDIU, LW, ADDIU, JR_RA, 0]
    fails = []

    d = make_elf(good)
    try:
        unfill(d, 0)
        words = struct.unpack_from('<7I', d, 52)
        if words[0] != 0x10A00002 or words[1] != 0:
            fails.append('genuine copy: wrong result %r' % (words[:2],))
    except Refused as e:
        fails.append('genuine copy refused: %s' % e)

    cases = {
        'slot is not a copy of the new target':
            ([0x10A00003, ADDIU, ADDIU, LW, ADDIU, JR_RA, 0], ()),
        'slot already a nop': ([0x10A00003, 0, ADDIU, LW, ADDIU, JR_RA, 0], ()),
        'not a branch': ([0x0C000000, LW, ADDIU, LW, ADDIU, JR_RA, 0], ()),
        'new target outside .text': ([0x10A07FFF, LW, ADDIU, LW, ADDIU, JR_RA, 0], ()),
        'relocated delay slot': (good, (4,)),
    }
    for label, (words, rels) in cases.items():
        d = make_elf(words, rels)
        before = bytes(d)
        try:
            unfill(d, 0)
            fails.append('NOT refused: ' + label)
        except Refused:
            if bytes(d) != before:
                fails.append('refused but modified the object: ' + label)
    total = 1 + len(cases)
    if fails:
        print('unfill_branch_delay self-test FAILED %d/%d' % (len(fails), total))
        for f in fails:
            print('  ' + f)
        return 1
    print('unfill_branch_delay self-test OK (%d cases)' % total)
    return 0


def main():
    if '--self-test' in sys.argv[1:]:
        return self_test()
    parser = argparse.ArgumentParser(description='Undo an as1 delay-slot copy')
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output ELF object file')
    parser.add_argument('--offset', required=True,
                        help='Offset of branch instruction in .text (hex)')
    args = parser.parse_args()
    branch_off = int(args.offset, 16)
    with open(args.input, 'rb') as f:
        data = bytearray(f.read())
    try:
        simm, new_simm, removed = unfill(data, branch_off)
    except Refused as e:
        print('Error: unfill_branch_delay REFUSED at 0x%x: %s' % (branch_off, e))
        return 1
    print('  Branch at 0x%04x: offset %d -> %d' % (branch_off, simm, new_simm))
    print('  Delay slot at 0x%04x: 0x%08x (copy of the new target) -> NOP'
          % (branch_off + 4, removed))
    with open(args.output, 'wb') as f:
        f.write(data)
    print('Success')
    return 0


if __name__ == '__main__':
    sys.exit(main())
