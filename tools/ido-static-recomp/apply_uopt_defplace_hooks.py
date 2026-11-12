#!/usr/bin/env python3
"""Apply ONLY the IDO52_CONST_DEF_EDGE hook to a (already hook-scripted)
build/5.2/uopt.c.  Mirrors what the apply_uopt_hooks.py addition in
hook_patch.py.txt does, so the regression build contains the new hook and
nothing else.

Usage: apply_def_edge.py <path-to-uopt.c>
"""
import sys

GLOBALS_ANCHOR = ('static struct { int bitpos; int nbb; int bbs[WEB_BIND_BB_MAX]; } '
                  'ido52_web_bind_entries[WEB_BIND_MAX];')

GLOBALS = '''
/* --- IDO52_CONST_DEF_EDGE (post-globalcolor region-entry def insertion).
 *     Format: BITPOS:BB[:EDGE][,BITPOS:BB[:EDGE]...]  EDGE = 'f' (default,
 *     fall-through edge, +236 list), 'j' (taken/jump edge, +240 list) or
 *     'e' (block entry, +228 list -- emitted as the block's FIRST insn).
 *     Appends a pending "load this web's ichain into this web's assigned
 *     color" record to the SAME per-block lists f_put_in_fallthru_bb /
 *     f_put_in_jump_bb populate for uopt's own PRE-style region-entry defs;
 *     reemit's pending-list emitter (func_425188 -> f_genrlodrstr) turns the
 *     record into real ucode.  Nothing is patched into the output. --- */
#define DEF_EDGE_MAX 16
static struct { int bitpos; int bb; int edge; } ido52_def_edge_entries[DEF_EDGE_MAX];
static int ido52_def_edge_count;

/* --- IDO52_CONST_DEF_BB (post-globalcolor range-head def control).
 *     Format: BITPOS:BB[:+|-][,...].  uopt emits a web's range-head def as a
 *     BLOCK-ENTRY RELOAD: for every color 1..35, if the block node's regdata
 *     slot (node+0x44+(color-1)*4) is non-zero AND the matching bit is set in
 *     the block's reload bitmap (node+208, MSB-first, bit 7-((color-1)&7)),
 *     the block-entry emitter (func_42b1ac's sibling loop) calls
 *     f_genrlodrstr(0x6d, color, ichain) -> f_genloadnum -> "li color,const".
 *     '-' clears that bit (SUPPRESS the natural def), '+' (default) sets the
 *     bit and writes the regdata slot (ADD a range-head def at that block's
 *     first instruction).  Both operate on uopt's own emitter; no output
 *     bytes are patched. --- */
#define DEF_BB_MAX 24
static struct { int bitpos; int bb; int add; } ido52_def_bb_entries[DEF_BB_MAX];
static int ido52_def_bb_count;

/* --- IDO52_RELOAD_SUPPRESS (range-head reload veto, occurrence-addressed).
 *     Format: BITPOS:N[,BITPOS:N...].  Skips the N-th (1-based) block-entry
 *     reload emission of the web at BITPOS.  Addresses the SAME emitter as
 *     IDO52_CONST_DEF_BB ':-' but at the emission point instead of the
 *     per-block bitmap, so it reaches blocks that are not in the
 *     LiveUnit-derived bb index (a block where no web is live has no
 *     LiveUnit and therefore no bb number to name).  Enumerate the
 *     occurrences with IDO52_RELOAD_EMIT_TRACE. --- */
#define RELOAD_SUP_MAX 16
static struct { int bitpos; int n; } ido52_reload_sup[RELOAD_SUP_MAX];
static int ido52_reload_sup_count;
static uint32_t ido52_reload_seen_ic[64];
static int ido52_reload_seen_n[64];
static int ido52_reload_seen_count;'''

# NOTE 2026-08-14: the anchor deliberately does NOT include the closing `}`
# of ido52_init_hooks -- newer apply_uopt_hooks.py versions append further
# parser blocks (e.g. IDO52_CMOT_KILL_AVAIL) after the web-bind block, so
# anchoring on end-of-function broke on any post-2026-08-13 chain.
PARSER_ANCHOR = '''        if (ido52_web_bind_count > 0) {
            fprintf(stderr, "[WEB-BIND-BBS] %d entries configured\\n", ido52_web_bind_count);
        }
    }'''

