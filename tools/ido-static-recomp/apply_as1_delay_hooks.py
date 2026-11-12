#!/usr/bin/env python3
"""Apply the as1 CROSS-BB BOOST / DELAY-SLOT hooks to a regenerated as1.c.

Standalone and per-hook idempotent, in the style of apply_as1_hooks.py.  Run it
AFTER apply_as1_hooks.py on build/5.2/as1.c, then rebuild:

    rm build/5.2/as1.c && make VERSION=5.2 c_files
    python3 apply_as1_hooks.py
    python3 apply_as1_delay_hooks.py          # <-- this script
    make VERSION=5.2

ALL hooks are DEFAULT OFF: with none of the environment variables set the
patched as1 is bit identical in behaviour to the unpatched one.

=========================================================================
THE MECHANISM (why these hooks exist)
=========================================================================
as1 has exactly three ways to get an instruction into a branch delay slot,
and only one way to fill a load-use interlock bubble:

  1. f_schedule / f_reorganize_bb (per basic block, before everything else)
     -- the intra-block list scheduler.  It can only move instructions that
     are already IN the block, so it fills the delay slot from ABOVE.

  2. f_do_xbb_opt / func_4281e0 (once per procedure, after all the per-block
     scheduling) -- the CROSS-BASIC-BLOCK BOOST.  For a destination block D it
     walks a candidate list of source blocks S, asks func_427800(S, D) for a
     movable instruction, trial-moves it, reschedules both blocks and keeps
     the move if the combined schedule got shorter.  THIS is the only pass
     that can put a successor's instruction into D's load-use interlock
     bubble, and it is also the only one that can take the delay slot's
     occupant from the BRANCH-TARGET successor.

  3. func_41d7b4 (the per-block emit pass, after do_xbb_opt) -- the
     FALL-THROUGH fill.  Not a move at all: it verifies that the previous
     block in layout order ends exactly at the current pc, then rewinds the
     pc by one word and drops the previous block's delay-slot nop, so the
     next block's first word physically lands in the slot.  Its donor is
     therefore always the fall-through successor.

  4. f_do_branch_opt (last) -- copies the branch target's first word into the
     delay slot and rewrites the branch to its LIKELY form (beq -> beql).
     mips2+ only; mips1 code never reaches its commit path, so it can never
     be the source of a mips1 target's delay-slot occupant.

The 1995 shape "interlock filled from the fall-through successor + delay slot
filled from the branch-target successor" is therefore TWO do_xbb_opt boosts
into the same block, one from each successor.  Two things stop our as1 from
finding them:

  (a) A per-destination CANDIDATE BUDGET.  func_4281e0 scans the source list
      forward from the destination's own position and gives up after 10
      candidates (100 when an internal flag is set).  In a big function the
      destination's own successors sit far down that list and the budget is
      spent on unrelated blocks long before they are reached.
      -> IDO52_AS1_XBB_BUDGET

  (b) The `.set volatile` PIN.  Every instruction record carries a byte at
      +33 copied from global 0x1002f8f8, which `.set volatile` sets to 1 and
      `.set novolatile` clears (f_parse_option, L454038/L454044).
      func_427800 refuses to boost any instruction whose +33 is non-zero.
      ugen wraps every access to a `volatile` extern in `.set volatile`, so a
      `volatile` global used purely as a decomp codegen-steering device also
      pins the two halves of its `lui`/`lXX` macro inside their block --
      something the original non-volatile source never did.
      -> IDO52_AS1_XBB_VOLATILE_OK

Both hooks only widen the SEARCH.  Every move as1 then makes still goes
through its own legality test (func_427550 / f_defuse) and its own
profitability test, exactly as for a candidate it found by itself.

=========================================================================
HOOKS
=========================================================================
IDO52_AS1_XBB_BUDGET=<n>
    Override the per-destination-block candidate budget in func_4281e0
    (as1's own value is 10, or 100 with its internal flag set).  A value
    large enough to exhaust the list (e.g. 200) makes as1 consider every
    source block for every destination.  Costs compile time, nothing else.

IDO52_AS1_XBB_VOLATILE_OK=all|N[,N...]
    Let the boost consider instructions pinned by `.set volatile`
    (inst+33 != 0), globally or at the listed 0-based occurrence indices.
    Occurrences are counted over PINNED candidates only, in the order
    func_427800 reaches them, over the whole translation unit.
    ⚠️ Acting on occurrence N changes the schedule, so later indices shift --
    re-derive with IDO52_AS1_XBB_TRACE=1 after any source or env change.

IDO52_AS1_XBB_TRACE=1|2
    1: every (destination, source) pair examined -- `[AS1-XBB-PAIR]` with the
       running budget -- every candidate returned -- `[AS1-XBB] #N` with the
       candidate's instruction word -- every heuristic undo, and every
       volatile-pinned candidate with its occurrence index `[AS1-XBB-VOL] #N`.
    2: additionally every instruction func_427800 inspects `[AS1-XBB-INSN]`
       with its +33/+34 movability bytes.

IDO52_AS1_NO_FALLTHRU_FILL=all|N[,N...]
    Veto func_41d7b4's fall-through delay-slot fill, globally or at the
    listed 0-based occurrence indices.  Vetoing is byte for byte the control
    flow as1 takes when f_eligible() rejects the candidate: the delay slot
    keeps its nop.

IDO52_AS1_FTF_TRACE=1
    One `[AS1-FTF]` line per fall-through fill opportunity (occurrence index,
    block, donated instruction word, previous block) plus a COMMIT line.

IDO52_AS1_FTF_BACKWARD=all|N[,N...]|occ:N,...      (Hook 54, 2026-09-22)
    Let the fall-through fill run after a BACKWARD conditional branch.
    When func_41d7b4 emits a block that ends in `branch ; nop` it records
    in bb+70 bit 0x02 whether the NEXT block's first word may later be
    dropped into that slot.  The bit requires (a) a delay-slot nop, (b) not
    `beq x,x` (an unconditional b: the fall-through is unreachable) and
    (c) a non-negative displacement field -- i.e. a FORWARD branch, whose
    label is still unresolved (displacement 0) at emit time.  (c) is the
    only test this hook lifts, at the listed 0-based occurrence among
    branches that pass (a)+(b) and fail (c).  Everything that decides
    whether the fill is LEGAL still runs unchanged on the next block:
    f_eligible, f_cannot_trap, func_41d22c (f_defuse: the donated word
    must not define anything live at the branch TARGET -- f_find_branch_target
    resolves backward targets exactly as forward ones -- or load from
    memory), and the pc-contiguity check before the rewind.  func_41d22c
    has no test against the branch's own operands for FORWARD branches
    either: `beq v0,at,L ; li at,2` is legal MIPS I (the branch reads its
    operands before the slot executes) and as1 emits it natively for a
    forward branch.  The backward branch's displacement is already encoded
    relative to its own pc, which the rewind does not move.
IDO52_AS1_FTF_BACKWARD_TRACE=1
    `[AS1-FTFB] #N bb=<block> branch=<word> [-> PERMITTED]`, one line per
    backward conditional branch that failed only test (c).

IDO52_AS1_XBB_SKIP_CAND=N[,N...]
    Skip the N-th candidate func_427800 would RETURN to the boost driver
    (0-based occurrence over accepted candidates, whole translation unit).
    The veto takes func_427800's own "not movable" path (L427b50), so it
    keeps scanning and offers the NEXT movable instruction of the same
    source block instead -- a SOURCE-preference steer for which instruction
    gets boosted into a delay/interlock bubble (e.g. the LOAD base's lui
    where as1 offers the STORE base's).  Every subsequent candidate still
    passes as1's own legality and profitability tests.
    ⚠️ Indices shift when a skip changes what is scanned later -- re-derive
    with IDO52_AS1_XBB_PICK_TRACE=1 after any change.

IDO52_AS1_XBB_PICK_TRACE=1
    One `[AS1-XBB-PICK]` line per accepted candidate (occurrence index,
    source/destination blocks, record index, instruction word).

IDO52_AS1_BOPT_TRACE=1
IDO52_AS1_NO_BOPT=N[,N...]|all   -- veto branch-opt candidate N (same index space)
    Diagnostic trace of f_do_branch_opt's branch-likely conversion.

=========================================================================
FIRST CONSUMER
=========================================================================
ENDING/func_8013A318 (1,162 insns) sat at an honest SEMI, +2 surplus words:
two load-use interlock nops that the target does not contain.  With

    @CompileEnv: IDO52_PRECOLOR_WEB=a0:35 IDO52_AS1_XBB_BUDGET=200
                 IDO52_AS1_XBB_VOLATILE_OK=2

the raw body is 1162 words -- EXACTLY the target -- with zero surplus and
zero missing instructions, 1156/1162 mnemonic-identical, and both walls gone:
word [65] is the interlock `lui a1` and [68] the branch-target `andi`; [387]
is the interlock `lui` and [392] the delay-slot `lui`.  What remains is
register-field drift plus the three known count-neutral or/andi transposition
pairs.
"""

