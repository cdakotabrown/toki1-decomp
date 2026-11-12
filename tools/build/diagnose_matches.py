#!/usr/bin/env python3
"""
Diagnose which C functions are matching/mismatching and why.

This helps identify functions that need post-processing documentation.
"""

import json
import sys
from pathlib import Path

# Add parent to path for imports
sys.path.insert(0, str(Path(__file__).parent))
from parse_postproc import parse_postproc_header

ROOT_DIR = Path(__file__).parent.parent.parent

def main():
    # Parse build output
    build_output = Path("/tmp/build_output.txt").read_text()

    # Load config
    config_path = ROOT_DIR / "config/level3_functions.json"
    with open(config_path, 'r') as f:
        config = json.load(f)

    matched_c = []
    mismatched_c = []

    for func in config['functions']:
        source = func['source']
        if not source.endswith('.c'):
            continue

        name = func['name']
        addr = func['address']

        # Check if matched or mismatched in build output
        if f"{name}:\n" in build_output:
            # Find the section for this function
            start = build_output.find(f"{name}:\n")
            end = build_output.find("\n\n  ", start)
            if end == -1:
                end = len(build_output)
            section = build_output[start:end]

            if "MATCH!" in section:
                matched_c.append(func)
            elif "MISMATCH:" in section:
                mismatched_c.append(func)

    # Analyze matched functions
    print("=" * 70)
    print(f"MATCHED C FUNCTIONS: {len(matched_c)}")
    print("=" * 70)

    with_postproc = 0
    without_postproc = 0

    for func in matched_c:
        source_path = ROOT_DIR / func['source']
        postproc = parse_postproc_header(source_path)
        if postproc:
            with_postproc += 1
        else:
            without_postproc += 1

    print(f"With post-processing:    {with_postproc}")
    print(f"Without post-processing: {without_postproc}")

    # Analyze mismatched functions
    print(f"\n{'=' * 70}")
    print(f"MISMATCHED C FUNCTIONS: {len(mismatched_c)}")
    print("=" * 70)

    with_postproc_m = 0
    without_postproc_m = 0
    gcc_region_m = 0
    ido_region_m = 0

    print("\nFirst 20 mismatches:\n")
    for i, func in enumerate(mismatched_c[:20]):
        name = func['name']
        addr = int(func['address'], 16)
        source_path = ROOT_DIR / func['source']
        postproc = parse_postproc_header(source_path)
        region = "GCC" if addr < 0x80086000 else "IDO"

        if postproc:
            with_postproc_m += 1
        else:
            without_postproc_m += 1

        if region == "GCC":
            gcc_region_m += 1
        else:
            ido_region_m += 1

        postproc_str = f"{len(postproc)} steps" if postproc else "none"
        print(f"  {name:30s} {func['address']} {region:3s} postproc={postproc_str}")

    print(f"\n{'.' * 70}")
    print(f"With documented post-processing:    {with_postproc_m}")
    print(f"Without documented post-processing: {without_postproc_m}")
    print(f"GCC region:                         {gcc_region_m}")
    print(f"IDO region:                         {ido_region_m}")

    print(f"\n{'=' * 70}")
    print("SUMMARY")
    print("=" * 70)
    print(f"Total C functions:     {len(matched_c) + len(mismatched_c)}")
    print(f"Matching:              {len(matched_c)} ({100*len(matched_c)//(len(matched_c)+len(mismatched_c))}%)")
    print(f"Mismatching:           {len(mismatched_c)} ({100*len(mismatched_c)//(len(matched_c)+len(mismatched_c))}%)")
    print()
    print("Next steps:")
    print("1. Review mismatched functions to determine if they need post-processing")
    print("2. Document required post-processing in C file headers")
    print("3. For functions that can't match, remove from level3_functions.json")


if __name__ == '__main__':
    main()
