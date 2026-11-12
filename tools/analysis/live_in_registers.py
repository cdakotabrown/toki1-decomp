#!/usr/bin/env python3
"""
Live-In Register Analysis Tool for Tokimeki Memorial PS1 Decompilation

Performs forward dataflow analysis on MIPS assembly files to determine which
registers are "live-in" -- read before they are written on any execution path
from function entry.

Focuses on the GCC region (0x80041000-0x80085FFF) where 345+ functions exhibit
unusual "hidden register parameter" patterns with registers like t6, t7, t8,
t9, $at, v0, v1 being read before written.

Analysis approach:
  1. Parse .s files and build a control flow graph (basic blocks + edges)
  2. For each basic block, compute GEN (first-read registers) and KILL (written
     registers before any read)
  3. Propagate live-in sets through the CFG from entry block
  4. A register is live-in to the function if it is live-in to the entry block

Output includes:
  - Per-function live-in register sets
  - Cluster analysis grouping functions by live-in signature
  - Frequency statistics for individual registers and combinations
  - Cross-reference with existing function_classification.json

Usage:
    python3 tools/analysis/live_in_registers.py
    python3 tools/analysis/live_in_registers.py --output build/analysis/live_in_report.txt
    python3 tools/analysis/live_in_registers.py --json build/analysis/live_in_data.json
    python3 tools/analysis/live_in_registers.py --region IDO
"""

import os
import re
import sys
import json
import argparse
from pathlib import Path
from collections import defaultdict, Counter

# Project root
PROJECT_ROOT = Path(__file__).resolve().parent.parent.parent

# Constants
ASM_DIR = PROJECT_ROOT / "asm" / "main" / "matched"
CONFIG_DIR = PROJECT_ROOT / "config"

BASE_ADDR = 0x80041000
GCC_REGION_END = 0x80085FFF
IDO_REGION_START = 0x80086000
CODE_END = 0x800C0000

# Branch/jump instructions
BRANCH_OPS = {
    'beq', 'bne', 'bgez', 'bgtz', 'blez', 'bltz',
    'bltzal', 'bgezal', 'bc1f', 'bc1t', 'bc0f', 'bc0t',
    'bnez', 'beqz',
}
JUMP_OPS = {'j', 'jal', 'jr', 'jalr'}
UNCONDITIONAL_BRANCHES = {'j', 'jr', 'b'}

# Instructions that write their first operand
WRITE_FIRST_OPS = {
    'lui', 'li', 'la', 'move',
    'addiu', 'addi', 'add', 'addu', 'sub', 'subu',
    'and', 'andi', 'or', 'ori', 'xor', 'xori', 'nor',
    'sll', 'srl', 'sra', 'sllv', 'srlv', 'srav',
    'slt', 'slti', 'sltu', 'sltiu',
    'lb', 'lbu', 'lh', 'lhu', 'lw', 'lwl', 'lwr',
    'mfhi', 'mflo', 'mfc0', 'mfc2', 'cfc2',
}

# Store instructions (read first operand + base)
STORE_OPS = {'sb', 'sh', 'sw', 'swl', 'swr', 'swc2'}

# Interesting registers (not including a0-a3 which are expected args)
INTERESTING_REGS = {
    'at', 'v0', 'v1',
    't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7', 't8', 't9',
}

# All expected live-in registers that are normal
EXPECTED_LIVEIN = {'a0', 'a1', 'a2', 'a3', 'sp', 'ra', 'gp', 'zero'}


def normalize_reg(reg):
    """Normalize register name to standard form."""
    if not reg:
        return None
    reg = reg.strip().lstrip('$')

    num_map = {
        '0': 'zero', '1': 'at', '2': 'v0', '3': 'v1',
        '4': 'a0', '5': 'a1', '6': 'a2', '7': 'a3',
        '8': 't0', '9': 't1', '10': 't2', '11': 't3',
        '12': 't4', '13': 't5', '14': 't6', '15': 't7',
        '16': 's0', '17': 's1', '18': 's2', '19': 's3',
        '20': 's4', '21': 's5', '22': 's6', '23': 's7',
        '24': 't8', '25': 't9', '26': 'k0', '27': 'k1',
        '28': 'gp', '29': 'sp', '30': 'fp', '31': 'ra',
    }
    if reg in num_map:
        return num_map[reg]

    valid = {
        'zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
        't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
        's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
        't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra',
    }
    if reg in valid:
        return reg
    return None


def parse_instruction(line):
    """Parse a single assembly instruction line into (op, operands_list).

    Each operand is either:
      ('reg_or_imm', value_string)
      ('mem', offset_string, base_reg_string)
    """
    # Strip comments (both # and /* */ styles)
    line = re.sub(r'#.*$', '', line).strip()
    line = re.sub(r'/\*.*?\*/', '', line).strip()
    if not line:
        return None, []

    match = re.match(r'^([a-z]\w*)\s*(.*)', line, re.I)
    if not match:
        return None, []

    op = match.group(1).lower()
    args_str = match.group(2).strip()

    operands = []
    if args_str:
        parts = re.split(r',\s*', args_str)
        for part in parts:
            part = part.strip()
            mem_match = re.match(r'(-?(?:0x[\da-fA-F]+|\d+)|\w+)\s*\((\$?\w+)\)', part)
            if mem_match:
                operands.append(('mem', mem_match.group(1), mem_match.group(2)))
            else:
                operands.append(('reg_or_imm', part))

    return op, operands


