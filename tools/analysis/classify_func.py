#!/usr/bin/env python3
"""
Function Classifier for Tokimeki Memorial PS1 Decompilation

Categorizes all 881 ASM functions by their conversion difficulty patterns.

Categories detected:
1. dual_entry_thunk - Has secondary label inside, or prologue begins after label
2. hidden_treg - t6/t7/t8/t9 read-before-write (loaded by caller)
3. hidden_vreg - v0/v1 read-before-write (implicit params)
4. hidden_at - $at read-before-write (rare)
5. delay_slot_sensitive - Stores in branch delay slots
6. jump_table - Contains switch/jump tables (jr to non-ra register)
7. unaligned_access - Uses lwl/lwr/swl/swr instructions
8. hidden_hilo - Uses mfhi/mflo without prior mult/div
9. clean - No obvious issues, should be convertible

Usage:
    python3 classify_func.py                    # Classify all ASM files
    python3 classify_func.py --address 0x80041234
    python3 classify_func.py --file asm/main/matched/func_80041234.s
    python3 classify_func.py --csv              # Output to config/function_classification.csv
    python3 classify_func.py --json             # Output as JSON
"""

import os
import re
import sys
import json
import csv
import argparse
from pathlib import Path
from collections import defaultdict

# Constants
ASM_DIR = Path("asm/main/matched")
OUTPUT_CSV = Path("config/function_classification.csv")
OUTPUT_JSON = Path("config/function_classification.json")

BASE_ADDR = 0x80041000
GCC_REGION_END = 0x80085FFF
IDO_REGION_START = 0x80086000
CODE_END = 0x800C0000

# Branch/jump instructions
BRANCH_OPS = {'beq', 'bne', 'bgez', 'bgtz', 'blez', 'bltz', 'bltzal', 'bgezal',
              'bc1f', 'bc1t', 'bc0f', 'bc0t', 'bnez', 'beqz'}
JUMP_OPS = {'j', 'jal', 'jr', 'jalr'}

# Instructions that WRITE to the first operand
WRITE_FIRST_OPS = {
    'lui', 'li', 'la', 'move',
    'addiu', 'addi', 'add', 'addu', 'sub', 'subu',
    'and', 'andi', 'or', 'ori', 'xor', 'xori', 'nor',
    'sll', 'srl', 'sra', 'sllv', 'srlv', 'srav',
    'slt', 'slti', 'sltu', 'sltiu',
    'lb', 'lbu', 'lh', 'lhu', 'lw', 'lwl', 'lwr',
    'mfhi', 'mflo', 'mfc0', 'mfc2', 'cfc2'
}


def get_region(addr):
    """Determine compiler region from address."""
    if BASE_ADDR <= addr <= GCC_REGION_END:
        return "GCC"
    elif IDO_REGION_START <= addr < CODE_END:
        return "IDO"
    return "unknown"


def normalize_reg(reg):
    """Normalize register name to standard form."""
    if not reg:
        return None
    reg = reg.strip().lstrip('$')

    # Convert numeric to named
    num_map = {
        '0': 'zero', '1': 'at', '2': 'v0', '3': 'v1',
        '4': 'a0', '5': 'a1', '6': 'a2', '7': 'a3',
        '8': 't0', '9': 't1', '10': 't2', '11': 't3',
        '12': 't4', '13': 't5', '14': 't6', '15': 't7',
        '16': 's0', '17': 's1', '18': 's2', '19': 's3',
        '20': 's4', '21': 's5', '22': 's6', '23': 's7',
        '24': 't8', '25': 't9', '26': 'k0', '27': 'k1',
        '28': 'gp', '29': 'sp', '30': 'fp', '31': 'ra'
    }
    if reg in num_map:
        return num_map[reg]

    # Already named
    if reg in ('zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
               't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
               's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
               't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra'):
        return reg

    return None


