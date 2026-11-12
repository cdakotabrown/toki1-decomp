#!/usr/bin/env python3
"""Report which statement SEEDS each constant web, for FORM_SITE targeting.

When a held constant's `li` is hoisted to the entry block but retail defines it
inside an arm, the lever is `IDO52_FORM_SITE=const:VAL:STMT:0` at the statement
that SEEDED the web -- NOT Hook 24, which addresses block-entry range-head
reloads and cannot see this (IDO52_RELOAD_DUMP prints nothing for it).

The web's live range starts where it was seeded, so globalcolor places the def
at a block dominating every use; if the seed is an early, unrelated use of the
same literal -- a `% 3` divisor seeding the web that three later `== 3`
compares join -- that dominator is the function entry.

This runs the compile once with IDO52_FORM_TRACE=1 and prints, per constant
web: the seeding statement (the query with bit=65535 and res=1), the statements
that join it, and any value that has MORE THAN ONE descriptor -- which is the
u8/s8 typing split, where `type=8` (uchar) and `type=6` (int) uses of the same
literal end up in different webs and each re-materializes its own `li`.

    python3 tools/analysis/find_const_seed.py touzyou_check [--value 3]

Denying a JOIN does nothing: membership comes from the descriptor cache and
never re-consults f_constinreg.  Aim at the seed.

⚠️ SCOPE.  This is only the lever when a const web's def is HOISTED.  A residual
that happens to involve a constant is not automatically this: on
func_8007375C (one word from exact, filed dead_web_home_copy) the trace names a
seed at stmt 7 and both FORM_SITE and FORM_SPLIT there leave it at 60/61.
Confirm from the disassembly that retail defines the constant deeper than we do
before spending directives on it.
"""
import argparse
import collections
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools' / 'build'))

FORMQ = re.compile(
    r'\[FORMQ (\w+)\] q=(\d+) stmt=(\d+) op=(\S+) val=(\S+) type=(\d+) '
    r'ctx=(\d+) desc=(\S+) bit=(\d+) res=(\d+)')


def find_source(name):
    for d in ('matched', 'semi-matched'):
        p = ROOT / 'src/main' / d / (name + '.c')
        if p.exists():
            return p
    return None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('func')
    ap.add_argument('--value', help='only report this constant (decimal or 0x...)')
    args = ap.parse_args()

    src = find_source(args.func)
    if src is None:
        sys.exit('no src/main/{matched,semi-matched}/%s.c' % args.func)

    import ido_flags
    cc = ROOT / 'tools/ido-static-recomp/build/5.2/out/cc'
    cmd = [str(cc)] + ido_flags.base_flags([ROOT / 'include'])
    cmd += ido_flags.pic_flags(str(src))
    cmd += ['-c', str(src), '-o', os.devnull]
    env = dict(os.environ, IDO52_FORM_TRACE='1')
    r = subprocess.run(cmd, capture_output=True, text=True, env=env, cwd=str(ROOT))

    want = None
    if args.value:
        want = int(args.value, 0)

    webs = collections.defaultdict(lambda: {'seed': None, 'joins': [], 'denied': []})
    by_val = collections.defaultdict(set)
    for line in r.stderr.splitlines() + r.stdout.splitlines():
        m = FORMQ.search(line)
        if not m:
            continue
        kind, q, stmt, op, val, typ, ctx, desc, bit, res = m.groups()
        v = int(val, 0)
        if want is not None and v != want:
            continue
        by_val[v].add((desc, typ))
        key = (v, desc)
        w = webs[key]
        if bit == '65535' and res == '1':
            w['seed'] = (int(stmt), int(q), typ)
        elif res == '1':
            w['joins'].append(int(stmt))
        else:
            w['denied'].append(int(stmt))

    if not webs:
        print('no constant-formation queries%s' % (
            ' for that value' if want is not None else ''))
        return

    for (v, desc), w in sorted(webs.items()):
        seed = w['seed']
        print('value %-10s desc=%s type=%s' % (
            hex(v), desc, seed[2] if seed else '?'))
        if seed:
            print('   SEED   stmt=%-5d (q=%d)   ->  IDO52_FORM_SITE=const:%d:%d:0'
                  % (seed[0], seed[1], v, seed[0]))
        else:
            if w['denied'] and not w['joins']:
                print('   no web -- every query returned res=0, so this literal '
                      'is materialized\n           per site and there is nothing '
                      'for FORM_SITE to move')
            else:
                print('   SEED   not seen in this trace (web formed before the '
                      'first query here)')
        if w['joins']:
            print('   joins  %s   (denying these does nothing)'
                  % ', '.join('stmt=%d' % s for s in w['joins']))
        if w['denied']:
            print('   res=0  %s' % ', '.join('stmt=%d' % s for s in w['denied']))

    for v, descs in sorted(by_val.items()):
        if len(descs) > 1:
            print('\n⚠️  value %s has %d descriptors %s -- the u8/s8 typing split.'
                  % (hex(v), len(descs), sorted(d for d, _ in descs)))
            print('    type=8 is uchar, type=6 is int; they will not share a web,'
                  ' so each\n    re-materializes its own `li`. Flip the u8'
                  ' field/parameter to s8 to merge.')


if __name__ == '__main__':
    main()
