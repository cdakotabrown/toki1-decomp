#!/usr/bin/env python3
"""Every non-zero word of every overlay's CODE must be covered by a verified window.

`audit_overlay_universe.py` counts `.sym` SYMBOLS.  A stretch of retail code
that no symbol names -- or that a symbol names with a window too short to
reach it -- is invisible to it, exactly the defect
`tools/analysis/audit_main_coverage.py` found in the main binary on
2026-10-03 (49 functions no counted entry covered, while the main audit said
"0 open").  This is the overlay sibling.  It works from the BYTES.

For each of the 26 `assets/overlays/<OV>.EXN` (raw images loaded at
0x80132000, no header; the last 4 bytes are a checksum) it marks every word
of the code section claimed by

  1. a `byte_perfect` (or rodata-blocked `reloc_only`) function in
     config/overlay_match_status.json, over the window the AUTHORITATIVE
     resolver compares: the `.sym` entry (last duplicate wins, as in
     resolve_overlay_relocs.parse_sym_file), overridden by the `.c`'s
     anchored `@SymbolStart` / `@SymbolSize`;
  2. an `asm_only` function in that JSON whose blocker is documented
     (config/c_unmatchable.txt or a `Status: ASM-ONLY` `.c` header) and whose
     listing asm/overlay/<OV>/<FUNC>.s ASSEMBLES (build_clean_exn's
     mechanical translator + GNU as + ld at the VA) to exactly the EXN bytes.
     The covered window is what the listing assembles to, never more;
  3. config/overlay_coverage_allowlist.txt -- NON-CODE ranges only
     (`OV 0xSTART 0xEND data|padding evidence...`).  An allowlist range that
     overlaps a verified window is an error.

Where the code section ENDS (there is no section header to read):
  the lowest overlay-local address that VERIFIED code loads or stores through
  a `lui`/`%lo` pair (hi tracking is per basic block and killed by any write
  to the register, so stale pairs do not count).  Code never loads from code;
  IDO links each overlay as .text then .rodata/.data, so the first datum read
  bounds the text from above.  Two EXNs are NOT linked at their load
  address -- GYOZI at 0x80134000, EVENT at 0x800F6000 -- so their jal targets
  and data addresses are runtime addresses while the .sym map is file
  offset + 0x80132000; link_delta() derives the shift from the jal targets of
  verified code (the shift that puts them on window starts) and the data
  references are translated back before taking the minimum.  Cross-checks printed per overlay: the highest `jal` target
  into the overlay (must be below the end) and the last verified window end.

Then every uncovered non-zero run is printed (split at `jr ra` + delay slot
+ zero padding so each range is roughly one function).  Zero words are
padding EXCEPT in a branch-delay slot, which is code.  Overlapping verified
windows are reported too.

Exit status non-zero on any uncovered code, any overlap, any asm_only listing
that disagrees with the EXN, or a malformed allowlist.

    python3 tools/overlay/audit_overlay_coverage.py
    python3 tools/overlay/audit_overlay_coverage.py --overlay GYOZI -v
    python3 tools/overlay/audit_overlay_coverage.py --tsv /tmp/gaps.tsv
    python3 tools/overlay/audit_overlay_coverage.py --self-test
"""
import argparse, glob, json, os, re, struct, sys, tempfile

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'analysis'))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'overlay'))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'build'))
from audit_main_coverage import compute_gaps, is_jump, JR_RA  # noqa: E402
import overlay_names as ON  # noqa: E402   (landed overlay names -> addresses)

BASE = 0x80132000
PAYLOAD = 192 * 1024 - 4
STATUS = os.path.join(ROOT, 'config/overlay_match_status.json')
UNMATCHABLE = os.path.join(ROOT, 'config/c_unmatchable.txt')
ALLOWLIST = os.path.join(ROOT, 'config/overlay_coverage_allowlist.txt')
VERIFIED = ('byte_perfect', 'reloc_only')
ALLOWED_CATEGORIES = ('data', 'padding')

