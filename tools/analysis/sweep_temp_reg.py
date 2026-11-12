#!/usr/bin/env python3
"""Greedy Hook 36 (IDO52_UGEN_TEMP_REG) sweep for a main-binary function.

Temp-register name mismatches on an otherwise word-exact function are the
class Hook 36 retires: it forces which register ugen pops off the integer
free list for a given expression-temp REQUEST, so the result is an allocation
ugen's own allocator could have produced.  It ranks above PostProc on the
CLAUDE.md ladder, so sweep it before reaching for patch_insn_reg_field.

The sweep is greedy and additive: it keeps a force only if the exact-word
count strictly improves while the instruction COUNT stays equal to the target
and the Fixup count stays 0 -- so it can never "improve" by copying words
from the target or by changing the shape of the function.

    python3 tools/analysis/sweep_temp_reg.py card_file_write [--max-occ 16]

Writes nothing on failure beyond restoring the file it started from.
"""
import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
REGS = ['t%d' % i for i in range(10)]
ENV_RE = re.compile(r'^/\* @CompileEnv: IDO52_UGEN_TEMP_REG=[^*]*\*/\n', re.M)


def measure(src_path, base_text, env):
    text = base_text
    if env:
        line = '/* @CompileEnv: IDO52_UGEN_TEMP_REG=%s */\n' % env
        if '@CompileFlags:' in text:
            text = text.replace('/* @CompileFlags:', line + '/* @CompileFlags:', 1)
        else:
            text = text.rstrip('\n') + '\n' + line
    src_path.write_text(text)
    r = subprocess.run(
        ['python3', 'tools/analysis/test_single_match.py', src_path.stem],
        cwd=ROOT, capture_output=True, text=True, timeout=600)
    exact = re.search(r'Exact:\s+(\d+)/(\d+)', r.stdout)
    comp = re.search(r'Compiled:\s+(\d+)', r.stdout)
    fixup = re.search(r'Fixup:\s+(\d+)', r.stdout)
    if not exact or not comp:
        return None
    return (int(exact.group(1)), int(exact.group(2)),
            int(comp.group(1)), int(fixup.group(1)) if fixup else 0)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('func')
    ap.add_argument('--max-occ', type=int, default=16)
    ap.add_argument('--rounds', type=int, default=3)
    args = ap.parse_args()

    for d in ('matched', 'semi-matched'):
        src = ROOT / 'src/main' / d / (args.func + '.c')
        if src.exists():
            break
    else:
        sys.exit('no such file: %s' % (ROOT / 'src/main/{matched,semi-matched}'
                                       / (args.func + '.c')))
    original = src.read_text()
    base = ENV_RE.sub('', original)

    try:
        best = measure(src, base, '')
        if best is None:
            sys.exit('baseline did not compile')
        print('base exact=%d/%d compiled=%d fixup=%d' % best, flush=True)
        base_fixup = best[3]
        chosen = ''
        for _ in range(args.rounds):
            improved = False
            for occ in range(1, args.max_occ + 1):
                if '%d:' % occ in chosen:
                    continue
                for reg in REGS:
                    cand = (chosen + ',' if chosen else '') + '%d:%s' % (occ, reg)
                    v = measure(src, base, cand)
                    if v is None:
                        continue
                    # never accept a shape change, and never accept MORE
                    # copied words than the baseline already had
                    if v[2] != best[1] or v[3] > base_fixup:
                        continue
                    if v[0] > best[0]:
                        best, chosen, improved = v, cand, True
                        print('  + %-40s exact=%d/%d' % (cand, v[0], v[1]), flush=True)
                        if v[0] == v[1]:
                            measure(src, base, chosen)
                            print('BYTE-PERFECT with IDO52_UGEN_TEMP_REG=%s' % chosen)
                            return
            if not improved:
                break
        measure(src, base, chosen)
        print('best exact=%d/%d with %s' % (best[0], best[1], chosen or '(none)'))
    except BaseException:
        src.write_text(original)
        raise


if __name__ == '__main__':
    main()
