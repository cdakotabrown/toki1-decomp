#!/usr/bin/env python3
"""Hook 55: IDO52_NO_JUMP_MERGE -- keep a jump target block where the source put it.

Idempotent (probes for its own marker); independent of the other uopt
appliers (its helper anchors on f_loopunroll's own definition).  Part of the
post-`make clean` chain (CLAUDE.md Known Gotchas / UOPT-HOOKS-REFERENCE).

    python3 tools/ido-static-recomp/apply_uopt_jumpmerge_hooks.py [build/5.2/uopt.c] [--rebuild]

THE TRANSFORM (uopt f_loopunroll, recomp labels L4775d8 / L477650 / L477c10)
--------------------------------------------------------------------------
Besides unrolling, f_loopunroll walks every basic block (graphnode list,
+12 = next) and STRAIGHTENS jumps: when block B ends in an unconditional
jump (last statement opcode 0x88, UJP) and the jump's target T
    - has exactly ONE predecessor (T+20 list has no second entry), and
    - ends in a statement whose opcode is in the bitset at 0x100013bc
      (a plain jump/fall-through tail -- a block ending in a CALL is not),
then T's statements are spliced into B in place of the jump (f_change_adj_node
re-links T's successors to B, f_post_loopblock / f_oneloopblockstmt /
f_codeimage re-emit them, the old T is dropped) and the test repeats on the
merged block (L477c10), so whole jump chains fold into one block.  The
merged code is then emitted where B is, i.e. T MOVES in the layout.

This is why a `goto dispatch; case_N: ...; goto done; ... dispatch: if (x
== N) goto case_N; ...` layout survives at -O1 (no uopt) and is straightened
at -O2: cfe lowers each `if (c) goto L` as a conditional jump OVER a UJP L,
every case_N has one predecessor, and each call-free case body is pulled up
into the compare chain (a body containing a call ends its block at the
call, so its tail opcode fails the bitset test and it stays put).  Retail
library code (PSY-Q-era IDO) lays such switches out bodies-first.
Runs even with -Wo,-loopunroll,0 (only IDO52_SKIP_PASSES=loopunroll, which
drops the whole pass, stopped it).

THE HOOK
--------
IDO52_NO_JUMP_MERGE=all | N[,N...] | occ:N[,N...] | bb:T[,T...]
    Veto the merge at candidate N (0-based, arrival order per uopt run,
    counted over candidates that passed every test above -- a BARE NUMBER
    IS AN INDEX, not `all`) or for target block number T (graphnode+8, the
    number IDO52_NO_JUMP_MERGE_TRACE prints).  The veto takes the path the
    pass takes for a target with two predecessors (L477ca4: advance to the
    next block), so the block, its UJP and its label stay exactly as the
    front end built them.  Sound by construction: it only declines an
    optional transform; no statement, edge or label is created.
IDO52_NO_JUMP_MERGE_TRACE=1
    `[JMERGE] #N from_bb=B to_bb=T tail_op=0x.. [-> VETOED]` per candidate.

Default OFF; with no env var set the helper returns before counting.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UOPT_C = os.path.join(HERE, "build", "5.2", "uopt.c")
MARKER = "ido52_jmerge_veto"

HELPERS = r"""/* --- IDO52_NO_JUMP_MERGE (Hook 55, 2026-09-22).  Veto f_loopunroll's
 * jump straightening (splicing a single-predecessor UJP target into the
 * jumping block); see apply_uopt_jumpmerge_hooks.py.  Default OFF. --- */
