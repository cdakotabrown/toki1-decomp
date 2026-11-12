#!/usr/bin/env python3
"""Find main-binary symbols that are not function entry points.

A genuine function entry satisfies at least one of:
  * the two words before it are `jr ra` + delay slot (the previous function
    ended there), or
  * its first word is a stack prologue (`addiu sp,sp,-N`), or
  * it is a cross-prologue HEAD: `lui rX,%hi` (+ optional load) immediately
    followed by the host function's prologue -- these are legitimate and are
    handled by absorption, not deletion, or
  * something actually reaches it: a `jal` in .text, or its raw address
    appears as a word anywhere in the image (function-pointer / jr tables).

Anything failing all four is a PHANTOM: an address the disassembler split at
that lands in the middle of a basic block.  It is not code anybody enters, it
inflates the open-function count, and it truncates the `size` of the real
function that encloses it.

Usage:  python3 tools/analysis/audit_phantom_symbols.py [--json OUT]
"""
import argparse
import json
import os
import re
import struct
import sys

# tools/analysis/x.py -> repo root is two dirnames up.  Getting this wrong
# silently breaks every path; the tool prints ROOT under --verbose so it can
# be eyeballed.
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

BASE = 0x80041000
FOFF = 0x800
TEXT_END = 0x800C0000
JR_RA = 0x03E00008
DATA_REGIONS = []


def load_image():
    path = os.path.join(ROOT, 'disks', 'SLPS_000.65')
    with open(path, 'rb') as fh:
        return fh.read()


def _is_prologue(w):
    return (w >> 16) == 0x27BD and (w & 0xFFFF) >= 0x8000


def _prologue_within(words, i, n):
    return any(_is_prologue(words[i + k])
               for k in range(1, n + 1) if i + k < len(words))



def _is_straight_line(words, lo, hi):
    """True if words[lo:hi] is pure fall-through setup (a hoist block)."""
    if hi <= lo:
        return False
    for k in range(lo, hi):
        w = words[k]
        op = w >> 26
        if op in (2, 3):                       # j, jal
            return False
        if op in (4, 5, 6, 7, 1, 0x14, 0x15, 0x16, 0x17):   # branches
            return False
        if op == 0 and (w & 0x3F) in (8, 9):   # jr, jalr
            return False
        # anything that moves the stack pointer is a prologue, not a hoist
        if ((w >> 16) & 0x1F) == 29 and op in (8, 9):        # addi/addiu $sp
            return False
        if op == 0 and ((w >> 11) & 0x1F) == 29:             # R-type writing $sp
            return False
    return True

# A MANUALLY VERIFIED data region -- not a heuristic.
#
# Three automatic tests were tried for this and all three were wrong:
#   * "symbol window has no jr ra"  -> a TRUNCATED window has none either, so
#     it flagged 147 symbols including functions decompiled the same day.
#   * "long run with no jr ra and no prologue" -> that is exactly what the
#     INTERIOR of a large function looks like; it flagged 73 regions, among
#     them the 2,214-word body of serifu_cmd_exec.
#   * "everything after the last jr ra" -> the PS1 BIOS call thunks at
#     0x800B5910 and up return with `jr t2`, never `jr ra`, and 76 call sites
#     target them. Widening to any `jr` then fails too, because DATA WORDS
#     DECODE AS INSTRUCTIONS BY CHANCE -- there are stray words matching the
#     `jr` encoding all the way to 0x800BFF28.
#
# So the boundary is recorded, with its evidence, rather than inferred:
# 0x800BB3C0..0x800C0000 is 4,880 words containing NOT ONE `jr ra` and NOT ONE
# stack prologue, and it disassembles as packed nibble tables
# (0x44444320-style runs) and 16-bit constant tables. Seven symbols totalling
# 10,340 bytes were recorded there as open functions.
DATA_RANGES = [(0x800BB3C0, 0x800C0000)]


