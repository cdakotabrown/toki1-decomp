#!/usr/bin/env python3
"""Rename mis-addressed D_XXXXXXXX / func_XXXXXXXX symbols in main-binary C.

Consumes the JSON from audit_main_relocs.py. For each MISMATCH entry it pairs
every `missing_from_target` address X with an `extra_in_target` address Y
when the pairing is UNAMBIGUOUS:
  - exactly one Y shares X's %hi16 (or lies within +-0x10000) and that Y is
    not claimed by another X,
  - the .c names X literally (D_X or func_X) and does NOT already name Y.
It rewrites D_X -> D_Y (func_X -> func_Y) in the .c, re-runs the reloc audit
on that entry, and keeps the edit only if the entry becomes OK/UNRESOLVED
(reverting otherwise). With --dry-run it only prints the plan.

Everything else (ambiguous pairs, named externs, scanner gaps) is printed
for hand review.
"""
import argparse, collections, json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
AUDIT = os.path.join(ROOT, 'tools/analysis/audit_main_relocs.py')


def plan(row):
    m = [int(x, 16) for x in row['missing_from_target']]
    e = [int(x, 16) for x in row['extra_in_target']]
    mc, ec = collections.Counter(m), collections.Counter(e)
    pairs = {}
    for x in mc:
        cands = [y for y in ec if (y >> 16) == (x >> 16)]
        if len(cands) != 1:
            cands = [y for y in ec if abs(y - x) < 0x10000]
            if len(cands) != 1:
                return None, f'ambiguous for {x:#x}: {[hex(c) for c in cands]}'
        y = cands[0]
        if y in pairs.values():
            return None, f'{y:#x} claimed twice'
        pairs[x] = y
    return pairs, None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('json')
    ap.add_argument('--dry-run', action='store_true')
    ap.add_argument('--only', nargs='*')
    args = ap.parse_args()
    funcs = {f['name']: f for f in json.load(open(os.path.join(ROOT, 'config/level3_functions.json')))['functions']}
    rows = [r for r in json.load(open(args.json)) if r['verdict'] == 'MISMATCH']
    fixed, skipped = [], []
    for r in rows:
        if args.only and r['name'] not in args.only:
            continue
        src = os.path.join(ROOT, funcs[r['name']]['source'])
        text = open(src).read()
        pairs, why = plan(r)
        if pairs is None and 'c_order' in r and len(r['c_order']) == len(r['t_order']):
            # positional pairing: same count of references, same order (no reordering PostProc)
            pos = {}
            ok = True
            for cx, tx, ck, tk in zip(r['c_order'], r['t_order'], r['c_kind'], r['t_kind']):
                cx, tx = int(cx, 16), int(tx, 16)
                if ck != tk:
                    ok = False; break      # order differs between object relocs and target scan
                if cx == tx:
                    continue
                if abs(cx - tx) >= 0x10000 and (cx >> 20) != (tx >> 20):
                    ok = False; break      # not a plausible typo of the same object
                if pos.get(cx, tx) != tx:
                    ok = False; break
                pos[cx] = tx
            if ok and pos and len(set(pos.values())) == len(pos):
                pairs, why = pos, None
        if pairs is None:
            # sorted-delta pairing: same number of missing/extra and ONE constant delta
            m = sorted(int(x, 16) for x in r['missing_from_target'])
            e = sorted(int(x, 16) for x in r['extra_in_target'])
            e = [y for y in e if y >= 0x80010000]   # drop lui/ori constant noise
            if m and len(m) == len(e):
                deltas = {y - x for x, y in zip(m, e)}
                if len(deltas) == 1 and abs(deltas.pop()) < 0x100000:
                    pairs, why = dict(zip(m, e)), None
        if pairs is None:
            skipped.append((r['name'], why)); continue
        renames = {}
        for x, y in pairs.items():
            hit = None
            for pfx in ('D_', 'func_'):
                for nm in (f'{pfx}{x:08X}', f'{pfx}{x:08x}'):
                    if re.search(r'\b' + nm + r'\b', text):
                        hit = (nm, f'{pfx}{y:08X}')
            if hit is None:
                skipped.append((r['name'], f'{x:#x} not named literally (named extern?)')); renames = None; break
            if re.search(r'\b' + hit[1] + r'\b', text):
                skipped.append((r['name'], f'{hit[1]} already present')); renames = None; break
            renames[hit[0]] = hit[1]
        if not renames:
            continue
        print(f"{r['name']:26s} " + ', '.join(f'{a}->{b}' for a, b in renames.items()))
        if args.dry_run:
            continue
        new = text
        for a, b in renames.items():
            new = re.sub(r'\b' + a + r'\b', b, new)
        open(src, 'w').write(new)
        res = subprocess.run([sys.executable, AUDIT, '--only', r['name']], capture_output=True, text=True, cwd=ROOT).stdout
        if "'MISMATCH'" in res:
            open(src, 'w').write(text)
            skipped.append((r['name'], 'rename did not clear the audit -- reverted'))
        else:
            fixed.append(r['name'])
    print(f"\nfixed {len(fixed)}; skipped {len(skipped)}")
    for n, w in skipped:
        print(f"  SKIP {n}: {w}")


if __name__ == '__main__':
    main()
