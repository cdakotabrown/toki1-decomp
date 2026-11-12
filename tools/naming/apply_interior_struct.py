#!/usr/bin/env python3
"""Rewrite interior D_<addr> tokens of a landed main STRUCT-ARRAY object as
named field accesses (the struct pass, Phase 3).

    extern s16 D_80127212;  ...  D_80127212 = 4096;
 -> #include "tokimeki/class_w.h" / extern CLASS_W class_w[];  ...  class_w[1].scale_x = 4096;

The layout is a JSON spec: {"host": "0x801271B0", "type": "CLASS_W",
"header": "tokimeki/class_w.h", "size": 68, "count": 160,
"leaves": [[offset, "path", "ctype"], ...]}; ctype is u8/s8/u16/s16/u32/s32/ptr.
Several leaves may share an offset (union views); the one whose width equals
the token's declared width is used.  A leaf named unk* is never used: such
tokens keep their D_ spelling.

Forms (T = the token's declared type):
  scalar, T == leaf type, not volatile    -> host[i].path
  scalar, otherwise                       -> (*(T *)&host[i].path)
  array `T D_X[]`                         -> ((T *)&host[i].path)
  pointer `T *D_X`                        -> (*(T **)&host[i].path)
If the file declares the host with another type, the element is reached as
((TYPE *)host)[i].  Files that define a token, or whose tokens are all
unnamed fields, are left alone.

Overlay records (2026-11-09): a layout with "scope": "<OV>" looks its hosts
up as `<OV>:0xADDR` records and scans src/overlay/<OV>/ only (every overlay
loads at 0x80132000, so an address means nothing outside its overlay).
"hosts": [...] in place of "host" applies one single-record type (count 1)
to several landed records.  With "head_leaf", a host still declared as a
scalar of that leaf's type (`extern s32 etc_ev36_res;`, the head word landed
before the struct existed) is re-declared as the struct (an array of it when
count > 1) and its bare uses become host.<head_leaf> (host[0].<head_leaf>).

usage: apply_interior_struct.py LAYOUT.json (--dry-run|--apply) [--skip-file F ...] [--out R.json]
"""
import argparse
import json
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from apply_interior import (ROOT, SCAN, SKIP_DIRS, SIZES, git, decl_iter, parse_decl,  # noqa: E402
                            remove_declarator, strip_comments)

SIZES = dict(SIZES, ptr=4)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('layout')
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument('--dry-run', action='store_true')
    g.add_argument('--apply', action='store_true')
    ap.add_argument('--skip-file', action='append', default=[])
    ap.add_argument('--out')
    ap.add_argument('--ptr-cast', action='store_true')
    ap.add_argument('--array-cast', action='store_true', help='also rewrite non-volatile array views (T D_X[]) as ((T *)&host[i].field), and whole-record views at a slot start through the slot')
    ap.add_argument('--allow-cast', action='store_true', help='also rewrite a same-width non-volatile scalar of another type through a cast')
    a = ap.parse_args()
    lay = json.load(open(a.layout))
    os.chdir(ROOT)
    syms = json.load(open('config/symbol_names.json'))['symbols']
    scope = lay.get('scope', 'main')
    hosts = lay['hosts'] if 'hosts' in lay else [lay['host']]
    assert len(hosts) == 1 or lay['count'] == 1, 'several hosts need count 1'
    total = {}
    for h in hosts:
        r = run_host(a, lay, syms, scope, h)
        for k, v in r.items():
            total.setdefault(k, {}).update(v)
    if a.out:
        json.dump(total, open(a.out, 'w'), indent=1)


