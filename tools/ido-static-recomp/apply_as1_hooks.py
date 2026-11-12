#!/usr/bin/env python3
"""Apply IDO52 as1 hooks to the regenerated as1.c file.

as1.c is NOT in git -- it gets regenerated from the MIPS binary by the
ido-static-recomp Makefile. This script patches the regenerated as1.c
to add environment-variable-gated hooks for controlling assembler behavior.

ALL hooks are DEFAULT OFF: with none of the environment variables set the
patched as1 is bit-identical in behaviour to the unpatched one.

Hooks added:
    IDO52_AS1_NO_BRANCH_OPT -- Skip f_do_branch_opt (branch optimization)
    IDO52_AS1_NO_SCHEDULE   -- Skip f_schedule (instruction scheduling) [CRASHES - use with caution]
    IDO52_AS1_NO_FILL       -- Skip func_42f7bc (scheduling sub-pass)
    IDO52_AS1_NO_DELAY_FILL -- Clear the delay-slot-fill flag in func_42f018
                               (NOTE: never observed to fire; see DELAY_FILL note below)
    IDO52_AS1_NO_LA_PULL    -- Veto as1's cross-basic-block instruction pull
                               (do_xbb_opt / func_4281e0), globally or per
                               occurrence.  See the long comment below.
    IDO52_AS1_FORCE_LA_PULL -- The opposite direction: keep a cross-BB pull that
                               as1's profitability heuristic decided to undo.
                               Same index space as IDO52_AS1_NO_LA_PULL.
    IDO52_AS1_LA_PULL_TRACE -- Log every cross-BB pull candidate and decision.
    IDO52_AS1_NO_COPY_PROP  -- Veto as1's pass-II copy propagation: the peephole
                               that deletes a `move $aN,$rX` by renaming the
                               defining instruction's destination register to
                               $aN.  Globally (`1`/`all`) or per candidate index
                               (`1`, `0,4`).  See the long comment below.
    IDO52_AS1_COPY_PROP_TRACE
                            -- Log every copy-propagation candidate + decision.
    IDO52_AS1_NO_PASS1_PROP -- Veto as1's PASS-I copy propagation (Hook 44):
                               the f_do_passI_opt -> func_414bf8 layer that
                               rewrites an instruction's source registers from
                               the reaching-copy table.  It runs BEFORE
                               NO_COPY_PROP's pass-II layer, which is why a
                               pass-II veto alone never preserved a
                               `move $rX,$zero` and its uses.  `all`, or per
                               candidate index (`17,18`).  SOUNDNESS: `all` is
                               always consistent; a PARTIAL veto is only sound
                               on instructions that DEFINE NO REGISTER (stores,
                               branches) -- skipping func_414bf8 for a defining
                               instruction also skips its kill of the stale
                               copy record and miscompiles.  See the long
                               comment below.
    IDO52_AS1_PASS1_PROP_TRACE
                            -- Log every pass-I candidate + decision.
    IDO52_AS1_NO_SCHED_LOAD_HOIST
                            -- Veto as1's INTRA-basic-block list scheduler when it
                               would issue a LOAD ahead of a ready STORE that comes
                               earlier in the block.  Globally (`1`/`all`) or per
                               opportunity index (`0,3`, or the `occ:N` spelling).
                               See the long comment below.
    IDO52_AS1_SCHED_LOAD_HOIST_TRACE
                            -- Log every load-over-store opportunity + decision.
    IDO52_AS1_TIE_SRC_ORDER -- At the list scheduler's EQUAL-START-TIME
                               tie-break, make the node earlier in ORIGINAL
                               program order win (overriding the critical-path
                               height compare).  `all`, `N[,N...]`, or the
                               `-N[,...]` all-except form.  See Hook 9 below.
    IDO52_AS1_TIE_TAKE      -- Force-TAKE the candidate at the listed tie-break
                               occurrence indices (same index space).
    IDO52_AS1_TIE_REJECT    -- Force-REJECT the candidate at the listed indices.
    IDO52_AS1_TIE_TRACE     -- Log every equal-start-time tie-break with both
                               nodes' positions, words and priorities.
    IDO52_AS1_FORCE_NARROW_FOLD
                            -- The INVERSE of IDO52_AS1_KEEP_ANDI (Hook 45):
                               force the pass-II narrow-op simplifier
                               (func_416ca8) to fold `andi rt,rs,imm` to the
                               move form `or rt,rs,$zero` at the listed
                               occurrences even though its own upper-bits
                               proof failed (typically because rs is defined
                               in another basic block, so as1's intra-block
                               significant-bit table says 32).  `all`, or per
                               occurrence index (`0`, `2,5`, `occ:2,5`).
                               *** SOUNDNESS IS THE CALLER'S BURDEN ***: a
                               force is correct ONLY when the source value
                               provably fits the immediate's mask on EVERY
                               path reaching the instruction.  See the long
                               comment below and UOPT-HOOKS-REFERENCE Hook 45.
    IDO52_AS1_FORCE_NARROW_FOLD_TRACE
                            -- Log every arrival at the upper-bits proof:
                               occurrence index, instruction word, position,
                               rs, the recorded significant-bit count, the
                               derived mask, the immediate and the natural
                               verdict.
    IDO52_AS1_KEEP_SEXT     -- Veto as1's pass-II sign-extension-pair fold
                               (Hook 48): func_416528 rewrites
                               `sll rX,rY,N ; sra rX,rX,N` to `move rX,rY`
                               (both halves via f_change_bb) when the
                               consumers only demand the low 32-N bits.
                               `all`, or per candidate index (`0,3`,
                               `occ:0,3`).  Subtractive: always sound.
    IDO52_AS1_KEEP_SEXT_TRACE
                            -- Log every sra arriving at the fold's test.
    IDO52_AS1_NO_BRANCH_FOLD
                            -- Veto as1's pass-II branch/def fold (Hook 49):
                               func_4179f0 rewrites a beq/bne whose compared
                               register is defined by a foldable op (e.g.
                               `xori v0,v0,1 ; beqz v0` -> `bnez v0`) and
                               lets the def die.  `all`, or per candidate
                               index.  Subtractive: always sound.
    IDO52_AS1_NO_BRANCH_FOLD_TRACE
                            -- Log every candidate (branch word, def word,
                               def's internal opcode).
    IDO52_AS1_NO_SHIFT_COMBINE
                            -- Veto as1's pass-II shift-combine (Hook 50):
                               func_41673c rewrites `sll rA,rY,M ; sll rX,rA,N`
                               to `sll rX,rY,M+N` (the first sll becomes a
                               dead move via f_change_bb) when M+N < 32.
                               `all`, or per candidate index.  Subtractive:
                               always sound.
    IDO52_AS1_NO_SHIFT_COMBINE_TRACE
                            -- Log every sll whose source's reaching def is
                               an sll (outer word, def word, summed shamt).
    IDO52_AS1_NO_JUMP_INVERT
                            -- Veto as1's jump-over-branch inversion (Hook 51):
                               func_41b5e8 (the per-procedure branch cleanup
                               run from f_traverse_bb) rewrites a block that
                               ends in `b L` where L's block is a lone
                               conditional branch `bcc X` whose FALL-THROUGH
                               is the jumping block's own successor into the
                               inverted `b!cc` retargeted past L (func_41b22c)
                               -- `b dispatch` at a function entry becomes
                               `bnez a0,dispatch+8`.  `all`, or per candidate
                               index.  Subtractive: always sound.
    IDO52_AS1_NO_JUMP_INVERT_TRACE
                            -- Log every candidate (jump word, target
                               block's branch word).
    IDO52_AS1_KEEP_DEAD_RELOAD
                            -- Veto as1's pass-II dead-instruction removal
                               (Hook 52): f_redundant (called from
                               func_419268 at L41936c) turns an instruction
                               whose destination is overwritten before any
                               use into a deleted record (f_change_bb mode
                               5) -- e.g. a `lw v1,<home>` after a call
                               that is re-read after the next call.  `all`,
                               or per candidate index (`0,3`, `occ:0,3`).
                               Subtractive (keeps a compiled word, never
                               creates one): always sound.
    IDO52_AS1_KEEP_DEAD_RELOAD_TRACE
                            -- Log every candidate f_redundant is about to
                               delete (index, instruction word, record).

After patching, rebuild with:
    cd tools/ido-static-recomp && make VERSION=5.2

Usage:
    python3 tools/ido-static-recomp/apply_as1_hooks.py [path/to/as1.c]
"""

import os
import sys


# --------------------------------------------------------------------------
# Hook 5: IDO52_AS1_NO_LA_PULL -- cross-basic-block pull veto
#
# WHERE IT LIVES
#   as1's per-procedure driver (func_41ce84 region, ~L41e8cc) runs, in order:
#       f_schedule()   per basic block           -- intra-BB list scheduling
#       f_do_xbb_opt() once for the procedure    -- CROSS-BB code motion
#   f_do_xbb_opt is a driver over seven sub-passes:
#       func_425fb0  dataflow (and_set/set_equals)     -- analysis
#       func_42679c  dataflow (or_set/is_subset)       -- analysis
#       func_426214  dataflow (and_set/set_equals)     -- analysis
#       func_426d0c  dataflow (or_set)                 -- analysis
#       func_428ab4  print_set                         -- debug dump (as1 -O>=4)
#       func_4281e0  THE TRANSFORMER (change_bb / reschedule / dump_bb)
#       func_428cec  teardown (deallocate_set / xfree)
#   func_4281e0 is the only sub-pass that moves instructions.  Its inner loop
#   is a classic "boosting"/speculative-motion driver:
#
#       fp = func_427800(pred_bb, succ_bb, bb_vec, &scratch);  /* candidate */
#       if (fp == 0) goto done;                 /* nothing movable       */
#   L428604:
#       D_10022660++;                           /* global "xbb move" ctr */
#       s5 = func_427c54(pred_bb, fp, succ_bb); /* source slot in succ   */
#       s0 = reschedule(succ_bb); s7 = reschedule(pred_bb);
#       if (profitable) { copy insn record (0x28 bytes) succ->pred;
#                         change_bb(); reschedule both; }
#       ... then re-enter L428604 via a second func_427800 call at L428984
#           to try the next candidate in the same BB pair.
#
#   L428604 is the single confluence point for BOTH candidate sources, so it
#   is the correct veto site: jumping from there to L4289b4 ("no candidate")
#   is byte-for-byte the same control flow as func_427800 having returned 0.
#
# WHY IT IS CALLED "LA PULL"
#   The blocker this hook was written for (GEKO/func_80136790,
#   SHOUGATU/func_8013F570, NAME_ENT/func_8013425C) is as1 speculating the
#   `la $aN,SYM` macro (lui+addiu, or just its lui half) out of a
#   branch-target block up into the dispatch block, where the two words fill
#   a load-use interlock and a branch delay slot that the target leaves as a
#   nop / occupies with a different instruction.  The pass is generic though:
#   it will pull anything func_427800 finds eligible.
#
# USAGE
#   IDO52_AS1_NO_LA_PULL=1      (or "all")  veto every cross-BB pull
#   IDO52_AS1_NO_LA_PULL=0,3,7             veto the 0th, 3rd and 7th pull
#                                          candidate (0-based, counted over
#                                          the whole translation unit, in the
#                                          order as1 reaches L428604)
#   IDO52_AS1_LA_PULL_TRACE=1              log every candidate to stderr with
#                                          its index -- use this to find the
#                                          index you want to veto
#
#   Vetoing candidate N also abandons the remaining candidates for that one
#   BB pair (as1 moves on to the next pair), which is exactly the semantics
#   of "as1 refuses this pull".
#
#   IDO52_AS1_FORCE_LA_PULL=<same syntax> is the opposite direction, on the
#   SAME candidate index space: as1 trial-moves the instruction, reschedules
#   both blocks, and undoes the move when the combined schedule did not get
#   shorter.  Listing a candidate here keeps the move anyway.  Use it when the
#   target pulls MORE than this build does (e.g. two `la` halves hoisted into a
#   div interlock shadow where we hoist one).
#
#   The two are complementary and can be combined; a vetoed candidate never
#   reaches the force decision, so the numbering stays stable.
# --------------------------------------------------------------------------
LA_PULL_HELPER = r'''
/* === IDO52_AS1_NO_LA_PULL / IDO52_AS1_FORCE_LA_PULL hooks: support === */
struct as1_pull_sel { int mode; int list[128]; int nlist; };   /* mode 0=off 1=all 2=list */

static void as1_pull_parse(struct as1_pull_sel *s, const char *e) {
    s->mode = 0;
    s->nlist = 0;
    if (e == NULL || e[0] == '\0') return;
    if (strcmp(e, "1") == 0 || strcmp(e, "all") == 0) {
        s->mode = 1;
        return;
    }
    {
        char *p = (char *)e;
        s->mode = 2;
        while (*p != '\0' && s->nlist < 128) {
            s->list[s->nlist++] = (int)strtol(p, &p, 0);
            while (*p == ',' || *p == ' ') p++;
        }
    }
}

static int as1_pull_hit(const struct as1_pull_sel *s, int idx) {
    int i;
    if (s->mode == 1) return 1;
    if (s->mode == 2) {
        for (i = 0; i < s->nlist; i++) if (s->list[i] == idx) return 1;
    }
    return 0;
}

static struct as1_pull_sel as1_pull_no;
static struct as1_pull_sel as1_pull_force;
static int as1_pull_trace = 0;
static int as1_pull_cur = -1;      /* candidate index currently being decided */

static void as1_pull_init(void) {
    static int inited = 0;
    if (inited) return;
    inited = 1;
    as1_pull_parse(&as1_pull_no, getenv("IDO52_AS1_NO_LA_PULL"));
    as1_pull_parse(&as1_pull_force, getenv("IDO52_AS1_FORCE_LA_PULL"));
    as1_pull_trace = getenv("IDO52_AS1_LA_PULL_TRACE") != NULL;
}

/* Called at as1's L428604 -- one call per cross-BB pull CANDIDATE.
   Returns 1 to refuse the candidate outright. */
static int as1_la_pull_veto(void) {
    static int seq = 0;
    as1_pull_init();
    as1_pull_cur = seq++;
    if (as1_pull_hit(&as1_pull_no, as1_pull_cur)) {
        if (as1_pull_trace)
            fprintf(stderr, "[AS1-LA-PULL] #%d VETOED\n", as1_pull_cur);
        return 1;
    }
    if (as1_pull_trace)
        fprintf(stderr, "[AS1-LA-PULL] #%d allowed\n", as1_pull_cur);
    return 0;
}

/* Called at as1's L4286b0 -- the UNDO path, i.e. as1's profitability heuristic
   has just decided NOT to keep the trial move.  Returning 1 diverts to the
   commit path (L4287b8) and keeps it anyway. */
static int as1_la_pull_forced(void) {
    as1_pull_init();
    if (as1_pull_hit(&as1_pull_force, as1_pull_cur)) {
        if (as1_pull_trace)
            fprintf(stderr, "[AS1-LA-PULL] #%d FORCED (heuristic said no)\n",
                    as1_pull_cur);
        return 1;
    }
    if (as1_pull_trace)
        fprintf(stderr, "[AS1-LA-PULL] #%d rejected by heuristic\n", as1_pull_cur);
    return 0;
}
'''


