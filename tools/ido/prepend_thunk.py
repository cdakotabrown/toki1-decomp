#!/usr/bin/env python3
"""
Prepend thunk instructions to the .text section of a MIPS ELF object file.

This is the inverse of strip_leading_instructions.py. It inserts N instructions
at the beginning of .text and shifts all existing relocations and symbols forward.

Unlike strip_leading_instructions (which shrinks .text and pads), this tool needs
to GROW .text. Strategy: rebuild the ELF with expanded .text section, adjusting
all section offsets that follow.

USAGE:
  python3 prepend_thunk.py input.o output.o --insns 0x3c0e8013 0x8dce140c

AUTHORS:
  Tokimeki Memorial decomp (March 2026)
"""

import sys
import struct
import argparse


def prepend_thunk_to_elf(elf_data, thunk_bytes, verbose=False):
    """Prepend thunk bytes to .text section, rebuilding ELF layout."""
    if len(thunk_bytes) % 4 != 0:
        raise ValueError(f"Thunk size {len(thunk_bytes)} is not a multiple of 4")

    thunk_size = len(thunk_bytes)
    endian = '<'  # Always little-endian for PS1

    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    if verbose:
        print(f"Prepending {thunk_size} bytes ({thunk_size // 4} instructions)")

    # Parse ELF header
    e_shoff = struct.unpack(endian + 'I', elf_data[32:36])[0]
    e_shentsize = struct.unpack(endian + 'H', elf_data[46:48])[0]
    e_shnum = struct.unpack(endian + 'H', elf_data[48:50])[0]
    e_shstrndx = struct.unpack(endian + 'H', elf_data[50:52])[0]

    # Parse all section headers
    sections = []
    for i in range(e_shnum):
        off = e_shoff + i * e_shentsize
        sh = {
            'idx': i,
            'sh_name': struct.unpack(endian + 'I', elf_data[off:off+4])[0],
            'sh_type': struct.unpack(endian + 'I', elf_data[off+4:off+8])[0],
            'sh_flags': struct.unpack(endian + 'I', elf_data[off+8:off+12])[0],
            'sh_addr': struct.unpack(endian + 'I', elf_data[off+12:off+16])[0],
            'sh_offset': struct.unpack(endian + 'I', elf_data[off+16:off+20])[0],
            'sh_size': struct.unpack(endian + 'I', elf_data[off+20:off+24])[0],
            'sh_link': struct.unpack(endian + 'I', elf_data[off+24:off+28])[0],
            'sh_info': struct.unpack(endian + 'I', elf_data[off+28:off+32])[0],
            'sh_addralign': struct.unpack(endian + 'I', elf_data[off+32:off+36])[0],
            'sh_entsize': struct.unpack(endian + 'I', elf_data[off+36:off+40])[0],
        }
        sections.append(sh)

    # Get section names via shstrtab
    shstrtab = sections[e_shstrndx]
    strtab_data = elf_data[shstrtab['sh_offset']:shstrtab['sh_offset']+shstrtab['sh_size']]

    def get_name(sh):
        idx = sh['sh_name']
        end = strtab_data.find(b'\x00', idx)
        return strtab_data[idx:end].decode('ascii')

    for sh in sections:
        sh['name'] = get_name(sh)

    # Find key sections
    text_sh = None
    rel_text_sh = None
    symtab_sh = None
    for sh in sections:
        if sh['name'] == '.text':
            text_sh = sh
        elif sh['name'] == '.rel.text':
            rel_text_sh = sh
        elif sh['name'] == '.symtab':
            symtab_sh = sh

    if text_sh is None:
        raise ValueError(".text section not found")

    text_offset = text_sh['sh_offset']
    text_size = text_sh['sh_size']

    if verbose:
        print(f".text at file offset 0x{text_offset:x}, size 0x{text_size:x}")

    # Find target FUNC symbol — the smallest non-zero st_value of FUNC-type
    # symbol in .text. The thunk is conceptually part of THAT function's body
    # (it's the cross-prologue instruction that hoisted above the prologue),
    # so the thunk must be inserted AT that function's entry point — NOT at
    # the start of .text. The function's st_value stays pinned (the function
    # entry doesn't move), but its st_size grows by thunk_size to cover the
    # extra instructions. Symbols at greater offsets shift forward.
    target_func_text_offset = 0  # offset within .text (i.e. st_value)
    if symtab_sh is not None:
        sym_off = symtab_sh['sh_offset']
        n_syms = symtab_sh['sh_size'] // 16
        for i in range(n_syms):
            soff = sym_off + i * 16
            st_value = struct.unpack(endian + 'I', elf_data[soff+4:soff+8])[0]
            st_info = elf_data[soff + 12]
            st_shndx = struct.unpack(endian + 'H', elf_data[soff+14:soff+16])[0]
            st_type = st_info & 0xF
            # STT_FUNC == 2
            if st_type != 2:
                continue
            if st_shndx != text_sh['idx']:
                continue
            if st_value == 0:
                continue
            if target_func_text_offset == 0 or st_value < target_func_text_offset:
                target_func_text_offset = st_value

    # Build new file: ELF header + sections with expanded .text
    # Strategy: copy everything, but insert thunk_bytes at insertion_point
    # and shift everything after that forward by thunk_size.
    if target_func_text_offset > 0:
        insertion_point = text_offset + target_func_text_offset
        if verbose:
            print(f"  Inserting thunk at file offset 0x{insertion_point:x} "
                  f"(text+0x{target_func_text_offset:x})")
    else:
        insertion_point = text_offset  # Fallback: insert at .text start
        if verbose:
            print(f"  Inserting thunk at .text start (no FUNC symbol found)")

    # Build new file
    new_data = bytearray()
    new_data += elf_data[:insertion_point]          # Everything before .text
    new_data += thunk_bytes                          # Thunk instructions
    new_data += elf_data[insertion_point:]            # Rest of file (including old .text)

    # Now adjust all offsets in section headers and ELF header
    # Section headers themselves may have moved if they were after insertion_point
    new_e_shoff = e_shoff
    if e_shoff >= insertion_point:
        new_e_shoff += thunk_size

    # Update ELF header's e_shoff
    struct.pack_into(endian + 'I', new_data, 32, new_e_shoff)

    # Update each section header
    for sh in sections:
        sh_hdr_offset = new_e_shoff + sh['idx'] * e_shentsize

        # Update sh_offset: if section data was at or after insertion point, shift forward
        new_sh_offset = sh['sh_offset']
        if sh['sh_offset'] >= insertion_point and sh['idx'] != 0:
            new_sh_offset += thunk_size

        # For .text section: offset stays the same (thunk goes at insertion_point),
        # but size grows
        if sh['name'] == '.text':
            new_sh_size = text_size + thunk_size
            struct.pack_into(endian + 'I', new_data, sh_hdr_offset + 20, new_sh_size)
            # sh_offset doesn't change — thunk was inserted AT the start
            if verbose:
                print(f"  .text: size 0x{text_size:x} -> 0x{new_sh_size:x}")
        else:
            # Other sections: update offset if it moved
            struct.pack_into(endian + 'I', new_data, sh_hdr_offset + 16, new_sh_offset)

    # Adjust .rel.text relocations: shift offsets forward by thunk_size,
    # but only for relocs that target instructions at or after the insertion
    # point (i.e. r_offset >= target_func_text_offset). Relocs targeting
    # earlier functions are unaffected.
    if rel_text_sh:
        rel_offset = rel_text_sh['sh_offset']
        if rel_offset >= insertion_point:
            rel_offset += thunk_size  # The reloc data itself moved
        rel_size = rel_text_sh['sh_size']
        num_relocs = rel_size // 8

        if verbose:
            print(f"  Adjusting {num_relocs} relocations (+{thunk_size} where "
                  f">= 0x{target_func_text_offset:x})")

        for i in range(num_relocs):
            roff = rel_offset + i * 8
            r_offset = struct.unpack(endian + 'I', new_data[roff:roff+4])[0]
            if r_offset >= target_func_text_offset:
                new_r_offset = r_offset + thunk_size
                struct.pack_into(endian + 'I', new_data, roff, new_r_offset)
                if verbose:
                    print(f"    Reloc 0x{r_offset:x} -> 0x{new_r_offset:x}")

    # Adjust .symtab symbols.
    #
    # For .text symbols:
    #   * The TARGET function (smallest non-zero FUNC st_value) keeps its
    #     st_value pinned but its st_size grows by thunk_size (the thunk is
    #     part of its body now).
    #   * Symbols with st_value > target_func_text_offset shift forward by
    #     thunk_size (everything after the insertion point moved).
    #   * Symbols with st_value < target_func_text_offset are unaffected.
    #   * Symbols with st_value == target_func_text_offset that are NOT the
    #     target function (e.g. labels at the same address) stay pinned.
    if symtab_sh:
        sym_offset = symtab_sh['sh_offset']
        if sym_offset >= insertion_point:
            sym_offset += thunk_size
        sym_size = symtab_sh['sh_size']
        num_syms = sym_size // 16

        for i in range(num_syms):
            soff = sym_offset + i * 16
            st_value = struct.unpack(endian + 'I', new_data[soff+4:soff+8])[0]
            st_size = struct.unpack(endian + 'I', new_data[soff+8:soff+12])[0]
            st_info = new_data[soff + 12]
            st_shndx = struct.unpack(endian + 'H', new_data[soff+14:soff+16])[0]
            st_type = st_info & 0xF

            if st_shndx != text_sh['idx']:
                continue

            is_target_func = (st_type == 2 and
                              st_value == target_func_text_offset and
                              target_func_text_offset > 0)

            if is_target_func:
                # Pin st_value, grow st_size
                if st_size > 0:
                    new_size = st_size + thunk_size
                    struct.pack_into(endian + 'I', new_data, soff + 8, new_size)
                if verbose:
                    print(f"  Target symbol: value 0x{st_value:x} (pinned), "
                          f"size {st_size} -> "
                          f"{st_size + thunk_size if st_size > 0 else 0}")
            elif st_value >= target_func_text_offset and target_func_text_offset > 0:
                # Shift forward
                new_value = st_value + thunk_size
                struct.pack_into(endian + 'I', new_data, soff + 4, new_value)
                if verbose:
                    print(f"  Symbol: value 0x{st_value:x} -> 0x{new_value:x}")
            elif target_func_text_offset == 0:
                # Fallback: old behavior — shift everything
                new_value = st_value + thunk_size
                struct.pack_into(endian + 'I', new_data, soff + 4, new_value)
                if st_size > 0:
                    new_size = st_size + thunk_size
                    struct.pack_into(endian + 'I', new_data, soff + 8, new_size)
                if verbose:
                    print(f"  Symbol (fallback): value 0x{st_value:x} -> "
                          f"0x{new_value:x}")
            # else: st_value < target_func_text_offset, leave alone

    return bytes(new_data)


def main():
    parser = argparse.ArgumentParser(
        description='Prepend thunk instructions to .text section of MIPS ELF .o file')
    parser.add_argument('input', help='Input ELF .o file')
    parser.add_argument('output', help='Output ELF .o file')
    parser.add_argument('--insns', nargs='+', required=True,
                        help='Hex instruction words to prepend (e.g., 0x3c0e8013 0x8dce140c)')
    parser.add_argument('-v', '--verbose', action='store_true', help='Verbose output')

    args = parser.parse_args()

    # Parse instruction words to bytes
    thunk_bytes = b''
    for insn in args.insns:
        word = int(insn, 16)
        thunk_bytes += struct.pack('<I', word)

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    result = prepend_thunk_to_elf(elf_data, thunk_bytes, verbose=args.verbose)

    with open(args.output, 'wb') as f:
        f.write(result)

    if args.verbose:
        print(f"Written to {args.output}")


if __name__ == '__main__':
    main()