PARSER = '''        if (ido52_web_bind_count > 0) {
            fprintf(stderr, "[WEB-BIND-BBS] %d entries configured\\n", ido52_web_bind_count);
        }
    }

    /* IDO52_CONST_DEF_EDGE=BITPOS:BB[:EDGE][,...] -- post-gc edge def insertion.
       EDGE: 'f'/'0' = fall-through edge (default), 'j'/'1' = taken/jump edge,
       'e'/'2' = block entry (+228, emitted as the block's first insn). */
    env = getenv("IDO52_CONST_DEF_EDGE");
    if (env && *env) {
        char bufd[512];
        char *tokd;
        strncpy(bufd, env, sizeof(bufd) - 1);
        bufd[sizeof(bufd) - 1] = '\\0';
        tokd = strtok(bufd, ",");
        while (tokd && ido52_def_edge_count < DEF_EDGE_MAX) {
            char *p = strchr(tokd, ':');
            if (p) {
                int _de = ido52_def_edge_count;
                char *q;
                *p = '\\0';
                ido52_def_edge_entries[_de].bitpos = atoi(tokd);
                ido52_def_edge_entries[_de].edge = 0;
                q = strchr(p + 1, ':');
                if (q) {
                    *q = '\\0';
                    if (q[1] == 'j' || q[1] == 'J' || q[1] == '1')
                        ido52_def_edge_entries[_de].edge = 1;
                    else if (q[1] == 'e' || q[1] == 'E' || q[1] == '2')
                        ido52_def_edge_entries[_de].edge = 2;
                }
                ido52_def_edge_entries[_de].bb = atoi(p + 1);
                ido52_def_edge_count++;
            }
            tokd = strtok(NULL, ",");
        }
        if (ido52_def_edge_count > 0) {
            fprintf(stderr, "[CONST-DEF-EDGE] %d entries configured\\n", ido52_def_edge_count);
        }
    }

    /* IDO52_CONST_DEF_BB=BITPOS:BB[:+|-][,...] -- range-head (block-entry
       reload) def control.  '-' suppress, '+' (default) add. */
    env = getenv("IDO52_CONST_DEF_BB");
    if (env && *env) {
        char bufr[512];
        char *tokr;
        strncpy(bufr, env, sizeof(bufr) - 1);
        bufr[sizeof(bufr) - 1] = '\\0';
        tokr = strtok(bufr, ",");
        while (tokr && ido52_def_bb_count < DEF_BB_MAX) {
            char *p = strchr(tokr, ':');
            if (p) {
                int _db = ido52_def_bb_count;
                char *q;
                *p = '\\0';
                ido52_def_bb_entries[_db].bitpos = atoi(tokr);
                ido52_def_bb_entries[_db].add = 1;
                q = strchr(p + 1, ':');
                if (q) {
                    *q = '\\0';
                    if (q[1] == '-' || q[1] == '0') ido52_def_bb_entries[_db].add = 0;
                }
                ido52_def_bb_entries[_db].bb = atoi(p + 1);
                ido52_def_bb_count++;
            }
            tokr = strtok(NULL, ",");
        }
        if (ido52_def_bb_count > 0) {
            fprintf(stderr, "[CONST-DEF-BB] %d entries configured\\n", ido52_def_bb_count);
        }
    }

    /* IDO52_RELOAD_SUPPRESS=BITPOS:N[,...] -- veto the N-th range-head reload
       emission of the web at BITPOS (1-based, per web, emission order). */
    env = getenv("IDO52_RELOAD_SUPPRESS");
    if (env && *env) {
        char bufs[512];
        char *toks;
        strncpy(bufs, env, sizeof(bufs) - 1);
        bufs[sizeof(bufs) - 1] = '\\0';
        toks = strtok(bufs, ",");
        while (toks && ido52_reload_sup_count < RELOAD_SUP_MAX) {
            char *p = strchr(toks, ':');
            if (p) {
                *p = '\\0';
                ido52_reload_sup[ido52_reload_sup_count].bitpos = atoi(toks);
                ido52_reload_sup[ido52_reload_sup_count].n = atoi(p + 1);
                ido52_reload_sup_count++;
            }
            toks = strtok(NULL, ",");
        }
        if (ido52_reload_sup_count > 0) {
            fprintf(stderr, "[RELOAD-SUPPRESS] %d entries configured\\n",
                    ido52_reload_sup_count);
        }
    }'''

