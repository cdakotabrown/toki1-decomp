#!/usr/bin/env python3
"""align_candidates.py -- Stage 0 candidate generator: o.bin debug names ->
retail main-binary functions by PIECEWISE size alignment.

READ-ONLY with respect to the tree.  Writes only
build/analysis/align_candidates.json (or --json OUT).

WHAT IT DOES
  naming_census.py `align` showed that the o.bin debug gap sequence (the
  distance from one extern function to the next in the debug link) aligns
  against retail function sizes far above chance in the game region:
  317 exact-size pairs and 36 runs of 3+ consecutive exact pairs, against
  106-116 exact pairs and 1-2 runs in shuffled-gap controls.  That is a
  CANDIDATE signal, never a name to land: the debug build is a different
  source revision, and statics are missing from o.bin (a debug gap spans the
  extern plus any statics after it).

  This tool turns that finding into a per-retail-address candidate list:

  1. Census alignment.  The same global DP as naming_census.py (imported, so
     the numbers are identical), per band.
  2. Run significance.  Every run of consecutive exact-size pairs is scored
     by its improbability, sum(-log10 p(size)), with p(size) the frequency of
     that size among the debug gaps (the null the shuffles sample).  E-value
     = mean number of runs at least that improbable per shuffled-gap control
     alignment (--controls, default 50).  A 3-run of three 8-byte stubs is
     cheap; a 2-run of 1452 + 2024 is not.  E <= --emax (0.05) = significant.
  3. Off-backbone runs.  A monotone DP cannot represent a translation unit
     that moved.  Diagonal exact/merge runs are seeded over ALL (debug,
     retail) index pairs, scored the same way against shuffled seeds, and a
     significant seed that does not overlap the backbone is reported as a
     REORDER candidate.
  4. Piecewise fill.  Between consecutive significant backbone anchors, the
     unanchored debug and retail entries are re-aligned locally with a
     position term: position error = (debug_addr - retail_addr) minus the
     offset linearly interpolated between the two flanking anchors.  The
     debug/retail offset is NOT constant (+0xA8xx .. +0x117xx in the game
     band); the piecewise fill uses the local offset only.

  Tiers (a candidate is evidence for a reviewer, never a lander):
      A_run        member of a significant backbone run
      B_reorder    member of a significant off-backbone (reordered) run
      C_run_ns     member of a census run of 3+ that is NOT significant
                   (kept as a secondary fill anchor)
      C_fill_exact piecewise fill, exact or 2-merge size, |pos err| <= 64
      D_fill       piecewise fill, anything else
      none         no candidate (retail-only function, or unaligned)

BANDS
  game  o.bin[0:697]   (K-game)                 vs retail 0x80041000-0x80086000
  lib   o.bin[697:1559] (K-sd, K-date, Sony)    vs retail 0x80086000-0x800BB3C0
  The library band is reported separately.  CLAUDE.md forbids assigning a
  Sony name from o.bin position; only whole-member archive identity (T1) or
  source shape (T2) may land one.  Library-band output is a pointer for
  those tests, nothing more.

VALIDATION (--validate, printed by default)
  * every retail address in a census run of 3+ appears with its census name
  * precision against the audit-CONFIRMED names in config/symbol_names.json
  * precision against the independent code readings quoted in
    docs/worklists/SYMBOL-NAMES-REVIEW.md ("rename to X", "o.bin X is a
    candidate", "candidate o.bin X/Y", "likely X", "X-shaped")

Usage:
    python3 tools/analysis/align_candidates.py                 # both bands
    python3 tools/analysis/align_candidates.py --band game --table
    python3 tools/analysis/align_candidates.py --addr 0x8005BE0C
    python3 tools/analysis/align_candidates.py --controls 50 --json OUT.json
"""
import argparse
import bisect
import collections
import json
import math
import os
import random
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import naming_census as NC  # noqa: E402  (read-only reuse; same DP)

ROOT = str(NC.ROOT)
OUT = os.path.join(ROOT, "build/analysis/align_candidates.json")
NAMES = os.path.join(ROOT, "config/symbol_names.json")
REVIEW = os.path.join(ROOT, "docs/worklists/SYMBOL-NAMES-REVIEW.md")
AUDIT = os.path.join(ROOT, "config/main_function_audit_by_addr.json")

BOUNDS = os.path.join(ROOT, "config/main_symbol_boundaries.json")
EXE = os.path.join(ROOT, "disks/SLPS_000.65")
TEXT_BASE, FILE_BASE = 0x80041000, 0x800
JR_RA = 0x03E00008

# Retail function starts the audit JSON does not carry at all, each verified
# by hand against the retail disassembly (and re-verified at run time by
# check_start(): the word 8 bytes before must be a `jr ra`).  Starts the
# audit DOES carry, but at the wrong address (heads, delay slots, phantoms),
# are corrected mechanically from config/main_symbol_boundaries.json instead.
EXTRA_STARTS = {
    0x800425D0: "vblank_callback 0x800425A4 ends `jr ra` 0x800425C8 + delay "
                "slot; the next 6 words (lw D_800F01F0; +887; store; return "
                "it) end in their own `jr ra` 0x800425E0.  The audit had only "
                "the delay slot 0x800425E4 (a `delay-slot` boundary), so the "
                "0x800425A4 window held two functions (44 + 24 = 68).  "
                "naming batch 1/2, 2026-09-27",
}
BANDS = {
    "game": dict(lo=0x80041000, hi=0x80086000, i0=0, i1=697),
    "lib": dict(lo=0x80086000, hi=0x800BB3C0, i0=697, i1=1559),
}


# --------------------------------------------------------------------- data
_EXE = None


def exe_bytes():
    global _EXE
    if _EXE is None:
        _EXE = open(EXE, "rb").read()
    return _EXE


def word(a):
    o = a - TEXT_BASE + FILE_BASE
    b = exe_bytes()[o:o + 4]
    return int.from_bytes(b, "little") if len(b) == 4 else None


