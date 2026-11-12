#!/usr/bin/env python3
"""
Audit debug symbol coverage across matched C and ASM code.

Cross-references the 1,954 debug symbols from o.bin with our matched functions
to identify naming coverage and opportunities to apply proper function names.
"""

import json
import os
import re
from pathlib import Path
from collections import defaultdict

BASE_DIR = Path(".")
SYMBOL_MAP = BASE_DIR / "build/slps00065/symbols/symbol_map.json"
ASM_DIR = BASE_DIR / "asm/main/matched"
SRC_DIR = BASE_DIR / "src/main/matched"
OUTPUT_DIR = BASE_DIR / "build/symbol_audit"


def load_debug_symbols():
    """Load debug symbols from JSON."""
    with open(SYMBOL_MAP) as f:
        data = json.load(f)

    symbols = {}
    for sym in data["symbols"]:
        addr = int(sym["addr"], 16)
        symbols[addr] = sym["name"]

    return symbols


def extract_address_from_filename(filename):
    """Extract hex address from func_XXXXXXXX.s or func_XXXXXXXX.c filename."""
    match = re.search(r'(?:func_|FUN_)([0-9A-Fa-f]{8})', filename)
    if match:
        return int(match.group(1), 16)
    return None


def scan_asm_files():
    """Scan ASM directory and extract function addresses."""
    functions = {}
    for f in ASM_DIR.glob("*.s"):
        addr = extract_address_from_filename(f.name)
        if addr:
            functions[addr] = {
                "file": f.name,
                "path": str(f),
                "type": "asm"
            }
    return functions


def scan_c_files():
    """Scan C source directory and extract function addresses."""
    functions = {}
    for f in SRC_DIR.glob("*.c"):
        addr = extract_address_from_filename(f.name)
        if addr:
            functions[addr] = {
                "file": f.name,
                "path": str(f),
                "type": "c"
            }
    return functions


def categorize_by_region(addr):
    """Categorize address by compiler region."""
    if 0x80041000 <= addr < 0x80086000:
        return "gcc"
    elif 0x80086000 <= addr < 0x800C0000:
        return "ido"
    else:
        return "other"