import os
import sys

HELPER = r'''
/* === IDO52_AS1 cross-BB-boost / delay-slot hooks: support ===
 * Inserted before the first function definition: the hook sites live in three
 * different functions (func_4281e0, func_427800, func_41d7b4, f_do_branch_opt)
 * spread across the file. */
struct as1_df_sel { int mode; int list[128]; int nlist; };  /* 0=off 1=all 2=list */

static void as1_df_parse(struct as1_df_sel *s, const char *e) {
    s->mode = 0;
    s->nlist = 0;
    if (e == NULL || e[0] == '\0') return;
    if (strcmp(e, "1") == 0 || strcmp(e, "all") == 0) { s->mode = 1; return; }
    {
        char *p = (char *)e;
        if (strncmp(p, "occ:", 4) == 0) p += 4;
        s->mode = 2;
        while (*p != '\0' && s->nlist < 128) {
            s->list[s->nlist++] = (int)strtol(p, &p, 0);
            while (*p == ',' || *p == ' ') p++;
        }
    }
}

static int as1_df_hit(const struct as1_df_sel *s, int idx) {
    int i;
    if (s->mode == 1) return 1;
    if (s->mode == 2)
        for (i = 0; i < s->nlist; i++) if (s->list[i] == idx) return 1;
    return 0;
}

static struct as1_df_sel as1_ftf_no;
static struct as1_df_sel as1_xbb_vol;
static struct as1_df_sel as1_xbb_skip;
static struct as1_df_sel as1_bopt_force;
static int as1_xbb_vol_seq;
static int as1_xbb_pick_seq;
static int as1_ftf_trace, as1_xbb_trace, as1_bopt_trace, as1_xbb_pick_trace;

static void as1_df_init(void) {
    static int inited = 0;
    if (inited) return;
    inited = 1;
    as1_df_parse(&as1_ftf_no, getenv("IDO52_AS1_NO_FALLTHRU_FILL"));
    as1_df_parse(&as1_xbb_vol, getenv("IDO52_AS1_XBB_VOLATILE_OK"));
    as1_df_parse(&as1_xbb_skip, getenv("IDO52_AS1_XBB_SKIP_CAND"));
    as1_df_parse(&as1_bopt_force, getenv("IDO52_AS1_FORCE_BOPT"));
    as1_ftf_trace = getenv("IDO52_AS1_FTF_TRACE") != NULL;
    { const char *e = getenv("IDO52_AS1_XBB_TRACE");
      as1_xbb_trace = (e && *e) ? (int)strtol(e, NULL, 0) : 0; }
    as1_bopt_trace = getenv("IDO52_AS1_BOPT_TRACE") != NULL;
    as1_xbb_pick_trace = getenv("IDO52_AS1_XBB_PICK_TRACE") != NULL;
}

/* --- fall-through delay-slot fill (func_41d7b4) -------------------------
 * bb layout (established from func_41d7b4 / f_do_branch_opt / func_4281e0):
 *   bb+12  -> instruction record array, stride 0x28, 1-based
 *   bb+60  u16 record count      bb+68 u32 flags     bb+70 u8 flags2
 *   bb+72  u16 block number      bb+74 u16 scheduled length
 *   bb+78  u16 emitted word count                    bb+80 -> u32 word array
 *   bb+84  u32 start address in the section
 * instruction record: +0 the 32-bit word (0x7fffffff = pseudo/label),
 *   +33 "assembled under .set volatile", +34 movable flag. */
static int as1_ftf_seq;

static int as1_ftf_veto(uint8_t *mem, uint32_t bb, uint32_t prev, uint32_t insn) {
    int idx;
    as1_df_init();
    idx = as1_ftf_seq++;
    if (as1_ftf_trace)
        fprintf(stderr, "[AS1-FTF] #%d bb=%08x@%04x insn=%08x prev=%08x@%04x "
                        "prevn=%d%s\n",
                idx, bb, bb ? MEM_U32(bb + 84) : 0, insn, prev,
                prev ? MEM_U32(prev + 84) : 0,
                prev ? (int)MEM_U16(prev + 78) : -1,
                as1_df_hit(&as1_ftf_no, idx) ? " -> VETOED" : "");
    return as1_df_hit(&as1_ftf_no, idx);
}

static void as1_ftf_commit(uint8_t *mem, uint32_t bb, uint32_t insn) {
    as1_df_init();
    if (as1_ftf_trace)
        fprintf(stderr, "[AS1-FTF] #%d COMMIT bb=%08x insn=%08x\n",
                as1_ftf_seq - 1, bb, insn);
}

/* --- cross-BB boost (func_4281e0 / do_xbb_opt) ------------------------- */
static int as1_xbb_seq;

static void as1_xbb_pair(uint8_t *mem, uint32_t dst, uint32_t src,
                         uint32_t budget_used, uint32_t budget_max) {
    as1_df_init();
    if (!as1_xbb_trace) return;
    fprintf(stderr, "[AS1-XBB-PAIR] dst=%08x idx=%d n=%d | src=%08x idx=%d n=%d "
                    "flags=%08x used=%d max=%d\n",
            dst, (int)MEM_U16(dst + 72), (int)MEM_U16(dst + 60),
            src, (int)MEM_U16(src + 72), (int)MEM_U16(src + 60),
            MEM_U32(src + 68), (int)budget_used, (int)budget_max);
}

/* rec is the 1-based index of the candidate inside the source block. */
static void as1_xbb_cand(uint8_t *mem, uint32_t src, uint32_t dst, uint32_t rec) {
    as1_df_init();
    as1_xbb_seq++;
    if (!as1_xbb_trace) return;
    {
        uint32_t r = MEM_U32(src + 12) + rec * 0x28u;
        fprintf(stderr, "[AS1-XBB] #%d src=%08x(n=%d,len=%d) dst=%08x(n=%d,len=%d) "
                        "rec=%d insn=%08x\n",
                as1_xbb_seq - 1, src, (int)MEM_U16(src + 60),
                (int)MEM_U16(src + 74), dst, (int)MEM_U16(dst + 60),
                (int)MEM_U16(dst + 74), (int)rec, MEM_U32(r + 0));
    }
}

static void as1_xbb_verdict(uint8_t *mem, int commit) {
    as1_df_init();
    if (as1_xbb_trace)
        fprintf(stderr, "[AS1-XBB] #%d %s\n", as1_xbb_seq - 1,
                commit ? "COMMIT" : "undo (heuristic)");
}

/* Per-destination candidate budget (as1's own: 10, or 100 with its flag). */
static int as1_xbb_budget_v = -2;
static uint32_t as1_xbb_budget(uint32_t cur) {
    if (as1_xbb_budget_v == -2) {
        const char *e = getenv("IDO52_AS1_XBB_BUDGET");
        as1_xbb_budget_v = (e && *e) ? (int)strtol(e, NULL, 0) : -1;
    }
    return as1_xbb_budget_v < 0 ? cur : (uint32_t)as1_xbb_budget_v;
}

/* Per-instruction inspection trace inside func_427800. */
static void as1_xbb_insn(uint8_t *mem, uint32_t src, uint32_t dst, int idx,
                         uint32_t rec) {
    as1_df_init();
    if (as1_xbb_trace < 2) return;
    fprintf(stderr, "[AS1-XBB-INSN] src=%08x dst=%08x i=%d insn=%08x "
                    "f33=%d f34=%d\n",
            src, dst, idx, MEM_U32(rec + 0), (int)MEM_U8(rec + 33),
            (int)MEM_U8(rec + 34));
}

/* Called for every boost candidate as1 has pinned because it was assembled
   under `.set volatile` (inst+33, from global 0x1002f8f8).  Returns 1 to let
   the candidate be considered anyway; as1's own legality and profitability
   tests still decide whether the move happens. */
static int as1_xbb_vol_ok(uint8_t *mem, uint32_t src, uint32_t dst, int i,
                          uint32_t rec) {
    int idx, hit;
    as1_df_init();
    idx = as1_xbb_vol_seq++;
    hit = as1_df_hit(&as1_xbb_vol, idx);
    if (as1_xbb_trace)
        fprintf(stderr, "[AS1-XBB-VOL] #%d src=%08x dst=%08x i=%d insn=%08x%s\n",
                idx, src, dst, i, MEM_U32(rec + 0), hit ? " -> ALLOWED" : "");
    return hit;
}

/* Called at func_427800's SUCCESS exit, once per candidate it is about to
   return to the boost driver.  Returning 1 sends it down its own
   "not movable" path (L427b50), so it keeps scanning and offers the NEXT
   movable instruction of the same source block instead.  Every candidate it
   then returns still passes as1's own legality and profitability tests. */
static int as1_xbb_pick(uint8_t *mem, uint32_t src, uint32_t dst, int i,
                        uint32_t rec) {
    int idx, hit;
    as1_df_init();
    idx = as1_xbb_pick_seq++;
    hit = as1_df_hit(&as1_xbb_skip, idx);
    if (as1_xbb_pick_trace)
        fprintf(stderr, "[AS1-XBB-PICK] #%d src=%08x dst=%08x i=%d insn=%08x%s\n",
                idx, src, dst, i, MEM_U32(rec + 0), hit ? " -> SKIPPED" : "");
    return hit;
}

/* --- branch-likely conversion (f_do_branch_opt), diagnostic only ------- */
static int as1_bopt_seq;

/* Hook 46: force the mips1 legality verdict on one f_do_branch_opt candidate.
 *
 * f_do_branch_opt is the ONLY pass that puts a COPY of the branch target's
 * first word into the delay slot and keeps the original -- as1 commits it by
 * writing target[0] one word before the branch and incrementing the branch
 * displacement.  On mips2+ the ISA test at L40a838 sends the candidate
 * straight to `v0 = 1` (accepted, no legality check) because the rewrite
 * there is to the branch-LIKELY form, which only executes the copy when the
 * branch is taken.  On mips1 there is no likely form, so the copy runs on
 * BOTH paths and as1 makes the candidate pass f_defuse / f_cannot_trap /
 * the register-interference test instead.
 *
 * Retail contains this transformation in places our C does not, i.e. the
 * original source shape passed that test and ours does not.  This hook lets
 * one occurrence through so the difference can be measured.  The copy itself
 * is as1's own committed code, unmodified.
 *
 * ⚠️ NOT automatically sound.  The test it overrides is the one that proves
 * the copied instruction is safe to execute when the branch is NOT taken.
 * Every use must be byte-verified against the retail bytes -- a forced
 * candidate that does not reproduce the target is a wrong answer, not a
 * near miss.  Enumerate the occurrence space with IDO52_AS1_BOPT_TRACE=1.
 */
static int as1_bopt_forced(int seq) {
    as1_df_init();
    return as1_df_hit(&as1_bopt_force, seq);
}

static void as1_bopt_cand(uint8_t *mem, uint32_t bb, uint32_t tgt, uint32_t insn) {
    as1_df_init();
    as1_bopt_seq++;
    if (as1_bopt_trace)
        fprintf(stderr, "[AS1-BOPT] #%d bb=%08x@%04x tgt=%08x@%04x insn=%08x\n",
                as1_bopt_seq - 1, bb, MEM_U32(bb + 84), tgt,
                MEM_U32(tgt + 84), insn);
}
'''

