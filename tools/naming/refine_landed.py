#!/usr/bin/env python3
"""History-preserving record correction for renaming an ALREADY-LANDED name.

A refinement replaces a landed mechanical name with a data-verified one (the
0x80084ED0 / 0x8008D778 merge pattern).  Renames
are done by the landers, never here: this tool only rewrites the RECORD.  The
old record is preserved verbatim in `history` (with a `superseded` marker) and
the refined name takes the top level with `landed: false`, ready for
`tools/naming/land_overlay.sh`'s forward pass.

The forward lander needs the `func_<ADDR>` spelling back in `<OV>.sym` first,
so the flow is (see tools/naming/refine_pre.sh, wired as land_overlay.sh's
PRE step):

  1. validate the rows                       refine_landed.py OV rows.json --dry-run
  2. pull the landed names back              apply_overlay_symbols.py --revert --apply
  3. correct the records (this tool)         refine_landed.py OV rows.json
  4. land the refined names + gates + commit land_overlay.sh OV rows.json (PRE=...)

Rows are the same decision shape record_decided.py consumes:
    {"id": "OV:0xADDR", "decision": "LAND", "name": "<refined name>",
     "was": "<current landed name>", "basis": ..., "agreement": ..., "evidence": ...}
`was` is optional but STRONGLY recommended: when present it must match the
record's current name exactly, so a stale file can never clobber a surprise.

Skipped (reported, never written): DEFER rows and other scopes' rows; ids with
no record; a record whose name already equals the refined name (idempotent
rerun); a refined name already held by another record of the same overlay
(norm_key collision pre-check) or used by main; duplicate refined names in the
batch.

usage: refine_landed.py OV DECIDED.json [DECIDED2.json ...] [--by LABEL] [--dry-run]
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
    ap.add_argument('ov')
    ap.add_argument('decided', nargs='+')
    ap.add_argument('--by', default='refine-20261017', help='provenance label written into the record')
    ap.add_argument('--dry-run', action='store_true')
    args = ap.parse_args()
    os.chdir(ROOT)

    doc = SN.load_names(all_scopes=True)
    syms = doc['symbols']
    scope_names = {e['name'] for k, e in syms.items()
                   if SN.is_overlay_key(k) and SN.split_key(k)[0] == args.ov.upper()}
    try:
        main_names = {f['name'] for f in SN.load_registry()['functions']}
    except OSError:
        main_names = set()

    changed, skipped, seen = 0, [], set()
    for path in args.decided:
        for r in json.load(open(path)):
            if r.get('decision') != 'LAND' or not r.get('name'):
                continue
            if not r['id'].startswith(args.ov + ':'):
                continue
            k = SN.norm_key(r['id'])
            if k not in syms:
                skipped.append((k, 'no record'))
                continue
            cur = syms[k]
            new = r['name']
            if cur['name'] == new:
                skipped.append((k, 'already refined to ' + new))
                continue
            if r.get('was') and cur['name'] != r['was']:
                skipped.append((k, f"was mismatch: record holds {cur['name']}, row says {r['was']}"))
                continue
            # evidence convention: "REFINEMENT of <old name>" must name the
            # function the record currently holds (safety net for rows without `was`)
            ev_text = r.get('evidence', '')
            if 'REFINEMENT of ' in ev_text:
                named = ev_text.split('REFINEMENT of ', 1)[1].split(None, 1)[0].rstrip(':;,')
                if named != cur['name']:
                    skipped.append((k, f'refinement evidence names {named}, record holds {cur["name"]}'))
                    continue
            if new in seen or new in scope_names or new in main_names:
                skipped.append((k, 'duplicate/colliding name ' + new))
                continue
            seen.add(new)
            scope_names.discard(cur['name'])
            scope_names.add(new)
            old = dict(cur)
            old['superseded'] = {
                'date': SN.TODAY,
                'from': cur['name'],
                'to': new,
                'by': 'refine_landed.py',
                'reason': 'data-verified refinement (record-level correction; the lander renamed the files)',
            }
            ev = (f"reviewed:consistent ({SN.TODAY}, {args.by}): REFINEMENT of {cur['name']}"
                  f"{(' (landed ' + str(cur.get('date')) + ')') if cur.get('date') else ''}: "
                  f"{ev_text} [basis: {r.get('basis')}; agreement: {r.get('agreement')}]")
            syms[k] = {
                'name': new,
                'kind': cur.get('kind', 'func'),
                'scope': args.ov,
                'size': cur.get('size'),
                'tier': 'T4',
                'style': 'derived',
                'confidence': 'DERIVED',
                'evidence': ev,
                'proposed_by': args.by,
                'date': SN.TODAY,
                'landed': False,
                'history': cur.get('history', []) + [old],
            }
            changed += 1
    print(f'{args.ov}: {changed} records refined, {len(skipped)} skipped')
    for k, why in skipped:
        print('  SKIP', k, why)
    if not args.dry_run:
        SN.save_names(doc)
    return 0


if __name__ == '__main__':
    sys.exit(main())
