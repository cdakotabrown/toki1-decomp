#!/usr/bin/env python3
"""
Strip leading instructions from the .text section of a MIPS ELF object file.

Removes the first N instructions (each 4 bytes) from the .text section and
adjusts all relocations and symbol table entries accordingly. This is useful
for removing cross-prologue instructions that IDO hoists above the function
prologue when the target binary does not have them.

This tool:
1. Opens an ELF32 little-endian MIPS .o file
2. Finds the .text section
3. Removes the first --count instructions (each 4 bytes) from .text data
4. Adjusts all .rel.text relocation offsets by subtracting count*4
5. Removes any relocations that pointed to the stripped region (offset < count*4)
6. Updates the .text section size in the section header
7. Updates symbol table entries for .text symbols (reduces size by count*4)
8. Writes the result to the output file

USAGE EXAMPLES:

  Strip first 2 instructions (8 bytes):
    python3 strip_leading_instructions.py input.o output.o --count 2

  Verbose mode (show details):
    python3 strip_leading_instructions.py input.o output.o --count 2 -v

WHY THIS IS NEEDED:

IDO at -O2 sometimes hoists instructions above the function prologue
(cross-prologue scheduling). When the target binary does not have these
hoisted instructions, they need to be stripped from the compiled output.

TECHNICAL DETAILS:

This tool directly modifies the ELF file structure:
  1. Parses ELF header to locate .text, .rel.text, and .symtab sections
  2. Removes the first count*4 bytes from .text data
  3. Shifts remaining .text data forward (padding with zeros at end)
  4. Adjusts relocation offsets and removes invalid ones
  5. Updates section size and symbol sizes

AUTHORS:
  - Tokimeki Memorial decomp (February 2026)

LICENSE:
  MIT License - Free to use and modify
"""

import sys
import struct
import argparse


def _find_named_func_text_offset(elf_data, text_section_idx, symtab_offset,
                                 symtab_size, endian, strtab_offset, func_name):
    """Return the .text-relative start offset of the FUNC symbol `func_name`.

    `--count auto` used to guess the TU's real function as the LARGEST
    FUNC symbol in .text. That guess is wrong for any function smaller
    than the types.h helper bodies GCC inlines ahead of it: on
    SetSemiTrans (11 words) the largest symbol is the 14-word
    `read_u32_unaligned` pad itself, so auto computed a pad of 0 and
    stripped nothing. When the caller knows the name (the build always
    does) we look it up directly instead of guessing.

    Returns None when the name is not a defined .text FUNC symbol, so the
    caller can fall back to the size heuristic.
    """
    if not func_name or symtab_offset is None or not symtab_size:
        return None
    if strtab_offset is None:
        return None
    n_syms = symtab_size // 16
    for i in range(n_syms):
        soff = symtab_offset + i * 16
        st_name = struct.unpack(endian + 'I', elf_data[soff:soff + 4])[0]
        st_value = struct.unpack(endian + 'I', elf_data[soff + 4:soff + 8])[0]
        st_info = elf_data[soff + 12]
        st_shndx = struct.unpack(endian + 'H', elf_data[soff + 14:soff + 16])[0]
        if (st_info & 0xF) != 2:        # STT_FUNC
            continue
        if st_shndx != text_section_idx:
            continue
        end = elf_data.index(b'\x00', strtab_offset + st_name)
        name = elf_data[strtab_offset + st_name:end].decode('ascii', 'replace')
        if name == func_name:
            return st_value
    return None


