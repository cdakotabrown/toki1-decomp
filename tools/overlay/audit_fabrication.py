#!/usr/bin/env python3
"""Tree-wide audit for fabricated matches and unverified byte-perfect claims.

Five independent checks, all of which have produced real findings:

  1. FABRICATED HEAD STUB -- `strip_trailing_instructions` on a <=16-byte symbol.
     IDO -O2 hoists a function's first global read above its prologue; those two
     words have no `jr ra` and are not a function.  A `.c` that returns the
     global (4 insns) and strips the `jr ra; nop` the target never had is
     fabricating a match by subtraction.  Fix: absorb the head into its host
     (tools/overlay/absorb_stub.py), or leave a comment-only ASM-ONLY marker.

  1b. SILENT HEAD STUB -- the same fabrication with NO directive to grep for.
     A <=16-byte symbol with a real compilable body whose target `.s` has no
     `jr` at all: the surplus `jr ra; nop` is discarded by link-window
     truncation instead of by strip_trailing, because the masked compare stops
     at the shorter buffer.  171 tree-wide, 152 counted byte_perfect.

  2. STRIP-ON-REAL-BODY -- `strip_trailing_instructions` on a large body, often
     alongside a big `pad_trailing_nop`.  Not automatically fabrication, but it
     is the shape that hid `pad_trailing_nop --count 181` on a 764-byte semi.
     Reported for human review, never auto-failed.

  3. OVERSIZED pad_trailing_nop -- a pad worth a large fraction of the body is
     the goto-dead-code fake (the C compiles to a few instructions and nops pad
     out the count).  Severity depends on status: at `semi_matched` it inflates
     nothing but hides real work; at `byte_perfect` it is counted as a win.
     Some large pads ARE legitimate -- triage by removing the pad and
     re-resolving.

  4. SYM INTEGRITY -- entries that overlap the next symbol, duplicate addresses
     or names, and gaps.  Host entries over-running 8-16 bytes into the next
     function's head are widespread and interact badly with the truncated
     compare (the over-run bytes are never checked).

Usage:
    audit_fabrication.py                 # audit every overlay
    audit_fabrication.py GEKO DATE       # audit specific overlays
    audit_fabrication.py --strict        # exit 1 if any category-1 finding
"""

import argparse
import collections
import glob
import os
import json
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'build'))

HEAD_STUB_MAX_BYTES = 16
SIZE_RE = re.compile(r'@SymbolSize:\s*(\d+)')
SIZE_COMMENT_RE = re.compile(r'Size:\s*(\d+)\s*bytes')
STRIP_RE = re.compile(r'(?<!\w)strip_trailing_instructions\b')
PAD_RE = re.compile(r'pad_trailing_nop\s+--count\s+(\d+)')


def declared_size(text):
    """@SymbolSize (ANCHORED to a directive line) else the header Size: line."""
    from parse_postproc import parse_symbol_size_text
    size = parse_symbol_size_text(text)
    if size is not None:
        return size
    m = SIZE_COMMENT_RE.search(text)
    return int(m.group(1)) if m else None


DIRECTIVE_LINE_RE = re.compile(r'^\s*(?:/\*+|\*|//)?\s*@PostProc:\s*(.*)$')


def directive_body(line):
    """Return the directive text if `line` IS a directive, else None.

    A line only counts when it OPENS with `@PostProc:` (after comment markers).
    Header prose routinely quotes directives inline -- "rewrite of a
    `@PostProc: pad_trailing_nop --count 78` fake stub" -- and a substring test
    counts those as live, which inflated one overlay's hits from 1 to 16.
    """
    m = DIRECTIVE_LINE_RE.match(line)
    return m.group(1) if m else None


def live_directive(text, pattern):
    """True if `pattern` appears on a real @PostProc directive line."""
    for line in text.splitlines():
        body = directive_body(line)
        if body is not None and pattern.search(body):
            return True
    return False


BODY_RE = re.compile(r'^\s*(?:s32|u32|s16|u16|s8|u8|void|int|char)\s+\w+\s*\(',
                     re.MULTILINE)


