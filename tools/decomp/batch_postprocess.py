#!/usr/bin/env python3
"""
Batch Post-Processing for All Permuter Results

Scans all permuter directories and runs auto_postprocess on each,
generating a report of which functions can be matched.

Usage:
    python3 batch_postprocess.py [--threshold 90] [--output report.json]
"""

import argparse
import json
import os
import re
import sys
from pathlib import Path
from datetime import datetime

# Import from auto_postprocess
sys.path.insert(0, str(Path(__file__).parent))
from auto_postprocess import (
    find_best_permuter_candidates,
    get_function_info,
    process_candidate,
    PROJECT_ROOT,
    PERMUTER_DIR
)


def find_all_permuter_functions():
    """Find all functions with permuter directories."""
    functions = []

    for item in PERMUTER_DIR.iterdir():
        if not item.is_dir():
            continue

        # Check if it's a function directory
        match = re.match(r'func_([0-9A-Fa-f]{8})', item.name, re.IGNORECASE)
        if not match:
            continue

        # Check if it has any output directories
        has_outputs = any(
            d.is_dir() and d.name.startswith('output-')
            for d in item.iterdir()
        )

        if has_outputs:
            functions.append(item.name)

    return sorted(functions)


def run_batch(threshold=90, max_candidates=3, verbose=False):
    """
    Run post-processing on all permuter results.

    Args:
        threshold: Match percentage threshold for "near match" classification
        max_candidates: Maximum candidates to try per function
        verbose: Print detailed output

    Returns:
        dict with results
    """
    functions = find_all_permuter_functions()

    results = {
        'timestamp': datetime.now().isoformat(),
        'total_functions': len(functions),
        'perfect_matches': [],
        'near_matches': [],  # >= threshold
        'partial_matches': [],  # 50-threshold
        'poor_matches': [],  # < 50
        'errors': [],
        'details': {}
    }

    print(f"Found {len(functions)} functions with permuter results")
    print(f"Processing with threshold={threshold}%, max_candidates={max_candidates}")
    print()

    for i, func_name in enumerate(functions):
        print(f"[{i+1}/{len(functions)}] {func_name}...", end=' ', flush=True)

        addr, size = get_function_info(func_name)
        if addr is None:
            print("SKIP (no address)")
            results['errors'].append({
                'function': func_name,
                'error': 'Could not find function address'
            })
            continue

        if size is None:
            size = 256

        candidates = find_best_permuter_candidates(func_name)
        if not candidates:
            print("SKIP (no candidates)")
            continue

        best_match = None
        best_info = None
        best_source = None

        for score, source in candidates[:max_candidates]:
            try:
                success, obj_path, info = process_candidate(
                    source, addr, size, verbose=False
                )

                if isinstance(info, str):
                    continue

                if success:
                    best_match = (score, source, obj_path)
                    best_info = info
                    break

                if best_info is None or info['matches'] > best_info['matches']:
                    best_match = (score, source, obj_path)
                    best_info = info
                    best_source = source

            except Exception as e:
                if verbose:
                    print(f"Error with {source}: {e}")
                continue

        if best_info is None:
            print("ERROR")
            results['errors'].append({
                'function': func_name,
                'error': 'All candidates failed'
            })
            continue

        pct = best_info['percentage']
        results['details'][func_name] = {
            'address': f"0x{addr:08X}",
            'size': size,
            'match_pct': pct,
            'matches': best_info['matches'],
            'total': best_info['total'],
            'permuter_score': best_match[0],
            'source': str(best_match[1])
        }

        if pct == 100:
            print(f"PERFECT ({best_info['matches']}/{best_info['total']})")
            results['perfect_matches'].append(func_name)
        elif pct >= threshold:
            print(f"NEAR ({pct:.1f}%)")
            results['near_matches'].append(func_name)
        elif pct >= 50:
            print(f"PARTIAL ({pct:.1f}%)")
            results['partial_matches'].append(func_name)
        else:
            print(f"POOR ({pct:.1f}%)")
            results['poor_matches'].append(func_name)

    return results


def main():
    parser = argparse.ArgumentParser(
        description='Batch post-processing for permuter results'
    )
    parser.add_argument('--threshold', '-t', type=int, default=90,
                        help='Near-match threshold percentage (default: 90)')
    parser.add_argument('--max-candidates', '-m', type=int, default=3,
                        help='Max candidates to try per function (default: 3)')
    parser.add_argument('--output', '-o', help='Output JSON report file')
    parser.add_argument('--verbose', '-v', action='store_true')

    args = parser.parse_args()

    results = run_batch(
        threshold=args.threshold,
        max_candidates=args.max_candidates,
        verbose=args.verbose
    )

    # Summary
    print()
    print("=" * 60)
    print("SUMMARY")
    print("=" * 60)
    print(f"Total functions: {results['total_functions']}")
    print(f"Perfect matches: {len(results['perfect_matches'])}")
    print(f"Near matches (>={args.threshold}%): {len(results['near_matches'])}")
    print(f"Partial matches (50-{args.threshold}%): {len(results['partial_matches'])}")
    print(f"Poor matches (<50%): {len(results['poor_matches'])}")
    print(f"Errors: {len(results['errors'])}")

    if results['perfect_matches']:
        print()
        print("Perfect matches (ready for promotion):")
        for func in results['perfect_matches']:
            print(f"  - {func}")

    if results['near_matches']:
        print()
        print(f"Near matches (>={args.threshold}%, may need manual tweaks):")
        for func in results['near_matches'][:10]:
            detail = results['details'][func]
            print(f"  - {func}: {detail['match_pct']:.1f}%")
        if len(results['near_matches']) > 10:
            print(f"  ... and {len(results['near_matches']) - 10} more")

    if args.output:
        with open(args.output, 'w') as f:
            json.dump(results, f, indent=2)
        print(f"\nReport saved to: {args.output}")


if __name__ == '__main__':
    main()
