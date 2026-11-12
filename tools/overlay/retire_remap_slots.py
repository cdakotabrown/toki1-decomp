#!/usr/bin/env python3
"""Retire `@PostProc: remap_stack_slots` directives whose every `--from/--to`
pair is a **flat +16** — the shift `-Wc,-pic1` already performs natively.

Sibling of `tools/overlay/retire_frame_directive.py`; same guards, same
comment-shape surgery, same auto-revert.  Read that tool first.

────────────────────────────────────────────────────────────────────────────
Why a pure-+16 remap is redundant under -pic1
────────────────────────────────────────────────────────────────────────────

`-Wc,-pic1` makes ugen reserve the IRIX-4 PIC linkage area
(docs/reference/FRAME-SIZE-ROOTCAUSE.md).  Concretely it

  * grows every emitted frame by 16 bytes,
  * leaves the bottom 16-byte outgoing-arg area and the `$ra` slot where they
    were, and
  * shifts everything above — i.e. **the local slots** — up by 16.

`patch_stack_complete --old N --new N+16` only reproduces the first and the
incoming-argument half of that (it shifts refs at offset `>= --old`).  The
local slots it leaves alone, which is exactly the hole
`remap_stack_slots --from L --to L+16` was dug to fill.

So for a file whose frame directive is `+16` *and* whose every remap pair is
`+16`, the pair of directives is a hand-rolled `-pic1`.  Deleting **both**
lets `tools/build/ido_flags.py` supply `-Wc,-pic1` (a file with no frame
directive and no explicit `-Wc,-picN` gets it automatically) and the compiler
emits the offsets itself.

They must move together.  Deleting only the remap would leave the file on
`pic0` with unshifted locals; deleting only the frame directive would apply
`pic1`'s +16 to locals *and* the remap's +16 on top of it.

`--rebase-frame` handles the 39 files whose remap is pure +16 but whose frame
directive is some other delta (+24, +8, ...).  There the remap still retires,
but the frame directive has to stay and gets its `--old` advanced by 16 to
account for the frame `-pic1` now emits:

    patch_stack_complete --old N --new M   +   remap +16     (pic0)
 -> patch_stack_complete --old N+16 --new M                  (pic1)

which is algebraically the same map (locals: pic1 does the +16; incoming args
at X >= N: pic1 gives X+16, then the directive adds M-N-16, total X+M-N — as
before).  That retires one directive instead of two.

────────────────────────────────────────────────────────────────────────────
SAFETY — three independent guards, none of them optional
────────────────────────────────────────────────────────────────────────────

1. **Byte guard.**  The FINAL post-PostProc `.text` must be byte-identical
   before and after.  This edit is meant to be a pure no-op on emitted bytes;
   anything else is a codegen change and is reverted.
2. **Verdict guard.**  The AUTHORITATIVE resolved-link verdict
   (`✅ BYTE-PERFECT after name-derived link` / `◐ .rodata-blocked`) must be
   the same kind of win after as before.  The masked `RELOC-ONLY:` line is
   never consulted.
3. **Length guard.**  On a resolved win, `resolve_overlay_relocs.py -v` must
   report `len t=X c=X` with X equal — the comparator stops at the shorter
   buffer, so an unequal length means part of the window was never looked at.

Plus a structural guard: after the edit the file must carry no
`remap_stack_slots`, no frame directive (unless `--rebase-frame`), and exactly
the same set of *other* `@PostProc` directives.

Any failure restores the file byte-for-byte from an in-memory backup and logs
a refusal.  There is no flag to disable that.

Usage:
    retire_remap_slots.py DATE func_801396DC
    retire_remap_slots.py --batch 20 --jobs 6
    retire_remap_slots.py --dry-run --all
    retire_remap_slots.py --rebase-frame --batch 20
    retire_remap_slots.py --list            # eligibility census, no edits
"""

import argparse
import json
import re
import sys
from collections import Counter
from concurrent.futures import ProcessPoolExecutor, as_completed
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ROOT = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "build"))
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

from ido_flags import has_explicit_pic, has_frame_directive    # noqa: E402
from overlay_postproc import _parse_postproc_line              # noqa: E402
from retire_frame_directive import (                           # noqa: E402
    EXCLUDED, SRC, STATUS_FILE, WINS,
    compiler_of, final_text_bytes, frame_directives,
    postproc_lines, remove_directive_line, verdict,
)

