#!/usr/bin/env python3
"""Audit config/level3_functions.json for SILENT TRUNCATION.

A registered C entry whose `size` is smaller than the bytes its .c actually
compiles to is only ever compared over the first `size` bytes -- the rest of
the body is never checked (fabrication audit category 1b, "silent head stub").
Three such entries shipped as "matched" until 2026-08-21 (SpuIsTransferCompleted size 8,
CDQ_init size 4, holiday_switch0/AB4 size 8 for 96-byte bodies).

For every C entry this compiles the file through the full PostProc pipeline
(same path as test_single_match) and reports:
  TRUNCATED  compiled body (minus benign trailing zero pad) > registered size
  SHORT      compiled body < registered size (would fail the build anyway)
  OK         sizes agree

Also flags entries whose .c carries @SymbolStart/@SymbolSize that disagree
with the registered address/size.

Usage:
    python3 tools/analysis/audit_registry_sizes.py [--only NAME ...] [--json OUT]
"""
import argparse, json, os, sys

_script_dir = os.path.dirname(os.path.abspath(__file__))
_build_dir = os.path.normpath(os.path.join(_script_dir, '..', 'build'))
sys.path.insert(0, _build_dir)

from build_level3_c import compile_c_function  # noqa: E402
from parse_postproc import (parse_postproc_header, parse_compile_flags,  # noqa: E402
                            parse_compiler, parse_compile_env,
                            parse_symbol_start, parse_symbol_size)


def body_len(func_bytes, size):
    """Extent of real code: registered size plus any NON-ZERO words past it
    (trailing zero words are a real delay-slot nop or IDO's .text pad; a
    non-zero word past the window is never compared = truncation)."""
    n = len(func_bytes)
    while n > size and func_bytes[n - 4:n] == b'\0\0\0\0':
        n -= 4
    return n


def main():
    root = os.path.dirname(os.path.dirname(_script_dir))
    os.chdir(root)
    ap = argparse.ArgumentParser()
    ap.add_argument('--config', default='config/level3_functions.json')
    ap.add_argument('--only', nargs='*')
    ap.add_argument('--json')
    args = ap.parse_args()

    funcs = json.load(open(args.config))['functions']
    rows = []
    for f in funcs:
        src = f['source']
        if not src.endswith('.c'):
            continue
        if args.only and f['name'] not in args.only:
            continue
        addr = int(f['address'], 16)
        size = int(f['size'])
        row = {'name': f['name'], 'address': f['address'], 'size': size, 'source': src}
        if not os.path.exists(src):
            row['verdict'] = 'MISSING'
            rows.append(row)
            continue
        ss, sz = parse_symbol_start(src), parse_symbol_size(src)
        if ss is not None and ss != addr:
            row['symbol_start_mismatch'] = hex(ss)
        if sz is not None and sz != size:
            row['symbol_size_mismatch'] = sz
        try:
            fb = compile_c_function(src, f['name'], addr, False,
                                    parse_postproc_header(src), parse_compile_flags(src),
                                    parse_compiler(src), parse_compile_env(src))
        except Exception as e:  # noqa: BLE001
            row['verdict'] = 'COMPILE_FAIL'
            row['error'] = str(e)[:200]
            rows.append(row)
            continue
        n = body_len(fb, size)
        row['compiled'] = n
        if n > size:
            row['verdict'] = 'TRUNCATED'
        elif n < size:
            row['verdict'] = 'SHORT'
        else:
            row['verdict'] = 'OK'
        rows.append(row)

    bad = [r for r in rows if r['verdict'] != 'OK' or 'symbol_start_mismatch' in r
           or 'symbol_size_mismatch' in r]
    for r in bad:
        extra = ''
        if 'symbol_start_mismatch' in r:
            extra += f" @SymbolStart={r['symbol_start_mismatch']}"
        if 'symbol_size_mismatch' in r:
            extra += f" @SymbolSize={r['symbol_size_mismatch']}"
        print(f"{r['verdict']:12s} {r['name']:28s} {r['address']} size={r['size']:5d} "
              f"compiled={r.get('compiled','?')}{extra}")
    counts = {}
    for r in rows:
        counts[r['verdict']] = counts.get(r['verdict'], 0) + 1
    print(f"\n{len(rows)} C entries: {counts}; {len(bad)} flagged")
    if args.json:
        json.dump(rows, open(args.json, 'w'), indent=1)
    return 1 if any(r['verdict'] == 'TRUNCATED' for r in rows) else 0


if __name__ == '__main__':
    sys.exit(main())
