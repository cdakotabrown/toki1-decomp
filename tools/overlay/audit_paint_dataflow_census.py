#!/usr/bin/env python3
"""Paint-dataflow census: does a `patch_insn_reg_field` layer CHANGE DATAFLOW?

    python3 tools/overlay/audit_paint_dataflow_census.py DATE func_80135B14
    python3 tools/overlay/audit_paint_dataflow_census.py --all -j 10

`--all` sweeps every byte-perfect overlay function carrying an active paint
directive and rewrites `config/paint_dataflow.json`.  It never touches
`config/overlay_match_status.json`, never modifies a `.c`, and never strips a
paint.  The 2026-09 results are kept in the project's private archive.

=============================================================================
TWO TIERS.  DO NOT CONFLATE THEM.
=============================================================================
PROVABLE -- a structural defect no value-equivalence argument can excuse:

  orphaned_consumer     at some operand position the UNPAINTED object reads a
                        value produced inside the function and the PAINTED
                        object reads a scratch register ($at, $v0/$v1,
                        $t0-$t9) with NO definition anywhere in the function.
  regimm_polarity_flip  an `rt`-field paint on a REGIMM word, where `rt` is
                        the condition selector: bltz <-> bgez is an OPERATION
                        change.
  wrong_symbol_hi_lo    a LO16 consumer of symbol S whose base register is
                        defined by a `lui %hi` of a DIFFERENT symbol whose
                        %hi VALUE differs.  The linked address is then
                        %hi(T)+%lo(S) -- neither S nor T.

ADVISORY -- `audit_paint_isomorphism.py` reaching-definition producer changes
surviving the UPSTREAM-REPAIR filter.  A SCREENING SIGNAL with documented
false-positive modes (read that module's docstring).  Never strip a paint on
an advisory flag alone: stripping a producer-changing paint on a MATCHING
function trades a verified property (bytes match) for an unverified one.

=============================================================================
THINGS THAT MAKE A NAIVE SCREEN LIE  (all handled here)
=============================================================================
1. CONDITIONAL OFFSET TRANSLATION.  `apply_postproc` shifts a WHOLE paint
   directive by `fto` (the function symbol's st_value in .text, normally 0x10
   -- the two types.h `jr ra; nop` thunks) when a `prepend_thunk` ran or when
   ANY offset in that directive is below `fto`.  Screening a function-relative
   layer at .text+off lands on a thunk nop and reports a bogus "paint on
   compiled NOP (fabrication)".  33 of 2,104 -- every one false.
2. CHAIN POSITION.  A paint directive at step k sees the object after
   steps[0:k].  294 files have a word-moving step after the paint.  This tool
   rebuilds the object state at each directive's own position.
3. NUMERIC REGISTER SPELLINGS (`0x14:rt:17:2`) are legal; a name-only screen
   calls them stale.
4. SAME-%hi BASE SWAPS.  Two symbols in one 64K page have equal %hi, so
   swapping their base registers is byte- AND semantics-identical.  510 sites.
5. THE HIDDEN-PARAMETER (ABI-move) CLASS.  A paint that moves an operand read
   from an argument register to a scratch register where BOTH producers are
   the function entry is implementing the overlay caller-residue convention
   (see DATE2/func_80132E74's header), not changing dataflow.  265 functions.
6. UPSTREAM REPAIR.  Removing a paint can CREATE the orphan, because
   `scheduler_normalizer`/`swap_instructions` broke def-use and the paint was
   the cure.  Counted, never reported as a divergence.

Self-check: `patch_insn_reg_field.py` exits 1 on an OLD-register mismatch, so
on a population that builds, `stale` must come out 0.  A non-zero stale is a
bug in THIS tool, not a finding.

WHAT IT STILL CANNOT SEE: `swap_registers` live-range collapses (present in
both objects); reads in blocks the CFG walk finds unreachable (5.8% of reads);
and whatever `_veq` over-absorbs -- the reason a LOOSE bound is reported
alongside the tight one.
"""
import argparse
import json
import os
import shutil
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "overlay"))
sys.path.insert(0, str(ROOT / "tools" / "build"))
sys.path.insert(0, str(ROOT / "tools" / "ido"))

import audit_paint_isomorphism as iso
from audit_paint_bans import screen_paint as _screen_paint, REGS as _REGS
from patch_insn_reg_field import parse_reg as _parse_reg
from overlay_postproc import (parse_postproc, apply_postproc,
                              get_func_text_offset)