RESOLVER = ROOT / "tools" / "overlay" / "resolve_overlay_relocs.py"

PIC1_DELTA = 16
_REMAP_RE = re.compile(r'\bremap_stack_slots\b')
_FRAME_RE = re.compile(r'\bpatch_stack(?:_complete)?\b')
_OLD_RE = re.compile(r'--old\s+(\d+)')


# ───────────────────────── header parsing ──────────────────────────

def remap_directives(text: str) -> List[Tuple[int, str, List[Tuple[int, int]]]]:
    """(line index, body, pairs) for each remap_stack_slots directive.

    Pairs are decoded with `overlay_postproc._parse_postproc_line`, the same
    parser the build uses, so both the `--from/--to` and the `--map 'a:b,...'`
    spellings are read exactly as the chain would read them.  A directive the
    build cannot parse raises, and the caller turns that into a refusal rather
    than guessing.
    """
    out = []
    for i, body in postproc_lines(text):
        if not _REMAP_RE.search(body):
            continue
        step = _parse_postproc_line(body)
        pairs = step.get("pairs", []) if step and step.get(
            "type") == "remap_stack_slots" else []
        out.append((i, body, pairs))
    return out


def other_postproc_ex(text: str) -> List[str]:
    """Bodies of the @PostProc directives that are neither frame nor remap."""
    return [re.sub(r'\s*\*+/?\s*$', '', b).strip()
            for _, b in postproc_lines(text)
            if not _FRAME_RE.search(b) and not _REMAP_RE.search(b)]


# ───────────────────────── the edit ──────────────────────────

def _tidy_empty_comment(lines: List[str]) -> List[str]:
    """Drop a comment block left holding nothing: `/*` then `*/`.

    `remove_directive_line` can leave a bare `/*` when the directive it
    deleted opened a block whose only other line was the terminator.  Narrow
    on purpose — exact `/*` followed by exact `*/`, nothing else matches.
    """
    out = []
    i = 0
    while i < len(lines):
        if (lines[i].strip() == '/*' and i + 1 < len(lines)
                and lines[i + 1].strip() == '*/'):
            i += 2
            continue
        out.append(lines[i])
        i += 1
    return out


_PIC_RE = re.compile(r'-Wc,-pic[012]\b')


def _stamp_pic1(lines: List[str]) -> List[str]:
    """Add `-Wc,-pic1` to (or create) the file's @CompileFlags directive.

    Only used by `--rebase-frame`, where the file keeps a frame directive and
    so cannot get `-pic1` from the derived policy in `ido_flags.pic_flags`.
    """
    for i, line in enumerate(lines):
        after = line.lstrip().lstrip('/*').lstrip('*').lstrip()
        if not after.startswith('@CompileFlags:'):
            continue
        if _PIC_RE.search(line):
            raise ValueError("file already names a -Wc,-pic mode")
        m = re.match(r'^(.*?)(\s*\*+/\s*)$', line)
        lines = list(lines)
        lines[i] = (f"{m.group(1)} -Wc,-pic1{m.group(2).rstrip()}" if m
                    else f"{line.rstrip()} -Wc,-pic1")
        return lines
    return ["/* @CompileFlags: -Wc,-pic1 */",
            "/* The remap_stack_slots +16 this file used to carry was the",
            " * local-slot half of -pic1; -pic1 now emits it.  The frame",
            " * directive that remains is the RESIDUAL delta only, and because",
            " * ido_flags derives the PIC mode from the absence of a frame",
            " * directive, the mode has to be named explicitly here.",
            " * See docs/reference/FRAME-SIZE-ROOTCAUSE.md.",
            " */"] + list(lines)


