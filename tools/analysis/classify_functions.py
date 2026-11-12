#!/usr/bin/env python3
"""
Classify functions as hand-written vs compiler-generated.

Criteria for likely HAND-WRITTEN:
1. Uses $t registers across JAL calls (violates calling convention)
2. Perfect delay slot filling (no NOPs after branches/jumps)
3. Unusual stack frame sizes (much larger than needed)
4. Uses unaligned access (lwl/lwr/swl/swr)

Criteria for likely COMPILER-GENERATED:
1. Saves $s registers when using them
2. Has NOPs after branches (imperfect scheduling)
3. Stack frame matches local variable needs
"""

import json
import re
import sys

def analyze_function(func):
    """Analyze a function and classify it."""
    name = func['name']
    disasm = func.get('disassembly', [])
    size = func.get('size', 0)

    # Parse disassembly
    instrs = []
    for line in disasm:
        # Format: "offset:\thex \tinstruction"
        match = re.match(r'[0-9a-f]+:\s*[0-9a-f]+\s+(\w+)\s*(.*)', line)
        if match:
            instrs.append((match.group(1), match.group(2)))

    # Flags
    has_jal = False
    jal_count = 0
    nop_after_branch = 0
    branch_count = 0
    uses_t_regs = set()
    uses_s_regs = set()
    saves_s_regs = False
    has_unaligned = False
    stack_frame = 0
    ra_offset = None

    t_regs = {'t0', 't1', 't2', 't3', 't4', 't5', 't6', 't7', 't8', 't9'}
    s_regs = {'s0', 's1', 's2', 's3', 's4', 's5', 's6', 's7'}
    branch_ops = {'beq', 'bne', 'bgez', 'bgtz', 'blez', 'bltz', 'j', 'jr', 'jal', 'jalr'}

    prev_was_branch = False

    for i, (op, args) in enumerate(instrs):
        # Check for JAL
        if op == 'jal':
            has_jal = True
            jal_count += 1

        # Check for branches
        if op in branch_ops:
            branch_count += 1
            prev_was_branch = True
        elif prev_was_branch:
            if op == 'nop':
                nop_after_branch += 1
            prev_was_branch = False

        # Check register usage
        for reg in t_regs:
            if reg in args or f'${reg[1:]}' in args:
                uses_t_regs.add(reg)
        for reg in s_regs:
            if reg in args or f'${reg[1:]}' in args:
                uses_s_regs.add(reg)

        # Check for $s saves (sw $s0, offset(sp))
        if op == 'sw' and any(f's{i}' in args or f'${16+i}' in args for i in range(8)):
            if 'sp' in args:
                saves_s_regs = True

        # Check for unaligned access
        if op in ('lwl', 'lwr', 'swl', 'swr'):
            has_unaligned = True

        # Extract stack frame size
        if op == 'addiu' and 'sp,sp,-' in args.replace(' ', ''):
            match = re.search(r'-(\d+)', args)
            if match:
                stack_frame = int(match.group(1))

        # Find $ra save offset
        if op == 'sw' and 'ra' in args and 'sp' in args:
            match = re.search(r'(\d+)\(', args)
            if match:
                ra_offset = int(match.group(1))

    # Calculate minimum needed stack (rough estimate)
    # 16 for args + 4 per saved reg + local variables
    min_stack_estimate = 16 + (4 if has_jal else 0) + (len(uses_s_regs) * 4 if saves_s_regs else 0)

    # Classification logic
    reasons = []
    hand_written_score = 0

    # Check for $t regs across calls (major red flag)
    if has_jal and uses_t_regs and jal_count > 1:
        hand_written_score += 3
        reasons.append(f"Uses $t regs ({uses_t_regs}) across {jal_count} JAL calls")

    # Perfect delay slots (no NOPs)
    if branch_count > 0 and nop_after_branch == 0:
        hand_written_score += 1
        reasons.append("Perfect delay slot filling (no NOPs)")

    # Many NOPs suggest compiler-generated
    if nop_after_branch > 0:
        hand_written_score -= 1
        reasons.append(f"{nop_after_branch} NOPs after branches (compiler)")

    # Oversized stack frame
    if stack_frame > 0 and min_stack_estimate > 0:
        if stack_frame > min_stack_estimate * 2:
            hand_written_score += 2
            reasons.append(f"Oversized stack: {stack_frame} vs ~{min_stack_estimate} needed")

    # Unaligned access
    if has_unaligned:
        hand_written_score += 2
        reasons.append("Uses unaligned access (lwl/lwr)")

    # Uses $s but doesn't save them
    if uses_s_regs and not saves_s_regs and has_jal:
        hand_written_score += 1
        reasons.append("Uses $s regs without saving across calls")

    # Classify
    if hand_written_score >= 3:
        classification = "HAND_WRITTEN"
    elif hand_written_score >= 1:
        classification = "POSSIBLY_HAND"
    elif hand_written_score <= -1:
        classification = "LIKELY_COMPILER"
    else:
        classification = "UNKNOWN"

    return {
        'name': name,
        'classification': classification,
        'score': hand_written_score,
        'reasons': reasons,
        'stats': {
            'size': size,
            'jal_count': jal_count,
            'stack_frame': stack_frame,
            't_regs': list(uses_t_regs),
            's_regs': list(uses_s_regs),
            'saves_s': saves_s_regs,
            'nops_after_branch': nop_after_branch,
            'has_unaligned': has_unaligned
        }
    }

