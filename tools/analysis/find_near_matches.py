#!/usr/bin/env python3
"""
Find WIP functions that are close to matching (high match percentage).

Usage:
    python3 tools/analysis/find_near_matches.py [--threshold 0.90] [--json]
"""

import argparse
import json
import subprocess
import sys
from pathlib import Path


def get_match_percentage(func_name: str) -> float:
    """Run ido_diff and extract match percentage."""
    try:
        wip_file = Path(f'src/main/wip/{func_name}.c')
        if not wip_file.exists():
            return 0.0

        result = subprocess.run(
            ['python3', 'tools/decomp/ido_diff.py', func_name, '-c', str(wip_file), '--quiet'],
            capture_output=True,
            text=True,
            timeout=60
        )

        # Parse output for match percentage
        # This depends on ido_diff.py output format
        output = result.stdout + result.stderr
        if 'MATCH' in output or '100%' in output:
            return 1.0

        # Try to extract percentage
        import re
        match = re.search(r'(\d+(?:\.\d+)?)\s*%', output)
        if match:
            return float(match.group(1)) / 100.0

        # If bytes different, estimate percentage
        match = re.search(r'(\d+)\s*bytes?\s*different', output)
        if match:
            diff_bytes = int(match.group(1))
            # Rough estimate: assume ~500 bytes per function
            return max(0.0, 1.0 - (diff_bytes / 500.0))

        return 0.0

    except Exception as e:
        print(f"Warning: Could not check {func_name}: {e}", file=sys.stderr)
        return 0.0


def main():
    parser = argparse.ArgumentParser(description='Find near-matching WIP functions')
    parser.add_argument('--threshold', type=float, default=0.90, help='Minimum match percentage')
    parser.add_argument('--json', action='store_true', help='Output as JSON')
    parser.add_argument('--limit', type=int, default=20, help='Max functions to check')
    args = parser.parse_args()

    wip_dir = Path('src/main/wip')
    if not wip_dir.exists():
        print("No src/wip directory found", file=sys.stderr)
        sys.exit(1)

    # Get WIP files with actual code (not just stubs)
    wip_files = []
    for c_file in wip_dir.glob('*.c'):
        content = c_file.read_text()
        # Skip stubs (files with only comments or very short)
        if len(content) > 200 and 'void' in content:
            wip_files.append(c_file.stem)

    print(f"Checking {min(len(wip_files), args.limit)} WIP files...", file=sys.stderr)

    near_matches = []
    for func_name in wip_files[:args.limit]:
        pct = get_match_percentage(func_name)
        if pct >= args.threshold:
            near_matches.append({
                'function': func_name,
                'match_percentage': round(pct, 3)
            })

    near_matches.sort(key=lambda x: x['match_percentage'], reverse=True)

    if args.json:
        for nm in near_matches:
            print(json.dumps(nm['function']))
    else:
        print(f"\nFound {len(near_matches)} near-matches (>= {args.threshold:.0%}):\n")
        for nm in near_matches:
            print(f"  {nm['function']}: {nm['match_percentage']:.1%}")


if __name__ == '__main__':
    main()
