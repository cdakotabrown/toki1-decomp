#!/usr/bin/env python3
"""
Delay Slot Rewriter - Per tokimeki_docs/04_post_link_rewriters.md

Moves instructions into delay slots when safe, to match aggressive
scheduling patterns in the original binary.

Safe rewrite checks for moving instruction I into delay slot of J:
- I has no memory dependency on side effects of J (or vice versa)
- I does not read registers clobbered by J
- For jal, treat $ra as clobbered and callee may clobber call-clobbered regs

Usage:
    python3 delay_slot_rewriter.py input.o output.o --target-exe EXE --address ADDR
"""

import argparse
import struct
import sys
from pathlib import Path

# MIPS instruction decoding
def decode_instruction(word):
    """Decode a MIPS instruction word into components."""
    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    shamt = (word >> 6) & 0x1F
    funct = word & 0x3F
    imm = word & 0xFFFF
    target = word & 0x3FFFFFF

    return {
        'word': word,
        'opcode': opcode,
        'rs': rs,
        'rt': rt,
        'rd': rd,
        'shamt': shamt,
        'funct': funct,
        'imm': imm,
        'target': target
    }

def is_branch_or_jump(instr):
    """Check if instruction is a branch or jump."""
    opcode = instr['opcode']
    funct = instr['funct']

    # J, JAL
    if opcode in [0x02, 0x03]:
        return True
    # BEQ, BNE, BLEZ, BGTZ
    if opcode in [0x04, 0x05, 0x06, 0x07]:
        return True
    # REGIMM (BLTZ, BGEZ, etc.)
    if opcode == 0x01:
        return True
    # JR, JALR (R-type)
    if opcode == 0x00 and funct in [0x08, 0x09]:
        return True
    return False

def is_nop(instr):
    """Check if instruction is a NOP (sll $zero, $zero, 0)."""
    return instr['word'] == 0x00000000

def get_read_regs(instr):
    """Get registers read by an instruction."""
    opcode = instr['opcode']
    funct = instr['funct']
    reads = set()

    if opcode == 0x00:  # R-type
        reads.add(instr['rs'])
        reads.add(instr['rt'])
    elif opcode in [0x04, 0x05]:  # BEQ, BNE
        reads.add(instr['rs'])
        reads.add(instr['rt'])
    elif opcode in [0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F]:  # I-type ALU
        reads.add(instr['rs'])
    elif opcode in [0x20, 0x21, 0x23, 0x24, 0x25]:  # Loads
        reads.add(instr['rs'])  # base register
    elif opcode in [0x28, 0x29, 0x2B]:  # Stores
        reads.add(instr['rs'])  # base register
        reads.add(instr['rt'])  # value to store

    reads.discard(0)  # $zero is never really read
    return reads

def get_write_regs(instr):
    """Get registers written by an instruction."""
    opcode = instr['opcode']
    funct = instr['funct']
    writes = set()

    if opcode == 0x00:  # R-type
        if funct not in [0x08]:  # JR doesn't write
            writes.add(instr['rd'])
        if funct == 0x09:  # JALR
            writes.add(instr['rd'])
    elif opcode == 0x03:  # JAL
        writes.add(31)  # $ra
    elif opcode in [0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F]:  # I-type ALU
        writes.add(instr['rt'])
    elif opcode in [0x20, 0x21, 0x23, 0x24, 0x25]:  # Loads
        writes.add(instr['rt'])
    elif opcode == 0x0F:  # LUI
        writes.add(instr['rt'])

    writes.discard(0)  # $zero is never really written
    return writes

def is_memory_op(instr):
    """Check if instruction accesses memory."""
    opcode = instr['opcode']
    # Loads: LB, LH, LW, LBU, LHU
    # Stores: SB, SH, SW
    return opcode in [0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B]

