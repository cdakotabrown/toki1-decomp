#!/usr/bin/env python3
"""Batch verify overlay functions at byte level.

Scans all functions in one or all overlays, compiles each through the
full PostProc pipeline, and compares bytes against target EXN files.

Generates:
  - config/overlay_match_status.json - per-function match status
  - Console summary per overlay

Usage:
    python3 tools/overlay/verify_overlay_bytes.py --all
    python3 tools/overlay/verify_overlay_bytes.py GEKO
    python3 tools/overlay/verify_overlay_bytes.py GEKO --verbose
    python3 tools/overlay/verify_overlay_bytes.py --summary  # Just show current status
"""
import sys
import os
import json
import time
from pathlib import Path
from typing import Dict, List, Optional
from concurrent.futures import ThreadPoolExecutor, as_completed

# Project root
ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

from test_overlay_match import test_overlay_function, parse_sym_file

# Paths
SRC_OVERLAY_DIR = ROOT / "src" / "overlay"
CONFIG_DIR = ROOT / "config" / "overlay"
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"


def get_all_overlays() -> List[str]:
    """Get list of all overlay names."""
    overlays = []
    if SRC_OVERLAY_DIR.exists():
        for d in sorted(SRC_OVERLAY_DIR.iterdir()):
            if d.is_dir() and not d.name.startswith('.'):
                overlays.append(d.name)
    return overlays


def _is_comment_only(c_file: Path) -> bool:
    """True if the file carries no code — only comments/blank lines.

    Such files are documentation markers (e.g. de-fabricated cross-prologue
    heads: "NOT A FUNCTION ... left as byte-perfect ASM"). They intentionally
    have no .sym symbol of their own and can never verify.
    """
    try:
        text = c_file.read_text()
    except OSError:
        return False
    in_comment = False
    for line in text.splitlines():
        stripped = line.strip()
        if not stripped:
            continue
        if in_comment:
            if '*/' in stripped:
                in_comment = False
                rest = stripped.split('*/', 1)[1].strip()
                if rest and not rest.startswith('/*'):
                    return False
            continue
        if stripped.startswith('/*'):
            if '*/' not in stripped:
                in_comment = True
            else:
                rest = stripped.split('*/', 1)[1].strip()
                if rest:
                    return False
            continue
        return False
    return True


def get_overlay_functions(overlay_name: str) -> List[str]:
    """Get list of all functions in an overlay from source files.

    Comment-only marker files whose stem is not a .sym symbol are skipped:
    they document non-function byte spans (de-fabricated head stubs) and
    would otherwise be reported as perpetual "errors".
    """
    src_dir = SRC_OVERLAY_DIR / overlay_name
    if not src_dir.exists():
        return []

    sym_names = set(parse_sym_file(overlay_name).keys())
    funcs = []
    skipped = 0
    for c_file in sorted(src_dir.glob("*.c")):
        if sym_names and c_file.stem not in sym_names and _is_comment_only(c_file):
            skipped += 1
            continue
        funcs.append(c_file.stem)
    if skipped:
        print(f"[{overlay_name}] skipped {skipped} comment-only marker file(s) "
              f"with no .sym symbol (documentation, not functions)")
    return funcs


def load_status() -> Dict:
    """Load existing status file."""
    if STATUS_FILE.exists():
        try:
            with open(STATUS_FILE) as f:
                return json.load(f)
        except:
            pass
    return {}


def save_status(status: Dict):
    """Save status to file."""
    STATUS_FILE.parent.mkdir(parents=True, exist_ok=True)
    with open(STATUS_FILE, 'w') as f:
        json.dump(status, f, indent=2, sort_keys=True)


