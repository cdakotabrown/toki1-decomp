#!/usr/bin/env python3
"""Rename an ALREADY-LANDED overlay or main data name (a correction).

A main record (key `0xADDR`, no scope) is renamed in every tracked file.

The record keeps its address; the old record is preserved in `history` and
the evidence says why.  The tree is rewritten whole-word, scoped to the
overlay (every overlay loads at 0x80132000, so the name means nothing
elsewhere) plus the PC-port places that carry overlay data names:

  src/overlay/<OV>/**, pc/src/overlay/<OV>/**,
  pc/src/overlay_inits/overlay_init_<OV>.c, pc/tools/*_plan.json,
  pc/ld/psx_globals.syms

pc/src/overlay_<OV>.c is generated; the lander regenerates it.  Run as a PRE
step of tools/naming/land_overlay_data.sh (PRE-only landing), which gates
every touched function.

rows: JSON list of {"id": "OV:0xADDR", "was": "<old>", "name": "<new>",
                    "why": "..."}
      main-scope Sony respell: also "tier": "T2" with "corroborated:" in why
      (then Sony's own spelling is accepted), and optional "allow_hits":
      [files whose existing mentions of the new name were read by hand]
usage: rename_landed_data.py ROWS.json (--dry-run|--apply) [--by LABEL]
"""
import argparse
import datetime
import glob
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'analysis'))
import symbol_names as SN  # noqa: E402


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('rows')
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument('--dry-run', action='store_true')
    g.add_argument('--apply', action='store_true')
    ap.add_argument('--by', default='rename_landed_data')
    a = ap.parse_args()
    os.chdir(ROOT)
    doc = SN.load_names(all_scopes=True)
    syms = doc['symbols']
    used = {v.get('name') for v in syms.values()}
    today = datetime.date.today().isoformat()
    bad = 0
    writes = []
    for r in json.load(open(a.rows)):
        key, old, new = r['id'], r['was'], r['name']
        ov = key.split(':')[0] if ':' in key else None
        rec = syms.get(key)
        if not rec or rec.get('name') != old or not rec.get('landed') or rec.get('kind') != 'data':
            print('REFUSE %s: record is not the landed data name %s' % (key, old))
            bad += 1
            continue
        # Sony's own spelling (GsLSMATRIX, _snd_1per2) is allowed for a MAIN
        # record when the row is T2 with archive corroboration (2026-11-11);
        # everything else stays Konami-style lower snake case
        sony = (not ov and r.get('tier') == 'T2' and 'corroborated:' in r.get('why', ''))
        ident = r'[A-Za-z_][A-Za-z0-9_]*' if sony else r'[a-z][a-z0-9_]*'
        # allow_hits: files already read by hand whose existing mentions of
        # NEW are comments or the same symbol's declaration (pc libetc.h)
        clash = [h for h in subprocess.run(['git', 'grep', '-lw', new, '--', '*.c', '*.h', '*.s'],
                                           capture_output=True, text=True).stdout.split()
                 if h not in r.get('allow_hits', [])]
        if not re.fullmatch(ident, new) or new in used or clash:
            print('REFUSE %s: %s is not a free identifier%s' % (
                key, new, (' (in use: %s)' % ' '.join(clash[:5])) if clash else ''))
            bad += 1
            continue
        if ov:
            files = ['src/overlay/%s' % ov, 'pc/src/overlay/%s' % ov, 'pc/src/overlay_inits/overlay_init_%s.c' % ov,
                     'pc/ld/psx_globals.syms'] + sorted(glob.glob('pc/tools/*_plan.json'))
        else:
            # a MAIN data name is global: every tracked source, header, PC
            # file (Makefile swaps, link map, plans) and patch reference
            # (2026-11-10)
            # (2026-11-10); blocker_database quotes data names in its prose
            # (2026-11-11)
            files = ['src', 'include', 'pc', 'asm', 'config/patch', 'tools/patch',
                     'config/blocker_database.json']
        hits = subprocess.run(['git', 'grep', '-lw', old, '--'] + [f for f in files if os.path.exists(f)],
                              capture_output=True, text=True).stdout.split()
        if ov:
            hits = [h for h in hits if not h.startswith('pc/src/overlay_%s.c' % ov)]
        print('%s %s: %s -> %s in %d files' % ('APPLY' if a.apply else 'DRY', key, old, new, len(hits)))
        if not a.apply:
            continue
        # every row is validated before any file is written: a refusal after
        # a partial rewrite left a half-renamed tree (2026-11-11)
        writes.append((hits, old, new))
        hist = rec.setdefault('history', [])
        hist.append({k: v for k, v in rec.items() if k != 'history'})
        rec['name'] = new
        if sony:
            rec.update(tier='T2', style='sony', confidence='HIGH')
        rec['evidence'] = rec.get('evidence', '') + '; renamed from %s (%s, %s): %s' % (old, today, a.by, r.get('why', ''))
        used.add(new)
    if a.apply and not bad:
        for hits, old, new in writes:
            for h in hits:
                t = open(h, errors='surrogateescape').read()
                open(h, 'w', errors='surrogateescape').write(re.sub(r'\b%s\b' % re.escape(old), new, t))
        SN.save_names(doc)
        # config/main_retail_symbols.json is generated from the ledger and
        # the names gate checks it (gen-retail --check); a main rename must
        # regenerate it (2026-11-11)
        if any(':' not in r['id'] for r in json.load(open(a.rows))):
            subprocess.run([sys.executable, 'tools/analysis/symbol_names.py', 'gen-retail'], check=True)
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
