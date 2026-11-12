#!/usr/bin/env python3
"""
find_call_mismatches.py - Find function calls with wrong argument counts

This script scans overlay C files and compares call site argument counts
against the ASM to detect "missing argument" bugs that add padding instructions.

Usage:
    python3 tools/analysis/find_call_mismatches.py [--fix] [--verbose]

The script detects patterns where:
- C code calls func_XXXXXXXX() with 0 arguments
- But ASM shows arguments being set up (lui+addiu for pointers, addiu for integers)
"""

import os
import re
import sys
import argparse
from pathlib import Path
from collections import defaultdict
from typing import Dict, List, Tuple, Optional, Set

# Project root
PROJECT_ROOT = Path(__file__).parent.parent.parent


def parse_args():
    parser = argparse.ArgumentParser(
        description="Find function calls with wrong argument counts"
    )
    parser.add_argument(
        "--fix", action="store_true",
        help="Show suggested fixes (not auto-applied)"
    )
    parser.add_argument(
        "--verbose", "-v", action="store_true",
        help="Show detailed analysis"
    )
    parser.add_argument(
        "--overlay", type=str, default=None,
        help="Analyze only specific overlay (e.g., GYOZI)"
    )
    parser.add_argument(
        "--function", type=str, default=None,
        help="Analyze only specific function (e.g., func_80132660)"
    )
    parser.add_argument(
        "--generate-header", type=str, default=None,
        help="Generate header file with correct extern declarations"
    )
    parser.add_argument(
        "--summary-only", action="store_true",
        help="Only show summary, not detailed report"
    )
    return parser.parse_args()


def find_overlay_c_files(overlay_filter: Optional[str] = None) -> List[Path]:
    """Find all overlay C files."""
    overlay_dir = PROJECT_ROOT / "src" / "overlay"
    c_files = []

    for overlay_path in overlay_dir.iterdir():
        if not overlay_path.is_dir():
            continue
        if overlay_filter and overlay_path.name != overlay_filter:
            continue
        for c_file in overlay_path.glob("*.c"):
            c_files.append(c_file)

    return sorted(c_files)


def get_asm_path(c_file: Path) -> Path:
    """Get corresponding ASM file path for a C file."""
    # src/overlay/GYOZI/func_XXXX.c -> asm/overlay/GYOZI/func_XXXX.s
    overlay_name = c_file.parent.name
    func_name = c_file.stem
    return PROJECT_ROOT / "asm" / "overlay" / overlay_name / f"{func_name}.s"


def extract_function_calls(c_content: str) -> List[Tuple[int, str, int]]:
    """
    Extract function calls from C code.
    Returns list of (line_number, function_name, arg_count).
    """
    calls = []

    # Pattern for function calls: func_XXXXXXXX(args)
    # We need to handle multi-line calls carefully
    pattern = r'\b(func_[0-9A-Fa-f]{8})\s*\(([^)]*)\)'

    for match in re.finditer(pattern, c_content, re.DOTALL):
        func_name = match.group(1)
        args_str = match.group(2).strip()

        # Find line number
        line_num = c_content[:match.start()].count('\n') + 1

        # Count arguments
        if not args_str:
            arg_count = 0
        else:
            # Handle nested parentheses by counting commas at depth 0
            depth = 0
            commas = 0
            for char in args_str:
                if char == '(':
                    depth += 1
                elif char == ')':
                    depth -= 1
                elif char == ',' and depth == 0:
                    commas += 1
            arg_count = commas + 1

        calls.append((line_num, func_name, arg_count))

    return calls


