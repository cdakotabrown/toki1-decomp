#!/usr/bin/env python3
"""Record adjudicated overlay function names as reviewed, pending T4 records.

Input: one or more decision files, each a JSON list of
    {"id": "OV:0x8013XXXX", "decision": "LAND"|"DEFER", "name": "...",
     "basis": "...", "agreement": "...", "evidence": "..."}
(the adjudicator output of the blind-review workflow, docs/reference/NAMING-TOOLS.md).

A row may carry `"internal_of": "<window>"` for a function the C defines
inside another .sym window (merged-symbol window); the record keeps it.

Each LAND row becomes a `config/symbol_names.json` record with
`reviewed:consistent` evidence, tier T4 / confidence DERIVED, `landed: false`,
which `tools/overlay/apply_overlay_symbols.py --min-tier DERIVED` lands.

Skipped (reported, never recorded):
  * DEFER rows and rows for other scopes;
  * ids that already have a symbol_names record (landed or pending);
  * duplicate names within the batch;
  * functions whose source carries a `custom_script` @PostProc: those scripts
    look the function up by its `func_` symbol and the lander does not rewrite
    them, so a rename breaks the PostProc chain.

usage: record_decided.py OV DECIDED.json [DECIDED2.json ...] [--by LABEL] [--dry-run]
"""
import argparse
import glob
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'analysis'))
import symbol_names as SN  # noqa: E402


def queue_sources():
    """id -> repo-relative source path, from the proposal queue shards."""
    out = {}
    for f in glob.glob(os.path.join(ROOT, 'config/symbol_proposals/*/[0-9]*.json')):
        for e in json.load(open(f))['entries']:
            out[e['id']] = (e.get('source'), e.get('size'))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('ov')
    ap.add_argument('decided', nargs='+')
    ap.add_argument('--by', default='blind-review', help='provenance label written into the record')
    ap.add_argument('--dry-run', action='store_true')
    args = ap.parse_args()
    os.chdir(ROOT)

    if not glob.glob(os.path.join(ROOT, 'config/symbol_proposals/*/[0-9]*.json')):
        print('STOP: no proposal queue in config/symbol_proposals (it is not '
              'published with the repository); build it first with '
              'python3 tools/analysis/symbol_proposals.py inventory')
        return 2
    src = queue_sources()
    doc = SN.load_names(all_scopes=True)
    syms = doc['symbols']
    added, skipped, seen = 0, [], set()
    for path in args.decided:
        for r in json.load(open(path)):
            if r.get('decision') != 'LAND' or not r.get('name'):
                continue
            if not r['id'].startswith(args.ov + ':'):
                continue
            k = SN.norm_key(r['id'])
            if k in syms:
                skipped.append((k, 'has record: ' + syms[k]['name']))
                continue
            s, size = src.get(r['id'], (None, None))
            if s and os.path.exists(s) and re.search(
                    r'@PostProc:\s*custom_script', open(s, errors='replace').read()):
                skipped.append((k, 'custom_script keyed on the address name'))
                continue
            if r['name'] in seen:
                skipped.append((k, 'duplicate name ' + r['name']))
                continue
            seen.add(r['name'])
            ev = (f"reviewed:consistent ({SN.TODAY}, {args.by}): {r.get('evidence', '')} "
                  f"[basis: {r.get('basis')}; agreement: {r.get('agreement')}]")
            syms[k] = {'name': r['name'], 'kind': 'func', 'scope': args.ov, 'size': size,
                       'tier': 'T4', 'style': 'derived', 'confidence': 'DERIVED',
                       'evidence': ev, 'proposed_by': args.by, 'date': SN.TODAY, 'landed': False}
            if r.get('internal_of'):
                # a function the C defines inside another window (no .sym
                # entry): apply_overlay_symbols checks the window and the C
                syms[k]['internal_of'] = r['internal_of']
            added += 1
    print(f'{args.ov}: {added} records added, {len(skipped)} skipped')
    for k, why in skipped:
        print('  SKIP', k, why)
    if not args.dry_run:
        SN.save_names(doc)
    return 0


if __name__ == '__main__':
    sys.exit(main())
