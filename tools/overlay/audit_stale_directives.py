#!/usr/bin/env python3
"""Tree-wide **stale @PostProc directive** audit and retirement.

A directive is STALE when deleting it leaves the function exactly as much of a
win as it was before.  That happens a lot: later C fixes, the `-Wc,-pic1`
rollout and the uopt/ugen hook work repeatedly made the compiled stream match
the target on its own, and nobody ever went back to re-test the scaffolding.
Three independent 2026-08-09 investigations each found a large stale fraction
as a *byproduct* (34 % of `unfill_*`, ~66 % of `scheduler_normalizer`-only
files, "most" `pad_trailing_nop`s).  This tool sweeps every directive type.

Algorithm, per byte-perfect function:

  1. **Fast path** — neuter *all* of its directives at once.  If the function
     is still a win, every directive is dead.  One compile, and it is the
     common case.
  2. **Greedy single-drop** — otherwise neuter each remaining directive in
     turn on top of the drops already accepted, keeping the ones whose removal
     preserves the win.
  3. **Fixpoint** — re-run the greedy pass until a full pass accepts nothing
     new (dropping one directive can unblock another).
  4. **Auto-revert** — the real `.c` is only rewritten at the very end, and the
     rewrite is bracketed by the authoritative verdict.  Anything that does not
     come back a win of at least the same rank is restored byte-for-byte.
     A function can never end worse than it started.

VERDICT.  Authoritative only:

    MATCHED         raw compare is byte-perfect, equal lengths
    RESOLVED_BP     `✅ BYTE-PERFECT after name-derived link`, 0 unresolved
    RODATA_BLOCKED  `◐ .rodata-blocked` (effective, legit)

`resolve_function` already requires `len(target) == len(compiled)`, so the
truncated-compare trap (`N/N words exact` over a short compile) cannot produce
a win here.  The masked `RELOC-ONLY:` line is never consulted.

TWO TRAPS this tool is built around (both have cost time before):

  * A directive is neutered by **renaming the keyword**
    (`@PostProc:` → `@DisabledPostProc:`), never by deleting the line.  Deleting
    a line whose text shares the `/*` comment opener breaks the file, and the
    resolver reports that as `compile/postproc failed` — indistinguishable from
    a real chain failure.  Line deletion happens once, at the end, through
    `retire_frame_directive.remove_directive_line`, which knows all five
    comment shapes, and is re-verified afterwards.
  * Directives appear both as `/* @PostProc: ... */` and as ` * @PostProc: ...`
    inside a block comment.  Both are live.  Directive lines are found with the
    shared anchored parser (`parse_postproc.directive_bodies`), the same one the
    build uses, so neither shape is missed.

Measurement never writes to a tracked file: trials compile a temp copy through
`src_override`.

--semis MODE (the non-byte-perfect population).  `semi_matched` and
`count_mismatch` functions have no win to preserve, so the "is it still a win?"
guard does not exist for them.  The guard used instead is the one
`retire_frame_directive.py` applies to its non-win cases:

    the FINAL post-PostProc `.text` bytes must be BYTE-IDENTICAL.

Compile with the full chain, capture the emitted `.text`; neuter the candidate;
recompile; if a single byte moved — even if the function scores *better* — the
directive is live and the trial is rejected.  Changing a semi's output is out
of scope: this tool only removes scaffolding that does nothing.

The fingerprint is the WHOLE final `.text` section of the post-PostProc object,
not just the compare window, so a directive that only moves bytes past the
symbol window (exactly what an oversized `pad_trailing_nop` does) cannot slip
through the window truncation.

Usage:
    audit_stale_directives.py GEKO func_80136648 --dry-run
    audit_stale_directives.py --all --dry-run --jobs 8 --json-out audit.json
    audit_stale_directives.py --batch 50 --jobs 8
    audit_stale_directives.py --from-list batch.json --jobs 8
    audit_stale_directives.py --semis --all --dry-run --max-passes 1 --jobs 12
"""

import argparse
import json
import os
import re
import sys
import tempfile
import time
from collections import Counter
from concurrent.futures import ProcessPoolExecutor, as_completed
from pathlib import Path
from typing import Dict, List, Optional, Sequence, Tuple