def can_move_to_delay_slot(candidate, branch):
    """
    Check if candidate instruction can safely be moved into branch's delay slot.

    Per tokimeki_docs/04_post_link_rewriters.md:
    - candidate has no memory dependency on side effects of branch
    - candidate does not read registers clobbered by branch
    - For jal, treat $ra as clobbered
    """
    cand = decode_instruction(candidate)
    br = decode_instruction(branch)

    # NOP is always safe but pointless
    if is_nop(cand):
        return False  # Don't move NOPs

    # Don't move branches/jumps into delay slots
    if is_branch_or_jump(cand):
        return False

    # Get register dependencies
    cand_reads = get_read_regs(cand)
    cand_writes = get_write_regs(cand)
    br_writes = get_write_regs(br)

    # For JAL, $ra is clobbered
    if br['opcode'] == 0x03:
        br_writes.add(31)

    # Candidate cannot read what branch writes
    if cand_reads & br_writes:
        return False

    # Candidate cannot write what branch reads (for conditional branches)
    br_reads = get_read_regs(br)
    if cand_writes & br_reads:
        return False

    # Memory operations need extra care
    if is_memory_op(cand):
        # Conservative: only allow stores of args ($a0-$a3) or $at
        # These are common patterns in the original binary
        if cand['opcode'] in [0x28, 0x29, 0x2B]:  # Stores
            reg = cand['rt']
            if reg not in [1, 4, 5, 6, 7]:  # $at, $a0-$a3
                return False

    return True

def rewrite_delay_slots(code_bytes, target_bytes, base_addr, verbose=False):
    """
    Attempt to rewrite delay slots to match target.

    Returns modified code bytes and list of changes made.
    """
    if len(code_bytes) != len(target_bytes):
        return code_bytes, []

    changes = []
    code = bytearray(code_bytes)

    # Process in 4-byte instruction chunks
    num_instrs = len(code) // 4

    for i in range(num_instrs - 1):
        offset = i * 4

        # Get current and target instructions
        curr_word = struct.unpack('<I', code[offset:offset+4])[0]
        next_word = struct.unpack('<I', code[offset+4:offset+8])[0]

        tgt_word = struct.unpack('<I', target_bytes[offset:offset+4])[0]
        tgt_next = struct.unpack('<I', target_bytes[offset+4:offset+8])[0]

        curr_instr = decode_instruction(curr_word)
        next_instr = decode_instruction(next_word)

        # If current is a branch/jump with NOP delay slot
        if is_branch_or_jump(curr_instr) and is_nop(next_instr):
            # Check if target has a filled delay slot
            tgt_instr = decode_instruction(tgt_word)
            tgt_delay = decode_instruction(tgt_next)

            if is_branch_or_jump(tgt_instr) and not is_nop(tgt_delay):
                # Target has filled delay slot - try to match
                # Look for the instruction to move (usually the one before the branch)
                if i > 0:
                    prev_offset = (i - 1) * 4
                    prev_word = struct.unpack('<I', code[prev_offset:prev_offset+4])[0]

                    if can_move_to_delay_slot(prev_word, curr_word):
                        # Check if moving this instruction matches target delay slot
                        if prev_word == tgt_next:
                            # Rewrite: swap prev with NOP, put prev in delay slot
                            struct.pack_into('<I', code, prev_offset, 0x00000000)  # NOP
                            struct.pack_into('<I', code, offset+4, prev_word)  # Fill delay

                            changes.append({
                                'address': hex(base_addr + offset),
                                'type': 'delay_slot_fill',
                                'moved_from': hex(base_addr + prev_offset),
                                'instruction': hex(prev_word)
                            })

                            if verbose:
                                print(f"  Filled delay slot at {hex(base_addr + offset)}")

    return bytes(code), changes

def main():
    parser = argparse.ArgumentParser(description='Delay slot rewriter')
    parser.add_argument('input', help='Input object file')
    parser.add_argument('output', help='Output object file')
    parser.add_argument('--target-exe', required=True, help='Target executable')
    parser.add_argument('--address', required=True, help='Function address (hex)')
    parser.add_argument('--verbose', '-v', action='store_true')

    args = parser.parse_args()

    # This is a stub - full implementation would:
    # 1. Read ELF object file
    # 2. Extract .text section
    # 3. Read target bytes from executable
    # 4. Apply delay slot rewriting
    # 5. Write modified object file

    print(f"Delay slot rewriter: {args.input} -> {args.output}")
    print(f"Target: {args.target_exe} @ {args.address}")
    print("Note: Full implementation pending - this is a framework stub")

    # Copy input to output for now
    import shutil
    shutil.copy(args.input, args.output)

if __name__ == '__main__':
    main()
