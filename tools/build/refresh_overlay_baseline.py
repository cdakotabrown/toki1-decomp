#!/usr/bin/env python3
"""Refresh config/overlay_baseline_counts.json from the live status JSON.

Run this after intentionally adding new RELOC-ONLY matches to record
them as the new floor for CI regression detection.
"""
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
LIVE = ROOT / 'config' / 'overlay_match_status.json'
BASELINE = ROOT / 'config' / 'overlay_baseline_counts.json'


def main():
    if not LIVE.exists():
        print(f"ERROR: live status file missing: {LIVE}", file=sys.stderr)
        print("Run: python3 tools/overlay/verify_overlay_bytes.py --all",
              file=sys.stderr)
        return 1

    with open(LIVE) as f:
        live = json.load(f)

    counts = {'total': 0, 'reloc_only': 0, 'semi_matched': 0,
              'count_mismatch': 0, 'reloc_only_funcs': []}
    for ov, fns in live.items():
        if not isinstance(fns, dict):
            continue
        for fn, info in fns.items():
            if not isinstance(info, dict):
                continue
            counts['total'] += 1
            s = info.get('status')
            if s == 'reloc_only':
                counts['reloc_only'] += 1
                counts['reloc_only_funcs'].append(f"{ov}/{fn}")
            elif s == 'semi_matched':
                counts['semi_matched'] += 1
            elif s == 'count_mismatch':
                counts['count_mismatch'] += 1
    counts['reloc_only_funcs'].sort()

    BASELINE.write_text(json.dumps(counts, indent=2) + '\n')
    print(f"Updated baseline: reloc_only={counts['reloc_only']} "
          f"semi={counts['semi_matched']} cm={counts['count_mismatch']}")
    return 0


if __name__ == '__main__':
    sys.exit(main())
