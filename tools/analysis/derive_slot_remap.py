#!/usr/bin/env python3
"""derive_slot_remap.py -- derive a `remap_stack_slots` map for a main-binary function.

    python3 tools/analysis/derive_slot_remap.py <FUNC> <ADDR> <SIZE> <src.c> [--apply]

WHY THIS EXISTS.  A function whose spill slots sit at different offsets from
retail's reports a large `Fixup` count and an UNCHANGED `Exact`, because the
harness copies every relocation-free immediate it cannot reproduce straight from
the target -- so an sp displacement that is simply wrong compares EQUAL and hides
itself.  `sprite_scatter_show` was carrying nine such words; correcting the slots moved
`Exact` by zero and `Fixup` from 12 to 3.  The honest score is `exact - fixup`,
and this class is invisible to anything that reads `Exact` alone.

WHAT IT DOES.  Walks the positional target-vs-compiled dump, and for every word
where the MNEMONIC agrees and the single `N(sp)` displacement does not, records
ours -> retail.  A mapping is emitted only when every site for a given source
offset agrees on one destination; a source offset with two different
destinations is reported and left out, because that means the streams are
misaligned there and a remap would be fitted to noise.

⚠️ `remap_stack_slots` also rewrites `addiu $sp,$sp,N` when N collides with a map
key, silently unbalancing the frame -- so this tool reads the frame size out of
the prologue and passes `--keep-frame` itself when a key collides with it.

⚠️ This is NOT a licence to bend offsets until bytes agree.  It is sound only
where the mnemonic, the registers and the surrounding stream already match and
the ONLY difference is which slot the compiler picked, which is a real and
well-documented IDO/retail divergence (declared locals sit above spill slots, so
source order cannot reach them).  Read the emitted map: a uniform shift is the
signature of the real thing, a scatter is the signature of a misaligned compare.
"""
import importlib.util, os, re, sys

ROOT = '.'


def main():
    if len(sys.argv) < 5:
        print(__doc__)
        sys.exit(2)
    fn, addr, size, src = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3]), sys.argv[4]
    apply_it = '--apply' in sys.argv[5:]

    saved = sys.argv
    sys.argv = ['dmd']
    spec = importlib.util.spec_from_file_location(
        'dmd', os.path.join(ROOT, 'tools/analysis/dump_main_diff.py'))
    m = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(m)
    sys.argv = saved
    m.ADDR_S = '0x%08X' % addr

    T = m.target(addr, size)
    C = m.words(m.build(src, fn), fn)
    MEM = re.compile(r'\b(lw|sw|lh|sh|lb|sb|lbu|lhu|lwl|lwr|swl|swr)\b')
    SP = re.compile(r'(-?\d+)\(sp\)')

    votes, frame = {}, None
    fm = re.search(r'addiu\s+sp,sp,(-\d+)', C[0][2] if C else '')
    if fm:
        frame = -int(fm.group(1))
    for i in range(min(len(T), len(C))):
        ts, cs = T[i][2], C[i][2]
        a, b = MEM.search(ts), MEM.search(cs)
        if not a or not b or a.group(1) != b.group(1):
            continue
        to, co = SP.findall(ts), SP.findall(cs)
        if len(to) == 1 and len(co) == 1 and to[0] != co[0]:
            votes.setdefault(int(co[0]), {}).setdefault(int(to[0]), 0)
            votes[int(co[0])][int(to[0])] += 1

    pairs, ambiguous = [], []
    for ours in sorted(votes):
        d = votes[ours]
        if len(d) == 1:
            pairs.append((ours, next(iter(d)), d[next(iter(d))]))
        else:
            ambiguous.append((ours, d))

    if not pairs and not ambiguous:
        print('no sp-displacement differences on matching mnemonics')
        return
    for ours, theirs, n in pairs:
        print('ours %5d -> retail %5d   (%d site%s)' % (ours, theirs, n, '' if n == 1 else 's'))
    for ours, d in ambiguous:
        print('⚠️  ours %5d is AMBIGUOUS %s -- left out; the streams are '
              'misaligned here' % (ours, dict(d)))
    if not pairs:
        return

    mp = ','.join('%d:%d' % (o, t) for o, t, _ in pairs)
    keep = frame is not None and any(o == frame for o, _, _ in pairs)
    line = "@PostProc: remap_stack_slots --map '%s'%s" % (mp, ' --keep-frame' if keep else '')
    if frame is not None:
        print('\nframe %d%s' % (frame, ' -- a map key collides with it, adding --keep-frame'
                                if keep else ''))
    print(' * ' + line)

    if apply_it:
        txt = open(src).read()
        if 'remap_stack_slots' in txt:
            print('file already carries a remap_stack_slots -- not written')
            return
        lines = txt.split('\n')
        idx = [i for i, l in enumerate(lines) if '@PostProc:' in l]
        if not idx:
            print('no @PostProc line to anchor to -- not written')
            return
        pre = re.match(r'^(\s*(?:/\*|\*)?\s*)@PostProc:', lines[idx[-1]]).group(1)
        block = not pre.strip().startswith('/*')
        lines.insert(idx[-1] + 1, pre + line + ('' if block else ' */'))
        open(src, 'w').write('\n'.join(lines))
        print('written to %s' % src)


if __name__ == '__main__':
    main()
