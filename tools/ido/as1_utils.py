#!/usr/bin/env python3
"""as1 replication utilities for asm_intercept transforms.

Provides functions that replicate as1's delay slot filling and lui sharing
behavior. These operate on lists of assembly lines (GNU as syntax, .set noreorder).

as1's delay slot filling algorithm:
  For each branch/jump with a nop delay slot:
    Scan backward 1 to 3 instructions
    If a candidate instruction:
      - Does not write a register the branch reads
      - Does not read a register the branch writes
      - Is not itself a branch/jump
      - Does not read memory that the branch depends on
    Then move it into the delay slot (replace the nop)

as1's lui sharing algorithm:
  Within a basic block, track the last lui $at,hi16 loaded.
  If another reference to the same hi16 appears before $at is clobbered,
  reuse $at instead of emitting a new lui.
"""

import re

# Registers that branches can read (source operands)
BRANCH_OPS = {'beq', 'bne', 'beqz', 'bnez', 'blez', 'bgez', 'bltz', 'bgtz',
              'bltzal', 'bgezal'}
JUMP_OPS = {'j', 'jal', 'jr', 'jalr'}
LOAD_OPS = {'lb', 'lbu', 'lh', 'lhu', 'lw', 'lwl', 'lwr'}
STORE_OPS = {'sb', 'sh', 'sw', 'swl', 'swr'}
MULT_OPS = {'mult', 'multu', 'div', 'divu'}


def parse_instruction(line):
    """Parse an assembly instruction line into (opcode, operands_str, raw_line).
    Returns None for non-instruction lines (labels, directives, empty)."""
    stripped = line.strip()
    if not stripped or stripped.startswith('.') or stripped.startswith('#') or stripped.endswith(':'):
        return None
    parts = stripped.split('\t', 1)
    if len(parts) < 2:
        parts = stripped.split(None, 1)
    if len(parts) < 1:
        return None
    opcode = parts[0].strip()
    operands = parts[1].strip() if len(parts) > 1 else ''
    return (opcode, operands, line)


def get_written_regs(opcode, operands):
    """Return set of registers written by an instruction."""
    regs = set()
    parts = [p.strip() for p in operands.split(',')]

    if opcode in STORE_OPS or opcode in BRANCH_OPS or opcode in JUMP_OPS:
        # Stores and branches don't write to register operands (first operand is source)
        pass
    elif opcode == 'jr':
        pass
    elif opcode == 'jalr' or opcode == 'jal':
        regs.add('$31')  # writes $ra
    elif opcode in ('mflo', 'mfhi'):
        if parts:
            regs.add(parts[0])
    elif opcode in MULT_OPS:
        regs.add('$lo')
        regs.add('$hi')
    elif opcode == 'nop':
        pass
    else:
        # ALU/load: first operand is destination
        if parts:
            regs.add(parts[0])

    return regs


def get_read_regs(opcode, operands):
    """Return set of registers read by an instruction."""
    regs = set()
    parts = [p.strip() for p in operands.split(',')]

    if opcode == 'nop':
        return regs
    if opcode in ('jr', 'jalr'):
        if parts:
            regs.add(parts[0])
        return regs
    if opcode in ('jal', 'j'):
        return regs
    if opcode in ('beqz', 'bnez', 'blez', 'bgez', 'bltz', 'bgtz', 'bltzal', 'bgezal'):
        if parts:
            regs.add(parts[0])
        return regs
    if opcode in ('beq', 'bne'):
        if len(parts) >= 2:
            regs.add(parts[0])
            regs.add(parts[1])
        return regs

    if opcode in STORE_OPS:
        # sw $rt, offset($rs) -> reads $rt and $rs
        if parts:
            regs.add(parts[0])
        if len(parts) >= 2:
            m = re.search(r'\((\$\d+)\)', parts[1])
            if m:
                regs.add(m.group(1))
        return regs

    if opcode in LOAD_OPS:
        # lw $rt, offset($rs) -> reads $rs
        if len(parts) >= 2:
            m = re.search(r'\((\$\d+)\)', parts[1])
            if m:
                regs.add(m.group(1))
        return regs

    if opcode in ('mflo', 'mfhi'):
        if opcode == 'mflo':
            regs.add('$lo')
        else:
            regs.add('$hi')
        return regs

    if opcode in MULT_OPS:
        for p in parts:
            p = p.strip()
            if p.startswith('$'):
                regs.add(p)
        return regs

    # ALU: reads all operands except first (destination)
    for p in parts[1:]:
        p = p.strip()
        if p.startswith('$'):
            regs.add(p)
        else:
            m = re.search(r'\((\$\d+)\)', p)
            if m:
                regs.add(m.group(1))
    return regs