def verify_overlay(overlay_name: str, verbose: bool = False,
                   parallel: int = 4, update_status: bool = True,
                   fix_relocs: bool = False) -> Dict:
    """Verify all functions in an overlay.

    Args:
        overlay_name: Overlay name (e.g., GEKO)
        verbose: Print detailed output
        parallel: Number of parallel workers
        update_status: Update status JSON file
        fix_relocs: Apply relocation fixup to achieve byte-perfect

    Returns dict with:
        - total: total function count
        - byte_perfect: list of byte-perfect function names
        - reloc_only: list of reloc-only diff function names
        - semi_matched: list of same-count but diff bytes
        - count_mismatch: list of instruction count mismatches
        - errors: list of compile/other errors
    """
    os.chdir(ROOT)

    funcs = get_overlay_functions(overlay_name)
    if not funcs:
        print(f"[{overlay_name}] No functions found")
        return {'total': 0}

    results = {
        'total': len(funcs),
        'byte_perfect': [],
        'reloc_only': [],
        'semi_matched': [],
        'count_mismatch': [],
        'errors': [],
    }

    # Load existing status for incremental updates
    status = load_status()
    if overlay_name not in status:
        status[overlay_name] = {}

    print(f"\n[{overlay_name}] Verifying {len(funcs)} functions...")

    def test_func(func_name):
        try:
            return func_name, test_overlay_function(overlay_name, func_name, verbose=False,
                                                     fix_relocs=fix_relocs)
        except SystemExit as e:
            # A PostProc step bailed (banned directive, or unresolved
            # custom_script/asm_intercept). Surface the precise reason when the
            # exit carried a message; report as error, don't crash the batch.
            if isinstance(e.code, str) and e.code:
                return func_name, {'error': e.code}
            return func_name, {'error': 'BANNED PostProc directive (see stderr)'}

    # Process functions (parallel or serial)
    if parallel > 1 and not verbose:
        with ThreadPoolExecutor(max_workers=parallel) as executor:
            futures = {executor.submit(test_func, f): f for f in funcs}
            done = 0
            for future in as_completed(futures):
                done += 1
                func_name, result = future.result()
                _classify_result(func_name, result, results, status[overlay_name])
                if done % 50 == 0:
                    print(f"  Progress: {done}/{len(funcs)}")
    else:
        for i, func_name in enumerate(funcs):
            if verbose:
                print(f"\n--- {func_name} ---")
            result = test_overlay_function(overlay_name, func_name, verbose,
                                          fix_relocs=fix_relocs)
            _classify_result(func_name, result, results, status[overlay_name])
            if not verbose and (i + 1) % 50 == 0:
                print(f"  Progress: {i+1}/{len(funcs)}")

    # Save updated status
    if update_status:
        save_status(status)

    return results


def _classify_result(func_name: str, result: Dict, results: Dict, status_entry: Dict):
    """Classify a function result and update tracking."""
    if 'error' in result:
        results['errors'].append(func_name)
        status_entry[func_name] = {'status': 'error', 'error': result['error']}
    elif result.get('asm_only'):
        # ASM-ONLY functions are byte-perfect by construction (raw EXN bytes)
        results.setdefault('asm_only', []).append(func_name)
        results['byte_perfect'].append(func_name)
        status_entry[func_name] = {'status': 'asm_only', 'exact_pct': 100.0}
    elif result['matched']:
        results['byte_perfect'].append(func_name)
        status_entry[func_name] = {'status': 'byte_perfect'}
    elif result['reloc_only']:
        results['reloc_only'].append(func_name)
        status_entry[func_name] = {'status': 'reloc_only', 'exact_pct': result['exact_pct']}
    elif result['n_target'] == result['n_compiled']:
        results['semi_matched'].append(func_name)
        status_entry[func_name] = {
            'status': 'semi_matched',
            'exact_pct': result['exact_pct'],
            'reloc_pct': result['reloc_pct'],
        }
        if result.get('postproc_failed'):
            # D1: keep the broken-chain fact visible in the JSON so the
            # "semi" is not misread as a codegen residual.
            status_entry[func_name]['postproc_failed'] = True
        if result.get('truncated_surplus'):
            status_entry[func_name]['truncated_surplus'] = True
    else:
        results['count_mismatch'].append(func_name)
        status_entry[func_name] = {
            'status': 'count_mismatch',
            'n_target': result['n_target'],
            'n_compiled': result['n_compiled'],
        }
        if result.get('postproc_failed'):
            status_entry[func_name]['postproc_failed'] = True


def print_overlay_summary(overlay_name: str, results: Dict):
    """Print summary for one overlay."""
    total = results['total']
    bp = len(results.get('byte_perfect', []))
    ro = len(results.get('reloc_only', []))
    sm = len(results.get('semi_matched', []))
    cm = len(results.get('count_mismatch', []))
    asm = len(results.get('asm_only', []))
    err = len(results.get('errors', []))

    effective = bp + ro  # asm_only is included in byte_perfect already
    print(f"\n[{overlay_name}] Results:")
    print(f"  Total:          {total}")
    print(f"  Byte-perfect:   {bp} ({100*bp/max(total,1):.1f}%) [{asm} ASM-only]")
    print(f"  Reloc-only:     {ro} ({100*ro/max(total,1):.1f}%)")
    print(f"  Semi-matched:   {sm} ({100*sm/max(total,1):.1f}%)")
    print(f"  Count mismatch: {cm}")
    print(f"  Errors:         {err}")
    print(f"  Effective:      {effective}/{total} ({100*effective/max(total,1):.1f}%)")