def run_host(a, lay, syms, scope, haddr):
    hkey = '0x%08X' % int(haddr, 16)
    if scope != 'main':
        hkey = '%s:%s' % (scope, hkey)
    rec = syms[hkey]
    assert rec.get('kind') == 'data' and rec.get('landed') and rec.get('scope', 'main') == scope, hkey
    host, hbase, esz, cnt = rec['name'], int(hkey.split(':')[-1], 16), lay['size'], lay['count']
    T, header = lay['type'], lay['header']
    leaves = {}
    for off, path, ct in lay['leaves']:
        leaves.setdefault(off, []).append((path, ct))

    pat = re.compile(r'\bD_(8[0-9A-F]{7})\b')
    scan = SCAN if scope == 'main' else ['src/overlay/%s/' % scope]
    files = [f for f in git('ls-files', *scan).split() if f.endswith(('.c', '.h')) and not f.startswith(SKIP_DIRS)]
    report = {'rewritten': {}, 'skipped': {}}
    head = lay.get('head_leaf')
    head_ct = dict((p, ct) for off, p, ct in lay['leaves'] if off == 0).get(head)
    for f in files:
        text = open(f, errors='surrogateescape').read()
        code = strip_comments(text)
        toks = sorted({int(m.group(1), 16) for m in pat.finditer(code)})
        toks = [t for t in toks if hbase < t < hbase + esz * cnt]
        hd0 = [s for _, _, s in decl_iter(code, host)]
        retype = bool(head and hd0 and {parse_decl(s, host) for s in hd0} == {(head_ct, 'scalar')})
        if not toks and not retype:
            continue
        if f in a.skip_file:
            report['skipped'].setdefault(f, []).append('excluded (--skip-file)')
            continue
        hd = hd0
        hforms = {parse_decl(s, host) for s in hd}
        single = cnt == 1
        if retype:
            base = host if single else '%s[%%d]' % host
        elif single:
            # one record: host.field, declared `extern T host;`
            if not hd or hforms == {(T, 'scalar')}:
                base = host
            elif None not in hforms and len(hforms) == 1:
                base = '(*(%s *)&%s)' % (T, host) if list(hforms)[0][1] == 'scalar' else '(*(%s *)%s)' % (T, host)
            else:
                report['skipped'].setdefault(f, []).append('host declared as %r' % [x.strip() for x in hd])
                continue
        elif not hd:
            base = '%s[%%d]' % host
        elif hforms == {(T, 'array')}:
            base = '%s[%%d]' % host
        elif a.allow_cast and not single and None not in hforms and all(fm == 'scalar' for _, fm in hforms):
            base = '((%s *)&%s)[%%d]' % (T, host)
        elif lay.get('header_declares_host'):
            report['skipped'].setdefault(f, []).append('host declared as %r (the header declares it too)' % [x.strip() for x in hd])
            continue
        elif None not in hforms and all(fm == 'array' for _, fm in hforms):
            base = '((%s *)%s)[%%d]' % (T, host)
        else:
            report['skipped'].setdefault(f, []).append('host declared as %r' % [x.strip() for x in hd])
            continue
        new, done = text, []
        for t in toks:
            tok = 'D_%08X' % t
            slot, rem = divmod(t - hbase, esz)
            decls = list(decl_iter(strip_comments(new), tok))
            if not decls:
                report['skipped'].setdefault(f, []).append('%s: no extern' % tok)
                continue
            forms = {parse_decl(s, tok) for _, _, s in decls}
            if None in forms or len(forms) != 1:
                report['skipped'].setdefault(f, []).append('%s: unparsed declaration' % tok)
                continue
            (qtype, form), = forms
            words = qtype.split()
            vol = 'volatile' in words
            btype = ' '.join(w for w in words if w not in ('volatile', 'const', 'static'))
            dsize = 4 if form == 'pointer' else SIZES.get(btype)
            cands = [(p, ct) for p, ct in leaves.get(rem, []) if not p.split('.')[-1].startswith('unk')]
            pick = [c for c in cands if SIZES[c[1]] == dsize] if form != 'array' else cands[:1]
            slot_el = base if single else base % slot
            if not pick and a.array_cast and rem == 0 and not vol and form in ('scalar', 'array'):
                # a whole-record view at the slot start (a file-local record type)
                path, ct, el = None, None, slot_el
            elif not pick:
                report['skipped'].setdefault(f, []).append('%s: +0x%X has no named %s field' % (tok, rem, btype))
                continue
            else:
                path, ct = pick[0]
                el = '%s.%s' % (slot_el, path)
            # only an exact-type scalar is rewritten: a cast through the field's
            # address (volatile, other signedness, array or pointer views) makes
            # IDO materialise the base address and the code changes
            if path is None:
                expr = ('(*(%s *)&%s)' if form == 'scalar' else '((%s *)&%s)') % (qtype, el)
            elif form == 'array' and not vol and a.array_cast:
                expr = '(&%s)' % el if btype == ct else '((%s *)&%s)' % (qtype, el)
            elif form == 'scalar' and btype == ct and not vol:
                expr = el
            elif form == 'scalar' and not vol and a.allow_cast and dsize == SIZES[ct]:
                expr = '(*(%s *)&%s)' % (qtype, el)
            elif form == 'pointer' and ct == 'ptr' and not vol and a.ptr_cast:
                expr = '(*(%s **)&%s)' % (qtype, el)
            else:
                report['skipped'].setdefault(f, []).append('%s: needs a cast (%s %s vs %s)' % (tok, qtype, form, ct))
                continue
            for s, e, stmt in reversed(decls):
                new = new[:s] + remove_declarator(new[s:e], tok) + new[e:]
            code2 = strip_comments(new)
            out, last = [], 0
            for m in re.finditer(r'\b%s\b' % tok, code2):
                out.append(new[last:m.start()])
                out.append(expr)
                last = m.end()
            out.append(new[last:])
            new = ''.join(out)
            done.append([tok, expr])
        if retype:
            # the head word: `extern s32 host;` -> `extern T host;`, bare uses -> host.head
            decls = list(decl_iter(strip_comments(new), host))
            for s, e, stmt in reversed(decls):
                new = new[:s] + remove_declarator(new[s:e], host) + new[e:]
            code2 = strip_comments(new)
            out, last = [], 0
            hexpr = ('%s.%s' if single else '%s[0].%s') % (host, head)
            for m in re.finditer(r'\b%s\b(?!\s*[.\[])' % host, code2):
                out.append(new[last:m.start()])
                out.append(hexpr)
                last = m.end()
            out.append(new[last:])
            new = ''.join(out)
            done.append([host, hexpr])
            hd = []
        if not done:
            continue
        inc = '#include "%s"\n' % header
        if inc not in new:
            m = re.search(r'(?m)^#include "types\.h"[ \t]*\n', new) or re.search(r'(?m)^#include[^\n]*\n', new)
            pos = m.end() if m else 0
            new = new[:pos] + inc + new[pos:]
        if not hd and not lay.get('header_declares_host'):
            nocpp = re.sub(r'(?m)^#[^\n]*', lambda m: ' ' * len(m.group(0)), strip_comments(new))
            first_use = re.search(r'\b%s\b' % host, nocpp)
            ins = 0
            for m in re.finditer(r'(?m)^(?:#include[^\n]*|[ \t]*extern\b[^;{}()]*;)[ \t]*\n', new):
                if first_use and m.end() > first_use.start():
                    break
                ins = m.end()
            new = new[:ins] + ('extern %s %s;\n' if single else 'extern %s %s[];\n') % (T, host) + new[ins:]
        report['rewritten'][f] = done
        if a.apply:
            open(f, 'w', errors='surrogateescape').write(new)
    n = sum(len(v) for v in report['rewritten'].values())
    u = len({t for v in report['rewritten'].values() for t, _ in v})
    print('%s %s: %d uses of %d tokens rewritten in %d files; %d files with skips' % (
        'APPLIED' if a.apply else 'DRY', host, n, u, len(report['rewritten']), len(report['skipped'])))
    reasons = {}
    for f, v in report['skipped'].items():
        for x in v:
            k = re.sub(r'D_[0-9A-F]{8}', 'D_x', x)
            k = re.sub(r'\+0x[0-9A-F]+', '+off', k)
            reasons[k] = reasons.get(k, 0) + 1
    for k, n in sorted(reasons.items(), key=lambda kv: -kv[1])[:15]:
        print('  SKIP x%d: %s' % (n, k[:160]))
    return report


if __name__ == '__main__':
    main()
