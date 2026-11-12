#!/usr/bin/env python3
"""Hook 62: IDO52_AS1_DIV_MACRO -- emit the div/divu/rem/remu macro in
ASPSX's order (divide first, zero/overflow check and mflo/mfhi DEFERRED)
instead of as1's fixed `div ; check ; mflo`.

Idempotent (probes for its own marker); independent of every other applier
(its anchors are f_parsestmt's / f_gendiv's own recomp labels).  Part of the
post-`make clean` chain (CLAUDE.md Known Gotchas / UOPT-HOOKS-REFERENCE).

    python3 tools/ido-static-recomp/apply_as1_divmacro_hooks.py [--as1 PATH] [--rebuild]

WHY
---
Retail's library-region IDO code was assembled by ASPSX, not as1 (CLAUDE.md
"THIRD PIPELINE").  For a register-divisor `div rd,rs,rt` (and divu / rem /
remu) as1's f_gendiv emits, in one go,

    div  $zero,rs,rt
    bnez rt,1f ; nop ; break 7                       (zero check)
 1: li at,-1 ; bne rt,at,2f ; lui at,0x8000 ;
    bne rs,at,2f ; nop ; break 6                     (signed overflow check)
 2: mflo rd                                          (mfhi for rem/remu)

ASPSX emits the `div $zero,rs,rt` at once but DEFERS both the check and the
mflo/mfhi (tools/maspsx/maspsx/__init__.py defer_div_macros, the model this
hook implements):
  * the CHECK is flushed before the first later instruction that writes rs or
    rt or uses $at (a symbolic store's `lui at` counts), and at any block end
    (label, branch, jump, directive);
  * the MFLO/MFHI is flushed before the first later instruction that reads or
    writes rd or touches HI/LO, and at any block end -- with the check first
    if rd is rs or rt;
  * at a block end the check goes first, then the mflo -- except `.set
    noreorder` (mflo first) and an immediate-compare branch macro that reads
    rd (`beq rd,0,L`: mflo first, maspsx's _spu_setVoiceAttr evidence).
Retail _SsStart 0x8008BC34: `div zero,t7,v1 ; lui t8 ; lw t8 ; nop ;
addiu t9 ; mflo a1 ; andi a1 ; <check> ; lui at ; b ; sw t9` -- as1's list
scheduler already reproduces the in-block order once the macro is deferred.

HOW
---
1. f_gendiv (recomp L464ac4, right after the `div $zero,rs,rt` has been
   emitted): for a register divisor with rd != $zero and the hook on for this
   occurrence, record the pending check (signed?, rs, rt) and the pending
   mflo/mfhi (op, rd) and return -- as1's own tail is skipped.
2. f_parsestmt (recomp L4563b0, before each binasm record is processed):
   classify the record (asmtype, opcode, format, register fields) and flush
   what ASPSX would flush, by calling the SAME emitters f_gendiv calls, with
   the same arguments, in the same order (f_emitbene / f_emitnop /
   f_emitspec / f_loadimmed / f_mark_unmovable / f_emitreg2, and the +34 byte
   clears).  Any record the classifier does not positively know is a
   flush-everything point, so an unfamiliar instruction can only cost
   fidelity, never correctness.
3. f_parsestmt exit (L457fc4): anything still pending is flushed, so no
   deferral ever crosses a chunk handed back to f_main.
4. SAFETY NET: every machine record emitted while something is pending is
   decoded (MIPS I) and checked -- a write to rs/rt under a pending check, a
   read/write of rd or any HI/LO op under a pending mflo, or any branch /
   jump / trap -- and a violation aborts the compile (exit 1) rather than
   miscompile.  The classifier's whitelist is: addu and or xor slt sltu sll
   sra srl subu (fmt afrrr; immediates only in their single-word range), li
   (afri), move not (afrr), la lb lbu lh lhu lw sb sh sw (afra).

Nothing is created and nothing is dropped: the words are exactly as1's own
expansion, moved later in the stream.  Immediate divisors (as1's shift / `li
at` forms), 64-bit divides (f_genddiv) and `mul` are untouched.

ENV
---
IDO52_AS1_DIV_MACRO=aspsx                 defer every register-divisor macro
IDO52_AS1_DIV_MACRO=aspsx:0,2             only occurrences #0 and #2 (0-based,
                                          arrival order over the whole TU; a
                                          BARE number is an index list too)
IDO52_AS1_DIV_MACRO=aspsx-mffirst[:N,..]  as `aspsx`, but when ONE record triggers
                                          both flushes the mflo/mfhi goes first
                                          (retail 0x8008F148 / 0x8008EC58
                                          `div ; mflo t2 ; <check> ; lui at ;
                                          sb t2,sym`; maspsx puts the check
                                          first).  Still check-first when rd is
                                          rs or rt.
IDO52_AS1_DIV_MACRO_TRACE=1               `[AS1-DIVM] #N op=.. rd rs rt ->
                                          DEFERRED|stock` and every flush with
                                          the record that caused it

Default OFF: with the variable unset the helpers return before touching
anything (proven: build-c SHA1 + regression set, UOPT-HOOKS-REFERENCE Hook 62).

HOOK 62d (2026-09-26): three more ASPSX flush rules, each gated separately
and only active on top of IDO52_AS1_DIV_MACRO.  Value: `all` or
`occ:N[,N...]` (0-based, counted over the records that reach that rule's
decision while a check is pending; a bare number is REJECTED, not read as
"on").  Retail evidence: GetMetaEvent 0x800946F4..0x80094738 and
0x800947F0..0x80094830.
IDO52_AS1_DIV_MACRO_MUL_PASS      a `mul rd,rs,rt` / `mul rd,rs,K` record
                                  passes the pending checks (a pending
                                  mflo/mfhi is flushed first: HI/LO), unless
                                  rd is a check's rs/rt, or a SIGNED check is
                                  pending and f_genmul will use $at (`move
                                  at`/`negu at` copy: K<0, or rd==rs with >=2
                                  set bits; the over-budget `li at` fallback
                                  is caught in f_genmul L462dd0).
IDO52_AS1_DIV_MACRO_QUEUE         a register-divisor div/divu/rem/remu that
                                  will itself be deferred, and an unsigned
                                  `divu/remu rd,rs,2^k` (one srl / andi),
                                  pass the pending checks; checks queue and
                                  flush oldest first.
IDO52_AS1_DIV_MACRO_SPLIT_CMPBR   a register compare-branch macro (bge bgeu
                                  bgt ... -> `slt[u] at ; beqz|bnez at`):
                                  flush the mflo/mfhi it reads, emit the
                                  slt, then the checks (f_parseafrrl
                                  L44f3ac), then the branch.  Not split while
                                  a signed check is pending (it uses $at).
Safety net extended: under a pending signed check any emitted word that
writes $at aborts the compile.

HOOK 62b: IDO52_AS1_DIV_POW2 (signed div/rem by an IMMEDIATE power of two)
---------------------------------------------------------------------------
ugen emits `div rd,rs,2^k` / `rem rd,rs,2^k` as macros (cc -S); as1's
func_463eb8 expands them.  Retail's ASPSX signed div is always
    bgez rs,1f ; move at,rs ; addiu at,at,2^k-1 ; 1: sra rd,at,k
(as1: `bgez ; sra rd,rs,k ; addiu at,rs,K ; sra` when rd != rs), and both
div and rem are preceded by one idle nop when rs's latest definition in the
same block is a load (as1 adds the load-delay nop itself when adjacent).
Sites: func_463eb8 L464138 (force the rd==rs `move at` shape), L4641b0
(addiu source $at), L464578 (rem: residue nop only).  The nop is emitted
first, inside the routine's own f_mark_unmovable range.
IDO52_AS1_DIV_POW2=aspsx[:N,..]        form + residue nop (0-based occurrences,
                                       div and rem share the counter)
IDO52_AS1_DIV_POW2=aspsx-nonop[:N,..]  form only
IDO52_AS1_DIV_POW2_TRACE=1             `[AS1-DP2] #N div|rem ... -> ASPSX|stock`
Default OFF (UOPT-HOOKS-REFERENCE Hook 62b).

HOOK 62c: IDO52_AS1_MUL_CONST (multiply by a constant, the $at-copy form)
---------------------------------------------------------------------------
ugen emits `x * K` as the macro `mul rd,rs,K` (cc -S); as1's f_genmul
expands it with f_smul's shift/add chain, which is ASPSX's chain exactly
(the same signed-digit choice: 3 = 4-1, 600 = ((5<<2)-1<<2)-1<<3).  When
rd == rs and |K| has >= 2 set bits, as1 (like ASPSX) first copies the
operand, `move at,rs`, and every add/subtract term reads $at.  The one
difference is what happens AFTERWARDS: as1's block optimisers treat the
macro's words as ordinary instructions -- pass II copy-propagation deletes
the `move at,rs` by renaming the producer (`lhu at,2(a0) ; sll v0,at,14`),
and pass I forward-propagation rewrites `sll rd,at,k` to `sll rd,rs,k`.
ASPSX never optimises a macro, so retail keeps all of it:
    lhu a1,2(a2) ; lhu a3,4(a2) ; move at,a1 ; sll a1,at,14 ; subu a1,a1,at
(0x8008D0AC).  Site: f_genmul's accept point (L462d24, the chain fitted the
instruction budget) -- for `mul` (op 0x38) whose first emitted word is
`addu at,rs,zero`, the record's +34 "optimisable" byte is cleared on the
copy and on every chain word that reads $at, the same byte f_gendiv clears
on its own check sequence.  Both passes then leave those records alone.
Nothing is emitted, moved or re-encoded; only two optimisations are
withheld, so the product is as1's own.  negu-form (K < 0) and rd != rs
chains are untouched (no retail evidence; rd != rs is already identical).
IDO52_AS1_MUL_CONST=aspsx[:N,..]   0-based occurrences over the $at-copy
                                   sites only (a bare list is an index list)
IDO52_AS1_MUL_CONST_TRACE=1        `[AS1-MULC] #N K=.. rd rs words=.. ->
                                   ASPSX|stock`, and `mul ... no $at copy`
Default OFF (UOPT-HOOKS-REFERENCE Hook 62c).
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
AS1_C = os.path.join(HERE, "build", "5.2", "as1.c")

MARKER = "as1_divm_pre_record"

HELPERS = r"""/* --- IDO52_AS1_DIV_MACRO (Hook 62, 2026-09-25; 62d 2026-09-26): ASPSX-order
 * div/rem macro expansion.  See apply_as1_divmacro_hooks.py.  Default OFF. --- */
