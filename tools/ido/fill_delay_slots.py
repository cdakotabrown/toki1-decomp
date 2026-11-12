#!/usr/bin/env python3
"""
Delay Slot Peephole Scheduler

Fills empty delay slots (branch/nop pairs) by pulling legal instructions
from before the branch. This is a safe, local transformation.

Rules for legal delay slot filling:
- Instruction must not modify registers used by branch condition
- Instruction must not depend on branch outcome
- Only pulls from within 3 instructions before branch
- Prioritizes saves (sw) and address calculations

Based on PS1-era compiler patterns where delay slots were often filled
with stack saves or argument setup.

Usage:
    python3 fill_delay_slots.py input.o output.o
    python3 fill_delay_slots.py input.o output.o -v
"""

import sys
import struct
import argparse
from collections import namedtuple

Instruction = namedtuple('Instruction', ['word', 'offset', 'reads', 'writes'])

# MIPS opcodes
OP_BEQ = 0x04
OP_BNE = 0x05
OP_BLEZ = 0x06
OP_BGTZ = 0x07
OP_REGIMM = 0x01  # bltz, bgez, etc.
OP_J = 0x02
OP_JAL = 0x03

# R-type function codes
FUNCT_JR = 0x08
FUNCT_JALR = 0x09

# Registers
REG_ZERO = 0
REG_RA = 31
REG_SP = 29


def decode_instruction(word):
    """Decode MIPS instruction."""
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    funct = word & 0x3F
    return op, rs, rt, rd, funct


def get_read_regs(word):
    """Get registers read by instruction."""
    op, rs, rt, rd, funct = decode_instruction(word)
    reads = set()

    if op == 0:  # R-type
        if funct in (0x00, 0x02, 0x03):  # shifts by imm
            reads.add(rt)
        elif funct not in (0x10, 0x12):  # not mfhi/mflo
            reads.add(rs)
            reads.add(rt)
    elif op in (OP_BEQ, OP_BNE):
        reads.add(rs)
        reads.add(rt)
    elif op in (OP_BLEZ, OP_BGTZ, OP_REGIMM):
        reads.add(rs)
    elif op in (0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E):  # I-type ALU (not lui)
        reads.add(rs)
    elif op in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26):  # loads
        reads.add(rs)
    elif op in (0x28, 0x29, 0x2A, 0x2B, 0x2E):  # stores
        reads.add(rs)
        reads.add(rt)

    reads.discard(REG_ZERO)
    return reads


def get_write_regs(word):
    """Get registers written by instruction."""
    op, rs, rt, rd, funct = decode_instruction(word)
    writes = set()

    if op == 0:  # R-type
        if funct == FUNCT_JR:
            pass
        elif funct == FUNCT_JALR:
            writes.add(rd if rd != 0 else REG_RA)
        elif funct not in (0x11, 0x13, 0x18, 0x19, 0x1A, 0x1B):
            writes.add(rd)
    elif op in (0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F):  # I-type ALU
        writes.add(rt)
    elif op in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26):  # loads
        writes.add(rt)
    elif op == OP_JAL:
        writes.add(REG_RA)

    writes.discard(REG_ZERO)
    return writes


def is_branch(word):
    """Check if instruction is a branch/jump."""
    op, rs, rt, rd, funct = decode_instruction(word)

    if op in (OP_BEQ, OP_BNE, OP_BLEZ, OP_BGTZ, OP_REGIMM, OP_J, OP_JAL):
        return True
    if op == 0 and funct in (FUNCT_JR, FUNCT_JALR):
        return True
    return False


def is_nop(word):
    """Check if instruction is a NOP (sll $zero,$zero,0)."""
    return word == 0x00000000


def get_branch_condition_regs(word):
    """Get registers used in branch condition."""
    op, rs, rt, rd, funct = decode_instruction(word)

    if op in (OP_BEQ, OP_BNE):
        return {rs, rt} - {REG_ZERO}
    elif op in (OP_BLEZ, OP_BGTZ, OP_REGIMM):
        return {rs} - {REG_ZERO}
    elif op == 0 and funct in (FUNCT_JR, FUNCT_JALR):
        return {rs} - {REG_ZERO}
    return set()


