#!/usr/bin/env python3
"""
swap_registers_liveness.py - Liveness-Aware Register Renaming Tool

This tool uses liveness analysis to safely rename registers by analyzing
def-use chains and ensuring non-overlapping live ranges.

Problem solved:
    The existing swap_registers_auto.py does global 1:1 swaps, which corrupts
    code when a register (e.g., v0) maps to MULTIPLE different registers
    (e.g., both t6 and t7) at different program points.

Solution:
    1. Build a control flow graph (CFG)
    2. Compute live ranges for each register
    3. Identify rename opportunities where live ranges don't overlap
    4. Apply per-instruction or per-basic-block renaming
    5. Validate by comparing against target

Usage:
    # Automatic detection with liveness analysis
    python3 swap_registers_liveness.py input.o output.o \
        --target-exe disks/SLPS_000.65 \
        --address 0x8006CF20

    # Report mode (show liveness info without modifying)
    python3 swap_registers_liveness.py input.o output.o \
        --target-exe disks/SLPS_000.65 \
        --address 0x8006CF20 \
        --report-only

Test case:
    icon_schedule_set has known s1<->s2 swap plus temp register differences

Date: February 2026
"""

import argparse
import struct
import sys
import shutil
import subprocess
import tempfile
import os
from pathlib import Path
from typing import List, Tuple, Dict, Optional, Set
from dataclasses import dataclass, field
from collections import defaultdict

# MIPS register numbers
REGS = {
    'zero': 0, 'at': 1,
    'v0': 2, 'v1': 3,
    'a0': 4, 'a1': 5, 'a2': 6, 'a3': 7,
    't0': 8, 't1': 9, 't2': 10, 't3': 11,
    't4': 12, 't5': 13, 't6': 14, 't7': 15,
    's0': 16, 's1': 17, 's2': 18, 's3': 19,
    's4': 20, 's5': 21, 's6': 22, 's7': 23,
    't8': 24, 't9': 25,
    'k0': 26, 'k1': 27,
    'gp': 28, 'sp': 29, 'fp': 30, 'ra': 31
}

REG_NAMES = {v: k for k, v in REGS.items()}

@dataclass
class Instruction:
    """Represents a decoded MIPS instruction with liveness info."""
    offset: int
    word: int
    asm: str
    opcode: int
    rs: int
    rt: int
    rd: int
    shamt: int
    funct: int
    imm: int

    # Liveness info
    def_regs: Set[int] = field(default_factory=set)  # Registers defined (written)
    use_regs: Set[int] = field(default_factory=set)  # Registers used (read)
    live_in: Set[int] = field(default_factory=set)   # Live at entry
    live_out: Set[int] = field(default_factory=set)  # Live at exit

    def is_branch(self) -> bool:
        """Check if this is a branch instruction."""
        return self.opcode in [0x01, 0x04, 0x05, 0x06, 0x07]  # bcond, beq, bne, blez, bgtz

    def is_jump(self) -> bool:
        """Check if this is a jump instruction."""
        return self.opcode in [0x02, 0x03] or (self.opcode == 0 and self.funct in [0x08, 0x09])  # j, jal, jr, jalr

    def is_load(self) -> bool:
        """Check if this is a load instruction."""
        return self.opcode in [0x20, 0x21, 0x23, 0x24, 0x25]  # lb, lh, lw, lbu, lhu

    def is_store(self) -> bool:
        """Check if this is a store instruction."""
        return self.opcode in [0x28, 0x29, 0x2b]  # sb, sh, sw

    def branch_target(self, pc: int) -> int:
        """Calculate branch target address."""
        if not self.is_branch():
            return -1
        # Sign-extend 16-bit offset, multiply by 4, add to PC+4
        offset = self.imm if self.imm < 0x8000 else self.imm - 0x10000
        return pc + 4 + (offset * 4)