def main():
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

    print("=" * 60)
    print("Debug Symbol Coverage Audit")
    print("=" * 60)

    # Load data
    print("\nLoading debug symbols...")
    symbols = load_debug_symbols()
    print(f"  Loaded {len(symbols)} debug symbols")

    print("\nScanning matched ASM files...")
    asm_funcs = scan_asm_files()
    print(f"  Found {len(asm_funcs)} ASM functions")

    print("\nScanning matched C files...")
    c_funcs = scan_c_files()
    print(f"  Found {len(c_funcs)} C functions")

    # Merge all functions
    all_funcs = {}
    all_funcs.update(asm_funcs)
    all_funcs.update(c_funcs)
    print(f"\nTotal matched functions: {len(all_funcs)}")

    # Cross-reference
    print("\n" + "=" * 60)
    print("Cross-Reference Results")
    print("=" * 60)

    matched = []
    unmatched_funcs = []

    for addr, func_info in sorted(all_funcs.items()):
        if addr in symbols:
            matched.append({
                "address": f"0x{addr:08X}",
                "current_name": func_info["file"],
                "debug_name": symbols[addr],
                "type": func_info["type"],
                "region": categorize_by_region(addr),
                "path": func_info["path"]
            })
        else:
            unmatched_funcs.append({
                "address": f"0x{addr:08X}",
                "current_name": func_info["file"],
                "type": func_info["type"],
                "region": categorize_by_region(addr)
            })

    # Find symbols that don't have matched functions
    matched_addrs = set(all_funcs.keys())
    unused_symbols = []
    for addr, name in sorted(symbols.items()):
        if addr not in matched_addrs:
            unused_symbols.append({
                "address": f"0x{addr:08X}",
                "name": name,
                "region": categorize_by_region(addr)
            })

    # Statistics
    print(f"\nFunctions with debug symbols: {len(matched)}")
    print(f"Functions without debug symbols: {len(unmatched_funcs)}")
    print(f"Debug symbols without matched functions: {len(unused_symbols)}")

    # Breakdown by type
    asm_matched = [m for m in matched if m["type"] == "asm"]
    c_matched = [m for m in matched if m["type"] == "c"]

    print(f"\nBreakdown by source type:")
    print(f"  ASM with symbols: {len(asm_matched)}")
    print(f"  C with symbols: {len(c_matched)}")

    # Breakdown by region
    gcc_matched = [m for m in matched if m["region"] == "gcc"]
    ido_matched = [m for m in matched if m["region"] == "ido"]

    print(f"\nBreakdown by compiler region:")
    print(f"  GCC region (0x80041000-0x80085FFF): {len(gcc_matched)} matched")
    print(f"  IDO region (0x80086000-0x800BFFFF): {len(ido_matched)} matched")

    # Coverage percentage
    coverage_pct = (len(matched) / len(all_funcs) * 100) if all_funcs else 0
    print(f"\nOverall symbol coverage: {coverage_pct:.1f}%")

    # Sample matches
    print("\n" + "=" * 60)
    print("Sample Matches (first 30)")
    print("=" * 60)
    for m in matched[:30]:
        print(f"  {m['address']} {m['type']:3} -> {m['debug_name']}")

    # Generate detailed reports
    print("\n" + "=" * 60)
    print("Generating Reports")
    print("=" * 60)

    # Full match report
    match_report = {
        "summary": {
            "total_functions": len(all_funcs),
            "functions_with_symbols": len(matched),
            "functions_without_symbols": len(unmatched_funcs),
            "unused_symbols": len(unused_symbols),
            "coverage_percent": round(coverage_pct, 1),
            "asm_with_symbols": len(asm_matched),
            "c_with_symbols": len(c_matched),
            "gcc_region_matched": len(gcc_matched),
            "ido_region_matched": len(ido_matched)
        },
        "matched_functions": matched,
        "unmatched_functions": unmatched_funcs,
        "unused_symbols": unused_symbols
    }

    report_path = OUTPUT_DIR / "symbol_coverage_report.json"
    with open(report_path, 'w') as f:
        json.dump(match_report, f, indent=2)
    print(f"  Wrote: {report_path}")

    # Generate rename suggestions (for files that should be renamed)
    rename_suggestions = []
    for m in matched:
        current = Path(m["current_name"]).stem
        debug = m["debug_name"]
        if current.lower() != debug.lower() and not current.startswith("FUN_"):
            rename_suggestions.append({
                "address": m["address"],
                "current": m["current_name"],
                "suggested": f"{debug}.{m['type']}",
                "type": m["type"],
                "region": m["region"]
            })

    rename_path = OUTPUT_DIR / "rename_suggestions.json"
    with open(rename_path, 'w') as f:
        json.dump(rename_suggestions, f, indent=2)
    print(f"  Wrote: {rename_path}")

    # Human-readable summary
    summary_path = OUTPUT_DIR / "SYMBOL_COVERAGE_SUMMARY.md"
    with open(summary_path, 'w') as f:
        f.write("# Debug Symbol Coverage Audit\n\n")
        f.write("## Summary\n\n")
        f.write(f"| Metric | Count |\n")
        f.write(f"|--------|-------|\n")
        f.write(f"| Total Matched Functions | {len(all_funcs)} |\n")
        f.write(f"| Functions with Debug Symbols | {len(matched)} |\n")
        f.write(f"| Functions without Debug Symbols | {len(unmatched_funcs)} |\n")
        f.write(f"| Unused Debug Symbols | {len(unused_symbols)} |\n")
        f.write(f"| Coverage | {coverage_pct:.1f}% |\n")
        f.write(f"\n")
        f.write(f"### By Source Type\n\n")
        f.write(f"| Type | With Symbols | Total |\n")
        f.write(f"|------|--------------|-------|\n")
        f.write(f"| ASM | {len(asm_matched)} | {len(asm_funcs)} |\n")
        f.write(f"| C | {len(c_matched)} | {len(c_funcs)} |\n")
        f.write(f"\n")
        f.write(f"### By Compiler Region\n\n")
        f.write(f"| Region | With Symbols |\n")
        f.write(f"|--------|-------------|\n")
        f.write(f"| GCC (0x80041000-0x80085FFF) | {len(gcc_matched)} |\n")
        f.write(f"| IDO (0x80086000-0x800BFFFF) | {len(ido_matched)} |\n")
        f.write(f"\n")
        f.write(f"## Matched Functions\n\n")
        f.write(f"| Address | Current File | Debug Name | Type | Region |\n")
        f.write(f"|---------|--------------|------------|------|--------|\n")
        for m in matched[:100]:
            f.write(f"| {m['address']} | {m['current_name']} | {m['debug_name']} | {m['type']} | {m['region']} |\n")
        if len(matched) > 100:
            f.write(f"\n... and {len(matched) - 100} more (see symbol_coverage_report.json)\n")

        f.write(f"\n## Functions Without Debug Symbols\n\n")
        f.write(f"| Address | Current File | Type | Region |\n")
        f.write(f"|---------|--------------|------|--------|\n")
        for m in unmatched_funcs[:50]:
            f.write(f"| {m['address']} | {m['current_name']} | {m['type']} | {m['region']} |\n")
        if len(unmatched_funcs) > 50:
            f.write(f"\n... and {len(unmatched_funcs) - 50} more\n")

    print(f"  Wrote: {summary_path}")

    print("\n" + "=" * 60)
    print("Audit Complete!")
    print("=" * 60)

    return match_report


if __name__ == "__main__":
    main()
