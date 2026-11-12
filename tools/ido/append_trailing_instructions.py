#!/usr/bin/env python3
"""Append specific instructions to the end of .text section.

Adds specified instruction words to the end of the .text section, growing the
section as needed. When `.text` grows past its original boundary, this tool
SHIFTS the file offsets of all subsequent sections (and `e_shoff` if it follows
`.text`) so that adjacent sections like `.options`/`.reginfo`/`.symtab` are
preserved instead of overwritten.

This is useful for cross-prologue scheduling where IDO hoists instructions
from the NEXT function into the current function's delay slot area (after jr ra).

Usage:
    python3 append_trailing_instructions.py input.o output.o --insns 0x3c0e800f 0x91cef86a
"""
import argparse
import struct
import sys


def main():
    parser = argparse.ArgumentParser(description="Append instructions to .text section")
    parser.add_argument('input', help='Input ELF .o file')
    parser.add_argument('output', help='Output ELF .o file')
    parser.add_argument('--insns', nargs='+', required=True,
                        help='Hex instruction words to append (e.g., 0x3c0e800f 0x91cef86a)')
    parser.add_argument('-v', '--verbose', action='store_true')
    args = parser.parse_args()

    insns = [int(s, 16) if s.startswith('0x') else int(s) for s in args.insns]

    with open(args.input, 'rb') as f:
        data = bytearray(f.read())

    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shentsize = struct.unpack('<H', data[46:48])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]
    shstr_hdr = e_shoff + e_shstrndx * e_shentsize
    shstr_off = struct.unpack('<I', data[shstr_hdr + 16:shstr_hdr + 20])[0]

    # Parse all section headers into a list so we can update offsets cleanly.
    sections = []
    for i in range(e_shnum):
        h = e_shoff + i * e_shentsize
        nm = struct.unpack('<I', data[h:h+4])[0]
        sh_type = struct.unpack('<I', data[h+4:h+8])[0]
        sh_off = struct.unpack('<I', data[h+16:h+20])[0]
        sh_sz = struct.unpack('<I', data[h+20:h+24])[0]
        end = data.find(b'\x00', shstr_off + nm)
        name = data[shstr_off+nm:end].decode(errors='replace')
        sections.append({'idx': i, 'name': name, 'type': sh_type,
                          'offset': sh_off, 'size': sh_sz})

    text = next((s for s in sections if s['name'] == '.text'), None)
    if text is None:
        print("ERROR: .text section not found", file=sys.stderr)
        sys.exit(1)

    symtab = next((s for s in sections if s['name'] == '.symtab' and s['type'] == 2), None)

    text_idx = text['idx']
    text_off = text['offset']
    text_sz = text['size']
    added = len(insns) * 4

    # Find last function symbol in .text — that's where we append.
    insert_at = text_sz  # fallback
    grow_sym_idx = -1
    if symtab is not None:
        n_syms = symtab['size'] // 16
        last_func_end = -1
        for k in range(n_syms):
            soff = symtab['offset'] + k * 16
            st_value = struct.unpack('<I', data[soff+4:soff+8])[0]
            st_size_i = struct.unpack('<I', data[soff+8:soff+12])[0]
            st_info = data[soff+12]
            st_shndx = struct.unpack('<H', data[soff+14:soff+16])[0]
            st_type = st_info & 0xf
            if st_shndx != text_idx or st_type != 2 or st_size_i == 0:
                continue
            end_v = st_value + st_size_i
            if end_v > last_func_end:
                last_func_end = end_v
                grow_sym_idx = k
        if grow_sym_idx >= 0 and last_func_end <= text_sz:
            insert_at = last_func_end

    needed_text_sz = max(text_sz, insert_at + added)
    text_end = text_off + text_sz
    grow = needed_text_sz - text_sz  # bytes to insert into the file

    # If .text needs to grow past its current file footprint, insert padding
    # bytes at text_end so that following sections shift instead of being
    # overwritten. Then patch every section header offset and e_shoff if they
    # were >= text_end.
    if grow > 0:
        data[text_end:text_end] = bytes(grow)
        for s in sections:
            if s['offset'] >= text_end:
                s['offset'] += grow
        if e_shoff >= text_end:
            e_shoff += grow
            struct.pack_into('<I', data, 32, e_shoff)

    # Update .text size in its section header.
    text['size'] = needed_text_sz
    text_h = e_shoff + text_idx * e_shentsize
    struct.pack_into('<I', data, text_h + 20, text['size'])

    # Persist the (possibly shifted) offsets back into headers.
    for s in sections:
        h = e_shoff + s['idx'] * e_shentsize
        struct.pack_into('<I', data, h + 16, s['offset'])

    # Write the appended instructions at insert_at within .text. Use the
    # post-shift text_off (which equals the original — .text doesn't move
    # within the file, only its size changes).
    for j, insn in enumerate(insns):
        pos = text_off + insert_at + j * 4
        struct.pack_into('<I', data, pos, insn)

    # Grow the last-function symbol so its st_size reflects the appended insns.
    if grow_sym_idx >= 0 and symtab is not None:
        soff = symtab['offset'] + grow_sym_idx * 16
        old_size = struct.unpack('<I', data[soff+8:soff+12])[0]
        struct.pack_into('<I', data, soff+8, old_size + added)

    if args.verbose:
        print(f"Appended {len(insns)} insn(s) at .text+0x{insert_at:x}: "
              f"text {text_sz//4}->{text['size']//4} insns, "
              f"file grew by {grow} bytes, "
              f"sym size grew by {added} bytes")

    with open(args.output, 'wb') as f:
        f.write(bytes(data))


if __name__ == '__main__':
    main()
