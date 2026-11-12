#!/usr/bin/env python3
"""Sweep scheduler_normalizer fingerprint flags over overlay semis; keep wins.

Many semi-matched overlay functions are one flag away from RELOC-ONLY: their
residual diffs are reordered constants (--const-fp), reordered sp-stores
(--store-fp) or reordered same-base loads (--load-fp), but the function's
`@PostProc: scheduler_normalizer` line was written without those flags.
`auto_infer_postproc.py` adds scheduler_normalizer but never the fingerprint
flags, so these functions sit unconverted.

For each candidate that already carries a scheduler_normalizer line, this:
  1. records the baseline status (skip if already byte_perfect/reloc_only),
  2. tries each missing fingerprint-flag combination on the scheduler line,
     regenerating patch_insn_reg_field after each (gen_reg_patches),
  3. keeps the first combo that reaches BYTE-PERFECT or RELOCATION-ONLY and
     passes the banned-postproc check,
  4. otherwise restores the file byte-for-byte.

Only toggles already-allowed flags and never edits C logic, so it cannot
introduce banned post-processing.  Safe by construction (revert on no-win).

Usage:
  python3 tools/overlay/sweep_fp_flags.py OVERLAY [func ...] [--max-insns N]
  python3 tools/overlay/sweep_fp_flags.py --all [--max-insns N]
"""
import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
SRC = REPO / "src/overlay"
TEST = REPO / "tools/overlay/test_overlay_match.py"
GENP = REPO / "tools/overlay/gen_reg_patches.py"
BANNED = REPO / "tools/build/banned_postproc.py"
STATUS = REPO / "config/overlay_match_status.json"

# Single flags then pairs then the triple, cheapest first so the resulting
# annotation stays minimal.
ALL_FLAGS = ["--const-fp", "--store-fp", "--load-fp"]
FLAG_COMBOS = [
    "--const-fp",
    "--store-fp",
    "--load-fp",
    "--const-fp --store-fp",
    "--const-fp --load-fp",
    "--store-fp --load-fp",
    "--const-fp --store-fp --load-fp",
]

SCHED_RE = re.compile(r"(@PostProc:\s*scheduler_normalizer)([^\n*]*)")
PATCH_RE = re.compile(r"(?m)^.*@PostProc: patch_insn_reg_field.*\n")


def status_of(ov, fn):
    r = subprocess.run([sys.executable, str(TEST), ov, fn],
                       capture_output=True, text=True, cwd=REPO)
    tail = r.stdout.strip().splitlines()[-1] if r.stdout.strip() else ""
    if "BYTE-PERFECT" in tail:
        return "byte_perfect"
    if "RELOC-ONLY" in tail or "RELOCATION-ONLY" in tail:
        return "reloc_only"
    return "semi"


def has_banned(path):
    r = subprocess.run([sys.executable, str(BANNED), str(path)],
                       capture_output=True, text=True, cwd=REPO)
    return "BANNED" in r.stdout.upper()


def set_flags(content, flags):
    """Set the scheduler_normalizer flag tail to exactly `flags`; drop the
    target-bin form and any stale patch_insn_reg_field line."""
    new = SCHED_RE.sub(lambda m: f"{m.group(1)} {flags}".rstrip(),
                       content, count=1)
    new = PATCH_RE.sub("", new)
    return new


def regen(ov, fn):
    for _ in range(2):
        subprocess.run([sys.executable, str(GENP), ov, fn],
                       capture_output=True, text=True, cwd=REPO)


def sweep_one(ov, fn):
    path = SRC / ov / f"{fn}.c"
    if not path.exists():
        return "missing"
    original = path.read_text()
    if not SCHED_RE.search(original):
        return "no-scheduler"

    base = status_of(ov, fn)
    if base in ("byte_perfect", "reloc_only"):
        return f"already-{base}"

    present = [f for f in ALL_FLAGS if f in original]
    try:
        for flags in FLAG_COMBOS:
            want = flags.split()
            # skip combos that don't introduce a flag the file lacks
            if set(want) <= set(present):
                continue
            path.write_text(set_flags(original, flags))
            regen(ov, fn)
            st = status_of(ov, fn)
            if st in ("byte_perfect", "reloc_only") and not has_banned(path):
                return f"WIN:{st}:{flags}"
    finally:
        pass
    path.write_text(original)
    return "no-win"


def candidates(ov, max_insns):
    d = json.loads(STATUS.read_text())
    node = d.get(ov, {})
    fns = node.get("functions", node) if isinstance(node, dict) else {}
    for fn, info in fns.items():
        if not isinstance(info, dict) or info.get("status") != "semi_matched":
            continue
        p = SRC / ov / f"{fn}.c"
        if not p.exists():
            continue
        s = p.read_text()
        if "scheduler_normalizer" not in s:
            continue
        m = re.search(r"\((\d+) insns", s) or re.search(r"@SymbolSize:\s*(\d+)", s)
        n = int(m.group(1)) if m else 9999
        if m and "SymbolSize" in m.group(0):
            n //= 4
        if n > max_insns:
            continue
        yield fn


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("overlay", nargs="?")
    ap.add_argument("funcs", nargs="*")
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--max-insns", type=int, default=120)
    a = ap.parse_args()

    d = json.loads(STATUS.read_text())
    if a.all:
        overlays = [k for k, v in d.items() if isinstance(v, dict)]
    elif a.overlay:
        overlays = [a.overlay]
    else:
        ap.error("give an OVERLAY or --all")

    wins = []
    for ov in overlays:
        funcs = a.funcs if a.funcs else list(candidates(ov, a.max_insns))
        for fn in funcs:
            res = sweep_one(ov, fn)
            if res.startswith("WIN"):
                print(f"  ✓ {ov}/{fn}  {res}")
                wins.append((ov, fn, res))
            elif res in ("missing", "no-scheduler"):
                pass
    print(f"\n{len(wins)} wins")
    for ov, fn, res in wins:
        print(f"  {ov}/{fn}  {res}")


if __name__ == "__main__":
    main()
