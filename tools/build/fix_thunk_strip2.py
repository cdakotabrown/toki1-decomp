#!/usr/bin/env python3
"""Pass 2: authoritative thunk-strip count fixer.

Uses build_level3_c.py per-function (mini-config) verification — the same
pipeline as the real build, reliable for thunk/GCC functions (unlike
test_single_match). For each still-mismatching registered function:
  - if it has no strip directive: try counts 4,8,12,16,19,23
  - if it HAS a strip directive (wrong count for current compiler): remove
    the existing strip line(s) and retry the count sweep
Keeps the change only if the function builds byte-perfect. Logs the rest.
"""
import json, subprocess, re, os, tempfile, argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
from collections import Counter

ROOT = '.'
os.chdir(ROOT)
CFG = json.load(open('config/level3_functions.json'))
BYNAME = {f['name']: f for f in CFG['functions']}

STRIP_RE = re.compile(r'^\s*\*\s*@PostProc:\s*strip_leading_instructions.*\n', re.M)


def verify(name, cfgpath):
    e = BYNAME[name]
    mini = {"functions": [{"name": name, "address": e['address'], "size": e['size'],
            "source": f"src/main/matched/{name}.c", "region": e.get('region', ''),
            "match_status": "v"}]}
    json.dump(mini, open(cfgpath, 'w'))
    try:
        r = subprocess.run(['python3', 'tools/build/build_level3_c.py', '--config', cfgpath,
                            '-j', '1', '--no-cache'], capture_output=True, text=True, timeout=150)
    except subprocess.TimeoutExpired:
        return None
    return ('MATCH!' in r.stdout) and ('MISMATCH' not in r.stdout) and ('ERROR' not in r.stdout)


def add_strip(src, cnt):
    line = f' * @PostProc: strip_leading_instructions --count {cnt}\n'
    if '@PostProc:' in src:
        return re.sub(r'( \* @PostProc:)', line + r'\1', src, count=1)
    return src.replace('*/', line + ' */', 1)


def process(func):
    name = func['name']
    p = f"src/main/matched/{name}.c"
    if name not in BYNAME or not os.path.exists(p):
        return (name, 'missing', None)
    cfgpath = os.path.join(tempfile.gettempdir(), f'mini_{name}.json')
    src = open(p).read()
    if verify(name, cfgpath):
        return (name, 'already_ok', None)
    # base (no strip) to retry counts from
    base = STRIP_RE.sub('', src)
    had_strip = (base != src)
    for cnt in (4, 8, 12, 16, 19, 23):
        open(p, 'w').write(add_strip(base, cnt))
        if verify(name, cfgpath):
            return (name, f'FIXED', cnt)
    open(p, 'w').write(src)  # restore original
    try: os.remove(cfgpath)
    except OSError: pass
    return (name, 'still_bad_hadstrip' if had_strip else 'still_bad', None)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--bands', default='/tmp/mainfix')
    ap.add_argument('--workers', type=int, default=10)
    args = ap.parse_args()
    funcs = []
    seen = set()
    for fn in sorted(os.listdir(args.bands)):
        for f in json.load(open(os.path.join(args.bands, fn))):
            if f['name'] not in seen:
                seen.add(f['name']); funcs.append(f)
    print(f'processing {len(funcs)} functions, {args.workers} workers')
    stats = Counter(); fixed = []; done = 0
    with ThreadPoolExecutor(max_workers=args.workers) as ex:
        futs = {ex.submit(process, f): f for f in funcs}
        for fut in as_completed(futs):
            name, status, cnt = fut.result()
            done += 1
            stats[status] += 1
            if status == 'FIXED':
                fixed.append((name, cnt))
            if done % 30 == 0:
                print(f'[{done}/{len(funcs)}] fixed={len(fixed)} {dict(stats)}', flush=True)
    print('\n=== DONE ===')
    for k, v in sorted(stats.items()):
        print(f'  {k}: {v}')
    print('FIXED:', len(fixed))
    json.dump({'fixed': fixed, 'stats': dict(stats)}, open('/tmp/thunk_fix2_results.json', 'w'), indent=2)


if __name__ == '__main__':
    main()