#define DIVM_MAX 64
#define DIVM_MAXQ 8
#define DIVM_BIT(r) ((r) < 32 ? (1u << (r)) : 0u)
static int divm_inited, divm_mode, divm_trace, divm_seq;   /* mode 0 off 1 all 2 list */
static int divm_occ[DIVM_MAX], divm_nocc;
/* pending checks, FIFO (more than one only under Hook 62d QUEUE) */
static int divm_nchk, divm_qsg[DIVM_MAXQ], divm_qrs[DIVM_MAXQ], divm_qrt[DIVM_MAXQ], divm_qocc[DIVM_MAXQ];
static int divm_mf, divm_mfop, divm_rd, divm_mfocc;         /* pending mflo/mfhi */
static int divm_noreorder, divm_scan_from, divm_cur, divm_mffirst;
/* Hook 62d: 0 off, 1 all, 2 occurrence list (per rule, 0-based over the
 * records that reach the rule's decision with a check pending) */
static int divm_mp_mode, divm_mp_occ[DIVM_MAX], divm_mp_n, divm_mp_seq;   /* MUL_PASS */
static int divm_sc_mode, divm_sc_occ[DIVM_MAX], divm_sc_n, divm_sc_seq;   /* SPLIT_CMPBR */
static int divm_qu_mode, divm_qu_occ[DIVM_MAX], divm_qu_n, divm_qu_seq;   /* QUEUE */
static int divm_62d, divm_armed, divm_mul_live;
/* Hook 62e: IDO52_AS1_DIV_MACRO_MFFIRST_FLUSH -- 0 off, 1 all, 2 list of
 * Hook 62 divide occurrences (the `#N` of the deferred macro) */
