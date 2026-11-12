#!/usr/bin/env python3
"""Apply IDO52 uopt hooks to the regenerated uopt.c file.

uopt.c is NOT in git -- it gets regenerated from the MIPS binary by the
ido-static-recomp Makefile. This script patches the regenerated uopt.c
to add environment-variable-gated hooks for controlling optimizer behavior.

Usage:
    python3 tools/ido-static-recomp/apply_uopt_hooks.py [path/to/uopt.c]

Hooks added:
    IDO52_SKIP_PASSES          -- Skip entire optimizer passes
    IDO52_CONST_NOINREG_VALUES -- Block constants from register promotion
    IDO52_LDA_NOINREG_CLASSES  -- Block address loads from register promotion
                                  (CLASS[:FROM[:TO]] occurrence windows)
    IDO52_LDA_NOINREG_DEBUG    -- Trace every f_ldainreg query of a listed class
    IDO52_NO_FINDINDUCT_STRIDE -- Skip stride creation in f_findinduct
    IDO52_CSE_MAX_REUSE        -- Limit CSE expression reuse count
    IDO52_NO_CSE               -- Disable CSE for operator expressions
    IDO52_NO_CSE_OPCODES       -- Per-opcode CSE disable
    IDO52_CSE_DEBUG            -- CSE debug tracing
    IDO52_RESERVE_SREGS        -- Reserve specific s-registers
    IDO52_MAX_SREGS            -- Cap s-register allocation count
    IDO52_NO_DEADSTORE_GLOBALS -- Protect global stores from dead store elimination
    IDO52_DEADSTORE_DEBUG      -- Dead store debug tracing
    IDO52_NO_LICM              -- Disable loop-invariant code motion
    IDO52_NO_STRENGTH_REDUCE   -- Disable strength reduction in f_eliminduct
    IDO52_STRENGTH_REDUCE_DEBUG -- Strength reduction debug tracing
    IDO52_NO_LOOP_TEST_REPLACEMENT -- Keep a loop's relational (`<`) exit test
                                  instead of rewriting it to `!=` (EQ_INEQ)
    IDO52_LOOP_TEST_REPL_DEBUG -- Trace every EQ_INEQ candidate (occ + BB)
    IDO52_PRECOLOR_LIVEIN      -- Pre-color variables to specific registers (hidden params)
    IDO52_PRECOLOR_WEB         -- Color-only web pin (no live-in marking; regional webs)
    IDO52_PRECOLOR_WEB_POSTONLY-- Restrict PRECOLOR_WEB to the post-globalcolor rebind
    IDO52_EXCLUDE_REGS         -- Exclude s-registers from globalcolor forbidden bitvector
    IDO52_NO_SREG_PROMOTE      -- Skip f_opt_saved_regs (no callee-save register promotion)
    IDO52_DUMP_WEBS            -- Print all web assignments after globalcolor
                                  (1 = normal, 2 = also dump raw bittab words)
    IDO52_UOPT_TRACE           -- Set the ORIGINAL uopt trace level (use with 'uopt -l file')
    IDO52_CONSTQ_TRACE         -- Log per-use const emission decisions (f_gen_outparcode)
    IDO52_WEB_BIND_BBS         -- Post-globalcolor: extend a web's per-block register
                                  binding into extra basic blocks (BITPOS:BB[:BB...])
    IDO52_CMOT_KILL_AVAIL      -- Break codemotion availability of expression IDX in
                                  block BB (IDX[:BB][,...]) so the block recomputes
    IDO52_CMOT_AVAIL_TRACE     -- Trace the per-block ppin/antlocs/avlocs state of
                                  every IDO52_CMOT_KILL_AVAIL entry

After patching, rebuild with:
    cd tools/ido-static-recomp && make -C build/5.2 uopt

See docs/reference/UOPT-HOOKS-REFERENCE.md for full documentation.
"""

import sys
import os

UOPT_C = os.path.join(os.path.dirname(__file__), "build", "5.2", "uopt.c")

# ============================================================================
# Feature-test macro -- inserted BEFORE #include "header.h"
# The hooks' debug-only memory probe (ido52_mem_readable) uses sigaction and
# sigsetjmp; -std=c11 hides both unless a POSIX feature-test macro is defined
# before the first #include.
# ============================================================================
POSIX_FEATURE_TEST = (
    "/* Injected by apply_uopt_hooks.py: the hooks' debug-only memory probe\n"
    " * needs sigaction/sigsetjmp, which -std=c11 hides.  Must precede every\n"
    " * #include. */\n"
    "#ifndef _POSIX_C_SOURCE\n"
    "#define _POSIX_C_SOURCE 200809L\n"
    "#endif"
)

