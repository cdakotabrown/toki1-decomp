#!/usr/bin/env python3
"""
Fix sign extension patterns to produce compact in-place code.

Converts patterns like:
  sll $temp, $src, 16
  sra $dst, $temp, 16
  move $final, $dst    (if present)

To:
  sll $final, $src, 16
  sra $final, $final, 16

And removes the now-unnecessary move instruction.

This handles the case where IDO uses temporary registers for sign extension
but the target binary does it in-place.
"""

import sys
import struct
import argparse

REG_NAMES = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
             't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
             's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
             't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra']

def decode_r_type(word):
    """Decode R-type instruction."""
    op = (word >> 26) & 0x3f
    rs = (word >> 21) & 0x1f
    rt = (word >> 16) & 0x1f
    rd = (word >> 11) & 0x1f
    sa = (word >> 6) & 0x1f
    funct = word & 0x3f
    return op, rs, rt, rd, sa, funct

def encode_r_type(op, rs, rt, rd, sa, funct):
    """Encode R-type instruction."""
    return (op << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (sa << 6) | funct

def is_sll_16(word):
    """Check if instruction is sll $rd, $rt, 16."""
    op, rs, rt, rd, sa, funct = decode_r_type(word)
    return op == 0 and rs == 0 and funct == 0 and sa == 16

def is_sra_16(word):
    """Check if instruction is sra $rd, $rt, 16."""
    op, rs, rt, rd, sa, funct = decode_r_type(word)
    return op == 0 and rs == 0 and funct == 3 and sa == 16

def is_move(word):
    """Check if instruction is move (or $rd, $rs, $zero)."""
    op, rs, rt, rd, sa, funct = decode_r_type(word)
    # move is: or $rd, $rs, $zero (funct=0x25) or addu $rd, $rs, $zero (funct=0x21)
    if op != 0:
        return False, 0, 0
    if funct == 0x25 and rt == 0 and sa == 0:  # or
        return True, rs, rd
    if funct == 0x21 and rt == 0 and sa == 0:  # addu
        return True, rs, rd
    return False, 0, 0

def patch_sign_extend_patterns(elf_data, verbose=False):
    """Patch sign extension patterns to be more compact."""
    patched = bytearray(elf_data)

    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

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

    text_offset = None
    text_size = None
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
            break

    if text_offset is None:
        raise ValueError(".text section not found")

    if verbose:
        print(f".text at offset 0x{text_offset:x}, size 0x{text_size:x}")

    # Read all instructions
    instrs = []
    for i in range(0, text_size, 4):
        file_off = text_offset + i
        if file_off + 4 > len(elf_data):
            break
        word = struct.unpack(endian + 'I', elf_data[file_off:file_off+4])[0]
        instrs.append((i, file_off, word))

    changes = 0
    nops_to_insert = []  # List of (instruction_index, offset_in_text)

    # Pattern 1: sll $temp, $src, 16; sra $dst, $temp, 16; move $final, $dst
    # Convert to: sll $final, $src, 16; sra $final, $final, 16; nop
    #
    # Pattern 2: sll $temp, $src, 16; sra $src, $temp, 16 (no move, src==dst)
    # Convert to: sll $src, $src, 16; sra $src, $src, 16

    i = 0
    while i < len(instrs) - 1:
        off1, foff1, word1 = instrs[i]

        if is_sll_16(word1):
            _, _, sll_rt, sll_rd, _, _ = decode_r_type(word1)
            sll_src = sll_rt
            sll_temp = sll_rd

            # Look for matching sra within next few instructions
            for j in range(i+1, min(i+8, len(instrs))):
                off_j, foff_j, word_j = instrs[j]

                if is_sra_16(word_j):
                    _, _, sra_rt, sra_rd, _, _ = decode_r_type(word_j)

                    # sra input should be sll output (the temp register)
                    if sra_rt != sll_temp:
                        continue

                    sra_dst = sra_rd

                    # Check if there's a move that uses sra output
                    found_move = False
                    for k in range(j+1, min(j+6, len(instrs))):
                        off_k, foff_k, word_k = instrs[k]
                        is_mv, mv_rs, mv_rd = is_move(word_k)

                        if is_mv and mv_rs == sra_dst:
                            # Found pattern with move
                            final_dst = mv_rd

                            if verbose:
                                print(f"  Pattern with move at 0x{off1:04x}-0x{off_k:04x}:")
                                print(f"    sll ${REG_NAMES[sll_temp]},${REG_NAMES[sll_src]},16")
                                print(f"    sra ${REG_NAMES[sra_dst]},${REG_NAMES[sra_rt]},16")
                                print(f"    move ${REG_NAMES[mv_rd]},${REG_NAMES[mv_rs]}")
                                print(f"  -> sll ${REG_NAMES[final_dst]},${REG_NAMES[sll_src]},16")
                                print(f"     sra ${REG_NAMES[final_dst]},${REG_NAMES[final_dst]},16")
                                print(f"     nop")

                            # New sll: sll $final_dst, $src, 16
                            new_sll = encode_r_type(0, 0, sll_src, final_dst, 16, 0)
                            # New sra: sra $final_dst, $final_dst, 16
                            new_sra = encode_r_type(0, 0, final_dst, final_dst, 16, 3)

                            struct.pack_into(endian + 'I', patched, foff1, new_sll)
                            struct.pack_into(endian + 'I', patched, foff_j, new_sra)
                            struct.pack_into(endian + 'I', patched, foff_k, 0)  # nop

                            changes += 1
                            found_move = True
                            break

                    if not found_move and sra_dst == sll_src:
                        # Pattern 2: sll temp, src, 16; sra src, temp, 16
                        # Just convert to in-place: sll src, src, 16; sra src, src, 16
                        if sll_temp != sll_src:  # Only if temp differs from src
                            if verbose:
                                print(f"  Pattern in-place at 0x{off1:04x}-0x{off_j:04x}:")
                                print(f"    sll ${REG_NAMES[sll_temp]},${REG_NAMES[sll_src]},16")
                                print(f"    sra ${REG_NAMES[sra_dst]},${REG_NAMES[sra_rt]},16")
                                print(f"  -> sll ${REG_NAMES[sll_src]},${REG_NAMES[sll_src]},16")
                                print(f"     sra ${REG_NAMES[sll_src]},${REG_NAMES[sll_src]},16")

                            # New sll: sll $src, $src, 16
                            new_sll = encode_r_type(0, 0, sll_src, sll_src, 16, 0)
                            # New sra: sra $src, $src, 16
                            new_sra = encode_r_type(0, 0, sll_src, sll_src, 16, 3)

                            struct.pack_into(endian + 'I', patched, foff1, new_sll)
                            struct.pack_into(endian + 'I', patched, foff_j, new_sra)

                            changes += 1
                    break
        i += 1

    return bytes(patched), changes

def main():
    parser = argparse.ArgumentParser(
        description='Fix sign extension patterns to use in-place registers'
    )
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('-v', '--verbose', action='store_true', help='Print changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, count = patch_sign_extend_patterns(elf_data, args.verbose)

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Fixed {count} sign extension patterns")

if __name__ == '__main__':
    main()