def apply_edit(text: str, rebase_frame: bool) -> Tuple[str, str]:
    """Remove the pure-+16 remap directive(s) and deal with the frame one.

    Default: delete the frame directive too (it is +16 and `-pic1` replaces
    both).  `rebase_frame`: keep it, advancing `--old` by 16.

    Lines are edited from the bottom up so that earlier indices stay valid and
    so `remove_directive_line`'s "promote the next line into the block opener"
    case never looks at a line that is itself about to go.
    """
    remaps = remap_directives(text)
    frames = frame_directives(text)
    if not remaps:
        raise ValueError("no remap_stack_slots directive")

    raw = text.splitlines()
    trailing_nl = text.endswith('\n')

    to_delete = sorted((i for i, _, _ in remaps), reverse=True)
    note_bits = [f"remap x{len(remaps)}"]

    if rebase_frame:
        if len(frames) != 1:
            raise ValueError(f"expected exactly 1 frame directive, "
                             f"found {len(frames)}")
        idx, body, old, new = frames[0]
        if old is None or new is None:
            raise ValueError("malformed frame directive (no --old/--new)")
        raw = list(raw)
        raw[idx] = _OLD_RE.sub(f"--old {old + PIC1_DELTA}", raw[idx], count=1)
        if _OLD_RE.search(raw[idx]).group(1) != str(old + PIC1_DELTA):
            raise ValueError("failed to rewrite --old")
        note_bits.append(f"frame --old {old}->{old + PIC1_DELTA} --new {new}")
    else:
        if len(frames) != 1:
            raise ValueError(f"expected exactly 1 frame directive, "
                             f"found {len(frames)}")
        idx, body, old, new = frames[0]
        to_delete = sorted(to_delete + [idx], reverse=True)
        note_bits.append(f"frame --old {old} --new {new}")

    for i in to_delete:
        raw = remove_directive_line(raw, i)
    raw = _tidy_empty_comment(raw)
    if rebase_frame:
        # Last, so the line-index arithmetic above is unaffected: ido_flags
        # DERIVES -pic1 from the ABSENCE of a frame directive, and this file
        # keeps one, so the mode has to be said out loud or the rebased --old
        # will not match the (still pic0) prologue and the chain dies with
        # "No function prologue found with frame size N".
        raw = _stamp_pic1(raw)
        note_bits.append("+@CompileFlags: -Wc,-pic1")
    return '\n'.join(raw) + ('\n' if trailing_nl else ''), "; ".join(note_bits)


# ───────────────────────── length guard ──────────────────────────

_LEN_RE = re.compile(r'len t=(\d+) c=(\d+)')


def resolved_lengths(overlay: str, func: str,
                     timeout: int = 300) -> Optional[Tuple[int, int]]:
    """`(len_target, len_compiled)` from the resolver, or None if it says none.

    None is not a failure by itself — a function with no relocations never
    reaches the resolver's compare line.  It only means this guard abstains.
    """
    import subprocess
    try:
        r = subprocess.run(
            [sys.executable, str(RESOLVER), overlay, func, "-v"],
            capture_output=True, text=True, timeout=timeout, cwd=str(ROOT))
    except subprocess.TimeoutExpired:
        return None
    m = _LEN_RE.search(r.stdout + r.stderr)
    return (int(m.group(1)), int(m.group(2))) if m else None


# ───────────────────────── eligibility ──────────────────────────

def load_status() -> Dict:
    return json.load(open(STATUS_FILE))


def eligible(overlay: str, func: str, status: Dict,
             rebase_frame: bool = False,
             require_byte_perfect: bool = True) -> Optional[str]:
    """None if eligible, else the reason it is not."""
    if (overlay, func) in EXCLUDED:
        return "excluded (owned by another workstream)"
    p = SRC / overlay / f"{func}.c"
    if not p.exists():
        return "no .c on disk"
    if compiler_of(p).startswith("gcc"):
        return "GCC translation unit (-Wc,-pic1 is an IDO/ugen flag)"
    st = status.get(overlay, {}).get(func, {}).get("status")
    if require_byte_perfect and st != "byte_perfect":
        return f"status is {st!r}, not byte_perfect"

    text = p.read_text(errors="replace")
    try:
        remaps = remap_directives(text)
    except ValueError as e:
        return f"unparseable remap directive: {e}"
    if not remaps:
        return "no remap_stack_slots directive"
    pairs = [q for _, _, ps in remaps for q in ps]
    if not pairs:
        return "remap directive parses to no pairs"
    deltas = {b - a for a, b in pairs}
    if deltas != {PIC1_DELTA}:
        return (f"remap deltas {sorted(deltas)}, not a pure +16 "
                f"({len(pairs)} pairs)")

    frames = frame_directives(text)
    if len(frames) == 0:
        return ("no frame directive — already on -pic1, so this remap is not "
                "the pic1 shift")
    if len(frames) > 1:
        return f"{len(frames)} frame directives (ambiguous)"
    _, _, old, new = frames[0]
    if old is None or new is None:
        return "malformed frame directive (no --old/--new)"
    delta = new - old
    if rebase_frame:
        if delta == PIC1_DELTA:
            return "frame delta is +16 — handled by the default mode"
        if has_explicit_pic(content=text):
            return "file already names a -Wc,-picN mode"
    else:
        if delta != PIC1_DELTA:
            return (f"frame delta {delta:+d}, not +16 "
                    f"(needs --rebase-frame)")
    return None