@dataclass
class BasicBlock:
    """Represents a basic block in the CFG."""
    start_offset: int
    end_offset: int
    instructions: List[Instruction]
    successors: List[int] = field(default_factory=list)  # Successor block start offsets
    predecessors: List[int] = field(default_factory=list)  # Predecessor block start offsets
    live_in: Set[int] = field(default_factory=set)
    live_out: Set[int] = field(default_factory=set)

@dataclass
class LiveRange:
    """Represents the live range of a register."""
    reg: int
    start_offset: int
    end_offset: int
    instruction_offsets: Set[int] = field(default_factory=set)

    def overlaps(self, other: 'LiveRange') -> bool:
        """Check if this live range overlaps with another."""
        return not (self.end_offset < other.start_offset or other.end_offset < self.start_offset)

def decode_instruction(instr: int) -> Dict:
    """Decode a MIPS instruction into its components."""
    opcode = (instr >> 26) & 0x3F
    rs = (instr >> 21) & 0x1F
    rt = (instr >> 16) & 0x1F
    rd = (instr >> 11) & 0x1F
    shamt = (instr >> 6) & 0x1F
    funct = instr & 0x3F
    imm = instr & 0xFFFF

    return {
        'opcode': opcode,
        'rs': rs, 'rt': rt, 'rd': rd,
        'shamt': shamt, 'funct': funct,
        'imm': imm
    }

def disassemble_bytes(data: bytes, vram_addr: int = 0) -> List[Instruction]:
    """
    Disassemble raw bytes into Instruction objects.
    """
    with tempfile.NamedTemporaryFile(suffix='.bin', delete=False) as tmp:
        tmp.write(data)
        tmp_path = tmp.name

    try:
        cmd = [
            "mipsel-linux-gnu-objdump",
            "-D", "-b", "binary", "-m", "mips:3000",
            "--adjust-vma", hex(vram_addr),
            tmp_path
        ]
        result = subprocess.run(cmd, capture_output=True, text=True)

        instructions = []
        in_disasm = False
        for line in result.stdout.split('\n'):
            if '<.data>:' in line:
                in_disasm = True
                continue
            if in_disasm and ':' in line:
                parts = line.strip().split('\t')
                if len(parts) >= 3:
                    addr_hex = parts[0].split(':')[0].strip()
                    asm_text = '\t'.join(parts[2:]).strip()

                    # Parse instruction word from actual data (little-endian)
                    offset = int(addr_hex, 16) - vram_addr
                    if offset >= 0 and offset + 4 <= len(data):
                        instr_word = struct.unpack('<I', data[offset:offset+4])[0]
                        decoded = decode_instruction(instr_word)

                        instr = Instruction(
                            offset=offset,
                            word=instr_word,
                            asm=asm_text,
                            **decoded
                        )

                        # Compute def/use sets
                        compute_def_use(instr)
                        instructions.append(instr)

        return instructions
    finally:
        os.unlink(tmp_path)

