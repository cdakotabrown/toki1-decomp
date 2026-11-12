#!/usr/bin/env python3
"""Hook 66: IDO52_UGEN_MOVE_LOOP_SHIFT -- gate the size threshold above which
ugen emits a block move (struct assignment / by-value struct argument) as a
LOOP instead of an unrolled lw/sw (lwl/lwr/swl/swr) sequence.

Idempotent (probes for its own marker); independent of the other ugen
appliers (its helper anchors on f_eval_mov's own definition, its site on
the recomp label L428bb8 inside it).  Part of the post-`make clean` chain
(CLAUDE.md Known Gotchas / UOPT-HOOKS-REFERENCE Hook 66).

    python3 tools/ido-static-recomp/apply_ugen_moveloop_hooks.py [--ugen PATH] [--rebuild]

THE DECISION (ugen f_eval_mov, recomp label L428bb8)
----------------------------------------------------
f_eval_mov picks the move unit s0 (4, or 8 when the doubleword path is
enabled -- never for our -mips1 compiles) and holds the byte size in s1.
At L428bb8:

    t5 = s0 << 3;  at = t5 < s1;  if (!at) goto L428df4;   /* unroll */

i.e. it LOOPS iff size > unit << 3 (32 bytes at unit 4).  The loop copies
3 units per iteration (a 12-byte body at unit 4: `lwl/lwr x3 ; bne`) and
the remainder falls to the unrolled tail at L428df4.  The hook replaces the
shift count 3.  The retail library-region compiler loops at size > 16
(shift 2); see the Hook 66 doc for the evidence.

ENV
---
IDO52_UGEN_MOVE_LOOP_SHIFT=<N>   N in 2..15 replaces the stock 3.  Values
    below 2 are REFUSED (stock 3 is used, a warning is printed once): at
    N=1 a 9..11-byte move would reach the loop, whose body always copies
    3 units, and over-copy.  N >= 2 guarantees size > 4*unit > 3*unit.
IDO52_UGEN_MOVE_LOOP_TRACE=1     `[UGEN-MOVELOOP] size=<bytes> unit=<u>
    thresh=<u<<N> -> LOOP|UNROLL` for every block move reaching the test.
Default OFF: with the env unset the shift is the stock 3.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UGEN_C = os.path.join(HERE, "build", "5.2", "ugen.c")

MARKER = "ugen_mloop_thresh"

HELPER_ANCHOR = ("static void f_eval_mov(uint8_t *mem, uint32_t sp, "
                 "uint32_t a0) {\nconst")

HELPERS = r"""/* --- IDO52_UGEN_MOVE_LOOP_SHIFT (Hook 66, 2026-09-25): the block-move
 * loop threshold in f_eval_mov (L428bb8) is `unit << 3`; this replaces the
 * shift count.  See apply_ugen_moveloop_hooks.py.  Default OFF. --- */
static int ugen_mloop_inited, ugen_mloop_shift = 3, ugen_mloop_trace;

static uint32_t ugen_mloop_thresh(uint32_t unit, uint32_t size) {
    uint32_t th;
    if (!ugen_mloop_inited) {
        const char *e = getenv("IDO52_UGEN_MOVE_LOOP_SHIFT");
        const char *t = getenv("IDO52_UGEN_MOVE_LOOP_TRACE");
        ugen_mloop_inited = 1;
        if (e != NULL && *e) {
            char *end;
            long n = strtol(e, &end, 10);
            if (*end == 0 && n >= 2 && n <= 15) {
                ugen_mloop_shift = (int)n;
            } else {
                fprintf(stderr, "[UGEN-MOVELOOP] IDO52_UGEN_MOVE_LOOP_SHIFT=%s "
                                "refused (need 2..15); using stock 3\n", e);
            }
        }
        ugen_mloop_trace = (t != NULL && *t && strcmp(t, "0") != 0);
    }
    th = unit << ugen_mloop_shift;
    if (ugen_mloop_trace)
        fprintf(stderr, "[UGEN-MOVELOOP] size=%u unit=%u thresh=%u -> %s\n",
                size, unit, th, th < size ? "LOOP" : "UNROLL");
    return th;
}

"""

SITE_OLD = "L428bb8:\nt5 = s0 << 3;\nat = t5 < s1;\n"
SITE_NEW = ("L428bb8:\n"
            "/* === IDO52_UGEN_MOVE_LOOP_SHIFT (Hook 66): s0 = unit, s1 = size */\n"
            "t5 = ugen_mloop_thresh(s0, s1);\n"
            "at = t5 < s1;\n")

REBUILD = [
    ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/ugen.o", "build/5.2/ugen.c"],
    ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/out/ugen.new", "build/5.2/ugen.o", "build/5.2/version_info.o",
     "build/5.2/libc_impl_52.o", "-lm"],
]


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print(f"[ugen moveloop hooks] Hook 66 already applied to {path}")
        return True
    for needle in (HELPER_ANCHOR, SITE_OLD):
        if s.count(needle) != 1:
            print(f"[ugen moveloop hooks] ERROR: anchor not unique/found: "
                  f"{needle[:48]!r} (count {s.count(needle)})")
            return False
    # the site must sit inside f_eval_mov
    start = s.index(HELPER_ANCHOR)
    nxt = s.find("\nstatic ", start + 1)
    end = nxt if nxt != -1 else len(s)
    if not (start < s.index(SITE_OLD) < end):
        print("[ugen moveloop hooks] ERROR: L428bb8 is not inside f_eval_mov")
        return False
    s = s.replace(SITE_OLD, SITE_NEW, 1)
    s = s.replace(HELPER_ANCHOR, HELPERS + HELPER_ANCHOR, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[ugen moveloop hooks] Hook 66 applied to {path}")
    return True


def main():
    argv = sys.argv[1:]
    path = UGEN_C
    if "--ugen" in argv:
        path = argv[argv.index("--ugen") + 1]
    if not apply(path):
        sys.exit(1)
    if "--rebuild" in argv:
        for cmd in REBUILD:
            r = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
            if r.returncode != 0:
                print(r.stderr[-2000:])
                sys.exit(1)
        os.replace(os.path.join(HERE, "build/5.2/out/ugen.new"),
                   os.path.join(HERE, "build/5.2/out/ugen"))
        print("[ugen moveloop hooks] ugen rebuilt")


if __name__ == "__main__":
    main()
