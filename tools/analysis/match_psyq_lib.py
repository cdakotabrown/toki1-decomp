#!/usr/bin/env python3
"""match_psyq_lib.py -- identify retail library-region functions by matching them
against the shipped Sony Psy-Q archives, masked by each archive member's OWN
relocation table.

WHY MASKING IS MANDATORY
    Archive members are UNLINKED objects.  Every `jal` target, every `%hi`/`%lo`
    immediate and every REL32 address word is ZERO in the archive and a real
    address in the retail binary.  A raw byte search dies at the first
    relocation (measured: longest common run 24 bytes = a generic prologue).
    This tool compares only the bits each relocation does NOT own, using the
    member's own relocation table -- never a guess.

WHY WHOLE-MEMBER ALIGNMENT, NOT PER-FUNCTION SEARCH
    A per-function search is not distinctive at short lengths: retail _mk_spr_packet
    (16 words) matches LIBGS `_mk_spr_packet` at 100%, because both compile to
    the same `if (p->x) A(); else B();` wrapper and all three jal targets are
    masked.  Percentages are worthless here.  A linker emits a member's .text
    CONTIGUOUSLY, so the sound unit of evidence is: does an ENTIRE member's
    .text appear at one alignment?  That is what this tool tests.

SIGNIFICANCE
    Reported as FULL = the number of words compared at all 32 bits (no
    relocation covering them) that matched.  Calibrated empirically against the
    Konami IDO game region (0x80041000-0x80086000, 70656 words -- 19% LARGER
    than the library region), which is guaranteed not to contain Sony objects:

        control region : 0 members match 100% of their .text, at any length
        library region : 180 members, 40 of them with FULL >= 16

    So FULL >= 16 has a measured false-positive rate of 0/70656 words.  Members
    below that are reported as NOISE and are not used for identification.

Modes:
    --map                  align every archive member against the library region
    --validate             ground-truth check + repo registered-name agreement
    --control              false-positive test on the Konami game region
    --sweep                identify the open library worklist functions
    --inventory OUT.json   symbol / source-file inventory from the archives
    --addr A --size N      score one function (diagnostic; see caveat above)
"""
import argparse
import collections
import json
import os
import re
import sys

import numpy as np

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, "tools", "analysis"))
import psyq_obj as P                                       # noqa: E402

TEXT_SECTION_START = 0x80041000
FILE_OFFSET_BASE = 0x800
LIB_LO, LIB_HI = 0x80086000, 0x800C0000
GAME_LO, GAME_HI = 0x80041000, 0x80086000
# Sony's Psy-Q archives are NOT in the tree (2026-11 public-release cleanup): they
# are copyrighted SDK material and no build path reads them. Supply your own copy
# with PSYQ_SDK_DIRS=/path/2.5/lib:/path/4.5/lib; without it this tool skips.
LIB_DIRS = [d for d in os.environ.get("PSYQ_SDK_DIRS", "").split(os.pathsep) if d]
if not LIB_DIRS:
    LIB_DIRS = [os.path.join(ROOT, "tools", "psyq-sdk", "2.5", "lib"),
                os.path.join(ROOT, "tools", "psyq-sdk", "4.5", "lib")]


def have_sdk(dirs=None):
    return any(os.path.isdir(d) for d in (dirs or LIB_DIRS))
BIN = os.path.join(ROOT, "disks", "SLPS_000.65")

SEED_K = 4          # consecutive fully-unmasked words used as a search seed
FULL_MIN = 16       # empirically calibrated significance floor (0 FP on control)


def read_region(addr, size):
    off = (addr - TEXT_SECTION_START) + FILE_OFFSET_BASE
    with open(BIN, "rb") as f:
        f.seek(off)
        b = f.read(size & ~3)
    return np.frombuffer(b, dtype="<u4").copy()


