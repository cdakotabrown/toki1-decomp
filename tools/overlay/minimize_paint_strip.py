#!/usr/bin/env python3
"""Greedy MINIMAL-STRIP search over a `patch_insn_reg_field` paint layer.

Removes paint entries until `audit_paint_isomorphism.py` reports no
reaching-definition producer changes, preferring the removal set that costs
the fewest entries.  The source file is NEVER mutated unless `--apply` is
given; every trial runs on an in-memory copy of the text.

    python3 tools/overlay/minimize_paint_strip.py DATE func_80134CF4
    python3 tools/overlay/minimize_paint_strip.py DATE func_80134CF4 --apply

Only the entry TOKENS are deleted; every other byte of the directive line is
preserved verbatim, so prose and comment terminators cannot be mangled.  A
directive left with no entries is renamed `@PostProcDISABLED` rather than
deleted.

=============================================================================
THIS TOOL PROPOSES, A HUMAN DISPOSES
=============================================================================
The strip set is a PROPOSAL.  The underlying iso signal is ADVISORY, not
provable — read the FALSE-POSITIVE MODES in
`tools/overlay/audit_paint_isomorphism.py` before applying.  In particular:

  * UPSTREAM-REPAIR FP MODE (2026-08-09).  A paint whose removal
    *creates* an orphaned read is REPAIRING damage done upstream by
    `scheduler_normalizer` / `swap_instructions`, not causing it.  Stripping
    it makes the object worse.  `nflags()` below filters that direction out
    (unpainted reads an undefined register, painted reads a real def), which
    is why this driver — and not raw `audit_paint_isomorphism.py` — is the
    thing you point at a file.  The filter is a heuristic, not a proof.

  * A score DROP after stripping an unsound paint is CORRECT and expected.
    Honest-down is a win.  Never "restore" the higher number.

  * When `audit_paint_isomorphism.py` flags an ORPHANED READ, check first
    whether the MISSING half is a DEF: ADDING that entry is usually free,
    whereas stripping the layer costs every word the layer bought
    (SHUGAKU/func_801337FC: adding `0x84:rt:s0:s1` restored 138/151 ->
    150/151).  This tool only ever removes — it cannot propose completions.

  * It cannot see WRONG-SYMBOL paints (a `%lo` re-pointed at a different
    `lui %hi` of the same shape) or `swap_registers` live-range collapses.

After applying, re-measure with `resolve_overlay_relocs.py` and re-run
`test_overlay_match.py --semcheck`.
"""
import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import audit_paint_isomorphism as iso

ROOT = Path(__file__).resolve().parents[2]
ENT = re.compile(r"0x[0-9a-fA-F]+:(?:rs|rt|rd):\w+:\w+")


def _is_directive(line):
    return "@PostProc" in line and "patch_insn_reg_field" in line


def entries(text):
    out = []
    for line in text.splitlines():
        if _is_directive(line):
            out += ENT.findall(line)
    return out


def with_removed(text, removed):
    """Delete only the listed entry TOKENS; every other byte of the line is
    preserved verbatim (so prose, comment terminators etc. cannot be mangled)."""
    rm = set(removed)
    out = []
    for line in text.splitlines():
        if _is_directive(line):
            new = ENT.sub(lambda m: "" if m.group(0) in rm else m.group(0), line)
            new = re.sub(r" {2,}", " ", new)
            if not ENT.search(new):
                new = new.replace("@PostProc", "@PostProcDISABLED")
            line = new
        out.append(line)
    return "\n".join(out) + "\n"


def _real_def(s):
    return any(isinstance(x, int) for x in s)


def _undef(s):
    return all((x == 'E' or isinstance(x, tuple)) for x in s)


def nflags(ov, fn, text):
    """Iso flags, minus the UPSTREAM-REPAIR direction.

    A flag where the UNPAINTED object reads an undefined register (the entry
    value or a call-clobbered one) and the PAINTED object reads a real
    definition is the paint FIXING an upstream (scheduler_normalizer /
    swap_instructions) def-use break, not causing one.  Those are false
    positives for this audit and must not drive a strip.
    """
    r = iso.audit(ov, fn, src_text=text)
    if "error" in r:
        return None, r["error"]
    out = []
    for i, ua, ub, sa, sb in r["bad"]:
        pairs = list(zip(sa, sb))
        if (any(_undef(a) and _real_def(b) for a, b in pairs) and
                all(a == b or (_undef(a) and _real_def(b)) for a, b in pairs)):
            continue
        out.append((i, ua, ub, sa, sb))
    return out, None


def main():
    ap = argparse.ArgumentParser(
        description="Greedy minimal patch_insn_reg_field strip search "
                    "(repair-aware).  PROPOSES a removal set; read the module "
                    "docstring before applying.",
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("overlay")
    ap.add_argument("func")
    ap.add_argument("--apply", action="store_true",
                    help="write the strip set back into the .c (default: "
                         "propose only, never mutate)")
    ap.add_argument("--max-iters", type=int, default=60,
                    help="greedy iteration cap (default 60)")
    args = ap.parse_args()

    ov, fn = args.overlay, args.func
    src = ROOT / "src" / "overlay" / ov / f"{fn}.c"
    text = src.read_text(errors="replace")
    all_e = entries(text)
    print(f"{len(all_e)} paint entries")
    removed = []
    for it in range(args.max_iters):
        bad, err = nflags(ov, fn, with_removed(text, removed))
        if err:
            print("ERR", err)
            return 2
        print(f"iter {it}: flags={len(bad)} removed={len(removed)}")
        if not bad:
            break
        # candidate entries: those at a flagged index, or at any producer index
        # of either side of a flag
        cand = set()
        for i, ua, ub, sa, sb in bad:
            offs = {i}
            for s in list(sa) + list(sb):
                for p in s:
                    if isinstance(p, int):
                        offs.add(p)
                    elif isinstance(p, tuple):
                        offs.add(p[1])
            for e in all_e:
                if e in removed:
                    continue
                o = int(e.split(":")[0], 16) // 4
                if o in offs:
                    cand.add(e)
        if not cand:
            print("no candidates left; residual flags:")
            for i, ua, ub, sa, sb in bad:
                print("  ", i, ua, ub, sa, sb)
            break
        best = None
        for e in sorted(cand):
            b2, er = nflags(ov, fn, with_removed(text, removed + [e]))
            if er or b2 is None:
                continue
            score = (len(b2), 1)
            if best is None or score < best[0]:
                best = (score, e, len(b2))
        if best is None:
            print("all candidate removals failed")
            break
        if best[2] >= len(bad):
            # no single removal helps: remove the whole candidate set
            print(f"  no single-entry win; removing all {len(cand)} candidates")
            removed += sorted(cand)
        else:
            print(f"  removing {best[1]} -> flags {best[2]}")
            removed.append(best[1])
    print("\nREMOVE SET (%d):" % len(removed))
    print(" ".join(sorted(removed, key=lambda e: int(e.split(':')[0], 16))))
    if args.apply:
        src.write_text(with_removed(text, removed))
        print("APPLIED")
    return 0


if __name__ == "__main__":
    sys.exit(main())
