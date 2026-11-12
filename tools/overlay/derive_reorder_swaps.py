import subprocess, sys, re
ov, fn = sys.argv[1], sys.argv[2]
def mask(w):
    op = (w >> 26) & 0x3F
    if op == 0:  # R-type: keep op+funct+shamt
        return w & 0xFC0007FF & ~0x7C0  # zero rs,rt,rd, keep shamt+funct
    return w & 0xFC00FFFF  # I-type: opcode + imm16
out = subprocess.run(['python3','tools/overlay/resolve_overlay_relocs.py',ov,fn,'-v'],
                     capture_output=True, text=True, cwd='.').stdout
diffs = {}
branch_ops = {4,5,1,6,7,20,21,22,23}
for m in re.finditer(r'\[\s*(\d+)\] target=([0-9a-f]{8}) compiled=([0-9a-f]{8})', out):
    t, c = int(m.group(2),16), int(m.group(3),16)
    if ((t>>26) in (1,4,5,6,7,20,21,22,23) or ((t>>26)==1)) and (t>>26)==(c>>26) and (t & 0xFFFF0000)==(c & 0xFFFF0000):
        continue
    diffs[int(m.group(1))] = (t, c)
idxs = sorted(diffs)
runs = []
cur = [idxs[0]]
for i in idxs[1:]:
    if i - cur[-1] <= 2: cur.append(i)
    else: runs.append(cur); cur=[i]
runs.append(cur)
allswaps = []
for run in runs:
    tgt = {w: mask(diffs[w][0]) for w in run}
    cmp_ = {w: mask(diffs[w][1]) for w in run}
    if sorted(tgt.values()) != sorted(cmp_.values()):
        print(f"# run {run[0]}-{run[-1]} ({len(run)}): not a masked permutation")
        continue
    pos = dict(cmp_)
    swaps = []
    ok = True
    for w in run:
        if pos[w] == tgt[w]: continue
        cands = [x for x in run if x > w and pos[x] == tgt[w]]
        if not cands: ok = False; break
        src = cands[0]
        pos[w], pos[src] = pos[src], pos[w]
        swaps.append((w, src))
    if not ok:
        print(f"# run {run[0]}-{run[-1]}: mapping failed")
        continue
    print(f"# run {run[0]}-{run[-1]}: {len(swaps)} swaps")
    allswaps += swaps
for a,b in allswaps:
    print(f"/* @PostProc: swap_instructions --offset1 {hex(a*4+0x10)} --offset2 {hex(b*4+0x10)} */")
