#!/usr/bin/env python3
"""
Categorize ASM functions for C conversion potential.

Analyzes all ASM files in asm/main/matched/ and categorizes them by:
- Hidden register usage (t6, t7, at, v0, v1 used without being set)
- Thunk patterns (code before prologue)
- Complexity (size, calls, branches)
- Already has C source

Outputs JSON with categorization for each function.
"""

import argparse
import json
import re
from pathlib import Path
from dataclasses import dataclass, asdict
from typing import List, Dict, Set, Optional


@dataclass
class FunctionAnalysis:
    address: str
    file: str
    size: int
    region: str  # "gcc" or "ido"
    num_calls: int
    num_branches: int
    has_c_source: bool

    # Categorization
    category: str  # "c_convertible", "hidden_reg", "thunk", "complex", "unknown"
    hidden_regs: List[str]  # Registers used without being set
    issues: List[str]  # Specific issues found

    # From comments
    comment_flags: List[str]  # Flags found in file comments


# Registers that might be "hidden" (set by caller)
CALLER_REGS = {'$t6', '$t7', '$t8', '$t9', '$at', '$v0', '$v1'}

# Patterns indicating hidden register usage in comments
HIDDEN_COMMENT_PATTERNS = [
    r'preceding\s+code',
    r'from\s+thunk',
    r'hidden',
    r'inter.?procedural',
    r'continuation',
    r'previous\s+code',
    r'requires\s+assembly',
    r'before\s+prologue',
    r'pre.?prologue',
    r'thunk\s+function',
    r'uses?\s+\$?[tvask]\d+.*from',
    r'caller\s+sets?',
    r'implicit\s+(input|param)',
]


def parse_asm_file(filepath: Path) -> Dict:
    """Parse an ASM file and extract analysis data."""
    content = filepath.read_text()
    lines = content.split('\n')

    result = {
        'size': 0,
        'num_calls': 0,
        'num_branches': 0,
        'comment_flags': [],
        'first_instr': None,
        'regs_used': set(),
        'regs_set': set(),
        'has_pre_prologue': False,
    }

    # Parse size from header
    size_match = re.search(r'[#\*]\s*Size:\s*(\d+)\s*bytes', content, re.IGNORECASE)
    if size_match:
        result['size'] = int(size_match.group(1))

    # Check comment patterns
    content_lower = content.lower()
    for pattern in HIDDEN_COMMENT_PATTERNS:
        if re.search(pattern, content_lower):
            result['comment_flags'].append(pattern)

    # Parse instructions
    in_code = False
    prologue_seen = False

    for line in lines:
        line_stripped = line.strip()

        # Skip empty lines and pure comments
        if not line_stripped:
            continue
        if line_stripped.startswith(('#', '/*', '*', '//')):
            continue
        if line_stripped.startswith('.'):
            if line_stripped.startswith('.globl') or line_stripped.startswith('.global'):
                in_code = True
            continue
        if line_stripped.endswith(':'):
            continue

        if not in_code:
            continue

        # This is an instruction
        instr = line_stripped.split('#')[0].strip()  # Remove inline comments
        instr = instr.split('/*')[0].strip()

        if not instr:
            continue

        # Check if this is the prologue
        if 'addiu' in instr and '$sp' in instr and '-' in instr:
            if not prologue_seen:
                prologue_seen = True
                if result['first_instr'] is not None:
                    result['has_pre_prologue'] = True

        # Record first instruction
        if result['first_instr'] is None:
            result['first_instr'] = instr

        # Count calls and branches
        if re.match(r'\s*jal\s', instr):
            result['num_calls'] += 1
        if re.match(r'\s*(beq|bne|bgtz|bltz|blez|bgez|bc1f|bc1t|b\s)\s', instr):
            result['num_branches'] += 1

        # Track register usage (simplified)
        # Registers used as source (after comma or in parens)
        for reg in CALLER_REGS:
            # Used as source: appears after a comma, or in parentheses, or as second/third operand
            if re.search(rf',\s*{re.escape(reg)}(?:\s|,|$|\))', instr):
                result['regs_used'].add(reg)
            if re.search(rf'\({re.escape(reg)}\)', instr):
                result['regs_used'].add(reg)
            # Set as destination: first operand after instruction
            if re.search(rf'^\s*\w+\s+{re.escape(reg)}\s*,', instr):
                result['regs_set'].add(reg)

    return result