ROOT = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "build"))
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

from parse_postproc import directive_bodies              # noqa: E402
from retire_frame_directive import (                     # noqa: E402
    remove_directive_line, EXCLUDED as FRAME_EXCLUDED,
)

SRC = ROOT / "src" / "overlay"
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"

# Same anchored shape the build uses: a directive counts only when its line
# OPENS with the keyword after nothing but comment markers / whitespace.
_COMMENT_PREFIX = r'^\s*(?:(?:/\*+|\*+|//)\s*)*'
_POSTPROC_RE = re.compile(_COMMENT_PREFIX + r'@PostProc:\s*(.*)$')

NEUTER_KEYWORD = "@DisabledPostProc:"

# Owned by other workstreams — never touch.
EXCLUDED = set(FRAME_EXCLUDED)

# Win ranking; a trial may never lower the rank.
RANK = {"RODATA_BLOCKED": 1, "RESOLVED_BP": 2, "MATCHED": 3}
WINS = set(RANK)

# --semis: the statuses that have no win to preserve.
SEMI_STATUSES = ("semi_matched", "count_mismatch")


# ───────────────────────── directive parsing ──────────────────────────

def directive_lines(text: str) -> List[Tuple[int, str]]:
    """(line index, body) for every live @PostProc directive line."""
    out = []
    for i, line in enumerate(text.splitlines()):
        m = _POSTPROC_RE.match(line)
        if m:
            out.append((i, m.group(1)))
    return out


def directive_kind(body: str) -> str:
    """The tool name a directive body invokes (used for the per-type table)."""
    body = re.sub(r'\s*\*+/?\s*$', '', body).strip()
    tok = body.split()[0] if body.split() else "(empty)"
    return tok.rstrip(',:').removesuffix('.py')


def live_bodies(text: str) -> List[str]:
    """Normalised bodies of the live directives, for the structural guard."""
    return [re.sub(r'\s*\*+/?\s*$', '', b).strip() for b in
            directive_bodies(text, '@PostProc:')]


def neuter(text: str, idxs: Sequence[int]) -> str:
    """Rename `@PostProc:` -> `@DisabledPostProc:` on the given lines.

    Renaming, not deleting: the comment block keeps its exact shape, so a
    failure can only ever mean the directive was load-bearing.
    """
    want = set(idxs)
    lines = text.splitlines()
    for i in want:
        lines[i] = lines[i].replace("@PostProc:", NEUTER_KEYWORD, 1)
    return '\n'.join(lines) + ('\n' if text.endswith('\n') else '')


_EMPTY_COMMENT_BODY = re.compile(r'^[\s/*]*$')


def strip_empty_comment_blocks(lines: List[str]) -> List[str]:
    """Drop comment blocks that the deletions emptied out.

    `remove_directive_line` deliberately keeps a block well-formed rather than
    guessing, so a header whose every line was a directive is left as a bare
    `/*` … `*/` pair.  That is valid C but it is litter, so a block with no
    text at all comes out.  A block with ANY prose left is never touched.
    """
    out: List[str] = []
    i = 0
    n = len(lines)
    while i < n:
        if lines[i].strip().startswith('/*'):
            # Walk to the line that closes this block. On the opening line the
            # terminator can only count if it comes AFTER the `/*`.
            j, start = i, lines[i].index('/*') + 2
            while j < n and lines[j].find('*/', start if j == i else 0) < 0:
                j += 1
            if j < n and all(_EMPTY_COMMENT_BODY.match(l) for l in lines[i:j + 1]):
                i = j + 1
                continue
        out.append(lines[i])
        i += 1
    return out


def delete_directives(text: str, idxs: Sequence[int]) -> str:
    """Really remove the directive lines, keeping the comment block valid."""
    raw = text.splitlines()
    trailing_nl = text.endswith('\n')
    for i in sorted(idxs, reverse=True):
        raw = remove_directive_line(raw, i)
    raw = strip_empty_comment_blocks(raw)
    return '\n'.join(raw) + ('\n' if trailing_nl else '')


