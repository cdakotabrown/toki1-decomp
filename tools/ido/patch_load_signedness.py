#!/usr/bin/env python3
"""Convert between signed/unsigned load instructions at specific offsets.

Handles: lh <-> lhu, lb <-> lbu
This fixes IDO quirks where the compiler picks the wrong sign-extension
for a load instruction (e.g., lhu instead of lh for a signed comparison).
"""
import struct
import sys
import argparse

# Load opcode pairs: (signed, unsigned)
LOAD_PAIRS = {
    0x21: 0x25,  # lh  <-> lhu
    0x25: 0x21,  # lhu <-> lh
    0x20: 0x24,  # lb  <-> lbu
    0x24: 0x20,  # lbu <-> lb
}


def main():
    parser = argparse.ArgumentParser(description='Convert load signedness in MIPS ELF')
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output ELF object file')
    parser.add_argument('--offsets', required=True, nargs='+',
                        help='Offsets of load instructions to convert (hex)')
    args = parser.parse_args()

    offsets = [int(x, 16) for x in args.offsets]

    with open(args.input, 'rb') as f:
        data = bytearray(f.read())

    # Parse ELF to find .text
    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]
    sections = []
    for i in range(e_shnum):
        off = e_shoff + i * 40
        sh = struct.unpack('<10I', data[off:off+40])
        sections.append(sh)
    strtab = sections[e_shstrndx]
    str_off = strtab[4]

    def get_name(idx):
        end = data.index(b'\x00', str_off + idx)
        return data[str_off + idx:end].decode()

    text_off = None
    for i, sh in enumerate(sections):
        if get_name(sh[0]) == '.text':
            text_off = sh[4]
            text_size = sh[5]
            break

    if text_off is None:
        print("No .text section found")
        sys.exit(1)

    patched = 0
    for off in offsets:
        if off >= text_size:
            print(f"Warning: offset 0x{off:x} beyond .text size 0x{text_size:x}")
            continue

        insn_off = text_off + off
        insn = struct.unpack('<I', data[insn_off:insn_off+4])[0]
        op = (insn >> 26) & 0x3F

        if op not in LOAD_PAIRS:
            print(f"Warning: instruction at 0x{off:x} (op=0x{op:02x}) is not a convertible load")
            continue

        new_op = LOAD_PAIRS[op]
        new_insn = (insn & 0x03FFFFFF) | (new_op << 26)
        struct.pack_into('<I', data, insn_off, new_insn)

        op_names = {0x20: 'lb', 0x21: 'lh', 0x24: 'lbu', 0x25: 'lhu'}
        print(f"  Offset 0x{off:04x}: {op_names.get(op, f'op{op:02x}')} -> {op_names.get(new_op, f'op{new_op:02x}')}")
        patched += 1

    print(f"Patched {patched} instruction(s)")

    with open(args.output, 'wb') as f:
        f.write(data)


if __name__ == '__main__':
    main()
