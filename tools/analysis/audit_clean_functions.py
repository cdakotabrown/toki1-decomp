#!/usr/bin/env python3
"""
Audit "clean" function classifications by analyzing ASM files.

Checks for blockers that prevent C conversion:
1. Cross-function branches (branches outside function's address range)
2. Hidden register inputs (registers used before prologue)
3. Tail calls with live registers
4. Documented as unmatchable
5. Raw .word format
6. Missing or malformed prologue
"""

import json
import re
import os
from pathlib import Path
from typing import Dict, List, Set, Tuple, Optional

# Project root
PROJECT_ROOT = Path(__file__).parent.parent.parent

# Paths
CLASSIFICATION_FILE = PROJECT_ROOT / "config" / "function_classification.json"
ASM_DIR = PROJECT_ROOT / "asm" / "main" / "matched"
SRC_DIR = PROJECT_ROOT / "src" / "main" / "matched"


def parse_address(addr_str: str) -> Optional[int]:
    """Parse address string (0x80041000 or 80041000) to int."""
    if not addr_str:
        return None
    addr_str = addr_str.strip().lower()
    if addr_str.startswith("0x"):
        return int(addr_str, 16)
    try:
        return int(addr_str, 16)
    except ValueError:
        return None


def get_function_info(asm_content: str) -> Tuple[Optional[int], Optional[int], Optional[int]]:
    """
    Extract function address, size, and end address from ASM header.

    Returns: (start_addr, size, end_addr)
    """
    start_addr = None
    size = None

    # Look for "Address: 0xXXXXXXXX" or "/* 0xXXXXXXXX */"
    addr_match = re.search(r'Address:\s*(0x[0-9A-Fa-f]+)', asm_content, re.IGNORECASE)
    if not addr_match:
        # Try alternate format
        addr_match = re.search(r'/\*\s*(0x[0-9A-Fa-f]+)\s*\*/', asm_content)

    if addr_match:
        start_addr = parse_address(addr_match.group(1))

    # Look for "Size: NN bytes"
    size_match = re.search(r'Size:\s*(\d+)\s*bytes?', asm_content, re.IGNORECASE)
    if size_match:
        size = int(size_match.group(1))

    end_addr = None
    if start_addr is not None and size is not None:
        end_addr = start_addr + size

    return start_addr, size, end_addr


def has_cross_function_branches(asm_content: str, start_addr: int, end_addr: int) -> Tuple[bool, List[str]]:
    """
    Check if function has branches/jumps outside its address range.

    Returns: (has_cross_branches, list of problem instructions)

    NOTE: jal (call) is OK - it's a normal function call.
    We're looking for b/beq/bne/j that go outside this function.
    """
    problems = []

    # Match branch instructions (NOT jal - that's a call, not a branch)
    # b/beq/bne/bgtz/blez/bltz/bgez + label or address
    # j (NOT jal) + address
    branch_pattern = re.compile(
        r'^\s*(?:b(?:eq|ne|gt|lt|ge|le|gtz|ltz|lez|gez)?|j)\s+.*?(0x[0-9A-Fa-f]+)',
        re.MULTILINE | re.IGNORECASE
    )

    for match in branch_pattern.finditer(asm_content):
        # Skip if this is a jal (function call)
        if 'jal' in match.group(0):
            continue

        target_str = match.group(1)
        target_addr = parse_address(target_str)

        if target_addr is not None:
            # Check if target is outside function range
            if target_addr < start_addr or target_addr >= end_addr:
                problems.append(match.group(0).strip())

    return len(problems) > 0, problems