def exe_header():
    """PS-X EXE header: pc0 (entry point), gp0, t_addr, t_size."""
    h = exe_bytes()
    if h[:8] != b"PS-X EXE":
        return None
    f = [int.from_bytes(h[0x10 + 4 * i:0x14 + 4 * i], "little")
         for i in range(4)]
    return dict(pc0=f[0], gp0=f[1], t_addr=f[2], t_size=f[3])


def check_start(a):
    """True when retail address A can begin a function: the previous
    function's `jr ra` + delay slot end right before it, allowing zero
    alignment words in between."""
    p = a - 4
    for _ in range(8):
        w = word(p - 4)
        if w == JR_RA:
            return True
        if word(p) != 0 or w is None:
            return False
        p -= 4
    return False


def corrected_retail_sizes(lo, hi, log=None):
    """NC.retail_sizes with the function boundaries corrected from the
    retail disassembly:
      * `head` / `delay-slot` / `phantom` symbols (config/
        main_symbol_boundaries.json, audit_phantom_symbols.py: walks back
        to the previous `jr ra`) move to their true start, so a hoisted
        cross-prologue head no longer splits one function into two
        entries and a phantom no longer truncates its host;
      * `data` symbols are dropped;
      * EXTRA_STARTS adds the hand-verified starts the audit lacks.
    Every start not at the band edge is re-checked against the bytes
    (check_start); a failure is reported in LOG, never silently used."""
    audit = json.load(open(AUDIT))["addresses"]
    try:
        bnd = json.load(open(BOUNDS))["boundaries"]
    except (OSError, ValueError):
        bnd = {}
    starts = set()
    for k, v in audit.items():
        if v["kind"] not in ("matched_c", "asm_only", "head_fragment",
                             "covered"):
            continue
        a = int(k, 16)
        if not lo <= a < hi:
            continue
        b = bnd.get(k) or bnd.get("0x%08X" % a) or {}
        cat = b.get("category")
        if cat == "data":
            if log is not None:
                log.append(("drop-data", a, None))
            continue
        if cat in ("head", "delay-slot", "phantom") and b.get("true_start"):
            t = int(b["true_start"], 16)
            if t != a and log is not None:
                log.append((cat, a, t))
            a = t
        starts.add(a)
    for a in EXTRA_STARTS:
        if lo <= a < hi and a not in starts:
            starts.add(a)
            if log is not None:
                log.append(("extra", a, a))
    ks = sorted(starts)
    if log is not None:
        for a in ks:
            if a != lo and not check_start(a):
                log.append(("unverified", a, None))
    ks = ks + [hi]
    return [(ks[i], ks[i + 1] - ks[i]) for i in range(len(ks) - 1)]


RAW_BOUNDARIES = False      # --raw-boundaries: the pre-2026-09-27 list


def load_band(band, log=None):
    b = BANDS[band]
    funcs, _, _ = NC.load_obin()
    sl = funcs[b["i0"]:b["i1"]]
    names = [f["name"] for f in sl]
    gaps = [f["gap"] for f in sl]
    daddr = [int(f["address"], 16) for f in sl]
    if RAW_BOUNDARIES:
        ret = NC.retail_sizes(b["lo"], b["hi"])
    else:
        ret = corrected_retail_sizes(b["lo"], b["hi"], log)
    return names, gaps, daddr, ret


def current_names():
    """retail address -> current tree name (source basename) or None."""
    out = {}
    try:
        a = json.load(open(AUDIT))["addresses"]
    except (OSError, ValueError):
        return out
    for k, v in a.items():
        src = v.get("source")
        nm = os.path.splitext(os.path.basename(src))[0] if src else None
        out[int(k, 16)] = nm or (v.get("aliases") or [None])[0]
    return out


def b_lo(band):
    return BANDS[band]["lo"]


def b_hi(band):
    return BANDS[band]["hi"]


# ----------------------------------------------------------------- scoring
class Null(object):
    """p(size) = frequency of that size among the debug gaps (smoothed)."""

    def __init__(self, gaps):
        self.c = collections.Counter(g for g in gaps if g is not None)
        self.n = sum(self.c.values())

    def p(self, z):
        return (self.c.get(z, 0) + 0.5) / (self.n + 0.5)

    def bits(self, z):
        return -math.log10(self.p(z))


def census_runs(gaps, pairs):
    """Runs exactly as naming_census.run_stats defines them: consecutive
    debug index, exact size (a 2-merge whose sum is exact counts)."""
    runs, cur, prev = [], [], None
    for k, a, z in pairs:
        ok = gaps[k] == z
        if ok and cur and prev == k - 1:
            cur.append((k, a, z))
        elif ok:
            if cur:
                runs.append(cur)
            cur = [(k, a, z)]
        else:
            if cur:
                runs.append(cur)
            cur = []
        prev = k
    if cur:
        runs.append(cur)
    return runs


def run_score(null, run):
    return sum(null.bits(z) for _, _, z in run)


def gapped_runs(gaps, pairs, ret):
    """Two census runs joined across ONE changed slot: the second run starts
    exactly two debug names and two retail entries after the first one's
    last member, so the single debug name and the single retail entry in
    between pair up by link order whatever their sizes (a function whose
    source was revised between the debug and the retail build).  The slot
    itself scores nothing.  Returns [(left, right, slot)] with
    slot = (debug_idx, retail_addr, retail_size)."""
    ridx = {a: j for j, (a, _) in enumerate(ret)}
    runs = census_runs(gaps, pairs)
    out = []

    def span(m):            # retail indices a member covers (merge = 2)
        k, a, z = m
        j = ridx[a]
        return j, (j if ret[j][1] == z else j + 1)
    for r1, r2 in zip(runs, runs[1:]):
        k1, k2 = r1[-1][0], r2[0][0]
        _, j1e = span(r1[-1])
        j2, _ = span(r2[0])
        if k2 != k1 + 2 or j2 != j1e + 2:
            continue
        slot = (k1 + 1, ret[j1e + 1][0], ret[j1e + 1][1])
        out.append((r1, r2, slot))
    return out


