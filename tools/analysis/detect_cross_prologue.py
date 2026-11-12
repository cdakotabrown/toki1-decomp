#!/usr/bin/env python3
"""Detect main-binary cross-prologue boundary-mislabel candidates.

For each game-region (0x80041000-0x80085FFF) function with a C source, compile
it (globals-first as written), then test whether sliding the compare window
START back by 1..3 instructions — and growing the size to match the compiled
output's extra leading words — turns a "low% from offset 0" function into a
near-perfect reloc-masked match.

This is the cross-prologue signature: IDO -O2 hoisted the function's own
leading li/lui setup into the prior function's dead epilogue space, and the
.sym drew the start late. The fix is @SymbolStart (sanctioned boundary
recovery), NOT a code change.

Usage:
    python3 tools/analysis/detect_cross_prologue.py            # scan semi-matched
    python3 tools/analysis/detect_cross_prologue.py -c FILE    # test one file
"""
import sys, os, re, struct, subprocess, tempfile, glob, argparse
from pathlib import Path

ROOT = Path(__file__).parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "build"))
from build_level3_c import compile_c_function as compile_fn, fixup_relocations
from parse_postproc import (parse_postproc_header, parse_compile_flags,
                            parse_compiler, parse_compile_env)

EXE = ROOT / "disks" / "SLPS_000.65"
GAME_LO, GAME_HI = 0x80041000, 0x80086000


def header_info(c_path):
    with open(c_path) as f:
        head = f.read(2000)
    a = re.search(r'Address:\s*0x([0-9A-Fa-f]+)', head)
    s = re.search(r'Size:\s*(\d+)\s*bytes', head)
    if not a or not s:
        return None
    return int(a.group(1), 16), int(s.group(1))


def words(b):
    return [struct.unpack_from('<I', b, i)[0] for i in range(0, len(b) - 3, 4)]


def reloc_eq(t, c):
    if t == c:
        return True
    ot, oc = (t >> 26) & 0x3f, (c >> 26) & 0x3f
    if ot != oc:
        return False
    if ot in (2, 3):           # j/jal
        return True
    return (t & 0xffff0000) == (c & 0xffff0000)


def compile_words(c_path, addr):
    compiler = parse_compiler(c_path) or "ido52"
    steps = parse_postproc_header(c_path)
    flags = parse_compile_flags(c_path)
    env = parse_compile_env(c_path)
    cb = compile_fn(str(c_path), c_path.stem, addr, postproc_steps=steps,
                    extra_flags=flags, compiler=compiler, compile_env=env)
    return words(cb)


def analyze(c_path, exe_data, verbose=False):
    info = header_info(c_path)
    if not info:
        return None
    addr, size = info
    if not (GAME_LO <= addr < GAME_HI):
        return {'name': c_path.stem, 'addr': addr, 'skip': 'not-game'}
    try:
        cw = compile_words(c_path, addr)
    except SystemExit:
        # apply_postprocessing sys.exit(1)s on banned/painting custom_script.
        return {'name': c_path.stem, 'addr': addr, 'err': 'banned-postproc'}
    except Exception as e:
        return {'name': c_path.stem, 'addr': addr, 'err': str(e)[:60]}

    foff = addr - 0x80041000 + 0x800
    # base reloc-match % at offset 0, current size window
    tw0 = words(exe_data[foff:foff + size])
    n0 = min(len(tw0), len(cw))
    base = sum(reloc_eq(tw0[i], cw[i]) for i in range(n0))
    base_pct = 100.0 * base / max(len(tw0), 1)

    best = None
    # Compiled output has (delta) extra leading words vs the .sym window.
    delta_extra = len(cw) - (size // 4)
    for delta in range(1, 4):
        # window starts delta instructions earlier; size grows by delta words
        a2 = addr - 4 * delta
        if a2 < GAME_LO:
            break
        f2 = a2 - 0x80041000 + 0x800
        sz2 = size + 4 * delta
        tw = words(exe_data[f2:f2 + sz2])
        # apply reloc fixup so jal/lui/addiu imms are masked properly
        n = min(len(tw), len(cw))
        if n == 0:
            continue
        m = sum(reloc_eq(tw[i], cw[i]) for i in range(n))
        pct = 100.0 * m / max(len(tw), 1)
        cand = {'delta': delta, 'start': a2, 'size': sz2, 'pct': pct,
                'n_t': len(tw), 'n_c': len(cw)}
        if best is None or pct > best['pct']:
            best = cand

    return {'name': c_path.stem, 'addr': addr, 'size': size,
            'base_pct': base_pct, 'delta_extra': delta_extra,
            'best': best}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('-c', '--source')
    ap.add_argument('--dir', default='src/main/semi-matched')
    ap.add_argument('--min-best', type=float, default=90.0)
    ap.add_argument('-v', '--verbose', action='store_true')
    args = ap.parse_args()
    os.chdir(ROOT)
    exe_data = open(EXE, 'rb').read()

    if args.source:
        r = analyze(Path(args.source), exe_data, args.verbose)
        print(r)
        return

    files = sorted(glob.glob(f'{args.dir}/*.c'))
    hits = []
    for fp in files:
        r = analyze(Path(fp), exe_data)
        if not r or 'err' in r or not r.get('best'):
            continue
        b = r['best']
        # Candidate: low base%, high windowed%, compiled has extra leading words
        if b['pct'] >= args.min_best and r['base_pct'] < b['pct'] - 5 \
           and r['delta_extra'] >= 1:
            hits.append(r)
            print(f"CAND {r['name']:28s} addr={r['addr']:#x} base={r['base_pct']:5.1f}% "
                  f"-> start={b['start']:#x} d={b['delta']} sz={b['size']} "
                  f"win={b['pct']:5.1f}% (n_t={b['n_t']} n_c={b['n_c']})")
    print(f"\n{len(hits)} cross-prologue candidates (>= {args.min_best}% windowed)")


if __name__ == '__main__':
    main()
