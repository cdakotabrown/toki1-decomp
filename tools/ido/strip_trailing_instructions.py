#!/usr/bin/env python3
"""
Strip trailing instructions from the .text section of a MIPS ELF object file.

Removes the last N instructions (each 4 bytes) from the .text section and
adjusts the symbol table entries accordingly. This is useful for removing
trailing nop padding that IDO/ido-static-recomp generates at the end of
some functions due to section alignment requirements.

This tool:
1. Opens an ELF32 little-endian MIPS .o file
2. Finds the .text section
3. Removes the last --count instructions (each 4 bytes) from .text data
4. Removes any relocations that pointed to the stripped region
5. Updates the .text section size in the section header
6. Updates symbol table entries for .text symbols (reduces size by count*4)
7. Writes the result to the output file

USAGE EXAMPLES:

  Strip last 1 instruction (4 bytes):
    python3 strip_trailing_instructions.py input.o output.o --count 1

  Only strip if trailing instructions are nops (0x00000000):
    python3 strip_trailing_instructions.py input.o output.o --count 1 --nop-only

  Verbose mode:
    python3 strip_trailing_instructions.py input.o output.o --count 1 -v

AUTHORS:
  - Tokimeki Memorial decomp (February 2026)

LICENSE:
  MIT License - Free to use and modify
"""

import sys
import struct
import argparse


def strip_trailing_instructions(elf_data, count, nop_only=False, verbose=False):
    """Strip trailing instructions from .text section.

    Args:
        elf_data: Raw ELF file bytes
        count: Number of instructions (4 bytes each) to strip
        nop_only: Only strip if all trailing instructions are nops (0x00000000)
        verbose: Print detailed information

    Returns:
        (patched_data, stripped_count) tuple
    """
    patched = bytearray(elf_data)
    strip_bytes = count * 4

    # Check ELF magic
    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

    if verbose:
        print(f"ELF endianness: {'big' if is_big_endian else 'little'}")
        print(f"Stripping {count} trailing instructions ({strip_bytes} bytes)")

    # Parse ELF header
    e_shoff = struct.unpack(endian + 'I', elf_data[32:36])[0]
    e_shentsize = struct.unpack(endian + 'H', elf_data[46:48])[0]
    e_shnum = struct.unpack(endian + 'H', elf_data[48:50])[0]
    e_shstrndx = struct.unpack(endian + 'H', elf_data[50:52])[0]

    # Get section header string table
    shstrtab_off = e_shoff + e_shstrndx * e_shentsize
    shstrtab_addr = struct.unpack(endian + 'I', elf_data[shstrtab_off+16:shstrtab_off+20])[0]
    shstrtab_size = struct.unpack(endian + 'I', elf_data[shstrtab_off+20:shstrtab_off+24])[0]
    shstrtab = elf_data[shstrtab_addr:shstrtab_addr+shstrtab_size]

    # Find .text, .rel.text, and .symtab sections
    text_offset = None
    text_size = None
    text_section_idx = None
    text_section_header_off = None
    rel_text_offset = None
    rel_text_size = None
    rel_text_header_off = None
    symtab_offset = None
    symtab_size = None

    for i in range(e_shnum):
        sh_offset = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack(endian + 'I', elf_data[sh_offset:sh_offset+4])[0]
        sh_offset_data = struct.unpack(endian + 'I', elf_data[sh_offset+16:sh_offset+20])[0]
        sh_size = struct.unpack(endian + 'I', elf_data[sh_offset+20:sh_offset+24])[0]

        name_end = shstrtab.find(b'\x00', sh_name_idx)
        name = shstrtab[sh_name_idx:name_end].decode('ascii')

        if name == '.text':
            text_offset = sh_offset_data
            text_size = sh_size
            text_section_idx = i
            text_section_header_off = sh_offset
        elif name == '.rel.text':
            rel_text_offset = sh_offset_data
            rel_text_size = sh_size
            rel_text_header_off = sh_offset
        elif name == '.symtab':
            symtab_offset = sh_offset_data
            symtab_size = sh_size

    if text_offset is None:
        raise ValueError(".text section not found")

    if verbose:
        print(f".text at offset 0x{text_offset:x}, size 0x{text_size:x} ({text_size} bytes)")

    if strip_bytes > text_size:
        raise ValueError(
            f"Cannot strip {strip_bytes} bytes from .text section of {text_size} bytes"
        )

    if strip_bytes == text_size:
        raise ValueError("Cannot strip entire .text section")

    # Show the instructions being stripped
    strip_start = text_offset + text_size - strip_bytes
    if verbose:
        print(f"Stripping trailing instructions:")
        for i in range(count):
            off = strip_start + i * 4
            word = struct.unpack(endian + 'I', elf_data[off:off+4])[0]
            print(f"  .text+0x{text_size - strip_bytes + i*4:04x}: 0x{word:08x}")

    # Check if trailing instructions are nops if requested
    if nop_only:
        for i in range(count):
            off = strip_start + i * 4
            word = struct.unpack(endian + 'I', elf_data[off:off+4])[0]
            if word != 0x00000000:
                raise ValueError(
                    f"Trailing instruction at .text+0x{text_size - strip_bytes + i*4:04x} "
                    f"is not a nop (0x{word:08x}); use without --nop-only to force"
                )

    # Step 1: Remove last strip_bytes from .text
    new_text_size = text_size - strip_bytes
    # Zero out the stripped bytes (they'll be beyond the new section size)
    patched[strip_start:strip_start + strip_bytes] = b'\x00' * strip_bytes

    # Step 2: Update .text section size in section header
    struct.pack_into(endian + 'I', patched, text_section_header_off + 20, new_text_size)

    if verbose:
        print(f"New .text size: 0x{new_text_size:x} ({new_text_size} bytes)")

    # Step 3: Remove any relocations that point into the stripped region
    removed_relocs = 0
    stripped_region_start = new_text_size  # offset within .text where stripping begins
    if rel_text_offset and rel_text_size:
        num_relocs = rel_text_size // 8
        new_relocs = bytearray()

        for i in range(num_relocs):
            rel_off = rel_text_offset + i * 8
            r_offset = struct.unpack(endian + 'I', patched[rel_off:rel_off+4])[0]
            r_info = struct.unpack(endian + 'I', patched[rel_off+4:rel_off+8])[0]

            if r_offset >= stripped_region_start:
                # This relocation points to the stripped region - remove it
                removed_relocs += 1
                if verbose:
                    r_type = r_info & 0xFF
                    r_sym = r_info >> 8
                    print(f"  Removing reloc at 0x{r_offset:x} (type={r_type}, sym={r_sym})")
                continue

            new_relocs.extend(struct.pack(endian + 'I', r_offset))
            new_relocs.extend(struct.pack(endian + 'I', r_info))

        # Write adjusted relocations back, pad remainder with zeros
        new_rel_size = len(new_relocs)
        padded_relocs = new_relocs + b'\x00' * (rel_text_size - new_rel_size)
        patched[rel_text_offset:rel_text_offset + rel_text_size] = padded_relocs

        # Update .rel.text section size
        struct.pack_into(endian + 'I', patched, rel_text_header_off + 20, new_rel_size)

        if verbose:
            kept = num_relocs - removed_relocs
            print(f"Relocations: {num_relocs} total, {removed_relocs} removed, {kept} kept")

    # Step 4: Adjust symbols in .symtab (reduce size)
    if symtab_offset and symtab_size:
        num_syms = symtab_size // 16
        for i in range(num_syms):
            sym_off = symtab_offset + i * 16
            st_value = struct.unpack(endian + 'I', patched[sym_off+4:sym_off+8])[0]
            st_size = struct.unpack(endian + 'I', patched[sym_off+8:sym_off+12])[0]
            st_shndx = struct.unpack(endian + 'H', patched[sym_off+14:sym_off+16])[0]

            if st_shndx != text_section_idx:
                continue

            if st_size == 0:
                continue

            sym_end = st_value + st_size
            if sym_end <= stripped_region_start:
                # Symbol entirely before stripped region - no change
                continue

            # How many stripped bytes fall within [st_value, sym_end)?
            overlap_start = max(st_value, stripped_region_start)
            overlap_end = min(sym_end, text_size)
            stripped_within = max(0, overlap_end - overlap_start)
            new_size = st_size - stripped_within

            if verbose and new_size != st_size:
                sym_name_idx = struct.unpack(endian + 'I', patched[sym_off:sym_off+4])[0]
                print(f"  Symbol size: 0x{st_size:x} -> 0x{new_size:x}")

            struct.pack_into(endian + 'I', patched, sym_off + 8, new_size)

    return bytes(patched), count


