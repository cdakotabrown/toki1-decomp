#!/usr/bin/env python3
"""
Quick query tool for frame size data.

Usage:
    python3 tools/analysis/query_frame_size.py pad_main
    python3 tools/analysis/query_frame_size.py 0x80042C80
    python3 tools/analysis/query_frame_size.py --size 40 --region GCC
    python3 tools/analysis/query_frame_size.py --no-patch-needed
"""

import argparse
import json
import sys
from pathlib import Path

ROOT_DIR = Path(__file__).parent.parent.parent
FRAME_DATA = ROOT_DIR / "build" / "frames_all.json"


def load_frame_data():
    """Load the frame size database."""
    if not FRAME_DATA.exists():
        print(f"Error: Frame data not found at {FRAME_DATA}")
        print("Run: python3 tools/analysis/infer_frame_sizes.py --output build/frames_all.json")
        sys.exit(1)

    with open(FRAME_DATA, 'r') as f:
        return json.load(f)


def find_function(data, func_name):
    """Find a function by name or address."""
    # Normalize input
    if func_name.startswith('0x'):
        # Search by address
        for func in data['functions']:
            if func['address'].lower() == func_name.lower():
                return func
    else:
        # Search by name
        for func in data['functions']:
            if func['function_name'] == func_name:
                return func
    return None


def format_function(func):
    """Format function info for display."""
    lines = [
        f"Function: {func['function_name']}",
        f"Address:  {func['address']}",
        f"Size:     {func['size']} bytes",
        f"Region:   {func['region']}",
    ]

    if func['target_frame_size'] is not None:
        lines.extend([
            f"Target frame size:  {func['target_frame_size']} bytes",
            f"IDO default frame:  {func['ido_default_frame']} bytes",
            f"Frame delta:        {func['frame_delta']:+d} bytes",
            f"Needs patching:     {'Yes' if func['needs_patching'] else 'No'}",
        ])

        if func['saved_registers']:
            lines.append(f"Saved registers:    {', '.join(func['saved_registers'])}")

        if func['needs_patching']:
            lines.append(f"\nPatch command:")
            lines.append(f"  python3 tools/ido/patch_stack_complete.py input.o output.o \\")
            lines.append(f"    --old {func['ido_default_frame']} --new {func['target_frame_size']}")
    else:
        lines.append("No prologue detected (leaf function or thunk)")

    if func['analysis_notes']:
        lines.append(f"\nNotes:")
        for note in func['analysis_notes']:
            lines.append(f"  - {note}")

    return '\n'.join(lines)


def main():
    parser = argparse.ArgumentParser(
        description='Query frame size database',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument('function', nargs='?',
                        help='Function name or address (e.g., pad_main or 0x80042C80)')
    parser.add_argument('--size', type=int,
                        help='Find functions with specific frame size')
    parser.add_argument('--region', choices=['GCC', 'IDO'],
                        help='Filter by region')
    parser.add_argument('--no-patch-needed', action='store_true',
                        help='List functions that do not need patching')
    parser.add_argument('--needs-patching', action='store_true',
                        help='List functions that need patching')
    parser.add_argument('--count', action='store_true',
                        help='Show count instead of listing functions')

    args = parser.parse_args()

    data = load_frame_data()

    # Single function lookup
    if args.function:
        func = find_function(data, args.function)
        if func:
            print(format_function(func))
        else:
            print(f"Error: Function '{args.function}' not found in database")
            sys.exit(1)
        return

    # Filter functions
    results = data['functions']

    if args.region:
        results = [f for f in results if f['region'] == args.region]

    if args.size is not None:
        results = [f for f in results if f['target_frame_size'] == args.size]

    if args.no_patch_needed:
        results = [f for f in results if not f['needs_patching']]

    if args.needs_patching:
        results = [f for f in results if f['needs_patching']]

    # Output
    if args.count:
        print(f"Found {len(results)} functions")
    else:
        if not results:
            print("No matching functions found")
        else:
            for func in results:
                frame_str = str(func['target_frame_size']) if func['target_frame_size'] is not None else 'N/A'
                delta_str = str(func['frame_delta']) if func['frame_delta'] is not None else 'N/A'
                print(f"{func['function_name']:20s} @ {func['address']:10s} "
                      f"frame={frame_str:>4s} "
                      f"delta={delta_str:>4s} "
                      f"region={func['region']}")


if __name__ == '__main__':
    main()
