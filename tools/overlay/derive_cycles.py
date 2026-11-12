#!/usr/bin/env python3
"""Zone-based k-cycle swap derivation from resolver residuals.

Improvements over tools/overlay/derive_reorder_swaps.py:
- registers wildcarded in the match key (rs/rt/rd zeroed) so a word that
  both moves AND needs a register paint still matches;
- branch words (beq/bne/blez/bgtz/regimm/b) are matched on opcode ONLY
  (imm16 wildcarded); the needed imm is emitted as adjust_branch_offset;
- jal/j matched on full word (imm26 is the callee);
- duplicate candidates disambiguated by distance (stable nearest-first).

Usage: derive_cycles.py OVERLAY FUNC [--gap N] [--base 0x10]
Emits @PostProc lines; prints unmatched zones as walls.
"""
import subprocess, sys, re, collections

ov, fn = sys.argv[1], sys.argv[2]
gap = 4
base = 0x10
for i, a in enumerate(sys.argv):
    if a == '--gap': gap = int(sys.argv[i+1])
    if a == '--base': base = int(sys.argv[i+1], 16)

out = subprocess.run(['python3', 'tools/overlay/resolve_overlay_relocs.py', ov, fn],
                     capture_output=True, text=True).stdout

BRANCH_OPS = {4, 5, 6, 7}  # beq bne blez bgtz
def key(w):
    op = w >> 26
    if op == 0:  # R-type: opcode+funct+shamt, wildcard rs/rt/rd
        return ('R', w & 0xFC0007FF)
    if op == 1:  # regimm branch: opcode+rt(cond kind), wildcard rs+imm
        return ('B', op, (w >> 16) & 0x1F)
    if op in BRANCH_OPS:
        return ('B', op)
    if op in (2, 3):  # j/jal: full
        return ('J', w)
    # I-type: opcode + imm16, wildcard rs/rt
    return ('I', op, w & 0xFFFF)

diffs = {}
for m in re.finditer(r'\[\s*(\d+)\] target=([0-9a-f]{8}) compiled=([0-9a-f]{8})', out):
    i, t, c = int(m.group(1)), int(m.group(2), 16), int(m.group(3), 16)
    diffs[i] = (t, c)

if not diffs:
    print('# no residuals parsed'); sys.exit(0)

idxs = sorted(diffs)
zones, cur = [], [idxs[0]]
for i in idxs[1:]:
    if i - cur[-1] <= gap: cur.append(i)
    else: zones.append(cur); cur = [i]
zones.append(cur)

all_swaps, all_adjust, walls = [], [], []
for zone in zones:
    tkeys = {i: key(diffs[i][0]) for i in zone}
    ckeys = {i: key(diffs[i][1]) for i in zone}
    if collections.Counter(tkeys.values()) != collections.Counter(ckeys.values()):
        tc = collections.Counter(tkeys.values()); cc = collections.Counter(ckeys.values())
        only_t = tc - cc; only_c = cc - tc
        walls.append((zone[0], zone[-1], dict(only_t), dict(only_c)))
        continue
    # fixed points first: same key in place
    pos = dict(ckeys)          # current key at each position
    swaps = []
    for w in zone:
        if pos[w] == tkeys[w]:
            continue
        cands = [x for x in zone if x > w and pos[x] == tkeys[w]]
        if not cands:
            walls.append((zone[0], zone[-1], {'mapping-failed@%d' % w: 1}, {}))
            swaps = None
            break
        src = cands[0]
        pos[w], pos[src] = pos[src], pos[w]
        swaps.append((w, src))
    if swaps is None:
        continue
    all_swaps += swaps
    # branch imm adjusts at final positions
    for w in zone:
        t = diffs[w][0]
        op = t >> 26
        if (op in BRANCH_OPS or op == 1):
            all_adjust.append((w, t & 0xFFFF))

for a, b in all_swaps:
    print(f"/* @PostProc: swap_instructions --offset1 {hex(a*4+base)} --offset2 {hex(b*4+base)} */")
for w, imm in all_adjust:
    print(f"/* @PostProc: adjust_branch_offset --offset {hex(w*4+base)} --imm {hex(imm)} */")
for z0, z1, ot, oc in walls:
    print(f"# WALL zone [{z0}..{z1}]: target-only {ot} compiled-only {oc}")
