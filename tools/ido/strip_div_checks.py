#!/usr/bin/env python3
"""
Strip division overflow/zero checks from .text section.

IDO's as1 assembler expands signed div instructions with safety checks:
  div $zero, $rs, $rt
  bnez $rt, .Lok1         <- REMOVE (div-by-zero check)
  nop                     <- REMOVE
  break 0x7               <- REMOVE
  .Lok1:
  li $at, -1              <- REMOVE (overflow check: INT_MIN / -1)
  bne $rt, $at, .Lok2     <- REMOVE
  lui $at, 0x8000         <- REMOVE
  bne $rs, $at, .Lok2     <- REMOVE
  nop                     <- REMOVE
  break 0x6               <- REMOVE

This tool removes the 9-instruction check sequence after each div,
leaving only the bare div instruction. It also removes preparatory
move instructions that copy the dividend for the overflow check,
and lone li/nop instructions before the div that are only used for
the checks.

Usage:
  python3 strip_div_checks.py input.o output.o [-v]

Post-processing pipeline step for functions with bare div in target.
"""

import sys
import struct
import argparse

NOP = 0x00000000
BREAK_7 = 0x0007000d  # break 0x7 (divide by zero)
BREAK_6 = 0x0006000d  # break 0x6 (overflow)


def decode_op(word):
    """Decode MIPS instruction fields."""
    op = (word >> 26) & 0x3f
    rs = (word >> 21) & 0x1f
    rt = (word >> 16) & 0x1f
    rd = (word >> 11) & 0x1f
    funct = word & 0x3f
    imm = word & 0xffff
    imm_signed = imm if imm < 0x8000 else imm - 0x10000
    return op, rs, rt, rd, funct, imm, imm_signed


def is_div(word):
    """Check if instruction is div $zero, $rs, $rt."""
    op, rs, rt, rd, funct, _, _ = decode_op(word)
    return op == 0 and rd == 0 and funct == 0x1a  # div


def is_divu(word):
    """Check if instruction is divu $zero, $rs, $rt."""
    op, rs, rt, rd, funct, _, _ = decode_op(word)
    return op == 0 and rd == 0 and funct == 0x1b  # divu


def is_bnez(word):
    """Check if instruction is bnez $rt, offset (bne $rt, $zero, offset)."""
    op, rs, rt, _, _, _, _ = decode_op(word)
    return op == 5 and rs != 0 and rt == 0  # bne with one operand = $zero


def is_bne(word):
    """Check if instruction is bne $rs, $rt, offset."""
    op = (word >> 26) & 0x3f
    return op == 5


def is_li_at_minus1(word):
    """Check if instruction is li $at, -1 (addiu $at, $zero, -1)."""
    return word == 0x2401ffff


def is_lui_at_0x8000(word):
    """Check if instruction is lui $at, 0x8000."""
    return word == 0x3c018000


def is_move(word):
    """Check if instruction is move $rd, $rs (or $rd, $rs, $zero or addu $rd, $rs, $zero)."""
    op, rs, rt, rd, funct, _, _ = decode_op(word)
    if op == 0 and rt == 0 and (funct == 0x25 or funct == 0x21):  # or / addu
        return True, rd, rs
    return False, 0, 0


def is_branch_or_jump(word):
    """Check if instruction is a branch or jump (has delay slot)."""
    op = (word >> 26) & 0x3f
    if op in (2, 3):  # j, jal
        return True
    if op in (4, 5, 6, 7):  # beq, bne, blez, bgtz
        return True
    if op == 1:  # REGIMM: bltz, bgez, etc.
        return True
    if op == 0:
        funct = word & 0x3f
        if funct in (8, 9):  # jr, jalr
            return True
    return False


