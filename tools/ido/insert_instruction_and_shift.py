#!/usr/bin/env python3
"""
insert_instruction_and_shift.py - Insert an arbitrary instruction and shift subsequent code

Inserts a raw instruction word at a given .text offset, shifts all subsequent
instructions by +4, adjusts .rel.text relocation offsets, and fixes branch
immediates (beq/bne/blez/bgtz/bltz/bgez) that cross the insertion point.

Two modes of operation:
  Default: Consumes 4 bytes of trailing NOP padding, keeping .text size constant.
  --grow:  Actually grows .text by 4 bytes and updates all ELF section offsets.

This is a generalization of insert_move_and_shift.py: instead of being
limited to move instructions + sllv patching, it inserts any 32-bit
instruction word specified by --insn.

Typical use (func_800984C4 / CD_init style):
    python3 tools/ido/insert_instruction_and_shift.py in.o out.o \
        --offset 0x48 --insn 0x24a50001 --grow
"""

import argparse
import struct
import sys
from pathlib import Path


def find_section(data: bytes, name: str):
    """Return (section_header_offset, section_file_offset, section_size)."""
    if data[:4] != b"\x7fELF":
        raise ValueError("Not an ELF file")

    e_shoff = struct.unpack("<I", data[32:36])[0]
    e_shentsize = struct.unpack("<H", data[46:48])[0]
    e_shnum = struct.unpack("<H", data[48:50])[0]
    e_shstrndx = struct.unpack("<H", data[50:52])[0]

    shstr_hdr_off = e_shoff + (e_shstrndx * e_shentsize)
    shstr_off = struct.unpack("<I", data[shstr_hdr_off + 16 : shstr_hdr_off + 20])[0]

    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack("<I", data[sh_off : sh_off + 4])[0]
        sec_off = struct.unpack("<I", data[sh_off + 16 : sh_off + 20])[0]
        sec_size = struct.unpack("<I", data[sh_off + 20 : sh_off + 24])[0]

        n0 = shstr_off + sh_name_idx
        n1 = data.find(b"\x00", n0)
        sec_name = data[n0:n1].decode("ascii", errors="ignore")

        if sec_name == name:
            return sh_off, sec_off, sec_size

    return None, None, None


def insert_inplace(text: bytes, insert_off: int, insn_word: int) -> bytes:
    """Insert instruction, shift rest, consume trailing NOP. Same size."""
    if len(text) < 4:
        raise ValueError("text section too small")

    tail = text[-4:]
    if tail != b"\x00\x00\x00\x00":
        raise ValueError(
            "no trailing NOP padding available in .text "
            f"(last 4 bytes: {tail.hex()})"
        )

    insn_bytes = struct.pack("<I", insn_word)
    new_text = text[:insert_off] + insn_bytes + text[insert_off:-4]
    assert len(new_text) == len(text)
    return new_text


def insert_grow(text: bytes, insert_off: int, insn_word: int) -> bytes:
    """Insert instruction, shift rest, grow by 4 bytes."""
    insn_bytes = struct.pack("<I", insn_word)
    new_text = text[:insert_off] + insn_bytes + text[insert_off:]
    assert len(new_text) == len(text) + 4
    return new_text


def is_branch_insn(word: int):
    """Check if a MIPS instruction is a PC-relative branch.

    Returns True for: beq, bne, blez, bgtz, and REGIMM class
    (bltz, bgez, bltzal, bgezal).
    """
    opcode = (word >> 26) & 0x3F
    if opcode in (0x04, 0x05, 0x06, 0x07):  # beq, bne, blez, bgtz
        return True
    if opcode == 0x01:  # REGIMM class
        rt = (word >> 16) & 0x1F
        if rt in (0x00, 0x01, 0x10, 0x11):  # bltz, bgez, bltzal, bgezal
            return True
    return False


def get_branch_imm(word: int) -> int:
    """Extract signed 16-bit branch immediate from a MIPS instruction."""
    imm = word & 0xFFFF
    if imm & 0x8000:
        imm -= 0x10000
    return imm


def set_branch_imm(word: int, imm: int) -> int:
    """Replace the 16-bit branch immediate in a MIPS instruction."""
    return (word & 0xFFFF0000) | (imm & 0xFFFF)


