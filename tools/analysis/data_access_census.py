#!/usr/bin/env python3
"""data_access_census.py -- Stage 0 access census of main-binary data.

READ-ONLY with respect to the tree.  Writes build/analysis/data_access_census.json
(or --json OUT).

For every main-binary data address (D_XXXXXXXX) it lists every function, in
main and in all 26 overlays, that reads or writes it, with the access width,
signedness, the offset relative to a candidate object base, and the stride of
indexed accesses; then it groups addresses into candidate objects/structs.

WHERE THE FACTS COME FROM
  The compiled code, not the C text.  Registered main C and overlay C are
  byte-identical to retail wherever they match, so the retail instruction
  stream IS what the C says -- with exact widths (lb/lh/lw), exact signedness
  (lb vs lbu), exact direction (load vs store) and exact offsets, none of which
  survive reliably in hand-written C spellings (`*(s16 *)(D_x + 2*i + 20)`,
  local structs, macros).  It also covers the asm_only functions and every
  overlay function that has no C yet.  The C text is scanned separately for
  the D_ NAMES each unit uses (`c_refs`), which is what a rename has to touch.

  Per function, a linear scan tracks register contents symbolically:
      lui rX,hi                    -> hi(K)
      addiu/ori rX,hi(K),lo        -> la(K)          (an address taken)
      sll / multu+mflo / addu / subu of an unknown value v -> lin(c * v)
      addu rX,hi|la(K),lin(c*v)    -> idx(K, stride c)
  and every load/store through such a register is an access:
      direct   l*/s* off(hi(K))      address K+off, the symbol itself
      la       l*/s* off(la(K))      base K, field +off   (C: base+offset)
      indexed  l*/s* off(idx(K,c))   field K+off in records of stride c
  jal/jalr clobber the caller-saved registers after the delay slot; s0-s8
  survive.  No join analysis: a value that differs on two paths into a block
  is taken from the path laid out first.  Heuristic, but every access it
  reports is a real l*/s* word in retail.

OBJECTS
  An interval is made for each la base (base .. max field + width) and for
  each direct or indexed field (its own bytes only), plus a link between
  indexed fields of equal stride less than one stride apart; overlapping
  intervals merge into one candidate object.  Its base is the interval start.
  A la base with far-flung fields (D_800EE760 + 0xF5F) therefore pulls the
  whole block into one object -- which is the point: that is how the C
  addresses it.  Layouts are candidates, to be confirmed by reading
  the C.

Usage:
    python3 tools/analysis/data_access_census.py               # top 60 table
    python3 tools/analysis/data_access_census.py --top 100
    python3 tools/analysis/data_access_census.py --addr 0x800EF6BF
    python3 tools/analysis/data_access_census.py --object 0x800EE760
    python3 tools/analysis/data_access_census.py --no-overlays # main only
"""
import argparse
import collections
import glob
import json
import os
import re
import struct
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
EXE = os.path.join(ROOT, "disks/SLPS_000.65")
AUDIT = os.path.join(ROOT, "config/main_function_audit_by_addr.json")
OUT = os.path.join(ROOT, "build/analysis/data_access_census.json")
TEXT_START, FILE_BASE = 0x80041000, 0x800
TEXT_END = 0x800BB3C0          # verified table-data range starts here
MAIN_LO, MAIN_HI = 0x80010000, 0x80132000   # main image (data below overlays)
OVL_BASE = 0x80132000

LOADS = {0x20: (1, "s"), 0x21: (2, "s"), 0x22: (4, "l"), 0x23: (4, "s"),
         0x24: (1, "u"), 0x25: (2, "u"), 0x26: (4, "r"), 0x32: (4, "c2")}
STORES = {0x28: 1, 0x29: 2, 0x2A: 4, 0x2B: 4, 0x2E: 4, 0x3A: 4}
CALLER_SAVED = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25, 31]
D_RE = re.compile(r"\bD_([0-9A-Fa-f]{8})\b")