def analyze_asm_for_call(asm_content: str, target_func: str) -> Tuple[int, List[str]]:
    """
    Analyze ASM to determine how many arguments are set up before a jal call.
    Returns (arg_count, list of argument setup descriptions).

    MIPS calling convention: a0, a1, a2, a3 are the first 4 arguments.
    """
    lines = asm_content.split('\n')

    # Find all jal instructions to the target function
    call_sites = []
    for i, line in enumerate(lines):
        # Match jal to the target function
        # Format: jal 0xXXXXXXXX or jal func_name
        jal_match = re.search(r'jal\s+(0x[0-9A-Fa-f]+|' + re.escape(target_func) + r')\b', line)
        if jal_match:
            addr_or_name = jal_match.group(1)
            # Check if this is our target
            if addr_or_name.startswith('0x'):
                # Convert address to function name format
                addr = int(addr_or_name, 16)
                expected_name = f"func_{addr:08X}"
                if expected_name.lower() == target_func.lower():
                    call_sites.append(i)
            elif addr_or_name == target_func:
                call_sites.append(i)

    if not call_sites:
        return (0, [])

    # Analyze the first call site (they should all have the same signature)
    call_line = call_sites[0]

    # Look backwards for argument setup (max 20 lines back)
    # Track which argument registers are set
    arg_regs = {'a0': None, 'a1': None, 'a2': None, 'a3': None}
    arg_descriptions = []

    start_line = max(0, call_line - 20)

    # Also check the delay slot (line after jal)
    check_lines = list(range(start_line, call_line + 2))

    # Track partial lui values for address construction
    lui_values = {}  # reg -> high value

    for i in check_lines:
        if i >= len(lines):
            continue
        line = lines[i].strip()

        # Skip empty lines, labels, comments
        if not line or line.startswith('#') or line.endswith(':'):
            continue

        # Pattern: lui $aX, 0xXXXX (high part of address)
        lui_match = re.match(r'lui\s+\$(\w+),\s*(0x[0-9A-Fa-f]+|\d+)', line)
        if lui_match:
            reg = lui_match.group(1)
            val = lui_match.group(2)
            if val.startswith('0x'):
                lui_values[reg] = int(val, 16)
            else:
                lui_values[reg] = int(val)

        # Pattern: addiu $aX, $aX, imm (low part of address, or reg copy)
        addiu_match = re.match(r'addiu\s+\$(\w+),\s*\$(\w+),\s*(-?\d+|0x[0-9A-Fa-f]+)', line)
        if addiu_match:
            dst = addiu_match.group(1)
            src = addiu_match.group(2)
            imm = addiu_match.group(3)

            if dst in arg_regs:
                if src == dst and dst in lui_values:
                    # This completes a pointer: lui + addiu pattern
                    if imm.startswith('0x') or imm.startswith('-0x'):
                        low = int(imm, 16)
                    elif imm.startswith('-'):
                        low = int(imm)
                    else:
                        low = int(imm)
                    # Handle signed extension
                    if low < 0:
                        low = low & 0xFFFF
                        if low >= 0x8000:
                            low = low - 0x10000
                    high = lui_values[dst]
                    addr = (high << 16) + low
                    arg_regs[dst] = f"D_0x{addr:08X}"
                elif src == 'zero':
                    # addiu $aX, $zero, imm - integer constant
                    if imm.startswith('0x'):
                        val = int(imm, 16)
                    else:
                        val = int(imm)
                    arg_regs[dst] = f"{val}"

        # Pattern: or $aX, $zero, $zero (set to 0)
        or_zero_match = re.match(r'or\s+\$(\w+),\s*\$zero,\s*\$zero', line)
        if or_zero_match:
            dst = or_zero_match.group(1)
            if dst in arg_regs:
                arg_regs[dst] = "0"

        # Pattern: or $aX, $vY, $zero (copy from return value)
        or_copy_match = re.match(r'or\s+\$(\w+),\s*\$(\w+),\s*\$zero', line)
        if or_copy_match:
            dst = or_copy_match.group(1)
            src = or_copy_match.group(2)
            if dst in arg_regs and src != 'zero':
                arg_regs[dst] = f"${src}"

        # Pattern: lw/lh/lb $aX, offset($reg) - loaded from memory
        load_match = re.match(r'(lw|lh|lb|lhu|lbu)\s+\$(\w+),\s*(-?\d+)\(\$(\w+)\)', line)
        if load_match:
            dst = load_match.group(2)
            if dst in arg_regs:
                arg_regs[dst] = f"<loaded>"

    # Count how many argument registers were set
    arg_count = 0
    for i, reg in enumerate(['a0', 'a1', 'a2', 'a3']):
        if arg_regs[reg] is not None:
            arg_count = i + 1
            arg_descriptions.append(f"{reg}={arg_regs[reg]}")

    return (arg_count, arg_descriptions)


def format_suggested_fix(func_name: str, arg_descriptions: List[str]) -> str:
    """Format a suggested function call with arguments."""
    args = []
    for desc in arg_descriptions:
        if desc.startswith('a') and '=' in desc:
            val = desc.split('=')[1]
            if val.startswith('D_0x'):
                # Pointer - cast to s32
                sym = val.replace('0x', '')
                args.append(f"(s32)&D_{sym[2:]}")
            elif val.startswith('$'):
                args.append(val[1:])  # Use variable name
            else:
                args.append(val)

    return f"{func_name}({', '.join(args)})"


