#!/usr/bin/env python3
"""Audit `byte_perfect` claims for unverified bytes (the truncated-compare trap).

`resolve_overlay_relocs.resolve_function` sets `matched` only when
len(target) == len(compiled), so anything the resolver promoted is
length-safe.  The masked path in `verify_overlay_bytes.py` has no such check:
it compares word-by-word up to the shorter buffer, so a function whose compare
window is LONGER than its compiled output can be marked `byte_perfect` while
the tail of the window was never looked at.

That is not hypothetical.  The 2026-07-21 absorption campaign found host
windows that silently skipped an entire second cross-prologue head -- nine
hidden sub-functions inside DATE hosts alone, each sitting inside a function
counted as a win.

This tool re-resolves every function currently marked `byte_perfect` and
reports three buckets:

  TRUNCATED  len_target != len_compiled -- bytes in the window were never
             compared.  The claim is unproven; the missing tail is usually
             either a real sub-function or trailing padding.
  FAILED     resolves cleanly but does NOT match -- the claim is wrong.
  WINDOW-SHORT  byte-perfect over a @SymbolSize window that stops before the
             .sym symbol ends, with non-padding words in the gap that no
             function window covers (2026-10-03).
  OK         byte-perfect with equal lengths.

`asm_only` functions are skipped (they have no C to compile, by definition).

Usage:
    audit_truncated_compare.py                  # every overlay
    audit_truncated_compare.py DATE EVENT       # subset
    audit_truncated_compare.py --workers=8
    audit_truncated_compare.py --strict         # exit 1 if any TRUNCATED/FAILED
"""

import collections
import json
import os
import sys
from concurrent.futures import ProcessPoolExecutor, as_completed
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"


def _check_one(args):
    """Worker: (overlay, func) -> (overlay, func, verdict, len_t, len_c)."""
    overlay, func = args
    sys.path.insert(0, str(HERE))
    import resolve_overlay_relocs as R
    try:
        r = R.resolve_function(overlay, func)
    except Exception as e:  # noqa: BLE001 - report, never crash the pass
        return (overlay, func, 'error:%s' % type(e).__name__, 0, 0)
    if 'error' in r:
        return (overlay, func, 'skip:%s' % r['error'], 0, 0)
    lt, lc = r.get('len_target', 0), r.get('len_compiled', 0)
    if lt != lc:
        return (overlay, func, 'TRUNCATED', lt, lc)
    if not r.get('matched'):
        return (overlay, func, 'FAILED', lt, lc)
    # 2026-10-03: a @SymbolSize window shorter than the .sym symbol leaves the
    # symbol's tail uncompared (red-team finding 5); padding and tails that
    # another function's window covers are fine (see window_tail_gap).
    try:
        import test_overlay_match as T
        gap, _d = T.window_tail_gap(overlay, func)
    except Exception:                       # noqa: BLE001
        gap = 0
    if gap:
        return (overlay, func, 'WINDOW-SHORT', lt, gap)
    return (overlay, func, 'OK', lt, lc)


def main():
    argv = sys.argv[1:]
    names = [a for a in argv if not a.startswith('--')]
    strict = '--strict' in argv
    workers = 6
    for a in argv:
        if a.startswith('--workers='):
            workers = int(a.split('=', 1)[1])

    status = json.load(open(STATUS_FILE))
    overlays = names or sorted(k for k, v in status.items()
                               if isinstance(v, dict))

    targets = []
    for ov in overlays:
        for fn, info in status.get(ov, {}).items():
            st = info.get('status') if isinstance(info, dict) else info
            if st == 'byte_perfect':
                targets.append((ov, fn))

    print('Re-resolving %d byte_perfect functions across %d overlays '
          '(%d workers)...' % (len(targets), len(overlays), workers))

    buckets = collections.Counter()
    truncated, failed, short = [], [], []
    with ProcessPoolExecutor(max_workers=workers) as ex:
        futs = {ex.submit(_check_one, t): t for t in targets}
        done = 0
        for fut in as_completed(futs):
            ov, fn, verdict, lt, lc = fut.result()
            done += 1
            if done % 500 == 0:
                print('  ...%d/%d' % (done, len(targets)))
            key = verdict.split(':')[0]
            buckets[key] += 1
            if verdict == 'TRUNCATED':
                truncated.append((ov, fn, lt, lc))
            elif verdict == 'WINDOW-SHORT':
                short.append((ov, fn, lt, lc))
            elif verdict == 'FAILED':
                failed.append((ov, fn, lt, lc))

    print('\n' + '=' * 68)
    print('RESULTS')
    print('=' * 68)
    for key, n in buckets.most_common():
        print('  %-10s %d' % (key, n))

    if truncated:
        print('\nTRUNCATED -- window longer than compiled output; the tail was')
        print('never compared. Unproven byte_perfect claims:')
        by_ov = collections.Counter(ov for ov, _f, _t, _c in truncated)
        for ov, n in by_ov.most_common():
            print('   %-10s %d' % (ov, n))
        print('\n  worst offenders (bytes never checked):')
        for ov, fn, lt, lc in sorted(truncated, key=lambda r: -(r[2] - r[3]))[:20]:
            print('   %-10s %-18s target=%-6d compiled=%-6d  unchecked=%d'
                  % (ov, fn, lt, lc, lt - lc))

    if failed:
        print('\nFAILED -- resolves but does not match. Wrong claims:')
        for ov, fn, lt, lc in failed[:20]:
            print('   %-10s %-18s target=%d compiled=%d' % (ov, fn, lt, lc))
        if len(failed) > 20:
            print('   ... and %d more' % (len(failed) - 20))

    if short:
        print('\nWINDOW-SHORT -- @SymbolSize ends before the .sym symbol and the')
        print('tail holds non-padding words no function window covers:')
        for ov, fn, lt, gap in sorted(short):
            print('   %-10s %-18s window=%-6d uncovered=%d' % (ov, fn, lt, gap))

    if strict and (truncated or failed or short):
        print('\nFAIL: %d truncated, %d failed, %d window-short'
              % (len(truncated), len(failed), len(short)), file=sys.stderr)
        sys.exit(1)


if __name__ == '__main__':
    main()