def analyze_function(asm_file: Path, c_files: Set[str]) -> FunctionAnalysis:
    """Analyze a single ASM function."""
    # Extract address from filename
    name = asm_file.stem
    addr_match = re.match(r'func_([0-9A-Fa-f]+)', name)
    if not addr_match:
        return None

    addr = int(addr_match.group(1), 16)
    addr_str = f"0x{addr:08X}"

    # Determine region
    region = "gcc" if addr < 0x80086000 else "ido"

    # Check for C source
    c_name = f"func_{addr:08X}.c"
    has_c = c_name in c_files or c_name.lower() in c_files

    # Parse the ASM file
    parsed = parse_asm_file(asm_file)

    # Determine hidden registers (used but not set before use)
    hidden_regs = list(parsed['regs_used'] - parsed['regs_set'])

    # Categorize
    issues = []
    category = "unknown"

    # Check for comment flags
    if parsed['comment_flags']:
        issues.append(f"comment_flags: {parsed['comment_flags']}")

    # Check for pre-prologue code
    if parsed['has_pre_prologue']:
        category = "thunk"
        issues.append("has_pre_prologue_code")

    # Check for hidden registers
    elif hidden_regs:
        category = "hidden_reg"
        issues.append(f"hidden_regs: {hidden_regs}")

    # Check for thunk indicators in comments
    elif any('thunk' in f or 'prologue' in f for f in parsed['comment_flags']):
        category = "thunk"
        issues.append("thunk_in_comments")

    # Check for hidden reg indicators in comments
    elif any('hidden' in f or 'preceding' in f or 'inter' in f for f in parsed['comment_flags']):
        category = "hidden_reg"
        issues.append("hidden_in_comments")

    # Size/complexity check
    elif parsed['size'] > 500:
        category = "complex"
        issues.append(f"large_size: {parsed['size']}")

    elif parsed['num_branches'] > 10:
        category = "complex"
        issues.append(f"many_branches: {parsed['num_branches']}")

    # Looks potentially convertible
    else:
        category = "c_convertible"

    return FunctionAnalysis(
        address=addr_str,
        file=asm_file.name,
        size=parsed['size'],
        region=region,
        num_calls=parsed['num_calls'],
        num_branches=parsed['num_branches'],
        has_c_source=has_c,
        category=category,
        hidden_regs=hidden_regs,
        issues=issues,
        comment_flags=parsed['comment_flags'],
    )


def main():
    parser = argparse.ArgumentParser(description="Categorize ASM functions for C conversion")
    parser.add_argument('--asm-dir', default='asm/main/matched', help='ASM directory')
    parser.add_argument('--c-dir', default='src/main/matched', help='C directory')
    parser.add_argument('--output', default='build/asm_categorization.json', help='Output JSON')
    parser.add_argument('--summary', action='store_true', help='Print summary')
    args = parser.parse_args()

    asm_dir = Path(args.asm_dir)
    c_dir = Path(args.c_dir)

    # Get list of C files
    c_files = set()
    if c_dir.exists():
        c_files = {f.name for f in c_dir.glob('func_*.c')}

    # Also check wip
    wip_dir = Path('src/main/wip')
    if wip_dir.exists():
        c_files.update(f.name for f in wip_dir.glob('func_*.c'))

    # Analyze all ASM files
    results = []
    for asm_file in sorted(asm_dir.glob('func_*.s')):
        analysis = analyze_function(asm_file, c_files)
        if analysis:
            results.append(asdict(analysis))

    # Categorize
    categories = {
        'c_convertible': [],
        'hidden_reg': [],
        'thunk': [],
        'complex': [],
        'unknown': [],
        'has_c_source': [],
    }

    for r in results:
        if r['has_c_source']:
            categories['has_c_source'].append(r)
        else:
            categories[r['category']].append(r)

    # Sort c_convertible by size
    categories['c_convertible'].sort(key=lambda x: (x['size'], x['num_calls']))

    output = {
        'total': len(results),
        'categories': {k: len(v) for k, v in categories.items()},
        'by_category': categories,
    }

    # Save
    Path(args.output).parent.mkdir(parents=True, exist_ok=True)
    Path(args.output).write_text(json.dumps(output, indent=2))
    print(f"Saved to {args.output}")

    if args.summary:
        print(f"\n=== CATEGORIZATION SUMMARY ===")
        print(f"Total ASM files: {len(results)}")
        for cat, count in output['categories'].items():
            print(f"  {cat}: {count}")

        print(f"\n=== TOP 20 C-CONVERTIBLE CANDIDATES ===")
        for r in categories['c_convertible'][:20]:
            print(f"  {r['address']}  {r['size']:3d}B  {r['num_calls']} calls  {r['region']}  {r['file']}")


if __name__ == '__main__':
    main()
