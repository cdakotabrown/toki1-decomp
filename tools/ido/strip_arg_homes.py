#!/usr/bin/env python3
"""
strip_arg_homes.py - Remove instructions at specified .text offsets and compact

IDO 5.2 at -O2 generates `sw aX, offset(sp)` "arg home" stores in function
prologues when parameters are declared. Some target binaries don't have these
stores (likely removed by a post-link optimization pass).

This tool removes instructions at specified .text offsets, compacts remaining
code (shifting subsequent instructions up), adjusts branch immediates, and
adjusts ELF relocations.

Two modes:
  Default: Pad end of .text with NOPs to maintain original size.
  --shrink: Actually shrink .text and update ELF section headers.

Usage:
    python3 tools/ido/strip_arg_homes.py in.o out.o --offsets 0x14 0x1c
    python3 tools/ido/strip_arg_homes.py in.o out.o --offsets 0x14 --shrink
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


def is_branch_insn(word: int) -> bool:
    """Check if a MIPS instruction is a PC-relative branch."""
    opcode = (word >> 26) & 0x3F
    if opcode in (0x04, 0x05, 0x06, 0x07):  # beq, bne, blez, bgtz
        return True
    if opcode == 0x01:  # REGIMM class
        rt = (word >> 16) & 0x1F
        if rt in (0x00, 0x01, 0x10, 0x11):  # bltz, bgez, bltzal, bgezal
            return True
    return False


def is_jump_insn(word: int) -> bool:
    """Check if a MIPS instruction is a jump (j/jal)."""
    opcode = (word >> 26) & 0x3F
    return opcode in (0x02, 0x03)  # j, jal


def get_branch_imm(word: int) -> int:
    """Extract signed 16-bit branch immediate."""
    imm = word & 0xFFFF
    if imm & 0x8000:
        imm -= 0x10000
    return imm


def set_branch_imm(word: int, imm: int) -> int:
    """Replace the 16-bit branch immediate."""
    return (word & 0xFFFF0000) | (imm & 0xFFFF)


def disasm_brief(word: int) -> str:
    """Quick disassembly for verbose output."""
    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    imm = word & 0xFFFF
    if imm & 0x8000:
        simm = imm - 0x10000
    else:
        simm = imm

    regs = ["zero","at","v0","v1","a0","a1","a2","a3",
            "t0","t1","t2","t3","t4","t5","t6","t7",
            "s0","s1","s2","s3","s4","s5","s6","s7",
            "t8","t9","k0","k1","gp","sp","s8","ra"]

    if opcode == 0x2B and rs == 29:  # sw rt, imm(sp)
        return f"sw {regs[rt]},{simm}(sp)"
    if opcode == 0x04:
        return f"beq {regs[rs]},{regs[rt]},{simm}"
    if opcode == 0x05:
        return f"bne {regs[rs]},{regs[rt]},{simm}"
    if opcode == 0x06:
        return f"blez {regs[rs]},{simm}"
    if opcode == 0x07:
        return f"bgtz {regs[rs]},{simm}"
    if opcode == 0x01:
        return f"regimm {regs[rs]},{simm}"
    return f"0x{word:08x}"


def count_removals_before(offset: int, sorted_offsets: list) -> int:
    """Count how many removal offsets are strictly before `offset`."""
    count = 0
    for r in sorted_offsets:
        if r < offset:
            count += 1
        else:
            break
    return count


def count_removals_in_range(lo: int, hi: int, sorted_offsets: list) -> int:
    """Count removals in range [lo, hi) (exclusive of hi)."""
    count = 0
    for r in sorted_offsets:
        if r >= hi:
            break
        if r >= lo:
            count += 1
    return count


def remove_and_compact(text: bytes, sorted_offsets: list) -> bytes:
    """Remove instructions at specified offsets and compact remaining code."""
    removed_set = set(sorted_offsets)
    parts = []
    for off in range(0, len(text), 4):
        if off not in removed_set:
            parts.append(text[off:off+4])
    return b''.join(parts)


def fix_branches(new_text: bytearray, sorted_offsets: list,
                 original_size: int, verbose: bool = False) -> int:
    """Adjust branch immediates after instruction removal.

    For each branch in the compacted code:
    1. Compute its original offset (before removals) via explicit mapping
    2. Compute the original target offset from the original immediate
    3. Compute the new target offset (after removals)
    4. Set new immediate = (new_target - (new_off + 4)) / 4

    When a branch targets a removed instruction, the next surviving
    instruction slides into that position, so the formula still works:
    new_target = orig_target - count_removals_before(orig_target) * 4
    """
    removed_set = set(sorted_offsets)

    # Build explicit new->original offset mapping
    new_to_orig = {}
    new_off = 0
    for orig_off in range(0, original_size, 4):
        if orig_off in removed_set:
            continue
        new_to_orig[new_off] = orig_off
        new_off += 4

    patched = 0
    for new_off in range(0, len(new_text), 4):
        word = struct.unpack("<I", new_text[new_off:new_off+4])[0]

        if not is_branch_insn(word):
            continue

        imm = get_branch_imm(word)
        orig_off = new_to_orig.get(new_off)
        if orig_off is None:
            continue

        # Original branch target
        orig_target = orig_off + 4 + imm * 4

        # Compute new target: subtract 4 bytes for each removal before target.
        # This works even if orig_target was a removed instruction: the next
        # surviving instruction slides into that exact position.
        new_target = orig_target - count_removals_before(orig_target, sorted_offsets) * 4

        if orig_target in removed_set and verbose:
            print(f"  NOTE: branch at 0x{new_off:x} (orig 0x{orig_off:x}) "
                  f"targeted removed instruction at 0x{orig_target:x}, "
                  f"now targets 0x{new_target:x}")

        new_imm = (new_target - (new_off + 4)) // 4

        if new_imm != imm:
            new_word = set_branch_imm(word, new_imm)
            struct.pack_into("<I", new_text, new_off, new_word)
            patched += 1

            if verbose:
                print(f"  branch at 0x{new_off:x} (orig 0x{orig_off:x}): "
                      f"imm {imm:+d} -> {new_imm:+d} "
                      f"(target orig 0x{orig_target:x} -> new 0x{new_target:x})")

    return patched


def patch_relocations(data: bytearray, rel_off: int, rel_size: int,
                      sorted_offsets: list, verbose: bool = False) -> int:
    """Adjust relocation offsets after instruction removal."""
    if rel_size % 8 != 0:
        raise ValueError(f".rel.text size not multiple of 8: {rel_size}")

    removed_set = set(sorted_offsets)
    n = rel_size // 8
    patched = 0

    for i in range(n):
        ent_off = rel_off + i * 8
        r_offset = struct.unpack("<I", data[ent_off : ent_off + 4])[0]

        if r_offset in removed_set:
            print(f"  WARNING: relocation at r_offset=0x{r_offset:x} targets "
                  f"a removed instruction", file=sys.stderr)
            continue

        # Count how many removed instructions are before this relocation
        n_before = count_removals_before(r_offset, sorted_offsets)
        if n_before > 0:
            new_offset = r_offset - n_before * 4
            struct.pack_into("<I", data, ent_off, new_offset)
            patched += 1

            if verbose:
                print(f"  reloc r_offset 0x{r_offset:x} -> 0x{new_offset:x} "
                      f"(-{n_before} instructions)")

    return patched


def main():
    ap = argparse.ArgumentParser(
        description="Remove instructions at specified .text offsets and compact"
    )
    ap.add_argument("input", type=Path, help="Input ELF .o")
    ap.add_argument("output", type=Path, help="Output ELF .o")
    ap.add_argument(
        "--offsets",
        required=True,
        nargs="+",
        type=lambda s: int(s, 0),
        help="List of .text offsets to remove (hex or decimal)",
    )
    ap.add_argument(
        "--shrink",
        action="store_true",
        help="Shrink .text instead of padding with trailing NOPs",
    )
    ap.add_argument("-v", "--verbose", action="store_true", help="Verbose output")
    args = ap.parse_args()

    sorted_offsets = sorted(args.offsets)
    n_remove = len(sorted_offsets)

    # Validate alignment
    for off in sorted_offsets:
        if off % 4 != 0:
            print(f"Error: offset 0x{off:x} is not 4-byte aligned", file=sys.stderr)
            return 1

    # Check for duplicates
    if len(set(sorted_offsets)) != len(sorted_offsets):
        print("Error: duplicate offsets specified", file=sys.stderr)
        return 1

    data = bytearray(args.input.read_bytes())

    text_sh_off, text_off, text_size = find_section(data, ".text")
    if text_off is None:
        print("Error: .text section not found", file=sys.stderr)
        return 1

    # Validate offsets are within .text
    for off in sorted_offsets:
        if off < 0 or off >= text_size:
            print(f"Error: offset 0x{off:x} out of range "
                  f"(text size 0x{text_size:x})", file=sys.stderr)
            return 1

    text = bytes(data[text_off : text_off + text_size])

    if args.verbose:
        print(f".text: file_off=0x{text_off:x} size=0x{text_size:x} "
              f"({text_size // 4} instructions)")
        print(f"Removing {n_remove} instruction(s) at offsets: "
              f"{', '.join(f'0x{o:x}' for o in sorted_offsets)}")
        for off in sorted_offsets:
            word = struct.unpack("<I", text[off:off+4])[0]
            # Check if this is in a delay slot
            if off >= 4:
                prev_word = struct.unpack("<I", text[off-4:off])[0]
                delay = is_branch_insn(prev_word) or is_jump_insn(prev_word)
            else:
                delay = False
            ds_mark = " [DELAY SLOT!]" if delay else ""
            print(f"  0x{off:x}: {word:08x}  {disasm_brief(word)}{ds_mark}")

    # Remove and compact
    new_text = remove_and_compact(text, sorted_offsets)
    assert len(new_text) == text_size - n_remove * 4

    # Fix branch immediates
    new_text_ba = bytearray(new_text)
    branch_patched = fix_branches(new_text_ba, sorted_offsets, text_size,
                                  verbose=args.verbose)
    if args.verbose:
        print(f"Patched {branch_patched} branch immediate(s)")

    if args.shrink:
        # Replace .text with compacted version (shrinks file)
        data = (bytearray(data[:text_off]) +
                new_text_ba +
                bytearray(data[text_off + text_size:]))

        growth = -(n_remove * 4)

        # Update ELF headers
        e_shoff_old = struct.unpack("<I", data[32:36])[0]
        # If we modified data before e_shoff, the old value is wrong
        # Recalculate: e_shoff was at byte 32-35 in original
        # Since we replaced text which is before section headers, shift shoff
        e_shoff = struct.unpack("<I", args.input.read_bytes()[32:36])[0]
        e_shentsize = struct.unpack("<H", data[46:48])[0]
        e_shnum = struct.unpack("<H", data[48:50])[0]

        if e_shoff > text_off:
            new_shoff = e_shoff + growth
            struct.pack_into("<I", data, 32, new_shoff)
        else:
            new_shoff = e_shoff

        for i in range(e_shnum):
            sh_off = new_shoff + (i * e_shentsize)
            sec_off = struct.unpack("<I", data[sh_off + 16 : sh_off + 20])[0]

            if sec_off == text_off:
                # Update .text size
                struct.pack_into("<I", data, sh_off + 20,
                                 text_size + growth)
            elif sec_off > text_off:
                # Shift sections after .text
                struct.pack_into("<I", data, sh_off + 16, sec_off + growth)

        if args.verbose:
            print(f".text shrunk from 0x{text_size:x} to "
                  f"0x{text_size + growth:x} bytes")
    else:
        # Pad with trailing NOPs
        padding = b'\x00\x00\x00\x00' * n_remove
        padded = new_text_ba + bytearray(padding)
        assert len(padded) == text_size
        data[text_off : text_off + text_size] = padded

        if args.verbose:
            print(f".text size maintained at 0x{text_size:x} bytes "
                  f"(padded with {n_remove} trailing NOP(s))")

    # Patch relocations
    rel_sh_off, rel_off, rel_size = find_section(data, ".rel.text")
    if rel_off is not None:
        rel_patched = patch_relocations(data, rel_off, rel_size,
                                        sorted_offsets, verbose=args.verbose)
        if args.verbose:
            print(f"Patched {rel_patched} relocation offset(s)")
    else:
        if args.verbose:
            print("Note: no .rel.text section found")

    args.output.write_bytes(data)
    if args.verbose:
        print(f"Wrote {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
