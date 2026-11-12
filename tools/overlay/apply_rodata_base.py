#!/usr/bin/env python3
"""Bake @RodataBase directives so .rodata-blocked reloc_only functions become
byte_perfect.

A function whose ONLY unresolved relocation is the anonymous `.rodata` section
symbol (a switch jump table / local float/string in its own .rodata) sits in the
reloc_only bucket because the isolated-link resolver has nowhere to put .rodata.
resolve_overlay_relocs.derive_rodata_base() computes that block's real overlay
address ONCE from the target (the same one-time target-derived extraction that
produced every D_<addr>/func_<addr>/.sym address) and SELF-VALIDATES by
re-linking at the base and requiring a full byte match. We then write a
`/* @RodataBase: 0xADDR */` directive into the .c. At verify time the resolver
uses the STORED base (a +4 corruption fails the strict compare), so this is the
same honesty tier as @SymbolStart -- not copy-from-target.

Effect: promotes single-block .rodata reloc_only -> byte_perfect. Effective
(bp+ro+asm) is UNCHANGED (these were already reloc_only/effective); this is a
quality upgrade (provisional -> proven). Multi-item .rodata (one base can't
satisfy all data) is left reloc_only.

Usage:
  python3 tools/overlay/apply_rodata_base.py BUNKAKEN GEKO   # overlays
  python3 tools/overlay/apply_rodata_base.py --all
  python3 tools/overlay/apply_rodata_base.py BUNKAKEN --dry-run
"""
import sys
import os
import re
import json
from pathlib import Path
from concurrent.futures import ProcessPoolExecutor, as_completed

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"
SRC = ROOT / "src" / "overlay"


def _worker(args):
    ov, fn, dry = args
    sys.path.insert(0, str(HERE))
    import resolve_overlay_relocs as R
    p = SRC / ov / f"{fn}.c"
    if not p.exists():
        return (ov, fn, 'skip', 'no source')
    try:
        r = R.resolve_function(ov, fn)
    except Exception as e:  # noqa: BLE001
        return (ov, fn, 'skip', f'exc:{e}')
    if 'error' in r:
        return (ov, fn, 'skip', r['error'])
    if r.get('matched'):
        return (ov, fn, 'already', '')
    if (r.get('unresolved') or []) != ['.rodata']:
        return (ov, fn, 'skip', 'not-rodata-only')
    if '@RodataBase' in p.read_text(errors='ignore'):
        return (ov, fn, 'skip', 'already-has-directive')
    try:
        base = R.derive_rodata_base(ov, fn)
    except Exception as e:  # noqa: BLE001
        return (ov, fn, 'skip', f'derive-exc:{e}')
    if base is None:
        return (ov, fn, 'multi', 'multi-item .rodata')
    if dry:
        return (ov, fn, 'fixed', f'0x{base:08X}')
    orig = p.read_text(errors='ignore')
    p.write_text(f"/* @RodataBase: 0x{base:08X} */\n" + orig)
    r2 = R.resolve_function(ov, fn)
    if r2.get('matched'):
        return (ov, fn, 'fixed', f'0x{base:08X}')
    p.write_text(orig)            # revert: didn't byte-match
    return (ov, fn, 'revert', 'no full match after directive')


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    dry = '--dry-run' in sys.argv
    # --include-semi: also scan semi_matched entries. The masked verify can
    # mislabel a byte-perfect-except-.rodata function as semi_matched (it
    # never promotes on its own), and the default reloc_only-only scan then
    # misses it forever. The 'unresolved == [.rodata]' gate in _worker keeps
    # genuine semis out.
    include_semi = '--include-semi' in sys.argv
    workers = 8
    for a in sys.argv[1:]:
        if a.startswith('--workers='):
            workers = int(a.split('=', 1)[1])
    # Positional args: overlay names, plus optional func_XXXXXXXX names to
    # target specific functions regardless of their JSON status (previously a
    # func name was silently treated as an unknown overlay and did nothing).
    func_filter = {a for a in args if re.match(r'^func_[0-9A-Fa-f]+$', a)}
    ov_args = [a for a in args if a not in func_filter]
    status = json.load(open(STATUS_FILE))
    overlays = ov_args if ov_args and '--all' not in sys.argv else sorted(
        k for k, v in status.items() if isinstance(v, dict))
    want = {'reloc_only'} | ({'semi_matched'} if include_semi else set())
    targets = []
    for ov in overlays:
        for fn, info in status.get(ov, {}).items():
            st = info.get('status') if isinstance(info, dict) else info
            if func_filter:
                if fn in func_filter:
                    targets.append((ov, fn, dry))
            elif st in want:
                targets.append((ov, fn, dry))
    print(f"Scanning {len(targets)} funcs ({' + '.join(sorted(want))}"
          f"{', explicit' if func_filter else ''}) across {len(overlays)} "
          f"overlays, {workers} workers{' [DRY-RUN]' if dry else ''}")
    fixed = {}
    counts = {'fixed': 0, 'multi': 0, 'already': 0, 'skip': 0, 'revert': 0}
    done = 0
    with ProcessPoolExecutor(max_workers=workers) as ex:
        futs = [ex.submit(_worker, t) for t in targets]
        for fut in as_completed(futs):
            ov, fn, outcome, detail = fut.result()
            counts[outcome] = counts.get(outcome, 0) + 1
            done += 1
            if done % 200 == 0:
                print(f"  ...{done}/{len(targets)}")
            if outcome == 'fixed':
                fixed.setdefault(ov, 0)
                fixed[ov] += 1
    total = sum(fixed.values())
    print(f"\n=== @RodataBase applied ===")
    print(f"single-block .rodata -> byte_perfect: {total}"
          f"{' [would; dry-run]' if dry else ''}")
    for ov in sorted(fixed, key=lambda k: -fixed[k]):
        print(f"  {ov:12} +{fixed[ov]}")
    print(f"multi-item .rodata (left reloc_only): {counts['multi']}   "
          f"already-bp: {counts['already']}   reverted: {counts['revert']}   "
          f"skip: {counts['skip']}")
    if not dry and total:
        print("\nNow run apply_resolved_status.py --demote to reclassify, "
              "then commit the .c files.")


if __name__ == '__main__':
    main()
