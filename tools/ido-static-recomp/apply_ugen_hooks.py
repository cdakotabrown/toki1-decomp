#!/usr/bin/env python3
"""Apply IDO52 ugen hooks to the regenerated ugen.c file.

ugen.c is NOT in git -- it gets regenerated from the MIPS binary by the
ido-static-recomp Makefile. This script patches the regenerated ugen.c
to add environment-variable-gated hooks for controlling register allocation.

Usage:
    python3 tools/ido-static-recomp/apply_ugen_hooks.py

Hooks added:
    IDO52_UGEN_MAX_SREGS=N       -- Clamp s-register count (0-8) in f_init_regs
    IDO52_UGEN_NUM_TREGS=N       -- Clamp temp register count (0-6) in f_init_regs
    IDO52_UGEN_TREG_ORDER=t2,t0  -- Reorder temp register free list (first=first allocated)
    IDO52_UGEN_SREG_ORDER=s2,s0  -- (placeholder) S-reg order controlled by uopt, not ugen
    IDO52_UGEN_DEBUG=1           -- Print register count diagnostics to stderr
    IDO52_UGEN_NO_ABS_ADDR=1    -- Force register-relative addressing for large offsets
    IDO52_UGEN_NO_ABS_BIAS=N    -- Subtract N from redirected offsets (base register drift)
    IDO52_UGEN_NO_ARG_COALESCE=1|occ:N[,M]
                                -- Suppress coalescing of a call-argument expression
                                   temp directly into $a0-$a3 (forces general temp +
                                   `or $aN,$tX,$zero`).  See NO_ARG_COALESCE below.
    IDO52_UGEN_ARG_COALESCE_DEBUG=1
                                -- Print every hard-register destination site ugen
                                   evaluates (occurrence index + target register)
    IDO52_UGEN_TREG_ORDER_WINDOW=<pool>:<from>:<to>[,...]
                                -- Occurrence-windowed temp pool clamp (hook 23).
                                   <pool> = a count N (keep the first N registers of
                                   the function-entry free list) or an explicit
                                   '+'-separated list (t6+t9).  <from>/<to> are
                                   inclusive 1-based f_get_reg occurrence indices,
                                   counted per function; <to> = -1 means "to end".
                                   Outside every window the entry pool is restored.
    IDO52_UGEN_TREG_WINDOW_DEBUG=1
                                -- Print the captured entry pool and every
                                   f_get_reg occurrence index / hold / release
    IDO52_UGEN_TEMP_REG=occ:reg[,...]
                                -- Hook 36: force which free-list register a
                                   f_get_one_free_reg request pops (+ _TRACE=1)
    IDO52_UGEN_NO_TARGET=occ[,...]|all
                                -- Hook 41: expression-result targeting veto in
                                   f_eval (force the evaluate-into-a-temp arm;
                                   +1 insn/site).  Companions: TARGET_TRACE=1;
                                   FORCE_TARGET (probe-only, refused without
                                   FORCE_TARGET_UNSAFE=1 -- see the hook text)

NO_ARG_COALESCE details:
    uopt hands ugen a statement whose destination is a HARD register (`ureg`),
    e.g. an outgoing call argument `... -> $4`.  ugen's f_eval evaluates the
    child expression with that register as the "wanted" destination, so the
    expression's final instruction writes $aN directly:

        andi $a0, $v0, 0xff        <- coalesced
        jal  callee
         sb  $a0, glob             <- second consumer reads $a0

    The 1995 target frequently keeps the value in a general temp and
    materialises the argument copy instead:

        andi $v0, $v1, 0xff
        or   $a0, $v0, $zero       <- the copy this hook restores
        jal  callee
         sb  $v0, glob

    The choice is made entirely in ugen, AFTER every uopt hook has run: the
    value never becomes a uopt web (verify with IDO52_DUMP_WEBS=1), so
    IDO52_PRECOLOR_WEB / EXCLUDE_REGS can never reach it.  This hook passes
    0x48 ("no preferred register") to the recursive f_eval, which makes ugen
    allocate a general temp; the existing f_move_to_dest call that follows
    then emits the copy.  Instruction count +1 per suppressed site.

    Occurrence index is 1-based and RESET PER FUNCTION (in f_init_regs), so
    it is stable for the one-function-per-file overlay sources.  Only sites
    whose destination is $a0-$a3 (regs 4-7) are counted or suppressed.

NO_ABS_ADDR details:
    When a load/store uses absolute addressing (e.g. lbu $9, D_80127230+7823),
    the assembler generates 2 instructions: lui $at + offset($at).
    Register-relative (e.g. lbu $9, 7823($16)) needs only 1 instruction.
    Each redirected reference saves 1 lui at the binary level.

    CAVEAT: After a loop that increments the base register, the offset must
    be adjusted by the drift amount. Set IDO52_UGEN_NO_ABS_BIAS=N where
    N = total drift (e.g., 2176 for a 32-iteration loop with stride 68).
    Without BIAS, redirects are skipped with a warning.

    Survey results (BIAS=0, may be semantically wrong but shows hook coverage):
      girl_rec_init: 28 absolute refs converted (48% of 58 total) ~28 lui saved
      func_801324AC: 10 absolute refs converted (10% of 105)
      schedule_make: 4 absolute refs converted (7% of 60)

    The iconst redirect hook (for large integer constants >= 0x80000000) is
    currently not triggered for typical functions -- large address loads go
    through emit_ra as symbol references, not through iconst nodes.

After patching, recompile with:
    cd tools/ido-static-recomp
    gcc -c -std=c11 -O0 -ggdb3 -fno-strict-aliasing -I. -o build/5.2/ugen.o build/5.2/ugen.c
    gcc -std=c11 -O0 -ggdb3 -fno-strict-aliasing -I. -o build/5.2/out/ugen \
        build/5.2/ugen.o build/5.2/version_info.o build/5.2/libc_impl_52.o -lm
"""

import sys
import os

UGEN_C = os.path.join(os.path.dirname(__file__), "build", "5.2", "ugen.c")

HOOK_HEADER = r'''

/* ================================================================== */
/* IDO52 ugen hooks -- register count clamping + order control        */
/* ================================================================== */
static int ugen_hooks_checked;
static int ugen_max_sregs = -1;   /* IDO52_UGEN_MAX_SREGS */
static int ugen_num_tregs = -1;   /* IDO52_UGEN_NUM_TREGS */
static int ugen_hooks_debug;
static int ugen_no_abs_addr;  /* IDO52_UGEN_NO_ABS_ADDR: force register-relative addressing */
static int ugen_no_abs_bias = -1;  /* IDO52_UGEN_NO_ABS_BIAS: subtract from offset when redirecting */

/* IDO52_UGEN_TREG_ORDER: reorder temp register free list.
 * Format: "t2,t0,t1,t3,t4,t5" -- register names or numbers.
 * First in list = first allocated by ugen. */
#define MAX_TREG_ORDER 16
static int ugen_treg_order[MAX_TREG_ORDER];
static int ugen_treg_order_count = 0;

/* IDO52_UGEN_SREG_ORDER: reorder s-register descriptor indices.
 * Format: "s2,s0,s1,s3,s4,s5,s6,s7" -- conceptual allocation priority.
 * Swaps register descriptor contents so s-reg N gets descriptor slot M. */
#define MAX_SREG_ORDER 8
static int ugen_sreg_order[MAX_SREG_ORDER];
static int ugen_sreg_order_count = 0;

static int parse_reg_name(const char *name) {
    /* Parse t0-t9, s0-s8, or raw number */
    if (name[0] == 't' && name[1] >= '0' && name[1] <= '9' && name[2] == '\0') {
        int n = name[1] - '0';
        if (n <= 7) return 8 + n;   /* t0=8 .. t7=15 */
        return 24 + (n - 8);        /* t8=24, t9=25 */
    }
    if (name[0] == 's' && name[1] >= '0' && name[1] <= '8' && name[2] == '\0') {
        if (name[1] == '8') return 30;  /* s8/fp = 30 */
        return 16 + (name[1] - '0');    /* s0=16 .. s7=23 */
    }
    return atoi(name);
}

static void parse_reg_order(const char *env, int *order, int *count, int max) {
    char buf[256];
    char *p, *tok;
    *count = 0;
    if (!env || !env[0]) return;
    strncpy(buf, env, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    p = buf;
    while ((tok = strtok(p, ",")) != NULL && *count < max) {
        /* strip whitespace */
        while (*tok == ' ') tok++;
        order[(*count)++] = parse_reg_name(tok);
        p = NULL;
    }
}

static void ugen_hooks_init(void) {
    char *env;
    if (ugen_hooks_checked) return;
    ugen_hooks_checked = 1;
    env = getenv("IDO52_UGEN_MAX_SREGS");
    if (env) {
        ugen_max_sregs = atoi(env);
        fprintf(stderr, "[UGEN] MAX_SREGS=%d\n", ugen_max_sregs);
    }
    env = getenv("IDO52_UGEN_NUM_TREGS");
    if (env) {
        ugen_num_tregs = atoi(env);
        fprintf(stderr, "[UGEN] NUM_TREGS=%d\n", ugen_num_tregs);
    }
    env = getenv("IDO52_UGEN_TREG_ORDER");
    if (env) {
        parse_reg_order(env, ugen_treg_order, &ugen_treg_order_count, MAX_TREG_ORDER);
        fprintf(stderr, "[UGEN] TREG_ORDER: %d regs:", ugen_treg_order_count);
        for (int i = 0; i < ugen_treg_order_count; i++)
            fprintf(stderr, " %d", ugen_treg_order[i]);
        fprintf(stderr, "\n");
    }
    env = getenv("IDO52_UGEN_SREG_ORDER");
    if (env) {
        parse_reg_order(env, ugen_sreg_order, &ugen_sreg_order_count, MAX_SREG_ORDER);
        fprintf(stderr, "[UGEN] SREG_ORDER: %d regs:", ugen_sreg_order_count);
        for (int i = 0; i < ugen_sreg_order_count; i++)
            fprintf(stderr, " %d", ugen_sreg_order[i]);
        fprintf(stderr, "\n");
    }
    env = getenv("IDO52_UGEN_DEBUG");
    if (env && env[0]) ugen_hooks_debug = 1;
    env = getenv("IDO52_UGEN_NO_ABS_ADDR");
    if (env && env[0]) {
        ugen_no_abs_addr = 1;
        fprintf(stderr, "[UGEN] NO_ABS_ADDR: forcing register-relative addressing\n");
    }
    env = getenv("IDO52_UGEN_NO_ABS_BIAS");
    if (env && env[0]) {
        ugen_no_abs_bias = atoi(env);
        fprintf(stderr, "[UGEN] NO_ABS_BIAS: %d (subtracted from redirected offsets)\n", ugen_no_abs_bias);
    }
}
'''

SREG_HOOK = r'''
/* --- HOOK: clamp s-register count before f_init_regs uses it --- */
ugen_hooks_init();
if (ugen_hooks_debug) fprintf(stderr, "[UGEN] f_init_regs: num_sregs = %u (at 0x10019008)\n", MEM_U32(0x10019008));
if (ugen_max_sregs >= 0 && (int32_t)MEM_U32(0x10019008) > ugen_max_sregs) {
    fprintf(stderr, "[UGEN] Clamping num_sregs %u -> %d\n", MEM_U32(0x10019008), ugen_max_sregs);
    MEM_U32(0x10019008) = (uint32_t)ugen_max_sregs;
}
'''

TREG_HOOK = r'''
/* --- HOOK: clamp temp register count before f_init_regs uses it --- */
ugen_hooks_init();
if (ugen_hooks_debug) fprintf(stderr, "[UGEN] f_init_regs: num_tregs = %u (at 0x10019010)\n", MEM_U32(0x10019010));
if (ugen_num_tregs >= 0 && (int32_t)MEM_U32(0x10019010) > ugen_num_tregs) {
    fprintf(stderr, "[UGEN] Clamping num_tregs %u -> %d\n", MEM_U32(0x10019010), ugen_num_tregs);
    MEM_U32(0x10019010) = (uint32_t)ugen_num_tregs;
}
'''

TREG_ORDER_HOOK = r'''
/* --- HOOK: reorder temp register free list (IDO52_UGEN_TREG_ORDER) --- */
/* At this point the normal t-reg loop has built the free list with
 * registers 8..8+N in sequential order.  If TREG_ORDER is set, we
 * clear the free list and rebuild it in the user-specified order.
 *
 * Free list structure:
 *   0x10019a94+0 = head (u8 reg number, 0x48 = empty)
 *   0x10019a94+1 = tail (u8 reg number, 0x48 = empty)
 *   Per-reg descriptor at 0x10019520 + reg*12:
 *     +6 = next pointer (u8 reg number, 0x48 = end)
 *     +7 = in-free-list flag (u8, 1 = in list)
 */
if (ugen_treg_order_count > 0) {
    int _i;
    /* First, remove all t-regs from the free list by clearing their flags */
    for (_i = 8; _i < 14; _i++) {
        uint32_t _desc = 0x10019520 + (uint32_t)_i * 12;
        MEM_U8(_desc + 7) = 0;   /* clear in-free-list flag */
        MEM_U8(_desc + 6) = 0x48; /* clear next pointer */
    }
    /* Also clear any extra regs from the table at 0x10005424 */
    {
        uint32_t _nextra = MEM_U32(0x10019018);
        uint32_t _tbl = 0x10005424;
        for (_i = 0; _i < (int)_nextra; _i++) {
            uint32_t _r = MEM_U8(_tbl + _i);
            uint32_t _desc = 0x10019520 + _r * 12;
            MEM_U8(_desc + 7) = 0;
            MEM_U8(_desc + 6) = 0x48;
        }
    }
    /* Reset free list head/tail to empty */
    MEM_U8(0x10019a94 + 0) = 0x48;
    MEM_U8(0x10019a94 + 1) = 0x48;
    /* Re-add registers in the specified order */
    if (ugen_hooks_debug) fprintf(stderr, "[UGEN] Rebuilding int free list:");
    for (_i = 0; _i < ugen_treg_order_count; _i++) {
        uint32_t _a0 = (uint32_t)ugen_treg_order[_i];
        if (ugen_hooks_debug) fprintf(stderr, " %u", _a0);
        f_add_to_free_list(mem, sp, _a0);
    }
    if (ugen_hooks_debug) fprintf(stderr, "\n");
}
'''

