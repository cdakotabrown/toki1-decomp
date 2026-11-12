#!/usr/bin/env python3
"""Propose MECHANICAL names for the single-use overlay `.rodata` strings.

Scope: the census class `string/single` of tools/naming/overlay_data_census.py
(an open overlay-local D_ whose EXN bytes are a NUL-terminated cp932 string and
whose every non-extern reference sits in ONE function of a live overlay).
These were string literals at the call site in the original source; until a
data-from-source build step lets them be written back as literals
(docs/SYMBOL-MAPPING-PLAN.md), they get the one mechanical name
NAMING-CONVENTIONS-1994.md section 4.2 sanctions for compiler-generated
objects:

    <user_func>_str<N>

  * <user_func> is the referencing function's current name;
  * N is the 0-based rank, glued with no underscore (the house ordinal style:
    `p_mes0`, `suuzi0`), of the string's FIRST textual use inside that
    function's body, counted over the strings this generator names in that
    function (strings already named or recorded are not counted).  N is
    always written, even for a function with one string (`_str0`).
  * When the same function name is defined in more than one live overlay, the
    overlay prefix is added in front: `<ov>_<user_func>_str<N>` (overlay data
    names must be unique across all scopes: the PC port links every overlay).

Skipped (reported, never proposed):
  * a string whose address already has a record of that overlay in
    config/symbol_names.json (landed, pending or DEFER) -- the reader pass (a)
    goes first, so (b) never touches what (a) named or deferred;
  * a user still named `func_XXXXXXXX` (the name would carry an address);
  * a candidate that is already spelled ANYWHERE in the tree as a whole word
    (git grep over every tracked and untracked file, comments and docs
    included) or held by any ledger record, even after the prefix fallback.

Output: one decided-JSON per overlay (the record_overlay_data.py row format)
in --out DIR, rows marked mechanical: tier T4, style `mechanical`,
confidence DERIVED, evidence starting `mechanical:` (generator, user function,
first-use rank, quoted text) so a later human pass can find and upgrade them.

usage: gen_string_names.py [--ov OV ...] --out DIR [--census JSON]
       gen_string_names.py --sample N [--seed S] ...   (print N random rows)
"""
import argparse
import json
import os
import random
import re
import subprocess
import sys
from collections import defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import overlay_data_census as C  # noqa: E402

ADDR_FUNC = re.compile(r'^(?:func|FUN)_[0-9A-Fa-f]{8}$')


def tree_words(names):
    """the subset of NAMES spelled as a whole word anywhere in the tree."""
    hit = set()
    names = sorted(names)
    for i in range(0, len(names), 400):
        chunk = names[i:i + 400]
        args = ['git', 'grep', '--untracked', '-I', '-h', '-o', '-w']
        for n in chunk:
            args += ['-e', n]
        out = subprocess.run(args, capture_output=True, text=True, cwd=ROOT)
        if out.returncode not in (0, 1):
            raise SystemExit(f'git grep failed: {out.stderr.strip()}')
        hit.update(out.stdout.split())
    return hit & set(names)


