#!/usr/bin/env python3
"""Retire `@PostProc: patch_stack_complete --old N --new N+16` in favour of
the compiler flag that actually causes the +16: `-Wc,-pic1`.

Root cause: docs/reference/FRAME-SIZE-ROOTCAUSE.md.  `-Wc,-pic1` makes ugen
reserve the IRIX-4 PIC linkage area — a silent +16 bytes on every emitted
frame, instruction stream unchanged.  A file gets `-pic1` automatically once
it no longer carries a frame directive (policy lives in
`tools/build/ido_flags.py`), so *removing the directive is the whole edit*.

Two actions:

  retire      (default)  delete a +16 frame directive.  The file then compiles
                         -pic1 and must still be byte-perfect.
  --stamp-pic0           the inverse: pin a file to the old footing with
                         `@CompileFlags: -Wc,-pic0`.  Needed for the ~271 files
                         that carry no frame directive but whose C was
                         hand-padded with 16 bytes of invented dummy locals
                         (or otherwise reacts to the flag).  Those stamps are a
                         to-do list — the real fix is deleting the invented
                         locals, after which the stamp comes off.

SAFETY.  Every edit is bracketed by the AUTHORITATIVE resolved-link verdict
(`test_overlay_match.py`, whose `✅ BYTE-PERFECT after name-derived link` /
`◐ .rodata-blocked` line is trusted — never the masked `RELOC-ONLY:` line).
A function that is a win before the edit must be the same kind of win after
it, or the file is restored byte-for-byte from an in-memory backup and the
function is logged as a refusal.  There is no flag to disable that.

Usage:
    retire_frame_directive.py GEKO func_80136648
    retire_frame_directive.py --batch 50 --jobs 6
    retire_frame_directive.py --overlay DATE --dry-run
    retire_frame_directive.py --stamp-pic0 --from-list need_pic0.json
"""

import argparse
import json
import os
import re
import subprocess
import sys
from concurrent.futures import ProcessPoolExecutor, as_completed
from pathlib import Path
from typing import Dict, List, Optional, Tuple

ROOT = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "build"))
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

from parse_postproc import directive_bodies          # noqa: E402
from ido_flags import has_frame_directive            # noqa: E402

SRC = ROOT / "src" / "overlay"
STATUS_FILE = ROOT / "config" / "overlay_match_status.json"
TESTER = ROOT / "tools" / "overlay" / "test_overlay_match.py"

# Owned by another workstream — never touch.
EXCLUDED = {
    ("BUNKA_SD", "func_80133D54"),
    ("BUNKA_SD", "func_801373D0"),
    ("BUNKA_SD", "func_8013842C"),
    ("ENDING",   "func_801375BC"),
    ("TEL",      "func_8013D758"),
}

_DIRECTIVE_RE = re.compile(
    r'^\s*(?:(?:/\*+|\*+|//)\s*)*@(?:PostProc|CompileFlags):\s*(.*)$')
_FRAME_RE = re.compile(r'\bpatch_stack(?:_complete)?\b')
_OLD_RE = re.compile(r'--old\s+(\d+)')
_NEW_RE = re.compile(r'--new\s+(\d+)')

PIC1_DELTA = 16


# ───────────────────────── header parsing ──────────────────────────

def postproc_lines(text: str) -> List[Tuple[int, str]]:
    """(index, body) for every real @PostProc directive line."""
    out = []
    for i, line in enumerate(text.splitlines()):
        m = _DIRECTIVE_RE.match(line)
        if m and line.lstrip().lstrip('/*').lstrip('*').lstrip().startswith('@PostProc:'):
            out.append((i, m.group(1)))
    return out


def frame_directives(text: str) -> List[Tuple[int, str, Optional[int], Optional[int]]]:
    """(line index, body, old, new) for each patch_stack[_complete] directive."""
    out = []
    for i, body in postproc_lines(text):
        if not _FRAME_RE.search(body):
            continue
        o = _OLD_RE.search(body)
        n = _NEW_RE.search(body)
        out.append((i, body, int(o.group(1)) if o else None,
                    int(n.group(1)) if n else None))
    return out


def other_postproc(text: str) -> List[str]:
    """Bodies of the non-frame @PostProc directives, normalised."""
    return [re.sub(r'\s*\*+/?\s*$', '', b).strip()
            for _, b in postproc_lines(text) if not _FRAME_RE.search(b)]