SREG_ORDER_HOOK = r'''
/* --- HOOK: reorder s-register descriptors (IDO52_UGEN_SREG_ORDER) --- */
/* After f_init_regs clears descriptors for s0(16)..s7(23), if SREG_ORDER
 * is set, swap descriptor contents so that the register allocator sees
 * them in a different order.
 *
 * The allocator scans 16,17,18... and picks the first available.
 * By swapping descriptor slot contents between register positions,
 * we can make it allocate "s2" when it would normally pick "s0".
 *
 * This swaps the 12-byte descriptor entries between positions.
 * The register NUMBER stays the same (slot position), but the
 * descriptor state (used/free) gets permuted.
 *
 * NOTE: This is experimental. It only works if the descriptors
 * are uniform at init time (all cleared to same state). For non-uniform
 * states, the swap would need to happen at allocation time instead. */
if (ugen_sreg_order_count > 0) {
    if (ugen_hooks_debug) {
        fprintf(stderr, "[UGEN] SREG_ORDER requested but s-reg allocation is controlled by uopt, not ugen.\n");
        fprintf(stderr, "[UGEN] Use IDO52_RESERVE_SREGS in uopt hooks instead.\n");
    }
    /* S-reg allocation order is determined by f_opt_saved_regs in uopt.c,
     * not by ugen's descriptor table. The descriptors are all initialized
     * identically (cleared), so swapping them has no effect.
     * This hook is a placeholder for future uopt integration. */
}
'''

# ---------------------------------------------------------------------------
# Hook 22: IDO52_UGEN_NO_ARG_COALESCE
#
# Self-contained (its own statics + lazy init), so it can be applied on its own
# to a ugen.c that already carries the older hook block.
# ---------------------------------------------------------------------------
ARG_COALESCE_HEADER = r'''

/* ================================================================== */
/* IDO52 ugen hook 22 -- IDO52_UGEN_NO_ARG_COALESCE                   */
/*                                                                    */
/* Suppresses ugen's coalescing of a call-argument expression temp    */
/* straight into $a0-$a3.  With the hook active at a site, f_eval is  */
/* asked for "no preferred register" (0x48) instead of $aN, so the    */
/* expression lands in a general temp and the f_move_to_dest that     */
/* already follows the recursive f_eval emits `or $aN,$tX,$zero`.     */
/*                                                                    */
/* Default OFF: with the env var unset ugen_argcoal_suppress() always */
/* returns 0 before touching any state, so codegen is bit-identical.  */
/* ================================================================== */
static int ugen_argcoal_checked;
static int ugen_argcoal_mode;    /* 0 = off, 1 = all $aN sites, 2 = occ list */
static int ugen_argcoal_debug;
#define UGEN_ARGCOAL_MAX_OCC 64
static int ugen_argcoal_occ[UGEN_ARGCOAL_MAX_OCC];
static int ugen_argcoal_nocc;
static int ugen_argcoal_seen;    /* 1-based occurrence counter, per function */

static void ugen_argcoal_init(void) {
    char *env;
    if (ugen_argcoal_checked) return;
    ugen_argcoal_checked = 1;

    env = getenv("IDO52_UGEN_ARG_COALESCE_DEBUG");
    if (env && env[0]) ugen_argcoal_debug = 1;

    env = getenv("IDO52_UGEN_NO_ARG_COALESCE");
    if (env && env[0] && strcmp(env, "0") != 0) {
        if (strncmp(env, "occ:", 4) == 0) {
            char buf[256];
            char *p, *tok;
            strncpy(buf, env + 4, sizeof(buf) - 1);
            buf[sizeof(buf) - 1] = '\0';
            p = buf;
            while ((tok = strtok(p, ",")) != NULL &&
                   ugen_argcoal_nocc < UGEN_ARGCOAL_MAX_OCC) {
                while (*tok == ' ') tok++;
                ugen_argcoal_occ[ugen_argcoal_nocc++] = atoi(tok);
                p = NULL;
            }
            ugen_argcoal_mode = 2;
        } else {
            ugen_argcoal_mode = 1;
        }
        fprintf(stderr, "[UGEN] NO_ARG_COALESCE=%s (mode %d, %d occ)\n",
                env, ugen_argcoal_mode, ugen_argcoal_nocc);
    }
}

/* Called once per function from f_init_regs so occurrence indices are
 * function-local (stable for one-function-per-file overlay sources). */
static void ugen_argcoal_reset(void) {
    ugen_argcoal_init();
    ugen_argcoal_seen = 0;
}

/* dst  = the hard destination register ugen is about to hand to f_eval.
 * site = which of the two "eval child into ureg" arms in f_eval fired
 *        ('A' = L42cb1c, 'B' = L42d408); both are counted in one sequence.
 * Returns 1 when the caller must substitute 0x48 (no preferred register). */
static int ugen_argcoal_suppress(uint32_t dst, int site) {
    int i;
    ugen_argcoal_init();
    if (dst < 4 || dst > 7) return 0;   /* $a0-$a3 only */
    if (!ugen_argcoal_mode && !ugen_argcoal_debug) return 0;
    ugen_argcoal_seen++;
    if (ugen_argcoal_mode == 1) {
        if (ugen_argcoal_debug)
            fprintf(stderr, "[UGEN-ARGCOAL] occ=%d site=%c dst=$a%d -> SUPPRESS\n",
                    ugen_argcoal_seen, site, (int)dst - 4);
        return 1;
    }
    for (i = 0; i < ugen_argcoal_nocc; i++) {
        if (ugen_argcoal_occ[i] == ugen_argcoal_seen) {
            if (ugen_argcoal_debug)
                fprintf(stderr, "[UGEN-ARGCOAL] occ=%d site=%c dst=$a%d -> SUPPRESS\n",
                        ugen_argcoal_seen, site, (int)dst - 4);
            return 1;
        }
    }
    if (ugen_argcoal_debug)
        fprintf(stderr, "[UGEN-ARGCOAL] occ=%d site=%c dst=$a%d -> keep\n",
                ugen_argcoal_seen, site, (int)dst - 4);
    return 0;
}
'''

# Reset of the per-function occurrence counter; inserted after the L43a1e8
# label in f_init_regs (same anchor the MAX_SREGS hook uses).
ARG_COALESCE_RESET = r'''
/* --- HOOK 22: reset the per-function NO_ARG_COALESCE occurrence counter --- */
ugen_argcoal_reset();
'''

# The suppression itself, inserted in f_eval right before the recursive
# f_eval that evaluates a statement's child into the hard `ureg` destination
# held in `fp` (label L42cb1c).
ARG_COALESCE_HOOK = r'''
/* --- HOOK 22: IDO52_UGEN_NO_ARG_COALESCE (site @SITE@) --- */
/* `fp` holds the hard destination register from f_ureg (an outgoing call
 * argument when it is $a0-$a3).  Handing 0x48 to f_eval instead makes the
 * child expression land in a general temp; the f_reg + f_move_to_dest pair
 * a few lines below then materialises `or $aN,$tX,$zero`. */
if (ugen_argcoal_suppress(fp, '@SITE@')) a1 = 0x48;
'''

# The two f_eval arms that evaluate a statement's child into a hard `ureg`
# destination.  Keyed by the label that starts the arm.
ARG_COALESCE_SITES = {"L42cb1c": "A", "L42d408": "B"}

