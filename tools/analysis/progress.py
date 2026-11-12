#!/usr/bin/env python3
"""
progress.py - Decompilation Progress Tracker

Tracks and reports decompilation progress for Tokimeki Memorial PS1 project.
Uses function_map.json to calculate completion statistics.

Usage:
    python3 tools/progress.py
    python3 tools/progress.py --verbose
    python3 tools/progress.py --json  # Output as JSON

Based on SOTN's progress tracker, simplified for our monolithic architecture.
"""

import argparse
import json
import os
import sys
from pathlib import Path

# Project paths
SCRIPT_DIR = Path(__file__).parent
ROOT_DIR = SCRIPT_DIR.parent
FUNCTION_MAP = ROOT_DIR / "build/function_analysis/function_map.json"
SRC_DIR = ROOT_DIR / "src"

def count_include_asm_macros(src_dir: Path) -> int:
    """
    Count INCLUDE_ASM macros in C source files.
    Each INCLUDE_ASM represents an undecompiled function.
    """
    count = 0
    for c_file in src_dir.rglob("*.c"):
        try:
            with open(c_file, 'r', encoding='utf-8') as f:
                content = f.read()
                # Count INCLUDE_ASM occurrences
                count += content.count("INCLUDE_ASM(")
        except Exception as e:
            print(f"Warning: Could not read {c_file}: {e}", file=sys.stderr)
    return count

def load_function_map() -> dict:
    """Load function metadata from function_map.json"""
    if not FUNCTION_MAP.exists():
        print(f"Error: Function map not found: {FUNCTION_MAP}", file=sys.stderr)
        print("Run: python3 tools/extract_all_functions.py", file=sys.stderr)
        sys.exit(1)

    try:
        with open(FUNCTION_MAP, 'r') as f:
            return json.load(f)
    except Exception as e:
        print(f"Error loading function map: {e}", file=sys.stderr)
        sys.exit(1)

def calculate_progress(verbose=False) -> dict:
    """
    Calculate decompilation progress statistics.

    Returns:
        dict with progress statistics
    """
    # Load function data
    func_data = load_function_map()
    functions = func_data.get('functions', [])

    # Count INCLUDE_ASM macros (undecompiled functions)
    include_asm_count = count_include_asm_macros(SRC_DIR)

    # Calculate statistics
    total_functions = len(functions)
    library_functions = len([f for f in functions if f.get('library')])

    # Functions that don't need decompiling (libraries)
    excluded_count = library_functions

    # Decompilation target (total - libraries)
    target_count = total_functions - excluded_count

    # Decompiled = target - INCLUDE_ASM
    decompiled_count = max(0, target_count - include_asm_count)

    # Calculate percentages
    if target_count > 0:
        decomp_pct = (decompiled_count / target_count) * 100
    else:
        decomp_pct = 0.0

    # Breakdown by complexity
    complexity_stats = {
        'leaf': len([f for f in functions if f.get('complexity') == 'leaf']),
        'simple': len([f for f in functions if f.get('complexity') == 'simple']),
        'medium': len([f for f in functions if f.get('complexity') == 'medium']),
        'complex': len([f for f in functions if f.get('complexity') == 'complex']),
        'wrapper': len([f for f in functions if f.get('complexity') == 'wrapper']),
    }

    # Calculate bytes
    total_bytes = sum(f.get('size', 0) for f in functions)
    library_bytes = sum(f.get('size', 0) for f in functions if f.get('library'))
    target_bytes = total_bytes - library_bytes

    # Estimate decompiled bytes (proportional to function count)
    if target_count > 0:
        decompiled_bytes = int((decompiled_count / target_count) * target_bytes)
    else:
        decompiled_bytes = 0

    stats = {
        'total_functions': total_functions,
        'library_functions': library_functions,
        'target_functions': target_count,
        'decompiled_functions': decompiled_count,
        'remaining_functions': target_count - decompiled_count,
        'include_asm_macros': include_asm_count,
        'progress_percent': decomp_pct,
        'complexity_breakdown': complexity_stats,
        'total_bytes': total_bytes,
        'library_bytes': library_bytes,
        'target_bytes': target_bytes,
        'decompiled_bytes': decompiled_bytes,
        'remaining_bytes': target_bytes - decompiled_bytes,
    }

    if verbose:
        # Add library breakdown
        libraries = {}
        for f in functions:
            lib = f.get('library')
            if lib:
                if lib not in libraries:
                    libraries[lib] = 0
                libraries[lib] += 1
        stats['library_breakdown'] = libraries

    return stats

