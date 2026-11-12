#!/usr/bin/env python3
"""Scoped catch-up of pc/ld/psx_globals.syms after a MAIN data landing.

The PC port links every data symbol at its absolute PS1 address through
pc/ld/psx_globals.syms (`--just-symbols`, the globals model).  Neither
pc/tools/sync_symbol_names.py (it skips pc/ld/) nor the landers touch that
file, so a landed data name that is missing from it fails the link, and one
whose line carries an OLD address (a moved name) links silently at the wrong
address -- a behaviour change no byte gate sees.

A full `pc/tools/gen_psx_globals.py --all` is not idempotent at HEAD
(2026-11-02: it drops a dozen address tokens the committed file still
carries), so this tool edits only the records it is given:

  * for each --only 0xADDR whose ledger record is LANDED: ensure exactly one
    `name = 0xADDR;` line (an existing line for that name with another
    address is corrected in place; a missing one is appended);
  * the `D_XXXXXXXX = 0xXXXXXXXX;` address line is always kept (or added for
    a reverted record), since alias views and untouched files may still use it.

Lines are edited in place and new ones appended (the linker does not care
about order); nothing else in the file changes.  Idempotent.

An overlay record is given as OV:0xADDR (its name is bound at the overlay
address; the PC port maps every overlay at 0x80132000, so the line is the
same shape).

usage: sync_psx_globals.py --only [OV:]0xADDR [--only ...] [--dry-run] [--file PATH]
"""
import argparse
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'analysis'))
import symbol_names as SN  # noqa: E402

LINE = re.compile(r'^([A-Za-z_][A-Za-z0-9_]*) = 0x([0-9A-Fa-f]{8});$')


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--only', action='append', required=True)
    ap.add_argument('--dry-run', action='store_true')
    ap.add_argument('--file', default=os.path.join(ROOT, 'pc', 'ld', 'psx_globals.syms'))
    args = ap.parse_args()
    syms = SN.load_names(all_scopes=True)['symbols']
    text = open(args.file).read()
    lines = text.rstrip('\n').split('\n')
    where = {}
    for i, l in enumerate(lines):
        m = LINE.match(l)
        if m:
            where.setdefault(m.group(1), i)
    changes = []

    def put(name, addr):
        line = f'{name} = 0x{addr:08X};'
        if name in where:
            if lines[where[name]] != line:
                changes.append(f'FIX   {line}  (was {lines[where[name]]})')
                lines[where[name]] = line
        else:
            changes.append(f'ADD   {line}')
            where[name] = len(lines)
            lines.append(line)

    for a in args.only:
        scope, _, hexa = a.rpartition(':')
        addr = int(hexa, 16)
        key = f'{scope}:0x{addr:08X}' if scope else f'0x{addr:08X}'
        put(f'D_{addr:08X}', addr)
        e = syms.get(key)
        if e and e.get('kind') == 'data' and e.get('landed'):
            put(e['name'], addr)
    for c in changes:
        print(c)
    print(f'{len(changes)} change(s)')
    if changes and not args.dry_run:
        open(args.file, 'w').write('\n'.join(lines) + '\n')
    return 0


if __name__ == '__main__':
    sys.exit(main())