def find_div_check_sequences(instructions, verbose=False):
    """Find all div check sequences and return indices to remove.

    IDO's as1 generates this check pattern for signed div:
      bnez $divisor_reg, +2
      nop
      break 0x7
      li $at, -1
      bne $divisor_reg, $at, +4
      lui $at, 0x8000
      bne $dividend_reg, $at, +2
      nop
      break 0x6

    For unsigned divu, only the first 3 instructions (bnez/nop/break 0x7).

    IMPORTANT: as1's scheduler may insert other instructions between
    the div and the check sequence. We search within a window of up
    to 10 instructions after the div for the start of the check.
    """
    to_remove = set()
    SEARCH_WINDOW = 10

    for i, (off, word) in enumerate(instructions):
        if not (is_div(word) or is_divu(word)):
            continue

        _, div_rs, div_rt, _, funct, _, _ = decode_op(word)
        is_unsigned = (funct == 0x1b)

        # Search within a window for the start of the check sequence
        found = False
        for start in range(i + 1, min(i + 1 + SEARCH_WINDOW, len(instructions))):
            # For signed div: look for full 9-instruction pattern
            # For unsigned divu: look for 3-instruction pattern (bnez/nop/break 0x7)
            if is_unsigned:
                # divu: just bnez + nop + break 0x7
                if start + 2 >= len(instructions):
                    continue
                w0 = instructions[start][1]
                w1 = instructions[start + 1][1]
                w2 = instructions[start + 2][1]

                if is_bnez(w0) and w1 == NOP and w2 == BREAK_7:
                    if verbose:
                        print(f"  Found divu check at index {start} (div at {i})")
                    for j in range(3):
                        to_remove.add(start + j)
                    found = True
                    break
            else:
                # signed div: full 9-instruction pattern
                if start + 8 >= len(instructions):
                    continue
                check = [instructions[start + j][1] for j in range(9)]

                if (is_bnez(check[0]) and
                    check[1] == NOP and
                    check[2] == BREAK_7 and
                    check[3] == 0x2401ffff and  # li $at, -1
                    is_bne(check[4]) and
                    check[5] == 0x3c018000 and  # lui $at, 0x8000
                    is_bne(check[6]) and
                    check[7] == NOP and
                    check[8] == BREAK_6):

                    if verbose:
                        print(f"  Found signed div check at index {start} (div at {i})")
                    for j in range(9):
                        to_remove.add(start + j)
                    found = True
                    break

        if not found and verbose:
            print(f"  WARNING: div at index {i} (0x{off:x}) has no matching check")

    return to_remove


def find_dead_moves(instructions, to_remove):
    """Find move instructions that only exist for the overflow check.

    When as1 expands div with overflow checks, it may use a saved copy
    of the dividend. If the move's destination is only used in the
    check sequence (which we're removing), the move is dead.
    """
    dead_moves = set()

    for i, (off, word) in enumerate(instructions):
        is_mv, rd, rs = is_move(word)
        if not is_mv:
            continue

        # Check if this move's destination is only used in removed instructions
        # and in the div's rs field
        used_in_kept = False
        for j, (off2, word2) in enumerate(instructions):
            if j == i:
                continue
            if j in to_remove:
                continue

            # Check if this instruction uses the register
            op2, rs2, rt2, rd2, _, _, _ = decode_op(word2)
            if rs2 == rd or rt2 == rd:
                # Used in a kept instruction
                used_in_kept = True
                break

        if not used_in_kept:
            dead_moves.add(i)

    return dead_moves