# --------------------------------------------------------------------------
# Hook 44: IDO52_AS1_NO_PASS1_PROP -- pass-I copy-propagation veto (2026-08-21)
#
# as1 -O2 runs TWO copy-propagation layers over the instruction list:
#
#   pass I  : f_do_passI_opt  -> func_414bf8   <-- THIS HOOK
#   pass II : f_do_passII_opt -> func_419268   <-- Hook 6 (IDO52_AS1_NO_COPY_PROP)
#
# Pass I runs first and does the register SUBSTITUTION into uses; pass II then
# removes the copy that has become dead.  Vetoing only pass II therefore never
# preserves anything -- the uses have already been rewritten.  Bisect-proven
# on the TT/func_8014C5E8 specimen: with f_do_passII_opt disabled the object
# still had `sb $zero`; with f_do_passI_opt ALSO disabled it had
# `or $v0,$zero,$zero` + `sb $v0,14/13` (the target's shape).
#
# THE CLASS THIS CLOSES.  ugen DOES emit the range-head def of a const-0 web
# (verified in ugen's binasm output, the `.G` file: the record is present and
# only its opcode field differs from a non-zero constant's `li`).  as1 pass I
# then propagates $zero into every use and pass II deletes the now-dead move.
# That is the whole of the `held_constant_zero` blocker class -- it is an as1
# transform, NOT a uopt colouring decision (globalcolor gives the web a real
# colour: measured colour 1/$v0, never colour 0).
#
# The veto is taken at the func_414bf8 CALL, returning the value the function's
# own "nothing changed" exit returns (0).  Purely subtractive: it can only
# PREVENT a rewrite, never create an instruction.
#
#   IDO52_AS1_NO_PASS1_PROP=all      veto every pass-I candidate
#   IDO52_AS1_NO_PASS1_PROP=17,18    veto candidates #17 and #18 only (0-based)
#   IDO52_AS1_PASS1_PROP_TRACE=1     log every candidate with its index
#
# SOUNDNESS.  `all` is always self-consistent (no propagation happens at all).
# A PARTIAL veto is sound only on instructions that DEFINE NO REGISTER: skipping
# func_414bf8 for a defining instruction also skips the KILL of the stale
# reaching-copy record, and the following instructions then read a register the
# table wrongly believes still holds the old value (measured: vetoing the
# `or $v0,$zero,$zero` candidate alone turned every `sb ...,N($a0)` into
# `sb ...,N($zero)`).  Prefer `all`, or index the CONSUMER instructions.
#
# Same index-shift caveat as Hook 6 / Hook 29: indices are per translation unit
# and move whenever the instruction stream changes -- re-enumerate with
# IDO52_AS1_PASS1_PROP_TRACE=1 after any source or env edit.
PASS1_PROP_HELPER = r'''
/* === IDO52_AS1_NO_PASS1_PROP hook (Hook 44): support === */
static int as1_p1p_init, as1_p1p_all, as1_p1p_trace, as1_p1p_n, as1_p1p_cnt;
static int as1_p1p_idx[64];
static int as1_pass1_prop_veto(uint32_t insn) {
    int i, hit = 0;
    if (!as1_p1p_init) {
        const char *e;
        as1_p1p_init = 1;
        as1_p1p_trace = getenv("IDO52_AS1_PASS1_PROP_TRACE") != NULL;
        e = getenv("IDO52_AS1_NO_PASS1_PROP");
        if (e && *e) {
            if (strcmp(e, "all") == 0) {
                as1_p1p_all = 1;
            } else {
                char buf[256];
                char *t;
                strncpy(buf, e, sizeof(buf) - 1);
                buf[sizeof(buf) - 1] = '\0';
                for (t = strtok(buf, ","); t && as1_p1p_cnt < 64;
                     t = strtok(NULL, ",")) {
                    as1_p1p_idx[as1_p1p_cnt++] = atoi(t);
                }
            }
        }
    }
    if (as1_p1p_all) {
        hit = 1;
    } else {
        for (i = 0; i < as1_p1p_cnt; i++) {
            if (as1_p1p_idx[i] == as1_p1p_n) { hit = 1; break; }
        }
    }
    if (as1_p1p_trace) {
        fprintf(stderr, "[AS1-PASS1-PROP] #%d insn=%08x %s\n",
                as1_p1p_n, insn, hit ? "VETOED" : "allowed");
    }
    as1_p1p_n++;
    return hit;
}
'''

# Hook 6: IDO52_AS1_NO_COPY_PROP -- pass-II copy-propagation veto (2026-07-27)
#
# THE BLOCKER CLASS IT ADDRESSES: `phi_copy` / "live-range-split" -- a missing
# `or $aN,$rX,$zero` (or `move $vN,$aN`) before a `jal`, where the target keeps
# a call argument in a general temp and materialises the copy, and our build
# folds the copy into the defining instruction.
#
# ⚠️ ROOT-CAUSE CORRECTION (supersedes the batch-15 filing, which blamed ugen's
# call-argument setup and specified a ugen hook `IDO52_UGEN_NO_ARG_COALESCE`):
# **ugen already emits the target's shape.**  For the canonical specimen
# ETC/func_80144D7C, `cc -S` (which stops after ugen) emits
#
#       and   $6, $2, 255
#       sb    $6, D_800EF878
#       move  $4, $6            <- the "missing" copy IS there
#       jal   card_file_make
#
# and the finished object has `andi $a0,$v0,0xff` with no copy at all.  The
# transformation is done by **as1**, in `f_do_passII_opt` -> `func_419268`,
# the per-basic-block peephole loop.  Its copy handling is:
#
#       s0 = func_413564(insn);              /* is this insn a copy?  */
#       if (s0 == 0) goto next;              /* no -- nothing to do   */
#       if (func_413c78(..., s0, ...)) goto changed;   /* forward prop */
#       if (func_4142d8(..., s0, ...) == 0) goto changed; /* backward */
#
# Vetoing at the `func_413564` result (pretending "this is not a copy") is the
# exact, minimal veto: it is byte-for-byte the control flow as1 takes for a
# non-copy instruction, so it disables NOTHING else in the peephole.
#
# USAGE
#   IDO52_AS1_NO_COPY_PROP=all              veto every copy candidate
#   IDO52_AS1_NO_COPY_PROP=1,4              veto candidates #1 and #4 only
#                                           (0-based, counted over the whole
#                                           translation unit in the order as1
#                                           reaches the test)
#   IDO52_AS1_COPY_PROP_TRACE=1             log every candidate with its index
#
#   ⚠️ `all` means ALL -- a bare `1` is candidate #1, NOT "everything" (this
#   deliberately differs from the LA_PULL selectors, where `1` == `all` and
#   candidate 1 is therefore unreachable).
#
#   Blanket `all` is usually too blunt: on ETC/func_80144D7C it restores 5
#   copies where the target has 1.  The per-index form is the working one.
#
#   ⚠️ Vetoing candidate N changes the instruction stream, so later candidate
#   indices in the SAME basic block can shift.  Re-derive the index with
#   IDO52_AS1_COPY_PROP_TRACE=1 after any source or env change (same caveat as
#   the CONST_INREG occurrence windows).
# --------------------------------------------------------------------------
COPY_PROP_HELPER = r'''
/* === IDO52_AS1_NO_COPY_PROP hook: support ===
 * Self-contained on purpose: func_419268 sits ~23k lines ABOVE func_4281e0,
 * so this block cannot use the LA_PULL helper's parser. */
static int as1_cprop_mode;              /* 0 = off, 1 = all, 2 = index list */
static int as1_cprop_list[128];
static int as1_cprop_nlist;
static int as1_cprop_trace = 0;

static void as1_cprop_init(void) {
    static int inited = 0;
    const char *e;
    if (inited) return;
    inited = 1;
    as1_cprop_trace = getenv("IDO52_AS1_COPY_PROP_TRACE") != NULL;
    e = getenv("IDO52_AS1_NO_COPY_PROP");
    if (e == NULL || e[0] == '\0') return;
    /* NOTE: unlike the LA_PULL selectors, a numeric value here is a candidate
     * INDEX, never "all" -- otherwise indices 0 and 1 would be unreachable.
     * Use "all" to veto everything, and simply leave the variable unset for
     * the default-off behaviour. */
    if (strcmp(e, "all") == 0) {
        as1_cprop_mode = 1;
        return;
    }
    {
        char *p = (char *)e;
        as1_cprop_mode = 2;
        while (*p != '\0' && as1_cprop_nlist < 128) {
            as1_cprop_list[as1_cprop_nlist++] = (int)strtol(p, &p, 0);
            while (*p == ',' || *p == ' ') p++;
        }
    }
}

static int as1_cprop_hit(int idx) {
    int i;
    if (as1_cprop_mode == 1) return 1;
    if (as1_cprop_mode == 2) {
        for (i = 0; i < as1_cprop_nlist; i++)
            if (as1_cprop_list[i] == idx) return 1;
    }
    return 0;
}

/* Called inside func_419268 once per COPY candidate (func_413564 said "yes").
   Returns 1 to make as1 treat the instruction as a non-copy, which leaves the
   `move` in the stream exactly as ugen emitted it. */
static int as1_copy_prop_veto(uint32_t insn) {
    static int seq = 0;
    int idx;
    as1_cprop_init();
    if (as1_cprop_mode == 0 && !as1_cprop_trace) return 0;
    idx = seq++;
    if (as1_cprop_hit(idx)) {
        if (as1_cprop_trace)
            fprintf(stderr, "[AS1-COPY-PROP] #%d insn=%08x VETOED\n", idx, insn);
        return 1;
    }
    if (as1_cprop_trace)
        fprintf(stderr, "[AS1-COPY-PROP] #%d insn=%08x allowed\n", idx, insn);
    return 0;
}
'''


