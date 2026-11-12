#!/usr/bin/env python3
"""
Refined ASM audit that distinguishes:
1. True thunks - Pre-prologue code followed by a real function with prologue (REQUIRES ASM)
2. Leaf functions - No stack frame needed, simple code (POTENTIALLY C-CONVERTIBLE)
3. Standard functions - Starts with prologue (POTENTIALLY C-CONVERTIBLE)
4. Other patterns requiring ASM (hidden registers used before init, etc.)
"""

import os
import re
from pathlib import Path
from collections import defaultdict

def parse_asm_file(filepath):
    """Parse an ASM file and classify it."""
    with open(filepath, 'r') as f:
        content = f.read()
        lines = content.split('\n')

    # Extract all instructions (skip directives, labels, comments)
    instructions = []
    labels = []

    for line in lines:
        stripped = line.strip()

        # Skip empty lines and comments
        if not stripped or stripped.startswith('#') or stripped.startswith('/*') or stripped.startswith('*'):
            continue

        # Skip assembler directives
        if stripped.startswith('.'):
            continue

        # Capture labels
        if stripped.endswith(':') and not stripped.startswith('.'):
            labels.append(stripped[:-1])
            continue

        # This is an instruction
        instructions.append(stripped)

    if not instructions:
        return {'type': 'empty', 'reason': 'No instructions found'}

    # Check for prologue pattern anywhere in file
    # Note: ASM files may use "sp" or "$sp" - handle both
    prologue_pattern = r'addiu\s+\$?sp,\s*\$?sp,\s*-\d+'
    has_prologue = any(re.match(prologue_pattern, instr) for instr in instructions)

    # Check if first instruction is prologue
    first_is_prologue = bool(re.match(prologue_pattern, instructions[0]))

    # Find prologue position
    prologue_idx = None
    for i, instr in enumerate(instructions):
        if re.match(prologue_pattern, instr):
            prologue_idx = i
            break

    # Check for jr $ra (function returns) - handle both "jr ra" and "jr $ra"
    has_jr_ra = any(re.match(r'jr\s+\$?ra', instr) for instr in instructions)

    # Check for jal (function calls) - must be at start of instruction
    has_jal = any(re.match(r'jal\s+', instr) for instr in instructions)

    # Check for jalr (indirect calls)
    has_jalr = any(re.match(r'jalr\s+', instr) for instr in instructions)

    # Registers used before being set (potential hidden registers)
    # This is a simplified check - look at first few instructions before prologue
    hidden_regs = set()
    temp_regs = ['t0', 't1', 't2', 't3', 't4', 't5', 't6', 't7', 't8', 't9', 'v0', 'v1', 'at']

    if prologue_idx is not None and prologue_idx > 0:
        # Check instructions before prologue for register usage
        for instr in instructions[:prologue_idx]:
            for reg in temp_regs:
                # Handle both $t6 and t6 notation
                pattern = rf'(?:\$|(?<=[\s,])){reg}(?=[,\s\)]|$)'
                if re.search(pattern, instr):
                    hidden_regs.add(reg)

    # Classification logic
    result = {
        'instructions': len(instructions),
        'labels': labels,
        'has_prologue': has_prologue,
        'first_is_prologue': first_is_prologue,
        'prologue_idx': prologue_idx,
        'has_jr_ra': has_jr_ra,
        'has_jal': has_jal,
        'has_jalr': has_jalr,
        'hidden_regs': hidden_regs,
        'pre_prologue_instrs': instructions[:prologue_idx] if prologue_idx else [],
    }

    # Classify
    if first_is_prologue:
        # Standard function - starts with prologue
        result['type'] = 'standard'
        result['convertible'] = True
        result['reason'] = 'Standard prologue, potentially C-convertible'

    elif not has_prologue:
        # No prologue at all
        if has_jal or has_jalr:
            # Makes calls but no stack frame - unusual, likely requires ASM
            result['type'] = 'no_frame_with_calls'
            result['convertible'] = False
            result['reason'] = 'No stack frame but makes function calls - requires ASM'
        elif has_jr_ra and len(instructions) <= 10:
            # Simple leaf function
            result['type'] = 'leaf'
            result['convertible'] = True
            result['reason'] = 'Leaf function without stack frame - potentially C-convertible'
        elif has_jr_ra:
            # Larger leaf function
            result['type'] = 'leaf_large'
            result['convertible'] = True
            result['reason'] = 'Larger leaf function - potentially C-convertible'
        else:
            # Fragment or unusual
            result['type'] = 'fragment'
            result['convertible'] = False
            result['reason'] = 'Code fragment or unusual pattern'

    else:
        # Has prologue but not first - TRUE THUNK
        pre_prologue_count = prologue_idx

        if pre_prologue_count <= 2:
            # Small thunk (1-2 instructions before prologue)
            result['type'] = 'thunk_small'
            result['convertible'] = False
            result['reason'] = f'{pre_prologue_count} instruction(s) before prologue - requires ASM'
        else:
            # Larger thunk
            result['type'] = 'thunk_large'
            result['convertible'] = False
            result['reason'] = f'{pre_prologue_count} instructions before prologue - requires ASM'

    return result


