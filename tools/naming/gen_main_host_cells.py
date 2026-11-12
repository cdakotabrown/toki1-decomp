#!/usr/bin/env python3
"""Name the remaining interior cells of landed MAIN Konami hosts (decision rows).

The main-binary sibling of gen_cell_names.py.  A `D_` token that lies inside a
landed main data record (address + size from config/symbol_names.json) and
was left a separate symbol by the struct/array passes (volatile access, or a
function where separate symbols stop IDO sharing a base register) is named
for its host and position:

  integer array host `T host[N]` (or [N][M])  -> host_<i> (host_<i>_<j>)
  host with a layout (tools/naming/layouts/)  -> host[_<slot>]_<leaf>
                                                  (array leaf title[0] at +k
                                                   -> host_title<k/width>)

Hosts that are Sony library data (`_svm*`, `gSD*`, `PSDOFS*`, `CF2_*`) are
skipped (naming covers Konami data only), and so is class_w, which
gen_main_cells.py names in its cw<slot>_<field> form.  The view at an offset
is chosen by the token's declared width; a token whose width disagrees with
the element is reported and left alone.

usage: gen_main_host_cells.py OUT.json
"""
import bisect
import collections
import glob
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
if not os.path.exists(os.path.join(ROOT, 'config/symbol_names.json')):
    ROOT = os.getcwd()
sys.path.insert(0, os.path.join(ROOT, 'tools', 'naming'))
from apply_interior import strip_comments  # noqa: E402

WIDTH = {'u8': 1, 's8': 1, 'char': 1, 'u16': 2, 's16': 2, 'short': 2, 'u32': 4, 's32': 4, 'int': 4,
         'long': 4, 'ptr': 4}
SONY = re.compile(r'^(_|gSD|PSDOFS|CF2_)')
SKIP_HOSTS = {'class_w'}


def decl_width(stmt):
    if '*' in stmt:
        return 4, False
    d = re.sub(r'\b(volatile|const|extern|static)\b', ' ', stmt).split()
    t = d[0] if d else ''
    if t in ('unsigned', 'signed') and len(d) > 2:
        t = ('u' if t == 'unsigned' else 's') + {'char': '8', 'short': '16', 'int': '32', 'long': '32'}.get(d[1], '')
    return WIDTH.get(t), '[' in stmt


def main():
    out = sys.argv[1]
    os.chdir(ROOT)
    syms = json.load(open('config/symbol_names.json'))['symbols']
    used = {v.get('name') for v in syms.values()}
    hosts = {}
    for k, v in syms.items():
        if ':' in k or not v.get('landed') or v.get('kind') != 'data' or not v.get('size'):
            continue
        hosts[int(k, 16)] = v
    starts = sorted(hosts)
    lays = {}
    for f in glob.glob('tools/naming/layouts/*.json'):
        lay = json.load(open(f))
        if lay.get('scope') in (None, 'main'):
            for h in lay.get('hosts') or [lay.get('host')]:
                lays[int(h, 16)] = lay
    widths = collections.defaultdict(set)
    files = glob.glob('src/main/matched/*.c') + [g for g in glob.glob('src/overlay/*/*.c')
                                                  if '/EVENT/' not in g and '/GYOZI/' not in g]
    for f in files:
        t = strip_comments(open(f, errors='replace').read())
        for m in set(re.findall(r'\bD_(80[01][0-9A-Fa-f]{5})\b', t)):
            a = int(m, 16)
            if a in hosts:
                continue
            for d in re.finditer(r'extern\s+([^;]*?\bD_%s\b[^;]*);' % m, t):
                widths[a].add(decl_width(d.group(1)))
            widths.setdefault(a, set())
    rows, skipped = [], []
    for a in sorted(widths):
        i = bisect.bisect_right(starts, a) - 1
        if i < 0:
            continue
        h = starts[i]
        rec = hosts[h]
        hname, ht = rec['name'], (rec.get('type') or '').replace(' ', '')
        off = a - h
        if off >= rec['size'] or hname in SKIP_HOSTS or SONY.match(hname):
            continue
        ws = {w for w, arr in widths[a] if w and not arr}
        anyarr = any(arr for w, arr in widths[a]) or not widths[a]
        if len(ws) > 1:
            skipped.append((a, hname, 'mixed widths %s' % sorted(ws)))
            continue
        w = next(iter(ws)) if ws else None
        m = re.fullmatch(r'(s32|u32|s16|u16|u8|s8|int)((?:\[\d+\])+)', ht)
        name = ctype = None
        if m:
            ctype = {'int': 's32'}.get(m.group(1), m.group(1))
            ew = WIDTH[ctype]
            if (w is not None and w != ew) or off % ew:
                skipped.append((a, hname, 'width %s vs element %d at +%d' % (w, ew, off)))
                continue
            dims = [int(x) for x in re.findall(r'\[(\d+)\]', m.group(2))]
            k = off // ew
            idx = []
            for d in reversed(dims[1:]):
                idx.append(k % d)
                k //= d
            idx.append(k)
            name = hname + ''.join('_%d' % x for x in reversed(idx))
        elif h in lays:
            lay = lays[h]
            rs, n = lay['size'], lay.get('count', 1)
            slot, r = divmod(off, rs)
            leaves = sorted(lay['leaves'])
            cand = []
            for j, (lo, path, t) in enumerate(leaves):
                lw = WIDTH.get('u32' if t == 'ptr' else t, 4)
                if '[' in path and path.endswith('[0]'):
                    hi = leaves[j + 1][0] if j + 1 < len(leaves) else rs
                    if lo <= r < hi and (r - lo) % lw == 0:
                        cand.append((path[:-3] + '[%d]' % ((r - lo) // lw), t, lw))
                elif lo == r:
                    cand.append((path, t, lw))
            if w is not None:
                cand = [c for c in cand if c[2] == w]
            cand = [c for c in cand if not c[0].split('.')[-1].startswith('unk')] or \
                [('w%02X' % r, c[1], c[2]) for c in cand]
            if not cand:
                skipped.append((a, hname, 'no leaf at +0x%X (slot %d) width %s' % (r, slot, w)))
                continue
            path, ctype, _ = cand[0]
            path = re.sub(r'((?:\[\d+\])+)', lambda q: '_'.join(re.findall(r'\d+', q.group(1))), path).replace('.', '_')
            name = hname + ('_%d' % slot if n > 1 else '') + '_' + path
            ctype = 'u32' if ctype == 'ptr' else ctype
        else:
            skipped.append((a, hname, 'host type %s, no layout' % (ht or '?')))
            continue
        if name in used:
            skipped.append((a, hname, 'name taken: ' + name))
            continue
        used.add(name)
        rows.append({'addr': '0x%08X' % a, 'decision': 'LAND', 'name': name, 'type': ctype,
                     'size': WIDTH[ctype], 'tier': 'T4',
                     'evidence': 'T4: cell %s of %s (%s at 0x%08X, +0x%X): a cell that keeps its own symbol '
                                 '(volatile access or a separate base register), '
                                 'named for its host and position; gen_main_host_cells.py (mechanical, 2026-11-10)'
                                 % (name[len(hname) + 1:], hname, ht or '?', h, off)})
    json.dump(rows, open(out, 'w'), indent=1)
    print('%d main host cells, %d skipped' % (len(rows), len(skipped)))
    for s in skipped:
        print('  SKIP 0x%08X %s: %s' % s)


if __name__ == '__main__':
    main()