# --------------------------------------------------------------------------
# Hook 7: IDO52_AS1_NO_SCHED_LOAD_HOIST -- intra-BB list-scheduler
#         load-over-ready-store veto (2026-07-27)
#
# THE BLOCKER CLASS IT ADDRESSES: the "+1-word epilogue duplication" wall.
# as1's per-basic-block list scheduler front-loads a longer load chain over a
# store that is ready in the same cycle.  At the bottom of the block that
# leaves the last group starting with a load, so as1 has no spare `lui` to pull
# into the following branch's delay slot and duplicates the epilogue `lw ra`
# there instead -- one word more than the target.  Specimens:
# RPG_BAT/func_8013F258 (65 vs 64 words), DATE2/func_80136868 (arms 0/1),
# KANGEI/func_80135850 (swapped-lbu residual pairs + unfilled delay slots).
#
# WHERE IT LIVES
#   f_schedule (L4251bc)
#     -> f_reorganize_bb (L4324xx), twice per block: pass a1=0 then a1=1
#          -> func_42f018   build the node array + dependence DAG
#          -> f_update_dag
#          -> func_4314ec   THE LIST SCHEDULER
#
#   Node records are a 0x38-stride array based at 0x10025410; as1's own
#   -O>=2 trace prints a node as `(node - 0x10025410) / 0x38`.  Fields used
#   here (all established from func_42f018's initialisation code):
#       node+0    the 32-bit MIPS instruction word
#       node+20   the instruction's 1-based ORIGINAL position in the block
#                 (func_42f018 walks the pinst array BACKWARD, so the node
#                 array index runs in reverse program order and is NOT an
#                 order key -- +20 is)
#       node+44   the scheduling priority (critical-path height)
#
#   func_4314ec's candidate loop walks the ready list (head 0x10024a9c, node
#   pointer at +8) keeping the best node in s4.  For each candidate s2 it
#   computes s3 = f_start_time(), s1 = max successor latency, s0 = node+44.
#   The comparison chain at L431854 is, in order:
#       1. no incumbent                       -> take
#       2. s3 <  fp   (earlier start time)    -> take
#       3. s3 != fp                           -> reject
#       4. equal start time, and then either
#            flag 0x100226b4 != 0  ->  f_is_node_better()      [pass a1=1]
#            flag 0x100226b4 == 0  ->  higher node+44 wins     [pass a1=0]
#          then successor-latency, then node+48, then node+12.
#
#   Step 4 is where the load beats the store: same cycle, but the load heads
#   the longer chain so its height is greater.  BOTH spellings of step 4 are
#   hooked, because f_schedule runs the block through pass a1=0 and pass a1=1.
#
# WHAT THE HOOK DOES
#   At each step-4 decision it asks: of the two nodes under comparison, is one
#   a LOAD and the other a STORE, with the STORE earlier in the block (node+20)?
#   If so it makes the STORE win -- vetoing the candidate when the candidate is
#   the load, forcing it when the candidate is the earlier store.  Nothing else
#   about the schedule is touched: start-time ordering (steps 1-3) and every
#   comparison between two loads, two stores, or any non-memory pair are left
#   exactly as as1 computed them.  Vetoing is byte-for-byte the control flow
#   as1 takes when the candidate simply loses the priority compare.
#
# USAGE
#   IDO52_AS1_NO_SCHED_LOAD_HOIST=1        (or `all`)  every opportunity
#   IDO52_AS1_NO_SCHED_LOAD_HOIST=0,2      opportunities #0 and #2 only
#   IDO52_AS1_NO_SCHED_LOAD_HOIST=occ:3    the `occ:N` spelling of a 1-element
#                                          list (accepted for the doc spec)
#   IDO52_AS1_NO_SCHED_LOAD_HOIST=strict   widen the partner from "a store" to
#                                          "any earlier instruction" -- needed
#                                          when the store is not yet ready
#                                          because the node the load jumps over
#                                          is the store's address-forming `lui`
#                                          (RPG_BAT's second copy is exactly
#                                          this).  `strict:0,2` = strict rule,
#                                          those indices only.
#   IDO52_AS1_SCHED_LOAD_HOIST_TRACE=1     list every opportunity as
#                                          `#N ld@P <insn> vs st@Q <insn> ...`
#
#   Occurrence indices are 0-based and counted over the whole translation unit,
#   in the order the scheduler reaches an applicable load/store pair.  A block
#   is scheduled TWICE (pass a1=0 and a1=1), so the same textual pair yields
#   two consecutive opportunities; `all` is usually what you want first, and
#   the per-index form is for surgically keeping one hoist.
#
#   ⚠️ Same caveat as every other occurrence-indexed hook: acting on index N
#   changes the schedule, so later indices shift.  Re-derive with
#   IDO52_AS1_SCHED_LOAD_HOIST_TRACE=1 after any source or env change.
# --------------------------------------------------------------------------
SCHED_LOAD_HOIST_HELPER = r'''
/* === IDO52_AS1_NO_SCHED_LOAD_HOIST hook: support ===
 * Self-contained on purpose (func_4314ec sits far from the other hook
 * helpers, and this must not depend on their parsers). */
#define AS1_LH_NODE_BASE   0x10025410u
#define AS1_LH_NODE_STRIDE 0x38u

static int as1_lh_mode;                 /* 0 = off, 1 = all, 2 = index list,
                                           3 = all EXCEPT the listed indices */
static int as1_lh_strict;               /* 1 = "any earlier node", not just stores */
static int as1_lh_list[128];
static int as1_lh_nlist;
static int as1_lh_trace = 0;

static void as1_lh_init(void) {
    static int inited = 0;
    const char *e;
    if (inited) return;
    inited = 1;
    as1_lh_trace = getenv("IDO52_AS1_SCHED_LOAD_HOIST_TRACE") != NULL;
    e = getenv("IDO52_AS1_NO_SCHED_LOAD_HOIST");
    if (e == NULL || e[0] == '\0') return;
    if (strncmp(e, "strict", 6) == 0) {   /* `strict` or `strict:<list>` */
        as1_lh_strict = 1;
        e += 6;
        if (*e == '\0') {
            as1_lh_mode = 1;
            return;
        }
        if (*e == ':' || *e == ',') e++;
    }
    if (strcmp(e, "1") == 0 || strcmp(e, "all") == 0) {
        as1_lh_mode = 1;
        return;
    }
    if (strncmp(e, "occ:", 4) == 0) e += 4;   /* the documented `occ:N` form */
    {
        char *p = (char *)e;
        /* A leading `-` on the first entry switches the whole list to
           "everything EXCEPT these" -- the useful form when one opportunity in
           the run is harmful (a $sp-relative prologue pair, typically) and the
           rest are wanted.  Positive indices are the include form. */
        as1_lh_mode = (*p == '-') ? 3 : 2;
        while (*p != '\0' && as1_lh_nlist < 128) {
            if (*p == '-') p++;
            as1_lh_list[as1_lh_nlist++] = (int)strtol(p, &p, 0);
            while (*p == ',' || *p == ' ') p++;
        }
    }
}

static int as1_lh_hit(int idx) {
    int i;
    if (as1_lh_mode == 1) return 1;
    if (as1_lh_mode == 2) {
        for (i = 0; i < as1_lh_nlist; i++)
            if (as1_lh_list[i] == idx) return 1;
    }
    if (as1_lh_mode == 3) {
        for (i = 0; i < as1_lh_nlist; i++)
            if (as1_lh_list[i] == idx) return 0;
        return 1;
    }
    return 0;
}

static int as1_lh_is_load(uint32_t insn) {
    uint32_t op = insn >> 26;
    /* lb lh lwl lw lbu lhu lwr | ll lwc1 lwc2 lwc3 ldc0..ldc3 */
    return (op >= 0x20u && op <= 0x27u) || (op >= 0x30u && op <= 0x37u);
}

static int as1_lh_is_store(uint32_t insn) {
    uint32_t op = insn >> 26;
    /* sb sh swl sw sdl sdr swr (0x2f = cache, excluded) | sc swc1..3 sdc0..3 */
    return (op >= 0x28u && op <= 0x2eu) || (op >= 0x38u && op <= 0x3fu);
}

/* Called from func_4314ec's step-4 comparison, once per candidate/incumbent
   pair that reaches it.  `cand` is the node being considered, `best` the
   current incumbent; both are node-record addresses in as1's guest memory.
   Returns:
        0   no opinion -- as1's own decision stands
       +1   VETO   the candidate (it is a load, `best` is an earlier store)
       -1   FORCE  the candidate (it is a store earlier than the load `best`) */
static int as1_sched_load_hoist(uint8_t *mem, uint32_t cand, uint32_t best) {
    static int seq = 0;
    uint32_t ci, bi, cpos, bpos;
    int idx, want;

    as1_lh_init();
    if (as1_lh_mode == 0 && !as1_lh_trace) return 0;
    if (cand == 0 || best == 0) return 0;

    ci = MEM_U32(cand + 0);
    bi = MEM_U32(best + 0);
    cpos = MEM_U32(cand + 20);          /* 1-based position in the block */
    bpos = MEM_U32(best + 20);

    /* Default rule: only a LOAD vs an earlier STORE.  `strict` widens the
       partner from "a store" to "any earlier instruction", which is what the
       block needs when the store is not itself ready yet because its
       address-forming `lui` is the node the load is jumping over. */
    if (as1_lh_is_load(ci) && !as1_lh_is_load(bi) && bpos < cpos &&
        (as1_lh_strict || as1_lh_is_store(bi))) {
        want = 1;
    } else if (as1_lh_is_load(bi) && !as1_lh_is_load(ci) && cpos < bpos &&
               (as1_lh_strict || as1_lh_is_store(ci))) {
        want = -1;
    } else {
        return 0;
    }

    idx = seq++;
    if (!as1_lh_hit(idx)) {
        if (as1_lh_trace)
            fprintf(stderr, "[AS1-LOAD-HOIST] #%d n%u@%u=%08x vs n%u@%u=%08x "
                            "allowed (as1 decides)\n",
                    idx, (cand - AS1_LH_NODE_BASE) / AS1_LH_NODE_STRIDE, cpos, ci,
                    (best - AS1_LH_NODE_BASE) / AS1_LH_NODE_STRIDE, bpos, bi);
        return 0;
    }
    if (as1_lh_trace)
        fprintf(stderr, "[AS1-LOAD-HOIST] #%d n%u@%u=%08x vs n%u@%u=%08x "
                        "-> store-first (%s)\n",
                idx, (cand - AS1_LH_NODE_BASE) / AS1_LH_NODE_STRIDE, cpos, ci,
                (best - AS1_LH_NODE_BASE) / AS1_LH_NODE_STRIDE, bpos, bi,
                want > 0 ? "VETO candidate" : "FORCE candidate");
    return want;
}

/* Diagnostic only: dump every candidate the list scheduler evaluates. */
static void as1_sched_cand_trace(uint8_t *mem, uint32_t cand, uint32_t prio,
                                 uint32_t start, uint32_t best,
                                 uint32_t best_start) {
    as1_lh_init();
    if (!as1_lh_trace) return;
    fprintf(stderr, "[AS1-SCHED] cand n%u@%u insn=%08x t=%d prio=%u | "
                    "best n%u@%u t=%d\n",
            (cand - AS1_LH_NODE_BASE) / AS1_LH_NODE_STRIDE,
            MEM_U32(cand + 20), MEM_U32(cand + 0), (int)start, prio,
            best ? (best - AS1_LH_NODE_BASE) / AS1_LH_NODE_STRIDE : 0xffffffffu,
            best ? MEM_U32(best + 20) : 0u, (int)best_start);
}
'''




