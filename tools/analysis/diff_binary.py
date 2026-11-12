#!/usr/bin/env python3
"""
diff_binary.py - Quick binary comparison tool

Compares rebuilt binary against original and shows differences.
Simpler alternative to full objdiff for quick checks.

Usage:
    python3 tools/diff_binary.py
    python3 tools/diff_binary.py --verbose
    python3 tools/diff_binary.py --bytes 100  # Show first 100 differing bytes
"""

import argparse
import sys
from pathlib import Path

ROOT_DIR = Path(__file__).parent.parent
ORIGINAL = ROOT_DIR / "disks/SLPS_000.65"
REBUILT = ROOT_DIR / "build/jp/SLPS_000.65.new"
EXPECTED_SHA1 = "75760c51d18f0a19c02d11cc04f3397933c4f78b"

def compare_binaries(max_diffs=20, verbose=False):
    """
    Compare original and rebuilt binaries.

    Args:
        max_diffs: Maximum number of differences to display
        verbose: Show detailed information
    """
    # Check files exist
    if not ORIGINAL.exists():
        print(f"Error: Original binary not found: {ORIGINAL}", file=sys.stderr)
        sys.exit(1)

    if not REBUILT.exists():
        print(f"Error: Rebuilt binary not found: {REBUILT}", file=sys.stderr)
        print("Run: make build", file=sys.stderr)
        sys.exit(1)

    # Read binaries
    with open(ORIGINAL, 'rb') as f:
        original_data = f.read()

    with open(REBUILT, 'rb') as f:
        rebuilt_data = f.read()

    # Check sizes
    orig_size = len(original_data)
    rebuilt_size = len(rebuilt_data)

    print("=" * 80)
    print("BINARY COMPARISON")
    print("=" * 80)
    print()

    print(f"Original:  {ORIGINAL}")
    print(f"Rebuilt:   {REBUILT}")
    print()

    print(f"Original size:  {orig_size:,} bytes")
    print(f"Rebuilt size:   {rebuilt_size:,} bytes")

    if orig_size != rebuilt_size:
        diff = rebuilt_size - orig_size
        print(f"Size difference: {diff:+,} bytes")
        print()
        print("❌ SIZE MISMATCH! Binaries have different sizes.")
        return False

    print()

    # Find differences
    differences = []
    for offset in range(orig_size):
        if original_data[offset] != rebuilt_data[offset]:
            differences.append((
                offset,
                original_data[offset],
                rebuilt_data[offset]
            ))

    diff_count = len(differences)

    if diff_count == 0:
        print("✅ PERFECT MATCH! Binaries are identical.")
        print()
        print(f"SHA1: {EXPECTED_SHA1}")
        return True

    # Show differences
    print(f"❌ MISMATCH! Found {diff_count:,} differing bytes ({diff_count / orig_size * 100:.2f}%)")
    print()

    if verbose or diff_count <= max_diffs:
        display_count = min(diff_count, max_diffs)
        print(f"First {display_count} differences:")
        print()
        print("  Offset      Original   Rebuilt    Diff")
        print("  " + "-" * 45)

        for i in range(display_count):
            offset, orig_byte, rebuilt_byte = differences[i]
            diff_val = rebuilt_byte - orig_byte
            print(f"  0x{offset:08X}    0x{orig_byte:02X}       0x{rebuilt_byte:02X}     {diff_val:+4d}")

        if diff_count > max_diffs:
            print(f"  ... and {diff_count - max_diffs:,} more differences")
    else:
        print(f"Use --bytes {diff_count} to see all differences")

    print()

    # Find continuous regions of differences
    if verbose and diff_count > 1:
        regions = []
        start_offset = differences[0][0]
        end_offset = start_offset
        last_offset = start_offset

        for offset, _, _ in differences[1:]:
            if offset == last_offset + 1:
                # Continuous region
                end_offset = offset
            else:
                # Gap found, save region
                regions.append((start_offset, end_offset, end_offset - start_offset + 1))
                start_offset = offset
                end_offset = offset
            last_offset = offset

        # Save last region
        regions.append((start_offset, end_offset, end_offset - start_offset + 1))

        print(f"Difference regions ({len(regions)} regions):")
        print()
        print("  Start       End         Length")
        print("  " + "-" * 40)

        for start, end, length in regions[:20]:
            print(f"  0x{start:08X}  0x{end:08X}  {length:>6} bytes")

        if len(regions) > 20:
            print(f"  ... and {len(regions) - 20} more regions")

    print()
    print("=" * 80)

    return False

def main():
    parser = argparse.ArgumentParser(
        description="Compare rebuilt binary against original",
        epilog="""
Examples:
    python3 tools/diff_binary.py
    python3 tools/diff_binary.py --verbose
    python3 tools/diff_binary.py --bytes 100
        """
    )
    parser.add_argument(
        '-v', '--verbose',
        action='store_true',
        help='Show detailed difference analysis'
    )
    parser.add_argument(
        '-b', '--bytes',
        type=int,
        default=20,
        help='Maximum number of differing bytes to display (default: 20)'
    )

    args = parser.parse_args()

    match = compare_binaries(max_diffs=args.bytes, verbose=args.verbose)
    sys.exit(0 if match else 1)

if __name__ == "__main__":
    main()