def analyze_file(c_file: Path, verbose: bool = False) -> List[Dict]:
    """Analyze a single C file for call mismatches."""
    mismatches = []

    asm_file = get_asm_path(c_file)
    if not asm_file.exists():
        if verbose:
            print(f"  Warning: ASM file not found: {asm_file}")
        return []

    try:
        c_content = c_file.read_text()
        asm_content = asm_file.read_text()
    except Exception as e:
        if verbose:
            print(f"  Error reading files: {e}")
        return []

    # Extract function calls from C
    calls = extract_function_calls(c_content)

    # Filter to only zero-argument calls to main binary functions
    # (addresses 0x80040000 - 0x800BFFFF are main binary)
    zero_arg_calls = []
    for line_num, func_name, arg_count in calls:
        try:
            addr = int(func_name.replace('func_', ''), 16)
        except ValueError:
            continue

        # Main binary address range
        if 0x80040000 <= addr <= 0x800BFFFF and arg_count == 0:
            zero_arg_calls.append((line_num, func_name))

    # Check each zero-arg call against ASM
    for line_num, func_name in zero_arg_calls:
        asm_arg_count, arg_descriptions = analyze_asm_for_call(asm_content, func_name)

        if asm_arg_count > 0:
            mismatches.append({
                'c_file': c_file,
                'line_num': line_num,
                'func_name': func_name,
                'c_args': 0,
                'asm_args': asm_arg_count,
                'arg_descriptions': arg_descriptions,
            })

    return mismatches


def main():
    args = parse_args()

    print("=" * 70)
    print("Function Call Mismatch Analyzer")
    print("=" * 70)
    print()

    # Find all overlay C files
    c_files = find_overlay_c_files(args.overlay)

    if args.function:
        c_files = [f for f in c_files if f.stem == args.function]

    if not c_files:
        print("No C files found matching criteria")
        return 1

    print(f"Analyzing {len(c_files)} overlay C files...")
    print()

    all_mismatches = []
    files_with_issues = 0

    for c_file in c_files:
        if args.verbose:
            print(f"Analyzing: {c_file.relative_to(PROJECT_ROOT)}")

        mismatches = analyze_file(c_file, args.verbose)

        if mismatches:
            files_with_issues += 1
            all_mismatches.extend(mismatches)

    # Report results
    print("=" * 70)
    print("RESULTS")
    print("=" * 70)
    print()

    if not all_mismatches:
        print("No call mismatches found!")
        return 0

    print(f"Found {len(all_mismatches)} call mismatches in {files_with_issues} files:")
    print()

    # Group by called function to show patterns
    by_func = defaultdict(list)
    for m in all_mismatches:
        by_func[m['func_name']].append(m)

    for func_name in sorted(by_func.keys()):
        calls = by_func[func_name]
        print(f"  {func_name}: {len(calls)} call sites with 0 args, ASM shows {calls[0]['asm_args']} args")

    if not args.summary_only:
        print()
        print("-" * 70)
        print("DETAILED REPORT")
        print("-" * 70)
        print()

        for m in sorted(all_mismatches, key=lambda x: (str(x['c_file']), x['line_num'])):
            rel_path = m['c_file'].relative_to(PROJECT_ROOT)
            print(f"{rel_path}:{m['line_num']} - {m['func_name']}() called with 0 args, ASM shows {m['asm_args']} args")

            if args.fix and m['arg_descriptions']:
                suggested = format_suggested_fix(m['func_name'], m['arg_descriptions'])
                print(f"  Suggested: {suggested}")

            if args.verbose and m['arg_descriptions']:
                print(f"  ASM args: {', '.join(m['arg_descriptions'])}")

            print()

    # Summary
    print("=" * 70)
    print("SUMMARY")
    print("=" * 70)
    print(f"Total mismatches: {len(all_mismatches)}")
    print(f"Unique functions: {len(by_func)}")
    print(f"Files affected: {files_with_issues}")
    print()
    print("Each mismatch adds 2-4 instructions of padding to the compiled output.")
    print("Fix by adding proper extern declarations and arguments to calls.")

    # Generate header file if requested
    if args.generate_header:
        generate_header_file(args.generate_header, by_func)

    return 0


def generate_header_file(output_path: str, by_func: Dict[str, List[Dict]]) -> None:
    """Generate a header file with correct extern declarations."""
    lines = [
        "/* overlay_func_signatures.h - Auto-generated function signatures",
        " * Generated by find_call_mismatches.py",
        " * ",
        " * These functions are called from overlays with the wrong number of arguments.",
        " * Include this header in overlay C files to get correct prototypes.",
        " */",
        "",
        "#ifndef OVERLAY_FUNC_SIGNATURES_H",
        "#define OVERLAY_FUNC_SIGNATURES_H",
        "",
        "#include \"types.h\"",
        "",
    ]

    for func_name in sorted(by_func.keys()):
        calls = by_func[func_name]
        arg_count = calls[0]['asm_args']

        # Generate argument list based on count
        if arg_count == 0:
            arg_list = "void"
        else:
            args = [f"s32 arg{i}" for i in range(arg_count)]
            arg_list = ", ".join(args)

        lines.append(f"extern s32 {func_name}({arg_list});")

    lines.extend([
        "",
        "#endif /* OVERLAY_FUNC_SIGNATURES_H */",
        "",
    ])

    output = Path(output_path)
    output.write_text("\n".join(lines))
    print(f"\nGenerated header file: {output_path}")


if __name__ == "__main__":
    sys.exit(main())