# --------------------------------------------------------------------------
# Hook 8: IDO52_AS1_KEEP_ANDI -- pass-II backward-propagation veto (2026-08-06)
#
# THE BLOCKER CLASS: the narrowing-truncation fold (KEEP_NARROW_TRUNC, TODO
# item 0).  ugen emits the target's `andi rd,rs,0xff` (+ write-back move) for
# a (u8) truncation; as1 -O2's func_4142d8 -- the BACKWARD propagation arm of
# func_419268's copy handling -- folds it away (bisect-proven: gating
# func_4142d8 alone preserves both andis of the 2-shape minimal repro; -O1
# never runs it).  NOT the same transform as forward copy-prop
# (IDO52_AS1_NO_COPY_PROP=all does not keep the andi).
#
# USAGE
#   IDO52_AS1_KEEP_ANDI=all           veto every backward-prop candidate
#   IDO52_AS1_KEEP_ANDI=0,4           veto candidates #0 and #4 (0-based,
#                                        counted over the translation unit in
#                                        call order; NOTE a bare `1` is
#                                        candidate #1, NOT "all")
#   IDO52_AS1_KEEP_ANDI_TRACE=1          log every candidate + verdict
#
#   Same index-shift caveat as NO_COPY_PROP: vetoing candidate N changes
#   the stream; re-derive indices with the trace after any source/env change.
# --------------------------------------------------------------------------
KEEP_ANDI_HELPER = r'''
/* === IDO52_AS1_KEEP_ANDI hook: support === */
static int as1_kandi_mode;              /* 0 = off, 1 = all, 2 = index list */
static int as1_kandi_list[128];
static int as1_kandi_nlist;
static int as1_kandi_trace;
static int as1_kandi_seq;

static void as1_kandi_init(void) {
    static int inited = 0;
    const char *e;
    char *pp;
    if (inited) return;
    inited = 1;
    as1_kandi_trace = getenv("IDO52_AS1_KEEP_ANDI_TRACE") != NULL;
    e = getenv("IDO52_AS1_KEEP_ANDI");
    if (e == NULL || e[0] == '\0') return;
    if (strcmp(e, "all") == 0) { as1_kandi_mode = 1; return; }
    pp = (char *)e;
    if (strncmp(pp, "occ:", 4) == 0) pp += 4;
    as1_kandi_mode = 2;
    while (*pp != '\0' && as1_kandi_nlist < 128) {
        as1_kandi_list[as1_kandi_nlist++] = (int)strtol(pp, &pp, 0);
        while (*pp == ',' || *pp == ' ') pp++;
    }
}

/* One call per instruction func_416ca8 examines past its +34 movability
   guard (the narrow-op simplifier).  insn_word is the encoded instruction.
   Returns 1 to refuse the transform (func_416ca8 then reports no change). */
static int as1_kandi_veto(uint32_t insn_word) {
    int i, idx, hit;
    as1_kandi_init();
    if (!as1_kandi_mode && !as1_kandi_trace) return 0;
    idx = as1_kandi_seq++;
    hit = 0;
    if (as1_kandi_mode == 1) hit = 1;
    else if (as1_kandi_mode == 2)
        for (i = 0; i < as1_kandi_nlist; i++)
            if (as1_kandi_list[i] == idx) { hit = 1; break; }
    if (as1_kandi_trace)
        fprintf(stderr, "[AS1-KANDI] #%d word=%08x%s\n", idx, insn_word,
                hit ? " -> VETOED" : "");
    return hit;
}
'''


# --------------------------------------------------------------------------
# Hook 45: IDO52_AS1_FORCE_NARROW_FOLD -- narrow-op simplifier FORCE
#          (2026-08-21).  Doc: UOPT-HOOKS-REFERENCE.md Hook 45.
#          The exact inverse of Hook 8 (IDO52_AS1_KEEP_ANDI), on the same
#          transform (func_416ca8) but at a different decision point.
#
# THE BLOCKER CLASS: the "u8-web-split copy vs allocator coalescing" wall
# filed by SHUGAKU/func_80139114 (word [33]), SHUGAKU/func_801337FC (word
# [29]) and KANGEI/func_80138F90 -- one family, one word each.  The C is a
# faithful `bi = best_i;` where `best_i` is a u8-ranged value; IDO emits
# `andi $s1,$s7,0x00ff` and the retail binary has `or $s1,$s7,$zero`.
#
# WHY as1 DOES NOT FOLD IT BY ITSELF.  func_416ca8's first arm (L416d68) is
# exactly the andi-is-redundant simplifier: it looks up the SOURCE register's
# recorded significant-bit count in the per-register table at 0x100221b0,
# builds mask = (1 << bits) - 1, and folds `andi rt,rs,imm` -> `or rt,rs,$0`
# (via f_change_bb(insn, def, 3, 0)) when `mask == (mask & imm)` -- i.e. when
# the immediate keeps every bit the source can possibly have.  That table is
# a LOCAL (intra-basic-block) property.  In this family `best_i` is assigned
# `i & 0xff` inside the loop and 255 before it, so the value provably fits
# 0xff on every path -- but the def is in a DIFFERENT block, the table reads
# 32 ("unknown"), mask is 0xffffffff, and the proof fails.  The original 1995
# as1 sees the same thing, so this is a knob for reproducing a target whose
# ORIGINAL source shape gave as1 the intra-block proof, not a claim that our
# as1 is wrong.
#
# WHAT THE HOOK DOES: at that test, when the occurrence is listed, it sets
# the compared value to the state a natural fold would have had, so control
# falls into func_416ca8's OWN fold path -- same f_change_bb call, same
# arguments, same `return 1`.  No instruction is synthesised by the hook and
# no new code path exists; it only removes a guard.
#
# USAGE
#   IDO52_AS1_FORCE_NARROW_FOLD=all       force every failing candidate
#   IDO52_AS1_FORCE_NARROW_FOLD=0         force candidate #0 (0-based, counted
#                                            over the translation unit in
#                                            arrival order; a bare `0` is an
#                                            index, NOT "all")
#   IDO52_AS1_FORCE_NARROW_FOLD=occ:2,5   the explicit `occ:` spelling
#   IDO52_AS1_FORCE_NARROW_FOLD_TRACE=1   enumerate every arrival + verdict
#
# ⚠️ SOUNDNESS IS THE CALLER'S BURDEN -- the same rule Hook 44 states for a
# partial veto.  Vetoes are subtractive and can only lose an optimisation;
# a FORCE deletes a correctness proof.  A forced fold is sound ONLY when the
# source value fits the immediate's mask on EVERY path that reaches the
# instruction (e.g. `x = y & 0xff` in the loop body and a <= 0xff constant on
# entry, for `andi rt,rs,0xff`).  If it does not, the fold silently widens
# the value and every consumer reads the un-truncated bits.  Establish the
# invariant from the C before writing the env line, record it in the
# function header, and never force `all` on a translation unit you have not
# enumerated with the trace first.
#
# Same index-shift caveat as Hooks 6/8/44: indices are per translation unit
# and move when the instruction stream changes.  Pass II ITERATES, so one
# source andi can arrive at the test more than once -- the trace shows the
# repeats (same word, same pos) and forcing the first arrival is normally
# enough, because the fold rewrites the instruction and later sweeps no
# longer see an andi.
# --------------------------------------------------------------------------
NARROW_FOLD_HELPER = r'''
/* === IDO52_AS1_FORCE_NARROW_FOLD hook (Hook 45): support === */
static int as1_nfold_mode;              /* 0 = off, 1 = all, 2 = index list */
static int as1_nfold_list[128];
static int as1_nfold_nlist;
static int as1_nfold_trace;
static int as1_nfold_seq;

static void as1_nfold_init(void) {
    static int inited = 0;
    const char *e;
    char *pp;
    if (inited) return;
    inited = 1;
    as1_nfold_trace = getenv("IDO52_AS1_FORCE_NARROW_FOLD_TRACE") != NULL;
    e = getenv("IDO52_AS1_FORCE_NARROW_FOLD");
    if (e == NULL || e[0] == '\0') return;
    if (strcmp(e, "all") == 0) { as1_nfold_mode = 1; return; }
    pp = (char *)e;
    if (strncmp(pp, "occ:", 4) == 0) pp += 4;
    as1_nfold_mode = 2;
    while (*pp != '\0' && as1_nfold_nlist < 128) {
        as1_nfold_list[as1_nfold_nlist++] = (int)strtol(pp, &pp, 0);
        while (*pp == ',' || *pp == ' ') pp++;
    }
}

/* One call per arrival at func_416ca8's upper-bits redundancy proof for a
   narrow op (`andi rt,rs,imm` at L416d68).

     insn_word  the encoded andi
     pos        its instruction index (func_416ca8's a0 argument)
     srcbits    the source register's recorded significant-bit count
                (32 == "unknown", which is what a cross-block def yields)
     mask       (1 << srcbits) - 1, the value as1 compares against
     imm        the andi's 16-bit immediate
     natural    as1's own verdict: 1 == it would fold anyway

   Returns 1 to FORCE the fold.  A forced candidate then takes func_416ca8's
   own fold path unchanged (f_change_bb(pos, def, 3, 0) -> `or rt,rs,$zero`,
   `return 1`).

   *** SOUNDNESS IS THE CALLER'S BURDEN.  as1's proof is intra-block; this
   hook removes it.  Only force where the source value provably fits the
   mask on every path.  See UOPT-HOOKS-REFERENCE.md Hook 45. *** */
static int as1_nfold_force(uint32_t insn_word, uint32_t pos, uint32_t srcbits,
                           uint32_t mask, uint32_t imm, int natural) {
    int i, idx, hit;
    as1_nfold_init();
    if (!as1_nfold_mode && !as1_nfold_trace) return 0;
    idx = as1_nfold_seq++;
    hit = 0;
    if (as1_nfold_mode == 1) hit = 1;
    else if (as1_nfold_mode == 2)
        for (i = 0; i < as1_nfold_nlist; i++)
            if (as1_nfold_list[i] == idx) { hit = 1; break; }
    if (natural) hit = 0;               /* already folding; nothing to force */
    if (as1_nfold_trace)
        fprintf(stderr, "[AS1-NFOLD] #%d word=%08x pos=%u rs=%u rt=%u "
                        "srcbits=%u mask=%08x imm=%04x natural=%s%s\n",
                idx, insn_word, pos, (insn_word >> 21) & 31u,
                (insn_word >> 16) & 31u, srcbits, mask, imm & 0xffffu,
                natural ? "FOLD" : "NOFOLD", hit ? " -> FORCED" : "");
    return hit;
}
'''