# ───────────────────────── the authoritative verdict ──────────────────────────

def _verdict_from(overlay: str, func: str, src: Optional[Path]) -> Dict:
    """(kind, detail, bytes) via test_overlay_function + the resolved link.

    The testers print a loud CHAIN-FAILED banner whenever a trial neuters a
    directive another directive depended on.  That is an expected, routine
    outcome here, so their stdout is swallowed; only the verdict is kept.
    """
    import contextlib
    import io
    from test_overlay_match import test_overlay_function
    import resolve_overlay_relocs as R

    sink = io.StringIO()
    try:
        with contextlib.redirect_stdout(sink), contextlib.redirect_stderr(sink):
            t = test_overlay_function(overlay, func, src_override=src,
                                      return_bytes=True)
    except BaseException as e:                                  # noqa: BLE001
        return {"kind": "ERROR", "detail": f"tester raised {type(e).__name__}: {e}"[:200]}
    if t.get('error'):
        return {"kind": "ERROR", "detail": str(t['error'])[:200]}
    if t.get('postproc_failed'):
        return {"kind": "CHAIN_FAILED", "detail": "POSTPROC CHAIN FAILED"}
    if t.get('truncated_surplus'):
        return {"kind": "TRUNCATED", "detail": "compiled body overruns the window"}
    body = t.get('compiled_bytes')
    if t.get('matched'):
        return {"kind": "MATCHED", "detail": "byte-perfect (raw compare)",
                "bytes": body}
    if not t.get('reloc_only'):
        return {"kind": "MISMATCH",
                "detail": (f"{t.get('exact_pct', 0):.1f}% exact, "
                           f"{t.get('n_compiled')}/{t.get('n_target')} insns"),
                "bytes": body}

    # Masked reloc_only is never trusted — settle it with the name-derived link.
    try:
        with contextlib.redirect_stdout(sink), contextlib.redirect_stderr(sink):
            r = R.resolve_function(overlay, func, src_override=src)
    except BaseException as e:                                  # noqa: BLE001
        return {"kind": "ERROR", "detail": f"resolver raised {type(e).__name__}: {e}"[:200]}
    if r.get('error'):
        return {"kind": "ERROR", "detail": f"resolve: {r['error']}"[:200]}
    if r.get('len_target') != r.get('len_compiled'):
        # The truncated-compare trap, made explicit.
        return {"kind": "LEN_MISMATCH",
                "detail": f"len t={r.get('len_target')} c={r.get('len_compiled')}",
                "bytes": body}
    if r.get('matched'):
        return {"kind": "RESOLVED_BP",
                "detail": "✅ BYTE-PERFECT after name-derived link (0 unresolved)",
                "bytes": body}
    if r.get('unresolved'):
        return {"kind": "RODATA_BLOCKED",
                "detail": f"◐ .rodata-blocked (unresolved: {','.join(r['unresolved'])})",
                "bytes": body, "diffs": len(r.get('diffs_after') or ())}
    return {"kind": "NOT_A_WIN",
            "detail": f"⚠️  {len(r.get('diffs_after') or ())} real codegen diffs",
            "bytes": body}