def collect_targets(status: Dict, overlay_filter: Optional[str],
                    rebase_frame: bool,
                    require_byte_perfect: bool) -> List[Tuple[str, str]]:
    out = []
    for ov in sorted(status):
        if overlay_filter and ov != overlay_filter:
            continue
        for fn in sorted(status[ov]):
            if eligible(ov, fn, status, rebase_frame,
                        require_byte_perfect) is None:
                out.append((ov, fn))
    return out


# ───────────────────────── the worker ──────────────────────────

def process(job) -> Dict:
    overlay, func, rebase_frame, dry_run, allow_semi = job
    p = SRC / overlay / f"{func}.c"
    res = {"overlay": overlay, "func": func}
    original = p.read_text(errors="replace")

    try:
        new_text, note = apply_edit(original, rebase_frame)
    except ValueError as e:
        res.update(action="refused", reason=f"edit not applicable: {e}")
        return res
    res["note"] = note

    if dry_run:
        res.update(action="dry-run")
        return res

    before = verdict(overlay, func)
    res["before"] = before[0]
    if before[0] not in WINS and not allow_semi:
        res.update(action="refused",
                   reason=f"not a win BEFORE the edit ({before[1]})")
        return res
    bytes_before = final_text_bytes(overlay, func)
    if bytes_before is None:
        res.update(action="refused",
                   reason="could not capture pre-edit bytes for the byte guard")
        return res
    len_before = (resolved_lengths(overlay, func)
                  if before[0] == "RESOLVED_BP" else None)

    p.write_text(new_text)

    def revert(**kw):
        p.write_text(original)
        res.update(action="reverted", **kw)
        return res

    # ── structural guard ──
    problems = []
    if remap_directives(new_text):
        problems.append("remap_stack_slots still present after edit")
    if not rebase_frame and has_frame_directive(content=new_text):
        problems.append("frame directive still present after edit")
    if rebase_frame and not has_frame_directive(content=new_text):
        problems.append("frame directive lost (should have been rebased)")
    if rebase_frame and not has_explicit_pic(content=new_text):
        problems.append("no explicit -Wc,-pic1 after the rebase")
    if sorted(other_postproc_ex(new_text)) != sorted(other_postproc_ex(original)):
        problems.append("other @PostProc directives changed")
    if problems:
        return revert(after="STRUCTURAL", reason="; ".join(problems))

    # ── byte guard ──
    bytes_after = final_text_bytes(overlay, func)
    if bytes_after is None:
        return revert(after="BYTES_FAIL",
                      reason="compile/postproc failed after the edit")
    if bytes_after != bytes_before:
        detail = (f"{len(bytes_before)} -> {len(bytes_after)} bytes"
                  if len(bytes_before) != len(bytes_after)
                  else "same length, different content")
        return revert(after="BYTES_DIFFER",
                      reason=f"emitted bytes changed ({detail})")

    # ── verdict guard ──
    after = verdict(overlay, func)
    res["after"] = after[0]
    if after[0] not in WINS:
        return revert(reason=f"{before[1]} -> {after[1]}")
    if before[0] in WINS and after[0] != before[0]:
        return revert(reason=f"win kind changed: {before[0]} -> {after[0]}")

    # ── length guard ──
    if after[0] == "RESOLVED_BP":
        lens = resolved_lengths(overlay, func)
        if lens is None:
            return revert(reason="resolver gave no `len t= c=` line to check")
        if lens[0] != lens[1]:
            return revert(reason=f"truncated compare: len t={lens[0]} "
                                 f"c={lens[1]} (must be equal)")
        if len_before and len_before != lens:
            return revert(reason=f"compare window changed: {len_before} -> {lens}")
        res["len"] = lens[0]

    res["action"] = "retired"
    res["retired_directives"] = (1 if rebase_frame else 2)
    return res


# ───────────────────────── cli ──────────────────────────

