#!/usr/bin/env python3
"""
Audit overlay function tracking for duplicates and inconsistencies.

Checks:
1. Duplicate function names across overlays
2. Mismatch between source files and status JSON
3. Functions in status JSON but no source file
4. Source files not in status JSON
5. Status JSON internal consistency
"""

import json
import os
import re
from pathlib import Path
from collections import defaultdict

ROOT = Path(__file__).parent.parent.parent
OVERLAY_SRC = ROOT / "src" / "overlay"
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"

def audit_overlays():
    print("=" * 70)
    print("OVERLAY AUDIT")
    print("=" * 70)

    # Load status JSON
    if not STATUS_FILE.exists():
        print(f"[ERROR] Status file not found: {STATUS_FILE}")
        return 1

    with open(STATUS_FILE) as f:
        status_data = json.load(f)

    # Collect all source files
    source_files = defaultdict(set)  # overlay -> set of func names
    all_source_funcs = set()  # (overlay, func) tuples

    for overlay_dir in OVERLAY_SRC.iterdir():
        if not overlay_dir.is_dir():
            continue
        overlay = overlay_dir.name
        for c_file in overlay_dir.glob("*.c"):
            func = c_file.stem
            source_files[overlay].add(func)
            all_source_funcs.add((overlay, func))

    # Collect all functions from status JSON
    status_funcs = set()  # (overlay, func) tuples
    status_by_overlay = defaultdict(set)

    for overlay, funcs in status_data.items():
        for func in funcs:
            status_funcs.add((overlay, func))
            status_by_overlay[overlay].add(func)

    print(f"\n### Raw Counts ###")
    print(f"Overlays with source files: {len(source_files)}")
    print(f"Overlays in status JSON:    {len(status_data)}")
    print(f"Total source files:         {len(all_source_funcs)}")
    print(f"Total in status JSON:       {len(status_funcs)}")

    # Check for duplicates across overlays (same func name in multiple overlays)
    print(f"\n### Cross-Overlay Duplicates ###")
    func_to_overlays = defaultdict(list)
    for overlay, func in all_source_funcs:
        func_to_overlays[func].append(overlay)

    duplicates = {f: ovs for f, ovs in func_to_overlays.items() if len(ovs) > 1}
    if duplicates:
        print(f"[INFO] {len(duplicates)} functions appear in multiple overlays:")
        for func, overlays in sorted(duplicates.items())[:20]:
            print(f"  {func}: {', '.join(overlays)}")
        if len(duplicates) > 20:
            print(f"  ... and {len(duplicates) - 20} more")
    else:
        print(f"[OK] No duplicate function names across overlays")

    # Check for mismatches between source and status
    print(f"\n### Source vs Status Consistency ###")

    # In source but not in status
    in_source_not_status = all_source_funcs - status_funcs
    if in_source_not_status:
        print(f"\n[WARNING] {len(in_source_not_status)} source files NOT in status JSON:")
        by_overlay = defaultdict(list)
        for ov, func in in_source_not_status:
            by_overlay[ov].append(func)
        for ov in sorted(by_overlay.keys())[:5]:
            print(f"  {ov}: {len(by_overlay[ov])} files")
            for f in sorted(by_overlay[ov])[:3]:
                print(f"    {f}")
    else:
        print(f"[OK] All source files are in status JSON")

    # In status but not in source
    in_status_not_source = status_funcs - all_source_funcs
    if in_status_not_source:
        print(f"\n[WARNING] {len(in_status_not_source)} status entries with NO source file:")
        by_overlay = defaultdict(list)
        for ov, func in in_status_not_source:
            by_overlay[ov].append(func)
        for ov in sorted(by_overlay.keys())[:5]:
            print(f"  {ov}: {len(by_overlay[ov])} entries")
            for f in sorted(by_overlay[ov])[:3]:
                print(f"    {f}")
    else:
        print(f"[OK] All status entries have source files")

    # Status breakdown
    print(f"\n### Status JSON Breakdown ###")
    status_counts = defaultdict(int)
    for overlay, funcs in status_data.items():
        for func, info in funcs.items():
            status = info.get('status', 'unknown')
            status_counts[status] += 1

    for status, count in sorted(status_counts.items()):
        print(f"  {status}: {count}")

    # Per-overlay analysis
    print(f"\n### Per-Overlay Status ###")
    print(f"{'Overlay':<12} {'Source':<8} {'Status':<8} {'RELOC':<8} {'Semi':<8} {'Stub':<8}")
    print("-" * 70)

    totals = {'source': 0, 'status': 0, 'reloc_only': 0, 'semi_matched': 0, 'count_mismatch': 0}

    for overlay in sorted(set(list(source_files.keys()) + list(status_data.keys()))):
        src_count = len(source_files.get(overlay, set()))
        stat_count = len(status_data.get(overlay, {}))

        reloc = sum(1 for f, i in status_data.get(overlay, {}).items() if i.get('status') == 'reloc_only')
        semi = sum(1 for f, i in status_data.get(overlay, {}).items() if i.get('status') == 'semi_matched')
        stub = sum(1 for f, i in status_data.get(overlay, {}).items() if i.get('status') == 'count_mismatch')

        mismatch = "" if src_count == stat_count else " [MISMATCH]"
        print(f"{overlay:<12} {src_count:<8} {stat_count:<8} {reloc:<8} {semi:<8} {stub:<8}{mismatch}")

        totals['source'] += src_count
        totals['status'] += stat_count
        totals['reloc_only'] += reloc
        totals['semi_matched'] += semi
        totals['count_mismatch'] += stub

    print("-" * 70)
    print(f"{'TOTAL':<12} {totals['source']:<8} {totals['status']:<8} {totals['reloc_only']:<8} {totals['semi_matched']:<8} {totals['count_mismatch']:<8}")

    # Summary
    print(f"\n" + "=" * 70)
    print("SUMMARY")
    print("=" * 70)
    print(f"Source files:        {totals['source']}")
    print(f"Status entries:      {totals['status']}")
    print(f"RELOC-ONLY:          {totals['reloc_only']} ({100*totals['reloc_only']/totals['status']:.1f}%)")
    print(f"Semi-matched:        {totals['semi_matched']} ({100*totals['semi_matched']/totals['status']:.1f}%)")
    print(f"Count-mismatch:      {totals['count_mismatch']} ({100*totals['count_mismatch']/totals['status']:.1f}%)")

    if in_source_not_status or in_status_not_source:
        print(f"\n[WARNING] Source/status mismatch detected!")
        return 1

    return 0

if __name__ == "__main__":
    exit(audit_overlays())
