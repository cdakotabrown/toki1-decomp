#!/usr/bin/env python3
"""Overlay array pass: rewrite the interior D_ tokens of every landed integer
array object of one overlay as elements (tools/naming/apply_interior.py --ov).

The hosts are the overlay's landed `kind: data` records whose `type` is an
integer array (`s32[79]`, `u8[3][4]`, ...).  Pointer-element tables are left
for a later pass (PC cell width), and so are volatile
tokens and files whose host is declared volatile: a volatile cast through
&host[i] changes IDO's codegen (DATE, 2026-11-09: 16 functions).  Run as a PRE step of
tools/naming/land_overlay_data.sh; the gates prove every touched function.

usage: array_pass.py OV [--skip-file F ...] (--dry-run|--apply)
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
    ap.add_argument('ov')
    ap.add_argument('--skip-file', action='append', default=[])
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument('--dry-run', action='store_true')
    g.add_argument('--apply', action='store_true')
    a = ap.parse_args()
    syms = json.load(open(os.path.join(ROOT, 'config/symbol_names.json')))['symbols']
    n = 0
    for k, v in sorted(syms.items()):
        if not k.startswith(a.ov + ':') or v.get('kind') != 'data' or not v.get('landed'):
            continue
        m = re.fullmatch(r'(s32|u32|s16|u16|u8|s8)\[(\d+)\](?:\[(\d+)\])?', (v.get('type') or '').replace(' ', ''))
        if not m:
            continue
        el, d1, d2 = m.groups()
        cmd = [sys.executable, os.path.join(ROOT, 'tools/naming/apply_interior.py'), k.split(':')[1], el,
               d1 + (',' + d2 if d2 else ''), '--ov', a.ov, '--no-volatile', '--apply' if a.apply else '--dry-run']
        for f in a.skip_file:
            cmd += ['--skip-file', f]
        r = subprocess.run(cmd, capture_output=True, text=True)
        if r.returncode:
            print(r.stdout + r.stderr)
            sys.exit(1)
        first = r.stdout.split('\n')[0]
        if not first.endswith('0 tokens rewritten in 0 files; 0 files with skips'):
            print(first)
        n += 1
    print('%d array hosts in %s' % (n, a.ov))


if __name__ == '__main__':
    main()
