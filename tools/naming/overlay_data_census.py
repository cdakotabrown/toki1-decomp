#!/usr/bin/env python3
"""Census of the open overlay-local data tokens, by class.

Every open overlay-local `D_<addr>` token (addr >= 0x80132000) in a live
overlay's C (src/overlay/<OV>/*.c|*.h; EVENT/GYOZI are never loaded and are
skipped; comments and string literals stripped) is put into exactly one class,
tested in this order (interior, bss, local_init, string, head):

  interior    strictly inside a landed object's [addr, addr+size) (a landed
              `kind: data` record of that overlay in config/symbol_names.json),
              or strictly inside an open head whose extern declares an array
              bound covering it (element size from a basic type only).
  bss         at or past the end of the EXN image (assets/overlays/<OV>.EXN,
              loaded raw at 0x80132000): a runtime buffer, a standalone head.
  string      the EXN bytes at the address are a NUL-terminated string that
              decodes as cp932 with only printable characters (tab/newline
              allowed), at least 2 bytes long (1 under a char/u8/s8 array
              extern), the first word is not a pointer into the image (that
              keeps pointer tables out), and no use subscripts it, takes a
              member of it or does pointer arithmetic on it (a literal is only
              ever passed whole; an indexed object is a table that happens to
              decode, e.g. tile-byte pairs or a 16-byte name table), it is
              used by address (an array extern, or `&D_X` on a scalar one;
              never a pointer-typed extern or a by-value scalar read), and a
              1-2 byte string must sit in a text context (an argument of a
              call whose name matches str/printf/kanji/disp/sndi/basyo/mes/
              string/suuzi/serifu, or a `return` value) -- otherwise it is
              script or table bytes.
  local_init  an extern of a non-basic (struct typedef) type, not a pointer or
              an array, whose every use is a whole-object copy `x = D_X;`: the
              .rodata template IDO emits for a local aggregate initialiser.
              (Tested before `string`: a template whose first bytes happen to
              decode is still a template.)
  head        everything else: a standalone non-string data head candidate.
              An upper bound: `objects` merges runs of heads (next address
              within 8 bytes of the previous head's extent, same user set).

Strings are subclassed for the naming passes:

  format      contains a printf conversion (%d, %s, %x, %5d, %%...)
  dataref     the address also appears as an aligned word in the EXN image
              (a pointer table entry): named by readers, never mechanically
  multi       referenced from more than one function (or from a file-scope
              initialiser)
  single      every reference sits in one function (the mechanical class)
  unused      declared only

`format` wins over the others; `dataref` over `multi`/`single`.

A token's USERS are the functions whose body contains it (a depth-0 brace
scanner: the function is the last `ident(` before the body's `{` with no `=`
in between; a body opened after `=` is a file-scope initialiser, user
`<data:FILE>`).  Order of first use = textual order inside the function.

usage: overlay_data_census.py [--ov OV ...] [--json OUT] [--markdown]
                              [--list CLASS[/SUB]]
Default output: the per-overlay table; --json writes every token's record
(build/naming/overlay_data_census.json by convention) for the generators.
"""
import argparse
import json
import os
import re
import struct
import sys
from collections import Counter

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BASE = 0x80132000
DEAD = ('EVENT', 'GYOZI')
TOKEN = re.compile(r'(?<![A-Za-z0-9_])D_([0-9A-Fa-f]{8})(?![A-Za-z0-9_])')
BASIC_SIZE = {'u8': 1, 's8': 1, 'char': 1, 'u16': 2, 's16': 2, 'short': 2,
              'u32': 4, 's32': 4, 'int': 4, 'long': 4, 'f32': 4, 'float': 4,
              'unsigned': 4, 'signed': 4, 'void': None}
FMT = re.compile(r'%[-+ #0]*(?:\d+|\*)?(?:\.(?:\d+|\*))?[hlL]?[diouxXcspeEfgG%]')


def strip_c(text):
    """comments -> spaces, string/char literal contents -> spaces (keeps offsets)."""
    out = []
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith('/*', i):
            j = text.find('*/', i + 2)
            j = n if j < 0 else j + 2
            out.append(re.sub(r'[^\n]', ' ', text[i:j]))
            i = j
        elif text.startswith('//', i):
            j = text.find('\n', i)
            j = n if j < 0 else j
            out.append(' ' * (j - i))
            i = j
        elif c in '"\'':
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == '\\' else 1
            j = min(j + 1, n)
            out.append(c + ' ' * max(0, j - i - 2) + (c if j - i >= 2 else ''))
            i = j
        else:
            out.append(c)
            i += 1
    return ''.join(out)


