#!/usr/bin/env python3
"""apply_uopt_splitpiece_hooks.py -- Hook 60: split-PIECE keys for Hook 39
(IDO52_FORCE_WEB_SPILL / IDO52_FORCE_WEB_NOSPILL = ROOT.PIECE).

Run on build/5.2/uopt.c AFTER apply_uopt_web_hooks.py (it patches Hook 39's
env parse and anchors on Hook 39's pre-globalcolor call):

    python3 apply_uopt_splitpiece_hooks.py [build/5.2/uopt.c] [--rebuild]

THE GAP.  Hook 39 marks lr+35 (the "worth colouring" byte) BEFORE
f_globalcolor runs.  f_globalcolor's phase-1 cost test (L46ef64) and its
neighbour re-split loop (L46f4fc) call f_split, which cuts the range being
coloured into a PIECE that KEEPS the original bitpos and a REMAINDER that
gets a brand-new bitpos from f_newbit (numwebs 0x1001cad8 grows).  f_split
also zeroes lr+35 on BOTH halves.  A remainder therefore does not exist when
Hook 39 parses/marks, and bare bitposes above the pre-colour numwebs print
`[WEB-SPILL] bitpos N out of range -- SKIPPED`.

THE HOOK.  f_split is renamed f_split_h60orig and wrapped.  After every
split the wrapper records the new bitpos under a stable key ROOT.PIECE:
ROOT = the pre-globalcolor bitpos the chain of splits started from, PIECE =
1, 2, ... in creation order for that root (a remainder of a remainder is
ROOT.2, etc.).  Keyed forces are then applied to the new range -- and
re-applied to the parent if the parent is itself a keyed piece, since
f_split just zeroed its lr+35:

    IDO52_FORCE_WEB_SPILL=R.P     lr+35 = 2 -> f_globalcolor's own
                                  uncolourable path spills it (phase 1
                                  L46ea34; phase 2 skips lr+35 != 1).
    IDO52_FORCE_WEB_NOSPILL=R.P   f_compute_save (so adjsave/lr+28 are the
                                  allocator's own), then lr+35 = 1 if it
                                  came out 2 -- the web enters the colour
                                  scan.  Pair with NOSPLIT on the piece's
                                  bitpos if it re-splits.
    IDO52_FORCE_WEB_NOSPILL=R.P:A as above, and once the piece has been cut
                                  by its OWN next split (the parent
                                  re-apply), set its adjsave (lr+48) to A so
                                  the phase-2 test `cost < adjsave*occ`
                                  accepts the cut piece.  Not applied at
                                  creation: there the piece is still the
                                  whole remainder, and a forced adjsave
                                  colours all of it.
    IDO52_FORCE_WEB_NOSPILL=R.P/A adjsave A at creation AND at every re-apply.

Also fixes Hook 37's NOSPLIT abort: at L46ef64 sp+220 (best colour, -1 =
none) is checked; vetoing the split of a web with no legal colour used to
commit colour -1 and abort uopt (`Internal: code: 5042`, signal 6).  The
veto is now declined with a `NOSPLIT DECLINED` line in that case only.
    IDO52_WEB_PIECE_TRACE=1       `[WEB-PIECE] split parent=B (key) ->
                                  bit=N key=R.P` for every split.

Hook 60b (2026-09-25), same applier (upgrades a live Hook-60 uopt.c in place):
    IDO52_WEB_PIECE_NOREAPPLY=R.P  after R.P's own cut, do NOT re-apply its
                                  NOSPILL to it as the parent -- the post-cut
                                  worth/adjsave stay natural (TACO: bit 171
                                  spills, 34.2 takes v1, as retail).
    IDO52_WEB_PIECE_RESCUE=R.P     when R.P's own cost-test cut leaves an EMPTY
                                  remainder (f_split L46d344 -> L46d3b0 marks
                                  BOTH halves lr+32=0xff and dequeues them),
                                  re-queue the piece: lr+32=0, queue bit set,
                                  neighbour lr+36 counts restored, and the
                                  remainder's (= parent's) interference list
                                  handed back to the piece (a superset: sound).
                                  Once per key per procedure.
    IDO52_WEB_PIECE_TRACE=1 also prints each phase-1 cost test (best colour,
    cost vs adjsave*occ, forbid mask, neighbour colours) and each cut's result.

Hook 60c (2026-09-25), same applier (upgrades a live 60b uopt.c in place):
    IDO52_FORCE_WEB_NOSPILL=R.0[:A]  key R.0 = the piece that KEEPS bitpos R
                                  after R's cut.  Applied only at the parent
                                  re-apply after each of R's own cuts (before
                                  any cut R is Hook 39's bare bitpos; bare
                                  `NOSPILL=R` is unchanged and still zeroed by
                                  the cut).  SPILL=R.0, RESCUE=R.0 and
                                  NOREAPPLY=R.0 are accepted the same way.
                                  With an R.0 force configured and R already
                                  cut, Hook 43's FORCE_WEB_RANGE_SPLIT=R
                                  re-force on the re-picked R.0 piece is
                                  DECLINED (it always emptied the remainder
                                  and spilled both halves).
    IDO52_FORCE_WEB_NOSPILL=R.P/A:B  adjsave A at creation, B after each own
                                  cut: `/-1:5` = split the new range at its
                                  first cost test (adjsave*occ < 0 <= cost),
                                  then colour the cut piece.

Bare (undotted) tokens keep Hook 39's exact old meaning; dotted tokens were
previously parsed by atoi() as the bare ROOT and are now excluded from the
bare list (no shipped @CompileEnv contains one).  Default OFF: with no
dotted token and no trace the wrapper is one false `if` after f_split.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UOPT_C = os.path.join(HERE, "build", "5.2", "uopt.c")
MARKER = "ido52_h60_after_split"
MARKER_B = "ido52_h60_costtrace"  # Hook 60b
MARKER_C = "ido52_h60c_piece0"  # Hook 60c

DEF_OLD = ("static void f_split(uint8_t *mem, uint32_t sp, uint32_t a0, "
           "uint32_t a1, uint32_t a2, uint32_t a3) {\n")
DEF_NEW = ("static void f_split_h60orig(uint8_t *mem, uint32_t sp, uint32_t a0, "
           "uint32_t a1, uint32_t a2, uint32_t a3) {\n")

HELPER = r'''/* === Hook 60: split-PIECE keys for Hook 39 (IDO52_FORCE_WEB_SPILL /
 * IDO52_FORCE_WEB_NOSPILL = ROOT.PIECE).  See apply_uopt_splitpiece_hooks.py.
 * f_split is wrapped; after each split the new (remainder) range is keyed
 * ROOT.PIECE and any keyed force is applied to it. */
#define IDO52_H60_MAXF 32
#define IDO52_H60_MAXP 4096
static int ido52_h60_inited, ido52_h60_active, ido52_h60_trace;
static struct { int root, piece, kind, has_adj; float adj, adj2; } ido52_h60_f[IDO52_H60_MAXF];
static int ido52_h60_nf;
static struct { int bit, root, piece; } ido52_h60_p[IDO52_H60_MAXP];
static int ido52_h60_np;
static uint32_t ido52_h60_base;
/* Hook 60b (2026-09-25): per-key modes. */
static struct { int root, piece; } ido52_h60_norea[IDO52_H60_MAXF], ido52_h60_resc[IDO52_H60_MAXF];
static int ido52_h60_nnorea, ido52_h60_nresc;
static int ido52_h60_rescued[IDO52_H60_MAXF];
static void f_split_h60orig(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3);

static void ido52_h60_parse(const char *name, int kind) {
    const char *e = getenv(name);
    char buf[512];
    char *p, *tok, *dot;
    if (!e || !*e || strlen(e) >= sizeof(buf)) return;
    strcpy(buf, e);
    p = buf;
    while ((tok = strtok(p, ",")) != NULL) {
        p = NULL;
        dot = strchr(tok, '.');
        if (!dot) continue;              /* bare: Hook 39's own list */
        if (ido52_h60_nf >= IDO52_H60_MAXF) break;
        ido52_h60_f[ido52_h60_nf].root = atoi(tok);
        ido52_h60_f[ido52_h60_nf].piece = atoi(dot + 1);
        ido52_h60_f[ido52_h60_nf].kind = kind;
        ido52_h60_f[ido52_h60_nf].has_adj = 0;
        if (kind == 1 && strchr(dot, '/') && strchr(dot, ':')) {
            /* Hook 60c: R.P/A:B = adjsave A at creation, B after each own cut */
            ido52_h60_f[ido52_h60_nf].has_adj = 3;
            ido52_h60_f[ido52_h60_nf].adj = (float)atof(strchr(dot, '/') + 1);
            ido52_h60_f[ido52_h60_nf].adj2 = (float)atof(strchr(dot, ':') + 1);
        } else if (kind == 1 && strchr(dot, ':')) {
            ido52_h60_f[ido52_h60_nf].has_adj = 1;
            ido52_h60_f[ido52_h60_nf].adj = (float)atof(strchr(dot, ':') + 1);
        } else if (kind == 1 && strchr(dot, '/')) {
            ido52_h60_f[ido52_h60_nf].has_adj = 2;
            ido52_h60_f[ido52_h60_nf].adj = (float)atof(strchr(dot, '/') + 1);
        }
        if (ido52_h60_f[ido52_h60_nf].piece < 0) {  /* Hook 60c: R.0 allowed */
            fprintf(stderr, "[WEB-PIECE] %s token %s: PIECE must be >= 0"
                            " -- IGNORED\n", name, tok);
            continue;
        }
        ido52_h60_nf++;
    }
}

/* Hook 60b: plain R.P key lists (IDO52_WEB_PIECE_NOREAPPLY / _RESCUE). */
static int ido52_h60_parse_keys(const char *name, void *arr_) {
    struct { int root, piece; } *arr = arr_;
    const char *e = getenv(name);
    char buf[512];
    char *p, *tok, *dot;
    int n = 0;
    if (!e || !*e || strlen(e) >= sizeof(buf)) return 0;
    strcpy(buf, e);
    p = buf;
    while ((tok = strtok(p, ",")) != NULL) {
        p = NULL;
        dot = strchr(tok, '.');
        if (!dot || atoi(dot + 1) < 0) {  /* Hook 60c: R.0 allowed */
            fprintf(stderr, "[WEB-PIECE] %s token %s: want ROOT.PIECE (PIECE >= 0)"
                            " -- IGNORED\n", name, tok);
            continue;
        }
        if (n >= IDO52_H60_MAXF) break;
        arr[n].root = atoi(tok);
        arr[n].piece = atoi(dot + 1);
        n++;
    }
    return n;
}

static int ido52_h60_inlist(void *arr_, int n, int root, int piece) {
    struct { int root, piece; } *arr = arr_;
    int i;
    for (i = 0; i < n; i++)
        if (arr[i].root == root && arr[i].piece == piece) return i;
    return -1;
}

/* per procedure, just before f_globalcolor */
static void ido52_h60_reset(uint8_t *mem) {
    const char *e;
    if (!ido52_h60_inited) {
        ido52_h60_inited = 1;
        e = getenv("IDO52_WEB_PIECE_TRACE");
        ido52_h60_trace = (e && *e) ? 1 : 0;
        ido52_h60_parse("IDO52_FORCE_WEB_SPILL", 2);
        ido52_h60_parse("IDO52_FORCE_WEB_NOSPILL", 1);
        ido52_h60_nnorea = ido52_h60_parse_keys("IDO52_WEB_PIECE_NOREAPPLY", ido52_h60_norea);
        ido52_h60_nresc = ido52_h60_parse_keys("IDO52_WEB_PIECE_RESCUE", ido52_h60_resc);
        ido52_h60_active = (ido52_h60_nf || ido52_h60_trace || ido52_h60_nnorea
                            || ido52_h60_nresc);
        if (ido52_h60_nf)
            fprintf(stderr, "[WEB-PIECE] %d ROOT.PIECE force(s) configured\n",
                    ido52_h60_nf);
    }
    if (!ido52_h60_active) return;
    ido52_h60_np = 0;
    memset(ido52_h60_rescued, 0, sizeof(ido52_h60_rescued));
    ido52_h60_base = MEM_U32(0x1001cad8);
    if (ido52_h60_trace)
        fprintf(stderr, "[WEB-PIECE] procedure: %u webs before colouring\n",
                ido52_h60_base);
}

static int ido52_h60_key(int bit, int *root, int *piece) {
    int i;
    if (bit >= 0 && (uint32_t)bit < ido52_h60_base) { *root = bit; *piece = 0; return 1; }
    for (i = 0; i < ido52_h60_np; i++)
        if (ido52_h60_p[i].bit == bit) {
            *root = ido52_h60_p[i].root; *piece = ido52_h60_p[i].piece; return 1;
        }
    return 0;
}

static void ido52_h60_apply(uint8_t *mem, uint32_t sp, uint32_t lr, int bit,
                            int root, int piece, const char *who) {
    int i;
    /* Hook 60c (ido52_h60c_piece0): key R.0 = the piece that KEEPS the
     * original bitpos R.  It exists only once R has been cut, so it is applied
     * only on the parent re-apply after each of R's own cuts (who="parent");
     * before any cut, R is Hook 39's bare bitpos and R.0 does nothing. */
    if (piece < 0) return;
    if (piece == 0 && who[0] != 'p') return;
    for (i = 0; i < ido52_h60_nf; i++) {
        if (ido52_h60_f[i].root != root || ido52_h60_f[i].piece != piece) continue;
        if (ido52_h60_f[i].kind == 2) {
            MEM_U8(lr + 35) = 2;
            fprintf(stderr, "[WEB-PIECE] %s bit=%d key=%d.%d lr=0x%08x"
                            " marked uncolourable (SPILL)\n", who, bit, root, piece, lr);
        } else {
            uint8_t before;
            if (MEM_U8(lr + 35) == 0) f_compute_save(mem, sp, lr);
            before = MEM_U8(lr + 35);
            if (before == 2) MEM_U8(lr + 35) = 1;
            if (ido52_h60_f[i].has_adj >= 2 || (ido52_h60_f[i].has_adj && who[0] == 'p')) {
                union { uint32_t u; float f; } nv;
                nv.f = (ido52_h60_f[i].has_adj == 3 && who[0] == 'p')
                       ? ido52_h60_f[i].adj2 : ido52_h60_f[i].adj;
                MEM_U32(lr + 48) = nv.u;
            }
            {
                union { uint32_t u; float f; } cv;
                cv.u = MEM_U32(lr + 48);
                fprintf(stderr, "[WEB-PIECE] %s bit=%d key=%d.%d lr=0x%08x"
                                " forced COLOURABLE (NOSPILL; natural worth=%u,"
                                " adjsave=%g%s)\n", who, bit, root, piece, lr, before,
                        (double)cv.f, (ido52_h60_f[i].has_adj >= 2 || (ido52_h60_f[i].has_adj && who[0] == 'p')) ? " FORCED" : "");
            }
        }
    }
}

static void ido52_h60_after_split(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1, uint32_t a3) {
    uint32_t olr, nlr;
    int ob, nb, root = -1, piece = 0, oroot, opiece, i, cnt;
    olr = MEM_U32(a0);
    nlr = MEM_U32(a1);
    if (olr == 0 || nlr == 0) return;
    nb = ido52_gc_bitpos_of_lr(mem, nlr);
    ob = ido52_gc_bitpos_of_lr(mem, olr);
    if (nb < 0) return;
    if (!ido52_h60_key(ob, &oroot, &opiece)) {
        if (ido52_h60_trace)
            fprintf(stderr, "[WEB-PIECE] split parent=%d has no key -- new bit=%d"
                            " left unkeyed\n", ob, nb);
        return;
    }
    root = oroot;
    cnt = 0;
    for (i = 0; i < ido52_h60_np; i++) if (ido52_h60_p[i].root == root) cnt++;
    piece = cnt + 1;
    if (ido52_h60_np < IDO52_H60_MAXP) {
        ido52_h60_p[ido52_h60_np].bit = nb;
        ido52_h60_p[ido52_h60_np].root = root;
        ido52_h60_p[ido52_h60_np].piece = piece;
        ido52_h60_np++;
    }
    if (ido52_h60_trace)
        fprintf(stderr, "[WEB-PIECE] split parent=%d (key %d.%d, lr=0x%08x) ->"
                        " bit=%d key=%d.%d lr=0x%08x (%s)\n",
                ob, oroot, opiece, olr, nb, root, piece, nlr,
                a3 ? "own cost test" : "neighbour re-split");
    if (ido52_h60_trace)
        fprintf(stderr, "[WEB-PIECE]   after cut: piece bit=%d lr+32=%d forbid=%08x,%08x;"
                        " remainder bit=%d lr+32=%d %s\n", ob, MEM_S8(olr + 32),
                MEM_U32(olr + 40), MEM_U32(olr + 44), nb, MEM_S8(nlr + 32),
                (MEM_S8(olr + 32) == -1 && MEM_S8(nlr + 32) == -1)
                    ? "EMPTY (split failed: both spilled)" : "has blocks");
    ido52_h60_apply(mem, sp, nlr, nb, root, piece, "new");
    /* Hook 60b RESCUE.  f_split's L46d344 test: when the cut piece has
     * absorbed every block (remainder has no livbb and an empty bvect) the
     * split is a failure and L46d3b0 marks BOTH halves spilled (lr+32 = 0xff),
     * resets both queue bits and decrements every neighbour's lr+36 over the
     * remainder's (= the parent's old) interference list.  For a listed key
     * cut by its OWN cost test, undo that for the PIECE only: lr+32 = 0, queue
     * bit set again, neighbour counts restored (their lists still name the
     * piece's lr, so the counts must match the lists or f_split's 0x13b1
     * check fires later).  The piece's forbidden set and interference list
     * were just recomputed precisely by f_split (f_updateforbidden /
     * f_findsharedintf over its real blocks), so it re-enters the pick loop
     * exactly as a piece from a successful split would.  Once per key per
     * procedure: a second failure takes the natural spill. */
    if (ido52_h60_nresc && a3 && opiece >= 0  /* Hook 60c: R.0 too */
        && MEM_S8(olr + 32) == -1 && MEM_S8(nlr + 32) == -1
        && MEM_U32(nlr + 8) == 0) {
        int ri = ido52_h60_inlist(ido52_h60_resc, ido52_h60_nresc, oroot, opiece);
        if (ri >= 0 && !ido52_h60_rescued[ri]) {
            uint32_t node;
            uint32_t cnt = 0;
            ido52_h60_rescued[ri] = 1;
            MEM_U8(olr + 32) = 0;
            f_setbit(mem, sp, 0x100223a8, (uint32_t)ob);
            for (node = MEM_U32(nlr + 56); node != 0; node = MEM_U32(node + 4))
                if (MEM_U32(node + 0) != 0) {
                    MEM_U32(MEM_U32(node + 0) + 36) += 1;
                    cnt++;
                }
            /* The failed path skipped the L46d618 partition loop, so the
             * piece's own interference list is EMPTY while the remainder
             * holds the parent's whole old list (and every neighbour's list
             * still names the piece's lr).  Hand that list back to the
             * piece: a SUPERSET of its real interferences, so colouring it
             * forbids its colour to every web that could overlap it (sound,
             * conservative).  lr+36 = the list's live-entry count, which
             * f_split's 0x13b1 consistency check expects. */
            MEM_U32(olr + 56) = MEM_U32(nlr + 56);
            MEM_U32(olr + 36) = cnt;
            MEM_U32(nlr + 56) = 0;
            MEM_U32(nlr + 36) = 0;
            fprintf(stderr, "[WEB-PIECE] RESCUE bit=%d key=%d.%d lr=0x%08x: own cut"
                            " left an empty remainder (bit=%d) -- piece re-queued"
                            " (forbid now %08x,%08x)\n", ob, oroot, opiece, olr, nb,
                    MEM_U32(olr + 40), MEM_U32(olr + 44));
        }
    }
    if (ido52_h60_nnorea
        && ido52_h60_inlist(ido52_h60_norea, ido52_h60_nnorea, oroot, opiece) >= 0) {
        fprintf(stderr, "[WEB-PIECE] parent bit=%d key=%d.%d NOREAPPLY: post-cut"
                        " worth left natural\n", ob, oroot, opiece);
        return;
    }
    ido52_h60_apply(mem, sp, olr, ob, oroot, opiece, "parent");
}

/* Hook 60c: Hook 43 (IDO52_FORCE_WEB_RANGE_SPLIT) is keyed by bitpos, and
 * the piece that keeps bitpos R after R's cut is re-picked under that same
 * bitpos -- so the force fires AGAIN, and that second cut empties the
 * remainder (the piece holds every block left) and f_split's failed path
 * spills both halves.  When an R.0 force is configured and R has already
 * been cut in this procedure, decline Hook 43's re-force: the R.0 piece goes
 * to the natural cost test with the worth/adjsave the R.0 key gave it. */
static int ido52_h60_rsplit_skip(uint8_t *mem, int bp) {
    int i, cut = 0;
    (void)mem;
    if (!ido52_h60_active || bp < 0 || (uint32_t)bp >= ido52_h60_base) return 0;
    for (i = 0; i < ido52_h60_np; i++)
        if (ido52_h60_p[i].root == bp) { cut = 1; break; }
    if (!cut) return 0;
    for (i = 0; i < ido52_h60_nf; i++)
        if (ido52_h60_f[i].root == bp && ido52_h60_f[i].piece == 0) {
            fprintf(stderr, "[WEB-PIECE] bit=%d key=%d.0: already cut -- Hook 43"
                            " re-force DECLINED (natural cost test)\n", bp, bp);
            return 1;
        }
    return 0;
}

/* Hook 60b trace: f_globalcolor phase-1 cost test at L46ef64 (split iff
 * adjsave*occ <= best colour's cost; best = -1 / 1e20 means no legal colour). */
static void ido52_h60_costtrace(uint8_t *mem, uint32_t lr, int best, float bestcost) {
    union { uint32_t u; float f; } adj;
    int occ = (int)MEM_U32(lr + 28);
    adj.u = MEM_U32(lr + 48);
    fprintf(stderr, "[WEB-PIECE] cost test bit=%d: best colour=%d cost=%g vs"
                    " adjsave*occ=%g*%d=%g -> %s  forbid=%08x,%08x\n",
            ido52_gc_bitpos_of_lr(mem, lr), best, (double)bestcost, (double)adj.f, occ,
            (double)(adj.f * (float)occ),
            (adj.f * (float)occ <= bestcost) ? "SPLIT" : "colour",
            MEM_U32(lr + 40), MEM_U32(lr + 44));
    {   /* who forbids what: the interference list (lr+56 = {lr, next}) */
        uint32_t node;
        fprintf(stderr, "[WEB-PIECE]   neighbours of bit=%d (colour):",
                ido52_gc_bitpos_of_lr(mem, lr));
        for (node = MEM_U32(lr + 56); node != 0; node = MEM_U32(node + 4))
            if (MEM_U32(node + 0) != 0)
                fprintf(stderr, " %d(%d)", ido52_gc_bitpos_of_lr(mem, MEM_U32(node + 0)),
                        MEM_S8(MEM_U32(node + 0) + 32));
        fprintf(stderr, "\n");
    }
}

static void f_split(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3) {
    f_split_h60orig(mem, sp, a0, a1, a2, a3);
    if (ido52_h60_active) ido52_h60_after_split(mem, sp, a0, a1, a3);
}
/* === end Hook 60 === */

'''

PARSE_S_OLD = "ido52_force_web_spill[ido52_force_web_spill_count++] = atoi(toks);"
PARSE_S_NEW = ("if (!strchr(toks, '.'))  /* Hook 60: dotted = ROOT.PIECE */\n"
               "            ido52_force_web_spill[ido52_force_web_spill_count++] = atoi(toks);")
PARSE_N_OLD = "ido52_force_web_nospill[ido52_force_web_nospill_count++] = atoi(tokns);"
PARSE_N_NEW = ("if (!strchr(tokns, '.'))  /* Hook 60: dotted = ROOT.PIECE */\n"
               "            ido52_force_web_nospill[ido52_force_web_nospill_count++] = atoi(tokns);")
PROTO_OLD = ("static void f_split(uint8_t *mem, uint32_t sp, uint32_t a0, "
             "uint32_t a1, uint32_t a2, uint32_t a3);\n")
PROTO_NEW = PROTO_OLD + "static void ido52_h60_reset(uint8_t *mem);  /* Hook 60 */\n"
CALL_OLD = "ido52_apply_force_web_spill(mem);\n"

# Hook 60b: phase-1 cost-test trace (trace-only; IDO52_WEB_PIECE_TRACE=1).
COST_OLD = ("    f_split(mem, sp, a0, a1, a2, a3);\n    goto L46efa4;\n}\n"
            "t2 = MEM_U32(s5 + 28);\n")
COST_NEW = ("    f_split(mem, sp, a0, a1, a2, a3);\n    goto L46efa4;\n}\n"
            "if (ido52_h60_trace) ido52_h60_costtrace(mem, s5, (int)MEM_U32(sp + 220), f20.f[0]);  /* Hook 60b */\n"
            "t2 = MEM_U32(s5 + 28);\n")
CALL_NEW = ("ido52_apply_force_web_spill(mem);\n"
            "ido52_h60_reset(mem);  /* Hook 60 */\n")

# Hook 37 NOSPLIT crash fix (2026-09-25).  At L46ef64 sp+220 is the best
# colour found by the scan, initialised to -1 at L46eb88.  When the web has NO
# legal colour the natural cost test always splits (f20 is still the 1e20
# sentinel); vetoing the split then committed colour -1 at L46f158 and uopt
# aborted (`Internal: code: 5042`, signal 6 -- TACO/func_8015ACDC with
# IDO52_FORCE_WEB_COLOR_NOSPLIT=34 under Hook 28).  The veto is now declined
# with a warning in that case; with a legal colour it is unchanged.
NOSPLIT_OLD = ("if (ido52_force_web_nosplit_count && ido52_gc_nosplit(mem, s5)) {\n"
               "    goto L46efe8;\n}\n")
NOSPLIT_NEW = ("if (ido52_force_web_nosplit_count && ido52_gc_nosplit(mem, s5)) {\n"
               "    if ((int32_t)MEM_U32(sp + 220) >= 0) goto L46efe8;  /* Hook 60 fix */\n"
               "    fprintf(stderr, \"[GC-COLOR] bitpos=%d NOSPLIT DECLINED: no legal colour\"\n"
               "            \" (split kept; FORCE_WEB_SPILL spills it whole)\\n\",\n"
               "            ido52_gc_bitpos_of_lr(mem, s5));\n"
               "}\n")

REBUILD = [
    ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-MMD", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/uopt.o", "build/5.2/uopt.c"],
    ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/out/uopt", "build/5.2/uopt.o", "build/5.2/version_info.o",
     "build/5.2/libc_impl_52.o", "-lm"],
]


# Hook 60c: Hook 43's re-force of an already-cut R with an R.0 key is declined.
RSPLIT_OLD = "        if (ido52_force_web_rsplit[i] == bp) {\n"
RSPLIT_NEW = ("        if (ido52_force_web_rsplit[i] == bp\n"
              "            && !ido52_h60_rsplit_skip(mem, bp)) {  /* Hook 60c */\n")
RSPLIT_DECL_OLD = "/* RANGE-SPLIT force: 1 = take the f_split arm for this web. */\n"
RSPLIT_DECL_NEW = ("static int ido52_h60_rsplit_skip(uint8_t *mem, int bp);  /* Hook 60c */\n"
                   + RSPLIT_DECL_OLD)


def apply_rsplit(s):
    if "ido52_h60_rsplit_skip(mem, bp)) {" in s:
        return s
    if s.count(RSPLIT_OLD) != 1 or s.count(RSPLIT_DECL_OLD) != 1:
        return None
    return s.replace(RSPLIT_OLD, RSPLIT_NEW, 1).replace(RSPLIT_DECL_OLD, RSPLIT_DECL_NEW, 1)


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s and MARKER_B in s and MARKER_C in s and "ido52_h60_rsplit_skip(mem, bp)) {" in s:
        print(f"[splitpiece hooks] Already applied to {path}")
        return True
    if MARKER in s and MARKER_B in s:
        # Hook 60b without 60c: swap the helper block in place.
        a = s.find("/* === Hook 60: split-PIECE keys")
        b = s.find("/* === end Hook 60 === */\n\n")
        if a < 0 or b < 0:
            print("[splitpiece hooks] ERROR: cannot upgrade Hook 60b -> 60c")
            return False
        s = s[:a] + HELPER + s[b + len("/* === end Hook 60 === */\n\n"):]
        s = apply_rsplit(s)
        if s is None:
            print("[splitpiece hooks] ERROR: Hook 43 rsplit anchor not found")
            return False
        open(path, "wb").write(s.encode("latin-1"))
        print(f"[splitpiece hooks] Hook 60b upgraded to 60c in {path}")
        return True
    if MARKER in s:
        # Hook 60 without 60b: swap the helper block, add the cost trace.
        a = s.find("/* === Hook 60: split-PIECE keys")
        b = s.find("/* === end Hook 60 === */\n\n")
        if a < 0 or b < 0 or s.count(COST_OLD) != 1:
            print("[splitpiece hooks] ERROR: cannot upgrade Hook 60 -> 60b")
            return False
        s = s[:a] + HELPER + s[b + len("/* === end Hook 60 === */\n\n"):]
        s = s.replace(COST_OLD, COST_NEW, 1)
        s = apply_rsplit(s)
        if s is None:
            print("[splitpiece hooks] ERROR: Hook 43 rsplit anchor not found")
            return False
        open(path, "wb").write(s.encode("latin-1"))
        print(f"[splitpiece hooks] Hook 60 upgraded to 60b+60c in {path}")
        return True
    for needle in (DEF_OLD, PROTO_OLD, PARSE_S_OLD, PARSE_N_OLD, CALL_OLD, NOSPLIT_OLD, COST_OLD):
        if s.count(needle) != 1:
            print(f"[splitpiece hooks] ERROR: anchor not unique/found: "
                  f"{needle[:50]!r} (count {s.count(needle)}) -- run "
                  f"apply_uopt_web_hooks.py first")
            return False
    s = s.replace(DEF_OLD, HELPER + DEF_NEW, 1)
    s = s.replace(PARSE_S_OLD, PARSE_S_NEW, 1)
    s = s.replace(PARSE_N_OLD, PARSE_N_NEW, 1)
    s = s.replace(CALL_OLD, CALL_NEW, 1)
    s = s.replace(PROTO_OLD, PROTO_NEW, 1)
    s = s.replace(NOSPLIT_OLD, NOSPLIT_NEW, 1)
    s = s.replace(COST_OLD, COST_NEW, 1)
    s = apply_rsplit(s)
    if s is None:
        print("[splitpiece hooks] ERROR: Hook 43 rsplit anchor not found")
        return False
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[splitpiece hooks] Hook 60 applied to {path}")
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
        print("[splitpiece hooks] uopt rebuilt")


if __name__ == "__main__":
    main()
