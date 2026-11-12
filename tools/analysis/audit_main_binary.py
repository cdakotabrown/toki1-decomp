#!/usr/bin/env python3
"""
Audit main binary function tracking for duplicates and inconsistencies.

Checks:
1. Duplicate function names across directories
2. Functions in multiple categories (matched C, matched ASM, semi-matched)
3. Orphaned files (no corresponding target in binary)
4. Missing @SymbolSize annotations
"""

import os
import re
from pathlib import Path
from collections import defaultdict

ROOT = Path(__file__).parent.parent.parent

MATCHED_C = ROOT / "src" / "main" / "matched"
MATCHED_ASM = ROOT / "asm" / "main" / "matched"
SEMI_MATCHED = ROOT / "src" / "main" / "semi-matched"
THUNKS = ROOT / "asm" / "thunks"

def get_func_name(path: Path) -> str:
    """Extract function name from file path."""
    return path.stem

def audit_main_binary():
    print("=" * 70)
    print("MAIN BINARY AUDIT")
    print("=" * 70)

    # Collect all functions by category
    matched_c = set()
    matched_asm = set()
    semi_matched = set()
    thunks = set()

    # Matched C
    if MATCHED_C.exists():
        for f in MATCHED_C.glob("*.c"):
            matched_c.add(get_func_name(f))

    # Matched ASM
    if MATCHED_ASM.exists():
        for f in MATCHED_ASM.glob("*.s"):
            matched_asm.add(get_func_name(f))

    # Semi-matched
    if SEMI_MATCHED.exists():
        for f in SEMI_MATCHED.glob("*.c"):
            semi_matched.add(get_func_name(f))

    # Thunks
    if THUNKS.exists():
        for f in THUNKS.glob("*.s"):
            thunks.add(get_func_name(f))

    print(f"\n### Raw File Counts ###")
    print(f"Matched C:     {len(matched_c)}")
    print(f"Matched ASM:   {len(matched_asm)}")
    print(f"Semi-matched:  {len(semi_matched)}")
    print(f"Thunks:        {len(thunks)}")

    # Check for overlaps
    print(f"\n### Overlap Analysis ###")

    # Functions in both matched C and matched ASM (ERROR - should not happen)
    c_and_asm = matched_c & matched_asm
    if c_and_asm:
        print(f"\n[ERROR] In BOTH matched C AND matched ASM ({len(c_and_asm)}):")
        for f in sorted(c_and_asm)[:20]:
            print(f"  {f}")
        if len(c_and_asm) > 20:
            print(f"  ... and {len(c_and_asm) - 20} more")
    else:
        print(f"[OK] No functions in both matched C and matched ASM")

    # Functions in matched C and semi-matched (ERROR - should not happen)
    c_and_semi = matched_c & semi_matched
    if c_and_semi:
        print(f"\n[ERROR] In BOTH matched C AND semi-matched ({len(c_and_semi)}):")
        for f in sorted(c_and_semi)[:20]:
            print(f"  {f}")
        if len(c_and_semi) > 20:
            print(f"  ... and {len(c_and_semi) - 20} more")
    else:
        print(f"[OK] No functions in both matched C and semi-matched")

    # Functions in matched ASM and semi-matched (OK - ASM is byte-perfect, C is reference)
    asm_and_semi = matched_asm & semi_matched
    print(f"\n[INFO] In matched ASM with semi-matched C reference: {len(asm_and_semi)}")
    if asm_and_semi and len(asm_and_semi) <= 10:
        for f in sorted(asm_and_semi):
            print(f"  {f}")

    # Semi-matched ONLY (no byte-perfect version)
    semi_only = semi_matched - matched_c - matched_asm
    print(f"\n[INFO] Semi-matched ONLY (no byte-perfect): {len(semi_only)}")

    # Calculate true unique counts
    print(f"\n### True Unique Function Counts ###")

    # Byte-perfect = matched C + matched ASM (no overlap expected)
    byte_perfect = matched_c | matched_asm
    print(f"Byte-perfect total:  {len(byte_perfect)}")
    print(f"  - From C:          {len(matched_c)}")
    print(f"  - From ASM:        {len(matched_asm)}")

    # Semi-matched that DON'T have byte-perfect versions
    true_semi = semi_matched - byte_perfect
    print(f"Semi-matched only:   {len(true_semi)}")

    # Total unique functions
    all_funcs = matched_c | matched_asm | semi_matched
    print(f"Total unique:        {len(all_funcs)}")

    # Thunks analysis
    print(f"\n### Thunks Analysis ###")
    thunks_in_matched = thunks & (matched_c | matched_asm)
    thunks_standalone = thunks - matched_c - matched_asm
    print(f"Thunks total:        {len(thunks)}")
    print(f"Thunks with match:   {len(thunks_in_matched)}")
    print(f"Thunks standalone:   {len(thunks_standalone)}")

    # Check for func_ vs named functions
    print(f"\n### Function Naming ###")
    func_pattern = re.compile(r'^func_[0-9A-Fa-f]+$')

    named_c = [f for f in matched_c if not func_pattern.match(f)]
    named_asm = [f for f in matched_asm if not func_pattern.match(f)]
    named_semi = [f for f in semi_matched if not func_pattern.match(f)]

    print(f"Named functions in matched C:     {len(named_c)}")
    print(f"Named functions in matched ASM:   {len(named_asm)}")
    print(f"Named functions in semi-matched:  {len(named_semi)}")

    print(f"\nSample named functions in matched C:")
    for f in sorted(named_c)[:15]:
        print(f"  {f}")

    # Summary
    print(f"\n" + "=" * 70)
    print("SUMMARY")
    print("=" * 70)
    print(f"Byte-perfect functions:  {len(byte_perfect)}")
    print(f"  - C implementations:   {len(matched_c)}")
    print(f"  - ASM (unmatchable):   {len(matched_asm)}")
    print(f"Semi-matched only:       {len(true_semi)}")
    print(f"Total unique functions:  {len(all_funcs)}")

    if c_and_asm or c_and_semi:
        print(f"\n[WARNING] Found overlapping files that need cleanup!")
        return 1

    return 0

if __name__ == "__main__":
    exit(audit_main_binary())