# ---------------------------------------------------------------------------
# Hook 23: IDO52_UGEN_TREG_ORDER_WINDOW
#
# Occurrence-windowed variant of IDO52_UGEN_TREG_ORDER.  The legacy hook
# rebuilds the integer temp free list ONCE, in f_init_regs, so the pool it
# installs is in force for the whole function.  Several targets need a
# SMALL pool at one site and the default pool everywhere else (the
# canonical case is DATE/func_80132728, whose sole count divergence is a
# `lui %hi(...)` that the target emits late -- reproducible with a 2-register
# pool, which then costs +2 words at two later sites that want the default
# pool).
#
# This hook clamps the pool DURING code generation, on an occurrence window
# over f_get_reg calls, and restores the captured entry pool outside the
# window.  Self-contained (own statics + lazy init) so it can be applied to a
# ugen.c that already carries the older hook blocks.
# ---------------------------------------------------------------------------
TREGWIN_HEADER = r'''

/* ================================================================== */
/* IDO52 ugen hook 23 -- IDO52_UGEN_TREG_ORDER_WINDOW                 */
/*                                                                    */
/* Syntax:  IDO52_UGEN_TREG_ORDER_WINDOW=<spec>[,<spec>...]           */
/*          spec = <pool>:<from>:<to>                                 */
/*                                                                    */
/*   <pool>  either a decimal count N (1..16), meaning "keep only the  */
/*           first N registers of the function-entry temp free list,   */
/*           in that list's own order", or a '+'-separated explicit    */
/*           register list (e.g. t6+t9), meaning "the temp free list   */
/*           is exactly these registers in this order".                */
/*   <from>, <to>  inclusive 1-based occurrence indices of f_get_reg   */
/*           calls, counted per function (the counter is reset from    */
/*           f_init_regs).  <to> may be -1 for "to end of function".   */
/*                                                                    */
/* Counting semantics: the window is tested at the ENTRY of every      */
/* f_get_reg call.  Occurrence k therefore denotes "the pool ugen sees */
/* when it makes its k-th temp-register request in this function".     */
/* The clamp is a state change on the free list, so it stays in force  */
/* until the next f_get_reg call re-evaluates it.                      */
/*                                                                    */
/* Only registers in the temp universe {t0..t7, t8, t9} are touched.   */
/* A register is removed only while it is genuinely in the free list,  */
/* and re-added only if this hook is the one that removed it, so a     */
/* register holding a live value is never handed back out.             */
/*                                                                    */
/* Default OFF: with the env var unset ugen_tregwin_event() returns    */
/* before touching any state, so codegen is bit-identical.             */
/* ================================================================== */
/* Forward declarations: this block may sit above both ugen.c's own
 * prototypes and the legacy hook block that defines parse_reg_name(). */
static void f_add_to_free_list(uint8_t *mem, uint32_t sp, uint32_t a0);
static void f_remove_from_free_list(uint8_t *mem, uint32_t sp, uint32_t a0);
static int parse_reg_name(const char *name);

#define UGEN_TREGWIN_MAX_SPEC 8
#define UGEN_TREGWIN_MAX_POOL 16
#define UGEN_TREGWIN_DESC(r)  (0x10019520u + (uint32_t)(r) * 12u)
#define UGEN_TREGWIN_HEAD     0x10019a94u

static int ugen_tregwin_checked;
static int ugen_tregwin_on;       /* 0 = hook disabled (default) */
static int ugen_tregwin_debug;
static int ugen_tregwin_nspec;
static int ugen_tregwin_from[UGEN_TREGWIN_MAX_SPEC];
static int ugen_tregwin_to[UGEN_TREGWIN_MAX_SPEC];
static int ugen_tregwin_cnt[UGEN_TREGWIN_MAX_SPEC];      /* N form, else -1 */
static int ugen_tregwin_pool[UGEN_TREGWIN_MAX_SPEC][UGEN_TREGWIN_MAX_POOL];
static int ugen_tregwin_npool[UGEN_TREGWIN_MAX_SPEC];

static int ugen_tregwin_univ[UGEN_TREGWIN_MAX_POOL]; /* entry free-list order */
static int ugen_tregwin_nuniv;
static uint32_t ugen_tregwin_held;   /* bitmask of regs WE pulled out */
static int ugen_tregwin_seen;        /* per-function f_get_reg counter */
static int ugen_tregwin_cur = -2;    /* spec index currently installed */

static int ugen_tregwin_is_temp(int r) {
    return (r >= 8 && r <= 15) || r == 24 || r == 25;
}

/* Authoritative free-list membership: walk the list.  The per-descriptor
 * "in free list" flag at +7 is NOT reliable -- a register reserved by other
 * machinery (a PRECOLOR'd variable, for one) can be off the list with the
 * flag still set, and f_remove_from_free_list then aborts ugen with
 * "register not free" (reg_mgr.p:967). */
static int ugen_tregwin_in_list(uint8_t *mem, int reg) {
    uint32_t r = MEM_U8(UGEN_TREGWIN_HEAD + 0);
    int guard = 0;
    while (r != 0x48 && guard++ < 64) {
        if ((int)r == reg) return 1;
        r = MEM_U8(UGEN_TREGWIN_DESC(r) + 6);
    }
    return 0;
}

static void ugen_tregwin_init(void) {
    char *env, *p, *tok;
    char buf[512];
    if (ugen_tregwin_checked) return;
    ugen_tregwin_checked = 1;

    env = getenv("IDO52_UGEN_TREG_WINDOW_DEBUG");
    if (env && env[0]) ugen_tregwin_debug = 1;

    env = getenv("IDO52_UGEN_TREG_ORDER_WINDOW");
    if (!env || !env[0]) return;

    strncpy(buf, env, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    p = buf;
    for (tok = strtok(p, ",;"); tok && ugen_tregwin_nspec < UGEN_TREGWIN_MAX_SPEC;
         tok = strtok(NULL, ",;")) {
        char *c1, *c2;
        int idx = ugen_tregwin_nspec;
        while (*tok == ' ') tok++;
        c1 = strchr(tok, ':');
        if (!c1) continue;
        *c1 = '\0';
        c2 = strchr(c1 + 1, ':');
        if (!c2) continue;
        *c2 = '\0';
        ugen_tregwin_from[idx] = atoi(c1 + 1);
        ugen_tregwin_to[idx] = atoi(c2 + 1);
        if (ugen_tregwin_to[idx] < 0) ugen_tregwin_to[idx] = 0x7fffffff;
        ugen_tregwin_cnt[idx] = -1;
        ugen_tregwin_npool[idx] = 0;
        if (tok[0] >= '0' && tok[0] <= '9') {
            ugen_tregwin_cnt[idx] = atoi(tok);
            if (ugen_tregwin_cnt[idx] < 1) ugen_tregwin_cnt[idx] = 1;
        } else {
            /* '+'-separated register list, split by hand: strtok() is
             * already in use for the outer spec loop. */
            char pbuf[128];
            char *st, *q;
            strncpy(pbuf, tok, sizeof(pbuf) - 1);
            pbuf[sizeof(pbuf) - 1] = '\0';
            st = pbuf;
            for (q = pbuf; ; q++) {
                if (*q == '+' || *q == '\0') {
                    char end = *q;
                    *q = '\0';
                    while (*st == ' ') st++;
                    if (*st && ugen_tregwin_npool[idx] < UGEN_TREGWIN_MAX_POOL)
                        ugen_tregwin_pool[idx][ugen_tregwin_npool[idx]++] =
                            parse_reg_name(st);
                    st = q + 1;
                    if (end == '\0') break;
                }
            }
            if (ugen_tregwin_npool[idx] == 0) continue;
        }
        ugen_tregwin_nspec++;
    }
    if (ugen_tregwin_nspec > 0) {
        int i, j;
        ugen_tregwin_on = 1;
        fprintf(stderr, "[UGEN] TREG_ORDER_WINDOW: %d spec(s)\n",
                ugen_tregwin_nspec);
        for (i = 0; i < ugen_tregwin_nspec; i++) {
            fprintf(stderr, "[UGEN]   occ %d..", ugen_tregwin_from[i]);
            if (ugen_tregwin_to[i] == 0x7fffffff) fprintf(stderr, "end");
            else fprintf(stderr, "%d", ugen_tregwin_to[i]);
            if (ugen_tregwin_cnt[i] >= 0)
                fprintf(stderr, " -> first %d of entry pool\n",
                        ugen_tregwin_cnt[i]);
            else {
                fprintf(stderr, " -> pool");
                for (j = 0; j < ugen_tregwin_npool[i]; j++)
                    fprintf(stderr, " %d", ugen_tregwin_pool[i][j]);
                fprintf(stderr, "\n");
            }
        }
    }
}

/* Walk the integer free list and record the temp registers it holds, in
 * list order.  Called once per function, from f_init_regs, AFTER the
 * normal (and any legacy TREG_ORDER) free-list construction. */
static void ugen_tregwin_reset(uint8_t *mem) {
    uint32_t r;
    int guard = 0;
    ugen_tregwin_init();
    ugen_tregwin_seen = 0;
    ugen_tregwin_held = 0;
    ugen_tregwin_cur = -2;
    ugen_tregwin_nuniv = 0;
    if (!ugen_tregwin_on && !ugen_tregwin_debug) return;
    r = MEM_U8(UGEN_TREGWIN_HEAD + 0);
    while (r != 0x48 && guard++ < 64) {
        if (ugen_tregwin_is_temp((int)r) &&
            ugen_tregwin_nuniv < UGEN_TREGWIN_MAX_POOL)
            ugen_tregwin_univ[ugen_tregwin_nuniv++] = (int)r;
        r = MEM_U8(UGEN_TREGWIN_DESC(r) + 6);
    }
    if (ugen_tregwin_debug) {
        int i;
        fprintf(stderr, "[UGEN-TREGWIN] function entry temp pool (%d):",
                ugen_tregwin_nuniv);
        for (i = 0; i < ugen_tregwin_nuniv; i++)
            fprintf(stderr, " %d", ugen_tregwin_univ[i]);
        fprintf(stderr, "\n");
    }
}

/* Install the pool described by spec `s` (-1 = the captured entry pool). */
static void ugen_tregwin_install(uint8_t *mem, uint32_t sp, int s) {
    int want[UGEN_TREGWIN_MAX_POOL];
    int nwant = 0, i, j;
    uint32_t wantmask = 0;

    if (s < 0) {
        for (i = 0; i < ugen_tregwin_nuniv; i++) want[nwant++] = ugen_tregwin_univ[i];
    } else if (ugen_tregwin_cnt[s] >= 0) {
        for (i = 0; i < ugen_tregwin_nuniv && nwant < ugen_tregwin_cnt[s]; i++)
            want[nwant++] = ugen_tregwin_univ[i];
    } else {
        for (i = 0; i < ugen_tregwin_npool[s]; i++) {
            int r = ugen_tregwin_pool[s][i];
            for (j = 0; j < ugen_tregwin_nuniv; j++)
                if (ugen_tregwin_univ[j] == r) { want[nwant++] = r; break; }
        }
    }
    if (nwant == 0) return;   /* refuse to empty the pool */
    for (i = 0; i < nwant; i++) wantmask |= (uint32_t)1 << want[i];

    /* Pull out every entry-pool temp that this spec does not want. */
    for (i = 0; i < ugen_tregwin_nuniv; i++) {
        int r = ugen_tregwin_univ[i];
        if (wantmask & ((uint32_t)1 << r)) continue;
        if (ugen_tregwin_in_list(mem, r)) {
            f_remove_from_free_list(mem, sp, (uint32_t)r);
            ugen_tregwin_held |= (uint32_t)1 << r;
            if (ugen_tregwin_debug)
                fprintf(stderr, "[UGEN-TREGWIN]   hold  %d\n", r);
        }
    }
    /* Hand back the ones this spec wants, in the spec's order, but only if
     * WE are the reason they are missing (never a live register). */
    for (i = 0; i < nwant; i++) {
        int r = want[i];
        if (!(ugen_tregwin_held & ((uint32_t)1 << r))) continue;
        if (ugen_tregwin_in_list(mem, r)) {
            ugen_tregwin_held &= ~((uint32_t)1 << r);
            continue;
        }
        /* Descriptor+4 is ugen's "what this register holds" field; it is the
         * exact condition f_add_to_free_list asserts on ("register not
         * free", reg_mgr.p:967).  A register can acquire a value while we
         * hold it out of the list -- a PRECOLOR'd variable pinned straight
         * to it does exactly this -- and forcing it back would abort ugen.
         * Drop our claim instead and let ugen's own f_free_reg return it. */
        if (MEM_U16(UGEN_TREGWIN_DESC(r) + 4) != 0) {
            ugen_tregwin_held &= ~((uint32_t)1 << r);
            if (ugen_tregwin_debug)
                fprintf(stderr, "[UGEN-TREGWIN]   skip-release %d (busy)\n", r);
            continue;
        }
        f_add_to_free_list(mem, sp, (uint32_t)r);
        ugen_tregwin_held &= ~((uint32_t)1 << r);
        if (ugen_tregwin_debug)
            fprintf(stderr, "[UGEN-TREGWIN]   release %d\n", r);
    }
}

/* Called at the entry of every f_get_reg. */
static void ugen_tregwin_event(uint8_t *mem, uint32_t sp) {
    int i, s = -1;
    ugen_tregwin_init();
    if (!ugen_tregwin_on) return;
    ugen_tregwin_seen++;
    for (i = 0; i < ugen_tregwin_nspec; i++) {
        if (ugen_tregwin_seen >= ugen_tregwin_from[i] &&
            ugen_tregwin_seen <= ugen_tregwin_to[i]) { s = i; break; }
    }
    if (ugen_tregwin_debug)
        fprintf(stderr, "[UGEN-TREGWIN] occ=%d spec=%d\n", ugen_tregwin_seen, s);
    if (s == ugen_tregwin_cur) return;
    ugen_tregwin_cur = s;
    ugen_tregwin_install(mem, sp, s);
}
'''

# Reset of the per-function occurrence counter + capture of the entry pool.
# Inserted after the L43a1e8 label in f_init_regs (the same anchor the
# MAX_SREGS hook uses), which is AFTER the integer free list is built.
TREGWIN_RESET = r'''
/* --- HOOK 23: reset TREG_ORDER_WINDOW state, capture entry temp pool --- */
ugen_tregwin_reset(mem);
'''

# The clamp itself, at the entry label of f_get_reg (before its own frame
# is allocated, so `sp` is still the caller's).
TREGWIN_HOOK = r'''
/* --- HOOK 23: IDO52_UGEN_TREG_ORDER_WINDOW occurrence clamp --- */
ugen_tregwin_event(mem, sp);
'''

# Hook for f_emit_ra: redirect to f_emit_rab for register-relative addressing.
# Inserted right after the L413c20: label in f_emit_ra.
EMIT_RA_NOABS_HOOK = r'''
/* --- HOOK: IDO52_UGEN_NO_ABS_ADDR redirect emit_ra -> emit_rab --- */
/* When active and offset (a3) > 256, redirect absolute address emission
 * to register-relative. We call f_emit_rab directly since emit_ra and
 * emit_rab have different instruction record formats (emit_ra stores a
 * symbol address in [-16]; emit_rab stores a base register number).
 *
 * CAVEAT: this is only correct when the base register ($16/s0) actually
 * holds the symbol base address at the point of the access.  If $16 has
 * been modified (e.g. by a loop that increments the slot pointer), the
 * offset must be adjusted by the delta between $16's current value and
 * the original symbol base.  We cannot compute that delta here, so we
 * require IDO52_UGEN_NO_ABS_BIAS which is subtracted from the offset.
 *
 * Set IDO52_UGEN_NO_ABS_BIAS=N to subtract N from each redirected offset
 * (N = amount $16 has drifted from the symbol base).
 * If BIAS is not set, the redirect is skipped with a warning. */
if (ugen_no_abs_addr && a3 > 256 && a3 < 0x8000) {
    if (ugen_no_abs_bias >= 0) {
        /* Adjust offset: subtract the bias (how much $16 has drifted). */
        uint32_t adjusted = (uint32_t)((int32_t)a3 - ugen_no_abs_bias);
        uint32_t _saved_sp16 = MEM_U32(sp + 16);
        uint32_t _saved_sp20 = MEM_U32(sp + 20);
        MEM_U32(sp + 16) = 16;  /* base reg byte: 16 for $16 (s0) */
        MEM_U32(sp + 20) = 0;   /* clear XOR metadata for offset field */
        fprintf(stderr, "[UGEN-NOABS] emit_ra->rab: op=0x%x rt=%d off=%d adj=%d bias=%d base=0($16)\n",
                a0, a1, a3, adjusted, ugen_no_abs_bias);
        f_emit_rab(mem, sp, a0, a1, adjusted, 0);
        MEM_U32(sp + 16) = _saved_sp16;
        MEM_U32(sp + 20) = _saved_sp20;
        return;
    } else {
        /* No bias specified -- can't safely redirect. Warn and fall through. */
        fprintf(stderr, "[UGEN-NOABS] emit_ra: op=0x%x rt=%d off=%d SKIPPED (set IDO52_UGEN_NO_ABS_BIAS)\n",
                a0, a1, a3);
    }
}
'''

# Hook for iconst nodes with large (>=0x80000000) values:
# redirect them to the register-relative path (L430d0c) instead
# of emitting an absolute address load.
# Inserted after the "at = 0x49; if ... goto L430d0c;" block.
ICONST_NOABS_HOOK = r'''
/* --- HOOK: IDO52_UGEN_NO_ABS_ADDR check iconst value --- */
{
    uint32_t _icv = MEM_U32(MEM_U32(s6 + 4) + 48);
    if (ugen_hooks_debug) fprintf(stderr, "[UGEN-NOABS] iconst-check: val=0x%08x s6=0x%08x\n", _icv, s6);
    if (ugen_no_abs_addr && _icv >= 0x80000000) {
        fprintf(stderr, "[UGEN-NOABS] REDIRECTING iconst 0x%08x to reg-relative\n", _icv);
        goto L430d0c;
    }
}
'''