def parse_instruction(line):
    """Parse a single instruction line into op and operands."""
    # Remove comments
    line = re.sub(r'#.*$', '', line).strip()
    line = re.sub(r'/\*.*?\*/', '', line).strip()

    if not line:
        return None, []

    # Match instruction
    match = re.match(r'^([a-z]+)\s*(.*)', line, re.I)
    if not match:
        return None, []

    op = match.group(1).lower()
    args_str = match.group(2).strip()

    # Parse operands
    operands = []
    if args_str:
        # Handle memory operands like "offset(base)"
        parts = re.split(r',\s*', args_str)
        for part in parts:
            part = part.strip()
            # Memory operand: "offset(reg)"
            mem_match = re.match(r'(-?\d+|\w+)\s*\((\$?\w+)\)', part)
            if mem_match:
                operands.append(('mem', mem_match.group(1), mem_match.group(2)))
            else:
                operands.append(('reg_or_imm', part))

    return op, operands


def get_read_write_regs(op, operands):
    """
    Determine which registers are read and written by an instruction.
    Returns (reads, writes) as sets of normalized register names.
    """
    reads = set()
    writes = set()

    if not op or not operands:
        return reads, writes

    # Special cases first
    if op == 'nop':
        return reads, writes

    if op in ('j', 'jal'):
        if op == 'jal':
            writes.add('ra')
        return reads, writes

    if op == 'jr':
        if operands:
            reg = normalize_reg(operands[0][1] if operands[0][0] == 'reg_or_imm' else None)
            if reg:
                reads.add(reg)
        return reads, writes

    if op == 'jalr':
        # jalr rd, rs  (rd defaults to ra)
        if len(operands) >= 2:
            rd = normalize_reg(operands[0][1] if operands[0][0] == 'reg_or_imm' else None)
            rs = normalize_reg(operands[1][1] if operands[1][0] == 'reg_or_imm' else None)
            if rd:
                writes.add(rd)
            if rs:
                reads.add(rs)
        elif len(operands) == 1:
            rs = normalize_reg(operands[0][1] if operands[0][0] == 'reg_or_imm' else None)
            if rs:
                reads.add(rs)
            writes.add('ra')
        return reads, writes

    # Store instructions: read rt and base
    if op in ('sb', 'sh', 'sw', 'swl', 'swr'):
        if len(operands) >= 2:
            rt = normalize_reg(operands[0][1] if operands[0][0] == 'reg_or_imm' else None)
            if rt:
                reads.add(rt)
            # Base register from memory operand
            if operands[1][0] == 'mem':
                base = normalize_reg(operands[1][2])
                if base:
                    reads.add(base)
        return reads, writes

    # Load instructions: write rt, read base
    if op in ('lb', 'lbu', 'lh', 'lhu', 'lw', 'lwl', 'lwr'):
        if len(operands) >= 2:
            rt = normalize_reg(operands[0][1] if operands[0][0] == 'reg_or_imm' else None)
            if rt:
                writes.add(rt)
            if operands[1][0] == 'mem':
                base = normalize_reg(operands[1][2])
                if base:
                    reads.add(base)
        return reads, writes

    # lui: writes rt only
    if op == 'lui':
        if operands:
            rt = normalize_reg(operands[0][1] if operands[0][0] == 'reg_or_imm' else None)
            if rt:
                writes.add(rt)
        return reads, writes

    # li/la: writes rd only
    if op in ('li', 'la'):
        if operands:
            rd = normalize_reg(operands[0][1] if operands[0][0] == 'reg_or_imm' else None)
            if rd:
                writes.add(rd)
        return reads, writes

    # move: writes rd, reads rs
    if op == 'move':
        if len(operands) >= 2:
            rd = normalize_reg(operands[0][1] if operands[0][0] == 'reg_or_imm' else None)
            rs = normalize_reg(operands[1][1] if operands[1][0] == 'reg_or_imm' else None)
            if rd:
                writes.add(rd)
            if rs:
                reads.add(rs)
        return reads, writes

    # mfhi/mflo: writes rd
    if op in ('mfhi', 'mflo'):
        if operands:
            rd = normalize_reg(operands[0][1] if operands[0][0] == 'reg_or_imm' else None)
            if rd:
                writes.add(rd)
        return reads, writes

    # mthi/mtlo: reads rs
    if op in ('mthi', 'mtlo'):
        if operands:
            rs = normalize_reg(operands[0][1] if operands[0][0] == 'reg_or_imm' else None)
            if rs:
                reads.add(rs)
        return reads, writes

    # mult/multu/div/divu: reads rs, rt
    if op in ('mult', 'multu', 'div', 'divu'):
        if len(operands) >= 2:
            rs = normalize_reg(operands[0][1] if operands[0][0] == 'reg_or_imm' else None)
            rt = normalize_reg(operands[1][1] if operands[1][0] == 'reg_or_imm' else None)
            if rs:
                reads.add(rs)
            if rt:
                reads.add(rt)
        return reads, writes

    # Branch instructions: read rs (and rt for beq/bne)
    if op in BRANCH_OPS:
        if operands:
            rs = normalize_reg(operands[0][1] if operands[0][0] == 'reg_or_imm' else None)
            if rs:
                reads.add(rs)
            if op in ('beq', 'bne') and len(operands) >= 2:
                rt = normalize_reg(operands[1][1] if operands[1][0] == 'reg_or_imm' else None)
                if rt:
                    reads.add(rt)
        return reads, writes

    # R-type and I-type ALU: write rd, read rs (and rt for R-type)
    if op in WRITE_FIRST_OPS:
        if operands:
            rd = normalize_reg(operands[0][1] if operands[0][0] == 'reg_or_imm' else None)
            if rd:
                writes.add(rd)
            for i in range(1, len(operands)):
                if operands[i][0] == 'reg_or_imm':
                    rs = normalize_reg(operands[i][1])
                    if rs:
                        reads.add(rs)
                elif operands[i][0] == 'mem':
                    base = normalize_reg(operands[i][2])
                    if base:
                        reads.add(base)
        return reads, writes

    # Default: try to parse registers
    for operand in operands:
        if operand[0] == 'reg_or_imm':
            reg = normalize_reg(operand[1])
            if reg:
                reads.add(reg)
        elif operand[0] == 'mem':
            base = normalize_reg(operand[2])
            if base:
                reads.add(base)

    return reads, writes