# ============================================================================
# Hook header block -- inserted after #include "header.h"
# ============================================================================
HOOK_HEADER = r'''
/* ================================================================== */
/* IDO52 uopt hooks -- environment-variable-gated optimizer controls  */
/* Injected by apply_uopt_hooks.py                                    */
/* ================================================================== */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>

static int ido52_hooks_initialized;

/* --- IDO52_SKIP_PASSES --- */
#define IDO52_SKIP_TAIL_RECURSION  (1 << 0)
#define IDO52_SKIP_LOOPUNROLL      (1 << 1)
#define IDO52_SKIP_CODEMOTION      (1 << 2)
#define IDO52_SKIP_ELIMINDUCT      (1 << 3)
#define IDO52_SKIP_COPYPROPAGATE   (1 << 4)
#define IDO52_SKIP_OPT_SAVED_REGS  (1 << 5)
#define IDO52_SKIP_FINDINDUCT      (1 << 6)
static uint32_t ido52_skip_passes_mask;

/* --- IDO52_CONST_NOINREG_VALUES --- */
static uint32_t ido52_const_noinreg_values[64];
static int ido52_const_noinreg_from[64];
static int ido52_const_noinreg_to[64];
static int ido52_const_noinreg_seen[64];
static int ido52_const_noinreg_count;

/* --- IDO52_CONST_INREG_VALUES --- */
static uint32_t ido52_const_inreg_values[64];
static int ido52_const_inreg_types[64];
static int ido52_const_inreg_from[64];
static int ido52_const_inreg_to[64];
static int ido52_const_inreg_seen[64];
static int ido52_const_inreg_count;

/* --- IDO52_LDA_NOINREG_CLASSES ---
 *     CLASS[:FROM[:TO]] -- 1-based occurrence window over f_ldainreg queries
 *     of CLASS (mirrors the CONST_NOINREG window of Hook 2).  A plain CLASS
 *     keeps the historical "block every occurrence" behaviour. --- */
static uint32_t ido52_lda_noinreg_classes[64];
static int ido52_lda_noinreg_from[64];
static int ido52_lda_noinreg_to[64];
static int ido52_lda_noinreg_seen[64];
static int ido52_lda_noinreg_count;
static int ido52_lda_noinreg_debug;

/* --- IDO52_NO_FINDINDUCT_STRIDE --- */
static int ido52_no_findinduct_stride;

/* --- IDO52_CSE_MAX_REUSE --- */
static int ido52_cse_max_reuse;  /* 0 = disabled */
#define CSE_TRACK_SIZE 4096
static struct { uint32_t ichain_ptr; int count; } ido52_cse_track[CSE_TRACK_SIZE];

/* --- IDO52_NO_CSE --- */
#define IDO52_NO_CSE_OFF     0
#define IDO52_NO_CSE_OP      1
#define IDO52_NO_CSE_ALL     2
#define IDO52_NO_CSE_OPCODES 3
static int ido52_no_cse_mode;
static uint8_t ido52_no_cse_opcode_list[64];
static int ido52_no_cse_opcode_count;
static int ido52_cse_debug;

/* --- IDO52_RESERVE_SREGS / IDO52_MAX_SREGS --- */
static uint32_t ido52_reserve_sregs_mask_hi;

/* --- IDO52_NO_DEADSTORE_GLOBALS --- */
#define IDO52_MAX_DEADSTORE_ADDRS 64
static uint32_t ido52_no_deadstore_addrs[IDO52_MAX_DEADSTORE_ADDRS];
static int ido52_no_deadstore_count;
static int ido52_deadstore_debug;

/* --- IDO52_NO_LICM --- */
static int ido52_no_licm;  /* 0=off, 1=all, 2=per-address */
static uint32_t ido52_no_licm_addrs[64];
static int ido52_no_licm_addr_count;

/* --- IDO52_NO_STRENGTH_REDUCE --- */
static int ido52_no_strength_reduce;
static int ido52_strength_reduce_debug;

/* --- IDO52_NO_LOOP_TEST_REPLACEMENT (EQ_INEQ loop-test conversion) ---
 *     f_constarith rewrites a loop's relational exit test (`<`/`<=`, emitted
 *     as slti/sltu + bne) into an equality test (`!=`, a single bne) when it
 *     can prove the induction variable hits the limit exactly.  That is one
 *     instruction SHORTER, so it cannot be repaired by any count-neutral
 *     post-processing step.  This hook suppresses the conversion.
 *     Modes: "1"/"all" = every site; "bb:N[,N]" = only the listed basic
 *     blocks (the number printed by the `EQ_INEQ at BB:` trace line);
 *     "occ:N[,N]" = only the listed 0-based occurrences in this file. --- */
#define IDO52_LTR_OFF 0
#define IDO52_LTR_ALL 1
#define IDO52_LTR_BB  2
#define IDO52_LTR_OCC 3
static int ido52_ltr_mode;
static int ido52_ltr_list[64];
static int ido52_ltr_list_count;
static int ido52_ltr_seq;
static int ido52_ltr_debug;

/* Decide whether the EQ_INEQ conversion at basic block `bb` is suppressed.
 * ALWAYS advances the occurrence counter so "occ:" indices stay stable
 * regardless of which sites end up suppressed. */
static int ido52_ltr_suppress(uint32_t bb) {
    int occ = ido52_ltr_seq++;
    int i;
    int sup = 0;
    if (ido52_ltr_mode == IDO52_LTR_ALL) {
        sup = 1;
    } else if (ido52_ltr_mode == IDO52_LTR_BB) {
        for (i = 0; i < ido52_ltr_list_count; i++) {
            if (ido52_ltr_list[i] == (int)bb) { sup = 1; break; }
        }
    } else if (ido52_ltr_mode == IDO52_LTR_OCC) {
        for (i = 0; i < ido52_ltr_list_count; i++) {
            if (ido52_ltr_list[i] == occ) { sup = 1; break; }
        }
    }
    if (ido52_ltr_debug) {
        fprintf(stderr, "[NO-LOOP-TEST-REPL] occ=%d bb=%u -> %s\n",
                occ, (unsigned)bb, sup ? "SUPPRESSED" : "kept");
    }
    return sup;
}

/* --- IDO52_EXCLUDE_REGS --- */
static uint32_t ido52_exclude_regs_mask_lo;

/* --- IDO52_EXCLUDE_REGS_WEB (Hook 28, 2026-08-02) ---
 * Per-web forbidden-color additions, applied in f_globalcolor's per-web
 * loop (L46e804) BEFORE f_updatecolorsleft/f_isconstrained, so the
 * coloring decision itself sees the extra forbids (unlike PRECOLOR_WEB
 * whose effective path is post-gc rebind).  Entry: BITPOS:reg[+reg...].
 * Colors use the PRECOLOR rc space (v0=1..s8=25) and the forbidden
 * bitvector at lr+40 is indexed BY THAT SAME rc VALUE, in MSB-first
 * (inverted) bit order: 1u << (~rc & 31).  f_updatecolorsleft reads it as
 *   word = lr + 40 + 4*(color>>5); taken if (word << (color & 31)) < 0
 * i.e. bit (31 - color).
 * ⚠️ 2026-08-09 FIX: this used to subtract 3 ("dump space", rc-3) because
 * the printregs `forbidden:` dump prints in rc-3 space.  The BITVECTOR
 * does not.  The result was an OFF-BY-THREE: `t3` (rc 10) forbade rc 7 =
 * $t0.  Proven by sweep (see tools/ido-static-recomp/test_hook28_regspec.py).
 * Any @CompileEnv written before this date used +3-shifted names. */
#define EXCLREGS_WEB_MAX 16
static struct { int bitpos; uint32_t mask; } ido52_exclregs_web[EXCLREGS_WEB_MAX];
static int ido52_exclregs_web_count;
static int ido52_exclregs_web_debug;

/* --- IDO52_FORCE_WEB_COLOR (Hook 37, 2026-08-13) ---------------------------
 * POSITIVE per-web colour assignment.  Hook 28 (IDO52_EXCLUDE_REGS_WEB) can
 * only forbid colours; this names the colour f_globalcolor must land on.
 *
 * f_globalcolor picks a colour in two places, both of which end in
 *   MEM_U8(s5 + 32) = <chosen>          (liverange.assigned_reg)
 * with the chosen colour staged in the frame slot MEM_U32(sp + 220):
 *   phase 1 (label L46f158)  - the cost-driven pass: repeatedly picks the
 *       highest-priority uncoloured web (its bittab index sits in sp+268),
 *       scans the two class colour ranges skipping forbidden colours, keeps
 *       the min-cost colour in sp+220, then either f_split()s the range
 *       (when spillcost <= bestcolourcost, label L46ef64) or assigns.
 *   phase 2 (label L46fbc0)  - the leftover pass over every web still in the
 *       bvect at 0x100223a0 (bittab index in sp+272); same scan, no split.
 * The hook rewrites sp+220 at the head of the assignment epilogue, so every
 * consumer downstream (the assigned_reg store, the sp+232/236 colour masks,
 * the class high-water marks, f_updatelivran) sees one consistent value.
 *
 * Webs are matched by their bittab liverange pointer, exactly like Hook 28,
 * so a BITPOS means the same thing in both hooks.
 *
 * ⚠️ The colour space is NOT the one Hook 18/28 document.  Measured
 * 2026-08-13 by forcing colours 1..25 on TACO/func_801570C0 bitpos 4:
 *   1 v0  2 v1  3 a0  4 a1  5 a2  6 a3  7 t0  8 t1  9 t2 10 t3 11 t4 12 t5
 *  13 ra 14 s0 15 s1 16 s2 17 s3 18 s4 19 s5 20 s6 21 s7 22 s8/fp
 * t6-t9 have NO colour; >=23 aborts uopt.  f_globalcolor's own class-1
 * ranges [1,13) and [14,22) (printed by the trace) corroborate it.
 * Hooks 18/28 agree only up to t5=12 and are wrong from 13 up: their
 * `s0` (17) really forbids $s3, and their `s8` (25) is out of range.
 * --------------------------------------------------------------------- */
#define FORCE_WEB_COLOR_MAX 24
static struct { int bitpos; int color; } ido52_force_web_color[FORCE_WEB_COLOR_MAX];
static int ido52_force_web_color_count;
static int ido52_gccolor_trace;
#define FORCE_WEB_NOSPLIT_MAX 24
static int ido52_force_web_nosplit[FORCE_WEB_NOSPLIT_MAX];
static int ido52_force_web_nosplit_count;

/* ⚠️ EMPIRICAL colour table (measured 2026-08-13 by forcing colours 1..25 on
 * TACO/func_801570C0 bitpos 4 and reading the emitted `sll` destination).
 * This is NOT the table Hook 28 / Hook 18 document: those are correct only
 * up to t5=12 and WRONG from 13 up.  Ground truth:
 *   1 v0  2 v1  3 a0  4 a1  5 a2  6 a3  7 t0  8 t1  9 t2 10 t3 11 t4 12 t5
 *  13 ra 14 s0 15 s1 16 s2 17 s3 18 s4 19 s5 20 s6 21 s7 22 s8/fp
 * f_globalcolor's own class ranges confirm it: class 1 scans [1,13) then
 * [14,22).  t6-t9 have NO colour — they are ugen's temp pool (Hook 36) and
 * uopt can never place a web there.  Colours >= 23 abort uopt. */
static int ido52_rc_from_name(const char *r) {
    static const char *names[] = {
        "zero", "v0", "v1", "a0", "a1", "a2", "a3",
        "t0", "t1", "t2", "t3", "t4", "t5", "ra",
        "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", 0
    };
    int i;
    if (r == 0 || *r == 0) return -1;
    if (strcmp(r, "fp") == 0) return 22;
    if (r[0] == 'N' && r[1] >= '0' && r[1] <= '9') return atoi(r + 1);
    if (strcmp(r, "t6") == 0 || strcmp(r, "t7") == 0 ||
        strcmp(r, "t8") == 0 || strcmp(r, "t9") == 0) {
        fprintf(stderr, "[GC-COLOR] WARNING: %s has no globalcolor colour "
                        "(ugen temp pool only) - entry ignored\n", r);
        return -1;
    }
    for (i = 0; names[i]; i++) if (strcmp(r, names[i]) == 0) return i;
    return -1;
}

/* reverse map a liverange pointer to its bittab index (Hook 28 key space) */
static int ido52_gc_bitpos_of_lr(uint8_t *mem, uint32_t lr) {
    uint32_t bt = MEM_U32(0x1001cbd0);
    uint32_t n = MEM_U32(0x1001cad8);
    uint32_t i;
    if (bt == 0 || lr == 0) return -1;
    if (n > 100000) n = 100000;
    for (i = 0; i < n; i++) {
        if (MEM_U32(bt + i * 8 + 4) == lr) return (int)i;
    }
    return -1;
}

/* Returns the forced colour for this web, or -1 to leave the pick alone.
 * phase: 1 = cost-driven pass, 2 = leftover pass.  cls = register class
 * (f_regclassof result; the colour range tables are indexed by cls-1). */
/* A force is accepted only if the colour is one the allocator itself would
 * have been allowed to choose for this web: inside one of the two class
 * colour ranges AND not set in the web's own forbidden bitvector at lr+40
 * (the exact test f_updatecolorsleft / the colour scan apply:
 *   forbidden  <=>  (MEM_U32(lr + 40 + 4*(c>>5)) << (c & 31)) < 0 ).
 * Otherwise the force is SKIPPED with a warning and the natural pick stands.
 * This is the same soundness argument as Hook 36's free-list force: a legal
 * alternative the compiler could have produced, never an illegal one. */
static int ido52_gc_color_legal(uint8_t *mem, uint32_t s5, int cls, int c) {
    uint32_t o, w;
    int lo1, hi1, lo2, hi2;
    if (c < 0 || c >= 64) return 0;
    if (cls < 1 || cls > 4) return 0;
    o = (uint32_t)((cls - 1) * 4);
    lo1 = (int)MEM_U32(0x1001e570 + o);
    hi1 = (int)MEM_U32(0x1001e580 + o);
    lo2 = (int)MEM_U32(0x1001e578 + o);
    hi2 = (int)MEM_U32(0x1001e588 + o);
    /* both scan loops run lo..hi INCLUSIVE (they pre-increment the
     * limit: `s0 = hi + 1` before `while (s2 != s0)`). */
    if (!((c >= lo1 && c <= hi1) || (c >= lo2 && c <= hi2))) return 0;
    w = MEM_U32(s5 + 40 + (uint32_t)((c >> 5) * 4));
    if ((int32_t)(w << (c & 31)) < 0) return 0;
    return 1;
}

static int ido52_gccolor_event(uint8_t *mem, int phase, uint32_t s5,
                               int chosen, int cls) {
    int bp, i, forced = -1;
    if (ido52_gccolor_trace == 0 && ido52_force_web_color_count == 0) return -1;
    bp = ido52_gc_bitpos_of_lr(mem, s5);
    for (i = 0; i < ido52_force_web_color_count; i++) {
        if (ido52_force_web_color[i].bitpos == bp) {
            forced = ido52_force_web_color[i].color;
        }
    }
    if (forced >= 0 && !ido52_gc_color_legal(mem, s5, cls, forced)) {
        fprintf(stderr, "[GC-COLOR] bitpos=%d colour %d ILLEGAL for this web "
                        "(out of class range or forbidden) - force SKIPPED\n",
                bp, forced);
        forced = -1;
    }
    if (ido52_gccolor_trace) {
        uint32_t o = 0;
        int lo1 = 0, hi1 = 0, lo2 = 0, hi2 = 0;
        if (cls >= 1 && cls <= 4) {
            o = (uint32_t)((cls - 1) * 4);
            lo1 = (int)MEM_U32(0x1001e570 + o);
            hi1 = (int)MEM_U32(0x1001e580 + o);
            lo2 = (int)MEM_U32(0x1001e578 + o);
            hi2 = (int)MEM_U32(0x1001e588 + o);
        }
        fprintf(stderr,
                "[GC-COLOR] phase=%d bitpos=%d lr=0x%08x class=%d color=%d"
                " ranges=[%d,%d][%d,%d] forbid=%08x,%08x%s\n",
                phase, bp, s5, cls, chosen, lo1, hi1, lo2, hi2,
                MEM_U32(s5 + 40), MEM_U32(s5 + 44),
                forced >= 0 ? "  <== FORCE" : "");
        if (forced >= 0) {
            fprintf(stderr, "[GC-COLOR] bitpos=%d colour %d -> %d [FORCED]\n",
                    bp, chosen, forced);
        }
    }
    return forced;
}

/* --- IDO52_FORCE_WEB_RANGE_SPLIT (Hook 43, 2026-08-19) --------------------
 * INVERSE of Hook 37's NOSPLIT: force a named web down f_globalcolor's own
 * phase-1 range-split path (label L46ef64's `f_split` arm) regardless of the
 * `adjsave * occ <= bestcolourcost` compare.  f_split is uopt's native
 * splitter (lr+52 chain and every colouring invariant maintained by the
 * compiler itself); the boundary copies a split produces are uopt's own
 * emissions.  This is the "live-range-split artifact" primitive: the
 * original compiler's dead `move` web-home copies (GYOZI/func_8013306C
 * class) are split boundaries our allocator prices out.
 * IDO52_FORCE_WEB_RANGE_SPLIT=BITPOS[,BITPOS...]   (Hook 28/37 key space)
 * Default OFF; env unset touches nothing. --- */
#define FORCE_WEB_RSPLIT_MAX 24
static int ido52_force_web_rsplit[FORCE_WEB_RSPLIT_MAX];
static int ido52_force_web_rsplit_count;

/* RANGE-SPLIT force: 1 = take the f_split arm for this web. */
static int ido52_gc_rsplit(uint8_t *mem, uint32_t s5) {
    int bp, i;
    if (ido52_force_web_rsplit_count == 0) return 0;
    bp = ido52_gc_bitpos_of_lr(mem, s5);
    for (i = 0; i < ido52_force_web_rsplit_count; i++) {
        if (ido52_force_web_rsplit[i] == bp) {
            if (ido52_gccolor_trace) {
                fprintf(stderr, "[GC-COLOR] bitpos=%d split FORCED\n", bp);
            }
            return 1;
        }
    }
    return 0;
}

/* NOSPLIT veto: 1 = this web must not be range-split at the phase-1 test. */
static int ido52_gc_nosplit(uint8_t *mem, uint32_t s5) {
    int bp, i;
    if (ido52_force_web_nosplit_count == 0) return 0;
    bp = ido52_gc_bitpos_of_lr(mem, s5);
    for (i = 0; i < ido52_force_web_nosplit_count; i++) {
        if (ido52_force_web_nosplit[i] == bp) {
            if (ido52_gccolor_trace) {
                fprintf(stderr, "[GC-COLOR] bitpos=%d split SUPPRESSED\n", bp);
            }
            return 1;
        }
    }
    return 0;
}

/* --- IDO52_PRECOLOR_LIVEIN --- */
/* Forward declarations for functions used in precolor hook */
static void f_setbit(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1);
#define PRECOLOR_MAX_ENTRIES 8
static struct {
    int reg_color;    /* RegisterColor: er_v0=1, er_v1=2, er_t0=7, ..., er_t5=12 */
    int bitpos;       /* bittab index of target variable */
} ido52_precolor_entries[PRECOLOR_MAX_ENTRIES];
static int ido52_precolor_count;

/* --- IDO52_PRECOLOR_WEB (color-only pin, no live-in marking; for regional
 *     const webs). Format: REG:BITPOS[,REG:BITPOS,...]; REG "no" = kill the
 *     web's color post-gc. IDO52_PRECOLOR_WEB_POSTONLY=1 skips the pre-gc
 *     application (no dataflow perturbation). --- */
#define PRECOLOR_WEB_MAX 24
static struct { int reg_color; int bitpos; } ido52_precolor_web_entries[PRECOLOR_WEB_MAX];
static int ido52_precolor_web_count;

/* --- IDO52_WEB_BIND_BBS (post-globalcolor per-block binding extension).
 *     Format: BITPOS:BB[:BB...][,BITPOS:BB...]. After globalcolor, binds the
 *     web's ichain to its ASSIGNED color in the regdata (+0x44) table of the
 *     listed extra basic blocks, so reemission's f_inreg sees the value as
 *     register-resident there (REGOP emission instead of immediate/loadnum).
 *     Emits NO defs and never clobbers an occupied slot (warn+skip). Use
 *     with a CONST_INREG occurrence window when f_constinreg natively
 *     declines the use (cir=0 short-circuits before f_inreg). --- */
#define WEB_BIND_MAX 16
#define WEB_BIND_BB_MAX 16
static struct { int bitpos; int nbb; int bbs[WEB_BIND_BB_MAX]; } ido52_web_bind_entries[WEB_BIND_MAX];
static int ido52_web_bind_count;

/* --- IDO52_CMOT_KILL_AVAIL ---
 *     IDX[:BB][,...]  -- IDX is a codemotion expression-table index (the
 *     middle column of the trace-3 `printtab` dump, i.e. the numbers that
 *     appear in the trace-3 `printcm` per-block antlocs/avlocs/ppin sets).
 *     BB is a basic-block number (`! ! ! ! ! node N ! ! ! ! !`); omitted or
 *     -1 = every block.  Clears bit IDX from the block's PPIN vector
 *     (block+0x12c) inside codemotion's final per-block delete/apply loop,
 *     after ppin has absorbed subinsert and before
 *         subdelete = (ppin - killed) INTERSECT antlocs
 *     is formed -- so the block's own local computation of that expression
 *     is NOT deleted in favour of an earlier one and the value is
 *     recomputed there. --- */
#define CMOT_KILL_MAX 64
static struct { int idx; int bb; } ido52_cmot_kill[CMOT_KILL_MAX];
static int ido52_cmot_kill_count;
static int ido52_cmot_avail_trace;

/* uopt bitvector: [+0] chunk count (128 bits each), [+4] data pointer;
 * each chunk is 4 x u32, bit b sits at MSB-first position 31-(b&31). */
static int ido52_bv_test(uint8_t *mem, uint32_t bv, int bit) {
    uint32_t nch, data, w;
    if (bit < 0) return 0;
    nch = MEM_U32(bv + 0);
    if ((uint32_t)bit >= (nch << 7)) return 0;
    data = MEM_U32(bv + 4);
    if (data == 0) return 0;
    w = data + (uint32_t)((bit >> 7) * 16) + (uint32_t)(((bit & 0x7f) >> 5) * 4);
    return (MEM_U32(w) >> (31 - (bit & 31))) & 1u;
}
static void ido52_bv_reset(uint8_t *mem, uint32_t bv, int bit) {
    uint32_t nch, data, w;
    if (bit < 0) return;
    nch = MEM_U32(bv + 0);
    if ((uint32_t)bit >= (nch << 7)) return;
    data = MEM_U32(bv + 4);
    if (data == 0) return;
    w = data + (uint32_t)((bit >> 7) * 16) + (uint32_t)(((bit & 0x7f) >> 5) * 4);
    MEM_U32(w) = MEM_U32(w) & ~(1u << (31 - (bit & 31)));
}

/*
 * ido52_cmot_kill_avail_block: called once per basic block at the head of
 * f_codemotion's final delete/apply loop (label L4105bc, right after
 * ppin |= subinsert and before subdelete is derived from ppin).
 * blk = the block node; bb number is MEM_U16(blk+8).
 * ppin = blk+0x12c, antlocs = blk+0x104, avlocs = blk+0x114.
 */
static void ido52_cmot_kill_avail_block(uint8_t *mem, uint32_t blk) {
    int i, bb;
    if (ido52_cmot_kill_count == 0 && !ido52_cmot_avail_trace) return;
    if (blk == 0) return;
    bb = (int)MEM_U16(blk + 8);
    for (i = 0; i < ido52_cmot_kill_count; i++) {
        int idx = ido52_cmot_kill[i].idx;
        int want = ido52_cmot_kill[i].bb;
        int pp, al;
        if (want >= 0 && want != bb) continue;
        pp = ido52_bv_test(mem, blk + 0x12c, idx);
        al = ido52_bv_test(mem, blk + 0x104, idx);
        if (ido52_cmot_avail_trace) {
            fprintf(stderr, "[CMOT-AVAIL] bb=%d idx=%d ppin=%d antlocs=%d avlocs=%d -> %s\n",
                    bb, idx, pp, al, ido52_bv_test(mem, blk + 0x114, idx),
                    pp ? "KILLED" : "already-clear");
        }
        if (pp) ido52_bv_reset(mem, blk + 0x12c, idx);
    }
    if (ido52_cmot_avail_trace > 1 && ido52_cmot_kill_count == 0) {
        fprintf(stderr, "[CMOT-AVAIL] bb=%d (no kill entries)\n", bb);
    }
}

/* Helper: check if a dead store target is protected */
static int ido52_is_protected_deadstore(uint8_t *mem, uint32_t expr_ptr) {
    int i;
    uint32_t addr;
    if (ido52_no_deadstore_count == 0) return 0;
    addr = MEM_U32(expr_ptr + 0x28);
    if (ido52_deadstore_debug) {
        fprintf(stderr, "[DEADSTORE] checking store to addr=0x%08x\n", addr);
    }
    for (i = 0; i < ido52_no_deadstore_count; i++) {
        if (addr == ido52_no_deadstore_addrs[i]) {
            if (ido52_deadstore_debug) {
                fprintf(stderr, "[DEADSTORE] PROTECTED: skipping elimination of store to 0x%08x\n", addr);
            }
            return 1;
        }
    }
    return 0;
}

/* Opcode name table for NO_CSE_OPCODES */
static const struct { const char *name; int value; } _opc_names[] = {
    {"abs", 0}, {"add", 1}, {"and", 4}, {"cvt", 24}, {"cvtl", 25},
    {"dec", 26}, {"div", 29}, {"inc", 55}, {"ior", 60}, {"ixa", 65},
    {"lnot", 80}, {"max", 85}, {"min", 86}, {"mod", 87}, {"mpy", 91},
    {"mus", 93}, {"neg", 94}, {"neq", 95}, {"not", 97}, {"rem", 105},
    {"shl", 115}, {"shr", 116}, {"sqr", 118}, {"sub", 125}, {"xor", 141},
    {NULL, 0}
};

static void ido52_init_hooks(void) {
    char *env;
    if (ido52_hooks_initialized) return;
    ido52_hooks_initialized = 1;

    /* IDO52_SKIP_PASSES */
    env = getenv("IDO52_SKIP_PASSES");
    if (env && *env) {
        char buf[256];
        strncpy(buf, env, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        char *tok = strtok(buf, ",");
        while (tok) {
            if (strcmp(tok, "tail_recursion") == 0) ido52_skip_passes_mask |= IDO52_SKIP_TAIL_RECURSION;
            else if (strcmp(tok, "loopunroll") == 0) ido52_skip_passes_mask |= IDO52_SKIP_LOOPUNROLL;
            else if (strcmp(tok, "codemotion") == 0) ido52_skip_passes_mask |= IDO52_SKIP_CODEMOTION;
            else if (strcmp(tok, "eliminduct") == 0) ido52_skip_passes_mask |= IDO52_SKIP_ELIMINDUCT;
            else if (strcmp(tok, "copypropagate") == 0) ido52_skip_passes_mask |= IDO52_SKIP_COPYPROPAGATE;
            else if (strcmp(tok, "opt_saved_regs") == 0) ido52_skip_passes_mask |= IDO52_SKIP_OPT_SAVED_REGS;
            else if (strcmp(tok, "findinduct") == 0) ido52_skip_passes_mask |= IDO52_SKIP_FINDINDUCT;
            tok = strtok(NULL, ",");
        }
        /* findinduct skip implies eliminduct skip */
        if ((ido52_skip_passes_mask & IDO52_SKIP_FINDINDUCT) &&
            !(ido52_skip_passes_mask & IDO52_SKIP_ELIMINDUCT)) {
            ido52_skip_passes_mask |= IDO52_SKIP_ELIMINDUCT;
        }
        fprintf(stderr, "[SKIP-PASS] mask=0x%02x\n", ido52_skip_passes_mask);
    }

    /* IDO52_CONST_NOINREG_VALUES */
    env = getenv("IDO52_CONST_NOINREG_VALUES");
    if (env && *env) {
        char buf2[256];
        strncpy(buf2, env, sizeof(buf2) - 1);
        buf2[sizeof(buf2) - 1] = '\0';
        char *tok2 = strtok(buf2, ",");
        while (tok2 && ido52_const_noinreg_count < 64) {
            /* VALUE[:FROM[:TO]] -- 1-based occurrence window; plain VALUE = all. */
            char *colon2 = strchr(tok2, ':');
            int idx2 = ido52_const_noinreg_count;
            ido52_const_noinreg_from[idx2] = 1;
            ido52_const_noinreg_to[idx2] = 0x7fffffff;
            ido52_const_noinreg_seen[idx2] = 0;
            if (colon2) {
                char *colon3;
                *colon2 = '\0';
                ido52_const_noinreg_from[idx2] = (int)strtol(colon2 + 1, NULL, 0);
                colon3 = strchr(colon2 + 1, ':');
                if (colon3) {
                    ido52_const_noinreg_to[idx2] = (int)strtol(colon3 + 1, NULL, 0);
                }
            }
            ido52_const_noinreg_values[ido52_const_noinreg_count++] = (uint32_t)strtoul(tok2, NULL, 0);
            tok2 = strtok(NULL, ",");
        }
        fprintf(stderr, "[CONST-NOINREG] blocking %d values\n", ido52_const_noinreg_count);
    }

    /* IDO52_CONST_INREG_VALUES (VALUE[:TYPE[:FROM[:TO]]] tuples; TYPE=-1 any,
       FROM/TO = 1-based occurrence window over f_constinreg queries of VALUE,
       default 1..INT_MAX. FROM=0:TO=0 = trace-only (never force). Occurrence
       counters are global per uopt run: use one function per translation unit. */
    env = getenv("IDO52_CONST_INREG_VALUES");
    if (env && *env) {
        char buf2b[256];
        strncpy(buf2b, env, sizeof(buf2b) - 1);
        buf2b[sizeof(buf2b) - 1] = '\0';
        char *tok2b = strtok(buf2b, ",");
        while (tok2b && ido52_const_inreg_count < 64) {
            char *colon2b = strchr(tok2b, ':');
            int idx2b = ido52_const_inreg_count;
            ido52_const_inreg_types[idx2b] = -1;
            ido52_const_inreg_from[idx2b] = 1;
            ido52_const_inreg_to[idx2b] = 0x7fffffff;
            ido52_const_inreg_seen[idx2b] = 0;
            if (colon2b) {
                char *colon3b;
                *colon2b = '\0';
                ido52_const_inreg_types[idx2b] = (int)strtol(colon2b + 1, NULL, 0);
                colon3b = strchr(colon2b + 1, ':');
                if (colon3b) {
                    char *colon4b = strchr(colon3b + 1, ':');
                    ido52_const_inreg_from[idx2b] = (int)strtol(colon3b + 1, NULL, 0);
                    if (colon4b) {
                        ido52_const_inreg_to[idx2b] = (int)strtol(colon4b + 1, NULL, 0);
                    }
                }
            }
            ido52_const_inreg_values[ido52_const_inreg_count++] = (uint32_t)strtoul(tok2b, NULL, 0);
            tok2b = strtok(NULL, ",");
        }
        fprintf(stderr, "[CONST-INREG] forcing %d values\n", ido52_const_inreg_count);
    }

    /* IDO52_LDA_NOINREG_CLASSES (CLASS[:FROM[:TO]] tuples; FROM/TO = 1-based
       occurrence window over f_ldainreg queries of CLASS, default 1..INT_MAX
       = the historical block-everything behaviour.  FROM=0:TO=0 = trace-only
       (never block).  Occurrence counters are global per uopt run: use one
       function per translation unit. */
    env = getenv("IDO52_LDA_NOINREG_CLASSES");
    if (env && *env) {
        char buf3[256];
        strncpy(buf3, env, sizeof(buf3) - 1);
        buf3[sizeof(buf3) - 1] = '\0';
        char *tok3 = strtok(buf3, ",");
        while (tok3 && ido52_lda_noinreg_count < 64) {
            char *colon3 = strchr(tok3, ':');
            int idx3 = ido52_lda_noinreg_count;
            ido52_lda_noinreg_from[idx3] = 1;
            ido52_lda_noinreg_to[idx3] = 0x7fffffff;
            ido52_lda_noinreg_seen[idx3] = 0;
            if (colon3) {
                char *colon4;
                *colon3 = '\0';
                ido52_lda_noinreg_from[idx3] = (int)strtol(colon3 + 1, NULL, 0);
                colon4 = strchr(colon3 + 1, ':');
                if (colon4) {
                    ido52_lda_noinreg_to[idx3] = (int)strtol(colon4 + 1, NULL, 0);
                }
            }
            ido52_lda_noinreg_classes[ido52_lda_noinreg_count++] = (uint32_t)strtoul(tok3, NULL, 0);
            tok3 = strtok(NULL, ",");
        }
        fprintf(stderr, "[LDA-NOINREG] blocking %d classes\n", ido52_lda_noinreg_count);
    }
    if (getenv("IDO52_LDA_NOINREG_DEBUG")) {
        ido52_lda_noinreg_debug = 1;
    }

    /* IDO52_NO_FINDINDUCT_STRIDE */
    env = getenv("IDO52_NO_FINDINDUCT_STRIDE");
    if (env && *env) {
        ido52_no_findinduct_stride = 1;
        fprintf(stderr, "[NO-FINDINDUCT-STRIDE] enabled\n");
    }

    /* IDO52_CSE_MAX_REUSE */
    env = getenv("IDO52_CSE_MAX_REUSE");
    if (env && *env) {
        ido52_cse_max_reuse = atoi(env);
        if (ido52_cse_max_reuse < 1) ido52_cse_max_reuse = 1;
        memset(ido52_cse_track, 0, sizeof(ido52_cse_track));
        fprintf(stderr, "[CSE-MAX-REUSE] limit=%d\n", ido52_cse_max_reuse);
    }

    /* IDO52_CSE_DEBUG */
    env = getenv("IDO52_CSE_DEBUG");
    if (env && *env) {
        ido52_cse_debug = 1;
        fprintf(stderr, "[CSE-DEBUG] enabled\n");
    }

    /* IDO52_NO_CSE */
    env = getenv("IDO52_NO_CSE");
    if (env && *env) {
        if (strcmp(env, "all") == 0) {
            ido52_no_cse_mode = IDO52_NO_CSE_ALL;
            fprintf(stderr, "[NO-CSE] mode=ALL\n");
        } else if (strcmp(env, "op") == 0 || strcmp(env, "1") == 0) {
            ido52_no_cse_mode = IDO52_NO_CSE_OP;
            fprintf(stderr, "[NO-CSE] mode=OP (binary/unary/ixa only)\n");
        }
    }

    /* IDO52_NO_CSE_OPCODES */
    env = getenv("IDO52_NO_CSE_OPCODES");
    if (env && *env) {
        char buf4[256];
        int i;
        ido52_no_cse_mode = IDO52_NO_CSE_OPCODES;
        strncpy(buf4, env, sizeof(buf4) - 1);
        buf4[sizeof(buf4) - 1] = '\0';
        char *tok4 = strtok(buf4, ",");
        while (tok4 && ido52_no_cse_opcode_count < 64) {
            int found = 0;
            for (i = 0; _opc_names[i].name; i++) {
                if (strcmp(tok4, _opc_names[i].name) == 0) {
                    ido52_no_cse_opcode_list[ido52_no_cse_opcode_count++] = (uint8_t)_opc_names[i].value;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                /* try numeric */
                ido52_no_cse_opcode_list[ido52_no_cse_opcode_count++] = (uint8_t)strtoul(tok4, NULL, 0);
            }
            tok4 = strtok(NULL, ",");
        }
        fprintf(stderr, "[NO-CSE-OPCODES] blocking %d opcodes\n", ido52_no_cse_opcode_count);
    }

    /* IDO52_RESERVE_SREGS */
    env = getenv("IDO52_RESERVE_SREGS");
    if (env && *env) {
        static const struct { const char *name; uint32_t bit; } _sreg_map[] = {
            {"s0", 0x00020000}, {"s1", 0x00010000}, {"s2", 0x00008000},
            {"s3", 0x00004000}, {"s4", 0x00002000}, {"s5", 0x00001000},
            {"s6", 0x00000800}, {"s7", 0x00000400}, {"fp", 0x00000200},
            {NULL, 0}
        };
        char buf5[256];
        int i;
        strncpy(buf5, env, sizeof(buf5) - 1);
        buf5[sizeof(buf5) - 1] = '\0';
        char *tok5 = strtok(buf5, ",");
        while (tok5) {
            for (i = 0; _sreg_map[i].name; i++) {
                if (strcmp(tok5, _sreg_map[i].name) == 0) {
                    ido52_reserve_sregs_mask_hi |= _sreg_map[i].bit;
                    break;
                }
            }
            tok5 = strtok(NULL, ",");
        }
        fprintf(stderr, "[RESERVE-SREGS] mask_hi=0x%08x\n", ido52_reserve_sregs_mask_hi);
    }

    /* IDO52_MAX_SREGS */
    env = getenv("IDO52_MAX_SREGS");
    if (env && *env) {
        static const uint32_t _sreg_bits[] = {
            0x00020000, 0x00010000, 0x00008000, 0x00004000,
            0x00002000, 0x00001000, 0x00000800, 0x00000400,
            0x00000200  /* fp */
        };
        int max_s = atoi(env);
        int i;
        if (max_s < 0) max_s = 0;
        if (max_s > 8) max_s = 8;
        for (i = max_s; i < 9; i++) {
            ido52_reserve_sregs_mask_hi |= _sreg_bits[i];
        }
        fprintf(stderr, "[MAX-SREGS] max=%d mask_hi=0x%08x\n", max_s, ido52_reserve_sregs_mask_hi);
    }

    /* IDO52_DEADSTORE_DEBUG */
    env = getenv("IDO52_DEADSTORE_DEBUG");
    if (env && *env) {
        ido52_deadstore_debug = 1;
        fprintf(stderr, "[DEADSTORE-DEBUG] enabled\n");
    }

    /* IDO52_NO_DEADSTORE_GLOBALS */
    env = getenv("IDO52_NO_DEADSTORE_GLOBALS");
    if (env && *env) {
        char buf6[512];
        strncpy(buf6, env, sizeof(buf6) - 1);
        buf6[sizeof(buf6) - 1] = '\0';
        char *tok6 = strtok(buf6, ",");
        while (tok6 && ido52_no_deadstore_count < IDO52_MAX_DEADSTORE_ADDRS) {
            ido52_no_deadstore_addrs[ido52_no_deadstore_count++] = (uint32_t)strtoul(tok6, NULL, 16);
            tok6 = strtok(NULL, ",");
        }
        if (ido52_deadstore_debug) {
            int i;
            fprintf(stderr, "[DEADSTORE] protecting %d addresses:", ido52_no_deadstore_count);
            for (i = 0; i < ido52_no_deadstore_count; i++)
                fprintf(stderr, " 0x%08x", ido52_no_deadstore_addrs[i]);
            fprintf(stderr, "\n");
        }
    }

    /* IDO52_NO_LICM */
    env = getenv("IDO52_NO_LICM");
    if (env && *env) {
        if (strcmp(env, "all") == 0 || strcmp(env, "1") == 0) {
            ido52_no_licm = 1;
            fprintf(stderr, "[NO-LICM] mode=ALL\n");
        } else {
            /* parse as comma-separated hex addresses */
            char buf7[512];
            ido52_no_licm = 2;
            strncpy(buf7, env, sizeof(buf7) - 1);
            buf7[sizeof(buf7) - 1] = '\0';
            char *tok7 = strtok(buf7, ",");
            while (tok7 && ido52_no_licm_addr_count < 64) {
                ido52_no_licm_addrs[ido52_no_licm_addr_count++] = (uint32_t)strtoul(tok7, NULL, 16);
                tok7 = strtok(NULL, ",");
            }
            fprintf(stderr, "[NO-LICM] mode=ADDR, %d addresses\n", ido52_no_licm_addr_count);
        }
    }

    /* IDO52_NO_STRENGTH_REDUCE */
    env = getenv("IDO52_NO_STRENGTH_REDUCE");
    if (env && *env) {
        ido52_no_strength_reduce = 1;
        fprintf(stderr, "[NO-STRENGTH-REDUCE] enabled\n");
    }

    /* IDO52_STRENGTH_REDUCE_DEBUG */
    env = getenv("IDO52_STRENGTH_REDUCE_DEBUG");
    if (env && *env) {
        ido52_strength_reduce_debug = 1;
        fprintf(stderr, "[STRENGTH-REDUCE-DEBUG] enabled\n");
    }

    /* IDO52_NO_LOOP_TEST_REPLACEMENT = 1 | all | bb:N[,N] | occ:N[,N] */
    env = getenv("IDO52_NO_LOOP_TEST_REPLACEMENT");
    if (env && *env && strcmp(env, "0") != 0) {
        char bufL[256];
        const char *spec = env;
        int isbb = 0;
        int isocc = 0;
        if (strncmp(env, "bb:", 3) == 0) { isbb = 1; spec = env + 3; }
        else if (strncmp(env, "occ:", 4) == 0) { isocc = 1; spec = env + 4; }
        if (isbb || isocc) {
            char *tokL;
            strncpy(bufL, spec, sizeof(bufL) - 1);
            bufL[sizeof(bufL) - 1] = '\0';
            tokL = strtok(bufL, ",");
            while (tokL && ido52_ltr_list_count < 64) {
                ido52_ltr_list[ido52_ltr_list_count++] = (int)strtol(tokL, NULL, 0);
                tokL = strtok(NULL, ",");
            }
            ido52_ltr_mode = isbb ? IDO52_LTR_BB : IDO52_LTR_OCC;
            fprintf(stderr, "[NO-LOOP-TEST-REPL] mode=%s entries=%d\n",
                    isbb ? "BB" : "OCC", ido52_ltr_list_count);
        } else {
            ido52_ltr_mode = IDO52_LTR_ALL;
            fprintf(stderr, "[NO-LOOP-TEST-REPL] mode=ALL\n");
        }
    }

    /* IDO52_LOOP_TEST_REPL_DEBUG */
    env = getenv("IDO52_LOOP_TEST_REPL_DEBUG");
    if (env && *env) {
        ido52_ltr_debug = 1;
        fprintf(stderr, "[NO-LOOP-TEST-REPL] debug enabled\n");
    }

    /* IDO52_EXCLUDE_REGS=s0,s1,v0,v1,a0..a3,t0..t9,fp,...
       OR into per-web AND global forbidden bitvectors so the Chaitin
       allocator skips these colors. Caller-saved regs (v/a/t) are
       supported in addition to callee-saved (s/fp) — useful when
       target binary's hidden-param convention forces a specific
       register that IDO would otherwise reuse for a temp. */
    env = getenv("IDO52_EXCLUDE_REGS");
    if (env && *env) {
        static const struct { const char *name; int regnum; } _ermap[] = {
            {"v0",  2}, {"v1",  3},
            {"a0",  4}, {"a1",  5}, {"a2",  6}, {"a3",  7},
            {"t0",  8}, {"t1",  9}, {"t2", 10}, {"t3", 11},
            {"t4", 12}, {"t5", 13}, {"t6", 14}, {"t7", 15},
            {"s0", 16}, {"s1", 17}, {"s2", 18}, {"s3", 19},
            {"s4", 20}, {"s5", 21}, {"s6", 22}, {"s7", 23},
            {"t8", 24}, {"t9", 25},
            {"fp", 30}, {"s8", 30}, {"ra", 31}, {NULL, 0}
        };
        char buf9[256];
        int i;
        strncpy(buf9, env, sizeof(buf9) - 1);
        buf9[sizeof(buf9) - 1] = '\0';
        {
            char *tok9 = strtok(buf9, ",");
            while (tok9) {
                for (i = 0; _ermap[i].name; i++) {
                    if (strcmp(tok9, _ermap[i].name) == 0) {
                        ido52_exclude_regs_mask_lo |= (1u << _ermap[i].regnum);  /* bit N = register N (direct mapping) */
                        break;
                    }
                }
                tok9 = strtok(NULL, ",");
            }
        }
        fprintf(stderr, "[EXCLUDE-REGS] mask_lo=0x%08x\\n", ido52_exclude_regs_mask_lo);
    }

    /* IDO52_EXCLUDE_REGS_WEB=BITPOS:reg[+reg...][,...] (Hook 28) */
    env = getenv("IDO52_EXCLUDE_REGS_WEB");
    if (env && *env) {
        char bufe[512];
        char *toke;
        strncpy(bufe, env, sizeof(bufe) - 1);
        bufe[sizeof(bufe) - 1] = '\0';
        toke = strtok(bufe, ",");
        while (toke && ido52_exclregs_web_count < EXCLREGS_WEB_MAX) {
            char *colon = strchr(toke, ':');
            if (colon) {
                uint32_t mask = 0;
                char *r = colon + 1;
                *colon = '\0';
                while (r && *r) {
                    char *plus = strchr(r, '+');
                    int rc = -1;   /* -1 = unrecognised; 0 is a VALID colour ($zero) */
                    if (plus) *plus = '\0';
                    if (strcmp(r, "zero") == 0 || strcmp(r, "z") == 0) rc = 0;
                    else if (strcmp(r, "v0") == 0) rc = 1;
                    else if (strcmp(r, "v1") == 0) rc = 2;
                    else if (strcmp(r, "a0") == 0) rc = 3;
                    else if (strcmp(r, "a1") == 0) rc = 4;
                    else if (strcmp(r, "a2") == 0) rc = 5;
                    else if (strcmp(r, "a3") == 0) rc = 6;
                    else if (strcmp(r, "t0") == 0) rc = 7;
                    else if (strcmp(r, "t1") == 0) rc = 8;
                    else if (strcmp(r, "t2") == 0) rc = 9;
                    else if (strcmp(r, "t3") == 0) rc = 10;
                    else if (strcmp(r, "t4") == 0) rc = 11;
                    else if (strcmp(r, "t5") == 0) rc = 12;
                    /* CORRECTED 2026-08-13 (Hook 37 empirical map):
                       ra=13, s0..s7=14..21, s8/fp=22.  t6-t9 have NO
                       color (ugen-only pool) -- refused loudly.  The
                       old table (t6..t9=13..16, s0..s8=17..25) made
                       every s-register spec forbid a different set
                       than it named. */
                    else if (strcmp(r, "ra") == 0) rc = 13;
                    else if (strcmp(r, "s0") == 0) rc = 14;
                    else if (strcmp(r, "s1") == 0) rc = 15;
                    else if (strcmp(r, "s2") == 0) rc = 16;
                    else if (strcmp(r, "s3") == 0) rc = 17;
                    else if (strcmp(r, "s4") == 0) rc = 18;
                    else if (strcmp(r, "s5") == 0) rc = 19;
                    else if (strcmp(r, "s6") == 0) rc = 20;
                    else if (strcmp(r, "s7") == 0) rc = 21;
                    else if (strcmp(r, "s8") == 0 || strcmp(r, "fp") == 0) rc = 22;
                    else if (strcmp(r, "t6") == 0 || strcmp(r, "t7") == 0 ||
                             strcmp(r, "t8") == 0 || strcmp(r, "t9") == 0) {
                        fprintf(stderr, "[EXCLREGS-WEB] %s has no globalcolor color (ugen temp) -- token IGNORED\n", r);
                    }
                    if (rc >= 0) {
                        /* The lr+40 forbidden bitvector is indexed by the rc
                         * color value ITSELF, MSB-first (bit 31 - rc); see
                         * f_updatecolorsleft.  The rc-3 "dump space" belongs
                         * to the printregs listing, NOT to this vector --
                         * subtracting 3 here was an OFF-BY-THREE (2026-08-09). */
                        mask |= 1u << ((~(uint32_t)rc) & 31u);
                    } else {
                        fprintf(stderr, "[EXCLREGS-WEB] WARNING: unknown register "
                                "name '%s' ignored\n", r);
                    }
                    r = plus ? plus + 1 : 0;
                }
                if (mask) {
                    ido52_exclregs_web[ido52_exclregs_web_count].bitpos = atoi(toke);
                    ido52_exclregs_web[ido52_exclregs_web_count].mask = mask;
                    ido52_exclregs_web_count++;
                }
            }
            toke = strtok(NULL, ",");
        }
        ido52_exclregs_web_debug = getenv("IDO52_EXCLUDE_REGS_WEB_DEBUG") != 0;
        if (ido52_exclregs_web_count > 0) {
            fprintf(stderr, "[EXCLREGS-WEB] %d entries configured\n",
                    ido52_exclregs_web_count);
        }
    }

    /* IDO52_FORCE_WEB_COLOR=BITPOS:reg[,BITPOS:reg...] (Hook 37) */
    env = getenv("IDO52_FORCE_WEB_COLOR");
    if (env && *env) {
        char buff[512];
        char *tokf;
        strncpy(buff, env, sizeof(buff) - 1);
        buff[sizeof(buff) - 1] = '\0';
        tokf = strtok(buff, ",");
        while (tokf && ido52_force_web_color_count < FORCE_WEB_COLOR_MAX) {
            char *colon = strchr(tokf, ':');
            if (colon) {
                int rc;
                *colon = '\0';
                rc = ido52_rc_from_name(colon + 1);
                if (rc >= 0) {
                    ido52_force_web_color[ido52_force_web_color_count].bitpos = atoi(tokf);
                    ido52_force_web_color[ido52_force_web_color_count].color = rc;
                    ido52_force_web_color_count++;
                } else {
                    fprintf(stderr, "[GC-COLOR] WARNING: unknown register '%s'\n",
                            colon + 1);
                }
            }
            tokf = strtok(NULL, ",");
        }
        if (ido52_force_web_color_count > 0) {
            fprintf(stderr, "[GC-COLOR] %d force entries configured\n",
                    ido52_force_web_color_count);
        }
    }
    /* IDO52_FORCE_WEB_RANGE_SPLIT=BITPOS[,BITPOS...] (Hook 43) */
    env = getenv("IDO52_FORCE_WEB_RANGE_SPLIT");
    if (env && *env) {
        char bufr[256];
        char *tokr;
        strncpy(bufr, env, sizeof(bufr) - 1);
        bufr[sizeof(bufr) - 1] = '\0';
        tokr = strtok(bufr, ",");
        while (tokr && ido52_force_web_rsplit_count < FORCE_WEB_RSPLIT_MAX) {
            ido52_force_web_rsplit[ido52_force_web_rsplit_count++] = atoi(tokr);
            tokr = strtok(NULL, ",");
        }
        fprintf(stderr, "[GC-COLOR] %d range-split force entries configured\n",
                ido52_force_web_rsplit_count);
    }
    /* IDO52_FORCE_WEB_COLOR_NOSPLIT=BITPOS[,BITPOS...] (Hook 37) */
    env = getenv("IDO52_FORCE_WEB_COLOR_NOSPLIT");
    if (env && *env) {
        char bufn[256];
        char *tokn;
        strncpy(bufn, env, sizeof(bufn) - 1);
        bufn[sizeof(bufn) - 1] = '\0';
        tokn = strtok(bufn, ",");
        while (tokn && ido52_force_web_nosplit_count < FORCE_WEB_NOSPLIT_MAX) {
            ido52_force_web_nosplit[ido52_force_web_nosplit_count++] = atoi(tokn);
            tokn = strtok(NULL, ",");
        }
        fprintf(stderr, "[GC-COLOR] %d nosplit entries configured\n",
                ido52_force_web_nosplit_count);
    }
    /* IDO52_GC_COLOR_TRACE=1 */
    env = getenv("IDO52_GC_COLOR_TRACE");
    if (env && *env) ido52_gccolor_trace = atoi(env) ? atoi(env) : 1;

    /* IDO52_PRECOLOR_LIVEIN=REG:BITPOS[,REG:BITPOS,...] */
    env = getenv("IDO52_PRECOLOR_LIVEIN");
    if (env && *env) {
        char buf8[256];
        char *tok8;
        strncpy(buf8, env, sizeof(buf8) - 1);
        buf8[sizeof(buf8) - 1] = '\0';
        tok8 = strtok(buf8, ",");
        while (tok8 && ido52_precolor_count < PRECOLOR_MAX_ENTRIES) {
            char *colon = strchr(tok8, ':');
            if (colon) {
                int rc = 0;
                *colon = '\0';
                /* IDO RegisterColor enum: er_v0=1, er_v1=2,
                 * er_a0=3..er_a3=6, er_t0=7..er_t9=16 */
                if (strcmp(tok8, "v0") == 0) rc = 1;
                else if (strcmp(tok8, "v1") == 0) rc = 2;
                else if (strcmp(tok8, "a0") == 0) rc = 3;
                else if (strcmp(tok8, "a1") == 0) rc = 4;
                else if (strcmp(tok8, "a2") == 0) rc = 5;
                else if (strcmp(tok8, "a3") == 0) rc = 6;
                else if (strcmp(tok8, "t0") == 0) rc = 7;
                else if (strcmp(tok8, "t1") == 0) rc = 8;
                else if (strcmp(tok8, "t2") == 0) rc = 9;
                else if (strcmp(tok8, "t3") == 0) rc = 10;
                else if (strcmp(tok8, "t4") == 0) rc = 11;
                else if (strcmp(tok8, "t5") == 0) rc = 12;
                else if (strcmp(tok8, "t6") == 0 || strcmp(tok8, "t7") == 0 ||
                         strcmp(tok8, "t8") == 0 || strcmp(tok8, "t9") == 0) {
                    /* t6-t9 are ugen scratch temps, NOT in globalcolor's
                       RegisterColor pool (measured 2026-08-13: 13 = the
                       special ra color, 14+ invalid).  Refuse loudly
                       instead of pinning garbage. */
                    fprintf(stderr, "[PRECOLOR] %s is not a colorable register (ugen temp) -- token IGNORED; pin v0..t5 and paint\n", tok8);
                }
                else if (tok8[0] == 'N') { int nv = atoi(tok8 + 1); if (nv > 0 && nv < 32) rc = nv; }
                if (rc > 0) {
                    int bp = atoi(colon + 1);
                    ido52_precolor_entries[ido52_precolor_count].reg_color = rc;
                    ido52_precolor_entries[ido52_precolor_count].bitpos = bp;
                    ido52_precolor_count++;
                    fprintf(stderr, "[PRECOLOR] %s (color=%d) -> bitpos %d\n", tok8, rc, bp);
                }
            }
            tok8 = strtok(NULL, ",");
        }
        if (ido52_precolor_count > 0) {
            fprintf(stderr, "[PRECOLOR] %d entries configured\n", ido52_precolor_count);
        }
    }

    /* IDO52_PRECOLOR_WEB=REG:BITPOS[,...] -- color-only web pinning */
    env = getenv("IDO52_PRECOLOR_WEB");
    if (env && *env) {
        char bufw[512];
        char *tokw;
        strncpy(bufw, env, sizeof(bufw) - 1);
        bufw[sizeof(bufw) - 1] = '\0';
        tokw = strtok(bufw, ",");
        while (tokw && ido52_precolor_web_count < PRECOLOR_WEB_MAX) {
            char *colon = strchr(tokw, ':');
            if (colon) {
                int rc = 0;
                *colon = '\0';
                if (strcmp(tokw, "no") == 0) rc = 255;  /* kill color post-gc */
                else if (strcmp(tokw, "v0") == 0) rc = 1;
                else if (strcmp(tokw, "v1") == 0) rc = 2;
                else if (strcmp(tokw, "a0") == 0) rc = 3;
                else if (strcmp(tokw, "a1") == 0) rc = 4;
                else if (strcmp(tokw, "a2") == 0) rc = 5;
                else if (strcmp(tokw, "a3") == 0) rc = 6;
                else if (strcmp(tokw, "t0") == 0) rc = 7;
                else if (strcmp(tokw, "t1") == 0) rc = 8;
                else if (strcmp(tokw, "t2") == 0) rc = 9;
                else if (strcmp(tokw, "t3") == 0) rc = 10;
                else if (strcmp(tokw, "t4") == 0) rc = 11;
                else if (strcmp(tokw, "t5") == 0) rc = 12;
                else if (strcmp(tokw, "t6") == 0) rc = 13;
                else if (strcmp(tokw, "t7") == 0) rc = 14;
                else if (strcmp(tokw, "t8") == 0) rc = 15;
                else if (strcmp(tokw, "t9") == 0) rc = 16;
                else if (strcmp(tokw, "s0") == 0) rc = 17;
                else if (strcmp(tokw, "s1") == 0) rc = 18;
                else if (strcmp(tokw, "s2") == 0) rc = 19;
                else if (strcmp(tokw, "s3") == 0) rc = 20;
                else if (strcmp(tokw, "s4") == 0) rc = 21;
                else if (strcmp(tokw, "s5") == 0) rc = 22;
                else if (strcmp(tokw, "s6") == 0) rc = 23;
                else if (strcmp(tokw, "s7") == 0) rc = 24;
                else if (strcmp(tokw, "s8") == 0 || strcmp(tokw, "fp") == 0) rc = 25;
                if (rc > 0) {
                    ido52_precolor_web_entries[ido52_precolor_web_count].reg_color = rc;
                    ido52_precolor_web_entries[ido52_precolor_web_count].bitpos = atoi(colon + 1);
                    ido52_precolor_web_count++;
                }
            }
            tokw = strtok(NULL, ",");
        }
        if (ido52_precolor_web_count > 0) {
            fprintf(stderr, "[PRECOLOR-WEB] %d entries configured\n", ido52_precolor_web_count);
        }
    }

    /* IDO52_WEB_BIND_BBS=BITPOS:BB[:BB...][,...] -- post-gc binding extension */
    env = getenv("IDO52_WEB_BIND_BBS");
    if (env && *env) {
        char bufb[512];
        char *tokb;
        strncpy(bufb, env, sizeof(bufb) - 1);
        bufb[sizeof(bufb) - 1] = '\0';
        tokb = strtok(bufb, ",");
        while (tokb && ido52_web_bind_count < WEB_BIND_MAX) {
            char *p = strchr(tokb, ':');
            if (p) {
                int _wb = ido52_web_bind_count;
                *p = '\0';
                ido52_web_bind_entries[_wb].bitpos = atoi(tokb);
                ido52_web_bind_entries[_wb].nbb = 0;
                while (p && ido52_web_bind_entries[_wb].nbb < WEB_BIND_BB_MAX) {
                    char *q = strchr(p + 1, ':');
                    if (q) *q = '\0';
                    ido52_web_bind_entries[_wb].bbs[ido52_web_bind_entries[_wb].nbb++] = atoi(p + 1);
                    p = q;
                }
                if (ido52_web_bind_entries[_wb].nbb > 0) ido52_web_bind_count++;
            }
            tokb = strtok(NULL, ",");
        }
        if (ido52_web_bind_count > 0) {
            fprintf(stderr, "[WEB-BIND-BBS] %d entries configured\n", ido52_web_bind_count);
        }
    }

    /* IDO52_CMOT_KILL_AVAIL -- IDX[:BB][,...] */
    env = getenv("IDO52_CMOT_KILL_AVAIL");
    if (env && *env) {
        char bufk[512];
        char *tokk;
        strncpy(bufk, env, sizeof(bufk) - 1);
        bufk[sizeof(bufk) - 1] = '\0';
        tokk = strtok(bufk, ",");
        while (tokk && ido52_cmot_kill_count < CMOT_KILL_MAX) {
            char *pk = strchr(tokk, ':');
            int _k = ido52_cmot_kill_count;
            if (pk) *pk = '\0';
            ido52_cmot_kill[_k].idx = atoi(tokk);
            ido52_cmot_kill[_k].bb  = pk ? atoi(pk + 1) : -1;
            ido52_cmot_kill_count++;
            tokk = strtok(NULL, ",");
        }
        if (ido52_cmot_kill_count > 0) {
            fprintf(stderr, "[CMOT-KILL-AVAIL] %d entries configured\n",
                    ido52_cmot_kill_count);
        }
    }

    /* IDO52_CMOT_AVAIL_TRACE */
    env = getenv("IDO52_CMOT_AVAIL_TRACE");
    if (env && *env) {
        ido52_cmot_avail_trace = atoi(env);
        if (ido52_cmot_avail_trace <= 0) ido52_cmot_avail_trace = 1;
        fprintf(stderr, "[CMOT-AVAIL] trace level %d\n", ido52_cmot_avail_trace);
    }
}

/*
 * ido52_apply_precolor_web: After makelivranges(), before regdataflow().
 * Color-only pin: sets the register color on all LiveUnits of the web and
 * marks the color used in the covered blocks. Does NOT mark live-on-entry,
 * does NOT set hasstore, does NOT touch the graphhead or livein bitvector --
 * safe for REGIONAL webs (e.g. split constant ranges).
 */
static void ido52_apply_precolor_web(uint8_t *mem, uint32_t sp) {
    int i;
    uint32_t bittab_ptr;
    if (ido52_precolor_web_count == 0) return;
    if (getenv("IDO52_PRECOLOR_WEB_POSTONLY")) return;
    bittab_ptr = MEM_U32(0x1001cbd0);
    if (bittab_ptr == 0) return;
    for (i = 0; i < ido52_precolor_web_count; i++) {
        int reg_color = ido52_precolor_web_entries[i].reg_color;
        int bitpos = ido52_precolor_web_entries[i].bitpos;
        uint32_t liverange, lu;
        if (reg_color == 255) continue;  /* "no": post-gc only */
        liverange = MEM_U32(bittab_ptr + (uint32_t)(bitpos * 8) + 4);
        if (liverange == 0) {
            fprintf(stderr, "[PRECOLOR-WEB] bitpos %d: no liverange\n", bitpos);
            continue;
        }
        lu = MEM_U32(liverange + 8);
        while (lu != 0) {
            uint32_t lu_node = MEM_U32(lu + 0);
            MEM_U8(lu + 0x13) = (uint8_t)reg_color;
            if (lu_node != 0) {
                uint32_t bm = 1u << ((~(uint32_t)reg_color) & 31u);
                MEM_U32(lu_node + 0x34) = MEM_U32(lu_node + 0x34) | bm;
            }
            lu = MEM_U32(lu + 4);
        }
        fprintf(stderr, "[PRECOLOR-WEB] applied bitpos %d -> color %d\n", bitpos, reg_color);
    }
}

/*
 * ido52_apply_precolor_web_post_gc: After globalcolor(), force assigned_reg
 * and rebind block regdata for the pinned webs (mirror of the LIVEIN post-gc
 * step, without any live-in side effects).
 */
static void ido52_apply_precolor_web_post_gc(uint8_t *mem) {
    int i;
    uint32_t bittab_ptr;
    if (ido52_precolor_web_count == 0) return;
    bittab_ptr = MEM_U32(0x1001cbd0);
    if (bittab_ptr == 0) return;
    for (i = 0; i < ido52_precolor_web_count; i++) {
        int rc = ido52_precolor_web_entries[i].reg_color;
        int bp = ido52_precolor_web_entries[i].bitpos;
        uint32_t lr = MEM_U32(bittab_ptr + (uint32_t)(bp * 8) + 4);
        uint32_t lu2, ichain;
        uint8_t old_ar;
        if (lr == 0) continue;
        old_ar = MEM_U8(lr + 0x20);
        ichain = MEM_U32(lr + 0x00);
        MEM_U8(lr + 0x20) = (uint8_t)rc;
        lu2 = MEM_U32(lr + 0x08);
        while (lu2 != 0) {
            uint32_t nd = MEM_U32(lu2 + 0x00);
            /* per-LiveUnit color: reemission reads this for def placement */
            MEM_U8(lu2 + 0x13) = (uint8_t)rc;
            if (nd != 0) {
                if (old_ar > 0 && old_ar <= 35) {
                    uint32_t os = nd + 0x44 + (uint32_t)(old_ar - 1) * 4;
                    if (MEM_U32(os) == ichain) MEM_U32(os) = 0;
                }
                if (rc > 0 && rc <= 35) {
                    MEM_U32(nd + 0x44 + (uint32_t)(rc - 1) * 4) = ichain;
                }
            }
            lu2 = MEM_U32(lu2 + 0x04);
        }
        fprintf(stderr, "[PRECOLOR-WEB] post-gc: bitpos %d assigned_reg %d->%d\n", bp, old_ar, rc);
    }
}

/*
 * ido52_apply_precolor_livein: After makelivranges(), before regdataflow().
 * Pre-colors specified variables to physical registers as Live-On-Entry.
 */
static void ido52_apply_precolor_livein(uint8_t *mem, uint32_t sp) {
    int i;
    uint32_t graphhead, bittab_ptr;
    if (ido52_precolor_count == 0) return;
    graphhead = MEM_U32(0x10021c14);
    bittab_ptr = MEM_U32(0x1001cbd0);
    if (graphhead == 0 || bittab_ptr == 0) return;

    for (i = 0; i < ido52_precolor_count; i++) {
        int reg_color = ido52_precolor_entries[i].reg_color;
        int bitpos = ido52_precolor_entries[i].bitpos;
        uint32_t liverange = MEM_U32(bittab_ptr + (uint32_t)(bitpos * 8) + 4);
        uint32_t lu;
        if (liverange == 0) continue;

        /* Set reg on all LiveUnits, mark regsused in each block */
        lu = MEM_U32(liverange + 8);
        while (lu != 0) {
            uint32_t lu_node = MEM_U32(lu + 0);
            MEM_U8(lu + 0x13) = (uint8_t)reg_color;
            if (lu_node == graphhead) MEM_U8(lu + 0x14) = 1;
            if (lu_node != 0) {
                uint32_t bm = 1u << ((~(uint32_t)reg_color) & 31u);
                MEM_U32(lu_node + 0x34) = MEM_U32(lu_node + 0x34) | bm;
            }
            lu = MEM_U32(lu + 4);
        }
        MEM_U8(liverange + 0x22) = 1;  /* hasstore */
        { uint32_t bm = 1u << ((~(uint32_t)reg_color) & 31u);
          MEM_U32(graphhead + 0x34) = MEM_U32(graphhead + 0x34) | bm; }
        f_setbit(mem, sp, 0x1001cb98, (uint32_t)bitpos);
        fprintf(stderr, "[PRECOLOR] applied bitpos %d -> color %d\n", bitpos, reg_color);
    }
}

/*
 * ido52_apply_precolor_post_globalcolor: After globalcolor(), force assigned_reg
 * and update regdata.unk44 in all blocks.
 */
static void ido52_apply_precolor_post_globalcolor(uint8_t *mem) {
    int i;
    uint32_t bittab_ptr;
    if (ido52_precolor_count == 0) return;
    bittab_ptr = MEM_U32(0x1001cbd0);
    if (bittab_ptr == 0) return;

    for (i = 0; i < ido52_precolor_count; i++) {
        int rc = ido52_precolor_entries[i].reg_color;
        int bp = ido52_precolor_entries[i].bitpos;
        uint32_t lr = MEM_U32(bittab_ptr + (uint32_t)(bp * 8) + 4);
        uint32_t lu2, ichain;
        uint8_t old_ar;
        if (lr == 0) continue;
        old_ar = MEM_U8(lr + 0x20);
        ichain = MEM_U32(lr + 0x00);
        MEM_U8(lr + 0x20) = (uint8_t)rc;

        /* Update regdata in all blocks */
        lu2 = MEM_U32(lr + 0x08);
        while (lu2 != 0) {
            uint32_t nd = MEM_U32(lu2 + 0x00);
            if (nd != 0) {
                if (old_ar > 0 && old_ar <= 35) {
                    uint32_t os = nd + 0x44 + (uint32_t)(old_ar - 1) * 4;
                    if (MEM_U32(os) == ichain) MEM_U32(os) = 0;
                }
                if (rc > 0 && rc <= 35) {
                    MEM_U32(nd + 0x44 + (uint32_t)(rc - 1) * 4) = ichain;
                }
            }
            lu2 = MEM_U32(lu2 + 0x04);
        }
        /* Also update graphhead */
        { uint32_t gh = MEM_U32(0x10021c14);
          if (old_ar > 0 && old_ar <= 35) {
              uint32_t os = gh + 0x44 + (uint32_t)(old_ar - 1) * 4;
              if (MEM_U32(os) == ichain) MEM_U32(os) = 0;
          }
          if (rc > 0 && rc <= 35) {
              MEM_U32(gh + 0x44 + (uint32_t)(rc - 1) * 4) = ichain;
          }
        }
        fprintf(stderr, "[PRECOLOR] post-gc: bitpos %d assigned_reg %d->%d\n", bp, old_ar, rc);
    }
}

/*
 * ido52_mem_readable: probe whether [addr, addr+len) in the emulated memory
 * arena can actually be read.  libc_impl reserves the whole ~3.75GB arena
 * PROT_NONE and only makes the committed sub-ranges RW, and `mem` itself is
 * biased (mmap_base - MEM_REGION_START), so a word that is not really a
 * pointer dereferences either onto a guard page or clean outside the
 * reservation -- either way SIGSEGV, not garbage.
 *
 * The probe touches the bytes with SIGSEGV/SIGBUS temporarily redirected to
 * a siglongjmp, which is the only check that covers both the PROT_NONE guard
 * pages (still "mapped", so mincore/msync say yes) and addresses outside the
 * reservation.  A write(2) probe does NOT work: writes to /dev/null are
 * discarded by the kernel without ever touching the user buffer, so they
 * report every address readable.
 *
 * DEBUG-ONLY: called exclusively from the env-gated dump path; the previous
 * handlers are saved and restored around every probe.
 */
static sigjmp_buf ido52_probe_jmp;
static volatile sig_atomic_t ido52_probe_active;

static void ido52_probe_fault(int sig) {
    if (ido52_probe_active) {
        siglongjmp(ido52_probe_jmp, 1);
    }
    /* Not ours: restore the default disposition and re-raise. */
    signal(sig, SIG_DFL);
    raise(sig);
}

static int ido52_mem_readable(uint8_t *mem, uint32_t addr, uint32_t len) {
    struct sigaction sa, old_segv, old_bus;
    const volatile uint8_t *p = (const volatile uint8_t *)(mem + addr);
    volatile uint8_t sink = 0;
    volatile uint32_t i;
    int ok = 1;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = ido52_probe_fault;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NODEFER;
    if (sigaction(SIGSEGV, &sa, &old_segv) != 0) return 1;
    sigaction(SIGBUS, &sa, &old_bus);

    ido52_probe_active = 1;
    if (sigsetjmp(ido52_probe_jmp, 1) == 0) {
        for (i = 0; i < len; i++) {
            sink = (uint8_t)(sink + p[i]);
        }
    } else {
        ok = 0;
    }
    ido52_probe_active = 0;

    sigaction(SIGSEGV, &old_segv, NULL);
    sigaction(SIGBUS, &old_bus, NULL);
    (void)sink;
    return ok;
}

/*
 * ido52_dump_web_assignments: After globalcolor(), print all web assignments.
 * Enabled by IDO52_DUMP_WEBS=1 (2 = also show the raw bittab words).
 *
 * numwebs (0x1001c8a4) is NOT the number of populated bittab slots: the tail
 * of the array holds stale/uninitialised words, and a slot whose second word
 * is a small integer (0x33, 0x13, ...) used to be dereferenced as a
 * liverange pointer and killed uopt with SIGSEGV mid-dump.  That is why the
 * crash looked size-independent -- it depended only on whether a junk slot
 * happened to fall below numwebs.  Every dereference is therefore probed
 * first, and the walk stops when the bittab itself leaves mapped memory.
 * The old `i < 200` cap (which silently truncated the dump on large
 * procedures, hiding exactly the webs PRECOLOR_WEB discovery needs) is gone.
 */
static void ido52_dump_web_assignments(uint8_t *mem) {
    uint32_t bittab_ptr, numwebs, i, shown = 0, skipped = 0;
    int verbose;
    char *env = getenv("IDO52_DUMP_WEBS");
    if (!env || env[0] == '\0' || env[0] == '0') return;
    verbose = (env[0] == '2');
    bittab_ptr = MEM_U32(0x1001cbd0);
    numwebs = MEM_U32(0x1001c8a4);
    if (bittab_ptr == 0) return;
    fprintf(stderr, "[DUMP-WEBS] %u webs after globalcolor (bittab=0x%08x):\n",
            numwebs, bittab_ptr);
    if (numwebs > 100000u) {
        fprintf(stderr, "[DUMP-WEBS] numwebs implausible, clamping to 100000\n");
        numwebs = 100000u;
    }
    for (i = 0; i < numwebs; i++) {
        uint32_t ent = bittab_ptr + i * 8;
        uint32_t lr, ichain;
        uint32_t assigned_reg, hasstore, ic_type, ic_addr;
        if (!ido52_mem_readable(mem, ent, 8)) {
            fprintf(stderr, "  [%3u] bittab entry 0x%08x unmapped -- stop\n",
                    i, ent);
            break;
        }
        lr = MEM_U32(ent + 4);
        if (verbose) {
            fprintf(stderr, "  [%3u] raw w0=0x%08x lr=0x%08x\n",
                    i, MEM_U32(ent + 0), lr);
        }
        if (lr == 0) continue;
        if (!ido52_mem_readable(mem, lr, 0x24)) {
            fprintf(stderr, "  [%3u] lr=0x%08x NOT-A-LIVERANGE (unmapped, skip)\n",
                    i, lr);
            skipped++;
            continue;
        }
        assigned_reg = MEM_U8(lr + 0x20);
        hasstore = MEM_U8(lr + 0x22);
        ichain = MEM_U32(lr + 0x00);
        ic_type = 0;
        ic_addr = 0;
        if (ichain != 0) {
            if (ido52_mem_readable(mem, ichain, 8)) {
                ic_type = MEM_U8(ichain + 0x03);
                ic_addr = MEM_U32(ichain + 0x04);
            } else {
                fprintf(stderr, "  [%3u] reg=%2u hasstore=%u ichain=0x%08x "
                        "(ichain unmapped)\n", i, assigned_reg, hasstore, ichain);
                shown++;
                continue;
            }
        }
        fprintf(stderr, "  [%3u] reg=%2u hasstore=%u ichain=0x%08x type=%u addr=0x%08x\n",
                i, assigned_reg, hasstore, ichain, ic_type, ic_addr);
        shown++;
    }
    fprintf(stderr, "[DUMP-WEBS] %u printed, %u unmapped-slot(s) skipped\n",
            shown, skipped);
}

/*
 * ido52_apply_web_bind_bbs: After globalcolor(). For each entry, bind the
 * web's ichain to its ASSIGNED color in the regdata (+0x44) table of the
 * listed extra blocks so reemission's f_inreg reports the value register-
 * resident there. Emits no defs; refuses to clobber an occupied slot.
 * Block nodes are located by walking every web's LiveUnits and indexing
 * nodes by their block number (node+8, u16).
 */
static void ido52_apply_web_bind_bbs(uint8_t *mem) {
    static uint32_t node_by_bb[4096];
    uint32_t bittab_ptr, numwebs, w;
    int i, j;
    if (ido52_web_bind_count == 0) return;
    bittab_ptr = MEM_U32(0x1001cbd0);
    numwebs = MEM_U32(0x1001c8a4);
    if (bittab_ptr == 0) return;
    memset(node_by_bb, 0, sizeof(node_by_bb));
    for (w = 0; w < numwebs; w++) {
        uint32_t lr = MEM_U32(bittab_ptr + w * 8 + 4);
        uint32_t lu;
        if (lr == 0) continue;
        lu = MEM_U32(lr + 8);
        while (lu != 0) {
            uint32_t nd = MEM_U32(lu + 0);
            if (nd != 0) {
                uint32_t bb = MEM_U16(nd + 8);
                if (bb < 4096) node_by_bb[bb] = nd;
            }
            lu = MEM_U32(lu + 4);
        }
    }
    for (i = 0; i < ido52_web_bind_count; i++) {
        int bp = ido52_web_bind_entries[i].bitpos;
        uint32_t lr = MEM_U32(bittab_ptr + (uint32_t)(bp * 8) + 4);
        uint32_t ichain;
        uint8_t rc;
        if (lr == 0) {
            fprintf(stderr, "[WEB-BIND-BBS] bitpos %d: no liverange\n", bp);
            continue;
        }
        rc = MEM_U8(lr + 0x20);
        ichain = MEM_U32(lr + 0x00);
        if (rc == 0 || rc > 35 || ichain == 0) {
            fprintf(stderr, "[WEB-BIND-BBS] bitpos %d: unusable (reg=%d)\n", bp, rc);
            continue;
        }
        for (j = 0; j < ido52_web_bind_entries[i].nbb; j++) {
            int bb = ido52_web_bind_entries[i].bbs[j];
            uint32_t nd = (bb >= 0 && bb < 4096) ? node_by_bb[bb] : 0;
            uint32_t slot;
            if (nd == 0) {
                fprintf(stderr, "[WEB-BIND-BBS] bitpos %d: bb %d not found\n", bp, bb);
                continue;
            }
            slot = nd + 0x44 + (uint32_t)(rc - 1) * 4;
            if (MEM_U32(slot) != 0 && MEM_U32(slot) != ichain) {
                fprintf(stderr, "[WEB-BIND-BBS] bitpos %d: bb %d color %d OCCUPIED, skip\n", bp, bb, rc);
                continue;
            }
            MEM_U32(slot) = ichain;
            { uint32_t bm = 1u << ((~(uint32_t)rc) & 31u);
              MEM_U32(nd + 0x34) = MEM_U32(nd + 0x34) | bm; }
            fprintf(stderr, "[WEB-BIND-BBS] bitpos %d bound to bb %d (reg color %d)\n", bp, bb, rc);
        }
    }
}

/*
 * ido52_exclude_regs_post_srp: After f_opt_saved_regs, clear save flags
 * for excluded registers in the register save table, and update web
 * assignments for any web still assigned to an excluded register.
 * Enabled by IDO52_EXCLUDE_REGS env var.
 */
static void ido52_exclude_regs_post_srp(uint8_t *mem) {
    if (ido52_exclude_regs_mask_lo == 0) return;
    {
        uint32_t regtable_ptr = MEM_U32(0x1001e540);
        if (regtable_ptr == 0) return;
        uint32_t r;
        for (r = 0; r < 32; r++) {
            if (ido52_exclude_regs_mask_lo & (1u << r)) {
                uint32_t entry = regtable_ptr + r * 12;
                uint8_t old_flag = MEM_U8(entry + 8);
                if (old_flag != 0) {
                    MEM_U8(entry + 8) = 0;
                    fprintf(stderr, "[EXCL-SRP] reg %u save_flag %u->0\n", r, old_flag);
                }
            }
        }
    }
    {
        uint32_t web = MEM_U32(0x1001c898);
        uint32_t changed = 0;
        while (web != 0) {
            uint8_t reg = MEM_U8(web + 0x20);
            if (reg < 32 && (ido52_exclude_regs_mask_lo & (1u << reg))) {
                fprintf(stderr, "[EXCL-SRP] web 0x%08x reg=%u->0\n", web, reg);
                MEM_U8(web + 0x20) = 0;
                changed++;
            }
            web = MEM_U32(web + 12);
        }
        if (changed > 0) fprintf(stderr, "[EXCL-SRP] %u webs demoted from excluded regs\n", changed);
    }
}

'''