def main():
    asm_dir = Path('./asm/main/matched')
    asm_files = sorted(asm_dir.glob('*.s'))

    results = defaultdict(list)
    convertible = []
    not_convertible = []

    for filepath in asm_files:
        analysis = parse_asm_file(filepath)
        func_name = filepath.stem
        analysis['name'] = func_name

        results[analysis['type']].append(analysis)

        if analysis.get('convertible', False):
            convertible.append(analysis)
        else:
            not_convertible.append(analysis)

    # Print summary
    print("=" * 80)
    print("REFINED ASM AUDIT RESULTS")
    print("=" * 80)
    print()

    total = len(asm_files)
    print(f"Total ASM files analyzed: {total}")
    print()

    print("CLASSIFICATION BREAKDOWN:")
    print("-" * 60)

    type_order = ['standard', 'leaf', 'leaf_large', 'thunk_small', 'thunk_large',
                  'no_frame_with_calls', 'fragment', 'empty']

    for t in type_order:
        if t in results:
            count = len(results[t])
            pct = count / total * 100
            convertible_str = "C-CONVERTIBLE" if results[t][0].get('convertible', False) else "REQUIRES ASM"
            print(f"  {t:25} : {count:4} ({pct:5.1f}%) - {convertible_str}")

    # Any types not in our expected list
    for t in results:
        if t not in type_order:
            count = len(results[t])
            pct = count / total * 100
            print(f"  {t:25} : {count:4} ({pct:5.1f}%)")

    print()
    print("=" * 80)
    print("SUMMARY")
    print("=" * 80)
    print()
    print(f"POTENTIALLY C-CONVERTIBLE: {len(convertible):4} ({len(convertible)/total*100:.1f}%)")
    print(f"REQUIRES ASM:              {len(not_convertible):4} ({len(not_convertible)/total*100:.1f}%)")
    print()

    # List convertible functions
    print("=" * 80)
    print("POTENTIALLY C-CONVERTIBLE FUNCTIONS")
    print("=" * 80)
    print()

    for cat in ['standard', 'leaf', 'leaf_large']:
        if cat in results:
            print(f"\n### {cat.upper()} ({len(results[cat])} functions)")
            print()
            for r in results[cat][:20]:  # First 20
                print(f"  {r['name']:30} - {r['instructions']:3} instr - {r['reason']}")
            if len(results[cat]) > 20:
                print(f"  ... and {len(results[cat]) - 20} more")

    print()
    print("=" * 80)
    print("REQUIRES ASM - BREAKDOWN")
    print("=" * 80)

    for cat in ['thunk_small', 'thunk_large', 'no_frame_with_calls', 'fragment']:
        if cat in results:
            print(f"\n### {cat.upper()} ({len(results[cat])} functions)")
            print()
            for r in results[cat][:10]:  # First 10
                pre = ', '.join(r.get('pre_prologue_instrs', [])[:3])
                if len(r.get('pre_prologue_instrs', [])) > 3:
                    pre += '...'
                print(f"  {r['name']:30} - {r['reason']}")
                if pre:
                    print(f"    Pre-prologue: {pre}")
            if len(results[cat]) > 10:
                print(f"  ... and {len(results[cat]) - 10} more")

    # Output detailed list
    print()
    print("=" * 80)
    print("FULL LIST OF C-CONVERTIBLE CANDIDATES")
    print("=" * 80)
    print()
    print("| # | Function | Type | Instructions | Notes |")
    print("|---|----------|------|--------------|-------|")

    for i, r in enumerate(convertible, 1):
        print(f"| {i} | {r['name']} | {r['type']} | {r['instructions']} | {r['reason'][:40]} |")

    return results, convertible, not_convertible


if __name__ == '__main__':
    main()
