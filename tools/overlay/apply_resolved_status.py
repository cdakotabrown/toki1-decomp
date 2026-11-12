#!/usr/bin/env python3
"""Promote reloc_only -> byte_perfect using the honest name-derived resolved link.

A function classified `reloc_only` by the lenient mask in verify_overlay_bytes.py
matches the target everywhere except at relocation immediates (which the masked,
single-.o pipeline leaves at 0). resolve_overlay_relocs.resolve_function compiles
that same C, links the .o at the function's true address with every func_/D_
symbol defined at its NAME-ENCODED address, and strict-compares to the shipped
EXN -- no masking, no copy-from-target. If that strict compare is byte-perfect
with 0 unresolved symbols, the function is genuinely byte_perfect (ld computed the
immediates from the symbol names the decompiler already wrote).

This pass ONLY promotes (reloc_only -> byte_perfect). It never demotes, so the
effective count (byte_perfect + reloc_only) is unchanged; it reclassifies proven
matches out of the provisional bucket. Functions whose strict compare fails --
including the documented `.rodata` local-data blocker (unresolved section symbol)
and mislabeled-semi codegen diffs -- are left as reloc_only.

Usage:
  python3 tools/overlay/apply_resolved_status.py            # all overlays
  python3 tools/overlay/apply_resolved_status.py DATE EVENT # subset
  python3 tools/overlay/apply_resolved_status.py --dry-run  # report only
"""
import sys
import os
import json
import collections
from pathlib import Path
from concurrent.futures import ProcessPoolExecutor, as_completed

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"


