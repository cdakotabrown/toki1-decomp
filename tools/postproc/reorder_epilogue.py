#!/usr/bin/env python3
"""reorder_epilogue.py — Reorder MIPS epilogue instructions to match target.

The IDO compiler generates epilogue in a specific order:
    lw $ra, X($sp)
    addiu $sp, $sp, Y
    jr $ra
    nop

But the target binary might have a different order, often:
    addiu $sp, $sp, Y     (or jr $ra first for simple returns)
    lw $ra, X($sp)
    jr $ra
    nop

Or delay-slot optimized:
    jr $ra                 (with useful work in delay slot)
    <useful instruction>   (delay slot)

This tool finds the epilogue sequence in the compiled output and reorders
it to match the target sequence. It operates on the 4-instruction epilogue
block at the END of the function.

Usage:
    reorder_epilogue.py in.o out.o --target-order jr_ra,lw_ra,addiu_sp,nop

Target order tokens:
    jr_ra      - jr $ra (0x03e00008)
    lw_ra      - lw $ra, X($sp) (0x8fbfXXXX)
    addiu_sp   - addiu $sp, $sp, Y (0x27bdXXXX)
    nop        - nop (0x00000000)
    delay_insn - any non-nop instruction (keep as-is, it's a delay slot fill)

PostProc annotation:
    @PostProc: reorder_epilogue --target-order jr_ra,lw_ra,addiu_sp,nop
"""

import argparse
import re
import struct
import sys
from pathlib import Path


# Instruction patterns
JR_RA = 0x03E00008
NOP = 0x00000000

def is_lw_ra(word):
    return (word >> 16) == 0x8FBF  # lw $ra, XXXX($sp)

def is_addiu_sp(word):
    return (word >> 16) == 0x27BD  # addiu $sp, $sp, XXXX

def is_jr_ra(word):
    return word == JR_RA

def is_nop(word):
    return word == NOP

def classify_epilogue_insn(word):
    if is_jr_ra(word):
        return 'jr_ra'
    if is_lw_ra(word):
        return 'lw_ra'
    if is_addiu_sp(word):
        return 'addiu_sp'
    if is_nop(word):
        return 'nop'
    return 'other'


def find_text_section(data: bytearray):
    e_shoff = struct.unpack('<I', data[32:36])[0]
    e_shentsize = struct.unpack('<H', data[46:48])[0]
    e_shnum = struct.unpack('<H', data[48:50])[0]
    e_shstrndx = struct.unpack('<H', data[50:52])[0]

    shstr_hdr_off = e_shoff + (e_shstrndx * e_shentsize)
    shstr_off = struct.unpack('<I', data[shstr_hdr_off + 16:shstr_hdr_off + 20])[0]

    for i in range(e_shnum):
        sh_off = e_shoff + (i * e_shentsize)
        sh_name_idx = struct.unpack('<I', data[sh_off:sh_off + 4])[0]
        sec_off = struct.unpack('<I', data[sh_off + 16:sh_off + 20])[0]
        sec_size = struct.unpack('<I', data[sh_off + 20:sh_off + 24])[0]

        n0 = shstr_off + sh_name_idx
        n1 = data.find(b'\x00', n0)
        sec_name = data[n0:n1].decode('ascii', errors='ignore')

        if sec_name == '.text':
            return sec_off, sec_size

    raise ValueError("No .text section found")


def reorder_epilogue(data: bytearray, target_order: str):
    """Find and reorder the epilogue sequence at the end of .text."""
    text_off, text_size = find_text_section(data)

    # Scan backwards from end of .text to find the epilogue
    # Look for the last jr $ra instruction
    jr_ra_offset = None
    for off in range(text_size - 4, -1, -4):
        abs_off = text_off + off
        word = struct.unpack('<I', data[abs_off:abs_off + 4])[0]
        if is_jr_ra(word):
            jr_ra_offset = off
            break

    if jr_ra_offset is None:
        print("  No jr $ra found in .text — nothing to reorder")
        return

    # Collect the epilogue block (up to 4 instructions ending with jr_ra + delay slot)
    # Scan backwards from jr_ra to find lw_ra and addiu_sp
    epilogue_insns = []
    scan_start = max(0, jr_ra_offset - 12)  # Look back up to 3 instructions
    for off in range(scan_start, jr_ra_offset + 8, 4):
        abs_off = text_off + off
        word = struct.unpack('<I', data[abs_off:abs_off + 4])[0]
        cls = classify_epilogue_insn(word)
        epilogue_insns.append((off, word, cls))

    # Find contiguous epilogue block containing jr_ra
    # The epilogue should have: lw_ra, addiu_sp, jr_ra, nop (in some order)
    epilogue_types = {cls for _, _, cls in epilogue_insns}

    if 'jr_ra' not in epilogue_types:
        return

    # Find the epilogue block start (first epilogue insn before jr_ra)
    epilogue_start = None
    for off, word, cls in epilogue_insns:
        if cls in ('lw_ra', 'addiu_sp', 'jr_ra', 'nop'):
            if epilogue_start is None or off < epilogue_start:
                epilogue_start = off

    if epilogue_start is None:
        return

    # Collect epilogue instructions from epilogue_start to jr_ra + delay slot
    epilogue_block = []
    for off in range(epilogue_start, jr_ra_offset + 8, 4):
        abs_off = text_off + off
        word = struct.unpack('<I', data[abs_off:abs_off + 4])[0]
        cls = classify_epilogue_insn(word)
        epilogue_block.append((off, word, cls))

    # Parse target order
    order_tokens = [t.strip() for t in target_order.split(',')]

    # Build the reordered sequence
    available = {cls: word for _, word, cls in epilogue_block if cls != 'other'}
    available['nop_filler'] = NOP  # For missing slots

    reordered = []
    for token in order_tokens:
        if token in available:
            reordered.append(available.pop(token))
        elif token == 'delay_insn':
            # Keep the 'other' instruction as delay slot fill
            other_insns = [w for _, w, c in epilogue_block if c == 'other']
            if other_insns:
                reordered.append(other_insns[0])
            else:
                reordered.append(NOP)
        else:
            print(f"  Warning: token '{token}' not found in epilogue, using nop")
            reordered.append(NOP)

    # Write the reordered instructions back
    for i, (off, old_word, _) in enumerate(epilogue_block):
        if i < len(reordered):
            new_word = reordered[i]
            abs_off = text_off + off
            if old_word != new_word:
                struct.pack_into('<I', data, abs_off, new_word)
                print(f"  Reordered 0x{off:x}: 0x{old_word:08x} -> 0x{new_word:08x}")


def main():
    parser = argparse.ArgumentParser(description="Reorder MIPS epilogue instructions")
    parser.add_argument("input", type=Path, help="Input ELF object file")
    parser.add_argument("output", type=Path, help="Output ELF object file")
    parser.add_argument("--target-order", required=True,
                       help="Comma-separated target order (e.g., jr_ra,lw_ra,addiu_sp,nop)")
    args = parser.parse_args()

    data = bytearray(args.input.read_bytes())
    reorder_epilogue(data, args.target_order)
    args.output.write_bytes(data)


if __name__ == "__main__":
    main()
