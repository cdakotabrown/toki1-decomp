#!/usr/bin/env python3
"""
Analyze register usage in target assembly to predict IDO register allocation issues.

Usage:
    python3 analyze_register_usage.py 0x800XXXXX
"""

import sys
import argparse
import re
from pathlib import Path
import subprocess

def get_function_asm(address):
    """Get function assembly using extract_function_asm.py."""
    try:
        result = subprocess.run(
            ['python3', 'tools/analysis/extract_function_asm.py',
             f'func_{address[2:]}'],  # Remove 0x prefix
            capture_output=True, text=True, check=False
        )
        return result.stdout.split('\n') if result.returncode == 0 else []
    except Exception as e:
        print(f"Error extracting function: {e}", file=sys.stderr)
        return []


def analyze_registers(asm_lines):
    """Analyze register usage patterns in assembly."""

    patterns = {
        'loop_counter_v1': False,
        'loop_counter_v0': False,
        'comparison_at': False,
        'comparison_v0': False,
        'temp_t6': False,
        'temp_t7': False,
        'temp_t8': False,
        'uses_v1': False,
        'move_or_encoding': False,
        'move_addu_encoding': False,
        'frame_size': None,
        'ra_offset': None,
    }

    issues = []
    suggestions = []

    for line in asm_lines:
        # Extract instruction
        if ':' not in line:
            continue

        parts = line.split(':')
        if len(parts) < 2:
            continue

        inst = parts[1].strip()

        # Check frame size
        if 'addiu' in inst and 'sp,sp,-' in inst:
            match = re.search(r'addiu\s+sp,sp,(-?\d+)', inst)
            if match:
                patterns['frame_size'] = abs(int(match.group(1)))

        # Check RA offset
        if 'sw' in inst and 'ra,' in inst and 'sp)' in inst:
            match = re.search(r'ra,(\d+)\(sp\)', inst)
            if match:
                patterns['ra_offset'] = int(match.group(1))

        # Check for sltu usage
        if 'sltu' in inst:
            if re.search(r'sltu\s+at,', inst):
                patterns['comparison_at'] = True
            elif re.search(r'sltu\s+v0,', inst):
                patterns['comparison_v0'] = True

        # Check for v1 usage (potential loop counter)
        if re.search(r'\bv1\b', inst):
            patterns['uses_v1'] = True
            if 'addiu' in inst and 'v1,v1,' in inst:
                patterns['loop_counter_v1'] = True

        if re.search(r'\bv0\b', inst):
            if 'addiu' in inst and 'v0,v0,' in inst:
                patterns['loop_counter_v0'] = True

        # Check temp register usage
        if re.search(r'\bt6\b', inst):
            patterns['temp_t6'] = True
        if re.search(r'\bt7\b', inst):
            patterns['temp_t7'] = True
        if re.search(r'\bt8\b', inst):
            patterns['temp_t8'] = True

        # Check move encoding (or vs addu)
        if re.search(r'or\s+', inst) and 'zero' in inst:
            patterns['move_or_encoding'] = True
        if re.search(r'addu\s+', inst) and 'zero' in inst:
            patterns['move_addu_encoding'] = True

    # Analyze patterns and generate suggestions

    if patterns['comparison_v0']:
        issues.append("⚠️  Uses 'sltu v0,...' for comparisons")
        suggestions.append("IDO likely produces 'sltu at,...' → Use swap_registers.py --from at --to v0")

    if patterns['loop_counter_v1']:
        issues.append("ℹ️  Uses v1 as loop counter")
        suggestions.append("IDO may optimize to pointer comparison using at register")

    if patterns['frame_size'] and patterns['frame_size'] != 24:
        issues.append(f"⚠️  Frame size: {patterns['frame_size']} bytes")
        if patterns['frame_size'] == 40:
            suggestions.append(f"IDO produces 24-byte frames → Use patch_frame_size.py --old 24 --new {patterns['frame_size']}")
        else:
            suggestions.append(f"Non-standard frame size → May need frame patching")

    if patterns['ra_offset'] and patterns['ra_offset'] != 20:
        issues.append(f"⚠️  RA offset: {patterns['ra_offset']} (not standard 20)")
        suggestions.append(f"IDO uses offset 20 → Use patch_ra_offset.py --old 20 --new {patterns['ra_offset']}")

    if patterns['move_addu_encoding']:
        issues.append("ℹ️  Uses ADDU encoding for 'move' instruction")
        suggestions.append("Check address range: 0x80086000+ uses ADDU, below uses OR")
        suggestions.append("If needed: convert_or_to_addu.py")

    if patterns['move_or_encoding']:
        issues.append("ℹ️  Uses OR encoding for 'move' instruction")
        suggestions.append("This is IDO's default encoding (no conversion needed)")

    if patterns['temp_t6'] or patterns['temp_t7'] or patterns['temp_t8']:
        temp_regs = []
        if patterns['temp_t6']:
            temp_regs.append('t6')
        if patterns['temp_t7']:
            temp_regs.append('t7')
        if patterns['temp_t8']:
            temp_regs.append('t8')
        issues.append(f"ℹ️  Uses temp registers: {', '.join(temp_regs)}")
        suggestions.append("IDO's temp register choice is unpredictable → May need swap_registers.py")

    return patterns, issues, suggestions


