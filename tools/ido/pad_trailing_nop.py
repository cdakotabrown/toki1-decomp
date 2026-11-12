#!/usr/bin/env python3
"""Pad .text section with trailing nop instructions.

Adds N nop (0x00000000) instructions to the end of the .text section.
Updates ELF section header size AND extends the last function symbol's
st_size so objdump-based tooling sees the appended nops as part of
the function.

If another section immediately follows .text (e.g. .options, .rel.text),
the nops are inserted properly by shifting subsequent sections forward
instead of overwriting them.

Usage:
    python3 pad_trailing_nop.py input.o output.o --count 1
"""
import argparse
import struct
import sys


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('input')
    parser.add_argument('output')
    parser.add_argument('--count', type=int, default=1)
    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        data = bytearray(f.read())

    e_shoff = struct.unpack_from('<I', data, 32)[0]
    e_shentsize = struct.unpack_from('<H', data, 46)[0]
    e_shnum = struct.unpack_from('<H', data, 48)[0]
    e_shstrndx = struct.unpack_from('<H', data, 50)[0]

    shstr_hdr = e_shoff + e_shstrndx * e_shentsize
    shstr_off = struct.unpack_from('<I', data, shstr_hdr + 16)[0]

    text_idx = -1
    text_off = 0
    text_old_sz = 0
    symtab_idx = -1

    for i in range(e_shnum):
        h = e_shoff + i * e_shentsize
        nm = struct.unpack_from('<I', data[h:h + 4])[0]
        sh_type = struct.unpack_from('<I', data[h + 4:h + 8])[0]
        off = struct.unpack_from('<I', data[h + 16:h + 20])[0]
        sz = struct.unpack_from('<I', data[h + 20:h + 24])[0]
        end = data.find(b'\x00', shstr_off + nm)
        name = data[shstr_off + nm:end].decode()
        if name == '.text':
            text_idx = i
            text_off = off
            text_old_sz = sz
        if sh_type == 2:  # SHT_SYMTAB
            symtab_idx = i

    if text_idx < 0:
        print("ERROR: no .text section found")
        sys.exit(1)

    text_end = text_off + text_old_sz
    extra_bytes = args.count * 4
    new_text_sz = text_old_sz + extra_bytes

    # Check if another section immediately follows .text
    gap_to_next = None
    for i in range(e_shnum):
        if i == text_idx or i == 0:
            continue
        h = e_shoff + i * e_shentsize
        sh_type = struct.unpack_from('<I', data[h + 4:h + 8])[0]
        if sh_type == 0:  # NULL
            continue
        sh_offset = struct.unpack_from('<I', data[h + 16:h + 20])[0]
        sh_size = struct.unpack_from('<I', data[h + 20:h + 24])[0]
        if sh_offset >= text_end and sh_size > 0:
            gap = sh_offset - text_end
            if gap_to_next is None or gap < gap_to_next:
                gap_to_next = gap

    if gap_to_next is not None and gap_to_next < extra_bytes:
        # Need to shift sections after .text to make room
        shift = extra_bytes - gap_to_next

        # Insert space at text_end
        data = data[:text_end] + bytearray(shift) + data[text_end:]

        # The section header table may have shifted too.
        # Update e_shoff FIRST since we need it to find other headers.
        if e_shoff >= text_end:
            e_shoff += shift
            struct.pack_into('<I', data, 32, e_shoff)

        # Update section headers: shift any section whose data lives
        # at or after text_end (except .text itself).
        for i in range(e_shnum):
            if i == 0 or i == text_idx:
                continue
            h = e_shoff + i * e_shentsize
            sh_offset = struct.unpack_from('<I', data, h + 16)[0]
            if sh_offset >= text_end:
                struct.pack_into('<I', data, h + 16, sh_offset + shift)

    # Write nops at end of .text (now there's guaranteed room)
    for j in range(args.count):
        pos = text_off + text_old_sz + j * 4
        if pos + 4 <= len(data):
            struct.pack_into('<I', data, pos, 0x00000000)
        else:
            data.extend(b'\x00' * (pos + 4 - len(data)))
            struct.pack_into('<I', data, pos, 0x00000000)

    # Update .text section size
    h_text = e_shoff + text_idx * e_shentsize
    struct.pack_into('<I', data, h_text + 20, new_text_sz)

    # Extend last function symbol so objdump-based tools see the nops
    if symtab_idx >= 0:
        h_sym = e_shoff + symtab_idx * e_shentsize
        sym_off = struct.unpack_from('<I', data, h_sym + 16)[0]
        sym_sz = struct.unpack_from('<I', data, h_sym + 20)[0]
        n_syms = sym_sz // 16
        target_sym = -1
        target_value = -1
        target_old_size = 0
        for k in range(n_syms):
            soff = sym_off + k * 16
            st_value = struct.unpack_from('<I', data, soff + 4)[0]
            st_size = struct.unpack_from('<I', data, soff + 8)[0]
            st_info = data[soff + 12]
            st_shndx = struct.unpack_from('<H', data, soff + 14)[0]
            st_type = st_info & 0xf
            if st_shndx != text_idx or st_type != 2 or st_size == 0:
                continue
            if st_value > target_value:
                target_value = st_value
                target_sym = k
                target_old_size = st_size
        if target_sym >= 0:
            soff = sym_off + target_sym * 16
            new_size = target_old_size + extra_bytes
            struct.pack_into('<I', data, soff + 8, new_size)

    print(f"Padded .text with {args.count} nop(s): {text_old_sz//4} -> {new_text_sz//4} instructions")

    with open(args.output, 'wb') as f:
        f.write(bytes(data))


if __name__ == '__main__':
    main()