RUNTIME_ANCHOR = 'static void ido52_apply_web_bind_bbs(uint8_t *mem) {'

RUNTIME = '''/*
 * ido52_reload_should_skip: called from uopt's block-entry reload emitter
 * (the loop that walks colors 1..35 testing node+0x44 slot / node+208 bit and
 * calls f_genrlodrstr(0x6d, color, ichain)).  Counts, per ichain, how many
 * range-head reloads have been emitted and vetoes the ones named by
 * IDO52_RELOAD_SUPPRESS.  Returns 1 = skip this emission.
 * Always no-ops when IDO52_RELOAD_SUPPRESS is unset (the trace is env-gated
 * too), so the emitter is bit-identical to the unhooked build.
 */
static int ido52_reload_should_skip(uint8_t *mem, uint32_t color, uint32_t ichain,
                                    uint32_t bbnode) {
    int i, n = 0, skip = 0;
    if (ido52_reload_sup_count == 0 && getenv("IDO52_RELOAD_EMIT_TRACE") == NULL)
        return 0;
    for (i = 0; i < ido52_reload_seen_count; i++) {
        if (ido52_reload_seen_ic[i] == ichain) { n = ++ido52_reload_seen_n[i]; break; }
    }
    if (i == ido52_reload_seen_count && ido52_reload_seen_count < 64) {
        ido52_reload_seen_ic[ido52_reload_seen_count] = ichain;
        ido52_reload_seen_n[ido52_reload_seen_count] = 1;
        ido52_reload_seen_count++;
        n = 1;
    }
    if (ido52_reload_sup_count > 0) {
        uint32_t bittab_ptr = MEM_U32(0x1001cbd0);
        for (i = 0; i < ido52_reload_sup_count; i++) {
            uint32_t lr, ic0;
            if (ido52_reload_sup[i].n != n) continue;
            if (bittab_ptr == 0) continue;
            lr = MEM_U32(bittab_ptr + (uint32_t)(ido52_reload_sup[i].bitpos * 8) + 4);
            if (lr == 0) continue;
            ic0 = MEM_U32(lr + 0x00);
            if (ic0 == ichain) { skip = 1; break; }
        }
    }
    if (getenv("IDO52_RELOAD_EMIT_TRACE")) {
        fprintf(stderr, "[RLD-EMIT] n=%d bb=%u color=%u ichain=%08x node=%08x%s\\n",
                n, (unsigned)MEM_U16(bbnode + 8), color, ichain, bbnode,
                skip ? "  <-- SUPPRESSED" : "");
    }
    if (skip)
        fprintf(stderr, "[RELOAD-SUPPRESS] skipped reload #%d of ichain %08x"
                " (color %u, bb %u)\\n", n, ichain, color, (unsigned)MEM_U16(bbnode + 8));
    return skip;
}

/*
 * ido52_apply_const_def_edge: After globalcolor().  For each entry, append a
 * pending "load ichain into color" record to a basic block's fall-through
 * (+236) or taken-edge (+240) pending list -- the same lists uopt's own
 * f_put_in_fallthru_bb / f_put_in_jump_bb use for PRE-style region-entry
 * defs, consumed by reemit (func_425188 -> f_genrlodrstr).  The def is
 * produced by the compiler's own emitter; no output bytes are patched.
 * Basic-block nodes are located by walking every web's LiveUnits and
 * indexing nodes by block number (node+8, u16) -- same index the
 * WEB_BIND_BBS hook uses, so BB numbers mean the same thing in both.
 * No-ops when IDO52_CONST_DEF_EDGE is unset.
 */
static uint32_t f_alloc_new(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1);
static int ido52_mem_readable(uint8_t *mem, uint32_t addr, uint32_t len);
static void ido52_apply_const_def_edge(uint8_t *mem, uint32_t sp) {
    static uint32_t node_by_bb2[4096];
    uint32_t bittab_ptr, numwebs, w;
    int i, dbg;
    if (ido52_def_edge_count == 0 && ido52_def_bb_count == 0) return;
    dbg = getenv("IDO52_CONST_DEF_EDGE_DEBUG") != NULL;
    bittab_ptr = MEM_U32(0x1001cbd0);
    numwebs = MEM_U32(0x1001c8a4);
    if (bittab_ptr == 0) return;
    memset(node_by_bb2, 0, sizeof(node_by_bb2));
    /* Hook 24b (2026-09-25): 0x1001c8a4 is the PRE-colour web count.  Webs
       cut off by f_split during globalcolor (and by Hooks 34/39/60) get a
       bitpos from f_newbit, which grows 0x1001cad8 instead, so a scan bounded
       by 0x1001c8a4 never indexed the blocks only those webs are live in
       ("bb N not found", nothing in RELOAD_DUMP).  Scan to the larger count;
       every read is probed; the first node seen for a block wins. */
    {
        uint32_t nsplit = MEM_U32(0x1001cad8);
        if (nsplit > numwebs) numwebs = nsplit;
        if (numwebs > 100000u) numwebs = 100000u;
    }
    for (w = 0; w < numwebs; w++) {
        uint32_t lr, lu;
        int guard = 0;
        if (!ido52_mem_readable(mem, bittab_ptr + w * 8, 8)) { numwebs = w; break; }
        lr = MEM_U32(bittab_ptr + w * 8 + 4);
        if (lr == 0 || !ido52_mem_readable(mem, lr, 0x24)) continue;
        lu = MEM_U32(lr + 8);
        while (lu != 0 && guard++ < 65536 && ido52_mem_readable(mem, lu, 8)) {
            uint32_t nd = MEM_U32(lu + 0);
            if (nd != 0 && ido52_mem_readable(mem, nd, 0x100)) {
                uint32_t bb = MEM_U16(nd + 8);
                if (bb < 4096 && node_by_bb2[bb] == 0) node_by_bb2[bb] = nd;
            }
            lu = MEM_U32(lu + 4);
        }
    }
    if (getenv("IDO52_RELOAD_DUMP")) {
        int b, cc;
        for (b = 0; b < 4096; b++) {
            uint32_t n = node_by_bb2[b];
            if (!n) continue;
            for (cc = 1; cc <= 35; cc++) {
                uint32_t ic = MEM_U32(n + 0x44 + (cc - 1) * 4);
                uint32_t byte = MEM_U8(n + 208 + ((cc - 1) >> 3));
                int bit = (byte >> (7 - ((cc - 1) & 7))) & 1;
                if (ic && bit)
                    fprintf(stderr, "[RELOAD] bb=%d color=%d ichain=%08x\\n", b, cc, ic);
            }
        }
    }
    for (i = 0; i < ido52_def_bb_count; i++) {
        int bp = ido52_def_bb_entries[i].bitpos;
        int bb = ido52_def_bb_entries[i].bb;
        int add = ido52_def_bb_entries[i].add;
        uint32_t lr = MEM_U32(bittab_ptr + (uint32_t)(bp * 8) + 4);
        uint32_t nd, slot, boff;
        uint8_t rc, mask;
        if (lr == 0) {
            fprintf(stderr, "[CONST-DEF-BB] bitpos %d: no liverange\\n", bp);
            continue;
        }
        rc = MEM_U8(lr + 0x20);
        nd = (bb >= 0 && bb < 4096) ? node_by_bb2[bb] : 0;
        if (rc == 0 || rc > 35 || nd == 0) {
            fprintf(stderr, "[CONST-DEF-BB] bitpos %d: bb %d unusable (reg=%d nd=%08x)\\n",
                    bp, bb, rc, nd);
            continue;
        }
        slot = nd + 0x44 + (uint32_t)(rc - 1) * 4;
        boff = nd + 208 + (uint32_t)((rc - 1) >> 3);
        mask = (uint8_t)(1u << (7 - ((rc - 1) & 7)));
        if (add) {
            uint32_t ich = MEM_U32(lr + 0x00);
            if (MEM_U32(slot) != 0 && MEM_U32(slot) != ich
                && getenv("IDO52_CONST_DEF_BB_FORCE") == NULL) {
                fprintf(stderr, "[CONST-DEF-BB] bitpos %d: bb %d color %d slot OCCUPIED"
                        " (%08x), skip\\n", bp, bb, rc, MEM_U32(slot));
                continue;
            }
            MEM_U32(slot) = ich;
            MEM_U8(boff) = (uint8_t)(MEM_U8(boff) | mask);
            fprintf(stderr, "[CONST-DEF-BB] bitpos %d: range-head def ADDED at bb %d"
                    " entry (color %d, ichain %08x)\\n", bp, bb, rc, ich);
        } else {
            uint8_t before = MEM_U8(boff);
            MEM_U8(boff) = (uint8_t)(before & (uint8_t)~mask);
            fprintf(stderr, "[CONST-DEF-BB] bitpos %d: range-head def at bb %d entry"
                    " %s (color %d)\\n", bp, bb, (before & mask) ? "SUPPRESSED" : "was already absent",
                    rc);
        }
    }
    if (getenv("IDO52_CONST_DEF_EDGE_MAP")) {
        int b;
        fprintf(stderr, "[DEF-EDGE-MAP] indexed bbs:");
        for (b = 0; b < 4096; b++)
            if (node_by_bb2[b]) fprintf(stderr, " %d=%08x", b, node_by_bb2[b]);
        fprintf(stderr, "\\n");
    }
    for (i = 0; i < ido52_def_edge_count; i++) {
        int bp = ido52_def_edge_entries[i].bitpos;
        int bb = ido52_def_edge_entries[i].bb;
        int edge = ido52_def_edge_entries[i].edge;
        uint32_t lr = MEM_U32(bittab_ptr + (uint32_t)(bp * 8) + 4);
        uint32_t ichain, nd, rec, off;
        uint8_t rc;
        if (lr == 0) {
            fprintf(stderr, "[CONST-DEF-EDGE] bitpos %d: no liverange\\n", bp);
            continue;
        }
        rc = MEM_U8(lr + 0x20);
        ichain = MEM_U32(lr + 0x00);
        if (dbg) {
            uint32_t lu = MEM_U32(lr + 8);
            fprintf(stderr, "[CONST-DEF-EDGE] bitpos %d reg=%d ichain=%08x live bbs:",
                    bp, rc, ichain);
            while (lu != 0) {
                uint32_t n2 = MEM_U32(lu + 0);
                if (n2) fprintf(stderr, " %u", (unsigned)MEM_U16(n2 + 8));
                lu = MEM_U32(lu + 4);
            }
            fprintf(stderr, "\\n");
        }
        if (rc == 0 || rc > 35 || ichain == 0) {
            fprintf(stderr, "[CONST-DEF-EDGE] bitpos %d: unusable (reg=%d)\\n", bp, rc);
            continue;
        }
        nd = (bb >= 0 && bb < 4096) ? node_by_bb2[bb] : 0;
        if (nd == 0) {
            fprintf(stderr, "[CONST-DEF-EDGE] bitpos %d: bb %d not found\\n", bp, bb);
            continue;
        }
        /* Interference guard: refuse if ANOTHER web shares this color and is
           live in the target block -- defining the color there would clobber
           a value globalcolor proved non-interfering.  Override with
           IDO52_CONST_DEF_EDGE_FORCE=1 (unsafe; verify against the target). */
        if (getenv("IDO52_CONST_DEF_EDGE_FORCE") == NULL) {
            int clash = 0;
            for (w = 0; w < numwebs && !clash; w++) {
                uint32_t olr = MEM_U32(bittab_ptr + w * 8 + 4);
                uint32_t olu;
                int og = 0;
                if (olr == 0 || olr == lr || !ido52_mem_readable(mem, olr, 0x24)) continue;
                if (MEM_U8(olr + 0x20) != rc) continue;
                olu = MEM_U32(olr + 8);
                while (olu != 0 && og++ < 65536 && ido52_mem_readable(mem, olu, 8)) {
                    uint32_t n3 = MEM_U32(olu + 0);
                    if (n3 && (int)MEM_U16(n3 + 8) == bb) { clash = 1; break; }
                    olu = MEM_U32(olu + 4);
                }
            }
            if (clash) {
                fprintf(stderr, "[CONST-DEF-EDGE] bitpos %d: bb %d color %d "
                        "OCCUPIED by another live web, skip\\n", bp, bb, rc);
                continue;
            }
        }
        rec = f_alloc_new(mem, sp, 0xc, 0x100133f0);
        if (rec == 0) continue;
        off = (edge == 2) ? 228u : (edge ? 240u : 236u);
        MEM_U8(rec + 0) = (uint8_t)rc;
        MEM_U8(rec + 1) = (uint8_t)1;
        MEM_U32(rec + 4) = ichain;
        MEM_U32(rec + 8) = MEM_U32(nd + off);
        MEM_U32(nd + off) = rec;
        fprintf(stderr,
                "[CONST-DEF-EDGE] bitpos %d: def of ichain %08x into reg color %d"
                " on %s edge of bb %d\\n",
                bp, ichain, rc, (edge == 2) ? "entry" : (edge ? "jump" : "fallthru"), bb);
    }
}

static void ido52_apply_web_bind_bbs(uint8_t *mem) {'''