def compute_def_use(instr: Instruction):
    """Compute def and use sets for an instruction."""
    opcode = instr.opcode

    # R-type instructions (opcode = 0)
    if opcode == 0:
        if instr.funct in [0x08]:  # jr
            instr.use_regs.add(instr.rs)
        elif instr.funct in [0x09]:  # jalr
            instr.use_regs.add(instr.rs)
            instr.def_regs.add(instr.rd if instr.rd != 0 else 31)  # Default to $ra
        elif instr.funct in [0x00, 0x02, 0x03]:  # sll, srl, sra (shifts by immediate)
            if instr.rt != 0:
                instr.use_regs.add(instr.rt)
            if instr.rd != 0:
                instr.def_regs.add(instr.rd)
        elif instr.funct in [0x10, 0x12]:  # mfhi, mflo
            if instr.rd != 0:
                instr.def_regs.add(instr.rd)
        else:  # Standard R-type (add, sub, and, or, etc.)
            if instr.rs != 0:
                instr.use_regs.add(instr.rs)
            if instr.rt != 0:
                instr.use_regs.add(instr.rt)
            if instr.rd != 0:
                instr.def_regs.add(instr.rd)

    # Branch instructions
    elif opcode in [0x04, 0x05]:  # beq, bne
        if instr.rs != 0:
            instr.use_regs.add(instr.rs)
        if instr.rt != 0:
            instr.use_regs.add(instr.rt)
    elif opcode in [0x06, 0x07]:  # blez, bgtz
        if instr.rs != 0:
            instr.use_regs.add(instr.rs)
    elif opcode == 0x01:  # bcond (bltz, bgez, etc.)
        if instr.rs != 0:
            instr.use_regs.add(instr.rs)

    # Jump instructions
    elif opcode in [0x02, 0x03]:  # j, jal
        if opcode == 0x03:  # jal
            instr.def_regs.add(31)  # $ra

    # Load instructions (I-type: rt is destination)
    elif opcode in [0x20, 0x21, 0x23, 0x24, 0x25]:  # lb, lh, lw, lbu, lhu
        if instr.rs != 0:
            instr.use_regs.add(instr.rs)
        if instr.rt != 0:
            instr.def_regs.add(instr.rt)

    # Store instructions (I-type: rt is source)
    elif opcode in [0x28, 0x29, 0x2b]:  # sb, sh, sw
        if instr.rs != 0:
            instr.use_regs.add(instr.rs)
        if instr.rt != 0:
            instr.use_regs.add(instr.rt)

    # I-type arithmetic/logical (rt is destination)
    elif opcode in [0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e]:  # addi, addiu, slti, sltiu, andi, ori, xori
        if instr.rs != 0:
            instr.use_regs.add(instr.rs)
        if instr.rt != 0:
            instr.def_regs.add(instr.rt)

    # lui (loads upper immediate into rt)
    elif opcode == 0x0f:
        if instr.rt != 0:
            instr.def_regs.add(instr.rt)

def build_basic_blocks(instructions: List[Instruction]) -> List[BasicBlock]:
    """Build basic blocks from instruction list."""
    if not instructions:
        return []

    # Identify block boundaries (leaders)
    leaders = {0}  # First instruction is always a leader

    for i, instr in enumerate(instructions):
        # Instruction after a branch/jump is a leader
        if instr.is_branch() or instr.is_jump():
            if i + 2 < len(instructions):  # +2 because of delay slot
                leaders.add(instructions[i + 2].offset)

        # Branch target is a leader
        if instr.is_branch():
            target = instr.branch_target(instr.offset)
            # Find instruction at target offset
            for target_instr in instructions:
                if target_instr.offset == target:
                    leaders.add(target)
                    break

    # Sort leaders
    leaders_list = sorted(leaders)

    # Create basic blocks
    blocks = []
    for i, start in enumerate(leaders_list):
        # Find end of block (next leader - 1, or end of function)
        if i + 1 < len(leaders_list):
            end = leaders_list[i + 1] - 4
        else:
            end = instructions[-1].offset

        # Collect instructions in this block
        block_instrs = [instr for instr in instructions if start <= instr.offset <= end]

        if block_instrs:
            block = BasicBlock(
                start_offset=start,
                end_offset=end,
                instructions=block_instrs
            )
            blocks.append(block)

    # Build CFG edges
    for block in blocks:
        last_instr = block.instructions[-1]

        # Handle delay slot: the instruction in delay slot always executes
        delay_slot_instr = None
        if len(block.instructions) >= 2:
            if block.instructions[-2].is_branch() or block.instructions[-2].is_jump():
                delay_slot_instr = block.instructions[-1]
                last_instr = block.instructions[-2]

        # Add successor edges
        if last_instr.is_branch():
            # Branch has two successors: taken and fall-through
            target = last_instr.branch_target(last_instr.offset)

            # Add taken branch target
            for succ in blocks:
                if succ.start_offset == target:
                    block.successors.append(succ.start_offset)
                    succ.predecessors.append(block.start_offset)
                    break

            # Add fall-through (instruction after delay slot)
            fall_through = last_instr.offset + 8  # +4 for branch, +4 for delay slot
            for succ in blocks:
                if succ.start_offset == fall_through:
                    block.successors.append(succ.start_offset)
                    succ.predecessors.append(block.start_offset)
                    break

        elif last_instr.is_jump() and last_instr.opcode in [0x02, 0x03]:
            # Unconditional jump (j, jal) - successor is jump target
            # Note: We can't statically determine target for j/jal (26-bit immediate)
            # so we assume it's an exit
            pass

        elif last_instr.opcode == 0 and last_instr.funct == 0x08:
            # jr - indirect jump, assume it's a return (exit)
            pass

        else:
            # Fall-through to next block
            fall_through = block.end_offset + 4
            for succ in blocks:
                if succ.start_offset == fall_through:
                    block.successors.append(succ.start_offset)
                    succ.predecessors.append(block.start_offset)
                    break

    return blocks

