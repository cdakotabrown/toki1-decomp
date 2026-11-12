#!/usr/bin/env python3
"""
Fix SLL/SRA sign-extension pattern to use in-place registers in ELF object files.

IDO 5.2 sometimes generates sign extension using a temporary register:
    sll $t6, $a1, 16    # shift left through temp register t6
    sra $a1, $t6, 16    # shift right from t6 back to a1

But the target binary has in-place sign extension:
    sll $a1, $a1, 16    # shift left in-place
    sra $a1, $a1, 16    # shift right in-place

This tool finds such SLL/SRA pairs in a MIPS ELF .o file's .text section
and patches the register fields so the temporary register is replaced with
the source/destination register.

MIPS instruction encoding for SLL/SRA:
    SLL: 000000 | 00000 | rt(5) | rd(5) | sa(5) | 000000
    SRA: 000000 | 00000 | rt(5) | rd(5) | sa(5) | 000011
    rd = destination, rt = source, sa = shift amount

Pattern matched:
    SLL: rd=temp, rt=src, sa=N  (temp != src)
    SRA: rd=dst,  rt=temp, sa=N (SRA reads from SLL's temp output)

Patch applied:
    SLL: rd=src, rt=src, sa=N   (in-place)
    SRA: rd=src, rt=src, sa=N   (in-place)

Handles shift amounts of 16 (s16 sign extension) and 24 (s8 sign extension).

Common temp registers: $t0-$t9 ($8-$15, $24-$25), $at ($1), $v0-$v1 ($2-$3).
"""

import sys
import struct
import argparse

# Register names for verbose output
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


def is_sll(word, sa_values=(16, 24)):
    """Check if instruction is SLL with shift amount in sa_values.

    SLL encoding: op=0, rs=0, funct=0
    Returns (rt, rd, sa) or None.
    """
    op = (word >> 26) & 0x3f
    rs = (word >> 21) & 0x1f
    rt = (word >> 16) & 0x1f
    rd = (word >> 11) & 0x1f
    sa = (word >> 6) & 0x1f
    funct = word & 0x3f

    if op == 0 and rs == 0 and funct == 0 and sa in sa_values:
        return (rt, rd, sa)
    return None


def is_sra(word, sa_values=(16, 24)):
    """Check if instruction is SRA with shift amount in sa_values.

    SRA encoding: op=0, rs=0, funct=3
    Returns (rt, rd, sa) or None.
    """
    op = (word >> 26) & 0x3f
    rs = (word >> 21) & 0x1f
    rt = (word >> 16) & 0x1f
    rd = (word >> 11) & 0x1f
    sa = (word >> 6) & 0x1f
    funct = word & 0x3f

    if op == 0 and rs == 0 and funct == 3 and sa in sa_values:
        return (rt, rd, sa)
    return None


def encode_sll(rt, rd, sa):
    """Encode SLL instruction: sll $rd, $rt, sa."""
    return (0 << 26) | (0 << 21) | (rt << 16) | (rd << 11) | (sa << 6) | 0


def encode_sra(rt, rd, sa):
    """Encode SRA instruction: sra $rd, $rt, sa."""
    return (0 << 26) | (0 << 21) | (rt << 16) | (rd << 11) | (sa << 6) | 3


def patch_sll_sra_pattern(elf_data, verbose=False):
    """Patch SLL/SRA sign-extension patterns to use in-place registers.

    Scans the .text section for SLL/SRA pairs where:
    - SLL writes to a temp register: sll $temp, $src, N
    - SRA reads from that temp: sra $dst, $temp, N
    - The temp register differs from the source register
    - The shift amounts match between SLL and SRA

    Patches both instructions to use the source register in-place:
    - sll $src, $src, N
    - sra $src, $src, N

    Returns (patched_data, count_of_patterns_fixed).
    """
    patched = bytearray(elf_data)

    # Check ELF magic
    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

    if verbose:
        print(f"ELF endianness: {'big' if is_big_endian else 'little'}")

    # Parse ELF header to find .text section
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

    # Collect all SLL and SRA instructions with sign-extension shift amounts
    sll_list = []
    sra_list = []

    for i in range(0, text_size, 4):
        file_off = text_offset + i
        if file_off + 4 > len(elf_data):
            break

        word = struct.unpack(endian + 'I', elf_data[file_off:file_off+4])[0]

        sll_info = is_sll(word)
        if sll_info is not None:
            rt, rd, sa = sll_info
            # Skip NOP (sll $zero, $zero, 0)
            if rd != 0 or rt != 0:
                sll_list.append((i, file_off, rt, rd, sa))

        sra_info = is_sra(word)
        if sra_info is not None:
            rt, rd, sa = sra_info
            sra_list.append((i, file_off, rt, rd, sa))

    # Match SLL/SRA pairs
    changes = 0
    matched_sll = set()
    matched_sra = set()

    for sll_text_off, sll_file_off, sll_rt, sll_rd, sll_sa in sll_list:
        if sll_text_off in matched_sll:
            continue

        # SLL: rd=temp, rt=src -- temp must differ from src
        if sll_rd == sll_rt:
            continue

        src_reg = sll_rt
        temp_reg = sll_rd

        # Find matching SRA that reads from the temp register
        for sra_text_off, sra_file_off, sra_rt, sra_rd, sra_sa in sra_list:
            if sra_text_off in matched_sra:
                continue
            # SRA must come after SLL
            if sra_text_off <= sll_text_off:
                continue
            # SRA rt (input) must be the temp register from SLL
            if sra_rt != temp_reg:
                continue
            # Shift amounts must match
            if sll_sa != sra_sa:
                continue

            # Found a match -- patch both to use src_reg in-place
            sa = sll_sa
            new_sll = encode_sll(src_reg, src_reg, sa)
            new_sra = encode_sra(src_reg, src_reg, sa)

            struct.pack_into(endian + 'I', patched, sll_file_off, new_sll)
            struct.pack_into(endian + 'I', patched, sra_file_off, new_sra)

            matched_sll.add(sll_text_off)
            matched_sra.add(sra_text_off)
            changes += 1

            if verbose:
                src_name = REG_NAMES.get(src_reg, str(src_reg))
                temp_name = REG_NAMES.get(temp_reg, str(temp_reg))
                dst_name = REG_NAMES.get(sra_rd, str(sra_rd))
                print(f"  0x{sll_text_off:04x}: sll ${temp_name},${src_name},{sa}"
                      f" -> sll ${src_name},${src_name},{sa}")
                print(f"  0x{sra_text_off:04x}: sra ${dst_name},${temp_name},{sa}"
                      f" -> sra ${src_name},${src_name},{sa}")

            # Only match once per SLL
            break

    return bytes(patched), changes


def main():
    parser = argparse.ArgumentParser(
        description='Fix SLL/SRA sign-extension pattern to use in-place registers'
    )
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('-v', '--verbose', action='store_true', help='Print changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, count = patch_sll_sra_pattern(elf_data, args.verbose)

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Fixed {count} SLL/SRA sign-extension pattern(s)")


if __name__ == '__main__':
    main()
