#!/usr/bin/env python3
"""
Fix the SLL/SRA sign extension pattern to use in-place register (v2).

Enhanced version that handles two patterns:

Pattern A (original):
  sll $temp, $src, N     ->    sll $src, $src, N
  sra $src, $temp, N     ->    sra $src, $src, N

  Here the SRA destination is the same as the SLL source. The temp is the
  SLL destination. Fix: make both use $src.

Pattern B (with move elimination):
  sll $dst, $src, N      ->    sll $dst, $src, N   (unchanged)
  sra $temp, $dst, N     ->    sra $dst, $dst, N   (temp -> dst)
  move $dst, $temp        ->    nop                 (eliminated)

  Here the SLL destination ($dst) is the real target, but the SRA uses a
  temp register and requires a subsequent move. Fix: make SRA write directly
  to $dst and NOP out the move.

Also handles shift-by-24 for s8 sign extension.
"""

import sys
import struct
import argparse


def is_move_instruction(word):
    """Check if instruction is a move (or $rd, $rs, $zero  OR  addu $rd, $rs, $zero)."""
    op = (word >> 26) & 0x3f
    rs = (word >> 21) & 0x1f
    rt = (word >> 16) & 0x1f
    rd = (word >> 11) & 0x1f
    sa = (word >> 6) & 0x1f
    funct = word & 0x3f

    if op != 0 or sa != 0:
        return None

    # or $rd, $rs, $zero  (funct=0x25)
    # addu $rd, $rs, $zero (funct=0x21)
    if funct in (0x25, 0x21) and rt == 0:
        return (rd, rs)  # (dest, src)

    return None


def patch_sll_sra_pattern_v2(elf_data, verbose=False):
    """Patch SLL/SRA pattern to use in-place register (v2 with move elimination)."""
    patched = bytearray(elf_data)

    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

    if verbose:
        print(f"ELF endianness: {'big' if is_big_endian else 'little'}")

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

    regs = ['zero','at','v0','v1','a0','a1','a2','a3',
            't0','t1','t2','t3','t4','t5','t6','t7',
            's0','s1','s2','s3','s4','s5','s6','s7',
            't8','t9','k0','k1','gp','sp','fp','ra']

    changes = 0

    # Read all instructions
    instructions = []
    for i in range(0, text_size, 4):
        file_off = text_offset + i
        if file_off + 4 > len(elf_data):
            break
        word = struct.unpack(endian + 'I', elf_data[file_off:file_off+4])[0]
        instructions.append((i, file_off, word))

    # Find SLL and SRA instructions with sa=16 or 24
    sll_list = []
    sra_list = []

    for idx, (off, file_off, word) in enumerate(instructions):
        op = (word >> 26) & 0x3f
        rs = (word >> 21) & 0x1f
        rt = (word >> 16) & 0x1f
        rd = (word >> 11) & 0x1f
        sa = (word >> 6) & 0x1f
        funct = word & 0x3f

        if op == 0 and rs == 0 and funct == 0 and sa in (16, 24):
            sll_list.append((idx, off, file_off, word, rt, rd, sa))
        if op == 0 and rs == 0 and funct == 3 and sa in (16, 24):
            sra_list.append((idx, off, file_off, word, rt, rd, sa))

    # Track which SLL/SRA have been matched
    matched_sll = set()
    matched_sra = set()

    for sll_idx, sll_off, sll_file_off, sll_word, sll_rt, sll_rd, sll_sa in sll_list:
        if sll_idx in matched_sll:
            continue

        for sra_idx, sra_off, sra_file_off, sra_word, sra_rt, sra_rd, sra_sa in sra_list:
            if sra_idx in matched_sra:
                continue
            if sra_off <= sll_off:
                continue
            if sll_sa != sra_sa:
                continue

            # Pattern A: sll $temp, $src, N; sra $src, $temp, N
            # SLL: rd=temp, rt=src. SRA: rd=src, rt=temp.
            # Condition: sra_rt == sll_rd (SRA reads SLL's temp output)
            #            sra_rd == sll_rt (SRA writes to SLL's source)
            #            sll_rd != sll_rt (temp != src)
            if (sra_rt == sll_rd and sra_rd == sll_rt and sll_rd != sll_rt):
                src = sll_rt
                sa = sll_sa
                new_sll = (src << 16) | (src << 11) | (sa << 6)
                new_sra = (src << 16) | (src << 11) | (sa << 6) | 3
                struct.pack_into(endian + 'I', patched, sll_file_off, new_sll)
                struct.pack_into(endian + 'I', patched, sra_file_off, new_sra)
                matched_sll.add(sll_idx)
                matched_sra.add(sra_idx)
                changes += 1
                if verbose:
                    print(f"  Pattern A at 0x{sll_off:04x}-0x{sra_off:04x}:")
                    print(f"    sll ${regs[sll_rd]},${regs[sll_rt]},{sa} -> sll ${regs[src]},${regs[src]},{sa}")
                    print(f"    sra ${regs[sra_rd]},${regs[sra_rt]},{sa} -> sra ${regs[src]},${regs[src]},{sa}")
                break

            # Pattern B: sll $dst, $src, N; sra $temp, $dst, N; ... move $dst, $temp
            # SLL: rd=dst, rt=src. SRA: rd=temp, rt=dst.
            # Condition: sra_rt == sll_rd (SRA reads from SLL's dst)
            #            sra_rd != sll_rd (SRA writes to temp, not dst)
            #            There must be a subsequent move $dst, $temp
            if (sra_rt == sll_rd and sra_rd != sll_rd and sll_rd != sll_rt):
                dst = sll_rd
                temp = sra_rd
                sa = sll_sa

                # Look for move $dst, $temp after the SRA
                move_found = False
                for check_idx in range(sra_idx + 1, len(instructions)):
                    check_off, check_file_off, check_word = instructions[check_idx]
                    mv = is_move_instruction(check_word)
                    if mv and mv[0] == dst and mv[1] == temp:
                        # Found the move! Fix the SRA and NOP the move.
                        new_sra = (dst << 16) | (dst << 11) | (sa << 6) | 3
                        struct.pack_into(endian + 'I', patched, sra_file_off, new_sra)
                        struct.pack_into(endian + 'I', patched, check_file_off, 0)  # NOP
                        matched_sll.add(sll_idx)
                        matched_sra.add(sra_idx)
                        changes += 1
                        move_found = True
                        if verbose:
                            print(f"  Pattern B at 0x{sll_off:04x}-0x{sra_off:04x}-0x{check_off:04x}:")
                            print(f"    sll ${regs[dst]},${regs[sll_rt]},{sa} (unchanged)")
                            print(f"    sra ${regs[temp]},${regs[dst]},{sa} -> sra ${regs[dst]},${regs[dst]},{sa}")
                            print(f"    move ${regs[dst]},${regs[temp]} -> nop")
                        break
                if move_found:
                    break

    return bytes(patched), changes


def main():
    parser = argparse.ArgumentParser(
        description='Fix SLL/SRA sign extension pattern (v2 with move elimination)'
    )
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('-v', '--verbose', action='store_true', help='Print changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, count = patch_sll_sra_pattern_v2(elf_data, args.verbose)

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Fixed {count} SLL/SRA patterns")


if __name__ == '__main__':
    main()
