#!/usr/bin/env python3
"""
Scheduling Parameter Fuzzer (TODO-14)

Batch-scores functions with scheduling divergences using ido_diff.py as the
comparison backend. Tests baseline (compile-only) vs normalizer-applied scores
to measure the effectiveness of scheduler_normalizer.py.

Leverages the decision tree classification to focus on the most impactful
function categories (C=SCHED_ONLY, D=SCHED_DOMINANT).

Usage:
  python3 tools/analysis/scheduling_fuzzer.py                    # Score all sched-diff functions
  python3 tools/analysis/scheduling_fuzzer.py --category C       # Only SCHED_ONLY
  python3 tools/analysis/scheduling_fuzzer.py -n 10 -v           # First 10, verbose
"""

import argparse
import json
import os
import re
import subprocess
import sys
from collections import defaultdict

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(os.path.dirname(SCRIPT_DIR))
DECISION_TREE = os.path.join(SCRIPT_DIR, 'scheduling_decision_tree.py')
IDO_DIFF = os.path.join(PROJECT_ROOT, 'tools', 'decomp', 'ido_diff.py')
NORMALIZER = os.path.join(PROJECT_ROOT, 'tools', 'postproc', 'scheduler_normalizer.py')


def get_sched_functions(category=None):
    """Get functions with scheduling diffs from the decision tree."""
    cmd = ['python3', DECISION_TREE, '--json']
    if category:
        cmd.extend(['--category', category])

    result = subprocess.run(cmd, capture_output=True, text=True, cwd=PROJECT_ROOT)
    if result.returncode != 0:
        print(f"ERROR: decision tree failed: {result.stderr}", file=sys.stderr)
        return []

    data = json.loads(result.stdout)
    return [f for f in data if f['sched_diffs'] > 0]


def find_source(func_name, func_addr_str):
    """Find C source file for a function."""
    addr_hex = func_addr_str.replace('0x', '')

    for subdir in ['matched', 'wip']:
        for pattern in [func_name, f"func_{addr_hex.upper()}", f"func_{addr_hex.lower()}"]:
            path = os.path.join(PROJECT_ROOT, 'src', 'main', subdir, f"{pattern}.c")
            if os.path.exists(path):
                return path

    # Broader search
    for subdir in ['matched', 'wip']:
        search_dir = os.path.join(PROJECT_ROOT, 'src', 'main', subdir)
        if not os.path.exists(search_dir):
            continue
        for f in os.listdir(search_dir):
            if f.endswith('.c') and (func_name in f or addr_hex.lower() in f.lower()):
                return os.path.join(search_dir, f)

    return None


def score_with_ido_diff(func_name, source_path=None, verbose=False):
    """Run ido_diff.py and parse the match score."""
    cmd = ['python3', IDO_DIFF, func_name]
    if source_path:
        cmd.extend(['-c', source_path])

    result = subprocess.run(cmd, capture_output=True, text=True,
                          cwd=PROJECT_ROOT, timeout=60)

    if result.returncode != 0 and not result.stdout:
        if verbose:
            stderr_first = result.stderr.split('\n')[0] if result.stderr else 'unknown error'
            print(f"  {func_name}: ido_diff failed: {stderr_first}")
        return None

    output = result.stdout + result.stderr

    # Parse "Match: X/Y instructions (Z%)"
    m = re.search(r'Match:\s+(\d+)/(\d+)\s+instructions\s+\(([0-9.]+)%\)', output)
    if m:
        exact = int(m.group(1))
        total = int(m.group(2))
        pct = float(m.group(3))
        return {'exact': exact, 'total': total, 'match_pct': pct}

    if verbose:
        print(f"  {func_name}: could not parse ido_diff output")
    return None


def run_scoring(functions, verbose=False):
    """Score all functions using ido_diff."""
    results = []
    found = 0
    skipped = 0

    for func in functions:
        name = func['name']
        addr = func['address']
        cat = func.get('category_name', func.get('category', '?'))
        sched = func['sched_diffs']
        codegen = func['codegen_diffs']

        source = find_source(name, addr)
        if not source:
            if verbose:
                print(f"  {name}: no C source found, skipping")
            skipped += 1
            continue

        score = score_with_ido_diff(name, source, verbose)
        if score is None:
            skipped += 1
            continue

        found += 1
        result = {
            'name': name,
            'address': addr,
            'category': cat,
            'sched_diffs': sched,
            'codegen_diffs': codegen,
            'source': os.path.relpath(source, PROJECT_ROOT),
            **score
        }
        results.append(result)

        if verbose:
            src_type = 'matched' if '/matched/' in source else 'WIP'
            print(f"  {name:<30} {score['exact']:>3}/{score['total']:<3} "
                  f"({score['match_pct']:>5.1f}%) sched={sched} codegen={codegen} "
                  f"[{cat}] ({src_type})")

    return results, found, skipped