def _resolve_one(args):
    """Worker: returns (overlay, func, matched_bool, reason)."""
    overlay, func = args
    # import inside worker so each process sets up cleanly
    sys.path.insert(0, str(HERE))
    import resolve_overlay_relocs as R
    try:
        r = R.resolve_function(overlay, func)
    except Exception as e:  # noqa: BLE001 - report, never crash the pass
        return (overlay, func, False, f"exception:{e}")
    if 'error' in r:
        return (overlay, func, False, f"error:{r['error']}")
    if r.get('matched'):
        return (overlay, func, True, 'resolved')
    unresolved = r.get('unresolved') or []
    if unresolved:
        return (overlay, func, False, f"unresolved:{','.join(unresolved)}")
    return (overlay, func, False, 'codegen_residual')


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    dry = '--dry-run' in sys.argv
    demote = '--demote' in sys.argv  # codegen-residual reloc_only -> semi_matched
    # --rescan-semi: also re-resolve semi_matched entries. The lenient mask can
    # label a genuinely byte-perfect function semi_matched (diff on a word it
    # doesn't type as reloc), and the default pass never looks at semis, so the
    # mislabel survives every reconcile (10 found on 2026-07-25, all resolver-
    # verified byte-perfect). Promotion-only: an unmatched semi stays semi.
    rescan_semi = '--rescan-semi' in sys.argv
    workers = 6
    for a in sys.argv[1:]:
        if a.startswith('--workers='):
            workers = int(a.split('=', 1)[1])

    status = json.load(open(STATUS_FILE))
    overlays = args if args else sorted(k for k, v in status.items()
                                        if isinstance(v, dict))

    # ── Prune ghosts ───────────────────────────────────────────────
    # This pass MERGES into the existing JSON, so an entry whose symbol has
    # since been removed from the .sym (absorbed into its host, merged after
    # an over-split, or de-registered) survives here forever -- still carrying
    # whatever status it last had. The 2026-07-21 absorption campaign left
    # 1,086 such ghosts, 1,082 of them counted `byte_perfect`: the exact fake
    # wins the campaign had just removed. A symbol that does not exist cannot
    # be matched, so drop anything the .sym no longer declares.
    pruned = collections.Counter()
    for ov in overlays:
        sym_file = os.path.join(os.path.dirname(STATUS_FILE), 'overlay',
                                f'{ov}.sym')
        if not os.path.exists(sym_file):
            continue
        declared = set()
        for line in open(sym_file):
            parts = line.split('#')[0].strip().split(':')
            if len(parts) >= 3:
                declared.add(parts[2])
        if not declared or not isinstance(status.get(ov), dict):
            continue
        for fn in [f for f in status[ov] if f not in declared]:
            del status[ov][fn]
            pruned[ov] += 1
    if pruned:
        print(f"Pruned {sum(pruned.values())} ghost entries "
              f"(in JSON, absent from .sym):")
        for ov in sorted(pruned, key=lambda k: -pruned[k])[:10]:
            print(f"  {ov:12} -{pruned[ov]}")

    # collect reloc_only (and, with --rescan-semi, semi_matched) functions
    targets = []
    orig_status = {}
    for ov in overlays:
        for fn, info in status.get(ov, {}).items():
            st = info.get('status') if isinstance(info, dict) else info
            if st == 'reloc_only' or (rescan_semi and st == 'semi_matched'):
                targets.append((ov, fn))
                orig_status[(ov, fn)] = st

    n_semi = sum(1 for t in targets if orig_status[t] == 'semi_matched')
    print(f"Scanning {len(targets) - n_semi} reloc_only"
          + (f" + {n_semi} semi_matched" if rescan_semi else "")
          + f" functions across {len(overlays)} overlays with {workers} workers...")

    promoted = 0
    demoted = 0
    by_overlay = {}
    demote_by_overlay = {}
    reasons = {'unresolved': 0, 'codegen_residual': 0, 'error': 0}
    with ProcessPoolExecutor(max_workers=workers) as ex:
        futs = {ex.submit(_resolve_one, t): t for t in targets}
        done = 0
        for fut in as_completed(futs):
            ov, fn, matched, reason = fut.result()
            done += 1
            if done % 200 == 0:
                print(f"  ...{done}/{len(targets)}")
            if matched:
                promoted += 1
                by_overlay[ov] = by_overlay.get(ov, 0) + 1
                if not dry:
                    status[ov][fn] = {'status': 'byte_perfect',
                                      'resolved': True}
            else:
                if reason.startswith('unresolved'):
                    reasons['unresolved'] += 1
                elif reason.startswith(('error', 'exception')):
                    reasons['error'] += 1
                else:
                    reasons['codegen_residual'] += 1
                    # codegen-residual = differs after resolving ALL named
                    # symbols => genuine codegen diff, honestly semi (the
                    # lenient mask mislabeled it reloc_only). Never .rodata.
                    if demote and orig_status.get((ov, fn)) == 'reloc_only':
                        demoted += 1
                        demote_by_overlay[ov] = demote_by_overlay.get(ov, 0) + 1
                        if not dry:
                            prev = status[ov][fn]
                            pct = (prev.get('exact_pct')
                                   if isinstance(prev, dict) else None)
                            status[ov][fn] = {
                                'status': 'semi_matched',
                                'demoted_from': 'reloc_only',
                                'reason': 'codegen_residual_after_resolve',
                            }
                            if pct is not None:
                                status[ov][fn]['exact_pct'] = pct

    # tally effective before/after (effective is unchanged by promotion)
    bp = ro = sm = 0
    for ov in status:
        if not isinstance(status[ov], dict):
            continue
        for fn, info in status[ov].items():
            st = info.get('status') if isinstance(info, dict) else info
            if st == 'byte_perfect':
                bp += 1
            elif st == 'reloc_only':
                ro += 1
            elif st == 'semi_matched':
                sm += 1

    print("\n=== Resolved-link promotion ===")
    print(f"Promoted reloc_only -> byte_perfect: {promoted}")
    for ov in sorted(by_overlay, key=lambda k: -by_overlay[k]):
        print(f"  {ov:12} +{by_overlay[ov]}")
    print(f"Not promoted: unresolved/.rodata={reasons['unresolved']}, "
          f"codegen_residual={reasons['codegen_residual']}, "
          f"error={reasons['error']}")
    if demote:
        print(f"Demoted codegen-residual reloc_only -> semi_matched: {demoted}")
        for ov in sorted(demote_by_overlay, key=lambda k: -demote_by_overlay[k]):
            print(f"  {ov:12} -{demote_by_overlay[ov]}")
    print(f"\nPost-pass buckets: byte_perfect={bp} reloc_only={ro} "
          f"semi_matched={sm}  effective={bp+ro}")
    if dry:
        print("\n[--dry-run] status JSON NOT written.")
    else:
        json.dump(status, open(STATUS_FILE, 'w'), indent=2)
        print(f"\nWrote {STATUS_FILE}")


if __name__ == '__main__':
    main()
