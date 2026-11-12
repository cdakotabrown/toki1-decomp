#!/usr/bin/env python3
"""
Match debug symbols to functions using proximity matching.

Since the o.bin symbols are from a development build with slightly different
addresses, we use proximity matching to find likely symbol names for functions.
"""

import json
import re
from pathlib import Path
from collections import defaultdict
from bisect import bisect_left

BASE_DIR = Path(".")
SYMBOL_MAP = BASE_DIR / "build/slps00065/symbols/symbol_map.json"
ASM_DIR = BASE_DIR / "asm/main/matched"
SRC_DIR = BASE_DIR / "src/main/matched"
OUTPUT_DIR = BASE_DIR / "build/symbol_audit"


def load_debug_symbols():
    """Load debug symbols sorted by address."""
    with open(SYMBOL_MAP) as f:
        data = json.load(f)

    symbols = []
    for sym in data["symbols"]:
        addr = int(sym["addr"], 16)
        symbols.append((addr, sym["name"]))

    return sorted(symbols, key=lambda x: x[0])


def extract_address_from_filename(filename):
    """Extract hex address from func_XXXXXXXX.s or func_XXXXXXXX.c filename."""
    match = re.search(r'(?:func_|FUN_)([0-9A-Fa-f]{8})', filename)
    if match:
        return int(match.group(1), 16)
    return None


def get_all_function_addresses():
    """Get all function addresses from ASM and C files."""
    functions = {}

    for f in ASM_DIR.glob("*.s"):
        addr = extract_address_from_filename(f.name)
        if addr:
            functions[addr] = {"file": f.name, "type": "asm", "path": str(f)}

    for f in SRC_DIR.glob("*.c"):
        addr = extract_address_from_filename(f.name)
        if addr:
            functions[addr] = {"file": f.name, "type": "c", "path": str(f)}

    return functions


def find_nearest_symbols(func_addr, symbols, max_distance=0x100):
    """Find nearest symbols within max_distance."""
    addrs = [s[0] for s in symbols]

    # Binary search for closest
    idx = bisect_left(addrs, func_addr)

    candidates = []

    # Check symbols before
    if idx > 0:
        prev_addr, prev_name = symbols[idx - 1]
        dist = func_addr - prev_addr
        if dist <= max_distance:
            candidates.append((dist, prev_addr, prev_name, "after"))

    # Check symbols at and after
    for i in range(idx, min(idx + 3, len(symbols))):
        sym_addr, sym_name = symbols[i]
        dist = abs(sym_addr - func_addr)
        if dist <= max_distance:
            direction = "before" if sym_addr > func_addr else "exact" if dist == 0 else "after"
            candidates.append((dist, sym_addr, sym_name, direction))

    return sorted(candidates)


def categorize_region(addr):
    """Categorize address by compiler region."""
    if 0x80041000 <= addr < 0x80086000:
        return "gcc"
    elif 0x80086000 <= addr < 0x800C0000:
        return "ido"
    else:
        return "other"