def is_branch_or_jump(opcode):
    """Check if an opcode is a branch or jump."""
    return opcode in BRANCH_OPS or opcode in JUMP_OPS or opcode.startswith('b')


def is_nop(line):
    """Check if a line is a nop instruction."""
    return line.strip() == 'nop' or line.strip() == '\tnop'


def is_memory_op(opcode):
    """Check if an opcode accesses memory."""
    return opcode in LOAD_OPS or opcode in STORE_OPS


def fill_delay_slots(lines, max_scan=3):
    """Fill branch delay slots by scanning backward for safe candidates.

    Replicates as1's delay slot filling algorithm:
    For each branch with a nop delay slot, scan backward up to max_scan
    instructions to find a safe candidate to move into the delay slot.

    Args:
        lines: list of assembly lines
        max_scan: maximum instructions to scan backward (as1 uses 1-3)

    Returns:
        Modified list of assembly lines
    """
    result = list(lines)

    i = 0
    while i < len(result) - 1:
        parsed = parse_instruction(result[i])
        if parsed is None:
            i += 1
            continue

        opcode, operands, _ = parsed

        if not is_branch_or_jump(opcode):
            i += 1
            continue

        # Check if delay slot is a nop
        if i + 1 >= len(result) or not is_nop(result[i + 1]):
            i += 1
            continue

        # Get registers the branch reads
        branch_reads = get_read_regs(opcode, operands)
        branch_writes = get_written_regs(opcode, operands)

        # Scan backward for a safe candidate
        found = -1
        for scan in range(1, max_scan + 1):
            cand_idx = i - scan
            if cand_idx < 0:
                break

            cand = parse_instruction(result[cand_idx])
            if cand is None:
                break  # label or directive, stop scanning

            cand_op, cand_operands, _ = cand

            # Cannot move branches/jumps
            if is_branch_or_jump(cand_op):
                break

            # Cannot move nops (pointless)
            if cand_op == 'nop':
                continue

            cand_writes = get_written_regs(cand_op, cand_operands)
            cand_reads = get_read_regs(cand_op, cand_operands)

            # Safety checks:
            # 1. Candidate must not write a register the branch reads
            if cand_writes & branch_reads:
                break  # also blocks further scanning

            # 2. Branch must not write a register the candidate reads
            if branch_writes & cand_reads:
                break

            # 3. Check intervening instructions don't depend on candidate
            safe = True
            for between in range(cand_idx + 1, i):
                between_parsed = parse_instruction(result[between])
                if between_parsed is None:
                    safe = False
                    break
                b_op, b_operands, _ = between_parsed
                b_writes = get_written_regs(b_op, b_operands)
                b_reads = get_read_regs(b_op, b_operands)

                # Candidate writes something intervening reads?
                if cand_writes & b_reads:
                    safe = False
                    break
                # Intervening writes something candidate reads?
                if b_writes & cand_reads:
                    safe = False
                    break

            if safe:
                found = cand_idx
                break

        if found >= 0:
            # Move candidate into delay slot (replace the nop)
            cand_line = result[found]
            del result[found]
            # After deletion, branch is at found (shifted), delay slot at found+1
            result[found + 1] = cand_line
            # Don't advance i since we deleted a line before it
        else:
            i += 1
            continue

        i += 1

    return result


def unhoist_delay_slot(lines, branch_idx, target_insn_pattern=None):
    """Unhoist an instruction from a delay slot back to its natural position.

    This is the INVERSE of as1's delay slot filling. Use when IDO's as1 filled
    a delay slot but the target binary has nop there.

    Args:
        lines: list of assembly lines
        branch_idx: index of the branch instruction
        target_insn_pattern: if given, verify the delay slot matches this pattern

    Returns:
        Modified list of lines, or None if unhoist failed
    """
    if branch_idx + 1 >= len(lines):
        return None

    delay_line = lines[branch_idx + 1]
    delay_parsed = parse_instruction(delay_line)
    if delay_parsed is None:
        return None

    if target_insn_pattern and target_insn_pattern not in delay_line:
        return None

    # Replace delay slot with nop
    result = list(lines)
    result[branch_idx + 1] = '\tnop'

    # Insert the instruction before the branch
    result.insert(branch_idx, delay_line)

    return result