def compiler_of(path: Path) -> str:
    for body in directive_bodies(path.read_text(errors="replace"), '@Compiler:'):
        t = body.strip().split()
        if t:
            return t[0]
    return "ido52"


# ───────────────────────── the edits ──────────────────────────

def remove_directive_line(lines: List[str], idx: int) -> List[str]:
    """Delete directive line `idx` while keeping the comment block well-formed.

    Handles all three shapes that occur in the tree (plus two defensive ones):

      a  `/* @PostProc: ... */`   self-contained     -> drop the line
      b  `/* @PostProc: ...`      OPENS a block      -> hand the `/*` to the
                                                        next line, else leave a
                                                        bare `/*`
      c  ` * @PostProc: ...`      inside a block     -> drop the line
      d  ` * @PostProc: ... */`   CLOSES a block     -> leave a bare ` */`
      e  `// @PostProc: ...`                          -> drop the line

    Shape b is the one that has bitten two independent harnesses: a line-wise
    stripper deletes the `/*` and the following prose becomes code.
    """
    line = lines[idx]
    stripped = line.strip()
    indent = line[:len(line) - len(line.lstrip())]

    if stripped.startswith('//'):
        return lines[:idx] + lines[idx + 1:]

    opens = stripped.startswith('/*')
    closes = stripped.endswith('*/') and not stripped.rstrip('*/').strip().endswith('/*')

    if opens and stripped.endswith('*/') and len(stripped) > 3:
        # a: whole comment on one line
        return lines[:idx] + lines[idx + 1:]

    if opens and not closes:
        # b: this line opens a block that continues below.
        nxt = idx + 1
        if nxt < len(lines):
            m = re.match(r'^\s*\*+ ?(.*)$', lines[nxt])
            if m and not lines[nxt].strip().startswith('*/'):
                # Promote the next line to be the block opener, keeping the
                # indentation of the line we are deleting.
                lines = list(lines)
                lines[nxt] = f"{indent}/* {m.group(1)}".rstrip()
                return lines[:idx] + lines[idx + 1:]
        # Nothing sensible to promote — keep the block open on its own line.
        lines = list(lines)
        lines[idx] = f"{indent}/*"
        return lines

    if closes and not opens:
        # d: this line closes the block; keep the terminator.
        lines = list(lines)
        lines[idx] = f"{indent} */"
        return lines

    # c: plain interior line
    return lines[:idx] + lines[idx + 1:]


def apply_retire(text: str) -> Tuple[str, str]:
    """Remove the (single) +16 frame directive. Returns (new_text, note)."""
    fds = frame_directives(text)
    if len(fds) != 1:
        raise ValueError(f"expected exactly 1 frame directive, found {len(fds)}")
    idx, body, old, new = fds[0]
    lines = text.splitlines(keepends=True)
    # operate on newline-less copies then restore
    raw = [l.rstrip('\n') for l in lines]
    trailing_nl = text.endswith('\n')
    out = remove_directive_line(raw, idx)
    new_text = '\n'.join(out) + ('\n' if trailing_nl else '')
    return new_text, f"--old {old} --new {new}"


def apply_stamp_pic0(text: str) -> Tuple[str, str]:
    """Add `-Wc,-pic0` to (or create) the file's @CompileFlags directive."""
    lines = text.splitlines()
    trailing_nl = text.endswith('\n')
    for i, line in enumerate(lines):
        m = _DIRECTIVE_RE.match(line)
        if not m:
            continue
        after = line.lstrip().lstrip('/*').lstrip('*').lstrip()
        if not after.startswith('@CompileFlags:'):
            continue
        if '-Wc,-pic' in line:
            raise ValueError("file already names a -Wc,-pic mode")
        # insert before any trailing comment terminator
        m2 = re.match(r'^(.*?)(\s*\*+/\s*)$', line)
        if m2:
            lines[i] = f"{m2.group(1)} -Wc,-pic0{m2.group(2).rstrip()}"
        else:
            lines[i] = f"{line.rstrip()} -Wc,-pic0"
        return '\n'.join(lines) + ('\n' if trailing_nl else ''), "appended"
    stamp = (
        "/* @CompileFlags: -Wc,-pic0 */\n"
        "/* Pinned to stock pic0. This translation unit's frame already\n"
        " * accounts for the PIC linkage area some other way -- typically\n"
        " * invented padding locals in the decompiled C -- so -Wc,-pic1 would\n"
        " * add the 16 bytes a second time. Model the frame honestly, then\n"
        " * delete this stamp. See docs/reference/FRAME-SIZE-ROOTCAUSE.md.\n"
        " */\n")
    return stamp + text, "created"