def main():
    # Load function map
    with open('build/function_analysis/function_map.json') as f:
        data = json.load(f)

    results = {
        'HAND_WRITTEN': [],
        'POSSIBLY_HAND': [],
        'LIKELY_COMPILER': [],
        'UNKNOWN': []
    }

    for func in data['functions']:
        analysis = analyze_function(func)
        results[analysis['classification']].append(analysis)

    # Print summary
    print("=" * 70)
    print("FUNCTION CLASSIFICATION SUMMARY")
    print("=" * 70)
    print(f"HAND_WRITTEN:    {len(results['HAND_WRITTEN']):4d} functions (use INCLUDE_ASM)")
    print(f"POSSIBLY_HAND:   {len(results['POSSIBLY_HAND']):4d} functions (try C, fallback to ASM)")
    print(f"LIKELY_COMPILER: {len(results['LIKELY_COMPILER']):4d} functions (good C candidates)")
    print(f"UNKNOWN:         {len(results['UNKNOWN']):4d} functions (need manual review)")
    print(f"{'':14s}   ----")
    print(f"TOTAL:           {sum(len(v) for v in results.values()):4d} functions")
    print()

    # Print top hand-written examples
    print("=" * 70)
    print("TOP HAND-WRITTEN FUNCTIONS (highest confidence)")
    print("=" * 70)
    sorted_hw = sorted(results['HAND_WRITTEN'], key=lambda x: -x['score'])
    for analysis in sorted_hw[:20]:
        print(f"\n{analysis['name']} (score: {analysis['score']})")
        print(f"  Stack: {analysis['stats']['stack_frame']} bytes, JALs: {analysis['stats']['jal_count']}")
        for reason in analysis['reasons']:
            print(f"  - {reason}")

    # Print top compiler-generated examples
    print()
    print("=" * 70)
    print("TOP COMPILER-GENERATED FUNCTIONS (best C candidates)")
    print("=" * 70)
    sorted_cg = sorted(results['LIKELY_COMPILER'], key=lambda x: x['score'])
    for analysis in sorted_cg[:20]:
        print(f"\n{analysis['name']} (score: {analysis['score']})")
        print(f"  Stack: {analysis['stats']['stack_frame']} bytes, JALs: {analysis['stats']['jal_count']}")
        for reason in analysis['reasons']:
            print(f"  - {reason}")

    # Save full results
    output_file = 'build/function_analysis/function_classification.json'
    with open(output_file, 'w') as f:
        json.dump(results, f, indent=2)
    print(f"\nFull results saved to: {output_file}")

if __name__ == '__main__':
    main()
