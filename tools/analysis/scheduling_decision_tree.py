#!/usr/bin/env python3
"""
Scheduling Divergence Decision Tree

Classifies functions by their scheduling divergence pattern and recommends
the optimal fix approach. Uses the scheduling divergence statistics data
to build a rule-based decision tree.

Output: Per-function classification with recommended fix approach.

Categories:
  A: PERFECT - Already matches (no sched/codegen diffs)
  B: KNOWN_FIXABLE - Only reloc/frame/sp_offset/move_enc diffs (existing tools handle)
  C: SCHED_ONLY - Pure scheduling diffs, no codegen (scheduler_normalizer or #line)
  D: SCHED_DOMINANT - Mostly scheduling diffs with few codegen (fix sched first)
  E: CODEGEN_DOMINANT - Mostly codegen diffs with some sched (fix C source first)
  F: CODEGEN_ONLY - Only codegen diffs, no scheduling (C source rewrite needed)
  G: MIXED_HEAVY - Many of both types (major C rewrite + scheduling fix)
"""

import argparse
import re
import sys
import os
from collections import defaultdict, Counter
from dataclasses import dataclass
from typing import List, Dict, Optional, Tuple


@dataclass
class FunctionStats:
    name: str
    address: int
    region: str  # 'G' = game, 'L' = library
    total_instrs: int
    exact: int
    reloc: int
    move_enc: int
    frame: int
    sp_offset: int
    sched: int
    codegen: int
    match_pct: float

    @property
    def trivial_diffs(self):
        """Diffs fixable by existing tools (reloc, frame, sp_offset, move_enc)"""
        return self.reloc + self.frame + self.sp_offset + self.move_enc

    @property
    def structural_diffs(self):
        """Diffs requiring code changes"""
        return self.sched + self.codegen

    @property
    def effective_match(self):
        """Match % including trivially-fixable diffs"""
        fixable = self.exact + self.reloc + self.move_enc + self.frame + self.sp_offset
        return fixable / self.total_instrs * 100 if self.total_instrs > 0 else 0

    @property
    def category(self):
        """Classify into fix category"""
        if self.sched == 0 and self.codegen == 0:
            return 'A'  # PERFECT (only trivially-fixable diffs remain)
        if self.sched > 0 and self.codegen == 0:
            return 'C'  # SCHED_ONLY
        if self.codegen > 0 and self.sched == 0:
            return 'F'  # CODEGEN_ONLY
        # Both present
        sched_ratio = self.sched / (self.sched + self.codegen)
        if sched_ratio >= 0.7:
            return 'D'  # SCHED_DOMINANT
        elif sched_ratio <= 0.3:
            return 'E'  # CODEGEN_DOMINANT
        elif self.sched + self.codegen > 20:
            return 'G'  # MIXED_HEAVY
        else:
            return 'E'  # Default to CODEGEN_DOMINANT for mixed

    @property
    def fix_recommendation(self):
        """Recommend fix approach based on category"""
        cat = self.category
        if cat == 'A':
            return "No fix needed — apply patch_stack/convert_or_to_addu only"
        elif cat == 'B':
            return "Apply existing post-processing tools"
        elif cat == 'C':
            if self.sched <= 4:
                return "scheduler_normalizer.py (low swap count, high confidence)"
            elif self.sched <= 8:
                return "scheduler_normalizer.py OR #line directives"
            else:
                return "#line directives + source reordering (many swaps)"
        elif cat == 'D':
            return "Fix scheduling first (scheduler_normalizer), then address minor codegen"
        elif cat == 'E':
            return "Rewrite C source to fix codegen, scheduling may resolve naturally"
        elif cat == 'F':
            if self.codegen <= 4:
                return "Minor C source adjustments (register swap, expression rewrite)"
            else:
                return "Significant C source rewrite needed"
        elif cat == 'G':
            return "Major C rewrite + scheduling fix — consider accepting ASM"
        return "Unknown"

    @property
    def priority(self):
        """Priority score (lower = higher priority = easier to fix)"""
        cat = self.category
        base = {'A': 0, 'C': 1, 'D': 2, 'B': 3, 'F': 4, 'E': 5, 'G': 6}
        return base.get(cat, 7) * 1000 + self.structural_diffs


CATEGORY_NAMES = {
    'A': 'PERFECT',
    'B': 'KNOWN_FIXABLE',
    'C': 'SCHED_ONLY',
    'D': 'SCHED_DOMINANT',
    'E': 'CODEGEN_DOMINANT',
    'F': 'CODEGEN_ONLY',
    'G': 'MIXED_HEAVY',
}

CATEGORY_DESCRIPTIONS = {
    'A': 'Matches with existing post-processing tools',
    'B': 'Only trivially-fixable diffs (reloc, frame, sp_offset)',
    'C': 'Pure scheduling diffs — scheduler_normalizer or #line directives',
    'D': 'Mostly scheduling diffs with minor codegen — fix scheduling first',
    'E': 'Mostly codegen diffs with minor scheduling — fix C source first',
    'F': 'Only codegen diffs — C source rewrite needed',
    'G': 'Heavy mix of both types — major effort required',
}