def _bytes_verdict_from(overlay: str, func: str, src: Optional[Path],
                        keep_root: Path) -> Dict:
    """FINAL post-PostProc `.text` fingerprint — the guard for NON-wins.

    `semi_matched` / `count_mismatch` functions have no win to preserve, so the
    only sound question is "did retiring this directive change the emitted
    bytes at all?".  This is the same guard `retire_frame_directive.py` uses
    for its non-win cases (`final_text_bytes`), strengthened in one way: the
    fingerprint is the WHOLE final `.text` of the post-PostProc object, not the
    window-truncated `compiled_bytes`.  `test_overlay_function` chops the
    compiled body to `func_size`, so a directive whose only effect lands past
    the symbol window (an oversized `pad_trailing_nop`, a `strip_trailing_nop`)
    would otherwise read as a no-op.
    """
    import contextlib
    import io
    import shutil
    from test_overlay_match import test_overlay_function, extract_text_bytes

    keep = keep_root / "keep"
    shutil.rmtree(keep, ignore_errors=True)
    keep.mkdir(parents=True, exist_ok=True)

    sink = io.StringIO()
    try:
        with contextlib.redirect_stdout(sink), contextlib.redirect_stderr(sink):
            t = test_overlay_function(overlay, func, src_override=src,
                                      return_bytes=True, keep_dir=str(keep))
    except BaseException as e:                                  # noqa: BLE001
        return {"kind": "ERROR",
                "detail": f"tester raised {type(e).__name__}: {e}"[:200]}
    if t.get('error'):
        return {"kind": "ERROR", "detail": str(t['error'])[:200]}
    if t.get('postproc_failed'):
        return {"kind": "CHAIN_FAILED", "detail": "POSTPROC CHAIN FAILED"}

    obj = keep / overlay / func / f"{func}.o"
    if not obj.exists():
        return {"kind": "ERROR", "detail": "no post-PostProc object kept"}
    try:
        full = extract_text_bytes(obj, None)
    except BaseException as e:                                  # noqa: BLE001
        return {"kind": "ERROR", "detail": f"objcopy: {e}"[:200]}
    if not full:
        return {"kind": "ERROR", "detail": "empty .text in final object"}

    return {
        "kind": "BYTES",
        "detail": (f"{t.get('exact_pct', 0):.1f}% exact, "
                   f"{t.get('reloc_pct', 0):.1f}% masked, "
                   f"{t.get('n_compiled')}/{t.get('n_target')} insns"),
        "bytes": t.get('compiled_bytes'),
        "full": full,
        "n_target": t.get('n_target'),
        "n_compiled": t.get('n_compiled'),
        "exact_pct": round(t.get('exact_pct', 0.0), 1),
        "reloc_pct": round(t.get('reloc_pct', 0.0), 1),
        "truncated_surplus": bool(t.get('truncated_surplus')),
    }


def bytes_accepts(base: Dict, trial: Dict) -> bool:
    """Bytes-identical or nothing.  A better-scoring trial is still REJECTED:
    changing a semi's output is out of scope for a staleness audit."""
    if base is None or base.get("kind") != "BYTES":
        return False
    if trial.get("kind") != "BYTES":
        return False
    return (trial.get("full") == base.get("full")
            and trial.get("bytes") == base.get("bytes"))


def trial_verdict(overlay: str, func: str, text: str, tmpdir: Path,
                  mode: str = "win") -> Dict:
    """Verdict for a candidate source text, without touching the tracked .c."""
    p = tmpdir / f"{func}.c"
    p.write_text(text)
    if mode == "semis":
        return _bytes_verdict_from(overlay, func, p, tmpdir)
    return _verdict_from(overlay, func, p)


def accepts(base: Dict, trial: Dict) -> bool:
    """May `trial` replace `base`?  Never a rank drop, never a byte drift on ◐."""
    if trial["kind"] not in WINS:
        return False
    if base is not None and RANK[trial["kind"]] < RANK[base["kind"]]:
        return False
    if trial["kind"] == "RODATA_BLOCKED":
        # A rodata-blocked win is only "as good" if nothing moved at all: its
        # residual diffs are unverifiable, so byte equality is the guard.
        if base is None or base.get("bytes") is None:
            return False
        if trial.get("bytes") != base["bytes"]:
            return False
    return True


# ───────────────────────── eligibility ──────────────────────────

def load_status() -> Dict:
    return json.load(open(STATUS_FILE))


def eligible(overlay: str, func: str, status: Dict,
             mode: str = "win") -> Optional[str]:
    """None if the function should be audited, else the reason to skip it."""
    if (overlay, func) in EXCLUDED:
        return "excluded (owned by another workstream)"
    st = status.get(overlay, {}).get(func, {}).get("status")
    if mode == "semis":
        if st not in SEMI_STATUSES:
            return f"status is {st!r}, not one of {SEMI_STATUSES}"
    elif st != "byte_perfect":
        return f"status is {st!r}, not byte_perfect"
    p = SRC / overlay / f"{func}.c"
    if not p.exists():
        return "no .c on disk"
    text = p.read_text(errors="replace")
    if not directive_lines(text):
        return "no @PostProc directives"
    return None