def data_regions(words):
    return [((lo - BASE) // 4, (hi - BASE) // 4) for lo, hi in DATA_RANGES]


def classify(img, words, addr, refs, size=None):
    """Return (category, detail) for one symbol address.

    The robust test is positional, not pattern-matching: walk BACK to the
    previous `jr ra` + delay slot.  The word after it is the true entry of
    whatever function encloses `addr`.  Everything else follows from the gap.
    """
    if addr < BASE or addr >= TEXT_END:
        return 'out-of-range', 'not in .text'
    i = (addr - BASE) // 4
    first = words[i]

    for lo, hi in DATA_REGIONS:
        if lo <= i < hi:
            return 'data', ('inside the verified table-data range '
                            '0x%08X..0x%08X -- not code'
                            % (BASE + lo * 4, BASE + hi * 4))

    # Walk back to the previous `jr ra`; its delay slot is the word after.
    j = None
    for k in range(1, 4000):
        if i - k < 0:
            break
        if words[i - k] == JR_RA:
            j = i - k
            break
    if j is None:
        # No return above it within range -- fall back to shape.
        if _is_prologue(first):
            return 'entry', 'own prologue (no jr ra above)'
        if addr in refs:
            return 'referenced', 'reached by %s' % refs[addr]
        return 'unknown', 'no jr ra within scan range'

    true_start = j + 2                      # skip the jr ra and its delay slot
    # Skip inter-function alignment padding (runs of zero words).
    while true_start < i and words[true_start] == 0:
        true_start += 1
    gap = (i - true_start) * 4

    if gap == 0:
        return 'entry', 'follows a jr ra cleanly'
    if gap < 0:
        return 'delay-slot', 'starts in the previous jr ra delay slot'
    if gap > 0 and _is_straight_line(words, true_start, i):
        # IDO -O2 hoists a function's opening global reads ABOVE the prologue.
        # The hoist block is by construction straight-line: no branch, no jump,
        # no `jr`, and no write to $sp -- it simply falls into the prologue.
        # Testing that STRUCTURE rather than a byte threshold matters: the
        # blocks run to 44 bytes and beyond (0x80050F74 hoists 11 words above
        # the prologue at 0x80050FA0), so any fixed cap silently reclassifies
        # long heads as clean entries and understates the host's size.
        return 'head', 'cross-prologue head; real entry 0x%08X (%d bytes earlier)' % (
            BASE + true_start * 4, gap)
    if _is_prologue(first):
        return 'entry', 'own prologue mid-run'
    if addr in refs:
        return 'referenced', 'reached by %s' % refs[addr]
    return 'phantom', 'inside the function at 0x%08X, unreferenced' % (
        BASE + true_start * 4)


def build_refs(img, words):
    """addr -> why it is reachable."""
    refs = {}
    for w in words:
        if (w >> 26) == 3:                        # jal
            refs.setdefault(((w & 0x3FFFFFF) << 2) | 0x80000000, 'jal')
    # raw address words anywhere in the image (fn-ptr tables, jr tables)
    n = len(img) // 4
    for a in struct.unpack('<%dI' % n, img[:n * 4]):
        if BASE <= a < TEXT_END:
            refs.setdefault(a, 'address word')
    return refs


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--json', help='write full results here')
    ap.add_argument('--write-config', action='store_true',
                    help='write config/main_symbol_boundaries.json, the durable '
                         'boundary-correction input consumed by '
                         'audit_main_functions.py')
    ap.add_argument('--verbose', action='store_true')
    args = ap.parse_args()

    if args.verbose:
        print('ROOT = %s' % ROOT)

    img = load_image()
    n = (TEXT_END - BASE) // 4
    words = struct.unpack('<%dI' % n, img[FOFF:FOFF + n * 4])

    audit = os.path.join(ROOT, 'config', 'main_function_audit_by_addr.json')
    with open(audit) as fh:
        entries = json.load(fh)['addresses']

    global DATA_REGIONS
    DATA_REGIONS = data_regions(words)
    refs = build_refs(img, words)
    out = {}
    for key, ent in entries.items():
        addr = int(key, 16)
        cat, why = classify(img, words, addr, refs, ent.get('size'))
        out[key] = {'category': cat, 'why': why,
                    'kind': ent.get('kind'), 'size': ent.get('size'),
                    'region': ent.get('region')}

    tally = {}
    for v in out.values():
        tally[v['category']] = tally.get(v['category'], 0) + 1
    print('%d symbols audited' % len(out))
    for k in sorted(tally, key=lambda x: -tally[x]):
        print('  %-14s %d' % (k, tally[k]))

    ph = sorted((k for k, v in out.items() if v['category'] == 'phantom'),
                key=lambda k: -(out[k]['size'] or 0))
    if ph:
        print('\nPHANTOMS (largest first) -- these are not entry points:')
        for k in ph[:40]:
            v = out[k]
            print('  %s  size=%-6s %-14s %s' % (k, v['size'], v['kind'], v['region']))
        if len(ph) > 40:
            print('  ... and %d more' % (len(ph) - 40))

    if args.write_config:
        cfg = {}
        for key, v in out.items():
            if v['category'] == 'entry':
                continue
            rec = {'category': v['category']}
            m = re.search(r'0x[0-9A-F]{8}', v['why'] or '')
            if m and v['category'] in ('head', 'phantom'):
                rec['true_start'] = m.group(0)
            if v['category'] == 'delay-slot':
                rec['true_start'] = '0x%08X' % (int(key, 16) + 4)
            cfg[key] = rec
        path = os.path.join(ROOT, 'config', 'main_symbol_boundaries.json')
        with open(path, 'w') as fh:
            json.dump({'generated_by': 'tools/analysis/audit_phantom_symbols.py',
                       'boundaries': cfg}, fh, indent=1, sort_keys=True)
        print('\nwrote %s (%d corrections)' % (path, len(cfg)))

    if args.json:
        with open(args.json, 'w') as fh:
            json.dump(out, fh, indent=1, sort_keys=True)
        print('\nwrote %s' % args.json)
    return 0


if __name__ == '__main__':
    sys.exit(main())