# Overlays whose code never addresses its own EXN data (so the reference rule
# finds nothing).  Value: (code end, evidence).
CODE_END_PIN = {}   # no overlay needs one since link_delta() (see below)

LOADSTORE = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
             0x28, 0x29, 0x2A, 0x2B, 0x2E, 0x32, 0x3A}


# --------------------------------------------------------------------------
# instruction helpers
# --------------------------------------------------------------------------
def dest_reg(w):
    """GPR an instruction writes, or None."""
    op = w >> 26
    if op == 0:
        f = w & 0x3F
        if f in (0x08, 0x0C, 0x0D, 0x11, 0x13, 0x18, 0x19, 0x1A, 0x1B):
            return None                     # jr syscall break mthi mtlo mult/div
        return (w >> 11) & 31
    if op == 3:
        return 31
    if op in (8, 9, 10, 11, 12, 13, 14, 15) or 0x20 <= op <= 0x26:
        return (w >> 16) & 31
    if op in (0x10, 0x11, 0x12, 0x13) and ((w >> 21) & 31) in (0, 2):
        return (w >> 16) & 31               # mfcz / cfcz
    return None


def data_refs(words, spans, with_la=False):
    """[(target, site)] for every lui/%lo LOAD or STORE inside `spans` (word
    index ranges); with_la=True also yields `lui`/`addiu` address formations
    as (target, site, 'la') -- those may name code (function pointers), so
    they never set the code end, only describe a gap.  hi values die on any write to the register, at a jr, and
    after the delay slot of any branch/jump (basic-block local)."""
    out = []
    for i0, i1 in spans:
        hi = {}
        kill_after = -1
        for i in range(i0, i1):
            w = words[i]
            op = w >> 26
            rs = (w >> 21) & 31
            if op in LOADSTORE and rs in hi:
                imm = w & 0xFFFF
                imm -= 0x10000 if imm & 0x8000 else 0
                out.append(((hi[rs] + imm) & 0xFFFFFFFF, BASE + 4 * i))
            elif with_la and op == 9 and rs in hi:
                imm = w & 0xFFFF
                imm -= 0x10000 if imm & 0x8000 else 0
                out.append(((hi[rs] + imm) & 0xFFFFFFFF, BASE + 4 * i, 'la'))
            rd = (w >> 11) & 31
            keep = (op == 0 and (w & 0x3F) in (0x20, 0x21) and rd in hi and
                    rd in (rs, (w >> 16) & 31))         # addu rX,rX,idx (either order)
            d = dest_reg(w)
            if op == 0x0F:
                hi[(w >> 16) & 31] = (w & 0xFFFF) << 16
            elif d is not None and not keep:
                hi.pop(d, None)
            if i == kill_after:
                hi = {}
            if is_jump(w):
                kill_after = i + 1
    return out