def find_line(lines, pattern, start=0, end=None):
    """Find first line containing pattern. Returns index or -1."""
    if end is None:
        end = len(lines)
    for i in range(start, end):
        if pattern in lines[i]:
            return i
    return -1


FORCE_WEB_COLOR_PHASE1 = '/* IDO52_FORCE_WEB_COLOR (Hook 37) phase 1: rewrite the picked colour before\n * the assigned_reg store and the colour-mask derivation below. */\nif (ido52_gccolor_trace || ido52_force_web_color_count) {\n    int _fc37 = ido52_gccolor_event(mem, 1, s5, (int)MEM_U32(sp + 220), (int)s6);\n    if (_fc37 >= 0) { MEM_U32(sp + 220) = (uint32_t)_fc37; t8 = (uint32_t)_fc37; }\n}\n'

FORCE_WEB_COLOR_PHASE2 = '/* IDO52_FORCE_WEB_COLOR (Hook 37) phase 2: same rewrite on the leftover pass. */\nif (ido52_gccolor_trace || ido52_force_web_color_count) {\n    int _fc37 = ido52_gccolor_event(mem, 2, s5, (int)MEM_U32(sp + 220), (int)s6);\n    if (_fc37 >= 0) { MEM_U32(sp + 220) = (uint32_t)_fc37; t0 = (uint32_t)_fc37; }\n}\n'

