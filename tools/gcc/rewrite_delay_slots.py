#!/usr/bin/env python3
"""
Delay Slot Rewriter for GCC->Target matching.

Algorithm:
1. Load ELF object file
2. Find all branch/jump instructions
3. For JR RA with NOP in delay slot:
   - Look for preceding LI V0,N instruction
   - If found and no hazards, move to delay slot
4. For other branches, apply similar heuristics
5. Write modified ELF

This tool helps match GCC-compiled code to target by rewriting
delay slot scheduling to match the original compiler's choices.
"""

import struct
import sys
from pathlib import Path
from typing import Dict, List, Tuple, Optional

# MIPS instruction encoding helpers
def decode_instruction(word: int) -> Dict[str, int]:
    """Decode MIPS instruction into components."""
    opcode = (word >> 26) & 0x3F
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    rd = (word >> 11) & 0x1F
    shamt = (word >> 6) & 0x1F
    funct = word & 0x3F
    imm = word & 0xFFFF
    # Sign-extend immediate
    simm = imm if imm < 0x8000 else imm - 0x10000
    return {
        'opcode': opcode,
        'rs': rs, 'rt': rt, 'rd': rd,
        'shamt': shamt, 'funct': funct,
        'imm': imm, 'simm': simm,
        'raw': word
    }

def encode_instruction(instr: Dict[str, int]) -> int:
    """Encode MIPS instruction from components."""
    if instr['opcode'] == 0:  # R-type
        return ((instr['rs'] << 21) | (instr['rt'] << 16) |
                (instr['rd'] << 11) | (instr['shamt'] << 6) |
                instr['funct'])
    else:  # I-type or J-type
        return ((instr['opcode'] << 26) | (instr['rs'] << 21) |
                (instr['rt'] << 16) | (instr['imm'] & 0xFFFF))

def is_jr_ra(instr: Dict[str, int]) -> bool:
    """Check if instruction is JR RA (return)."""
    return instr['raw'] == 0x03E00008

def is_nop(instr: Dict[str, int]) -> bool:
    """Check if instruction is NOP."""
    return instr['raw'] == 0x00000000

def is_li_v0(instr: Dict[str, int]) -> bool:
    """Check if instruction is LI V0, N (ORI v0, zero, N)."""
    # ORI v0, zero, N = 0x34020000 | N
    # Also check ADDIU v0, zero, N for small values
    if instr['opcode'] == 0x0D and instr['rt'] == 2 and instr['rs'] == 0:
        return True  # ORI v0, zero, N
    if instr['opcode'] == 0x09 and instr['rt'] == 2 and instr['rs'] == 0:
        return True  # ADDIU v0, zero, N
    return False

def is_addiu_sp(instr: Dict[str, int]) -> bool:
    """Check if instruction is ADDIU SP, SP, N."""
    # ADDIU sp, sp, N
    return (instr['opcode'] == 0x09 and instr['rt'] == 29 and
            instr['rs'] == 29)

def get_instruction_reads(instr: Dict[str, int]) -> List[int]:
    """Get list of registers read by instruction."""
    reads = []
    opcode = instr['opcode']

    if opcode == 0:  # R-type
        if instr['funct'] == 0x08:  # JR
            reads.append(instr['rs'])
        elif instr['funct'] in [0x09]:  # JALR
            reads.append(instr['rs'])
        else:  # Most R-type read rs and rt
            if instr['funct'] not in [0x00, 0x02, 0x03]:  # Not SLL/SRL/SRA
                reads.append(instr['rs'])
            reads.append(instr['rt'])
    elif opcode in [0x08, 0x09, 0x0C, 0x0D, 0x0A, 0x0B]:  # I-type arithmetic/logic
        reads.append(instr['rs'])
    elif opcode in [0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B]:  # Load/Store
        reads.append(instr['rs'])
        if opcode >= 0x28:  # Store operations also read rt
            reads.append(instr['rt'])
    elif opcode in [0x04, 0x05, 0x06, 0x07]:  # Branches
        reads.append(instr['rs'])
        if opcode in [0x04, 0x05]:  # BEQ/BNE
            reads.append(instr['rt'])

    return reads

def get_instruction_writes(instr: Dict[str, int]) -> List[int]:
    """Get list of registers written by instruction."""
    writes = []
    opcode = instr['opcode']

    if opcode == 0:  # R-type
        if instr['funct'] == 0x09:  # JALR
            writes.append(instr['rd'] if instr['rd'] != 0 else 31)
        elif instr['funct'] not in [0x08]:  # Not JR
            if instr['rd'] != 0:
                writes.append(instr['rd'])
    elif opcode in [0x08, 0x09, 0x0C, 0x0D, 0x0A, 0x0B, 0x0F]:  # I-type that write
        if instr['rt'] != 0:
            writes.append(instr['rt'])
    elif opcode in [0x20, 0x21, 0x23, 0x24, 0x25]:  # Load operations
        if instr['rt'] != 0:
            writes.append(instr['rt'])
    elif opcode in [0x03]:  # JAL
        writes.append(31)  # RA

    return writes

