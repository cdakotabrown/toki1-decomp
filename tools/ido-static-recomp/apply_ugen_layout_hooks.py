#!/usr/bin/env python3
"""Hook 56: IDO52_UGEN_NO_REGION_MOVE -- keep a jump target's code where the source put it.

Idempotent (probes for its own marker); independent of apply_ugen_hooks.py
(its helper anchors on func_435fb0's own definition).  Part of the
post-`make clean` chain (CLAUDE.md Known Gotchas / UOPT-HOOKS-REFERENCE).

    python3 tools/ido-static-recomp/apply_ugen_layout_hooks.py [build/5.2/ugen.c] [--rebuild]

THE TRANSFORM (ugen f_labelopt -> func_435fb0, recomp labels L43600c..L436134)
------------------------------------------------------------------------------
ugen's f_labelopt (run at -O1 and above, on uopt's output as well as on raw
cfe output) calls, per procedure, func_4351b8 / func_435908 (jump-chain and
jump-over-jump clean-ups), then func_435fb0, the REGION MOVE: it walks the
statement list region by region (func_435e94 = end of the region that starts
at a label); when a region ends in an unconditional jump (tree opcode 0x88,
UJP) whose target region passes func_435f44 / func_435e50 / func_435eec
(the target region is self-contained: it cannot be fallen into and it does
not fall out), it unlinks the TARGET region and splices it in right after
the jump, so the jump becomes a jump-to-next that the next func_4351b8 pass
deletes.  One move per call.

Effect on a hand-laid-out dispatch

    case_1: D = 60; goto test;          retail:  sw ; b test
    c1_0:   v = 5; ...; return;                  c1_0 body
    test:   if (x == 0) goto c1_0;               test: beqz -> c1_0
            v = 60; ...; return;                 default body

is `sw ; test ; default ; c1_0` -- the test is pulled up to the jump and the
body it skipped lands after the default.  Retail library code (PSY-Q-era
IDO) keeps source order.  Complements uopt Hook 55 (IDO52_NO_JUMP_MERGE),
which covers the uopt-side splice of a single-predecessor jump TARGET into
the jumping block; this one is the ugen-side move of a whole region.

THE HOOK
--------
IDO52_UGEN_NO_REGION_MOVE=all | N[,N...] | occ:N[,N...] | lab:L[,L...]
    Veto the move at candidate N (0-based, arrival order over the whole
    translation unit, counted over candidates that passed every test above
    -- a BARE NUMBER IS AN INDEX, not `all`) or for the jump whose target
    is ugen's INTERNAL label number L (tree node +36, as the trace prints
    it; it is not the `$N` of `cc -S` output, which is renumbered).  The
    veto takes func_435fb0's own "not movable" path (L436134: continue
    with the next region), so the region stays where the
    source put it and the UJP stays a real `b`.  Sound by construction:
    this is the layout ugen emits at -O0, and every later pass sees an
    ordinary jump to an ordinary label.  Because func_435fb0 moves at most
    one region per call, vetoing candidate N can expose a LATER region as
    the next candidate; re-enumerate with the trace after each change.
IDO52_UGEN_NO_REGION_MOVE_TRACE=1
    `[UGEN-RMOVE] #N label=L [-> VETOED]` per candidate.

Default OFF; with no env var set the helper returns before counting.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UGEN_C = os.path.join(HERE, "build", "5.2", "ugen.c")
MARKER = "ugen_rmove_veto"

HELPERS = r"""/* --- IDO52_UGEN_NO_REGION_MOVE (Hook 56, 2026-09-22).  Veto ugen
 * f_labelopt's region move (func_435fb0: splice a UJP target region in
 * right after the jump); see apply_ugen_layout_hooks.py.  Default OFF. --- */