# ----------------------------------------------------------- function lists
def main_functions():
    a = json.load(open(AUDIT))["addresses"]
    starts = {}
    for k, v in a.items():
        ad = int(k, 16)
        if not (TEXT_START <= ad < TEXT_END):
            continue
        if v["kind"] not in ("matched_c", "asm_only", "head_fragment",
                             "covered"):
            continue
        if v["kind"] == "covered" and v.get("enclosed_by") not in (None, k):
            continue
        src = v.get("source")
        nm = (os.path.splitext(os.path.basename(src))[0] if src else
              (v.get("aliases") or ["func_%08X" % ad])[0])
        starts[ad] = nm
    ks = sorted(starts) + [TEXT_END]
    return [(ks[i], ks[i + 1] - ks[i], starts[ks[i]])
            for i in range(len(ks) - 1)]


def overlay_functions(ov):
    out = []
    p = os.path.join(ROOT, "config/overlay/%s.sym" % ov)
    seen = set()
    for line in open(p, errors="replace"):
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        parts = line.split(":")
        if len(parts) < 3:
            continue
        try:
            ad, sz = int(parts[0], 16), int(parts[1])
        except ValueError:
            continue
        if ad in seen:
            continue
        seen.add(ad)
        out.append((ad, sz, parts[2].strip()))
    return sorted(out)


def overlays():
    return sorted(os.path.basename(p)[:-4]
                  for p in glob.glob(os.path.join(ROOT, "config/overlay/*.sym")))


# ------------------------------------------------------------------ scanner
def branch_target(w, i):
    """index of a PC-relative branch/j target within the function, or None."""
    op = w >> 26
    if op in (4, 5, 6, 7) or (op == 1 and ((w >> 16) & 31) in (0, 1)):
        off = w & 0xFFFF
        off = off - 0x10000 if off & 0x8000 else off
        return i + 1 + off
    return None


def is_uncond(w):
    op = w >> 26
    if op == 2:                                  # j
        return True
    if op == 4 and ((w >> 21) & 31) == 0 and ((w >> 16) & 31) == 0:
        return True                              # b (beq zero,zero)
    if op == 0 and (w & 63) == 8:                # jr
        return True
    return False


def writes(w):
    """(written GPR or None, constant increment if it is addiu r,r,c)."""
    op = w >> 26
    rs, rt, rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
    if op in (0x09, 0x08):
        imm = w & 0xFFFF
        imm = imm - 0x10000 if imm & 0x8000 else imm
        return rt or None, (imm if rs == rt else None)
    if op in (0x0F, 0x0D, 0x0A, 0x0B, 0x0C, 0x0E) or op in LOADS and op != 0x32:
        return rt or None, None
    if op in (0x10, 0x12) and rs in (0, 2):
        return rt or None, None
    if op == 0:
        fu = w & 63
        if w == 0 or fu in (0x08, 0x0C, 0x0D, 0x1A, 0x1B, 0x11, 0x13,
                            0x18, 0x19):
            return None, None
        return rd or None, None
    if op == 3:
        return 31, None
    return None, None


