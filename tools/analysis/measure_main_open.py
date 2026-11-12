#!/usr/bin/env python3
"""Measure EVERY open main-binary function once and persist the numbers
(2026-09-06): config/main_open_measurements.json (sidecar, regenerated wholesale)
plus one machine-readable `@Measured:` line in each file's header.

    python3 tools/analysis/measure_main_open.py [--jobs 6] [--no-stamp] [NAME ...]
    python3 tools/analysis/measure_main_open.py --only FUNC        (same as NAME)

⚠️ SHARED-TREE HAZARD: with NO names this rewrites the `@Measured:` line in
EVERY open semi (~345 files) and regenerates the sidecar wholesale. In a tree
where others are working that collides with their edits (344 unintended
files had to be reverted once). Pass the function NAME (or
--only NAME) when you have touched a single semi; save the bare full run for
the end of a piece of work on a quiet tree. It now warns and requires --yes to do the
full stamping run non-interactively.

Per entry: compiled words, target words, exact, pct, fixups (words the harness
copied from the target at non-reloc sites), overrun (compiled body longer than
the window), and a verdict:
    BYTE-PERFECT (Fixup 0 only -- a 100% with Fixup N is SEMI: N words are the
    target's, not the compiler's) / SEMI / OVERRUN / STUB (<=8 compiled words:
    no real body, target >= 16) / FAIL (compile or window error; the raw error line is kept).
main_open_inventory.py reads this sidecar instead of re-running ~440 tests.
"""
import os, re, sys, json, subprocess, datetime
from concurrent.futures import ThreadPoolExecutor
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OUT = os.path.join(ROOT, 'config/main_open_measurements.json')
OPEN = ('semi_real', 'semi_stub')
DATE = datetime.date.today().isoformat()

def measure(name, src):
    try:
        r = subprocess.run([sys.executable, os.path.join(ROOT, 'tools/analysis/test_single_match.py'),
                            name, '-c', src, '--verbose'], capture_output=True, text=True, cwd=ROOT, timeout=600)
        o = r.stdout + r.stderr
    except Exception as e:
        return {'verdict': 'FAIL', 'error': str(e)[:160]}
    m = re.search(r'Exact:\s+(\d+)/(\d+) \((\d+\.\d)%\)', o)
    c = re.search(r'Compiled:\s+(\d+)', o); u = re.search(r'untruncated: (\d+)', o)
    b = re.search(r'Body:\s+(\d+) words', o)
    t = re.search(r'Target:\s+(\d+)', o); f = re.search(r'Fixup:\s+(\d+)', o)
    rec = {'date': DATE}
    if not m or not c:
        err = [l.strip() for l in o.split('\n') if re.search(r'(?i)\berror\b|failed|could not', l)]
        rec.update(verdict='FAIL', error=(err[0] if err else 'no Exact: line')[:160]); return rec
    comp = int(b.group(1)) if b else int(c.group(1)); tgt = int(t.group(1)) if t else int(m.group(2))
    over = 'WINDOW OVERRUN' in o
    rec.update(compiled=comp, target=tgt, exact=int(m.group(1)), pct=float(m.group(3)),
               fixups=int(f.group(1)) if f else 0, overrun=(int(u.group(1)) - comp) if (u and over) else 0)
    # BYTE-PERFECT only when the harness copied nothing: a Fixup word is a target
    # byte, not a compiled one (docs/reference/MAIN-RELOC-AUDIT.md).
    if 'BYTE-PERFECT' in o and rec['fixups'] == 0 and not over: rec['verdict'] = 'BYTE-PERFECT'
    elif over: rec['verdict'] = 'OVERRUN'
    elif comp <= 8 and tgt >= 16: rec['verdict'] = 'STUB'   # an empty body; tiny real functions stay SEMI
    else: rec['verdict'] = 'SEMI'
    return rec

def stamp(src, rec):
    s = open(src).read(); end = s.find('*/')
    if end < 0: return
    if rec['verdict'] == 'FAIL':
        line = f" * @Measured: {rec['date']} FAIL -- {rec['error']}"
    else:
        line = (f" * @Measured: {rec['date']} {rec['exact']}/{rec['target']} exact ({rec['pct']}%), "
                f"compiled {rec['compiled']} vs target {rec['target']} words, Fixup {rec['fixups']}"
                + (f", OVERRUN +{rec['overrun']}" if rec['overrun'] else '') + f" -- {rec['verdict']}")
    head = s[:end]
    if '@Measured:' in head:
        s = re.sub(r' \* @Measured:[^\n]*\n', line + '\n', s, count=1)
    else:
        s = s[:end].rstrip(' ') + line + '\n ' + s[end:]
    open(src, 'w').write(s)

def main():
    jobs = 6; names = []; do_stamp = '--no-stamp' not in sys.argv
    assume_yes = '--yes' in sys.argv
    a = sys.argv[1:]
    while a:
        x = a.pop(0)
        if x == '--jobs': jobs = int(a.pop(0))
        elif x == '--only': names.append(a.pop(0))
        elif not x.startswith('--'): names.append(x)
    aud = json.load(open(os.path.join(ROOT, 'config/main_function_audit_by_addr.json')))['addresses']
    items = []
    for addr, v in aud.items():
        if v.get('kind') not in OPEN or not (v.get('source') or '').endswith('.c'): continue
        if not os.path.exists(v['source']):
            continue        # audit JSON older than a move to matched/ -- skip, do not crash
        name = os.path.basename(v['source'])[:-2]
        if names and name not in names: continue
        items.append((addr, name, v['source'], v))
    items.sort()
    # ⚠️ Guard the shared-tree hazard: a bare run stamps every open semi.
    if do_stamp and not names and not assume_yes:
        print(f'REFUSING: this would rewrite the @Measured: line in {len(items)} '
              f'files and regenerate the sidecar wholesale.')
        print('  For one function:  measure_main_open.py --only FUNC')
        print('  Sidecar only:      measure_main_open.py --no-stamp')
        print('  Really do it all:  measure_main_open.py --yes')
        print('Others may be editing these files; see the module docstring.')
        return 1
    old = json.load(open(OUT)) if os.path.exists(OUT) and names else {}
    def work(it):
        addr, name, src, v = it
        rec = measure(name, src); rec.update(addr=addr, size=v.get('size'), kind=v['kind'], region=v.get('region'))
        if do_stamp: stamp(src, rec)
        return name, rec
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        res = dict(ex.map(work, items))
    old.update(res)
    with open(OUT, 'w') as fh:            # keep the trailing newline: a bare
        json.dump(old, fh, indent=1, sort_keys=True)   # json.dump drops it and
        fh.write('\n')                    # every run showed a 1-line diff
    from collections import Counter
    print(f'{len(res)} measured; verdicts: {dict(Counter(r["verdict"] for r in res.values()))}')
    for n, r in sorted(res.items()):
        if r['verdict'] == 'FAIL': print(f'  FAIL {n}: {r["error"]}')
    print('wrote', os.path.relpath(OUT, ROOT))

if __name__ == '__main__':
    sys.exit(main() or 0)