def patch_branch_offsets(data: bytearray, text_off: int, text_size: int,
                         insert_off: int, verbose: bool = False):
    """Adjust branch immediates for branches that cross the insertion point.

    After inserting an instruction at insert_off, branches that straddle the
    insertion point need their immediates adjusted:
      - Branch BEFORE insertion, target AT/AFTER insertion: imm += 1
      - Branch AT/AFTER insertion, target BEFORE insertion: imm -= 1

    The insert_off is in .text-relative coordinates. The instruction at
    insert_off in the new .text is the newly inserted instruction (skipped).
    """
    patched = 0

    for byte_off in range(0, text_size, 4):
        # Skip the newly inserted instruction itself
        if byte_off == insert_off:
            continue

        abs_off = text_off + byte_off
        word = struct.unpack("<I", data[abs_off : abs_off + 4])[0]

        if not is_branch_insn(word):
            continue

        imm = get_branch_imm(word)

        # Compute original position of this instruction before insertion
        if byte_off < insert_off:
            orig_off = byte_off
        else:
            orig_off = byte_off - 4

        # Compute original branch target (PC-relative: target = orig + 4 + imm*4)
        target = orig_off + 4 + imm * 4

        # Check if branch crosses the insertion point
        if orig_off < insert_off and target >= insert_off:
            # Forward branch that crosses insertion: target shifted right
            new_imm = imm + 1
        elif orig_off >= insert_off and target < insert_off:
            # Backward branch that crosses insertion: target didn't move but
            # branch itself shifted right
            new_imm = imm - 1
        else:
            continue

        new_word = set_branch_imm(word, new_imm)
        struct.pack_into("<I", data, abs_off, new_word)
        patched += 1

        if verbose:
            direction = "fwd" if imm >= 0 else "bwd"
            print(
                f"  branch at 0x{byte_off:x} ({direction}): "
                f"imm {imm:+d} -> {new_imm:+d} "
                f"(target 0x{target:x})"
            )

    return patched


def patch_rel_text(data: bytearray, rel_off: int, rel_size: int, insert_off: int):
    """Patch Elf32_Rel entries (little-endian) in-place."""
    if rel_size % 8 != 0:
        raise ValueError(f".rel.text size not multiple of 8: {rel_size}")
    n = rel_size // 8
    patched = 0
    for i in range(n):
        ent_off = rel_off + i * 8
        r_offset = struct.unpack("<I", data[ent_off : ent_off + 4])[0]
        if r_offset >= insert_off:
            struct.pack_into("<I", data, ent_off, r_offset + 4)
            patched += 1
    return patched



def patch_func_symbol_sizes(data: bytearray, insert_off: int, growth: int = 4, verbose: bool = False):
    """Grow the st_size of any STT_FUNC symbol whose address range
    covers insert_off.  This is needed when --grow is used so that
    tools like objdump / overlay_test read the correct function length.
    """
    e_shoff = struct.unpack("<I", data[32:36])[0]
    e_shentsize = struct.unpack("<H", data[46:48])[0]
    e_shnum = struct.unpack("<H", data[48:50])[0]
    e_shstrndx = struct.unpack("<H", data[50:52])[0]

    # Find .symtab and .strtab sections
    symtab_off = symtab_size = strtab_off = None
    for i in range(e_shnum):
        sh = e_shoff + i * e_shentsize
        sh_type = struct.unpack("<I", data[sh + 4 : sh + 8])[0]
        if sh_type == 2:  # SHT_SYMTAB
            symtab_off = struct.unpack("<I", data[sh + 16 : sh + 20])[0]
            symtab_size = struct.unpack("<I", data[sh + 20 : sh + 24])[0]
            strtab_sh = e_shoff + struct.unpack("<I", data[sh + 24 : sh + 28])[0] * e_shentsize
            strtab_off = struct.unpack("<I", data[strtab_sh + 16 : strtab_sh + 20])[0]
            break

    if symtab_off is None:
        return 0

    patched = 0
    n_syms = symtab_size // 16
    for i in range(n_syms):
        ent = symtab_off + i * 16
        st_name = struct.unpack("<I", data[ent : ent + 4])[0]
        st_value = struct.unpack("<I", data[ent + 4 : ent + 8])[0]
        st_size = struct.unpack("<I", data[ent + 8 : ent + 12])[0]
        st_info = data[ent + 12]
        st_type = st_info & 0xF  # STT_FUNC = 2

        if st_type != 2:  # not STT_FUNC
            continue

        # Check if function covers the insert offset
        if st_value <= insert_off < st_value + st_size:
            new_size = st_size + growth
            struct.pack_into("<I", data, ent + 8, new_size)
            patched += 1
            if verbose:
                name_end = data[strtab_off + st_name :].index(0)
                name = data[strtab_off + st_name : strtab_off + st_name + name_end].decode()
                print(f"  grew FUNC symbol '{name}' size: 0x{st_size:x} -> 0x{new_size:x}")

    return patched

