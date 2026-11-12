#!/usr/bin/env python3
"""Rewrite interior D_<addr> tokens of a landed main ARRAY object as elements.

A landed main data object `host` (scalar array, e.g. `s32 tpage_buf[64]`) is
still reached in the C through separate externs `D_<addr>` for some of its
elements.  This tool rewrites each such token as the element expression and
drops the interior extern:

    extern s32 D_800EFA78;  ...  D_800EFA78 = x;
 -> extern s32 tpage_buf[];  ...  tpage_buf[60] = x;

Forms (T = the token's declared type, E = the host element type):
  scalar, T == E, not volatile     -> host[i]
  scalar, otherwise                -> (*(T *)&host[i])      (same address, same access)
  array  `T D_X[]`, T == E         -> (&host[i])
  array, otherwise                 -> ((T *)&host[i])
  pointer `T *D_X`                 -> (*(T **)&host[i])
Each keeps the access the old extern described (width, signedness, volatile),
so the compiled code is the same; the gates prove it.

A file is skipped (reported) when it defines the token (not extern), declares
the host with an incompatible type, or the token is not element-aligned.

Overlay objects (2026-11-09): --ov OV looks the host up as `OV:0xADDR` and
scans src/overlay/OV/ only.  There a pointer view (`T *D_X`) is refused: the
PC port is 64-bit, and `(*(T **)&host[i])` would read 8 bytes of a 4-byte
PS1 cell.

usage: apply_interior.py HOST_ADDR ELEM_TYPE DIMS [--ov OV] [--dry-run|--apply] [--files ...]
   e.g. apply_interior.py 0x800EF988 s32 64 --apply
        apply_interior.py 0x801308F0 s16 36,4 --apply
"""
import argparse
import json
import os
import re
import subprocess
import sys