HOOKS = [
    # (probe, old, new, message, where)
    (
        'IDO52_AS1_FORCE_BOPT hook',
        # f_do_branch_opt's FINAL verdict.  v0 is the mips1 legality result
        # (f_defuse + f_cannot_trap + the register-interference test); on
        # mips2+ the ISA test upstream already set it to 1 unconditionally,
        # because the mips2 rewrite is to the branch-LIKELY form and the
        # copy then only runs when the branch is taken.  Forcing it here
        # sends the candidate down as1's OWN commit path, which copies the
        # branch target's first word into the delay slot and increments the
        # branch displacement.  Nothing about the emission is synthesised.
        'L40a8f8:\nif ((int32_t)v0 == (int32_t)0) {\n//nop;\ngoto L40aab4;}',
        'L40a8f8:\n/* === IDO52_AS1_FORCE_BOPT hook === */\nif ((int32_t)v0 == (int32_t)0 && as1_bopt_forced(as1_bopt_seq - 1)) v0 = 1;\nif ((int32_t)v0 == (int32_t)0) {\n//nop;\ngoto L40aab4;}',
        'IDO52_AS1_FORCE_BOPT hook',
        'f_do_branch_opt final verdict',
    ),
    (
        'IDO52_AS1_NO_BOPT hook',
        # The VETO direction of Hook 46.  f_do_branch_opt's final verdict `v0`
        # is the mips1 legality result; when it is non-zero as1 commits the
        # transform (copy the branch target's first word into the delay slot,
        # bump the displacement).  Zeroing it sends the candidate down as1's
        # OWN reject path (`goto L40aab4`) -- the exact path any candidate
        # that fails f_defuse / f_cannot_trap / the register-interference test
        # already takes -- so a vetoed candidate is an outcome as1 itself
        # produces.  Sound by construction (unlike FORCE_BOPT, which overrides
        # a safety test).  Retail leaves some conditional-branch delay slots
        # empty where ours copies; per-candidate, because the SAME function
        # often needs another candidate (e.g. the unconditional `b` before an
        # epilogue) to keep firing.  Index space == IDO52_AS1_BOPT_TRACE.
        'L40a8f8:\n/* === IDO52_AS1_FORCE_BOPT hook === */\n'
        'if ((int32_t)v0 == (int32_t)0 && as1_bopt_forced(as1_bopt_seq - 1)) v0 = 1;\n',
        'L40a8f8:\n/* === IDO52_AS1_FORCE_BOPT hook === */\n'
        'if ((int32_t)v0 == (int32_t)0 && as1_bopt_forced(as1_bopt_seq - 1)) v0 = 1;\n'
        '/* === IDO52_AS1_NO_BOPT hook === */\n'
        '{\n'
        '    static struct as1_df_sel as1_bopt_no;\n'
        '    static int as1_bopt_no_init = 0;\n'
        '    if (!as1_bopt_no_init) {\n'
        '        as1_bopt_no_init = 1;\n'
        '        as1_df_parse(&as1_bopt_no, getenv("IDO52_AS1_NO_BOPT"));\n'
        '    }\n'
        '    if ((int32_t)v0 != (int32_t)0 && as1_df_hit(&as1_bopt_no, as1_bopt_seq - 1))\n'
        '        v0 = 0;\n'
        '}\n',
        'Applied IDO52_AS1_NO_BOPT (per-candidate branch-opt veto)',
        'f_do_branch_opt final verdict (veto direction)',
    ),
    (
        'IDO52_AS1_FTF hook (candidate)',
        # func_41d7b4: s2 = current bb, 0x1002239c = previous bb in layout
        # order, a0 = current bb's first emitted word.  v0 is f_eligible's
        # verdict; zeroing it is exactly as1's ineligible-candidate path.
        'L41daa4:\n'
        '// bdead 4078000b gp = MEM_U32(sp + 56);\n'
        'if ((int32_t)v0 == (int32_t)0) {\n//nop;\ngoto L41dbbc;}',
        '''L41daa4:
// bdead 4078000b gp = MEM_U32(sp + 56);
/* === IDO52_AS1_FTF hook (candidate) === */
if (v0 != 0 && as1_ftf_veto(mem, s2, MEM_U32(0x1002239c), a0)) {
    v0 = 0;
}
if ((int32_t)v0 == (int32_t)0) {
//nop;
goto L41dbbc;}''',
        'Applied IDO52_AS1_NO_FALLTHRU_FILL / IDO52_AS1_FTF_TRACE (candidate)',
        'L41daa4 fall-through delay-fill candidate test in func_41d7b4',
    ),
    (
        'IDO52_AS1_FTF hook (commit)',
        # The commit: the previous block ends exactly at the current pc, so
        # as1 rewinds the pc one word and drops its delay-slot nop.
        'if ((int32_t)t8 != (int32_t)a0) {\n'
        't9 = a0 + 0xfffffffc;\n'
        'goto L41dbbc;}\n'
        't9 = a0 + 0xfffffffc;\n'
        'MEM_U32(v1 + 0) = t9;',
        '''if ((int32_t)t8 != (int32_t)a0) {
t9 = a0 + 0xfffffffc;
goto L41dbbc;}
t9 = a0 + 0xfffffffc;
/* === IDO52_AS1_FTF hook (commit) === */
as1_ftf_commit(mem, s2, MEM_U32(MEM_U32(s2 + 80)));
MEM_U32(v1 + 0) = t9;''',
        'Applied IDO52_AS1_FTF_TRACE (commit site)',
        'func_41d7b4 fall-through delay-fill commit',
    ),
    (
        'IDO52_AS1_FTF_BACKWARD hook',
        # func_41d7b4, per emitted block ending in `branch ; nop`: bb+70 bit
        # 0x02 = "fall-through fillable" := slot nop && !(beq x,x) &&
        # displacement >= 0.  v0 is the last term; sp+288 is the branch word.
        'v0 = MEM_S16(sp + 290);\n'
        '//nop;\n'
        't4 = (int)v0 < (int)0x0;\n'
        'v0 = t4 ^ 0x1;\n'
        'L41e588:',
        '''v0 = MEM_S16(sp + 290);
//nop;
t4 = (int)v0 < (int)0x0;
v0 = t4 ^ 0x1;
/* === IDO52_AS1_FTF_BACKWARD hook (Hook 54) === */
if (v0 == 0 && as1_ftf_bwd_ok(mem, s2, MEM_U32(sp + 288))) v0 = 1;
L41e588:''',
        'Applied IDO52_AS1_FTF_BACKWARD (fall-through fill after a backward branch)',
        'func_41d7b4 L41e588 bb+70 bit-0x02 (fall-through fillable) computation',
    ),
    (
        'IDO52_AS1_XBB_BUDGET hook',
        # func_4281e0: sp+100 is the per-destination candidate budget, set to
        # 10 or 100 a few lines above; sp+104 is the running count.
        'L4284d0:\nt8 = MEM_U32(sp + 156);',
        '''L4284d0:
/* === IDO52_AS1_XBB_BUDGET hook === */
MEM_U32(sp + 100) = as1_xbb_budget(MEM_U32(sp + 100));
t8 = MEM_U32(sp + 156);''',
        'Applied IDO52_AS1_XBB_BUDGET (per-destination candidate budget)',
        'L4284d0 budget setup in func_4281e0',
    ),
    (
        'IDO52_AS1_XBB pair trace',
        'L4284f4:\n'
        't2 = MEM_U32(sp + 88);\n'
        '//nop;\n'
        's3 = MEM_U32(t2 + 0);',
        '''L4284f4:
t2 = MEM_U32(sp + 88);
//nop;
s3 = MEM_U32(t2 + 0);
/* === IDO52_AS1_XBB pair trace === */
as1_xbb_pair(mem, s4, s3, MEM_U32(sp + 104), MEM_U32(sp + 100));''',
        'Applied IDO52_AS1_XBB_TRACE (source/destination pair trace)',
        'L4284f4 source-block loop in func_4281e0',
    ),
    (
        'IDO52_AS1_XBB hook (candidate)',
        'fp = v0;\nL428604:\n/* === IDO52_AS1_NO_LA_PULL hook === */',
        '''fp = v0;
L428604:
/* === IDO52_AS1_XBB hook (candidate) === */
as1_xbb_cand(mem, s3, s4, v0);
/* === IDO52_AS1_NO_LA_PULL hook === */''',
        'Applied IDO52_AS1_XBB_TRACE (candidate site)',
        'L428604 cross-BB boost candidate in func_4281e0',
    ),
    (
        'IDO52_AS1_XBB hook (undo)',
        'L4286b0:\n/* === IDO52_AS1_FORCE_LA_PULL hook === */',
        '''L4286b0:
/* === IDO52_AS1_XBB hook (undo) === */
as1_xbb_verdict(mem, 0);
/* === IDO52_AS1_FORCE_LA_PULL hook === */''',
        'Applied IDO52_AS1_XBB_TRACE (undo site)',
        'L4286b0 cross-BB boost undo path',
    ),
    (
        'IDO52_AS1_XBB insn trace',
        'L427900:\nv0 = MEM_U32(s0 + 0);',
        '''L427900:
/* === IDO52_AS1_XBB insn trace === */
as1_xbb_insn(mem, s1, s6, (int)s2, s0);
v0 = MEM_U32(s0 + 0);''',
        'Applied IDO52_AS1_XBB_TRACE=2 (per-instruction inspection trace)',
        'L427900 instruction loop in func_427800',
    ),
    (
        'IDO52_AS1_XBB_VOLATILE_OK hook',
        # func_427800 refuses any candidate whose +33 byte is set -- the
        # `.set volatile` pin.  Clearing it for a selected occurrence puts the
        # candidate back on exactly the path an unpinned instruction takes.
        't9 = MEM_U8(s0 + 33);\n'
        'a3 = sp + 0x70;\n'
        'if ((int32_t)t9 != (int32_t)0) {\n'
        '//nop;\n'
        'goto L427b50;}',
        '''t9 = MEM_U8(s0 + 33);
a3 = sp + 0x70;
/* === IDO52_AS1_XBB_VOLATILE_OK hook === */
if (t9 != 0 && as1_xbb_vol_ok(mem, s1, s6, (int)s2, s0)) {
    t9 = 0;
}
if ((int32_t)t9 != (int32_t)0) {
//nop;
goto L427b50;}''',
        'Applied IDO52_AS1_XBB_VOLATILE_OK (.set volatile boost pin override)',
        'func_427800 .set-volatile movability test',
    ),
    (
        'IDO52_AS1_XBB_SKIP_CAND hook',
        # func_427800's success exit: v0 = s2 is the accepted candidate's
        # 1-based record index; L427b50 is its own keep-scanning path.
        'if ((int32_t)v0 == (int32_t)0) {\n'
        '//nop;\n'
        'goto L427b50;}\n'
        '//nop;\n'
        'v0 = s2;\n'
        'goto L427c24;\n'
        'v0 = s2;',
        '''if ((int32_t)v0 == (int32_t)0) {
//nop;
goto L427b50;}
//nop;
/* === IDO52_AS1_XBB_SKIP_CAND hook === */
if (as1_xbb_pick(mem, s1, s6, (int)s2, s0)) {
    goto L427b50;
}
v0 = s2;
goto L427c24;
v0 = s2;''',
        'Applied IDO52_AS1_XBB_SKIP_CAND / IDO52_AS1_XBB_PICK_TRACE '
        '(boost candidate source-preference veto)',
        'func_427800 success exit (accepted boost candidate)',
    ),
    (
        'IDO52_AS1_BOPT hook',
        'L40a838:\n'
        '// bdead c1fe000b gp = MEM_U32(sp + 52);\n'
        'if ((int32_t)v0 == (int32_t)0) {',
        '''L40a838:
// bdead c1fe000b gp = MEM_U32(sp + 52);
/* === IDO52_AS1_BOPT hook === */
if (v0 != 0) as1_bopt_cand(mem, s0, s1, MEM_U32(MEM_U32(s1 + 80)));
if ((int32_t)v0 == (int32_t)0) {''',
        'Applied IDO52_AS1_BOPT_TRACE (branch-likely conversion trace)',
        'L40a838 branch-likely candidate in f_do_branch_opt',
    ),
]