RELOAD_ANCHOR = '''a0 = 0x6d;
a1 = s0;
f_genrlodrstr(mem, sp, a0, a1, a2);
goto L42b30c;'''

RELOAD_PATCH = '''a0 = 0x6d;
a1 = s0;
if (!ido52_reload_should_skip(mem, s0, a2, s3))
f_genrlodrstr(mem, sp, a0, a1, a2);
goto L42b30c;'''

CALL_ANCHOR = 'ido52_apply_web_bind_bbs(mem);'
CALL = 'ido52_apply_web_bind_bbs(mem);\nido52_apply_const_def_edge(mem, sp);'


# ---------------------------------------------------------------------------
# Hook 25 (IDO52_RELOAD_SUPPRESS) pieces, derived from the blocks above so
# there is a single source of truth.  They can be grafted on their own onto a
# uopt.c that already carries Hook 24 (e.g. the shared tree's build).
# ---------------------------------------------------------------------------
RELOAD_GLOBALS_ANCHOR = 'static int ido52_def_bb_count;'
RELOAD_GLOBALS = GLOBALS[GLOBALS.index('\n/* --- IDO52_RELOAD_SUPPRESS'):]

_rp = PARSER.index('    /* IDO52_RELOAD_SUPPRESS=')
_rp0 = PARSER.index('        if (ido52_def_bb_count > 0) {')
RELOAD_PARSER_ANCHOR = PARSER[_rp0:_rp].rstrip('\n ') 
RELOAD_PARSER = RELOAD_PARSER_ANCHOR + '\n\n' + PARSER[_rp:].rstrip().rstrip('}').rstrip()