def main():
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

    print("=" * 70)
    print("Debug Symbol Proximity Matching")
    print("=" * 70)

    # Load data
    print("\nLoading debug symbols...")
    symbols = load_debug_symbols()
    print(f"  Loaded {len(symbols)} debug symbols")
    print(f"  Address range: 0x{symbols[0][0]:08X} - 0x{symbols[-1][0]:08X}")

    print("\nLoading function addresses...")
    functions = get_all_function_addresses()
    func_addrs = sorted(functions.keys())
    print(f"  Found {len(functions)} functions")
    print(f"  Address range: 0x{func_addrs[0]:08X} - 0x{func_addrs[-1]:08X}")

    # Find overlapping range
    overlap_start = max(symbols[0][0], func_addrs[0])
    overlap_end = min(symbols[-1][0], func_addrs[-1])
    print(f"\nOverlapping range: 0x{overlap_start:08X} - 0x{overlap_end:08X}")

    # Count functions in overlapping range
    funcs_in_range = [a for a in func_addrs if overlap_start <= a <= overlap_end]
    print(f"Functions in overlapping range: {len(funcs_in_range)}")

    # Proximity matching
    print("\n" + "=" * 70)
    print("Proximity Matching (within 0x100 bytes)")
    print("=" * 70)

    exact_matches = []
    close_matches = []  # within 16 bytes
    near_matches = []   # within 256 bytes
    no_matches = []

    for addr in sorted(functions.keys()):
        func_info = functions[addr]
        candidates = find_nearest_symbols(addr, symbols, max_distance=0x100)

        if candidates:
            best = candidates[0]
            dist, sym_addr, sym_name, direction = best

            match_info = {
                "func_addr": f"0x{addr:08X}",
                "func_file": func_info["file"],
                "func_type": func_info["type"],
                "region": categorize_region(addr),
                "sym_addr": f"0x{sym_addr:08X}",
                "sym_name": sym_name,
                "distance": dist,
                "direction": direction
            }

            if dist == 0:
                exact_matches.append(match_info)
            elif dist <= 16:
                close_matches.append(match_info)
            else:
                near_matches.append(match_info)
        else:
            no_matches.append({
                "func_addr": f"0x{addr:08X}",
                "func_file": func_info["file"],
                "func_type": func_info["type"],
                "region": categorize_region(addr)
            })

    print(f"\nExact matches (distance=0): {len(exact_matches)}")
    print(f"Close matches (distance<=16): {len(close_matches)}")
    print(f"Near matches (distance<=256): {len(near_matches)}")
    print(f"No nearby symbol: {len(no_matches)}")

    # Show exact matches
    if exact_matches:
        print("\n--- Exact Matches ---")
        for m in exact_matches[:30]:
            print(f"  {m['func_addr']} {m['func_type']:3} = {m['sym_name']}")
        if len(exact_matches) > 30:
            print(f"  ... and {len(exact_matches) - 30} more")

    # Show close matches
    if close_matches:
        print("\n--- Close Matches (1-16 bytes) ---")
        for m in close_matches[:30]:
            print(f"  {m['func_addr']} {m['func_type']:3} ~ {m['sym_name']} (off by {m['distance']})")
        if len(close_matches) > 30:
            print(f"  ... and {len(close_matches) - 30} more")

    # Analyze offset patterns
    print("\n" + "=" * 70)
    print("Offset Pattern Analysis")
    print("=" * 70)

    all_matches = exact_matches + close_matches + near_matches
    offsets = defaultdict(int)
    for m in all_matches:
        func_addr = int(m["func_addr"], 16)
        sym_addr = int(m["sym_addr"], 16)
        offset = func_addr - sym_addr
        offsets[offset] += 1

    print("\nMost common offsets:")
    for offset, count in sorted(offsets.items(), key=lambda x: -x[1])[:20]:
        print(f"  {offset:+5d} (0x{offset & 0xFFFF:04X}): {count} functions")

    # Generate reports
    print("\n" + "=" * 70)
    print("Generating Reports")
    print("=" * 70)

    report = {
        "summary": {
            "total_functions": len(functions),
            "exact_matches": len(exact_matches),
            "close_matches": len(close_matches),
            "near_matches": len(near_matches),
            "no_matches": len(no_matches)
        },
        "exact_matches": exact_matches,
        "close_matches": close_matches,
        "near_matches": near_matches,
        "no_matches": no_matches,
        "offset_patterns": dict(sorted(offsets.items(), key=lambda x: -x[1])[:50])
    }

    report_path = OUTPUT_DIR / "proximity_match_report.json"
    with open(report_path, "w") as f:
        json.dump(report, f, indent=2)
    print(f"  Wrote: {report_path}")

    # Generate actionable rename list (exact + close matches)
    rename_actions = []
    for m in exact_matches + close_matches:
        current_stem = Path(m["func_file"]).stem
        if current_stem.lower().startswith("func_") or current_stem.lower().startswith("fun_"):
            rename_actions.append({
                "address": m["func_addr"],
                "current_file": m["func_file"],
                "suggested_name": m["sym_name"],
                "type": m["func_type"],
                "confidence": "exact" if m["distance"] == 0 else "high",
                "offset": m["distance"]
            })

    rename_path = OUTPUT_DIR / "high_confidence_renames.json"
    with open(rename_path, "w") as f:
        json.dump(rename_actions, f, indent=2)
    print(f"  Wrote: {rename_path}")

    # Generate markdown summary
    summary_path = OUTPUT_DIR / "PROXIMITY_MATCH_SUMMARY.md"
    with open(summary_path, "w") as f:
        f.write("# Debug Symbol Proximity Matching Results\n\n")
        f.write("## Summary\n\n")
        f.write(f"| Category | Count |\n")
        f.write(f"|----------|-------|\n")
        f.write(f"| Total Functions | {len(functions)} |\n")
        f.write(f"| Exact Matches | {len(exact_matches)} |\n")
        f.write(f"| Close Matches (≤16 bytes) | {len(close_matches)} |\n")
        f.write(f"| Near Matches (≤256 bytes) | {len(near_matches)} |\n")
        f.write(f"| No Nearby Symbol | {len(no_matches)} |\n")
        f.write(f"\n")
        f.write(f"## High-Confidence Matches (Exact + Close)\n\n")
        f.write(f"These {len(exact_matches) + len(close_matches)} functions can be renamed with high confidence:\n\n")
        f.write(f"| Address | Current File | Debug Symbol | Offset | Type |\n")
        f.write(f"|---------|--------------|--------------|--------|------|\n")
        for m in exact_matches + close_matches:
            f.write(f"| {m['func_addr']} | {m['func_file']} | {m['sym_name']} | {m['distance']} | {m['func_type']} |\n")
        f.write(f"\n")
        f.write(f"## Offset Pattern Analysis\n\n")
        f.write(f"Most common offsets (function_addr - symbol_addr):\n\n")
        f.write(f"| Offset | Count |\n")
        f.write(f"|--------|-------|\n")
        for offset, count in sorted(offsets.items(), key=lambda x: -x[1])[:20]:
            f.write(f"| {offset:+d} | {count} |\n")

    print(f"  Wrote: {summary_path}")
    print("\n" + "=" * 70)
    print("Done!")
    print("=" * 70)


if __name__ == "__main__":
    main()