def parse_asm_file(filepath):
    """Parse an ASM file and extract structured information."""
    with open(filepath, 'r') as f:
        content = f.read()
        lines = content.split('\n')

    # Extract address from filename
    filename = os.path.basename(filepath)
    addr_match = re.search(r'func_([0-9a-fA-F]+)', filename)
    address = int(addr_match.group(1), 16) if addr_match else 0

    # Track labels and instructions
    labels = []
    instructions = []
    prologue_idx = None

    for i, line in enumerate(lines):
        stripped = line.strip()

        # Skip empty lines and pure comments
        if not stripped or stripped.startswith('#'):
            continue

        # Check for .globl directives
        if '.globl' in stripped.lower() or '.global' in stripped.lower():
            match = re.search(r'\.globl?\s+(\w+)', stripped, re.I)
            if match:
                labels.append(('globl', match.group(1), i))
            continue

        # Check for labels (lines ending with ':')
        if stripped.endswith(':') and not stripped.startswith('.'):
            label_name = stripped[:-1].strip()
            labels.append(('label', label_name, i))
            continue

        # Skip directives
        if stripped.startswith('.'):
            continue

        # Parse instruction
        op, operands = parse_instruction(stripped)
        if op:
            reads, writes = get_read_write_regs(op, operands)
            instructions.append({
                'op': op,
                'operands': operands,
                'reads': reads,
                'writes': writes,
                'line': i,
                'raw': stripped
            })

            # Check for prologue (addiu sp,sp,-N)
            if prologue_idx is None and op == 'addiu':
                if 'sp' in writes and 'sp' in reads:
                    # Check for negative offset (stack allocation)
                    raw_lower = stripped.lower().replace(' ', '')
                    if '-' in raw_lower:
                        prologue_idx = len(instructions) - 1

    return {
        'filepath': str(filepath),
        'address': address,
        'labels': labels,
        'instructions': instructions,
        'prologue_idx': prologue_idx,
        'content': content
    }


# Categories that do NOT prevent a byte-perfect C conversion.  `clean` is the
# no-findings case; `cross_prologue_hoist` is IDO -O2 lifting a function's first
# global reads above its prologue, which 119 already-byte-perfect functions show
# is no obstacle at all -- the fix is to widen the compare window, not to give up.
NON_BLOCKING_CATEGORIES = {'clean', 'cross_prologue_hoist'}