def check_prologue_and_hidden_regs(asm_content: str) -> Tuple[str, List[str]]:
    """
    Check function prologue and detect hidden register inputs.

    Returns: (status, list of issues)
    Issues:
    - "no_prologue": No standard addiu sp,sp,- found
    - "code_before_prologue_ok": Has loads before prologue (IDO cross-prologue scheduling, OK)
    - "hidden_regs": Uses registers without initialization (NOT OK)
    """
    issues = []

    # Remove block comments first
    asm_content_no_comments = re.sub(r'/\*.*?\*/', '', asm_content, flags=re.DOTALL)

    # Remove comments and labels to get clean instruction flow
    lines = asm_content_no_comments.split('\n')
    instructions = []

    for line in lines:
        # Remove line comments
        line = re.sub(r'#.*$', '', line)
        line = line.strip()

        # Skip empty lines, labels, directives, standalone comment markers
        if not line or line.endswith(':') or line.startswith('.') or line in ['/*', '*/']:
            continue

        # Skip lines that are clearly not instructions
        if not re.match(r'\w+\s+', line):
            continue

        instructions.append(line)

    if not instructions:
        return "empty", ["No instructions found"]

    # Find prologue (addiu sp,sp,-)
    prologue_idx = None
    for i, instr in enumerate(instructions):
        if re.match(r'addiu\s+\$?sp\s*,\s*\$?sp\s*,\s*-', instr, re.IGNORECASE):
            prologue_idx = i
            break

    if prologue_idx is None:
        # Check if it's a leaf function (no prologue at all)
        # These are OK if they don't use stack
        if any(re.search(r'\b(sw|lw|sd|ld)\s+.*,.*\(\$?sp\)', i, re.IGNORECASE) for i in instructions):
            return "no_prologue", ["Uses stack but has no prologue"]
        # Leaf function, check for hidden register usage
        prologue_idx = 0  # Treat as if prologue is at start

    # Check instructions BEFORE prologue
    before_prologue = instructions[:prologue_idx]

    if before_prologue:
        # These are potentially IDO cross-prologue hoisted loads
        # Check if they're lui/lw/lbu/lh (loads) - these are OK
        all_loads = all(
            re.match(r'(lui|lw|lbu|lh|lb|lhu|lwu|ld)\s+', instr, re.IGNORECASE)
            for instr in before_prologue
        )

        if all_loads:
            # IDO cross-prologue scheduling - OK
            return "code_before_prologue_ok", [f"IDO hoisted loads: {', '.join(before_prologue[:3])}"]
        else:
            # Non-load instructions before prologue - suspicious
            issues.append(f"Non-load before prologue: {before_prologue[0]}")

    # Check for hidden register usage in first ~10 instructions
    # (registers read before written)
    check_range = instructions[prologue_idx:min(prologue_idx + 10, len(instructions))]

    # Track which registers are written to
    written_regs = set()

    # Registers that are OK to read (function args, stack pointer, return addr)
    ok_regs = {'a0', 'a1', 'a2', 'a3', 'sp', 'ra', 'gp', 'zero'}

    for instr in check_range:
        # Extract destination register (first operand for most instructions)
        # Pattern: instr $dest, ...
        dest_match = re.match(r'\w+\s+\$?(\w+)', instr, re.IGNORECASE)
        if dest_match:
            dest_reg = dest_match.group(1).lower()
            written_regs.add(dest_reg)

        # Check for reads of t6/t7/t8/t9/v0/v1/at before they're written
        for suspicious_reg in ['t6', 't7', 't8', 't9', 'v0', 'v1', 'at']:
            if suspicious_reg not in written_regs and suspicious_reg not in ok_regs:
                # Check if this instruction reads the register
                if re.search(rf'\$?{suspicious_reg}\b', instr, re.IGNORECASE):
                    # Verify it's actually a read (appears after first comma or in branch)
                    parts = instr.split(',')
                    if len(parts) > 1:
                        # Check if reg appears in source operands
                        if any(re.search(rf'\$?{suspicious_reg}\b', part, re.IGNORECASE) for part in parts[1:]):
                            issues.append(f"Hidden register ${suspicious_reg}: {instr}")
                            written_regs.add(suspicious_reg)  # Don't report multiple times

    if issues:
        return "hidden_regs", issues

    return "ok", []