# ---------------------------------------------------------------------------
# Hook 36: IDO52_UGEN_TEMP_REG
#
# Per-request FORCE of the integer temp register ugen hands out for an
# expression temp, keyed by the per-function occurrence index of
# f_get_one_free_reg requests.  This is the "ugen expression-temp
# REGISTER-CHOICE control" (UOPT-HOOKS-REFERENCE.md Hook 35 "sharpened
# wall"): the chooser is f_get_one_free_reg's free-list head pop at
# L43bcd4; the hook removes a NAMED register from the free list instead
# of the head.  Any genuinely-free register is a legal choice, so the
# steered outcome is one ugen's own allocator could have produced.
#
# Self-contained (own statics + lazy init), applied independently of the
# older hook blocks.
# ---------------------------------------------------------------------------
TEMPREG_HEADER = r'''

/* ================================================================== */
/* IDO52 ugen hook 36 -- IDO52_UGEN_TEMP_REG                          */
/*                                                                    */
/* Syntax:  IDO52_UGEN_TEMP_REG=<occ>:<reg>[,<occ>:<reg>...]          */
/*          IDO52_UGEN_TEMP_REG_TRACE=1                               */
/*                                                                    */
/*   <occ>  1-based occurrence index of an integer temp-register      */
/*          REQUEST (= a call of f_get_one_free_reg), counted per     */
/*          function (reset from f_init_regs).  This is a DIFFERENT   */
/*          index space from Hook 23's f_get_reg occurrences: it also */
/*          counts requests arriving via f_get_free_reg, and it does  */
/*          not count two-register (64-bit) requests.                 */
/*   <reg>  t0-t9 / s0-s8 register name or raw MIPS register number.  */
/*                                                                    */
/* At request <occ>, if <reg> is currently IN the integer free list,  */
/* it is removed and handed out instead of the list head.  If it is   */
/* not free the force is SKIPPED with a warning and the default head  */
/* pop proceeds -- the hook never spills, never steals a live value,  */
/* and never touches the spill path (free-list-empty requests are     */
/* traced but not forceable).                                         */
/*                                                                    */
/* TRACE prints one line per request:                                 */
/*   [UGEN-TREG36] occ=N free=[r r r ...] -> rX [FORCED|not-free]     */
/*                                                                    */
/* Default OFF: with both env vars unset ugen_tempreg_event() returns */
/* before touching any state, so codegen is bit-identical.            */
/* ================================================================== */
static void f_remove_from_free_list(uint8_t *mem, uint32_t sp, uint32_t a0);

#define UGEN_TEMPREG_MAX 64
#define UGEN_TEMPREG_DESC(r)  (0x10019520u + (uint32_t)(r) * 12u)
#define UGEN_TEMPREG_HEAD     0x10019a94u

static int ugen_tempreg_checked;
static int ugen_tempreg_on;      /* 0 = hook disabled (default) */
static int ugen_tempreg_trace;
static int ugen_tempreg_n;
static int ugen_tempreg_occ[UGEN_TEMPREG_MAX];
static int ugen_tempreg_reg[UGEN_TEMPREG_MAX];
static int ugen_tempreg_seen;    /* per-function request counter */

static int ugen_tempreg_parse_reg(const char *name) {
    if (name[0] == 't' && name[1] >= '0' && name[1] <= '9' && name[2] == '\0') {
        int n = name[1] - '0';
        if (n <= 7) return 8 + n;
        return 24 + (n - 8);
    }
    if (name[0] == 's' && name[1] >= '0' && name[1] <= '8' && name[2] == '\0') {
        if (name[1] == '8') return 30;
        return 16 + (name[1] - '0');
    }
    if (name[0] == 'v' && (name[1] == '0' || name[1] == '1') && name[2] == '\0')
        return 2 + (name[1] - '0');
    if (name[0] == 'a' && name[1] >= '0' && name[1] <= '3' && name[2] == '\0')
        return 4 + (name[1] - '0');
    return atoi(name);
}

static void ugen_tempreg_init(void) {
    char *env, *p, *tok;
    char buf[512];
    if (ugen_tempreg_checked) return;
    ugen_tempreg_checked = 1;

    env = getenv("IDO52_UGEN_TEMP_REG_TRACE");
    if (env && env[0]) ugen_tempreg_trace = 1;

    env = getenv("IDO52_UGEN_TEMP_REG");
    if (!env || !env[0]) return;
    strncpy(buf, env, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    p = buf;
    while ((tok = strtok(p, ",;")) != NULL && ugen_tempreg_n < UGEN_TEMPREG_MAX) {
        char *c;
        p = NULL;
        while (*tok == ' ') tok++;
        c = strchr(tok, ':');
        if (!c) continue;
        *c = '\0';
        ugen_tempreg_occ[ugen_tempreg_n] = atoi(tok);
        ugen_tempreg_reg[ugen_tempreg_n] = ugen_tempreg_parse_reg(c + 1);
        ugen_tempreg_n++;
    }
    if (ugen_tempreg_n > 0) {
        int i;
        ugen_tempreg_on = 1;
        fprintf(stderr, "[UGEN] TEMP_REG: %d force(s):", ugen_tempreg_n);
        for (i = 0; i < ugen_tempreg_n; i++)
            fprintf(stderr, " %d:%d", ugen_tempreg_occ[i], ugen_tempreg_reg[i]);
        fprintf(stderr, "\n");
    }
}

/* Walk the integer free list; return 1 iff reg is a member.  Membership
 * is WALKED, never read from the +7 flag (same safety rule as Hook 23:
 * the flag lies for registers reserved by other machinery). */
static int ugen_tempreg_in_list(uint8_t *mem, int reg) {
    uint32_t r = MEM_U8(UGEN_TEMPREG_HEAD + 0);
    int guard = 0;
    while (r != 0x48 && guard++ < 64) {
        if ((int)r == reg) return 1;
        r = MEM_U8(UGEN_TEMPREG_DESC(r) + 6);
    }
    return 0;
}

/* Called once per function from f_init_regs. */
static void ugen_tempreg_reset(void) {
    ugen_tempreg_init();
    ugen_tempreg_seen = 0;
}

/* Called at the entry of every f_get_one_free_reg. */
static void ugen_tempreg_event(uint8_t *mem) {
    ugen_tempreg_init();
    if (!ugen_tempreg_on && !ugen_tempreg_trace) return;
    ugen_tempreg_seen++;
    if (ugen_tempreg_trace) {
        uint32_t r = MEM_U8(UGEN_TEMPREG_HEAD + 0);
        int guard = 0;
        fprintf(stderr, "[UGEN-TREG36] occ=%d free=[", ugen_tempreg_seen);
        while (r != 0x48 && guard++ < 64) {
            fprintf(stderr, "%s%u", guard > 1 ? " " : "", r);
            r = MEM_U8(UGEN_TEMPREG_DESC(r) + 6);
        }
        fprintf(stderr, "]\n");
    }
}

/* Called on the free-list pop path (L43bcd4).  Returns the forced
 * register (already removed from the free list) or -1 for default. */
static int ugen_tempreg_force(uint8_t *mem, uint32_t sp) {
    int i, want = -1;
    if (!ugen_tempreg_on) return -1;
    for (i = 0; i < ugen_tempreg_n; i++)
        if (ugen_tempreg_occ[i] == ugen_tempreg_seen) { want = ugen_tempreg_reg[i]; break; }
    if (want < 0) return -1;
    if (!ugen_tempreg_in_list(mem, want)) {
        fprintf(stderr, "[UGEN-TREG36] occ=%d force r%d NOT FREE -- default pop\n",
                ugen_tempreg_seen, want);
        return -1;
    }
    f_remove_from_free_list(mem, sp, (uint32_t)want);
    /* f_remove_from_free_list ends in f_fill_reg(reg,0,0,1), which clears
     * the descriptor's on-a-list flag (+7).  The claim path we are about
     * to resume (f_append_to_list onto the in-use list at 0x10019a98)
     * asserts that flag is still set -- f_remove_head, the pop this force
     * replaces, never touches it.  Restore it so the claim proceeds with
     * exactly the state the default path would have. */
    MEM_U8(UGEN_TEMPREG_DESC(want) + 7) = 1;
    if (ugen_tempreg_trace)
        fprintf(stderr, "[UGEN-TREG36] occ=%d -> r%d FORCED\n",
                ugen_tempreg_seen, want);
    return want;
}

/* Called at L43bcec with the chosen register (all paths converge here). */
static void ugen_tempreg_note(uint8_t *mem, uint32_t reg) {
    if (ugen_tempreg_trace)
        fprintf(stderr, "[UGEN-TREG36] occ=%d -> r%u\n", ugen_tempreg_seen, reg);
}
'''

# Reset of the per-function request counter (same f_init_regs anchor the
# other per-function counters use).
TEMPREG_RESET = r'''
/* --- HOOK 36: reset the per-function TEMP_REG request counter --- */
ugen_tempreg_reset();
'''

# Request tick + free-list trace, at the entry of f_get_one_free_reg
# (label L43bb4c, before the frame adjustment -- only `mem` is needed).
TEMPREG_EVENT = r'''
/* --- HOOK 36: IDO52_UGEN_TEMP_REG request tick + trace --- */
ugen_tempreg_event(mem);
'''

# The force itself, on the free-list head pop path (label L43bcd4).  When
# a force fires the register has already been removed from the free list;
# jump straight to the common tail (L43bcec) with s0 = the chosen register,
# exactly as the default `f_remove_head` path would.
TEMPREG_FORCE = r'''
/* --- HOOK 36: IDO52_UGEN_TEMP_REG force (replaces the head pop) --- */
{
    int _fr = ugen_tempreg_force(mem, sp);
    if (_fr >= 0) { s0 = (uint32_t)_fr; goto L43bcec; }
}
'''

# Chosen-register trace at the common tail (label L43bcec).
TEMPREG_NOTE = r'''
/* --- HOOK 36: IDO52_UGEN_TEMP_REG chosen-register trace --- */
ugen_tempreg_note(mem, s0);
'''

