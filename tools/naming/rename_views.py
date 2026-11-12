#!/usr/bin/env python3
"""Rename VIEW aliases: a token that declares an already-named address a
second time with another type or `volatile` (`D_800EF864_v`, `DAT_8012B200`,
`Db_800F022D`).

The view becomes `<name>` in every tracked file that spells it, and is
recorded on its HOST record in config/symbol_names.json as
`views: [{"name": <name>, "offset": <bytes from the host start>}]`.  Every
resolver (tools/overlay/overlay_names.py, audit_main_relocs.py, maspsx's
--reuse-at-same-hi) maps a view name to host + offset, so a view needs no
address in its spelling.

rows: JSON list of {"current": "D_800EF864_v", "name": "step_counter_v",
                    "host_key": "0x800EF864" | "OV:0x801...", "offset": 0,
                    "why": "..."}
usage: rename_views.py ROWS.json (--dry-run|--apply) [--by LABEL]
"""
import argparse
import datetime
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'analysis'))
import symbol_names as SN  # noqa: E402

TREES = ['src', 'include', 'pc', 'asm', 'config/patch', 'tools/patch']


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('rows')
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument('--dry-run', action='store_true')
    g.add_argument('--apply', action='store_true')
    ap.add_argument('--by', default='rename_views')
    a = ap.parse_args()
    os.chdir(ROOT)
    doc = SN.load_names(all_scopes=True)
    syms = doc['symbols']
    used = {v.get('name') for v in syms.values()}
    for v in syms.values():
        used.update(x.get('name') for x in v.get('views') or ())
    today = datetime.date.today().isoformat()
    bad = 0
    for r in json.load(open(a.rows)):
        old, new, key, off = r['current'], r['name'], r['host_key'], int(r.get('offset', 0))
        host = syms.get(key)
        if not host or not host.get('landed') or host.get('kind') != 'data':
            print(f'REFUSE {old}: host {key} is not a landed data record')
            bad += 1
            continue
        size = host.get('size') or 0
        if off < 0 or (size and off >= size and off != 0):
            print(f'REFUSE {old}: offset {off} outside host {host["name"]} [{size}]')
            bad += 1
            continue
        if not re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*', new) or len(new) > 36 or new in used or \
                subprocess.run(['git', 'grep', '-qw', new, '--'] + TREES, capture_output=True).returncode == 0:
            print(f'REFUSE {old}: {new} is not a free identifier')
            bad += 1
            continue
        hits = subprocess.run(['git', 'grep', '-lw', old, '--'] + TREES,
                              capture_output=True, text=True).stdout.split()
        print(f'{"APPLY" if a.apply else "DRY"} {old} -> {new} (view of {host["name"]}+{off}) in {len(hits)} files')
        if not a.apply:
            continue
        for h in hits:
            t = open(h, errors='surrogateescape').read()
            open(h, 'w', errors='surrogateescape').write(
                re.sub(r'(?<![A-Za-z0-9_])%s(?![A-Za-z0-9_])' % re.escape(old), new, t))
        host.setdefault('views', []).append({'name': new, 'offset': off, 'was': old, 'date': today,
                                             'by': a.by, 'why': r.get('why', '')})
        used.add(new)
    if a.apply and not bad:
        SN.save_names(doc)
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
