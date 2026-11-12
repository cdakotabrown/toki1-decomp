#!/usr/bin/env python3
"""Apply unfold_dead_or_move (Tier A4) PostProc to scanner-identified
candidates.

For each function with at least one A4 site found by
scan_a2_a4_candidates.py:
  1. Read the .c file and look for an existing block comment header.
  2. Insert one or more `@PostProc: unfold_dead_or_move ...` directives,
     one per A4 site (function-relative offsets, after the implicit 0x10
     thunk-pad translation done by overlay_postproc.py).
  3. Run test_overlay_match.py to see if the candidate is now RELOC-ONLY.
  4. If yes → commit. If no → revert the C edit.

Usage:
    python3 tools/overlay/apply_a4_candidates.py /tmp/a2a4_scan_full.json
    python3 tools/overlay/apply_a4_candidates.py /tmp/scan.json --dry-run
    python3 tools/overlay/apply_a4_candidates.py /tmp/scan.json --max-sites 1
"""
from __future__ import annotations

import argparse
import json
import re
import shutil
import subprocess
import sys
from pathlib import Path
from typing import Dict, List, Tuple

ROOT = Path(__file__).parent.parent.parent
SRC_OVERLAY_DIR = ROOT / "src" / "overlay"

REG_NAMES = [
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra",
]

# Overlay loads at 0x80132000. test_overlay_match.py auto-prepends a 0x10
# thunk pad to compiled bytes; @PostProc offsets in the C header are
# .text-relative AFTER the pad. The scanner emitted target-relative
# offsets (which == .text-relative for un-thunked functions). overlay_postproc
# adds func_text_offset internally (typically 0x10 for thunked, 0 otherwise),
# but A4's wiring already adds fto. So pass the SCANNER offset minus fto.
#
# Wait — actually the scanner compares target_bytes vs compiled_bytes which
# both start at the function entry. Scanner offsets are FUNCTION-relative
# (== .text-relative once fto translation happens in overlay_postproc).
# So we pass scanner offsets directly.

_HEADER_RE = re.compile(r"\A(/\*(?:[^*]|\*(?!/))*\*/)", re.MULTILINE)


def inject_postproc_lines(c_text: str, lines: List[str]) -> str:
    """Insert one or more `@PostProc:` lines into the file's leading block
    comment. Creates one if none exists."""
    annot = "".join(f" * @PostProc: {ln}\n" for ln in lines)
    m = _HEADER_RE.match(c_text)
    if m:
        header = m.group(1)
        close_idx = header.rfind("*/")
        new_header = header[:close_idx].rstrip() + "\n" + annot + " */"
        return new_header + c_text[m.end():]
    return f"/*\n{annot} */\n" + c_text


def directive_for_site(site: Dict) -> str:
    off = site["offset"]
    rs = REG_NAMES[site["rs"]]
    rd = REG_NAMES[site["rd"]]
    return f"unfold_dead_or_move --offset 0x{off:x} --rs {rs} --rd {rd}"


def run_match(ovl: str, fn: str) -> Tuple[bool, bool, float]:
    """Returns (matched, reloc_only, reloc_pct)."""
    res = subprocess.run(
        [sys.executable,
         str(ROOT / "tools" / "overlay" / "test_overlay_match.py"),
         ovl, fn],
        capture_output=True, text=True, cwd=str(ROOT), timeout=120,
    )
    out = res.stdout + res.stderr
    matched = "BYTE-PERFECT MATCH" in out
    reloc_only = "RELOCATION-ONLY DIFFS" in out
    m = re.search(r"Masked:\s+\d+/\d+\s+\((\d+(?:\.\d+)?)%\)", out)
    pct = float(m.group(1)) if m else 0.0
    return matched, reloc_only, pct