def collect_targets(status: Dict, overlay_filter: Optional[str],
                    mode: str = "win") -> List[Tuple[str, str]]:
    out = []
    for ov in sorted(status):
        if overlay_filter and ov != overlay_filter.upper():
            continue
        for fn in sorted(status[ov]):
            if eligible(ov, fn, status, mode) is None:
                out.append((ov, fn))
    return out


# ───────────────────────── the worker ──────────────────────────

_PAD_COUNT_RE = re.compile(r'--count\s+(\d+)')


def _pad_count(body: str) -> Optional[int]:
    m = _PAD_COUNT_RE.search(body)
    return int(m.group(1)) if m else None


def process(job) -> Dict:
    overlay, func, dry_run, max_directives, max_passes, mode = job
    p = SRC / overlay / f"{func}.c"
    original = p.read_text(errors="replace")
    dirs = directive_lines(original)
    kinds = {i: directive_kind(b) for i, b in dirs}
    bodies = {i: re.sub(r'\s*\*+/?\s*$', '', b).strip() for i, b in dirs}
    res = {
        "overlay": overlay, "func": func, "mode": mode,
        "n_directives": len(dirs),
        "kinds": sorted(Counter(kinds.values()).items()),
        "stale": [], "live": [], "passes": 0, "path": None,
    }
    t0 = time.time()

    with tempfile.TemporaryDirectory() as td:
        td = Path(td)
        idxs = [i for i, _ in dirs]

        if mode == "semis":
            # No win to preserve: the guard is bytes-identical, so the baseline
            # MUST be captured first (there is no rank shortcut).
            base = _bytes_verdict_from(overlay, func, None, td)
            res["before"] = base["kind"]
            if base["kind"] != "BYTES":
                res.update(action="refused", elapsed=round(time.time() - t0, 1),
                           reason=f"no baseline bytes ({base['kind']}: "
                                  f"{base['detail']})")
                return res
            res["baseline"] = {k: base[k] for k in
                               ("detail", "n_target", "n_compiled", "exact_pct",
                                "reloc_pct", "truncated_surplus")}
            ok = bytes_accepts
        else:
            base = None
            ok = accepts

        # ── 1. fast path: neuter everything at once ───────────────────────
        allv = trial_verdict(overlay, func, neuter(original, idxs), td, mode)
        if mode != "semis" and allv["kind"] == "MATCHED":
            # Top rank; nothing the baseline could be that this is worse than.
            dead = list(idxs)
            res["path"] = "fast"
            res["passes"] = 1
        else:
            if mode != "semis":
                base = _verdict_from(overlay, func, None)
                res["before"] = base["kind"]
                if base["kind"] not in WINS:
                    res.update(action="refused", elapsed=round(time.time() - t0, 1),
                               reason=f"not a win BEFORE any edit ({base['kind']}: {base['detail']})")
                    return res
            if ok(base, allv):
                dead = list(idxs)
                res["path"] = "fast"
                res["passes"] = 1
            elif len(dirs) > max_directives:
                res.update(action="skipped", elapsed=round(time.time() - t0, 1),
                           reason=f"{len(dirs)} directives > --max-directives "
                                  f"{max_directives} (greedy too expensive)")
                return res
            else:
                # ── 2/3. greedy single-drop, to fixpoint ──────────────────
                dead: List[int] = []
                remaining = list(idxs)
                gains: List[int] = []
                for npass in range(1, max_passes + 1):
                    res["passes"] = npass
                    gained = 0
                    for i in list(remaining):
                        v = trial_verdict(overlay, func,
                                          neuter(original, dead + [i]), td, mode)
                        if ok(base, v):
                            dead.append(i)
                            remaining.remove(i)
                            gained += 1
                    gains.append(gained)
                    # A pass that gains nothing is the fixpoint; a pass that
                    # gains something may have unblocked an earlier directive,
                    # so scan again.
                    if gained == 0:
                        break
                res["gains"] = gains
                res["path"] = "greedy"

    live_idxs = sorted(set(idxs) - set(dead))
    res["stale"] = sorted(kinds[i] for i in dead)
    res["live"] = sorted(kinds[i] for i in live_idxs)
    # Keep the bodies of surviving pad_trailing_nop directives: on a
    # non-matching body a LARGE live pad is CLAUDE.md's "fabrication by
    # addition" signature (nops faking the instruction count), and the point of
    # this sweep is to surface those, not to touch them.
    res["live_pads"] = [{"count": _pad_count(bodies[i]), "body": bodies[i][:120]}
                        for i in live_idxs if kinds[i] == "pad_trailing_nop"]
    res["stale_pads"] = [{"count": _pad_count(bodies[i]), "body": bodies[i][:120]}
                         for i in dead if kinds[i] == "pad_trailing_nop"]
    res["elapsed"] = round(time.time() - t0, 1)

    if not dead:
        res["action"] = "nothing-stale"
        return res
    if dry_run:
        res["action"] = "dry-run"
        return res

    # ── 4. apply for real, then re-verify and auto-revert ─────────────────
    new_text = delete_directives(original, dead)
    expect = sorted(live_bodies(neuter(original, dead)))
    got = sorted(live_bodies(new_text))
    if got != expect:
        res.update(action="refused",
                   reason=f"structural: surviving directives changed "
                          f"({len(got)} vs {len(expect)} expected)")
        return res

    p.write_text(new_text)
    with tempfile.TemporaryDirectory() as td2:
        after = (_bytes_verdict_from(overlay, func, None, Path(td2))
                 if mode == "semis" else _verdict_from(overlay, func, None))
    res["after"] = after["kind"]
    # The real edit deletes lines where the trial renamed a keyword; re-check
    # against the real baseline so a comment-shape accident cannot slip past.
    if base is None:
        good = after["kind"] == "MATCHED"
    else:
        good = ok(base, after)
    if not good:
        p.write_text(original)
        res.update(action="reverted",
                   reason=f"after real deletion: {after['kind']}: {after['detail']}")
        return res
    res["action"] = "retired"
    res["retired"] = len(dead)
    return res


