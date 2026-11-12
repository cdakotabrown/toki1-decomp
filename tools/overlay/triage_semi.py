#!/usr/bin/env python3
"""Triage semi_matched overlay functions for cheap semi->reloc_only flips.

Dry-runs gen_reg_patches on each close semi_matched function, decodes the
remaining OTHER (non-register) diff words, and buckets them by the signatures
that turned out to be mis-decompiled C (not codegen blockers), see
docs/reference/MATCHING-TRICKS.md (2026-06-13):

  HIDDEN-ARG   target has a `move aN, vM` (N in a0..a3) the compile lacks
               -> a value belongs in that arg register (delay-slot arg).
  CTRLFLOW     compiled has `lw ra, N(sp)` where target has nop/branch
               -> wrong control flow (call wrongly inside an if, etc.).
  SPILL        compiled has an sp-relative sw/lw the target lacks.
  DEADMOVE     target nop, compiled `move rD, rS` -> clear with nop_at_offset.

Functions whose every OTHER diff falls in these buckets (small NOTHER) are the
best candidates for a quick fix. Usage:

  python3 tools/overlay/triage_semi.py EVENT DATE GYOZI [--min-exact 40]
  python3 tools/overlay/triage_semi.py --all [--max-nother 6] [--jobs 6]
"""
import argparse, json, os, re, subprocess, sys
from concurrent.futures import ProcessPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
STATUS = os.path.join(ROOT, "config", "overlay_match_status.json")


def decode(w):
    op = w >> 26
    funct = w & 0x3F
    rs, rt, rd = (w >> 21) & 0x1F, (w >> 16) & 0x1F, (w >> 11) & 0x1F
    return op, funct, rs, rt, rd


def is_move_to_arg(w):              # or/addu rd,rs,$zero with rd in a0..a3
    op, funct, rs, rt, rd = decode(w)
    return op == 0 and funct in (0x21, 0x25) and rt == 0 and 4 <= rd <= 7


def is_move(w):                     # any `move rD, rS` (or/addu rd,rs,$zero)
    op, funct, rs, rt, rd = decode(w)
    return op == 0 and funct in (0x21, 0x25) and rt == 0 and rd != 0


def is_lw_ra(w):                    # lw $ra, N($sp)
    op, funct, rs, rt, rd = decode(w)
    return op == 0x23 and rt == 31 and rs == 29


def is_spill(w):                    # sw/lw with base $sp
    op = w >> 26
    base = (w >> 21) & 0x1F
    return op in (0x23, 0x2B) and base == 29


def classify(ov, fn):
    try:
        out = subprocess.run(
            ["python3", "tools/overlay/gen_reg_patches.py", ov, fn, "--dry-run"],
            cwd=ROOT, capture_output=True, text=True, timeout=120).stdout
    except Exception as e:
        return (ov, fn, None, str(e), {})
    m = re.search(r"Masked:\s+\d+/\d+\s+\(([\d.]+)%\)\s+NREG=(\d+)\s+NOTHER=(\d+)", out)
    if not m:
        return (ov, fn, None, "no-summary", {})
    masked, nreg, nother = float(m.group(1)), int(m.group(2)), int(m.group(3))
    others = re.findall(r"OTHER \[\s*(\d+)\]\s+target=([0-9a-f]+)\s+compiled=([0-9a-f]+)", out)
    tags = {"HIDDEN-ARG": 0, "CTRLFLOW": 0, "SPILL": 0, "DEADMOVE": 0, "OTHER": 0}
    for idx, t, c in others:
        tw, cw = int(t, 16), int(c, 16)
        if is_move_to_arg(tw):
            tags["HIDDEN-ARG"] += 1
        elif tw == 0 and is_move(cw):
            tags["DEADMOVE"] += 1
        elif is_lw_ra(cw):
            tags["CTRLFLOW"] += 1
        elif is_spill(cw):
            tags["SPILL"] += 1
        else:
            tags["OTHER"] += 1
    return (ov, fn, masked, nother, tags)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("overlays", nargs="*", help="overlay names (default: all)")
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--min-exact", type=float, default=40.0,
                    help="skip semis below this exact_pct (default 40)")
    ap.add_argument("--max-nother", type=int, default=8)
    ap.add_argument("--jobs", type=int, default=6)
    args = ap.parse_args()

    st = json.load(open(STATUS))
    want = set(args.overlays) if args.overlays and not args.all else set(st)
    targets = []
    for ov, fns in st.items():
        if ov not in want:
            continue
        for fn, info in fns.items():
            if info.get("status") != "semi_matched":
                continue
            if info.get("exact_pct", 0) < args.min_exact:
                continue
            if os.path.exists(os.path.join(ROOT, f"src/overlay/{ov}/{fn}.c")):
                targets.append((ov, fn))
    print(f"triaging {len(targets)} semi functions (exact>={args.min_exact}%)...",
          file=sys.stderr)

    results = []
    with ProcessPoolExecutor(max_workers=args.jobs) as ex:
        for r in ex.map(classify, *zip(*targets)) if targets else []:
            results.append(r)

    # candidates: small NOTHER, no/low OTHER (unclassified) bucket
    cands = []
    for ov, fn, masked, nother, tags in results:
        if masked is None or not isinstance(nother, int):
            continue
        if nother == 0 or nother > args.max_nother:
            continue
        unclassified = tags.get("OTHER", 0)
        cands.append((unclassified, nother, -masked, ov, fn, tags))
    cands.sort()
    print(f"\n{'OVERLAY/FUNC':32} {'mask%':>6} NOTH  signatures")
    print("-" * 78)
    for unc, nother, negmask, ov, fn, tags in cands:
        sig = " ".join(f"{k}={v}" for k, v in tags.items() if v)
        clean = "  <- CLEAN" if unc == 0 else ""
        print(f"{ov+'/'+fn:32} {-negmask:6.1f} {nother:>4}  {sig}{clean}")
    clean_n = sum(1 for c in cands if c[0] == 0)
    print(f"\n{len(cands)} candidates, {clean_n} CLEAN (all diffs classified)")


if __name__ == "__main__":
    main()