def detect_dual_entry_thunk(parsed):
    """
    Detect dual_entry_thunk pattern:
    - Has secondary label inside function
    - Prologue begins after first instruction
    - Code before prologue (excluding nops)
    """
    labels = parsed['labels']
    instructions = parsed['instructions']
    prologue_idx = parsed['prologue_idx']

    thunk_reasons = []
    hoist_reasons = []

    # A GENUINE second entry: more than one .globl func_* label in the body.
    func_pattern = re.compile(r'func_[0-9a-fA-F]+')
    globl_func_labels = [l for l in labels if l[0] == 'globl' and func_pattern.match(l[1])]
    if len(globl_func_labels) > 1:
        thunk_reasons.append(f"Multiple function labels: {[l[1] for l in globl_func_labels]}")

    # ⚠️ 2026-09-17: "code before prologue" is NOT a second entry point.  It is
    # IDO -O2's ordinary cross-prologue hoist -- the compiler lifting a
    # function's opening global reads ABOVE its own prologue.  Lumping it in
    # with dual_entry_thunk marked 230 of 236 functions is_convertible=False,
    # and 119 of those 236 are ALREADY registered byte-perfect C (a random
    # sample of 10 re-tested at 100% exact, Fixup 0).  Several headers had even
    # copied the label into a "CONFIRMED UNMATCHABLE" claim that was simply
    # wrong.  It is now its own NON-BLOCKING category.
    if prologue_idx is not None and prologue_idx > 0:
        pre_prologue = instructions[:prologue_idx]
        actual_code = [i for i in pre_prologue if i['op'] != 'nop']
        if actual_code:
            ops = [i['op'] for i in actual_code]
            hoist_reasons.append(f"Code before prologue: {ops}")

    return bool(thunk_reasons), thunk_reasons, hoist_reasons


def detect_hidden_regs(parsed):
    """
    Detect hidden register parameters.
    Only flag registers that are:
    1. Read BEFORE any write to that register
    2. Not a standard argument register (a0-a3)
    3. Not zero, sp, ra, gp (standard always-defined regs)
    """
    instructions = parsed['instructions']
    prologue_idx = parsed['prologue_idx']

    # Start analysis after prologue
    start_idx = prologue_idx + 1 if prologue_idx is not None else 0

    # Initialize: standard registers that are always "defined"
    defined_regs = {'zero', 'sp', 'ra', 'gp', 'fp',
                    'a0', 'a1', 'a2', 'a3',  # Function arguments
                    's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7'}  # Saved regs (caller-defined)

    # If there's a prologue, sp is modified there
    if prologue_idx is not None:
        for instr in instructions[:prologue_idx + 1]:
            defined_regs.update(instr['writes'])

    hidden_t = []
    hidden_v = []
    hidden_at = []

    # Check pre-prologue for hidden register reads (critical pattern)
    if prologue_idx is not None:
        for instr in instructions[:prologue_idx]:
            for reg in instr['reads']:
                if reg in ('t6', 't7', 't8', 't9'):
                    if reg not in defined_regs:
                        hidden_t.append(f"Pre-prologue read: {reg} in '{instr['raw']}'")
                        defined_regs.add(reg)
                elif reg in ('v0', 'v1'):
                    if reg not in defined_regs:
                        hidden_v.append(f"Pre-prologue read: {reg} in '{instr['raw']}'")
                        defined_regs.add(reg)
                elif reg == 'at':
                    if reg not in defined_regs:
                        hidden_at.append(f"Pre-prologue read: {reg} in '{instr['raw']}'")
                        defined_regs.add(reg)
            # Track writes
            defined_regs.update(instr['writes'])

    # Analyze first 20 instructions after prologue
    for instr in instructions[start_idx:start_idx + 20]:
        # Check reads
        for reg in instr['reads']:
            if reg not in defined_regs:
                if reg in ('t6', 't7', 't8', 't9'):
                    hidden_t.append(f"Read before write: {reg} in '{instr['raw']}'")
                elif reg in ('v0', 'v1'):
                    hidden_v.append(f"Read before write: {reg} in '{instr['raw']}'")
                elif reg == 'at':
                    hidden_at.append(f"Read before write: {reg} in '{instr['raw']}'")

        # Track writes (mark as defined)
        defined_regs.update(instr['writes'])

    return hidden_t, hidden_v, hidden_at