#define RMOVE_MAX 64
static int ugen_rmove_mode;            /* 0 off, 1 all, 2 list */
static int ugen_rmove_occ[RMOVE_MAX], ugen_rmove_nocc;
static int ugen_rmove_lab[RMOVE_MAX], ugen_rmove_nlab;
static int ugen_rmove_trace, ugen_rmove_inited, ugen_rmove_seq;
static void ugen_rmove_init(void) {
    const char *env;
    char buf[512];
    char *tok;
    int in_lab = 0;
    if (ugen_rmove_inited) return;
    ugen_rmove_inited = 1;
    env = getenv("IDO52_UGEN_NO_REGION_MOVE_TRACE");
    ugen_rmove_trace = (env && *env) ? 1 : 0;
    env = getenv("IDO52_UGEN_NO_REGION_MOVE");
    if (!env || !*env) return;
    if (strcmp(env, "all") == 0) { ugen_rmove_mode = 1; return; }
    ugen_rmove_mode = 2;
    strncpy(buf, env, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    for (tok = strtok(buf, ","); tok; tok = strtok(NULL, ",")) {
        if (strncmp(tok, "occ:", 4) == 0) { in_lab = 0; tok += 4; }
        else if (strncmp(tok, "lab:", 4) == 0) { in_lab = 1; tok += 4; }
        if (in_lab) {
            if (ugen_rmove_nlab < RMOVE_MAX) ugen_rmove_lab[ugen_rmove_nlab++] = atoi(tok);
        } else {
            if (ugen_rmove_nocc < RMOVE_MAX) ugen_rmove_occ[ugen_rmove_nocc++] = atoi(tok);
        }
    }
}
/* ujp = the region's terminating UJP tree node (opcode +32 == 0x88); its
 * +36 is the target label number.  Returns 1 to veto. */
static int ugen_rmove_veto(uint8_t *mem, uint32_t ujp) {
    int idx, i, hit = 0, lab;
    ugen_rmove_init();
    if (ugen_rmove_mode == 0 && !ugen_rmove_trace) return 0;
    lab = (int)MEM_U32(ujp + 36);
    idx = ugen_rmove_seq++;
    if (ugen_rmove_mode == 1) hit = 1;
    for (i = 0; i < ugen_rmove_nocc; i++) if (ugen_rmove_occ[i] == idx) hit = 1;
    for (i = 0; i < ugen_rmove_nlab; i++) if (ugen_rmove_lab[i] == lab) hit = 1;
    if (ugen_rmove_trace)
        fprintf(stderr, "[UGEN-RMOVE] #%d label=%d%s\n", idx, lab,
                hit ? " -> VETOED" : "");
    return hit;
}

"""

HELPER_ANCHOR = "static void func_435fb0(uint8_t *mem, uint32_t sp, uint32_t a0) {\nconst"

# func_435fb0 after its last legality test (func_435e50 on the region
# before the target), immediately before the relinking.  s2 = next region,
# MEM_U32(s2 + 12) = its predecessor = the UJP.
SITE_ANCHOR = ("if ((int32_t)v0 == (int32_t)0) {\n//nop;\ngoto L436134;}\n//nop;\n"
               "v1 = MEM_U32(s0 + 12);\na0 = MEM_U32(s3 + 12);\n")
SITE_CODE = ("if ((int32_t)v0 == (int32_t)0) {\n//nop;\ngoto L436134;}\n//nop;\n"
             "/* Hook 56: IDO52_UGEN_NO_REGION_MOVE */\n"
             "if (ugen_rmove_veto(mem, MEM_U32(s2 + 12))) goto L436134;\n"
             "v1 = MEM_U32(s0 + 12);\na0 = MEM_U32(s3 + 12);\n")

REBUILD = [
    ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/ugen.o", "build/5.2/ugen.c"],
    ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/out/ugen", "build/5.2/ugen.o", "build/5.2/version_info.o",
     "build/5.2/libc_impl_52.o", "-lm"],
]


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print(f"[ugen layout hooks] Already applied to {path}")
        return True
    for needle in (HELPER_ANCHOR, SITE_ANCHOR):
        if s.count(needle) != 1:
            print(f"[ugen layout hooks] ERROR: anchor not unique/found: "
                  f"{needle[:40]!r} (count {s.count(needle)})")
            return False
    s = s.replace(HELPER_ANCHOR, HELPERS + HELPER_ANCHOR, 1)
    s = s.replace(SITE_ANCHOR, SITE_CODE, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[ugen layout hooks] Hook 56 applied to {path}")
    return True


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    path = args[0] if args else UGEN_C
    if not apply(path):
        sys.exit(1)
    if "--rebuild" in sys.argv:
        for cmd in REBUILD:
            r = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
            if r.returncode != 0:
                print(r.stderr[-2000:])
                sys.exit(1)
        print("[ugen layout hooks] ugen rebuilt")


if __name__ == "__main__":
    main()