def strip_div_checks(elf_data, verbose=False, strip_moves=True):
    """Strip division check sequences from .text section."""
    patched = bytearray(elf_data)

    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

    if verbose:
        print(f"ELF endianness: {'big' if is_big_endian else 'little'}")

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

    # Find sections
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
        print(f".text at offset 0x{text_offset:x}, size 0x{text_size:x}")

    # Read instructions
    instructions = []
    for i in range(0, text_size, 4):
        file_off = text_offset + i
        word = struct.unpack(endian + 'I', elf_data[file_off:file_off+4])[0]
        instructions.append((i, word))

    # Find div check sequences to remove
    to_remove = find_div_check_sequences(instructions, verbose=verbose)

    if verbose:
        print(f"Found {len(to_remove)} instructions in div check sequences")

    # Optionally find dead moves
    if strip_moves:
        dead_moves = find_dead_moves(instructions, to_remove)
        if verbose and dead_moves:
            print(f"Found {len(dead_moves)} dead move instructions")
        to_remove |= dead_moves

    if not to_remove:
        return bytes(patched), 0

    # Build removed offsets (sorted) for relocation adjustment
    removed_offsets = sorted([instructions[i][0] for i in to_remove])

    if verbose:
        for idx in sorted(to_remove):
            off, word = instructions[idx]
            print(f"  Removing 0x{off:04x}: {word:08x}")

    # Build new text section without removed instructions
    new_text = bytearray()
    offset_map = {}  # old_offset -> new_offset

    current_new_off = 0
    for idx, (old_off, word) in enumerate(instructions):
        if idx in to_remove:
            continue
        offset_map[old_off] = current_new_off
        new_text.extend(struct.pack(endian + 'I', word))
        current_new_off += 4

    removed_count = len(to_remove)
    removed_bytes = removed_count * 4
    new_text_size = len(new_text)

    if verbose:
        print(f"New .text size: 0x{new_text_size:x} (removed {removed_bytes} bytes)")

    # Replace text section
    padded_text = new_text + b'\x00' * (text_size - new_text_size)
    patched[text_offset:text_offset+text_size] = padded_text

    # Update .text section size
    struct.pack_into(endian + 'I', patched, text_section_header_off + 20, new_text_size)

    # Adjust relocations
    if rel_text_offset and rel_text_size:
        num_relocs = rel_text_size // 8
        new_relocs = bytearray()
        for i in range(num_relocs):
            rel_off = rel_text_offset + i * 8
            r_offset = struct.unpack(endian + 'I', patched[rel_off:rel_off+4])[0]
            r_info = struct.unpack(endian + 'I', patched[rel_off+4:rel_off+8])[0]

            # Skip relocations pointing to removed instructions
            if r_offset in removed_offsets:
                if verbose:
                    print(f"  Dropping reloc at 0x{r_offset:x} (removed instruction)")
                continue

            # Adjust offset
            adjustment = sum(4 for ro in removed_offsets if ro < r_offset)
            new_r_offset = r_offset - adjustment

            if verbose and adjustment > 0:
                print(f"  Reloc at 0x{r_offset:x} -> 0x{new_r_offset:x}")

            new_relocs.extend(struct.pack(endian + 'I', new_r_offset))
            new_relocs.extend(struct.pack(endian + 'I', r_info))

        # Write adjusted relocations
        new_rel_size = len(new_relocs)
        padded_relocs = new_relocs + b'\x00' * (rel_text_size - new_rel_size)
        patched[rel_text_offset:rel_text_offset+rel_text_size] = padded_relocs

        # Update .rel.text section size
        struct.pack_into(endian + 'I', patched, rel_text_header_off + 20, new_rel_size)

    # Adjust symbols
    if symtab_offset and symtab_size:
        num_syms = symtab_size // 16
        for i in range(num_syms):
            sym_off = symtab_offset + i * 16
            st_value = struct.unpack(endian + 'I', patched[sym_off+4:sym_off+8])[0]
            st_size = struct.unpack(endian + 'I', patched[sym_off+8:sym_off+12])[0]
            st_shndx = struct.unpack(endian + 'H', patched[sym_off+14:sym_off+16])[0]

            if st_shndx != text_section_idx:
                continue

            # Adjust value
            adjustment = sum(4 for ro in removed_offsets if ro < st_value)
            new_value = st_value - adjustment

            # Adjust size
            removed_within = sum(4 for ro in removed_offsets
                                if st_value <= ro < st_value + st_size)
            new_size = st_size - removed_within

            struct.pack_into(endian + 'I', patched, sym_off+4, new_value)
            struct.pack_into(endian + 'I', patched, sym_off+8, new_size)

    return bytes(patched), removed_count


def main():
    parser = argparse.ArgumentParser(
        description='Strip division overflow/zero checks from .text section'
    )
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('-v', '--verbose', action='store_true', help='Print changes')
    parser.add_argument('--no-strip-moves', action='store_true',
                        help='Do not remove dead move instructions')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, count = strip_div_checks(elf_data, args.verbose,
                                       strip_moves=not args.no_strip_moves)

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Stripped {count} instructions from div check sequences ({count * 4} bytes)")


if __name__ == '__main__':
    main()