FORCE_WEB_COLOR_NOSPLIT = '/* IDO52_FORCE_WEB_COLOR_NOSPLIT (Hook 37): keep this web whole. */\nif (ido52_force_web_nosplit_count && ido52_gc_nosplit(mem, s5)) {\n    goto L46efe8;\n}\n'
FORCE_WEB_COLOR_NOSPLIT += '/* IDO52_FORCE_WEB_RANGE_SPLIT (Hook 43): force this web down f_split. */\nif (ido52_force_web_rsplit_count && ido52_gc_rsplit(mem, s5)) {\n    MEM_U32(sp + 276) = s5;\n    a0 = fp;\n    a1 = sp + 0x118;\n    a2 = s6;\n    a3 = 0x1;\n    f_split(mem, sp, a0, a1, a2, a3);\n    goto L46efa4;\n}\n'

FORCE_WEB_COLOR_SPILLTRACE = 'if (ido52_gccolor_trace) {\n    fprintf(stderr, "[GC-COLOR] phase=1 bitpos=%d lr=0x%08x SPILLED (no colour)\\n",\n            ido52_gc_bitpos_of_lr(mem, s5), s5);\n}\n'

FORCE_WEB_COLOR_SPLITTRACE = 'if (ido52_gccolor_trace) {\n    fprintf(stderr, "[GC-COLOR] phase=1 bitpos=%d lr=0x%08x SPLIT\\n",\n            ido52_gc_bitpos_of_lr(mem, s5), s5);\n}\n'


