#!/usr/bin/env python3
"""Every non-zero word of the main binary's code must be ACCOUNTED FOR.

`audit_main_functions.py` builds its universe from SYMBOLS -- registry
entries, alias tables, `Address:` headers.  A stretch of retail code that no
symbol names is therefore invisible to it: absent from the numerator AND the
denominator, exactly the defect `tools/overlay/audit_overlay_universe.py`
exposed on the overlay side.  On 2026-10-03 the main audit read "0 open"
while ~50 ranges of real code (schedule_mark_on's 2,348 bytes among them)
were covered by no registered C window and no asm_only listing.

This gate works from the BYTES instead.  It walks the code section of
disks/SLPS_000.65 word by word and marks every word claimed by

  1. a registered entry in config/level3_functions.json (`address`/`size`).
     That is the window the whole build patches; `audit_registry_overlaps.py`
     separately makes `@SymbolStart`/`@SymbolSize` agree with it, so the
     registry IS the honoured window.
  2. an asm_only listing in asm/main/matched/*.s -- its TRUE window, i.e. the
     bytes the listing actually assembles to (GNU as, verified word-for-word
     against retail), not the size some symbol table guessed.  Listings that
     cannot be assembled standalone (absolute branch targets) fall back to
     their `Size:` header and are reported as such.
     ⚠️ asm/main/matched/*.s is .gitignored.  On a fresh checkout regenerate
     the listings first with `make asm-main` or every asm_only window reads
     as uncovered.
  3. config/main_coverage_allowlist.txt -- NON-CODE ranges only (data,
     padding), one per line, each with its evidence.  An allowlist range that
     overlaps a registered or asm_only window is an error: it cannot be both.

Whatever is left is printed.  Runs of zero words are alignment padding and
never a gap; an uncovered run is split at a `jr ra` + delay slot that is
followed by zero padding, so each reported range is roughly one function.

Exit status is non-zero if any uncovered non-zero range remains, if an
asm_only listing disagrees with retail, or if the allowlist is malformed.

    python3 tools/analysis/audit_main_coverage.py            # the gate
    python3 tools/analysis/audit_main_coverage.py --tsv out   # machine-readable
    python3 tools/analysis/audit_main_coverage.py --self-test
"""
import argparse, glob, json, os, re, struct, subprocess, sys, tempfile

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
BIN = os.path.join(ROOT, 'disks/SLPS_000.65')
TEXT_BASE = 0x80041000
FILE_BASE = 0x800
# End of CODE.  The PS-X EXE header's t_size (0xA8000) spans code AND data, so
# it cannot be used.  Evidence for 0x800B5A60: it is the end of the last
# BIOS thunk (StopPAD, 0x800B5A50: li t2,0xB0 / jr t2 / li t1,0x14 / nop), and
# the very next words are Shift-JIS text followed by the ASCII string
# "TOKIMEKI_MEMORIAL_FOREVER WITH YOU" at 0x800B5A90 and then a jump table of
# 0x800427xx..0x800429xx code addresses at 0x800B5AC0 -- .rodata/.data.
CODE_END = 0x800B5A60
ALLOWLIST = os.path.join(ROOT, 'config/main_coverage_allowlist.txt')
ALLOWED_CATEGORIES = ('data', 'padding')
JR_RA = 0x03E00008


# --------------------------------------------------------------------------
# inputs
# --------------------------------------------------------------------------
def load_words(path=BIN, start=TEXT_BASE, end=CODE_END):
    raw = open(path, 'rb').read()
    off = start - TEXT_BASE + FILE_BASE
    n = (end - start) // 4
    return list(struct.unpack('<%dI' % n, raw[off:off + 4 * n]))


def registry_windows():
    reg = json.load(open(os.path.join(ROOT, 'config/level3_functions.json')))
    out = []
    for e in reg['functions']:
        a = int(e['address'], 16)
        n = int(e.get('size') or 0)
        out.append((a, a + n, 'registry', e['name'], e.get('source', '')))
    return out


