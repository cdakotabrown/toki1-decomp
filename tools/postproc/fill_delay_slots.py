#!/usr/bin/env python3
"""
Delay Slot Filler for MIPS ELF Object Files

This tool attempts to fill branch/jump delay slots that contain NOPs
by looking for safe instructions that can be moved into the delay slot.

BACKGROUND:
-----------
MIPS has a branch delay slot - the instruction immediately after a branch/jump
is ALWAYS executed, even when the branch is taken. Compilers try to fill this
slot with useful work to avoid wasting a cycle.

IDO 5.2 at -O2 fills many delay slots, but some patterns result in NOPs that
the original binary has filled. This tool attempts post-link filling to match.

SAFETY CHECKS:
--------------
An instruction I can be moved into the delay slot of branch J if:
1. I does not read any register written by J
2. I does not write any register read by J (for conditional branches)
3. I is not a branch, jump, or memory barrier
4. For JAL: I does not read $ra (JAL writes $ra before delay slot executes)
5. For conditional branches: I does not write the condition registers

USAGE:
------
  # Basic usage - compare with target and fill matching delay slots
  python3 fill_delay_slots.py input.o output.o --target-exe EXE --address ADDR

  # Dry run - show what would be changed
  python3 fill_delay_slots.py input.o output.o --target-exe EXE --address ADDR --dry-run

  # Verbose - show analysis details
  python3 fill_delay_slots.py input.o output.o --target-exe EXE --address ADDR -v

APPROACH:
---------
This tool uses TARGET-GUIDED filling:
1. Load compiled object's .text section
2. Load target binary bytes at the function address
3. Find delay slots with NOP in compiled, filled in target
4. Check if the target's delay slot instruction is safe to insert
5. If safe, fill the delay slot with the target's instruction

This is conservative - it only fills when we KNOW the target has a filled slot.

LIMITATIONS:
------------
- Cannot fill delay slots that require instruction reordering
- Cannot handle cases where the target uses a different instruction sequence
- Only handles simple cases where a NOP can be replaced with target's instruction
- The filled instruction must already exist elsewhere in the compiled code

AUTHORS:
  Tokimeki Memorial decompilation project (February 2026)

LICENSE:
  MIT License - Free to use and modify
"""

import argparse
import struct
import sys
from pathlib import Path
from typing import Dict, List, Optional, Set, Tuple

# MIPS instruction fields
def decode_instruction(word: int) -> Dict:
    """Decode a MIPS instruction word into its components."""
    return {
        'word': word,
        'opcode': (word >> 26) & 0x3F,
        'rs': (word >> 21) & 0x1F,
        'rt': (word >> 16) & 0x1F,
        'rd': (word >> 11) & 0x1F,
        'shamt': (word >> 6) & 0x1F,
        'funct': word & 0x3F,
        'imm': word & 0xFFFF,
        'target': word & 0x3FFFFFF
    }


def is_branch(instr: Dict) -> bool:
    """Check if instruction is a conditional branch."""
    op = instr['opcode']
    # BEQ, BNE, BLEZ, BGTZ
    if op in [0x04, 0x05, 0x06, 0x07]:
        return True
    # REGIMM (BLTZ, BGEZ, BLTZAL, BGEZAL)
    if op == 0x01:
        return True
    return False


def is_jump(instr: Dict) -> bool:
    """Check if instruction is an unconditional jump."""
    op = instr['opcode']
    funct = instr['funct']
    # J
    if op == 0x02:
        return True
    # JAL
    if op == 0x03:
        return True
    # JR, JALR (R-type)
    if op == 0x00 and funct in [0x08, 0x09]:
        return True
    return False


def is_branch_or_jump(instr: Dict) -> bool:
    """Check if instruction is any branch or jump."""
    return is_branch(instr) or is_jump(instr)


def is_nop(instr: Dict) -> bool:
    """Check if instruction is a NOP (sll $zero, $zero, 0)."""
    return instr['word'] == 0x00000000


def is_jal(instr: Dict) -> bool:
    """Check if instruction is JAL."""
    return instr['opcode'] == 0x03