def get_read_write_regs(op, operands):
    """Determine which registers are read and written by an instruction.

    Returns (reads, writes) as sets of normalized register names.
    """
    reads = set()
    writes = set()

    if not op or op == 'nop':
        return reads, writes

    # Syscall, break
    if op in ('syscall', 'break', 'rfe', 'eret', 'sync'):
        return reads, writes

    # j target - no register interaction
    if op == 'j' or op == 'b':
        return reads, writes

    # jal target - writes ra
    if op == 'jal':
        writes.add('ra')
        return reads, writes

    # jr rs
    if op == 'jr':
        if operands:
            r = _get_reg(operands[0])
            if r:
                reads.add(r)
        return reads, writes

    # jalr [rd,] rs
    if op == 'jalr':
        if len(operands) >= 2:
            rd = _get_reg(operands[0])
            rs = _get_reg(operands[1])
            if rd:
                writes.add(rd)
            if rs:
                reads.add(rs)
        elif len(operands) == 1:
            rs = _get_reg(operands[0])
            if rs:
                reads.add(rs)
            writes.add('ra')
        return reads, writes

    # Store instructions: read value register and base register
    if op in STORE_OPS:
        if len(operands) >= 2:
            rt = _get_reg(operands[0])
            if rt:
                reads.add(rt)
            if operands[1][0] == 'mem':
                base = normalize_reg(operands[1][2])
                if base:
                    reads.add(base)
        return reads, writes

    # Load instructions: write target, read base
    if op in ('lb', 'lbu', 'lh', 'lhu', 'lw', 'lwl', 'lwr', 'lwc2'):
        if len(operands) >= 2:
            rt = _get_reg(operands[0])
            if rt:
                writes.add(rt)
            if operands[1][0] == 'mem':
                base = normalize_reg(operands[1][2])
                if base:
                    reads.add(base)
        return reads, writes

    # lui: writes rt only (immediate value, no register read)
    if op == 'lui':
        if operands:
            rt = _get_reg(operands[0])
            if rt:
                writes.add(rt)
        return reads, writes

    # li/la: writes rd only
    if op in ('li', 'la'):
        if operands:
            rd = _get_reg(operands[0])
            if rd:
                writes.add(rd)
        return reads, writes

    # move: writes rd, reads rs
    if op == 'move':
        if len(operands) >= 2:
            rd = _get_reg(operands[0])
            rs = _get_reg(operands[1])
            if rd:
                writes.add(rd)
            if rs:
                reads.add(rs)
        return reads, writes

    # mfhi/mflo: writes rd (reads HI/LO, not GPR)
    if op in ('mfhi', 'mflo'):
        if operands:
            rd = _get_reg(operands[0])
            if rd:
                writes.add(rd)
        return reads, writes

    # mthi/mtlo: reads rs (writes HI/LO, not GPR)
    if op in ('mthi', 'mtlo'):
        if operands:
            rs = _get_reg(operands[0])
            if rs:
                reads.add(rs)
        return reads, writes

    # mfc0/mfc2/cfc2: writes rt (reads coprocessor, not GPR)
    if op in ('mfc0', 'mfc2', 'cfc2'):
        if operands:
            rt = _get_reg(operands[0])
            if rt:
                writes.add(rt)
        return reads, writes

    # mtc0/mtc2/ctc2: reads rt (writes coprocessor)
    if op in ('mtc0', 'mtc2', 'ctc2'):
        if operands:
            rt = _get_reg(operands[0])
            if rt:
                reads.add(rt)
        return reads, writes

    # mult/multu/div/divu: reads rs, rt (writes HI/LO)
    if op in ('mult', 'multu', 'div', 'divu'):
        for i in range(min(2, len(operands))):
            r = _get_reg(operands[i])
            if r:
                reads.add(r)
        return reads, writes

    # Branch instructions: read rs (and rt for beq/bne)
    if op in BRANCH_OPS:
        if operands:
            rs = _get_reg(operands[0])
            if rs:
                reads.add(rs)
            if op in ('beq', 'bne') and len(operands) >= 2:
                rt = _get_reg(operands[1])
                if rt:
                    reads.add(rt)
        return reads, writes

    # ALU R-type and I-type: write first operand, read remaining
    if op in WRITE_FIRST_OPS:
        if operands:
            rd = _get_reg(operands[0])
            if rd:
                writes.add(rd)
            for i in range(1, len(operands)):
                if operands[i][0] == 'reg_or_imm':
                    r = normalize_reg(operands[i][1])
                    if r:
                        reads.add(r)
                elif operands[i][0] == 'mem':
                    base = normalize_reg(operands[i][2])
                    if base:
                        reads.add(base)
        return reads, writes

    # Fallback: treat all register-like operands as reads
    for operand in operands:
        if operand[0] == 'reg_or_imm':
            r = normalize_reg(operand[1])
            if r:
                reads.add(r)
        elif operand[0] == 'mem':
            base = normalize_reg(operand[2])
            if base:
                reads.add(base)

    return reads, writes


def _get_reg(operand):
    """Extract normalized register from an operand tuple."""
    if operand[0] == 'reg_or_imm':
        return normalize_reg(operand[1])
    return None


def extract_address_from_file(filepath):
    """Extract the function address from an ASM file.

    Checks multiple sources:
      1. func_XXXXXXXX in filename
      2. Address comment in header (# Address: 0xXXXXXXXX)
      3. VRAM comment in header (# VRAM: 0xXXXXXXXX - ...)
      4. Address from instruction comments (/* XXXXX XXXXXXXX ... */)
      5. Function label (glabel/globl func_XXXXXXXX)
    """
    filename = filepath.name

    # Try filename first
    m = re.search(r'func_(8[0-9a-fA-F]{7})', filename)
    if m:
        return int(m.group(1), 16)

    # Read file header to find address
    try:
        with open(filepath, 'r') as f:
            header = f.read(2000)
    except Exception:
        return None

    # Try header comments
    m = re.search(r'Address:\s*0x(8[0-9a-fA-F]{7})', header)
    if m:
        return int(m.group(1), 16)

    m = re.search(r'VRAM:\s*0x(8[0-9a-fA-F]{7})', header)
    if m:
        return int(m.group(1), 16)

    # Try function label
    m = re.search(r'(?:glabel|\.globl|\.global)\s+func_(8[0-9a-fA-F]{7})', header)
    if m:
        return int(m.group(1), 16)

    # Try instruction address comments
    m = re.search(r'/\*\s*[0-9a-fA-F]+\s+(8[0-9a-fA-F]{7})\s+', header)
    if m:
        return int(m.group(1), 16)

    # Try Function: comment
    m = re.search(r'Function:\s*func_(8[0-9a-fA-F]{7})', header)
    if m:
        return int(m.group(1), 16)

    return None