def scan_users(text):
    """[(start, end, user)] spans of depth-0 bodies in comment-stripped TEXT."""
    spans = []
    depth = 0
    last_call = None          # the last depth-0 `ident(` (K&R parameter
    eq_since = False          # declarations after it keep it)
    start = None
    user = None
    pdepth = 0
    i, n = 0, len(text)
    ident = re.compile(r'[A-Za-z_][A-Za-z0-9_]*')
    while i < n:
        c = text[i]
        if depth == 0:
            if c == '(':
                if pdepth == 0:
                    k = i - 1
                    while k >= 0 and text[k].isspace():
                        k -= 1
                    e = k + 1
                    while k >= 0 and (text[k].isalnum() or text[k] == '_'):
                        k -= 1
                    name = text[k + 1:e]
                    if name and ident.fullmatch(name) and name not in ('if', 'while', 'for', 'switch', 'sizeof', 'return'):
                        last_call = name
                        eq_since = False
                pdepth += 1
            elif c == ')':
                pdepth = max(0, pdepth - 1)
            elif c == '=' and pdepth == 0:
                eq_since = True
            elif c == '{':
                user = last_call if (last_call and not eq_since) else None
                start = i
                depth = 1
        else:
            if c == '{':
                depth += 1
            elif c == '}':
                depth -= 1
                if depth == 0:
                    spans.append((start, i + 1, user))
                    last_call = None
                    eq_since = False
        i += 1
    return spans


def parse_externs(text):
    """{ADDR: {'type': base type str, 'ptr': bool, 'dims': [..], 'decl': str}}"""
    out = {}
    for m in re.finditer(r'(?m)^\s*extern\s+([^;(]*?);', text):
        body = m.group(1)
        if 'D_' not in body:
            continue
        # base type = leading tokens before the first declarator
        parts = body.split(',')
        head = parts[0]
        tm = re.match(r'\s*((?:[A-Za-z_][A-Za-z0-9_]*\s+)+?)\s*(\**)\s*D_([0-9A-Fa-f]{8})\s*((?:\[[^\]]*\]\s*)*)$', head)
        if not tm:
            continue
        btype = ' '.join(tm.group(1).split())
        decls = [(tm.group(2), tm.group(3), tm.group(4))]
        for p in parts[1:]:
            dm = re.match(r'\s*(\**)\s*D_([0-9A-Fa-f]{8})\s*((?:\[[^\]]*\]\s*)*)$', p)
            if dm:
                decls.append(dm.groups())
        for stars, a, dims in decls:
            dl = [d.strip() for d in re.findall(r'\[([^\]]*)\]', dims)]
            out[int(a, 16)] = {'type': btype, 'ptr': bool(stars), 'dims': dl,
                               'decl': f'{btype} {stars}D_{a.upper()}' + ''.join(f'[{d}]' for d in dl)}
    return out


def core_type(btype):
    toks = [t for t in btype.split() if t not in ('extern', 'volatile', 'const', 'static')]
    if not toks:
        return None
    if 'unsigned' in toks or 'signed' in toks:
        rest = [t for t in toks if t not in ('unsigned', 'signed')]
        return rest[-1] if rest else 'int'
    return toks[-1]


def elem_size(info):
    if info['ptr']:
        return 4
    return BASIC_SIZE.get(core_type(info['type']) or '', None)


def array_extent(info):
    if not info['dims'] or info['ptr']:
        return None
    es = elem_size(info)
    if not es:
        return None
    n = 1
    for d in info['dims']:
        try:
            n *= int(d, 0)
        except ValueError:
            return None
    return n * es


TEXT_CALLEE = re.compile(r'(str|printf|kanji|disp|sndi|basyo|mes|string|suuzi|serifu)', re.I)


def text_context(text, pos):
    """Is the use at POS an argument of a text call (strcpy, printf,
    set_kanji_string, sndisp, ...: callee name matches TEXT_CALLEE) or a
    `return` value?  Decides whether a 1-2 byte string is text."""
    depth = 0
    i = pos - 1
    while i >= 0:
        c = text[i]
        if c == ')':
            depth += 1
        elif c == '(':
            if depth == 0:
                m = re.search(r'([A-Za-z_][A-Za-z0-9_]*)\s*$', text[max(0, i - 64):i])
                if m and TEXT_CALLEE.search(m.group(1)):
                    return True
                if m is None:          # a cast or grouping paren: keep walking out
                    i -= 1
                    continue
                return False
            depth -= 1
        elif c in ';{}':
            return bool(re.match(r'\s*return\b', text[i + 1:pos]))
        i -= 1
    return False


