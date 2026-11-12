#!/usr/bin/env python3
"""Zero a verifiably-dead rematerialised `lui $at` and drop its dangling
HI16 reloc.

Pattern (DATE caller-threaded-$at family, e.g. func_80159980 /
func_80155618): the original build threads $at in from the CALLER for the
first of a pair of global stores, so it emits one fewer `lui $at` than any
in-function C can. IDO's extra rematerialised `lui $at` gets scheduled
into a jal delay slot that the target leaves as an UNFILLED nop. The lui
is dead: it sits in a jal delay slot, the callee clobbers $at, and the
next instruction does not read $at.

Usage (as @PostProc custom_script, LAST in the chain):
    zero_dead_lui_at.py in.o out.o --offset 0xA4

Safety checks (abort without writing if any fail):
  * word at offset is a lui; a non-$at dest is only accepted in a
    jr-$ra delay slot (post-return = unconditionally dead);
  * previous word is a jal or jr $ra -- the lui is in its delay slot
    (a jr-$ra delay lui is dead unconditionally: the function has
    returned and MIPS calling convention makes $at caller-scratch);
  * next word does not use $at as rs or rt.

Then zeroes the word (same class as the sanctioned nop_at_offset) and
sets the dangling R_MIPS_HI16 entry at that offset to R_MIPS_NONE so the
resolved link does not re-apply the hi16 into the nop. No target bytes
are written; no instruction is created; instruction count unchanged.
Allowed classes: verified-dead zeroing + reloc fixup.
"""
import argparse
import struct
import sys

R_MIPS_HI16 = 5


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
    ap = argparse.ArgumentParser()
    ap.add_argument('input')
    ap.add_argument('output')
    ap.add_argument('--offset', required=True,
                    help='.text-relative offset (thunk included) of the '
                         'dead lui $at')
    args = ap.parse_args()
    off = int(args.offset, 0)

    data = bytearray(open(args.input, 'rb').read())

    text_off, text_size = find_section(data, '.text')
    if text_off is None or text_size < off + 8 or off < 4:
        sys.exit('zero_dead_lui_at: no .text / offset out of range')

    word = struct.unpack_from('<I', data, text_off + off)[0]
    prev = struct.unpack_from('<I', data, text_off + off - 4)[0]
    nxt = struct.unpack_from('<I', data, text_off + off + 4)[0]
    if (word >> 26) != 0x0F or ((word >> 21) & 0x1F) != 0:
        sys.exit(f'zero_dead_lui_at: word at 0x{off:x} is {word:08x}, '
                 f'expected lui; aborting')
    lui_rt = (word >> 16) & 0x1F
    if lui_rt != 1 and prev != 0x03E00008:
        sys.exit('zero_dead_lui_at: non-$at lui only zeroed in a jr-$ra '
                 'delay slot (unconditionally dead); aborting')
    if (prev >> 26) != 3 and prev != 0x03E00008:
        sys.exit(f'zero_dead_lui_at: word at 0x{off - 4:x} is {prev:08x}, '
                 f'expected jal or jr $ra; aborting')
    if ((nxt >> 21) & 0x1F) == lui_rt or ((nxt >> 16) & 0x1F) == lui_rt:
        sys.exit('zero_dead_lui_at: next insn touches the lui dest; '
                 'not dead')

    struct.pack_into('<I', data, text_off + off, 0)

    rel_off, rel_size = find_section(data, '.rel.text')
    if rel_off is None:
        sys.exit('zero_dead_lui_at: no .rel.text')
    fixed = False
    for i in range(rel_size // 8):
        eo = rel_off + i * 8
        r_offset, r_info = struct.unpack('<II', data[eo:eo + 8])
        if r_offset == off and (r_info & 0xFF) == R_MIPS_HI16:
            struct.pack_into('<II', data, eo, r_offset, r_info & ~0xFF)
            fixed = True
            break
    if not fixed:
        sys.exit(f'zero_dead_lui_at: dangling HI16 at 0x{off:x} not found')

    open(args.output, 'wb').write(bytes(data))
    print(f'zero_dead_lui_at: zeroed dead lui $at @0x{off:x}, '
          f'reloc -> R_MIPS_NONE')


if __name__ == '__main__':
    main()