def has_hazard(branch_instr: Dict[str, int], delay_candidate: Dict[str, int]) -> bool:
    """
    Check if moving delay_candidate to delay slot creates hazard.

    A hazard exists if:
    - Delay candidate writes a register that branch reads
    - Branch writes a register that delay candidate reads
    - Delay candidate is a branch/jump itself
    """
    # Can't put branches in delay slots
    if delay_candidate['opcode'] in [0x02, 0x03] or (
        delay_candidate['opcode'] == 0 and delay_candidate['funct'] in [0x08, 0x09]):
        return True

    # Can't put loads in delay slots of JR (pipeline hazard)
    if is_jr_ra(branch_instr) and delay_candidate['opcode'] in [0x20, 0x21, 0x23, 0x24, 0x25]:
        return True

    branch_reads = get_instruction_reads(branch_instr)
    branch_writes = get_instruction_writes(branch_instr)
    delay_reads = get_instruction_reads(delay_candidate)
    delay_writes = get_instruction_writes(delay_candidate)

    # Check write-after-read hazard
    for reg in delay_writes:
        if reg in branch_reads:
            return True

    # Check read-after-write hazard
    for reg in branch_writes:
        if reg in delay_reads:
            return True

    return False

def parse_elf_sections(elf_data: bytes) -> Dict[str, Tuple[int, int]]:
    """
    Parse ELF file and return section information.
    Returns dict of {section_name: (offset, size)}
    """
    # Check ELF magic
    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not a valid ELF file")

    # Read ELF header
    ei_class = elf_data[4]  # 1 = 32-bit, 2 = 64-bit
    ei_data = elf_data[5]   # 1 = little-endian, 2 = big-endian

    if ei_class != 1:
        raise ValueError("Only 32-bit ELF supported")

    endian = '<' if ei_data == 1 else '>'

    # Parse ELF32 header
    e_shoff = struct.unpack(endian + 'I', elf_data[32:36])[0]  # Section header offset
    e_shentsize = struct.unpack(endian + 'H', elf_data[46:48])[0]  # Section header size
    e_shnum = struct.unpack(endian + 'H', elf_data[48:50])[0]  # Number of section headers
    e_shstrndx = struct.unpack(endian + 'H', elf_data[50:52])[0]  # String table index

    # Read section header string table
    shstrtab_offset = e_shoff + e_shstrndx * e_shentsize
    sh_offset = struct.unpack(endian + 'I', elf_data[shstrtab_offset+16:shstrtab_offset+20])[0]
    sh_size = struct.unpack(endian + 'I', elf_data[shstrtab_offset+20:shstrtab_offset+24])[0]
    shstrtab = elf_data[sh_offset:sh_offset+sh_size]

    # Parse all section headers
    sections = {}
    for i in range(e_shnum):
        sh_addr = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack(endian + 'I', elf_data[sh_addr:sh_addr+4])[0]
        sh_offset = struct.unpack(endian + 'I', elf_data[sh_addr+16:sh_addr+20])[0]
        sh_size = struct.unpack(endian + 'I', elf_data[sh_addr+20:sh_addr+24])[0]

        # Extract section name
        name_end = shstrtab.find(b'\x00', sh_name_idx)
        if name_end == -1:
            name = shstrtab[sh_name_idx:].decode('ascii', errors='ignore')
        else:
            name = shstrtab[sh_name_idx:name_end].decode('ascii', errors='ignore')

        sections[name] = (sh_offset, sh_size)

    return sections

