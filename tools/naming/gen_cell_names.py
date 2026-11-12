#!/usr/bin/env python3
"""Name the interior cells that must stay separate symbols (decision rows).

A volatile access to a table cell cannot become `host[i]`: IDO shares one
base register across a host's elements (base CSE), so the cell keeps its own
symbol.  It can still have a
real NAME: the host's name plus the cell's position.

  integer array host  `T host[N]`      -> host_<i>        (host_<i>_<j> for 2-D)
                                           a narrower view at an element's
                                           start is that element; a 16-bit
                                           view of its upper half is
                                           host_<i>_hi (a scalar host: host_hi)
  array view of a row of a 2-D table    -> host_<row>
  inline struct host `struct {..}[N]`   -> host_<i>_<field>
  struct host with a layout leaf        -> host_<leaf>     (tile[3] -> host_tile3)
  struct-typed view of a whole element  -> host_<i>

Input: the census (tools/naming/overlay_data_census.py --json) and the
ledger.  Every `interior` token of OV whose host is a landed data record of
OV is a candidate; --volatile-only (default) keeps the tokens every one of
whose declarations is volatile (the class the passes cannot rewrite).  The
row's type is the token's declared base type; size is that width.

Output: a decision list for record_overlay_data.py / land_overlay_data.sh
(tier T4, style derived; the evidence cites the host and the cell).

usage: gen_cell_names.py CENSUS.json OV OUT.json [--all]
"""
import glob
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
if not os.path.exists(os.path.join(ROOT, 'config/symbol_names.json')):
    ROOT = os.getcwd()
SIZES = {'ptr': 4, 'u8': 1, 's8': 1, 'char': 1, 'u16': 2, 's16': 2, 'short': 2, 'u32': 4, 's32': 4,
         'int': 4, 'long': 4, 'unsigned': 4}


# the 11-word object resource block (include/tokimeki/etc_ev_res.h ETC_EV_RES;
# DATE's DATE_OBJ_RES and the s32[11] *_res hosts have the same layout)
RES44 = {0: ('hdr', 4), 4: ('cels', 4), 8: ('anim_tbl', 4), 12: ('scale', 2), 16: ('pal', 4),
         20: ('bg0', 4), 24: ('bg1', 4), 28: ('bg2', 4), 32: ('bg3', 4), 36: ('bg4', 4), 40: ('bg5', 4)}


# other ETC/DATE resource records (include/tokimeki/etc_ev_res.h)
_B6 = [('bg%d' % i, 4) for i in range(6)]
TYPE_LEAVES = {
    'ETC_EV_RES': RES44,
    'DATE_OBJ_RES': RES44,
    'ETC_EV_RES_BG': dict(zip(range(0, 28, 4), [('pal', 4)] + _B6)),
    'ETC_EV_RES_OBJ': {k: v for k, v in RES44.items() if k <= 16},
    'ETC_EV_RES_BLK': dict(list({k: v for k, v in RES44.items() if k <= 16}.items())
                           + [(20, ('blk', 4))] + list(zip(range(24, 48, 4), _B6)) + [(48, ('blk_prev', 4))]),
}


def base_type(decl):
    """'volatile s16 D_X' -> ('s16', 1 if scalar else None)."""
    d = re.sub(r'\b(volatile|const|extern|static)\b', ' ', decl or '').split()
    if not d:
        return None
    t = d[0]
    if t in ('unsigned', 'signed') and len(d) > 1 and d[1] in ('char', 'short', 'int', 'long'):
        t = {'unsignedchar': 'u8', 'unsignedshort': 'u16', 'unsignedint': 'u32', 'unsignedlong': 'u32',
             'signedchar': 's8', 'signedshort': 's16', 'signedint': 's32', 'signedlong': 's32'}[t + d[1]]
    if '*' in decl:
        return 'ptr'
    if '[' in decl:
        return ('arr', t) if t in SIZES else None
    return t if t in SIZES else None


def inline_struct(ht):
    """'struct {u8 *imp; u8 *kao;}[20]' -> ([(off, field, width)], size, count),
    for word-sized or naturally aligned basic members only."""
    m = re.fullmatch(r'struct\{(.*)\}\[(\d+)\]', ht)
    if not m:
        return None
    out, off = [], 0
    for f in [x.strip() for x in m.group(1).split(';') if x.strip()]:
        fm = re.fullmatch(r'([A-Za-z_][A-Za-z0-9_]*)\s*(\*?)\s*([A-Za-z_][A-Za-z0-9_]*)', f.replace(' *', '*').replace('* ', '*').replace('*', ' * ').strip())
        if not fm:
            return None
        w = 4 if fm.group(2) else SIZES.get(fm.group(1))
        if not w:
            return None
        off = (off + w - 1) // w * w
        out.append((off, fm.group(3), w))
        off += w
    size = (off + 3) // 4 * 4
    return out, size, int(m.group(2))


def layouts(ov):
    out = {}
    for f in glob.glob(os.path.join(ROOT, 'tools/naming/layouts/*.json')) + glob.glob(os.path.join(os.environ.get('CELL_LAYOUTS', '/nonexistent'), '*.json')):
        lay = json.load(open(f))
        if lay.get('scope') != ov:
            continue
        for h in lay.get('hosts') or [lay.get('host')]:
            out[int(h, 16)] = lay
    return out


