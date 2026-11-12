#!/usr/bin/env python3
"""audit_zero_paints.py -- adjudicate `$zero` <-> register paints.

`patch_insn_reg_field` exists to bridge REGISTER-ALLOCATION differences.  `$zero`
is not an allocation: it is the constant 0.  A paint that repoints a READ off
`$zero` onto `$rX` asserts that `$rX` holds 0 at that instruction, and one that
repoints a read ONTO `$zero` asserts that the register it replaces held 0.  A
paint cannot establish either fact -- the compiled object either has a
zero-defining instruction reaching that point or it does not.

This tool decides which.  For every `off:field:old:new` triple where one side is
`zero`, it builds the object with all paints REMOVED (every other PostProc step
kept, so offsets line up) and walks reaching definitions to ask whether the
non-`zero` register is DEFINED-AS-ZERO at that instruction:

    SOUND    every reaching definition sets the register to 0
             (`or rX,$zero,$zero`, `addu rX,$zero,$zero`, `addiu rX,$zero,0`,
             `ori rX,$zero,0`, `sll rX,$zero,N`, or `and rX,$zero,*`)
    UNSOUND  at least one reaching definition sets it to something else, or the
             value is the function-entry value (an argument register!)
    INCONCLUSIVE  the CFG walk never reached the block, so no claim is made
                  (same situation `audit_paint_isomorphism` marks '?')

Expect ZERO `SOUND` verdicts in practice, and that is the point: `$zero` is
never the outcome of register ALLOCATION, so a paint involving it is always
bridging a value difference rather than a naming one.

The motivating case (2026-08-23, TT/func_8014C5E8): a repaint offered
`0x48:rt:zero:a0`, turning `sb $zero` into `sb $a0`.  Retail sets $a0 up with a
`move $a0,$zero` our compile never emits, so $a0 there still held the incoming
`u8 *src` argument -- the paint stored the low byte of a pointer where the game
stores 0.  It scored +2 on the resolver, `semcheck` passed it, and
`audit_paint_isomorphism` passed it too (`$zero` has no producer and `$a0`'s is
the function-entry value, which it classes as provable).

    python3 tools/overlay/audit_zero_paints.py [OVERLAY FUNC] [--all] [--json OUT]
"""
import argparse
import json
import os
import re
import subprocess
import sys
from pathlib import Path

_here = Path(__file__).resolve().parent
sys.path.insert(0, str(_here))
sys.path.insert(0, str(_here.parent / "build"))

import audit_paint_isomorphism as ISO  # noqa: E402

ROOT = _here.parent.parent
SRC = ROOT / "src" / "overlay"
RA = ISO.RA

def reaching_all(W):
    """Reaching definitions for EVERY register at EVERY instruction.

    `ISO.reaching` only records the registers an instruction actually READS,
    which is no use here: the paint asks about a register the UNPAINTED object
    does not read at that offset.  Same CFG and transfer function, but the
    state is snapshotted in full.
    """
    B, succ, bidx = ISO.blocks_of(W)
    pred = [[] for _ in B]
    for k, ss in enumerate(succ):
        for t in ss:
            pred[t].append(k)
    IN = [dict() for _ in B]
    IN[0] = {r: frozenset(['E']) for r in range(32)}
    CLOB = ISO.CLOB

    def run(state, s, e, snap=None):
        st = dict(state)
        pending = None
        for i in range(s, e):
            if snap is not None:
                snap[i] = dict(st)
            d, u, br, jmp, call = ISO.decode(W[i])
            for r in d:
                st[r] = frozenset([i])
            if pending is not None:
                for r in CLOB:
                    st[r] = frozenset([('call', pending)])
                pending = None
            if call:
                pending = i
        if pending is not None:
            for r in CLOB:
                st[r] = frozenset([('call', pending)])
        return st

    changed, it = True, 0
    while changed and it < 200:
        changed, it = False, it + 1
        for k, (s, e) in enumerate(B):
            if k == 0:
                continue
            merged = {}
            for pk in pred[k]:
                out = run(IN[pk], B[pk][0], B[pk][1])
                for r, v in out.items():
                    merged[r] = merged.get(r, frozenset()) | v
            if merged != IN[k]:
                IN[k] = merged
                changed = True
    snap = {}
    for k, (s, e) in enumerate(B):
        run(IN[k], s, e, snap)
    return snap

PAINT = re.compile(r'(0x[0-9a-f]+):(rs|rt|rd):([a-z0-9]+):([a-z0-9]+)')


