#!/usr/bin/env python3
"""IDO52_GENUINE_GP_SAVES -- restore the prologue `sw $gp, N($sp)` stores that
the static recompilation drops, so reads of UNINITIALISED stack slots see the
same values they see when the IRIX binaries themselves run.  On by default.

This is a recomp FIDELITY fix, not a codegen hook: with it, the recompiled
cfe/uopt/ugen/as1 compile the whole repository byte-identically to the same
IDO 5.2 binaries (tools/ido-static-recomp/ido/5.2) run under qemu-irix
(7,422 / 7,422 IDO translation units on 2026-11-11, 0 DIFF, no
@CompileEnv in the environment; tools/analysis/validate_ido52.py).  Without it 50 files
diverged, all in inline block moves (whole-struct copies).

THE BUG
-------
recomp.cpp elides every store whose source register is not "live" by its
own dataflow.  $gp is never live in its model (gp-relative addressing is
resolved statically), so every prologue save

    sw  gp, 192(sp)          ->   // fdead 400001eb MEM_U32(sp + 192) = gp;

is emitted as a comment.  On IRIX the slot really does receive gp (a
constant per executable: ugen 0x100172d0).  Any later function whose frame
overlaps that slot and READS a local before writing it therefore sees gp on
IRIX and some older value in the recomp.

ugen's f_eval_mov does exactly that.  On the path where the move's source
is already in a register (sp+272 != 0) the offset local at sp+232 is never
assigned, yet L428888 reads it and compares it with 0x7ff8.  On IRIX: the
slot holds 0x100172d0 (a gp save left by an earlier callee), the "offset
too large" branch is taken (L428918 -> L428934) and TWO scratch registers
are drawn from the pool, no `.set noat`.  Recomp: the slot holds a small
value, the short path (L4288c4) emits an extra `.set noat` and draws ONE --
so every register allocated after an inline struct copy is one pool slot
off.  That is the whole "scratch temporary off by one" divergence class.

The restored cfe/uopt/as1 saves are measured inert on the corpus; they are
included because the defect is in the translation, not in one pass.

ENV
---
(unset)                    perform the stores, as IRIX does.  The default.
IDO52_GENUINE_GP_SAVES=0   explicit opt-out: reproduce the legacy recomp
                           (elided stores, ugen's off-by-one scratch pool).
                           Kept only so the old behaviour stays reachable
                           for bisection; no registered file uses it.
IDO52_GENUINE_GP_SAVES=1   accepted and identical to unset.

    python3 tools/ido-static-recomp/apply_gp_save_hooks.py [--rebuild] [PASS ...]

Idempotent (probes for its v2 marker; a build/5.2/*.c carrying the v1
default-OFF helper of an earlier version of this applier is refused:
regenerate it); independent of every other
applier (the sites are recomp.cpp's own `// fdead` gp-save comments, which
no other applier anchors on).  Part of the post-`make clean` chain.
"""
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
BUILD = os.path.join(HERE, "build", "5.2")

# ri_gp_value from each IRIX 5.2 binary's .reginfo (constant for the run)
GP = {"cfe": 0x10022B80, "uopt": 0x1000FEF0, "ugen": 0x100172D0,
      "as1": 0x10029310}

MARKER = "ido52_genuine_gp_saves"          # v1 (default OFF) or v2: sites present
MARKER_V2 = "ido52_genuine_gp_saves_v2"    # helper with the default-ON semantics
RUN_ANCHOR = b"int run(uint8_t *mem, int argc, char *argv[]) {"
SITE_RE = re.compile(
    rb"^// fdead [0-9a-f]+ (MEM_U32\(sp \+ -?[0-9]+\)) = gp;$", re.M)