def _assemble(path):
    """Assembled .text of an asm_only listing, or None if it will not
    assemble standalone (some listings use absolute branch targets)."""
    try:
        from elftools.elf.elffile import ELFFile
    except ImportError:
        return None
    with tempfile.NamedTemporaryFile(suffix='.o', delete=False) as tf:
        obj = tf.name
    try:
        r = subprocess.run(['mipsel-linux-gnu-as', '-march=r3000', '-mabi=32',
                            '-EL', '-no-pad-sections', '-o', obj, path],
                           capture_output=True, text=True)
        if r.returncode:
            return None
        with open(obj, 'rb') as fh:
            elf = ELFFile(fh)
            text = elf.get_section_by_name('.text').data()
            rel = elf.get_section_by_name('.rel.text')
            if rel is not None and rel.num_relocations():
                return None
            return bytes(text)
    except (OSError, FileNotFoundError):
        return None
    finally:
        try:
            os.unlink(obj)
        except OSError:
            pass


def asm_only_windows(words, problems, notes, assemble=True):
    out = []
    for p in sorted(glob.glob(os.path.join(ROOT, 'asm/main/matched/*.s'))):
        txt = open(p, errors='replace').read()
        rel = os.path.relpath(p, ROOT)
        ma = re.search(r'Address:\s*0x([0-9A-Fa-f]+)', txt)
        if not ma:
            problems.append('asm_only %s has no Address: header' % rel)
            continue
        a = int(ma.group(1), 16)
        ms = re.search(r'Size:\s*(\d+)', txt)
        hdr = int(ms.group(1)) if ms else None
        body = _assemble(p) if assemble else None
        if body is not None:
            n = len(body)
            i0 = (a - TEXT_BASE) // 4
            got = struct.unpack('<%dI' % (n // 4), body)
            bad = [k for k, w in enumerate(got)
                   if i0 + k >= len(words) or words[i0 + k] != w]
            if bad:
                problems.append('asm_only %s: %d of %d words differ from retail '
                                '(first at 0x%08X)' % (rel, len(bad), n // 4,
                                                       a + 4 * bad[0]))
            if hdr is not None and hdr != n:
                problems.append('asm_only %s: Size: header %d but the listing '
                                'assembles to %d bytes' % (rel, hdr, n))
        elif hdr is not None:
            n = hdr
            if assemble:
                notes.append('asm_only %s does not assemble standalone; using its '
                             'Size: header (%d B)' % (rel, n))
        else:
            problems.append('asm_only %s: cannot assemble and no Size: header' % rel)
            continue
        name = os.path.splitext(os.path.basename(p))[0]
        out.append((a, a + n, 'asm_only', name, rel))
    return out


def load_allowlist(path, problems):
    """`0xSTART 0xEND category evidence...` (END exclusive)."""
    out = []
    if not os.path.exists(path):
        return out
    for ln, line in enumerate(open(path), 1):
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        parts = line.split(None, 3)
        if len(parts) < 4:
            problems.append('%s:%d: need START END CATEGORY EVIDENCE' % (path, ln))
            continue
        try:
            a, b = int(parts[0], 16), int(parts[1], 16)
        except ValueError:
            problems.append('%s:%d: bad address' % (path, ln))
            continue
        if parts[2] not in ALLOWED_CATEGORIES:
            problems.append('%s:%d: category %r is not one of %s -- code never '
                            'goes on the allowlist' % (path, ln, parts[2],
                                                       ALLOWED_CATEGORIES))
            continue
        if b <= a or a % 4 or b % 4:
            problems.append('%s:%d: bad range' % (path, ln))
            continue
        out.append((a, b, 'allow:' + parts[2], parts[3].strip(), path))
    return out


# --------------------------------------------------------------------------
# core (pure; exercised by --self-test)
# --------------------------------------------------------------------------
def compute_gaps(words, base, windows):
    """Return (gaps, padding_words).  `windows` is [(start, end, ...)].
    A gap is a maximal uncovered run with at least one non-zero word, split
    after any `jr ra` + delay slot that is followed by zero padding, with
    zero words trimmed from both ends."""
    n = len(words)
    cov = bytearray(n)
    for w in windows:
        i0 = max(0, (w[0] - base) // 4)
        i1 = min(n, (w[1] - base + 3) // 4)
        for i in range(i0, i1):
            cov[i] = 1
    gaps, pad = [], 0
    i = 0
    while i < n:
        if cov[i]:
            i += 1
            continue
        j = i
        while j < n and not cov[j]:
            j += 1
        # uncovered run [i, j): split into segments
        seg_start = i
        k = i
        cuts = []
        while k < j:
            if words[k] == JR_RA and k + 2 < j and words[k + 2] == 0:
                cuts.append(k + 2)
            k += 1
        bounds = [i] + cuts + [j]
        for s, e in zip(bounds, bounds[1:]):
            # A zero word in a branch-delay slot is a real instruction (nop),
            # not padding -- whoever owns the branch owns it.
            while s < e and words[s] == 0 and not (s > 0 and is_jump(words[s - 1])):
                s += 1; pad += 1
            while e > s and words[e - 1] == 0 and not (e >= 2 and is_jump(words[e - 2])):
                e -= 1; pad += 1
            if s < e:
                gaps.append((base + 4 * s, base + 4 * e))
        i = j
    return gaps, pad


def is_jump(w):
    """True for any MIPS I instruction that has a delay slot."""
    op = w >> 26
    if op == 0:
        return (w & 0x3F) in (0x08, 0x09)          # jr, jalr
    if op in (0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07):
        return True                                  # regimm, j, jal, b*
    if op in (0x10, 0x11, 0x12, 0x13):
        return ((w >> 21) & 0x1F) == 0x08            # bcz
    return False


def describe(words, base, a, b):
    ws = words[(a - base) // 4:(b - base) // 4]
    jr = sum(1 for w in ws if w == JR_RA)
    nz = sum(1 for w in ws if w)
    cop2 = sum(1 for w in ws if (w >> 26) in (0x12, 0x32, 0x3A))
    prolog = next((k for k, w in enumerate(ws)
                   if (w & 0xFFFF0000) == 0x27BD0000 and (w & 0x8000)), None)
    jal = 0x0C000000 | ((a >> 2) & 0x03FFFFFF)
    callers = sum(1 for w in words if w == jal)
    addrw = sum(1 for w in words if w == a)
    return dict(nonzero=nz, jr_ra=jr, cop2=cop2,
                prologue=('+0x%X' % (4 * prolog)) if prolog is not None else '-',
                callers=callers, addr_words=addrw)


def neighbour(windows, a, b):
    before = max((w for w in windows if w[1] <= a), key=lambda w: w[1], default=None)
    after = min((w for w in windows if w[0] >= b), key=lambda w: w[0], default=None)
    f = lambda w: ('%s@0x%08X+%d' % (w[3], w[0], w[1] - w[0])) if w else '-'
    return f(before), f(after)


def self_test():
    NOP = 0
    fails = 0

    def check(label, got, want):
        nonlocal fails
        ok = got == want
        fails += not ok
        print('  %-58s %s' % (label, 'ok' if ok else 'FAIL got %r want %r' % (got, want)))

    B = 0x1000
    f1 = [0x27BDFFE8, 0xAFBF0010, 0x8FBF0010, JR_RA, 0x27BD0018]  # 5 words
    words = f1 + [NOP, NOP, NOP] + f1 + [NOP, NOP, NOP]
    win_all = [(B, B + 20), (B + 32, B + 52)]
    check('fully covered -> no gaps', compute_gaps(words, B, win_all)[0], [])
    check('zero padding never a gap', compute_gaps(words, B, win_all)[1], 6)
    check('second function uncovered',
          compute_gaps(words, B, win_all[:1])[0], [(B + 32, B + 52)])
    check('short window leaves the tail',
          compute_gaps(words, B, [(B, B + 8), (B + 32, B + 52)])[0], [(B + 8, B + 20)])
    check('two adjacent uncovered functions split at jr ra + pad',
          compute_gaps(words, B, [])[0], [(B, B + 20), (B + 32, B + 52)])
    # function ending in jr ra; nop (delay slot is zero) then padding
    g = [0x24020001, JR_RA, NOP, NOP, 0x24020002, JR_RA, NOP]
    check('jr ra with nop delay slot, then another function',
          compute_gaps(g, B, [])[0], [(B, B + 12), (B + 16, B + 28)])
    # early return inside a function (no padding after the delay slot)
    h = [0x10800002, NOP, JR_RA, 0x24020001, 0x24020002, JR_RA, NOP]
    check('early jr ra without padding does not split',
          compute_gaps(h, B, [])[0], [(B, B + 28)])
    check('all-zero uncovered run is padding', compute_gaps([NOP] * 4, B, [])[0], [])
    # a window that stops one word short, leaving its jr ra's nop delay slot
    k = [0x24020001, JR_RA, NOP, NOP, NOP]
    check('uncovered nop delay slot is code, not padding',
          compute_gaps(k, B, [(B, B + 8)])[0], [(B + 8, B + 12)])
    check('uncovered function keeps its nop delay slot',
          compute_gaps(k, B, [])[0], [(B, B + 12)])
    # allowlist validation
    probs = []
    with tempfile.NamedTemporaryFile('w', suffix='.txt', delete=False) as tf:
        tf.write('# c\n0x1000 0x1010 data jump table, evidence\n'
                 '0x1010 0x1020 code oops\n0x1020 0x1030 data\n')
        name = tf.name
    al = load_allowlist(name, probs)
    os.unlink(name)
    check('allowlist accepts data with evidence', len(al), 1)
    check('allowlist rejects code / missing evidence', len(probs), 2)
    print('self-test: %s' % ('PASS' if not fails else '%d FAIL' % fails))
    return 1 if fails else 0


# --------------------------------------------------------------------------
def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--tsv', help='write the gap list here')
    ap.add_argument('--json', help='write gaps as JSON here (consumed by '
                    'audit_main_functions.py when present)')
    ap.add_argument('--no-assemble', action='store_true',
                    help='trust asm_only Size: headers instead of assembling')
    ap.add_argument('--quiet', action='store_true')
    ap.add_argument('--self-test', action='store_true')
    args = ap.parse_args()
    if args.self_test:
        return self_test()

    words = load_words()
    problems, notes = [], []
    reg = registry_windows()
    asm = asm_only_windows(words, problems, notes, assemble=not args.no_assemble)
    allow = load_allowlist(ALLOWLIST, problems)
    if not asm:
        problems.append('no asm_only listings in asm/main/matched/ -- they are '
                        'gitignored; run `make asm-main` before trusting this gate')
    code = reg + asm
    for a, b, kind, ev, _ in allow:
        for w in code:
            if w[0] < b and a < w[1]:
                problems.append('allowlist 0x%08X-0x%08X (%s) overlaps %s %s '
                                '0x%08X+%d' % (a, b, kind, w[2], w[3], w[0], w[1] - w[0]))
    gaps, pad = compute_gaps(words, TEXT_BASE, code + allow)

    rows = []
    for a, b in gaps:
        d = describe(words, TEXT_BASE, a, b)
        bef, aft = neighbour(code, a, b)
        rows.append(dict(start='0x%08X' % a, end='0x%08X' % b, size=b - a,
                         region='library' if a >= 0x80086000 else 'game',
                         after=bef, before=aft, **d))
    total = sum(r['size'] for r in rows)
    covered = sum(w[1] - w[0] for w in code)
    if not args.quiet:
        print('main code 0x%08X-0x%08X (%d words): %d registry windows, %d '
              'asm_only listings, %d allowlisted non-code ranges'
              % (TEXT_BASE, CODE_END, len(words), len(reg), len(asm), len(allow)))
        print('padding (uncovered zero words): %d' % pad)
        for nline in notes:
            print('note: ' + nline)
        print()
        if rows:
            print('%-11s %6s %5s %4s %4s %5s %5s  %-8s  %s' % (
                'start', 'bytes', 'nz', 'jrra', 'cop2', 'prolog', 'calls', 'region',
                'between'))
            for r in rows:
                print('%-11s %6d %5d %4d %4d %6s %5d  %-8s  %s .. %s' % (
                    r['start'], r['size'], r['nonzero'], r['jr_ra'], r['cop2'],
                    r['prologue'], r['callers'], r['region'], r['after'], r['before']))
            print()
    for p in problems:
        print('PROBLEM: ' + p)
    print('UNCOVERED CODE: %d range(s), %d bytes' % (len(rows), total))
    if args.tsv:
        with open(args.tsv, 'w') as f:
            keys = list(rows[0].keys()) if rows else ['start']
            f.write('\t'.join(keys) + '\n')
            for r in rows:
                f.write('\t'.join(str(r[k]) for k in keys) + '\n')
    if args.json:
        with open(args.json, 'w') as f:
            json.dump({'gaps': rows, 'problems': problems}, f, indent=1)
    return 1 if (rows or problems) else 0


if __name__ == '__main__':
    sys.exit(main())
