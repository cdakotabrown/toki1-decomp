#!/usr/bin/env python3
"""
Fix the SLL/SRA sign extension pattern where IDO uses a temp register
and a subsequent MOVE, converting to in-place on the destination register.

Handles two patterns:

Pattern 1 (same register, handled by fix_sll_sra_pattern.py too):
  sll $temp, $src, 16     ->   sll $src, $src, 16
  sra $dst, $temp, 16     ->   sra $src, $src, 16
  (where dst == src, temp is different)

Pattern 2 (cross-register with move):
  sll $dst, $src, 16      ->   sll $dst, $src, 16  (keep)
  sra $temp, $dst, 16     ->   sra $dst, $dst, 16  (fix: temp -> dst)
  ...
  move $dst, $temp         ->   (removed, instructions shifted)

This script handles Pattern 2 by:
1. Finding sll+sra pairs where sra uses a temp
2. Finding the corresponding move that copies temp back to dst
3. Patching sra to write directly to dst
4. Removing the move instruction (shifting subsequent instructions)
5. Also applies Pattern 1 (same as fix_sll_sra_pattern.py)
"""

import sys
import struct
import argparse


# MIPS register numbers
REG_NAMES = {
    0: 'zero', 1: 'at', 2: 'v0', 3: 'v1',
    4: 'a0', 5: 'a1', 6: 'a2', 7: 'a3',
    8: 't0', 9: 't1', 10: 't2', 11: 't3',
    12: 't4', 13: 't5', 14: 't6', 15: 't7',
    16: 's0', 17: 's1', 18: 's2', 19: 's3',
    20: 's4', 21: 's5', 22: 's6', 23: 's7',
    24: 't8', 25: 't9', 26: 'k0', 27: 'k1',
    28: 'gp', 29: 'sp', 30: 'fp', 31: 'ra',
}


def decode_r_type(word):
    """Decode R-type MIPS instruction."""
    op = (word >> 26) & 0x3f
    rs = (word >> 21) & 0x1f
    rt = (word >> 16) & 0x1f
    rd = (word >> 11) & 0x1f
    sa = (word >> 6) & 0x1f
    funct = word & 0x3f
    return op, rs, rt, rd, sa, funct


def encode_r_type(op, rs, rt, rd, sa, funct):
    """Encode R-type MIPS instruction."""
    return ((op & 0x3f) << 26) | ((rs & 0x1f) << 21) | \
           ((rt & 0x1f) << 16) | ((rd & 0x1f) << 11) | \
           ((sa & 0x1f) << 6) | (funct & 0x3f)


def is_sll(word, sa_val=None):
    """Check if instruction is SLL with optional sa value filter."""
    op, rs, rt, rd, sa, funct = decode_r_type(word)
    if op != 0 or rs != 0 or funct != 0:
        return False
    if sa_val is not None and sa not in (sa_val if isinstance(sa_val, (list, tuple)) else [sa_val]):
        return False
    return True


def is_sra(word, sa_val=None):
    """Check if instruction is SRA with optional sa value filter."""
    op, rs, rt, rd, sa, funct = decode_r_type(word)
    if op != 0 or rs != 0 or funct != 3:
        return False
    if sa_val is not None and sa not in (sa_val if isinstance(sa_val, (list, tuple)) else [sa_val]):
        return False
    return True


def is_move(word):
    """Check if instruction is MOVE (addu rd, rs, zero or or rd, rs, zero)."""
    op, rs, rt, rd, sa, funct = decode_r_type(word)
    # addu: op=0, funct=0x21; or: op=0, funct=0x25
    if op == 0 and sa == 0 and funct in (0x21, 0x25):
        if rt == 0:
            # move rd, rs (rs=source, rt=zero)
            return True, rs, rd
        if rs == 0:
            # move rd, rt (rt=source, rs=zero) - less common
            return True, rt, rd
    return False, 0, 0