# ---------------------------------------------------------------------------
# Hook 41: IDO52_UGEN_NO_TARGET
#
# The expression-result TARGETING veto: at f_eval's binary/shift arm, force
# ugen to evaluate into a free-list temp and copy to the destination item
# (`op $tX,...` + `or $dst,$tX,$zero`) instead of writing the item directly.
# Self-contained (own statics + lazy init).
# ---------------------------------------------------------------------------
TARGET_HEADER = r'''

/* ================================================================== */
/* IDO52 ugen hook 41 -- IDO52_UGEN_NO_TARGET                         */
/* the expression-result TARGETING veto                                */
/*                                                                     */
/* Syntax:  IDO52_UGEN_NO_TARGET=<occ>[,<occ>...]      (or "all")      */
/*          IDO52_UGEN_FORCE_TARGET=<occ>[,<occ>...]   (see WARNING)   */
/*          IDO52_UGEN_TARGET_TRACE=1                                  */
/*                                                                     */
/* THE SITE (f_eval, the binary/shift arm, label L4308e4):             */
/*                                                                     */
/*     s3 = f_reg(child)              // register the operand is in    */
/*     v1 = f_get_dest(node, fp)      // register the DESTINATION ITEM */
/*                                    //   lives in (uopt's colour)    */
/*     if (s3 != v1)          -> s2 = v1        DIRECT                 */
/*     else if (opcode may be in-place) -> s2 = v1   DIRECT            */
/*     else                   -> s2 = f_get_free_reg(node,1)  TEMP     */
/*     ... emit the operation with destination s2 ...                  */
/*     if (s2 != fp) emit_rr(0x31, fp, s2)   // `or $fp,$s2,$zero`     */
/*     if (s2 != fp) f_free_reg(s2)                                    */
/*                                                                     */
/* i.e. ugen either TARGETS the destination item register with the     */
/* operation itself, or evaluates into a free-list temp and copies.    */
/* `f_get_free_reg(node,1)` above is the ONLY site in ugen.c that      */
/* requests a temp with (node, 1) inside f_eval -- this decision is    */
/* unique, which is why one occurrence index suffices.                 */
/*                                                                     */
/* NO_TARGET at occurrence <occ> forces the TEMP arm, so the target's  */
/*     <op>  $tX, ...                                                  */
/*     or    $dst, $tX, $zero                                          */
/* two-instruction form is emitted where the stock compiler emits one. */
/* Instruction count +1 per vetoed site.                               */
/*                                                                     */
/* SAFETY.  The TEMP arm is one ugen takes itself (it is the arm for   */
/* opcodes that may not write a register they also read); it allocates */
/* through ugen's own allocator, and the copy that follows is ugen's   */
/* own emission, guarded by the same `s2 != fp` test.  No live value    */
/* is stolen and no instruction is fabricated.  Which register the     */
/* temp gets is Hook 36's index space (IDO52_UGEN_TEMP_REG): the veto  */
/* ADDS one f_get_one_free_reg request, so Hook 36 occurrence indices  */
/* downstream of a veto shift by one -- re-trace after enabling this.  */
/*                                                                     */
/* ⚠ FORCE_TARGET (force the DIRECT arm) is the inverse and is         */
/* CONDITIONALLY UNSOUND: the TEMP arm is only ever reached when the   */
/* operand register EQUALS the destination register AND the node's     */
/* opcode is not in ugen's may-write-a-source table (0x10005214), i.e. */
/* exactly the case where writing the destination early would clobber  */
/* an operand the expansion still has to read.  It is implemented for  */
/* probing only, is refused unless IDO52_UGEN_FORCE_TARGET_UNSAFE=1 is */
/* also set, and must never be used in a committed @CompileEnv.        */
/*                                                                     */
/* Occurrence index is 1-based, counts EVERY arrival at the decision   */
/* (both arms), and is reset per function from f_init_regs.            */
/*                                                                     */
/* TRACE prints one line per decision:                                 */
/*   [UGEN-TGT41] occ=N node=0x... op=0x52 src=t6 dst=t0 nat=DIRECT    */
/*                                                                     */
/* Default OFF: with all env vars unset ugen_target_decide() returns 0 */
/* before touching any state, so codegen is bit-identical.             */
/* ================================================================== */
#define UGEN_TARGET_MAX 64
#define UGEN_TGT_DEFAULT 0
#define UGEN_TGT_DIRECT  1
#define UGEN_TGT_TEMP    2

static int ugen_target_checked;
static int ugen_target_trace;
static int ugen_target_all;      /* NO_TARGET=all */
static int ugen_target_unsafe;   /* IDO52_UGEN_FORCE_TARGET_UNSAFE */
static int ugen_target_nno;
static int ugen_target_no[UGEN_TARGET_MAX];
static int ugen_target_nfc;
static int ugen_target_fc[UGEN_TARGET_MAX];
static int ugen_target_on;
static int ugen_target_seen;     /* per-function decision counter */

static const char *ugen_target_regname(uint32_t r) {
    static const char *n[32] = {
        "zero","at","v0","v1","a0","a1","a2","a3",
        "t0","t1","t2","t3","t4","t5","t6","t7",
        "s0","s1","s2","s3","s4","s5","s6","s7",
        "t8","t9","k0","k1","gp","sp","s8","ra"};
    if (r == 0x48) return "NONE";
    if (r < 32) return n[r];
    return "f?";
}

static int ugen_target_parse_list(const char *env, int *dst, int max) {
    char buf[512], *p, *tok;
    int n = 0;
    strncpy(buf, env, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    p = buf;
    while ((tok = strtok(p, ",;")) != NULL && n < max) {
        p = NULL;
        while (*tok == ' ') tok++;
        dst[n++] = atoi(tok);
    }
    return n;
}

static void ugen_target_init(void) {
    char *env;
    if (ugen_target_checked) return;
    ugen_target_checked = 1;

    env = getenv("IDO52_UGEN_TARGET_TRACE");
    if (env && env[0]) ugen_target_trace = 1;
    env = getenv("IDO52_UGEN_FORCE_TARGET_UNSAFE");
    if (env && env[0]) ugen_target_unsafe = 1;

    env = getenv("IDO52_UGEN_NO_TARGET");
    if (env && env[0] && strcmp(env, "0") != 0) {
        if (strcmp(env, "all") == 0)
            ugen_target_all = 1;
        else
            ugen_target_nno = ugen_target_parse_list(env, ugen_target_no,
                                                     UGEN_TARGET_MAX);
        ugen_target_on = 1;
        fprintf(stderr, "[UGEN] NO_TARGET=%s (%d occ%s)\n", env,
                ugen_target_nno, ugen_target_all ? ", ALL" : "");
    }

    env = getenv("IDO52_UGEN_FORCE_TARGET");
    if (env && env[0] && strcmp(env, "0") != 0) {
        if (!ugen_target_unsafe) {
            fprintf(stderr, "[UGEN] FORCE_TARGET=%s REFUSED "
                    "(set IDO52_UGEN_FORCE_TARGET_UNSAFE=1 to probe; the "
                    "direct arm can clobber an operand the expansion still "
                    "reads)\n", env);
        } else {
            ugen_target_nfc = ugen_target_parse_list(env, ugen_target_fc,
                                                     UGEN_TARGET_MAX);
            if (ugen_target_nfc > 0) {
                ugen_target_on = 1;
                fprintf(stderr, "[UGEN] FORCE_TARGET=%s (%d occ) UNSAFE-OK\n",
                        env, ugen_target_nfc);
            }
        }
    }
}

/* Called once per function from f_init_regs. */
static void ugen_target_reset(void) {
    ugen_target_init();
    ugen_target_seen = 0;
}

/* Called at f_eval's binary/shift result-targeting decision.
 *   node = the ucode node (`s6`)
 *   src  = the operand register f_reg returned (`s3`)
 *   dst  = the destination-item register f_get_dest returned (`v1`)
 *   nat  = the arm ugen would take by itself (UGEN_TGT_DIRECT/TEMP)
 * Returns UGEN_TGT_DEFAULT to leave the decision alone, or the arm to
 * take instead. */
static int ugen_target_decide(uint8_t *mem, uint32_t node, uint32_t src,
                              uint32_t dst, int nat) {
    int i, out = UGEN_TGT_DEFAULT;
    ugen_target_init();
    if (!ugen_target_on && !ugen_target_trace) return UGEN_TGT_DEFAULT;
    ugen_target_seen++;
    if (ugen_target_on) {
        if (ugen_target_all) {
            out = UGEN_TGT_TEMP;
        } else {
            for (i = 0; i < ugen_target_nno; i++)
                if (ugen_target_no[i] == ugen_target_seen) {
                    out = UGEN_TGT_TEMP;
                    break;
                }
            for (i = 0; i < ugen_target_nfc; i++)
                if (ugen_target_fc[i] == ugen_target_seen) {
                    out = UGEN_TGT_DIRECT;
                    break;
                }
        }
        if (out == nat) out = UGEN_TGT_DEFAULT;
    }
    if (ugen_target_trace)
        fprintf(stderr,
                "[UGEN-TGT41] occ=%d node=0x%x op=0x%02x src=%s dst=%s "
                "nat=%s%s\n",
                ugen_target_seen, node, node ? MEM_U8(node + 32) : 0xff,
                ugen_target_regname(src), ugen_target_regname(dst),
                nat == UGEN_TGT_TEMP ? "TEMP" : "DIRECT",
                out == UGEN_TGT_TEMP ? "  -> TEMP [HOOK41]"
                    : (out == UGEN_TGT_DIRECT ? "  -> DIRECT [HOOK41]" : ""));
    return out;
}

/* --- HOOK 41: replica of ugen's own in-place predicate (see below) --- */
static int ugen_target_natural(uint8_t *mem, uint32_t node, uint32_t src,
                               uint32_t dst) {
    uint32_t op, w;
    if ((src & 0xff) != (dst & 0xff)) return UGEN_TGT_DIRECT;
    op = MEM_U8(node + 32);
    if (op >= 0x80) return UGEN_TGT_TEMP;
    w = MEM_U32(0x10005214u + (uint32_t)(op >> 5) * 4u);
    if ((int32_t)(w << (op & 0x1f)) < 0) return UGEN_TGT_DIRECT;
    return UGEN_TGT_TEMP;
}
'''

# Reset of the per-function decision counter (the f_init_regs anchor the
# other per-function counters already share).
TARGET_RESET = r'''
/* --- HOOK 41: reset the per-function targeting-decision counter --- */
ugen_target_reset();
'''

# The veto itself, in f_eval right after the `v1 = v0 & 0xff;` that follows
# label L4308e4 (so both arms are still reachable by goto).
TARGET_HOOK = r'''
/* --- HOOK 41: IDO52_UGEN_NO_TARGET expression-result targeting veto --- */
/* s3 = the operand register, v1 = the destination item's register.  The
 * two arms below are ugen's own: L430950 targets the item directly,
 * LH41_TEMP evaluates into a free-list temp and lets the `s2 != fp`
 * test further down emit `or $fp,$s2,$zero`. */
{
    int _nat = ugen_target_natural(mem, s6, s3, v1);
    int _act = ugen_target_decide(mem, s6, s3, v1, _nat);
    if (_act == UGEN_TGT_TEMP) { fp = v0 & 0xff; goto LH41_TEMP; }
    if (_act == UGEN_TGT_DIRECT) { fp = v0 & 0xff; goto L430950; }
}
'''

# New label giving the hook an entry into ugen's own evaluate-into-a-temp
# arm; inserted immediately BEFORE the `a0 = s6;` of the unique
# f_get_free_reg(node, 1) request inside f_eval.
TARGET_TEMP_LABEL = r'''
LH41_TEMP: ;   /* HOOK 41 entry into ugen's evaluate-into-a-temp arm */
'''

# Debug log hooks for emit functions (inserted at their entry labels)
EMIT_RAB_DEBUG = 'if (ugen_hooks_debug) fprintf(stderr, "[UGEN-EMIT] emit_rab opcode=0x%x rt=%d off=0x%x base=%d\\n", a0, a1, a2, a3);\n'
EMIT_RA_DEBUG = 'if (ugen_hooks_debug) fprintf(stderr, "[UGEN-EMIT] emit_ra opcode=0x%x rt=%d addr=0x%x kind=%d\\n", a0, a1, a2, a3);\n'
EMIT_ROB_DEBUG = 'if (ugen_hooks_debug) fprintf(stderr, "[UGEN-EMIT] emit_rob opcode=0x%x rt=%d off=0x%x base=%d\\n", a0, a1, a2, a3);\n'
EMIT_RI_DEBUG = 'if (ugen_hooks_debug) fprintf(stderr, "[UGEN-EMIT] emit_ri_ opcode=0x%x rt=%d imm=0x%x kind=%d\\n", a0, a1, a2, a3);\n'
EMIT_RRI_DEBUG = 'if (ugen_hooks_debug) fprintf(stderr, "[UGEN-EMIT] emit_rri_ opcode=0x%x dst=%d src=%d imm=0x%x\\n", a0, a1, a2, a3);\n'




# =====================================================================
# Hook 42: IDO52_UGEN_DEST_TEMP -- f_get_dest destination-hint override
# (2026-08-19, the web-home UCP copy campaign's site-(b) probe/primitive).
#
#   IDO52_UGEN_DEST_TEMP=<occ>[,<occ>...]   force hint 0x48 (free temp)
#   IDO52_UGEN_DEST_TRACE=1                 one line per f_get_dest call
#
# f_get_dest(node, hint): hint == 0x48 allocates a free-list temp and
# binds the node to it; any other hint claims that register for the
# node.  Every ugen expression arm chooses direct-vs-temp by the HINT it
# passes.  Forcing 0x48 at a keyed occurrence makes the value land in a
# colourless temp -- the multi-home materialisation shape the original
# compiler uses (one temp + one UCP copy per home).  The override never
# invents an instruction: it selects the allocate-a-temp path f_get_dest
# itself implements, and every downstream copy is ugen's own emission.
# SOUNDNESS CAVEAT (why this is probe-grade until a consumer validates):
# a caller that passes a real hint MAY assume the return equals the
# hint; the win gates (resolver + semcheck + audits) are the arbiter on
# every consumer.  Occurrence is 1-based, counted per function (reset
# alongside Hook 41's counter).  Default OFF: env unset returns before
# touching state; codegen bit-identical.
# =====================================================================
DEST42_HEADER = r'''
/* --- HOOK 42: IDO52_UGEN_DEST_TEMP f_get_dest hint override --- */
#define UGEN_DEST42_MAX 64
static int ugen_dest42_checked;
static int ugen_dest42_trace;
static int ugen_dest42_occ[UGEN_DEST42_MAX];
static int ugen_dest42_count;
static int ugen_dest42_n;

static void ugen_dest42_init(void) {
    char *env, buf[512], *tok;
    if (ugen_dest42_checked) return;
    ugen_dest42_checked = 1;
    env = getenv("IDO52_UGEN_DEST_TRACE");
    if (env && *env) ugen_dest42_trace = 1;
    env = getenv("IDO52_UGEN_DEST_TEMP");
    if (env && *env) {
        strncpy(buf, env, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = 0;
        tok = strtok(buf, ",");
        while (tok && ugen_dest42_count < UGEN_DEST42_MAX)
            { ugen_dest42_occ[ugen_dest42_count++] = atoi(tok);
              tok = strtok(NULL, ","); }
        fprintf(stderr, "[UGEN-DEST42] %d force site(s)\n", ugen_dest42_count);
    }
}

static void ugen_dest42_reset(void) { ugen_dest42_n = 0; }

/* returns 1 to force hint 0x48; ticks the occurrence on every arrival */
static int ugen_dest42_decide(uint8_t *mem, uint32_t node, int hint) {
    int i, n, force = 0;
    ugen_dest42_init();
    if (!ugen_dest42_trace && !ugen_dest42_count) return 0;
    n = ++ugen_dest42_n;
    for (i = 0; i < ugen_dest42_count; i++)
        if (ugen_dest42_occ[i] == n) { force = 1; break; }
    if (force && hint == 0x48) force = 0;   /* already the temp path */
    return force;
}

static void ugen_dest42_trace_ret(uint8_t *mem, uint32_t node, int hint,
                                  int ret, int forced) {
    if (!ugen_dest42_trace) return;
    fprintf(stderr, "[UGEN-DEST42] occ=%d node=0x%x op=0x%02x hint=%d "
            "ret=%d%s\n", ugen_dest42_n, node, MEM_U8(node + 0), hint,
            ret, forced ? " [FORCED->0x48]" : "");
}
'''

DEST42_DEF_OLD = "static uint32_t f_get_dest(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1) {\n"

DEST42_DEF_NEW = r'''static uint32_t f_get_dest_orig(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1);
static uint32_t f_get_dest(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1) {
    /* HOOK 42 wrapper: keyed hint override, trace */
    uint32_t r;
    int forced = ugen_dest42_decide(mem, a0, (int)(a1 & 0xff));
    if (forced) a1 = 0x48;
    r = f_get_dest_orig(mem, sp, a0, a1);
    ugen_dest42_trace_ret(mem, a0, (int)(a1 & 0xff), (int)(r & 0xff), forced);
    return r;
}
static uint32_t f_get_dest_orig(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1) {
'''