def find_line_exact(lines, pattern, start=0, end=None):
    """Find first line where stripped content equals pattern. Returns index or -1."""
    if end is None:
        end = len(lines)
    for i in range(start, end):
        if lines[i].strip() == pattern:
            return i
    return -1


def inject_after(lines, idx, code, results, name):
    """Insert code lines after lines[idx]. Returns new lines list."""
    code_lines = code.rstrip().split('\n')
    new_lines = lines[:idx+1] + code_lines + lines[idx+1:]
    results.append((name, "OK", idx + 1))
    return new_lines, len(code_lines)


def inject_before(lines, idx, code, results, name):
    """Insert code lines before lines[idx]. Returns new lines list."""
    code_lines = code.rstrip().split('\n')
    new_lines = lines[:idx] + code_lines + lines[idx:]
    results.append((name, "OK", idx))
    return new_lines, len(code_lines)


def replace_line(lines, idx, old_content, new_content, results, name):
    """Replace a single line, verifying it matches old_content."""
    if old_content not in lines[idx]:
        results.append((name, f"FAIL: line {idx+1} does not contain '{old_content}'", idx))
        return lines, 0
    lines[idx] = lines[idx].replace(old_content, new_content)
    results.append((name, "OK", idx + 1))
    return lines, 0


def apply_hooks(path):
    with open(path, "r") as f:
        content = f.read()

    if "ido52_init_hooks" in content:
        print(f"[uopt hooks] Already applied to {path}")
        return True

    lines = content.split("\n")
    results = []
    offset = 0  # cumulative line offset from insertions

    # ====================================================================
    # 1. Insert hook header after #include "header.h"
    # ====================================================================
    idx = find_line(lines, '#include "header.h"')
    if idx < 0:
        print("[uopt hooks] ERROR: Could not find '#include \"header.h\"'")
        return False

    lines, delta = inject_after(lines, idx, HOOK_HEADER, results, "HEADER")
    offset += delta

    # The debug-only memory probe used by the web dump needs sigaction /
    # sigsetjmp, which -std=c11 hides.  The feature-test macro has to precede
    # every #include, so it goes in front of "header.h" itself.
    lines.insert(idx, POSIX_FEATURE_TEST)
    results.append(("POSIX_FEATURE_TEST", "OK", idx + 1))
    offset += 1

    # ====================================================================
    # 2. INIT_HOOKS call in driver -- before f_tail_recursion call
    #    Pattern: "L45aaf0:" label followed by f_tail_recursion call
    # ====================================================================
    idx = find_line_exact(lines, "L45aaf0:", offset)
    if idx < 0:
        # Fallback: search for f_tail_recursion call
        idx = find_line(lines, "f_tail_recursion(mem, sp);", offset)
        if idx >= 0:
            lines, delta = inject_before(lines, idx, "ido52_init_hooks();", results, "INIT_HOOKS")
            offset += delta
        else:
            results.append(("INIT_HOOKS", "FAIL: could not find L45aaf0 or f_tail_recursion", -1))
    else:
        # Insert right after the label
        lines, delta = inject_after(lines, idx, "ido52_init_hooks();", results, "INIT_HOOKS")
        offset += delta

    # ====================================================================
    # 3. SKIP_PASSES: f_findinduct guard
    #    Pattern: bare "f_findinduct(mem, sp);" call in driver
    # ====================================================================
    idx = find_line(lines, "f_findinduct(mem, sp);", offset)
    if idx >= 0:
        old = "f_findinduct(mem, sp);"
        new = ("if (!(ido52_skip_passes_mask & IDO52_SKIP_FINDINDUCT)) {\n"
               "f_findinduct(mem, sp);\n"
               "}")
        lines[idx] = lines[idx].replace(old, new)
        results.append(("SKIP_FINDINDUCT", "OK", idx + 1))
    else:
        results.append(("SKIP_FINDINDUCT", "FAIL: could not find f_findinduct call in driver", -1))

    # ====================================================================
    # 4. SKIP_PASSES: f_codemotion guard with bitvector stub
    #    Pattern: bare "f_codemotion(mem, sp);" call in driver
    # ====================================================================
    idx = find_line(lines, "f_codemotion(mem, sp);", offset)
    if idx >= 0:
        old = "f_codemotion(mem, sp);"
        new = (
            "if (ido52_skip_passes_mask & IDO52_SKIP_CODEMOTION) {\n"
            "/* Stub: skip LICM pass while initializing the data structures that\n"
            " * f_getexpsources reads downstream. Without this, error 411 crash.\n"
            " * Phase 1: Clear global expression bitvectors */\n"
            "f_bvectminus(mem, sp, 0x1001caf8, 0x1001cb18);\n"
            "f_bvectminus(mem, sp, 0x1001cb10, 0x1001cb20);\n"
            "/* Phase 2: Initialize 0x1001cb50 (\"moved expressions\" bitvector) */\n"
            "MEM_U32(0x1001cb50 + 0) = 0;\n"
            "MEM_U32(0x1001cb50 + 4) = 0;\n"
            "f_checkbvlist(mem, sp, 0x1001cb50);\n"
            "/* Phase 3: Per-block CM-available bitvector at +0x124 */\n"
            "{\n"
            "    uint32_t _blk = MEM_U32(0x1001c898);\n"
            "    while (_blk != 0) {\n"
            "        uint32_t _bv124 = _blk + 0x124;\n"
            "        MEM_U32(_bv124 + 0) = 0;\n"
            "        MEM_U32(_bv124 + 4) = 0;\n"
            "        f_checkbvlist(mem, sp, _bv124);\n"
            "        _blk = MEM_U32(_blk + 12);\n"
            "    }\n"
            "}\n"
            "} else {\n"
            "f_codemotion(mem, sp);\n"
            "}"
        )
        lines[idx] = lines[idx].replace("f_codemotion(mem, sp);", new)
        results.append(("SKIP_CODEMOTION", "OK", idx + 1))
    else:
        results.append(("SKIP_CODEMOTION", "FAIL: could not find f_codemotion call in driver", -1))

    # ====================================================================
    # 5. SKIP_PASSES: f_eliminduct guard
    #    Pattern: bare "f_eliminduct(mem, sp, a0, a1, a2);" in driver
    # ====================================================================
    # Must be in the driver function area (after f_codemotion, around line 126700+)
    start = find_line(lines, "f_codemotion(mem, sp);", offset)
    if start < 0:
        start = offset
    idx = find_line(lines, "f_eliminduct(mem, sp, a0, a1, a2);", start)
    if idx >= 0:
        old = "f_eliminduct(mem, sp, a0, a1, a2);"
        new = ("if (!(ido52_skip_passes_mask & IDO52_SKIP_ELIMINDUCT)) {\n"
               "f_eliminduct(mem, sp, a0, a1, a2);\n"
               "}")
        lines[idx] = lines[idx].replace(old, new)
        results.append(("SKIP_ELIMINDUCT", "OK", idx + 1))
    else:
        results.append(("SKIP_ELIMINDUCT", "FAIL: could not find f_eliminduct call in driver", -1))

    # ====================================================================
    # 5b. SKIP_PASSES: the four tokens that used to be parsed but never
    #     wired (latent bug found during the Hook 20 build, 2026-07-26).
    #
    #     Each of these passes is invoked from the uopt driver exactly once,
    #     as a bare `f_NAME(mem, sp);` statement followed by a `goto L...;`
    #     to the state-reload block.  The forward declaration and the
    #     definition both spell the parameter list `(uint8_t *mem, uint32_t
    #     sp)`, so the bare-call pattern is unique to the driver site and no
    #     anchoring is needed.  Guarding the call alone leaves the `goto`
    #     and the reload block untouched -- the same shape as the already
    #     wired findinduct/eliminduct guards.
    #
    #     Unlike f_codemotion, none of these four publishes a global data
    #     structure that a later pass reads unconditionally (codemotion owns
    #     the CM-available bitvectors, hence its stub), so a plain guard is
    #     sufficient.  All are default-off: with IDO52_SKIP_PASSES unset the
    #     mask is 0 and every call runs exactly as before.
    # ====================================================================
    for token_name, call_stmt, flag in (
        ("SKIP_TAIL_RECURSION", "f_tail_recursion(mem, sp);", "IDO52_SKIP_TAIL_RECURSION"),
        ("SKIP_LOOPUNROLL", "f_loopunroll(mem, sp);", "IDO52_SKIP_LOOPUNROLL"),
        ("SKIP_COPYPROPAGATE", "f_copypropagate(mem, sp);", "IDO52_SKIP_COPYPROPAGATE"),
        ("SKIP_OPT_SAVED_REGS", "f_opt_saved_regs(mem, sp);", "IDO52_SKIP_OPT_SAVED_REGS"),
    ):
        idx = find_line(lines, call_stmt, offset)
        if idx >= 0:
            new = ("if (!(ido52_skip_passes_mask & %s)) {\n"
                   "%s\n"
                   "}" % (flag, call_stmt))
            lines[idx] = lines[idx].replace(call_stmt, new)
            results.append((token_name, "OK", idx + 1))
        else:
            results.append((token_name,
                            "FAIL: could not find %s call in driver" % call_stmt, -1))

    # ====================================================================
    # 6. NO_FINDINDUCT_STRIDE: skip func_431cfc call in f_findinduct
    #    Pattern: "v0 = func_431cfc(mem, sp, v0, a0);" near goto L4337a8
    # ====================================================================
    idx = find_line(lines, "v0 = func_431cfc(mem, sp, v0, a0);")
    if idx >= 0:
        old = "v0 = func_431cfc(mem, sp, v0, a0);"
        new = ("/* IDO52_NO_FINDINDUCT_STRIDE hook: skip stride creation */\n"
               "if (ido52_no_findinduct_stride) {\n"
               "    v0 = 0;\n"
               "} else {\n"
               "    v0 = func_431cfc(mem, sp, v0, a0);\n"
               "}")
        lines[idx] = lines[idx].replace(old, new)
        results.append(("NO_FINDINDUCT_STRIDE", "OK", idx + 1))
    else:
        results.append(("NO_FINDINDUCT_STRIDE", "FAIL: could not find func_431cfc call", -1))

    # ====================================================================
    # 7. CSE_MAX_REUSE: inject at L447124 in f_isearchloop
    #    Pattern: "L447124:" label, followed by "v1 = MEM_U8(s1 + 0);"
    # ====================================================================
    idx = find_line_exact(lines, "L447124:")
    if idx >= 0:
        cse_hook = (
            "/* IDO52_CSE_MAX_REUSE hook: limit expression reuse count */\n"
            "if (ido52_cse_max_reuse > 0 && (int32_t)t0 != 0 && s0 != 0) {\n"
            "    uint32_t _hash = (s0 >> 2) & (CSE_TRACK_SIZE - 1);\n"
            "    uint32_t _probe;\n"
            "    int _found = 0;\n"
            "    for (_probe = 0; _probe < 16; _probe++) {\n"
            "        uint32_t _idx = (_hash + _probe) & (CSE_TRACK_SIZE - 1);\n"
            "        if (ido52_cse_track[_idx].ichain_ptr == s0) {\n"
            "            ido52_cse_track[_idx].count++;\n"
            "            if (ido52_cse_track[_idx].count >= ido52_cse_max_reuse) {\n"
            "                t0 = 0; s0 = 0; /* force miss */\n"
            "            }\n"
            "            _found = 1;\n"
            "            break;\n"
            "        }\n"
            "        if (ido52_cse_track[_idx].ichain_ptr == 0) {\n"
            "            ido52_cse_track[_idx].ichain_ptr = s0;\n"
            "            ido52_cse_track[_idx].count = 1;\n"
            "            _found = 1;\n"
            "            break;\n"
            "        }\n"
            "    }\n"
            "    if (!_found) {\n"
            "        /* table full at this probe chain, don't interfere */\n"
            "    }\n"
            "}"
        )
        lines, delta = inject_after(lines, idx, cse_hook, results, "CSE_MAX_REUSE")
        offset += delta
    else:
        results.append(("CSE_MAX_REUSE", "FAIL: could not find L447124 label", -1))

    # ====================================================================
    # 8. NO_CSE: inject in f_isearchloop after hash bucket head load
    #    Pattern: "s0 = MEM_U32(t9 + 0);" followed by "s1 = a1;" in f_isearchloop
    #    Must find the DEFINITION (not forward declaration) -- look for the
    #    one that has a function body (followed by variable declarations)
    # ====================================================================
    # Find f_isearchloop definition (the second occurrence, with the body)
    func_start = find_line(lines, "static uint32_t f_isearchloop(uint8_t *mem,")
    if func_start >= 0:
        # Skip forward declaration -- find the one with a body
        next_line = func_start + 1
        if next_line < len(lines) and "const uint32_t zero" not in lines[next_line]:
            func_start = find_line(lines, "static uint32_t f_isearchloop(uint8_t *mem,", func_start + 1)
    if func_start >= 0:
        # Find "s0 = MEM_U32(t9 + 0);" within the function (first occurrence after def)
        idx = find_line(lines, "s0 = MEM_U32(t9 + 0);", func_start)
        if idx >= 0:
            cse_no_cse_hook = (
                "/* IDO52_NO_CSE hook: skip hash search to force new ichain */\n"
                "if (ido52_cse_debug) {\n"
                "    fprintf(stderr, \"[CSE] type=%u opc=%u (0x%x) s0=%u\\n\",\n"
                "            MEM_U8(s1 + 0), MEM_U8(s1 + 32), MEM_U8(s1 + 32), s0);\n"
                "}\n"
                "if (ido52_no_cse_mode == IDO52_NO_CSE_ALL) {\n"
                "    s0 = 0;\n"
                "} else if (ido52_no_cse_mode == IDO52_NO_CSE_OP) {\n"
                "    uint8_t _etype = MEM_U8(s1 + 0);\n"
                "    if (_etype == 4 || _etype == 5 || _etype == 6) s0 = 0;\n"
                "} else if (ido52_no_cse_mode == IDO52_NO_CSE_OPCODES) {\n"
                "    uint8_t _etype = MEM_U8(s1 + 0);\n"
                "    if (_etype == 4 || _etype == 5) {\n"
                "        uint8_t _opc = MEM_U8(s1 + 32);\n"
                "        int _oi;\n"
                "        for (_oi = 0; _oi < ido52_no_cse_opcode_count; _oi++) {\n"
                "            if (_opc == ido52_no_cse_opcode_list[_oi]) { s0 = 0; break; }\n"
                "        }\n"
                "    }\n"
                "}"
            )
            # Insert after s1 = a1 line (two lines after s0 = MEM_U32)
            s1_idx = find_line(lines, "s1 = a1;", idx)
            if s1_idx and s1_idx < idx + 5:
                lines, delta = inject_after(lines, s1_idx, cse_no_cse_hook, results, "NO_CSE")
                offset += delta
            else:
                lines, delta = inject_after(lines, idx, cse_no_cse_hook, results, "NO_CSE")
                offset += delta
        else:
            results.append(("NO_CSE", "FAIL: could not find 's0 = MEM_U32(t9 + 0)' in f_isearchloop", -1))
    else:
        results.append(("NO_CSE", "FAIL: could not find f_isearchloop definition", -1))

    # ====================================================================
    # 9. CONST_NOINREG: inject at f_constinreg entry
    #    Pattern: "MEM_U32(sp + 48) = a2;" in f_constinreg, before decision logic
    #    Must find the DEFINITION not the forward declaration
    # ====================================================================
    func_start = find_line(lines, "static uint32_t f_constinreg(uint8_t *mem,")
    if func_start >= 0:
        # Skip forward declaration
        next_line = func_start + 1
        if next_line < len(lines) and "const uint32_t zero" not in lines[next_line]:
            func_start = find_line(lines, "static uint32_t f_constinreg(uint8_t *mem,", func_start + 1)
    if func_start >= 0:
        idx = find_line(lines, "MEM_U32(sp + 48) = a2;", func_start)
        if idx >= 0:
            const_hook = (
                "/* IDO52_CONST_NOINREG_VALUES hook: block specific constants from register\n"
                "   promotion. Occurrence-window scoped (VALUE[:FROM[:TO]], 1-based over\n"
                "   f_constinreg queries of VALUE) for regional const-web control. */\n"
                "if (ido52_const_noinreg_count > 0) {\n"
                "    int _ci;\n"
                "    int _block = 0;\n"
                "    int _matchedn = 0;\n"
                "    for (_ci = 0; _ci < ido52_const_noinreg_count; _ci++) {\n"
                "        if (a2 == ido52_const_noinreg_values[_ci]) {\n"
                "            int _n = ++ido52_const_noinreg_seen[_ci];\n"
                "            _matchedn = _n;\n"
                "            if (_n >= ido52_const_noinreg_from[_ci] && _n <= ido52_const_noinreg_to[_ci]) _block = 1;\n"
                "        }\n"
                "    }\n"
                "    if (_matchedn && getenv(\"IDO52_CONST_INREG_DEBUG\")) {\n"
                "        fprintf(stderr, \"[CONST-NOINREG] val=%u n=%d a0=0x%08x block=%d\\n\",\n"
                "                a2, _matchedn, a0, _block);\n"
                "    }\n"
                "    if (_block) {\n"
                "        MEM_U8(sp + 39) = (uint8_t)zero;\n"
                "        goto L4853ac;\n"
                "    }\n"
                "}\n"
                "/* IDO52_CONST_INREG_VALUES hook: force specific constants into register promotion.\n"
                "   Occurrence-window scoped (FROM..TO-th query of VALUE) for regional const-web\n"
                "   control; entries with an impossible window (e.g. :0:0) trace without forcing. */\n"
                "if (ido52_const_inreg_count > 0) {\n"
                "    int _ci;\n"
                "    int _force = 0;\n"
                "    int _matched = 0;\n"
                "    int _nprint = 0;\n"
                "    for (_ci = 0; _ci < ido52_const_inreg_count; _ci++) {\n"
                "        if (a2 == ido52_const_inreg_values[_ci]) {\n"
                "            int _n = ++ido52_const_inreg_seen[_ci];\n"
                "            int _tok = (ido52_const_inreg_types[_ci] < 0 || (uint32_t)ido52_const_inreg_types[_ci] == a0);\n"
                "            int _rok = (_n >= ido52_const_inreg_from[_ci] && _n <= ido52_const_inreg_to[_ci]);\n"
                "            _matched = 1;\n"
                "            _nprint = _n;\n"
                "            if (_tok && _rok) _force = 1;\n"
                "        }\n"
                "    }\n"
                "    if (_matched) {\n"
                "        if (getenv(\"IDO52_CONST_INREG_DEBUG\")) {\n"
                "            fprintf(stderr, \"[CONST-INREG] val=%u n=%d a0=0x%08x a1=%u a3=0x%08x force=%d\\n\",\n"
                "                    a2, _nprint, a0, a1, a3, _force);\n"
                "        }\n"
                "        if (_force) {\n"
                "            MEM_U8(sp + 39) = (uint8_t)1;\n"
                "            goto L4853ac;\n"
                "        }\n"
                "    }\n"
                "}"
            )
            lines, delta = inject_after(lines, idx, const_hook, results, "CONST_NOINREG")
            offset += delta
        else:
            results.append(("CONST_NOINREG", "FAIL: could not find 'MEM_U32(sp + 48) = a2' in f_constinreg", -1))
    else:
        results.append(("CONST_NOINREG", "FAIL: could not find f_constinreg definition", -1))

    # ====================================================================
    # 10. LDA_NOINREG: inject at f_ldainreg entry
    #     Pattern: "t6 = 0x1001eabc;" in f_ldainreg, before decision logic
    #     Must find the DEFINITION not the forward declaration
    # ====================================================================
    func_start = find_line(lines, "static uint32_t f_ldainreg(uint8_t *mem,")
    if func_start >= 0:
        next_line = func_start + 1
        if next_line < len(lines) and "const uint32_t zero" not in lines[next_line]:
            func_start = find_line(lines, "static uint32_t f_ldainreg(uint8_t *mem,", func_start + 1)
    if func_start >= 0:
        idx = find_line(lines, "t6 = 0x1001eabc;", func_start)
        if idx >= 0:
            lda_hook = (
                "/* IDO52_LDA_NOINREG_CLASSES hook: block specific classes from register\n"
                "   promotion.  Occurrence-window scoped (CLASS[:FROM[:TO]], 1-based over\n"
                "   f_ldainreg queries of CLASS) so a SINGLE address load can be released\n"
                "   back to rematerialization without de-registering the whole class. */\n"
                "if (ido52_lda_noinreg_count > 0) {\n"
                "    int _li;\n"
                "    int _lblock = 0;\n"
                "    int _lmatchedn = 0;\n"
                "    for (_li = 0; _li < ido52_lda_noinreg_count; _li++) {\n"
                "        if (a0 == ido52_lda_noinreg_classes[_li]) {\n"
                "            int _ln = ++ido52_lda_noinreg_seen[_li];\n"
                "            _lmatchedn = _ln;\n"
                "            if (_ln >= ido52_lda_noinreg_from[_li] && _ln <= ido52_lda_noinreg_to[_li]) _lblock = 1;\n"
                "        }\n"
                "    }\n"
                "    if (_lmatchedn && ido52_lda_noinreg_debug) {\n"
                "        fprintf(stderr, \"[LDA-NOINREG] class=%u n=%d a1=0x%08x a2=0x%08x block=%d\\n\",\n"
                "                a0, _lmatchedn, a1, a2, _lblock);\n"
                "    }\n"
                "    if (_lblock) {\n"
                "        v0 = 0;\n"
                "        return v0;\n"
                "    }\n"
                "}"
            )
            lines, delta = inject_before(lines, idx, lda_hook, results, "LDA_NOINREG")
            offset += delta
        else:
            results.append(("LDA_NOINREG", "FAIL: could not find 't6 = 0x1001eabc' in f_ldainreg", -1))
    else:
        results.append(("LDA_NOINREG", "FAIL: could not find f_ldainreg definition", -1))

    # ====================================================================
    # 11. RESERVE_SREGS / MAX_SREGS: inject at f_procinit_regs exit
    #     Pattern: "MEM_U32(a1 + 4) = t9;" followed by "return;" -- the
    #     last write to dftregsused high word before function returns
    # ====================================================================
    func_start = find_line(lines, "static void f_procinit_regs(uint8_t *mem,")
    if func_start >= 0:
        idx = find_line(lines, "MEM_U32(a1 + 4) = t9;", func_start)
        if idx >= 0:
            sregs_hook = (
                "/* IDO52_RESERVE_SREGS / IDO52_MAX_SREGS hook: OR mask into dftregsused high word */\n"
                "if (ido52_reserve_sregs_mask_hi) {\n"
                "    t9 = t9 | ido52_reserve_sregs_mask_hi;\n"
                "    MEM_U32(a1 + 4) = t9;\n"
                "}"
            )
            lines, delta = inject_after(lines, idx, sregs_hook, results, "RESERVE_SREGS")
            offset += delta
        else:
            results.append(("RESERVE_SREGS", "FAIL: could not find 'MEM_U32(a1 + 4) = t9' in f_procinit_regs", -1))
    else:
        results.append(("RESERVE_SREGS", "FAIL: could not find f_procinit_regs definition", -1))

    # ====================================================================
    # 12. NO_DEADSTORE_GLOBALS site 1: f_copypropagate
    #     Pattern: After f_bvectin returns nonzero, before "MEM_U8(t9 + 9) = (uint8_t)zero;"
    #     The dead store check: "v0 = f_bvectin(...)" then "if v0 == 0 goto L419580"
    #     We inject after the "goto L419580;}" to protect before the elimination
    #     Must scope to f_copypropagate (not other functions with same pattern)
    # ====================================================================
    cp_start = find_line(lines, "static void f_copypropagate(uint8_t *mem,")
    if cp_start >= 0:
        next_cp = cp_start + 1
        if next_cp < len(lines) and "const uint32_t zero" not in lines[next_cp]:
            cp_start = find_line(lines, "static void f_copypropagate(uint8_t *mem,", cp_start + 1)
    # Find the dead store marker within f_copypropagate
    # It follows the f_bvectin check and goto L419580 pattern
    idx = -1
    if cp_start >= 0:
        # Search for "MEM_U8(t9 + 9) = (uint8_t)zero;" within f_copypropagate
        # There should be one near "goto L419580;"
        search_start = find_line(lines, "goto L419580;", cp_start)
        if search_start >= 0:
            idx = find_line(lines, "MEM_U8(t9 + 9) = (uint8_t)zero;", search_start)
    if idx >= 0:
        # Look backwards for the pattern "goto L419580;}" to find the right spot
        # The hook goes right before the elimination code
        deadstore1_hook = (
            "/* IDO52_NO_DEADSTORE_GLOBALS hook (site 1: f_copypropagate) */\n"
            "if (ido52_is_protected_deadstore(mem, MEM_U32(s2 + 4))) {\n"
            "    goto L419580;\n"
            "}"
        )
        lines, delta = inject_before(lines, idx, deadstore1_hook, results, "NO_DEADSTORE_COPYPROPAGATE")
        offset += delta
    else:
        results.append(("NO_DEADSTORE_COPYPROPAGATE", "FAIL: could not find 'MEM_U8(t9 + 9) = (uint8_t)zero' pattern", -1))

    # ====================================================================
    # 13. NO_DEADSTORE_GLOBALS site 2: f_eliminduct
    #     Pattern: Similar dead store elimination in f_eliminduct
    #     After "goto L481694;}" check, before "MEM_U8(t1 + 9) = (uint8_t)zero;"
    #     Must scope to f_eliminduct area (near L481694 label)
    # ====================================================================
    # Find "MEM_U8(t1 + 9) = (uint8_t)zero;" near "goto L481694;" in f_eliminduct
    el_start = find_line(lines, "goto L481694;")
    idx = -1
    if el_start >= 0:
        idx = find_line(lines, "MEM_U8(t1 + 9) = (uint8_t)zero;", el_start)
    if idx >= 0:
        deadstore2_hook = (
            "/* IDO52_NO_DEADSTORE_GLOBALS hook (site 2: f_eliminduct) */\n"
            "if (ido52_is_protected_deadstore(mem, MEM_U32(s0 + 4))) {\n"
            "    goto L481694;\n"
            "}"
        )
        lines, delta = inject_before(lines, idx, deadstore2_hook, results, "NO_DEADSTORE_ELIMINDUCT")
        offset += delta
    else:
        results.append(("NO_DEADSTORE_ELIMINDUCT", "FAIL: could not find 'MEM_U8(t1 + 9) = (uint8_t)zero' pattern", -1))

    # ====================================================================
    # 14. NO_LICM: inject at func_47efd4 entry (LICM hoist-marking function)
    #     Pattern: "static void func_47efd4" definition, inject before prologue
    # ====================================================================
    func_start = find_line(lines, "static void func_47efd4(uint8_t *mem,")
    if func_start >= 0:
        # Find the label line (e.g. L47efd4:)
        label_idx = find_line(lines, "L47efd4:", func_start)
        if label_idx >= 0:
            # Find the prologue "sp = sp + 0xffffffe0" after the label
            prologue_idx = find_line(lines, "sp = sp + 0xffffffe0", label_idx)
            if prologue_idx and prologue_idx < label_idx + 10:
                licm_hook = (
                    "/* IDO52_NO_LICM hook: skip LICM hoist marking */\n"
                    "if (ido52_no_licm == 1) {\n"
                    "    return;\n"
                    "}\n"
                    "if (ido52_no_licm == 2) {\n"
                    "    /* Check if this is an LDA expression with a protected address */\n"
                    "    uint8_t _ltype = MEM_U8(a0 + 0);\n"
                    "    if (_ltype == 3) { /* LDA type */\n"
                    "        uint32_t _laddr = MEM_U32(a0 + 0x28);\n"
                    "        int _lk;\n"
                    "        for (_lk = 0; _lk < ido52_no_licm_addr_count; _lk++) {\n"
                    "            if (_laddr == ido52_no_licm_addrs[_lk]) {\n"
                    "                return;\n"
                    "            }\n"
                    "        }\n"
                    "    }\n"
                    "}"
                )
                lines, delta = inject_before(lines, prologue_idx, licm_hook, results, "NO_LICM")
                offset += delta
            else:
                results.append(("NO_LICM", "FAIL: could not find prologue after L47efd4", -1))
        else:
            results.append(("NO_LICM", "FAIL: could not find L47efd4 label", -1))
    else:
        results.append(("NO_LICM", "FAIL: could not find func_47efd4 definition", -1))

    # ====================================================================
    # 15. NO_STRENGTH_REDUCE: inject before func_47fbbc call in f_eliminduct
    #     Pattern: "func_47fbbc(mem, sp, a0, a1);" in f_eliminduct (NOT the
    #     one inside func_47fbbc's own definition)
    #     The call near L481000 is the one in f_eliminduct's bitvector loop
    # ====================================================================
    # Find the func_47fbbc call that is followed by "goto L481000;"
    idx = find_line(lines, "func_47fbbc(mem, sp, a0, a1);")
    # There are multiple calls -- find the one near L481000
    while idx >= 0:
        # Check if L481000 is within next 5 lines
        for check in range(idx + 1, min(idx + 5, len(lines))):
            if "L481000:" in lines[check]:
                # This is the right one (in f_eliminduct)
                sr_hook = (
                    "/* IDO52_NO_STRENGTH_REDUCE hook: skip strength reduction */\n"
                    "if (ido52_no_strength_reduce) {\n"
                    "    if (ido52_strength_reduce_debug) {\n"
                    "        fprintf(stderr, \"[SR] skipping func_47fbbc(0x%08x, 0x%08x)\\n\", a0, a1);\n"
                    "    }\n"
                    "    goto L481000;\n"
                    "}"
                )
                lines, delta = inject_before(lines, idx, sr_hook, results, "NO_STRENGTH_REDUCE")
                offset += delta
                idx = -2  # sentinel: found
                break
        if idx == -2:
            break
        idx = find_line(lines, "func_47fbbc(mem, sp, a0, a1);", idx + 1)

    if idx != -2:
        results.append(("NO_STRENGTH_REDUCE", "FAIL: could not find func_47fbbc call near L481000", -1))

    # ====================================================================
    # 15b. NO_LOOP_TEST_REPLACEMENT: guard the two EQ_INEQ stores in
    #      f_constarith that flip a loop's relational exit test to an
    #      equality test (relation field +5 := 2).
    #
    #      Site 1 (before label L459150):
    #          MEM_U8(a1 + 5) = (uint8_t)t5;
    #          goto L459184;
    #          MEM_U8(a1 + 5) = (uint8_t)t5;   <- dead delay-slot copy
    #      Site 2 (immediately before label L459184):
    #          MEM_U8(a1 + 5) = (uint8_t)t7;
    #
    #      The basic-block number is MEM_U16(MEM_U32(sp + 104) + 8) -- the
    #      same value the original `EQ_INEQ at BB:` listing line prints.
    # ====================================================================
    ltr_bb = "(uint32_t)MEM_U16(MEM_U32(sp + 104) + 8)"
    ltr_ok = 0

    idx = find_line_exact(lines, "L459150:")
    if idx >= 3 and lines[idx - 3].strip() == "MEM_U8(a1 + 5) = (uint8_t)t5;" \
            and lines[idx - 2].strip() == "goto L459184;":
        lines[idx - 3] = (
            "/* IDO52_NO_LOOP_TEST_REPLACEMENT hook (EQ_INEQ site 1) */\n"
            "if (!ido52_ltr_suppress(" + ltr_bb + ")) MEM_U8(a1 + 5) = (uint8_t)t5;"
        )
        results.append(("NO_LOOP_TEST_REPLACEMENT_1", "OK", idx - 2))
        ltr_ok += 1
    else:
        results.append(("NO_LOOP_TEST_REPLACEMENT_1",
                        "FAIL: could not find EQ_INEQ store before L459150", -1))

    idx = find_line_exact(lines, "L459184:")
    if idx >= 1 and lines[idx - 1].strip() == "MEM_U8(a1 + 5) = (uint8_t)t7;":
        lines[idx - 1] = (
            "/* IDO52_NO_LOOP_TEST_REPLACEMENT hook (EQ_INEQ site 2) */\n"
            "if (!ido52_ltr_suppress(" + ltr_bb + ")) MEM_U8(a1 + 5) = (uint8_t)t7;"
        )
        results.append(("NO_LOOP_TEST_REPLACEMENT_2", "OK", idx))
        ltr_ok += 1
    else:
        results.append(("NO_LOOP_TEST_REPLACEMENT_2",
                        "FAIL: could not find EQ_INEQ store before L459184", -1))

    # ====================================================================
    # 15c. NO_LOOP_TEST_REPLACEMENT: the DOMINANT site -- f_loopunroll's
    #      exit-test normalization.  Before unrolling, f_loopunroll rewrites
    #      a countable loop's relational test into an equality test via
    #      f_change_to_const_eq / f_change_to_var_eq (then deletes the
    #      original condition with f_del_orig_cond).  This runs even with
    #      `-Wo,-loopunroll,0`, and it is what turns the target's
    #      `sltu $at,p,lim; bne $at,$zero` into a single `bne p,lim`.
    #
    #      Suppression jumps past both the conversion and the matching
    #      f_del_orig_cond, straight to the state-reload block that all
    #      arms fall through to (L47745c for the const-eq arms, L477508
    #      for the var-eq arm), leaving the loop's original condition and
    #      the rest of the unroll pass untouched.
    # ====================================================================
    ce_hook = (
        "/* IDO52_NO_LOOP_TEST_REPLACEMENT hook (f_loopunroll const-eq) */\n"
        "if (ido52_ltr_suppress((uint32_t)MEM_U16(s2 + 8))) goto L47745c;"
    )
    ce_found = 0
    search = 0
    while True:
        idx = find_line(lines, "v0 = f_change_to_const_eq(mem, sp, a0, a1, a2, a3);", search)
        if idx < 0:
            break
        lines, delta = inject_before(lines, idx, ce_hook, results,
                                     "NO_LOOP_TEST_REPL_CONSTEQ_%d" % (ce_found + 1))
        offset += delta
        ce_found += 1
        search = idx + delta + 1
    if ce_found != 4:
        results.append(("NO_LOOP_TEST_REPL_CONSTEQ",
                        "WARN: expected 4 f_change_to_const_eq calls, found %d" % ce_found, -1))

    idx = find_line(lines, "v0 = f_change_to_var_eq(mem, sp, a0, a1, a2, a3);")
    if idx >= 0:
        ve_hook = (
            "/* IDO52_NO_LOOP_TEST_REPLACEMENT hook (f_loopunroll var-eq) */\n"
            "if (ido52_ltr_suppress((uint32_t)MEM_U16(s2 + 8))) goto L477508;"
        )
        lines, delta = inject_before(lines, idx, ve_hook, results, "NO_LOOP_TEST_REPL_VAREQ")
        offset += delta
    else:
        results.append(("NO_LOOP_TEST_REPL_VAREQ",
                        "FAIL: could not find f_change_to_var_eq call", -1))

    # ====================================================================
    # 16. PRECOLOR_LIVEIN: inject after f_makelivranges call (at L45b394)
    #     Pattern: "L45b394:" label after f_makelivranges
    # ====================================================================
    idx = find_line_exact(lines, "L45b394:")
    if idx >= 0:
        precolor_hook1 = (
            "/* === IDO52_PRECOLOR_LIVEIN hook === */\n"
            "ido52_apply_precolor_livein(mem, sp);\n"
            "/* === IDO52_PRECOLOR_WEB hook === */\n"
            "ido52_apply_precolor_web(mem, sp);"
        )
        lines, delta = inject_after(lines, idx, precolor_hook1, results, "PRECOLOR_POST_MAKELIVRANGES")
        offset += delta
    else:
        results.append(("PRECOLOR_POST_MAKELIVRANGES", "FAIL: could not find L45b394", -1))

    # ====================================================================
    # 17. PRECOLOR_LIVEIN: inject after f_globalcolor call (at L45b5d8)
    #     Pattern: "L45b5d8:" label after f_globalcolor
    # ====================================================================
    idx = find_line_exact(lines, "L45b5d8:")
    if idx >= 0:
        precolor_hook2 = (
            "/* === IDO52_PRECOLOR_LIVEIN post-globalcolor hook === */\n"
            "ido52_apply_precolor_post_globalcolor(mem);\n"
            "/* === IDO52_PRECOLOR_WEB post-globalcolor hook === */\n"
            "ido52_apply_precolor_web_post_gc(mem);"
        )
        lines, delta = inject_after(lines, idx, precolor_hook2, results, "PRECOLOR_POST_GLOBALCOLOR")
        offset += delta
    else:
        results.append(("PRECOLOR_POST_GLOBALCOLOR", "FAIL: could not find L45b5d8", -1))

    # ====================================================================
    # 17b. IDO52_UOPT_TRACE: env override of the original uopt trace level
    #      (0x1001ea98, zeroed in the init routine). Levels gate the ORIGINAL
    #      Pascal dump routines; they write to the listing file opened by the
    #      undocumented 'uopt -l <file>' flag (REQUIRED, else segfault):
    #      1=printitab/printtab, 2=+printlinfo, 3=+printcm (PRE bitvectors),
    #      4=printscm, 5=printregs (web/coloring dump incl. >>>active<<<
    #      entries with forbidden sets + live-bbs), 6=flow graph,
    #      7=printtab+printinterproc, 0x15=printhoist, 0x19=printprecm.
    # ====================================================================
    idx = find_line_exact(lines, "at = 0x1001ea98;")
    if idx >= 0 and lines[idx + 1].strip() == "t8 = 0x9;" and \
            lines[idx + 2].strip() == "MEM_U32(at + 0) = zero;":
        trace_hook = (
            "/* IDO52_UOPT_TRACE hook: set the original uopt trace level (gates\n"
            " * f_printitab/f_printlinfo/f_printcm/f_printregs dumps; needs -l) */\n"
            "{\n"
            "    const char *_tr = getenv(\"IDO52_UOPT_TRACE\");\n"
            "    if (_tr && *_tr) {\n"
            "        MEM_U32(at + 0) = (uint32_t)strtol(_tr, NULL, 0);\n"
            "        fprintf(stderr, \"[UOPT-TRACE] level %s -- requires a listing\"\n"
            "                \" file (cc -Wo,-l,<file>); WITHOUT it the original\"\n"
            "                \" dump routines write through a NULL FILE* and uopt\"\n"
            "                \" dies with SIGSEGV.\\n\", _tr);\n"
            "    }\n"
            "}"
        )
        lines, delta = inject_after(lines, idx + 2, trace_hook, results, "UOPT_TRACE")
        offset += delta
    else:
        results.append(("UOPT_TRACE", "FAIL: could not find trace-level init at 0x1001ea98", -1))

    # ====================================================================
    # 17c. IDO52_CONSTQ_TRACE: log per-use constant emission decisions in
    #      f_gen_outparcode (const value, lr, f_constinreg verdict, f_inreg
    #      verdict -> REGOP vs loadnum). Diagnostic only.
    # ====================================================================
    idx = find_line_exact(lines, "L422c88:")
    if idx >= 0:
        q1_hook = (
            "/* IDO52_CONSTQ_TRACE: log const emission decision (site Q1, f_gen_outparcode) */\n"
            "if (getenv(\"IDO52_CONSTQ_TRACE\")) {\n"
            "    uint32_t _lr = MEM_U32(s0 + 20);\n"
            "    fprintf(stderr, \"[Q1] node=%08x opc=%u val=%u lr=%08x uc=%u blk=%08x bb=%u cir=%u\",\n"
            "            s0, MEM_U8(s0 + 1), MEM_U32(s0 + 0x20), _lr, MEM_U32(s0 + 40),\n"
            "            MEM_U32(MEM_U32(sp + 124) + -12),\n"
            "            MEM_U16(MEM_U32(MEM_U32(sp + 124) + -12) + 8), v0);\n"
            "    if (_lr && getenv(\"IDO52_CONSTQ_LRDUMP\")) {\n"
            "        int _i;\n"
            "        fprintf(stderr, \"\\n  LR[\");\n"
            "        for (_i = 0; _i < 17; _i++) fprintf(stderr, \"%08x \", MEM_U32(_lr + 4*_i));\n"
            "        fprintf(stderr, \"]\");\n"
            "    }\n"
            "    if ((int32_t)v0 == (int32_t)0) fprintf(stderr, \" -> loadnum\\n\");\n"
            "}"
        )
        lines, delta = inject_after(lines, idx + 1, q1_hook, results, "CONSTQ_TRACE_Q1")
        offset += delta
        idx2 = find_line_exact(lines, "L422cac:")
        if idx2 >= 0:
            q1_hook2 = (
                "if (getenv(\"IDO52_CONSTQ_TRACE\")) {\n"
                "    fprintf(stderr, \" inreg=%u -> %s\\n\", v0, v0 ? \"REGOP\" : \"loadnum\");\n"
                "}"
            )
            lines, delta = inject_after(lines, idx2 + 1, q1_hook2, results, "CONSTQ_TRACE_Q1B")
            offset += delta
    else:
        results.append(("CONSTQ_TRACE_Q1", "FAIL: could not find L422c88", -1))

    # ====================================================================
    # 18. EXCLUDE_REGS: inject at L46e804 in f_globalcolor
    #     OR exclusion mask into per-web forbidden bitvector
    # ====================================================================
    idx = find_line_exact(lines, "L46e804:")
    if idx >= 0:
        exclude_regs_hook = (
            "/* IDO52_EXCLUDE_REGS hook: OR exclusion mask into per-web forbidden bitvector */\n"
            "if (ido52_exclude_regs_mask_lo) {\n"
            "    uint32_t _old_forbid = MEM_U32(s5 + 40);\n"
            "    MEM_U32(s5 + 40) = _old_forbid | ido52_exclude_regs_mask_lo;\n"
            "}"
            "/* IDO52_EXCLUDE_REGS_WEB (Hook 28): per-web forbidden additions, matched by\n"
            " * bittab liverange pointer so only the named web is touched. */\n"
            "if (ido52_exclregs_web_count) {\n"
            "    uint32_t _bt = MEM_U32(0x1001cbd0);\n"
            "    int _i;\n"
            "    if (_bt) {\n"
            "        for (_i = 0; _i < ido52_exclregs_web_count; _i++) {\n"
            "            uint32_t _lr = MEM_U32(_bt + (uint32_t)(ido52_exclregs_web[_i].bitpos * 8) + 4);\n"
            "            if (_lr != 0 && _lr == s5) {\n"
            "                uint32_t _of = MEM_U32(s5 + 40);\n"
            "                MEM_U32(s5 + 40) = _of | ido52_exclregs_web[_i].mask;\n"
            "                if (ido52_exclregs_web_debug) {\n"
            "                    fprintf(stderr, \"[EXCLREGS-WEB] bitpos %d: forbid 0x%08x -> 0x%08x\\n\",\n"
            "                            ido52_exclregs_web[_i].bitpos, _of, _of | ido52_exclregs_web[_i].mask);\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "}\n"
        )
        lines, delta = inject_after(lines, idx, exclude_regs_hook, results, "EXCLUDE_REGS")
        offset += delta
    else:
        results.append(("EXCLUDE_REGS", "FAIL: could not find L46e804", -1))

    # ====================================================================
    # 37. FORCE_WEB_COLOR (Hook 37): positive per-web colour assignment.
    #     Sites in f_globalcolor:
    #       L46f158  phase-1 (cost-driven) colour-pick epilogue
    #       L46fbc0  phase-2 (leftover-web) colour-pick epilogue
    #       L46ef64  phase-1 range-split test        -> NOSPLIT veto
    #       L46ea34  "web could not be coloured"     -> trace only
    #       first f_split() call                     -> trace only
    # ====================================================================
    for _label, _code, _tag in (
            ("L46f158:", FORCE_WEB_COLOR_PHASE1, "FORCE_WEB_COLOR_P1"),
            ("L46fbc0:", FORCE_WEB_COLOR_PHASE2, "FORCE_WEB_COLOR_P2"),
            ("L46ef64:", FORCE_WEB_COLOR_NOSPLIT, "FORCE_WEB_COLOR_NOSPLIT"),
            ("L46ea34:", FORCE_WEB_COLOR_SPILLTRACE, "FORCE_WEB_COLOR_SPILLTRACE"),
    ):
        idx = find_line_exact(lines, _label)
        if idx >= 0:
            lines, delta = inject_after(lines, idx, _code, results, _tag)
            offset += delta
        else:
            results.append((_tag, "FAIL: could not find " + _label, -1))
    idx = find_line(lines, "f_split(mem, sp, a0, a1, a2, a3);")
    if idx >= 0:
        lines, delta = inject_before(lines, idx, FORCE_WEB_COLOR_SPLITTRACE,
                                     results, "FORCE_WEB_COLOR_SPLITTRACE")
        offset += delta
    else:
        results.append(("FORCE_WEB_COLOR_SPLITTRACE",
                        "FAIL: could not find f_split call", -1))

    # ====================================================================
    # 18. NO_SREG_PROMOTE: skip f_opt_saved_regs entirely
    #     Pattern: "L47a0ec:" label at f_opt_saved_regs entry
    # ====================================================================
    label_idx = find_line(lines, "L47a0ec:")
    if label_idx >= 0:
        no_sreg_hook = (
            '/* IDO52_NO_SREG_PROMOTE hook: skip callee-save register promotion */\n'
            '{\n'
            '    char *env = getenv("IDO52_NO_SREG_PROMOTE");\n'
            '    if (env && env[0] == \'1\') {\n'
            '        return;\n'
            '    }\n'
            '}\n'
        )
        lines, delta = inject_after(lines, label_idx, no_sreg_hook, results, "NO_SREG_PROMOTE")
        offset += delta
    else:
        results.append(("NO_SREG_PROMOTE", "FAIL: could not find L47a0ec label", -1))

    # ====================================================================
    # 19. DUMP_WEBS: print web assignments after globalcolor
    #     Pattern: "ido52_apply_precolor_post_globalcolor(mem);" line
    # ====================================================================
    # ====================================================================
    # 21. CMOT_KILL_AVAIL: per-block codemotion availability break.
    #     Anchor: label "L4105bc:" -- head of f_codemotion's LAST per-block
    #     loop, right after `ppin |= subinsert` and right before
    #     `subdelete = ppin` (which is then INTERSECTed with antlocs).
    #     s5 = the current block node in that loop.
    # ====================================================================
    cmot_idx = find_line_exact(lines, "L4105bc:")
    if cmot_idx >= 0:
        cmot_hook = (
            "/* IDO52_CMOT_KILL_AVAIL hook: break a listed expression\'s codemotion\n"
            " * availability for this block, so its local computation survives. */\n"
            "ido52_cmot_kill_avail_block(mem, s5);"
        )
        lines, delta = inject_after(lines, cmot_idx + 1, cmot_hook, results, "CMOT_KILL_AVAIL")
        offset += delta
    else:
        results.append(("CMOT_KILL_AVAIL", "FAIL: could not find L4105bc label", -1))

    dump_idx = find_line(lines, "ido52_apply_precolor_post_globalcolor(mem);")
    if dump_idx >= 0:
        dump_hook = (
            'ido52_dump_web_assignments(mem);\n'
            'ido52_apply_web_bind_bbs(mem);\n'
        )
        lines, delta = inject_after(lines, dump_idx, dump_hook, results, "DUMP_WEBS")
        offset += delta
    else:
        results.append(("DUMP_WEBS", "FAIL: could not find post-globalcolor hook call", -1))

    # ====================================================================
    # 21. EXCLUDE_REGS post-SRP: modify register table and web assignments
    #     after f_opt_saved_regs to exclude specified registers
    #     Pattern: "f_opt_saved_regs(mem, sp);" in f_oneproc
    # ====================================================================
    srp_idx = find_line(lines, "f_opt_saved_regs(mem, sp);")
    if srp_idx >= 0:
        srp_hook = (
            'ido52_exclude_regs_post_srp(mem);\n'
        )
        lines, delta = inject_after(lines, srp_idx, srp_hook, results, "EXCL-SRP")
        offset += delta
    else:
        results.append(("EXCL-SRP", "FAIL: could not find f_opt_saved_regs call", -1))

    # ====================================================================
    # Write result
    # ====================================================================
    result = "\n".join(lines)
    with open(path, "w") as f:
        f.write(result)

    # Report
    print(f"\n[uopt hooks] Results for {path}:")
    ok_count = 0
    fail_count = 0
    for name, status, line in results:
        if status == "OK":
            print(f"  {name}: OK (line ~{line})")
            ok_count += 1
        else:
            print(f"  {name}: {status}")
            fail_count += 1

    print(f"\n[uopt hooks] {ok_count} hooks applied, {fail_count} failed")

    return fail_count == 0


if __name__ == "__main__":
    path = sys.argv[1] if len(sys.argv) > 1 else UOPT_C
    if not os.path.exists(path):
        print(f"[uopt hooks] ERROR: {path} not found. Run 'make setup-ido52' first.")
        sys.exit(1)
    if not apply_hooks(path):
        sys.exit(1)