# ───────────────────────── reporting ──────────────────────────

def per_type_table(results: List[Dict]) -> str:
    stale = Counter()
    live = Counter()
    for r in results:
        if r.get("action") in ("refused", "skipped"):
            continue
        stale.update(r.get("stale") or ())
        live.update(r.get("live") or ())
    rows = sorted(set(stale) | set(live),
                  key=lambda k: -(stale[k] + live[k]))
    w = max([len(k) for k in rows] + [9])
    out = [f"{'directive':<{w}} {'tested':>7} {'stale':>7} {'live':>7} {'stale%':>7}",
           "-" * (w + 32)]
    for k in rows:
        tot = stale[k] + live[k]
        out.append(f"{k:<{w}} {tot:>7} {stale[k]:>7} {live[k]:>7} "
                   f"{100.0 * stale[k] / tot:>6.1f}%")
    tot = sum(stale.values()) + sum(live.values())
    out.append("-" * (w + 32))
    out.append(f"{'TOTAL':<{w}} {tot:>7} {sum(stale.values()):>7} "
               f"{sum(live.values()):>7} "
               f"{100.0 * sum(stale.values()) / max(tot, 1):>6.1f}%")
    return '\n'.join(out)


def pad_report(results: List[Dict], frac_flag: float = 0.10,
               frac_blatant: float = 0.50) -> str:
    """Three-way classification of `pad_trailing_nop` on non-matching bodies.

    CLAUDE.md: "A large `pad_trailing_nop` is the same fabrication by addition"
    — goto-dead-code or register-transcription pseudo-C padded with nops to
    fake the instruction count.  A LEGITIMATE pad only adds real trailing nops
    to an ALREADY byte-matching body.  So on this population a live pad splits:

      stale                bytes unchanged without it -> retire
      live, small          a couple of real trailing nops -> plausibly real
      live, LARGE          -> SUSPECTED FABRICATION, flag for review

    The discriminator is `pad / target`: how much of the compare window is nops
    the compiler never emitted.  `real` is the honest instruction count the C
    actually produces (`target - pad`).
    """
    stale = [d for r in results for d in (r.get("stale_pads") or ())]
    rows = []
    for r in results:
        live = r.get("live_pads") or ()
        if not live:
            continue
        b = r.get("baseline") or {}
        cnt = sum(d.get("count") or 0 for d in live)
        nt = b.get("n_target") or 0
        rows.append({"ov": r["overlay"], "fn": r["func"], "n": len(live),
                     "pad": cnt, "target": nt, "real": nt - cnt,
                     "exact": b.get("exact_pct"),
                     "frac": (cnt / nt) if nt else 0.0})
    susp = [x for x in rows if x["frac"] >= frac_flag]
    legit = [x for x in rows if x["frac"] < frac_flag]
    blatant = [x for x in susp if x["frac"] >= frac_blatant]
    out = [f"pad_trailing_nop: {len(stale)} stale, "
           f"{sum(x['n'] for x in rows)} live across {len(rows)} function(s) — "
           f"{len(susp)} SUSPECTED FABRICATION ({len(blatant)} blatant, "
           f"pad >= {frac_blatant:.0%} of the window), {len(legit)} plausibly legit"]

    def _tbl(title, xs):
        out.append("")
        out.append(f"  {title}")
        out.append(f"  {'overlay/func':<30} {'pads':>4} {'pad':>6} {'real':>6} "
                   f"{'target':>7} {'pad/target':>10} {'exact%':>7}")
        for x in sorted(xs, key=lambda y: -y["frac"]):
            ex = "?" if x["exact"] is None else ("%.1f" % x["exact"])
            out.append(f"  {x['ov'] + '/' + x['fn']:<30} {x['n']:>4} {x['pad']:>6} "
                       f"{x['real']:>6} {x['target']:>7} {100 * x['frac']:>9.1f}% "
                       f"{ex:>7}")
    if susp:
        _tbl("SUSPECTED FABRICATION BY ADDITION (review; do NOT retire):", susp)
        if any(x["real"] < 0 for x in susp):
            out.append("  (real < 0 = OVER-PAD: body+nops overruns the compare "
                       "window and the surplus nops are chopped)")
    if legit:
        _tbl("plausibly legitimate (pad is a small tail fixup):", legit)
    return '\n'.join(out)