# ───────────────────────── verification ──────────────────────────

WIN_RESOLVED = re.compile(r'✅ BYTE-PERFECT after name-derived link')
WIN_RODATA = re.compile(r'◐ .*rodata-blocked')
WIN_DIRECT = re.compile(r'^BYTE-PERFECT: ', re.M)
NOT_A_WIN = re.compile(r'⚠️\s+NOT A WIN')
CHAIN_FAIL = re.compile(r'POSTPROC CHAIN FAILED')


def verdict(overlay: str, func: str, timeout: int = 300) -> Tuple[str, str]:
    """Authoritative verdict. Returns (kind, one-line detail)."""
    try:
        r = subprocess.run(
            [sys.executable, str(TESTER), overlay, func],
            capture_output=True, text=True, timeout=timeout, cwd=str(ROOT))
    except subprocess.TimeoutExpired:
        return "TIMEOUT", "tester timed out"
    out = r.stdout + r.stderr
    if CHAIN_FAIL.search(out):
        return "CHAIN_FAILED", _first(out, CHAIN_FAIL)
    if NOT_A_WIN.search(out):
        return "NOT_A_WIN", _first(out, NOT_A_WIN)
    if WIN_RESOLVED.search(out):
        return "RESOLVED_BP", "✅ BYTE-PERFECT after name-derived link"
    if WIN_RODATA.search(out):
        return "RODATA_BLOCKED", "◐ .rodata-blocked (effective)"
    if WIN_DIRECT.search(out):
        return "DIRECT_BP", "BYTE-PERFECT (no relocs to resolve)"
    m = re.search(r'MISMATCH: \S+ \(([0-9.]+%[^)]*)\)', out)
    if m:
        return "MISMATCH", f"MISMATCH ({m.group(1)})"
    m = re.search(r'Compile failed:.*', out)
    if m:
        return "COMPILE_FAIL", m.group(0)[:160]
    return "UNKNOWN", out.strip().splitlines()[-1][:160] if out.strip() else "no output"


def _first(out: str, pat) -> str:
    m = pat.search(out)
    line = out[:m.start()].split('\n')[-1] + out[m.start():].split('\n')[0]
    return line.strip()[:160]


WINS = {"RESOLVED_BP", "RODATA_BLOCKED", "DIRECT_BP"}


def final_text_bytes(overlay: str, func: str) -> Optional[bytes]:
    """The function's FINAL post-PostProc .text bytes, or None on failure.

    This is the guard for functions that are not (yet) wins: retiring the
    directive must be a pure no-op on the emitted bytes, so a semi stays
    exactly as semi as it was rather than silently drifting.
    """
    from test_overlay_match import test_overlay_function
    try:
        r = test_overlay_function(overlay, func, return_bytes=True)
    except Exception:
        return None
    if r.get("error") or r.get("postproc_failed"):
        return None
    return r.get("compiled_bytes")


def raw_text_bytes(src: Path, force_stock: bool) -> Optional[bytes]:
    """Compile `src` and return its raw .text bytes (pre-PostProc).

    `force_stock=True` reproduces the historical pre-rollout command line
    (no PIC flag at all), bypassing the ido_flags policy.  Used to prove a
    `-Wc,-pic0` stamp freezes a file at exactly its old codegen.
    """
    import tempfile
    from parse_postproc import parse_compile_flags, parse_compile_env
    from ido_flags import base_flags, pic_flags

    inc = ROOT / "include"
    cc = ROOT / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
    cflags = list(parse_compile_flags(src))
    if force_stock:
        cflags = [f for f in cflags if not f.startswith("-Wc,-pic")]
        pic: List[str] = []
    else:
        pic = pic_flags(src, compile_flags=cflags)
    with tempfile.TemporaryDirectory() as td:
        obj = Path(td) / "o.o"
        binf = Path(td) / "o.bin"
        env = os.environ.copy()
        env["TMPDIR"] = td
        env.update(parse_compile_env(src))
        r = subprocess.run(
            [str(cc)] + base_flags([inc, f"{inc}/psyq"]) + pic + cflags
            + ["-c", str(src), "-o", str(obj)],
            capture_output=True, text=True, timeout=300, env=env, cwd=td)
        if r.returncode != 0:
            return None
        r = subprocess.run(["mipsel-linux-gnu-objcopy", "-O", "binary",
                            "--only-section=.text", str(obj), str(binf)],
                           capture_output=True)
        if r.returncode != 0 or not binf.exists():
            return None
        return binf.read_bytes()