def get_region(addr):
    """Determine compiler region from address."""
    if BASE_ADDR <= addr <= GCC_REGION_END:
        return "GCC"
    elif IDO_REGION_START <= addr < CODE_END:
        return "IDO"
    return "unknown"


class BasicBlock:
    """A basic block of instructions."""
    __slots__ = ['id', 'instructions', 'successors', 'predecessors',
                 'gen', 'kill', 'live_in']

    def __init__(self, block_id):
        self.id = block_id
        self.instructions = []  # list of (op, operands, line_text)
        self.successors = []    # block IDs
        self.predecessors = []  # block IDs
        self.gen = set()        # registers read before written in this block
        self.kill = set()       # registers written in this block
        self.live_in = set()    # live-in registers at block entry


def parse_asm_file(filepath):
    """Parse an ASM file and return a list of (label_or_none, op, operands, raw_line)
    tuples for the function body.

    Returns (func_name, instructions_list) where each instruction has:
      (address_or_none, label_or_none, op, operands, is_delay_slot_of_branch)
    """
    try:
        with open(filepath, 'r') as f:
            lines = f.readlines()
    except Exception:
        return None, []

    instructions = []
    func_name = None
    in_function = False
    found_first_label = False

    for line in lines:
        raw = line.rstrip()
        stripped = raw.strip()

        # Skip empty lines and directives (except labels)
        if not stripped:
            continue

        # Check for function label
        label_match = re.match(r'(?:glabel|\.globl|\.global)\s+(\w+)', stripped)
        if label_match and not found_first_label:
            func_name = label_match.group(1)
            continue

        # Detect labels (including local labels like .LXXXXXXXX or func_XXXXXXXX:)
        label = None
        lm = re.match(r'^(\.\w+|func_\w+|[A-Za-z_]\w*):\s*$', stripped)
        if lm:
            label = lm.group(1)
            instructions.append((None, label, None, None))
            continue

        # Check for label on same line as instruction (label: instruction)
        lm2 = re.match(r'^(\.\w+|func_\w+|[A-Za-z_]\w*):\s+(.+)$', stripped)
        if lm2:
            label = lm2.group(1)
            instructions.append((None, label, None, None))
            stripped = lm2.group(2).strip()

        # Skip directives
        if stripped.startswith('.'):
            continue
        if stripped.startswith('#'):
            continue
        if stripped.startswith('/*') and '*/' in stripped and not re.search(r'\*/\s*\w', stripped):
            continue

        # Extract address from comment if present
        addr = None
        addr_match = re.search(r'(?:#|/\*)\s*(?:[0-9a-fA-F]+\s+)?(8[0-9a-fA-F]{7})', stripped)
        if addr_match:
            try:
                addr = int(addr_match.group(1), 16)
            except ValueError:
                pass

        # Parse instruction from the line
        # Remove leading comment block if format is "/* offset addr hex */ instruction"
        instr_text = stripped
        cm = re.match(r'/\*.*?\*/\s*(.+)', instr_text)
        if cm:
            instr_text = cm.group(1).strip()

        # Also handle lines that start with the instruction directly
        op, operands = parse_instruction(instr_text)
        if op:
            if not found_first_label and func_name:
                found_first_label = True
            instructions.append((addr, None, op, operands))

    return func_name, instructions


