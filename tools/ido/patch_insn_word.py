#!/usr/bin/env python3
"""patch_insn_word.py - Replace entire instruction word at a given offset.

Usage:
    python3 patch_insn_word.py input.o output.o --offset 0x60 --word 0x2c410003

The offset is a .text section-relative address (including thunk prefix).
"""
import struct, sys, argparse
from pathlib import Path

def find_text_section(data):
    e_shoff = struct.unpack("<I", data[32:36])[0]
    e_shentsize = struct.unpack("<H", data[46:48])[0]
    e_shnum = struct.unpack("<H", data[48:50])[0]
    e_shstrndx = struct.unpack("<H", data[50:52])[0]
    shstr_off = struct.unpack("<I", data[e_shoff + e_shstrndx * e_shentsize + 16:
                                        e_shoff + e_shstrndx * e_shentsize + 20])[0]
    for i in range(e_shnum):
        sh = e_shoff + i * e_shentsize
        name_idx = struct.unpack("<I", data[sh:sh+4])[0]
        sec_off = struct.unpack("<I", data[sh+16:sh+20])[0]
        sec_size = struct.unpack("<I", data[sh+20:sh+24])[0]
        name = data[shstr_off+name_idx:].split(b'\x00')[0].decode()
        if name == '.text':
            return sec_off, sec_size
    raise ValueError(".text section not found")

def main():
    p = argparse.ArgumentParser()
    p.add_argument("input", type=Path)
    p.add_argument("output", type=Path)
    p.add_argument("--offset", required=True, help=".text-section-relative offset (hex)")
    p.add_argument("--word", required=True, help="Replacement instruction word (hex)")
    args = p.parse_args()

    data = bytearray(args.input.read_bytes())
    text_off, text_size = find_text_section(data)
    off = int(args.offset, 16) if args.offset.startswith('0x') else int(args.offset)
    word = int(args.word, 16) if args.word.startswith('0x') else int(args.word)

    abs_off = text_off + off
    if abs_off + 4 > text_off + text_size:
        print(f"error: offset 0x{off:x} out of .text range", file=sys.stderr)
        return 1

    old = struct.unpack("<I", data[abs_off:abs_off+4])[0]
    struct.pack_into("<I", data, abs_off, word)
    args.output.write_bytes(data)
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