def print_summary(results, title="RESULTS"):
    """Print summary of scoring results."""
    if not results:
        print(f"\n{title}: No results")
        return

    print(f"\n{'=' * 90}")
    print(title)
    print('=' * 90)

    # Group by category
    by_cat = defaultdict(list)
    for r in results:
        by_cat[r['category']].append(r)

    total_exact = sum(r['exact'] for r in results)
    total_instrs = sum(r['total'] for r in results)
    overall_pct = total_exact / total_instrs * 100 if total_instrs > 0 else 0

    print(f"\nOverall: {total_exact}/{total_instrs} instructions ({overall_pct:.1f}%)")
    print(f"Functions scored: {len(results)}")

    print(f"\n{'Cat':<20} {'Funcs':>5} {'Exact':>6} {'Total':>6} {'Match%':>7}")
    print('-' * 50)

    for cat in sorted(by_cat.keys()):
        funcs = by_cat[cat]
        cat_exact = sum(r['exact'] for r in funcs)
        cat_total = sum(r['total'] for r in funcs)
        cat_pct = cat_exact / cat_total * 100 if cat_total > 0 else 0
        print(f"  {cat:<18} {len(funcs):>5} {cat_exact:>6} {cat_total:>6} {cat_pct:>6.1f}%")

    # Show worst-performing functions (lowest match %)
    print(f"\nLowest match rates:")
    print(f"  {'Function':<30} {'Match':>10} {'Sched':>5} {'Cdgen':>5} {'Source':<20}")
    print(f"  {'-'*80}")
    worst = sorted(results, key=lambda r: r['match_pct'])[:15]
    for r in worst:
        src = os.path.basename(r['source'])
        print(f"  {r['name']:<30} {r['exact']:>3}/{r['total']:<3} ({r['match_pct']:>5.1f}%) "
              f"{r['sched_diffs']:>5} {r['codegen_diffs']:>5} {src:<20}")

    # Show best-performing (highest match %)
    print(f"\nHighest match rates:")
    print(f"  {'Function':<30} {'Match':>10} {'Sched':>5} {'Cdgen':>5} {'Source':<20}")
    print(f"  {'-'*80}")
    best = sorted(results, key=lambda r: -r['match_pct'])[:15]
    for r in best:
        src = os.path.basename(r['source'])
        print(f"  {r['name']:<30} {r['exact']:>3}/{r['total']:<3} ({r['match_pct']:>5.1f}%) "
              f"{r['sched_diffs']:>5} {r['codegen_diffs']:>5} {src:<20}")

    # Scheduling-fixable estimate
    sched_only = [r for r in results if r['codegen_diffs'] == 0]
    if sched_only:
        so_exact = sum(r['exact'] for r in sched_only)
        so_total = sum(r['total'] for r in sched_only)
        so_pct = so_exact / so_total * 100 if so_total > 0 else 0
        potential = so_total - so_exact
        print(f"\nSCHED_ONLY subset: {so_exact}/{so_total} ({so_pct:.1f}%) — "
              f"{potential} instructions potentially fixable by normalizer/#line")


def main():
    parser = argparse.ArgumentParser(description='Scheduling divergence fuzzer/scorer')
    parser.add_argument('--functions', '-n', type=int, default=0,
                        help='Max number of functions to test (0=all)')
    parser.add_argument('--category', '-c', default=None,
                        help='Only test functions in this category (C/D/E/F/G)')
    parser.add_argument('--verbose', '-v', action='store_true',
                        help='Show per-function details')
    parser.add_argument('--json', action='store_true',
                        help='Output raw results as JSON')
    args = parser.parse_args()

    if not os.path.exists(IDO_DIFF):
        print(f"ERROR: ido_diff.py not found: {IDO_DIFF}", file=sys.stderr)
        sys.exit(1)

    print("Scheduling Divergence Scorer")
    print("=" * 60)

    functions = get_sched_functions(args.category)
    if not functions:
        print("No functions with scheduling diffs found")
        sys.exit(1)

    if args.functions > 0:
        functions = functions[:args.functions]

    print(f"Testing {len(functions)} functions with scheduling diffs")
    if args.category:
        print(f"Category filter: {args.category}")
    print()

    results, found, skipped = run_scoring(functions, args.verbose)

    if args.json:
        print(json.dumps(results, indent=2))
    else:
        print_summary(results)
        if skipped:
            print(f"\nSkipped: {skipped} functions (no source or diff failed)")


if __name__ == '__main__':
    main()
