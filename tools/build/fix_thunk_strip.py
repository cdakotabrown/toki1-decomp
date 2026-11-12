#!/usr/bin/env python3
"""Add missing strip_leading_instructions --count N to matched-C headers.

The recompiled IDO 5.2 (correctly) emits leading types.h thunk preambles
(jr ra; nop pairs) ahead of function bodies. Older matched-C files predate
this and lack the strip directive, so they now mismatch by a 16-byte shift.
This deterministically adds the right strip count and KEEPS the change only
if the function becomes byte-perfect.

Usage: python3 tools/build/fix_thunk_strip.py [--workers N] [--limit N]
"""
import json, subprocess, re, os, sys, argparse
from concurrent.futures import ThreadPoolExecutor, as_completed

ROOT = '.'
os.chdir(ROOT)


def exact(name):
    try:
        r = subprocess.run(['python3', 'tools/analysis/test_single_match.py', name],
                           capture_output=True, text=True, timeout=120)
    except subprocess.TimeoutExpired:
        return None
    m = re.search(r'Exact:\s+(\d+)/(\d+)', r.stdout)
    return (int(m.group(1)), int(m.group(2))) if m else None


def add_strip(src, cnt):
    line = f' * @PostProc: strip_leading_instructions --count {cnt}\n'
    if '@PostProc:' in src:
        return re.sub(r'( \* @PostProc:)', line + r'\1', src, count=1)
    # insert before first close-comment
    return src.replace('*/', line + ' */', 1)


def process(func):
    name = func['name']
    p = func['source']
    if not os.path.exists(p):
        return (name, 'missing', None)
    src = open(p).read()
    if 'strip_leading_instructions' in src:
        return (name, 'has_strip', None)
    e = exact(name)
    if e is None:
        return (name, 'test_fail', None)
    if e[0] == e[1]:
        return (name, 'already_ok', None)
    bak = src
    for cnt in (4, 8, 12, 19, 23):
        open(p, 'w').write(add_strip(bak, cnt))
        e2 = exact(name)
        if e2 and e2[0] == e2[1]:
            return (name, f'FIXED_strip{cnt}', cnt)
    open(p, 'w').write(bak)
    return (name, 'still_bad', None)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--workers', type=int, default=10)
    ap.add_argument('--limit', type=int, default=0)
    args = ap.parse_args()
    cfg = json.load(open('config/level3_functions.json'))
    fns = [f for f in cfg['functions'] if str(f.get('source', '')).endswith('.c')]
    if args.limit:
        fns = fns[:args.limit]
    from collections import Counter
    stats = Counter()
    fixed = []
    done = 0
    with ThreadPoolExecutor(max_workers=args.workers) as ex:
        futs = {ex.submit(process, f): f for f in fns}
        for fut in as_completed(futs):
            name, status, cnt = fut.result()
            done += 1
            key = status.split('strip')[0] if status.startswith('FIXED') else status
            stats[key] += 1
            if status.startswith('FIXED'):
                fixed.append((name, cnt))
            if done % 50 == 0:
                print(f'[{done}/{len(fns)}] fixed={len(fixed)} {dict(stats)}', flush=True)
    print('\n=== DONE ===')
    print('total:', len(fns))
    for k, v in sorted(stats.items()):
        print(f'  {k}: {v}')
    print(f'FIXED: {len(fixed)}')
    open('/tmp/thunk_fix_results.json', 'w').write(json.dumps(
        {'fixed': fixed, 'stats': dict(stats)}, indent=2))
    print('results -> /tmp/thunk_fix_results.json')


if __name__ == '__main__':
    main()