# --------------------------------------------------------------------------
# Hook 48: IDO52_AS1_KEEP_SEXT -- sign-extension-pair fold veto (2026-09-22)
# Hook 49: IDO52_AS1_NO_BRANCH_FOLD -- branch/def fold veto (2026-09-22)
# Hook 50: IDO52_AS1_NO_SHIFT_COMBINE -- sll-of-sll combine veto (2026-09-22)
#          Doc: UOPT-HOOKS-REFERENCE.md Hooks 48/49/50.
#
# Both live in func_419268 (the per-BB pass-II peephole driven by
# f_do_passII_opt), which dispatches on the instruction's major opcode:
#     SPECIAL funct 0x03 (sra)  -> func_416528   <- Hook 48
#     beq / bne (4, 5)          -> func_4179f0   <- Hook 49
#     andi / ori (12, 13)       -> func_416ca8   <- Hook 29 (KEEP_ANDI)
#     SPECIAL funct 0x00 (sll)  -> func_41673c   <- Hook 50
#     SPECIAL funct 0x24 (and)  -> func_416ac4
#     addiu (9)                 -> func_4171dc
#
# func_416528 (sra): if the destination's demanded-bit count (per-register
# table at 0x100221b0) is <= 32 - shamt, and the source's reaching def
# (func_416304) is an `sll` by the SAME shamt, it change_bb()s BOTH halves to
# a move -- `sll t2,a2,16 ; sra t2,t2,16` becomes `move t2,a2`, and copy-prop
# then usually deletes the move.  This is the sign-extension sibling of the
# andi fold Hook 29 vetoes; KEEP_ANDI's trace never sees it.  First consumer
# Snd_SetVol (x/y s16 params stored with `sh` and masked with `andi
# 0xffff`: retail keeps both extensions).
#
# func_4179f0 (beq/bne): finds the compared register's reaching def and
# dispatches on its internal opcode (+36), rewriting the branch to test the
# def's source and letting the def die.  First consumer get_alarm
# (`xori v0,v0,1 ; beqz v0` -> `bnez v0`; retail keeps the xori).
#
# func_41673c (sll): if the source's reaching def (func_416304) is itself an
# `sll` (internal op 2) with no other pending state and the summed shift
# amount is < 32, it f_change_bb()s the def to a move and rewrites this sll
# to shift the def's source by the sum -- `sll t8,s0,3 ; sll t9,t8,16`
# becomes `move t8,s0 ; sll t9,s0,19`, and copy-prop deletes the move.
# First consumer SsUtAllKeyOff (retail keeps both shifts).
#
# All three vetoes take the function's own `return 0` ("no change") exit, so a
# vetoed candidate is exactly as1's control flow for an unfoldable one.
# Subtractive -> always sound.  Indices are per translation unit in arrival
# order; pass II iterates, so one instruction can arrive more than once
# (the trace shows repeats; veto all arrivals of a word, or use `all`).
# --------------------------------------------------------------------------
KSEXT_HELPER = r'''
/* === IDO52_AS1_KEEP_SEXT / NO_BRANCH_FOLD hooks (48/49): support === */
typedef struct {
    int mode;           /* 0 = off, 1 = all, 2 = index list */
    int list[128];
    int nlist;
    int trace;
    int seq;
    int inited;
} as1_occ_sel;

static void as1_occ_sel_init(as1_occ_sel *h, const char *var, const char *tvar) {
    const char *e;
    char *pp;
    if (h->inited) return;
    h->inited = 1;
    h->trace = getenv(tvar) != NULL;
    e = getenv(var);
    if (e == NULL || e[0] == '\0') return;
    if (strcmp(e, "all") == 0) { h->mode = 1; return; }
    pp = (char *)e;
    if (strncmp(pp, "occ:", 4) == 0) pp += 4;
    h->mode = 2;
    while (*pp != '\0' && h->nlist < 128) {
        h->list[h->nlist++] = (int)strtol(pp, &pp, 0);
        while (*pp == ',' || *pp == ' ') pp++;
    }
}

/* Returns the candidate index, or -1 when the hook is fully off. */
static int as1_occ_sel_next(as1_occ_sel *h, int *hit) {
    int i, idx;
    *hit = 0;
    if (!h->mode && !h->trace) return -1;
    idx = h->seq++;
    if (h->mode == 1) *hit = 1;
    else if (h->mode == 2)
        for (i = 0; i < h->nlist; i++)
            if (h->list[i] == idx) { *hit = 1; break; }
    return idx;
}

static as1_occ_sel as1_ksext_sel;
static as1_occ_sel as1_nbfold_sel;

/* Hook 48: one call per sra reaching func_416528's fold test. */
static int as1_ksext_veto(uint32_t insn_word) {
    int hit, idx;
    as1_occ_sel_init(&as1_ksext_sel, "IDO52_AS1_KEEP_SEXT",
                     "IDO52_AS1_KEEP_SEXT_TRACE");
    idx = as1_occ_sel_next(&as1_ksext_sel, &hit);
    if (idx < 0) return 0;
    if (as1_ksext_sel.trace)
        fprintf(stderr, "[AS1-KSEXT] #%d word=%08x rd=%u rt=%u sa=%u%s\n",
                idx, insn_word, (insn_word >> 11) & 31u,
                (insn_word >> 16) & 31u, (insn_word >> 6) & 31u,
                hit ? " -> VETOED" : "");
    return hit;
}

/* Hook 49: one call per beq/bne whose compared register has a reaching def
   (func_4179f0 past func_416304).  def_op is the def record's internal
   opcode (+36), the key func_4179f0 dispatches on. */
static int as1_nbfold_veto(uint32_t br_word, uint32_t def_word, uint32_t def_op) {
    int hit, idx;
    as1_occ_sel_init(&as1_nbfold_sel, "IDO52_AS1_NO_BRANCH_FOLD",
                     "IDO52_AS1_NO_BRANCH_FOLD_TRACE");
    idx = as1_occ_sel_next(&as1_nbfold_sel, &hit);
    if (idx < 0) return 0;
    if (as1_nbfold_sel.trace)
        fprintf(stderr, "[AS1-NBFOLD] #%d branch=%08x def=%08x def_op=0x%x%s\n",
                idx, br_word, def_word, def_op, hit ? " -> VETOED" : "");
    return hit;
}
'''

# Hook 50 helper (needs as1_occ_sel from KSEXT_HELPER, which lands earlier
# in as1.c: func_416528 precedes func_41673c).
NSCOMB_HELPER = r'''
/* === IDO52_AS1_NO_SHIFT_COMBINE hook (50): support === */
static as1_occ_sel as1_nscomb_sel;

/* Hook 50: one call per sll whose source's reaching def is a combinable sll
   (func_41673c at L41687c, immediately before its f_change_bb). */
static int as1_nscomb_veto(uint32_t insn_word, uint32_t def_word, uint32_t sum) {
    int hit, idx;
    as1_occ_sel_init(&as1_nscomb_sel, "IDO52_AS1_NO_SHIFT_COMBINE",
                     "IDO52_AS1_NO_SHIFT_COMBINE_TRACE");
    idx = as1_occ_sel_next(&as1_nscomb_sel, &hit);
    if (idx < 0) return 0;
    if (as1_nscomb_sel.trace)
        fprintf(stderr, "[AS1-NSCOMB] #%d sll=%08x def=%08x sum=%u%s\n",
                idx, insn_word, def_word, sum, hit ? " -> VETOED" : "");
    return hit;
}
'''

# --------------------------------------------------------------------------
# Hook 9: IDO52_AS1_TIE_* -- list-scheduler equal-start-time tie-break
#         override (2026-08-11).  Doc: UOPT-HOOKS-REFERENCE.md Hook 35.
#
# THE BLOCKER CLASS: the "delay-fill / lui-source-preference" wall (filed by
# DATE2/func_80136868, KANGEI/func_801327E4, SHOUGATU/func_80137364).  When
# two instructions are READY IN THE SAME CYCLE, func_4314ec's step-4 tie-break
# picks by critical-path height (node+44), so a long load CHAIN's `lui` beats
# the short read-modify-write chain's `lui` and the whole block front-loads
# the wrong chain -- which then also decides WHICH `lui` do_xbb_opt later
# boosts into a predecessor's load-delay bubble.  The original 1995 as1 kept
# ORIGINAL PROGRAM ORDER in the specimen blocks.  Hook 7's veto is keyed to
# load-vs-store pairs and cannot reach a lui-vs-lui choice; this hook is the
# class-free generalization.
#
# WHERE IT LIVES: the same two step-4 sites as Hook 7 (L43189c is_node_better
# pass a1=1, L4318b8 node+44 priority pass a1=0).  Both are reached ONLY when
# candidate and incumbent have EQUAL start times, so every override picks a
# node that is ready in the same cycle: any steered outcome is a schedule
# as1's own tie-break could have produced.  Dependence legality is untouched.
#
# USAGE
#   IDO52_AS1_TIE_SRC_ORDER=bb:5,6     program order wins every tie in basic
#                                      blocks #5 and #6 (bb+72 block number,
#                                      printed by the trace) -- THE FORM TO
#                                      PREFER: it is deterministic per
#                                      comparison, so do_xbb_opt's trial-move
#                                      reschedules see a consistent rule.
#   IDO52_AS1_TIE_SRC_ORDER=all        program order wins every tie (also
#                                      deterministic, usually too broad)
#   IDO52_AS1_TIE_SRC_ORDER=3,7        ... only at tie-break #3 and #7
#   IDO52_AS1_TIE_SRC_ORDER=-12        ... everywhere EXCEPT #12
#   IDO52_AS1_TIE_TAKE=N[,N...]        force-take the candidate at #N
#   IDO52_AS1_TIE_REJECT=N[,N...]      force-reject the candidate at #N
#   IDO52_AS1_TIE_TRACE=1              enumerate all tie-breaks
#
#   TAKE/REJECT bind tighter than SRC_ORDER.  One shared 0-based counter over
#   the translation unit counts every equal-start-time comparison.
#   ⚠️ THE OCCURRENCE-INDEXED FORMS ARE FRAGILE HERE, beyond the usual
#   index-shift caveat: do_xbb_opt RE-SCHEDULES blocks during trial moves, so
#   the same block's comparisons are counted again with shifted indices and an
#   index-keyed rule can give the trial and the final schedule DIFFERENT
#   answers -- observed as `as1xbb.c, line 923: assertion (new_from_time ==
#   old_from_time) failed`.  If you hit that assertion, switch to the bb:
#   form.  Re-derive indices with the trace after any source or env change.
# --------------------------------------------------------------------------
TIE_HELPER = r'''
/* === IDO52_AS1_TIE hook: support ===
 * Equal-start-time tie-break override in func_4314ec (Hook 35 in
 * docs/reference/UOPT-HOOKS-REFERENCE.md).  Self-contained on purpose. */
struct as1_tie_sel { int mode; int list[128]; int nlist; };
        /* mode: 0 off, 1 all, 2 index list, 3 all-except, 4 block list */

static struct as1_tie_sel as1_tie_src, as1_tie_take, as1_tie_rej;
static int as1_tie_trace;
static int as1_tie_seq;
static int as1_tie_on;

static void as1_tie_parse(struct as1_tie_sel *s, const char *e) {
    char *p;
    s->mode = 0;
    s->nlist = 0;
    if (e == NULL || e[0] == '\0') return;
    if (strcmp(e, "all") == 0) { s->mode = 1; return; }
    p = (char *)e;
    if (strncmp(p, "bb:", 3) == 0) {
        p += 3;
        s->mode = 4;
    } else {
        if (strncmp(p, "occ:", 4) == 0) p += 4;
        s->mode = (*p == '-') ? 3 : 2;
    }
    while (*p != '\0' && s->nlist < 128) {
        if (*p == '-') p++;
        s->list[s->nlist++] = (int)strtol(p, &p, 0);
        while (*p == ',' || *p == ' ') p++;
    }
}

/* Blocks are keyed by FIRST-SEEN ordinal of their bb record (bb+72 is not
   yet assigned during first-pass scheduling): block #k is the k-th distinct
   block to reach the tie-break site, i.e. first-scheduling order, printed by
   the trace as bb=k.  Deterministic, and stable across do_xbb_opt's
   trial-move reschedules of the same block. */
static uint32_t as1_tie_bbs[1024];
static int as1_tie_nbbs;

static int as1_tie_bbno(uint32_t bb) {
    int i;
    if (bb == 0) return -1;
    for (i = 0; i < as1_tie_nbbs; i++)
        if (as1_tie_bbs[i] == bb) return i;
    if (as1_tie_nbbs < 1024) {
        as1_tie_bbs[as1_tie_nbbs++] = bb;
        return as1_tie_nbbs - 1;
    }
    return -1;
}

/* idx is the running tie-break occurrence; bbno the block's first-seen
   ordinal.  Mode 4 keys on bbno (deterministic across do_xbb_opt's
   trial-move reschedules); modes 2/3 key on idx (fragile there, see the
   header note). */

/* Branches/jumps are excluded from the SRC_ORDER rule: a branch's issue slot
   is what decides whether its delay slot gets filled from above, and forcing
   it to its program-order position (last) leaves the slot a nop.  j/jal =
   opcodes 2/3, beq..bgtz = 4..7, REGIMM bltz/bgez = 1, jr/jalr = SPECIAL
   with function 8/9. */
static int as1_tie_is_branch(uint32_t insn) {
    uint32_t op = insn >> 26;
    if (op >= 1 && op <= 7) return 1;
    if (op == 0) {
        uint32_t fn = insn & 0x3f;
        if (fn == 8 || fn == 9) return 1;
    }
    return 0;
}
static int as1_tie_hit(const struct as1_tie_sel *s, int idx, int bbno) {
    int i, key;
    if (s->mode == 1) return 1;
    if (s->mode == 2 || s->mode == 4) {
        key = (s->mode == 4) ? bbno : idx;
        for (i = 0; i < s->nlist; i++) if (s->list[i] == key) return 1;
        return 0;
    }
    if (s->mode == 3) {
        for (i = 0; i < s->nlist; i++) if (s->list[i] == idx) return 0;
        return 1;
    }
    return 0;
}

static void as1_tie_init(void) {
    static int inited = 0;
    if (inited) return;
    inited = 1;
    as1_tie_parse(&as1_tie_src, getenv("IDO52_AS1_TIE_SRC_ORDER"));
    as1_tie_parse(&as1_tie_take, getenv("IDO52_AS1_TIE_TAKE"));
    as1_tie_parse(&as1_tie_rej, getenv("IDO52_AS1_TIE_REJECT"));
    as1_tie_trace = getenv("IDO52_AS1_TIE_TRACE") != NULL;
    as1_tie_on = as1_tie_src.mode || as1_tie_take.mode || as1_tie_rej.mode;
}

/* Called once per EQUAL-START-TIME tie-break comparison in func_4314ec's
   candidate loop (whichever of the two step-4 spellings this pass takes,
   so exactly once per comparison).  cand/best are node records; both are
   ready in the same cycle, so either outcome is a schedule as1's own
   tie-break could have produced.  Returns +1 = take the candidate,
   -1 = reject it, 0 = as1's own choice stands. */
static int as1_tie_break(uint8_t *mem, uint32_t cand, uint32_t best,
                         uint32_t cprio, uint32_t cslat,
                         uint32_t bprio, uint32_t bslat, uint32_t bb) {
    int idx, act, bbno;
    const char *why;
    uint32_t cpos, bpos;
    as1_tie_init();
    if (!as1_tie_on && !as1_tie_trace) return 0;
    if (cand == 0 || best == 0) return 0;
    idx = as1_tie_seq++;
    bbno = as1_tie_bbno(bb);
    cpos = MEM_U32(cand + 20);          /* 1-based original block position */
    bpos = MEM_U32(best + 20);
    act = 0;
    why = "";
    if (as1_tie_hit(&as1_tie_take, idx, bbno)) {
        act = 1;
        why = " -> TAKE (forced)";
    } else if (as1_tie_hit(&as1_tie_rej, idx, bbno)) {
        act = -1;
        why = " -> REJECT (forced)";
    } else if (as1_tie_hit(&as1_tie_src, idx, bbno) && cpos != bpos &&
               !as1_tie_is_branch(MEM_U32(cand + 0)) &&
               !as1_tie_is_branch(MEM_U32(best + 0))) {
        act = (cpos < bpos) ? 1 : -1;
        why = (act > 0) ? " -> TAKE (src-order)" : " -> REJECT (src-order)";
    }
    if (as1_tie_trace)
        fprintf(stderr, "[AS1-TIE] #%d bb=%d cand@%u=%08x prio=%u slat=%u | "
                        "best@%u=%08x prio=%u slat=%u%s\n",
                idx, bbno, cpos, MEM_U32(cand + 0), cprio, cslat,
                bpos, MEM_U32(best + 0), bprio, bslat, why);
    return act;
}
'''