def patch_text(data, text_offset, text_size, endian, verbose=False):
    """Apply both Pattern 1 and Pattern 2 fixes."""
    patched = bytearray(data)
    changes_p1 = 0
    changes_p2 = 0
    remove_offsets = []  # text-relative offsets of instructions to remove

    # Read all instructions
    instructions = []
    for i in range(0, text_size, 4):
        file_off = text_offset + i
        if file_off + 4 > len(data):
            break
        word = struct.unpack(endian + 'I', data[file_off:file_off+4])[0]
        instructions.append((i, file_off, word))

    # Track which SLL instructions have been matched
    matched_sll = set()

    # Find SLL instructions with sa=16 or 24
    for idx, (sll_toff, sll_foff, sll_word) in enumerate(instructions):
        if idx in matched_sll:
            continue
        if not is_sll(sll_word, [16, 24]):
            continue
        _, _, sll_rt, sll_rd, sll_sa, _ = decode_r_type(sll_word)

        # Skip NOP (sll zero, zero, 0)
        if sll_rd == 0 and sll_rt == 0:
            continue

        # Find matching SRA
        for idx2, (sra_toff, sra_foff, sra_word) in enumerate(instructions):
            if idx2 <= idx:
                continue
            if not is_sra(sra_word, sll_sa):
                continue
            _, _, sra_rt, sra_rd, sra_sa, _ = decode_r_type(sra_word)

            # SRA must read from SLL's destination
            if sra_rt != sll_rd:
                continue

            # Pattern 1: sll temp, src, N; sra src, temp, N (temp != src)
            if sll_rd != sll_rt and sra_rd == sll_rt:
                # Convert to in-place: sll src, src, N; sra src, src, N
                src = sll_rt
                new_sll = encode_r_type(0, 0, src, src, sll_sa, 0)
                new_sra = encode_r_type(0, 0, src, src, sra_sa, 3)
                struct.pack_into(endian + 'I', patched, sll_foff, new_sll)
                struct.pack_into(endian + 'I', patched, sra_foff, new_sra)
                changes_p1 += 1
                matched_sll.add(idx)
                if verbose:
                    print(f"  P1 0x{sll_toff:04x}: sll ${REG_NAMES.get(sll_rd,'?')},${REG_NAMES.get(sll_rt,'?')},{sll_sa} -> sll ${REG_NAMES.get(src,'?')},${REG_NAMES.get(src,'?')},{sll_sa}")
                    print(f"  P1 0x{sra_toff:04x}: sra ${REG_NAMES.get(sra_rd,'?')},${REG_NAMES.get(sra_rt,'?')},{sra_sa} -> sra ${REG_NAMES.get(src,'?')},${REG_NAMES.get(src,'?')},{sra_sa}")
                break

            # Pattern 2: sll dst, src, N; sra temp, dst, N; ...; move dst, temp
            if sll_rd != sll_rt and sra_rd != sll_rd:
                temp = sra_rd
                dst = sll_rd
                # Look for move dst, temp after the sra
                for idx3, (mov_toff, mov_foff, mov_word) in enumerate(instructions):
                    if idx3 <= idx2:
                        continue
                    is_mov, mov_src, mov_dst = is_move(mov_word)
                    if is_mov and mov_src == temp and mov_dst == dst:
                        # Found the full pattern!
                        # Fix sra: change destination from temp to dst
                        new_sra = encode_r_type(0, 0, dst, dst, sra_sa, 3)
                        struct.pack_into(endian + 'I', patched, sra_foff, new_sra)
                        # Mark move for removal
                        remove_offsets.append(mov_toff)
                        changes_p2 += 1
                        matched_sll.add(idx)
                        if verbose:
                            print(f"  P2 0x{sra_toff:04x}: sra ${REG_NAMES.get(temp,'?')},${REG_NAMES.get(sra_rt,'?')},{sra_sa} -> sra ${REG_NAMES.get(dst,'?')},${REG_NAMES.get(dst,'?')},{sra_sa}")
                            print(f"  P2 0x{mov_toff:04x}: move ${REG_NAMES.get(dst,'?')},${REG_NAMES.get(temp,'?')} -> REMOVED")
                        break
                break

    return patched, changes_p1, changes_p2, remove_offsets


