#!/usr/bin/env python3
"""Name the class_w field cells that keep their own symbol (main decision rows).

class_w[160] (0x801271B0, 68-byte CLASS_W records, include/tokimeki/class_w.h)
is reached through `class_w[i].field` by the struct pass wherever that keeps
the bytes.  A volatile access, or a function where the separate symbols stop
IDO sharing a base register, keeps one symbol per field; those already carry
names of the form cw<slot>_<field> (cw78_x, cw94_prio, cw98_y, ...).  This
generator gives every remaining `D_` cell inside class_w the same form, from
the struct layout (tools/naming/layouts/class_w.json):

    x.half[1] -> x, y.half[1] -> y (the pixel coordinate), x.half[0] -> x_frac,
    x.fix -> x_fix, disp_flag -> disp, attribute -> attr, tpage_sel -> tpage,
    unkNN -> wNN (the offset), every other leaf by its own name.

The view at the offset is chosen by the token's declared width.

usage: gen_main_cells.py OUT.json
"""
import collections
import glob
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'naming'))
from apply_interior import strip_comments  # noqa: E402

WIDTH = {'u8': 1, 's8': 1, 'char': 1, 'u16': 2, 's16': 2, 'short': 2, 'u32': 4, 's32': 4, 'int': 4, 'ptr': 4}
SHORT = {'x.half[1]': 'x', 'y.half[1]': 'y', 'x.half[0]': 'x_frac', 'y.half[0]': 'y_frac',
         'x.fix': 'x_fix', 'y.fix': 'y_fix', 'disp_flag': 'disp', 'attribute': 'attr', 'tpage_sel': 'tpage'}


def decl_width(stmt):
    if '*' in stmt:
        return 4, True
    d = re.sub(r'\b(volatile|const|extern|static)\b', ' ', stmt).split()
    t = d[0] if d else ''
    if t in ('unsigned', 'signed') and len(d) > 2:
        t = {'char': 'u8', 'short': 'u16', 'int': 'u32', 'long': 'u32'}.get(d[1], t)
    return WIDTH.get(t), '[' in stmt


def main():
    out = sys.argv[1]
    os.chdir(ROOT)
    lay = json.load(open('tools/naming/layouts/class_w.json'))
    lo, rs, n = int(lay['host'], 16), lay['size'], lay['count']
    syms = json.load(open('config/symbol_names.json'))['symbols']
    used = {v.get('name') for v in syms.values()}
    landed = {int(k, 16) for k, v in syms.items() if ':' not in k and v.get('landed') and v.get('kind') == 'data'}
    widths = collections.defaultdict(set)
    files = glob.glob('src/main/matched/*.c') + [g for g in glob.glob('src/overlay/*/*.c')
                                                  if '/EVENT/' not in g and '/GYOZI/' not in g]
    for f in files:
        t = strip_comments(open(f, errors='replace').read())
        for m in set(re.findall(r'\bD_(80[0-9A-Fa-f]{6})\b', t)):
            a = int(m, 16)
            if not (lo < a < lo + rs * n) or a in landed:
                continue
            for d in re.finditer(r'extern\s+([^;]*?\bD_%s\b[^;]*);' % m, t):
                w, arr = decl_width(d.group(1))
                widths[a].add((w, arr))
            widths.setdefault(a, set())
    rows, skipped = [], []
    for a in sorted(widths):
        slot, off = divmod(a - lo, rs)
        ws = {w for w, arr in widths[a] if not arr and w}
        leaves = [l for l in lay['leaves'] if l[0] == off]
        if ws:
            leaves = [l for l in leaves if WIDTH[l[2]] in ws] or leaves
        if not leaves:
            skipped.append((a, 'no leaf at +0x%X' % off))
            continue
        leaf = leaves[0][1]
        fld = SHORT.get(leaf) or (('w%02X' % off) if leaf.startswith('unk') else leaf.replace('.', '_'))
        name = 'cw%d_%s' % (slot, fld)
        if name in used:
            skipped.append((a, 'name taken: ' + name))
            continue
        used.add(name)
        ctype = leaves[0][2]
        rows.append({'addr': '0x%08X' % a, 'decision': 'LAND', 'name': name,
                     'type': 'u32' if ctype == 'ptr' else ctype, 'size': WIDTH[ctype], 'tier': 'T4',
                     'evidence': 'T4: class_w[%d].%s (CLASS_W +0x%X, include/tokimeki/class_w.h): a field cell '
                                 'that keeps its own symbol (volatile access or a separate base register), named '
                                 'in the landed cw<slot>_<field> form; gen_main_cells.py (mechanical, 2026-11-10)'
                                 % (slot, leaf, off)})
    json.dump(rows, open(out, 'w'), indent=1)
    print('%d class_w cells, %d skipped' % (len(rows), len(skipped)))
    for s in skipped:
        print('  SKIP 0x%08X %s' % s)


if __name__ == '__main__':
    main()