def defines_zero(word):
    """Does `word` unconditionally define its destination as 0?"""
    op = word >> 26
    rs, rt, rd, f = (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31, word & 0x3F
    if op == 0:
        if f in (0x20, 0x21, 0x25) and rs == 0 and rt == 0:   # add/addu/or rd,$0,$0
            return True
        if f in (0x00, 0x02, 0x03) and rt == 0:               # sll/srl/sra rd,$0,N
            return True
        if f == 0x24 and (rs == 0 or rt == 0):                # and rd,$0,*
            return True
        return False
    if op in (0x08, 0x09) and rs == 0 and (word & 0xFFFF) == 0:   # addi/addiu rt,$0,0
        return True
    if op in (0x0d, 0x0e) and rs == 0 and (word & 0xFFFF) == 0:   # ori/xori rt,$0,0
        return True
    if op == 0x0c and (word & 0xFFFF) == 0:                       # andi rt,*,0
        return True
    return False


def audit_one(ov, func):
    text = (SRC / ov / f"{func}.c").read_text()
    paints = [m.groups() for m in PAINT.finditer(text)]
    zp = [p for p in paints if 'zero' in (p[2], p[3])]
    if not zp:
        return {'ov': ov, 'func': func, 'zero_paints': 0, 'rows': []}
    r = ISO.audit(ov, func)
    if 'error' in r:
        return {'ov': ov, 'func': func, 'error': r['error'], 'zero_paints': len(zp)}
    A = r['A']
    state = reaching_all(A)
    rows = []
    for off, field, old, new in zp:
        i = int(off, 16) // 4
        reg = new if old == 'zero' else old
        ri = RA.index(reg) if reg in RA else None
        verdict = 'UNKNOWN'
        detail = ''
        if ri is None:
            verdict = 'UNKNOWN'
        elif state is None:
            verdict = 'NOSTATE'
        else:
            prods = state.get(i, {}).get(ri)
            if prods is not None:
                prods = set(prods)
            if not prods:
                verdict, detail = 'INCONCLUSIVE', 'the CFG walk did not reach this block'
            elif '?' in prods:
                verdict, detail = 'UNREACHABLE', 'CFG walk found the block unreachable'
            elif any(not isinstance(x, int) for x in prods):
                verdict = 'UNSOUND'
                detail = 'entry/call value: %s' % sorted(map(str, prods))
            elif all(defines_zero(A[x]) for x in prods):
                verdict = 'SOUND'
                detail = 'all %d def(s) set $%s = 0' % (len(prods), reg)
            else:
                verdict = 'UNSOUND'
                detail = 'def(s) %s do not set $%s = 0' % (
                    ' '.join('%08x' % A[x] for x in sorted(prods)), reg)
        rows.append({'off': off, 'field': field, 'old': old, 'new': new,
                     'reg': reg, 'verdict': verdict, 'detail': detail})
    return {'ov': ov, 'func': func, 'zero_paints': len(zp), 'rows': rows}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('overlay', nargs='?')
    ap.add_argument('func', nargs='?')
    ap.add_argument('--all', action='store_true')
    ap.add_argument('--json')
    args = ap.parse_args()

    targets = []
    if args.all:
        out = subprocess.run(
            ['grep', '-rlE', r':(rs|rt|rd):zero:[a-z0-9]+|:(rs|rt|rd):[a-z0-9]+:zero',
             str(SRC)], capture_output=True, text=True).stdout.split()
        for f in sorted(out):
            p = Path(f)
            targets.append((p.parent.name, p.stem))
    else:
        targets = [(args.overlay, args.func)]

    results = []
    tally = {}
    for ov, fn in targets:
        r = audit_one(ov, fn)
        results.append(r)
        if 'error' in r:
            print(f"{ov}/{fn}: ERROR {r['error']}")
            tally['ERROR'] = tally.get('ERROR', 0) + 1
            continue
        for row in r['rows']:
            tally[row['verdict']] = tally.get(row['verdict'], 0) + 1
            if row['verdict'] != 'SOUND':
                print(f"{ov}/{fn} {row['off']}:{row['field']}:{row['old']}:"
                      f"{row['new']}  {row['verdict']}  {row['detail']}")
    print('\nverdicts:', tally)
    if args.json:
        json.dump(results, open(args.json, 'w'), indent=1)


if __name__ == '__main__':
    main()
