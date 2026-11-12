#!/usr/bin/env python3
"""Replace a stack-spill instruction with a register move using the same source.

Used when IDO fills a branch delay slot with `sw $rT, M($sp)` (a dead
spill to clear up live ranges) but the target binary fills the same
delay slot with `or $rD, $rT, $zero` — a dead move, semantically
equivalent in the post-branch dispatch context but using a different
opcode entirely.

The transform reads the rt field of the SW at the given offset and
constructs an OR (special, funct=0x25) using:
  rs = rt  (the value being moved)
  rt = $0
  rd = --dst register name

Refuses if the instruction at the given offset is not a `sw $rX, M($sp)`,
to avoid silent corruption.

Usage:
    swap_spill_with_move.py in.o out.o --offset 0x1c --dst v0
"""
import struct
import sys
import argparse


REG_NAMES = {
    'zero': 0, 'at': 1,
    'v0': 2, 'v1': 3,
    'a0': 4, 'a1': 5, 'a2': 6, 'a3': 7,
    't0': 8, 't1': 9, 't2': 10, 't3': 11,
    't4': 12, 't5': 13, 't6': 14, 't7': 15,
    's0': 16, 's1': 17, 's2': 18, 's3': 19,
    's4': 20, 's5': 21, 's6': 22, 's7': 23,
    't8': 24, 't9': 25,
    'k0': 26, 'k1': 27,
    'gp': 28, 'sp': 29, 'fp': 30, 's8': 30, 'ra': 31,
}


def find_text_section(data: bytearray):
    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]
    sections = []
    for i in range(e_shnum):
        off = e_shoff + i * 40
        sections.append(struct.unpack('<10I', data[off:off+40]))
    str_off = sections[e_shstrndx][4]

    def get_name(idx):
        end = data.index(b'\x00', str_off + idx)
        return data[str_off + idx:end].decode()

    for sh in sections:
        if get_name(sh[0]) == '.text':
            return sh[4], sh[5]
    return None, None


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                  formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('input', help='Input ELF object file')
    ap.add_argument('output', help='Output ELF object file')
    ap.add_argument('--offset', required=True,
                    help='Byte offset of SW instruction in .text (hex)')
    ap.add_argument('--dst', required=True,
                    help='Destination register for OR (e.g. v0, v1, a0)')
    ap.add_argument('--allow-non-sp', action='store_true',
                    help='Allow base register other than $sp (default: refuse)')
    args = ap.parse_args()

    if args.dst not in REG_NAMES:
        print(f"unknown register name: {args.dst}", file=sys.stderr)
        sys.exit(1)
    rd = REG_NAMES[args.dst]
    off = int(args.offset, 16)

    with open(args.input, 'rb') as f:
        data = bytearray(f.read())

    text_off, text_size = find_text_section(data)
    if text_off is None:
        print("No .text section found", file=sys.stderr)
        sys.exit(1)
    if off + 4 > text_size:
        print(f"offset 0x{off:x} outside .text (size 0x{text_size:x})", file=sys.stderr)
        sys.exit(1)

    abs_off = text_off + off
    insn = struct.unpack('<I', data[abs_off:abs_off+4])[0]

    op = (insn >> 26) & 0x3f
    rs = (insn >> 21) & 0x1f
    rt = (insn >> 16) & 0x1f
    imm = insn & 0xffff

    if op != 0x2b:
        print(
            f"insn at 0x{off:x} (op=0x{op:02x}) is not SW; "
            f"refusing to swap_spill_with_move",
            file=sys.stderr,
        )
        sys.exit(1)

    if rs != REG_NAMES['sp'] and not args.allow_non_sp:
        print(
            f"SW at 0x{off:x} uses base register #{rs} (not $sp); "
            f"pass --allow-non-sp to override",
            file=sys.stderr,
        )
        sys.exit(1)

    # Build OR (special, funct 0x25): rs=rt (source), rt=$0, rd=parameterized
    new_insn = (rt << 21) | (0 << 16) | (rd << 11) | 0x25

    struct.pack_into('<I', data, abs_off, new_insn)
    print(
        f"  Swapped SW $r{rt}, {imm:#x}($r{rs}) -> "
        f"OR ${args.dst}, $r{rt}, $zero  at offset 0x{off:x} "
        f"(was 0x{insn:08x}, now 0x{new_insn:08x})"
    )

    with open(args.output, 'wb') as f:
        f.write(data)
    print(f"Wrote {args.output}")


if __name__ == '__main__':
    main()