def try_one(ovl: str, fn: str, sites: List[Dict], dry_run: bool, verbose: bool) -> Dict:
    c_file = SRC_OVERLAY_DIR / ovl / f"{fn}.c"
    if not c_file.exists():
        return {"overlay": ovl, "function": fn, "status": "no-c-file"}

    original = c_file.read_text()

    # Skip if already has unfold_dead_or_move
    if "unfold_dead_or_move" in original:
        return {"overlay": ovl, "function": fn, "status": "already-annotated"}

    # Baseline reloc_pct
    base_matched, base_reloc, base_pct = run_match(ovl, fn)
    if base_reloc or base_matched:
        return {"overlay": ovl, "function": fn, "status": "already-passing", "pct": base_pct}

    directives = [directive_for_site(s) for s in sites]
    new_text = inject_postproc_lines(original, directives)

    if dry_run:
        return {
            "overlay": ovl, "function": fn,
            "status": "dry-run",
            "base_pct": base_pct,
            "directives": directives,
        }

    c_file.write_text(new_text)
    matched, reloc, new_pct = run_match(ovl, fn)

    if matched or reloc:
        # Commit win
        msg = f"overlay: {ovl}/{fn} RELOC-ONLY (A4 unfold_dead_or_move)"
        subprocess.run(
            ["git", "add", str(c_file.relative_to(ROOT))],
            cwd=str(ROOT), check=True,
        )
        subprocess.run(
            ["git", "commit", "-m", msg],
            cwd=str(ROOT), check=True, capture_output=True,
        )
        return {"overlay": ovl, "function": fn, "status": "win",
                "base_pct": base_pct, "new_pct": new_pct}

    # Revert
    c_file.write_text(original)
    return {"overlay": ovl, "function": fn, "status": "no-improvement",
            "base_pct": base_pct, "new_pct": new_pct}


def main() -> int:
    p = argparse.ArgumentParser()
    p.add_argument("scan_json", help="output of scan_a2_a4_candidates.py --json")
    p.add_argument(
        "--max-sites", type=int, default=1,
        help="only attempt candidates with at most N A4 sites (default 1; "
             "multi-site has lower hit rate)",
    )
    p.add_argument(
        "--min-pct", type=float, default=80.0,
        help="skip if baseline reloc%% below threshold (default 80; lower = "
             "more attempts but lower hit rate)",
    )
    p.add_argument("--limit", type=int, default=0, help="cap attempts (0=all)")
    p.add_argument("--dry-run", action="store_true")
    p.add_argument("-v", "--verbose", action="store_true")
    args = p.parse_args()

    hits = json.load(open(args.scan_json))
    candidates = [
        r for r in hits
        if r["a4"] and not r["a2"] and len(r["a4"]) <= args.max_sites
    ]
    print(f"Candidates with ≤{args.max_sites} A4 site(s): {len(candidates)}",
          file=sys.stderr)

    # Need baseline reloc% from status file too. We'll get it via run_match.
    if args.limit:
        candidates = candidates[: args.limit]

    wins = 0
    no_improve = 0
    skipped = 0
    for i, r in enumerate(candidates):
        res = try_one(r["overlay"], r["function"], r["a4"],
                      dry_run=args.dry_run, verbose=args.verbose)
        if res["status"] == "win":
            wins += 1
            print(f"[{i+1}/{len(candidates)}] WIN  {r['overlay']:<10} "
                  f"{r['function']:<22} "
                  f"({res['base_pct']:.1f}% -> {res['new_pct']:.1f}%)")
        elif res["status"] == "no-improvement":
            no_improve += 1
            if args.verbose:
                print(f"[{i+1}/{len(candidates)}] miss {r['overlay']:<10} "
                      f"{r['function']:<22} "
                      f"({res['base_pct']:.1f}% -> {res['new_pct']:.1f}%)")
        elif res["status"] == "already-passing":
            skipped += 1
            if args.verbose:
                print(f"[{i+1}/{len(candidates)}] already-passing "
                      f"{r['overlay']}/{r['function']}")
        else:
            skipped += 1
            if args.verbose:
                print(f"[{i+1}/{len(candidates)}] skip ({res['status']}) "
                      f"{r['overlay']}/{r['function']}")

    print(f"\nWins: {wins}  No-improve: {no_improve}  Skipped: {skipped}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