def check_raw_word_format(asm_content: str) -> bool:
    """Check if ASM uses raw .word format instead of mnemonics."""
    # Look for .word directives in code section (not just data)
    # Check if >50% of instructions are .word
    lines = asm_content.split('\n')
    in_text = False
    word_count = 0
    instr_count = 0

    for line in lines:
        line = line.strip()
        if '.text' in line or 'glabel' in line:
            in_text = True
            continue
        if '.data' in line or '.rodata' in line:
            in_text = False
            continue

        if in_text and line and not line.startswith('.') and not line.endswith(':'):
            instr_count += 1
            if line.startswith('.word'):
                word_count += 1

    if instr_count == 0:
        return False

    return (word_count / instr_count) > 0.5


def check_documented_unmatchable(asm_content: str) -> Tuple[bool, Optional[str]]:
    """Check if ASM header documents function as unmatchable."""
    unmatchable_keywords = [
        'cannot be matched',
        'requires asm',
        'hand-written',
        'unmatchable',
        'asm only',
        'not convertible',
        'inline asm',
    ]

    # Check first 30 lines for these keywords
    lines = asm_content.split('\n')[:30]
    header = '\n'.join(lines).lower()

    for keyword in unmatchable_keywords:
        if keyword in header:
            return True, keyword

    return False, None


def check_tail_calls(asm_content: str) -> Tuple[bool, List[str]]:
    """
    Check for suspicious tail calls (j instead of jal with live registers).

    Simple heuristic: j to another function is usually OK (tail call optimization).
    But if there are complex register setups before the j, it might be problematic.
    """
    problems = []

    # Look for 'j func_' patterns (tail calls)
    tail_call_pattern = re.compile(r'^\s*j\s+(func_[0-9A-Fa-f]+)', re.MULTILINE | re.IGNORECASE)

    # For now, tail calls are generally OK (compiler optimization)
    # We'd need more sophisticated analysis to detect problems

    return False, problems


def audit_clean_function(func_name: str, func_data: dict) -> dict:
    """
    Audit a single "clean" function.

    Returns dict with:
    - status: truly_clean | cross_function | documented_unmatchable | raw_word_format |
              has_prologue_issue | file_missing | already_matched
    - details: list of specific issues found
    - size: function size in bytes
    """
    result = {
        'name': func_name,
        'status': 'unknown',
        'details': [],
        'size': 0,
        'address': func_data.get('address', '0x00000000'),
    }

    # Check if already matched (has C file)
    c_file = SRC_DIR / f"{func_name}.c"
    if c_file.exists():
        result['status'] = 'already_matched'
        return result

    # Check if ASM file exists
    asm_file = ASM_DIR / f"{func_name}.s"
    if not asm_file.exists():
        result['status'] = 'file_missing'
        result['details'].append(f"ASM file not found: {asm_file}")
        return result

    # Read ASM content
    try:
        asm_content = asm_file.read_text()
    except Exception as e:
        result['status'] = 'file_missing'
        result['details'].append(f"Failed to read ASM: {e}")
        return result

    # Get function address and size
    start_addr, size, end_addr = get_function_info(asm_content)
    if size:
        result['size'] = size

    # Check 1: Documented as unmatchable
    is_unmatchable, keyword = check_documented_unmatchable(asm_content)
    if is_unmatchable:
        result['status'] = 'documented_unmatchable'
        result['details'].append(f"Header contains: '{keyword}'")
        return result

    # Check 2: Raw .word format
    if check_raw_word_format(asm_content):
        result['status'] = 'raw_word_format'
        result['details'].append("Uses .word encoding instead of mnemonics")
        return result

    # Check 3: Cross-function branches
    if start_addr and end_addr:
        has_cross, problems = has_cross_function_branches(asm_content, start_addr, end_addr)
        if has_cross:
            result['status'] = 'cross_function'
            result['details'].extend(problems[:5])  # First 5 problems
            if len(problems) > 5:
                result['details'].append(f"... and {len(problems) - 5} more")
            return result

    # Check 4: Prologue and hidden registers
    prologue_status, issues = check_prologue_and_hidden_regs(asm_content)
    if prologue_status in ['no_prologue', 'hidden_regs']:
        result['status'] = 'has_prologue_issue'
        result['details'].extend(issues[:5])
        return result
    elif prologue_status == 'code_before_prologue_ok':
        # This is OK (IDO cross-prologue scheduling), but note it
        result['details'].append("Has IDO cross-prologue scheduling (OK)")

    # Check 5: Tail calls (currently not blocking)
    has_tail_issues, tail_problems = check_tail_calls(asm_content)
    if has_tail_issues:
        result['details'].extend(tail_problems[:3])

    # If we got here, function appears truly clean
    result['status'] = 'truly_clean'

    return result