def _find_main_func_text_offset(elf_data, text_section_idx, symtab_offset, symtab_size, endian, strtab_data):
    """Return the .text-relative start offset of the 'main' (largest, named)
    FUNC symbol.

    When .text contains pre-function pad code (e.g. inlined helpers from
    types.h like `read_u32_unaligned`), the leading bytes belong to a
    different function — stripping from .text+0 would remove the pad, not
    the target function's prologue. This helper returns the start of the
    FUNC symbol with the largest non-zero st_size so that strip_leading
    operates on the function body itself.

    Returns 0 if no qualifying symbol is found (preserves legacy behavior).
    """
    if symtab_offset is None or symtab_size is None:
        return 0
    if symtab_size == 0:
        return 0
    n_syms = symtab_size // 16
    best_value = 0
    best_size = 0
    found = False
    for i in range(n_syms):
        soff = symtab_offset + i * 16
        st_value = struct.unpack(endian + 'I', elf_data[soff+4:soff+8])[0]
        st_size = struct.unpack(endian + 'I', elf_data[soff+8:soff+12])[0]
        st_info = elf_data[soff + 12]
        st_shndx = struct.unpack(endian + 'H', elf_data[soff+14:soff+16])[0]
        st_type = st_info & 0xF
        if st_type != 2:  # STT_FUNC
            continue
        if st_shndx != text_section_idx:
            continue
        if st_size == 0:
            continue
        # Pick the largest defined FUNC symbol — that's the function
        # the C file is intended to compile.
        if not found or st_size > best_size:
            best_value = st_value
            best_size = st_size
            found = True
    return best_value