class Corpus(object):
    """Every library member's .text, with a per-word relocation mask."""

    def __init__(self, lib_dirs=LIB_DIRS):
        words, masks, self.members = [], [], []
        base = 0
        for lp in P.iter_libs(lib_dirs):
            ver = os.path.basename(os.path.dirname(os.path.dirname(lp)))
            for m in P.parse_lib(lp):
                t = m.obj.section_by_name(".text")
                if t is None or len(t.data) < 4:
                    continue
                n = len(t.data) // 4
                w = np.frombuffer(bytes(t.data[:n * 4]), dtype="<u4").copy()
                k = np.full(n, 0xFFFFFFFF, dtype=np.uint32)
                for off, rt, _e in t.relocs:
                    if off % 4 or off // 4 >= n:
                        continue
                    _nm, bits = P.RELOC_MASKS.get(rt, ("UNKNOWN", 0xFFFFFFFF))
                    k[off // 4] &= np.uint32(~bits & 0xFFFFFFFF)
                syms = {}
                for e in m.obj.exports:
                    if e["section"] == t.index:
                        syms[e["offset"]] = e["name"]
                for e in m.obj.functions:
                    if e["section"] == t.index:
                        syms.setdefault(e["offset"], e["name"])
                for e in m.obj.locals:
                    if e["section"] == t.index:
                        syms.setdefault(e["offset"], e["name"])
                self.members.append({
                    "ver": ver, "lib": m.libname, "member": m.name,
                    "src": m.obj.source_file(), "base": base, "nwords": n,
                    "syms": syms, "libsyms": m.symbols,
                    "imports": sorted(set(m.obj.imports.values())),
                    "nreloc": len(t.relocs)})
                words.append(w)
                masks.append(k)
                base += n
        self.words = np.concatenate(words)
        self.masks = np.concatenate(masks)
        self.mwords = self.words & self.masks
        self.total = base


def align_members(corpus, lo, hi):
    """Find every alignment at which an ENTIRE member .text appears in [lo,hi).

    BLAST-style: seed on SEED_K consecutive fully-unmasked words, then verify
    the member's whole extent at that alignment.
    """
    rw = read_region(lo, hi - lo)
    seed = collections.defaultdict(list)
    for i in range(len(rw) - SEED_K + 1):
        seed[rw[i:i + SEED_K].tobytes()].append(i)
    out = []
    for mi, m in enumerate(corpus.members):
        b, n = m["base"], m["nwords"]
        mw = corpus.mwords[b:b + n]
        mk = corpus.masks[b:b + n]
        full = mk == 0xFFFFFFFF
        cand = collections.Counter()
        for i in range(n - SEED_K + 1):
            if not full[i:i + SEED_K].all():
                continue
            for r in seed.get(mw[i:i + SEED_K].tobytes(), ()):
                cand[r - i] += 1
        for d, _c in cand.most_common(20):
            s, e = max(0, -d), min(n, len(rw) - d)
            if (e - s) < n:
                continue
            eq = (rw[s + d:e + d] & mk[s:e]) == mw[s:e]
            if int(eq.sum()) != n:
                continue
            out.append({"mi": mi, "start": lo + d * 4, "end": lo + (d + n) * 4,
                        "nwords": n, "full": int((full & eq).sum())})
            break
    return out


def dedupe(corpus, spans):
    """Same member ships in both SDK versions; keep one span per address range."""
    best = {}
    for s in spans:
        key = (s["start"], s["end"])
        cur = best.get(key)
        if cur is None or corpus.members[s["mi"]]["ver"] < \
                corpus.members[cur["mi"]]["ver"]:
            best[key] = s
    return sorted(best.values(), key=lambda s: s["start"])


def symbol_map(corpus, spans):
    """symbol name -> (address, size, span) for every significant span."""
    out = {}
    for s in spans:
        if s["full"] < FULL_MIN:
            continue
        m = corpus.members[s["mi"]]
        offs = sorted(m["syms"])
        for i, o in enumerate(offs):
            nxt = offs[i + 1] if i + 1 < len(offs) else s["nwords"] * 4
            out[m["syms"][o]] = {"addr": s["start"] + o, "size": nxt - o,
                                 "lib": m["lib"], "ver": m["ver"],
                                 "member": m["member"], "src": m["src"],
                                 "span_full": s["full"]}
    return out


def load_sizes():
    out = {}
    l3 = json.load(open(os.path.join(ROOT, "config/level3_functions.json")))
    for f in l3["functions"]:
        out[int(f["address"], 16)] = (f["name"], f["size"])
    aud = json.load(open(os.path.join(
        ROOT, "config/main_function_audit_by_addr.json")))["addresses"]
    for a, v in aud.items():
        ai = int(a, 16)
        if v.get("size") and ai not in out:
            nm = next((x for x in v.get("aliases", []) if x != "if"), a)
            out[ai] = (nm, v["size"])
    return out


def load_worklist():
    # The sweep's worklist is a finished queue kept in the project's private
    # archive (docs/archive is monorepo-only): without it, say so and stop.
    path = os.path.join(ROOT, "docs/archive/worklists/MAIN-OPEN-LIBRARY.md")
    if not os.path.exists(path):
        sys.exit("sweep: needs the private archive's worklist %s (not in the "
                 "public repository); the other modes do not" % os.path.relpath(path, ROOT))
    rows, seen = [], set()
    for ln in open(path):
        m = re.match(r"^\|\s*(0x[0-9A-Fa-f]+)\s*\|\s*(\d+)\s*\|\s*([^|]+?)\s*\|",
                     ln)
        if m:
            a = int(m.group(1), 16)
            if a not in seen:
                seen.add(a)
                rows.append((a, int(m.group(2)), m.group(3)))
    return rows


# ------------------------------------------------------------------ modes ---
def cmd_map(c, args):
    spans = dedupe(c, align_members(c, LIB_LO, LIB_HI))
    sig = [s for s in spans if s["full"] >= FULL_MIN]
    cov = sum(s["nwords"] for s in sig) * 4
    print("significant spans (FULL >= %d): %d, covering %d bytes (%.1f%% of "
          "the %d-byte library region)"
          % (FULL_MIN, len(sig), cov, 100.0 * cov / (LIB_HI - LIB_LO),
             LIB_HI - LIB_LO))
    print("\n%-10s %-10s %-5s %-12s %-10s %5s %5s  symbols"
          % ("start", "end", "sdk", "lib", "member", "words", "full"))
    for s in sig:
        m = c.members[s["mi"]]
        print("0x%08X 0x%08X %-5s %-12s %-10s %5d %5d  %s"
              % (s["start"], s["end"], m["ver"], m["lib"], m["member"],
                 s["nwords"], s["full"],
                 ",".join(sorted(m["syms"].values()))[:70]))
    noise = [s for s in spans if s["full"] < FULL_MIN]
    print("\nrejected as NOISE (FULL < %d): %d spans" % (FULL_MIN, len(noise)))
    return spans


def cmd_control(c, args):
    spans = align_members(c, GAME_LO, GAME_HI)
    print("CONTROL: Konami IDO game region 0x%08X-0x%08X (%d words)"
          % (GAME_LO, GAME_HI, (GAME_HI - GAME_LO) // 4))
    print("  members matching 100%% of their .text : %d" % len(spans))
    sig = [s for s in spans if s["full"] >= FULL_MIN]
    print("  ... of which FULL >= %d (would be reported) : %d" % (FULL_MIN, len(sig)))
    for s in sig:
        m = c.members[s["mi"]]
        print("    FALSE POSITIVE %-12s %-10s @0x%08X full=%d"
              % (m["lib"], m["member"], s["start"], s["full"]))
    print("  FALSE-POSITIVE RATE: %d / %d words" % (len(sig), (GAME_HI - GAME_LO) // 4))
    return spans


def cmd_validate(c, args):
    spans = dedupe(c, align_members(c, LIB_LO, LIB_HI))
    sm = symbol_map(c, spans)
    sizes = load_sizes()
    reg = {}
    for a, (nm, sz) in sizes.items():
        reg.setdefault(nm, (a, sz))
    print("=== A. Is the identification self-consistent? ===")
    print("  symbols located byte-exactly : %d" % len(sm))
    ov = collections.Counter()
    for nm, v in sm.items():
        ov[v["addr"]] += 1
    print("  addresses claimed by >1 symbol: %d" % sum(1 for v in ov.values() if v > 1))
    print("\n=== B. Agreement with names the repo already registered ===")
    ag = dis = 0
    for nm, v in sorted(sm.items(), key=lambda x: x[1]["addr"]):
        if nm in reg:
            ra, rs = reg[nm]
            good = (ra == v["addr"])
            ag += good
            dis += not good
            print("  %-22s archive=0x%08X/%-4d repo=0x%08X/%-4d  %s"
                  % (nm, v["addr"], v["size"], ra, rs,
                     "AGREE" if good else "DISAGREE"))
    print("  agree=%d disagree=%d" % (ag, dis))
    print("\n=== C. Does the retail binary contain the Sony objects verbatim? ===")
    bylib = collections.Counter()
    for s in spans:
        if s["full"] >= FULL_MIN:
            bylib[c.members[s["mi"]]["lib"]] += 1
    tot = collections.Counter(m["lib"] for m in c.members)
    for lib in sorted(tot):
        print("  %-12s %3d / %-4d members found verbatim" % (lib, bylib.get(lib, 0), tot[lib]))
    return spans


def cmd_sweep(c, args):
    spans = dedupe(c, align_members(c, LIB_LO, LIB_HI))
    sm = symbol_map(c, spans)
    byaddr = {v["addr"]: (k, v) for k, v in sm.items()}
    sig = [s for s in spans if s["full"] >= FULL_MIN]
    rows = load_worklist()
    exact = inside = none = 0
    out = []
    for a, sz, nm in rows:
        hit = byaddr.get(a)
        span = next((s for s in sig if s["start"] <= a < s["end"]), None)
        if hit:
            exact += 1
            k, v = hit
            out.append((a, sz, nm, "EXACT", k, v))
        elif span:
            inside += 1
            m = c.members[span["mi"]]
            out.append((a, sz, nm, "INSIDE", "%s/%s" % (m["lib"], m["member"]),
                        {"src": m["src"], "span_full": span["full"],
                         "addr": span["start"], "size": 0, "lib": m["lib"],
                         "ver": m["ver"], "member": m["member"]}))
        else:
            none += 1
            out.append((a, sz, nm, "NONE", None, None))
    print("open library functions: %d" % len(rows))
    print("  EXACT  (address is a symbol in a byte-matched member): %d" % exact)
    print("  INSIDE (address falls inside a byte-matched member,   ")
    print("          but not on a symbol boundary)                : %d" % inside)
    print("  NONE                                                 : %d" % none)
    for a, sz, nm, st, k, v in out:
        if st == "EXACT":
            print("  EXACT  0x%08X %-4d %-22s -> %-22s %s/%s  %s"
                  % (a, sz, nm, k, v["ver"], v["lib"], v["src"] or ""))
    for a, sz, nm, st, k, v in out:
        if st == "INSIDE":
            print("  INSIDE 0x%08X %-4d %-22s -> %s @0x%08X" % (a, sz, nm, k, v["addr"]))
    return out


def cmd_inventory(c, args):
    rows = []
    for m in c.members:
        rows.append({"sdk": m["ver"], "lib": m["lib"], "member": m["member"],
                     "src": m["src"], "text_bytes": m["nwords"] * 4,
                     "symbols": [m["syms"][o] for o in sorted(m["syms"])],
                     "lib_symbols": m["libsyms"], "imports": m["imports"]})
    json.dump(rows, open(args.inventory, "w"), indent=1)
    print("wrote %s: %d members, %d symbols, %d with a source path"
          % (args.inventory, len(rows), sum(len(r["symbols"]) for r in rows),
             sum(1 for r in rows if r["src"])))
    return rows


def cmd_addr(c, args):
    """Diagnostic single-function score.  NOT sound identification on its own --
    short functions collide (see module docstring)."""
    t = read_region(int(args.addr, 16), args.size)
    n = len(t)
    L = corpus_len = c.total - n + 1
    best = np.zeros(L, dtype=np.int32)
    step = max(1, 4_000_000 // max(n, 1))
    for s in range(0, L, step):
        e = min(L, s + step)
        M_ = np.lib.stride_tricks.sliding_window_view(c.mwords[s:e + n - 1], n)
        K_ = np.lib.stride_tricks.sliding_window_view(c.masks[s:e + n - 1], n)
        best[s:e] = ((t[None, :] & K_) == M_).sum(axis=1)
    owner = np.empty(c.total, dtype=np.int32)
    for i, m in enumerate(c.members):
        owner[m["base"]:m["base"] + m["nwords"]] = i
    seen = set()
    for o in np.argsort(-best)[:400]:
        o = int(o)
        mi = int(owner[o])
        if mi in seen or owner[min(o + n - 1, c.total - 1)] != mi:
            continue
        seen.add(mi)
        m = c.members[mi]
        k = c.masks[o:o + n]
        eq = (t & k) == c.mwords[o:o + n]
        print("  %-5s %-12s %-10s +0x%-5x %3d/%-3d %5.1f%% full=%-3d %s"
              % (m["ver"], m["lib"], m["member"], (o - m["base"]) * 4,
                 int(eq.sum()), n, 100.0 * int(eq.sum()) / n,
                 int((k == 0xFFFFFFFF).sum()),
                 m["syms"].get((o - m["base"]) * 4, "")))
        if len(seen) >= args.topk:
            break
    print("  NOTE: a high percentage here is NOT identification; use --map.")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--map", action="store_true")
    ap.add_argument("--validate", action="store_true")
    ap.add_argument("--control", action="store_true")
    ap.add_argument("--sweep", action="store_true")
    ap.add_argument("--inventory")
    ap.add_argument("--addr")
    ap.add_argument("--size", type=int, default=64)
    ap.add_argument("--topk", type=int, default=5)
    ap.add_argument("--json")
    a = ap.parse_args()
    print("ROOT = %s" % ROOT, file=sys.stderr)
    if not have_sdk():
        print("Psy-Q SDK archives not found: this tool needs Sony's .LIB files, "
              "which are not in the tree (2026-11 public-release cleanup).\n"
              "Set PSYQ_SDK_DIRS=/path/to/2.5/lib:/path/to/4.5/lib to run it. "
              "Skipping.", file=sys.stderr)
        return 0
    c = Corpus()
    print("corpus: %d members, %d .text words" % (len(c.members), c.total),
          file=sys.stderr)
    if a.inventory:
        return cmd_inventory(c, a) and 0
    if a.map:
        cmd_map(c, a)
    if a.control:
        cmd_control(c, a)
    if a.validate:
        cmd_validate(c, a)
    if a.sweep:
        cmd_sweep(c, a)
    if a.addr:
        cmd_addr(c, a)
    if not any([a.map, a.control, a.validate, a.sweep, a.addr]):
        ap.print_help()
    if a.json:
        spans = dedupe(c, align_members(c, LIB_LO, LIB_HI))
        json.dump({"symbols": symbol_map(c, spans),
                   "spans": [{**s, **{k: c.members[s["mi"]][k]
                                      for k in ("ver", "lib", "member", "src")}}
                             for s in spans]}, open(a.json, "w"), indent=1)
        print("wrote %s" % a.json, file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main())
