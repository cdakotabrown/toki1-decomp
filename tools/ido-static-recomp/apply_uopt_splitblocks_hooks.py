#!/usr/bin/env python3
"""Hook 63: IDO52_SPLIT_BLOCKS -- choose the exact block set of an f_split piece.

Run AFTER apply_uopt_splitadmit_hooks.py (Hook 53: it anchors on Hook 53's
helper block and its f_split trace calls) and AFTER
apply_uopt_splitpiece_hooks.py (Hook 60: it hooks ido52_h60_reset / the
ido52_h60_active expression so R.P keys resolve).  Idempotent: probes for
its own marker.  Part of the post-`make clean` chain (CLAUDE.md Known
Gotchas / UOPT-HOOKS-REFERENCE Hook 63).

    python3 tools/ido-static-recomp/apply_uopt_splitblocks_hooks.py [build/5.2/uopt.c] [--rebuild]

THE DECISION (uopt f_split, recomp L46cd98; f_addadjacents L46b6ac)
------------------------------------------------------------------
f_split(lr, &rest, class, flag) copies lr into a fresh remainder `rest`
(new bitpos, owning EVERY LiveUnit of lr), then picks a START LiveUnit
from rest's list (L46cf24: first LU with firstisstr (lu+20) whose block
has colour `class` free, or passed in a register (lu+19); else L46cfc0:
first LU with occurrences) and arrives at L46d0a8 with s0 = that LU.
L46d0a8 moves it into the piece (lr keeps its bitpos), and the BFS at
L46d208 grows the piece over SUCCESSOR edges, calling f_addadjacents for
every expanded block that is not dead-out / is_cup / ijp-tailed;
f_addadjacents' L46b7ec test admits (L46b858) or rejects (L46b988) each
successor still in rest.  Everything downstream -- the interference-list
rebuild from the +9 -> +8 marks, forbidden sets, colouring, and the
rlods/rstrs boundary reload/store placement -- runs on whatever partition
comes out.  So the piece's block set is decided at exactly two points:
the start LU and the per-candidate admission.  Both are free choices for
the allocator's correctness: Hook 53 already proved the admission side
(its ADMIT is uopt's own -noheurAB rule), and the start is any LU of the
range (the fallback scan L46cfc0 takes an arbitrary one).

THE HOOK
--------
IDO52_SPLIT_BLOCKS=KEY:BLOCKS[,KEY:BLOCKS...]
    KEY    = BITPOS | BITPOS#N | R.P | R.P#N
             BITPOS  the bitpos of the range being split (Hook 28/37 key
                     space; the piece KEEPS it, so B#2 is the second cut of
                     B, i.e. the re-pick of the piece that kept B)
             R.P     Hook 60's ROOT.PIECE key of a remainder (needs Hook 60
                     tracking; this hook switches it on when a dotted key
                     is configured)
             #N      only the N-th split of that key in the procedure
                     (1-based); without #N every split of the key
    BLOCKS = bb[+bb...], each `bb` a number or an inclusive `lo-hi` range
             (numbers = MEM_U16(graphnode+8), as Hook 53 / `-Wo,-l` print).
    For a matching split:
      * START = the first listed block (in list order) that has a
        LiveUnit in the range; if none does, the natural start stands.
      * ADMISSION: a candidate successor is admitted iff it is listed and
        the piece keeps a colour (colorsleft_after != 0, Hook 53's
        guard); every unlisted candidate is rejected (the natural reject
        path -- stopping the BFS anywhere is always legal).
    Blocks are still reached only through successor edges from admitted,
    non-dead-out blocks; a listed block the BFS never reaches is reported.
IDO52_SPLIT_BLOCKS_TRACE=1
    [SPLITBLK] lines for EVERY split: the candidate LiveUnit set of the
    range (bb, firstisstr, passedinreg, deadout, occurrences), the natural
    start and the chosen one, forced admits/rejects, and the final piece
    (with any listed block that was not reached).

Default OFF; with no env var set every added line is an untaken branch.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UOPT_C = os.path.join(HERE, "build", "5.2", "uopt.c")
MARKER = "ido52_splitblk_init"

HELPERS = r"""/* --- IDO52_SPLIT_BLOCKS / IDO52_SPLIT_BLOCKS_TRACE (Hook 63, 2026-09-25).
 * Exact block-set control of an f_split piece (start LiveUnit + admission);
 * see apply_uopt_splitblocks_hooks.py.  Default OFF. --- */