def compute_liveness(blocks: List[BasicBlock]) -> None:
    """
    Compute liveness information using iterative dataflow analysis.

    Live-out of a block = Union of live-in of all successors
    Live-in of a block = Use ∪ (Live-out - Def)
    """
    changed = True
    iterations = 0
    max_iterations = 100

    while changed and iterations < max_iterations:
        changed = False
        iterations += 1

        # Process blocks in reverse postorder for faster convergence
        for block in reversed(blocks):
            # Compute live-out: union of successors' live-in
            new_live_out = set()
            for succ_offset in block.successors:
                for succ_block in blocks:
                    if succ_block.start_offset == succ_offset:
                        new_live_out.update(succ_block.live_in)
                        break

            # Compute live-in: use ∪ (live-out - def)
            new_live_in = set()
            live = new_live_out.copy()

            # Process instructions in reverse order
            for instr in reversed(block.instructions):
                instr.live_out = live.copy()

                # Remove defined registers
                live -= instr.def_regs

                # Add used registers
                live.update(instr.use_regs)

                instr.live_in = live.copy()

            new_live_in = live

            # Check if anything changed
            if new_live_in != block.live_in or new_live_out != block.live_out:
                changed = True
                block.live_in = new_live_in
                block.live_out = new_live_out

def compute_live_ranges(instructions: List[Instruction]) -> Dict[int, List[LiveRange]]:
    """
    Compute live ranges for each register.

    Returns: dict mapping register number to list of LiveRange objects
    """
    live_ranges = defaultdict(list)

    # For each register, find contiguous ranges where it's live
    for reg in range(32):
        if reg == 0:  # $zero is never truly live
            continue

        current_range = None

        for instr in instructions:
            is_live = reg in instr.live_in or reg in instr.live_out or reg in instr.def_regs or reg in instr.use_regs

            if is_live:
                if current_range is None:
                    # Start new range
                    current_range = LiveRange(
                        reg=reg,
                        start_offset=instr.offset,
                        end_offset=instr.offset,
                        instruction_offsets={instr.offset}
                    )
                else:
                    # Extend current range
                    current_range.end_offset = instr.offset
                    current_range.instruction_offsets.add(instr.offset)
            else:
                if current_range is not None:
                    # End current range
                    live_ranges[reg].append(current_range)
                    current_range = None

        # Don't forget to add the last range if it's still open
        if current_range is not None:
            live_ranges[reg].append(current_range)

    return live_ranges