from parse_postproc import (parse_compile_flags, parse_compile_env,
                            parse_compiler, parse_symbol_start_text,
                            parse_symbol_size_text)
from ido_flags import pic_flags

import re

SRC = ROOT / "src" / "overlay"
CFG = ROOT / "config" / "overlay"
STATUS = ROOT / "config" / "overlay_match_status.json"
OUT_JSON = ROOT / "config" / "paint_dataflow.json"
# the PC port's overlay overrides only annotate the census; they are read
# where that part is (tools/repo_paths.py) and simply absent without it
sys.path.insert(0, str(ROOT / "tools"))
from repo_paths import PC as _PC_ROOT  # noqa: E402
PC_OVERLAY = (_PC_ROOT or ROOT) / "pc" / "src" / "overlay"
PC_INITS = (_PC_ROOT or ROOT) / "pc" / "src" / "overlay_inits"

SCRATCH = {1, 2, 3, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25}   # at, v0/v1, t0-t9
RELINE = re.compile(r"^([0-9a-fA-F]+)\s+(R_MIPS_\w+)\s+(\S+)")


# ------------------------------------------------------------------ helpers
def screen(word, field, old, new):
    """audit_paint_bans.screen_paint with numeric register spellings
    normalised the way `patch_insn_reg_field.py` normalises them."""
    try:
        old, new = _REGS[_parse_reg(old)], _REGS[_parse_reg(new)]
    except Exception:
        pass
    return _screen_paint(word, field, old, new, check_at=True)


def sym_hi(name):
    """The %hi value the linker writes for a D_/func_-style name, or None."""
    m = re.search(r"(80[0-9a-fA-F]{6})$", name)
    return None if not m else ((int(m.group(1), 16) + 0x8000) >> 16) & 0xFFFF


def _entry_only(s):
    # iso tags entry values by register since 2026-09-28: ('E', reg)
    return bool(s) and all(x == 'E' or (isinstance(x, tuple) and x[0] == 'E')
                           for x in s)


def _has_real(s):
    return any(isinstance(x, int) for x in s)


def _undef(s):
    """`minimize_paint_strip.nflags`'s predicate: the entry value OR a
    call-clobbered one.  Used ONLY for the upstream-repair filter, so this
    tool's advisory count is identical to that driver's."""
    return all((x == 'E' or isinstance(x, tuple)) for x in s)


def _sym_addr_size(ov, func, text):
    funcs = {}
    for line in (CFG / f"{ov}.sym").read_text().splitlines():
        p = line.strip().split(":")
        if len(p) >= 3:
            try:
                funcs[p[2]] = (int(p[0], 16), int(p[1]))
            except ValueError:
                pass
    addr, size = funcs.get(func, (None, None))
    ss, st = parse_symbol_size_text(text), parse_symbol_start_text(text)
    return (st if st is not None else addr), (ss if ss is not None else size)


def _compile(ov, func, wd, name):
    src = SRC / ov / f"{func}.c"
    text = src.read_text(errors="replace")
    flags = parse_compile_flags(src)
    compiler = parse_compiler(src)
    obj = Path(wd) / name
    if compiler.startswith("gcc"):
        cmd = [sys.executable, str(iso.GCCWRAP), str(src), "-o", str(obj),
               "--gcc-version", compiler[3:], "-I", str(iso.INC),
               "-I", str(iso.INC / "psyq")]
        extra = []
        for f in flags:
            (cmd if f in iso.GCCWRAP_FLAGS else extra).append(f)
        if extra:
            cmd += ["--extra-flags", " ".join(extra)]
    else:
        cmd = ([str(iso.IDO_CC)] + iso.IDO_FLAGS
               + pic_flags(src, compile_flags=flags)
               + flags + ["-c", str(src), "-o", str(obj)])
    env = os.environ.copy()
    env["TMPDIR"] = str(wd)
    env.update(parse_compile_env(src) or {})
    r = subprocess.run(cmd, capture_output=True, text=True, timeout=600,
                       env=env, cwd=str(wd))
    if r.returncode != 0:
        return None, "compile failed: " + r.stderr[:200]
    return obj, None


def _run_chain(obj, steps, addr, ov, func):
    asm = ROOT / "asm" / "overlay" / ov / f"{func}.s"
    if steps:
        apply_postproc(obj, steps, addr, ov, verbose=False,
                       target_asm=asm if asm.exists() else None)