def parse_stats_file(filepath: str) -> List[FunctionStats]:
    """Parse the scheduling divergence statistics markdown file."""
    functions = []

    with open(filepath, 'r') as f:
        content = f.read()

    # Find the full function table
    table_match = re.search(
        r'\| # \| Function \| Address.*?\n\|[-: |]+\n(.*?)(?:\n\n|\Z)',
        content, re.DOTALL
    )
    if not table_match:
        print("ERROR: Could not find function table in stats file", file=sys.stderr)
        sys.exit(1)

    for line in table_match.group(1).strip().split('\n'):
        line = line.strip()
        if not line or not line.startswith('|'):
            continue

        cols = [c.strip() for c in line.split('|')[1:-1]]
        if len(cols) < 13:
            continue

        try:
            func = FunctionStats(
                name=cols[1],
                address=int(cols[2], 16),
                region=cols[3],
                total_instrs=int(cols[4]),
                exact=int(cols[5]),
                reloc=int(cols[6]),
                move_enc=int(cols[7]),
                frame=int(cols[8]),
                sp_offset=int(cols[9]),
                sched=int(cols[10]),
                codegen=int(cols[11]),
                match_pct=float(cols[12].rstrip('%')),
            )
            functions.append(func)
        except (ValueError, IndexError) as e:
            continue

    return functions


def print_summary(functions: List[FunctionStats]):
    """Print category summary."""
    cats = Counter(f.category for f in functions)
    total = len(functions)

    print("=" * 80)
    print("SCHEDULING DIVERGENCE DECISION TREE — FUNCTION CLASSIFICATION")
    print("=" * 80)
    print()
    print(f"Total functions analyzed: {total}")
    print()
    print("Category Distribution:")
    print("-" * 80)
    print(f"{'Cat':>4} {'Name':<20} {'Count':>6} {'%':>6}  Description")
    print("-" * 80)

    for cat in ['A', 'C', 'D', 'F', 'E', 'G']:
        count = cats.get(cat, 0)
        pct = count / total * 100 if total > 0 else 0
        name = CATEGORY_NAMES.get(cat, '?')
        desc = CATEGORY_DESCRIPTIONS.get(cat, '')
        print(f"  {cat}  {name:<20} {count:>6} {pct:>5.1f}%  {desc}")

    print("-" * 80)
    print()


def print_sched_only(functions: List[FunctionStats]):
    """Print details of SCHED_ONLY functions (highest priority fixes)."""
    sched_only = sorted(
        [f for f in functions if f.category == 'C'],
        key=lambda f: f.sched
    )

    if not sched_only:
        print("No pure scheduling-only functions found.")
        return

    print("=" * 80)
    print("CATEGORY C: PURE SCHEDULING DIFFS (highest priority — normalizer candidates)")
    print("=" * 80)
    print()
    print(f"{'Function':<30} {'Addr':<12} {'Rgn':>3} {'Instrs':>6} {'Sched':>5} "
          f"{'Eff%':>5}  Recommendation")
    print("-" * 120)

    for f in sched_only:
        print(f"{f.name:<30} {f.address:#010x} {f.region:>3} {f.total_instrs:>6} "
              f"{f.sched:>5} {f.effective_match:>5.1f}%  {f.fix_recommendation}")

    print()
    total_sched = sum(f.sched for f in sched_only)
    print(f"Total: {len(sched_only)} functions, {total_sched} scheduling diff instructions")
    print()


def print_actionable(functions: List[FunctionStats]):
    """Print all functions with structural diffs, sorted by priority."""
    actionable = sorted(
        [f for f in functions if f.structural_diffs > 0],
        key=lambda f: f.priority
    )

    if not actionable:
        print("No functions with structural diffs found.")
        return

    print("=" * 80)
    print("ALL FUNCTIONS WITH STRUCTURAL DIFFS (sorted by fix priority)")
    print("=" * 80)
    print()
    print(f"{'#':>3} {'Cat':>3} {'Function':<28} {'Addr':<12} {'Rgn':>3} "
          f"{'Instrs':>5} {'Sched':>5} {'CdGen':>5} {'Eff%':>5}  Fix")
    print("-" * 130)

    for i, f in enumerate(actionable, 1):
        cat = f.category
        cat_name = CATEGORY_NAMES.get(cat, '?')
        print(f"{i:>3} {cat:>3}  {f.name:<28} {f.address:#010x} {f.region:>3} "
              f"{f.total_instrs:>5} {f.sched:>5} {f.codegen:>5} "
              f"{f.effective_match:>5.1f}%  {f.fix_recommendation}")

    print()