def detect_delay_slot_sensitive(parsed):
    """
    Detect delay_slot_sensitive pattern.

    NOTE: Stores in delay slots are actually normal compiler behavior and
    not a blocking pattern. Modern compilers can match this.
    We only flag TRULY unusual patterns like writing to a register that
    is used in the branch condition (which would be a data hazard).

    For now, this is disabled as it was flagging too many normal functions.
    """
    # Disabled - stores in delay slots are normal
    return False, []


def detect_jump_table(parsed):
    """
    Detect jump_table pattern:
    - jr with computed register (not $ra)
    """
    instructions = parsed['instructions']
    reasons = []

    for instr in instructions:
        if instr['op'] == 'jr':
            # Check what register is being jumped to
            for reg in instr['reads']:
                if reg not in ('ra',):
                    reasons.append(f"Indirect jump: jr {reg}")
                    break

    return bool(reasons), reasons


def detect_unaligned_access(parsed):
    """Detect unaligned memory access patterns."""
    instructions = parsed['instructions']
    unaligned_ops = []

    for instr in instructions:
        if instr['op'] in ('lwl', 'lwr', 'swl', 'swr'):
            unaligned_ops.append(instr['raw'])

    return bool(unaligned_ops), unaligned_ops[:5]  # Limit


def detect_hidden_hilo(parsed):
    """Detect mfhi/mflo without prior mult/div."""
    instructions = parsed['instructions']
    has_mult_div = False
    issues = []

    for instr in instructions:
        if instr['op'] in ('mult', 'multu', 'div', 'divu'):
            has_mult_div = True
        if instr['op'] in ('mfhi', 'mflo') and not has_mult_div:
            issues.append(f"Hidden HI/LO: {instr['op']} before mult/div")

    return bool(issues), issues


def classify_function(filepath):
    """Classify a single function and return all applicable categories."""
    parsed = parse_asm_file(filepath)

    categories = []
    details = {}

    address = parsed['address']
    region = get_region(address)

    # 1. Check for dual_entry_thunk (a genuine second entry) and, separately,
    #    the benign IDO cross-prologue hoist.
    is_thunk, thunk_reasons, hoist_reasons = detect_dual_entry_thunk(parsed)
    if is_thunk:
        categories.append('dual_entry_thunk')
        details['dual_entry_thunk'] = thunk_reasons
    if hoist_reasons:
        categories.append('cross_prologue_hoist')
        details['cross_prologue_hoist'] = hoist_reasons

    # 2-4. Check for hidden registers
    hidden_t, hidden_v, hidden_at = detect_hidden_regs(parsed)
    if hidden_t:
        categories.append('hidden_treg')
        details['hidden_treg'] = hidden_t
    if hidden_v:
        categories.append('hidden_vreg')
        details['hidden_vreg'] = hidden_v
    if hidden_at:
        categories.append('hidden_at')
        details['hidden_at'] = hidden_at

    # 5. Check for delay_slot_sensitive
    is_delay_sensitive, delay_reasons = detect_delay_slot_sensitive(parsed)
    if is_delay_sensitive:
        categories.append('delay_slot_sensitive')
        details['delay_slot_sensitive'] = delay_reasons

    # 6. Check for jump_table
    has_jump_table, jump_reasons = detect_jump_table(parsed)
    if has_jump_table:
        categories.append('jump_table')
        details['jump_table'] = jump_reasons

    # 7. Check for unaligned access
    has_unaligned, unaligned_ops = detect_unaligned_access(parsed)
    if has_unaligned:
        categories.append('unaligned_access')
        details['unaligned_access'] = unaligned_ops

    # 8. Check for hidden HI/LO
    has_hidden_hilo, hilo_reasons = detect_hidden_hilo(parsed)
    if has_hidden_hilo:
        categories.append('hidden_hilo')
        details['hidden_hilo'] = hilo_reasons

    # If no issues found, it's clean
    if not categories:
        categories.append('clean')

    return {
        'filepath': str(filepath),
        'address': f'0x{address:08X}',
        'address_int': address,
        'name': os.path.basename(filepath).replace('.s', ''),
        'region': region,
        'categories': categories,
        'details': details,
        'is_convertible': not (set(categories) - NON_BLOCKING_CATEGORIES)
    }