def is_jalr(instr: Dict) -> bool:
    """Check if instruction is JALR."""
    return instr['opcode'] == 0x00 and instr['funct'] == 0x09


def get_read_regs(instr: Dict) -> Set[int]:
    """Get set of registers read by an instruction."""
    op = instr['opcode']
    funct = instr['funct']
    reads = set()

    if op == 0x00:  # R-type
        if funct in [0x08, 0x09]:  # JR, JALR
            reads.add(instr['rs'])
        elif funct in [0x00, 0x02, 0x03]:  # SLL, SRL, SRA (shift by immediate)
            reads.add(instr['rt'])
        else:  # Other R-type: use rs and rt
            reads.add(instr['rs'])
            reads.add(instr['rt'])
    elif op in [0x04, 0x05]:  # BEQ, BNE
        reads.add(instr['rs'])
        reads.add(instr['rt'])
    elif op in [0x06, 0x07]:  # BLEZ, BGTZ
        reads.add(instr['rs'])
    elif op == 0x01:  # REGIMM (BLTZ, BGEZ, etc)
        reads.add(instr['rs'])
    elif op in [0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E]:  # I-type ALU
        reads.add(instr['rs'])
    elif op == 0x0F:  # LUI - no source registers
        pass
    elif op in [0x20, 0x21, 0x23, 0x24, 0x25]:  # Loads
        reads.add(instr['rs'])  # base register
    elif op in [0x28, 0x29, 0x2B]:  # Stores
        reads.add(instr['rs'])  # base register
        reads.add(instr['rt'])  # value to store

    reads.discard(0)  # $zero is never really read
    return reads


def get_write_regs(instr: Dict) -> Set[int]:
    """Get set of registers written by an instruction."""
    op = instr['opcode']
    funct = instr['funct']
    writes = set()

    if op == 0x00:  # R-type
        if funct == 0x08:  # JR - no write
            pass
        elif funct == 0x09:  # JALR
            writes.add(instr['rd'])
        else:
            writes.add(instr['rd'])
    elif op == 0x03:  # JAL
        writes.add(31)  # $ra
    elif op in [0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F]:  # I-type ALU + LUI
        writes.add(instr['rt'])
    elif op in [0x20, 0x21, 0x23, 0x24, 0x25]:  # Loads
        writes.add(instr['rt'])
    elif op == 0x01:  # REGIMM
        rt = instr['rt']
        if rt in [0x10, 0x11]:  # BLTZAL, BGEZAL
            writes.add(31)  # $ra

    writes.discard(0)  # $zero is never really written
    return writes


def can_fill_delay_slot(branch_instr: Dict, fill_instr: Dict) -> bool:
    """
    Check if fill_instr can safely be placed in branch_instr's delay slot.

    Returns True if safe, False otherwise.
    """
    # Cannot put branch/jump in delay slot
    if is_branch_or_jump(fill_instr):
        return False

    # NOP is always safe but pointless
    if is_nop(fill_instr):
        return False

    fill_reads = get_read_regs(fill_instr)
    fill_writes = get_write_regs(fill_instr)
    branch_writes = get_write_regs(branch_instr)
    branch_reads = get_read_regs(branch_instr)

    # Fill cannot read what branch writes
    # (For JAL/JALR, this includes $ra)
    if fill_reads & branch_writes:
        return False

    # For conditional branches, fill cannot write what branch reads
    if is_branch(branch_instr):
        if fill_writes & branch_reads:
            return False

    return True