def print_region_breakdown(functions: List[FunctionStats]):
    """Print breakdown by region."""
    print("=" * 80)
    print("REGION BREAKDOWN")
    print("=" * 80)
    print()

    for region, label in [('G', 'Game Region (0x80041000-0x80085FFF)'),
                           ('L', 'Library Region (0x80086000+)')]:
        region_funcs = [f for f in functions if f.region == region]
        cats = Counter(f.category for f in region_funcs)
        total = len(region_funcs)

        print(f"  {label}: {total} functions")
        for cat in ['A', 'C', 'D', 'F', 'E', 'G']:
            count = cats.get(cat, 0)
            if count > 0:
                name = CATEGORY_NAMES.get(cat, '?')
                print(f"    {cat} {name:<20}: {count:>4} ({count/total*100:>5.1f}%)")
        print()


def print_decision_rules():
    """Print the decision tree rules."""
    print("=" * 80)
    print("DECISION TREE RULES")
    print("=" * 80)
    print()
    print("""
  Function has scheduling or codegen diffs?
  ├── NO → Category A (PERFECT): Apply patch_stack / convert_or_to_addu only
  └── YES
      ├── Only scheduling diffs (codegen = 0)?
      │   └── YES → Category C (SCHED_ONLY)
      │       ├── sched ≤ 4 → scheduler_normalizer.py (high confidence)
      │       ├── sched ≤ 8 → scheduler_normalizer.py OR #line directives
      │       └── sched > 8 → #line directives + source statement reordering
      ├── Only codegen diffs (sched = 0)?
      │   └── YES → Category F (CODEGEN_ONLY)
      │       ├── codegen ≤ 4 → Minor C adjustments (register swap, expression)
      │       └── codegen > 4 → Significant C rewrite needed
      └── Both sched AND codegen diffs?
          ├── sched/(sched+codegen) ≥ 0.7 → Category D (SCHED_DOMINANT)
          │   └── Fix scheduling first, then minor codegen
          ├── sched/(sched+codegen) ≤ 0.3 → Category E (CODEGEN_DOMINANT)
          │   └── Rewrite C source first, scheduling may resolve
          └── Mixed (0.3 < ratio < 0.7)
              ├── sched+codegen > 20 → Category G (MIXED_HEAVY)
              │   └── Major rewrite or accept ASM
              └── sched+codegen ≤ 20 → Category E (CODEGEN_DOMINANT)
                  └── Fix C source, then scheduling

  Root cause context:
  - Scheduling diffs are caused by source line number tiebreaker (off12 field)
  - Our decompiled C has different line numbers than the original source
  - Fix: #line directives, source statement reordering, or scheduler_normalizer.py
  - Codegen diffs are caused by different C source (expressions, types, control flow)
  - Fix: Rewrite C to match original patterns
""")


def main():
    parser = argparse.ArgumentParser(
        description='Classify functions by scheduling divergence pattern')
    parser.add_argument('--stats', default='docs/SCHEDULING_DIVERGENCE_STATISTICS.md',
                        help='Path to scheduling divergence statistics file')
    parser.add_argument('--category', '-c', choices=list(CATEGORY_NAMES.keys()),
                        help='Show only functions in this category')
    parser.add_argument('--rules', action='store_true',
                        help='Print decision tree rules')
    parser.add_argument('--summary', action='store_true',
                        help='Print summary only')
    parser.add_argument('--json', action='store_true',
                        help='Output as JSON')
    args = parser.parse_args()

    if args.rules:
        print_decision_rules()
        return

    # Find stats file relative to script or working directory
    stats_path = args.stats
    if not os.path.exists(stats_path):
        script_dir = os.path.dirname(os.path.abspath(__file__))
        project_root = os.path.dirname(os.path.dirname(script_dir))
        stats_path = os.path.join(project_root, args.stats)

    if not os.path.exists(stats_path):
        print(f"ERROR: Stats file not found: {args.stats}", file=sys.stderr)
        sys.exit(1)

    functions = parse_stats_file(stats_path)
    if not functions:
        print("ERROR: No functions parsed from stats file", file=sys.stderr)
        sys.exit(1)

    if args.category:
        functions = [f for f in functions if f.category == args.category]

    if args.json:
        import json
        output = []
        for f in sorted(functions, key=lambda x: x.priority):
            output.append({
                'name': f.name,
                'address': f'{f.address:#010x}',
                'region': f.region,
                'total_instrs': f.total_instrs,
                'sched_diffs': f.sched,
                'codegen_diffs': f.codegen,
                'category': f.category,
                'category_name': CATEGORY_NAMES[f.category],
                'effective_match_pct': round(f.effective_match, 1),
                'fix_recommendation': f.fix_recommendation,
            })
        print(json.dumps(output, indent=2))
        return

    if args.category:
        filtered = [f for f in functions if f.category == args.category]
        for f in sorted(filtered, key=lambda x: x.sched + x.codegen):
            print(f"{f.name:<30} {f.address:#010x} {f.region} "
                  f"sched:{f.sched} codegen:{f.codegen} "
                  f"eff:{f.effective_match:.1f}% — {f.fix_recommendation}")
        print(f"\nTotal: {len(filtered)} functions")
        return

    if args.summary:
        print_summary(functions)
        return

    # Full report
    print_summary(functions)
    print_decision_rules()
    print_sched_only(functions)
    print_region_breakdown(functions)
    print_actionable(functions)


if __name__ == '__main__':
    main()