def audit_silent_head_stubs(overlays, status):
    """Head stubs that fake a match WITHOUT a strip_trailing directive.

    Same fabrication as category 1, different disguise. The symbol is <=16
    bytes, the `.c` has a real compilable body (`return D_xxxx;` -> 4 insns:
    lui, load, jr ra, nop), and the target `.s` contains no `jr` at all -- it
    is a cross-prologue head. Nothing strips the surplus `jr ra; nop`; instead
    the LINK-WINDOW TRUNCATION does it, because the masked comparison only
    reaches the shorter of target/compiled and stops after 2 words.

    So there is no directive to grep for: the trap itself supplies the fake.
    Found 2026-07-21 by the TEL pass (46 instances there); 171 tree-wide, 152
    of them counted `byte_perfect`.

    Fix is the same as category 1: absorb the head into its host
    (tools/overlay/absorb_stub.py), or leave a comment-only ASM-ONLY marker.
    """
    rows = []
    for overlay in overlays:
        sym = os.path.join(ROOT, 'config', 'overlay', '%s.sym' % overlay)
        if not os.path.exists(sym):
            continue
        for line in open(sym):
            parts = line.split('#')[0].strip().split(':')
            if len(parts) < 3:
                continue
            try:
                size = int(parts[1], 0)
            except ValueError:
                continue
            if size > HEAD_STUB_MAX_BYTES:
                continue
            fn = parts[2]
            c_path = os.path.join(ROOT, 'src', 'overlay', overlay, '%s.c' % fn)
            if not os.path.exists(c_path):
                continue
            text = open(c_path, errors='ignore').read()
            if not BODY_RE.search(text):
                continue        # comment-only ASM-ONLY marker: honest
            asm = os.path.join(ROOT, 'asm', 'overlay', overlay, '%s.s' % fn)
            if not os.path.exists(asm):
                continue
            if 'jr' in open(asm, errors='ignore').read():
                continue        # genuinely returns -> a real function
            entry = status.get(overlay, {}).get(fn)
            st = (entry.get('status') if isinstance(entry, dict) else entry) \
                or 'absent'
            rows.append((overlay, fn, size, st))
    return rows


def audit_pads(overlays, status):
    """Find `pad_trailing_nop` counts too large to be real trailing padding.

    A LEGITIMATE pad adds the handful of zero words a target genuinely ends
    with, on a body that ALREADY matches. A pad worth a large fraction of the
    function is the goto-dead-code fake: the C compiles to a few instructions
    and the rest is nops padding out the instruction count.

    Severity depends on the recorded status. A padded function sitting at
    `semi_matched` inflates nothing -- it is junk hiding real work. One at
    `byte_perfect`/`reloc_only` is being counted as a win.

    Measured 2026-07-21: 479 suspicious, of which 62 counted as wins. Triage of
    one overlay's 40 found 9 legitimate and 31 genuine fakes, and rewriting
    three of the fakes produced byte-perfect matches.
    """
    rows = []
    for overlay in overlays:
        for path in sorted(glob.glob(os.path.join(
                ROOT, 'src', 'overlay', overlay, '*.c'))):
            try:
                text = open(path, errors='ignore').read()
            except OSError:
                continue
            pad = None
            for line in text.splitlines():
                body = directive_body(line)
                if body is None:
                    continue
                m = PAD_RE.search(body)
                if m:
                    pad = int(m.group(1))
                    break
            if pad is None:
                continue
            size = declared_size(text)
            if not size:
                continue
            insns = size // 4
            if insns <= 0 or pad <= 4 or pad / insns < 0.10:
                continue
            fn = os.path.basename(path)[:-2]
            entry = status.get(overlay, {}).get(fn)
            st = (entry.get('status') if isinstance(entry, dict) else entry) \
                or 'absent'
            rows.append((overlay, fn, insns, pad, st))
    return rows


def audit_sources(overlays):
    head_stubs, big_strips = [], []
    for overlay in overlays:
        for path in sorted(glob.glob(os.path.join(
                ROOT, 'src', 'overlay', overlay, '*.c'))):
            try:
                text = open(path, errors='ignore').read()
            except OSError:
                continue
            if not live_directive(text, STRIP_RE):
                continue
            size = declared_size(text)
            rel = os.path.relpath(path, ROOT)
            if size is not None and size <= HEAD_STUB_MAX_BYTES:
                head_stubs.append((rel, size))
            else:
                pad = PAD_RE.search(text)
                big_strips.append((rel, size, int(pad.group(1)) if pad else 0))
    return head_stubs, big_strips


