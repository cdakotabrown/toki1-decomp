#!/usr/bin/env python3
"""
build_all_functions.py - Unified verification for ALL game functions

Verifies byte-perfect matching for:
  - Main binary: ~1,300 functions (1,051 matched C + 142 ASM + 107 semi-matched)
  - Overlays: ~5,363 functions across 26 EXN overlays

Total: ~6,663 functions

Features:
  - Uses existing cache for main binary (content-hash based)
  - Parallel compilation for performance
  - Combined statistics across all code regions

Usage:
    python3 tools/build/build_all_functions.py              # Full verify
    python3 tools/build/build_all_functions.py --main-only  # Main binary only
    python3 tools/build/build_all_functions.py --overlay-only  # Overlays only
    python3 tools/build/build_all_functions.py --no-cache   # Skip cache
    python3 tools/build/build_all_functions.py --summary    # Summary only
"""

import os
import sys
import time
import argparse
import subprocess
from pathlib import Path
from concurrent.futures import ProcessPoolExecutor, as_completed

# Project root
ROOT = Path(__file__).parent.parent.parent

# Import build modules
sys.path.insert(0, str(ROOT / "tools" / "build"))
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

# Paths
SRC_MATCHED = ROOT / "src" / "main" / "matched"
SRC_SEMI = ROOT / "src" / "main" / "semi-matched"
ASM_MATCHED = ROOT / "asm" / "main" / "matched"
SRC_OVERLAY = ROOT / "src" / "overlay"
BUILD_OVERLAY_PY = ROOT / "tools" / "overlay" / "build_overlay.py"
BUILD_LEVEL3_PY = ROOT / "tools" / "build" / "build_level3_c.py"


def count_functions():
    """Count all functions in the repository."""
    counts = {
        'matched_c': 0,
        'matched_asm': 0,
        'semi_matched': 0,
        'overlays': {},
        'overlay_total': 0,
    }

    # Main binary
    if SRC_MATCHED.exists():
        counts['matched_c'] = len(list(SRC_MATCHED.glob("*.c")))
    if ASM_MATCHED.exists():
        counts['matched_asm'] = len(list(ASM_MATCHED.glob("*.s")))
    if SRC_SEMI.exists():
        counts['semi_matched'] = len(list(SRC_SEMI.glob("*.c")))

    # Overlays
    if SRC_OVERLAY.exists():
        for overlay_dir in SRC_OVERLAY.iterdir():
            if overlay_dir.is_dir() and not overlay_dir.name.startswith('.'):
                c_count = len(list(overlay_dir.glob("*.c")))
                counts['overlays'][overlay_dir.name] = c_count
                counts['overlay_total'] += c_count

    counts['main_total'] = counts['matched_c'] + counts['matched_asm'] + counts['semi_matched']
    counts['grand_total'] = counts['main_total'] + counts['overlay_total']

    return counts


def run_main_binary_build(no_cache=False, verbose=False):
    """Run main binary build with level3 matching."""
    print("\n" + "=" * 70)
    print("MAIN BINARY VERIFICATION")
    print("=" * 70)

    cmd = [sys.executable, str(BUILD_LEVEL3_PY)]
    if no_cache:
        cmd.append("--no-cache")
    if verbose:
        cmd.append("--verbose")

    start = time.time()
    result = subprocess.run(cmd, capture_output=False)
    elapsed = time.time() - start

    success = result.returncode == 0

    return {
        'success': success,
        'elapsed': elapsed,
    }


def run_overlay_verification(overlay_name=None, verbose=False):
    """Run overlay verification for one or all overlays."""
    if overlay_name:
        cmd = [sys.executable, str(BUILD_OVERLAY_PY), "--verify", overlay_name]
    else:
        cmd = [sys.executable, str(BUILD_OVERLAY_PY), "--verify-all"]

    if verbose:
        cmd.append("--verbose")

    result = subprocess.run(cmd, capture_output=True, text=True)

    # Parse results from output
    stats = {
        'total': 0,
        'matched': 0,
        'will_match': 0,
        'unmatched': 0,
        'overlays': {},
    }

    current_overlay = None
    for line in result.stdout.split('\n'):
        if line.startswith("Verifying "):
            current_overlay = line.replace("Verifying ", "").strip()
        elif "Total functions:" in line:
            try:
                stats['total'] += int(line.split(":")[1].strip())
            except (ValueError, IndexError):
                pass
        elif "Matched:" in line and "Will match" not in line:
            try:
                stats['matched'] += int(line.split(":")[1].strip())
            except (ValueError, IndexError):
                pass
        elif "Will match:" in line:
            try:
                stats['will_match'] += int(line.split(":")[1].split("(")[0].strip())
            except (ValueError, IndexError):
                pass
        elif "Unmatched:" in line:
            try:
                stats['unmatched'] += int(line.split(":")[1].strip())
            except (ValueError, IndexError):
                pass

    return {
        'success': result.returncode == 0 or stats['unmatched'] == 0,
        'stats': stats,
        'output': result.stdout,
    }