def classify_all_functions():
    """Classify all ASM functions."""
    asm_files = sorted(ASM_DIR.glob("func_*.s"))
    results = []

    for i, filepath in enumerate(asm_files):
        if (i + 1) % 100 == 0:
            print(f"  Processed {i+1}/{len(asm_files)}...", file=sys.stderr)

        try:
            result = classify_function(filepath)
            results.append(result)
        except Exception as e:
            print(f"Error processing {filepath}: {e}", file=sys.stderr)
            results.append({
                'filepath': str(filepath),
                'address': 'error',
                'name': os.path.basename(filepath).replace('.s', ''),
                'region': 'unknown',
                'categories': ['error'],
                'details': {'error': str(e)},
                'is_convertible': False
            })

    return results


def generate_summary(results):
    """Generate summary statistics."""
    category_counts = defaultdict(int)
    for r in results:
        for cat in r['categories']:
            category_counts[cat] += 1

    region_counts = defaultdict(int)
    for r in results:
        region_counts[r['region']] += 1

    convertible_count = sum(1 for r in results if r['is_convertible'])

    region_convertible = defaultdict(lambda: {'convertible': 0, 'blocked': 0})
    for r in results:
        if r['is_convertible']:
            region_convertible[r['region']]['convertible'] += 1
        else:
            region_convertible[r['region']]['blocked'] += 1

    blocking_patterns = defaultdict(int)
    for r in results:
        if not r['is_convertible']:
            for cat in r['categories']:
                if cat != 'error':
                    blocking_patterns[cat] += 1

    # Count functions by number of blocking patterns
    pattern_count_dist = defaultdict(int)
    for r in results:
        cats = [c for c in r['categories'] if c not in ('clean', 'error')]
        pattern_count_dist[len(cats)] += 1

    # Functions with only one blocking pattern (easiest to fix)
    single_pattern = defaultdict(int)
    for r in results:
        cats = [c for c in r['categories'] if c not in ('clean', 'error')]
        if len(cats) == 1:
            single_pattern[cats[0]] += 1

    return {
        'total': len(results),
        'convertible': convertible_count,
        'blocked': len(results) - convertible_count,
        'by_category': dict(category_counts),
        'by_region': dict(region_counts),
        'region_convertibility': dict(region_convertible),
        'blocking_patterns': dict(sorted(blocking_patterns.items(), key=lambda x: -x[1])),
        'by_pattern_count': dict(sorted(pattern_count_dist.items())),
        'single_pattern_functions': dict(sorted(single_pattern.items(), key=lambda x: -x[1]))
    }


def save_csv(results, filepath):
    """Save results to CSV."""
    with open(filepath, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['address', 'name', 'categories', 'region', 'is_convertible', 'notes'])

        for r in sorted(results, key=lambda x: x.get('address_int', 0)):
            notes = []
            for cat, details in r.get('details', {}).items():
                if isinstance(details, list) and details:
                    notes.append(f"{cat}: {details[0]}")

            writer.writerow([
                r['address'],
                r['name'],
                '|'.join(r['categories']),
                r['region'],
                r['is_convertible'],
                '; '.join(notes[:2])
            ])


def save_json(results, summary, filepath):
    """Save results to JSON."""
    output = {
        'summary': summary,
        'functions': results
    }
    with open(filepath, 'w') as f:
        json.dump(output, f, indent=2)