# ───────────────────────── eligibility ──────────────────────────

def load_status() -> Dict:
    return json.load(open(STATUS_FILE))


def eligible(overlay: str, func: str, status: Dict,
             require_byte_perfect: bool = True) -> Optional[str]:
    """None if eligible for retirement, else the reason it is not."""
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
    fds = frame_directives(text)
    if not fds:
        return "no frame directive"
    if len(fds) > 1:
        return f"{len(fds)} frame directives (ambiguous)"
    _, _, old, new = fds[0]
    if old is None or new is None:
        return "malformed frame directive (no --old/--new)"
    if new - old != PIC1_DELTA:
        return f"delta {new - old:+d}, not +16 (keeps a recomputed directive)"
    if any("remap_stack_slots" in b for b in other_postproc(text)):
        return "carries remap_stack_slots (--from values need regenerating)"
    return None


def collect_targets(status: Dict, overlay_filter: Optional[str],
                    require_byte_perfect: bool) -> List[Tuple[str, str]]:
    out = []
    for ov in sorted(status):
        if overlay_filter and ov != overlay_filter:
            continue
        for fn in sorted(status[ov]):
            if eligible(ov, fn, status, require_byte_perfect) is None:
                out.append((ov, fn))
    return out


# ───────────────────────── the worker ──────────────────────────

def process(job) -> Dict:
    overlay, func, stamp_pic0, dry_run, skip_before, allow_semi = job
    p = SRC / overlay / f"{func}.c"
    res = {"overlay": overlay, "func": func}
    original = p.read_text(errors="replace")

    try:
        new_text, note = (apply_stamp_pic0(original) if stamp_pic0
                          else apply_retire(original))
    except ValueError as e:
        res.update(action="refused", reason=f"edit not applicable: {e}")
        return res
    res["note"] = note

    if dry_run:
        res.update(action="dry-run", before="(not run)", after="(not run)")
        return res

    if stamp_pic0:
        # The flag rollout has already changed this file's codegen, so the
        # pre-edit *verdict* is meaningless.  The guard that matters is byte
        # equality with the historical stock (no-PIC-flag) compile: the stamp
        # must freeze the file at exactly the bytes it had before the rollout.
        stock = raw_text_bytes(p, force_stock=True)
        if stock is None:
            res.update(action="refused", reason="stock compile failed")
            return res
        p.write_text(new_text)
        got = raw_text_bytes(p, force_stock=False)
        if got is None:
            p.write_text(original)
            res.update(action="reverted", reason="compile failed after stamp")
            return res
        if got != stock:
            p.write_text(original)
            res.update(action="reverted",
                       reason=f".text differs from stock compile "
                              f"({len(got)} vs {len(stock)} bytes)")
            return res
        res["action"] = "stamped"
        return res

    before = ("SKIPPED", "skipped") if skip_before else verdict(overlay, func)
    res["before"] = before[0]

    # A function that is not a win cannot be guarded by the verdict (it has no
    # win to preserve). Guard it by exact byte equality instead: retiring a +16
    # directive in favour of -pic1 must not change the emitted bytes at all.
    byte_guard = before[0] not in WINS
    if byte_guard and not allow_semi:
        res.update(action="refused",
                   reason=f"not a win BEFORE the edit ({before[1]})")
        return res
    bytes_before = final_text_bytes(overlay, func) if byte_guard else None
    if byte_guard and bytes_before is None:
        res.update(action="refused",
                   reason="could not capture pre-edit bytes for the byte guard")
        return res

    p.write_text(new_text)

    # Structural guard: the edit must remove ONLY the frame directive and must
    # not disturb the other directives or break the comment block.
    problems = []
    if not stamp_pic0 and has_frame_directive(content=new_text):
        problems.append("frame directive still present after edit")
    if sorted(other_postproc(new_text)) != sorted(other_postproc(original)):
        problems.append("other @PostProc directives changed")
    if problems:
        p.write_text(original)
        res.update(action="reverted", after="STRUCTURAL",
                   reason="; ".join(problems))
        return res

    if byte_guard:
        bytes_after = final_text_bytes(overlay, func)
        if bytes_after is not None and bytes_after == bytes_before:
            res.update(action="retired", after="BYTES_IDENTICAL")
            return res
        p.write_text(original)
        res.update(action="reverted", after="BYTES_DIFFER",
                   reason=("emitted bytes changed (%s)" %
                           ("compile/postproc failed" if bytes_after is None
                            else f"{len(bytes_before)} -> {len(bytes_after)} bytes"
                            if len(bytes_before) != len(bytes_after)
                            else "same length, different content")))
        return res

    after = verdict(overlay, func)
    res["after"] = after[0]
    if after[0] in WINS and (skip_before or after[0] == before[0]
                             or before[0] in WINS):
        res["action"] = "retired" if not stamp_pic0 else "stamped"
        return res

    p.write_text(original)
    res.update(action="reverted",
               reason=f"{before[1]} -> {after[1]}")
    return res