def format_bytes(bytes_count: int) -> str:
    """Format bytes in human-readable form"""
    if bytes_count < 1024:
        return f"{bytes_count}B"
    elif bytes_count < 1024 * 1024:
        return f"{bytes_count / 1024:.1f}KB"
    else:
        return f"{bytes_count / (1024 * 1024):.2f}MB"

def print_progress(stats: dict, verbose: bool = False):
    """Print progress report to console"""
    print("=" * 80)
    print("TOKIMEKI MEMORIAL DECOMPILATION PROGRESS")
    print("=" * 80)
    print()

    print("Function Statistics:")
    print(f"  Total functions in binary:     {stats['total_functions']:>5}")
    print(f"  Library functions (excluded):  {stats['library_functions']:>5}")
    print(f"  Functions to decompile:        {stats['target_functions']:>5}")
    print()

    print("Decompilation Progress:")
    print(f"  Decompiled:                    {stats['decompiled_functions']:>5}  ({stats['progress_percent']:>5.1f}%)")
    print(f"  Remaining (INCLUDE_ASM):       {stats['remaining_functions']:>5}  ({100 - stats['progress_percent']:>5.1f}%)")
    print()

    # Progress bar
    bar_width = 50
    filled = int(bar_width * stats['progress_percent'] / 100)
    bar = "█" * filled + "░" * (bar_width - filled)
    print(f"  [{bar}] {stats['progress_percent']:.1f}%")
    print()

    print("Size Breakdown:")
    print(f"  Total code size:               {format_bytes(stats['total_bytes']):>10}")
    print(f"  Library code (excluded):       {format_bytes(stats['library_bytes']):>10}")
    print(f"  Target code to decompile:      {format_bytes(stats['target_bytes']):>10}")
    print(f"  Decompiled (estimated):        {format_bytes(stats['decompiled_bytes']):>10}")
    print(f"  Remaining:                     {format_bytes(stats['remaining_bytes']):>10}")
    print()

    print("Complexity Breakdown:")
    comp = stats['complexity_breakdown']
    print(f"  Leaf functions:                {comp['leaf']:>5}")
    print(f"  Simple functions:              {comp['simple']:>5}")
    print(f"  Medium complexity:             {comp['medium']:>5}")
    print(f"  Complex functions:             {comp['complex']:>5}")
    print(f"  Wrapper functions:             {comp['wrapper']:>5}")
    print()

    if verbose and 'library_breakdown' in stats:
        print("Library Functions Identified:")
        for lib, count in sorted(stats['library_breakdown'].items()):
            print(f"  {lib:<25}  {count:>5} functions")
        print()

    print("=" * 80)

    # Motivational message
    if stats['progress_percent'] == 0:
        print("🚀 Ready to start decompilation! Check TOP_50_CANDIDATES.md for easy functions.")
    elif stats['progress_percent'] < 10:
        print("📈 Great start! Keep going!")
    elif stats['progress_percent'] < 25:
        print("💪 Nice progress! Momentum is building!")
    elif stats['progress_percent'] < 50:
        print("🔥 Over 25%! You're making real progress!")
    elif stats['progress_percent'] < 75:
        print("⭐ Halfway there! Keep up the excellent work!")
    elif stats['progress_percent'] < 90:
        print("🎯 Almost there! The finish line is in sight!")
    elif stats['progress_percent'] < 100:
        print("🏆 SO CLOSE! Just a few more functions!")
    else:
        print("🎉 COMPLETE! 100% decompiled! Amazing work!")

    print()

def main():
    parser = argparse.ArgumentParser(
        description="Track decompilation progress for Tokimeki Memorial PS1",
        epilog="""
Examples:
    python3 tools/progress.py
    python3 tools/progress.py --verbose
    python3 tools/progress.py --json > progress.json
        """
    )
    parser.add_argument(
        '-v', '--verbose',
        action='store_true',
        help='Show detailed statistics including library breakdown'
    )
    parser.add_argument(
        '--json',
        action='store_true',
        help='Output progress data as JSON'
    )

    args = parser.parse_args()

    # Calculate progress
    stats = calculate_progress(verbose=args.verbose)

    # Output
    if args.json:
        print(json.dumps(stats, indent=2))
    else:
        print_progress(stats, verbose=args.verbose)

if __name__ == "__main__":
    main()