class Scanner(object):
    """Symbolic register scan of one function.  Values:
       ('c', n)            constant
       ('lin', c, v)       c * (unknown v)
       ('hi', K)           lui result
       ('la', K)           full address
       ('idx', K, c, part) K + c*v ; part=True if K came from a bare lui"""

    def __init__(self):
        self.var = 0

    def fresh(self):
        self.var += 1
        return ("lin", 1, self.var)

    def scan(self, words, fn_addr, sink):
        """Forward scan with a cheap control-flow model: states flowing along
        forward branches are MET at their targets (a register keeps its value
        only if every incoming path agrees); at a loop head, every register
        the loop body writes is killed, except a pointer that the body only
        bumps by a constant (`addiu r,r,c`), which becomes an indexed base
        with stride c."""
        n_w = len(words)
        incoming = collections.defaultdict(list)
        heads = {}
        for i, w in enumerate(words):
            t = branch_target(w, i)
            if t is not None and 0 <= t <= i and i + 1 < n_w:
                wrs, incs = {}, {}
                for k in range(t, min(i + 2, n_w)):
                    r, inc = writes(words[k])
                    if r:
                        if inc is not None and wrs.get(r, "inc") == "inc":
                            wrs[r] = "inc"
                            incs[r] = inc
                        else:
                            wrs[r] = "other"
                h = heads.setdefault(t, ({}, {}))
                for r, kind in wrs.items():
                    if h[0].get(r) != "other":
                        h[0][r] = kind
                    if kind == "inc":
                        h[1][r] = incs[r]
        R = {i: self.fresh() for i in range(32)}
        R[0] = ("c", 0)
        st = dict(R=R, hilo=None, pending=0)
        uncond_prev = False
        for n, w in enumerate(words):
            if n in incoming:
                ins = incoming.pop(n)
                if not uncond_prev:
                    ins = ins + [st]
                st = self.meet(ins)
            if n in heads:
                wrs, incs = heads[n]
                R = st["R"]
                for r, kind in wrs.items():
                    v = R.get(r)
                    if (kind == "inc" and v and v[0] in ("la", "idx", "hi")):
                        stride = v[2] if v[0] == "idx" and v[2] else abs(incs[r])
                        R[r] = ("idx", v[1], stride,
                                v[3] if v[0] == "idx" else v[0] == "hi")
                    else:
                        R[r] = self.fresh()
            self.step(st, w, fn_addr + 4 * n, sink)
            # a branch's state (after its delay slot) flows to its target
            if n >= 1:
                t = branch_target(words[n - 1], n - 1)
                if t is not None and t > n:
                    incoming[t].append(self.copy(st))
            uncond_prev = n >= 1 and is_uncond(words[n - 1])

    def copy(self, st):
        return dict(R=dict(st["R"]), hilo=st["hilo"], pending=st["pending"])

    def meet(self, sts):
        R = {}
        for r in range(32):
            v = sts[0]["R"].get(r)
            if all(s["R"].get(r) == v for s in sts[1:]):
                R[r] = v
            else:
                R[r] = self.fresh()
        R[0] = ("c", 0)
        return dict(R=R, hilo=None, pending=max(s["pending"] for s in sts))

    def step(self, st, w, pc, sink):
        R = st["R"]
        hilo = st["hilo"]
        op = w >> 26
        rs, rt, rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        sh, fu = (w >> 6) & 31, w & 63
        imm = w & 0xFFFF
        simm = imm - 0x10000 if imm & 0x8000 else imm
        wr, val = None, None
        clob = False
        if op == 0x0F:
            wr, val = rt, ("hi", (imm << 16) & 0xFFFFFFFF)
        elif op in (0x09, 0x08):
            s = R[rs]
            wr = rt
            if s[0] == "hi":
                val = ("la", (s[1] + simm) & 0xFFFFFFFF)
                sink(pc, "addr", val[1], 0, "", "la", val[1], 0)
            elif s[0] == "la":
                val = ("la", (s[1] + simm) & 0xFFFFFFFF)
            elif s[0] == "idx":
                val = ("idx", (s[1] + simm) & 0xFFFFFFFF, s[2], s[3])
            elif s[0] == "c":
                val = ("c", s[1] + simm)
            elif s[0] == "lin":
                val = s
        elif op == 0x0D:
            s = R[rs]
            wr = rt
            if s[0] == "hi":
                val = ("la", s[1] | imm)
                sink(pc, "addr", val[1], 0, "", "la", val[1], 0)
            elif s[0] == "c":
                val = ("c", s[1] | imm)
        elif op in LOADS or op in STORES:
            s = R[rs]
            if op in LOADS:
                width, sg = LOADS[op]
                rw = "R"
            else:
                width, sg, rw = STORES[op], "", "W"
            if s[0] == "hi":
                a = (s[1] + simm) & 0xFFFFFFFF
                sink(pc, rw, a, width, sg, "direct", a, 0)
            elif s[0] == "la":
                a = (s[1] + simm) & 0xFFFFFFFF
                sink(pc, rw, a, width, sg, "la", s[1], 0)
            elif s[0] == "idx":
                a = (s[1] + simm) & 0xFFFFFFFF
                base = a if s[3] else s[1]
                sink(pc, rw, a, width, sg, "indexed", base, s[2])
            if op in LOADS and op != 0x32:
                wr = rt
        elif op == 0:
            if fu in (0x21, 0x20, 0x25):       # addu add or
                a, b = R[rs], R[rt]
                wr = rd
                if fu == 0x25 or b == ("c", 0) or a == ("c", 0):
                    if b == ("c", 0):
                        val = a
                    elif a == ("c", 0):
                        val = b
                if val is None and fu != 0x25:
                    val = self.add(a, b)
            elif fu in (0x23, 0x22):           # subu
                a, b = R[rs], R[rt]
                wr = rd
                if a[0] == "lin" and b[0] == "lin" and a[2] == b[2]:
                    val = ("lin", a[1] - b[1], a[2])
                elif b == ("c", 0):
                    val = a
            elif fu == 0x00:                   # sll
                wr = rd
                s = R[rt]
                if w == 0:
                    wr = None
                elif s[0] == "lin":
                    val = ("lin", s[1] << sh, s[2])
                elif s[0] == "c":
                    val = ("c", s[1] << sh)
            elif fu in (0x18, 0x19):           # mult multu
                a, b = R[rs], R[rt]
                hilo = None
                if a[0] == "c" and b[0] == "lin":
                    hilo = ("lin", a[1] * b[1], b[2])
                elif b[0] == "c" and a[0] == "lin":
                    hilo = ("lin", b[1] * a[1], a[2])
            elif fu == 0x12:                   # mflo
                wr, val = rd, hilo
            elif fu == 0x09:                   # jalr
                wr = rd
                clob = True
            elif fu in (0x08, 0x0C, 0x0D, 0x1A, 0x1B, 0x11, 0x13):
                wr = None                      # jr syscall break div mt*
            else:
                wr = rd
        elif op == 0x03:                       # jal
            clob = True
        elif op in (0x0A, 0x0B, 0x0C, 0x0E):   # slti sltiu andi xori
            wr = rt
        elif op in (0x10, 0x12):               # mfc0/cfc0/mfc2/cfc2
            if rs in (0, 2):
                wr = rt
        if wr:
            R[wr] = val if val is not None else self.fresh()
        if st["pending"]:
            st["pending"] -= 1
            if st["pending"] == 0:
                for r in CALLER_SAVED:
                    R[r] = self.fresh()
        if clob:
            st["pending"] = 1
        st["hilo"] = hilo

    def add(self, a, b):
        for x, y in ((a, b), (b, a)):
            if x[0] in ("hi", "la") and y[0] == "lin":
                return ("idx", x[1], y[1], x[0] == "hi")
            if x[0] in ("hi", "la") and y[0] == "c":
                return (x[0], (x[1] + y[1]) & 0xFFFFFFFF)
            if x[0] == "idx" and y[0] == "lin":
                return ("idx", x[1], None, x[3])
        if a[0] == "lin" and b[0] == "lin" and a[2] == b[2]:
            return ("lin", a[1] + b[1], a[2])
        if a[0] == "c" and b[0] == "c":
            return ("c", a[1] + b[1])
        return None