def print_summary_from_status():
    """Print summary from existing status file."""
    status = load_status()
    if not status:
        print("No status file found. Run verification first.")
        return

    print("\n" + "=" * 70)
    print("OVERLAY BYTE-MATCH STATUS SUMMARY")
    print("=" * 70)

    totals = {'total': 0, 'byte_perfect': 0, 'reloc_only': 0, 'asm_only': 0,
              'semi_matched': 0, 'count_mismatch': 0, 'errors': 0}

    for overlay_name in sorted(status.keys()):
        funcs = status[overlay_name]
        counts = {'byte_perfect': 0, 'reloc_only': 0, 'asm_only': 0,
                  'semi_matched': 0, 'count_mismatch': 0, 'errors': 0}

        for func_name, info in funcs.items():
            s = info.get('status', 'unknown')
            if s in counts:
                counts[s] += 1

        total = sum(counts.values())
        bp = counts['byte_perfect']
        ro = counts['reloc_only']
        asm = counts['asm_only']
        # ASM-only is byte-perfect by construction (raw EXN bytes)
        effective = bp + ro + asm

        totals['total'] += total
        totals['byte_perfect'] += bp
        totals['reloc_only'] += ro
        totals['asm_only'] += asm
        totals['semi_matched'] += counts['semi_matched']
        totals['count_mismatch'] += counts['count_mismatch']
        totals['errors'] += counts['errors']

        print(f"{overlay_name:15} {total:4} funcs | BP: {bp:4} | RO: {ro:3} | "
              f"ASM: {asm:3} | Semi: {counts['semi_matched']:3} | "
              f"Eff: {100*effective/max(total,1):5.1f}%")

    print("-" * 70)
    t = totals
    eff = t['byte_perfect'] + t['reloc_only'] + t['asm_only']
    print(f"{'TOTAL':15} {t['total']:4} funcs | BP: {t['byte_perfect']:4} | "
          f"RO: {t['reloc_only']:3} | ASM: {t['asm_only']:3} | "
          f"Semi: {t['semi_matched']:3} | "
          f"Eff: {100*eff/max(t['total'],1):5.1f}%")
    print("=" * 70)


def verify_all(parallel: int = 4, fix_relocs: bool = False):
    """Verify all overlays."""
    overlays = get_all_overlays()
    if not overlays:
        print("No overlays found")
        return

    print(f"Verifying {len(overlays)} overlays...")
    if fix_relocs:
        print("(with relocation fixup enabled)")
    start = time.time()

    all_results = {}
    for overlay_name in overlays:
        results = verify_overlay(overlay_name, verbose=False, parallel=parallel,
                                fix_relocs=fix_relocs)
        all_results[overlay_name] = results
        print_overlay_summary(overlay_name, results)

    elapsed = time.time() - start
    print(f"\nTotal time: {elapsed:.1f}s")

    # Print grand totals
    print("\n" + "=" * 70)
    print("GRAND TOTALS")
    print("=" * 70)

    totals = {'total': 0, 'byte_perfect': 0, 'reloc_only': 0,
              'semi_matched': 0, 'count_mismatch': 0, 'errors': 0}
    for results in all_results.values():
        totals['total'] += results['total']
        totals['byte_perfect'] += len(results.get('byte_perfect', []))
        totals['reloc_only'] += len(results.get('reloc_only', []))
        totals['semi_matched'] += len(results.get('semi_matched', []))
        totals['count_mismatch'] += len(results.get('count_mismatch', []))
        totals['errors'] += len(results.get('errors', []))

    t = totals
    eff = t['byte_perfect'] + t['reloc_only']
    print(f"Total functions: {t['total']}")
    print(f"Byte-perfect:    {t['byte_perfect']} ({100*t['byte_perfect']/max(t['total'],1):.1f}%)")
    print(f"Reloc-only:      {t['reloc_only']} ({100*t['reloc_only']/max(t['total'],1):.1f}%)")
    print(f"Semi-matched:    {t['semi_matched']} ({100*t['semi_matched']/max(t['total'],1):.1f}%)")
    print(f"Count mismatch:  {t['count_mismatch']}")
    print(f"Errors:          {t['errors']}")
    print(f"Effective match: {eff}/{t['total']} ({100*eff/max(t['total'],1):.1f}%)")
    print("=" * 70)