def build_cfg(instructions):
    """Build a control flow graph from parsed instructions.

    Returns a list of BasicBlock objects with the entry block at index 0.
    """
    if not instructions:
        return []

    # First pass: identify basic block leaders
    # Leaders are:
    #   - First instruction
    #   - Target of any branch/jump
    #   - Instruction following a branch/jump (+ delay slot)
    leaders = set()
    leaders.add(0)

    # Map label names to instruction indices
    label_to_idx = {}
    # Filter to just real instructions and track label positions
    real_instructions = []
    label_positions = {}

    for item in instructions:
        addr, label, op, operands = item
        if label is not None and op is None:
            # This is a label; map it to the next real instruction index
            label_positions[label] = len(real_instructions)
        elif op is not None:
            real_instructions.append((addr, op, operands))

    if not real_instructions:
        return []

    # Finalize label_to_idx
    label_to_idx = label_positions

    # Second pass: find branch targets and mark leaders
    i = 0
    while i < len(real_instructions):
        addr, op, operands = real_instructions[i]

        is_branch = op in BRANCH_OPS
        is_jump = op in ('j', 'b')
        is_jr = op == 'jr'
        is_jal = op == 'jal'
        is_jalr = op == 'jalr'

        if is_branch or is_jump:
            # The delay slot is the next instruction
            delay_slot_idx = i + 1 if i + 1 < len(real_instructions) else None

            # Find branch target
            target_label = None
            for operand in operands:
                if operand[0] == 'reg_or_imm':
                    val = operand[1]
                    # Check if it's a label reference
                    if val.startswith('.') or val.startswith('func_') or re.match(r'^[A-Za-z_]', val):
                        target_label = val
                    elif val.startswith('0x') or val.isdigit():
                        # Numeric target - try to find matching label
                        try:
                            target_addr = int(val, 0)
                            # Look for label at this address
                            for lbl, idx in label_to_idx.items():
                                if idx < len(real_instructions):
                                    iaddr = real_instructions[idx][0]
                                    if iaddr and iaddr == target_addr:
                                        target_label = lbl
                                        break
                        except ValueError:
                            pass

            if target_label and target_label in label_to_idx:
                target_idx = label_to_idx[target_label]
                leaders.add(target_idx)

            # Instruction after delay slot is a leader
            if delay_slot_idx is not None and delay_slot_idx + 1 < len(real_instructions):
                leaders.add(delay_slot_idx + 1)

        elif is_jr or is_jal or is_jalr:
            # After jal/jalr + delay slot, next instruction is a leader
            delay_slot_idx = i + 1 if i + 1 < len(real_instructions) else None
            if delay_slot_idx is not None and delay_slot_idx + 1 < len(real_instructions):
                if is_jal or is_jalr:
                    leaders.add(delay_slot_idx + 1)
                elif is_jr:
                    leaders.add(delay_slot_idx + 1)

        i += 1

    # Also mark labels as leaders
    for lbl, idx in label_to_idx.items():
        if idx < len(real_instructions):
            leaders.add(idx)

    # Build basic blocks
    sorted_leaders = sorted(leaders)
    blocks = []
    leader_to_block = {}

    for bi, leader_idx in enumerate(sorted_leaders):
        block = BasicBlock(bi)
        end_idx = sorted_leaders[bi + 1] if bi + 1 < len(sorted_leaders) else len(real_instructions)
        for ii in range(leader_idx, end_idx):
            if ii < len(real_instructions):
                addr, op, operands = real_instructions[ii]
                block.instructions.append((op, operands, ii))
        blocks.append(block)
        leader_to_block[leader_idx] = bi

    # Build edges (successors/predecessors)
    for bi, block in enumerate(blocks):
        if not block.instructions:
            continue

        last_idx = block.instructions[-1][2]  # instruction index
        last_op = block.instructions[-1][0]

        # Check second-to-last instruction for branch (delay slot handling)
        # In MIPS, the branch/jump is BEFORE its delay slot
        branch_op = None
        branch_operands = None
        if len(block.instructions) >= 2:
            second_last_op = block.instructions[-2][0]
            second_last_operands = block.instructions[-2][1]
            if second_last_op in BRANCH_OPS or second_last_op in ('j', 'b', 'jr', 'jal', 'jalr'):
                branch_op = second_last_op
                branch_operands = second_last_operands
        # Also check if the last instruction itself is a branch (no delay slot filled?)
        if branch_op is None:
            if last_op in BRANCH_OPS or last_op in ('j', 'b', 'jr', 'jal', 'jalr'):
                branch_op = last_op
                branch_operands = block.instructions[-1][1]

        is_return = False
        is_unconditional = False
        branch_target_block = None

        if branch_op:
            if branch_op == 'jr':
                # Check if it's jr $ra (return)
                if branch_operands:
                    r = _get_reg(branch_operands[0])
                    if r == 'ra':
                        is_return = True
                    else:
                        # Indirect jump - can't determine target
                        is_return = True  # Conservative: treat as return
                else:
                    is_return = True

            elif branch_op in ('j', 'b'):
                is_unconditional = True
                # Find target
                target_block = _find_branch_target(branch_operands, label_to_idx,
                                                   real_instructions, leader_to_block, sorted_leaders)
                if target_block is not None:
                    branch_target_block = target_block

            elif branch_op in ('jal', 'jalr'):
                # Function call - fall through after delay slot
                pass  # Fall through handled below

            elif branch_op in BRANCH_OPS:
                # Conditional branch
                target_block = _find_branch_target(branch_operands, label_to_idx,
                                                   real_instructions, leader_to_block, sorted_leaders)
                if target_block is not None:
                    branch_target_block = target_block

        # Add edges
        if branch_target_block is not None:
            block.successors.append(branch_target_block)
            blocks[branch_target_block].predecessors.append(bi)

        # Fall-through edge (unless unconditional jump/return)
        if not is_return and not is_unconditional:
            if bi + 1 < len(blocks):
                block.successors.append(bi + 1)
                blocks[bi + 1].predecessors.append(bi)
        elif is_unconditional and branch_target_block is None:
            # Unconditional jump to external target - no fall-through
            pass

    return blocks


def _find_branch_target(operands, label_to_idx, real_instructions, leader_to_block, sorted_leaders):
    """Find the basic block index for a branch target."""
    if not operands:
        return None

    for operand in operands:
        if operand[0] == 'reg_or_imm':
            val = operand[1]
            # Label reference
            if val in label_to_idx:
                target_instr_idx = label_to_idx[val]
                return _instr_idx_to_block(target_instr_idx, leader_to_block, sorted_leaders)

            # Try as hex address
            try:
                target_addr = int(val, 0)
                # Find instruction at this address
                for idx, (iaddr, iop, iops) in enumerate(real_instructions):
                    if iaddr and iaddr == target_addr:
                        return _instr_idx_to_block(idx, leader_to_block, sorted_leaders)
            except ValueError:
                pass

    return None


def _instr_idx_to_block(instr_idx, leader_to_block, sorted_leaders):
    """Map instruction index to block index."""
    if instr_idx in leader_to_block:
        return leader_to_block[instr_idx]
    # Find the block containing this instruction
    for i in range(len(sorted_leaders) - 1, -1, -1):
        if sorted_leaders[i] <= instr_idx:
            return leader_to_block.get(sorted_leaders[i])
    return None


