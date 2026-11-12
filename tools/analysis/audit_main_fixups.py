#!/usr/bin/env python3
"""audit_main_fixups.py -- catch main-binary "matches" propped up by the harness.

`build_level3_c`'s `fixup_relocations` copies immediates from the RETAIL binary
so a compiled object can be compared without linking.  It is meant to cover
relocation sites (jal targets, %hi/%lo), but it also copies:

  * imm16s at NON-relocation sites  -- a wrong stack-slot offset or a wrong
    frame size reads as a match (see srn_vram_set, where a dead
    `patch_stack_complete --old 24` hid a 144-vs-160 frame behind 22 copied
    words while still printing 98.2%);
  * BRANCH DISPLACEMENTS               -- which can hide genuinely WRONG control
    flow (see func_80060950, where a `scheduler_normalizer` rotated a guard so
    both branches went to the exit and `sub_sub_step_up` became unreachable; the
    corrupted `b` was papered over and the function read 133/134).

`test_single_match.py` reports these on its `Fixup:` line.  This tool runs every
registered C entry and flags any with a non-zero count, so the whole registry
can be checked in one pass instead of per-function by eye.

A clean run means every registered C function reproduces its bytes from its own
compiled object, with only true relocation sites resolved from the target.

    python3 tools/analysis/audit_main_fixups.py [--only NAME ...] [--json OUT]
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
    ap.add_argument('--only', nargs='*', default=None)
    ap.add_argument('--json', default=None)
    args = ap.parse_args()

    reg = json.load(open(os.path.join(ROOT, 'config/level3_functions.json')))
    rows = []
    flagged = []
    for f in reg['functions']:
        if not str(f.get('source', '')).endswith('.c'):
            continue
        if args.only and f['name'] not in args.only:
            continue
        r = subprocess.run(
            [sys.executable, os.path.join(ROOT, 'tools/analysis/test_single_match.py'), f['name']],
            capture_output=True, text=True, cwd=ROOT, timeout=600)
        m = re.search(r'Fixup:\s+(\d+) word', r.stdout)
        n = int(m.group(1)) if m else 0
        bp = 'BYTE-PERFECT' in r.stdout
        kinds = sorted(set(re.findall(r'\[([A-Z][A-Z ]+) copied', r.stdout)))
        row = {'name': f['name'], 'fixups': n, 'byte_perfect': bp, 'kinds': kinds,
               'source': f['source']}
        rows.append(row)
        if n or not bp:
            flagged.append(row)
            print(f"FLAG {f['name']:28s} fixups={n:3d} byte_perfect={bp} {','.join(kinds)}")

    print(f"\n{len(rows)} registered C entries: {len(flagged)} flagged "
          f"({sum(1 for r in rows if not r['byte_perfect'])} not byte-perfect, "
          f"{sum(1 for r in rows if r['fixups'])} with copied words)")
    if args.json:
        json.dump(rows, open(args.json, 'w'), indent=1)


if __name__ == '__main__':
    main()