def main():
    ap = argparse.ArgumentParser(
        description="Insert an arbitrary instruction at a .text offset and shift subsequent code"
    )
    ap.add_argument("input", type=Path, help="Input ELF .o")
    ap.add_argument("output", type=Path, help="Output ELF .o")
    ap.add_argument(
        "--offset",
        required=True,
        type=lambda s: int(s, 0),
        help="Insert offset in .text (bytes, hex or decimal)",
    )
    ap.add_argument(
        "--insn",
        required=True,
        type=lambda s: int(s, 0),
        help="Instruction word to insert (hex or decimal, e.g. 0x24a50001)",
    )
    ap.add_argument(
        "--grow",
        action="store_true",
        help="Grow .text by 4 bytes instead of consuming trailing NOP",
    )
    ap.add_argument(
        "--fix-branches",
        action="store_true",
        help="Adjust branch immediates (beq/bne/etc.) that cross the insertion point",
    )
    ap.add_argument("-v", "--verbose", action="store_true", help="Verbose output")
    args = ap.parse_args()

    if args.offset % 4 != 0:
        print(f"Error: offset must be 4-byte aligned: 0x{args.offset:x}", file=sys.stderr)
        return 1

    data = bytearray(args.input.read_bytes())

    text_sh_off, text_off, text_size = find_section(data, ".text")
    if text_off is None:
        print("Error: .text section not found", file=sys.stderr)
        return 1

    if args.verbose:
        print(f".text: file_off=0x{text_off:x} size=0x{text_size:x}")
        print(f"Inserting 0x{args.insn:08x} at offset 0x{args.offset:x} (grow={args.grow})")

    if args.offset < 0 or args.offset > text_size:
        print(f"Error: offset 0x{args.offset:x} out of range (text size 0x{text_size:x})", file=sys.stderr)
        return 1

    text = bytes(data[text_off : text_off + text_size])

    if args.grow:
        new_text = insert_grow(text, args.offset, args.insn)
        growth = 4

        # Read e_shoff BEFORE growing
        e_shoff_old = struct.unpack("<I", data[32:36])[0]
        e_shentsize = struct.unpack("<H", data[46:48])[0]
        e_shnum = struct.unpack("<H", data[48:50])[0]

        # Replace .text in data (grows the file by 4 bytes)
        data = bytearray(data[:text_off]) + bytearray(new_text) + bytearray(data[text_off + text_size:])

        # Compute new e_shoff (if section header table was after .text, it shifted)
        if e_shoff_old > text_off:
            e_shoff_new = e_shoff_old + growth
            struct.pack_into("<I", data, 32, e_shoff_new)
        else:
            e_shoff_new = e_shoff_old

        # Update all section headers using the NEW e_shoff
        for i in range(e_shnum):
            sh_off = e_shoff_new + (i * e_shentsize)
            sec_off = struct.unpack("<I", data[sh_off + 16 : sh_off + 20])[0]
            sec_size = struct.unpack("<I", data[sh_off + 20 : sh_off + 24])[0]

            # Update .text section size
            if sec_off == text_off:
                struct.pack_into("<I", data, sh_off + 20, text_size + growth)
            # Update offsets for sections that were after .text
            elif sec_off > text_off:
                struct.pack_into("<I", data, sh_off + 16, sec_off + growth)

        if args.verbose:
            print(f".text grew from 0x{text_size:x} to 0x{text_size + growth:x} bytes")
    else:
        new_text = insert_inplace(text, args.offset, args.insn)
        data[text_off : text_off + text_size] = new_text

    # Optionally patch branch immediates that cross the insertion point
    if args.fix_branches:
        _, new_text_off, new_text_size = find_section(data, ".text")
        branch_patched = patch_branch_offsets(
            data, new_text_off, new_text_size, args.offset, verbose=args.verbose
        )
        if args.verbose:
            print(f"Patched {branch_patched} branch immediate(s)")

    # Patch relocations if present
    # Re-find .rel.text since offsets may have shifted in grow mode
    rel_sh_off, rel_off, rel_size = find_section(data, ".rel.text")
    if rel_off is not None:
        rel_patched = patch_rel_text(data, rel_off, rel_size, args.offset)
        if args.verbose:
            print(
                f".rel.text: patched {rel_patched} relocation offsets (+4) "
                f"at r_offset>=0x{args.offset:x}"
            )
    else:
        if args.verbose:
            print("Note: no .rel.text section found, skipping relocation patch")

    # Update FUNC symbol sizes when --grow is used
    if args.grow:
        sym_patched = patch_func_symbol_sizes(data, args.offset, growth=4, verbose=args.verbose)
        if args.verbose and sym_patched:
            print(f"Grew {sym_patched} FUNC symbol size(s)")

    args.output.write_bytes(data)
    if args.verbose:
        print(f"Wrote {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