def compute_block_gen_kill(block):
    """Compute GEN and KILL sets for a basic block.

    GEN = registers read before being written in this block
    KILL = registers written in this block (before or after read)
    """
    gen = set()
    kill = set()

    for op, operands, _ in block.instructions:
        reads, writes = get_read_write_regs(op, operands)

        # Remove 'zero' - it's always zero, not interesting
        reads.discard('zero')
        writes.discard('zero')

        # GEN: registers read that haven't been killed yet
        for r in reads:
            if r not in kill:
                gen.add(r)

        # KILL: registers written
        kill.update(writes)

    block.gen = gen
    block.kill = kill


def compute_live_in_cfg(blocks):
    """Compute live-in registers for the entry block using CFG dataflow.

    Uses a forward "first-use before def" analysis.
    For each block, live_in = gen U (union of predecessor live_out that reach through)

    Actually we want: a register is live-in to the function if there exists ANY path
    from function entry where the register is read before written.

    Simple approach: forward propagation of "not-yet-defined" registers.
    Start with all interesting regs as potentially live. For each path through the CFG,
    if a register is read before written, it's live-in.

    We compute this as:
      live_in(entry) = gen(entry) U { r in live_in(succ) | r not in kill(entry) }
    for all successors reachable.

    This is the standard "available definitions" problem inverted.
    """
    if not blocks:
        return set()

    # Compute gen/kill for each block
    for block in blocks:
        compute_block_gen_kill(block)

    # Forward dataflow: compute which registers are read-before-written
    # on any path from the entry block
    #
    # For each block b:
    #   exposed_use(b) = gen(b) U { r : r in exposed_use(succ) and r not in kill(b) }
    #
    # This propagates backwards from uses to the entry.
    # We use a worklist algorithm.

    changed = True
    iterations = 0
    max_iterations = 100

    # Initialize live_in for each block to its gen set
    for block in blocks:
        block.live_in = set(block.gen)

    while changed and iterations < max_iterations:
        changed = False
        iterations += 1

        # Process blocks in reverse order for faster convergence
        for block in reversed(blocks):
            old_live_in = set(block.live_in)

            # live_in = gen U (union of successor live_ins - kill)
            new_live_in = set(block.gen)
            for succ_id in block.successors:
                if 0 <= succ_id < len(blocks):
                    for r in blocks[succ_id].live_in:
                        if r not in block.kill:
                            new_live_in.add(r)

            block.live_in = new_live_in
            if block.live_in != old_live_in:
                changed = True

    # The live-in of the entry block (block 0) gives us function live-ins
    if blocks:
        return blocks[0].live_in
    return set()


def analyze_function_simple(instructions):
    """Simple linear scan analysis (fallback if CFG is too complex).

    For each register, check if it's read before written scanning linearly.
    This is conservative: it may miss some live-ins that only appear on
    branch-taken paths, but it's fast and handles most cases.
    """
    written = set()
    live_in = set()

    for item in instructions:
        addr, label, op, operands = item
        if op is None:
            continue

        reads, writes = get_read_write_regs(op, operands)
        reads.discard('zero')
        writes.discard('zero')

        for r in reads:
            if r not in written:
                live_in.add(r)

        written.update(writes)

    return live_in


def analyze_file(filepath, use_cfg=True):
    """Analyze a single ASM file for live-in registers.

    Returns dict with:
      'address': int
      'name': str
      'filepath': str
      'region': str
      'all_live_in': set of all live-in registers
      'interesting_live_in': set of non-standard live-in registers
      'standard_live_in': set of expected live-in registers (a0-a3, sp, ra, etc.)
    """
    address = extract_address_from_file(filepath)
    if address is None:
        return None

    region = get_region(address)

    func_name, instructions = parse_asm_file(filepath)
    if not instructions:
        return None

    if func_name is None:
        func_name = filepath.stem

    # Try CFG-based analysis first
    if use_cfg:
        try:
            blocks = build_cfg(instructions)
            if blocks:
                all_live_in = compute_live_in_cfg(blocks)
            else:
                all_live_in = analyze_function_simple(instructions)
        except Exception:
            all_live_in = analyze_function_simple(instructions)
    else:
        all_live_in = analyze_function_simple(instructions)

    # Categorize
    interesting = all_live_in & INTERESTING_REGS
    standard = all_live_in & EXPECTED_LIVEIN

    return {
        'address': address,
        'name': func_name,
        'filepath': str(filepath.relative_to(PROJECT_ROOT)),
        'region': region,
        'all_live_in': all_live_in,
        'interesting_live_in': interesting,
        'standard_live_in': standard,
    }


def load_existing_classification():
    """Load existing function classification for cross-reference."""
    json_path = CONFIG_DIR / "function_classification.json"
    if not json_path.exists():
        return {}

    try:
        with open(json_path) as f:
            data = json.load(f)
    except Exception:
        return {}

    result = {}
    for func in data.get('functions', []):
        addr_str = func.get('address', '')
        try:
            addr = int(addr_str, 16)
        except (ValueError, TypeError):
            continue
        result[addr] = func.get('categories', [])

    return result