def decode_string(img, off, char_decl):
    if off < 0 or off >= len(img):
        return None
    e = img.find(b'\0', off)
    if e < 0:
        return None
    raw = img[off:e]
    if len(raw) < (1 if char_decl else 2):
        return None
    if off + 4 <= len(img):
        w = struct.unpack_from('<I', img, off)[0]
        if BASE <= w < BASE + len(img):
            return None
    try:
        s = raw.decode('cp932')
    except UnicodeDecodeError:
        return None
    for ch in s:
        if ch in '\n\t':
            continue
        if not ch.isprintable() and ch != '　':
            return None
    return s, len(raw)


def live_overlays():
    d = os.path.join(ROOT, 'src', 'overlay')
    return sorted(o for o in os.listdir(d)
                  if os.path.isdir(os.path.join(d, o)) and o not in DEAD)


def landed_objects(ov):
    doc = json.load(open(os.path.join(ROOT, 'config', 'symbol_names.json')))
    out = []
    for k, e in doc['symbols'].items():
        if e.get('scope') == ov and e.get('kind') == 'data' and e.get('landed') and e.get('size'):
            out.append((int(k.split(':')[1], 16), int(e['size']), e['name']))
    return sorted(out)


def census_overlay(ov):
    d = os.path.join(ROOT, 'src', 'overlay', ov)
    img = open(os.path.join(ROOT, 'assets', 'overlays', f'{ov}.EXN'), 'rb').read()
    end = BASE + len(img)
    datawords = set()
    for off in range(0, len(img) - 3, 4):
        w = struct.unpack_from('<I', img, off)[0]
        if BASE <= w < end:
            datawords.add(w)
    toks = {}
    externs = {}
    for fn in sorted(os.listdir(d)):
        if not fn.endswith(('.c', '.h')):
            continue
        rel = f'src/overlay/{ov}/{fn}'
        raw = open(os.path.join(d, fn), encoding='utf-8', errors='replace').read()
        text = strip_c(raw)
        for a, info in parse_externs(text).items():
            prev = externs.get(a)
            if prev is None or (array_extent(info) or 0) > (array_extent(prev) or 0):
                externs[a] = info
        spans = scan_users(text)
        ext_lines = [(m.start(), m.end()) for m in re.finditer(r'(?m)^\s*extern\b[^;]*;', text)]
        for m in TOKEN.finditer(text):
            a = int(m.group(1), 16)
            if a < BASE:
                continue
            t = toks.setdefault(a, {'addr': a, 'files': set(), 'uses': [], 'users': [],
                                    'copy_only': True, 'nuses': 0, 'indexed': False,
                                    'byvalue': False, 'textctx': False})
            t['files'].add(rel)
            pos = m.start()
            if any(s <= pos < e for s, e in ext_lines):
                continue
            user = None
            for s, e, u in spans:
                if s <= pos < e:
                    user = u if u else f'<data:{fn}>'
                    break
            if user is None:
                user = f'<decl:{fn}>'
            t['nuses'] += 1
            t['uses'].append((rel, pos))
            if user not in t['users']:
                t['users'].append(user)
            # whole-object copy: `= D_X ;`
            before = text[max(0, pos - 40):pos]
            after = text[m.end():m.end() + 8]
            if not (re.search(r'(?<![=!<>])=\s*$', before) and re.match(r'\s*;', after)):
                t['copy_only'] = False
            # subscript, member access or pointer arithmetic: a table, not a
            # literal passed whole
            if re.match(r'\s*(\[|\.|->|\+|-(?!>))', after) or re.search(r'[-+]\s*&?\s*$', before) \
                    or re.search(r'\*\s*\([^()]*\)\s*$', before):
                t['indexed'] = True
            if not re.search(r'&\s*$', before):
                t['byvalue'] = True
            if text_context(text, pos):
                t['textctx'] = True
    landed = landed_objects(ov)
    heads_with_extent = []
    for a, info in externs.items():
        ext = array_extent(info)
        if ext and ext > 1:
            heads_with_extent.append((a, ext))
    recs = []
    for a in sorted(toks):
        t = toks[a]
        info = externs.get(a)
        r = {'ov': ov, 'addr': f'0x{a:08X}', 'token': f'D_{a:08X}', 'files': sorted(t['files']),
             'users': t['users'], 'nuses': t['nuses'], 'decl': info['decl'] if info else None,
             'first': list(t['uses'][0]) if t['uses'] else None}
        host = next(((b, s, n) for b, s, n in landed if b < a < b + s), None)
        if host is None:
            hh = next(((b, s) for b, s in heads_with_extent if b < a < b + s and b in toks), None)
            host = (hh[0], hh[1], f'D_{hh[0]:08X}') if hh else None
        if host:
            r['class'] = 'interior'
            r['host'] = host[2]
        elif a >= end:
            r['class'] = 'bss'
        else:
            char_decl = bool(info and not info['ptr'] and info['dims']
                             and core_type(info['type']) in ('u8', 's8', 'char'))
            st = decode_string(img, a - BASE, char_decl)
            ct = core_type(info['type']) if info else None
            # a literal is used by ADDRESS: an array extern, or `&D_X` on a
            # scalar extern; never through a pointer-typed extern
            by_addr = bool(info is None or (not info['ptr'] and (info['dims'] or not t['byvalue'])))
            if st and st[1] <= 2 and not t['textctx']:
                st = None       # 1-2 byte "strings" outside a text call: data bytes
            is_local_init = bool(info and not info['ptr'] and not info['dims'] and ct
                                 and ct not in BASIC_SIZE and t['nuses'] and t['copy_only'])
            if is_local_init:
                r['class'] = 'local_init'
            elif st and not t['indexed'] and by_addr:
                s, n = st
                r['class'] = 'string'
                r['text'] = s
                r['len'] = n
                real = [u for u in t['users'] if not u.startswith('<decl:')]
                if FMT.search(s):
                    r['sub'] = 'format'
                elif a in datawords:
                    r['sub'] = 'dataref'
                elif not real:
                    r['sub'] = 'unused'
                elif len(real) == 1 and not real[0].startswith('<'):
                    r['sub'] = 'single'
                else:
                    r['sub'] = 'multi'
            else:
                r['class'] = 'head'
                r['ext'] = (array_extent(info) if info else None) or 4
        recs.append(r)
    # objects estimate for heads (+ bss)
    hs = [r for r in recs if r['class'] in ('head', 'bss')]
    objs = 0
    prev = None
    for r in hs:
        a = int(r['addr'], 16)
        if prev and a - (int(prev['addr'], 16) + prev.get('ext', 4)) <= 8 and \
                set(prev['users']) == set(r['users']) and prev['class'] == r['class']:
            prev = r
            continue
        objs += 1
        prev = r
    return recs, objs


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--ov', action='append')
    ap.add_argument('--json')
    ap.add_argument('--markdown', action='store_true')
    ap.add_argument('--list', help='print the tokens of CLASS or CLASS/SUB')
    a = ap.parse_args()
    ovs = [o.upper() for o in a.ov] if a.ov else live_overlays()
    allrecs = []
    rows = []
    tot = Counter()
    for ov in ovs:
        recs, objs = census_overlay(ov)
        allrecs += recs
        c = Counter(r['class'] for r in recs)
        sc = Counter(r.get('sub') for r in recs if r['class'] == 'string')
        row = {'ov': ov, 'open': len(recs), 'head': c['head'], 'objects': objs, 'bss': c['bss'],
               'string': c['string'], 'single': sc['single'], 'multi': sc['multi'],
               'format': sc['format'], 'dataref': sc['dataref'], 'unused': sc['unused'],
               'local_init': c['local_init'], 'interior': c['interior'],
               'refs': sum(r['nuses'] for r in recs)}
        rows.append(row)
        for k, v in row.items():
            if k != 'ov':
                tot[k] += v
    rows.sort(key=lambda r: -r['open'])
    cols = ['open', 'head', 'objects', 'bss', 'string', 'single', 'multi', 'format', 'dataref',
            'unused', 'local_init', 'interior']
    if a.list:
        cl, _, sub = a.list.partition('/')
        for r in allrecs:
            if r['class'] == cl and (not sub or r.get('sub') == sub):
                print(r['ov'], r['addr'], r.get('sub', ''), ','.join(r['users']),
                      repr(r.get('text', ''))[:80])
    elif a.markdown:
        print('| overlay | ' + ' | '.join(cols) + ' |')
        print('|---|' + '---:|' * len(cols))
        for r in rows:
            print(f"| {r['ov']} | " + ' | '.join(f'{r[c]:,}' for c in cols) + ' |')
        print('| **total** | ' + ' | '.join(f'**{tot[c]:,}**' for c in cols) + ' |')
    else:
        print(f"{'overlay':10s} " + ' '.join(f'{c[:8]:>8s}' for c in cols))
        for r in rows:
            print(f"{r['ov']:10s} " + ' '.join(f'{r[c]:8d}' for c in cols))
        print(f"{'total':10s} " + ' '.join(f'{tot[c]:8d}' for c in cols))
        print(f"references (non-extern uses): {tot['refs']}")
    if a.json:
        os.makedirs(os.path.dirname(os.path.abspath(a.json)), exist_ok=True)
        json.dump(allrecs, open(a.json, 'w'), ensure_ascii=False, indent=1)
    return 0


if __name__ == '__main__':
    sys.exit(main())