def audit_sym(overlay):
    path = os.path.join(ROOT, 'config', 'overlay', '%s.sym' % overlay)
    if not os.path.exists(path):
        return None
    entries = []
    for line in open(path):
        parts = line.split('#')[0].strip().split(':')
        if len(parts) < 3:
            continue
        try:
            entries.append((int(parts[0], 16), int(parts[1], 0), parts[2]))
        except ValueError:
            continue
    entries.sort()

    overlaps = sum(1 for i in range(len(entries) - 1)
                   if entries[i][0] + entries[i][1] > entries[i + 1][0])
    addrs = collections.Counter(a for a, _s, _n in entries)
    names = collections.Counter(n for _a, _s, n in entries)
    return {
        'entries': len(entries),
        'overlaps': overlaps,
        'dup_addrs': sum(1 for c in addrs.values() if c > 1),
        'dup_names': sum(1 for c in names.values() if c > 1),
    }


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('overlays', nargs='*', help='overlays to audit (default: all)')
    ap.add_argument('--strict', action='store_true',
                    help='exit 1 if any fabricated head stub is found')
    args = ap.parse_args()

    overlays = args.overlays or sorted(
        os.path.basename(p)[:-4]
        for p in glob.glob(os.path.join(ROOT, 'config', 'overlay', '*.sym')))

    head_stubs, big_strips = audit_sources(overlays)
    try:
        status = json.load(open(os.path.join(
            ROOT, 'config', 'overlay_match_status.json')))
    except (OSError, ValueError):
        status = {}
    pads = audit_pads(overlays, status)
    silent = audit_silent_head_stubs(overlays, status)

    print('=' * 68)
    print('1. FABRICATED HEAD STUBS (strip_trailing on a <=%d byte symbol)'
          % HEAD_STUB_MAX_BYTES)
    print('=' * 68)
    if head_stubs:
        by_ov = collections.Counter(p.split(os.sep)[2] for p, _ in head_stubs)
        print('%d found in %d overlays' % (len(head_stubs), len(by_ov)))
        for ov, n in by_ov.most_common():
            print('   %-10s %d' % (ov, n))
        print('\n   Fix: tools/overlay/absorb_stub.py <OV> <STUB> --apply')
    else:
        print('none -- clean')

    print()
    print('=' * 68)
    print('1b. SILENT HEAD STUBS (no directive -- faked by window truncation)')
    print('=' * 68)
    if silent:
        counted = [r for r in silent if r[3] in ('byte_perfect', 'reloc_only')]
        print('%d found; %d COUNTED AS WINS' % (len(silent), len(counted)))
        by_ov = collections.Counter(r[0] for r in silent)
        for ov, n in by_ov.most_common():
            print('   %-10s %d' % (ov, n))
        print('\n   Same fix as 1: absorb_stub.py, or a comment-only ASM-ONLY marker.')
    else:
        print('none -- clean')

    print()
    print('=' * 68)
    print('2. strip_trailing ON A REAL BODY (review by hand, not auto-failed)')
    print('=' * 68)
    if big_strips:
        print('%d found' % len(big_strips))
        for rel, size, pad in sorted(big_strips, key=lambda r: -(r[2] or 0))[:15]:
            note = '  + pad_trailing_nop --count %d' % pad if pad else ''
            print('   %-52s size=%s%s' % (rel, size, note))
        if len(big_strips) > 15:
            print('   ... and %d more' % (len(big_strips) - 15))
    else:
        print('none')

    print()
    print('=' * 68)
    print('3. OVERSIZED pad_trailing_nop (pad > 4 and >= 10% of the body)')
    print('=' * 68)
    if pads:
        counted = [r for r in pads if r[4] in ('byte_perfect', 'reloc_only')]
        print('%d suspicious; %d of them COUNTED AS WINS' % (len(pads), len(counted)))
        by_status = collections.Counter(r[4] for r in pads)
        for st, n in by_status.most_common():
            mark = '  <-- inflating the count' if st in (
                'byte_perfect', 'reloc_only') else ''
            print('   %-16s %d%s' % (st, n, mark))
        print('\n   worst by pad size:')
        for ov, fn, insns, pad, st in sorted(pads, key=lambda r: -r[3])[:12]:
            print('   %-10s %-18s insns=%-5d pad=%-4d %s' % (ov, fn, insns, pad, st))
        print('\n   NOTE: some large pads are legitimate (target really does end')
        print('   in that many zero words). Triage by removing the pad and')
        print('   re-resolving: if the body still matches, the pad was real.')
    else:
        print('none')

    print()
    print('=' * 68)
    print('4. SYM INTEGRITY')
    print('=' * 68)
    print('%-12s %8s %9s %10s %10s' %
          ('overlay', 'entries', 'overlaps', 'dup addrs', 'dup names'))
    totals = collections.Counter()
    for overlay in overlays:
        info = audit_sym(overlay)
        if not info:
            continue
        for key in ('overlaps', 'dup_addrs', 'dup_names'):
            totals[key] += info[key]
        flag = '' if not (info['overlaps'] or info['dup_addrs']
                          or info['dup_names']) else '  <--'
        print('%-12s %8d %9d %10d %10d%s' %
              (overlay, info['entries'], info['overlaps'],
               info['dup_addrs'], info['dup_names'], flag))
    print('%-12s %8s %9d %10d %10d' %
          ('TOTAL', '', totals['overlaps'], totals['dup_addrs'],
           totals['dup_names']))

    if args.strict and (head_stubs or silent):
        print('\nFAIL: %d fabricated head stubs, %d silent head stubs'
              % (len(head_stubs), len(silent)), file=sys.stderr)
        sys.exit(1)


if __name__ == '__main__':
    main()
