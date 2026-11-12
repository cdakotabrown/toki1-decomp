#!/usr/bin/env python3
"""Build a minimal MIPS ELF .o file containing raw bytes for an ASM-only function.

For overlay functions tagged `Status: ASM-ONLY`, the original assembly is
hand-tuned (raw .word, $at use, BGEZAL, etc.) and cannot be matched from C.
Rather than decompile, we extract the original bytes from the source EXN
file and wrap them in a minimal ELF object that the linker will splice in
at the right address.

The resulting .o has:
- A single `.text` section holding the raw function bytes
- A single global FUNC symbol (the function name) at offset 0
- No relocations (the original bytes are absolute-resolved)

Usage:
    build_asm_only_obj.py --overlay DATE --func func_80132728 \\
                          --size 5916 --output build/overlay/DATE/obj/func_80132728.o
"""
import argparse
import struct
import sys
from pathlib import Path


def build_minimal_elf(func_name: str, text_bytes: bytes) -> bytes:
    """Build a tiny MIPS little-endian ELF .o with one .text section + symbol."""
    # MIPS ELF32 little-endian; sections needed:
    # [0] NULL, [1] .text, [2] .shstrtab, [3] .symtab, [4] .strtab
    text_size = len(text_bytes)

    # String tables
    shstrtab_data = b'\x00.text\x00.shstrtab\x00.symtab\x00.strtab\x00'
    strtab_data = b'\x00' + func_name.encode('ascii') + b'\x00'
    func_name_off = 1  # offset of func_name in strtab

    # Symbols: NULL + FUNC global
    # Elf32_Sym = (st_name:4, st_value:4, st_size:4, st_info:1, st_other:1, st_shndx:2)
    sym_null = struct.pack('<IIIBBH', 0, 0, 0, 0, 0, 0)
    # st_info: bind=GLOBAL(1), type=FUNC(2) -> (1<<4)|2 = 0x12
    sym_func = struct.pack('<IIIBBH', func_name_off, 0, text_size, 0x12, 0, 1)  # shndx=1 (.text)
    symtab_data = sym_null + sym_func

    # Section header layout:
    # SHT_NULL=0, SHT_PROGBITS=1, SHT_SYMTAB=2, SHT_STRTAB=3
    # SHF_ALLOC=2, SHF_EXECINSTR=4

    # Compute section offsets after header (52 bytes) + 5 sections * 40 bytes
    e_shoff_estimate = 52 + 5 * 40  # but we'll place section headers at end

    # Layout (data first, then section headers):
    # ELF header: 0..52
    # .text data: 52..52+text_size
    # .shstrtab data: ...
    # .symtab data: ...
    # .strtab data: ...
    # Section headers: at end

    off = 52
    text_off = off
    off += text_size
    # Align to 4
    while off % 4 != 0:
        off += 1

    shstrtab_off = off
    off += len(shstrtab_data)
    while off % 4 != 0:
        off += 1

    symtab_off = off
    off += len(symtab_data)
    while off % 4 != 0:
        off += 1

    strtab_off = off
    off += len(strtab_data)
    while off % 4 != 0:
        off += 1

    e_shoff = off
    e_shnum = 5
    e_shstrndx = 2  # index of .shstrtab in section table

    # ELF header
    elf_header = bytearray(52)
    elf_header[0:4] = b'\x7fELF'
    elf_header[4] = 1  # ELFCLASS32
    elf_header[5] = 1  # ELFDATA2LSB (little-endian)
    elf_header[6] = 1  # EV_CURRENT
    elf_header[7] = 0  # ELFOSABI_NONE
    # bytes 8-15: padding (0)
    struct.pack_into('<H', elf_header, 16, 1)        # e_type = ET_REL
    struct.pack_into('<H', elf_header, 18, 8)        # e_machine = EM_MIPS
    struct.pack_into('<I', elf_header, 20, 1)        # e_version
    struct.pack_into('<I', elf_header, 24, 0)        # e_entry
    struct.pack_into('<I', elf_header, 28, 0)        # e_phoff
    struct.pack_into('<I', elf_header, 32, e_shoff)  # e_shoff
    struct.pack_into('<I', elf_header, 36, 0x70000000 | 0x00001000)  # e_flags = MIPS_ARCH_1
    struct.pack_into('<H', elf_header, 40, 52)       # e_ehsize
    struct.pack_into('<H', elf_header, 42, 0)        # e_phentsize
    struct.pack_into('<H', elf_header, 44, 0)        # e_phnum
    struct.pack_into('<H', elf_header, 46, 40)       # e_shentsize
    struct.pack_into('<H', elf_header, 48, e_shnum)  # e_shnum
    struct.pack_into('<H', elf_header, 50, e_shstrndx)  # e_shstrndx

    # Build section data block
    data = bytearray(e_shoff)
    data[0:52] = elf_header
    data[text_off:text_off + text_size] = text_bytes
    data[shstrtab_off:shstrtab_off + len(shstrtab_data)] = shstrtab_data
    data[symtab_off:symtab_off + len(symtab_data)] = symtab_data
    data[strtab_off:strtab_off + len(strtab_data)] = strtab_data

    # Section headers
    # Each Shdr: (sh_name:4, sh_type:4, sh_flags:4, sh_addr:4, sh_offset:4,
    #            sh_size:4, sh_link:4, sh_info:4, sh_addralign:4, sh_entsize:4)
    sh_names = {
        '': 0,
        '.text': 1,
        '.shstrtab': 7,
        '.symtab': 17,
        '.strtab': 25,
    }

    sec_headers = []
    # [0] NULL
    sec_headers.append(struct.pack('<10I',
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0))
    # [1] .text — SHT_PROGBITS, ALLOC|EXEC
    sec_headers.append(struct.pack('<10I',
        sh_names['.text'], 1, 6, 0, text_off, text_size, 0, 0, 4, 0))
    # [2] .shstrtab — SHT_STRTAB
    sec_headers.append(struct.pack('<10I',
        sh_names['.shstrtab'], 3, 0, 0, shstrtab_off, len(shstrtab_data),
        0, 0, 1, 0))
    # [3] .symtab — SHT_SYMTAB; sh_link = .strtab idx, sh_info = 1 (1st global)
    sec_headers.append(struct.pack('<10I',
        sh_names['.symtab'], 2, 0, 0, symtab_off, len(symtab_data),
        4, 1, 4, 16))
    # [4] .strtab — SHT_STRTAB
    sec_headers.append(struct.pack('<10I',
        sh_names['.strtab'], 3, 0, 0, strtab_off, len(strtab_data),
        0, 0, 1, 0))

    return bytes(data) + b''.join(sec_headers)


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                  formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--overlay', required=True, help='Overlay name (e.g., DATE)')
    ap.add_argument('--func', required=True, help='Function name (e.g., func_80132728)')
    ap.add_argument('--size', required=True, type=int, help='Function size in bytes')
    ap.add_argument('--output', required=True, help='Output .o file path')
    ap.add_argument('--exn-base', default='0x80132000',
                    help='Overlay load address (default: 0x80132000)')
    args = ap.parse_args()

    # Find EXN file
    root = Path(__file__).parent.parent.parent
    exn_path = root / 'assets' / 'overlays' / f'{args.overlay}.EXN'
    if not exn_path.exists():
        exn_path = root / 'assets' / 'extracted' / 'CDROM' / 'EXEDIR' / f'{args.overlay}.EXN'
    if not exn_path.exists():
        print(f"Error: cannot find {args.overlay}.EXN", file=sys.stderr)
        sys.exit(1)

    # Compute file offset
    # func_XXXXXXXX -> XXXXXXXX, or a landed overlay name through the name table
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import overlay_names
    addr = overlay_names.func_addr(args.overlay.upper(), args.func)
    if addr is None:
        print(f"Error: cannot derive an address for {args.func} in {args.overlay}", file=sys.stderr)
        sys.exit(1)
    base = int(args.exn_base, 16)
    file_off = addr - base
    if file_off < 0:
        print(f"Error: function address {hex(addr)} below overlay base {hex(base)}",
              file=sys.stderr)
        sys.exit(1)

    # Read function bytes
    with open(exn_path, 'rb') as f:
        f.seek(file_off)
        text_bytes = f.read(args.size)

    if len(text_bytes) != args.size:
        print(f"Error: read {len(text_bytes)} bytes, expected {args.size}",
              file=sys.stderr)
        sys.exit(1)

    # Build minimal ELF
    elf_data = build_minimal_elf(args.func, text_bytes)

    out_path = Path(args.output)
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_bytes(elf_data)

    print(f"Wrote {args.output} ({len(elf_data)} bytes ELF, "
          f"{args.size} bytes .text from {exn_path.name}@{hex(file_off)})")


if __name__ == '__main__':
    main()
