#!/usr/bin/env python3
#
# Written 2026-08-14. Part of the
# mnemonic-SHAPE alignment harness: shape_tdis.py (target disasm) + shape_dis.py
# (explicit-env-only compile, dodges the @CompileEnv prose-scrape trap) +
# shape_align.py (difflib opcode-stream alignment) + shape_sbs.py (side-by-side).
# Shape alignment finds real holes that exact%% hides — exact%% moves wholesale on
# allocation shifts and rewards count-compensating hooks. Intermediates in /tmp.
"""align.py <FUNC> [--full] [--ctx N] -- align /tmp/<FUNC>.tdis vs /tmp/<FUNC>.dis
on mnemonic sequence (register/immediate agnostic)."""
import sys, re, difflib

fn = sys.argv[1]
full = '--full' in sys.argv


def load(p):
    out = []
    for l in open(p):
        m = re.match(r'^\s*(\d+) ([0-9a-f]{8}) (.*)$', l.rstrip())
        if m:
            txt = m.group(3)
            out.append((txt.split()[0], txt, m.group(2)))
    return out


t = load(f'/tmp/{fn}.tdis')
c = load(f'/tmp/{fn}.dis')
a = [x[0] for x in t]
b = [x[0] for x in c]
print(f"target {len(a)} words, compiled {len(b)} words")
sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
print(f"mnemonic shape ratio = {sm.ratio()*100:.1f}%")
nex = sum(1 for i in range(min(len(t), len(c))) if t[i][2] == c[i][2])
print(f"positional exact words = {nex}/{len(a)}")
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal':
        if full:
            print(f"  equal  t[{i1}:{i2}] c[{j1}:{j2}]  len {i2-i1}  (off {j1-i1:+d})")
        continue
    print(f"  {tag:8s} t[{i1}:{i2}] ({i2-i1}) c[{j1}:{j2}] ({j2-j1})  off_before={j1-i1:+d} off_after={j2-i2:+d}")
    for k in range(i1, i2):
        print(f"      T{k:4d}  {t[k][1]}")
    for k in range(j1, j2):
        print(f"      C{k:4d}        {c[k][1]}")
