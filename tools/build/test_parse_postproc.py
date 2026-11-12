#!/usr/bin/env python3
"""Unit tests for the prose-immune directive parsing in parse_postproc.py.

Regression guard for the @CompileFlags/@CompileEnv/@Compiler/@PostProc parser
bug (2026-07-23): the old naive whole-file substring search grabbed the FIRST
occurrence of a directive keyword ANYWHERE in the header, including prose
comments, and fed garbage words to the compiler ("uget: input file not
initialized" on RPG_BAT/func_801328C4).

A directive now counts ONLY when its line OPENS with the keyword after nothing
but C/C++ comment markers and whitespace.

Run directly:  python3 tools/build/test_parse_postproc.py
"""
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "overlay"))

import parse_postproc as pp


# ── Fixtures ──────────────────────────────────────────────────────────

# 1. PROSE ONLY: every directive keyword appears, but only inside prose
#    sentences / bullet lists / quotes -- NONE must parse as a directive.
PROSE_ONLY = '''\
/* func_DEADBEEF -- FAKE overlay, Address: 0xDEADBEEF, Size: 16 bytes
 *
 * This body was rewritten (the @CompileFlags: -Wo,-loopunroll,0 directive
 * above) after disabling loop unrolling. Cracked by the
 * `@CompileEnv: IDO52_CONST_INREG_VALUES=99` hook. The correct fix would
 * window this with "@SymbolStart: 0x80136138" and "@SymbolSize: 744", and it
 * is not a @Compiler: gcc260 function.
 *   - @PostProc: swap_registers a0 a1 (that is for the TAIL function only)
 */
#include "types.h"
void func_DEADBEEF(void) {}
'''

# 2. REAL DIRECTIVES: well-formed directive lines that MUST parse.
REAL_DIRECTIVES = '''\
/*
 * @Compiler: gcc260
 * @SymbolStart: 0x80157274
 * @SymbolSize: 268
 * @CompileFlags: -Wo,-loopunroll,0 -Wo,-notail
 * @CompileEnv: IDO52_CONST_INREG_VALUES=127 IDO52_NO_LICM=1
 * @PostProc: patch_stack_complete --old 24 --new 40
 * @PostProc: scheduler_normalizer
 */
#include "types.h"
void func_REAL(void) {}
'''

# 3. MIXED: a REAL directive line plus a later PROSE mention of the SAME
#    keyword with a DIFFERENT (corrupt) value. Only the real one must win.
MIXED = '''\
/*
 * @CompileEnv: IDO52_CONST_INREG_VALUES=127
 * @CompileFlags: -Wo,-loopunroll,0
 *
 * Prose: cracked by `@CompileEnv: IDO52_CONST_INREG_VALUES=999` (do NOT
 * apply the @CompileFlags: -Wo,-junk-value mentioned here in prose).
 */
#include "types.h"
void func_MIXED(void) {}
'''

# 4. DOUBLE-MARKER: a malformed-but-genuine directive whose line carries an
#    extra comment marker ("/*  * @PostProc:"). This is NOT prose (only
#    markers/whitespace precede the keyword) and MUST still parse.
DOUBLE_MARKER = '''\
/*  * @PostProc: patch_stack_complete --old 264 --new 280
 */
/* @PostProc: scheduler_normalizer */
#include "types.h"
void func_DBL(void) {}
'''


def _write(tmp, name, text):
    p = Path(tmp) / name
    p.write_text(text)
    return p


