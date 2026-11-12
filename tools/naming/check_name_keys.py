#!/usr/bin/env python3
"""Check the places that key on a symbol's SPELLING, which a rename can
silently bypass (each one broke a gate in the 2026-11-10 renames):

  1. pc/Makefile `-DD_XXXXXXXX=hal_*` swaps: once D_XXXXXXXX is landed under
     a name, the name must carry the same swap (else the matched C binds
     the PS1 cell, not the native HAL pointer: trapIntrVSync boot crash);
  2. pc/ld/psx_globals.syms: every landed MAIN data name has its
     `name = 0xADDR;` line (else the PC link fails, or binds an old address);
  3. pc/tools/*_plan.json keys `OV/name`: the C file exists (else
     generate_overlay_modules.py refuses the module);
  4. config/patch/compositions.json `src` paths exist (else the English
     build refuses the composition);
  5. tracked files (asm_only listings under the gitignored asm/ tree, PC
     overrides) are not left deleted in the worktree by a rename.

Checks 1-3 need the PC port and check 4 the English patch in this checkout
(tools/repo_paths.py); without them those checks are
skipped and the report says so.

Exit 1 on any problem.  usage: check_name_keys.py [--fix-makefile]
"""
import argparse
import glob
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from repo_paths import ENGLISH, PC  # noqa: E402

# the other parts are checked only when they are in THIS checkout: a rename
# here never edits a sibling repository
HAVE_PC = PC is not None and os.path.realpath(str(PC)) == os.path.realpath(ROOT)
HAVE_ENGLISH = ENGLISH is not None and os.path.realpath(str(ENGLISH)) == os.path.realpath(ROOT)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--fix-makefile', action='store_true',
                    help='add the landed name next to each D_ swap in pc/Makefile')
    ap.add_argument('--fix-tracked', action='store_true',
                    help='a deleted tracked asm listing whose Address line an untracked listing in '
                         'the same directory carries: git rm the old, git add -f the new, print both')
    args = ap.parse_args(argv)
    os.chdir(ROOT)
    syms = json.load(open('config/symbol_names.json'))['symbols']
    landed = {int(k, 16): e['name'] for k, e in syms.items()
              if ':' not in k and e.get('landed') and e.get('name')}
    bad = []

    if not HAVE_PC:
        print('check_name_keys: SKIP checks 1-3 (pc/Makefile swaps, pc/ld/psx_globals.syms, '
              'pc/tools/*_plan.json): the PC port is not in this checkout')
    if not HAVE_ENGLISH:
        print('check_name_keys: SKIP check 4 (config/patch/compositions.json): '
              'the English patch is not in this checkout')
    # 1. Makefile swaps
    mk = open('pc/Makefile').read() if HAVE_PC else ''

    fixed = mk
    for m in re.finditer(r'-D(D_([0-9A-Fa-f]{8}))=(\w+)', mk):
        name = landed.get(int(m.group(2), 16))
        if name and name != m.group(3) and not re.search(r'-D' + re.escape(name) + r'=', mk):
            if args.fix_makefile:
                fixed = fixed.replace(m.group(0), f'-D{name}={m.group(3)} {m.group(0)}', 1)
            else:
                bad.append(f'pc/Makefile: -D{m.group(1)}={m.group(3)} but {m.group(1)} is landed as '
                           f'{name}: add -D{name}={m.group(3)}')
    if fixed != mk:
        open('pc/Makefile', 'w').write(fixed)
        print('pc/Makefile: swaps added for landed names')

    # 2. psx_globals.syms
    have = dict(re.findall(r'^\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+)',
                           open('pc/ld/psx_globals.syms').read(), re.M)) if HAVE_PC else {}
    for k, e in syms.items():
        if not HAVE_PC:
            break
        if ':' in k or not e.get('landed') or e.get('kind') != 'data':
            continue
        if have.get(e['name'], '').lower() != k.lower():
            bad.append(f'pc/ld/psx_globals.syms: {e["name"]} ({k}) missing or wrong: '
                       f'tools/naming/sync_psx_globals.py --only {k}')

    # 3. PC plan keys
    for f in sorted(glob.glob('pc/tools/*_plan.json')) if HAVE_PC else ():
        try:
            doc = json.load(open(f))
        except ValueError:
            continue
        keys = set()

        def walk(x):
            if isinstance(x, dict):
                for kk, v in x.items():
                    keys.add(kk)
                    walk(v)
            elif isinstance(x, list):
                for v in x:
                    walk(v)
        walk(doc)
        for kk in sorted(keys):
            m = re.fullmatch(r'([A-Z][A-Z0-9_]+)/(\w+)', kk)
            if m and os.path.isdir(f'src/overlay/{m.group(1)}') \
                    and not os.path.exists(f'src/overlay/{m.group(1)}/{m.group(2)}.c'):
                bad.append(f'{f}: key {kk} names no src/overlay/{kk}.c')

    # 4. patch compositions
    comp = open('config/patch/compositions.json').read() if HAVE_ENGLISH else ''

    for p in sorted(set(re.findall(r'"(src/overlay/[^"]+\.c)"', comp))):
        if not os.path.exists(p):
            bad.append(f'config/patch/compositions.json: {p} does not exist')

    # 5. tracked files deleted in the worktree
    out = subprocess.run(['git', 'ls-files', '--deleted', '--', 'asm'],
                         capture_output=True, text=True).stdout.split()
    staged = set(subprocess.run(['git', 'diff', '--cached', '--name-only', '--diff-filter=D'],
                                capture_output=True, text=True).stdout.split())
    if args.fix_tracked:
        for p in list(out):
            if p in staged or not p.endswith('.s'):
                continue
            blob = subprocess.run(['git', 'show', f'HEAD:{p}'], capture_output=True, text=True).stdout
            m = re.search(r'^ \* Address: (0x[0-9A-Fa-f]+)', blob, re.M)
            if not m:
                continue
            d = os.path.dirname(p)
            cands = [q for q in glob.glob(f'{d}/*.s')
                     if re.search(r'^ \* Address: ' + m.group(1) + r'\b', open(q).read(), re.M)
                     and subprocess.run(['git', 'ls-files', '--error-unmatch', q],
                                        capture_output=True).returncode != 0]
            if len(cands) == 1:
                subprocess.run(['git', 'rm', '-q', '--cached', '--', p], check=True)
                subprocess.run(['git', 'add', '-f', '--', cands[0]], check=True)
                staged.add(p)
                print(f'TRACKED {p} {cands[0]}')
    for p in out:
        if p not in staged:
            bad.append(f'{p}: tracked but deleted in the worktree (a rename moved it; '
                       'git add -f the new path, git rm the old)')

    for b in bad:
        print('NAME KEY', b)
    print(f'check_name_keys: {len(bad)} problem(s)')
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