def main():
    parser = argparse.ArgumentParser(
        description='Classify ASM functions by conversion difficulty',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Categories:
  clean              - No blocking patterns, should be convertible
  dual_entry_thunk   - Has secondary label or code before prologue
  hidden_treg        - t6/t7/t8/t9 read before write
  hidden_vreg        - v0/v1 read before write
  hidden_at          - $at read before write
  hidden_hilo        - mfhi/mflo without prior mult/div
  delay_slot_sensitive - Stores in branch delay slots
  jump_table         - Contains switch/jump tables (jr to non-ra)
  unaligned_access   - Uses lwl/lwr/swl/swr

Examples:
    %(prog)s                                    # Classify all, print summary
    %(prog)s --csv                              # Save to config/function_classification.csv
    %(prog)s --json                             # Save to config/function_classification.json
    %(prog)s --address 0x80041234               # Classify single function
    %(prog)s --file asm/main/matched/func_80041234.s
"""
    )
    parser.add_argument('--address', type=lambda x: int(x, 0),
                        help='Classify function at specific address')
    parser.add_argument('--file', type=str,
                        help='Classify specific ASM file')
    parser.add_argument('--csv', action='store_true',
                        help='Save results to CSV')
    parser.add_argument('--json', action='store_true',
                        help='Save results to JSON')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='Show detailed output')

    args = parser.parse_args()

    script_dir = Path(__file__).parent
    project_root = script_dir.parent.parent
    os.chdir(project_root)

    if args.address:
        filepath = ASM_DIR / f"func_{args.address:08X}.s"
        if not filepath.exists():
            filepath = ASM_DIR / f"func_{args.address:08x}.s"
        if not filepath.exists():
            print(f"Error: No ASM file found for address 0x{args.address:08X}")
            sys.exit(1)

        result = classify_function(filepath)
        print(json.dumps(result, indent=2))

    elif args.file:
        filepath = Path(args.file)
        if not filepath.exists():
            print(f"Error: File not found: {filepath}")
            sys.exit(1)

        result = classify_function(filepath)
        print(json.dumps(result, indent=2))

    else:
        print(f"Classifying all ASM functions in {ASM_DIR}...", file=sys.stderr)
        results = classify_all_functions()
        summary = generate_summary(results)

        print("\n" + "=" * 70)
        print("FUNCTION CLASSIFICATION SUMMARY")
        print("=" * 70)
        print(f"Total ASM functions: {summary['total']}")
        print(f"Potentially convertible (clean): {summary['convertible']} ({100*summary['convertible']/summary['total']:.1f}%)")
        print(f"Blocked by patterns: {summary['blocked']} ({100*summary['blocked']/summary['total']:.1f}%)")

        print("\n--- By Region ---")
        for region, counts in sorted(summary['region_convertibility'].items()):
            total = counts['convertible'] + counts['blocked']
            pct = 100*counts['convertible']/total if total > 0 else 0
            print(f"  {region:6s}: {total:4d} total, {counts['convertible']:4d} clean ({pct:.1f}%)")

        print("\n--- Blocking Patterns ---")
        for cat, count in summary['blocking_patterns'].items():
            pct = 100*count/summary['blocked'] if summary['blocked'] > 0 else 0
            print(f"  {cat:25s}: {count:4d} ({pct:.1f}% of blocked)")

        print("\n--- All Categories (may overlap) ---")
        for cat, count in sorted(summary['by_category'].items(), key=lambda x: -x[1]):
            print(f"  {cat:25s}: {count:4d}")

        print("\n--- By Number of Blocking Patterns ---")
        for n, count in summary['by_pattern_count'].items():
            label = "clean" if n == 0 else f"{n} pattern{'s' if n > 1 else ''}"
            print(f"  {label:20s}: {count:4d}")

        print("\n--- Single-Pattern Functions (best conversion targets) ---")
        for pat, count in summary['single_pattern_functions'].items():
            print(f"  {pat:25s}: {count:4d}")

        if args.csv:
            OUTPUT_CSV.parent.mkdir(parents=True, exist_ok=True)
            save_csv(results, OUTPUT_CSV)
            print(f"\nCSV saved to: {OUTPUT_CSV}")

        if args.json:
            OUTPUT_JSON.parent.mkdir(parents=True, exist_ok=True)
            save_json(results, summary, OUTPUT_JSON)
            print(f"JSON saved to: {OUTPUT_JSON}")

        if not args.csv and not args.json:
            print(f"\nUse --csv to save to {OUTPUT_CSV}")
            print(f"Use --json to save to {OUTPUT_JSON}")


if __name__ == '__main__':
    main()
