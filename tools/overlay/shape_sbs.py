#!/usr/bin/env python3
#
# Written 2026-08-14. Part of the
# mnemonic-SHAPE alignment harness: shape_tdis.py (target disasm) + shape_dis.py
# (explicit-env-only compile, dodges the @CompileEnv prose-scrape trap) +
# shape_align.py (difflib opcode-stream alignment) + shape_sbs.py (side-by-side).
# Shape alignment finds real holes that exact%% hides — exact%% moves wholesale on
# allocation shifts and rewards count-compensating hooks. Intermediates in /tmp.
"""sbs2.py <FUNC> [TSTART] [TEND] -- aligned side-by-side of tdis vs dis."""
import sys, re, difflib

fn = sys.argv[1]
ts = int(sys.argv[2]) if len(sys.argv) > 2 else 0
te = int(sys.argv[3]) if len(sys.argv) > 3 else 10**9


def load(p):
    out = []
    for l in open(p):
        m = re.match(r'^\s*(\d+) ([0-9a-f]{8}) (.*)$', l.rstrip())
        if m:
            out.append((m.group(3).split()[0], m.group(3).replace('\t', ' '), m.group(2)))
    return out


t = load(f'/tmp/{fn}.tdis')
c = load(f'/tmp/{fn}.dis')
sm = difflib.SequenceMatcher(None, [x[0] for x in t], [x[0] for x in c], autojunk=False)
rows = []
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal':
        for k in range(i2 - i1):
            rows.append((i1 + k, j1 + k, tag))
    else:
        n = max(i2 - i1, j2 - j1)
        for k in range(n):
            rows.append((i1 + k if i1 + k < i2 else None,
                         j1 + k if j1 + k < j2 else None, tag))
for ti, ci, tag in rows:
    if ti is not None and not (ts <= ti < te):
        if ti is not None:
            continue
    if ti is None and not rows:
        continue
    if ti is None:
        # keep inserts near range
        pass
    a = f"{ti:4d} {t[ti][1]}" if ti is not None else "     " + " " * 30
    b = f"{ci:4d} {c[ci][1]}" if ci is not None else ""
    mark = '  ' if (ti is not None and ci is not None and t[ti][2] == c[ci][2]) else \
           ('~ ' if tag == 'equal' else '**')
    print(f"{a:<44}{mark}{b}")
