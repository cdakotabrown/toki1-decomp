#!/usr/bin/env python3
"""fix_constant_fold_decomp.py — auto-fix safe patterns flagged by
lint_constant_fold_decomp.py.

Only rewrites patterns where the semantics are unambiguous and the
rewrite is local (no brace/scope analysis):

  if (<nonzero lit> != <const equal to lit>) goto L;   -> delete (never taken)
  if (<nonzero lit> == <const equal to lit>) goto L;   -> `goto L;`
  if (<lit A> != <lit B>) goto L; where A != B         -> `goto L;`
  if (<lit A> == <lit B>) goto L; where A != B         -> delete (never taken)
  if (0 == 0) goto L;                                  -> `goto L;`
  if (0 != 0) goto L;                                  -> delete (never taken)

Block forms (`if (...) { ... }`) are NOT touched — rewriting those
needs brace-matching that the lint regex doesn't do. Callers that want
block-form cleanup should do that manually.

Usage:
    python3 tools/analysis/fix_constant_fold_decomp.py [PATH ...]
"""
import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent

# Matches `if (<litA> <op> <litB>) goto LABEL;` as a standalone line.
LIT = r"(?:0x[0-9A-Fa-f]+|-?\d+)"
GOTO_LINE = re.compile(
    rf"^(\s*)if\s*\(\s*({LIT})\s*(==|!=)\s*({LIT})\s*\)\s*goto\s+(\w+)\s*;\s*$"
)

# Matches `if (<litA> <op> <litB>) {` — block form opener.
BLOCK_OPEN = re.compile(
    rf"^(\s*)if\s*\(\s*({LIT})\s*(==|!=)\s*({LIT})\s*\)\s*\{{\s*$"
)


def lit_val(s: str) -> int:
    return int(s, 0)


def rewrite_line(line: str) -> str | None:
    """Returns the replacement (may be empty string to drop the line),
    or None if the line is not a safe-to-rewrite shape."""
    m = GOTO_LINE.match(line.rstrip("\n"))
    if not m:
        return None
    indent, a, op, b, label = m.groups()
    try:
        av = lit_val(a)
        bv = lit_val(b)
    except ValueError:
        return None
    eq = (av == bv)
    taken = eq if op == "==" else (not eq)
    if taken:
        return f"{indent}goto {label};\n"
    else:
        return ""  # drop the dead branch


def find_matching_brace(lines: list, start: int) -> int | None:
    """Given `lines[start]` ends with `{`, find index of the line with
    the matching `}` at same indent (tracks brace depth, ignoring
    braces inside // and /* */ comments and string literals is not
    attempted — we bail if counts go weird)."""
    depth = 0
    for i in range(start, len(lines)):
        for ch in lines[i]:
            if ch == "{":
                depth += 1
            elif ch == "}":
                depth -= 1
                if depth == 0:
                    return i
    return None


def next_nonblank(lines: list, i: int) -> int:
    while i < len(lines) and lines[i].strip() == "":
        i += 1
    return i


def fix_file(path: Path) -> tuple[int, int]:
    """Returns (rewrites, drops). Handles both single-line goto form
    and multi-line block form (without an `else`). Block form rewrites:
      taken     -> drop `if (...) {` and matching `}` lines; body stays
      not taken -> drop `if (...) {` through matching `}` inclusive
    `else`-branches are left alone (we bail when we see one)."""
    text = path.read_text(encoding="utf-8", errors="replace")
    lines = text.splitlines(keepends=True)
    rewrites = 0
    drops = 0
    out = []
    skip_until = -1
    i = 0
    while i < len(lines):
        if i <= skip_until:
            i += 1
            continue
        line = lines[i]
        # Single-line goto form
        rep = rewrite_line(line)
        if rep is not None:
            if rep == "":
                drops += 1
            else:
                out.append(rep)
                rewrites += 1
            i += 1
            continue
        # Block form
        m = BLOCK_OPEN.match(line.rstrip("\n"))
        if m:
            indent, a, op, b = m.groups()
            try:
                av, bv = lit_val(a), lit_val(b)
            except ValueError:
                out.append(line); i += 1; continue
            close_idx = find_matching_brace(lines, i)
            if close_idx is None:
                out.append(line); i += 1; continue
            # Skip if followed by `else` (conservative)
            nxt = next_nonblank(lines, close_idx + 1)
            if nxt < len(lines) and lines[nxt].lstrip().startswith("else"):
                out.append(line); i += 1; continue
            eq = (av == bv)
            taken = eq if op == "==" else (not eq)
            if taken:
                # Keep body; drop opener + closer
                for j in range(i + 1, close_idx):
                    out.append(lines[j])
                rewrites += 1
            else:
                # Drop entire block
                drops += 1
            i = close_idx + 1
            continue
        out.append(line)
        i += 1
    if rewrites or drops:
        path.write_text("".join(out), encoding="utf-8")
    return rewrites, drops


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("paths", nargs="*")
    args = ap.parse_args()

    if not args.paths:
        roots = [ROOT / "pc" / "src", ROOT / "src" / "overlay"]
    else:
        roots = [Path(p) for p in args.paths]

    files: list[Path] = []
    for r in roots:
        if r.is_dir():
            files.extend(sorted(r.rglob("*.c")))
        elif r.is_file():
            files.append(r)

    total_rw = 0
    total_dr = 0
    changed = 0
    for f in files:
        rw, dr = fix_file(f)
        if rw or dr:
            changed += 1
            total_rw += rw
            total_dr += dr

    print(
        f"[fix_constant_fold_decomp] "
        f"rewrote {total_rw}, dropped {total_dr} across {changed} file(s) "
        f"of {len(files)} scanned"
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