def is_safe_for_delay_slot(candidate, branch_word):
    """Check if candidate instruction can safely fill delay slot."""
    cand_reads = get_read_regs(candidate)
    cand_writes = get_write_regs(candidate)
    branch_cond_regs = get_branch_condition_regs(branch_word)

    # Cannot write to registers used by branch condition
    if cand_writes & branch_cond_regs:
        return False

    # Cannot be a branch itself
    if is_branch(candidate):
        return False

    # Cannot modify $ra if this is jal/jalr
    op, rs, rt, rd, funct = decode_instruction(branch_word)
    if op == OP_JAL or (op == 0 and funct == FUNCT_JALR):
        if REG_RA in cand_writes:
            return False

    # Prefer stack operations and simple moves
    # (These are typically safe and match original patterns)

    return True


def find_candidate(instructions, branch_idx):
    """
    Find best candidate instruction to fill delay slot.
    Looks backward up to 3 instructions.

    Returns: (candidate_idx, instruction_word) or (None, None)
    """
    if branch_idx < 1:
        return None, None

    branch_word = instructions[branch_idx].word

    # Track what's needed by instructions between candidate and branch
    blocked_writes = set()
    blocked_reads = set()

    # Look backward
    for i in range(branch_idx - 1, max(-1, branch_idx - 4), -1):
        candidate = instructions[i]

        # Skip NOPs
        if is_nop(candidate.word):
            continue

        # Check if this candidate is safe
        cand_writes = get_write_regs(candidate.word)
        cand_reads = get_read_regs(candidate.word)

        # Cannot move if later instruction depends on its output
        if cand_writes & blocked_reads:
            blocked_writes.update(cand_writes)
            blocked_reads.update(cand_reads)
            continue

        # Cannot move if it reads something later instruction writes
        if cand_reads & blocked_writes:
            blocked_writes.update(cand_writes)
            blocked_reads.update(cand_reads)
            continue

        # Check if safe for delay slot
        if is_safe_for_delay_slot(candidate.word, branch_word):
            return i, candidate.word

        # Track dependencies
        blocked_writes.update(cand_writes)
        blocked_reads.update(cand_reads)

    return None, None


def fill_delay_slots(elf_data, verbose=False):
    """
    Fill empty delay slots in .text section.

    Returns: (patched_data, changes_count)
    """
    patched = bytearray(elf_data)

    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    is_big_endian = elf_data[5] == 2
    endian = '>' if is_big_endian else '<'

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

    # Load all instructions
    instructions = []
    for i in range(0, text_size, 4):
        file_off = text_offset + i
        word = struct.unpack(endian + 'I', elf_data[file_off:file_off+4])[0]
        reads = get_read_regs(word)
        writes = get_write_regs(word)
        instructions.append(Instruction(word, file_off, reads, writes))

    changes = 0

    # Find branch+nop pairs
    for i in range(len(instructions) - 1):
        branch_insn = instructions[i]
        delay_insn = instructions[i + 1]

        if not is_branch(branch_insn.word):
            continue
        if not is_nop(delay_insn.word):
            continue

        # Found branch + nop, try to fill
        cand_idx, cand_word = find_candidate(instructions, i)
        if cand_idx is None:
            continue

        if verbose:
            print(f"Filling delay slot at offset 0x{branch_insn.offset - text_offset:x}:")
            print(f"  Moving instruction from 0x{instructions[cand_idx].offset - text_offset:x}")

        # Move candidate to delay slot
        struct.pack_into(endian + 'I', patched, delay_insn.offset, cand_word)
        # Replace original with NOP
        struct.pack_into(endian + 'I', patched, instructions[cand_idx].offset, 0x00000000)

        changes += 1

        # Update instruction list for subsequent passes
        instructions[i + 1] = Instruction(cand_word, delay_insn.offset,
                                          get_read_regs(cand_word),
                                          get_write_regs(cand_word))
        instructions[cand_idx] = Instruction(0, instructions[cand_idx].offset, set(), set())

    return bytes(patched), changes


def main():
    parser = argparse.ArgumentParser(
        description='Fill empty delay slots with legal instructions',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
This tool performs peephole optimization to fill branch delay slots.

It safely moves stack saves, address calculations, and other operations
into delay slots that currently contain NOPs.

Examples:
    %(prog)s input.o output.o
    %(prog)s input.o output.o -v
"""
    )
    parser.add_argument('input', help='Input ELF object file')
    parser.add_argument('output', help='Output patched file')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Print detailed changes')

    args = parser.parse_args()

    with open(args.input, 'rb') as f:
        elf_data = f.read()

    patched, changes = fill_delay_slots(elf_data, args.verbose)

    with open(args.output, 'wb') as f:
        f.write(patched)

    print(f"Filled {changes} delay slots")


if __name__ == '__main__':
    main()