# --------------------------------------------------------------------------
# Hook 51: IDO52_AS1_NO_JUMP_INVERT -- jump-over-branch inversion veto
#          (2026-09-22).  Doc: UOPT-HOOKS-REFERENCE.md Hook 51.
#
# func_41b5e8 is the per-procedure branch clean-up f_traverse_bb calls once
# per procedure (before pass I).  For every block it asks func_41aa60 to
# classify the block's terminating branch; class 2 is "unconditional jump
# whose target block is a single conditional branch".  If that conditional
# branch's target (func_41b444) is the jumping block's own fall-through
# successor (s2 + 4), func_41b22c copies the conditional into the jump's
# slot with the condition INVERTED (func_41b12c) and retargets it at the
# conditional's own successor:
#
#       b     L           ; entry                bnez  a0, L+8
#       sw    ra,20(sp)                    ->    sw    ra,20(sp)
#   C0: ...               ; fall-through     C0: ...
#   L:  beqz  a0, C0                          L:  beqz  a0, C0
#       nop                                       nop
#   L+8:...                                   L+8:...
#
# Retail (SpuSetReverb, SpuSetTransferMode) keeps the `b`.  Located by gating
# every call in f_traverse_bb and then every call in func_41b5e8 with a
# private as1 (2026-09-22): only func_41aa60 / func_41b444 / func_41b22c
# change the entry word.  The veto jumps to L41bbb0, which is exactly where
# func_41b5e8 goes when func_41b22c declines (returns 0), so a vetoed
# candidate is as1's own control flow for an un-invertible branch.
# Subtractive -> always sound.  Indices count class-2 candidates that PASS
# the fall-through test, per translation unit in arrival order.  Class 1
# (jump-to-jump threading) and class 3 are not affected.
# --------------------------------------------------------------------------
NJINV_HELPER = r'''
/* === IDO52_AS1_NO_JUMP_INVERT hook (51): support === */
static as1_occ_sel as1_njinv_sel;

/* Hook 51: one call per jump-over-conditional inversion candidate, just
   before func_41b22c.  blk = the jumping block, tgt = the target block. */
static int as1_njinv_veto(uint8_t *mem, uint32_t blk, uint32_t tgt) {
    int hit, idx;
    uint32_t jw, bw;
    as1_occ_sel_init(&as1_njinv_sel, "IDO52_AS1_NO_JUMP_INVERT",
                     "IDO52_AS1_NO_JUMP_INVERT_TRACE");
    idx = as1_occ_sel_next(&as1_njinv_sel, &hit);
    if (idx < 0) return 0;
    if (as1_njinv_sel.trace) {
        jw = MEM_U32(MEM_U32(blk + 12) + MEM_U16(blk + 62) * 40u);
        bw = MEM_U32(MEM_U32(tgt + 12) + MEM_U16(tgt + 62) * 40u);
        fprintf(stderr, "[AS1-NJINV] #%d jump=%08x target_branch=%08x%s\n",
                idx, jw, bw, hit ? " -> VETOED" : "");
    }
    return hit;
}
'''

# --------------------------------------------------------------------------
# Hook 52: IDO52_AS1_KEEP_DEAD_RELOAD -- dead-instruction removal veto
#          (2026-09-22).  Doc: UOPT-HOOKS-REFERENCE.md Hook 52.
#
# func_419268 (the per-BB pass-II peephole) calls f_redundant (0x415988) at
# L41936c for every instruction record that is not marked "keep" (+33).
# f_redundant asks func_414570 which register the instruction defines, then
# func_413624 whether that register is dead at this point (overwritten
# before any use -- for a hi/lo pair both halves), and if so it
# f_change_bb(idx, 0, 5, 0)s the record, i.e. DELETES the instruction, and
# returns 1.  That is how a reload IDO's uopt emits after a call and which
# is re-read after the next call before any use disappears; the libgs
# emitter family 0x800AA060.. keeps it (retail `lw v1,<home>` after the
# first `jal`).  Located with a private as1 that skipped the
# call (2026-09-22): the deletion was the entire count residual of six
# functions.
#
# The veto sits at L415a8c, after every legality test and immediately
# before f_change_bb, and takes f_redundant's own `v0 = 0` exit (L415aec)
# -- exactly the control flow for an instruction whose destination is
# live, after which func_419268 goes on to try copy-prop on it as usual.
# Subtractive -> always sound: the kept word is the compiler's own.
# Indices count f_redundant's positive verdicts per translation unit in
# arrival order; pass II iterates, so one instruction can arrive more than
# once (the trace shows repeats; keep all arrivals of a word, or use
# `all`).  The helper is DEFINED after as1_occ_sel (before func_419268) and
# PROTOTYPED before f_redundant, which precedes func_416528 in as1.c.
# --------------------------------------------------------------------------
KDREL_PROTO = r'''
/* === IDO52_AS1_KEEP_DEAD_RELOAD hook (52): prototype === */
static int as1_kdrel_veto(uint8_t *mem, uint32_t idx, uint32_t dead_reg);
'''

KDREL_HELPER = r'''
/* === IDO52_AS1_KEEP_DEAD_RELOAD hook (52): support === */
static as1_occ_sel as1_kdrel_sel;

/* Hook 52: one call per instruction f_redundant is about to delete.
   idx = the record index into the 0x1002f2d0 table (40-byte records),
   dead_reg = the register func_413624 proved dead. */
static int as1_kdrel_veto(uint8_t *mem, uint32_t idx, uint32_t dead_reg) {
    int hit, n;
    uint32_t w;
    as1_occ_sel_init(&as1_kdrel_sel, "IDO52_AS1_KEEP_DEAD_RELOAD",
                     "IDO52_AS1_KEEP_DEAD_RELOAD_TRACE");
    n = as1_occ_sel_next(&as1_kdrel_sel, &hit);
    if (n < 0) return 0;
    if (as1_kdrel_sel.trace) {
        w = MEM_U32(MEM_U32(0x1002f2d0) + idx * 40u);
        fprintf(stderr, "[AS1-KDREL] #%d rec=%u word=%08x dead_reg=%u%s\n",
                n, idx, w, dead_reg, hit ? " -> KEPT" : "");
    }
    return hit;
}
'''