def _relocs(obj):
    r = subprocess.run(["mipsel-linux-gnu-objdump", "-r", str(obj)],
                       capture_output=True, text=True)
    out, cur = {}, None
    for ln in r.stdout.splitlines():
        if ln.startswith("RELOCATION RECORDS FOR ["):
            cur = ln.split("[")[1].split("]")[0]
            continue
        if cur != ".text":
            continue
        m = RELINE.match(ln.strip())
        if m:
            out[int(m.group(1), 16)] = (m.group(2), m.group(3))
    return out


def _wrong_symbol(W, rel):
    """Hard %hi/%lo mispairings (same-%hi swaps filtered out)."""
    rec = iso.reaching(W)
    hard = []
    for off, (rt, sym) in rel.items():
        if rt != 'R_MIPS_LO16':
            continue
        i = off // 4
        if i >= len(W):
            continue
        _, u, _, _, _ = iso.decode(W[i])
        rs = (W[i] >> 21) & 31
        if rs not in u:
            continue
        paired = set()
        for p in rec.get((i, rs), frozenset()):
            if isinstance(p, int):
                r2 = rel.get(p * 4)
                paired.add(r2[1] if r2 and r2[0] == 'R_MIPS_HI16' else None)
        real = {h for h in paired if h}
        if not real or sym in real:
            continue
        want, got = sym_hi(sym), {sym_hi(h) for h in real}
        if want is None or None in got or got != {want}:
            hard.append("0x%x:%s<-%s" % (off, sym, "|".join(sorted(real))))
    return hard