def _log(r, i, n):
    sym = {"retired": "✓", "dry-run": "·", "nothing-stale": "=",
           "reverted": "↩", "refused": "✗", "skipped": "»"}.get(r["action"], "?")
    if r["action"] in ("retired", "dry-run"):
        extra = (f"  {len(r['stale'])}/{r['n_directives']} stale "
                 f"[{','.join(sorted(set(r['stale'])))}] "
                 f"{r['path']} p{r['passes']} {r['elapsed']}s")
    elif r["action"] in ("reverted", "refused", "skipped"):
        extra = f"  {r.get('reason', '?')}"
    else:
        extra = f"  0/{r['n_directives']} stale {r.get('elapsed', 0)}s"
    print(f"[{i}/{n}] {sym} {r['overlay']}/{r['func']} {r['action']}{extra}",
          flush=True)


# ───────────────────────── cli ──────────────────────────

def main():
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("overlay", nargs="?")
    ap.add_argument("func", nargs="?")
    ap.add_argument("--batch", type=int, help="process the next N eligible functions")
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--overlay", dest="overlay_filter")
    ap.add_argument("--from-list", help="JSON list of [overlay, func] pairs")
    ap.add_argument("--skip-list", help="JSON list of [overlay, func] pairs to exclude")
    ap.add_argument("--dry-run", action="store_true",
                    help="measure only; never write a tracked .c")
    ap.add_argument("--jobs", type=int, default=6)
    ap.add_argument("--max-directives", type=int, default=200,
                    help="skip greedy on functions carrying more than this many "
                         "directives (the fast path is still tried)")
    ap.add_argument("--max-passes", type=int, default=4,
                    help="fixpoint cap for the greedy pass")
    ap.add_argument("--json-out")
    ap.add_argument("--report-from", metavar="JSON",
                    help="re-print the tables from a previous --json-out, "
                         "without recompiling anything")
    ap.add_argument("--semis", action="store_true",
                    help="audit semi_matched / count_mismatch functions instead "
                         "of byte_perfect ones, using the bytes-identical guard")
    args = ap.parse_args()

    mode = "semis" if args.semis else "win"

    if args.report_from:
        results = json.load(open(args.report_from))
        print(per_type_table(results))
        if any(r.get("live_pads") or r.get("stale_pads") for r in results):
            print("\n" + pad_report(results))
        return 0

    status = load_status()

    if args.from_list:
        targets = [tuple(x) for x in json.load(open(args.from_list))]
    elif args.overlay and args.func:
        why = eligible(args.overlay.upper(), args.func, status, mode)
        if why:
            print(f"REFUSED {args.overlay}/{args.func}: {why}")
            return 1
        targets = [(args.overlay.upper(), args.func)]
    else:
        targets = collect_targets(status, args.overlay_filter, mode)
        if args.batch:
            targets = targets[:args.batch]
        elif not args.all:
            ap.error("give OVERLAY FUNC, or --batch N, or --all, or --from-list")

    if args.skip_list:
        skip = {tuple(x) for x in json.load(open(args.skip_list))}
        targets = [t for t in targets if tuple(t) not in skip]

    jobs = [(ov, fn, args.dry_run, args.max_directives, args.max_passes, mode)
            for ov, fn in targets]
    print(f"{'DRY-RUN: ' if args.dry_run else ''}auditing {len(jobs)} function(s) "
          f"with {args.jobs} job(s)"
          f"{'  [--semis: guard = final .text bytes UNCHANGED]' if args.semis else ''}")

    results = []
    t0 = time.time()
    if args.jobs > 1 and len(jobs) > 1:
        with ProcessPoolExecutor(max_workers=args.jobs) as ex:
            futs = {ex.submit(process, j): j for j in jobs}
            for i, f in enumerate(as_completed(futs)):
                try:
                    r = f.result()
                except BaseException as e:                      # noqa: BLE001
                    ov, fn = futs[f][0], futs[f][1]
                    r = {"overlay": ov, "func": fn, "action": "refused",
                         "n_directives": 0, "reason": f"worker crashed: {e}"}
                results.append(r)
                _log(r, i + 1, len(jobs))
    else:
        for i, j in enumerate(jobs):
            r = process(j)
            results.append(r)
            _log(r, i + 1, len(jobs))

    tally = Counter(r["action"] for r in results)
    print("\n" + "=" * 72)
    print(f"elapsed {time.time() - t0:.0f}s")
    for k, v in sorted(tally.items()):
        print(f"  {k:14s} {v}")
    n_stale = sum(len(r.get("stale") or ()) for r in results)
    n_live = sum(len(r.get("live") or ()) for r in results)
    print(f"\n  stale directives: {n_stale}   live: {n_live}")
    fast = sum(1 for r in results if r.get("path") == "fast")
    greedy = [r for r in results if r.get("path") == "greedy"]
    # A LATER pass earning a drop is the thing worth counting: it proves the
    # fixpoint loop is load-bearing (removing one directive unblocked another).
    multi = [r for r in greedy if sum((r.get("gains") or [0])[1:]) > 0]
    late = sum(sum((r.get("gains") or [0])[1:]) for r in greedy)
    print(f"  fast path (all directives dead): {fast}")
    print(f"  greedy:  {len(greedy)}   functions needing a 2nd+ fixpoint pass: "
          f"{len(multi)}  (directives found only after pass 1: {late})")
    print("\n" + per_type_table(results))
    if mode == "semis" and any(r.get("live_pads") or r.get("stale_pads")
                               for r in results):
        print("\n" + pad_report(results))
    bad = [r for r in results if r["action"] in ("reverted", "refused", "skipped")]
    if bad:
        print(f"\n{len(bad)} function(s) refused/reverted/skipped:")
        for r in bad[:80]:
            print(f"  {r['overlay']}/{r['func']}: {r.get('reason', '?')}")
        if len(bad) > 80:
            print(f"  ... and {len(bad) - 80} more")
    if args.json_out:
        json.dump(results, open(args.json_out, "w"), indent=1)
        print(f"\nwrote {args.json_out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