def seed_runs(gaps, sizes, min_len=3):
    """All maximal diagonal runs of exact (1:1) or 2-merge (1:2) size
    matches, over every (debug, retail) start.  Returns list of runs, each a
    list of (debug_idx, retail_idx, n_retail, size)."""
    by1 = collections.defaultdict(list)
    for j, z in enumerate(sizes):
        by1[z].append((j, 1))
    for j in range(len(sizes) - 1):
        by1[sizes[j] + sizes[j + 1]].append((j, 2))

    def step(i, j):
        if i >= len(gaps) or j >= len(sizes) or gaps[i] is None:
            return None
        if sizes[j] == gaps[i]:
            return 1
        if j + 1 < len(sizes) and sizes[j] + sizes[j + 1] == gaps[i]:
            return 2
        return None

    out = []
    for i, g in enumerate(gaps):
        if g is None:
            continue
        for j, n in by1.get(g, ()):
            # maximal start: the previous debug entry must not continue into j
            if i > 0:
                if step(i - 1, j - 1) == 1 or (j >= 2 and step(i - 1, j - 2) == 2):
                    continue
            run, ii, jj, nn = [], i, j, n
            while nn:
                run.append((ii, jj, nn, sum(sizes[jj:jj + nn])))
                ii, jj = ii + 1, jj + nn
                nn = step(ii, jj)
            if len(run) >= min_len:
                out.append(run)
    return out


def seed_score(null, run):
    return sum(null.bits(z) for _, _, _, z in run)


def evalue(real_scores, ctrl_scores, n_ctrl):
    """E(x) = mean #control runs with score >= x."""
    cs = sorted(ctrl_scores)
    out = []
    for s in real_scores:
        k = len(cs) - bisect.bisect_left(cs, s - 1e-9)
        out.append(k / float(n_ctrl))
    return out


# --------------------------------------------------------- piecewise fill
def dp_local(D, R, gaps, sizes, daddr, raddr, off_at, pos_scale=128.0):
    """Monotone alignment of debug indices D against retail indices R (both
    sorted), scored on size AND position error against the interpolated
    offset.  Returns list of (d, j, n_retail, size, poserr)."""
    n, m = len(D), len(R)
    NEG = -1e18
    S = [[NEG] * (m + 1) for _ in range(n + 1)]
    B = [[0] * (m + 1) for _ in range(n + 1)]
    S[0][0] = 0.0

    def sc(di, jlist):
        g = gaps[D[di]]
        z = sum(sizes[R[x]] for x in jlist)
        a = raddr[R[jlist[0]]]
        err = (daddr[D[di]] - a) - off_at(a)
        if g is None:
            base = -1.0
        elif g == z:
            base = 5.0 if len(jlist) == 1 else 4.5
        elif abs(g - z) <= 16:
            base = 2.0
        else:
            base = -2.0
        return base - min(abs(err) / pos_scale, 3.0), z, err

    for i in range(n + 1):
        for j in range(m + 1):
            if i == 0 and j == 0:
                continue
            best, bb = NEG, 0
            if i and S[i - 1][j] - 1 > best:
                best, bb = S[i - 1][j] - 1, 1
            if j and S[i][j - 1] - 1 > best:
                best, bb = S[i][j - 1] - 1, 2
            if i and j:
                v = S[i - 1][j - 1] + sc(i - 1, [j - 1])[0]
                if v > best:
                    best, bb = v, 3
            if i and j > 1 and R[j - 1] == R[j - 2] + 1:
                v = S[i - 1][j - 2] + sc(i - 1, [j - 2, j - 1])[0] - 0.5
                if v > best:
                    best, bb = v, 4
            S[i][j], B[i][j] = best, bb
    i, j, out = n, m, []
    while i or j:
        b = B[i][j]
        if b == 1:
            i -= 1
        elif b == 2:
            j -= 1
        elif b == 3:
            s, z, err = sc(i - 1, [j - 1])
            if s > -1.0:          # keep only pairs better than a double skip
                out.append((D[i - 1], R[j - 1], 1, z, err))
            i, j = i - 1, j - 1
        else:
            s, z, err = sc(i - 1, [j - 2, j - 1])
            if s > -1.0:
                out.append((D[i - 1], R[j - 2], 2, z, err))
            i, j = i - 1, j - 2
    return out[::-1]


