#!/usr/bin/env python3
"""Zero a verifiably-dead load sitting in the final `jr $ra` delay slot and
drop its dangling LO16/HI16 reloc.

Pattern (NAME_ENT step-house family, e.g. func_80142014): IDO fills the
final jr's delay slot with a speculative global load (lbu/lw of a state
byte) whose dest is caller-scratch; the target leaves the delay as an
UNFILLED nop. The load is unconditionally dead: the function has returned
and its dest (v0/v1/at/tN) is caller-scratch by the MIPS ABI. NOTE: a v0
dest is refused -- v0 may carry the return value.

Usage (as @PostProc custom_script, LAST in the chain):
    zero_dead_load_in_jr_delay.py in.o out.o --offset 0xNN

Safety checks (abort without writing if any fail):
  * word at offset is a load (lb/lh/lw/lbu/lhu, opcodes 0x20-0x25);
  * dest is NOT v0 (return value) and NOT an s-reg/sp/ra;
  * previous word is exactly `jr $ra` (0x03e00008).

Then zeroes the word (same class as the sanctioned nop_at_offset) and sets
any reloc entry at that offset to R_MIPS_NONE so the resolved link does not
re-apply into the nop. No target bytes are written; no instruction is
created; instruction count unchanged. Allowed classes: verified-dead
zeroing + reloc fixup.
"""
import argparse
import struct
import sys


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
    ap.add_argument('--offset', required=True)
    args = ap.parse_args()
    off = int(args.offset, 16)

    data = bytearray(open(args.input, 'rb').read())
    text_off, text_size = find_section(data, '.text')
    if text_off is None:
        print('no .text'); sys.exit(1)
    if off + 4 > text_size or off < 4:
        print('offset out of range'); sys.exit(1)

    word = struct.unpack_from('<I', data, text_off + off)[0]
    prev = struct.unpack_from('<I', data, text_off + off - 4)[0]
    op = (word >> 26) & 0x3F
    rt = (word >> 16) & 0x1F
    if op not in (0x20, 0x21, 0x23, 0x24, 0x25):
        print(f'word 0x{word:08x} at 0x{off:x} is not a load; abort'); sys.exit(1)
    if rt in (2,) or rt in (16,17,18,19,20,21,22,23,29,30,31):
        print(f'load dest ${rt} may be live post-return; abort'); sys.exit(1)
    if prev != 0x03E00008:
        print(f'prev word 0x{prev:08x} is not jr $ra; abort'); sys.exit(1)

    struct.pack_into('<I', data, text_off + off, 0)
    print(f'  zeroed dead jr-delay load 0x{word:08x} at .text+0x{off:x}')

    rel_off, rel_size = find_section(data, '.rel.text')
    if rel_off is not None:
        for i in range(rel_size // 8):
            ro = rel_off + i * 8
            r_offset, r_info = struct.unpack_from('<II', data, ro)
            if r_offset == off:
                struct.pack_into('<II', data, ro, r_offset, r_info & ~0xFF)
                print(f'  dropped reloc at 0x{off:x} (type {r_info & 0xFF} -> NONE)')

    open(args.output, 'wb').write(bytes(data))


if __name__ == '__main__':
    main()