# ───────────────────────── cli ──────────────────────────

def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("overlay", nargs="?")
    ap.add_argument("func", nargs="?")
    ap.add_argument("--batch", type=int, help="process the next N eligible functions")
    ap.add_argument("--all", action="store_true", help="process every eligible function")
    ap.add_argument("--overlay", dest="overlay_filter", help="restrict to one overlay")
    ap.add_argument("--from-list", help="JSON list of [overlay, func] pairs")
    ap.add_argument("--stamp-pic0", action="store_true",
                    help="inverse action: pin the file to stock pic0")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--jobs", type=int, default=6)
    ap.add_argument("--allow-semi", action="store_true",
                    help="also consider functions that are not byte_perfect "
                         "(they must still not regress)")
    ap.add_argument("--json-out")
    args = ap.parse_args()

    status = load_status()
    require_bp = not args.allow_semi

    if args.from_list:
        targets = [tuple(x) for x in json.load(open(args.from_list))]
    elif args.overlay and args.func:
        targets = [(args.overlay, args.func)]
    else:
        targets = collect_targets(status, args.overlay_filter, require_bp)
        if args.batch:
            targets = targets[:args.batch]
        elif not args.all:
            ap.error("give OVERLAY FUNC, or --batch N, or --all, or --from-list")

    # skip_before for stamping: those files are being pinned to the behaviour
    # they already have, so the pre-edit verdict is the post-edit expectation.
    skip_before = False

    if not args.stamp_pic0:
        # Report why non-targets were skipped when an explicit pair was given.
        if args.overlay and args.func:
            why = eligible(args.overlay, args.func, status, require_bp)
            if why:
                print(f"REFUSED {args.overlay}/{args.func}: {why}")
                return 1

    jobs = [(ov, fn, args.stamp_pic0, args.dry_run, skip_before, args.allow_semi)
            for ov, fn in targets]
    print(f"{'DRY-RUN: ' if args.dry_run else ''}"
          f"{'stamping -Wc,-pic0 on' if args.stamp_pic0 else 'retiring frame directive from'} "
          f"{len(jobs)} function(s)")

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

    from collections import Counter
    tally = Counter(r["action"] for r in results)
    print("\n" + "=" * 64)
    for k, v in sorted(tally.items()):
        print(f"  {k:10s} {v}")
    bad = [r for r in results if r["action"] in ("reverted", "refused")]
    if bad:
        print(f"\n{len(bad)} function(s) did not take the change:")
        for r in bad:
            print(f"  {r['overlay']}/{r['func']}: {r.get('reason', '?')}")
    if args.json_out:
        json.dump(results, open(args.json_out, "w"), indent=1)
    return 0


def _log(r, i, n):
    sym = {"retired": "✓", "stamped": "✓", "dry-run": "·",
           "reverted": "↩", "refused": "✗"}.get(r["action"], "?")
    extra = f"  {r.get('reason', '')}" if r["action"] in ("reverted", "refused") else ""
    print(f"[{i}/{n}] {sym} {r['overlay']}/{r['func']} {r['action']}{extra}")


if __name__ == "__main__":
    sys.exit(main())
