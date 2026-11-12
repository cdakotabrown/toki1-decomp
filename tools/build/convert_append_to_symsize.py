#!/usr/bin/env python3
"""Honest recovery of de-registered append_trailing_instructions overlay funcs.

`append_trailing_instructions` (BANNED) pasted hardcoded instruction words to the
end of .text. Two populations used it:
  (A) HONEST over-count: the function's real body is complete (ends in jr ra) but
      the .sym size runs a few words long into the next, symbol-less function. The
      appended words were just the next function's bytes. The honest fix is
      @SymbolSize (trim the compare window) — adds/changes NO bytes.
  (B) FABRICATION: the C body is incomplete and the appended words were the
      function's OWN missing instructions, copied from the target. No honest fix.

This converter is the self-validating CUTOUT between (A) and (B): for each
candidate it tries @SymbolSize(size - trim_words) and KEEPS the conversion only if
the trimmed body is byte-perfect / reloc-only (i.e. the real body genuinely
matches) AND the trim is small (an over-count, not a rewrite). Fabrication cases
fail the byte-check and are left de-registered. Nothing is fabricated.

Usage: python3 tools/build/convert_append_to_symsize.py [--overlays A,B,..] [--apply]
       (without --apply it only classifies and reports)
"""
import json, os, re, subprocess, argparse
from concurrent.futures import ThreadPoolExecutor, as_completed

ROOT = '.'
os.chdir(ROOT)
DEREG_COMMIT = '4b177270'
MAX_TRIM_WORDS = 8          # an over-count is a few words; bigger = a rewrite
MAX_TRIM_FRACTION = 0.25    # never trim more than a quarter of the function


def candidates():
    """(overlay, func, trim_words) for every append_trailing overlay func that was
    de-registered in the enforcement commit."""
    diff = subprocess.run(['git', 'show', DEREG_COMMIT], capture_output=True, text=True).stdout
    cur = None
    out = []
    for l in diff.splitlines():
        m = re.match(r'diff --git a/(src/overlay/\S+\.c) b/', l)
        if m:
            cur = m.group(1); continue
        if l.startswith('-') and '@PostProc:' in l and 'append_trailing' in l and cur:
            words = len(re.findall(r'0x[0-9a-fA-F]{8}', l))
            out.append((cur.split('/')[2], os.path.splitext(os.path.basename(cur))[0], words))
    return out


def run_test(ov, fn):
    r = subprocess.run(['python3', 'tools/overlay/test_overlay_match.py', ov, fn],
                       capture_output=True, text=True, timeout=150)
    return r.stdout + r.stderr


def insns(o):
    m = re.search(r'(\d+)/(\d+)\s*insns', o)
    return (int(m.group(1)), int(m.group(2))) if m else None


def classify(task):
    ov, fn, words = task
    p = f'src/overlay/{ov}/{fn}.c'
    if not os.path.exists(p) or words <= 0:
        return (ov, fn, words, 'skip')
    o = run_test(ov, fn)
    ii = insns(o)
    if not ii:
        # already reloc/byte without trim, or unparseable
        if 'RELOC-ONLY' in o or 'BYTE-PERFECT' in o:
            return (ov, fn, words, 'already_ok')
        return (ov, fn, words, 'unparseable')
    N, M = ii
    size = M * 4
    newsize = size - words * 4
    if words > MAX_TRIM_WORDS or words * 4 > MAX_TRIM_FRACTION * size or newsize <= 0:
        return (ov, fn, words, 'fabrication(trim-too-large)')
    src = open(p).read()
    nsrc = (re.sub(r'@SymbolSize:\s*\d+', f'@SymbolSize: {newsize}', src)
            if '@SymbolSize' in src
            else src.replace('/*', f'/* @SymbolSize: {newsize} */\n/*', 1))
    open(p, 'w').write(nsrc)
    o2 = run_test(ov, fn)
    ok = ('RELOC-ONLY' in o2) or ('BYTE-PERFECT' in o2) or \
         (insns(o2) and insns(o2)[0] == insns(o2)[1])
    if not ok:
        open(p, 'w').write(src)  # revert; this was fabrication
        return (ov, fn, words, 'fabrication(body-mismatch)')
    if not args.apply:
        open(p, 'w').write(src)  # classify-only
    return (ov, fn, words, 'HONEST')


def main():
    global args
    ap = argparse.ArgumentParser()
    ap.add_argument('--overlays', default='', help='comma list to limit (default all)')
    ap.add_argument('--apply', action='store_true', help='write @SymbolSize for honest cases')
    ap.add_argument('--workers', type=int, default=10)
    args = ap.parse_args()
    cands = candidates()
    if args.overlays:
        keep = set(args.overlays.split(','))
        cands = [c for c in cands if c[0] in keep]
    print(f'{"APPLYING" if args.apply else "CLASSIFYING"} {len(cands)} append_trailing overlay funcs')
    from collections import Counter
    stats = Counter(); honest = []
    with ThreadPoolExecutor(max_workers=args.workers) as ex:
        for fut in as_completed([ex.submit(classify, c) for c in cands]):
            ov, fn, w, verdict = fut.result()
            key = 'HONEST' if verdict == 'HONEST' else (
                'already_ok' if verdict == 'already_ok' else
                'fabrication' if verdict.startswith('fabrication') else verdict)
            stats[key] += 1
            if verdict == 'HONEST':
                honest.append(f'{ov}/{fn}')
    print('\n=== RESULT ===')
    for k, v in sorted(stats.items()):
        print(f'  {k}: {v}')
    print(f'HONEST (recoverable via @SymbolSize): {len(honest)}')
    json.dump(honest, open('/tmp/honest_append.json', 'w'))


if __name__ == '__main__':
    main()