# -------------------------------------------------------------- collection
def collect(include_overlays=True):
    acc = []      # dicts
    sc = Scanner()
    exe = open(EXE, "rb").read()

    def run(unit, fns, getw):
        for ad, sz, nm in fns:
            words = getw(ad, sz)
            if not words:
                continue
            fid = "%s:%s" % (unit, nm)

            def sink(pc, rw, a, width, sg, via, base, stride, fid=fid):
                if not (MAIN_LO <= a < MAIN_HI):
                    return
                if via != "direct" and not (MAIN_LO <= base < MAIN_HI):
                    return
                acc.append((fid, pc, rw, a, width, sg, via, base, stride))
            sc.scan(words, ad, sink)

    def main_words(ad, sz):
        o = ad - TEXT_START + FILE_BASE
        b = exe[o:o + sz]
        return list(struct.unpack("<%dI" % (len(b) // 4), b[:len(b) // 4 * 4]))
    run("main", main_functions(), main_words)
    if include_overlays:
        for ov in overlays():
            p = os.path.join(ROOT, "assets/overlays/%s.EXN" % ov)
            if not os.path.exists(p):
                continue
            data = open(p, "rb").read()

            def ov_words(ad, sz, data=data):
                o = ad - OVL_BASE
                b = data[o:o + sz]
                return list(struct.unpack("<%dI" % (len(b) // 4),
                                          b[:len(b) // 4 * 4]))
            run(ov, overlay_functions(ov), ov_words)
    return acc


def c_refs():
    """D_ name -> (refs, set of units) over main + overlay C."""
    refs = collections.Counter()
    units = collections.defaultdict(set)
    pats = [("main", "src/main/matched/*.c"), ("main", "src/main/semi-matched/*.c")]
    for ov in overlays():
        pats.append((ov, "src/overlay/%s/*.c" % ov))
    for unit, pat in pats:
        for p in glob.glob(os.path.join(ROOT, pat)):
            try:
                t = open(p, errors="replace").read()
            except OSError:
                continue
            fn = os.path.splitext(os.path.basename(p))[0]
            for m in D_RE.finditer(t):
                a = int(m.group(1), 16)
                if unit != "main" and a >= OVL_BASE:
                    continue          # overlay-local
                refs[a] += 1
                units[a].add("%s:%s" % (unit, fn))
    return refs, units


DECL_RE = re.compile(
    r"extern\s+(?:volatile\s+)?(unsigned\s+char|signed\s+char|unsigned\s+short"
    r"|unsigned\s+int|unsigned\s+long|u8|s8|u16|s16|u32|s32|char|short|int|long)"
    r"\s+(?:volatile\s+)?D_([0-9A-Fa-f]{8})\s*;")
DECL_TY = {"u8": (1, "u"), "unsigned char": (1, "u"), "s8": (1, "s"),
           "signed char": (1, "s"), "char": (1, "s"), "u16": (2, "u"),
           "unsigned short": (2, "u"), "s16": (2, "s"), "short": (2, "s"),
           "u32": (4, ""), "s32": (4, ""), "int": (4, ""), "long": (4, ""),
           "unsigned int": (4, ""), "unsigned long": (4, "")}


def validate_decls(acc):
    """Self-check: scalar `extern T D_x;` declarations in the C against the
    loads the scanner saw at exactly D_x (direct %hi/%lo loads only).
    Agreement = the declared width is among the loaded widths and, for
    8/16-bit, the declared signedness is among the load signedness."""
    seen = collections.defaultdict(set)
    for fid, pc, rw, a, w, sg, via, base, stride in acc:
        if rw == "R" and via == "direct" and sg in ("s", "u"):
            seen[a].add((w, sg))
        elif rw == "R" and via == "direct" and w == 4:
            seen[a].add((4, ""))
    decl = collections.defaultdict(collections.Counter)
    for p in glob.glob(os.path.join(ROOT, "src/main/matched/*.c")) + \
            glob.glob(os.path.join(ROOT, "src/overlay/*/*.c")):
        try:
            t = open(p, errors="replace").read()
        except OSError:
            continue
        for m in DECL_RE.finditer(t):
            decl[int(m.group(2), 16)][" ".join(m.group(1).split())] += 1
    agree = width_only = disagree = 0
    bad = []
    for a, tys in decl.items():
        if a not in seen:
            continue
        ty = tys.most_common(1)[0][0]
        w, sg = DECL_TY[ty]
        ws = set(x[0] for x in seen[a])
        if w not in ws:
            disagree += 1
            bad.append(("D_%08X" % a, ty, sorted(seen[a])))
        elif w == 4 or (w, sg) in seen[a]:
            agree += 1
        else:
            width_only += 1
    return dict(checked=agree + width_only + disagree, agree=agree,
                width_only=width_only, width_disagree=disagree,
                examples=bad[:15])


# ---------------------------------------------------------------- grouping
def group(acc, crefs):
    iv = []
    for fid, pc, rw, a, w, sg, via, base, stride in acc:
        if via == "la":
            iv.append((min(a, base), max(a + max(w, 1), base + 1)))
        else:
            # an indexed field only claims its own bytes: the array's extent
            # is unknown, and a stride-sized claim (3840-B records) swallows
            # the neighbours
            iv.append((a, a + max(w, 1)))
    # fields of one record array: indexed accesses with the same stride that
    # sit less than a stride apart are linked into one interval
    bystride = collections.defaultdict(set)
    for fid, pc, rw, a, w, sg, via, base, stride in acc:
        if via == "indexed" and stride and stride > 1:
            bystride[stride].add((a, max(w, 1)))
    for st, xs in bystride.items():
        xs = sorted(xs)
        for (a0, w0), (a1, w1) in zip(xs, xs[1:]):
            if a1 - a0 < st:
                iv.append((a0, a1 + w1))
    iv.sort()
    merged = []
    for lo, hi in iv:
        if merged and lo < merged[-1][1]:
            merged[-1][1] = max(merged[-1][1], hi)
        else:
            merged.append([lo, hi])
    starts = [m[0] for m in merged]
    import bisect

    def obj_of(a):
        i = bisect.bisect_right(starts, a) - 1
        if i >= 0 and a < merged[i][1]:
            return merged[i][0]
        return None
    objs = {}
    for lo, hi in merged:
        objs[lo] = dict(base=lo, end=hi, sites=0, units=set(), readers=set(),
                        writers=set(), fields=collections.defaultdict(
                            lambda: dict(n=0, R=0, W=0, widths=set(),
                                         sign=set(), units=set(),
                                         via=set(), strides=set())),
                        la_bases=set(), strides=collections.defaultdict(set),
                        c_names=set(), c_refs=0, c_units=set())
    per_addr = collections.defaultdict(lambda: dict(
        R=collections.defaultdict(set), W=collections.defaultdict(set),
        taken=set(), strides=set(), units=set()))
    for fid, pc, rw, a, w, sg, via, base, stride in acc:
        o = objs[obj_of(a)]
        o["sites"] += 1
        o["units"].add(fid)
        if rw == "addr":
            o["la_bases"].add(a)
            per_addr[a]["taken"].add(fid)
            per_addr[a]["units"].add(fid)
            continue
        (o["readers"] if rw == "R" else o["writers"]).add(fid)
        f = o["fields"][a - o["base"]]
        f["n"] += 1
        f[rw] += 1
        f["widths"].add(w)
        if sg:
            f["sign"].add(sg)
        f["units"].add(fid)
        f["via"].add(via)
        if stride and stride > 1:
            f["strides"].add(stride)
            o["strides"][stride].add(a - o["base"])
        pa = per_addr[a]
        pa[rw]["%d%s" % (w, sg)].add(fid)
        pa["units"].add(fid)
        if stride and stride > 1:
            pa["strides"].add(stride)
    refs, units = crefs
    for a, n in refs.items():
        ob = obj_of(a)
        if ob is None:
            continue
        o = objs[ob]
        o["c_names"].add("D_%08X" % a)
        o["c_refs"] += n
        o["c_units"].update(units[a])
    return objs, per_addr, obj_of


def region(a):
    if a < TEXT_END:
        return "rodata/text"
    if a >= 0x80180000:
        return "fixed-buffer"
    return "data/bss"


def layout_str(o, maxf=10):
    """compact layout: stride groups first, then the hottest fields."""
    parts = []
    for s, offs in sorted(o["strides"].items(), key=lambda x: -len(x[1])):
        offs = sorted(offs)
        parts.append("%d-B records, fields at +0x%X..+0x%X (%d)" % (
            s, offs[0], offs[-1], len(offs)))
    fl = sorted(o["fields"].items(), key=lambda x: -x[1]["n"])[:maxf]
    fs = []
    for off, f in sorted(fl):
        ws = "/".join(sorted("%d" % w for w in f["widths"]))
        sg = "".join(sorted(f["sign"])) if f["sign"] else ""
        sg = {"s": "s", "u": "u", "su": "s+u"}.get(sg, sg)
        rw = ("R" if f["R"] else "") + ("W" if f["W"] else "")
        st = ("*%s" % ",".join(str(x) for x in sorted(f["strides"]))
              if f["strides"] else "")
        fs.append("+0x%X:%s%s%s%s(%d)" % (off, sg, ws, rw, st, f["n"]))
    return parts, fs


def summarise(objs, top):
    rows = []
    for b, o in objs.items():
        rows.append(o)
    rows.sort(key=lambda o: -(o["sites"] + o["c_refs"]))
    return rows[:top]


def main():
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--top", type=int, default=60)
    ap.add_argument("--addr", help="print every access to one address")
    ap.add_argument("--object", help="print the full layout of the object "
                    "containing this address")
    ap.add_argument("--no-overlays", action="store_true")
    ap.add_argument("--region", choices=["data", "all"], default="data",
                    help="data (default): only 0x800BB3C0-0x8017FFFF in the "
                         "tables; all: include rodata/text and fixed buffers")
    ap.add_argument("--json", default=OUT)
    a = ap.parse_args()

    acc = collect(not a.no_overlays)
    cr = c_refs()
    objs, per_addr, obj_of = group(acc, cr)

    def keep(addr):
        return a.region == "all" or region(addr) == "data/bss"

    if a.addr:
        ad = int(a.addr, 16)
        pa = per_addr.get(ad)
        ob = obj_of(ad)
        print("D_%08X  object base %s  (+0x%X)" % (
            ad, "0x%08X" % ob if ob else None, ad - ob if ob else 0))
        if pa:
            for rw in ("R", "W"):
                for k, fs in sorted(pa[rw].items()):
                    print("  %s %-4s %s" % (rw, k, ", ".join(sorted(fs))))
            if pa["taken"]:
                print("  address taken by:", ", ".join(sorted(pa["taken"])))
            if pa["strides"]:
                print("  strides:", sorted(pa["strides"]))
        print("  C refs: %d in %s" % (cr[0].get(ad, 0),
                                      ", ".join(sorted(cr[1].get(ad, ())))))
        return
    if a.object:
        ad = int(a.object, 16)
        ob = obj_of(ad)
        if ob is None:
            print("no object contains 0x%08X" % ad)
            return
        o = objs[ob]
        print("object 0x%08X .. 0x%08X (%d B): %d sites, %d units, C refs %d"
              % (o["base"], o["end"], o["end"] - o["base"], o["sites"],
                 len(o["units"]), o["c_refs"]))
        print("  la bases:", " ".join("+0x%X" % (x - ob)
                                      for x in sorted(o["la_bases"])))
        for s, offs in sorted(o["strides"].items()):
            print("  stride %d: %s" % (s, " ".join("+0x%X" % x
                                                  for x in sorted(offs))))
        for off, f in sorted(o["fields"].items()):
            print("  +0x%04X  n=%-4d R=%-4d W=%-4d w=%s sign=%s via=%s "
                  "stride=%s units=%d" % (
                      off, f["n"], f["R"], f["W"], sorted(f["widths"]),
                      "".join(sorted(f["sign"])), ",".join(sorted(f["via"])),
                      sorted(f["strides"]) or "", len(f["units"])))
        return

    # JSON
    jo = []
    for b in sorted(objs):
        o = objs[b]
        jo.append(dict(
            base="0x%08X" % o["base"], end="0x%08X" % o["end"],
            size=o["end"] - o["base"], region=region(o["base"]),
            sites=o["sites"], units=sorted(o["units"]),
            readers=len(o["readers"]), writers=len(o["writers"]),
            la_bases=["0x%08X" % x for x in sorted(o["la_bases"])],
            strides={str(s): ["+0x%X" % x for x in sorted(v)]
                     for s, v in o["strides"].items()},
            c_names=sorted(o["c_names"]), c_refs=o["c_refs"],
            c_units=len(o["c_units"]),
            fields=[dict(offset=off, n=f["n"], R=f["R"], W=f["W"],
                         widths=sorted(f["widths"]), sign=sorted(f["sign"]),
                         via=sorted(f["via"]), strides=sorted(f["strides"]),
                         units=sorted(f["units"]))
                    for off, f in sorted(o["fields"].items())]))
    ja = {}
    for ad in sorted(per_addr):
        pa = per_addr[ad]
        ob = obj_of(ad)
        ja["D_%08X" % ad] = dict(
            object="0x%08X" % ob if ob else None,
            offset=ad - ob if ob else None,
            reads={k: sorted(v) for k, v in pa["R"].items()},
            writes={k: sorted(v) for k, v in pa["W"].items()},
            address_taken=sorted(pa["taken"]), strides=sorted(pa["strides"]),
            c_refs=cr[0].get(ad, 0))
    os.makedirs(os.path.dirname(a.json), exist_ok=True)
    with open(a.json, "w") as fh:
        json.dump(dict(generated_by="tools/analysis/data_access_census.py",
                       note="candidate objects from compiled code; confirm "
                            "layouts by reading the C before naming",
                       access_sites=len(acc),
                       units_scanned=len(set(x[0] for x in acc)),
                       objects=jo, addresses=ja), fh, indent=1)

    # summary
    data_objs = [o for o in objs.values() if keep(o["base"])]
    n_addr = sum(1 for ad in per_addr if keep(ad))
    print("access sites %d in %d functions; %d main addresses touched "
          "(%s); %d candidate objects; C names: %d distinct D_ in main+overlay C"
          % (len(acc), len(set(x[0] for x in acc)), n_addr,
             "data/bss" if a.region == "data" else "all regions",
             len(data_objs), len(cr[0])))
    top = sorted(data_objs, key=lambda o: -(o["sites"] + o["c_refs"]))[:a.top]
    V = validate_decls(acc)
    print("self-check vs scalar extern declarations in the C: %d checked, "
          "%d agree, %d width agrees but signedness differs, %d width "
          "differs" % (V["checked"], V["agree"], V["width_only"],
                       V["width_disagree"]))
    for b in V["examples"][:8]:
        print("   width differs:", b)
    print("\ntop %d objects by references (compiled access sites + C name refs)"
          % len(top))
    print("%-3s %-10s %6s %6s %5s %5s %4s  %s" % (
        "#", "base", "size", "sites", "Cref", "units", "ovl", "layout"))
    for i, o in enumerate(top, 1):
        ovs = set(u.split(":")[0] for u in o["units"]) - {"main"}
        parts, fs = layout_str(o, 8)
        print("%-3d 0x%08X %6d %6d %5d %5d %4d  %s" % (
            i, o["base"], o["end"] - o["base"], o["sites"], o["c_refs"],
            len(o["units"]), len(ovs), "; ".join(parts) or "-"))
        print("%47s %s" % ("", " ".join(fs)))
    print("\nwrote", os.path.relpath(a.json, ROOT))


if __name__ == "__main__":
    main()
