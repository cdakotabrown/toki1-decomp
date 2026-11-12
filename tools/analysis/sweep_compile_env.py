#!/usr/bin/env python3
"""Sweep ONE `@CompileEnv:` variable for a main-binary function.

Compiler-hook candidate indices (Hook 8 KEEP_ANDI, Hook 36 UGEN_TEMP_REG,
Hook 37 FORCE_WEB_COLOR, ...) are counted over the translation unit and SHIFT
whenever the instruction stream changes, so UOPT-HOOKS-REFERENCE prescribes
re-deriving them empirically after every source or env edit.  This driver
rewrites a single token inside the file's `@CompileEnv:` line, runs the
authoritative single-function test, and reports the resulting word counts.

The source file is always restored, including on exceptions and Ctrl-C.

Examples:
    # re-derive the Hook 8 index after a source change
    sweep_compile_env.py save_file_select_main src.c IDO52_AS1_KEEP_ANDI \\
        NONE all occ:0 occ:1 occ:2

    # blind-sweep a second web colour onto $v0 (doc: Hook 37 hits 33%)
    sweep_compile_env.py save_file_select_main src.c IDO52_FORCE_WEB_COLOR \\
        --keep 12:a0 $(seq 0 20 | sed 's/$/:v0/')

⚠️ `@CompileEnv` parses into a DICT, so a variable may appear only ONCE —
emitting the same name twice silently keeps one and tests nothing.  To sweep a
second setting alongside a load-bearing one, use `--template`, which builds a
single token:

    --template '12:a0,{}'      ->  IDO52_FORCE_WEB_COLOR=12:a0,7:v0
"""
import argparse
import re
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
TEST = REPO / "tools" / "analysis" / "test_single_match.py"

ENV_LINE = re.compile(r"^(\s*\*\s*@CompileEnv:\s*)(.*)$", re.M)
EXACT = re.compile(r"Exact:\s+(\d+)/(\d+)")
COMPILED = re.compile(r"Compiled:\s+(\d+)")
POSITIONS = re.compile(r"mismatches at positions: (\[[^\]]*\])")

ABSENT = "NONE"


def set_token(text, var, value):
    """Return `text` with `var` set to `value` (ABSENT removes it)."""
    match = ENV_LINE.search(text)
    if not match:
        raise SystemExit("no '@CompileEnv:' line in source")
    env = re.sub(rf"{re.escape(var)}=\S+\s*", "", match.group(2)).strip()
    parts = [] if value == ABSENT else [f"{var}={value}"]
    body = " ".join(parts + ([env] if env else [])).strip()
    return text[: match.start()] + match.group(1) + body + text[match.end() :]


def measure(func):
    """Run the authoritative test; return (exact, compiled, positions)."""
    proc = subprocess.run(
        [sys.executable, str(TEST), func], capture_output=True, text=True, cwd=REPO
    )
    exact = EXACT.search(proc.stdout)
    if not exact:
        return "COMPILE-FAIL", "?", "-"
    compiled = COMPILED.search(proc.stdout)
    positions = POSITIONS.search(proc.stdout)
    return (
        f"{exact.group(1)}/{exact.group(2)}",
        compiled.group(1) if compiled else "?",
        positions.group(1) if positions else "none (byte-perfect?)",
    )


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("func")
    parser.add_argument("source", type=Path)
    parser.add_argument("var")
    parser.add_argument("values", nargs="+")
    parser.add_argument(
        "--template",
        default="{}",
        help="wrap each value into one token, e.g. '12:a0,{}' (the var may "
        "appear only once in @CompileEnv)",
    )
    args = parser.parse_args()

    original = args.source.read_text()
    best = None
    try:
        for value in args.values:
            token = value if value == ABSENT else args.template.format(value)
            args.source.write_text(set_token(original, args.var, token))
            exact, compiled, positions = measure(args.func)
            print(f"{token:<18} exact={exact:<10} compiled={compiled:<5} {positions}")
            sys.stdout.flush()
            score = int(exact.split("/")[0]) if "/" in exact else -1
            if best is None or score > best[0]:
                best = (score, value)
    finally:
        args.source.write_text(original)
        print("[source restored]")
    if best:
        print(f"BEST: {args.var}={best[1]} at {best[0]} exact")


if __name__ == "__main__":
    main()