#define JMERGE_MAX 64
static int ido52_jmerge_mode;          /* 0 off, 1 all, 2 list */
static int ido52_jmerge_occ[JMERGE_MAX], ido52_jmerge_nocc;
static int ido52_jmerge_bb[JMERGE_MAX], ido52_jmerge_nbb;
static int ido52_jmerge_trace, ido52_jmerge_inited, ido52_jmerge_seq;
static void ido52_jmerge_init(void) {
    const char *env;
    char buf[512];
    char *tok;
    if (ido52_jmerge_inited) return;
    ido52_jmerge_inited = 1;
    env = getenv("IDO52_NO_JUMP_MERGE_TRACE");
    ido52_jmerge_trace = (env && *env) ? 1 : 0;
    env = getenv("IDO52_NO_JUMP_MERGE");
    if (!env || !*env) return;
    if (strcmp(env, "all") == 0) { ido52_jmerge_mode = 1; return; }
    ido52_jmerge_mode = 2;
    strncpy(buf, env, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    {
        int in_bb = 0;
        for (tok = strtok(buf, ","); tok; tok = strtok(NULL, ",")) {
            if (strncmp(tok, "occ:", 4) == 0) { in_bb = 0; tok += 4; }
            else if (strncmp(tok, "bb:", 3) == 0) { in_bb = 1; tok += 3; }
            if (in_bb) {
                if (ido52_jmerge_nbb < JMERGE_MAX)
                    ido52_jmerge_bb[ido52_jmerge_nbb++] = atoi(tok);
            } else {
                if (ido52_jmerge_nocc < JMERGE_MAX)
                    ido52_jmerge_occ[ido52_jmerge_nocc++] = atoi(tok);
            }
        }
    }
}
/* Called at L477650, after every legality test of the merge.  blk = the
 * block ending in UJP (s2); its first successor is the target. Returns 1 to
 * veto (take L477ca4, the not-applicable path). */
static int ido52_jmerge_veto(uint8_t *mem, uint32_t blk) {
    uint32_t tgt;
    int idx, i, hit = 0, tbb;
    ido52_jmerge_init();
    if (ido52_jmerge_mode == 0 && !ido52_jmerge_trace) return 0;
    tgt = MEM_U32(MEM_U32(blk + 24) + 0);
    tbb = (int)MEM_U16(tgt + 8);
    idx = ido52_jmerge_seq++;
    if (ido52_jmerge_mode == 1) hit = 1;
    for (i = 0; i < ido52_jmerge_nocc; i++) if (ido52_jmerge_occ[i] == idx) hit = 1;
    for (i = 0; i < ido52_jmerge_nbb; i++) if (ido52_jmerge_bb[i] == tbb) hit = 1;
    if (ido52_jmerge_trace)
        fprintf(stderr, "[JMERGE] #%d from_bb=%d to_bb=%d tail_op=0x%x%s\n",
                idx, (int)MEM_U16(blk + 8), tbb,
                MEM_U32(tgt + 32) ? MEM_U8(MEM_U32(tgt + 32)) : 0xff,
                hit ? " -> VETOED" : "");
    return hit;
}

"""

HELPER_ANCHOR = "static void f_loopunroll(uint8_t *mem, uint32_t sp) {\nconst"

SITE_ANCHOR = "L477650:\ns3 = MEM_U32(s2 + 28);\n"
SITE_CODE = ("L477650:\n"
             "/* Hook 55: IDO52_NO_JUMP_MERGE */\n"
             "if (ido52_jmerge_veto(mem, s2)) goto L477ca4;\n"
             "s3 = MEM_U32(s2 + 28);\n")

REBUILD = [
    ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/uopt.o", "build/5.2/uopt.c"],
    ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/out/uopt", "build/5.2/uopt.o", "build/5.2/version_info.o",
     "build/5.2/libc_impl_52.o", "-lm"],
]


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print(f"[jumpmerge hooks] Already applied to {path}")
        return True
    for needle in (HELPER_ANCHOR, SITE_ANCHOR):
        if s.count(needle) != 1:
            print(f"[jumpmerge hooks] ERROR: anchor not unique/found: "
                  f"{needle[:40]!r} (count {s.count(needle)})")
            return False
    s = s.replace(HELPER_ANCHOR, HELPERS + HELPER_ANCHOR, 1)
    s = s.replace(SITE_ANCHOR, SITE_CODE, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[jumpmerge hooks] Hook 55 applied to {path}")
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
        print("[jumpmerge hooks] uopt rebuilt")


if __name__ == "__main__":
    main()