def find_safe_renames(target_instrs: List[Instruction],
                      compiled_instrs: List[Instruction],
                      target_ranges: Dict[int, List[LiveRange]],
                      compiled_ranges: Dict[int, List[LiveRange]]) -> List[Tuple[int, int, Set[int]]]:
    """
    Find safe register renames by comparing instruction differences and checking liveness.

    Returns: list of (from_reg, to_reg, instruction_offsets) tuples where rename is safe
    """
    # First, identify instruction-level register differences
    reg_mismatches = defaultdict(list)  # (from_reg, to_reg) -> [instruction_offsets]

    for i, (target_instr, compiled_instr) in enumerate(zip(target_instrs, compiled_instrs)):
        if target_instr.word == compiled_instr.word:
            continue

        # Check if only register fields differ
        if target_instr.opcode != compiled_instr.opcode:
            continue
        if target_instr.opcode == 0 and target_instr.funct != compiled_instr.funct:
            continue

        # Check rs difference
        if target_instr.rs != compiled_instr.rs:
            pair = (compiled_instr.rs, target_instr.rs)
            reg_mismatches[pair].append(compiled_instr.offset)

        # Check rt difference
        if target_instr.rt != compiled_instr.rt:
            pair = (compiled_instr.rt, target_instr.rt)
            reg_mismatches[pair].append(compiled_instr.offset)

        # Check rd difference (R-type only)
        if target_instr.opcode == 0 and target_instr.rd != compiled_instr.rd:
            pair = (compiled_instr.rd, target_instr.rd)
            reg_mismatches[pair].append(compiled_instr.offset)

    # Now check which renames are safe (non-overlapping live ranges)
    safe_renames = []

    for (from_reg, to_reg), offsets in reg_mismatches.items():
        if from_reg == 0 or to_reg == 0:  # Never rename $zero
            continue

        # Get live ranges for both registers in compiled code
        from_ranges = compiled_ranges.get(from_reg, [])
        to_ranges = compiled_ranges.get(to_reg, [])

        # Check if the ranges overlap at the mismatch points
        overlaps = False
        for offset in offsets:
            # Find which range contains this offset for each register
            from_range_at_offset = None
            to_range_at_offset = None

            for lr in from_ranges:
                if offset in lr.instruction_offsets:
                    from_range_at_offset = lr
                    break

            for lr in to_ranges:
                if offset in lr.instruction_offsets:
                    to_range_at_offset = lr
                    break

            # If both registers are live at this point and their ranges overlap, unsafe
            if from_range_at_offset and to_range_at_offset:
                if from_range_at_offset.overlaps(to_range_at_offset):
                    overlaps = True
                    break

        if not overlaps:
            safe_renames.append((from_reg, to_reg, set(offsets)))

    return safe_renames