# ------------------------------------------------------------------ per-func
def audit_one(ov, func):
    rec = {"overlay": ov, "func": func}
    wd = tempfile.mkdtemp(prefix="pdc_")
    try:
        src = SRC / ov / f"{func}.c"
        text = src.read_text(errors="replace")
        steps = parse_postproc(src)
        addr, _size = _sym_addr_size(ov, func, text)
        nopaint = [s for s in steps if s.get('type') != 'patch_insn_reg_field']

        # A/B come from a FRESH compile each (objA via `iso.build`'s exact
        # path).  Do NOT compile once and copy: several PostProc steps are
        # sensitive to the object path/tempdir and a copied object yields a
        # different chain result on ~13 functions.
        objA, e1 = _compile(ov, func, wd, "A.o")
        if e1:
            rec["error"] = e1
            return rec
        objB, e2 = _compile(ov, func, wd, "B.o")
        if e2:
            rec["error"] = e2
            return rec
        try:
            _run_chain(objA, nopaint, addr, ov, func)
            _run_chain(objB, steps, addr, ov, func)
        except Exception as e:
            rec["error"] = "postproc failed: %r" % (e,)
            return rec
        A = iso.objwords(objA, Path(wd) / "a.bin")
        B = iso.objwords(objB, Path(wd) / "b.bin")
        rec["insns"] = len(A)
        rec["paint_layer_is_noop"] = (A == B)

        # ---- ADVISORY: iso, tight and loose
        bad = iso.compare(A, B)
        ra_, rb = iso.reaching(A), iso.reaching(B)
        tight, orph, repair, abi = [], [], [], 0
        n_reads = n_qskip = n_loose = 0
        for i in range(min(len(A), len(B))):
            _, ua, _, _, _ = iso.decode(A[i])
            _, ub, _, _, _ = iso.decode(B[i])
            if len(ua) != len(ub):
                continue
            sa = [ra_.get((i, r), frozenset()) for r in ua]
            sb = [rb.get((i, r), frozenset()) for r in ub]
            if any('?' in x for x in sa) or any('?' in x for x in sb):
                n_qskip += len(ua)
                continue
            n_reads += len(ua)
            op, f = A[i] >> 26, A[i] & 0x3F
            comm = op in (4, 5) or (op == 0 and f in (0x21, 0x20, 0x24, 0x25,
                                                      0x26, 0x18, 0x19))
            key = lambda L: sorted(sorted(map(str, x)) for x in L)
            if not (sa == sb or (comm and key(sa) == key(sb))):
                n_loose += 1
            for rA, rB, x, y in zip(ua, ub, sa, sb):
                if rB in SCRATCH and _entry_only(y) and _has_real(x):
                    orph.append("0x%x:%s->%s" % (i * 4, iso.RA[rA], iso.RA[rB]))
                elif rA in SCRATCH and _entry_only(x) and _has_real(y):
                    repair.append("0x%x" % (i * 4))
                elif _entry_only(x) and _entry_only(y) and rA != rB:
                    abi += 1
        for i, ua, ub, sa, sb in bad:
            pairs = list(zip(sa, sb))
            # UPSTREAM-REPAIR direction: the paint is the cure, not the disease
            if (any(_undef(a) and _has_real(b) for a, b in pairs) and
                    all(a == b or (_undef(a) and _has_real(b))
                        for a, b in pairs)):
                continue
            tight.append((i, ua, ub, sa, sb))

        rec.update({
            "iso_flags_filtered": len(tight),
            "iso_flags_raw": len(bad),
            "iso_flags_veq_off": n_loose,
            "reads_compared": n_reads,
            "unreachable_reads_skipped": n_qskip,
            "abi_move_sites": abi,
            "upstream_repair_sites": len(repair),
        })
        rec["advisory_evidence"] = [{
            "off": "0x%x" % (i * 4), "A": "%08x" % A[i], "B": "%08x" % B[i],
            "uses": [iso.RA[x] for x in ua] + ["->"] + [iso.RA[x] for x in ub],
            "prodA": [sorted(map(str, x)) for x in sa],
            "prodB": [sorted(map(str, x)) for x in sb],
        } for i, ua, ub, sa, sb in tight[:4]]

        # ---- PROVABLE: chain-position-accurate ban screen
        pidx = [i for i, s in enumerate(steps)
                if s.get('type') == 'patch_insn_reg_field']
        banned, stale, oob, at_read = [], [], [], 0
        n_ent = n_xlat = nbranch = 0
        for j, k in enumerate(pidx):
            o, be = _compile(ov, func, wd, f"s{j}.o")
            if be:
                rec.setdefault("state_errors", []).append(be)
                continue
            try:
                _run_chain(o, steps[:k], addr, ov, func)
            except Exception as e:
                rec.setdefault("state_errors", []).append("%d:%r" % (k, e))
                continue
            fto = get_func_text_offset(o)
            words = iso.objwords(o, Path(wd) / f"w{j}.bin")
            prepend = any(s.get('type') == 'prepend_thunk' for s in steps[:k])
            parsed = []
            for tok in steps[k].get('patches', []):
                p = tok.split(':')
                if len(p) != 4:
                    continue
                try:
                    n = int(p[0], 16) if p[0].startswith('0x') else int(p[0])
                except ValueError:
                    continue
                parsed.append((n, p[1], p[2], p[3]))
            n_ent += len(parsed)
            xlat = fto > 0 and (prepend or any(n < fto for n, _, _, _ in parsed))
            if xlat:
                n_xlat += len(parsed)
            seen = set()
            for n, fld, old, new in parsed:
                off = n + fto if xlat else n
                if (off, fld) in seen:      # chained paint: `old` is the
                    continue                # previous paint's OUTPUT
                seen.add((off, fld))
                idx, tag = off // 4, "0x%x:%s:%s:%s" % (n, fld, old, new)
                if not (0 <= idx < len(words)):
                    oob.append(tag)
                    continue
                for verdict, reason in screen(words[idx], fld, old, new):
                    if verdict == 'BANNED':
                        banned.append(tag + "|" + reason)
                    elif verdict == 'STALE':
                        stale.append(tag + "|" + reason)
                    elif 'at' in (old, new):
                        at_read += (new == 'at')
                    else:
                        nbranch += 1
        rec.update({
            "paint_entries": n_ent, "paint_directives": len(pidx),
            "translated_entries": n_xlat,
            "branch_operand_paints": nbranch,
            "at_read_repoints": at_read,
            "stale_paints": stale[:8], "oob_paints": oob[:8],
        })

        # ---- PROVABLE: wrong symbol
        relA, relB = _relocs(objA), _relocs(objB)
        rec["reloc_tables_identical"] = (relA == relB)
        ws = sorted(set(_wrong_symbol(B, relB)) - set(_wrong_symbol(A, relA)))

        prov = []
        pol = [b for b in banned if 'POLARITY FLIP' in b]
        if pol:
            prov.append({"class": "regimm_polarity_flip", "sites": pol})
        if orph:
            prov.append({"class": "orphaned_consumer", "n": len(orph),
                         "sites": orph[:12]})
        if ws:
            prov.append({"class": "wrong_symbol_hi_lo", "n": len(ws),
                         "sites": ws[:12]})
        other = [b for b in banned if 'POLARITY FLIP' not in b]
        if other:
            rec["ban_screen_notes"] = other[:12]

        if rec["paint_layer_is_noop"] and n_ent:
            rec["classification"] = "unchecked"
            rec["reason"] = "paint layer is a no-op; nothing to compare"
        elif prov:
            rec["classification"] = "dataflow_divergent"
            rec["tier"] = "provable"
            rec["provable_evidence"] = prov
            rec["also_advisory"] = bool(tight)
        elif tight:
            rec["classification"] = "dataflow_divergent"
            rec["tier"] = "advisory"
        else:
            rec["classification"] = "clean"
            rec["tier"] = None
        return rec
    except Exception as e:                                    # noqa: BLE001
        rec["error"] = "%r" % (e,)
        return rec
    finally:
        shutil.rmtree(wd, ignore_errors=True)


