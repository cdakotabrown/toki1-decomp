#!/usr/bin/env python3
"""
lint_constant_fold_decomp.py — flag decompiler-produced always-true/false
conditions that create dead code paths or infinite loops.

A well-known failure mode of the mips-to-c decompiler is that it leaves
arithmetic/comparison expressions in the source even after folding them
against known-constant operands. These look like real branches to a
reader but compile to unconditional jumps, causing:

  - Infinite loops (`if ((1 + 1) < 20) goto label;`)
  - Unreachable code that exercises intended logic (`if (0x80140000 != 0)
    goto skip;  real_logic(); skip:`)

This was the root cause of the GEKO func_80133024 regression that
stalled eleven scenes for multiple sessions. Commit a669b548 fixed 83
similar loops in bulk but this one slipped through because its
constant was a data pointer (`0x80140000`) rather than a small integer.

Usage:
    python3 tools/analysis/lint_constant_fold_decomp.py [PATH ...]

If no path is given, scans src/overlay/ by default. Exit code 1 on hits.
"""
import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent

# Patterns whose left/right-hand sides fold to known constants under
# simple evaluation. Conservative — only catches clearly-decomp-generated
# shapes; hand-written C rarely looks like this.
#
# Each entry: (regex, description). The regex should capture the offending
# condition so we can print it.

INT_LIT = r"(?:0x[0-9A-Fa-f]+|-?\d+)"
SIMPLE_ARITH = rf"(?:{INT_LIT}(?:\s*[+\-*/|&^]\s*{INT_LIT})*)"

PATTERNS = [
    # `if (<hex literal> != 0)` — always true, no side effects. The
    # decomp emits this when it has a pointer constant it didn't bind.
    (
        re.compile(
            r"\bif\s*\(\s*(0x[0-9A-Fa-f]+)\s*!=\s*0\s*\)",
        ),
        "pointer-literal != 0 (always true)",
    ),
    # `if (<hex literal> == 0)` — always false.
    (
        re.compile(
            r"\bif\s*\(\s*(0x[0-9A-Fa-f]+)\s*==\s*0\s*\)",
        ),
        "pointer-literal == 0 (always false)",
    ),
    # `if ((<int expr>) <op> <int expr>)` — constant arithmetic with no
    # variable input. We parenthesize tightly to avoid false positives on
    # mixed `var + const < const` patterns.
    (
        re.compile(
            rf"\bif\s*\(\s*\(?\s*{SIMPLE_ARITH}\s*\)?\s*"
            rf"(?:<|<=|>|>=|==|!=)\s*{SIMPLE_ARITH}\s*\)"
        ),
        "constant-only comparison (folds to true/false)",
    ),
    # `if (((<expr>) <op> <const>) != 0)` where the outer expr is also
    # constant-only. Catches the (1+1) < 20 shape specifically.
    (
        re.compile(
            rf"\bif\s*\(\s*\(\s*\(\s*{SIMPLE_ARITH}\s*\)\s*"
            rf"(?:<|<=|>|>=|==|!=)\s*{SIMPLE_ARITH}\s*\)\s*!=\s*0\s*\)"
        ),
        "wrapped constant-only comparison (folds to true/false)",
    ),
]


def scan_file(path: Path) -> list[tuple[int, str, str]]:
    """Returns list of (line_no, pattern_desc, matched_text)."""
    try:
        text = path.read_text(encoding="utf-8", errors="replace")
    except Exception:
        return []

    hits: list[tuple[int, str, str]] = []
    for ln, line in enumerate(text.splitlines(), start=1):
        stripped = line.strip()
        if stripped.startswith("/*") or stripped.startswith("//"):
            continue
        for pat, desc in PATTERNS:
            m = pat.search(line)
            if m:
                hits.append((ln, desc, line.rstrip()))
                break  # one hit per line is enough
    return hits


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument(
        "paths", nargs="*",
        help="Files or directories to scan (default: src/overlay/)",
    )
    ap.add_argument(
        "--quiet", action="store_true",
        help="Only report count, not individual hits",
    )
    args = ap.parse_args()

    if not args.paths:
        roots = [ROOT / "src" / "overlay"]
    else:
        roots = [Path(p) for p in args.paths]

    files: list[Path] = []
    for r in roots:
        if r.is_dir():
            files.extend(sorted(r.rglob("*.c")))
        elif r.is_file():
            files.append(r)

    total_hits = 0
    files_with_hits = 0
    for f in files:
        hits = scan_file(f)
        if not hits:
            continue
        files_with_hits += 1
        total_hits += len(hits)
        if args.quiet:
            continue
        rel = f.relative_to(ROOT) if f.is_absolute() else f
        for ln, desc, text in hits:
            print(f"{rel}:{ln}: {desc}")
            print(f"    {text}")

    print(
        f"\n[lint_constant_fold_decomp] "
        f"{total_hits} hit(s) across {files_with_hits} file(s) of "
        f"{len(files)} scanned"
    )
    return 1 if total_hits else 0


if __name__ == "__main__":
    sys.exit(main())
