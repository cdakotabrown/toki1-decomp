#!/usr/bin/env python3
"""Hook 53: IDO52_SPLIT_ADMIT -- per-web / per-block f_split piece admission.

Run AFTER apply_uopt_hooks.py (needs ido52_gc_bitpos_of_lr and the Hook 43
block as an anchor).  Idempotent: probes for its own marker.  Part of the
post-`make clean` chain (CLAUDE.md Known Gotchas / UOPT-HOOKS-REFERENCE).

    python3 tools/ido-static-recomp/apply_uopt_splitadmit_hooks.py [build/5.2/uopt.c] [--rebuild]

THE DECISION (uopt f_split / f_addadjacents, recomp labels L46cd98 / L46b6ac)
--------------------------------------------------------------------------
f_split(lr, &rest, class, flag) turns `lr` into a PIECE grown by BFS over
SUCCESSOR edges (graphnode+24) from a start LiveUnit, and moves every block
it does not take into a fresh remainder range `rest` (new bitpos).  The
piece keeps the original bitpos.  f_addadjacents is the admission test for
each successor `adj` that is still in the remainder: it computes
    s6 = f_marksharedintf(rest.intf, adj)     interferences adj adds
    s4 = piece.colorsleft (lr+33) BEFORE, then f_updateforbidden /
    f_updatecolorsleft with adj's registers,
and admits (L46b858) or rejects (L46b988, restoring lr+33/+40/+44) by
    heuristic ON  (0x1001eab0 != 0, the default):
        s6 < s4  &&  2*colorsleft_after >= piece.numintf(lr+36) + s6
    heuristic OFF (0x1001eab0 == 0, uopt's own option clears it):
        colorsleft_after != 0
Both arms use the SAME bookkeeping (queue push, bitvector moves, LiveUnit
transfer, the +9 -> +8 shared-interference marks that f_split later turns
into the piece's and the remainder's interference lists), so the heuristic
is a pure quality knob over which connected block set becomes the piece.

THE HOOK
--------
IDO52_SPLIT_ADMIT=BITPOS:BB[,BITPOS:BB...]   (BB = bb number or `*`)
    When f_addadjacents is growing the piece whose bitpos is BITPOS (Hook
    28/37 key space; the bitpos of the lr being split) and the candidate
    block's number is BB, decide by the HEURISTIC-OFF rule instead:
    admit iff colorsleft_after != 0.  If the heuristic already admits,
    nothing changes; if colorsleft_after == 0 the natural reject stands
    (the piece would be uncolourable -- never forced).
    This is exactly the choice uopt makes for EVERY block when its
    heuristic is switched off, so it is sound by construction: the rest
    of f_split and f_globalcolor (interference rebuild, forbidden sets,
    the rlods/rstrs boundary store/reload placement) run unmodified on
    the resulting partition.
IDO52_SPLIT_REJECT=BITPOS:BB[,...]
    The mirror: keep a named block OUT of the piece (take the natural
    reject path).  Rejecting is always legal (the BFS may stop anywhere).
IDO52_SPLIT_TRACE=1
    [SPLIT] lines: start block, every admission test (with both rules'
    verdicts), every expansion skip (deadout / is_cup / ijp tail), and
    the final piece block list.

Default OFF; with no env var set every added line is an untaken branch.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UOPT_C = os.path.join(HERE, "build", "5.2", "uopt.c")
MARKER = "ido52_split_admit_init"

HELPERS = r"""/* --- IDO52_SPLIT_ADMIT / IDO52_SPLIT_REJECT / IDO52_SPLIT_TRACE (Hook 53,
 * 2026-09-22).  Per-web, per-block override of f_addadjacents' piece
 * admission heuristic; see apply_uopt_splitadmit_hooks.py.  Default OFF. --- */
