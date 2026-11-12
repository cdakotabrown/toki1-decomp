#!/usr/bin/env python3
"""Generate a JSON candidate list of MISMATCH overlay functions.

Scans all overlays, tests each function, and produces a prioritized
JSON list for the matching loop to consume without re-scanning.

Usage:
    python3 tools/overlay/generate_mismatch_candidates.py
    python3 tools/overlay/generate_mismatch_candidates.py --overlay GEKO
    python3 tools/overlay/generate_mismatch_candidates.py --refresh
"""

import json
import sys
import time
from pathlib import Path

ROOT = Path(__file__).parent.parent.parent
SRC_DIR = ROOT / "src" / "overlay"
OUTPUT_FILE = ROOT / "data" / "mismatch_candidates.json"

# Import test function
sys.path.insert(0, str(ROOT / "tools" / "overlay"))
from test_overlay_match import test_overlay_function, parse_sym_file


def compute_priority(result: dict) -> int:
    """Compute priority score (higher = easier win).

    Scoring:
        100 — BYTE-PERFECT (shouldn't appear, but just in case)
         90 — RELOC-ONLY (verify with --fix-relocs)
         70 — 90%+ exact, ≤5 mismatches (quick PostProc wins)
         50 — 50-89% exact, ≤10 mismatches (moderate effort)
         30 — 20-49% exact (rework needed)
         10 — <20% exact (hard, skip unless bored)
          0 — 0 insns compiled (stub / structurally different)
    """
    if result.get("error"):
        return -1
    if result.get("matched"):
        return 100
    if result.get("reloc_only"):
        return 90
    if result.get("asm_only"):
        return -1

    exact_pct = result.get("exact_pct", 0)
    mismatches = result.get("mismatches", 999)
    n_compiled = result.get("n_compiled", 0)
    n_target = result.get("n_target", 0)

    if n_compiled == 0 or n_target == 0:
        return 0

    if exact_pct >= 90 and mismatches <= 5:
        return 70
    if exact_pct >= 50 and mismatches <= 10:
        return 50
    if exact_pct >= 20:
        return 30
    return 10


def detect_hints(result: dict) -> list[str]:
    """Detect likely mismatch categories from result data."""
    hints = []
    exact_pct = result.get("exact_pct", 0)
    reloc_pct = result.get("reloc_pct", 0)
    mismatches = result.get("mismatches", 0)
    n_target = result.get("n_target", 0)

    # Category 1: scheduling (high masked, low exact)
    if reloc_pct > 90 and exact_pct < 50:
        hints.append("scheduler_normalizer")

    # Category 2: lw $ra in delay slots (heuristic — need verbose to confirm)
    if mismatches <= 10 and exact_pct > 20:
        hints.append("possible_lw_ra_delay_slots")

    # Size mismatch → likely needs insert_instruction or patch_stack
    n_compiled = result.get("n_compiled", 0)
    if n_compiled < n_target:
        hints.append("missing_instructions")
    elif n_compiled > n_target:
        hints.append("extra_instructions")

    return hints


def scan_overlay(overlay_name: str, skip_reloc_only: bool = False) -> list[dict]:
    """Scan one overlay and return list of mismatch candidates."""
    funcs = parse_sym_file(overlay_name)
    candidates = []

    for func_name in funcs:
        src_file = SRC_DIR / overlay_name / f"{func_name}.c"
        if not src_file.exists():
            continue

        # Quick check — read file to skip ASM-ONLY early
        with open(src_file) as f:
            content = f.read()
        if "Status: ASM-ONLY" in content or "@AsmOnly" in content:
            continue

        result = test_overlay_function(overlay_name, func_name, verbose=False)
        if result.get("error") or result.get("matched") or result.get("asm_only"):
            continue

        is_reloc_only = result.get("reloc_only", False)

        # Skip RELOC-ONLY in fast mode (they're already effectively matched)
        if skip_reloc_only and is_reloc_only:
            continue

        priority = compute_priority(result)
        if priority < 0:
            continue

        entry = {
            "overlay": overlay_name,
            "func": func_name,
            "address": hex(funcs[func_name][0]),
            "status": "RELOC-ONLY" if is_reloc_only else "MISMATCH",
            "exact_pct": round(result.get("exact_pct", 0), 1),
            "masked_pct": round(result.get("reloc_pct", 0), 1),
            "mismatches": result.get("mismatches", 0),
            "n_target": result.get("n_target", 0),
            "n_compiled": result.get("n_compiled", 0),
            "priority": priority,
            "fix_relocs_bp": None,  # verified on-the-fly by the matching loop
            "hints": detect_hints(result),
        }
        candidates.append(entry)

    return candidates