#define SPLITBLK_MAX 32
#define SPLITBLK_MAXB 64
static struct { int root, piece, occ, nb; int bb[SPLITBLK_MAXB]; } ido52_splitblk_e[SPLITBLK_MAX];
static int ido52_splitblk_n, ido52_splitblk_trace, ido52_splitblk_inited, ido52_splitblk_dotted;
static struct { int root, piece, cnt; } ido52_splitblk_seen[256];
static int ido52_splitblk_nseen;
static int ido52_splitblk_cur = -1;       /* entry in force for the split in progress */
static uint32_t ido52_splitblk_curlr;     /* the range (piece) being grown */
static int ido52_h60_key(int bit, int *root, int *piece);
static void ido52_splitblk_init(void) {
    char buf[1024], *tok, *next;
    const char *env;
    if (ido52_splitblk_inited) return;
    ido52_splitblk_inited = 1;
    env = getenv("IDO52_SPLIT_BLOCKS_TRACE");
    ido52_splitblk_trace = (env && *env) ? 1 : 0;
    env = getenv("IDO52_SPLIT_BLOCKS");
    if (!env || !*env) return;
    strncpy(buf, env, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    for (tok = buf; tok && *tok && ido52_splitblk_n < SPLITBLK_MAX; tok = next) {
        char *colon, *hash, *dot, *b, *bn;
        int k = ido52_splitblk_n;
        next = strchr(tok, ',');
        if (next) *next++ = '\0';
        colon = strchr(tok, ':');
        if (!colon) {
            fprintf(stderr, "[SPLITBLK] WARNING: entry '%s' needs KEY:BLOCKS\n", tok);
            continue;
        }
        *colon = '\0';
        hash = strchr(tok, '#');
        ido52_splitblk_e[k].occ = 0;
        if (hash) { *hash = '\0'; ido52_splitblk_e[k].occ = atoi(hash + 1); }
        dot = strchr(tok, '.');
        ido52_splitblk_e[k].root = atoi(tok);
        ido52_splitblk_e[k].piece = dot ? atoi(dot + 1) : 0;
        if (dot && ido52_splitblk_e[k].piece > 0) ido52_splitblk_dotted = 1;
        ido52_splitblk_e[k].nb = 0;
        for (b = colon + 1; b && *b; b = bn) {
            int lo, hi, x;
            char *dash;
            bn = strchr(b, '+');
            if (bn) *bn++ = '\0';
            lo = atoi(b);
            hi = lo;
            dash = strchr(b + 1, '-');
            if (dash) hi = atoi(dash + 1);
            for (x = lo; x <= hi && ido52_splitblk_e[k].nb < SPLITBLK_MAXB; x++)
                ido52_splitblk_e[k].bb[ido52_splitblk_e[k].nb++] = x;
        }
        ido52_splitblk_n++;
    }
    if (ido52_splitblk_n)
        fprintf(stderr, "[SPLITBLK] %d block-set entr%s configured\n",
                ido52_splitblk_n, ido52_splitblk_n == 1 ? "y" : "ies");
}
/* Hook 60 must track ROOT.PIECE keys for a dotted entry. */
static int ido52_splitblk_wants_h60(void) {
    ido52_splitblk_init();
    return ido52_splitblk_dotted;
}
/* Per procedure (called from ido52_h60_reset, which runs before every
 * f_globalcolor). */
static void ido52_splitblk_reset(void) {
    ido52_splitblk_nseen = 0;
    ido52_splitblk_cur = -1;
    ido52_splitblk_curlr = 0;
}
static int ido52_splitblk_inset(int k, int bb) {
    int i;
    for (i = 0; i < ido52_splitblk_e[k].nb; i++)
        if (ido52_splitblk_e[k].bb[i] == bb) return 1;
    return 0;
}
/* L46d0a8: natural start LU `nat` chosen for splitting `piece` (the lr that
 * keeps its bitpos) against remainder `rest`.  Returns the LU to use. */
static uint32_t ido52_splitblk_start(uint8_t *mem, uint32_t piece, uint32_t rest,
                                     uint32_t nat) {
    int bit, root, pc, i, k = -1, cnt = 0;
    uint32_t lu, pick = 0;
    ido52_splitblk_init();
    ido52_splitblk_cur = -1;
    if (!ido52_splitblk_n && !ido52_splitblk_trace) return nat;
    bit = ido52_gc_bitpos_of_lr(mem, piece);
    root = bit; pc = 0;
    if (ido52_splitblk_dotted) {
        int r2, p2;
        if (ido52_h60_key(bit, &r2, &p2) && p2 > 0) { root = r2; pc = p2; }
    }
    for (i = 0; i < ido52_splitblk_nseen; i++)
        if (ido52_splitblk_seen[i].root == root && ido52_splitblk_seen[i].piece == pc) break;
    if (i == ido52_splitblk_nseen && i < 256) {
        ido52_splitblk_seen[i].root = root;
        ido52_splitblk_seen[i].piece = pc;
        ido52_splitblk_seen[i].cnt = 0;
        ido52_splitblk_nseen++;
    }
    if (i < 256) cnt = ++ido52_splitblk_seen[i].cnt;
    for (i = 0; i < ido52_splitblk_n; i++) {
        if (ido52_splitblk_e[i].piece > 0) {
            if (ido52_splitblk_e[i].root != root || ido52_splitblk_e[i].piece != pc) continue;
        } else if (ido52_splitblk_e[i].root != bit) continue;
        if (ido52_splitblk_e[i].occ && ido52_splitblk_e[i].occ != cnt) continue;
        k = i;
    }
    if (ido52_splitblk_trace) {
        fprintf(stderr, "[SPLITBLK] split bit=%d", bit);
        if (pc > 0) fprintf(stderr, " key=%d.%d", root, pc);
        fprintf(stderr, " cut#%d rest=%d natural start bb=%d; candidates (bb:firstisstr/"
                "inreg/deadout/occ):", cnt, ido52_gc_bitpos_of_lr(mem, rest),
                (int)MEM_U16(MEM_U32(nat + 0) + 8));
        for (lu = MEM_U32(rest + 8); lu != 0; lu = MEM_U32(lu + 4))
            fprintf(stderr, " %d:%d/%d/%d/%d", (int)MEM_U16(MEM_U32(lu + 0) + 8),
                    MEM_U8(lu + 20), MEM_U8(lu + 19), MEM_U8(lu + 23),
                    (int)MEM_U16(lu + 16) + (int)MEM_U8(lu + 18));
        fprintf(stderr, "\n");
    }
    if (k < 0) return nat;
    for (i = 0; i < ido52_splitblk_e[k].nb && !pick; i++)
        for (lu = MEM_U32(rest + 8); lu != 0; lu = MEM_U32(lu + 4))
            if ((int)MEM_U16(MEM_U32(lu + 0) + 8) == ido52_splitblk_e[k].bb[i]) {
                pick = lu;
                break;
            }
    ido52_splitblk_cur = k;
    ido52_splitblk_curlr = piece;
    if (!pick) {
        fprintf(stderr, "[SPLITBLK] bit=%d cut#%d: no listed block has a LiveUnit in "
                "the range -- natural start bb=%d kept\n", bit, cnt,
                (int)MEM_U16(MEM_U32(nat + 0) + 8));
        return nat;
    }
    fprintf(stderr, "[SPLITBLK] bit=%d cut#%d: start bb=%d (natural %d), block set forced\n",
            bit, cnt, (int)MEM_U16(MEM_U32(pick + 0) + 8),
            (int)MEM_U16(MEM_U32(nat + 0) + 8));
    return pick;
}
/* L46b7ec: 1 = admit, 0 = reject, -1 = not ours (Hook 53 / natural). */
static int ido52_splitblk_admit(uint8_t *mem, uint32_t piece, uint32_t gn) {
    int k = ido52_splitblk_cur, bb, in;
    if (k < 0 || piece != ido52_splitblk_curlr) return -1;
    bb = (int)MEM_U16(gn + 8);
    in = ido52_splitblk_inset(k, bb);
    if (in && MEM_U8(piece + 33) == 0) {
        fprintf(stderr, "[SPLITBLK]   bb=%d listed but would leave the piece no colour"
                " -- REJECTED\n", bb);
        return 0;
    }
    if (ido52_splitblk_trace)
        fprintf(stderr, "[SPLITBLK]   bb=%d %s\n", bb, in ? "ADMIT (listed)" : "REJECT (unlisted)");
    return in;
}
/* After the BFS (just before f_split's remainder bookkeeping). */
static void ido52_splitblk_done(uint8_t *mem, uint32_t piece) {
    uint32_t q, n, i;
    int k = ido52_splitblk_cur, j;
    if (k < 0 && !ido52_splitblk_trace) return;
    q = MEM_U32(0x100223b8);
    n = MEM_U32(0x100223c0);
    fprintf(stderr, "[SPLITBLK] piece bit=%d blocks:", ido52_gc_bitpos_of_lr(mem, piece));
    for (i = 0; i <= n && i < 4096; i++)
        fprintf(stderr, " %d", (int)MEM_U16(MEM_U32(q + 4 * i) + 8));
    fprintf(stderr, " (colorsleft %d)\n", MEM_U8(piece + 33));
    if (k >= 0) {
        for (j = 0; j < ido52_splitblk_e[k].nb; j++) {
            int b = ido52_splitblk_e[k].bb[j], hit = 0;
            for (i = 0; i <= n && i < 4096; i++)
                if ((int)MEM_U16(MEM_U32(q + 4 * i) + 8) == b) hit = 1;
            if (!hit) fprintf(stderr, "[SPLITBLK]   listed bb=%d NOT in the piece\n", b);
        }
    }
    ido52_splitblk_cur = -1;
    ido52_splitblk_curlr = 0;
}

"""

HELPER_ANCHOR = "/* --- IDO52_SPLIT_ADMIT / IDO52_SPLIT_REJECT / IDO52_SPLIT_TRACE (Hook 53,"

ADJ_ANCHOR = "L46b7ec:\n/* Hook 53: IDO52_SPLIT_ADMIT / IDO52_SPLIT_REJECT (piece admission) */\n"
ADJ_CODE = """L46b7ec:
/* Hook 63: IDO52_SPLIT_BLOCKS (exact piece block set) */
{
    int _h63 = ido52_splitblk_admit(mem, s2, MEM_U32(s1 + 0));
    if (_h63 == 1) goto L46b858;
    if (_h63 == 0) { t1 = MEM_U32(sp + 92); goto L46b988; }
}
/* Hook 53: IDO52_SPLIT_ADMIT / IDO52_SPLIT_REJECT (piece admission) */
"""

START_ANCHOR = "L46d0a8:\nido52_split_trace_start("
START_CODE = ("L46d0a8:\n"
              "s0 = ido52_splitblk_start(mem, MEM_U32(s2 + 0), a3, s0);   /* Hook 63 */\n"
              "ido52_split_trace_start(")

DONE_ANCHOR = "ido52_split_trace_done(mem, MEM_U32(s2 + 0));   /* Hook 53 trace */\n"
DONE_CODE = (DONE_ANCHOR +
             "ido52_splitblk_done(mem, MEM_U32(s2 + 0));   /* Hook 63 */\n")

RESET_ANCHOR = "static void ido52_h60_reset(uint8_t *mem) {\n    const char *e;\n"
RESET_CODE = RESET_ANCHOR + "    ido52_splitblk_reset();   /* Hook 63 */\n"

ACTIVE_ANCHOR = ("        ido52_h60_active = (ido52_h60_nf || ido52_h60_trace || ido52_h60_nnorea\n"
                 "                            || ido52_h60_nresc);\n")
ACTIVE_CODE = ("        ido52_h60_active = (ido52_h60_nf || ido52_h60_trace || ido52_h60_nnorea\n"
               "                            || ido52_h60_nresc\n"
               "                            || ido52_splitblk_wants_h60());   /* Hook 63 */\n")

# ido52_h60_reset is defined after the helpers but its body now calls two
# Hook 63 helpers defined earlier -- fine.  The Hook 63 helpers call
# ido52_h60_key, forward-declared in HELPERS.

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
        print(f"[splitblocks hooks] Already applied to {path}")
        return True
    for needle in (HELPER_ANCHOR, ADJ_ANCHOR, START_ANCHOR, DONE_ANCHOR,
                   RESET_ANCHOR, ACTIVE_ANCHOR):
        if s.count(needle) != 1:
            print(f"[splitblocks hooks] ERROR: anchor not unique/found: "
                  f"{needle[:50]!r} (count {s.count(needle)}) -- apply Hooks 53 and 60 first")
            return False
    s = s.replace(HELPER_ANCHOR, HELPERS + HELPER_ANCHOR, 1)
    s = s.replace(ADJ_ANCHOR, ADJ_CODE, 1)
    s = s.replace(START_ANCHOR, START_CODE, 1)
    s = s.replace(DONE_ANCHOR, DONE_CODE, 1)
    s = s.replace(RESET_ANCHOR, RESET_CODE, 1)
    s = s.replace(ACTIVE_ANCHOR, ACTIVE_CODE, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[splitblocks hooks] Hook 63 applied to {path}")
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
        print("[splitblocks hooks] uopt rebuilt")


if __name__ == "__main__":
    main()