HOOKS = [
    (
        'IDO52_AS1_KEEP_DEAD_RELOAD hook',
        # f_redundant past every legality test, just before it deletes the
        # record: MEM_U32(sp + 76) = record index, MEM_U32(sp + 56) = the
        # register proved dead.
        'L415a8c:\n//nop;\na0 = MEM_U32(sp + 76);\na1 = zero;\na2 = 0x5;',
        '''L415a8c:
//nop;
/* === IDO52_AS1_KEEP_DEAD_RELOAD hook (Hook 52): veto the dead-instruction
 * delete.  v0 = zero + L415aec is f_redundant's own "not redundant" exit. */
if (as1_kdrel_veto(mem, MEM_U32(sp + 76), MEM_U32(sp + 56))) {
    v0 = zero;
    goto L415aec;
}
a0 = MEM_U32(sp + 76);
a1 = zero;
a2 = 0x5;''',
        'Applied IDO52_AS1_KEEP_DEAD_RELOAD (dead-instruction delete veto in f_redundant)',
        'f_redundant L415a8c (dead-instruction delete)',
    ),
    (
        'IDO52_AS1_KEEP_ANDI hook',
        'L416cf4:\n'
        't3 = MEM_U8(t0 + 29);\n'
        'a2 = MEM_U8(t0 + 28);\n'
        'MEM_U32(sp + 56) = t3;\n'
        't4 = MEM_U32(t0 + 0);',
        '''L416cf4:
t3 = MEM_U8(t0 + 29);
a2 = MEM_U8(t0 + 28);
MEM_U32(sp + 56) = t3;
t4 = MEM_U32(t0 + 0);
/* === IDO52_AS1_KEEP_ANDI hook: veto the andi/narrow-op simplifier ===
 * t4 is the instruction word; count + veto only real candidates.  Returning
 * 0 here is func_416ca8\'s own "no change" exit value. */
if (as1_kandi_veto(t4)) {
    return 0;
}''',
        'Applied IDO52_AS1_KEEP_ANDI (narrow-op simplifier veto in func_416ca8)',
        'func_416ca8 body (pass-II andi/narrow-op simplifier)',
    ),

    (
        'IDO52_AS1_FORCE_NARROW_FOLD hook',
        'L416d68:\n'
        't4 = v1 & t2;\n'
        'if ((int32_t)a0 != (int32_t)t4) {\n'
        'a3 = zero;\n'
        'goto L416dcc;}\n',
        '''L416d68:
t4 = v1 & t2;
/* === IDO52_AS1_FORCE_NARROW_FOLD hook (Hook 45): force the andi -> move
 * simplification past func_416ca8\'s own upper-bits proof.  a0 is the mask
 * (1 << significant-bits(rs)) - 1 and t4 is (mask & imm); the natural fold
 * runs when they are equal.  Assigning t4 = a0 reproduces EXACTLY the
 * register state a natural fold would have had at this point (t4 is dead on
 * both arms afterwards), so the forced candidate falls into func_416ca8\'s
 * own f_change_bb(pos, def, 3, 0) fold path.  SOUNDNESS is the caller\'s
 * burden -- see the helper comment. */
if (as1_nfold_force(MEM_U32(sp + 44), MEM_U32(sp + 72), v0, a0, t2,
                    (int)(a0 == t4))) {
    t4 = a0;
}
if ((int32_t)a0 != (int32_t)t4) {
a3 = zero;
goto L416dcc;}
''',
        'Applied IDO52_AS1_FORCE_NARROW_FOLD (narrow-op simplifier force in func_416ca8)',
        'func_416ca8 L416d68 (andi upper-bits redundancy proof)',
    ),

    (
        'IDO52_AS1_KEEP_SEXT hook',
        # func_416528 past its movability guards: v0 is the sra's record.
        'L416584:\na0 = MEM_U32(v0 + 0);\na1 = MEM_U8(v0 + 28);',
        '''L416584:
/* === IDO52_AS1_KEEP_SEXT hook (Hook 48): veto the sll/sra pair fold ===
 * v0 is the sra's instruction record; returning 0 is func_416528's own
 * "no change" exit. */
if (as1_ksext_veto(MEM_U32(v0 + 0))) {
    v0 = zero;
    goto L41672c;
}
a0 = MEM_U32(v0 + 0);
a1 = MEM_U8(v0 + 28);''',
        'Applied IDO52_AS1_KEEP_SEXT (sll/sra pair fold veto in func_416528)',
        'func_416528 L416584 (sra sign-extension fold)',
    ),
    (
        'IDO52_AS1_NO_BRANCH_FOLD hook',
        # func_4179f0 once the compared register's reaching def is known:
        # v0 = def index, t2 = the branch's record.
        'L417ad0:\nt1 = 0x1002f2d0;\nt3 = v0 << 2;',
        '''L417ad0:
/* === IDO52_AS1_NO_BRANCH_FOLD hook (Hook 49): veto the branch/def fold ===
 * v0 is the def's record index, t2 the branch's record; returning 0 is
 * func_4179f0's own "no change" exit. */
{
    uint32_t nbf_def = MEM_U32(0x1002f2d0) + v0 * 40u;
    if (as1_nbfold_veto(MEM_U32(t2 + 0), MEM_U32(nbf_def + 0),
                        MEM_U16(nbf_def + 36))) {
        v0 = zero;
        goto L418a00;
    }
}
t1 = 0x1002f2d0;
t3 = v0 << 2;''',
        'Applied IDO52_AS1_NO_BRANCH_FOLD (branch/def fold veto in func_4179f0)',
        'func_4179f0 L417ad0 (branch reaching-def fold)',
    ),
    (
        'IDO52_AS1_NO_SHIFT_COMBINE hook',
        # func_41673c past all its legality tests (def is an sll, sum < 32):
        # t0 = this sll's record, v1 = the def's record, t1 = def shamt,
        # MEM_U32(sp + 36) = this sll's shamt.
        'L41687c:\na1 = MEM_U8(v1 + 29);',
        '''L41687c:
/* === IDO52_AS1_NO_SHIFT_COMBINE hook (Hook 50): veto the sll-of-sll
 * combine.  v0 = zero + L416954 is func_41673c's own "no change" exit. */
if (as1_nscomb_veto(MEM_U32(t0 + 0), MEM_U32(v1 + 0),
                    t1 + MEM_U32(sp + 36))) {
    v0 = zero;
    goto L416954;
}
a1 = MEM_U8(v1 + 29);''',
        'Applied IDO52_AS1_NO_SHIFT_COMBINE (sll-of-sll combine veto in func_41673c)',
        'func_41673c L41687c (sll-of-sll combine)',
    ),

    (
        'IDO52_AS1_NO_JUMP_INVERT hook',
        # func_41b5e8 case 2, after the fall-through test, before func_41b22c:
        # s2 = the jumping block, MEM_U32(sp + 152) = the target block.
        'L41ba98:\n'
        't0 = MEM_U32(s2 + 4);\n'
        '// bdead c1fe020b gp = MEM_U32(sp + 52);\n'
        'if ((int32_t)v0 != (int32_t)t0) {\n'
        '//nop;\n'
        'goto L41bbb0;}\n',
        '''L41ba98:
t0 = MEM_U32(s2 + 4);
// bdead c1fe020b gp = MEM_U32(sp + 52);
if ((int32_t)v0 != (int32_t)t0) {
//nop;
goto L41bbb0;}
/* === IDO52_AS1_NO_JUMP_INVERT hook (Hook 51): veto the jump-over-branch
 * inversion.  L41bbb0 is where func_41b5e8 goes when func_41b22c declines. */
if (as1_njinv_veto(mem, s2, MEM_U32(sp + 152))) {
    goto L41bbb0;
}
''',
        'Applied IDO52_AS1_NO_JUMP_INVERT (jump-over-branch inversion veto in func_41b5e8)',
        'func_41b5e8 L41ba98 (class-2 jump inversion)',
    ),

    # (env var name used as the "already applied" probe, old text, new text, message)
    (
        'IDO52_AS1_NO_BRANCH_OPT',
        'L40a76c:\n//do_branch_opt:\n//nop;\n//nop;\n//nop;\nsp = sp + 0xffffff88;',
        '''L40a76c:
//do_branch_opt:
/* === IDO52_AS1_NO_BRANCH_OPT hook === */
{
    char *env = getenv("IDO52_AS1_NO_BRANCH_OPT");
    if (env && env[0] == '1') {
        return;
    }
}
//nop;
//nop;
//nop;
sp = sp + 0xffffff88;''',
        'Applied IDO52_AS1_NO_BRANCH_OPT',
        'f_do_branch_opt entry point',
    ),
    (
        'IDO52_AS1_NO_SCHEDULE',
        'L4251bc:\n//schedule:\n//nop;\n//nop;\n//nop;\nsp = sp + 0xffffff78;',
        '''L4251bc:
//schedule:
/* === IDO52_AS1_NO_SCHEDULE hook === */
{
    char *env = getenv("IDO52_AS1_NO_SCHEDULE");
    if (env && env[0] == '1') {
        return;
    }
}
//nop;
//nop;
//nop;
sp = sp + 0xffffff78;''',
        'Applied IDO52_AS1_NO_SCHEDULE (WARNING: crashes - use with caution)',
        'f_schedule entry point',
    ),
    (
        'IDO52_AS1_NO_FILL',
        'L42f7bc:\n//nop;\n//nop;\n//nop;\nsp = sp + 0xffffffb8;',
        '''L42f7bc:
/* === IDO52_AS1_NO_FILL hook === */
{
    char *env = getenv("IDO52_AS1_NO_FILL");
    if (env && env[0] == '1') {
        return;
    }
}
//nop;
//nop;
//nop;
sp = sp + 0xffffffb8;''',
        'Applied IDO52_AS1_NO_FILL',
        'func_42f7bc entry point',
    ),
    (
        'IDO52_AS1_NO_DELAY_FILL',
        'L42f170:\nif ((int32_t)v0 == (int32_t)0) {\ns0 = zero;\ngoto L42f288;}\ns0 = zero;\nif ((int32_t)s1 == (int32_t)0) {',
        '''L42f170:
/* === IDO52_AS1_NO_DELAY_FILL hook === */
{
    char *env = getenv("IDO52_AS1_NO_DELAY_FILL");
    if (env && env[0] == '1') {
        if (v0 != 0) fprintf(stderr, "[AS1-NO-FILL] Skipping delay fill (v0=%d)\\n", v0);
        v0 = 0;
    }
}
if ((int32_t)v0 == (int32_t)0) {
s0 = zero;
goto L42f288;}
s0 = zero;
if ((int32_t)s1 == (int32_t)0) {''',
        'Applied IDO52_AS1_NO_DELAY_FILL (NOTE: hook never fires in testing -- '
        'v0 is already 0 on every observed entry; the real delay-slot decisions '
        'for cross-block motion are in func_4281e0, see IDO52_AS1_NO_LA_PULL)',
        'L42f170 delay slot entry',
    ),
    (
        'IDO52_AS1_NO_LA_PULL',
        # The confluence point of both func_427800 candidate call sites inside
        # func_4281e0 (the do_xbb_opt transformer).
        'fp = v0;\nL428604:\nv0 = 0x10022660;',
        '''fp = v0;
L428604:
/* === IDO52_AS1_NO_LA_PULL hook === */
if (as1_la_pull_veto()) {
    goto L4289b4;
}
v0 = 0x10022660;''',
        'Applied IDO52_AS1_NO_LA_PULL (cross-BB pull veto in do_xbb_opt/func_4281e0)',
        'L428604 cross-BB move confluence point',
    ),
    (
        'IDO52_AS1_FORCE_LA_PULL',
        # L4286b0 is the UNDO path: as1 has trial-moved the instruction, run
        # reschedule() on both blocks, compared the combined schedule length
        # against the pre-move length and decided the move is not worth it, so
        # it copies the record back and re-asserts the original lengths.
        # L4287b8 is the COMMIT path (it is the one that printf()s the move at
        # as1 -O>=2).  Diverting here keeps a move the heuristic rejected.
        # v1 == 0x28 (the instruction-record stride) on every path into
        # L4286b0, which is all the commit path needs.
        'L4286ac:\nv1 = 0x28;\nL4286b0:\nlo = s5 * v1;',
        '''L4286ac:
v1 = 0x28;
L4286b0:
/* === IDO52_AS1_FORCE_LA_PULL hook === */
if (as1_la_pull_forced()) {
    goto L4287b8;
}
lo = s5 * v1;''',
        'Applied IDO52_AS1_FORCE_LA_PULL (cross-BB pull profitability override)',
        'L4286b0 cross-BB move undo path',
    ),
    (
        'IDO52_AS1_NO_COPY_PROP',
        # func_419268 (the per-BB peephole driven by f_do_passII_opt).  v0 here
        # is func_413564's verdict "this instruction is a copy"; zeroing it is
        # exactly the control flow as1 takes for a non-copy instruction.
        'L4193b0:\n// bdead c1fc000b gp = MEM_U32(sp + 52);\nif ((int32_t)v0 == (int32_t)0) {\ns0 = v0;\ngoto L419414;}',
        '''L4193b0:
// bdead c1fc000b gp = MEM_U32(sp + 52);
/* === IDO52_AS1_NO_COPY_PROP hook === */
if (v0 != 0 && as1_copy_prop_veto(MEM_U32(sp + 100))) {
    v0 = 0;
}
if ((int32_t)v0 == (int32_t)0) {
s0 = v0;
goto L419414;}''',
        'Applied IDO52_AS1_NO_COPY_PROP (pass-II copy-propagation veto in func_419268)',
        'L4193b0 copy-candidate test in func_419268',
    ),
    (
        'IDO52_AS1_NO_PASS1_PROP',
        # f_do_passI_opt's per-instruction call into func_414bf8, the PASS-I
        # copy-propagation layer.  Returning 0 is exactly what func_414bf8's
        # own "nothing changed" exit (L414c8c) returns, so the caller's
        # `s3 = s3 | v0` accumulator sees a clean no-change.
        'v0 = func_414bf8(mem, sp, v0, a0, a1, a2, a3);',
        '''/* === IDO52_AS1_NO_PASS1_PROP hook (Hook 44) === */
v0 = as1_pass1_prop_veto(MEM_U32(v0 + 0))
     ? 0 : func_414bf8(mem, sp, v0, a0, a1, a2, a3);''',
        'Applied IDO52_AS1_NO_PASS1_PROP (pass-I copy-propagation veto in func_414bf8)',
        'func_414bf8 call in f_do_passI_opt',
    ),
    (
        'IDO52_AS1_SCHED_LOAD_HOIST_TRACE hook',
        # Entry of func_4314ec's candidate comparison chain.  Diagnostic only:
        # prints the candidate (s2, start time s3, priority s0) against the
        # incumbent (s4, start time fp) and changes nothing.
        'L431854:\nif ((int32_t)s4 == (int32_t)0) {\nat = s3 < fp;\ngoto L431940;}',
        '''L431854:
/* === IDO52_AS1_SCHED_LOAD_HOIST_TRACE hook (diagnostic only) === */
as1_sched_cand_trace(mem, s2, s0, s3, s4, fp);
if ((int32_t)s4 == (int32_t)0) {
at = s3 < fp;
goto L431940;}''',
        'Applied IDO52_AS1_SCHED_LOAD_HOIST_TRACE (list-scheduler candidate trace)',
        'L431854 candidate comparison entry in func_4314ec',
    ),
    (
        'IDO52_AS1_NO_SCHED_LOAD_HOIST hook (is_node_better pass)',
        # Step 4, spelling A: pass a1=1, where f_is_node_better decides.
        # v0 is its verdict; 0 falls through to L4318b8 which -- with the
        # 0x100226b4 flag non-zero on this pass -- rejects the candidate, i.e.
        # exactly the control flow of "is_node_better said no".
        'L43189c:\n// bdead c1fe010b gp = MEM_U32(sp + 60);\n'
        'if ((int32_t)v0 != (int32_t)0) {\nt8 = MEM_U32(sp + 136);\ngoto L431944;}',
        '''L43189c:
// bdead c1fe010b gp = MEM_U32(sp + 60);
/* === IDO52_AS1_NO_SCHED_LOAD_HOIST hook (is_node_better pass) === */
{
    int lh = as1_sched_load_hoist(mem, s2, s4);
    if (lh > 0) {
        goto L43195c;                   /* reject candidate, keep incumbent */
    } else if (lh < 0) {
        t8 = MEM_U32(sp + 136);
        goto L431944;                   /* candidate becomes the new best */
    }
}
if ((int32_t)v0 != (int32_t)0) {
t8 = MEM_U32(sp + 136);
goto L431944;}''',
        'Applied IDO52_AS1_NO_SCHED_LOAD_HOIST (is_node_better pass, L43189c)',
        'L43189c is_node_better verdict in func_4314ec',
    ),
    (
        'IDO52_AS1_NO_SCHED_LOAD_HOIST hook (priority pass)',
        # Step 4, spelling B: pass a1=0, where the raw node+44 priority and then
        # successor latency / node+48 / node+12 decide.  The hook runs BEFORE
        # the whole tie-break chain and jumps straight to as1's own take
        # (L431944) / reject (L43195c) labels -- merely zeroing the priority
        # comparison is NOT enough, because an equal-priority pair then falls
        # through to the later tie-breaks and the load wins anyway (measured on
        # RPG_BAT/func_8013F258, where load and `lui` both have priority 3).
        't6 = MEM_U32(sp + 116);\n//nop;\nat = t6 < s0;\n'
        'if ((int32_t)at != (int32_t)0) {\nt8 = MEM_U32(sp + 136);\ngoto L431944;}',
        '''/* === IDO52_AS1_NO_SCHED_LOAD_HOIST hook (priority pass) === */
{
    int lh = as1_sched_load_hoist(mem, s2, s4);
    if (lh > 0) {
        goto L43195c;                   /* reject candidate, keep incumbent */
    } else if (lh < 0) {
        t8 = MEM_U32(sp + 136);
        goto L431944;                   /* candidate becomes the new best */
    }
}
t6 = MEM_U32(sp + 116);
//nop;
at = t6 < s0;
if ((int32_t)at != (int32_t)0) {
t8 = MEM_U32(sp + 136);
goto L431944;}''',
        'Applied IDO52_AS1_NO_SCHED_LOAD_HOIST (node+44 priority pass, L4318b8)',
        'L4318b8 priority comparison in func_4314ec',
    ),
    (
        'IDO52_AS1_TIE hook (is_node_better pass)',
        # Anchors on the Hook 7 marker, so Hook 7 must be applied first (it is
        # earlier in this list, so a single run handles a fresh as1.c).
        '/* === IDO52_AS1_NO_SCHED_LOAD_HOIST hook (is_node_better pass) === */',
        '''/* === IDO52_AS1_TIE hook (is_node_better pass) === */
{
    int tb = as1_tie_break(mem, s2, s4, s0, s1,
                           MEM_U32(sp + 116), MEM_U32(sp + 120),
                           MEM_U32(sp + 176));
    if (tb > 0) {
        t8 = MEM_U32(sp + 136);
        goto L431944;                   /* candidate becomes the new best */
    } else if (tb < 0) {
        goto L43195c;                   /* reject candidate, keep incumbent */
    }
}
/* === IDO52_AS1_NO_SCHED_LOAD_HOIST hook (is_node_better pass) === */''',
        'Applied IDO52_AS1_TIE_* (is_node_better pass, L43189c)',
        'L43189c Hook-7 marker in func_4314ec (apply NO_SCHED_LOAD_HOIST first)',
    ),
    (
        'IDO52_AS1_TIE hook (priority pass)',
        '/* === IDO52_AS1_NO_SCHED_LOAD_HOIST hook (priority pass) === */',
        '''/* === IDO52_AS1_TIE hook (priority pass) === */
{
    int tb = as1_tie_break(mem, s2, s4, s0, s1,
                           MEM_U32(sp + 116), MEM_U32(sp + 120),
                           MEM_U32(sp + 176));
    if (tb > 0) {
        t8 = MEM_U32(sp + 136);
        goto L431944;                   /* candidate becomes the new best */
    } else if (tb < 0) {
        goto L43195c;                   /* reject candidate, keep incumbent */
    }
}
/* === IDO52_AS1_NO_SCHED_LOAD_HOIST hook (priority pass) === */''',
        'Applied IDO52_AS1_TIE_* (node+44 priority pass, L4318b8)',
        'L4318b8 Hook-7 marker in func_4314ec (apply NO_SCHED_LOAD_HOIST first)',
    ),
]


