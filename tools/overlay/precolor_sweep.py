"""Brute PRECOLOR_WEB bitpos sweep for a single overlay function.

Usage: .venv/bin/python3 wip-scratch/precolor_sweep.py OV FUNC REG NMAX
Scores by number of starred diff lines from sbs.py (lower is better) and
reports whether the compiled side gained a `move`.
"""
import re
import subprocess
import sys

ov, fn, reg, nmax = sys.argv[1], sys.argv[2], sys.argv[3], int(sys.argv[4])
p = 'src/overlay/%s/%s.c' % (ov, fn)
base = open(p).read()

# strip any existing @CompileEnv line, remember it
m = re.search(r'^.*@CompileEnv:.*$\n?', base, re.M)
stripped = re.sub(r'^.*@CompileEnv:.*$\n?', '', base, flags=re.M)


def score():
    r = subprocess.run(['.venv/bin/python3', 'tools/overlay/sbs.py', ov, fn],
                       capture_output=True, text=True)
    lines = r.stdout.splitlines()
    if not lines:
        return None, 0, r.stderr.strip()[:80]
    n = len([x for x in lines if x.startswith('*')])
    cm = len([x for x in lines if '|' in x and 'move' in x.split('|', 1)[1]])
    return n, cm, lines[0]


open(p, 'w').write(stripped)
b, bm, info = score()
print('BASELINE (no CompileEnv): diffs=%s moves=%s  %s' % (b, bm, info))

best = []
for i in range(nmax + 1):
    s = '/* @CompileEnv: IDO52_PRECOLOR_WEB=%s:%d */\n' % (reg, i) + stripped
    open(p, 'w').write(s)
    n, cm, info = score()
    if n is None:
        continue
    if (b is not None and n < b) or cm > 0:
        print('  %s:%-4d diffs=%-4d compiled_moves=%d' % (reg, i, n, cm))
        best.append((n, cm, i))

open(p, 'w').write(base)
print('done; best:', sorted(best)[:8])
