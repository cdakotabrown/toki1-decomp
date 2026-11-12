#!/usr/bin/env python3
"""Sweep: rebuild lax-era patch_insn_reg_field lists with the strict classifier.

For every semi_matched overlay function whose C carries a
patch_insn_reg_field PostProc line, strip the patch lines, regenerate with
gen_reg_patches.py (3 passes), ALWAYS keeping the strict result (a lax list can
only outscore strict via imm16-blind frankenwords, which are bugs; small
masked-count drops are honesty corrections). Logs one line per function to stdout.

Usage: python3 tools/overlay/restrict_patch_sweep.py [--exclude OVL/func ...]
"""
import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]


def masked(ov, fn):
    out = subprocess.run(
        ['python3', str(REPO / 'tools/overlay/test_overlay_match.py'), ov, fn],
        capture_output=True, text=True, cwd=REPO).stdout
    m = re.search(r'\((\d+\.\d)% exact, (\d+)/(\d+)', out)
    out2 = subprocess.run(
        ['python3', str(REPO / 'tools/overlay/gen_reg_patches.py'), ov, fn,
         '--dry-run'], capture_output=True, text=True, cwd=REPO).stdout
    m2 = re.search(r'Masked:\s+(\d+)/(\d+)', out2)
    return (int(m2.group(1)) if m2 else -1,
            'RELOC-ONLY' in out, m.group(1) if m else '?')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--exclude', nargs='*', default=[])
    ap.add_argument('--limit', type=int, default=0)
    args = ap.parse_args()
    excl = set(args.exclude)

    # wiki/ (the per-function notes and their manifest) is private-monorepo only
    mpath = REPO / 'wiki/manifest.json'
    if not mpath.exists():
        sys.exit('restrict_patch_sweep: needs wiki/manifest.json (private monorepo only; '
                 'the public repositories carry no wiki)')
    manifest = json.loads(mpath.read_text())
    todo = [e for e in manifest
            if e['status'] == 'semi_matched'
            and f"{e['overlay']}/{e['func']}" not in excl]
    done = 0
    for e in todo:
        ov, fn = e['overlay'], e['func']
        cpath = REPO / 'src/overlay' / ov / (fn + '.c')
        if not cpath.exists():
            continue
        orig = cpath.read_text(errors='replace')
        if 'patch_insn_reg_field' not in orig:
            continue
        base_masked, base_ro, _ = masked(ov, fn)
        stripped = re.sub(
            r'(?m)^(?: \* @PostProc: patch_insn_reg_field [^\n]+|'
            r'/\* @PostProc: patch_insn_reg_field [^\n]*\*/)\n', '', orig)
        cpath.write_text(stripped)
        for _ in range(3):
            r = subprocess.run(
                ['python3', str(REPO / 'tools/overlay/gen_reg_patches.py'),
                 ov, fn], capture_output=True, text=True, cwd=REPO).stdout
            if 'NREG=0' in r:
                break
        new_masked, new_ro, _ = masked(ov, fn)
        if new_masked < 0:
            cpath.write_text(orig)
            print('ERROR-REVERT %s/%s (test failed)' % (ov, fn), flush=True)
        else:
            # Always keep the strict list: a lax list can only outscore it
            # via imm16-blind frankenwords, which are bugs.
            tag = 'UP' if new_masked > base_masked else \
                  ('SAME' if new_masked == base_masked else 'HONEST-DOWN')
            print('%s %s/%s masked %d -> %d%s' % (
                tag, ov, fn, base_masked, new_masked,
                ' RELOC-ONLY' if new_ro else ''), flush=True)
        done += 1
        if args.limit and done >= args.limit:
            break
    print('sweep done: %d functions processed' % done, flush=True)


if __name__ == '__main__':
    main()