def generate_report(results, classification, output_file=None):
    """Generate a comprehensive text report."""
    lines = []

    def w(s=''):
        lines.append(s)

    w("=" * 80)
    w("  LIVE-IN REGISTER ANALYSIS REPORT")
    w("  Tokimeki Memorial PS1 (SLPS-00065) -- GCC Region")
    w("=" * 80)
    w()

    # Filter to only functions with interesting live-ins
    gcc_results = [r for r in results if r['region'] == 'GCC']
    interesting_results = [r for r in gcc_results if r['interesting_live_in']]

    w(f"Total GCC-region ASM functions analyzed: {len(gcc_results)}")
    w(f"Functions with interesting live-in registers: {len(interesting_results)}")
    w(f"Functions with NO interesting live-ins (clean): {len(gcc_results) - len(interesting_results)}")
    w()

    # ===== SECTION 1: Register frequency statistics =====
    w("-" * 80)
    w("  SECTION 1: REGISTER FREQUENCY STATISTICS")
    w("-" * 80)
    w()

    reg_counts = Counter()
    for r in gcc_results:
        for reg in r['interesting_live_in']:
            reg_counts[reg] += 1

    w(f"{'Register':<10} {'Count':>8} {'% of GCC funcs':>16} {'% of live-in funcs':>20}")
    w("-" * 58)
    for reg, count in reg_counts.most_common():
        pct_total = 100.0 * count / len(gcc_results) if gcc_results else 0
        pct_livein = 100.0 * count / len(interesting_results) if interesting_results else 0
        w(f"  {reg:<8} {count:>8} {pct_total:>15.1f}% {pct_livein:>19.1f}%")
    w()

    # Group registers by category
    w("Register categories:")
    vreg_count = sum(1 for r in gcc_results if r['interesting_live_in'] & {'v0', 'v1'})
    treg_count = sum(1 for r in gcc_results if r['interesting_live_in'] & {'t0', 't1', 't2', 't3', 't4', 't5', 't6', 't7', 't8', 't9'})
    at_count = sum(1 for r in gcc_results if 'at' in r['interesting_live_in'])
    w(f"  v-regs (v0, v1):      {vreg_count:>6} functions")
    w(f"  t-regs (t0-t9):       {treg_count:>6} functions")
    w(f"  $at:                   {at_count:>6} functions")
    w()

    # ===== SECTION 2: Live-in signature clusters =====
    w("-" * 80)
    w("  SECTION 2: LIVE-IN SIGNATURE CLUSTERS")
    w("-" * 80)
    w()
    w("Functions grouped by their exact set of interesting live-in registers.")
    w("This reveals calling convention patterns and compiler behaviors.")
    w()

    # Build signature -> function list mapping
    sig_to_funcs = defaultdict(list)
    for r in gcc_results:
        sig = frozenset(r['interesting_live_in'])
        sig_to_funcs[sig].append(r)

    # Sort by cluster size (descending)
    sorted_sigs = sorted(sig_to_funcs.items(), key=lambda x: len(x[1]), reverse=True)

    for sig, funcs in sorted_sigs:
        if not sig:
            continue  # Skip "clean" functions here
        sig_str = ", ".join(sorted(sig, key=lambda r: _reg_sort_key(r)))
        w(f"Signature: {{ {sig_str} }}  ({len(funcs)} functions)")
        w(f"  {'Address':<14} {'Name':<40} {'Classification'}")
        w(f"  {'-'*13} {'-'*39} {'-'*25}")
        for func in sorted(funcs, key=lambda f: f['address']):
            addr_str = f"0x{func['address']:08X}"
            name = func['name'][:38]
            cats = classification.get(func['address'], [])
            cat_str = ", ".join(cats) if cats else "(not classified)"
            w(f"  {addr_str:<14} {name:<40} {cat_str}")
        w()

    # ===== SECTION 3: Most common combinations =====
    w("-" * 80)
    w("  SECTION 3: MOST COMMON LIVE-IN COMBINATIONS")
    w("-" * 80)
    w()

    combo_counts = Counter()
    for r in gcc_results:
        if r['interesting_live_in']:
            sig = frozenset(r['interesting_live_in'])
            combo_counts[sig] += 1

    w(f"{'Rank':<6} {'Count':>6} {'%':>7}  {'Register Combination'}")
    w("-" * 70)
    for rank, (combo, count) in enumerate(combo_counts.most_common(30), 1):
        pct = 100.0 * count / len(interesting_results) if interesting_results else 0
        combo_str = ", ".join(sorted(combo, key=lambda r: _reg_sort_key(r)))
        w(f"  {rank:<4} {count:>6} {pct:>6.1f}%  {{ {combo_str} }}")
    w()

    # ===== SECTION 4: Cross-reference with existing classification =====
    w("-" * 80)
    w("  SECTION 4: CROSS-REFERENCE WITH EXISTING CLASSIFICATION")
    w("-" * 80)
    w()

    if classification:
        # Compare our analysis with the existing classification
        existing_hidden = set()
        for addr, cats in classification.items():
            if any(c in ('hidden_vreg', 'hidden_treg', 'hidden_at', 'hidden_hilo')
                   for c in cats):
                existing_hidden.add(addr)

        our_hidden = set()
        for r in gcc_results:
            if r['interesting_live_in']:
                our_hidden.add(r['address'])

        both = existing_hidden & our_hidden
        only_existing = existing_hidden - our_hidden
        only_ours = our_hidden - existing_hidden

        # Filter to GCC region only for existing
        gcc_existing = {a for a in existing_hidden if BASE_ADDR <= a <= GCC_REGION_END}
        both_gcc = {r['address'] for r in gcc_results if r['address'] in both}
        only_existing_gcc = {a for a in only_existing if BASE_ADDR <= a <= GCC_REGION_END}
        only_ours_gcc = {r['address'] for r in gcc_results if r['address'] in only_ours}

        w(f"Existing classification (GCC region, hidden_* categories): {len(gcc_existing)}")
        w(f"Our analysis (functions with interesting live-ins):        {len(our_hidden)}")
        w()
        w(f"  Agreement (both detect hidden regs): {len(both_gcc)}")
        w(f"  Only in existing classification:     {len(only_existing_gcc)}")
        w(f"  Only in our analysis:                {len(only_ours_gcc)}")
        w()

        if only_existing_gcc:
            w("Functions flagged by existing classification but NOT by our analysis:")
            w("  (may use binary analysis that catches patterns not in ASM files)")
            for addr in sorted(only_existing_gcc)[:20]:
                cats = classification.get(addr, [])
                w(f"  0x{addr:08X}  categories: {', '.join(cats)}")
            if len(only_existing_gcc) > 20:
                w(f"  ... and {len(only_existing_gcc) - 20} more")
            w()

        if only_ours_gcc:
            w("Functions flagged by our analysis but NOT by existing classification:")
            w("  (may indicate new hidden register patterns)")
            for addr in sorted(only_ours_gcc)[:20]:
                func = next((r for r in gcc_results if r['address'] == addr), None)
                if func:
                    regs = ", ".join(sorted(func['interesting_live_in'],
                                           key=lambda r: _reg_sort_key(r)))
                    w(f"  0x{addr:08X}  {func['name']:<35}  live-in: {{ {regs} }}")
            if len(only_ours_gcc) > 20:
                w(f"  ... and {len(only_ours_gcc) - 20} more")
            w()
    else:
        w("(No existing classification data found for cross-reference)")
        w()

    # ===== SECTION 5: Per-function detail =====
    w("-" * 80)
    w("  SECTION 5: PER-FUNCTION LIVE-IN DETAILS (GCC REGION)")
    w("-" * 80)
    w()
    w(f"{'Address':<14} {'Name':<35} {'Interesting Live-Ins':<35} {'Standard Live-Ins'}")
    w("-" * 120)

    for r in sorted(gcc_results, key=lambda x: x['address']):
        addr_str = f"0x{r['address']:08X}"
        name = r['name'][:33]
        interesting = ", ".join(sorted(r['interesting_live_in'],
                                       key=lambda reg: _reg_sort_key(reg))) if r['interesting_live_in'] else "(none)"
        standard = ", ".join(sorted(r['standard_live_in'],
                                    key=lambda reg: _reg_sort_key(reg)))
        w(f"  {addr_str:<12} {name:<35} {interesting:<35} {standard}")

    w()

    # ===== SECTION 6: Analysis of "thunk" patterns =====
    w("-" * 80)
    w("  SECTION 6: THUNK / PRE-PROLOGUE PATTERN ANALYSIS")
    w("-" * 80)
    w()
    w("Functions where live-in registers suggest a thunk pattern")
    w("(code before the stack frame setup that uses caller-set registers).")
    w()

    thunk_funcs = []
    for r in gcc_results:
        if r['interesting_live_in']:
            # Check if at and a t-reg are both live-in (common thunk pattern)
            has_at = 'at' in r['interesting_live_in']
            has_treg = bool(r['interesting_live_in'] & {'t6', 't7', 't8', 't9'})
            has_vreg = bool(r['interesting_live_in'] & {'v0', 'v1'})
            if (has_at and has_treg) or (has_at and has_vreg):
                thunk_funcs.append(r)

    w(f"Functions with $at + t-reg/v-reg live-in (likely thunks): {len(thunk_funcs)}")
    for func in sorted(thunk_funcs, key=lambda f: f['address'])[:30]:
        addr_str = f"0x{func['address']:08X}"
        regs = ", ".join(sorted(func['interesting_live_in'],
                                key=lambda r: _reg_sort_key(r)))
        w(f"  {addr_str}  {func['name']:<35}  {{ {regs} }}")
    if len(thunk_funcs) > 30:
        w(f"  ... and {len(thunk_funcs) - 30} more")
    w()

    # ===== SECTION 7: Summary statistics =====
    w("-" * 80)
    w("  SECTION 7: SUMMARY")
    w("-" * 80)
    w()

    # Count by number of interesting live-ins
    livein_count_dist = Counter()
    for r in gcc_results:
        livein_count_dist[len(r['interesting_live_in'])] += 1

    w("Distribution by number of interesting live-in registers:")
    for count in sorted(livein_count_dist.keys()):
        num = livein_count_dist[count]
        bar = "#" * (num // 2)
        w(f"  {count:>2} registers: {num:>4} functions  {bar}")
    w()

    # Top register pairs
    w("Top co-occurring register pairs:")
    pair_counts = Counter()
    for r in gcc_results:
        regs = sorted(r['interesting_live_in'], key=lambda x: _reg_sort_key(x))
        for i in range(len(regs)):
            for j in range(i + 1, len(regs)):
                pair_counts[(regs[i], regs[j])] += 1

    for (r1, r2), count in pair_counts.most_common(15):
        w(f"  ({r1}, {r2}): {count}")
    w()

    # Conclusion
    w("=" * 80)
    w("  CONCLUSIONS")
    w("=" * 80)
    w()
    w(f"The GCC region contains {len(gcc_results)} ASM functions.")
    w(f"{len(interesting_results)} ({100.0*len(interesting_results)/len(gcc_results):.1f}%) "
      f"have non-standard live-in registers.")
    w()
    if reg_counts:
        top_reg = reg_counts.most_common(1)[0]
        w(f"Most common live-in register: {top_reg[0]} ({top_reg[1]} functions)")
    if combo_counts:
        top_combo = combo_counts.most_common(1)[0]
        combo_str = ", ".join(sorted(top_combo[0], key=lambda r: _reg_sort_key(r)))
        w(f"Most common live-in combination: {{ {combo_str} }} ({top_combo[1]} functions)")
    w()
    w("These hidden register parameters indicate the 'mystery compiler' passes")
    w("data through registers that standard C calling conventions do not use for")
    w("parameters. This is strong evidence of either:")
    w("  1. A non-standard compiler or custom calling convention")
    w("  2. Hand-written assembly with inline register passing")
    w("  3. A linker/post-processor that inserts pre-prologue instructions")
    w()

    report = "\n".join(lines)

    if output_file:
        os.makedirs(os.path.dirname(output_file), exist_ok=True)
        with open(output_file, 'w') as f:
            f.write(report)
        print(f"Report written to {output_file}")

    return report


def generate_json(results, output_file=None):
    """Generate JSON output for further processing."""
    gcc_results = [r for r in results if r['region'] == 'GCC']

    data = {
        'summary': {
            'total_analyzed': len(gcc_results),
            'with_interesting_live_ins': sum(1 for r in gcc_results if r['interesting_live_in']),
            'clean': sum(1 for r in gcc_results if not r['interesting_live_in']),
        },
        'register_frequencies': {},
        'signature_clusters': {},
        'functions': [],
    }

    # Register frequencies
    reg_counts = Counter()
    for r in gcc_results:
        for reg in r['interesting_live_in']:
            reg_counts[reg] += 1
    data['register_frequencies'] = dict(reg_counts.most_common())

    # Signature clusters
    sig_to_funcs = defaultdict(list)
    for r in gcc_results:
        sig = frozenset(r['interesting_live_in'])
        sig_key = ",".join(sorted(sig, key=lambda x: _reg_sort_key(x))) if sig else "(clean)"
        sig_to_funcs[sig_key].append(f"0x{r['address']:08X}")
    data['signature_clusters'] = {
        k: {'count': len(v), 'functions': v}
        for k, v in sorted(sig_to_funcs.items(), key=lambda x: -len(x[1]))
    }

    # Per-function data
    for r in sorted(gcc_results, key=lambda x: x['address']):
        data['functions'].append({
            'address': f"0x{r['address']:08X}",
            'name': r['name'],
            'filepath': r['filepath'],
            'interesting_live_in': sorted(r['interesting_live_in'],
                                          key=lambda x: _reg_sort_key(x)),
            'standard_live_in': sorted(r['standard_live_in'],
                                       key=lambda x: _reg_sort_key(x)),
        })

    if output_file:
        os.makedirs(os.path.dirname(output_file), exist_ok=True)
        with open(output_file, 'w') as f:
            json.dump(data, f, indent=2)
        print(f"JSON data written to {output_file}")

    return data


def _reg_sort_key(reg):
    """Sort key for registers in a natural order."""
    order = {
        'at': 0, 'v0': 1, 'v1': 2,
        'a0': 10, 'a1': 11, 'a2': 12, 'a3': 13,
        't0': 20, 't1': 21, 't2': 22, 't3': 23,
        't4': 24, 't5': 25, 't6': 26, 't7': 27,
        't8': 28, 't9': 29,
        's0': 30, 's1': 31, 's2': 32, 's3': 33,
        's4': 34, 's5': 35, 's6': 36, 's7': 37,
        'gp': 40, 'sp': 41, 'fp': 42, 'ra': 43,
        'zero': 50,
    }
    return order.get(reg, 99)


def main():
    parser = argparse.ArgumentParser(
        description='Live-in register analysis for MIPS ASM files')
    parser.add_argument('--output', '-o',
                        default=str(PROJECT_ROOT / 'build' / 'analysis' / 'live_in_report.txt'),
                        help='Output file for text report')
    parser.add_argument('--json', '-j',
                        default=None,
                        help='Output file for JSON data')
    parser.add_argument('--region', '-r',
                        choices=['GCC', 'IDO', 'all'],
                        default='GCC',
                        help='Region to analyze (default: GCC)')
    parser.add_argument('--no-cfg', action='store_true',
                        help='Use simple linear scan instead of CFG analysis')
    parser.add_argument('--verbose', '-v', action='store_true',
                        help='Print progress during analysis')
    args = parser.parse_args()

    # Collect ASM files
    asm_files = sorted(ASM_DIR.glob('*.s'))
    if not asm_files:
        print(f"No .s files found in {ASM_DIR}")
        sys.exit(1)

    print(f"Found {len(asm_files)} ASM files in {ASM_DIR}")

    # Load existing classification for cross-reference
    classification = load_existing_classification()
    print(f"Loaded existing classification for {len(classification)} functions")

    # Analyze each file
    results = []
    skipped = 0
    errors = 0

    for i, filepath in enumerate(asm_files):
        if filepath.name == 'README.md':
            continue

        if args.verbose and (i + 1) % 100 == 0:
            print(f"  Analyzing {i+1}/{len(asm_files)}...")

        try:
            result = analyze_file(filepath, use_cfg=not args.no_cfg)
            if result is None:
                skipped += 1
                continue

            # Region filter
            if args.region != 'all' and result['region'] != args.region:
                continue

            results.append(result)
        except Exception as e:
            errors += 1
            if args.verbose:
                print(f"  Error analyzing {filepath.name}: {e}")

    print(f"Analyzed {len(results)} functions (skipped {skipped}, errors {errors})")

    # Generate report
    report = generate_report(results, classification, args.output)

    # Generate JSON if requested
    if args.json:
        generate_json(results, args.json)

    # Print summary to stdout
    gcc_results = [r for r in results if r['region'] == 'GCC']
    interesting = [r for r in gcc_results if r['interesting_live_in']]
    print(f"\nSummary:")
    print(f"  GCC-region functions analyzed: {len(gcc_results)}")
    print(f"  With interesting live-ins:     {len(interesting)}")
    print(f"  Clean (no hidden regs):        {len(gcc_results) - len(interesting)}")

    if interesting:
        reg_counts = Counter()
        for r in interesting:
            for reg in r['interesting_live_in']:
                reg_counts[reg] += 1
        print(f"\n  Top live-in registers:")
        for reg, count in reg_counts.most_common(10):
            print(f"    {reg:<5}: {count}")


if __name__ == '__main__':
    main()
