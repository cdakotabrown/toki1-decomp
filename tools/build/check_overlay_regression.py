#!/usr/bin/env python3
"""Detect overlay match regressions vs the committed baseline.

Reads the live JSON status file produced by `verify_overlay_bytes.py
--all` (config/overlay_match_status.json) and compares against the
committed baseline (config/overlay_baseline_counts.json).

Fails CI when:
  - Total reloc_only count drops below baseline.
  - A function that was reloc_only in the baseline no longer is
    (i.e. it regressed to semi_matched / count_mismatch / error).

Skips the live verify-overlay-bytes invocation; the workflow runs
that step separately. This script is purely the comparator.

NOTE: requires verify_overlay_bytes.py to have been run first to
refresh the live status file.
"""
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BASELINE = ROOT / 'config' / 'overlay_baseline_counts.json'
LIVE = ROOT / 'config' / 'overlay_match_status.json'


def main():
    if not BASELINE.exists():
        print(f"ERROR: baseline file missing: {BASELINE}", file=sys.stderr)
        return 1
    if not LIVE.exists():
        print(f"ERROR: live status file missing: {LIVE}", file=sys.stderr)
        print("Run: python3 tools/overlay/verify_overlay_bytes.py --all",
              file=sys.stderr)
        return 1

    with open(BASELINE) as f:
        base = json.load(f)
    with open(LIVE) as f:
        live = json.load(f)

    base_funcs = set(base.get('reloc_only_funcs', []))
    live_funcs = set()
    live_counts = {'reloc_only': 0, 'semi_matched': 0, 'count_mismatch': 0}
    for ov, fns in live.items():
        if not isinstance(fns, dict):
            continue
        for fn, info in fns.items():
            if not isinstance(info, dict):
                continue
            status = info.get('status')
            if status in live_counts:
                live_counts[status] += 1
            if status == 'reloc_only':
                live_funcs.add(f"{ov}/{fn}")

    base_count = base.get('reloc_only', 0)
    live_count = live_counts['reloc_only']

    regressed = sorted(base_funcs - live_funcs)
    new_wins = sorted(live_funcs - base_funcs)

    print(f"Baseline reloc_only: {base_count}")
    print(f"Live     reloc_only: {live_count}")
    print(f"Delta:               {live_count - base_count:+d}")
    print(f"New wins:            {len(new_wins)}")
    print(f"Regressions:         {len(regressed)}")

    if new_wins:
        print("\nNew RELOC-ONLY functions (not in baseline):")
        for fn in new_wins[:20]:
            print(f"  + {fn}")
        if len(new_wins) > 20:
            print(f"  ... +{len(new_wins) - 20} more")

    if regressed:
        print("\nREGRESSED functions (were RELOC-ONLY in baseline, no longer):")
        for fn in regressed:
            ov, name = fn.split('/', 1)
            current = live.get(ov, {}).get(name, {}).get('status', 'missing')
            print(f"  - {fn} (now: {current})")

    if regressed:
        print("\nFAIL: Regressions detected. If intentional, refresh baseline:",
              file=sys.stderr)
        print("  python3 tools/build/refresh_overlay_baseline.py", file=sys.stderr)
        return 1

    if live_count < base_count:
        # No specific function regressed but total dropped (e.g. errors
        # increased). Still treated as a soft fail.
        print(f"\nFAIL: total reloc_only dropped from {base_count} to {live_count}",
              file=sys.stderr)
        return 1

    print("\nPASS: no regressions.")
    return 0


if __name__ == '__main__':
    sys.exit(main())