def strip_leading_instructions(elf_data, count, verbose=False, from_function=False,
                               func_name=None):
    """Strip leading instructions from the .text section of a MIPS ELF object.

    ``count`` may be the string ``"auto"``: strip exactly the pre-function
    pad, i.e. everything before the largest named FUNC symbol in .text.
    That is the whole point of the directive for a one-function TU — the
    inlined helper bodies that ``types.h`` emits ahead of the real body —
    and the pad length varies by compiler (IDO emits 4 thunk words; PSY-Q
    GCC emits the full helper bodies, 18-19 words depending on version).
    Hand-counting it is the only reason the count is ever wrong.

    By default the strip begins at .text+0 (legacy behavior). When
    ``from_function`` is true, the strip begins at the start of the
    largest named FUNC symbol in .text — useful when .text contains
    pre-function pad code emitted from inlined header helpers (e.g.
    `read_u32_unaligned` from types.h). The from-function mode is
    auto-engaged when (a) the function symbol is at .text+0x10 or
    later, (b) the bytes preceding the function look like an inlined
    helper (small `jr $ra; nop` body), and (c) the legacy mode would
    consume those helper bytes.

    Args:
        elf_data: Raw ELF file bytes
        count: Number of instructions (4 bytes each) to strip
        verbose: Print detailed information
        from_function: If true, strip from the function symbol start
            instead of from .text+0.

    Returns:
        (patched_data, stripped_count) tuple
    """
    patched = bytearray(elf_data)
    auto = isinstance(count, str) and count.lower() == 'auto'
    strip_bytes = 0 if auto else count * 4

    # Check ELF magic
    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

    if verbose and not auto:
        print(f"ELF endianness: {'big' if is_big_endian else 'little'}")
        print(f"Stripping {count} instructions ({strip_bytes} bytes)")

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
    strtab_offset = None

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
        elif name == '.strtab':
            strtab_offset = sh_offset_data

    if text_offset is None:
        raise ValueError(".text section not found")

    # Locate the main FUNC symbol in .text (skip any pre-function pad
    # code emitted from inlined header helpers such as
    # `read_u32_unaligned` in types.h).
    func_sym_offset = None
    if func_name:
        func_sym_offset = _find_named_func_text_offset(
            elf_data, text_section_idx, symtab_offset, symtab_size, endian,
            strtab_offset, func_name
        )
        if func_sym_offset is None and verbose:
            print(f"--func {func_name}: not a defined .text FUNC symbol — "
                  f"falling back to the largest-symbol heuristic")
    if func_sym_offset is None:
        func_sym_offset = _find_main_func_text_offset(
            elf_data, text_section_idx, symtab_offset, symtab_size, endian, shstrtab
        )

    # Auto-engage from-function mode when:
    #   - the function symbol is not at .text+0,
    #   - legacy stripping would land entirely inside the pre-function
    #     pad region (count*4 <= func_sym_offset).
    # In that case the user almost certainly meant to strip from the
    # function body (helper code from headers shouldn't be touched).
    # Exception: when strip_bytes == func_sym_offset, the user is
    # stripping exactly the entire pre-function pad (e.g., a
    # cross-prologue thunk) — that's a legitimate operation, so
    # we don't auto-engage in that case.
    if auto:
        if func_sym_offset == 0:
            # No pre-function pad: "strip the pad" is a no-op, not an error.
            # (A TU that pulls in no inlined types.h helper has nothing ahead
            #  of the body.) Say so loudly, then pass the object through.
            print("--count auto: no pre-function pad (function symbol is "
                  "already at .text+0) — nothing stripped")
            return bytes(patched), 0
        strip_bytes = func_sym_offset
        count = strip_bytes // 4
        if verbose:
            print(f"--count auto: stripping the {count}-instruction "
                  f"pre-function pad (func at .text+0x{func_sym_offset:x})")

    if (not from_function and func_sym_offset > 0
            and strip_bytes < func_sym_offset):
        from_function = True
        if verbose:
            print(f"Auto-engaging --from-function: strip count {count} "
                  f"would land in pre-function pad region "
                  f"(func at .text+0x{func_sym_offset:x})")

    func_offset_in_text = func_sym_offset if from_function else 0

    # Absolute file offset where the strip begins.
    strip_start = text_offset + func_offset_in_text

    if verbose:
        print(f".text at offset 0x{text_offset:x}, size 0x{text_size:x} ({text_size} bytes)")
        if func_offset_in_text:
            print(f"Function symbol begins at .text+0x{func_offset_in_text:x}; "
                  f"stripping from there")

    available_bytes = text_size - func_offset_in_text
    if strip_bytes > available_bytes:
        raise ValueError(
            f"Cannot strip {strip_bytes} bytes from {available_bytes}-byte function region"
        )

    if strip_bytes == available_bytes:
        raise ValueError("Cannot strip the entire function body")

    # Show the instructions being stripped (relative to .text and to the function)
    if verbose:
        print(f"Stripping instructions:")
        for i in range(count):
            off = strip_start + i * 4
            word = struct.unpack(endian + 'I', elf_data[off:off+4])[0]
            print(f"  .text+0x{func_offset_in_text + i*4:04x}: 0x{word:08x}")

    # Step 1: Remove strip_bytes from inside .text starting at the
    # function symbol. Shift the post-strip remainder forward and pad
    # tail with zeros so the file layout is stable.
    new_text_size = text_size - strip_bytes
    pre = elf_data[text_offset:strip_start]
    post = elf_data[strip_start + strip_bytes:text_offset + text_size]
    padded_text = pre + post + b'\x00' * strip_bytes
    patched[text_offset:text_offset + text_size] = padded_text

    # Step 2: Update .text section size in section header
    struct.pack_into(endian + 'I', patched, text_section_header_off + 20, new_text_size)

    if verbose:
        print(f"New .text size: 0x{new_text_size:x} ({new_text_size} bytes)")

    # Step 3: Adjust relocations in .rel.text. Only relocations at or
    # beyond the strip region need adjustment; relocations in the
    # pre-function region are unaffected.
    strip_lo = func_offset_in_text
    strip_hi = func_offset_in_text + strip_bytes
    removed_relocs = 0
    if rel_text_offset and rel_text_size:
        num_relocs = rel_text_size // 8
        new_relocs = bytearray()

        for i in range(num_relocs):
            rel_off = rel_text_offset + i * 8
            r_offset = struct.unpack(endian + 'I', patched[rel_off:rel_off+4])[0]
            r_info = struct.unpack(endian + 'I', patched[rel_off+4:rel_off+8])[0]

            if strip_lo <= r_offset < strip_hi:
                # Reloc points into the stripped instructions — drop it
                removed_relocs += 1
                if verbose:
                    r_type = r_info & 0xFF
                    r_sym = r_info >> 8
                    print(f"  Removing reloc at 0x{r_offset:x} (type={r_type}, sym={r_sym})")
                continue

            if r_offset >= strip_hi:
                new_r_offset = r_offset - strip_bytes
            else:
                # Pre-function reloc — unchanged
                new_r_offset = r_offset

            if verbose and new_r_offset != r_offset:
                print(f"  Reloc 0x{r_offset:x} -> 0x{new_r_offset:x}")

            new_relocs.extend(struct.pack(endian + 'I', new_r_offset))
            new_relocs.extend(struct.pack(endian + 'I', r_info))

        # Write adjusted relocations back, pad remainder with zeros
        new_rel_size = len(new_relocs)
        padded_relocs = new_relocs + b'\x00' * (rel_text_size - new_rel_size)
        patched[rel_text_offset:rel_text_offset + rel_text_size] = padded_relocs

        # Update .rel.text section size
        struct.pack_into(endian + 'I', patched, rel_text_header_off + 20, new_rel_size)

        if verbose:
            kept = num_relocs - removed_relocs
            print(f"Relocations: {num_relocs} total, {removed_relocs} removed, {kept} adjusted")

    # Step 4: Adjust symbols in .symtab
    if symtab_offset and symtab_size:
        num_syms = symtab_size // 16
        for i in range(num_syms):
            sym_off = symtab_offset + i * 16
            st_value = struct.unpack(endian + 'I', patched[sym_off+4:sym_off+8])[0]
            st_size = struct.unpack(endian + 'I', patched[sym_off+8:sym_off+12])[0]
            st_shndx = struct.unpack(endian + 'H', patched[sym_off+14:sym_off+16])[0]

            if st_shndx != text_section_idx:
                continue

            # Adjust symbol value (offset within section)
            if st_value >= strip_hi:
                new_value = st_value - strip_bytes
            elif st_value >= strip_lo:
                new_value = strip_lo
            else:
                new_value = st_value

            # Reduce size by the bytes stripped from within this symbol's range
            if st_size > 0:
                sym_start = st_value
                sym_end = st_value + st_size
                # Intersection of [sym_start, sym_end) with [strip_lo, strip_hi)
                inter_lo = max(sym_start, strip_lo)
                inter_hi = min(sym_end, strip_hi)
                stripped_within = max(0, inter_hi - inter_lo)
                new_size = st_size - stripped_within
            else:
                new_size = st_size

            if verbose and (new_value != st_value or new_size != st_size):
                print(f"  Symbol: value 0x{st_value:x}->0x{new_value:x}, "
                      f"size 0x{st_size:x}->0x{new_size:x}")

            struct.pack_into(endian + 'I', patched, sym_off + 4, new_value)
            struct.pack_into(endian + 'I', patched, sym_off + 8, new_size)

    return bytes(patched), count