RELOAD_HELPER_ANCHOR = 'static void ido52_apply_const_def_edge('
RELOAD_HELPER = RUNTIME[:RUNTIME.index('/*\n * ido52_apply_const_def_edge:')]

RELOAD_ANCHOR = """a0 = 0x6d;
a1 = s0;
f_genrlodrstr(mem, sp, a0, a1, a2);
goto L42b30c;"""

RELOAD_PATCH = """a0 = 0x6d;
a1 = s0;
if (!ido52_reload_should_skip(mem, s0, a2, s3))
f_genrlodrstr(mem, sp, a0, a1, a2);
goto L42b30c;"""


# Hook 24b (2026-09-25): upgrade a uopt.c that already carries Hook 24 with the
# numwebs-bounded block index.  Text pairs are the C as emitted (unescaped).
HOOK24B_UPGRADES = [
    ('    memset(node_by_bb2, 0, sizeof(node_by_bb2));\n    for (w = 0; w < numwebs; w++) {\n        uint32_t lr = MEM_U32(bittab_ptr + w * 8 + 4);\n        uint32_t lu;\n        if (lr == 0) continue;\n        lu = MEM_U32(lr + 8);\n        while (lu != 0) {\n            uint32_t nd = MEM_U32(lu + 0);\n            if (nd != 0) {\n                uint32_t bb = MEM_U16(nd + 8);\n                if (bb < 4096) node_by_bb2[bb] = nd;\n            }\n            lu = MEM_U32(lu + 4);\n        }\n    }\n', '    memset(node_by_bb2, 0, sizeof(node_by_bb2));\n    /* Hook 24b (2026-09-25): 0x1001c8a4 is the PRE-colour web count.  Webs\n       cut off by f_split during globalcolor (and by Hooks 34/39/60) get a\n       bitpos from f_newbit, which grows 0x1001cad8 instead, so a scan bounded\n       by 0x1001c8a4 never indexed the blocks only those webs are live in\n       ("bb N not found", nothing in RELOAD_DUMP).  Scan to the larger count;\n       every read is probed; the first node seen for a block wins. */\n    {\n        uint32_t nsplit = MEM_U32(0x1001cad8);\n        if (nsplit > numwebs) numwebs = nsplit;\n        if (numwebs > 100000u) numwebs = 100000u;\n    }\n    for (w = 0; w < numwebs; w++) {\n        uint32_t lr, lu;\n        int guard = 0;\n        if (!ido52_mem_readable(mem, bittab_ptr + w * 8, 8)) { numwebs = w; break; }\n        lr = MEM_U32(bittab_ptr + w * 8 + 4);\n        if (lr == 0 || !ido52_mem_readable(mem, lr, 0x24)) continue;\n        lu = MEM_U32(lr + 8);\n        while (lu != 0 && guard++ < 65536 && ido52_mem_readable(mem, lu, 8)) {\n            uint32_t nd = MEM_U32(lu + 0);\n            if (nd != 0 && ido52_mem_readable(mem, nd, 0x100)) {\n                uint32_t bb = MEM_U16(nd + 8);\n                if (bb < 4096 && node_by_bb2[bb] == 0) node_by_bb2[bb] = nd;\n            }\n            lu = MEM_U32(lu + 4);\n        }\n    }\n'),
    ('                uint32_t olr = MEM_U32(bittab_ptr + w * 8 + 4);\n                uint32_t olu;\n                if (olr == 0 || olr == lr) continue;\n                if (MEM_U8(olr + 0x20) != rc) continue;\n                olu = MEM_U32(olr + 8);\n                while (olu != 0) {', '                uint32_t olr = MEM_U32(bittab_ptr + w * 8 + 4);\n                uint32_t olu;\n                int og = 0;\n                if (olr == 0 || olr == lr || !ido52_mem_readable(mem, olr, 0x24)) continue;\n                if (MEM_U8(olr + 0x20) != rc) continue;\n                olu = MEM_U32(olr + 8);\n                while (olu != 0 && og++ < 65536 && ido52_mem_readable(mem, olu, 8)) {'),
    ('static uint32_t f_alloc_new(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1);\nstatic void ido52_apply_const_def_edge(', 'static uint32_t f_alloc_new(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1);\nstatic int ido52_mem_readable(uint8_t *mem, uint32_t addr, uint32_t len);\nstatic void ido52_apply_const_def_edge('),
]