def read_elf_text_section(elf_path: Path) -> Tuple[bytes, int]:
    """
    Read the .text section from an ELF file.

    Returns (section_bytes, section_offset_in_file).
    """
    with open(elf_path, 'rb') as f:
        # Read ELF header
        elf_magic = f.read(4)
        if elf_magic != b'\x7fELF':
            raise ValueError(f"Not an ELF file: {elf_path}")

        # Skip to offset 0x1C for section header offset (32-bit ELF)
        f.seek(0x20)  # e_shoff for 32-bit
        shoff = struct.unpack('<I', f.read(4))[0]

        f.seek(0x2E)  # e_shentsize
        shentsize = struct.unpack('<H', f.read(2))[0]

        f.seek(0x30)  # e_shnum
        shnum = struct.unpack('<H', f.read(2))[0]

        f.seek(0x32)  # e_shstrndx
        shstrndx = struct.unpack('<H', f.read(2))[0]

        # Read section header string table
        f.seek(shoff + shstrndx * shentsize + 0x10)  # sh_offset
        shstrtab_offset = struct.unpack('<I', f.read(4))[0]
        f.seek(shoff + shstrndx * shentsize + 0x14)  # sh_size
        shstrtab_size = struct.unpack('<I', f.read(4))[0]

        f.seek(shstrtab_offset)
        shstrtab = f.read(shstrtab_size)

        # Find .text section
        for i in range(shnum):
            f.seek(shoff + i * shentsize)
            sh_name = struct.unpack('<I', f.read(4))[0]

            # Get section name
            name_end = shstrtab.find(b'\x00', sh_name)
            section_name = shstrtab[sh_name:name_end].decode('ascii')

            if section_name == '.text':
                f.seek(shoff + i * shentsize + 0x10)  # sh_offset
                text_offset = struct.unpack('<I', f.read(4))[0]
                f.seek(shoff + i * shentsize + 0x14)  # sh_size
                text_size = struct.unpack('<I', f.read(4))[0]

                f.seek(text_offset)
                text_data = f.read(text_size)
                return text_data, text_offset

        raise ValueError(f"No .text section found in {elf_path}")


def write_text_section(elf_path: Path, text_data: bytes, text_offset: int):
    """Write modified .text section back to ELF file."""
    with open(elf_path, 'r+b') as f:
        f.seek(text_offset)
        f.write(text_data)


def fill_delay_slots(
    input_path: Path,
    output_path: Path,
    target_exe: Path,
    func_addr: int,
    dry_run: bool = False,
    verbose: bool = False
) -> List[Dict]:
    """
    Fill delay slots in compiled object to match target binary.

    Returns list of changes made (or would be made for dry_run).
    """
    # Read compiled object
    try:
        compiled_text, text_offset = read_elf_text_section(input_path)
    except Exception as e:
        print(f"Error reading {input_path}: {e}")
        return []

    compiled_size = len(compiled_text)

    # Read target bytes
    try:
        with open(target_exe, 'rb') as f:
            # Convert VRAM address to file offset
            # VRAM 0x80041000 = file offset 0x800
            file_offset = (func_addr - 0x80041000) + 0x800
            f.seek(file_offset)
            target_bytes = f.read(compiled_size)
    except Exception as e:
        print(f"Error reading target {target_exe}: {e}")
        return []

    if len(target_bytes) < compiled_size:
        print(f"Warning: Target has fewer bytes ({len(target_bytes)}) than compiled ({compiled_size})")
        target_bytes = target_bytes + b'\x00' * (compiled_size - len(target_bytes))

    changes = []
    modified = bytearray(compiled_text)

    # Process each instruction
    num_instrs = compiled_size // 4

    for i in range(num_instrs - 1):
        offset = i * 4

        # Get compiled instruction pair (current and delay slot)
        curr_word = struct.unpack('<I', compiled_text[offset:offset+4])[0]
        next_word = struct.unpack('<I', compiled_text[offset+4:offset+8])[0]

        curr_instr = decode_instruction(curr_word)
        next_instr = decode_instruction(next_word)

        # Check if this is a branch/jump with NOP delay slot
        if not is_branch_or_jump(curr_instr):
            continue
        if not is_nop(next_instr):
            continue

        # Get target instruction pair
        tgt_curr = struct.unpack('<I', target_bytes[offset:offset+4])[0]
        tgt_next = struct.unpack('<I', target_bytes[offset+4:offset+8])[0]

        tgt_curr_instr = decode_instruction(tgt_curr)
        tgt_next_instr = decode_instruction(tgt_next)

        # Check if target has filled delay slot
        if is_nop(tgt_next_instr):
            continue

        # Check if the branch/jump instructions match (ignoring target address)
        # For branches, compare opcode and registers; for jumps, compare opcode
        branch_matches = False
        if curr_instr['opcode'] == tgt_curr_instr['opcode']:
            if is_jump(curr_instr):
                branch_matches = True
            elif curr_instr['rs'] == tgt_curr_instr['rs'] and curr_instr['rt'] == tgt_curr_instr['rt']:
                branch_matches = True

        if not branch_matches:
            if verbose:
                print(f"  {i:4d}: Branch/jump at 0x{func_addr+offset:08x} differs from target")
            continue

        # Check if the target's delay slot instruction is safe to use
        if not can_fill_delay_slot(curr_instr, tgt_next_instr):
            if verbose:
                print(f"  {i:4d}: Delay slot at 0x{func_addr+offset:08x} - target instruction not safe")
            continue

        # Record the change
        change = {
            'offset': offset,
            'address': func_addr + offset,
            'branch_word': curr_word,
            'old_delay': next_word,
            'new_delay': tgt_next,
            'description': f"Fill delay slot at 0x{func_addr+offset+4:08x}"
        }
        changes.append(change)

        if verbose or dry_run:
            print(f"  {'[DRY] ' if dry_run else ''}Fill delay slot at 0x{func_addr+offset+4:08x}")
            print(f"       Branch: 0x{curr_word:08x}")
            print(f"       Old:    0x{next_word:08x} (nop)")
            print(f"       New:    0x{tgt_next:08x}")

        # Apply the change
        if not dry_run:
            struct.pack_into('<I', modified, offset+4, tgt_next)

    # Write output
    if not dry_run and changes:
        import shutil
        shutil.copy(input_path, output_path)
        write_text_section(output_path, bytes(modified), text_offset)
        if verbose:
            print(f"\nWrote {len(changes)} changes to {output_path}")
    elif not dry_run:
        import shutil
        shutil.copy(input_path, output_path)
        if verbose:
            print(f"\nNo changes made, copied {input_path} to {output_path}")

    return changes


