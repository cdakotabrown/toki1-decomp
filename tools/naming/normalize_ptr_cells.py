#!/usr/bin/env python3
"""Re-declare `void *` address cells of struct-layout records as s32.

A record field that holds a PS1 address is declared `extern s32 D_X;` by
the files that read it and `extern void * D_X;` by the init that fills it
with `D_X = (void *)0x801B2000;`.  The struct pass (apply_interior_struct.py)
only rewrites a token whose declared type equals the field's, so the init
files keep their D_ spellings.  This tool makes the init files say what the
readers say: the cell is a 32-bit integer.

    extern void * D_8014EA94;          ->  extern s32 D_8014EA94;
    D_8014EA94 = (void *)0x801CE0FC;   ->  D_8014EA94 = 0x801CE0FC;

A constant store of the same 32-bit value: the compiled code is the same
(the landing gates prove it), and on the PC port the cell becomes a plain
4-byte integer instead of a pointer squeezed into a PS1 cell.

Only tokens inside a layout's hosts (heads included), only in the layout's
overlay, and only where EVERY use in the file is such a constant store.
Anything else is refused and the file is left alone.

Range mode (2026-11-09): --ov OV --range 0xADDR SIZE takes every token in
[ADDR, ADDR+SIZE) of that overlay plus the landed head's own name, for a
pointer table typed as an s32 array (the void *[3][13] serifu banks).

usage: normalize_ptr_cells.py LAYOUT.json [...] (--dry-run|--apply)
       normalize_ptr_cells.py --ov OV --range 0xADDR SIZE (--dry-run|--apply)
"""
import argparse
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('layouts', nargs='*')
    ap.add_argument('--ov')
    ap.add_argument('--range', nargs=2)
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument('--dry-run', action='store_true')
    g.add_argument('--apply', action='store_true')
    a = ap.parse_args()
    os.chdir(ROOT)
    syms = json.load(open('config/symbol_names.json'))['symbols']
    toks, scopes = set(), set()
    if a.range:
        base, size = int(a.range[0], 16), int(a.range[1], 0)
        scopes.add(a.ov)
        rec = syms['%s:0x%08X' % (a.ov, base)]
        assert rec.get('landed'), a.range
        toks.add(rec['name'])
        for off in range(4, size, 4):
            toks.add('D_%08X' % (base + off))
    for p in a.layouts:
        lay = json.load(open(p))
        scope = lay['scope']
        scopes.add(scope)
        fields = {off for off, _, ct in lay['leaves'] if ct in ('s32', 'u32')}
        for h in lay['hosts']:
            rec = syms['%s:0x%08X' % (scope, int(h, 16))]
            assert rec.get('landed') and rec['size'] == lay['size'], h
            base = int(h, 16)
            for off in fields:
                toks.add(rec['name'] if off == 0 else 'D_%08X' % (base + off))
    assert len(scopes) == 1, scopes
    ov = scopes.pop()
    files = subprocess.run(['git', 'ls-files', 'src/overlay/%s/' % ov],
                           capture_output=True, text=True).stdout.split()
    changed, refused = 0, 0
    for f in files:
        if not f.endswith('.c'):
            continue
        text = open(f, errors='surrogateescape').read()
        new, did = text, []
        for m in re.finditer(r'(?m)^extern void \* ?(\w+);[ \t]*$', text):
            t = m.group(1)
            if t not in toks:
                continue
            uses = [ln for ln in new.split('\n')
                    if re.search(r'\b%s\b' % t, ln) and not ln.lstrip().startswith('extern')]
            if not all(re.fullmatch(r'\s*%s = \(void \*\)0x[0-9A-Fa-f]+;\s*' % t, ln) for ln in uses):
                print('REFUSE %s %s: a use is not a constant store' % (f, t))
                refused += 1
                continue
            new = re.sub(r'(?m)^extern void \* ?%s;' % t, 'extern s32 %s;' % t, new)
            new = re.sub(r'(?m)^(\s*%s = )\(void \*\)(0x[0-9A-Fa-f]+;)' % t, r'\1\2', new)
            did.append(t)
        if did:
            changed += 1
            print('%s %s: %d cells' % ('APPLY' if a.apply else 'DRY', f, len(did)))
            if a.apply:
                open(f, 'w', errors='surrogateescape').write(new)
    print('%d files, %d refused' % (changed, refused))
    sys.exit(1 if refused else 0)


if __name__ == '__main__':
    main()