def print_summary(counts, main_result=None, overlay_result=None):
    """Print combined summary."""
    print("\n" + "=" * 70)
    print("COMBINED VERIFICATION SUMMARY")
    print("=" * 70)

    print(f"\nFunction Counts:")
    print(f"  Main Binary:")
    print(f"    Matched C:      {counts['matched_c']:,}")
    print(f"    Matched ASM:    {counts['matched_asm']:,}")
    print(f"    Semi-matched:   {counts['semi_matched']:,}")
    print(f"    Subtotal:       {counts['main_total']:,}")
    print(f"  Overlays:         {counts['overlay_total']:,} (across {len(counts['overlays'])} overlays)")
    print(f"  GRAND TOTAL:      {counts['grand_total']:,}")

    if main_result:
        print(f"\nMain Binary Build:")
        status = "PASS (SHA1 verified)" if main_result['success'] else "FAIL"
        print(f"  Status: {status}")
        print(f"  Time:   {main_result['elapsed']:.1f}s")

    if overlay_result:
        stats = overlay_result['stats']
        print(f"\nOverlay Verification:")
        print(f"  Direct matches:    {stats['matched']:,}")
        print(f"  PostProc matches:  {stats['will_match']:,}")
        print(f"  Unmatched:         {stats['unmatched']:,}")
        print(f"  Total:             {stats['total']:,}")

        effective = stats['matched'] + stats['will_match']
        if stats['total'] > 0:
            pct = 100.0 * effective / stats['total']
            print(f"  Effective match:   {effective:,} ({pct:.1f}%)")

    # Combined totals
    if main_result and overlay_result:
        print(f"\nCombined Results:")
        main_matched = counts['matched_c'] + counts['matched_asm']
        overlay_matched = overlay_result['stats']['matched'] + overlay_result['stats']['will_match']
        total_matched = main_matched + overlay_matched
        total_funcs = counts['grand_total']

        print(f"  Total functions:   {total_funcs:,}")
        print(f"  Total matched:     {total_matched:,}")
        if total_funcs > 0:
            pct = 100.0 * total_matched / total_funcs
            print(f"  Match rate:        {pct:.1f}%")

        all_pass = main_result['success'] and overlay_result['success']
        print(f"\n  OVERALL: {'PASS' if all_pass else 'FAIL'}")


def main():
    parser = argparse.ArgumentParser(description="Unified function verification")
    parser.add_argument("--main-only", action="store_true", help="Verify main binary only")
    parser.add_argument("--overlay-only", action="store_true", help="Verify overlays only")
    parser.add_argument("--no-cache", action="store_true", help="Disable build cache")
    parser.add_argument("--summary", action="store_true", help="Show summary only (no build)")
    parser.add_argument("--verbose", "-v", action="store_true", help="Verbose output")
    parser.add_argument("--overlay", type=str, help="Verify specific overlay")
    args = parser.parse_args()

    # Always count functions
    print("Counting functions...")
    counts = count_functions()

    if args.summary:
        print_summary(counts)
        return 0

    main_result = None
    overlay_result = None

    start_total = time.time()

    # Run main binary build
    if not args.overlay_only:
        main_result = run_main_binary_build(no_cache=args.no_cache, verbose=args.verbose)

    # Run overlay verification
    if not args.main_only:
        print("\n" + "=" * 70)
        print("OVERLAY VERIFICATION")
        print("=" * 70)
        overlay_result = run_overlay_verification(
            overlay_name=args.overlay,
            verbose=args.verbose
        )
        if args.verbose:
            print(overlay_result['output'])

    elapsed_total = time.time() - start_total

    # Print summary
    print_summary(counts, main_result, overlay_result)
    print(f"\nTotal time: {elapsed_total:.1f}s")

    # Return code
    success = True
    if main_result and not main_result['success']:
        success = False
    if overlay_result and not overlay_result['success']:
        success = False

    return 0 if success else 1


if __name__ == "__main__":
    sys.exit(main())