# =====================================================================
# Hook 36b: IDO52_UGEN_TEMP_REG_INPLACE -- let a Hook 36 force name a
# register that is NOT a pool temp (an argument register holding the
# operand being evaluated).  The register never joins the temp lists: the
# call site skips f_append_to_list and only binds the node with f_fill_reg.
# Hook 41b: IDO52_UGEN_FORCE_TARGET_SAFE=<occ,...>|all -- the SOUND subset
# of FORCE_TARGET: force the DIRECT arm only when src == dst and the node's
# U-code op expands to ONE instruction that reads every source before
# writing the destination (0x04 and, 0x3c or, 0x73 sll, 0x74 sra).  This is
# retail's `sll a0,a0,16; sra a0,a0,16` / `andi a2,a2,0xffff` narrow-
# parameter form (2026-09-03; first consumer main/get_mode).  Both
# default OFF: env unset is bit-identical.
# =====================================================================
H36B_FORCE_OLD = """    if (!ugen_tempreg_in_list(mem, want)) {
        fprintf(stderr, "[UGEN-TREG36] occ=%d force r%d NOT FREE -- default pop\\n",
                ugen_tempreg_seen, want);
        return -1;
    }
    f_remove_from_free_list(mem, sp, (uint32_t)want);"""
H36B_FORCE_NEW = """    ugen_tempreg_inplace_hit = 0;
    if (!ugen_tempreg_in_list(mem, want)) {
        /* HOOK 36b (IDO52_UGEN_TEMP_REG_INPLACE=1): a forced register that
         * is NOT a pool temp (an argument register holding the operand of
         * the expression being evaluated) is handed out WITHOUT joining the
         * temp lists: the caller skips f_append_to_list and only binds the
         * node with f_fill_reg.  Retail's ugen evaluates a narrowing
         * conversion of a parameter in place (`sll a0,a0,16; sra a0,a0,16`,
         * `andi a1,a1,0xffff`); ours pops a temp.  Sound only when the old
         * value in that register is dead once the expression is evaluated,
         * which the resolver/byte gates establish per consumer. */
        if (ugen_tempreg_inplace) {
            ugen_tempreg_inplace_hit = 1;
            if (ugen_tempreg_trace)
                fprintf(stderr, "[UGEN-TREG36] occ=%d -> r%d FORCED IN PLACE (not a pool temp)\\n",
                        ugen_tempreg_seen, want);
            return want;
        }
        fprintf(stderr, "[UGEN-TREG36] occ=%d force r%d NOT FREE -- default pop\\n",
                ugen_tempreg_seen, want);
        return -1;
    }
    f_remove_from_free_list(mem, sp, (uint32_t)want);"""
H36B_CALL_OLD = """    int _fr = ugen_tempreg_force(mem, sp);
    if (_fr >= 0) { s0 = (uint32_t)_fr; goto L43bcec; }"""
H36B_CALL_NEW = """    int _fr = ugen_tempreg_force(mem, sp);
    if (_fr >= 0 && ugen_tempreg_inplace_hit) { s0 = (uint32_t)_fr; ugen_tempreg_note(mem, s0); goto L43bcfc; }
    if (_fr >= 0) { s0 = (uint32_t)_fr; goto L43bcec; }"""
H41B_STATIC_OLD = "static int ugen_target_unsafe;   /* IDO52_UGEN_FORCE_TARGET_UNSAFE */"
H41B_STATIC_NEW = """static int ugen_target_unsafe;   /* IDO52_UGEN_FORCE_TARGET_UNSAFE */
static int ugen_target_safe_all;             /* IDO52_UGEN_FORCE_TARGET_SAFE=all */
static int ugen_target_nsafe;
static int ugen_target_safe[UGEN_TARGET_MAX]; /* IDO52_UGEN_FORCE_TARGET_SAFE=occ,... */
static int ugen_target_safe_div;             /* IDO52_UGEN_FORCE_TARGET_SAFE_DIV=1 */
/* Hook 41b whitelist: U-code ops whose in-place expansion is ONE MIPS
 * instruction that reads every source before writing the destination, so
 * `op $rX,$rX,...` is always sound: 0x04 and (andi), 0x3c or (ori), 0x73 sll,
 * 0x74 sra.
 *
 * 0x1d div is admitted only under IDO52_UGEN_FORCE_TARGET_SAFE_DIV=1.  It is
 * a MACRO, not one instruction, so it needs its own argument -- but every
 * expansion we have copies the dividend out before touching the quotient
 * register, which makes `div $rX,$rX,K` sound too:
 *   as1 (ours AND the IRIX 5.2 as1 binary under qemu-irix, both checked on
 *        main/get_dx): `bgez rX,L ; move $at,rX ; addiu $at,rX,K-1 ;
 *        L: sra rX,$at,k` -- rX is read in the branch and in the slot.
 *   ASPSX / gccwrap --assembler gas: `addu $at,rX,$0 ; bgez rX,L ;
 *        addiu $at,$at,K-1 ; L: sra rX,$at,k` -- same.
 *   non-pow2 / register divisor: `div $0,rX,rt ; mflo rX` -- the quotient
 *        register is written only by the mflo.
 * It stays opt-in because the argument is about the ASSEMBLER's macro
 * expansion rather than about a single opcode, which is a weaker guarantee
 * than the other four have. */
static int ugen_target_safe_op(uint32_t op) {
    return op == 0x04 || op == 0x3c || op == 0x73 || op == 0x74
        || (ugen_target_safe_div && op == 0x1d);
}"""
H41B_DECIDE_OLD = """        if (out == nat) out = UGEN_TGT_DEFAULT;
    }
    if (ugen_target_trace)"""
H41B_DECIDE_NEW = """        if (out == UGEN_TGT_DEFAULT && (ugen_target_safe_all || ugen_target_nsafe)
            && (src & 0xff) == (dst & 0xff) && node && ugen_target_safe_op(MEM_U8(node + 32))) {
            int hit = ugen_target_safe_all;
            for (i = 0; i < ugen_target_nsafe && !hit; i++)
                if (ugen_target_safe[i] == ugen_target_seen) hit = 1;
            if (hit) out = UGEN_TGT_DIRECT;   /* Hook 41b */
        }
        if (out == nat) out = UGEN_TGT_DEFAULT;
    }
    if (ugen_target_trace)"""
H41B_ENV = ('    env = getenv("IDO52_UGEN_FORCE_TARGET_SAFE");\n'
            '    if (env && env[0]) {\n'
            '        if (!strcmp(env, "all")) ugen_target_safe_all = 1;\n'
            '        else ugen_target_nsafe = ugen_target_parse_list(env, ugen_target_safe, UGEN_TARGET_MAX);\n'
            '        ugen_target_on = 1;\n'
            '        fprintf(stderr, "[UGEN] FORCE_TARGET_SAFE=%s (shift/and in-place only)\\n", env);\n'
            '    }\n'
            '    env = getenv("IDO52_UGEN_FORCE_TARGET_SAFE_DIV");\n'
            '    if (env && env[0] && env[0] != \'0\') {\n'
            '        ugen_target_safe_div = 1;\n'
            '        fprintf(stderr, "[UGEN] FORCE_TARGET_SAFE_DIV=1 (pow2/div macro in place)\\n");\n'
            '    }\n')


def apply_hook36b_41b(content):
    """Post-pass: Hooks 36b/41b on content that already carries 36 and 41."""
    if "ugen_tempreg_inplace" in content:
        return content, False
    reps = [
        ("static int ugen_tempreg_on;",
         "static int ugen_tempreg_inplace;      /* IDO52_UGEN_TEMP_REG_INPLACE=1 (Hook 36b) */\n"
         "static int ugen_tempreg_inplace_hit;  /* set by the force when in-place applied */\n"
         "static int ugen_tempreg_on;"),
        ('    env = getenv("IDO52_UGEN_TEMP_REG");',
         '    env = getenv("IDO52_UGEN_TEMP_REG_INPLACE");\n'
         "    if (env && *env && *env != '0') ugen_tempreg_inplace = 1;\n"
         '    env = getenv("IDO52_UGEN_TEMP_REG");'),
        (H36B_FORCE_OLD, H36B_FORCE_NEW),
        (H36B_CALL_OLD, H36B_CALL_NEW),
        (H41B_STATIC_OLD, H41B_STATIC_NEW),
        (H41B_DECIDE_OLD, H41B_DECIDE_NEW),
    ]
    for old, new in reps:
        n = content.count(old)
        assert n == 1, "hook 36b/41b anchor x%d: %r" % (n, old[:60])
        content = content.replace(old, new, 1)
    i = content.index('static void ugen_target_init(void) {')
    j = content.index('if (env && env[0]) ugen_target_trace = 1;', i)
    j = content.index('\n', j) + 1
    content = content[:j] + H41B_ENV + content[j:]
    return content, True


# =====================================================================
# Hook 36c: per-occurrence IDO52_UGEN_TEMP_REG_INPLACE + in-place safety
# check (2026-09-25).
#
# The defect: Hook 36b's IDO52_UGEN_TEMP_REG_INPLACE=1 is GLOBAL, so EVERY
# Hook 36 force whose register is not in ugen's free list is handed out in
# place -- including registers a coloured uopt web holds.  On
# main/mascot_move that grabbed t2..t5 thirteen times and computed
# `andi t3,v0,0x3f` for `& 0x7f` behind a 178/238 score.
#
#  (1) IDO52_UGEN_TEMP_REG_INPLACE=<occ>[,<occ>...]  or  =occ:<occ>[,...]
#      only the listed Hook 36 occurrences may go in place.  `=1` and `=all`
#      keep the old global behaviour (so the shipped `=1` files are
#      untouched); to name occurrence 1 alone write `occ:1`.  `=0` = off.
#  (2) a safety check on every in-place grab (both modes).  REFUSED (the
#      force falls back to the default head pop) when the register is
#        - on ugen's in-use temp list (a live ugen expression temp),
#        - a ugen-tracked register with a pending use count (desc +4),
#        - a t-register outside ugen's temp pool (t-registers are withheld
#          from the pool only because uopt coloured webs into them), or
#        - an s-register (never a ugen temp: a coloured web, or unsaved).
#      ugen has NO per-point liveness for uopt webs: the t-/s-register test
#      is function-granular (conservative), and v0/v1/a0-a3 grabs cannot be
#      checked at all -- those print INPLACE UNCHECKED and their soundness
#      still rests on the byte/resolver gates.
#  (3) trace lines: INPLACE REFUSED / INPLACE UNCHECKED / INPLACE NOT LISTED
#      (the last two only under IDO52_UGEN_TEMP_REG_TRACE=1; REFUSED always).
# Env unset: bit-identical (the new code only runs inside the in-place
# branch, which requires the env var).
# =====================================================================
H36C_STATIC_OLD = "static int ugen_tempreg_inplace_hit;  /* set by the force when in-place applied */"
H36C_STATIC_NEW = """static int ugen_tempreg_inplace_hit;  /* set by the force when in-place applied */
/* Hook 36c: IDO52_UGEN_TEMP_REG_INPLACE=<occ>,... -- the listed occurrences
 * only (n == 0 with ugen_tempreg_inplace set = global, the old `=1`). */
static int ugen_tempreg_trace;         /* tentative; defined with Hook 36 below */
static int ugen_tempreg_inplace_n;
static int ugen_tempreg_inplace_occ[64];
static int ugen_tempreg_inplace_listed(int occ) {
    int i;
    if (ugen_tempreg_inplace_n == 0) return 1;
    for (i = 0; i < ugen_tempreg_inplace_n; i++)
        if (ugen_tempreg_inplace_occ[i] == occ) return 1;
    return 0;
}
static int ugen_tempreg_list_has(uint8_t *mem, uint32_t head, int reg) {
    uint32_t r = MEM_U8(head + 0);
    int guard = 0;
    while (r != 0x48 && guard++ < 64) {
        if ((int)r == reg) return 1;
        r = MEM_U8(0x10019520u + r * 12u + 6);
    }
    return 0;
}
/* Returns 1 = refuse, 0 = allow.  Prints REFUSED / UNCHECKED. */
static int ugen_tempreg_inplace_check(uint8_t *mem, int occ, int reg, uint32_t node) {
    uint32_t d = 0x10019520u + (uint32_t)reg * 12u;
    uint32_t held = MEM_U32(d + 0);
    int uses = MEM_U16(d + 4);
    int is_t = (reg >= 8 && reg <= 15) || reg == 24 || reg == 25;
    int is_s = (reg >= 16 && reg <= 23) || reg == 30;
    if (ugen_tempreg_list_has(mem, 0x10019a98u, reg)) {
        fprintf(stderr, "[UGEN-TREG36] INPLACE REFUSED occ=%d reg=r%d: live ugen temp "
                "(on the in-use list, uses=%d)\\n", occ, reg, uses);
        return 1;
    }
    if (uses != 0) {
        /* The descriptor names the tree node whose value the register holds
         * (+0) and its pending use count (+4).  Two holdings are harmless:
         * the very node being allocated (the value is already there --
         * main/get_alarm's call result in v0), or an operand of it on its LAST
         * use (it dies in this expression).  Anything else is live. */
        if (held == node) {
            if (ugen_tempreg_trace)
                fprintf(stderr, "[UGEN-TREG36] INPLACE OK occ=%d reg=r%d: already holds "
                        "this node (uses=%d)\\n", occ, reg, uses);
            return 0;
        }
        if (node && uses == 1 && held != 0
            && (held == MEM_U32(node + 0) || held == MEM_U32(node + 4))) {
            if (ugen_tempreg_trace)
                fprintf(stderr, "[UGEN-TREG36] INPLACE OK occ=%d reg=r%d: holds an operand "
                        "on its last use\\n", occ, reg);
            return 0;
        }
        fprintf(stderr, "[UGEN-TREG36] INPLACE REFUSED occ=%d reg=r%d: ugen descriptor "
                "holds another value (node %08x) with %d pending use(s)\\n",
                occ, reg, held, uses);
        return 1;
    }
    if (is_t) {
        fprintf(stderr, "[UGEN-TREG36] INPLACE REFUSED occ=%d reg=r%d: live in web ? -- "
                "t-register withheld from ugen's temp pool, i.e. reserved by uopt's "
                "colouring (ugen has no per-point web liveness; function-granular)\\n",
                occ, reg);
        return 1;
    }
    if (is_s) {
        fprintf(stderr, "[UGEN-TREG36] INPLACE REFUSED occ=%d reg=r%d: s-register is never a "
                "ugen temp (a coloured uopt web, or not saved by the prologue)\\n", occ, reg);
        return 1;
    }
    if (ugen_tempreg_trace)
        fprintf(stderr, "[UGEN-TREG36] INPLACE UNCHECKED occ=%d reg=r%d: v/a register -- "
                "ugen has no liveness for uopt webs here; soundness rests on the "
                "byte/resolver gates\\n", occ, reg);
    return 0;
}"""
H36C_ENV_OLD = "    if (env && *env && *env != '0') ugen_tempreg_inplace = 1;"
H36C_ENV_NEW = """    if (env && *env && *env != '0') {
        /* Hook 36c: `1` / `all` = global (Hook 36b); otherwise an occurrence
         * list, optionally prefixed `occ:` (write `occ:1` for occurrence 1). */
        const char *q = env;
        ugen_tempreg_inplace = 1;
        if (!strncmp(q, "occ:", 4)) q += 4;
        else if (!strcmp(q, "1") || !strcmp(q, "all")) q = NULL;
        while (q && *q && ugen_tempreg_inplace_n < 64) {
            while (*q == ',' || *q == ';' || *q == ' ') q++;
            if (*q < '0' || *q > '9') break;
            ugen_tempreg_inplace_occ[ugen_tempreg_inplace_n++] = atoi(q);
            while (*q >= '0' && *q <= '9') q++;
        }
        if (q && ugen_tempreg_inplace_n == 0) ugen_tempreg_inplace = 0;
        if (ugen_tempreg_inplace_n > 0) {
            int _i;
            fprintf(stderr, "[UGEN] TEMP_REG_INPLACE: %d listed occurrence(s):",
                    ugen_tempreg_inplace_n);
            for (_i = 0; _i < ugen_tempreg_inplace_n; _i++)
                fprintf(stderr, " %d", ugen_tempreg_inplace_occ[_i]);
            fprintf(stderr, "\\n");
        }
    }"""