def scan_write_insn_regressions(parallel: int = 4, fix_relocs: bool = False):
    """Scan all functions with write_insn_at_offset for regressions.

    A regression means write_insn_at_offset is making the match WORSE,
    which indicates stale offsets after scheduler reordering.
    """
    from pathlib import Path
    SRC_DIR = Path(__file__).parent.parent.parent / "src" / "overlay"

    # Find all functions with write_insn_at_offset
    candidates = []
    for ovl_dir in sorted(SRC_DIR.iterdir()):
        if not ovl_dir.is_dir():
            continue
        ovl = ovl_dir.name
        for c_file in sorted(ovl_dir.glob("*.c")):
            with open(c_file) as f:
                content = f.read()
            if 'write_insn_at_offset' not in content:
                continue
            fn = c_file.stem
            candidates.append((ovl, fn))

    print(f"Scanning {len(candidates)} functions with write_insn_at_offset for regressions...")

    from concurrent.futures import ProcessPoolExecutor, as_completed
    import subprocess

    def check_one(args):
        ovl, fn = args
        result = subprocess.run(
            ["python3", "tools/overlay/test_overlay_match.py",
             ovl, fn, "--check-write-insn-regression"],
            capture_output=True, text=True, timeout=60
        )
        return result.stdout.strip()

    regressions = []
    no_effect = []
    bp = 0
    ok = 0

    # Run serially (test_overlay_match is already fast)
    for ovl, fn in candidates:
        try:
            out = check_one((ovl, fn))
            if "REGRESSION" in out:
                regressions.append((ovl, fn, out))
                print(f"  ⚠️  {out}")
            elif "NO EFFECT" in out:
                no_effect.append((ovl, fn))
            elif "BYTE-PERFECT" in out:
                bp += 1
            else:
                ok += 1
        except Exception as e:
            print(f"  ERROR: {ovl}::{fn}: {e}")

    print(f"\n{'='*70}")
    print(f"WRITE_INSN REGRESSION SCAN RESULTS")
    print(f"{'='*70}")
    print(f"Total scanned: {len(candidates)}")
    print(f"OK (helping):  {ok}")
    print(f"BP (100%):     {bp}")
    print(f"No effect:    {len(no_effect)}")
    print(f"REGRESSIONS:  {len(regressions)}")

    if regressions:
        print(f"\n⚠️  REGRESSIONS DETECTED — these need write_insn offsets fixed:")
        for ovl, fn, out in regressions:
            print(f"  {ovl}::{fn}: {out}")
    else:
        print(f"\n✅ No regressions — all write_insn_at_offset patches are valid!")

    if no_effect:
        print(f"\nℹ️  Functions with zero-effect write_insn (dead patches):")
        for ovl, fn in no_effect:
            print(f"  {ovl}::{fn}")


def main():
    import argparse
    parser = argparse.ArgumentParser(description='Verify overlay functions at byte level')
    parser.add_argument('overlay', nargs='?', help='Overlay name (e.g., GEKO)')
    parser.add_argument('--all', action='store_true', help='Verify all overlays')
    parser.add_argument('--summary', action='store_true', help='Show current status summary')
    parser.add_argument('--verbose', '-v', action='store_true')
    parser.add_argument('--parallel', '-j', type=int, default=4,
                        help='Parallel workers (default: 4)')
    parser.add_argument('--fix-relocs', action='store_true',
                        help='Apply relocation fixup to achieve byte-perfect matches')
    parser.add_argument('--scan-write-insn-regressions', action='store_true',
                        help='Scan all functions with write_insn_at_offset for regressions')
    args = parser.parse_args()

    if args.summary:
        print_summary_from_status()
        return

    if args.scan_write_insn_regressions:
        scan_write_insn_regressions(args.parallel, args.fix_relocs)
        return

    if args.all:
        verify_all(parallel=args.parallel, fix_relocs=args.fix_relocs)
    elif args.overlay:
        results = verify_overlay(args.overlay.upper(), args.verbose, args.parallel,
                                fix_relocs=args.fix_relocs)
        print_overlay_summary(args.overlay.upper(), results)
    else:
        print("Usage: verify_overlay_bytes.py [--all | OVERLAY_NAME | --summary]")
        print("\nAvailable overlays:")
        for name in get_all_overlays():
            print(f"  {name}")
        sys.exit(1)


if __name__ == '__main__':
    main()