def main():
    parser = argparse.ArgumentParser(
        description='Strip trailing instructions from .text section of MIPS ELF object file',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
EXAMPLES:
  # Strip last 1 instruction (4 bytes) - trailing nop
  %(prog)s input.o output.o --count 1

  # Strip last 2 instructions only if they are nops
  %(prog)s input.o output.o --count 2 --nop-only

  # Verbose mode
  %(prog)s input.o output.o --count 1 -v

NOTES:
  Each instruction is 4 bytes (MIPS I). --count 1 removes 4 bytes.
  Relocations pointing to the stripped region are removed.
  Symbol sizes are reduced accordingly.
  Use --nop-only to ensure only nop padding is removed (safe mode).
        """)

    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('--count', type=int, required=True,
                        help='Number of trailing instructions to strip (each 4 bytes)')
    parser.add_argument('--nop-only', action='store_true',
                        help='Only strip if trailing instructions are nops (0x00000000)')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Print detailed changes')

    args = parser.parse_args()

    # Validate
    if args.count <= 0:
        parser.error("--count must be a positive integer")

    # Read input
    try:
        with open(args.input, 'rb') as f:
            elf_data = f.read()
    except IOError as e:
        print(f"ERROR: Cannot read input file: {e}")
        return 1

    # Strip
    try:
        patched, stripped = strip_trailing_instructions(
            elf_data, args.count, nop_only=args.nop_only, verbose=args.verbose)
    except ValueError as e:
        print(f"ERROR: {e}")
        return 1

    # Write output
    try:
        with open(args.output, 'wb') as f:
            f.write(patched)
    except IOError as e:
        print(f"ERROR: Cannot write output file: {e}")
        return 1

    print(f"Stripped {stripped} trailing instructions ({stripped * 4} bytes) from .text")
    return 0


if __name__ == '__main__':
    sys.exit(main())