def defined_functions(ov):
    d = os.path.join(ROOT, 'src', 'overlay', ov)
    out = set()
    for fn in os.listdir(d):
        if fn.endswith('.c'):
            text = C.strip_c(open(os.path.join(d, fn), encoding='utf-8', errors='replace').read())
            out.update(u for _, _, u in C.scan_users(text) if u)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--ov', action='append')
    ap.add_argument('--out')
    ap.add_argument('--census', help='census JSON (default: recompute)')
    ap.add_argument('--sample', type=int)
    ap.add_argument('--seed', type=int, default=1)
    a = ap.parse_args()
    ovs = [o.upper() for o in a.ov] if a.ov else C.live_overlays()
    if a.census:
        recs = [r for r in json.load(open(a.census)) if r['ov'] in ovs]
    else:
        recs = []
        for ov in ovs:
            recs += C.census_overlay(ov)[0]
    ledger = json.load(open(os.path.join(ROOT, 'config', 'symbol_names.json')))['symbols']
    held = {e.get('name') for e in ledger.values()}
    # function names defined in more than one live overlay
    owners = defaultdict(set)
    for ov in C.live_overlays():
        for f in defined_functions(ov):
            owners[f].add(ov)
    shared = {f for f, s in owners.items() if len(s) > 1}

    skipped = []
    groups = defaultdict(list)          # (ov, func) -> [rec]
    for r in recs:
        if r['class'] != 'string' or r.get('sub') != 'single':
            continue
        key = f"{r['ov']}:{r['addr']}"
        if key in ledger:
            skipped.append((key, f"already recorded: {ledger[key].get('name')}"))
            continue
        fn = r['users'][0]
        if ADDR_FUNC.match(fn):
            skipped.append((key, f'user {fn} still address-named'))
            continue
        groups[(r['ov'], fn)].append(r)
    rows = []
    for (ov, fn), rs in sorted(groups.items()):
        rs.sort(key=lambda r: (r['first'][0], r['first'][1], r['addr']))
        stem = f'{ov.lower()}_{fn}' if fn in shared else fn
        for n, r in enumerate(rs):
            rows.append({'r': r, 'fn': fn, 'n': n, 'name': f'{stem}_str{n}',
                         'alt': f'{ov.lower()}_{fn}_str{n}', 'prefixed': fn in shared})
    # collisions: whole-word anywhere in the tree, or held by a record
    cands = {x['name'] for x in rows} | {x['alt'] for x in rows}
    spelled = tree_words(cands) | (cands & held)
    seen = set()
    out = defaultdict(list)
    for x in rows:
        r = x['r']
        key = f"{r['ov']}:{r['addr']}"
        name = x['name']
        if name in spelled or name in seen:
            if not x['prefixed'] and x['alt'] not in spelled and x['alt'] not in seen:
                name = x['alt']
            else:
                skipped.append((key, f'{x["name"]} collides (tree/ledger) and so does the prefixed form'))
                continue
        seen.add(name)
        a_ = int(r['addr'], 16)
        size = r['len'] + 1
        if a_ % 4 == 0:
            size = (size + 3) & ~3
        text = r['text'] if len(r['text']) <= 60 else r['text'][:57] + '...'
        out[r['ov']].append({
            'id': key, 'decision': 'LAND', 'name': name, 'size': size, 'type': f'u8[{size}]',
            'tier': 'T4', 'style': 'mechanical', 'confidence': 'DERIVED',
            'evidence': (f"mechanical: gen_string_names.py <user_func>_str<N>; single-use .rodata "
                         f"string, sole user {x['fn']} ({r['first'][0]}), first-use rank {x['n']} "
                         f"in that function; {r['nuses']} reference(s); text {json.dumps(text, ensure_ascii=False)}. "
                         f"Placeholder for a literal: upgrade by a human pass or the data-from-source write-back"),
        })
    total = sum(len(v) for v in out.values())
    if a.sample:
        allrows = [row for v in out.values() for row in v]
        random.Random(a.seed).shuffle(allrows)
        for row in allrows[:a.sample]:
            print(json.dumps({k: row[k] for k in ('id', 'name', 'size')} | {'ev': row['evidence'][:200]},
                             ensure_ascii=False))
    if a.out:
        os.makedirs(a.out, exist_ok=True)
        for ov, v in sorted(out.items()):
            json.dump(v, open(os.path.join(a.out, f'strings-{ov}.json'), 'w'), ensure_ascii=False, indent=1)
    for ov in sorted(out):
        print(f'{ov:10s} {len(out[ov]):5d}')
    print(f'total {total} proposed, {len(skipped)} skipped')
    for k, why in skipped:
        print('  SKIP', k, why)
    return 0


if __name__ == '__main__':
    sys.exit(main())