def apply_selective_renames(input_path: str, output_path: str,
                            renames: List[Tuple[int, int, Set[int]]],
                            verbose: bool = False) -> int:
    """
    Apply selective register renames (only at specified instruction offsets).

    Args:
        input_path: Input ELF .o file
        output_path: Output ELF .o file
        renames: List of (from_reg, to_reg, instruction_offsets) tuples
        verbose: Print detailed output

    Returns:
        Number of instructions modified
    """
    shutil.copy(input_path, output_path)

    with open(output_path, 'r+b') as f:
        # Read ELF header
        magic = f.read(4)
        if magic != b'\x7fELF':
            raise ValueError(f"{input_path} is not an ELF file")

        # Check endianness
        f.seek(5)
        ei_data = f.read(1)[0]
        big_endian = (ei_data == 2)
        endian_fmt = '>' if big_endian else '<'

        # Get section header info
        f.seek(32)
        e_shoff = struct.unpack(endian_fmt + 'I', f.read(4))[0]
        f.seek(46)
        e_shentsize = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shnum = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shstrndx = struct.unpack(endian_fmt + 'H', f.read(2))[0]

        # Find .shstrtab section
        f.seek(e_shoff + e_shstrndx * e_shentsize + 16)
        shstrtab_offset = struct.unpack(endian_fmt + 'I', f.read(4))[0]

        # Find .text section
        text_offset = None
        text_size = None

        for i in range(e_shnum):
            f.seek(e_shoff + i * e_shentsize)
            sh_name_idx = struct.unpack(endian_fmt + 'I', f.read(4))[0]

            # Read section name
            pos = f.tell()
            f.seek(shstrtab_offset + sh_name_idx)
            name = b''
            while True:
                c = f.read(1)
                if c == b'\x00' or not c:
                    break
                name += c
            f.seek(pos)

            if name == b'.text':
                f.seek(e_shoff + i * e_shentsize + 16)
                text_offset = struct.unpack(endian_fmt + 'I', f.read(4))[0]
                text_size = struct.unpack(endian_fmt + 'I', f.read(4))[0]
                break

        if text_offset is None:
            raise ValueError(".text section not found")

        if verbose:
            print(f"Found .text at offset {text_offset}, size {text_size}")

        # Build a set of (offset, from_reg, to_reg) for fast lookup
        rename_map = {}  # offset -> [(from_reg, to_reg)]
        for from_reg, to_reg, offsets in renames:
            for offset in offsets:
                if offset not in rename_map:
                    rename_map[offset] = []
                rename_map[offset].append((from_reg, to_reg))

        # Process instructions
        total_changes = 0
        f.seek(text_offset)
        for i in range(0, text_size, 4):
            offset = i
            pos = text_offset + i
            f.seek(pos)
            instr_bytes = f.read(4)
            if len(instr_bytes) < 4:
                break

            instr = struct.unpack(endian_fmt + 'I', instr_bytes)[0]

            # Check if this instruction needs renaming
            if offset not in rename_map:
                continue

            new_instr = instr
            changed = False

            for from_reg, to_reg in rename_map[offset]:
                # Apply rename
                opcode = (instr >> 26) & 0x3F
                rs = (instr >> 21) & 0x1F
                rt = (instr >> 16) & 0x1F
                rd = (instr >> 11) & 0x1F

                # Replace rs
                if rs == from_reg:
                    new_instr = (new_instr & ~(0x1F << 21)) | (to_reg << 21)
                    changed = True

                # Replace rt
                if rt == from_reg:
                    new_instr = (new_instr & ~(0x1F << 16)) | (to_reg << 16)
                    changed = True

                # Replace rd (only for R-type instructions)
                if opcode == 0 and rd == from_reg:
                    new_instr = (new_instr & ~(0x1F << 11)) | (to_reg << 11)
                    changed = True

            if changed:
                if verbose:
                    print(f"  Offset {offset:04x}: {instr:08x} -> {new_instr:08x}")
                f.seek(pos)
                f.write(struct.pack(endian_fmt + 'I', new_instr))
                total_changes += 1

        if verbose or total_changes > 0:
            for from_reg, to_reg, offsets in renames:
                from_name = REG_NAMES.get(from_reg, str(from_reg))
                to_name = REG_NAMES.get(to_reg, str(to_reg))
                print(f"Applied rename: ${from_name} -> ${to_name} at {len(offsets)} location(s)")
            print(f"Total instructions modified: {total_changes}")

    return total_changes

def extract_function_from_exe(exe_path: str, vram_addr: int, size: int = None) -> bytes:
    """Extract function bytes from executable."""
    TEXT_SECTION_START = 0x80041000
    FILE_OFFSET_BASE = 0x800

    file_offset = (vram_addr - TEXT_SECTION_START) + FILE_OFFSET_BASE

    with open(exe_path, 'rb') as f:
        f.seek(file_offset)
        if size is None:
            # Read until we find a jr ra (return)
            data = bytearray()
            while len(data) < 4096:  # Safety limit
                chunk = f.read(4)
                if not chunk:
                    break
                data.extend(chunk)
                word = struct.unpack('<I', chunk)[0]
                # Check for jr ra (0x03e00008)
                if word == 0x03e00008:
                    # Read one more instruction (delay slot)
                    chunk = f.read(4)
                    if chunk:
                        data.extend(chunk)
                    break
            return bytes(data)
        else:
            return f.read(size)