def remove_instructions(data, text_offset, text_size, remove_offsets, endian, verbose=False):
    """Remove instructions at specified text-relative offsets by shifting."""
    if not remove_offsets:
        return data, text_size

    patched = bytearray(data)
    remove_offsets_sorted = sorted(remove_offsets)

    # For each removal, shift subsequent text bytes up
    total_removed = 0
    for rem_off in remove_offsets_sorted:
        actual_off = rem_off - total_removed
        file_off = text_offset + actual_off
        # Shift everything after this instruction up by 4 bytes
        end_of_text = text_offset + text_size - total_removed
        patched[file_off:end_of_text-4] = patched[file_off+4:end_of_text]
        # Fill the gap at the end with NOP
        nop = struct.pack(endian + 'I', 0)
        patched[end_of_text-4:end_of_text] = nop
        total_removed += 4

    new_text_size = text_size - total_removed

    # Update .text section size in ELF header
    e_shoff = struct.unpack(endian + 'I', patched[32:36])[0]
    e_shentsize = struct.unpack(endian + 'H', patched[46:48])[0]
    e_shnum = struct.unpack(endian + 'H', patched[48:50])[0]
    e_shstrndx = struct.unpack(endian + 'H', patched[50:52])[0]

    shstrtab_off = e_shoff + e_shstrndx * e_shentsize
    shstrtab_addr = struct.unpack(endian + 'I', patched[shstrtab_off+16:shstrtab_off+20])[0]
    shstrtab_size = struct.unpack(endian + 'I', patched[shstrtab_off+20:shstrtab_off+24])[0]
    shstrtab = patched[shstrtab_addr:shstrtab_addr+shstrtab_size]

    for i in range(e_shnum):
        sh_offset = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack(endian + 'I', patched[sh_offset:sh_offset+4])[0]
        name_end = shstrtab.find(b'\x00', sh_name_idx)
        name = shstrtab[sh_name_idx:name_end].decode('ascii')
        if name == '.text':
            struct.pack_into(endian + 'I', patched, sh_offset+20, new_text_size)
            if verbose:
                print(f"  Updated .text size: 0x{text_size:x} -> 0x{new_text_size:x}")
            break

    return bytes(patched), new_text_size


def main():
    parser = argparse.ArgumentParser(
        description='Fix SLL/SRA sign extension patterns (both in-place and cross-register)'
    )
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('-v', '--verbose', action='store_true', help='Print changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    if elf_data[:4] != b'\x7fELF':
        print("Error: Not an ELF file", file=sys.stderr)
        sys.exit(1)

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

    if args.verbose:
        print(f"ELF endianness: {'big' if is_big_endian else 'little'}")

    # Parse ELF to find .text
    e_shoff = struct.unpack(endian + 'I', elf_data[32:36])[0]
    e_shentsize = struct.unpack(endian + 'H', elf_data[46:48])[0]
    e_shnum = struct.unpack(endian + 'H', elf_data[48:50])[0]
    e_shstrndx = struct.unpack(endian + 'H', elf_data[50:52])[0]

    shstrtab_off = e_shoff + e_shstrndx * e_shentsize
    shstrtab_addr = struct.unpack(endian + 'I', elf_data[shstrtab_off+16:shstrtab_off+20])[0]
    shstrtab_size = struct.unpack(endian + 'I', elf_data[shstrtab_off+20:shstrtab_off+24])[0]
    shstrtab = elf_data[shstrtab_addr:shstrtab_addr+shstrtab_size]

    text_offset = None
    text_size = None
    for i in range(e_shnum):
        sh_offset = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack(endian + 'I', elf_data[sh_offset:sh_offset+4])[0]
        name_end = shstrtab.find(b'\x00', sh_name_idx)
        name = shstrtab[sh_name_idx:name_end].decode('ascii')
        if name == '.text':
            text_offset = struct.unpack(endian + 'I', elf_data[sh_offset+16:sh_offset+20])[0]
            text_size = struct.unpack(endian + 'I', elf_data[sh_offset+20:sh_offset+24])[0]
            break

    if text_offset is None:
        print("Error: .text section not found", file=sys.stderr)
        sys.exit(1)

    if args.verbose:
        print(f".text at offset 0x{text_offset:x}, size 0x{text_size:x}")

    patched, p1_count, p2_count, remove_offsets = patch_text(
        elf_data, text_offset, text_size, endian, args.verbose
    )

    if remove_offsets:
        patched, new_size = remove_instructions(
            patched, text_offset, text_size, remove_offsets, endian, args.verbose
        )
    else:
        new_size = text_size

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Fixed {p1_count} Pattern 1 (in-place) + {p2_count} Pattern 2 (cross-reg) = {p1_count + p2_count} total")
    if remove_offsets:
        print(f"Removed {len(remove_offsets)} instruction(s)")


if __name__ == '__main__':
    main()