# --------------------------------------------------------------------------
# inputs
# --------------------------------------------------------------------------
def load_words(ov):
    raw = open(os.path.join(ROOT, 'assets/overlays/%s.EXN' % ov), 'rb').read()
    return list(struct.unpack('<%dI' % (PAYLOAD // 4), raw[:PAYLOAD]))


def parse_sym(ov):
    """name -> (addr, size), last duplicate wins (resolver semantics), plus
    the list of all entries for duplicate / name-mismatch reporting."""
    d, allent = {}, []
    for line in open(os.path.join(ROOT, 'config/overlay/%s.sym' % ov),
                     errors='replace'):
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        p = line.split(':')
        if len(p) < 3:
            continue
        try:
            a, s = int(p[0], 16), int(p[1])
        except ValueError:
            continue
        d[p[2].strip()] = (a, s)
        allent.append((a, s, p[2].strip()))
    return d, allent


_DIRECTIVE = {}


def c_window(ov, name):
    """(start, size, asm_only_marker) from the .c, anchored parse."""
    p = os.path.join(ROOT, 'src/overlay/%s/%s.c' % (ov, name))
    if not os.path.exists(p):
        return None, None, False
    from parse_postproc import parse_symbol_start_text, parse_symbol_size_text
    t = open(p, errors='ignore').read()
    return (parse_symbol_start_text(t), parse_symbol_size_text(t),
            ('Status: ASM-ONLY' in t or '@AsmOnly' in t))


def load_blocked():
    out = set()
    if os.path.exists(UNMATCHABLE):
        for line in open(UNMATCHABLE, errors='replace'):
            line = line.split('#')[0].strip()
            if line:
                out.add(line.split()[0])
    return out


def assemble_listing(ov, name, addr):
    from pathlib import Path
    from build_clean_exn import assemble_at_va
    s = Path(ROOT) / 'asm/overlay' / ov / ('%s.s' % name)
    if not s.exists():
        return None
    with tempfile.TemporaryDirectory() as td:
        return assemble_at_va(s, addr, Path(td))


def load_allowlist(path, problems):
    """`OV 0xSTART 0xEND category evidence...` (END exclusive)."""
    out = {}
    if not os.path.exists(path):
        return out
    for ln, line in enumerate(open(path), 1):
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        p = line.split(None, 4)
        if len(p) < 5:
            problems.append('%s:%d: need OV START END CATEGORY EVIDENCE'
                            % (os.path.basename(path), ln))
            continue
        try:
            a, b = int(p[1], 16), int(p[2], 16)
        except ValueError:
            problems.append('%s:%d: bad address' % (os.path.basename(path), ln))
            continue
        if p[3] not in ALLOWED_CATEGORIES:
            problems.append('%s:%d: category %r is not one of %s -- code never '
                            'goes on the allowlist'
                            % (os.path.basename(path), ln, p[3],
                               ALLOWED_CATEGORIES))
            continue
        if b <= a or a % 4 or b % 4:
            problems.append('%s:%d: bad range' % (os.path.basename(path), ln))
            continue
        out.setdefault(p[0], []).append((a, b, 'allow:' + p[3], p[4], 'allow'))
    return out


# --------------------------------------------------------------------------
# core
# --------------------------------------------------------------------------
def find_overlaps(windows):
    """Pairs of windows (start, end, kind, name, ...) that share bytes."""
    ws = sorted(windows, key=lambda w: (w[0], w[1]))
    out = []
    active = []
    for w in ws:
        active = [x for x in active if x[1] > w[0]]
        for x in active:
            if x[0] < w[1] and w[0] < x[1]:
                out.append((x, w))
        active.append(w)
    return out


def link_delta(jal_targets, starts):
    """The overlay's link address minus 0x80132000.  Most EXNs are linked at
    their load address, but some are not (GYOZI at 0x80134000, EVENT at
    0x800F6000): their jal targets and data addresses are runtime addresses
    while the .sym map is file offset + 0x80132000.  Returns (delta, votes):
    the shift that puts the most jal targets on a window start, if it beats
    delta 0 by a clear margin; else (0, votes for 0)."""
    st = set(starts)
    zero = sum(1 for t in jal_targets if t in st)
    cand = {}
    for t in sorted(set(jal_targets))[:400]:
        for s in st:
            d = t - s
            if d and -0x80000 < d < 0x80000 and d % 0x1000 == 0:
                cand[d] = cand.get(d, 0) + 1
    best = max(cand.items(), key=lambda kv: kv[1], default=(0, 0))
    if best[1] < 8:
        return 0, zero
    score = sum(1 for t in jal_targets if t - best[0] in st)
    if score > 2 * zero and score >= 8:
        return best[0], score
    return 0, zero


def code_end_from_refs(refs, lo, hi):
    """Lowest load/store target in [lo, hi) and its site, or (None, None)."""
    best = None
    for t, site in refs:
        if lo <= t < hi and (best is None or t < best[0]):
            best = (t, site)
    return best if best else (None, None)


def audit_overlay(ov, status, blocked, allow, assemble=True):
    words = load_words(ov)
    sym, allent = parse_sym(ov)
    st = status.get(ov, {}) if isinstance(status.get(ov), dict) else {}
    problems, notes, wins = [], [], []
    for name, v in sorted(st.items()):
        s = v.get('status') if isinstance(v, dict) else v
        if s not in VERIFIED + ('asm_only',):
            continue
        if name not in sym:
            problems.append('%s %s: in the status JSON but not in the .sym' % (ov, name))
            continue
        a, n = sym[name]
        ca, cn, marker = c_window(ov, name)
        if ca is not None:
            a = ca
        if cn is not None:
            n = cn
        if s in VERIFIED:
            wins.append((a, a + n, s, name, ''))
            continue
        # asm_only: documented blocker + listing that reproduces the EXN
        if not (marker or name in blocked or '%s/%s' % (ov, name) in blocked):
            problems.append('%s %s: asm_only with no blocker (no c_unmatchable '
                            'entry, no Status: ASM-ONLY header)' % (ov, name))
        body = assemble_listing(ov, name, a) if assemble else None
        if body is None:
            if assemble:
                problems.append('%s %s: asm_only listing missing or does not '
                                'assemble -- not counted' % (ov, name))
                continue
            wins.append((a, a + n, 'asm_only', name, 'unassembled'))
            continue
        k = len(body) // 4
        got = struct.unpack('<%dI' % k, body)
        i0 = (a - BASE) // 4
        bad = [j for j in range(k) if words[i0 + j] != got[j]]
        if bad:
            problems.append('%s %s: asm_only listing differs from the EXN in %d '
                            'of %d words (first 0x%08X)' % (ov, name, len(bad), k,
                                                          a + 4 * bad[0]))
            continue
        if len(body) != n:
            notes.append('%s %s: asm_only listing assembles to %d B, window is '
                         '%d B; covering %d B' % (ov, name, len(body), n, len(body)))
        wins.append((a, a + len(body), 'asm_only', name, ''))

    # name-vs-address and duplicate .sym entries (informational)
    seen = {}
    for a, s, name in allent:
        # the address the NAME stands for: func_<ADDR>, or a landed overlay
        # name (apply_overlay_symbols.py) through the scoped name table
        na = ON.func_addr(ov, name)
        # a .sym widened BACKWARDS over an absorbed cross-prologue head (the
        # FRONT class, <= 256 B) is the sanctioned shape; report anything else
        if na is not None and not (0 <= na - a <= 256) and name in st:
            ca, _, _ = c_window(ov, name)
            if ca is None:
                notes.append('%s %s: .sym address 0x%08X differs from its name'
                             % (ov, name, a))
        if name in seen and seen[name] != (a, s):
            notes.append('%s %s: duplicate .sym entries %s / %s (resolver uses '
                         'the last)' % (ov, name, seen[name], (a, s)))
        seen[name] = (a, s)

    # link delta, then code end
    spans = [((w[0] - BASE) // 4, (w[1] - BASE) // 4) for w in wins]
    jals = [((w & 0x3FFFFFF) << 2) | (BASE & 0xF0000000)
            for i0, i1 in spans for w in words[i0:i1] if w >> 26 == 3]
    delta, dscore = link_delta(jals, [w[0] for w in wins] +
                               [ON.func_addr(ov, w[3]) for w in wins
                                if ON.func_addr(ov, w[3]) is not None])
    refs = [(t - delta, site) for t, site in data_refs(words, spans)]
    win_end = max((w[1] for w in wins), default=BASE)
    internal = [t - delta for t in jals if BASE <= t - delta < BASE + PAYLOAD]
    jal_max = max(internal, default=None)
    if ov in CODE_END_PIN:
        end, ev = CODE_END_PIN[ov]
        src = 'pinned: ' + ev
    else:
        t, site = code_end_from_refs(refs, BASE, BASE + PAYLOAD)
        if t is None:
            problems.append('%s: no data reference from verified code; pin the '
                            'code end in CODE_END_PIN with evidence' % ov)
            end, src = win_end, 'last verified window (NO evidence)'
        else:
            end = t & ~3
            src = 'first datum read: 0x%08X by the load at 0x%08X' % (t, site)
    if delta:
        src += ' [linked at 0x%08X: map = runtime %+#x; %d jal targets agree]' % (
            BASE + delta, -delta, dscore)
    beyond = sorted(set(t for t in internal if t >= end))
    if beyond:
        notes.append('%s: %d jal target(s) at/after the code end (cross-overlay '
                     'or co-loaded module calls?): %s' % (
                         ov, len(beyond), ' '.join('0x%08X' % t for t in beyond[:8])))
    for w in wins:
        if w[1] > end:
            problems.append('%s %s: window 0x%08X-0x%08X runs past the code end '
                            '0x%08X (into data)' % (ov, w[3], w[0], w[1], end))

    al = allow.get(ov, [])
    for a, b, kind, ev, _ in al:
        hit = [t for t in internal if a <= t < b]
        if hit:
            problems.append('%s allowlist 0x%08X-0x%08X (%s) is CALLED by verified '
                            'code (jal 0x%08X) -- it is code, not %s'
                            % (ov, a, b, kind, hit[0], kind[6:]))
        for w in wins:
            if w[0] < b and a < w[1]:
                problems.append('%s allowlist 0x%08X-0x%08X overlaps %s %s'
                                % (ov, a, b, w[2], w[3]))
    nwords = (end - BASE) // 4
    gaps, pad = compute_gaps(words[:nwords], BASE, wins + al)
    overlaps = find_overlaps(wins)
    la = [r[0] - delta for r in data_refs(words, spans, with_la=True) if len(r) == 3]
    return dict(ov=ov, words=words, wins=wins, gaps=gaps, pad=pad, end=end, la=la,
                end_src=src, win_end=win_end, jal_max=jal_max,
                overlaps=overlaps, problems=problems, notes=notes)


def text_fraction(words, a, b):
    """Share of the non-zero bytes in [a,b) that parse as Shift-JIS double
    bytes or printable ASCII / newline -- strings are rodata, not code."""
    raw = struct.pack('<%dI' % ((b - a) // 4), *words[(a - BASE) // 4:(b - BASE) // 4])
    ok = tot = 0
    i = 0
    while i < len(raw):
        c = raw[i]
        if c == 0:
            i += 1
            continue
        if (0x81 <= c <= 0x9F or 0xE0 <= c <= 0xEF) and i + 1 < len(raw) \
                and 0x40 <= raw[i + 1] <= 0xFC and raw[i + 1] != 0x7F:
            ok += 2; tot += 2; i += 2
            continue
        tot += 1
        ok += (32 <= c < 127 or c in (9, 10, 13))
        i += 1
    return ok / tot if tot else 0.0


def describe(words, a, b, la=()):
    ws = words[(a - BASE) // 4:(b - BASE) // 4]
    prolog = next((k for k, w in enumerate(ws)
                   if (w & 0xFFFF0000) == 0x27BD0000 and (w & 0x8000)), None)
    jal = 0x0C000000 | ((a >> 2) & 0x03FFFFFF)
    return dict(nonzero=sum(1 for w in ws if w),
                jr_ra=sum(1 for w in ws if w == JR_RA),
                prologue=('+0x%X' % (4 * prolog)) if prolog is not None else '-',
                text='%d%%' % round(100 * text_fraction(words, a, b)),
                la_refs=sum(1 for t in la if a <= t < b),
                callers=sum(1 for w in words if w == jal),
                addr_words=sum(1 for w in words if w == a))


def neighbour(wins, a, b):
    before = max((w for w in wins if w[1] <= a), key=lambda w: w[1], default=None)
    after = min((w for w in wins if w[0] >= b), key=lambda w: w[0], default=None)
    f = lambda w: ('%s@0x%08X+%d' % (w[3], w[0], w[1] - w[0])) if w else '-'
    return f(before), f(after)


# --------------------------------------------------------------------------
def self_test():
    fails = 0

    def check(label, got, want):
        nonlocal fails
        ok = got == want
        fails += not ok
        print('  %-60s %s' % (label, 'ok' if ok else 'FAIL got %r want %r'
                                                   % (got, want)))

    # data_refs: lui at,0x8014 ; lw t0,-16(at) -> 0x8013FFF0
    w = [0x3C018014, 0x8C28FFF0, JR_RA, 0]
    check('lui/lw pair gives a data reference',
          data_refs(w, [(0, 4)]), [(0x8013FFF0, BASE + 4)])
    # stale hi: lui at ; addiu at,sp,4 (overwrites at) ; lw t0,0(at)
    w = [0x3C018014, 0x27A10004, 0x8C280000]
    check('register overwrite kills the hi', data_refs(w, [(0, 3)]), [])
    # hi does not cross a branch: lui at ; b ; nop ; lw t0,0(at)
    w = [0x3C018014, 0x10000001, 0, 0x8C280000]
    check('hi dies after a branch delay slot', data_refs(w, [(0, 4)]), [])
    # jump table form: lui at ; addu at,at,t6 ; lw t6,0x100(at)
    w = [0x3C018014, 0x002E0821, 0x8C2E0100]
    check('addu rX,rX,idx keeps the base (jump table)',
          data_refs(w, [(0, 3)]), [(0x80140100, BASE + 8)])
    # store counts too: lui at ; sw t4,0x3BBC(at)
    w = [0x3C018014, 0xAC2C3BBC]
    check('stores are references', data_refs(w, [(0, 2)]), [(0x80143BBC, BASE + 4)])
    # jal kills ra only, delay slot still sees at -> but then hi resets
    w = [0x3C018014, 0x0C04CA8D, 0xA022F6BF, 0x8C280000]
    check('pair in a jal delay slot counts, then the block ends',
          data_refs(w, [(0, 4)]), [(0x8013F6BF, BASE + 8)])
    check('code_end_from_refs picks the lowest in range',
          code_end_from_refs([(0x80150000, 1), (0x8014FFF8, 2), (0x80100000, 3)],
                             BASE, BASE + PAYLOAD), (0x8014FFF8, 2))
    check('addu rX,idx,rX keeps the base too',
          data_refs([0x3C018014, 0x01C10821, 0x8C2E0100], [(0, 3)]),
          [(0x80140100, BASE + 8)])
    # link delta: jal targets are window starts + 0x2000
    starts = [BASE + 0x100 * k for k in range(40)]
    check('link_delta finds a +0x2000 link shift',
          link_delta([s + 0x2000 for s in starts] * 2, starts)[0], 0x2000)
    check('link_delta keeps 0 when targets hit starts',
          link_delta(starts * 2 + [BASE + 0x2004], starts)[0], 0)
    # overlaps
    ws = [(0, 8, 'b', 'A', ''), (8, 16, 'b', 'B', ''), (12, 20, 'b', 'C', '')]
    check('adjacent windows do not overlap; nested ones do',
          [(x[3], y[3]) for x, y in find_overlaps(ws)], [('B', 'C')])
    ws = [(0, 100, 'b', 'A', ''), (10, 20, 'b', 'B', ''), (30, 40, 'b', 'C', '')]
    check('a big window overlapping two small ones -> two pairs',
          sorted((x[3], y[3]) for x, y in find_overlaps(ws)),
          [('A', 'B'), ('A', 'C')])
    # gaps (shared core with the main gate)
    f1 = [0x27BDFFE8, 0xAFBF0010, 0x8FBF0010, JR_RA, 0x27BD0018]
    words = f1 + [0, 0, 0] + f1
    B = BASE
    check('uncovered second function', compute_gaps(words, B, [(B, B + 20)])[0],
          [(B + 32, B + 52)])
    k = [0x24020001, JR_RA, 0, 0, 0]
    check('nop in an uncovered delay slot is code',
          compute_gaps(k, B, [(B, B + 8)])[0], [(B + 8, B + 12)])
    check('zero padding after jr ra delay slot is not a gap',
          compute_gaps(k, B, [(B, B + 12)])[0], [])
    # allowlist
    probs = []
    with tempfile.NamedTemporaryFile('w', suffix='.txt', delete=False) as tf:
        tf.write('# c\nGEKO 0x80140000 0x80140010 data jump table, evidence\n'
                 'GEKO 0x80140010 0x80140020 code oops evidence\n'
                 'GEKO 0x80140020 0x80140030 data\n')
        name = tf.name
    al = load_allowlist(name, probs)
    os.unlink(name)
    check('allowlist accepts data with evidence', len(al.get('GEKO', [])), 1)
    check('allowlist rejects code / missing evidence', len(probs), 2)
    print('self-test: %s' % ('PASS' if not fails else '%d FAIL' % fails))
    return 1 if fails else 0


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--overlay', action='append', help='limit to these overlays')
    ap.add_argument('--tsv', help='write the gap list here')
    ap.add_argument('--no-assemble', action='store_true',
                    help='trust asm_only windows without assembling the listing')
    ap.add_argument('-v', '--verbose', action='store_true',
                    help='print notes (duplicate .sym entries, name mismatches)')
    ap.add_argument('--self-test', action='store_true')
    args = ap.parse_args()
    if args.self_test:
        return self_test()

    status = json.load(open(STATUS))
    blocked = load_blocked()
    problems = []
    allow = load_allowlist(ALLOWLIST, problems)
    ovs = args.overlay or sorted(os.path.basename(p)[:-4] for p in
                                 glob.glob(os.path.join(ROOT, 'assets/overlays/*.EXN')))
    rows, n_over = [], 0
    print('%-9s %-10s %-10s %-10s %5s %5s %6s  %s' % (
        'overlay', 'code_end', 'win_end', 'jal_max', 'wins', 'gaps', 'bytes',
        'code-end evidence'))
    reports = []
    for ov in ovs:
        r = audit_overlay(ov, status, blocked, allow, assemble=not args.no_assemble)
        reports.append(r)
        gb = sum(b - a for a, b in r['gaps'])
        print('%-9s 0x%08X 0x%08X %-10s %5d %5d %6d  %s' % (
            ov, r['end'], r['win_end'],
            ('0x%08X' % r['jal_max']) if r['jal_max'] else '-',
            len(r['wins']), len(r['gaps']), gb, r['end_src'][:90]))
    print()
    for r in reports:
        for a, b in r['gaps']:
            d = describe(r['words'], a, b, r['la'])
            bef, aft = neighbour(r['wins'], a, b)
            rows.append(dict(ov=r['ov'], start='0x%08X' % a, end='0x%08X' % b,
                             size=b - a, after=bef, before=aft, **d))
        for x, y in r['overlaps']:
            n_over += 1
            problems.append('%s OVERLAP %s 0x%08X-0x%08X (%s) and %s 0x%08X-0x%08X '
                            '(%s)' % (r['ov'], x[3], x[0], x[1], x[2], y[3], y[0],
                                      y[1], y[2]))
        problems.extend(r['problems'])
        if args.verbose:
            for nline in r['notes']:
                print('note: ' + nline)
    if rows:
        print('%-9s %-11s %6s %5s %4s %6s %4s %3s %5s %5s  %s' % (
            'overlay', 'start', 'bytes', 'nz', 'jrra', 'prolog', 'text', 'la',
            'calls', 'addrw', 'between'))
        for r in rows:
            print('%-9s %-11s %6d %5d %4d %6s %4s %3d %5d %5d  %s .. %s' % (
                r['ov'], r['start'], r['size'], r['nonzero'], r['jr_ra'],
                r['prologue'], r['text'], r['la_refs'], r['callers'],
                r['addr_words'], r['after'], r['before']))
        print()
    for p in problems:
        print('PROBLEM: ' + p)
    total = sum(r['size'] for r in rows)
    print('UNCOVERED CODE: %d range(s), %d bytes; %d overlapping window pair(s); '
          '%d problem(s)' % (len(rows), total, n_over,
                             len(problems) - n_over))
    if args.tsv:
        with open(args.tsv, 'w') as f:
            keys = list(rows[0].keys()) if rows else ['ov']
            f.write('\t'.join(keys) + '\n')
            for r in rows:
                f.write('\t'.join(str(r[k]) for k in keys) + '\n')
    return 1 if (rows or problems) else 0


if __name__ == '__main__':
    sys.exit(main())