def main():
    """Main audit function."""
    print("=== Clean Function Classification Audit ===\n")

    # Load function classification
    if not CLASSIFICATION_FILE.exists():
        print(f"ERROR: Classification file not found: {CLASSIFICATION_FILE}")
        return 1

    with open(CLASSIFICATION_FILE) as f:
        classification = json.load(f)

    # Find all "clean" functions
    clean_functions = {}

    # Handle both old dict format and new list format
    if 'functions' in classification:
        # New format: list of function objects
        for func_data in classification['functions']:
            if 'clean' in func_data.get('categories', []):
                func_name = func_data['name']
                clean_functions[func_name] = func_data
    else:
        # Old format: dict of functions
        for func_name, func_data in classification.items():
            if isinstance(func_data, dict) and func_data.get('classification') == 'clean':
                clean_functions[func_name] = func_data

    print(f"Found {len(clean_functions)} functions classified as 'clean'\n")

    # Audit each function
    results = {}
    status_counts = {}

    for i, (func_name, func_data) in enumerate(clean_functions.items(), 1):
        if i % 20 == 0:
            print(f"Progress: {i}/{len(clean_functions)}...", end='\r')

        result = audit_clean_function(func_name, func_data)
        results[func_name] = result

        status = result['status']
        status_counts[status] = status_counts.get(status, 0) + 1

    print(f"Progress: {len(clean_functions)}/{len(clean_functions)} - Done!\n")

    # Print summary
    print("=== AUDIT RESULTS ===\n")
    print("Status counts:")
    for status in sorted(status_counts.keys()):
        count = status_counts[status]
        pct = (count / len(clean_functions)) * 100
        print(f"  {status:25s}: {count:4d} ({pct:5.1f}%)")

    print(f"\n  {'TOTAL':25s}: {len(clean_functions):4d}")

    # Show truly clean functions sorted by size
    truly_clean = [r for r in results.values() if r['status'] == 'truly_clean']
    if truly_clean:
        print(f"\n=== TRULY CLEAN FUNCTIONS ({len(truly_clean)}) ===")
        print("(Sorted by size - smallest first)\n")

        truly_clean_sorted = sorted(truly_clean, key=lambda x: x['size'])

        print(f"{'Function':<30s} {'Address':<12s} {'Size':>6s}  Details")
        print("-" * 80)

        for r in truly_clean_sorted[:50]:  # Show first 50
            details = r['details'][0] if r['details'] else ''
            if len(details) > 35:
                details = details[:32] + "..."
            print(f"{r['name']:<30s} {r['address']:<12s} {r['size']:5d}B  {details}")

        if len(truly_clean_sorted) > 50:
            print(f"\n... and {len(truly_clean_sorted) - 50} more")

    # Show sample problems from each category
    print("\n=== SAMPLE ISSUES BY CATEGORY ===\n")

    for status in ['cross_function', 'has_prologue_issue', 'documented_unmatchable', 'raw_word_format']:
        funcs = [r for r in results.values() if r['status'] == status]
        if funcs:
            print(f"{status} ({len(funcs)} functions):")
            for r in funcs[:3]:  # Show 3 examples
                print(f"  {r['name']}:")
                for detail in r['details'][:2]:  # First 2 details
                    print(f"    - {detail}")
            if len(funcs) > 3:
                print(f"  ... and {len(funcs) - 3} more")
            print()

    # Save detailed results
    output_file = PROJECT_ROOT / "build" / "clean_function_audit.json"
    output_file.parent.mkdir(exist_ok=True)

    with open(output_file, 'w') as f:
        json.dump(results, f, indent=2)

    print(f"\nDetailed results saved to: {output_file}")

    return 0


if __name__ == '__main__':
    exit(main())