def extract_text_section_from_elf(elf_path: str) -> Tuple[bytes, bool]:
    """
    Extract .text section from ELF object file.

    Returns (text_data, big_endian)
    """
    with open(elf_path, 'rb') as f:
        # Read ELF header
        magic = f.read(4)
        if magic != b'\x7fELF':
            raise ValueError(f"{elf_path} is not an ELF file")

        # Check endianness
        f.seek(5)
        ei_data = f.read(1)[0]
        big_endian = (ei_data == 2)
        endian_fmt = '>' if big_endian else '<'

        # Get section header info
        f.seek(32)
        e_shoff = struct.unpack(endian_fmt + 'I', f.read(4))[0]
        f.seek(46)
        e_shentsize = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shnum = struct.unpack(endian_fmt + 'H', f.read(2))[0]
        e_shstrndx = struct.unpack(endian_fmt + 'H', f.read(2))[0]

        # Find .shstrtab section
        f.seek(e_shoff + e_shstrndx * e_shentsize + 16)
        shstrtab_offset = struct.unpack(endian_fmt + 'I', f.read(4))[0]

        # Find .text section
        for i in range(e_shnum):
            f.seek(e_shoff + i * e_shentsize)
            sh_name_idx = struct.unpack(endian_fmt + 'I', f.read(4))[0]

            # Read section name
            pos = f.tell()
            f.seek(shstrtab_offset + sh_name_idx)
            name = b''
            while True:
                c = f.read(1)
                if c == b'\x00' or not c:
                    break
                name += c
            f.seek(pos)

            if name == b'.text':
                f.seek(e_shoff + i * e_shentsize + 16)
                text_offset = struct.unpack(endian_fmt + 'I', f.read(4))[0]
                text_size = struct.unpack(endian_fmt + 'I', f.read(4))[0]

                f.seek(text_offset)
                return f.read(text_size), big_endian

        raise ValueError(".text section not found")

def generate_liveness_report(blocks: List[BasicBlock],
                             live_ranges: Dict[int, List[LiveRange]],
                             safe_renames: List[Tuple[int, int, Set[int]]]) -> str:
    """Generate a detailed liveness analysis report."""
    lines = []
    lines.append("=" * 80)
    lines.append("LIVENESS-AWARE REGISTER RENAMING REPORT")
    lines.append("=" * 80)
    lines.append("")

    # Basic block summary
    lines.append(f"Basic Blocks: {len(blocks)}")
    lines.append("")

    # Live range summary
    lines.append("-" * 80)
    lines.append("LIVE RANGES:")
    lines.append("-" * 80)
    for reg in sorted(live_ranges.keys()):
        ranges = live_ranges[reg]
        if not ranges:
            continue
        reg_name = REG_NAMES.get(reg, str(reg))
        lines.append(f"  ${reg_name}: {len(ranges)} range(s)")
        for i, lr in enumerate(ranges):
            lines.append(f"    Range {i+1}: offset {lr.start_offset:04x} - {lr.end_offset:04x} ({len(lr.instruction_offsets)} instructions)")
    lines.append("")

    # Safe renames
    if safe_renames:
        lines.append("-" * 80)
        lines.append("SAFE RENAMES IDENTIFIED:")
        lines.append("-" * 80)
        for from_reg, to_reg, offsets in safe_renames:
            from_name = REG_NAMES.get(from_reg, str(from_reg))
            to_name = REG_NAMES.get(to_reg, str(to_reg))
            lines.append(f"  ${from_name} -> ${to_name} at {len(offsets)} location(s):")
            for offset in sorted(offsets):
                lines.append(f"    0x{offset:04x}")
        lines.append("")
    else:
        lines.append("No safe renames identified (all potential renames have overlapping live ranges)")
        lines.append("")

    return '\n'.join(lines)