def _apply_reload_only(s):
    assert RELOAD_GLOBALS_ANCHOR in s, 'reload globals anchor missing'
    s = s.replace(RELOAD_GLOBALS_ANCHOR, RELOAD_GLOBALS_ANCHOR + RELOAD_GLOBALS, 1)
    assert RELOAD_PARSER_ANCHOR in s, 'reload parser anchor missing'
    s = s.replace(RELOAD_PARSER_ANCHOR, RELOAD_PARSER, 1)
    assert RELOAD_HELPER_ANCHOR in s, 'reload helper anchor missing'
    s = s.replace(RELOAD_HELPER_ANCHOR, RELOAD_HELPER + RELOAD_HELPER_ANCHOR, 1)
    return s


def main():
    p = sys.argv[1]
    s = open(p, errors='ignore').read()
    did = []
    if 'ido52_apply_const_def_edge' not in s:
        for a in (GLOBALS_ANCHOR, PARSER_ANCHOR, RUNTIME_ANCHOR, CALL_ANCHOR):
            assert a in s, 'anchor missing: %r' % a[:60]
        s = s.replace(GLOBALS_ANCHOR, GLOBALS_ANCHOR + GLOBALS, 1)
        s = s.replace(PARSER_ANCHOR, PARSER, 1)
        s = s.replace(RUNTIME_ANCHOR, RUNTIME, 1)
        idx = s.rindex(CALL_ANCHOR)
        s = s[:idx] + CALL + s[idx + len(CALL_ANCHOR):]
        did.append('Hook24 CONST_DEF_EDGE+CONST_DEF_BB')
    elif 'ido52_reload_should_skip' not in s:
        s = _apply_reload_only(s)
        did.append('Hook25 RELOAD_SUPPRESS (grafted onto existing Hook 24)')
    if 'ido52_reload_should_skip' in s and RELOAD_ANCHOR in s:
        assert s.count(RELOAD_ANCHOR) == 1, 'reload emitter anchor not unique'
        s = s.replace(RELOAD_ANCHOR, RELOAD_PATCH, 1)
        did.append('reload-emitter veto')
    for old, new in HOOK24B_UPGRADES:
        if old in s and new not in s:
            assert s.count(old) == 1, 'Hook 24b anchor not unique'
            s = s.replace(old, new, 1)
            if 'Hook24b' not in did:
                did.append('Hook24b')
    if not did:
        print('already applied'); return
    open(p, 'w').write(s)
    print('applied: ' + '; '.join(did))


if __name__ == '__main__':
    main()