ROOT = os.environ.get('TOKI_ROOT', os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
SIZES = {'u8': 1, 's8': 1, 'char': 1, 'u16': 2, 's16': 2, 'u32': 4, 's32': 4, 'int': 4}
SCAN = ['src/main/matched', 'src/overlay']
SKIP_DIRS = ('src/overlay/EVENT/', 'src/overlay/GYOZI/')


def git(*a):
    return subprocess.run(['git', '-C', ROOT] + list(a), capture_output=True, text=True).stdout


def decl_iter(text, tok):
    """yield (start, end, stmt) of each file-scope `extern ...;` statement naming tok"""
    # statements at line start, and further ones sharing a line (`extern a; extern b;`)
    for m in re.finditer(r'(?m)(?:^|(?<=;))[ \t]*extern\b[^;{}()]*;(?:[ \t]*\n)?', text):
        if re.search(r'\b%s\b' % tok, m.group(0)):
            yield m.start(), m.end(), m.group(0)


def parse_decl(stmt, tok):
    """(qual_type, form) of tok in an extern statement; form scalar|array|pointer"""
    body = stmt.strip()[len('extern'):].rstrip(';').strip()
    m = re.match(r'((?:(?:volatile|const|unsigned|signed|static)\s+)*\w+)\s+(.*)$', body, re.S)
    if not m:
        return None
    base, decls = m.group(1), m.group(2)
    for d in [x.strip() for x in decls.split(',')]:
        if not re.search(r'\b%s\b' % tok, d):
            continue
        if re.fullmatch(r'%s' % tok, d):
            return base, 'scalar'
        if re.fullmatch(r'%s\s*\[[^\]]*\]' % tok, d):
            return base, 'array'
        if re.fullmatch(r'\*\s*%s' % tok, d):
            return base, 'pointer'
        return None
    return None


def remove_declarator(stmt, tok):
    head = re.match(r'(\s*extern\s+(?:(?:volatile|const|unsigned|signed)\s+)*\w+\s+)', stmt).group(1)
    rest = stmt[len(head):].rstrip()
    assert rest.endswith(';')
    parts = [x.strip() for x in rest[:-1].split(',')]
    keep = [p for p in parts if not re.search(r'\b%s\b' % tok, p)]
    if not keep:
        return ''
    return head + ', '.join(keep) + ';\n'


def strip_comments(t):
    return re.sub(r'/\*.*?\*/', lambda m: ' ' * len(m.group(0)), t, flags=re.S)


def index_expr(off, esize, dims):
    if off % esize:
        return None
    n = off // esize
    idx = []
    for d in reversed(dims[1:]):
        idx.append(n % d)
        n //= d
    idx.append(n)
    if idx[-1] >= dims[0]:
        return None
    return ''.join('[%d]' % i for i in reversed(idx))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('host')
    ap.add_argument('elem')
    ap.add_argument('dims')
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument('--dry-run', action='store_true')
    g.add_argument('--apply', action='store_true')
    ap.add_argument('--only', action='append', default=[], help='interior address (repeatable)')
    ap.add_argument('--out', help='write a JSON report here')
    ap.add_argument('--skip-file', action='append', default=[], help='leave this file alone (its codegen changes)')
    ap.add_argument('--no-volatile', action='store_true', help='leave volatile tokens alone (a volatile cast changes IDO codegen)')
    ap.add_argument('--ov', help='overlay scope of the host record')
    a = ap.parse_args()
    os.chdir(ROOT)
    syms = json.load(open('config/symbol_names.json'))['symbols']
    hkey = '0x%08X' % int(a.host, 16)
    if a.ov:
        hkey = '%s:%s' % (a.ov, hkey)
    rec = syms[hkey]
    assert rec.get('kind') == 'data' and rec.get('landed') and rec.get('scope', 'main') == (a.ov or 'main'), hkey
    host = rec['name']
    hbase = int(hkey.split(':')[-1], 16)
    esize = SIZES[a.elem]
    dims = [int(x) for x in a.dims.split(',')]
    total = esize
    for d in dims:
        total *= d
    hdecl = '%s %s%s' % (a.elem, host, '[]' + ''.join('[%d]' % d for d in dims[1:]))

    pat = re.compile(r'\bD_(8[0-9A-F]{7})\b')
    scan = ['src/overlay/%s/' % a.ov] if a.ov else SCAN
    files = [f for f in git('ls-files', *scan).split() if f.endswith(('.c', '.h')) and not f.startswith(SKIP_DIRS)]
    report = {'host': host, 'addr': hkey, 'decl': hdecl, 'rewritten': {}, 'skipped': {}}
    only = {int(x, 16) for x in a.only}
    for f in files:
        if f in a.skip_file:
            report['skipped'].setdefault(f, []).append('excluded (--skip-file)')
            continue
        text = open(f, errors='surrogateescape').read()
        code = strip_comments(text)
        toks = sorted({int(m.group(1), 16) for m in pat.finditer(code)})
        toks = [t for t in toks if hbase < t < hbase + total and (not only or t in only)]
        if not toks:
            continue
        if a.ov and re.search(r'(?m)^(?!\s*extern\b)[^;{}()\n]*\b%s\s*\[[^\]]*\]\s*=' % host, code):
            # the file defines the host (same-TU initialised definition): an
            # added extern next to it breaks the PC module generator
            report['skipped'].setdefault(f, []).append('defines the host (kept)')
            continue
        if a.no_volatile and any('volatile' in s.split('=')[0] for _, _, s in decl_iter(code, host)):
            report['skipped'].setdefault(f, []).append('host declared volatile (kept)')
            continue
        new = text
        done = []
        for t in toks:
            tok = 'D_%08X' % t
            decls = list(decl_iter(strip_comments(new), tok))
            if not decls:
                report['skipped'].setdefault(f, []).append('%s: no extern (defined or macro)' % tok)
                continue
            forms = {parse_decl(s, tok) for _, _, s in decls}
            if None in forms or len(forms) != 1:
                report['skipped'].setdefault(f, []).append('%s: unparsed declaration %r' % (tok, [s.strip() for _, _, s in decls]))
                continue
            (qtype, form), = forms
            if a.ov and form == 'pointer':
                report['skipped'].setdefault(f, []).append('%s: pointer view (PC cell width)' % tok)
                continue
            words = qtype.split()
            vol = 'volatile' in words
            if vol and a.no_volatile:
                report['skipped'].setdefault(f, []).append('%s: volatile (kept)' % tok)
                continue
            btype = [w for w in words if w not in ('volatile', 'const', 'static')]
            btype = ' '.join(btype)
            if a.ov and form == 'scalar' and (btype not in SIZES or t + SIZES[btype] > hbase + total):
                # a record type or a view that runs past the host: not an element
                report['skipped'].setdefault(f, []).append('%s: %s does not fit inside the host' % (tok, btype))
                continue
            ix = index_expr(t - hbase, esize, dims)
            if ix is None:
                report['skipped'].setdefault(f, []).append('%s: not element-aligned' % tok)
                continue
            el = '%s%s' % (host, ix)
            if form == 'scalar':
                expr = el if (btype == a.elem and not vol) else '(*(%s *)&%s)' % (qtype, el)
            elif form == 'array':
                expr = '(&%s)' % el if (btype == a.elem and not vol) else '((%s *)&%s)' % (qtype, el)
            else:
                expr = '(*(%s **)&%s)' % (qtype, el)
            # drop the declarations (from the end so offsets hold), then rewrite uses
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
        if not done:
            continue
        # the host declaration
        hd = [stmt for _, _, stmt in decl_iter(strip_comments(new), host)]
        if hd:
            forms = {parse_decl(s, host) for s in hd}
            if forms == {(a.elem, 'array')} and dims[1:] == []:
                pass
            elif len(forms) == 1 and None not in forms and list(forms)[0][1] == 'scalar':
                # a scalar view of element 0: make the declaration the array and
                # spell the old uses as element 0 with the access they had
                qtype = list(forms)[0][0]
                words = qtype.split()
                btype = ' '.join(w for w in words if w not in ('volatile', 'const', 'static'))
                el0 = host + '[0]' * len(dims)
                expr = el0 if (btype == a.elem and 'volatile' not in words) else '(*(%s *)&%s)' % (qtype, el0)
                for s, e, stmt in reversed(list(decl_iter(strip_comments(new), host))):
                    rest = remove_declarator(new[s:e], host)
                    new = new[:s] + rest + 'extern %s;\n' % hdecl + new[e:]
                code2 = strip_comments(new)
                decl_spans = [(s, e) for s, e, _ in decl_iter(code2, host)]
                out, last = [], 0
                # skip the element expressions this run just wrote (host[...]):
                # the old scalar uses are never subscripted
                for m in re.finditer(r'\b%s\b(?!\s*\[)' % host, code2):
                    if any(s <= m.start() < e for s, e in decl_spans):
                        continue
                    out.append(new[last:m.start()])
                    out.append(expr)
                    last = m.end()
                out.append(new[last:])
                new = ''.join(out)
                done.append([host, expr])
            else:
                report['skipped'].setdefault(f, []).append('host declared incompatibly: %r' % [s.strip() for s in hd])
                continue
        elif f.endswith('.c') or True:
            # after the last file-scope extern / include line before first use
            first_use = re.search(r'\b%s\b' % host, strip_comments(new))
            ins = 0
            for m in re.finditer(r'(?m)^(?:#include[^\n]*|[ \t]*extern\b[^;{}()]*;)[ \t]*\n', new):
                if first_use and m.end() > first_use.start():
                    break
                ins = m.end()
            new = new[:ins] + 'extern %s;\n' % hdecl + new[ins:]
        report['rewritten'][f] = done
        if a.apply:
            open(f, 'w', errors='surrogateescape').write(new)
    if a.out:
        json.dump(report, open(a.out, 'w'), indent=1)
    n = sum(len(v) for v in report['rewritten'].values())
    print('%s %s: %d tokens rewritten in %d files; %d files with skips' % (
        'APPLIED' if a.apply else 'DRY', host, n, len(report['rewritten']), len(report['skipped'])))
    for f, v in report['skipped'].items():
        for x in v:
            print('  SKIP %s: %s' % (f, x))


if __name__ == '__main__':
    main()