#define SPLIT_ADMIT_MAX 64
static int ido52_split_admit_bp[SPLIT_ADMIT_MAX];
static int ido52_split_admit_bb[SPLIT_ADMIT_MAX];   /* -1 = any block */
static int ido52_split_admit_mode[SPLIT_ADMIT_MAX]; /* 1 admit, 0 reject */
static int ido52_split_admit_count;
static int ido52_split_trace;
static int ido52_split_inited;
static int ido52_gc_bitpos_of_lr(uint8_t *mem, uint32_t lr);
static void ido52_split_parse(const char *name, int mode) {
    char buf[512];
    char *tok;
    const char *env = getenv(name);
    if (!env || !*env) return;
    strncpy(buf, env, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    tok = strtok(buf, ",");
    while (tok && ido52_split_admit_count < SPLIT_ADMIT_MAX) {
        char *colon = strchr(tok, ':');
        if (colon) {
            *colon = '\0';
            ido52_split_admit_bp[ido52_split_admit_count] = atoi(tok);
            ido52_split_admit_bb[ido52_split_admit_count] =
                (colon[1] == '*') ? -1 : atoi(colon + 1);
            ido52_split_admit_mode[ido52_split_admit_count] = mode;
            ido52_split_admit_count++;
        } else {
            fprintf(stderr, "[SPLIT] WARNING: %s entry '%s' needs BITPOS:BB\n",
                    name, tok);
        }
        tok = strtok(NULL, ",");
    }
}
static void ido52_split_admit_init(void) {
    char *env;
    if (ido52_split_inited) return;
    ido52_split_inited = 1;
    ido52_split_parse("IDO52_SPLIT_ADMIT", 1);
    ido52_split_parse("IDO52_SPLIT_REJECT", 0);
    env = getenv("IDO52_SPLIT_TRACE");
    if (env && *env) ido52_split_trace = atoi(env) ? atoi(env) : 1;
    if (ido52_split_admit_count) {
        fprintf(stderr, "[SPLIT] %d admission override entries configured\n",
                ido52_split_admit_count);
    }
}
/* Called at f_addadjacents' decision point (L46b7ec).  rest = remainder lr
 * (s5), piece = lr being grown (s2), gn = candidate graphnode, s6 = shared
 * interference count, s4 = colorsleft before the candidate.
 * Returns 1 = admit (only when colorsleft after != 0), 0 = reject,
 * -1 = leave the natural decision alone. */
static int ido52_split_admit(uint8_t *mem, uint32_t rest, uint32_t piece,
                             uint32_t gn, int s6, int s4) {
    int bp, bb, i, cl, nintf, heur, h_on, h_off, want = -1;
    ido52_split_admit_init();
    if (ido52_split_admit_count == 0 && ido52_split_trace == 0) return -1;
    bp = ido52_gc_bitpos_of_lr(mem, piece);
    bb = (int)MEM_U16(gn + 8);
    for (i = 0; i < ido52_split_admit_count; i++) {
        if (ido52_split_admit_bp[i] == bp &&
            (ido52_split_admit_bb[i] < 0 || ido52_split_admit_bb[i] == bb)) {
            want = ido52_split_admit_mode[i];
        }
    }
    cl = (int)MEM_U8(piece + 33);
    nintf = (int)MEM_U32(piece + 36);
    heur = MEM_U8(0x1001eab0) != 0;
    h_off = cl != 0;
    h_on = (s6 < s4) && !(cl * 2 < nintf + s6);
    if (want == 1 && !h_off) want = -1;   /* never make a piece uncolourable */
    if (ido52_split_trace) {
        fprintf(stderr, "[SPLIT] admit? piece=%d rest=%d bb=%d shared=%d "
                "colorsleft %d->%d numintf=%d heurON=%d heurOFF=%d natural=%s%s\n",
                bp, ido52_gc_bitpos_of_lr(mem, rest), bb, s6, s4, cl, nintf,
                h_on, h_off, (heur ? h_on : h_off) ? "ADMIT" : "REJECT",
                want == 1 ? "  <== FORCE ADMIT" : want == 0 ? "  <== FORCE REJECT" : "");
    }
    return want;
}
static void ido52_split_trace_start(uint8_t *mem, uint32_t piece, uint32_t rest,
                                    uint32_t lu) {
    ido52_split_admit_init();
    if (!ido52_split_trace) return;
    fprintf(stderr, "[SPLIT] start piece=%d rest=%d bb=%d lu: firstisstr=%d "
            "needreglod=%d needregsave=%d deadout=%d\n",
            ido52_gc_bitpos_of_lr(mem, piece), ido52_gc_bitpos_of_lr(mem, rest),
            (int)MEM_U16(MEM_U32(lu + 0) + 8), MEM_U8(lu + 20), MEM_U8(lu + 21),
            MEM_U8(lu + 22), MEM_U8(lu + 23));
}
static void ido52_split_trace_expand(uint8_t *mem, uint32_t lu) {
    uint32_t q, gn;
    if (!ido52_split_trace) return;
    q = MEM_U32(0x100223b8);
    gn = MEM_U32(q + 4 * MEM_U32(0x100223bc));
    fprintf(stderr, "[SPLIT]   expand bb=%d deadout=%d tailop=0x%x\n",
            (int)MEM_U16(gn + 8), MEM_U8(lu + 23),
            MEM_U32(gn + 32) ? MEM_U8(MEM_U32(gn + 32)) : 0xff);
}
static void ido52_split_trace_done(uint8_t *mem, uint32_t piece) {
    uint32_t q, n, i;
    if (!ido52_split_trace) return;
    q = MEM_U32(0x100223b8);
    n = MEM_U32(0x100223c0);
    fprintf(stderr, "[SPLIT] piece=%d blocks:", ido52_gc_bitpos_of_lr(mem, piece));
    for (i = 0; i <= n && i < 4096; i++) {
        fprintf(stderr, " %d", (int)MEM_U16(MEM_U32(q + 4 * i) + 8));
    }
    fprintf(stderr, "\n");
}

"""

HELPER_ANCHOR = "/* --- IDO52_FORCE_WEB_RANGE_SPLIT (Hook 43, 2026-08-19) ---"

ADJ_ANCHOR = "L46b7ec:\n"
ADJ_CODE = """/* Hook 53: IDO52_SPLIT_ADMIT / IDO52_SPLIT_REJECT (piece admission) */
{
    int _h53 = ido52_split_admit(mem, s5, s2, MEM_U32(s1 + 0), (int)s6, (int)s4);
    if (_h53 == 1) goto L46b858;
    if (_h53 == 0) { t1 = MEM_U32(sp + 92); goto L46b988; }
}
"""

START_ANCHOR = "L46d0a8:\n"
START_CODE = "ido52_split_trace_start(mem, MEM_U32(s2 + 0), a3, s0);   /* Hook 53 trace */\n"

EXPAND_ANCHOR = "L46d230:\n"
EXPAND_CODE = "ido52_split_trace_expand(mem, v0);   /* Hook 53 trace */\n"

DONE_ANCHOR = "goto L46d208;}\n//nop;\nv0 = 0x10021ca4;"
DONE_CODE = ("goto L46d208;}\n//nop;\n"
             "ido52_split_trace_done(mem, MEM_U32(s2 + 0));   /* Hook 53 trace */\n"
             "v0 = 0x10021ca4;")

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
        print(f"[splitadmit hooks] Already applied to {path}")
        return True
    for needle in (HELPER_ANCHOR, ADJ_ANCHOR, START_ANCHOR, EXPAND_ANCHOR,
                   DONE_ANCHOR):
        if s.count(needle) != 1:
            print(f"[splitadmit hooks] ERROR: anchor not unique/found: "
                  f"{needle[:40]!r} (count {s.count(needle)})")
            return False
    s = s.replace(HELPER_ANCHOR, HELPERS + HELPER_ANCHOR, 1)
    s = s.replace(ADJ_ANCHOR, ADJ_ANCHOR + ADJ_CODE, 1)
    s = s.replace(START_ANCHOR, START_ANCHOR + START_CODE, 1)
    s = s.replace(EXPAND_ANCHOR, EXPAND_ANCHOR + EXPAND_CODE, 1)
    s = s.replace(DONE_ANCHOR, DONE_CODE, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[splitadmit hooks] Hook 53 applied to {path}")
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
        print("[splitadmit hooks] uopt rebuilt")


if __name__ == "__main__":
    main()
