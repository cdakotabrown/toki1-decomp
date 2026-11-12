#!/usr/bin/env python3
"""Record adjudicated MAIN data names as reviewed, pending data records.

Input: decision files, each a JSON list of
    {"addr": "0x800D07FC", "decision": "LAND"|"DEFER", "name": "...",
     "type": "u8", "size": 1, "tier": "T4"|"T3"|"T2", "evidence": "..."}

Each LAND row becomes a `config/symbol_names.json` record
    {"name", "kind": "data", "scope": "main", "size", "type", "tier",
     "style": derived|obin|sony, "confidence": DERIVED|HIGH, "evidence": "...;
     reviewed:consistent (...)", "landed": false}
that `tools/analysis/apply_data_symbols.py --min-tier DERIVED` lands. A T2
(Psy-Q archive/source) or T3 (o.bin) row must carry `corroborated:` in its
evidence (the data lander refuses it otherwise); T2 records get style `sony`.

Skipped (reported): DEFER rows, addresses that already have a record, duplicate
names in the batch.

usage: record_data_decided.py DECIDED.json [...] [--by LABEL] [--dry-run]
"""
import argparse
import json
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'analysis'))
import symbol_names as SN  # noqa: E402


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('decided', nargs='+')
    ap.add_argument('--by', default='data blind-review')
    ap.add_argument('--dry-run', action='store_true')
    args = ap.parse_args()
    os.chdir(ROOT)
    doc = SN.load_names(all_scopes=True)
    syms = doc['symbols']
    added, skipped, seen = 0, [], set()
    for path in args.decided:
        for r in json.load(open(path)):
            if r.get('decision') != 'LAND' or not r.get('name'):
                continue
            k = f"0x{int(r['addr'], 16):08X}"
            if k in syms:
                skipped.append((k, 'has record: ' + syms[k]['name']))
                continue
            if r['name'] in seen:
                skipped.append((k, 'duplicate name ' + r['name']))
                continue
            tier = r.get('tier', 'T4')
            if tier in ('T2', 'T3') and 'corroborated:' not in r.get('evidence', ''):
                skipped.append((k, f'{tier} without corroborated: evidence'))
                continue
            seen.add(r['name'])
            rec = {'name': r['name'], 'kind': 'data', 'scope': 'main', 'size': r.get('size'),
                   'tier': tier, 'style': {'T2': 'sony', 'T3': 'obin'}.get(tier, 'derived'),
                   'confidence': 'HIGH' if tier in ('T2', 'T3') else 'DERIVED',
                   'evidence': f"{r.get('evidence', '')}; reviewed:consistent ({SN.TODAY}, {args.by})",
                   'proposed_by': args.by, 'date': SN.TODAY, 'landed': False}
            if r.get('type'):
                rec['type'] = r['type']
            syms[k] = rec
            added += 1
    print(f'{added} data records added, {len(skipped)} skipped')
    for k, why in skipped:
        print('  SKIP', k, why)
    if not args.dry_run:
        SN.save_names(doc)
    return 0


if __name__ == '__main__':
    sys.exit(main())