static int divm_mfu_mode, divm_mfu_occ[DIVM_MAX], divm_mfu_n;
static int divm_mfu_hit(int occ) {
    int i;
    if (divm_mfu_mode == 1) return 1;
    for (i = 0; i < divm_mfu_n; i++) if (divm_mfu_occ[i] == occ) return 1;
    return 0;
}
static void divm_parse_62d(const char *name, int *mode, int *occ, int *n) {
    const char *e = getenv(name);
    char buf[512];
    char *tok;
    if (!e || !*e) return;
    if (strcmp(e, "all") == 0) { *mode = 1; divm_62d = 1; return; }
    if (strncmp(e, "occ:", 4) != 0) {
        fprintf(stderr, "as1: %s=%s: expected `all` or `occ:N[,N...]`; ignored\n", name, e);
        return;
    }
    *mode = 2; divm_62d = 1;
    strncpy(buf, e + 4, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    for (tok = strtok(buf, ","); tok; tok = strtok(NULL, ","))
        if (*n < DIVM_MAX) occ[(*n)++] = atoi(tok);
}
static int divm_62d_on(int mode, const int *occ, int n, int cur) {
    int i;
    if (mode == 1) return 1;
    for (i = 0; i < n; i++) if (occ[i] == cur) return 1;
    return 0;
}
static void divm_parse(void) {
    const char *e;
    char buf[512];
    char *tok;
    if (divm_inited) return;
    divm_inited = 1;
    e = getenv("IDO52_AS1_DIV_MACRO_TRACE");
    divm_trace = (e && *e && strcmp(e, "0") != 0);
    divm_parse_62d("IDO52_AS1_DIV_MACRO_MUL_PASS", &divm_mp_mode, divm_mp_occ, &divm_mp_n);
    divm_parse_62d("IDO52_AS1_DIV_MACRO_SPLIT_CMPBR", &divm_sc_mode, divm_sc_occ, &divm_sc_n);
    divm_parse_62d("IDO52_AS1_DIV_MACRO_QUEUE", &divm_qu_mode, divm_qu_occ, &divm_qu_n);
    e = getenv("IDO52_AS1_DIV_MACRO_MFFIRST_FLUSH");      /* Hook 62e */
    if (e && *e) {
        if (strcmp(e, "all") == 0) divm_mfu_mode = 1;
        else if (strncmp(e, "occ:", 4) == 0) {
            divm_mfu_mode = 2;
            strncpy(buf, e + 4, sizeof(buf) - 1);
            buf[sizeof(buf) - 1] = '\0';
            for (tok = strtok(buf, ","); tok; tok = strtok(NULL, ","))
                if (divm_mfu_n < DIVM_MAX) divm_mfu_occ[divm_mfu_n++] = atoi(tok);
        } else
            fprintf(stderr, "as1: IDO52_AS1_DIV_MACRO_MFFIRST_FLUSH=%s: expected `all` or "
                    "`occ:N[,N...]`; ignored\n", e);
    }
    e = getenv("IDO52_AS1_DIV_MACRO");
    if (!e || !*e) return;
    if (strncmp(e, "aspsx-mffirst", 13) == 0) { divm_mffirst = 1; e += 13; }
    else if (strncmp(e, "aspsx", 5) == 0) e += 5;
    else if (strcmp(e, "all") == 0) e += 3;
    if (*e == '\0') { divm_mode = 1; return; }
    if (*e == ':') e++;
    divm_mode = 2;
    strncpy(buf, e, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    for (tok = strtok(buf, ","); tok; tok = strtok(NULL, ","))
        if (divm_nocc < DIVM_MAX) divm_occ[divm_nocc++] = atoi(tok);
}
static uint32_t divm_rec(uint8_t *mem, uint32_t idx) {
    return MEM_U32(0x1002f2d0) + idx * 40;
}
static void divm_clear34(uint8_t *mem) {
    MEM_U8(divm_rec(mem, MEM_U32(0x1002f894)) + 34) = (uint8_t)0;
}
/* MIPS I decode of one emitted word.  *w = GPR written (0 = none),
 * rmask = GPRs read, return: 0 plain, 1 HI/LO op, 2 control transfer /
 * trap / unknown (never allowed while anything is pending). */
static int divm_decode(uint32_t x, int *w, uint32_t *rmask) {
    int op = x >> 26, rs = (x >> 21) & 31, rt = (x >> 16) & 31, rd = (x >> 11) & 31, fn = x & 63;
    *w = 0; *rmask = 0;
    if (op == 0) {
        switch (fn) {
        case 0: case 2: case 3: *w = rd; *rmask = 1u << rt; return 0;
        case 4: case 6: case 7: *w = rd; *rmask = (1u << rt) | (1u << rs); return 0;
        case 16: case 18: *w = rd; return 1;
        case 17: case 19: *rmask = 1u << rs; return 1;
        case 24: case 25: case 26: case 27: *rmask = (1u << rs) | (1u << rt); return 1;
        case 32: case 33: case 34: case 35: case 36: case 37: case 38: case 39:
        case 42: case 43:
            *w = rd; *rmask = (1u << rs) | (1u << rt); return 0;
        default: return 2;
        }
    }
    if (op >= 8 && op <= 14) { *w = rt; *rmask = 1u << rs; return 0; }
    if (op == 15) { *w = rt; return 0; }
    if (op >= 32 && op <= 38) { *w = rt; *rmask = 1u << rs; return 0; }
    if (op >= 40 && op <= 46 && op != 44 && op != 45) { *rmask = (1u << rs) | (1u << rt); return 0; }
    return 2;
}
static int divm_any_signed(void) {
    int i;
    for (i = 0; i < divm_nchk; i++) if (divm_qsg[i]) return 1;
    return 0;
}
static uint32_t divm_chk_regs(void) {   /* rs/rt of every pending check */
    uint32_t m = 0;
    int i;
    for (i = 0; i < divm_nchk; i++) m |= DIVM_BIT(divm_qrs[i]) | DIVM_BIT(divm_qrt[i]);
    return m;
}
/* Assert that nothing emitted since the deferral point breaks it. */
static void divm_scan(uint8_t *mem) {
    uint32_t i, last = MEM_U32(0x1002f894), cregs = divm_chk_regs();
    int sg = divm_any_signed();
    for (i = (uint32_t)divm_scan_from + 1; (int32_t)i <= (int32_t)last; i++) {
        uint32_t word = MEM_U32(divm_rec(mem, i)), rm;
        int wr, k = divm_decode(word, &wr, &rm), bad = 0;
        if (divm_nchk && (k == 2 || (wr && ((cregs >> wr) & 1u)))) bad = 1;
        /* 62d: a word that writes $at between a pending SIGNED check's
         * deferral point and its flush is exactly what the classifier must
         * never let through (the check itself uses $at). */
        if (divm_62d && sg && wr == 1) bad = 1;
        if (divm_mf && (k != 0 || (wr && wr == divm_rd) || (rm & (1u << divm_rd)))) bad = 1;
        if (bad) {
            fprintf(stderr, "as1: IDO52_AS1_DIV_MACRO: deferred div/rem #%d would be broken by "
                    "emitted word 0x%08x (record %u); refusing to miscompile\n",
                    divm_nchk ? divm_qocc[0] : divm_mfocc, (unsigned)word, (unsigned)i);
            exit(1);
        }
    }
    divm_scan_from = (int)last;
}
static void divm_emit_one_check(uint8_t *mem, uint32_t sp, int sg, int rs, int rt, int occ) {
    uint32_t sp2 = sp - 0x40, start;
    if (divm_trace) fprintf(stderr, "[AS1-DIVM]   #%d flush CHECK%s rs=%d rt=%d\n",
                            occ, sg ? "+overflow" : "", rs, rt);
    /* f_gendiv L464b40..L464d3c, register-divisor path, verbatim */
    MEM_U32(sp2 + 16) = 2;
    f_emitbene(mem, sp2, 0, 0x4c, rt, 0, 0);
    divm_clear34(mem);
    f_emitnop(mem, sp2, 0, 1);
    divm_clear34(mem);
    f_emitspec(mem, sp2, 0, 0xb, 7);
    divm_clear34(mem);
    if (sg) {
        start = MEM_U32(0x1002f894) + 1;
        f_loadimmed(mem, sp2, 0, 0xffffffff, 1);
        MEM_U32(sp2 + 16) = 4;
        f_emitbene(mem, sp2, 0, 0x4c, rt, 1, 0);
        f_loadimmed(mem, sp2, 0, 0x80000000, 1);
        MEM_U32(sp2 + 16) = 2;
        f_emitbene(mem, sp2, 0, 0x4c, rs, 1, 0);
        f_emitnop(mem, sp2, 0, 1);
        f_emitspec(mem, sp2, 0, 0xb, 6);
        if (MEM_U8(0x1002f898) == 0) f_macro_error(mem, sp2, 0);
        f_mark_unmovable(mem, sp2, start, MEM_U32(0x1002f894));
    }
}
/* flush every pending check, oldest first */
static void divm_emit_check(uint8_t *mem, uint32_t sp) {
    int i, n = divm_nchk;
    if (!n) return;
    divm_nchk = 0;
    for (i = 0; i < n; i++)
        divm_emit_one_check(mem, sp, divm_qsg[i], divm_qrs[i], divm_qrt[i], divm_qocc[i]);
    divm_scan_from = (int)MEM_U32(0x1002f894);
}
static void divm_emit_mf(uint8_t *mem, uint32_t sp) {
    uint32_t sp2 = sp - 0x40;
    if (!divm_mf) return;
    if (divm_nchk && ((divm_chk_regs() >> divm_rd) & 1u)) divm_emit_check(mem, sp);
    divm_mf = 0;
    if (divm_trace) fprintf(stderr, "[AS1-DIVM]   #%d flush %s rd=%d\n",
                            divm_mfocc, divm_mfop == 0xe ? "MFLO" : "MFHI", divm_rd);
    f_emitreg2(mem, sp2, 0, divm_mfop, divm_rd, 0x48);   /* f_gendiv L464d48 */
    divm_scan_from = (int)MEM_U32(0x1002f894);
}
static void divm_flush_all(uint8_t *mem, uint32_t sp) {
    divm_emit_check(mem, sp);
    divm_emit_mf(mem, sp);
}
static int divm_hit(int occ) {
    int i;
    if (divm_mode == 1) return 1;
    for (i = 0; i < divm_nocc; i++) if (divm_occ[i] == occ) return 1;
    return 0;
}
/* f_gendiv, right after `div $zero,rs,rt` (L464ac4).  1 = deferred. */
static int as1_divm_gendiv(uint8_t *mem, uint32_t sp) {
    int op, rd, rs, rt, imm, hit = 0, t, sg;
    divm_parse();
    if (divm_mode == 0 && !divm_trace) return 0;
    op = MEM_U16(sp + 130); rd = MEM_U8(sp + 135); rs = MEM_U8(sp + 139);
    rt = MEM_U8(sp + 125); imm = MEM_U8(sp + 147);
    if (rd == 0 || imm != 0) return 0;
    divm_cur = divm_seq++;
    hit = divm_hit(divm_cur);
    if (divm_noreorder || rs >= 32 || rt >= 32 || rd >= 32) hit = 0;
    /* a new divide overwrites HI/LO: nothing may still wait for the old one
     * (the pre-record classifier flushed it; stock expansion path otherwise) */
    if (divm_mf || divm_nchk >= DIVM_MAXQ) hit = 0;
    if (divm_trace)
        fprintf(stderr, "[AS1-DIVM] #%d op=%d rd=%d rs=%d rt=%d -> %s%s\n",
                divm_cur, op, rd, rs, rt, hit ? "DEFERRED" : "stock",
                (hit && divm_nchk) ? " (queued behind a pending check)" : "");
    if (!hit) return 0;
    divm_rd = rd;
    /* signedness: f_gendiv L464b0c (bitset 0x1000426c over op-32) */
    t = op - 32;
    sg = ((uint32_t)t < 0x40) &&
        ((int32_t)(MEM_U32(0x1000426c + ((t >> 5) << 2)) << (t & 0x1f)) < 0);
    /* mflo/mfhi selector: f_gendiv L464d48 */
    t = op - 32;
    divm_mfop = (((uint32_t)t < 0x20) && ((int32_t)(0xc0000000u << (t & 0x1f)) < 0)) ? 0xe : 0xc;
    divm_qsg[divm_nchk] = sg; divm_qrs[divm_nchk] = rs; divm_qrt[divm_nchk] = rt;
    divm_qocc[divm_nchk] = divm_cur; divm_nchk++;
    divm_mf = 1; divm_mfocc = divm_cur;
    divm_scan_from = (int)MEM_U32(0x1002f894);
    return 1;
}
static int divm_fits(int op, int32_t imm) {
    switch (op) {
    case 2: case 80: case 81: return imm >= -32768 && imm <= 32767;   /* addu slt sltu */
    case 86: return imm >= -32767 && imm <= 32768;                      /* subu */
    case 3: case 64: case 89: return imm >= 0 && imm <= 65535;          /* and or xor */
    case 79: case 83: case 84: return imm >= 0 && imm <= 31;            /* sll sra srl */
    }
    return 0;
}
static int divm_popcount(uint32_t x) {
    int n = 0;
    while (x) { n += x & 1u; x >>= 1; }
    return n;
}
/* Hook 62d rules, tried before the Hook 62 classifier.  1 = record handled
 * (it may pass the pending checks), 0 = fall through to the classifier. */
static int divm_62d_record(uint8_t *mem, uint32_t sp, int op, int fmt, int form,
                           int r1, int r2, int r3, int32_t imm) {
    uint32_t cregs = divm_chk_regs();
    int cur;
    if (!divm_62d || divm_nchk == 0) return 0;
    /* (1) MUL_PASS: `mul rd,rs,rt` (multu ; mflo) and `mul rd,rs,K` (f_genmul's
     * shift/add chain).  Retail: 0x800946F4..0x80094710 (the signed check of
     * the 60000000 div waits past `multu t0,v1 ; mflo t4` and both *60 / *10
     * chains) and 0x800947F0..0x800947FC (the divu check waits past the
     * in-place `move at,v0 ; sll ; subu ; sll` *60 chain). */
    if (op == 56 && fmt == 3 && (form == 3 || form == 4) && r1 > 1 && r1 < 32 && r2 > 1 && r2 < 32 &&
        (form == 4 || (r3 > 1 && r3 < 32))) {
        int uses_at = 0;
        uint32_t k;
        cur = divm_mp_seq++;
        if (!divm_62d_on(divm_mp_mode, divm_mp_occ, divm_mp_n, cur)) {
            if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62d MUL_PASS #%d not listed\n", cur);
            return 0;
        }
        if (form == 4) {
            /* f_genmul L462a6c..L462afc: `move at,rs` / `negu at,rs` copy iff
             * K < 0, or rd == rs with >= 2 set bits in |K|; 0 / 1 / -1 never.
             * The over-budget fallback (`li at,K ; multu ; mflo`) is caught
             * inside f_genmul (as1_divm_mul_fallback). */
            k = imm < 0 ? (uint32_t)(-imm) : (uint32_t)imm;
            if (imm != 0 && imm != 1 && imm != -1 &&
                (imm < 0 || (r1 == r2 && divm_popcount(k) >= 2))) uses_at = 1;
        }
        /* the multiply (or the fallback) overwrites HI/LO: a pending mflo/mfhi
         * goes first; so does it when the mul reads or writes its rd */
        if (divm_mf) {
            if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62d MUL_PASS #%d: mul touches HI/LO\n", cur);
            divm_emit_mf(mem, sp);
            cregs = divm_chk_regs();
        }
        if (divm_nchk && ((cregs >> r1) & 1u)) {
            if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62d MUL_PASS #%d: mul writes rs/rt -> flush\n", cur);
            divm_emit_check(mem, sp);
        } else if (divm_nchk && uses_at && divm_any_signed()) {
            if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62d MUL_PASS #%d: $at chain, signed check -> flush\n", cur);
            divm_emit_check(mem, sp);
        } else if (divm_nchk && divm_trace) {
            fprintf(stderr, "[AS1-DIVM]   62d MUL_PASS #%d: op=56 form=%d rd=%d K=%d%s -> PASS\n",
                    cur, form, r1, form == 4 ? (int)imm : 0, uses_at ? " ($at chain)" : "");
        }
        divm_mul_live = divm_nchk ? 1 : 0;
        return 1;
    }
    /* (2) QUEUE: a register-divisor div/divu/rem/remu that will itself be
     * deferred passes the pending checks (it writes no GPR; HI/LO only).
     * Retail 0x80094800: `divu zero,v1,v0` with the previous divu's check
     * (bnez t8) still pending; both checks come out oldest first. */
    if ((op == 32 || op == 33 || op == 65 || op == 66) && fmt == 3 && form == 3 &&
        r1 > 1 && r1 < 32 && r2 > 1 && r2 < 32 && r3 > 1 && r3 < 32 && divm_hit(divm_seq) &&
        !divm_noreorder && divm_nchk < DIVM_MAXQ) {
        cur = divm_qu_seq++;
        if (!divm_62d_on(divm_qu_mode, divm_qu_occ, divm_qu_n, cur)) {
            if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62d QUEUE #%d not listed\n", cur);
            return 0;
        }
        if (divm_mf) {
            if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62d QUEUE #%d: new divide touches HI/LO\n", cur);
            divm_emit_mf(mem, sp);
        }
        if (divm_nchk && divm_trace)
            fprintf(stderr, "[AS1-DIVM]   62d QUEUE #%d: op=%d -> PASS (%d check(s) stay pending)\n",
                    cur, op, divm_nchk);
        return 1;
    }
    /* (2b) QUEUE, unsigned power-of-two immediate: as1's func_463eb8 expands
     * `divu rd,rs,2^k` to one `srl rd,rs,k` and `remu rd,rs,2^k` to one
     * `andi rd,rs,2^k-1` (no HI/LO, no $at) -- an ordinary ALU word to ASPSX.
     * Retail 0x80094804 `srl t5,v0,1` (the `/ 2`) between the queued divu
     * and its check. */
    if ((op == 33 || op == 66) && fmt == 3 && form == 4 && r1 > 1 && r1 < 32 && r2 > 1 && r2 < 32 &&
        imm >= 2 && (imm & (imm - 1)) == 0 && (op == 33 || imm <= 0x10000)) {
        cur = divm_qu_seq++;
        if (!divm_62d_on(divm_qu_mode, divm_qu_occ, divm_qu_n, cur)) {
            if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62d QUEUE #%d not listed\n", cur);
            return 0;
        }
        if (divm_mf && (r1 == divm_rd || r2 == divm_rd)) {
            if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62d QUEUE #%d: pow2 %s touches rd\n",
                                    cur, op == 33 ? "divu" : "remu");
            divm_emit_mf(mem, sp);
        }
        if (divm_nchk && ((divm_chk_regs() >> r1) & 1u)) {
            if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62d QUEUE #%d: pow2 writes rs/rt -> flush\n", cur);
            divm_emit_check(mem, sp);
        } else if (divm_nchk && divm_trace) {
            fprintf(stderr, "[AS1-DIVM]   62d QUEUE #%d: %s rd=%d rs=%d,%d -> PASS (one %s)\n", cur,
                    op == 33 ? "divu" : "remu", r1, r2, (int)imm, op == 33 ? "srl" : "andi");
        }
        return 1;
    }
    /* (3) SPLIT_CMPBR: a register compare-branch macro (bge/bgeu/blt/bltu/...
     * -> `slt[u] at,rs,rt ; beqz|bnez at`, f_parseafrrl L44f390..L44f3e0).
     * Flush a pending mflo/mfhi the compare reads, let the `slt at` through,
     * and flush the checks between it and the branch (as1_divm_cmpbr_mid).
     * Retail 0x80094808..0x80094828: `mfhi t4 ; sltu at,t5,t4 ; bnez t8 ;
     * nop ; break 7 ; bnez v0 ; nop ; break 7 ; beqz at`.  A pending SIGNED
     * check uses $at, so it is flushed before the slt (0x80094714..0x80094738). */
    if (fmt == 6 && form == 8 && (op == 14 || op == 15 || op == 17 || op == 18 || op == 20 ||
                                  op == 21 || op == 23 || op == 24) &&
        r1 > 1 && r1 < 32 && r2 > 1 && r2 < 32) {
        cur = divm_sc_seq++;
        if (!divm_62d_on(divm_sc_mode, divm_sc_occ, divm_sc_n, cur)) {
            if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62d SPLIT_CMPBR #%d not listed\n", cur);
            return 0;
        }
        if (divm_any_signed() || ((cregs >> 1) & 1u)) {
            if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62d SPLIT_CMPBR #%d: signed check pending -> no split\n", cur);
            return 0;
        }
        if (divm_mf && (r1 == divm_rd || r2 == divm_rd)) {
            if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62d SPLIT_CMPBR #%d: compare reads rd\n", cur);
            divm_emit_mf(mem, sp);
        }
        if (divm_nchk == 0 && !divm_mf) return 1;
        if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62d SPLIT_CMPBR #%d: op=%d -> SPLIT (checks after the slt)\n", cur, op);
        divm_armed = 1;
        return 1;
    }
    return 0;
}
/* f_parsestmt, before each binasm record (L4563b0). */
static void as1_divm_pre_record(uint8_t *mem, uint32_t sp, uint32_t r) {
    int asmtype, op, fmt, r1, r2, r3, form, known = 0, at = 0, hilo = 0, mf_first = 0;
    uint32_t w2, reads = 0, writes = 0, cregs;
    int32_t imm;
    divm_parse();
    if (divm_mode == 0 && !divm_trace) return;
    divm_armed = 0;
    divm_mul_live = 0;
    asmtype = MEM_U8(r + 5) & 0x3f;
    if (asmtype == 27) divm_noreorder = 0;   /* .ent: new procedure */
    if (asmtype == 32) {                     /* .set */
        uint32_t opt = MEM_U32(r + 8);
        /* 11/12 = volatile/novolatile (f_parseset cases 10/11 -> 0x1002f8f8):
         * quiet, as in ASPSX.  Any other .set (reorder, at, macro, ...) is
         * never emitted by ugen for C; treat it as a flush point and stop
         * deferring for the rest of the procedure. */
        if (opt == 11 || opt == 12) return;
        if (divm_trace) fprintf(stderr, "[AS1-DIVM]   .set %u -> flush, deferral off\n", (unsigned)opt);
        divm_noreorder = 1;
    }
    if (!divm_nchk && !divm_mf) return;
    divm_scan(mem);
    if (asmtype == 28 || asmtype == 53) return;   /* .loc, .livereg: quiet */
    if (asmtype != 23) {
        if (divm_trace) fprintf(stderr, "[AS1-DIVM]   asmtype %d -> flush\n", asmtype);
        divm_flush_all(mem, sp);
        return;
    }
    op = (MEM_U16(r + 6) >> 1) & 0x1ff;
    fmt = MEM_U8(0x1002ffd0 + op);
    w2 = MEM_U32(r + 8);
    r1 = w2 >> 25; r2 = (w2 >> 18) & 0x7f; form = (w2 >> 14) & 0xf; r3 = (w2 >> 7) & 0x7f;
    imm = (int32_t)MEM_U32(r + 12);
    if (divm_62d_record(mem, sp, op, fmt, form, r1, r2, r3, imm)) return;
    if (fmt == 3 && (op == 2 || op == 3 || op == 64 || op == 89 || op == 80 || op == 81 ||
                     op == 79 || op == 83 || op == 84 || op == 86)) {
        /* form 3: rd,rs,rt   form 4: rd,rs,imm   form 2: rd,imm (rd op= imm) */
        if (form == 3) { known = r1 < 32 && r2 < 32 && r3 < 32; reads = DIVM_BIT(r2) | DIVM_BIT(r3); }
        else if (form == 4) { known = r1 < 32 && r2 < 32; reads = DIVM_BIT(r2); }
        else if (form == 2) { known = r1 < 32; reads = DIVM_BIT(r1); }
        if (form != 3 && !divm_fits(op, imm)) at = 1;
        writes = DIVM_BIT(r1);
    } else if (fmt == 2 && op == 41) {       /* li */
        known = r1 < 32; writes = DIVM_BIT(r1);
    } else if (fmt == 4 && (op == 49 || op == 90) && form == 5) {  /* move, not */
        known = r1 < 32 && r2 < 32; writes = DIVM_BIT(r1); reads = DIVM_BIT(r2);
    } else if (fmt == 1 && (op == 36 || op == 37 || op == 38 || op == 39 || op == 40 || op == 42 ||
                            op == 70 || op == 76 || op == 87)) {
        int store = (op == 70 || op == 76 || op == 87);
        known = r1 < 32 && (r2 == 72 || r2 < 32) && (form == 0 || form == 1);
        if (r2 != 72) reads |= DIVM_BIT(r2);
        if (store) reads |= DIVM_BIT(r1); else writes |= DIVM_BIT(r1);
        if (form == 1 && (store || r2 != 72)) at = 1;          /* lui at */
        if (form == 0 && (imm < -32768 || imm > 32767)) at = 1;
        if (form == 0 && r2 == 72) known = 0;
    } else if (fmt == 6 && form == 12 && r1 < 32) {
        /* compare-with-immediate branch macro: reads r1 before the block end */
        if (divm_mf && r1 == divm_rd) mf_first = 1;
    }
    if (reads & 2u || writes & 2u) at = 1;
    writes &= ~1u;
    if (!known) {
        if (divm_trace) fprintf(stderr, "[AS1-DIVM]   op=%d fmt=%d form=%d -> flush%s\n",
                                op, fmt, form, mf_first ? " (mflo first)" : "");
        if (mf_first) divm_emit_mf(mem, sp);
        else if (divm_mf && divm_nchk && divm_mfu_hit(divm_mfocc)) {
            /* Hook 62e: an unclassified record (a branch macro, the signed
             * `div rd,rs,2^k` expansion, ...) flushes both -> the mfhi/mflo
             * first; divm_emit_mf still puts the check first when rd is rs
             * or rt.  Retail 0x800912EC / 0x800916E8: `div zero,t2,v0 ; ... ;
             * mfhi t6 ; <signed check> ; bgez v0 ; ... ; sra t7,at,1`. */
            if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62e #%d: mfhi/mflo first\n", divm_mfocc);
            divm_emit_mf(mem, sp);
        }
        divm_flush_all(mem, sp);
        return;
    }
    cregs = divm_chk_regs();
    if (divm_mffirst && divm_mf && divm_nchk && (((reads | writes) >> divm_rd) & 1u || hilo) &&
        (at || (writes & cregs))) {
        /* aspsx-mffirst: one record flushes both -> mflo first (retail
         * 0x8008F148 `div ; mflo t2 ; <check> ; lui at ; sb t2,sym`);
         * divm_emit_mf still puts the check first when rd is rs or rt. */
        if (divm_trace) fprintf(stderr, "[AS1-DIVM]   op=%d flushes both (mflo first)\n", op);
        divm_emit_mf(mem, sp);
    }
    if (divm_nchk && (at || (writes & cregs))) {
        if (divm_trace) fprintf(stderr, "[AS1-DIVM]   op=%d %s\n", op, at ? "uses $at" : "writes rs/rt");
        divm_emit_check(mem, sp);
    }
    if (divm_mf && (((reads | writes) >> divm_rd) & 1u || hilo)) {
        if (divm_trace) fprintf(stderr, "[AS1-DIVM]   op=%d touches rd\n", op);
        divm_emit_mf(mem, sp);
    }
}
/* Hook 62d SPLIT_CMPBR, f_parseafrrl L44f3ac: the compare macro's `slt at`
 * has been emitted, its branch has not.  Flush the checks here, then a
 * pending mflo/mfhi the compare did not read. */
void as1_divm_cmpbr_mid(uint8_t *mem, uint32_t sp) {
    if (!divm_armed) return;
    divm_armed = 0;
    divm_scan(mem);
    if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62d SPLIT_CMPBR: slt emitted -> flush before the branch\n");
    divm_flush_all(mem, sp);
}
/* Hook 62d MUL_PASS, f_genmul L462dd0: the chain did not fit the budget and
 * as1 falls back to `li at,K ; multu ; mflo`.  A pending SIGNED check must
 * be emitted before the `li at`. */
void as1_divm_mul_fallback(uint8_t *mem, uint32_t sp) {
    if (!divm_mul_live || !divm_nchk || !divm_any_signed()) return;
    divm_scan(mem);
    if (divm_trace) fprintf(stderr, "[AS1-DIVM]   62d MUL_PASS: over-budget fallback uses $at -> flush\n");
    divm_emit_check(mem, sp);
}
/* f_parsestmt exit (L457fc4): never let a deferral cross a chunk. */
static void as1_divm_at_return(uint8_t *mem, uint32_t sp) {
    divm_armed = 0;
    divm_mul_live = 0;
    if (!divm_nchk && !divm_mf) return;
    divm_scan(mem);
    if (divm_trace) fprintf(stderr, "[AS1-DIVM]   parsestmt return -> flush\n");
    divm_flush_all(mem, sp);
}

"""

HELPERS_62B = r"""/* --- IDO52_AS1_DIV_POW2 (Hook 62b, 2026-09-25): ASPSX's signed
 * div/rem by an immediate power of two.  See apply_as1_divmacro_hooks.py.
 * Default OFF. --- */
static int dp2_inited, dp2_mode, dp2_trace, dp2_seq, dp2_nonop, dp2_at_src;
static int dp2_occ[64], dp2_nocc;
static void dp2_parse(void) {
    const char *e;
    char buf[512];
    char *tok;
    if (dp2_inited) return;
    dp2_inited = 1;
    e = getenv("IDO52_AS1_DIV_POW2_TRACE");
    dp2_trace = (e && *e && strcmp(e, "0") != 0);
    e = getenv("IDO52_AS1_DIV_POW2");
    if (!e || !*e) return;
    if (strncmp(e, "aspsx-nonop", 11) == 0) { dp2_nonop = 1; e += 11; }
    else if (strncmp(e, "aspsx", 5) == 0) e += 5;
    else if (strcmp(e, "all") == 0) e += 3;
    if (*e == '\0') { dp2_mode = 1; return; }
    if (*e == ':') e++;
    dp2_mode = 2;
    strncpy(buf, e, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    for (tok = strtok(buf, ","); tok; tok = strtok(NULL, ","))
        if (dp2_nocc < 64) dp2_occ[dp2_nocc++] = atoi(tok);
}
/* Common entry for an eligible site (func_463eb8's sp).  Returns 1 if the
 * hook is on for this occurrence; emits the residue nop when the last
 * record already emitted is a load whose destination is rs. */
static int as1_dp2_site(uint8_t *mem, uint32_t sp, int is_rem) {
    int rd, rs, hit = 0, i, cur, nop = 0;
    int32_t imm;
    uint32_t last, word;
    dp2_parse();
    if (dp2_mode == 0 && !dp2_trace) return 0;
    rd = MEM_U8(sp + 75); rs = MEM_U8(sp + 79); imm = (int32_t)MEM_U32(sp + 80);
    if (imm <= 1) return 0;
    cur = dp2_seq++;
    if (dp2_mode == 1) hit = 1;
    for (i = 0; i < dp2_nocc; i++) if (dp2_occ[i] == cur) hit = 1;
    if (rs >= 32 || rd >= 32 || rs == 1 || rd == 1) hit = 0;
    last = MEM_U32(0x1002f894);
    /* Residue nop: rs's most recent definition among the records already
     * emitted for this block (the record array holds one block/chunk; it is
     * reset when as1 schedules it) is a LOAD.  Retail: 0x8008D5AC (lb a1 four
     * words up), 0x800A149C, and the 2-nop `lw rs ; nop ; nop ; bgez` shape
     * (as1 adds the load-delay nop itself).  Any control transfer ends the
     * scan. */
    if (hit && !dp2_nonop && last + 1 == MEM_U32(sp + 64)) {
        int32_t j;
        for (j = (int32_t)last; j >= 0; j--) {
            int op, wr = 0, fn;
            word = MEM_U32(MEM_U32(0x1002f2d0) + (uint32_t)j * 40);
            op = word >> 26; fn = word & 63;
            if (op == 2 || op == 3 || (op >= 4 && op <= 7) || op == 1 ||
                (op == 0 && (fn == 8 || fn == 9 || fn == 12 || fn == 13))) break;
            if (op == 0) wr = (word >> 11) & 31;
            else if ((op >= 8 && op <= 15) || (op >= 32 && op <= 38)) wr = (word >> 16) & 31;
            if (wr != rs) continue;
            if (op >= 32 && op <= 38 && op != 34 && op != 38) nop = 1;
            break;
        }
        if (dp2_trace) fprintf(stderr, "[AS1-DP2]   records 0..%d scanned\n", (int)last);
    }
    if (dp2_trace)
        fprintf(stderr, "[AS1-DP2] #%d %s rd=%d rs=%d imm=%d -> %s%s\n", cur,
                is_rem ? "rem" : "div", rd, rs, (int)imm,
                hit ? "ASPSX" : "stock", nop ? " +residue-nop" : "");
    if (!hit) return 0;
    if (nop) f_emitnop(mem, sp, 0, 1);
    return 1;
}
static int as1_dp2_div(uint8_t *mem, uint32_t sp) {
    dp2_at_src = as1_dp2_site(mem, sp, 0);
    return dp2_at_src;
}

"""

SITES = [
    # (anchor, replacement, tag)
    ("static void f_parsestmt(uint8_t *mem, uint32_t sp) {\n",
     HELPERS + "static void f_parsestmt(uint8_t *mem, uint32_t sp) {\n", "helpers"),
    ("L4563b0:\nv1 = 0x1002f8d0;\n",
     "L4563b0:\n/* Hook 62: IDO52_AS1_DIV_MACRO */\nas1_divm_pre_record(mem, sp, s0);\n"
     "v1 = 0x1002f8d0;\n", "pre-record"),
    ("L457fc4:\n// bdead 1 s0 = MEM_U32(sp + 176);\n",
     "L457fc4:\n/* Hook 62: IDO52_AS1_DIV_MACRO */\nas1_divm_at_return(mem, sp);\n"
     "// bdead 1 s0 = MEM_U32(sp + 176);\n", "return"),
    ("L464ac4:\nt0 = MEM_U8(sp + 135);\n",
     "L464ac4:\n/* Hook 62: IDO52_AS1_DIV_MACRO */\nif (as1_divm_gendiv(mem, sp)) goto L464da0;\n"
     "t0 = MEM_U8(sp + 135);\n", "gendiv"),
]

MARKER_62B = "as1_dp2_site"
SITES_62B = [
    ("static uint32_t func_463eb8(uint8_t *mem, uint32_t sp, uint32_t v0, uint32_t a0, "
     "uint32_t a1, uint32_t a2, uint32_t a3) {\n",
     HELPERS_62B + "static uint32_t func_463eb8(uint8_t *mem, uint32_t sp, uint32_t v0, "
     "uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3) {\n", "62b helpers"),
    # signed div, |imm| < 0x10000: force the rd==rs shape (`move at,rs` in the
    # bgez slot, branch over the addiu) ...
    ("L464138:\nif ((int32_t)at == (int32_t)0) {\na2 = zero;\ngoto L4641d4;}",
     "L464138:\n/* Hook 62b: IDO52_AS1_DIV_POW2 */\n"
     "if ((int32_t)at != 0 && as1_dp2_div(mem, sp)) v0 = 0;\n"
     "if ((int32_t)at == (int32_t)0) {\na2 = zero;\ngoto L4641d4;}", "62b div"),
    # ... and take the addiu's source from $at instead of rs.
    ("L4641b0:\n//nop;\na2 = MEM_U8(sp + 79);\n",
     "L4641b0:\n//nop;\na2 = MEM_U8(sp + 79);\n/* Hook 62b */\n"
     "if (dp2_at_src) { a2 = 1; dp2_at_src = 0; }\n", "62b addiu"),
    # signed rem, |imm| < 0x8000: as1's own form is ASPSX's; only the residue nop.
    ("L464578:\n//nop;\na1 = MEM_U8(sp + 79);\n",
     "L464578:\n/* Hook 62b: IDO52_AS1_DIV_POW2 */\nas1_dp2_site(mem, sp, 1);\n"
     "//nop;\na1 = MEM_U8(sp + 79);\n", "62b rem"),
]

HELPERS_62C = r"""/* --- IDO52_AS1_MUL_CONST (Hook 62c, 2026-09-25): keep f_genmul's
 * `move at,rs` copy form out of as1's block optimisers, as ASPSX does.
 * See apply_as1_divmacro_hooks.py.  Default OFF. --- */
static int mulc_inited, mulc_mode, mulc_trace, mulc_seq;
static int mulc_occ[64], mulc_nocc;
static void mulc_parse(void) {
    const char *e;
    char buf[512];
    char *tok;
    if (mulc_inited) return;
    mulc_inited = 1;
    e = getenv("IDO52_AS1_MUL_CONST_TRACE");
    mulc_trace = (e && *e && strcmp(e, "0") != 0);
    e = getenv("IDO52_AS1_MUL_CONST");
    if (!e || !*e) return;
    if (strncmp(e, "aspsx", 5) == 0) e += 5;
    else if (strcmp(e, "all") == 0) e += 3;
    if (*e == '\0') { mulc_mode = 1; return; }
    if (*e == ':') e++;
    mulc_mode = 2;
    strncpy(buf, e, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    for (tok = strtok(buf, ","); tok; tok = strtok(NULL, ","))
        if (mulc_nocc < 64) mulc_occ[mulc_nocc++] = atoi(tok);
}
/* f_genmul accept point (L462d24): records first+1..last are the macro. */
static void as1_mulc_accept(uint8_t *mem, uint32_t sp, uint32_t first, uint32_t last) {
    uint32_t base, i, w;
    int op, rd, rs, hit = 0, cur, n = 0;
    int32_t k;
    mulc_parse();
    if (mulc_mode == 0 && !mulc_trace) return;
    op = MEM_U16(sp + 274); rd = MEM_U8(sp + 279); rs = MEM_U8(sp + 283);
    k = (int32_t)MEM_U32(sp + 292);
    if (op != 0x38 || (int32_t)last <= (int32_t)first) return;
    base = MEM_U32(0x1002f2d0);
    w = MEM_U32(base + (first + 1) * 40);
    if ((w & 0xfc1fffffu) != 0x00000821u) {
        if (mulc_trace)
            fprintf(stderr, "[AS1-MULC] mul K=%d rd=%d rs=%d: no $at copy -> stock\n",
                    (int)k, rd, rs);
        return;
    }
    cur = mulc_seq++;
    if (mulc_mode == 1) hit = 1;
    for (i = 0; i < (uint32_t)mulc_nocc; i++) if (mulc_occ[i] == cur) hit = 1;
    if (hit) {
        for (i = first + 1; (int32_t)i <= (int32_t)last; i++) {
            w = MEM_U32(base + i * 40);
            /* the copy itself, and every R-type chain word reading $at */
            if (i == first + 1 ||
                ((w >> 26) == 0 && (((w >> 21) & 31) == 1 || ((w >> 16) & 31) == 1))) {
                MEM_U8(base + i * 40 + 34) = (uint8_t)0;
                n++;
            }
        }
    }
    if (mulc_trace)
        fprintf(stderr, "[AS1-MULC] #%d mul K=%d rd=%d rs=%d words=%d -> %s (%d kept)\n",
                cur, (int)k, rd, rs, (int)(last - first), hit ? "ASPSX" : "stock", n);
}

"""

MARKER_62C = "as1_mulc_accept"
SITES_62C = [
    ("static uint32_t f_genmul(uint8_t *mem, uint32_t sp, uint32_t v0, uint32_t a0, "
     "uint32_t a1, uint32_t a2, uint32_t a3) {\n",
     HELPERS_62C + "static uint32_t f_genmul(uint8_t *mem, uint32_t sp, uint32_t v0, "
     "uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3) {\n", "62c helpers"),
    # the chain fitted the budget: keep the $at-copy form's records out of
    # pass I / pass II (as f_gendiv does for its check sequence).
    ("v1 = t7 - t4;\nat = (int)v1 < (int)t2;\nif ((int32_t)at != (int32_t)0) {\n"
     "MEM_U32(sp + 216) = t2;\ngoto L4631dc;}",
     "v1 = t7 - t4;\nat = (int)v1 < (int)t2;\nif ((int32_t)at != (int32_t)0) {\n"
     "MEM_U32(sp + 216) = t2;\n/* Hook 62c: IDO52_AS1_MUL_CONST */\n"
     "as1_mulc_accept(mem, sp, t4, t7);\ngoto L4631dc;}", "62c accept"),
]

MARKER_62D = "Hook 62d: IDO52_AS1_DIV_MACRO_SPLIT_CMPBR site"
MARKER_62D_HELPERS = "divm_62d_record"
MARKER_62E_HELPERS = "divm_mfu_hit"
HELPERS_START = "/* --- IDO52_AS1_DIV_MACRO (Hook 62, 2026-09-25"
PARSESTMT_DEF = "static void f_parsestmt(uint8_t *mem, uint32_t sp) {\n"
SITES_62D = [
    # the helper called from f_parseafrrl (defined BEFORE the helper block)
    # needs a file-scope prototype ahead of it.
    ("static uint32_t f_parseafrrl(uint8_t *mem, uint32_t sp, uint32_t v0, uint32_t a0, "
     "uint32_t a1) {\n",
     "void as1_divm_cmpbr_mid(uint8_t *mem, uint32_t sp);   /* Hook 62d */\n"
     "static uint32_t f_parseafrrl(uint8_t *mem, uint32_t sp, uint32_t v0, uint32_t a0, "
     "uint32_t a1) {\n", "62d prototype"),
    # f_parseafrrl: the compare macro's slt[i][u] at,... has just been emitted
    # (the register path L44f390 and the immediate path L44f33c join here),
    # the beqz/bnez at has not.
    ("L44f3ac:\nt8 = 0x1002f898;\n",
     "L44f3ac:\n/* " + MARKER_62D + " */\nas1_divm_cmpbr_mid(mem, sp);\n"
     "t8 = 0x1002f898;\n", "62d cmpbr mid"),
    # f_genmul: an over-budget constant multiply falls back to
    # `li at,K ; multu ; mflo` -- a pending signed check goes before the li.
    ("L462dd0:\n//nop;\na0 = MEM_U32(sp + 292);\n",
     "L462dd0:\n/* Hook 62d: IDO52_AS1_DIV_MACRO_MUL_PASS fallback */\n"
     "as1_divm_mul_fallback(mem, sp);\n//nop;\na0 = MEM_U32(sp + 292);\n", "62d mul fallback"),
]


def _upgrade_helpers(s):
    """An as1.c patched by the pre-62d applier carries the old helper block
    (inserted right before f_parsestmt); replace it with the current one."""
    if MARKER not in s or (MARKER_62D_HELPERS in s and MARKER_62E_HELPERS in s):
        return s, True
    if s.count(HELPERS_START) != 1 or s.count(PARSESTMT_DEF) != 1:
        print("[divmacro hooks] ERROR: cannot locate the Hook 62 helper block to upgrade")
        return s, False
    a = s.index(HELPERS_START)
    b = s.index(PARSESTMT_DEF)
    if not a < b:
        print("[divmacro hooks] ERROR: Hook 62 helper block is not before f_parsestmt")
        return s, False
    print("[divmacro hooks] Hook 62 helpers upgraded (62d/62e)")
    return s[:a] + HELPERS + s[b:], True


# f_emitbene etc. are defined after f_parsestmt; the helpers need prototypes,
# which the recomp already emits at the top of the file -- verified here.
PROTOS = ["static uint32_t f_emitbene(", "static uint32_t f_emitnop(",
          "static uint32_t f_emitspec(", "static uint32_t f_loadimmed(",
          "static uint32_t f_emitreg2(", "static uint32_t f_macro_error(",
          "static uint32_t f_mark_unmovable("]


def rebuild_cmds():
    return [
        ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
         "-o", "build/5.2/as1.o", "build/5.2/as1.c"],
        ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
         "-o", "build/5.2/out/as1.new", "build/5.2/as1.o", "build/5.2/version_info.o",
         "build/5.2/libc_impl_52.o", "-lm"],
    ]


def _apply_sites(s, marker, sites, name):
    if marker in s:
        print(f"[divmacro hooks] {name} already applied")
        return s, True
    for anchor, _, tag in sites:
        if s.count(anchor) != 1:
            print(f"[divmacro hooks] ERROR: {tag} anchor not unique/found "
                  f"(count {s.count(anchor)}): {anchor[:40]!r}")
            return s, False
    for anchor, repl, _ in sites:
        s = s.replace(anchor, repl, 1)
    print(f"[divmacro hooks] {name} applied")
    return s, True


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER not in s:
        for p in PROTOS:
            if p not in s.split("static void f_parsestmt(uint8_t *mem, uint32_t sp) {\n")[0]:
                print(f"[divmacro hooks] ERROR: no prototype before f_parsestmt: {p}")
                return False
    s, ok = _upgrade_helpers(s)
    if not ok:
        return False
    s, ok = _apply_sites(s, MARKER, SITES, "Hook 62")
    if not ok:
        return False
    s, ok = _apply_sites(s, MARKER_62D, SITES_62D, "Hook 62d")
    if not ok:
        return False
    s, ok = _apply_sites(s, MARKER_62B, SITES_62B, "Hook 62b")
    if not ok:
        return False
    s, ok = _apply_sites(s, MARKER_62C, SITES_62C, "Hook 62c")
    if not ok:
        return False
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[divmacro hooks] {path} up to date")
    return True


def main():
    argv = sys.argv[1:]
    path = AS1_C
    if "--as1" in argv:
        path = argv[argv.index("--as1") + 1]
    if not apply(path):
        sys.exit(1)
    if "--rebuild" in argv:
        for cmd in rebuild_cmds():
            r = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
            if r.returncode != 0:
                print(r.stderr[-2000:])
                sys.exit(1)
        os.replace(os.path.join(HERE, "build/5.2/out/as1.new"),
                   os.path.join(HERE, "build/5.2/out/as1"))
        print("[divmacro hooks] as1 rebuilt")


if __name__ == "__main__":
    main()
