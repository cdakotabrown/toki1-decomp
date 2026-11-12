#!/usr/bin/env python3
"""main_open_inventory.py -- one line per OPEN main-binary function, with its blocker.

`audit_main_functions.py` says how many are open.  This says what each one needs,
so the population can be attacked systematically instead of by picking whatever
is nearest.

Every open address is placed in exactly one bucket:

  NEAR-MISS     a faithful .c exists and scores >= 75% exact -- finish it
  PARTIAL       a faithful .c exists, 25-75% -- real decomp, real gap
  WEAK          a faithful .c exists, < 25% -- probably needs re-deriving
  UNMEASURED    a .c exists but has never been scored (no window, no address, ...)
  UNFINISHED    a STUB whose own header says deferred / out of scope / stubbed --
                this is abandoned work, NOT a compiler blocker
  STRUCTURAL    a STUB whose header claims the shape cannot be expressed in C
                (BIOS trampoline, dual entry, hidden register, no jr ra)
  PC-OVERRIDE   only a src/main/pc-port/ gameplay override exists: UNSTARTED
  UNSTARTED     no source at all

and carries the blocker sentence its header declares, if any.

    python3 tools/analysis/main_open_inventory.py [--md OUT.md] [--json OUT.json]
                                                  [--region game|library|all]
"""
import argparse
import json
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OPEN = ('semi_real', 'semi_stub', 'missing', 'pc_port_only')

GAVE_UP = re.compile(r'(?i)out of scope|deferred|stubbed|not implemented|placeholder'
                     r'|todo|full decomp|incomplete|for now|reconstructing')
STRUCT = re.compile(r'(?i)cannot be (?:expressed|implemented|written)|inline asm'
                    r'|no `?jr ra|dual[- ]entry|hidden reg|bios|trampoline'
                    r'|hand-written|alt[- ]entry')
BLOCKER = re.compile(r'(?im)^[\s*/]*(?:BLOCKER|Blocker category|Blocker class|Status)\s*[:=]\s*(.{4,150})')


