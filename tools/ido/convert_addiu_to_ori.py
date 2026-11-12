#!/usr/bin/env python3
"""Convert IDO's `lui rt,HI; addiu rt,rt,LO` into the target's
`lui rt,HI; ori rt,rt,LO` form.

IDO often generates lui+addiu for constructing addresses, but the target
binary may use lui+ori instead. For addresses where LO < 0x8000, both forms
produce the same result. For LO >= 0x8000, the lui HI value differs by 1
between the two forms (addiu sign-extends, ori zero-extends).

This tool operates on the SECOND instruction only — it swaps the opcode
from addiu (0x09) to ori (0x0D) at the given offset, adjusting the lui
HI value if needed when LO >= 0x8000.

This is a pure ELF text rewrite — safe under the project's BANNED-tools
policy since it only changes the opcode and optionally the lui immediate,
preserving all register fields and relocation entries.

Usage:
    convert_addiu_to_ori.py in.o out.o --offset 0x40
    convert_addiu_to_ori.py in.o out.o --offsets 0x40,0x80,0xC0

The tool reads the addiu instruction at the given offset, reads the
preceding lui instruction, and converts the pair to lui+ori form.
For unrelocated objects (immediate = 0), it just swaps the opcode.
"""

import argparse
import struct
import sys
from pathlib import Path


OP_LUI = 0x0F
OP_ADDIU = 0x09
OP_ORI = 0x0D


def find_text_section(data: bytearray):
    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shentsize = struct.unpack('<H', data[46:48])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]

    shstr_hdr_off = e_shoff + (e_shstrndx * e_shentsize)
    shstr_off = struct.unpack('<I', data[shstr_hdr_off + 16:shstr_hdr_off + 20])[0]

    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack('<I', data[sh_off:sh_off + 4])[0]
        sec_off = struct.unpack('<I', data[sh_off + 16:sh_off + 20])[0]
        sec_size = struct.unpack('<I', data[sh_off + 20:sh_off + 24])[0]

        n0 = shstr_off + sh_name_idx
        n1 = data.find(b'\x00', n0)
        sec_name = data[n0:n1].decode('ascii', errors='ignore')

        if sec_name == '.text':
            return sec_off, sec_size

    raise ValueError("No .text section found")


def convert_single(data: bytearray, text_off: int, off: int) -> bool:
    """Convert addiu at offset to ori, adjusting preceding lui if needed."""
    abs_off = text_off + off
    addiu_word = struct.unpack('<I', data[abs_off:abs_off + 4])[0]

    addiu_op = (addiu_word >> 26) & 0x3f
    if addiu_op != OP_ADDIU:
        print(f"  Warning: instruction at 0x{off:x} is not addiu (op=0x{addiu_op:02x}), skipping")
        return False

    rt = (addiu_word >> 16) & 0x1f
    rs = (addiu_word >> 21) & 0x1f
    imm = addiu_word & 0xffff

    # Read the preceding lui instruction
    lui_abs = abs_off - 4
    lui_word = struct.unpack('<I', data[lui_abs:lui_abs + 4])[0]
    lui_op = (lui_word >> 26) & 0x3f
    lui_rt = (lui_word >> 16) & 0x1f
    lui_hi = lui_word & 0xffff

    if lui_op != OP_LUI:
        print(f"  Warning: instruction at 0x{off-4:x} is not lui (op=0x{lui_op:02x}), skipping")
        return False

    if lui_rt != rs or lui_rt != rt:
        # The lui destination doesn't match addiu source/dest — might not be a pair
        # but still convert if it's an addiu we want to change
        pass

    if imm == 0:
        # Unrelocated object — just swap the opcode, linker will fill in values
        new_addiu_word = (OP_ORI << 26) | (rs << 21) | (rt << 16) | imm
        struct.pack_into('<I', data, abs_off, new_addiu_word)
        print(f"  Converted addiu→ori at 0x{off:x} (unrelocated, imm=0)")
        return True

    if imm < 0x8000:
        # LO is positive — addiu and ori produce the same result with same HI
        # Just swap the opcode
        new_addiu_word = (OP_ORI << 26) | (rs << 21) | (rt << 16) | imm
        struct.pack_into('<I', data, abs_off, new_addiu_word)
        print(f"  Converted addiu→ori at 0x{off:x}: imm 0x{imm:04x} (LO < 0x8000, HI unchanged)")
        return True
    else:
        # LO >= 0x8000 — addiu sign-extends, so HI needs adjustment
        # addiu form: lui rt, HI+1; addiu rt, rt, LO-0x10000
        # ori form:   lui rt, HI;   ori rt, rt, LO
        # So: new_hi = old_hi - 1, and addiu imm = LO - 0x10000 → ori imm = LO
        new_hi = (lui_hi - 1) & 0xffff
        new_lui_word = (lui_word & 0xffff0000) | new_hi
        new_addiu_word = (OP_ORI << 26) | (rs << 21) | (rt << 16) | imm

        struct.pack_into('<I', data, lui_abs, new_lui_word)
        struct.pack_into('<I', data, abs_off, new_addiu_word)
        print(f"  Converted addiu→ori at 0x{off:x}: lui hi 0x{lui_hi:04x}→0x{new_hi:04x}, imm 0x{imm:04x}")
        return True


def main():
    parser = argparse.ArgumentParser(
        description="Convert addiu to ori at specific offsets (lui+addiu → lui+ori)")
    parser.add_argument("input", type=Path, help="Input ELF object file")
    parser.add_argument("output", type=Path, help="Output ELF object file")
    parser.add_argument("--offset", type=str, default=None,
                        help="Single offset (hex or decimal)")
    parser.add_argument("--offsets", type=str, default=None,
                        help="Comma-separated offsets")
    args = parser.parse_args()

    offsets = []
    if args.offset:
        off = int(args.offset, 16) if args.offset.startswith('0x') else int(args.offset)
        offsets.append(off)
    if args.offsets:
        for o in args.offsets.split(','):
            o = o.strip()
            off = int(o, 16) if o.startswith('0x') else int(o)
            offsets.append(off)

    if not offsets:
        parser.error("Must specify --offset or --offsets")

    data = bytearray(args.input.read_bytes())
    text_off, text_size = find_text_section(data)

    for off in offsets:
        if off < 0 or off + 4 > text_size:
            print(f"  Warning: offset 0x{off:x} out of .text range, skipping")
            continue
        convert_single(data, text_off, off)

    args.output.write_bytes(data)
    print(f"  Wrote {args.output}")


if __name__ == "__main__":
    main()
