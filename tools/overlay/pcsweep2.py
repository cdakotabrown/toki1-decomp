"""PRECOLOR_WEB sweep reporting compiled word count (the count-deficit metric).

Usage: pcsweep2.py OV FUNC NMAX REG[,REG...]
"""
import re
import subprocess
import sys

ov, fn, nmax = sys.argv[1], sys.argv[2], int(sys.argv[3])
regs = sys.argv[4].split(',')
p = 'src/overlay/%s/%s.c' % (ov, fn)
base = open(p).read()
stripped = re.sub(r'^.*@CompileEnv:.*$\n?', '', base, flags=re.M)


def run():
    r = subprocess.run(['.venv/bin/python3', 'tools/overlay/sbs.py', ov, fn],
                       capture_output=True, text=True)
    lines = r.stdout.splitlines()
    if not lines:
        return None
    m = re.search(r'words target=(\d+) compiled=(\d+)', lines[0])
    if not m:
        return None
    n = len([x for x in lines if x.startswith('*')])
    return int(m.group(1)), int(m.group(2)), n


open(p, 'w').write(stripped)
b = run()
print('BASELINE:', b)
hits = []
for reg in regs:
    for i in range(nmax + 1):
        open(p, 'w').write('/* @CompileEnv: IDO52_PRECOLOR_WEB=%s:%d */\n'
                           % (reg, i) + stripped)
        v = run()
        if v is None:
            continue
        t, c, n = v
        if c == t:
            print('  ** %s:%-4d target=%d compiled=%d diffs=%d' % (reg, i, t, c, n))
            hits.append((n, reg, i))
open(p, 'w').write(base)
print('count-exact hits:', sorted(hits)[:10])