def apply_hooks(path):
    with open(path, 'r') as f:
        content = f.read()

    applied = 0
    for probe, old, new, msg, where in HOOKS:
        if probe in content:
            print("[as1 hooks] %s already present, skipping" % probe)
            continue
        if old not in content:
            print("[as1 hooks] WARNING: Could not find %s" % where)
            continue
        if content.count(old) != 1:
            print("[as1 hooks] WARNING: %s anchor is not unique (%d hits) -- skipped"
                  % (probe, content.count(old)))
            continue
        content = content.replace(old, new)
        print("[as1 hooks] %s" % msg)
        applied += 1

    # The NO_LA_PULL hook needs its helper defined before func_4281e0.
    if 'as1_la_pull_veto' in content and 'static int as1_la_pull_veto(void)' not in content:
        anchor = 'static void func_4281e0(uint8_t *mem, uint32_t sp) {'
        if content.count(anchor) == 1:
            content = content.replace(anchor, LA_PULL_HELPER + anchor)
            print("[as1 hooks] Inserted as1_la_pull_veto() helper")
        else:
            print("[as1 hooks] ERROR: func_4281e0 definition not found/unique -- "
                  "NO_LA_PULL will not compile")

    # The NO_BACK_PROP hook needs its helper defined before func_4142d8.
    if 'as1_kandi_veto' in content and \
            'static int as1_kandi_veto(uint32_t insn_word)' not in content:
        anchor = ('static uint32_t func_416ca8(uint8_t *mem, uint32_t sp, '
                  'uint32_t a0) {')
        if content.count(anchor) == 1:
            content = content.replace(anchor, KEEP_ANDI_HELPER + anchor)
            print("[as1 hooks] Inserted as1_kandi_veto() helper")
        else:
            print("[as1 hooks] ERROR: func_416ca8 definition not found/unique -- "
                  "KEEP_ANDI will not compile")

    # The FORCE_NARROW_FOLD hook (45) needs its helper before func_416ca8 too.
    if 'as1_nfold_force' in content and \
            'static int as1_nfold_force(uint32_t insn_word' not in content:
        anchor = ('static uint32_t func_416ca8(uint8_t *mem, uint32_t sp, '
                  'uint32_t a0) {')
        if content.count(anchor) == 1:
            content = content.replace(anchor, NARROW_FOLD_HELPER + anchor)
            print("[as1 hooks] Inserted as1_nfold_force() helper")
        else:
            print("[as1 hooks] ERROR: func_416ca8 definition not found/unique -- "
                  "FORCE_NARROW_FOLD will not compile")

    # The NO_COPY_PROP hook needs its helper defined before func_419268.
    if 'as1_copy_prop_veto' in content and \
            'static int as1_copy_prop_veto(uint32_t insn)' not in content:
        anchor = 'static uint32_t func_419268(uint8_t *mem, uint32_t sp, uint32_t a0) {'
        if content.count(anchor) == 1:
            content = content.replace(anchor, COPY_PROP_HELPER + anchor)
            print("[as1 hooks] Inserted as1_copy_prop_veto() helper")
        else:
            print("[as1 hooks] ERROR: func_419268 definition not found/unique -- "
                  "NO_COPY_PROP will not compile")

    # The NO_PASS1_PROP hook needs its helper defined before f_do_passI_opt.
    if 'as1_pass1_prop_veto' in content and \
            'static int as1_pass1_prop_veto(uint32_t insn)' not in content:
        anchor = ('static uint32_t f_do_passI_opt(uint8_t *mem, uint32_t sp, '
                  'uint32_t a0) {')
        if content.count(anchor) == 1:
            content = content.replace(anchor, PASS1_PROP_HELPER + anchor)
            print("[as1 hooks] Inserted as1_pass1_prop_veto() helper")
        else:
            print("[as1 hooks] ERROR: f_do_passI_opt definition not found/unique -- "
                  "NO_PASS1_PROP will not compile")

    # The NO_SCHED_LOAD_HOIST hook needs its helper defined before func_4314ec.
    if 'as1_sched_load_hoist' in content and \
            'static int as1_sched_load_hoist(uint8_t *mem' not in content:
        anchor = ('static uint32_t func_4314ec(uint8_t *mem, uint32_t sp, '
                  'uint32_t a0, uint32_t a1, uint32_t a2) {')
        if content.count(anchor) == 1:
            content = content.replace(anchor, SCHED_LOAD_HOIST_HELPER + anchor)
            print("[as1 hooks] Inserted as1_sched_load_hoist() helper")
        else:
            print("[as1 hooks] ERROR: func_4314ec definition not found/unique -- "
                  "NO_SCHED_LOAD_HOIST will not compile")

    # The TIE hook needs its helper defined before func_4314ec too.
    if 'as1_tie_break' in content and \
            'static int as1_tie_break(uint8_t *mem' not in content:
        anchor = ('static uint32_t func_4314ec(uint8_t *mem, uint32_t sp, '
                  'uint32_t a0, uint32_t a1, uint32_t a2) {')
        if content.count(anchor) == 1:
            content = content.replace(anchor, TIE_HELPER + anchor)
            print("[as1 hooks] Inserted as1_tie_break() helper")
        else:
            print("[as1 hooks] ERROR: func_4314ec definition not found/unique -- "
                  "TIE hooks will not compile")

    # Hooks 48/49 need their shared helper before func_416528 (the earlier
    # of the two functions in as1.c).
    if 'as1_ksext_veto' in content and \
            'static int as1_ksext_veto(uint32_t insn_word)' not in content:
        anchor = ('static uint32_t func_416528(uint8_t *mem, uint32_t sp, '
                  'uint32_t a0) {')
        if content.count(anchor) == 1:
            content = content.replace(anchor, KSEXT_HELPER + anchor)
            print("[as1 hooks] Inserted as1_ksext_veto()/as1_nbfold_veto() helpers")
        else:
            print("[as1 hooks] ERROR: func_416528 definition not found/unique -- "
                  "KEEP_SEXT/NO_BRANCH_FOLD will not compile")

    # Hook 50 needs its helper before func_41673c (after Hook 48/49's).
    if 'as1_nscomb_veto' in content and \
            'static int as1_nscomb_veto(uint32_t insn_word' not in content:
        anchor = ('static uint32_t func_41673c(uint8_t *mem, uint32_t sp, '
                  'uint32_t a0) {')
        if content.count(anchor) == 1 and 'as1_occ_sel_init' in content:
            content = content.replace(anchor, NSCOMB_HELPER + anchor)
            print("[as1 hooks] Inserted as1_nscomb_veto() helper")
        else:
            print("[as1 hooks] ERROR: func_41673c definition not found/unique "
                  "(or Hook 48 helper missing) -- NO_SHIFT_COMBINE will not compile")

    # Hook 51 needs its helper before func_41b5e8 (after Hook 48's
    # as1_occ_sel, which lands before func_416528).
    if 'as1_njinv_veto' in content and \
            'static int as1_njinv_veto(uint8_t *mem' not in content:
        anchor = 'static void func_41b5e8(uint8_t *mem, uint32_t sp) {'
        if content.count(anchor) == 1 and 'as1_occ_sel_init' in content:
            content = content.replace(anchor, NJINV_HELPER + anchor)
            print("[as1 hooks] Inserted as1_njinv_veto() helper")
        else:
            print("[as1 hooks] ERROR: func_41b5e8 definition not found/unique "
                  "(or Hook 48 helper missing) -- NO_JUMP_INVERT will not compile")

    # Hook 52: prototype before f_redundant (which precedes func_416528,
    # where as1_occ_sel is defined), definition before func_419268.
    if 'as1_kdrel_veto(mem' in content and \
            'static int as1_kdrel_veto(uint8_t *mem, uint32_t idx, uint32_t dead_reg) {' \
            not in content:
        a_proto = ('static uint32_t f_redundant(uint8_t *mem, uint32_t sp, '
                   'uint32_t v0, uint32_t a0, uint32_t a1) {')
        a_def = 'static uint32_t func_419268(uint8_t *mem, uint32_t sp, uint32_t a0) {'
        if content.count(a_proto) == 1 and content.count(a_def) == 1 and \
                'as1_occ_sel_init' in content:
            content = content.replace(a_proto, KDREL_PROTO + a_proto)
            content = content.replace(a_def, KDREL_HELPER + a_def)
            print("[as1 hooks] Inserted as1_kdrel_veto() prototype + helper")
        else:
            print("[as1 hooks] ERROR: f_redundant/func_419268 definition not "
                  "found/unique (or Hook 48 helper missing) -- "
                  "KEEP_DEAD_RELOAD will not compile")

    with open(path, 'w') as f:
        f.write(content)

    print("[as1 hooks] Done (%d new). Rebuild with: "
          "cd tools/ido-static-recomp && make VERSION=5.2" % applied)


if __name__ == '__main__':
    default_path = os.path.join(os.path.dirname(__file__), 'build', '5.2', 'as1.c')
    path = sys.argv[1] if len(sys.argv) > 1 else default_path
    if not os.path.exists(path):
        print("[as1 hooks] ERROR: %s not found. Run 'make setup-ido52' first." % path)
        sys.exit(1)
    apply_hooks(path)