# ------------------------------------------------------------------ sweep
def population():
    st = json.loads(STATUS.read_text())
    pop = []
    for ov, d in sorted(st.items()):
        for fn, r in sorted(d.items()):
            if r.get('status') != 'byte_perfect':
                continue
            p = SRC / ov / f"{fn}.c"
            if not p.exists():
                continue
            if any('@PostProc:' in l and 'patch_insn_reg_field' in l
                   for l in p.read_text(errors="replace").splitlines()):
                pop.append((ov, fn))
    return pop


def main():
    ap = argparse.ArgumentParser(
        description="Paint-dataflow census (PROVABLE vs ADVISORY tiers). "
                    "Measurement only -- never modifies a .c, a .sym, or "
                    "config/overlay_match_status.json.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="exit 0 = clean, 1 = divergent, 2 = error")
    ap.add_argument("overlay", nargs="?")
    ap.add_argument("func", nargs="?")
    ap.add_argument("--all", action="store_true",
                    help="sweep the population and rewrite "
                         "config/paint_dataflow.json")
    ap.add_argument("-j", "--jobs", type=int, default=8)
    args = ap.parse_args()

    if not args.all:
        if not (args.overlay and args.func):
            ap.error("give OVERLAY FUNC, or --all")
        r = audit_one(args.overlay, args.func)
        print(json.dumps(r, indent=1))
        if "error" in r:
            return 2
        return 1 if r["classification"] == "dataflow_divergent" else 0

    pop = population()
    print(f"{len(pop)} painted byte-perfect functions", flush=True)
    pc_over = {(p.parent.name, p.stem)
               for p in PC_OVERLAY.rglob("func_*.c")} if PC_OVERLAY.exists() else set()
    pc_init = {p.stem.replace("overlay_init_", "")
               for p in PC_INITS.glob("*.c")} if PC_INITS.exists() else set()
    done = [0]

    def go(t):
        r = audit_one(*t)
        r["pc_port_override"] = t in pc_over
        r["pc_init_replay_overlay"] = t[0] in pc_init
        done[0] += 1
        if done[0] % 200 == 0:
            print(f"  {done[0]}/{len(pop)}", flush=True)
        return r

    with ThreadPoolExecutor(max_workers=args.jobs) as ex:
        res = list(ex.map(go, pop))

    counts = {"clean": 0, "dataflow_divergent": 0, "unchecked": 0}
    tiers = {"provable": 0, "advisory": 0}
    funcs = {}
    for r in res:
        if "error" in r:
            r["classification"] = "unchecked"
        counts[r["classification"]] = counts.get(r["classification"], 0) + 1
        if r.get("tier") in tiers:
            tiers[r["tier"]] += 1
        funcs["%s/%s" % (r["overlay"], r["func"])] = r
    out = {"_meta": {
        "generator": "tools/overlay/audit_paint_dataflow_census.py --all",
        "population": "byte_perfect overlay functions with an active "
                      "@PostProc: patch_insn_reg_field directive",
        "warning": "PROVABLE and ADVISORY are different epistemic tiers. "
                   "ADVISORY is a screening signal with documented "
                   "false-positive modes, NOT proof. Never strip a paint on "
                   "an advisory flag: it trades a verified property (bytes "
                   "match) for an unverified one.",
        "total": len(pop), "counts": counts, "tier_counts": tiers,
    }, "functions": funcs}
    OUT_JSON.write_text(json.dumps(out, indent=1) + "\n")
    print(counts, tiers, "->", OUT_JSON)
    return 0


if __name__ == "__main__":
    sys.exit(main())