# ------------------------------------------------------------------- core
def analyse(band, n_ctrl=20, emax=0.05, seed0=1):
    blog = []
    names, gaps, daddr, ret = load_band(band, blog)
    raddr = [a for a, _ in ret]
    sizes = [z for _, z in ret]
    ridx = {a: j for j, a in enumerate(raddr)}
    null = Null(gaps)

    # 1. census alignment + runs
    pairs = NC.dp_align(gaps, ret)
    cruns = census_runs(gaps, pairs)
    cscores = [run_score(null, r) for r in cruns]

    # 2. controls for DP runs and for seeds
    ctrl_dp, ctrl_len, ctrl_seed, ctrl_gap = [], [], [], []
    for s in range(seed0, seed0 + n_ctrl):
        g = gaps[:]
        random.Random(s).shuffle(g)
        cn = Null(g)
        cp = NC.dp_align(g, ret)
        for r in census_runs(g, cp):
            ctrl_dp.append(run_score(cn, r))
            ctrl_len.append(len(r))
        for r1, r2, _ in gapped_runs(g, cp, ret):
            ctrl_gap.append(run_score(cn, r1) + run_score(cn, r2))
        for r in seed_runs(g, sizes):
            ctrl_seed.append(seed_score(cn, r))
    ev = evalue(cscores, ctrl_dp, n_ctrl)
    lens_sorted = sorted(ctrl_len)
    ev_len = [(len(lens_sorted) - bisect.bisect_left(lens_sorted, len(r)))
              / float(n_ctrl) for r in cruns]

    runs = []
    for rid, (r, s, e, el) in enumerate(zip(cruns, cscores, ev, ev_len)):
        offs = [daddr[k] - a for k, a, _ in r]
        runs.append(dict(
            id="%s-R%03d" % (band, rid), kind="backbone", len=len(r),
            score=round(s, 2), E=round(e, 4), E_len=round(el, 3),
            significant=e <= emax,
            first=names[r[0][0]], last=names[r[-1][0]],
            retail_first="0x%08X" % r[0][1], retail_last="0x%08X" % r[-1][1],
            offset="0x%X" % int(sorted(offs)[len(offs) // 2]),
            offset_min=min(offs), offset_max=max(offs),
            members=[(k, a, z) for k, a, z in r]))

    # 2b. gapped runs: two census runs across one changed slot, scored as
    # the sum of both halves, E against the same statistic on the shuffled
    # controls.  Reported next to the runs; a significant one is the
    # structural half of T3 for its members AND for the slot (whose name is
    # then fixed by link order on both sides, not by size).
    run_of = {}
    for R_ in runs:
        run_of[R_["members"][0][:2]] = R_["id"]
    greal = gapped_runs(gaps, pairs, ret)
    gsc = [run_score(null, r1) + run_score(null, r2) for r1, r2, _ in greal]
    gev = evalue(gsc, ctrl_gap, n_ctrl)
    gruns = []
    for gid, ((r1, r2, slot), sc_, e) in enumerate(zip(greal, gsc, gev)):
        k, a, z = slot
        gruns.append(dict(
            id="%s-G%03d" % (band, gid), kind="gapped",
            len=len(r1) + len(r2), score=round(sc_, 2), E=round(e, 4),
            significant=e <= emax, first=names[r1[0][0]],
            last=names[r2[-1][0]], retail_first="0x%08X" % r1[0][1],
            retail_last="0x%08X" % r2[-1][1],
            halves=[run_of.get(r1[0][:2]), run_of.get(r2[0][:2])],
            slot=dict(name=names[k], debug="0x%08X" % daddr[k],
                      debug_gap=gaps[k], retail="0x%08X" % a,
                      retail_size=z),
            members=[dict(name=names[kk], debug="0x%08X" % daddr[kk],
                          retail="0x%08X" % aa, size=zz)
                     for kk, aa, zz in list(r1) + list(r2)]))

    # 3. diagonal seeds.  The significant DP runs form a monotone chain.  A
    # significant seed that fits into that chain without crossing it is
    # BACKBONE the global DP missed (typically right after a big offset
    # break, where the DP's skip costs hide it) and becomes an anchor; one
    # that crosses the chain is a REORDER candidate.
    chain = []          # sorted (debug_idx, retail_first_idx, retail_last_idx)
    used_d, used_r = set(), set()

    def occupy(mem):
        for k, j, n in mem:
            used_d.add(k)
            used_r.update(range(j, j + n))
            bisect.insort(chain, (k, j, j + n - 1))

    def insertable(d0, d1, r0, r1):
        ds = [c[0] for c in chain]
        p = bisect.bisect_left(ds, d0)
        q = bisect.bisect_right(ds, d1)
        if p != q:
            return False
        if p > 0 and chain[p - 1][2] >= r0:
            return False
        if q < len(chain) and chain[q][1] <= r1:
            return False
        return True

    def mem_of(R_):
        out = []
        for k, a, z in R_["members"]:
            j = ridx[a]
            out.append((k, j, 1 if sizes[j] == z else 2))
        return out

    for R_ in runs:
        if R_["significant"]:
            occupy(mem_of(R_))
    seeds = seed_runs(gaps, sizes)
    sscore = [seed_score(null, r) for r in seeds]
    sev = evalue(sscore, ctrl_seed, n_ctrl)
    order = sorted(range(len(seeds)), key=lambda x: -sscore[x])
    reorders = []       # every accepted seed; kind backbone-seed | reorder
    for x in order:
        if sev[x] > emax:
            break
        r = seeds[x]
        rj = set()
        for i, j, n, _ in r:
            rj.update(range(j, j + n))
        if any(i in used_d for i, _, _, _ in r) or rj & used_r:
            continue
        kind = ("backbone-seed" if insertable(r[0][0], r[-1][0], min(rj),
                                              max(rj)) else "reorder")
        if kind == "backbone-seed":
            occupy([(i, j, n) for i, j, n, _ in r])
        else:
            used_d.update(i for i, _, _, _ in r)
            used_r.update(rj)
        offs = [daddr[i] - raddr[j] for i, j, _, _ in r]
        reorders.append(dict(
            id="%s-S%03d" % (band, len(reorders)), kind=kind,
            len=len(r), score=round(sscore[x], 2), E=round(sev[x], 4),
            significant=True, first=names[r[0][0]], last=names[r[-1][0]],
            retail_first="0x%08X" % raddr[r[0][1]],
            retail_last="0x%08X" % raddr[r[-1][1]],
            offset="0x%X" % offs[0],
            members=[(i, raddr[j], z) for i, j, _, z in r]))

    # 4. anchors = the chain, plus census runs of 3+ that are NOT significant
    # ("weak") where they still fit: secondary anchors, reported C_run_ns, so
    # a census 3+ run address keeps its census candidate wherever nothing
    # stronger displaced it.
    weak = []
    for R_ in runs:
        if R_["significant"] or R_["len"] < 3:
            continue
        mem = mem_of(R_)
        rj = set()
        for k, j, n in mem:
            rj.update(range(j, j + n))
        if any(k in used_d for k, _, _ in mem) or rj & used_r:
            continue
        if not insertable(mem[0][0], mem[-1][0], min(rj), max(rj)):
            continue
        weak.append(R_)
        occupy(mem)
    anchors = [(k, j, daddr[k] - raddr[j]) for k, j, _ in chain]
    anchors.sort()
    fills = []
    bounds = [(-1, -1, None)] + anchors + [(len(gaps), len(ret), None)]
    for (k0, j0, o0), (k1, j1, o1) in zip(bounds, bounds[1:]):
        # j1 of the left anchor may be a 2-merge: skip past its tail
        j0e = j0
        if 0 <= k0 and o0 is not None and gaps[k0] != sizes[j0]:
            j0e = j0 + 1
        D = [k for k in range(k0 + 1, k1) if k not in used_d]
        R = [j for j in range(j0e + 1, j1) if j not in used_r]
        if not D or not R:
            continue
        a0 = raddr[j0] if j0 >= 0 else raddr[0]
        a1 = raddr[j1] if j1 < len(raddr) else raddr[-1] + 4
        oa = o0 if o0 is not None else o1
        ob = o1 if o1 is not None else o0
        if oa is None:
            oa = ob = 0

        def off_at(a, a0=a0, a1=a1, oa=oa, ob=ob):
            if a1 == a0:
                return oa
            t = min(max((a - a0) / float(a1 - a0), 0.0), 1.0)
            return oa + t * (ob - oa)
        if len(D) * len(R) > 250000:
            continue
        for d, j, n, z, err in dp_local(D, R, gaps, sizes, daddr, raddr,
                                        off_at):
            fills.append((d, j, n, z, err))

    # 5. per retail address records
    cur = current_names()
    census_of = {}
    crun_of = {}
    for k, a, z in pairs:
        census_of[a] = (k, z)
    for R_ in runs:
        for k, a, z in R_["members"]:
            crun_of[a] = R_
    rec = {}

    def put(j, k, z, tier, run, err, merged_n):
        a = raddr[j]
        r = dict(retail="0x%08X" % a, retail_size=sizes[j],
                 current=cur.get(a), candidate=names[k],
                 debug_addr="0x%08X" % daddr[k], debug_gap=gaps[k],
                 aligned_size=z,
                 size_match=("exact" if gaps[k] == z and merged_n == 1 else
                             "merge2" if gaps[k] == z else
                             "near" if gaps[k] is not None
                             and abs(gaps[k] - z) <= 16 else "none"),
                 tier=tier, run=run["id"] if run else None,
                 run_len=run["len"] if run else 0,
                 run_E=run["E"] if run else None,
                 pos_err=int(round(err)) if err is not None else None)
        rec[a] = r
        if merged_n == 2 and j + 1 < len(raddr):
            rec[raddr[j + 1]] = dict(r, retail="0x%08X" % raddr[j + 1],
                                     retail_size=sizes[j + 1],
                                     current=cur.get(raddr[j + 1]),
                                     tier=tier + "_tail",
                                     merged_into="0x%08X" % a)

    for R_ in runs:
        if not R_["significant"]:
            continue
        offs = sorted(daddr[k] - a for k, a, _ in R_["members"])
        med = offs[len(offs) // 2]
        for k, a, z in R_["members"]:
            j = ridx[a]
            put(j, k, z, "A_run", R_, daddr[k] - a - med,
                1 if sizes[j] == z else 2)
    for R_ in reorders:
        med = sorted(daddr[i] - a for i, a, _ in R_["members"])
        med = med[len(med) // 2]
        for i, a, z in R_["members"]:
            j = ridx[a]
            put(j, i, z, "A_run" if R_["kind"] == "backbone-seed"
                else "B_reorder", R_, daddr[i] - a - med,
                1 if sizes[j] == z else 2)
    for R_ in weak:
        offs = sorted(daddr[k] - a for k, a, _ in R_["members"])
        med = offs[len(offs) // 2]
        for k, a, z in R_["members"]:
            j = ridx[a]
            if a in rec:
                continue
            put(j, k, z, "C_run_ns", R_, daddr[k] - a - med,
                1 if sizes[j] == z else 2)
    for d, j, n, z, err in fills:
        if raddr[j] in rec:
            continue
        ok = gaps[d] == z and abs(err) <= 64
        put(j, d, z, "C_fill_exact" if ok else "D_fill", None, err, n)
    rows = []
    for j, a in enumerate(raddr):
        r = rec.get(a) or dict(retail="0x%08X" % a, retail_size=sizes[j],
                               current=cur.get(a), candidate=None,
                               tier="none")
        c = census_of.get(a)
        r["census_candidate"] = names[c[0]] if c else None
        cr = crun_of.get(a)
        r["census_run_len"] = cr["len"] if cr else 0
        rows.append(r)

    byaddr = dict((r["retail"], r) for r in rows)
    for G in gruns:
        for m in G["members"]:
            r = byaddr.get(m["retail"])
            if r is not None and (r.get("gap_run_E") is None
                                  or G["E"] < r["gap_run_E"]):
                r.update(gap_run=G["id"], gap_run_E=G["E"],
                         gap_role="member", gap_candidate=m["name"])
        r = byaddr.get(G["slot"]["retail"])
        if r is not None and (r.get("gap_run_E") is None
                              or G["E"] < r["gap_run_E"]):
            r.update(gap_run=G["id"], gap_run_E=G["E"],
                     gap_role="revised slot",
                     gap_candidate=G["slot"]["name"])

    # breaks: offset jumps between consecutive significant backbone runs
    sig = sorted([R_ for R_ in runs if R_["significant"]] +
                 [R_ for R_ in reorders if R_["kind"] == "backbone-seed"],
                 key=lambda R_: R_["members"][0][1])
    breaks = []
    for p, q in zip(sig, sig[1:]):
        o_p = daddr[p["members"][-1][0]] - p["members"][-1][1]
        o_q = daddr[q["members"][0][0]] - q["members"][0][1]
        if abs(o_q - o_p) >= 0x400:
            kp, kq = p["members"][-1][0], q["members"][0][0]
            breaks.append(dict(
                after=p["last"], before=q["first"],
                retail_from=p["retail_last"], retail_to=q["retail_first"],
                offset_from="0x%X" % o_p, offset_to="0x%X" % o_q,
                delta=o_q - o_p, debug_names_between=kq - kp - 1,
                retail_entries_between=ridx[q["members"][0][1]]
                - ridx[p["members"][-1][1]] - 1,
                debug_between=names[kp + 1:kq][:12]))

    summary = dict(
        band=band, debug_names=len(gaps), retail_entries=len(ret),
        census_pairs=len(pairs),
        census_exact=sum(1 for k, a, z in pairs if gaps[k] == z),
        census_runs3=sum(1 for r in cruns if len(r) >= 3),
        census_in_runs3=sum(len(r) for r in cruns if len(r) >= 3),
        controls=n_ctrl, emax=emax,
        control_runs3_per_alignment=round(
            sum(1 for L in ctrl_len if L >= 3) / float(n_ctrl), 2),
        significant_runs=sum(1 for r in runs if r["significant"]),
        significant_pairs=sum(r["len"] for r in runs if r["significant"]),
        significant_runs3=sum(1 for r in runs if r["significant"]
                              and r["len"] >= 3),
        significant_runs2=sum(1 for r in runs if r["significant"]
                              and r["len"] == 2),
        insignificant_runs3=sum(1 for r in runs if not r["significant"]
                                and r["len"] >= 3),
        reorder_runs=sum(1 for r in reorders if r["kind"] == "reorder"),
        backbone_seed_runs=sum(1 for r in reorders
                               if r["kind"] == "backbone-seed"),
        tiers=dict(collections.Counter(r["tier"] for r in rows)),
        gapped_runs=len(gruns),
        gapped_significant=sum(1 for G in gruns if G["significant"]),
        control_gapped_per_alignment=round(len(ctrl_gap) / float(n_ctrl), 2))
    for R_ in runs + reorders:
        R_["members"] = [dict(name=names[k], debug="0x%08X" % daddr[k],
                              retail="0x%08X" % a, size=z)
                         for k, a, z in R_["members"]]
    # EXE-header rule.  The PS-X EXE entry point pc0 is structural evidence
    # independent of the size alignment: it is o.bin's `main` (the crt0
    # target), and the retail function right after it is the debug name
    # right after `main` when the two links agree on that adjacency (checked:
    # the debug successor must directly follow `main` in o.bin).
    exe = exe_header()
    exe_rule = []
    if exe and b_lo(band) <= exe["pc0"] < b_hi(band) and "main" in names:
        k = names.index("main")
        j = ridx.get(exe["pc0"])
        if j is not None:
            exe_rule.append(dict(
                retail="0x%08X" % raddr[j], retail_size=sizes[j],
                candidate="main", debug_gap=gaps[k],
                size_match="exact" if gaps[k] == sizes[j] else "revised",
                basis="PS-X EXE header pc0 = 0x%08X" % exe["pc0"]))
            if j + 1 < len(raddr) and k + 1 < len(names):
                exe_rule.append(dict(
                    retail="0x%08X" % raddr[j + 1],
                    retail_size=sizes[j + 1], candidate=names[k + 1],
                    debug_gap=gaps[k + 1],
                    size_match=("exact" if gaps[k + 1] == sizes[j + 1]
                                else "revised"),
                    basis="link-order successor of the EXE entry (debug "
                          "%s -> %s directly)" % ("main", names[k + 1])))
        for X in exe_rule:
            r = byaddr.get(X["retail"])
            if r is not None:
                r["exe_rule"] = X["candidate"]
    summary["exe_header"] = ({k_: "0x%08X" % v for k_, v in exe.items()}
                             if exe else None)
    blog_out = collections.Counter(k_ for k_, _, _ in blog)
    summary["boundary_corrections"] = dict(blog_out)
    return dict(summary=summary, runs=runs, reorders=reorders,
                gapped=gruns, exe_rule=exe_rule, breaks=breaks, rows=rows,
                boundary_log=[dict(kind=k_, symbol="0x%08X" % a,
                                   true_start=("0x%08X" % t
                                               if t is not None else None))
                              for k_, a, t in blog])


# -------------------------------------------------------------- validation
READ_PATS = [
    (re.compile(r"rename to ([A-Za-z_][A-Za-z0-9_]*)"), "rename to"),
    (re.compile(r"o\.bin ([A-Za-z_][A-Za-z0-9_]*) is a candidate"),
     "o.bin X is a candidate"),
    (re.compile(r"candidate o\.bin ([A-Za-z_][A-Za-z0-9_/]*)"),
     "candidate o.bin X"),
    (re.compile(r"likely ([A-Za-z_][A-Za-z0-9_]*)"), "likely X"),
    (re.compile(r"\b([A-Za-z_][A-Za-z0-9_]*)-shaped"), "X-shaped"),
    (re.compile(r"\(([A-Za-z_][A-Za-z0-9_]*) family\)"), "(X family)"),
]
READ_STOP = {"func", "a", "the", "libgs", "libgpu", "libgte", "libspu",
             "libsnd", "libcd", "libpress", "per", "card", "CdInit"}


def review_readings():
    """retail addr -> (set of names, source phrase) from the review doc's
    refuted/suspect rows.  CdInit is dropped: its row names a DIFFERENT
    address for it."""
    out = {}
    try:
        txt = open(REVIEW).read().splitlines()
    except OSError:
        return out
    for line in txt:
        m = re.match(r"\| (0x[0-9A-Fa-f]{8}) \| `([^`]*)` \|(.*)", line)
        if not m:
            continue
        a, landed, rest = int(m.group(1), 16), m.group(2), m.group(3)
        cells = [c.strip() for c in rest.split("|")]
        action = cells[1] if len(cells) > 1 else ""
        text = action if action else cells[0]
        names, how = set(), []
        for rx, lab in READ_PATS:
            for g in rx.findall(text):
                for n in g.split("/"):
                    if (n and n not in READ_STOP and not n.startswith("func_")
                            and n != landed):
                        names.add(n)
                        how.append(lab)
        if names:
            out.setdefault(a, (set(), []))
            out[a][0].update(names)
            out[a][1].extend(how)
    return out


def _norm(n):
    return n.replace("_", "").lower()


def validate(res_by_band):
    funcs, _, _ = NC.load_obin()
    obin_funcs = set(f["name"] for f in funcs)
    rows = {}
    for band, res in res_by_band.items():
        for r in res["rows"]:
            rows[int(r["retail"], 16)] = (band, r)
    V = {}
    # census 3+ run coverage
    for band, res in res_by_band.items():
        tot = same = 0
        miss = []
        for r in res["rows"]:
            if r["census_run_len"] >= 3:
                tot += 1
                if r.get("candidate") == r["census_candidate"]:
                    same += 1
                else:
                    miss.append((r["retail"], r["census_candidate"],
                                 r.get("candidate"), r["tier"]))
        V["census_run3_coverage_" + band] = dict(
            addresses=tot, present=tot, same_name=same,
            differ=miss[:20])
    # confirmed names
    names = {a: v for a, v in json.load(open(NAMES))["symbols"].items()
             if ":" not in a}    # main records only (overlay keys are "OV:0xADDR")
    conf = [(int(a, 16), v["name"]) for a, v in names.items()
            if (v.get("audit") or {}).get("verdict") == "confirmed"]
    per = collections.defaultdict(lambda: collections.Counter())
    detail = []
    for a, nm in sorted(conf):
        band, r = rows.get(a, (None, None))
        if r is None:
            detail.append(("0x%08X" % a, nm, None, "not-a-retail-entry", None))
            continue
        tier = r["tier"]
        cand = r.get("candidate")
        ok = cand == nm
        per[band][(tier, ok)] += 1
        detail.append(("0x%08X" % a, nm, cand, tier, ok))
    V["confirmed"] = dict(
        total=len(conf),
        in_obin=sum(1 for _, n in conf if n in obin_funcs),
        by_band_tier={b: {"%s:%s" % (t, "hit" if o else "miss"): c
                          for (t, o), c in cnt.items()}
                      for b, cnt in per.items()},
        detail=detail)
    # independent readings
    rd = review_readings()
    det, per2 = [], collections.Counter()
    for a, (nms, how) in sorted(rd.items()):
        band, r = rows.get(a, (None, None))
        inob = sorted(n for n in nms if n in obin_funcs)
        cand = r.get("candidate") if r else None
        tier = r["tier"] if r else "not-a-retail-entry"
        ok = cand in nms if cand else None
        if cand and not ok and any(_norm(cand).endswith(_norm(n))
                                   for n in nms):
            ok = "variant"      # e.g. title_init_View for reading initView
        key = "%s/%s/%s" % (band, "obin" if inob else "non-obin",
                            {True: "hit", "variant": "variant"}.get(
                                ok, "miss" if cand else "nocand"))
        per2[key] += 1
        det.append(("0x%08X" % a, sorted(nms), cand, tier, ok,
                    sorted(set(how))))
    V["readings"] = dict(total=len(rd), summary=dict(per2), detail=det)
    # precision by tier over both truth sets (hit / variant / miss)
    pt = collections.defaultdict(collections.Counter)
    for d in V["confirmed"]["detail"]:
        if d[2]:
            pt["confirmed:" + d[3].replace("_tail", "")][
                "hit" if d[4] else "miss"] += 1
    for d in det:
        if d[2]:
            pt["readings:" + d[3].replace("_tail", "")][
                {True: "hit", "variant": "variant"}.get(d[4], "miss")] += 1
    V["precision_by_tier"] = {k: dict(v) for k, v in sorted(pt.items())}
    return V


# ------------------------------------------------------------------ print
def pr(res, table=False, top=40):
    s = res["summary"]
    print("== band %s: o.bin %d names vs retail %d entries ==" % (
        s["band"], s["debug_names"], s["retail_entries"]))
    print("  census DP: paired %d, exact %d, runs>=3 %d (%d pairs)" % (
        s["census_pairs"], s["census_exact"], s["census_runs3"],
        s["census_in_runs3"]))
    print("  controls: %d shuffles, runs>=3 per control alignment %.2f" % (
        s["controls"], s["control_runs3_per_alignment"]))
    print("  significant (E<=%.2f): %d runs / %d pairs  (len>=3: %d, len 2:"
          " %d); census 3+ runs NOT significant: %d" % (
              s["emax"], s["significant_runs"], s["significant_pairs"],
              s["significant_runs3"], s["significant_runs2"],
              s["insignificant_runs3"]))
    print("  diagonal seeds accepted: %d backbone (missed by the global DP), "
          "%d reorder (cross the backbone)" % (s["backbone_seed_runs"],
                                               s["reorder_runs"]))
    print("  tiers:", dict(sorted(s["tiers"].items())))
    print("\n  -- runs (len>=2 or significant), in retail order")
    print("  %-10s %3s %6s %7s %-10s %-10s %-26s %s" % (
        "id", "len", "score", "E", "retail", "offset", "first", "last"))
    for r in res["runs"]:
        if r["len"] < 2 and not r["significant"]:
            continue
        print("  %-10s %3d %6.1f %7.3f %-10s %-10s %-26s %s%s" % (
            r["id"], r["len"], r["score"], r["E"], r["retail_first"],
            r["offset"], r["first"][:26], r["last"],
            "" if r["significant"] else "   (n.s.)"))
    if res.get("gapped"):
        print("\n  -- gapped runs (two census runs across ONE changed slot;"
              " E vs the same statistic on the controls)")
        print("  %-10s %3s %6s %7s %-10s %-26s %-22s %s" % (
            "id", "len", "score", "E", "retail", "first", "last",
            "changed slot (debug gap / retail size)"))
        for G in res["gapped"]:
            if G["len"] < 3 and not G["significant"]:
                continue
            sl = G["slot"]
            print("  %-10s %3d %6.1f %7.3f %-10s %-26s %-22s %s %s %s/%s%s" % (
                G["id"], G["len"], G["score"], G["E"], G["retail_first"],
                G["first"][:26], G["last"][:22], sl["retail"], sl["name"],
                sl["debug_gap"], sl["retail_size"],
                "" if G["significant"] else "   (n.s.)"))
    if res.get("exe_rule"):
        print("\n  -- EXE-header rule (entry point; independent of sizes)")
        for X in res["exe_rule"]:
            print("  %-10s %5d %-16s debug %-5s %-8s %s" % (
                X["retail"], X["retail_size"], X["candidate"],
                X["debug_gap"], X["size_match"], X["basis"]))
    bl = res.get("boundary_log") or []
    if bl:
        c = collections.Counter(x["kind"] for x in bl)
        print("\n  -- boundary corrections (config/main_symbol_boundaries.json"
              " + EXTRA_STARTS): %s" % dict(sorted(c.items())))
        for x in bl:
            if x["kind"] in ("extra", "unverified"):
                print("      %-10s %s" % (x["kind"], x["symbol"]))
    if res["reorders"]:
        print("\n  -- significant diagonal SEEDS not in the global DP")
        for r in res["reorders"]:
            print("  %-10s %-13s %3d %6.1f %7.3f %-10s %-10s %s .. %s" % (
                r["id"], r["kind"], r["len"], r["score"], r["E"],
                r["retail_first"], r["offset"], r["first"], r["last"]))
    if res["breaks"]:
        print("\n  -- offset BREAKS between consecutive significant runs "
              "(|delta| >= 0x400)")
        for b in res["breaks"]:
            print("  %s..%s  %s -> %s (delta %+d)  debug names between %d, "
                  "retail entries between %d" % (
                      b["retail_from"], b["retail_to"], b["offset_from"],
                      b["offset_to"], b["delta"], b["debug_names_between"],
                      b["retail_entries_between"]))
            print("      after %s / before %s; debug: %s" % (
                b["after"], b["before"], ", ".join(b["debug_between"])))
    if table:
        print("\n  -- candidates (tier != none)")
        print("  %-10s %5s %-26s %-28s %-13s %6s %5s %4s" % (
            "retail", "size", "current", "candidate", "tier", "match",
            "err", "run"))
        for r in res["rows"]:
            if r["tier"] == "none":
                continue
            print("  %-10s %5d %-26s %-28s %-13s %6s %5s %4s" % (
                r["retail"], r["retail_size"], (r["current"] or "")[:26],
                r["candidate"], r["tier"], r["size_match"],
                r["pos_err"], r["run_len"]))


def pr_validation(V):
    print("\n== VALIDATION ==")
    for k in sorted(V):
        if k.startswith("census_run3"):
            v = V[k]
            print("  %s: %d addresses, all present; same name as census %d;"
                  " differ %d" % (k, v["addresses"], v["same_name"],
                                  v["addresses"] - v["same_name"]))
            for d in v["differ"]:
                print("      ", d)
    c = V["confirmed"]
    print("  confirmed names: %d (%d are o.bin names)" % (c["total"],
                                                          c["in_obin"]))
    for b, d in c["by_band_tier"].items():
        print("     ", b, d)
    for d in c["detail"]:
        print("      %s %-24s cand=%-24s %s %s" % (d[0], d[1], d[2], d[3],
                                                   {True: "HIT",
                                                    False: "miss",
                                                    None: ""}[d[4]]))
    print("  precision by tier (only rows that HAVE a candidate):")
    for k, v in V["precision_by_tier"].items():
        print("      %-28s %s" % (k, v))
    r = V["readings"]
    print("  independent readings (review doc): %d addresses" % r["total"])
    print("     ", r["summary"])
    for d in r["detail"]:
        print("      %s reading=%s cand=%s %s %s" % (
            d[0], "/".join(d[1]), d[2], d[3],
            {True: "HIT", False: "miss", None: "",
             "variant": "VARIANT"}[d[4]]))


def main():
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--band", choices=["game", "lib", "both"], default="both")
    ap.add_argument("--controls", type=int, default=50)
    ap.add_argument("--emax", type=float, default=0.05)
    ap.add_argument("--table", action="store_true",
                    help="print every candidate row")
    ap.add_argument("--addr", help="print the record for one retail address")
    ap.add_argument("--json", default=OUT)
    ap.add_argument("--no-validate", action="store_true")
    ap.add_argument("--raw-boundaries", action="store_true",
                    help="use the audit JSON's symbol starts as they are "
                         "(the pre-2026-09-27 behaviour: heads split their "
                         "host, phantoms truncate it)")
    a = ap.parse_args()
    global RAW_BOUNDARIES
    RAW_BOUNDARIES = a.raw_boundaries
    bands = ["game", "lib"] if a.band == "both" else [a.band]
    res = {b: analyse(b, a.controls, a.emax) for b in bands}
    V = None if a.no_validate else validate(res)
    os.makedirs(os.path.dirname(a.json), exist_ok=True)
    with open(a.json, "w") as fh:
        json.dump(dict(generated_by="tools/analysis/align_candidates.py",
                       note="CANDIDATES ONLY -- never land a name from this "
                            "file without code-reading review",
                       bands=res, validation=V), fh, indent=1)
    if a.addr:
        want = int(a.addr, 16)
        for b in res.values():
            for r in b["rows"]:
                if int(r["retail"], 16) == want:
                    print(json.dumps(r, indent=1))
        return
    for b in bands:
        pr(res[b], a.table)
        print()
    if V:
        pr_validation(V)
    print("\nwrote", os.path.relpath(a.json, ROOT))


if __name__ == "__main__":
    main()