def run():
    failures = []

    def check(cond, msg):
        if cond:
            print(f"  ok: {msg}")
        else:
            failures.append(msg)
            print(f"  FAIL: {msg}")

    with tempfile.TemporaryDirectory() as tmp:
        prose = _write(tmp, "prose.c", PROSE_ONLY)
        real = _write(tmp, "real.c", REAL_DIRECTIVES)
        mixed = _write(tmp, "mixed.c", MIXED)
        dbl = _write(tmp, "dbl.c", DOUBLE_MARKER)

        # 1. Prose-only: NOTHING parses.
        print("[prose-only fixture -> no directives]")
        check(pp.parse_compile_flags(prose) == [],
              "prose @CompileFlags ignored")
        check(pp.parse_compile_env(prose) == {},
              "prose @CompileEnv ignored")
        check(pp.parse_compiler(prose) == "ido52",
              "prose @Compiler ignored (defaults ido52)")
        check(pp.parse_symbol_start(prose) is None,
              "prose @SymbolStart ignored")
        check(pp.parse_symbol_size(prose) is None,
              "prose @SymbolSize ignored")
        check(pp.parse_postproc_header(prose) is None,
              "prose @PostProc ignored")

        # 2. Real directives: parse exactly.
        print("[real-directive fixture -> parsed]")
        check(pp.parse_compile_flags(real) ==
              ["-Wo,-loopunroll,0", "-Wo,-notail"],
              "real @CompileFlags parsed")
        check(pp.parse_compile_env(real) ==
              {"IDO52_CONST_INREG_VALUES": "127", "IDO52_NO_LICM": "1"},
              "real @CompileEnv parsed")
        check(pp.parse_compiler(real) == "gcc260",
              "real @Compiler parsed")
        check(pp.parse_symbol_start(real) == 0x80157274,
              "real @SymbolStart parsed")
        check(pp.parse_symbol_size(real) == 268,
              "real @SymbolSize parsed")
        steps = pp.parse_postproc_header(real) or []
        types = [s["type"] for s in steps]
        check(types == ["patch_frame", "scheduler_normalizer"],
              f"real @PostProc parsed (got {types})")

        # 3. Mixed: real wins, prose value NOT applied.
        print("[mixed fixture -> real directive wins, no prose corruption]")
        env = pp.parse_compile_env(mixed)
        check(env == {"IDO52_CONST_INREG_VALUES": "127"},
              f"mixed @CompileEnv keeps real 127, not prose 999 (got {env})")
        check(pp.parse_compile_flags(mixed) == ["-Wo,-loopunroll,0"],
              "mixed @CompileFlags keeps real, not prose junk")

        # 4. Double-marker malformed directive still parses.
        print("[double-marker fixture -> genuine directive parses]")
        steps = pp.parse_postproc_header(dbl) or []
        types = [s["type"] for s in steps]
        check("patch_frame" in types and "scheduler_normalizer" in types,
              f"double-marker @PostProc parsed (got {types})")

        # 5. Cross-parser agreement: overlay_postproc + build_overlay must
        #    reach the SAME prose-immunity via the shared helper.
        print("[cross-parser: overlay_postproc / build_overlay prose-immune]")
        try:
            import overlay_postproc as op
            check(op.parse_postproc(prose) == [],
                  "overlay_postproc.parse_postproc prose-immune")
            check(len(op.parse_postproc(real)) == 2,
                  "overlay_postproc.parse_postproc parses real")
        except Exception as e:  # noqa
            failures.append(f"overlay_postproc import/parse: {e!r}")
            print(f"  FAIL: overlay_postproc: {e!r}")
        try:
            import build_overlay as bo
            check(bo.parse_compile_flags(prose) == [],
                  "build_overlay.parse_compile_flags prose-immune")
            check(bo.parse_compile_flags(real) ==
                  ["-Wo,-loopunroll,0", "-Wo,-notail"],
                  "build_overlay.parse_compile_flags parses real")
            check(bo.parse_compile_env(mixed) ==
                  {"IDO52_CONST_INREG_VALUES": "127"},
                  "build_overlay.parse_compile_env mixed keeps real")
        except Exception as e:  # noqa
            failures.append(f"build_overlay import/parse: {e!r}")
            print(f"  FAIL: build_overlay: {e!r}")

    print()
    if failures:
        print(f"FAILED ({len(failures)}):")
        for f in failures:
            print("  -", f)
        return 1
    print("ALL PASSED")
    return 0


if __name__ == "__main__":
    sys.exit(run())
