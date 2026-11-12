#!/usr/bin/env python3
"""Hook 74: IDO52_NO_FINDINDUCT_STRIDE_OCC -- per-OCCURRENCE (per-loop) veto of
uopt f_findinduct's stride creation (func_431cfc).  The occurrence-keyed
sibling of Hook 13 (`IDO52_NO_FINDINDUCT_STRIDE`, global: every candidate in
the TU).  Hook 13 is left exactly as it is.

Order-independent among the uopt appliers: its four anchors are
f_findinduct's own definition line, its entry label `L432e00:` /
`//findinduct:`, the candidate-confirmed test at its label `L433774:`, and
the raw stride-creation statement
`v0 = func_431cfc(mem, sp, v0, a0);` (the only call of func_431cfc in
uopt.c).  Hook 13 (apply_uopt_hooks.py) wraps that same statement in an
`if (ido52_no_findinduct_stride) { v0 = 0; } else { <stmt> }`; this applier
wraps the statement in its own if/else and KEEPS the raw statement line
intact, so either applier may run first: the result is two nested guards
and both find their anchor.  Must run after the uopt.c regen.  Idempotent:
probes for its own marker.  Part of the post-`make clean` chain (CLAUDE.md
Known Gotchas / UOPT-HOOKS-REFERENCE Hook 74).

    python3 tools/ido-static-recomp/apply_uopt_findinduct_occ_hooks.py [build/5.2/uopt.c] [--rebuild]

WHAT THE SITE IS
----------------
f_findinduct walks the procedure's basic-block list (global 0x1001c898,
node+12 = next); for each block `fp` that heads a loop it walks the loop's
induction-candidate bitvector (fp+320) one variable index `s5` at a time and,
for every set bit, calls func_431cfc(record of variable s5) which creates the
strength-reduction stride for that (loop, variable).  A zero return clears
the candidate bit, i.e. the variable is NOT strength-reduced in that loop
(exactly what Hook 13 forces for every candidate).  Retail's IRIX 5.2 uopt
strength-reduces SOME loops of a function and not others; Hook 13 is
all-or-nothing, so the loops that retail DID reduce break when it is used.

THE HOOK
--------
IDO52_NO_FINDINDUCT_STRIDE_OCC=<sel>[,<sel>...]
    all       every candidate (== Hook 13)
    N, occ:N  the N-th candidate (1-based) reaching the stride site, counted
              per uopt run (= per TU) in f_findinduct's call order
              (procedure order, then block-list order, then variable index)
    loop:N    every candidate of the N-th loop (1-based) reaching the site --
              a loop is a (procedure, header block) pair; the ordinal
              increments each time the pair changes at the site
    bb:N      every candidate whose loop-header block number is N, in EVERY
              procedure of the TU (block numbers restart per procedure)
IDO52_NO_FINDINDUCT_STRIDE_TRACE=1
    `[FINDINDUCT] occ=N loop=L proc=P bb=B var=V : KEEP|VETO` per candidate
    (`H13` is appended when the global Hook 13 is also set: then every
    candidate is vetoed by Hook 13 whatever this line says -- the trace and
    the numbering are evaluated at the candidate test, before either guard,
    so they are the same with or without Hook 13).  Each KEPT candidate is
    followed by `[FINDINDUCT]   occ=N -> STRIDE|none`: whether func_431cfc
    actually created a stride (only those are strength-reduction decisions;
    `none` candidates are not induction variables of that block and vetoing
    them changes nothing).  bb = MEM_U16(node+8), the same
    block number Hooks 20/24/59 use; var = uopt's variable index (the
    candidate bit).

Vetoing a candidate emits the loop as retail does when it did not
strength-reduce it: the index/address is recomputed per pass.  It never
creates or rewrites a word.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UOPT_C = os.path.join(HERE, "build", "5.2", "uopt.c")
MARKER = "ido52_h74_findinduct_occ"

HELPERS = r"""/* ---- Hook 74: IDO52_NO_FINDINDUCT_STRIDE_OCC (per-occurrence stride-creation veto) ---- */
static int ido52_h74_findinduct_occ_inited;
static int ido52_h74_all;
static int ido52_h74_trace;
static int ido52_h74_h13;          /* global Hook 13 also set (trace only) */
static int ido52_h74_n_occ, ido52_h74_n_loop, ido52_h74_n_bb;
static unsigned ido52_h74_occs[64], ido52_h74_loops[64], ido52_h74_bbs[64];
static unsigned ido52_h74_occ, ido52_h74_loop, ido52_h74_proc;
static unsigned ido52_h74_last_proc, ido52_h74_last_node;
static int ido52_h74_cur;          /* decision for the candidate in flight */
static void ido52_h74_init(void) {
    char *env;
    if (ido52_h74_findinduct_occ_inited) return;
    ido52_h74_findinduct_occ_inited = 1;
    env = getenv("IDO52_NO_FINDINDUCT_STRIDE_TRACE");
    if (env && *env && strcmp(env, "0") != 0) ido52_h74_trace = 1;
    env = getenv("IDO52_NO_FINDINDUCT_STRIDE");
    if (env && *env) ido52_h74_h13 = 1;
    env = getenv("IDO52_NO_FINDINDUCT_STRIDE_OCC");
    if (env && *env) {
        char buf[256];
        char *tok;
        strncpy(buf, env, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        tok = strtok(buf, ",");
        while (tok) {
            if (strcmp(tok, "all") == 0) {
                ido52_h74_all = 1;
            } else if (strncmp(tok, "loop:", 5) == 0) {
                if (atoi(tok + 5) > 0 && ido52_h74_n_loop < 64)
                    ido52_h74_loops[ido52_h74_n_loop++] = (unsigned)atoi(tok + 5);
            } else if (strncmp(tok, "bb:", 3) == 0) {
                if (atoi(tok + 3) > 0 && ido52_h74_n_bb < 64)
                    ido52_h74_bbs[ido52_h74_n_bb++] = (unsigned)atoi(tok + 3);
            } else {
                if (strncmp(tok, "occ:", 4) == 0) tok += 4;
                if (atoi(tok) > 0 && ido52_h74_n_occ < 64)
                    ido52_h74_occs[ido52_h74_n_occ++] = (unsigned)atoi(tok);
            }
            tok = strtok(NULL, ",");
        }
        fprintf(stderr, "[FINDINDUCT] IDO52_NO_FINDINDUCT_STRIDE_OCC: %s (%d occ, %d loop, %d bb)\n",
                ido52_h74_all ? "all" : "listed", ido52_h74_n_occ, ido52_h74_n_loop, ido52_h74_n_bb);
    }
}
/* returns 1 to veto stride creation for (loop header node, variable var) */
static int ido52_h74_veto(uint8_t *mem, uint32_t node, uint32_t var) {
    int i, veto = 0;
    unsigned bb;
    ido52_h74_init();
    ido52_h74_occ++;
    if (ido52_h74_proc != ido52_h74_last_proc || node != ido52_h74_last_node) {
        ido52_h74_loop++;
        ido52_h74_last_proc = ido52_h74_proc;
        ido52_h74_last_node = node;
    }
    bb = (unsigned)MEM_U16(node + 8);
    if (ido52_h74_all) veto = 1;
    for (i = 0; i < ido52_h74_n_occ; i++) if (ido52_h74_occs[i] == ido52_h74_occ) veto = 1;
    for (i = 0; i < ido52_h74_n_loop; i++) if (ido52_h74_loops[i] == ido52_h74_loop) veto = 1;
    for (i = 0; i < ido52_h74_n_bb; i++) if (ido52_h74_bbs[i] == bb) veto = 1;
    if (ido52_h74_trace)
        fprintf(stderr, "[FINDINDUCT] occ=%u loop=%u proc=%u bb=%u var=%u : %s%s\n",
                ido52_h74_occ, ido52_h74_loop, ido52_h74_proc, bb, (unsigned)var,
                veto ? "VETO" : "KEEP", ido52_h74_h13 ? " H13" : "");
    return veto;
}
"""

HELPER_ANCHOR = "static void f_findinduct(uint8_t *mem, uint32_t sp) {\n"

ENTRY_ANCHOR = "L432e00:\n//findinduct:\n"
ENTRY_CODE = ENTRY_ANCHOR + "ido52_h74_init(); ido52_h74_proc++;   /* Hook 74: procedure ordinal */\n"

# The raw statement stays on its own line so Hook 13's applier
# (apply_uopt_hooks.py, find_line + replace on this exact text) still finds
# it whichever applier runs first.
# The candidate is confirmed (its bit is set) once the `t3 == 0` test at
# L433774 falls through; the decision + trace live there so they run even
# when Hook 13's guard (which may be the OUTER one) skips the call itself.
CAND_ANCHOR = "L433774:\nif ((int32_t)t3 == (int32_t)0) {\n//nop;\ngoto L433834;}\n"
CAND_CODE = (CAND_ANCHOR +
             "/* Hook 74: IDO52_NO_FINDINDUCT_STRIDE_OCC -- fp = loop header node, s5 = variable */\n"
             "ido52_h74_cur = ido52_h74_veto(mem, fp, s5);\n")
SITE_ANCHOR = "v0 = func_431cfc(mem, sp, v0, a0);\n"
SITE_CODE = ("if (ido52_h74_cur) {   /* Hook 74 */\n"
             "    v0 = 0;\n"
             "} else {\n"
             "v0 = func_431cfc(mem, sp, v0, a0);\n"
             "if (ido52_h74_trace) fprintf(stderr, \"[FINDINDUCT]   occ=%u -> %s\\n\", ido52_h74_occ, v0 ? \"STRIDE\" : \"none\");\n"
             "}\n")

REBUILD = [
    ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/uopt.o", "build/5.2/uopt.c"],
    ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/out/uopt.new", "build/5.2/uopt.o", "build/5.2/version_info.o",
     "build/5.2/libc_impl_52.o", "-lm"],
]


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print(f"[findinduct-occ hooks] Already applied to {path}")
        return True
    for needle in (HELPER_ANCHOR, ENTRY_ANCHOR, CAND_ANCHOR, SITE_ANCHOR):
        if s.count(needle) != 1:
            print(f"[findinduct-occ hooks] ERROR: anchor not unique/found: "
                  f"{needle[:50]!r} (count {s.count(needle)})")
            return False
    s = s.replace(HELPER_ANCHOR, HELPERS + HELPER_ANCHOR, 1)
    s = s.replace(ENTRY_ANCHOR, ENTRY_CODE, 1)
    s = s.replace(CAND_ANCHOR, CAND_CODE, 1)
    s = s.replace(SITE_ANCHOR, SITE_CODE, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[findinduct-occ hooks] Hook 74 applied to {path}")
    return True


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    path = args[0] if args else UOPT_C
    if not apply(path):
        sys.exit(1)
    if "--rebuild" in sys.argv:
        for cmd in REBUILD:
            r = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
            if r.returncode != 0:
                print(r.stderr[-2000:])
                sys.exit(1)
        # install by copy-then-rename so a concurrent compile never sees a
        # half-written binary
        os.replace(os.path.join(HERE, "build/5.2/out/uopt.new"),
                   os.path.join(HERE, "build/5.2/out/uopt"))
        print("[findinduct-occ hooks] uopt rebuilt")


if __name__ == "__main__":
    main()
