#!/usr/bin/env python3
"""
audit_unmatchable.py - Audit config/c_unmatchable.txt for stale entries.

An entry is "stale" if it falls into one of these categories:

  MATCHED_C: function has matching C in src/main/matched/<name>.c.
           Genuinely stale -- the C reproduces the bytes, so the function is
           not unmatchable and the blocker entry is obsolete.

  MATCHED_ASM: function has byte-perfect asm in asm/main/matched/<name>.s.
           *** NOT STALE. REPORTED ONLY, NEVER REMOVED. ***
           This category encoded the pre-2026-08-09 policy, under which
           byte-perfect ASM meant the blocker entry was obsolete.  That is
           backwards under current doctrine: `asm_only` + a blocker category
           in c_unmatchable.txt IS the terminal state, and byte-perfect ASM in
           asm/main/matched/ is exactly what such a function HAS.  Removing
           these entries would strip the recorded reason from every
           legitimately terminal function -- 81 of them as of 2026-08-23,
           including all 24 hand-written Sony GTE routines in the
           0x800B0508 window, which have no C form at all.
           See CLAUDE.md, "asm_only ... is a legitimate finish line".

  ORPHAN:  function no longer exists anywhere on disk (src/main/, asm/main/,
           src/overlay/, asm/overlay/). Probably renamed or deleted.
           The entry is stale because the function it refers to is gone.

Usage:
  python3 tools/conversion/audit_unmatchable.py           # show plan only
  python3 tools/conversion/audit_unmatchable.py --apply   # remove stale entries
  python3 tools/conversion/audit_unmatchable.py --backup  # also create .bak.<date>

Run this periodically to keep c_unmatchable.txt in sync with the actual state
of the decomp. Stale entries waste attention and pollute tooling that
greps for unmatchable functions.
"""

import argparse
import glob
import os
import sys
from collections import Counter
from datetime import date

UNMATCHABLE_FILE = "config/c_unmatchable.txt"
BUCKETS = [
    "src/main/matched",
    "src/main/semi-matched",
    "src/main/wip",
    "src/main/stubs",
    "src/main/archive",
    "src/main/extras",
    "asm/main/matched",
]


def find_all_function_names():
    """Scan all known buckets and return the set of function names on disk."""
    names = set()
    for bucket in BUCKETS:
        if not os.path.isdir(bucket):
            continue
        for f in glob.glob(f"{bucket}/*.c") + glob.glob(f"{bucket}/*.s"):
            base = os.path.basename(f).rsplit(".", 1)[0]
            names.add(base)
    # Also scan overlay buckets
    for overlay_dir in glob.glob("src/overlay/*") + glob.glob("asm/overlay/*"):
        if not os.path.isdir(overlay_dir):
            continue
        overlay = os.path.basename(overlay_dir)
        for f in glob.glob(f"{overlay_dir}/*.c") + glob.glob(f"{overlay_dir}/*.s"):
            base = os.path.basename(f).rsplit(".", 1)[0]
            names.add(base)
            names.add(f"{overlay}/{base}")
    return names


def parse_unmatchable(path):
    """Read unmatchable.txt preserving structure.

    Returns:
        lines: list of (lineno, raw_line, entry_or_None)
               where entry_or_None is the first token of the line
               (None for blank/comment lines).
    """
    with open(path) as f:
        lines = f.readlines()
    parsed = []
    for i, line in enumerate(lines):
        stripped = line.strip()
        if not stripped or stripped.startswith("#"):
            parsed.append((i, line, None))
        else:
            entry = stripped.split()[0]
            parsed.append((i, line, entry))
    return parsed


def categorize(parsed, all_names, asm_matched, c_matched):
    """For each entry, decide if it is MATCHED, ORPHAN, or OK."""
    decisions = []
    for lineno, raw, entry in parsed:
        if entry is None:
            decisions.append((lineno, raw, entry, "COMMENT"))
            continue
        if "/" in entry:
            _, fn = entry.split("/", 1)
        else:
            fn = entry
        if fn in asm_matched:
            decisions.append((lineno, raw, entry, "MATCHED_ASM"))
        elif fn in c_matched:
            decisions.append((lineno, raw, entry, "MATCHED_C"))
        elif entry not in all_names and fn not in all_names:
            decisions.append((lineno, raw, entry, "ORPHAN"))
        else:
            decisions.append((lineno, raw, entry, "OK"))
    return decisions


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--apply", action="store_true", help="Actually remove stale entries (default: plan only)")
    parser.add_argument("--backup", action="store_true", help="Create .bak.<date> backup before applying")
    args = parser.parse_args()

    if not os.path.exists(UNMATCHABLE_FILE):
        print(f"ERROR: {UNMATCHABLE_FILE} not found")
        sys.exit(1)

    # Build the universe of names
    all_names = find_all_function_names()
    asm_matched = set()
    c_matched = set()
    for f in glob.glob("asm/main/matched/*.s"):
        asm_matched.add(os.path.basename(f).rsplit(".", 1)[0])
    for f in glob.glob("src/main/matched/*.c"):
        c_matched.add(os.path.basename(f).rsplit(".", 1)[0])

    # Parse and categorize
    parsed = parse_unmatchable(UNMATCHABLE_FILE)
    decisions = categorize(parsed, all_names, asm_matched, c_matched)

    # Summarize
    cat_counter = Counter(d[3] for d in decisions)
    print("=== Audit Plan ===")
    for cat, n in sorted(cat_counter.items(), key=lambda x: -x[1]):
        print(f"  {cat}: {n}")
    print()

    # Show what we'd remove
    # MATCHED_ASM is deliberately EXCLUDED -- see the module docstring.  An
    # asm_only function with byte-perfect ASM is terminal, not stale, and its
    # blocker category is the record of why.
    to_remove = [d for d in decisions if d[3] in ("MATCHED_C", "ORPHAN")]
    if not to_remove:
        print("Nothing to remove. c_unmatchable.txt is clean.")
        return 0

    print(f"=== Entries to remove ({len(to_remove)}) ===")
    for lineno, raw, entry, cat in to_remove:
        print(f"  L{lineno + 1:4d} [{cat:11s}] {entry}")
    print()

    if not args.apply:
        print("Dry run. Pass --apply to actually remove these entries.")
        return 0

    # Apply: filter out stale lines
    stale_linenos = {d[0] for d in to_remove}
    keep_lines = [d[1] for d in decisions if d[0] not in stale_linenos]
    new_content = "".join(keep_lines)

    # Update date stamp
    today = date.today().isoformat()
    new_content = new_content.replace("Last updated: 2026-03-28", f"Last updated: {today}")
    new_content = new_content.replace("Last updated: 2026-06-05", f"Last updated: {today}")

    # Backup if requested
    if args.backup:
        backup_path = f"{UNMATCHABLE_FILE}.bak.{today}"
        with open(UNMATCHABLE_FILE) as f:
            orig = f.read()
        with open(backup_path, "w") as f:
            f.write(orig)
        print(f"Backup written: {backup_path}")

    # Write
    with open(UNMATCHABLE_FILE, "w") as f:
        f.write(new_content)
    print(f"Wrote {UNMATCHABLE_FILE} with {len(keep_lines)} lines (was {len(parsed)}).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