def main():
    parser = argparse.ArgumentParser(
        description='Liveness-aware register renaming for matching decompiled code',
        epilog="""
Examples:
    # Automatic detection with liveness analysis
    python3 swap_registers_liveness.py input.o output.o \\
        --target-exe disks/SLPS_000.65 --address 0x8006CF20

    # Report only (show liveness info)
    python3 swap_registers_liveness.py input.o output.o \\
        --target-exe disks/SLPS_000.65 --address 0x8006CF20 --report-only

Test function:
    icon_schedule_set (0x8006CF20) - known s1<->s2 swap plus temp register differences
        """,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )

    parser.add_argument('input', help='Input .o file (compiled)')
    parser.add_argument('output', help='Output .o file (fixed)')
    parser.add_argument('--target-exe', required=True, help='Target executable (e.g., disks/SLPS_000.65)')
    parser.add_argument('--address', required=True, help='VRAM address of function (e.g., 0x8006CF20)')
    parser.add_argument('--size', type=int, help='Function size in bytes (auto-detect if not specified)')
    parser.add_argument('--report-only', action='store_true', help='Generate report without modifying files')
    parser.add_argument('-v', '--verbose', action='store_true', help='Verbose output')

    args = parser.parse_args()

    vram_addr = int(args.address, 16)

    if args.verbose:
        print(f"Liveness analysis mode:")
        print(f"  Target: {args.target_exe}")
        print(f"  Address: 0x{vram_addr:08X}")
        print()

    # Extract target function
    target_data = extract_function_from_exe(args.target_exe, vram_addr, args.size)
    target_instrs = disassemble_bytes(target_data, vram_addr)

    # Extract compiled function
    compiled_data, big_endian = extract_text_section_from_elf(args.input)
    compiled_instrs = disassemble_bytes(compiled_data, 0)

    if args.verbose:
        print(f"Target instructions: {len(target_instrs)}")
        print(f"Compiled instructions: {len(compiled_instrs)}")
        print()

    # Build CFG and compute liveness for both
    if args.verbose:
        print("Building control flow graph...")

    target_blocks = build_basic_blocks(target_instrs)
    compiled_blocks = build_basic_blocks(compiled_instrs)

    if args.verbose:
        print(f"Target basic blocks: {len(target_blocks)}")
        print(f"Compiled basic blocks: {len(compiled_blocks)}")
        print()
        print("Computing liveness...")

    compute_liveness(target_blocks)
    compute_liveness(compiled_blocks)

    if args.verbose:
        print("Computing live ranges...")

    target_ranges = compute_live_ranges(target_instrs)
    compiled_ranges = compute_live_ranges(compiled_instrs)

    if args.verbose:
        print("Finding safe renames...")
        print()

    safe_renames = find_safe_renames(target_instrs, compiled_instrs, target_ranges, compiled_ranges)

    # Generate report
    report = generate_liveness_report(compiled_blocks, compiled_ranges, safe_renames)
    print(report)

    if safe_renames and not args.report_only:
        print("Applying safe renames...")
        changes = apply_selective_renames(args.input, args.output, safe_renames, args.verbose)
        print(f"Modified {changes} instruction(s)")
        print()

        # Verify result
        fixed_data, _ = extract_text_section_from_elf(args.output)
        fixed_instrs = disassemble_bytes(fixed_data, 0)

        # Compare
        matches = sum(1 for t, f in zip(target_instrs, fixed_instrs) if t.word == f.word)
        total = min(len(target_instrs), len(fixed_instrs))

        if matches == total:
            print("✓ PERFECT MATCH achieved!")
        else:
            print(f"Match rate: {matches}/{total} ({100*matches/total:.1f}%)")
            print(f"Remaining differences: {total - matches}")

    elif args.report_only:
        print("(Report-only mode: no modifications made)")
    else:
        # A PostProc chain step must always produce its output: the build
        # driver (build_level3_c.apply_postproc) advances to `output` on
        # exit status 0, so returning without writing it made the NEXT step
        # fail on a missing file and the whole compile abort (func_80074E98,
        # 2026-09-24: "objcopy: func_step2.o: No such file", reported as a
        # strip_trailing_instructions failure).  No safe rename = unchanged.
        shutil.copyfile(args.input, args.output)
        print("No safe renames found - manual intervention required "
              "(output written unchanged)")

if __name__ == '__main__':
    main()