def print_analysis(address, asm_lines):
    """Print register usage analysis."""

    print(f"\n{'='*70}")
    print(f"Register Usage Analysis for {address}")
    print(f"{'='*70}\n")

    patterns, issues, suggestions = analyze_registers(asm_lines)

    # Print detected patterns
    print("Detected Patterns:")
    print("-" * 70)

    if patterns['frame_size']:
        print(f"  Stack frame: {patterns['frame_size']} bytes")

    if patterns['ra_offset']:
        print(f"  RA offset: {patterns['ra_offset']} bytes")

    if patterns['uses_v1']:
        print(f"  Uses v1: Yes (loop counter: {patterns['loop_counter_v1']})")

    if patterns['comparison_at']:
        print(f"  Comparison register: $at")
    elif patterns['comparison_v0']:
        print(f"  Comparison register: v0")

    if patterns['move_or_encoding']:
        print(f"  Move encoding: OR (0x25)")
    elif patterns['move_addu_encoding']:
        print(f"  Move encoding: ADDU (0x21)")

    print()

    # Print issues
    if issues:
        print("Potential Matching Issues:")
        print("-" * 70)
        for issue in issues:
            print(f"  {issue}")
        print()

    # Print suggestions
    if suggestions:
        print("Suggested Post-Processing:")
        print("-" * 70)
        for i, suggestion in enumerate(suggestions, 1):
            print(f"  {i}. {suggestion}")
        print()

    # Print recommended workflow
    print("Recommended Matching Workflow:")
    print("-" * 70)
    print("  1. Write C code matching instruction sequence")
    print("  2. Compile: ./tools/ido-static-recomp/build/5.2/out/cc -O2 -G0 -mips1 -EL -non_shared")
    print("  3. Apply post-processing as suggested above")
    print("  4. Compare with: mipsel-linux-gnu-objdump -d output.o")
    print()

    print(f"{'='*70}\n")


def main():
    parser = argparse.ArgumentParser(
        description="Analyze register usage in target assembly",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python3 analyze_register_usage.py 0x80088C44
  python3 analyze_register_usage.py 0x80086590
        """
    )

    parser.add_argument('address',
                        help='Function address (e.g., 0x80088C44)')

    args = parser.parse_args()

    # Get assembly
    asm_lines = get_function_asm(args.address)

    if not asm_lines:
        print(f"Error: Could not extract function at {args.address}", file=sys.stderr)
        return 1

    print_analysis(args.address, asm_lines)

    return 0


if __name__ == '__main__':
    sys.exit(main())