HELPER = b"""/* --- IDO52_GENUINE_GP_SAVES: recomp.cpp drops
 * every prologue `sw gp,N(sp)`; IRIX performs it, and code that reads an
 * uninitialised stack slot sees the saved gp.  See apply_gp_save_hooks.py.
 * Default OFF. --- */
#include <stdlib.h>
static int ido52_genuine_gp_saves_state = -1;
static int ido52_genuine_gp_saves(void) {
    if (ido52_genuine_gp_saves_state < 0) {
        const char *e = getenv("IDO52_GENUINE_GP_SAVES");
        ido52_genuine_gp_saves_state =
            (e != NULL && *e && !(e[0] == '0' && e[1] == 0));
    }
    return ido52_genuine_gp_saves_state;
}

"""

HELPER = b"""/* --- IDO52_GENUINE_GP_SAVES: recomp.cpp drops
 * every prologue `sw gp,N(sp)`; IRIX performs it, and code that reads an
 * uninitialised stack slot sees the saved gp.  See apply_gp_save_hooks.py.
 * Default ON (ido52_genuine_gp_saves_v2); IDO52_GENUINE_GP_SAVES=0 is the
 * explicit opt-out that reproduces the legacy recomp. --- */
#include <stdlib.h>
static int ido52_genuine_gp_saves_state = -1;
static int ido52_genuine_gp_saves(void) {
    if (ido52_genuine_gp_saves_state < 0) {
        const char *e = getenv("IDO52_GENUINE_GP_SAVES");
        ido52_genuine_gp_saves_state =
            !(e != NULL && e[0] == '0' && e[1] == 0);
    }
    return ido52_genuine_gp_saves_state;
}

"""


def rebuild_cmds(p):
    flags = ["-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I."]
    return [
        ["gcc", "-c"] + flags + ["-o", f"build/5.2/{p}.o", f"build/5.2/{p}.c"],
        ["gcc"] + flags + ["-o", f"build/5.2/out/{p}.new", f"build/5.2/{p}.o",
                           "build/5.2/version_info.o", "build/5.2/libc_impl_52.o",
                           "-lm"],
    ]


def apply(p):
    path = os.path.join(BUILD, f"{p}.c")
    s = open(path, "rb").read()
    if MARKER_V2.encode() in s:
        print(f"[gp-save hooks] already applied (v2, default ON) to {path}")
        return True
    if MARKER.encode() in s:
        print(f"[gp-save hooks] ERROR: {path} carries the v1 (default OFF) "
              f"helper; regenerate it (apply_all_hooks.py --regen)")
        return False
    if s.count(RUN_ANCHOR) != 1:
        print(f"[gp-save hooks] ERROR: run() anchor count {s.count(RUN_ANCHOR)} in {path}")
        return False
    gp = GP[p]
    s, n = SITE_RE.subn(
        lambda m: b"if (ido52_genuine_gp_saves()) " + m.group(1)
        + b" = 0x%x; /* sw gp (IDO52_GENUINE_GP_SAVES) */" % gp, s)
    if n == 0:
        print(f"[gp-save hooks] ERROR: no gp-save sites in {path}")
        return False
    s = s.replace(RUN_ANCHOR, HELPER + RUN_ANCHOR, 1)
    open(path, "wb").write(s)
    print(f"[gp-save hooks] {p}: {n} gp saves restored (gp=0x{gp:08x}, "
          f"default ON) in {path}")
    return True


def main():
    argv = sys.argv[1:]
    passes = [a for a in argv if a in GP] or list(GP)
    ok = all([apply(p) for p in passes])
    if not ok:
        sys.exit(1)
    if "--rebuild" in argv:
        for p in passes:
            for cmd in rebuild_cmds(p):
                r = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
                if r.returncode != 0:
                    print(r.stderr[-2000:])
                    sys.exit(1)
            os.replace(os.path.join(HERE, f"build/5.2/out/{p}.new"),
                       os.path.join(HERE, f"build/5.2/out/{p}"))
            print(f"[gp-save hooks] {p} rebuilt")


if __name__ == "__main__":
    main()