def main():
    import argparse
    parser = argparse.ArgumentParser(description="Generate mismatch candidate JSON")
    parser.add_argument("--overlay", help="Only scan this overlay")
    parser.add_argument("--refresh", action="store_true",
                       help="Force re-scan even if JSON exists and is fresh")
    parser.add_argument("--min-priority", type=int, default=0,
                       help="Minimum priority score to include")
    parser.add_argument("--fast", action="store_true",
                       help="Skip RELOC-ONLY functions (only collect MISMATCH)")
    parser.add_argument("--output", default=str(OUTPUT_FILE),
                       help="Output JSON path")
    args = parser.parse_args()

    # Ensure data dir exists
    Path(args.output).parent.mkdir(parents=True, exist_ok=True)

    # Check freshness (skip if <1 hour old)
    out_path = Path(args.output)
    if not args.refresh and out_path.exists():
        age = time.time() - out_path.stat().st_mtime
        if age < 3600:
            with open(out_path) as f:
                data = json.load(f)
            print(f"Candidate list is {age/60:.0f} min old ({len(data['candidates'])} candidates). "
                  f"Use --refresh to force re-scan.")
            return

    # Discover overlays
    if args.overlay:
        overlays = [args.overlay.upper()]
    else:
        overlays = sorted(
            d.name for d in SRC_DIR.iterdir()
            if d.is_dir() and (ROOT / "config" / "overlay" / f"{d.name}.sym").exists()
        )

    print(f"Scanning {len(overlays)} overlays...")
    all_candidates = []
    for overlay in overlays:
        print(f"  Scanning {overlay}...", end=" ", flush=True)
        cands = scan_overlay(overlay, skip_reloc_only=args.fast)
        n_mismatch = sum(1 for c in cands if c["status"] == "MISMATCH")
        n_reloc = sum(1 for c in cands if c["status"] == "RELOC-ONLY")
        print(f"{n_mismatch} MISMATCH, {n_reloc} RELOC-ONLY")
        all_candidates.extend(cands)

    # Sort by priority desc, then by mismatches asc, then by exact_pct desc
    all_candidates.sort(key=lambda c: (-c["priority"], c["mismatches"], -c["exact_pct"]))

    # Filter by min priority
    all_candidates = [c for c in all_candidates if c["priority"] >= args.min_priority]

    data = {
        "generated": time.strftime("%Y-%m-%dT%H:%M:%S"),
        "total_candidates": len(all_candidates),
        "by_priority": {
            "90_reloc_only_verify": sum(1 for c in all_candidates if c["priority"] == 90),
            "70_quick_postproc": sum(1 for c in all_candidates if c["priority"] == 70),
            "50_moderate": sum(1 for c in all_candidates if c["priority"] == 50),
            "30_rework": sum(1 for c in all_candidates if c["priority"] == 30),
            "10_hard": sum(1 for c in all_candidates if c["priority"] == 10),
            "0_stub": sum(1 for c in all_candidates if c["priority"] == 0),
        },
        "candidates": all_candidates,
    }

    with open(args.output, "w") as f:
        json.dump(data, f, indent=2)

    print(f"\nWrote {len(all_candidates)} candidates to {args.output}")
    print(f"Priority breakdown: {data['by_priority']}")


if __name__ == "__main__":
    main()