BWD_HELPER = r'''
/* Hook 54: IDO52_AS1_FTF_BACKWARD -- see the module docstring.  Called
   from func_41d7b4 at the bb+70 bit-0x02 computation for a block ending in
   a conditional branch + delay-slot nop whose displacement is NEGATIVE
   (a backward branch).  Returns 1 to record the block as fall-through
   fillable, as it would be for a forward branch. */
static struct as1_df_sel as1_ftfb_sel;
static int as1_ftfb_seq, as1_ftfb_inited, as1_ftfb_trace;

static int as1_ftf_bwd_ok(uint8_t *mem, uint32_t bb, uint32_t word) {
    int idx, hit;
    (void)mem;
    if (!as1_ftfb_inited) {
        as1_ftfb_inited = 1;
        as1_df_parse(&as1_ftfb_sel, getenv("IDO52_AS1_FTF_BACKWARD"));
        as1_ftfb_trace = getenv("IDO52_AS1_FTF_BACKWARD_TRACE") != NULL;
    }
    if (as1_ftfb_sel.mode == 0 && !as1_ftfb_trace) return 0;
    idx = as1_ftfb_seq++;
    hit = as1_df_hit(&as1_ftfb_sel, idx);
    if (as1_ftfb_trace)
        fprintf(stderr, "[AS1-FTFB] #%d bb=%08x branch=%08x%s\n",
                idx, bb, word, hit ? " -> PERMITTED" : "");
    return hit;
}

'''