H36C_FORCE_OLD = """        if (ugen_tempreg_inplace) {
            ugen_tempreg_inplace_hit = 1;"""
H36C_FORCE_NEW = """        if (ugen_tempreg_inplace && !ugen_tempreg_inplace_listed(ugen_tempreg_seen)) {
            if (ugen_tempreg_trace)
                fprintf(stderr, "[UGEN-TREG36] INPLACE NOT LISTED occ=%d reg=r%d\\n",
                        ugen_tempreg_seen, want);
        } else if (ugen_tempreg_inplace
                   && ugen_tempreg_inplace_check(mem, ugen_tempreg_seen, want,
                                              MEM_U32(sp + 32))) {
            /* Hook 36c: refused -- fall through to the default pop. */
        } else if (ugen_tempreg_inplace) {
            ugen_tempreg_inplace_hit = 1;"""


def apply_hook36c(content):
    """Post-pass: Hook 36c on content that already carries 36b."""
    if "ugen_tempreg_inplace_listed" in content:
        return content, False
    for old, new in ((H36C_STATIC_OLD, H36C_STATIC_NEW),
                     (H36C_ENV_OLD, H36C_ENV_NEW),
                     (H36C_FORCE_OLD, H36C_FORCE_NEW)):
        n = content.count(old)
        assert n == 1, "hook 36c anchor x%d: %r" % (n, old[:60])
        content = content.replace(old, new, 1)
    return content, True


# =====================================================================
# Hook 41b-EQU: IDO52_UGEN_FORCE_TARGET_SAFE_EQU=1 (2026-09-26) -- widen
# Hook 41b's in-place whitelist with U-code op 0x23 EQU against an
# immediate (`== K`, `!x`), which ugen hands to as1 as the macro
# `seq rX,rX,K`.
#
# Why it is sound.  The Hook 41 decision site is ugen's binary-op-with-
# immediate arm (f_eval, L4308bc..L430a60): src = f_reg(child 0), dst =
# f_get_dest(node).  BOTH arms overwrite dst with the node's value -- the
# TEMP arm computes into a pool temp and then emits `or dst,temp,$0`, the
# DIRECT arm computes straight into dst -- so the liveness of dst's old
# value after the node is ugen's own decision and is identical in both
# arms; Hook 41b never changes it (this is what the and/or/sll/sra entries
# already rely on).  The only question is whether the expansion reads its
# source after writing its destination, and as1's seq-with-immediate
# expansions all read src first:
#     K == 0:          sltiu rX,rX,1
#     0 < K < 0x10000: xori  rX,rX,K ; sltiu rX,rX,1
#     wide K:          li $at,K ; xor rX,rX,$at ; sltiu rX,rX,1
# The first word reads rX before any write; later words read only the
# value just written.  64-bit (Idt/Kdt/Wdt) operands are excluded -- their
# f_dw_emit_rri expansion works on a register pair and is not audited.
# Opt-in (separate env, like _DIV) so no existing FORCE_TARGET_SAFE user
# changes; needs FORCE_TARGET_SAFE set as well (it widens the whitelist, it
# does not enable the hook).  First consumer: GYOZI/func_801401BC term 5
# (retail `xor t5,.. ; sltiu t5,t5,1`; trace `op=0x23 nat=TEMP`).
# Default OFF: env unset is bit-identical.
# =====================================================================
H41E_STATIC_OLD = "static int ugen_target_safe_div;             /* IDO52_UGEN_FORCE_TARGET_SAFE_DIV=1 */\n"
H41E_STATIC_NEW = (H41E_STATIC_OLD +
    "static int ugen_target_safe_equ;             /* IDO52_UGEN_FORCE_TARGET_SAFE_EQU=1 (Hook 41b-EQU) */\n")
H41E_FN_OLD = """static int ugen_target_safe_op(uint32_t op) {
    return op == 0x04 || op == 0x3c || op == 0x73 || op == 0x74
        || (ugen_target_safe_div && op == 0x1d);
}
"""
H41E_FN_NEW = H41E_FN_OLD + """/* Hook 41b-EQU: node-level whitelist.  0x23 EQU-vs-immediate is `seq
 * rX,rX,K`, whose as1 expansion reads rX in its first word (sltiu / xori /
 * xor-with-$at) and afterwards only the value it wrote.  64-bit operand
 * types (Idt 5, Kdt 7, Wdt 15: ugen's own 0x5010000 dword mask) excluded. */
static int ugen_target_dw_type(uint32_t t) {
    t &= 0x1f;
    return (int32_t)(0x5010000u << t) < 0;
}
static int ugen_target_safe_node(uint8_t *mem, uint32_t node) {
    uint32_t op = MEM_U8(node + 32), c0, c1;
    if (ugen_target_safe_op(op)) return 1;
    if (!ugen_target_safe_equ || op != 0x23) return 0;
    c0 = MEM_U32(node + 0);
    c1 = MEM_U32(node + 4);
    if (!c0 || !c1) return 0;
    if (ugen_target_dw_type(MEM_U8(c0 + 33)) || ugen_target_dw_type(MEM_U8(c1 + 33)))
        return 0;
    return 1;
}
"""
H41E_CALL_OLD = "&& (src & 0xff) == (dst & 0xff) && node && ugen_target_safe_op(MEM_U8(node + 32))) {"
H41E_CALL_NEW = "&& (src & 0xff) == (dst & 0xff) && node && ugen_target_safe_node(mem, node)) {"
H41E_ENV_OLD = """        fprintf(stderr, "[UGEN] FORCE_TARGET_SAFE_DIV=1 (pow2/div macro in place)\\n");
    }
"""
H41E_ENV_NEW = H41E_ENV_OLD + """    env = getenv("IDO52_UGEN_FORCE_TARGET_SAFE_EQU");
    if (env && env[0] && env[0] != '0') {
        ugen_target_safe_equ = 1;
        fprintf(stderr, "[UGEN] FORCE_TARGET_SAFE_EQU=1 (seq-vs-immediate in place)\\n");
    }
"""


def apply_hook41b_equ(content):
    """Post-pass: Hook 41b-EQU on content that already carries 41b."""
    if "ugen_target_safe_equ" in content:
        return content, False
    for old, new in ((H41E_STATIC_OLD, H41E_STATIC_NEW),
                     (H41E_FN_OLD, H41E_FN_NEW),
                     (H41E_CALL_OLD, H41E_CALL_NEW),
                     (H41E_ENV_OLD, H41E_ENV_NEW)):
        n = content.count(old)
        assert n == 1, "hook 41b-EQU anchor x%d: %r" % (n, old[:60])
        content = content.replace(old, new, 1)
    return content, True


def apply_hook42(content):
    """Post-pass: Hook 42 on the final content (needs Hook 41's reset anchor)."""
    if "ugen_dest42_decide" in content:
        return content, False
    anchor_hdr = '#include "header.h"'
    assert content.count("ugen_dest42") == 0
    n_def = content.count(DEST42_DEF_OLD)
    assert n_def == 1, f"hook 42 f_get_dest definition anchor x{n_def}"
    reset_anchor = "ugen_target_reset();"
    assert content.count(reset_anchor) == 1, "hook 42 reset anchor (needs Hook 41)"
    assert content.count(anchor_hdr) >= 1
    content = content.replace(anchor_hdr, anchor_hdr + "\n" + DEST42_HEADER, 1)
    content = content.replace(DEST42_DEF_OLD, DEST42_DEF_NEW, 1)
    content = content.replace(reset_anchor,
                              reset_anchor + "\nugen_dest42_reset();", 1)
    return content, True