_MEAS = None
def score(name, path, tries=2):
    """Prefer config/main_open_measurements.json (measure_main_open.py) when it
    has this function; fall back to running the test."""
    global _MEAS
    if _MEAS is None:
        mp = os.path.join(ROOT, 'config/main_open_measurements.json')
        _MEAS = json.load(open(mp)) if os.path.exists(mp) else {}
    rec = _MEAS.get(name)
    if rec and rec.get('verdict') != 'FAIL':
        if rec['verdict'] == 'OVERRUN':
            return ('OVERRUN', rec['pct'], rec['fixups'], rec['overrun'])
        return ('OK', rec['pct'], rec['fixups'], 0)
    r = ''
    for _ in range(tries):
        try:
            r = subprocess.run([sys.executable, os.path.join(ROOT, 'tools/analysis/test_single_match.py'),
                                name, '-c', path], capture_output=True, text=True,
                               cwd=ROOT, timeout=600).stdout
            break
        except Exception:
            continue
    m = re.search(r'Exact:\s+(\d+)/(\d+) \((\d+\.\d)%\)', r)
    f = re.search(r'Fixup:\s+(\d+)', r)
    pct = float(m.group(3)) if m else None
    fx = int(f.group(1)) if f else 0
    if 'WINDOW OVERRUN' in r:
        # The compiled body is LONGER than the symbol.  Real information, not a
        # measurement failure: the C emits instructions the target does not have.
        u = re.search(r'untruncated: (\d+)', r)
        c = re.search(r'Compiled:\s+(\d+)', r)
        over = (int(u.group(1)) - int(c.group(1))) if (u and c) else None
        return ('OVERRUN', pct, fx, over)
    if pct is None:
        return None
    return ('OK', pct, fx, 0)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--md')
    ap.add_argument('--json')
    ap.add_argument('--region', default='game', choices=('game', 'library', 'all'))
    ap.add_argument('--jobs', type=int, default=8)
    args = ap.parse_args()

    aud = json.load(open(os.path.join(ROOT, 'config/main_function_audit_by_addr.json')))['addresses']
    rows = []
    for a, v in aud.items():
        if v.get('kind') not in OPEN:
            continue
        if args.region != 'all' and v.get('region') != args.region:
            continue
        rows.append((int(a, 16), v))
    rows.sort()

    def build(item):
        addr, v = item
        src = v.get('source') or ''
        name = os.path.basename(src)[:-2] if src.endswith('.c') else None
        rec = {'addr': '0x%08X' % addr, 'size': v.get('size'), 'kind': v['kind'],
               'true_start': v.get('true_start'), 'true_size': v.get('true_size'),
               'name': name, 'source': src.replace(ROOT + '/', ''), 'blocker': None}
        txt = ''
        if src.endswith('.c') and os.path.exists(src):
            txt = open(src).read()
            m = BLOCKER.search(txt)
            if m:
                rec['blocker'] = ' '.join(m.group(1).split())[:150]
        if v['kind'] == 'pc_port_only':
            rec['bucket'] = 'PC-OVERRIDE'
        elif v['kind'] == 'missing':
            rec['bucket'] = 'UNSTARTED'
        elif v['kind'] == 'semi_stub':
            rec['bucket'] = ('STRUCTURAL' if STRUCT.search(txt)
                             else 'UNFINISHED' if GAVE_UP.search(txt) else 'UNFINISHED?')
        else:
            s = score(name, src) if name else None
            if s is None:
                rec['bucket'] = 'UNMEASURED'
            elif s[0] == 'OVERRUN':
                rec['bucket'] = 'OVERRUN'
                rec['pct'], rec['fixups'], rec['over'] = s[1], s[2], s[3]
                rec['blocker'] = ((rec['blocker'] or '') +
                                  ' [body %s words LONGER than the symbol]' % s[3]).strip()
            else:
                rec['pct'], rec['fixups'] = s[1], s[2]
                rec['bucket'] = ('NEAR-MISS' if s[1] >= 75 else
                                 'PARTIAL' if s[1] >= 25 else 'WEAK')
        return rec

    with ThreadPoolExecutor(max_workers=args.jobs) as ex:
        recs = list(ex.map(build, rows))

    order = ['NEAR-MISS', 'PARTIAL', 'WEAK', 'OVERRUN', 'UNMEASURED', 'UNFINISHED', 'UNFINISHED?',
             'STRUCTURAL', 'PC-OVERRIDE', 'UNSTARTED']
    from collections import Counter
    c = Counter(r['bucket'] for r in recs)
    print(f'{len(recs)} open {args.region}-region functions')
    for k in order:
        if c.get(k):
            print(f'  {k:12s} {c[k]:4d}')

    if args.json:
        json.dump(recs, open(args.json, 'w'), indent=1)
    if args.md:
        with open(args.md, 'w') as f:
            f.write(f'# Main-binary open worklist ({args.region} region)\n\n')
            f.write(f'{len(recs)} open functions. Generated by '
                    f'`tools/analysis/main_open_inventory.py`.\n\n')
            f.write('**START HERE / real size**: when present, the recorded '
                    'address is a PROLOGUE and the function actually begins at '
                    'the listed address -- IDO -O2 hoisted its opening global '
                    'reads above the prologue. C written from the recorded '
                    'address is missing those statements and cannot match, '
                    'however good it otherwise is. Use the corrected window. '
                    'See docs/reference/PHANTOM-SYMBOL-AUDIT.md.\n\n')
            for k in order:
                sel = [r for r in recs if r['bucket'] == k]
                if not sel:
                    continue
                f.write(f'\n## {k} ({len(sel)})\n\n')
                sel.sort(key=lambda r: -(r.get('pct') or 0) if 'pct' in r else (r['size'] or 0))
                f.write('| addr | size | START HERE | real size | name | % '
                        '| fixups | blocker |\n')
                f.write('|---|---:|---|---:|---|---:|---:|---|\n')
                for r in sel:
                    f.write('| %s | %s | %s | %s | %s | %s | %s | %s |\n' % (
                        r['addr'], r['size'] or '',
                        r.get('true_start') or '',
                        r.get('true_size') or '',
                        r['name'] or '',
                        ('%.1f' % r['pct']) if 'pct' in r else '',
                        r.get('fixups', ''), (r['blocker'] or '').replace('|', '/')))
        print('wrote', args.md)


if __name__ == '__main__':
    main()