def main():
    parser = argparse.ArgumentParser(
        description='Strip leading instructions from .text section of MIPS ELF object file',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
EXAMPLES:
  # Strip first 2 instructions (8 bytes)
  %(prog)s input.o output.o --count 2

  # Verbose mode
  %(prog)s input.o output.o --count 2 -v

NOTES:
  Each instruction is 4 bytes (MIPS I). --count 2 removes 8 bytes.
  Relocations pointing to the stripped region are removed.
  Remaining relocations have their offsets adjusted.
  Symbol sizes are reduced accordingly.
        """)

    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('--count', required=True,
                        help='Number of instructions to strip (each 4 bytes), '
                             'or "auto" to strip exactly the pre-function pad '
                             '(everything before the largest FUNC symbol)')
    parser.add_argument('--from-function', action='store_true',
                        help='Strip from the start of the largest FUNC '
                             'symbol in .text instead of from .text+0. '
                             'Auto-engages when the strip count would '
                             'otherwise land inside pre-function pad code.')
    parser.add_argument('--func', default=None,
                        help='Name of the TU function. With --count auto the '
                             'pre-function pad is measured up to THIS symbol '
                             'instead of guessing the largest FUNC symbol '
                             '(which is wrong whenever the inlined types.h '
                             'helpers are bigger than the function itself).')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Print detailed changes')

    args = parser.parse_args()

    # Validate
    if isinstance(args.count, str) and args.count.lower() == 'auto':
        args.count = 'auto'
    else:
        try:
            args.count = int(args.count, 0)
        except ValueError:
            parser.error("--count must be a positive integer or 'auto'")
        if args.count <= 0:
            parser.error("--count must be a positive integer or 'auto'")

    # Read input
    try:
        with open(args.input, 'rb') as f:
            elf_data = f.read()
    except IOError as e:
        print(f"ERROR: Cannot read input file: {e}")
        return 1

    # Strip
    try:
        patched, stripped = strip_leading_instructions(
            elf_data, args.count,
            verbose=args.verbose,
            from_function=args.from_function,
            func_name=args.func,
        )
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

    print(f"Stripped {stripped} leading instructions ({stripped * 4} bytes) from .text")
    return 0


if __name__ == '__main__':
    sys.exit(main())