def apply_hooks(path):
    """Apply the ugen hooks.

    Per-hook idempotent: each hook block probes for its own marker, so a new
    hook can be added to a ugen.c that already carries the older ones without
    regenerating it from the MIPS binary.
    """
    with open(path, "r") as f:
        content = f.read()

    # --- per-hook presence probes -----------------------------------------
    want_legacy = "ugen_hooks_init" not in content
    want_argcoal = "ugen_argcoal_suppress" not in content
    want_tregwin = "ugen_tregwin_event" not in content
    want_tempreg = "ugen_tempreg_event" not in content
    want_target = "ugen_target_decide" not in content

    want_dest42 = "ugen_dest42_decide" not in content
    want_36b = "ugen_tempreg_inplace" not in content
    want_36c = "ugen_tempreg_inplace_listed" not in content
    want_41e = "ugen_target_safe_equ" not in content
    if not want_legacy and not want_argcoal and not want_tregwin \
            and not want_tempreg and not want_target and not want_dest42 and not want_36b \
            and not want_36c and not want_41e:
        print(f"[ugen hooks] Already applied to {path} (all hooks present)")
        return True
    if not want_legacy and not want_argcoal and not want_tregwin \
            and not want_tempreg and not want_target and (want_dest42 or want_36b or want_36c
                                                          or want_41e):
        content2, h42 = apply_hook42(content)
        content2, h36b = apply_hook36b_41b(content2)
        content2, h36c = apply_hook36c(content2)
        content2, h41e = apply_hook41b_equ(content2)
        if h42 or h36b or h36c or h41e:
            with open(path, "w") as f:
                f.write(content2)
            if h42:
                print("[ugen hooks] Hook 42 (IDO52_UGEN_DEST_TEMP) applied "
                      "(all earlier hooks already present)")
            if h36b:
                print("[ugen hooks] Hooks 36b/41b (TEMP_REG_INPLACE / FORCE_TARGET_SAFE) applied")
            if h36c:
                print("[ugen hooks] Hook 36c (per-occurrence TEMP_REG_INPLACE + safety check) applied")
            if h41e:
                print("[ugen hooks] Hook 41b-EQU (FORCE_TARGET_SAFE_EQU) applied")
        return True
    if not want_legacy:
        missing = []
        if want_argcoal:
            missing.append("22 (NO_ARG_COALESCE)")
        if want_tregwin:
            missing.append("23 (TREG_ORDER_WINDOW)")
        if want_tempreg:
            missing.append("36 (TEMP_REG)")
        if want_target:
            missing.append("41 (NO_TARGET)")
        print("[ugen hooks] legacy block present -- adding hook(s) "
              + ", ".join(missing) + " only")

    lines = content.split("\n")
    patched = False
    out = []
    header_inserted = not want_legacy
    argcoal_header_inserted = not want_argcoal
    argcoal_reset_inserted = not want_argcoal
    argcoal_hook_inserted = not want_argcoal
    argcoal_pending = None
    argcoal_sites_done = set()
    tregwin_header_inserted = not want_tregwin
    tregwin_reset_inserted = not want_tregwin
    tregwin_hook_inserted = not want_tregwin
    tempreg_header_inserted = not want_tempreg
    tempreg_reset_inserted = not want_tempreg
    tempreg_event_inserted = not want_tempreg
    tempreg_force_inserted = not want_tempreg
    tempreg_note_inserted = not want_tempreg
    target_header_inserted = not want_target
    target_reset_inserted = not want_target
    target_hook_inserted = not want_target
    target_temp_inserted = not want_target
    target_pending = False

    for i, line in enumerate(lines):
        # Insert hook header(s) after #include "header.h"
        if (not header_inserted or not argcoal_header_inserted
                or not tregwin_header_inserted
                or not tempreg_header_inserted
                or not target_header_inserted) and \
                line.strip() == '#include "header.h"':
            out.append(line)
            if not header_inserted:
                out.append(HOOK_HEADER.rstrip())
                header_inserted = True
            if not argcoal_header_inserted:
                out.append(ARG_COALESCE_HEADER.rstrip())
                argcoal_header_inserted = True
                patched = True
            if not tregwin_header_inserted:
                out.append(TREGWIN_HEADER.rstrip())
                tregwin_header_inserted = True
                patched = True
            if not tempreg_header_inserted:
                out.append(TEMPREG_HEADER.rstrip())
                tempreg_header_inserted = True
                patched = True
            if not target_header_inserted:
                out.append(TARGET_HEADER.rstrip())
                target_header_inserted = True
                patched = True
            continue

        # Hook s-reg count reader: "L43a1e8:" label followed by s1 = 0x10019008
        if line.strip() == "L43a1e8:":
            # Check the following lines for the s-reg read pattern.  The
            # lookahead has to clear any hook blocks already inserted at this
            # same anchor (hook 22's reset, the legacy SREG hook), so it is
            # deliberately generous -- the label itself is unique in ugen.c.
            remaining = "\n".join(lines[i:i+40])
            if "0x10019008" in remaining and "MEM_U32" in remaining:
                out.append(line)
                if not argcoal_reset_inserted:
                    out.append(ARG_COALESCE_RESET.rstrip())
                    argcoal_reset_inserted = True
                    patched = True
                if not tregwin_reset_inserted:
                    out.append(TREGWIN_RESET.rstrip())
                    tregwin_reset_inserted = True
                    patched = True
                if not tempreg_reset_inserted:
                    out.append(TEMPREG_RESET.rstrip())
                    tempreg_reset_inserted = True
                    patched = True
                if not target_reset_inserted:
                    out.append(TARGET_RESET.rstrip())
                    target_reset_inserted = True
                    patched = True
                if want_legacy:
                    out.append(SREG_HOOK.rstrip())
                    out.append(SREG_ORDER_HOOK.rstrip())
                    patched = True
                continue

        # Hook 22: suppress call-argument coalescing.  Anchors are the two
        # arms of f_eval that evaluate a statement's child into the hard
        # `ureg` destination held in `fp` (labels L42cb1c and L42d408).  The
        # hook must go AFTER the `a1 = fp;` that sets the wanted register and
        # immediately BEFORE the recursive call, so it wins.
        if want_argcoal and line.strip()[:-1] in ARG_COALESCE_SITES and \
                line.strip().endswith(":"):
            remaining = "\n".join(lines[i:i + 6])
            if "a1 = fp;" in remaining and "f_eval(mem, sp, a0, a1);" in remaining:
                argcoal_pending = ARG_COALESCE_SITES[line.strip()[:-1]]

        if argcoal_pending and line.strip() == "f_eval(mem, sp, a0, a1);":
            out.append(ARG_COALESCE_HOOK.replace("@SITE@", argcoal_pending).rstrip())
            out.append(line)
            argcoal_sites_done.add(argcoal_pending)
            argcoal_pending = None
            argcoal_hook_inserted = len(argcoal_sites_done) == len(ARG_COALESCE_SITES)
            patched = True
            continue

        # Hook 23: occurrence clamp at the entry of f_get_reg.  The label
        # L43b11c is f_get_reg's entry (verified by the //get_reg: comment
        # that follows it); inserting here runs before f_get_reg allocates
        # its own frame, so `sp` is still the caller's and the free-list
        # helpers can be called safely.
        if not tregwin_hook_inserted and line.strip() == "L43b11c:":
            if i + 1 < len(lines) and "//get_reg:" in lines[i + 1]:
                out.append(line)
                out.append(TREGWIN_HOOK.rstrip())
                tregwin_hook_inserted = True
                patched = True
                continue

        # Hook 36: temp-register request tick + force + chosen-reg trace,
        # all inside f_get_one_free_reg.  L43bb4c is its entry (verified by
        # the //get_one_free_reg: comment); L43bcd4 is the free-list head
        # pop path; L43bcec is the common tail every path converges on.
        # All three labels are unique in ugen.c.
        if not tempreg_event_inserted and line.strip() == "L43bb4c:":
            if i + 1 < len(lines) and "//get_one_free_reg:" in lines[i + 1]:
                out.append(line)
                out.append(TEMPREG_EVENT.rstrip())
                tempreg_event_inserted = True
                patched = True
                continue

        if not tempreg_force_inserted and line.strip() == "L43bcd4:":
            out.append(line)
            out.append(TEMPREG_FORCE.rstrip())
            tempreg_force_inserted = True
            patched = True
            continue

        if not tempreg_note_inserted and line.strip() == "L43bcec:":
            out.append(line)
            out.append(TEMPREG_NOTE.rstrip())
            tempreg_note_inserted = True
            patched = True
            continue

        # Hook 41: expression-result targeting veto inside f_eval.  The veto
        # goes after the FIRST `v1 = v0 & 0xff;` following label L4308e4 (both
        # of ugen's own arms are still reachable by goto there); the temp-arm
        # entry label goes in front of the unique f_get_free_reg(node, 1)
        # request (identified by `a0 = s6;` + `a1 = 0x1;` in its lead-in).
        if not target_hook_inserted and line.strip() == "L4308e4:":
            target_pending = True
            out.append(line)
            continue

        if target_pending and line.strip() == "v1 = v0 & 0xff;":
            out.append(line)
            out.append(TARGET_HOOK.rstrip())
            target_pending = False
            target_hook_inserted = True
            patched = True
            continue

        if (not target_temp_inserted
                and line.strip() == "v0 = f_get_free_reg(mem, sp, a0, a1);"):
            ctx = [x.strip() for x in lines[max(0, i - 6):i]]
            if "a0 = s6;" in ctx and "a1 = 0x1;" in ctx:
                for k in range(len(out) - 1, max(0, len(out) - 8), -1):
                    if out[k].strip() == "a0 = s6;":
                        out.insert(k, TARGET_TEMP_LABEL.rstrip())
                        target_temp_inserted = True
                        patched = True
                        break

        # Hook temp reg count reader: "L439fd8:" label followed by s1 = 0x10019010
        if want_legacy and line.strip() == "L439fd8:":
            remaining = "\n".join(lines[i:i+6])
            if "0x10019010" in remaining and "MEM_U32" in remaining:
                out.append(line)
                out.append(TREG_HOOK.rstrip())
                patched = True
                continue

        # Hook t-reg free list reorder: "L43a018:" label follows the t-reg loop
        # This is where the t-reg free list has been fully built
        if want_legacy and line.strip() == "L43a018:":
            remaining = "\n".join(lines[i:i+6])
            if "0x10019a9c" in remaining:
                out.append(TREG_ORDER_HOOK.rstrip())
                out.append(line)
                patched = True
                continue

        # --- NO_ABS_ADDR hooks ---

        # Hook f_emit_ra entry: insert redirect hook after L413c20: label
        # This is the entry label for f_emit_ra where a0..a3 are still intact.
        # The label appears on its own line followed by //emit_ra: comment.
        if want_legacy and line.strip() == "L413c20:":
            # Verify we're in f_emit_ra by checking the next line for //emit_ra:
            if i + 1 < len(lines) and "//emit_ra:" in lines[i + 1]:
                out.append(line)
                out.append(EMIT_RA_NOABS_HOOK.rstrip())
                out.append(EMIT_RA_DEBUG.rstrip())
                patched = True
                continue

        # Hook f_emit_rab entry: insert debug log after L413288: label
        if want_legacy and line.strip() == "L413288:":
            # Verify we're in f_emit_rab by checking the next line for //emit_rab:
            if i + 1 < len(lines) and "//emit_rab:" in lines[i + 1]:
                out.append(line)
                out.append(EMIT_RAB_DEBUG.rstrip())
                patched = True
                continue

        # Hook iconst node: after the "at = 0x49; if ... goto L430d0c;" block
        # This is the dispatch for iconst opcodes that loads large constants.
        # The pattern is: compare s0+32 against 0x49, branch to L430d0c if not equal.
        # We insert the hook right after that branch to intercept large address constants.
        # The 0x49 comparison is a few lines before the goto; use a lookback.
        if want_legacy and line.strip() == "goto L430d0c;}":
            # Only insert once -- look back up to 5 lines for the 0x49 pattern
            found_049 = False
            for j in range(max(0, i - 5), i):
                if "0x49" in lines[j] and "at" in lines[j]:
                    found_049 = True
                    break
            if found_049:
                out.append(line)
                out.append(ICONST_NOABS_HOOK.rstrip())
                patched = True
                continue

        out.append(line)

    if not header_inserted:
        print(f"[ugen hooks] ERROR: Could not find '#include \"header.h\"' in {path}")
        return False

    if want_argcoal and not (argcoal_header_inserted and argcoal_hook_inserted
                             and argcoal_reset_inserted):
        print("[ugen hooks] ERROR: hook 22 (NO_ARG_COALESCE) anchors not found "
              f"(header={argcoal_header_inserted} "
              f"reset={argcoal_reset_inserted} site={argcoal_hook_inserted})")
        return False

    if want_tregwin and not (tregwin_header_inserted and tregwin_reset_inserted
                             and tregwin_hook_inserted):
        print("[ugen hooks] ERROR: hook 23 (TREG_ORDER_WINDOW) anchors not found "
              f"(header={tregwin_header_inserted} "
              f"reset={tregwin_reset_inserted} site={tregwin_hook_inserted})")
        return False

    if want_tempreg and not (tempreg_header_inserted and tempreg_reset_inserted
                             and tempreg_event_inserted
                             and tempreg_force_inserted
                             and tempreg_note_inserted):
        print("[ugen hooks] ERROR: hook 36 (TEMP_REG) anchors not found "
              f"(header={tempreg_header_inserted} "
              f"reset={tempreg_reset_inserted} "
              f"event={tempreg_event_inserted} "
              f"force={tempreg_force_inserted} "
              f"note={tempreg_note_inserted})")
        return False

    if want_target and not (target_header_inserted and target_reset_inserted
                            and target_hook_inserted and target_temp_inserted):
        print("[ugen hooks] ERROR: hook 41 (NO_TARGET) anchors not found "
              f"(header={target_header_inserted} "
              f"reset={target_reset_inserted} "
              f"hook={target_hook_inserted} "
              f"temp={target_temp_inserted})")
        return False

    if not patched:
        print(f"[ugen hooks] WARNING: Header inserted but some hook points not found")
        print(f"[ugen hooks] The label names may differ in this regeneration")

    result = "\n".join(out)
    result, h42 = apply_hook42(result)
    if h42:
        print("[ugen hooks] Hook 42 (IDO52_UGEN_DEST_TEMP) applied")
    result, h36b = apply_hook36b_41b(result)
    if h36b:
        print("[ugen hooks] Hooks 36b/41b (TEMP_REG_INPLACE / FORCE_TARGET_SAFE) applied")
    result, h36c = apply_hook36c(result)
    if h36c:
        print("[ugen hooks] Hook 36c (per-occurrence TEMP_REG_INPLACE + safety check) applied")
    result, h41e = apply_hook41b_equ(result)
    if h41e:
        print("[ugen hooks] Hook 41b-EQU (FORCE_TARGET_SAFE_EQU) applied")
    with open(path, "w") as f:
        f.write(result)

    print(f"[ugen hooks] Applied to {path}")
    return True


REBUILD = [
    ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/ugen.o", "build/5.2/ugen.c"],
    ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/out/ugen.new", "build/5.2/ugen.o", "build/5.2/version_info.o",
     "build/5.2/libc_impl_52.o", "-lm"],
]


if __name__ == "__main__":
    args = [a for a in sys.argv[1:] if a != "--rebuild"]
    path = args[0] if args else UGEN_C
    if not os.path.exists(path):
        print(f"[ugen hooks] ERROR: {path} not found. Run 'make VERSION=5.2' first.")
        sys.exit(1)
    if not apply_hooks(path):
        sys.exit(1)
    if "--rebuild" in sys.argv[1:]:
        import subprocess
        here = os.path.dirname(os.path.abspath(__file__))
        for cmd in REBUILD:
            r = subprocess.run(cmd, cwd=here, capture_output=True, text=True)
            if r.returncode != 0:
                print(r.stderr[-2000:])
                sys.exit(1)
        os.replace(os.path.join(here, "build/5.2/out/ugen.new"),
                   os.path.join(here, "build/5.2/out/ugen"))
        print("[ugen hooks] ugen rebuilt")