HELPER_ANCHOR = ('static void f_add_address_pair(uint8_t *mem, uint32_t sp, '
                 'uint32_t a0, uint32_t a1, uint32_t a2) {')


def apply_hooks(path):
    with open(path) as f:
        content = f.read()

    applied = 0
    for probe, old, new, msg, where in HOOKS:
        if probe in content:
            print("[as1 xbb hooks] %s already present, skipping" % probe)
            continue
        if content.count(old) != 1:
            print("[as1 xbb hooks] WARNING: anchor for %s not unique/found "
                  "(%d hits) -- skipped (%s)" % (probe, content.count(old), where))
            continue
        content = content.replace(old, new)
        print("[as1 xbb hooks] %s" % msg)
        applied += 1

    if 'as1_ftf_veto' in content and 'static int as1_ftf_veto(' not in content:
        if content.count(HELPER_ANCHOR) == 1:
            content = content.replace(HELPER_ANCHOR, HELPER + HELPER_ANCHOR)
            print("[as1 xbb hooks] Inserted helper block")
        else:
            print("[as1 xbb hooks] ERROR: helper anchor not unique -- "
                  "hooks will not compile")

    if 'as1_ftf_bwd_ok(mem' in content and 'static int as1_ftf_bwd_ok(' not in content:
        if content.count(HELPER_ANCHOR) == 1:
            content = content.replace(HELPER_ANCHOR, BWD_HELPER + HELPER_ANCHOR)
            print("[as1 xbb hooks] Inserted Hook 54 helper")
        else:
            print("[as1 xbb hooks] ERROR: helper anchor not unique -- "
                  "Hook 54 will not compile")

    with open(path, 'w') as f:
        f.write(content)
    print("[as1 xbb hooks] Done (%d new). Rebuild with: make VERSION=5.2" % applied)


if __name__ == '__main__':
    default = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                           'ido-private', 'build', '5.2', 'as1.c')
    path = sys.argv[1] if len(sys.argv) > 1 else default
    if not os.path.exists(path):
        print("[as1 xbb hooks] ERROR: %s not found" % path)
        sys.exit(1)
    apply_hooks(path)