def census(status: Dict) -> None:
    """Eligibility breakdown over every overlay .c that carries a remap."""
    reasons = Counter()
    prime, rebase = [], []
    for c in sorted(SRC.glob("*/*.c")):
        ov, fn = c.parent.name, c.stem
        text = c.read_text(errors="replace")
        try:
            if not remap_directives(text):
                continue
        except ValueError:
            reasons["unparseable remap directive"] += 1
            continue
        why = eligible(ov, fn, status)
        if why is None:
            prime.append((ov, fn))
            continue
        why2 = eligible(ov, fn, status, rebase_frame=True)
        if why2 is None:
            rebase.append((ov, fn))
            continue
        reasons[why.split('(')[0].strip()] += 1
    print(f"eligible, default mode (retires 2 directives): {len(prime)}")
    print(f"eligible, --rebase-frame  (retires 1 directive): {len(rebase)}")
    print("not eligible:")
    for k, v in reasons.most_common():
        print(f"  {v:5d}  {k}")


def main():
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("overlay", nargs="?")
    ap.add_argument("func", nargs="?")
    ap.add_argument("--batch", type=int,
                    help="process the next N eligible functions")
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--overlay", dest="overlay_filter")
    ap.add_argument("--from-list", help="JSON list of [overlay, func] pairs")
    ap.add_argument("--rebase-frame", action="store_true",
                    help="pure-+16 remap but non-+16 frame delta: retire the "
                         "remap and advance the frame directive's --old by 16")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--list", action="store_true",
                    help="print the eligibility census and exit")
    ap.add_argument("--jobs", type=int, default=6)
    ap.add_argument("--allow-semi", action="store_true",
                    help="also consider functions that are not byte_perfect "
                         "(the byte guard still forbids any drift)")
    ap.add_argument("--json-out")
    args = ap.parse_args()

    status = load_status()
    if args.list:
        census(status)
        return 0

    require_bp = not args.allow_semi
    if args.from_list:
        targets = [tuple(x) for x in json.load(open(args.from_list))]
    elif args.overlay and args.func:
        why = eligible(args.overlay, args.func, status, args.rebase_frame,
                       require_bp)
        if why:
            print(f"REFUSED {args.overlay}/{args.func}: {why}")
            return 1
        targets = [(args.overlay, args.func)]
    else:
        targets = collect_targets(status, args.overlay_filter,
                                  args.rebase_frame, require_bp)
        if args.batch:
            targets = targets[:args.batch]
        elif not args.all:
            ap.error("give OVERLAY FUNC, or --batch N, or --all, "
                     "or --from-list, or --list")

    jobs = [(ov, fn, args.rebase_frame, args.dry_run, args.allow_semi)
            for ov, fn in targets]
    print(f"{'DRY-RUN: ' if args.dry_run else ''}retiring pure-+16 "
          f"remap_stack_slots from {len(jobs)} function(s)"
          f"{' (--rebase-frame)' if args.rebase_frame else ''}")

    results = []
    if args.jobs > 1 and len(jobs) > 1:
        with ProcessPoolExecutor(max_workers=args.jobs) as ex:
            futs = {ex.submit(process, j): j for j in jobs}
            for i, f in enumerate(as_completed(futs)):
                r = f.result()
                results.append(r)
                _log(r, i + 1, len(jobs))
    else:
        for i, j in enumerate(jobs):
            r = process(j)
            results.append(r)
            _log(r, i + 1, len(jobs))

    tally = Counter(r["action"] for r in results)
    print("\n" + "=" * 64)
    for k, v in sorted(tally.items()):
        print(f"  {k:10s} {v}")
    print(f"  directives retired: "
          f"{sum(r.get('retired_directives', 0) for r in results)}")
    bad = [r for r in results if r["action"] in ("reverted", "refused")]
    if bad:
        print(f"\n{len(bad)} function(s) did not take the change:")
        for r in bad:
            print(f"  {r['overlay']}/{r['func']}: {r.get('reason', '?')}")
    if args.json_out:
        json.dump(results, open(args.json_out, "w"), indent=1)
    return 0


def _log(r, i, n):
    sym = {"retired": "✓", "dry-run": "·",
           "reverted": "↩", "refused": "✗"}.get(r["action"], "?")
    extra = (f"  {r.get('reason', '')}"
             if r["action"] in ("reverted", "refused") else "")
    print(f"[{i}/{n}] {sym} {r['overlay']}/{r['func']} {r['action']}{extra}")


if __name__ == "__main__":
    sys.exit(main())