def main():
    census, ov, out = sys.argv[1], sys.argv[2], sys.argv[3]
    allv = '--all' in sys.argv
    rows = json.load(open(census))
    syms = json.load(open(os.path.join(ROOT, 'config/symbol_names.json')))['symbols']
    hosts = {}
    for k, v in syms.items():
        if k.startswith(ov + ':') and v.get('kind') == 'data' and v.get('landed'):
            hosts[v['name']] = (int(k.split(':')[1], 16), v)
    lays = layouts(ov)
    used = {v.get('name') for v in syms.values()}
    dec, skipped = [], []
    for r in rows:
        if r['ov'] != ov or r['class'] != 'interior' or r.get('host') not in hosts:
            continue
        decls = r.get('decls') or [r.get('decl')]
        if not allv and not all('volatile' in (d or '') for d in decls):
            continue
        t = base_type(r.get('decl'))
        view = None
        if t is None:
            mv = re.match(r'\s*(?:volatile\s+)?([A-Za-z_][A-Za-z0-9_]*)\s+D_', r.get('decl') or '')
            if not mv or '[' in (r.get('decl') or ''):
                skipped.append((r['addr'], 'type', r.get('decl')))
                continue
            view = mv.group(1)          # a struct-typed view of one element
        arr = isinstance(t, tuple)
        if arr:
            t = t[1]
        a = int(r['addr'], 16)
        hname = r['host']
        h, rec = hosts[hname]
        off = a - h
        w = SIZES[t] if t else None
        ht = (rec.get('type') or '').replace(' ', '')
        name = None
        ctype = {'ptr': 'u32', 'int': 's32', 'char': 'u8', 'long': 's32', 'short': 's16', 'unsigned': 'u32'}.get(t, t)
        csize = w
        m = re.fullmatch(r'(s32|u32|s16|u16|u8|s8|int|char)((?:\[\d+\])*)', ht)
        mp = re.fullmatch(r'[A-Za-z_][A-Za-z0-9_ ]*\*((?:\[\d+\])+)', ht)
        ist = inline_struct(ht)
        leaves = TYPE_LEAVES.get(ht) or (RES44 if ht == 's32[11]' and hname.endswith('_res') else None)
        if leaves is not None and off in leaves and (arr or leaves[off][1] == w):
            name = hname + '_' + leaves[off][0]
        elif (m or mp) and view is None:
            ew = 4 if mp else SIZES[{'int': 's32', 'char': 'u8'}.get(m.group(1), m.group(1))]
            dims = [int(x) for x in re.findall(r'\[(\d+)\]', (mp or m).group(1 if mp else 2))]
            sub = off % ew
            hi = ''
            if not arr and w != ew:
                if w < ew and sub == 0:
                    pass                # a narrow view of the element itself
                elif w == 2 and ew == 4 and sub == 2:
                    hi = '_hi'          # the upper half of the element
                else:
                    skipped.append((r['addr'], 'width %d vs element %d at +%d' % (w, ew, off), ht))
                    continue
            elif sub:
                skipped.append((r['addr'], 'misaligned +%d in %d-byte elements' % (off, ew), ht))
                continue
            i = off // ew
            idx = []
            for d in reversed(dims[1:]):
                idx.append(i % d)
                i //= d
            idx.append(i)
            idx = list(reversed(idx))
            if not dims:
                idx = []                # a scalar host
            if arr and len(dims) > 1:
                while len(idx) > 1 and idx[-1] == 0:
                    idx.pop()           # an array view of a whole row
            name = hname + ''.join('_%d' % x for x in idx) + hi
            if not name.startswith(hname + '_'):
                skipped.append((r['addr'], 'host itself', ht))
                continue
        elif ist:
            fields, rs, n = ist
            fl = [f for f in fields if f[0] == off % rs and (arr or f[2] == w)]
            if not fl:
                skipped.append((r['addr'], 'no member', ht))
                continue
            name = '%s_%d_%s' % (hname, off // rs, fl[0][1])
        elif h in lays:
            lay = lays[h]
            rs = lay['size']
            n = lay.get('count', 1)
            if view is not None:
                if off % rs or n == 1:
                    skipped.append((r['addr'], 'struct view %s off element start' % view, ht))
                    continue
                name = '%s_%d' % (hname, off // rs)
                ctype, csize = lay['type'], rs
            else:
                leaf = [l for l in lay['leaves'] if l[0] == off % rs and (arr or SIZES.get(
                    {'ptr': 'u32'}.get(l[2], l[2])) == w) and not l[1].startswith('unk')]
                if not leaf:
                    skipped.append((r['addr'], 'no leaf', ht))
                    continue
                path = re.sub(r'\[(\d+)\]', r'\1', leaf[0][1]).replace('.', '_')
                name = hname + ('_%d' % (off // rs) if n > 1 else '') + '_' + path
        else:
            skipped.append((r['addr'], 'host type', ht))
            continue
        if name in used:
            skipped.append((r['addr'], 'name taken', name))
            continue
        used.add(name)
        dec.append({'id': '%s:%s' % (ov, r['addr'].upper().replace('0X', '0x')), 'decision': 'LAND', 'name': name,
                    'size': csize, 'type': ctype, 'tier': 'T4', 'style': 'derived', 'confidence': 'DERIVED',
                    'evidence': 'T4: cell %s of %s (%s at %s, +0x%X): the cell keeps its own symbol '
                                '(a volatile access, or IDO shares a base register across host[i]), '
                                'so it is named for its host and position.'
                                % (name[len(hname) + 1:], hname, ht, '0x%08X' % h, off),
                    'note': 'gen_cell_names.py (mechanical, 2026-11-10)'})
    json.dump(dec, open(out, 'w'), indent=1)
    print('%s: %d cell names, %d skipped' % (ov, len(dec), len(skipped)))
    for s in skipped:
        print('  SKIP', *s)


if __name__ == '__main__':
    main()