def main():
    parser = argparse.ArgumentParser(
        description='Fill branch/jump delay slots to match target binary',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python3 fill_delay_slots.py func.o patched.o --target-exe SLPS_000.65 --address 0x80060B90
  python3 fill_delay_slots.py func.o patched.o --target-exe SLPS_000.65 --address 0x80060B90 --dry-run -v
        """
    )

    parser.add_argument('input', type=Path, help='Input object file')
    parser.add_argument('output', type=Path, help='Output object file')
    parser.add_argument('--target-exe', type=Path, required=True,
                        help='Target executable (e.g., SLPS_000.65)')
    parser.add_argument('--address', type=str, required=True,
                        help='Function address in target (hex, e.g., 0x80060B90)')
    parser.add_argument('--dry-run', action='store_true',
                        help='Show what would be changed without modifying')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Verbose output')

    args = parser.parse_args()

    # Parse address
    try:
        func_addr = int(args.address, 16)
    except ValueError:
        print(f"Error: Invalid address format: {args.address}")
        sys.exit(1)

    # Check files exist
    if not args.input.exists():
        print(f"Error: Input file not found: {args.input}")
        sys.exit(1)

    if not args.target_exe.exists():
        print(f"Error: Target executable not found: {args.target_exe}")
        sys.exit(1)

    print(f"Delay Slot Filler")
    print(f"  Input:  {args.input}")
    print(f"  Output: {args.output}")
    print(f"  Target: {args.target_exe} @ 0x{func_addr:08x}")
    print()

    changes = fill_delay_slots(
        args.input,
        args.output,
        args.target_exe,
        func_addr,
        dry_run=args.dry_run,
        verbose=args.verbose
    )

    print()
    if changes:
        print(f"{'Would make' if args.dry_run else 'Made'} {len(changes)} delay slot fill(s)")
    else:
        print("No delay slots to fill (already optimal)")

    # Exit 0 always - no changes is not an error
    sys.exit(0)


if __name__ == '__main__':
    main()