def rewrite_delay_slots(elf_data: bytes, verbose: bool = False) -> Tuple[bytes, int]:
    """
    Rewrite delay slots to match target patterns.

    Patterns applied:
    1. JR RA with NOP -> move preceding LI V0 to delay slot
    2. JR RA with NOP -> move preceding ADDIU SP to delay slot (for epilogue)
    3. JR RA with ADDIU SP -> swap with preceding LI V0 (GCC 2.7.2 pattern)
    """
    sections = parse_elf_sections(elf_data)

    if '.text' not in sections:
        print("Warning: .text section not found, searching entire file")
        text_offset = 0x34  # Typical offset
        text_size = len(elf_data) - text_offset
    else:
        text_offset, text_size = sections['.text']

    if verbose:
        print(f"Processing .text section: offset=0x{text_offset:x}, size=0x{text_size:x}")

    modified = bytearray(elf_data)
    changes = 0
    patterns_found = {'li_v0_moved': 0, 'addiu_sp_moved': 0, 'li_v0_swapped': 0, 'already_optimal': 0}

    i = text_offset
    while i < text_offset + text_size - 4:  # -4 to ensure we can read next instr
        # Read current and next instruction (if available)
        curr = struct.unpack('<I', modified[i:i+4])[0]
        if i + 4 < text_offset + text_size:
            next_instr = struct.unpack('<I', modified[i+4:i+8])[0]
        else:
            next_instr = 0  # NOP if at end

        curr_decoded = decode_instruction(curr)
        next_decoded = decode_instruction(next_instr)

        # Pattern: JR RA followed by something
        if is_jr_ra(curr_decoded):
            if is_nop(next_decoded):
                # Look back for useful instruction to move
                if i >= text_offset + 4:
                    prev = struct.unpack('<I', modified[i-4:i])[0]
                    prev_decoded = decode_instruction(prev)

                    # Try LI V0, N pattern
                    if is_li_v0(prev_decoded) and not has_hazard(curr_decoded, prev_decoded):
                        # Move LI V0 to delay slot
                        # Replace: [LI V0] [JR RA] [NOP]
                        # With:    [NOP] [JR RA] [LI V0]
                        modified[i-4:i] = struct.pack('<I', 0)  # NOP
                        modified[i+4:i+8] = struct.pack('<I', prev)  # LI V0
                        changes += 1
                        patterns_found['li_v0_moved'] += 1
                        if verbose:
                            print(f"  0x{i:08x}: Moved LI V0 to delay slot")

                    # Try ADDIU SP pattern (epilogue)
                    elif is_addiu_sp(prev_decoded) and not has_hazard(curr_decoded, prev_decoded):
                        # Move ADDIU SP to delay slot
                        modified[i-4:i] = struct.pack('<I', 0)  # NOP
                        modified[i+4:i+8] = struct.pack('<I', prev)  # ADDIU SP
                        changes += 1
                        patterns_found['addiu_sp_moved'] += 1
                        if verbose:
                            print(f"  0x{i:08x}: Moved ADDIU SP to delay slot")

            # GCC 2.7.2 pattern: JR RA with ADDIU SP in delay slot, but we want LI V0
            elif is_addiu_sp(next_decoded):
                # Look back for LI V0
                if i >= text_offset + 4:
                    prev = struct.unpack('<I', modified[i-4:i])[0]
                    prev_decoded = decode_instruction(prev)

                    if is_li_v0(prev_decoded):
                        # Swap: [LI V0] [JR RA] [ADDIU SP]
                        # To:   [ADDIU SP] [JR RA] [LI V0]
                        modified[i-4:i] = struct.pack('<I', next_instr)  # ADDIU SP before JR
                        modified[i+4:i+8] = struct.pack('<I', prev)  # LI V0 in delay slot
                        changes += 1
                        patterns_found['li_v0_swapped'] += 1
                        if verbose:
                            print(f"  0x{i:08x}: Swapped LI V0 with ADDIU SP (GCC 2.7.2 fix)")

            # Check if already optimal (LI V0 in delay slot)
            elif is_li_v0(next_decoded):
                patterns_found['already_optimal'] += 1

        i += 4

    if verbose:
        print(f"\nDelay slot statistics:")
        print(f"  LI V0 moved to slot: {patterns_found['li_v0_moved']}")
        print(f"  ADDIU SP moved to slot: {patterns_found['addiu_sp_moved']}")
        print(f"  LI V0 swapped with ADDIU SP: {patterns_found['li_v0_swapped']}")
        print(f"  Already optimal: {patterns_found['already_optimal']}")

    return bytes(modified), changes

def main():
    if len(sys.argv) < 3:
        print(f'Usage: {sys.argv[0]} <input.o> <output.o> [-v|--verbose]')
        print()
        print('Rewrites delay slots in GCC-compiled MIPS ELF to match target patterns.')
        print()
        print('Patterns:')
        print('  - Moves LI V0 instructions into JR RA delay slots')
        print('  - Moves ADDIU SP instructions into JR RA delay slots')
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]
    verbose = '-v' in sys.argv or '--verbose' in sys.argv

    if not Path(input_path).exists():
        print(f'Error: {input_path} not found')
        sys.exit(1)

    with open(input_path, 'rb') as f:
        elf_data = f.read()

    try:
        modified, changes = rewrite_delay_slots(elf_data, verbose=verbose)
    except Exception as e:
        print(f'Error: {e}')
        sys.exit(1)

    with open(output_path, 'wb') as f:
        f.write(modified)

    print(f'Rewrote {changes} delay slots')
    print(f'Output: {output_path}')

if __name__ == '__main__':
    main()
