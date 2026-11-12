#!/usr/bin/env python3
"""Reaching-definition ISOMORPHISM audit for `patch_insn_reg_field` paint layers.

Compiles the function through the REAL PostProc chain:

  A = the chain with the whole register-RENAME layer REMOVED
      (`patch_insn_reg_field`, `swap_registers`, `swap_registers_liveness`,
      `rename_register_chain`; every other step kept, so offsets still line
      up after count-changing allowed steps such as `cse_lui_at`)
  B = the full chain

and checks that every instruction operand read keeps the same *producing
instruction*.  A paint that changes a producer means the rewritten object no
longer computes the compiler's values — the masked compare and the
name-derived resolver both still score it, and `semcheck` does not see it.

When A and B differ and the chain also carries a REORDER layer
(`swap_instructions`, `scheduler_normalizer`, `hoist_out_of_delay_slot`),
the verdict is taken instead against R = the RAW compile (no PostProc):
every R word is paired with the B word it became (through A when A is a
pure permutation of R, else register-blind by shape), ambiguous pairings are
settled by the dataflow itself, and each read of R must have, in B, the
producers the pairing maps R's to -- the rename and the reorder judged
together (mode "unpermuted").

Source files are never mutated.

    python3 tools/overlay/audit_paint_isomorphism.py DATE func_80134CF4

Output: one line per instruction whose operand producers changed, with the
producer sets.  An `int` is the producing instruction index; `('E', r)` is
the incoming value of register r (bare `'E'` for callee-saved registers,
whose entry value is only saved and restored); `('call', n, r)` is the
value call n returns in $v0/$v1 and `('call', n)` a register call n
clobbered.  Argument registers are compared at each call, up to the
callee's arity as the C declares or calls it.

2026-09-28 MODELLING FIXES (each an FP or a blind spot measured on the 93
flags that no honesty worklist explained, and on corrupted-paint controls):
  * lwl/lwr are a paired full-word load, not a read of the old rt;
  * `j`/`jr` do not fall through (the epilogue's delay slot fed the next
    block); gcc `j` targets are followed;
  * jump-table case bodies inherit the dispatch state only when the object's
    own table (R_MIPS_32 .text relocations in its data) names them; dead
    words and a TU's second function no longer do (each STT_FUNC is a root);
  * a branch whose delay slot is itself a branch target still executes that
    slot on its own path;
  * entry values are register-tagged and call results are $v0/$v1-tagged
    (an untagged 'E' made "reads $a1" equal "reads $a0": 3/60 controls
    missed);
  * `addiu rX,rY,0` is not a copy (it is `%lo(sym)` unlinked); relocated
    words are value-equal only when their relocations agree (%hi by value).
Negative controls, 2026-09-28: 198/200 random corrupted paints in clean
functions caught (old tool 195; the 2 misses re-point between two
UNRELOCATED `lui $x,0x8014` -- the same constant), 69/69 corrupted paints
inside jump-table case bodies (old 68).

=============================================================================
FALSE-POSITIVE MODES  (2026-08-08/09 audit; ~15% raw FP rate measured against
40 byte-perfect controls, 6 of which flag)
=============================================================================
The RAW iso signal is **ADVISORY, NOT AUTO-STRIPPABLE**.  Only the "provable
classes" listed further down may be acted on without a human reading the
target `.s`.  Everything else needs adjudication.

1. UPSTREAM REPAIR  (the commonest; found 2026-08-09).
   `scheduler_normalizer` / `swap_instructions` frequently BREAK def-use in
   the UNPAINTED reference A — a `--const-fp` run hoists a `li` on top of a
   live load, a whole-function run swaps two `lui %hi` words into each
   other's slots, or it sinks a `li` across a branch target.  The paint layer
   then RESTORES the raw compiler's value flow.  iso reports a producer
   change, but the paint is the CURE, not the disease.
   Signature: A's producer set is entirely `'E'` / `('call', n)` and B's is a
   real definition.  `minimize_paint_strip.py` filters this direction out
   (see `nflags()` there); always cross-check against the raw (all-PostProc-
   stripped) compile before believing a flag.

2. ENTRY-SPILL.  The flagged read is a callee-save register being spilled in
   the prologue or reloaded in the epilogue; the "producer" is the entry
   value on both sides and the difference is cosmetic.

3. VALUE-EQUIVALENT.  The two producers compute the same value — a copy, a
   rematerialised constant/address, the same load from the same symbol.  The
   `_veq`/`_setveq` pair below already absorbs most of this class (without
   it, every legitimate "target parks the value in a different register"
   paint reads as a producer change even on byte-perfect objects), but the
   depth-5 recursion limit means deep webs still leak through.

4. DEAD DEF.  The flagged instruction's destination is redefined before any
   read, so the producer change is unobservable.

5. SPURIOUS INSTRUCTION.  The flag sits on a word the TARGET replaces with a
   nop or a different opcode, so NEITHER object is right.  That is a C bug,
   not a paint bug — do not strip, fix the C.

1b. REORDER REPAIR (handled since 2026-09-28, see "unpermuted" above).  A
   swap/normalizer step applied to the compiled registers breaks a def-use
   pair that only the renamed registers make independent; the paints repair
   it.  Also: a one-way `swap_registers` collapse split again by the paints
   (now part of the removed rename layer).  When the reorder layer cannot be
   removed cleanly the CLI says so ("reorder layer not removed"): treat
   those flags as mode 1 until read.

1c. SAME-%hi LUI RE-PAIRING (reported as a NOTE, not a flag).  Two luis
   whose symbols share one %hi are the same word after linking; a
   reorder + paint pair that makes each consumer read the other one changes
   no linked value, but the object's HI16/LO16 pairing no longer matches the
   C.  gen_reg_patches refuses this for new paints; the note counts them.

6. UNREACHABLE-BLOCK (`'?'`).  A paint on a branch operand can flip
   `beq $0,$0` into a conditional and change reachability; the CFG walk then
   has no state for the block.  Those reads are SKIPPED, not reported — the
   audit makes no claim there.

=============================================================================
PROVABLE CLASSES (safe to refuse / strip automatically)
=============================================================================
These do not need the reaching-def graph at all and are checked by
`audit_paint_bans.py` (and enforced at emit time by `gen_reg_patches.py`):

  * paint onto a compiled `nop` — a nop IS `sll $zero,$zero,0`, so painting a
    register field into it FABRICATES an instruction (banned by CLAUDE.md)
  * `rt`-field paint on REGIMM (op 1) or on blez/bgtz (op 6/7) — that field
    is the branch CONDITION, not a register
  * zero <-> register paint on a branch word — flips branch semantics
  * paint onto `$at` — `$at` is the assembler temp, created by macro
    expansion and never by this C; a read repointed at `$at` is garbage

CORRECTED BRANCH RULE (2026-08-09): the rt-field ban applies ONLY to
REGIMM and blez/bgtz.  For **beq/bne (op 4/5) `rt` is an ordinary register
operand and painting it is legal.**  The earlier over-broad "no rt paint on
any branch" screen produced false bans.

=============================================================================
WHAT THIS AUDIT CANNOT SEE
=============================================================================
  * WRONG-SYMBOL paints.  A paint can re-point a `%lo` consumer at a
    different `lui %hi` of the same shape, so a store lands on the WRONG
    GLOBAL.  The producer graph is IDENTICAL (same instruction indices, same
    shapes) — only the relocation symbol differs.  Use the reloc-pairing
    check in `gen_reg_patches.py` (`--audit-relocs`) for that class.
  * (fixed 2026-09-28) `swap_registers` / `rename_register_chain` layers
    are now removed from A with the paints, so a one-way collapse is judged.
  * memory dataflow (which stack slot / global a value goes through) and
    values equal only by a program invariant (two variables that always
    hold the same number): both read as producer changes, or not at all.
  * instruction-count defects.  Use `audit_raw_count.py`.
"""
import argparse
import os
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "build"))
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

from parse_postproc import (parse_compile_flags, parse_compile_env,
                            parse_compiler, parse_symbol_start_text,
                            parse_symbol_size_text)
from overlay_postproc import parse_postproc, apply_postproc
from ido_flags import base_flags, pic_flags

SRC = ROOT / "src" / "overlay"
CFG = ROOT / "config" / "overlay"
INC = ROOT / "include"
IDO_CC = ROOT / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
GCCWRAP = ROOT / "tools" / "gcc" / "gccwrap.py"
IDO_FLAGS = base_flags([INC, f"{INC}/psyq"])
GCCWRAP_FLAGS = {"--fill-jalr", "--fill-epilogue", "--fill-all",
                 "--no-delay-slots", "--no-cross-jumping"}

RA = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2', 't3',
      't4', 't5', 't6', 't7', 's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
      't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra']
CLOB = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25, 31}
CALLEE_SAVED = {16, 17, 18, 19, 20, 21, 22, 23, 28, 30, 31}
ARGS = (4, 5, 6, 7)


def _passed(s, r):
    """True when argument register r holds a value the code PUT there at the
    call (definitions, or the caller's own incoming r passed through) --
    not garbage from an earlier call or an unset temp."""
    return bool(s) and all(isinstance(x, int) or x == ('E', r) for x in s)


def _strip_comments(text):
    import re
    return re.sub(r'/\*.*?\*/', ' ', text, flags=re.S)


def _count_args(s):
    """Top-level comma count of an argument / parameter list body."""
    s = s.strip()
    if not s:
        return 0
    depth, n = 0, 1
    for ch in s:
        if ch in '([{':
            depth += 1
        elif ch in ')]}':
            depth -= 1
        elif ch == ',' and depth == 0:
            n += 1
    return n


def callee_arity(text, name):
    """How many register arguments the C passes to `name`: its prototype's
    parameter count ((void) = 0), else the first call site's argument count;
    None when variadic or unknown (a function pointer, no mention)."""
    import re
    src = _strip_comments(text)
    for m in re.finditer(r'\b' + re.escape(name) + r'\s*\(', src):
        # balanced parenthesis body
        k, depth = m.end(), 1
        while k < len(src) and depth:
            if src[k] == '(':
                depth += 1
            elif src[k] == ')':
                depth -= 1
            k += 1
        body = src[m.end():k - 1]
        before = src[max(0, m.start() - 80):m.start()]
        is_decl = bool(re.search(r'(?:^|[;{}\n])\s*(?:extern\s+|static\s+)?'
                                 r'[A-Za-z_][\w\s\*]*$', before)) and \
            not re.search(r'\b(return|if|while|switch)\s*$', before)
        if '...' in body:
            return None
        if is_decl:
            b = body.strip()
            if b == 'void':
                return 0
            if b == '':
                continue            # K&R `f();` says nothing: use a call site
            return _count_args(b)
    for m in re.finditer(r'\b' + re.escape(name) + r'\s*\(', src):
        before = src[max(0, m.start() - 80):m.start()]
        # 2026-10-03: `return f(x);` is a CALL, not a declaration -- the
        # declaration regex below matches "return " as a type, so every
        # tail call's arity came out None and its arguments went unchecked
        if re.search(r'(?:^|[;{}\n])\s*(?:extern\s+|static\s+)?'
                     r'[A-Za-z_][\w\s\*]*$', before) and \
                not re.search(r'\b(return|if|while|switch|else|do|case)\s*$', before):
            continue
        k, depth = m.end(), 1
        while k < len(src) and depth:
            if src[k] == '(':
                depth += 1
            elif src[k] == ')':
                depth -= 1
            k += 1
        return _count_args(src[m.end():k - 1])
    return None


def call_arities(text, rel):
    """{word index of a call: callee_arity} from an object's relocations."""
    out, cache = {}, {}
    for i, (typ, name) in rel.items():
        if typ != 4 or not name:          # R_MIPS_26
            continue
        if name not in cache:
            cache[name] = callee_arity(text, name)
        out[i] = cache[name]
    return out


def _arg_reads(W, rec, arity):
    """[(call index, r, producers)] for the argument registers each call
    receives (read after its delay slot).  2026-09-28: `jal` had no uses, so a
    paint that moved an argument set-up out of $aN (`addiu $a0,$zero,0x8b` ->
    `$s2` in the delay slot) left the callee reading garbage and the audit
    silent (negative control BUNKAKEN/func_8013CF38)."""
    out = []
    if not arity:
        return out
    for i, w in enumerate(W):
        _, _, _, _, call = decode(w)
        if not call or arity.get(i) is None:
            continue
        for r in ARGS[:arity[i]]:
            s = rec.get((i, ('arg', r)))
            if s is not None:
                out.append((i, r, s))
    return out

# The register-RENAME layer: every step that rewrites only register fields.
# A is built with ALL of them removed, not just `patch_insn_reg_field`.  A
# one-way `swap_registers t8:t9` COLLAPSES two webs onto one register and the
# paint list that follows it splits them again; auditing the paints against
# an A that still carries the collapse reports the repair as a producer change
# (35 of 93 flags on 2026-09-28 were this) and hides a collapse the paints do
# NOT repair.  Removing the whole layer checks the rename as one directive.
RENAME_STEPS = {'patch_insn_reg_field', 'swap_registers',
                'swap_registers_liveness', 'rename_register_chain'}

# Pure PERMUTATION steps.  A reorder that precedes the paints in the chain is
# applied to the COMPILED registers, where it can break a def-use pair that
# only the renamed registers make independent (a `li $a0,0x3a` swapped above
# the `andi` that reads the `$a0` load it now clobbers); the paints then
# repair it and the old audit called the repair a producer change.  When A
# and B differ only by such a layer the audit UN-PERMUTES B back into the
# reorder-free order (see unpermute_check) and asks two separate questions:
# is the rename sound in the compiler's order, and is the reorder legal in
# the renamed registers.
REORDER_STEPS = {'swap_instructions', 'scheduler_normalizer',
                 'hoist_out_of_delay_slot'}

# 2026-10-03 (red-team finding 1): the PAINT LAYER is every step that can
# rewrite fields of a word in place -- not only the named rename steps.  A
# `custom_script` (or `asm_intercept`) that edits register fields or an
# immediate was invisible: it stayed in BOTH A and B, so its edits were part
# of the reference and "0 producer changes" was guaranteed.  It is now
# removed from A with the rename layer; `patch_insn_imm16` likewise (an
# immediate edit, restricted).  What the layer may change is checked too:
# see field_changes() -- a rename may touch REGISTER fields only.
SCRIPT_STEPS = {'custom_script', 'asm_intercept'}
PAINT_LAYER_STEPS = RENAME_STEPS | SCRIPT_STEPS | {'patch_insn_imm16'}


def nonreg_fields(w):
    """A word with its register fields masked: what a pure rename may NOT
    change (opcode, funct, shift amount, REGIMM selector, immediate,
    jump target)."""
    op = w >> 26
    if op == 0:
        return w & 0xFC0007FF
    if op == 1:
        return w & 0xFC1FFFFF          # rt is the bltz/bgez selector
    if op in (2, 3) or op >= 16 and op < 20:
        return w
    return w & 0xFC00FFFF


def field_changes(A, B):
    """Word indices where the paint layer changed a NON-register field."""
    if len(A) != len(B):
        return list(range(min(len(A), len(B))))
    return [i for i, (a, b) in enumerate(zip(A, B))
            if nonreg_fields(a) != nonreg_fields(b)]
BRANCH_OPS = (1, 4, 5, 6, 7)


# ------------------------------------------------------------ decode
def decode(w):
    op = w >> 26
    rs, rt, rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
    f = w & 0x3F
    d, u, br, jmp, call = [], [], False, False, False
    if op == 0:
        if f in (0, 2, 3): d, u = [rd], [rt]
        elif f in (4, 6, 7): d, u = [rd], [rt, rs]
        elif f == 8: u, jmp = [rs], True
        elif f == 9: d, u, jmp, call = [rd], [rs], True, True
        elif f in (0x10, 0x12): d = [rd]
        elif f in (0x11, 0x13): u = [rs]
        elif f in (0x18, 0x19, 0x1a, 0x1b): u = [rs, rt]
        elif f in (0x0c, 0x0d): pass
        else: d, u = [rd], [rs, rt]
    elif op == 1: u, br = [rs], True
    elif op in (2, 3):
        jmp = True
        if op == 3: d, call = [31], True
    elif op in (4, 5): u, br = [rs, rt], True
    elif op in (6, 7): u, br = [rs], True
    elif op in (8, 9, 0xa, 0xb, 0xc, 0xd, 0xe): d, u = [rt], [rs]
    elif op == 0xf: d = [rt]
    elif op in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26):
        # lwl (0x22) / lwr (0x26) merge into rt, but IDO only ever emits them
        # as a PAIR on one register, which overwrites all four bytes: the
        # merged-in old value is dead.  Modelling lwl as a READ of rt made the
        # (dead) previous definition of rt a "producer" that a legitimate
        # rename re-points (FP, TACO/func_8015777C, 2026-09-28).
        d, u = [rt], [rs]
    elif op in (0x28, 0x29, 0x2b): u = [rt, rs]
    else: u = [rs, rt]
    return [x for x in d if x], [x for x in u if x], br, jmp, call


def blocks_of(W, jt=None):
    n = len(W)
    leaders = {0} | {x for x in (jt or ()) if 0 <= x < n}
    for i, w in enumerate(W):
        d, u, br, jmp, call = decode(w)
        if br:
            off = w & 0xFFFF
            if off & 0x8000: off -= 0x10000
            t = i + 1 + off
            if 0 <= t < n: leaders.add(t)
            if i + 2 < n: leaders.add(i + 2)
        elif jmp and not call:
            if i + 2 < n: leaders.add(i + 2)
            if (w >> 26) == 2 and (w & 0x3FFFFFF) < n:
                leaders.add(w & 0x3FFFFFF)     # gcc `j`: in-section word index
    L = sorted(leaders)
    bidx = {s: k for k, s in enumerate(L)}
    B = [(s, L[k + 1] if k + 1 < len(L) else n) for k, s in enumerate(L)]
    succ = [[] for _ in B]
    for k, (s, e) in enumerate(B):
        term = None
        for i in range(s, e):
            d, u, br, jmp, call = decode(W[i])
            if (br or jmp) and not call: term = i
        if term is None:
            if k + 1 < len(B): succ[k].append(k + 1)
        else:
            w = W[term]
            d, u, br, jmp, call = decode(w)
            if br:
                off = w & 0xFFFF
                if off & 0x8000: off -= 0x10000
                t = term + 1 + off
                if t in bidx: succ[k].append(bidx[t])
                op = w >> 26
                uncond = (op == 4 and ((w >> 21) & 31) == 0 and ((w >> 16) & 31) == 0)
                if not uncond:
                    if (term + 2) in bidx: succ[k].append(bidx[term + 2])
                    elif k + 1 < len(B): succ[k].append(k + 1)
            elif (w >> 26) == 2 and (w & 0x3FFFFFF) in bidx:
                succ[k].append(bidx[w & 0x3FFFFFF])
            # `j` / `jr` do not fall through.  Treating the word after a
            # `jr $ra` epilogue as its successor fed the epilogue's delay-slot
            # definitions into whatever block follows (usually a branch
            # target), so a legitimate rename there read as a producer change
            # (FP, ETC/func_80140BC8, 2026-09-28).  Case bodies after a
            # `jr rX` dispatch get their state from reaching() instead.
    return B, succ, bidx


def _clobber(n):
    """State after the call at n: $v0/$v1 hold its RETURN value (tagged by
    register -- an untagged token made "reads the return value" equal to
    "reads a clobbered $a0"), every other caller-saved register garbage."""
    return {r: frozenset([('call', n, r) if r in (2, 3) else ('call', n)])
            for r in CLOB}


def reaching(W, jt=None, entries=()):
    """Reaching definitions.  `jt` = the word indices a jump table in the
    object's data sections points at (see jump_targets()); None = unknown."""
    B, succ, bidx = blocks_of(W, list(jt or ()) + list(entries))
    # A branch whose DELAY SLOT is itself a branch target ends its block one
    # word early (the slot starts the next block), so the slot's definitions
    # never reached the branch's successors -- the walk behaved as if the
    # branching path skipped its own delay slot, and a join fed by that path
    # looked like it read an older definition (FP, DATE/func_8014494C,
    # 2026-09-28).  `dslot[k]` is that slot; its definitions are applied to
    # block k's out-state (its reads on this path are not recorded).
    dslot = {}
    for k, (s, e) in enumerate(B):
        if e >= len(W) or e - 1 < s:
            continue
        _, _, br, jmp, call = decode(W[e - 1])
        if (br or jmp) and not call:
            dslot[k] = e
    roots = {0} | {bidx[x] for x in entries if x in bidx}
    pred = [[] for _ in B]
    for k, ss in enumerate(succ):
        for t in ss: pred[t].append(k)
    # Jump-table targets (2026-09-27).  A `jr rX` (rX != $ra) dispatches
    # through a table this walk cannot read, so its case bodies had no
    # predecessor and every read in them came out '?' -- which compare()
    # SKIPS.  On a switch dispatcher that silently exempted every case body
    # from the audit (a paint re-pointing a case-body load at an unrelated
    # register reported "0 producer changes").  Treat every otherwise
    # predecessor-less block as a successor of each register-jump block.
    jr_blocks = []
    for k, (s, e) in enumerate(B):
        for i in range(s, e):
            w = W[i]
            if (w >> 26) == 0 and (w & 0x3F) == 8 and ((w >> 21) & 31) != 31:
                jr_blocks.append(k)
                break
    # 2026-09-28: when the object's own jump table is readable (`jt`, from
    # the R_MIPS_32 .text relocations in its data sections) only the blocks
    # it names inherit the dispatch state.  The blanket "every predecessor-
    # less block" rule also fed that state into DEAD words -- the original
    # of an as1 branch-target copy left behind an unconditional `b` -- and a
    # legitimate rename there read as a producer change (FP,
    # EN_NICHI/func_80133B70).  The blanket rule stays as the fallback for a
    # runtime-relocated table (`lui $at,0x8012`), which the object lacks.
    if jr_blocks:
        if jt:
            tgt = {bidx[x] for x in jt if x in bidx}
            for k in tgt:
                for j in jr_blocks:
                    if j not in pred[k]:
                        pred[k].append(j)
        else:
            for k in range(1, len(B)):
                if not pred[k]:
                    pred[k] = list(jr_blocks)
    IN = [dict() for _ in B]
    # Entry values are REGISTER-TAGGED (2026-09-28): an untagged 'E' made
    # "reads incoming $a1" equal to "reads incoming $a0", so a paint that
    # re-pointed a parameter read at another argument register (or at
    # garbage in a temp) was invisible -- 3 of 60 corrupted-paint controls.
    # Callee-saved registers stay untagged: their entry value is only ever
    # saved and restored, and an s-register permutation legitimately
    # re-pairs those saves (FP mode 2).
    ENTRY = {r: frozenset(['E'] if r in CALLEE_SAVED else [('E', r)])
             for r in range(32)}
    for k in roots:
        IN[k] = dict(ENTRY)

    def run(state, s, e, record=None, k=None):
        st = dict(state)
        pending = None
        stop = dslot[k] + 1 if (record is None and k in dslot) else e
        for i in range(s, stop):
            d, u, br, jmp, call = decode(W[i])
            if record is not None:
                for r in u:
                    record[(i, r)] = st.get(r, frozenset(['?']))
            for r in d: st[r] = frozenset([i])
            if pending is not None:
                if record is not None:
                    for r in ARGS:
                        record[(pending, ('arg', r))] = st.get(r, frozenset(['?']))
                st.update(_clobber(pending))
                pending = None
            if call:
                pending = i
        if pending is not None:
            if record is not None:
                for r in ARGS:
                    record[(pending, ('arg', r))] = st.get(r, frozenset(['?']))
            st.update(_clobber(pending))
        return st

    changed, it = True, 0
    while changed and it < 200:
        changed, it = False, it + 1
        for k, (s, e) in enumerate(B):
            if k in roots and not pred[k]: continue
            # a root that is also a loop head keeps the entry values
            merged = dict(ENTRY) if k in roots else {}
            for p in pred[k]:
                out = run(IN[p], B[p][0], B[p][1], k=p)
                for r, v in out.items():
                    merged[r] = merged.get(r, frozenset()) | v
            if merged and merged != IN[k]:
                IN[k] = merged
                changed = True
    rec = {}
    for k, (s, e) in enumerate(B):
        run(IN[k], s, e, rec)
    return rec


COPY_F = {0x21, 0x25, 0x20}          # addu/or/add rd,rs,$zero  == move


def _is_copy(w):
    """Return the source register if w is a register-to-register copy."""
    op, f = w >> 26, w & 0x3F
    rs, rt = (w >> 21) & 31, (w >> 16) & 31
    if op == 0 and f in COPY_F:
        if rt == 0: return rs
        if rs == 0: return rt
    # `addiu rt,rs,0` is NOT a copy here: in an unlinked object it is almost
    # always `addiu rt,rs,%lo(sym)` with the addend still zero (IDO spells a
    # move `or`/`addu`), and skipping it made a paint that re-pointed a read
    # from the full address to the bare `%hi` invisible (negative control
    # SHOUGATU/func_80140228, 2026-09-28).
    return None


def _skip_copies(W, rec, p, seen):
    """Follow move chains back to the real producer."""
    while isinstance(p, int) and p not in seen:
        seen.add(p)
        s = _is_copy(W[p])
        if s is None:
            return p
        nxt = rec.get((p, s))
        if nxt is None or len(nxt) != 1:
            return p
        p = next(iter(nxt))
    return p


_RELCTX = [None, None]       # (relocs of A, relocs of B) for _veq


def _relval(rel, w):
    """What a relocated word links to: a %hi by its VALUE when the symbol can
    be placed (two symbols sharing a %hi are one lui after linking), else by
    name; None for an unrelocated word."""
    if not rel:
        return None
    if rel[0] == 5:
        a = _name_addr(rel[1])
        if a is not None:
            return ('HI', ((a + ((w & 0xFFFF) << 16) + 0x8000) >> 16) & 0xFFFF)
    return rel


def _veq(A, B, ra_, rb, pa, pb, depth=0):
    """True if producers pa (in A) and pb (in B) compute the same VALUE.

    Covers FP mode 3 (VALUE-EQUIVALENT): the target keeps a value in a
    different register (a copy, a rematerialised constant/address, or the
    same load) than the one IDO allocated.  Without this, every such paint
    reads as a producer change even on byte-perfect objects.
    """
    if pa == pb and not isinstance(pa, int):
        return True
    pa = _skip_copies(A, ra_, pa, set())
    pb = _skip_copies(B, rb, pb, set())
    if pa == pb and not isinstance(pa, int):
        return True
    if not (isinstance(pa, int) and isinstance(pb, int)):
        return False
    wa, wb = A[pa], B[pb]
    # same operation, same shift amount / immediate: register fields may differ
    if (wa >> 26) != (wb >> 26):
        return False
    if (wa >> 26) == 0 and (wa & 0x7FF) != (wb & 0x7FF):
        return False
    if (wa >> 26) != 0 and (wa & 0xFFFF) != (wb & 0xFFFF):
        return False
    # an unlinked `lui $x,0` / `addiu $x,$y,0` is only the same value as
    # another when their RELOCATIONS agree (the immediates are both 0);
    # without this two different globals' addresses compared equal
    # (negative control BUNKAKEN/func_801526C0, 2026-09-28)
    if _RELCTX[0] is not None and pa != pb and \
            _relval(_RELCTX[0].get(pa), wa) != _relval(_RELCTX[1].get(pb), wb):
        return False
    if pa == pb:
        return True
    if depth >= 5:
        return False
    _, ua, _, _, _ = decode(wa)
    _, ub, _, _, _ = decode(wb)
    if len(ua) != len(ub):
        return False
    for r1, r2 in zip(ua, ub):
        s1 = ra_.get((pa, r1), frozenset())
        s2 = rb.get((pb, r2), frozenset())
        if not _setveq(A, B, ra_, rb, s1, s2, depth + 1):
            return False
    return True


def _setveq(A, B, ra_, rb, s1, s2, depth=0):
    if not s1 or not s2:
        return False
    for x in s1:
        if not any(_veq(A, B, ra_, rb, x, y, depth) for y in s2):
            return False
    for y in s2:
        if not any(_veq(A, B, ra_, rb, x, y, depth) for x in s1):
            return False
    return True


def compare(A, B_, jt=None, entries=(), arity=None, rels=None):
    """`arity` = {call index in A: register arguments the C passes} (see
    call_arities); without it argument registers are not compared.  `rels`
    = (relocs of A, relocs of B) makes value-equivalence reloc-aware."""
    _RELCTX[:] = list(rels) if rels else [None, None]
    try:
        return _compare(A, B_, jt, entries, arity)
    finally:
        _RELCTX[:] = [None, None]


def _compare(A, B_, jt, entries, arity):
    ra_, rb = reaching(A, jt, entries), reaching(B_, jt, entries)
    bad = []
    for i in range(min(len(A), len(B_))):
        da, ua, _, _, _ = decode(A[i])
        db, ub, _, _, _ = decode(B_[i])
        if len(ua) != len(ub):
            continue
        sa = [ra_.get((i, r), frozenset()) for r in ua]
        sb = [rb.get((i, r), frozenset()) for r in ub]
        # '?' = the instruction sits in a block the CFG walk found unreachable
        # (paints on branch operand registers can flip a beq $0,$0 into a
        # conditional and change reachability).  No claim can be made there.
        if any('?' in x for x in sa) or any('?' in x for x in sb):
            continue
        op, f = A[i] >> 26, A[i] & 0x3F
        comm = op in (4, 5) or (op == 0 and f in (0x21, 0x20, 0x24, 0x25, 0x26, 0x18, 0x19))
        key = lambda L: sorted(sorted(map(str, x)) for x in L)
        if comm and key(sa) == key(sb):
            continue
        if sa == sb:
            continue
        if comm and len(sa) == 2:
            if ((_setveq(A, B_, ra_, rb, sa[0], sb[0]) and
                 _setveq(A, B_, ra_, rb, sa[1], sb[1])) or
                (_setveq(A, B_, ra_, rb, sa[0], sb[1]) and
                 _setveq(A, B_, ra_, rb, sa[1], sb[0]))):
                continue
        elif all(_setveq(A, B_, ra_, rb, x, y) for x, y in zip(sa, sb)):
            continue
        bad.append((i, ua, ub, sa, sb))
    seen = {b[0] for b in bad}
    for i, r, sa in _arg_reads(A, ra_, arity):
        if i in seen or not _passed(sa, r) or '?' in sa:
            continue
        sb = rb.get((i, ('arg', r)), frozenset(['?']))
        if '?' in sb or sa == sb or _setveq(A, B_, ra_, rb, sa, sb):
            continue
        bad.append((i, [r], [r], [sa], [sb]))
        seen.add(i)
    bad.sort(key=lambda b: b[0])
    return bad


# ------------------------------------------------------------ build
def func_entries(o):
    """Word indices of every STT_FUNC symbol in .text (a TU can hold more
    than one function; the later ones have no predecessor and used to read
    '?' -- skipped -- throughout)."""
    try:
        from elftools.elf.elffile import ELFFile
    except ImportError:
        return []
    out = set()
    with open(o, 'rb') as f:
        e = ELFFile(f)
        text_idx = None
        for k, s in enumerate(e.iter_sections()):
            if s.name == '.text':
                text_idx = k
        symtab = e.get_section_by_name('.symtab')
        for s in symtab.iter_symbols():
            if s['st_info']['type'] == 'STT_FUNC' and s['st_shndx'] == text_idx \
                    and s['st_value'] % 4 == 0:
                out.add(s['st_value'] // 4)
    return sorted(out)


def text_relocs(o):
    """{word index: (reloc type, symbol name)} for .text relocations -- the
    identity of a reloc'd word (`lui $at,0` is one word for every %hi)."""
    try:
        from elftools.elf.elffile import ELFFile
        from elftools.elf.relocation import RelocationSection
    except ImportError:
        return {}
    out = {}
    with open(o, 'rb') as f:
        e = ELFFile(f)
        symtab = e.get_section_by_name('.symtab')
        for s in e.iter_sections():
            if not isinstance(s, RelocationSection):
                continue
            if e.get_section(s['sh_info']).name != '.text':
                continue
            for r in s.iter_relocations():
                sym = symtab.get_symbol(r['r_info_sym'])
                name = sym.name
                if not name and sym['st_shndx'] not in ('SHN_UNDEF', 'SHN_ABS'):
                    name = e.get_section(sym['st_shndx']).name
                out[r['r_offset'] // 4] = (r['r_info_type'], name)
    return out


def jump_targets(o):
    """Word indices in .text named by R_MIPS_32 relocations against .text
    in the object's data sections -- IDO's switch jump tables (the addend is
    the case body's .text offset, stored in the table word itself)."""
    try:
        from elftools.elf.elffile import ELFFile
        from elftools.elf.relocation import RelocationSection
    except ImportError:
        return None
    out = set()
    with open(o, 'rb') as f:
        e = ELFFile(f)
        text_idx = None
        for k, s in enumerate(e.iter_sections()):
            if s.name == '.text':
                text_idx = k
        symtab = e.get_section_by_name('.symtab')
        for s in e.iter_sections():
            if not isinstance(s, RelocationSection):
                continue
            tgt = e.get_section(s['sh_info'])
            # data sections only: a gcc object's .pdr also carries
            # R_MIPS_32 .text relocations (one per procedure start)
            if not tgt.name.startswith(('.rodata', '.rdata', '.data',
                                        '.sdata')) or \
                    tgt['sh_type'] == 'SHT_NOBITS':
                continue
            data = tgt.data()
            for r in s.iter_relocations():
                if r['r_info_type'] != 2:              # R_MIPS_32
                    continue
                sym = symtab.get_symbol(r['r_info_sym'])
                if sym['st_shndx'] != text_idx:
                    continue
                off = r['r_offset']
                if off + 4 > len(data):
                    continue
                v = struct.unpack_from('<I', data, off)[0] + sym['st_value']
                if v % 4 == 0:
                    out.add(v // 4)
    return sorted(out)


def objwords(o, tmp):
    subprocess.run(["mipsel-linux-gnu-objcopy", "-O", "binary",
                    "--only-section=.text", str(o), str(tmp)],
                   capture_output=True)
    b = open(tmp, 'rb').read()
    return list(struct.unpack("<%dI" % (len(b) // 4), b[:len(b) // 4 * 4]))


def build(ov, func, workdir, drop_paints, src_text=None, drop_reorder=False):
    src = SRC / ov / f"{func}.c"
    text = src_text if src_text is not None else src.read_text(errors="replace")
    if src_text is not None:
        src = Path(workdir) / f"{func}.c"
        src.write_text(text)
    steps = parse_postproc(src)
    if drop_paints:
        steps = [s for s in steps if s.get('type') not in PAINT_LAYER_STEPS]
    if drop_reorder:
        # The reorder-free reference is the RAW compile.  Dropping only the
        # reorder steps is not enough: position-keyed steps later in the chain
        # (adjust_branch_offset --offset, swap_instructions after a
        # normalizer) then land on different words (TACO/func_8014A8B0).
        # The immediate-only steps this also drops are masked in _bkey.
        steps = []
    flags = parse_compile_flags(src)
    cenv = parse_compile_env(src)
    compiler = parse_compiler(src)

    funcs = {}
    for line in (CFG / f"{ov}.sym").read_text().splitlines():
        p = line.strip().split(":")
        if len(p) >= 3:
            try:
                funcs[p[2]] = (int(p[0], 16), int(p[1]))
            except ValueError:
                pass
    addr, size = funcs.get(func, (None, None))
    ss, st = parse_symbol_size_text(text), parse_symbol_start_text(text)
    if ss is not None: size = ss
    if st is not None: addr = st

    # The object's stem is the identity the restricted-PostProc gate checks
    # against the grandfather list, so it must stay the real function name;
    # the two variants live in separate subdirectories instead.
    objdir = Path(workdir) / ("nr" if drop_reorder else
                              "np" if drop_paints else "fp")
    objdir.mkdir(exist_ok=True)
    obj = objdir / f"{func}.o"
    if compiler.startswith("gcc"):
        cmd = [sys.executable, str(GCCWRAP), str(src), "-o", str(obj),
               "--gcc-version", compiler[3:], "-I", str(INC), "-I", str(INC / "psyq")]
        extra = []
        for f in flags:
            (cmd if f in GCCWRAP_FLAGS else extra).append(f)
        if extra:
            cmd += ["--extra-flags", " ".join(extra)]
    else:
        cmd = ([str(IDO_CC)] + IDO_FLAGS
               + pic_flags(src, compile_flags=flags)
               + flags + ["-c", str(src), "-o", str(obj)])
    env = os.environ.copy()
    env["TMPDIR"] = str(workdir)
    env.update(cenv or {})
    r = subprocess.run(cmd, capture_output=True, text=True, timeout=600,
                       env=env, cwd=str(workdir))
    if r.returncode != 0:
        return None, "compile failed: " + r.stderr[:300]
    asm = ROOT / "asm" / "overlay" / ov / f"{func}.s"
    try:
        apply_postproc(obj, steps, addr, ov, verbose=False,
                       target_asm=asm if asm.exists() else None)
    except Exception as e:
        return None, "postproc failed: %r" % (e,)
    return objwords(obj, Path(workdir) / "w.bin"), None


def _bkey(w):
    """Pairing key: branch displacements and $sp-relative immediates are
    masked (adjust_branch_offset / patch_stack / remap_stack_slots rewrite
    them), and ori/addiu from $zero are one operation (convert_ori_to_addiu)."""
    op = w >> 26
    if op in BRANCH_OPS:
        return w & 0xFFFF0000
    if ((w >> 21) & 31) == 29 and (op >= 0x20 or op == 9):
        return w & 0xFFFF0000
    if op == 13 and ((w >> 21) & 31) == 0:
        return (9 << 26) | (w & 0x03FFFFFF)
    return w


def _name_addr(sym):
    try:
        from resolve_overlay_relocs import name_to_addr
    except Exception:                      # noqa: BLE001
        return None
    try:
        return name_to_addr(sym)
    except Exception:                      # noqa: BLE001
        return None


def _mkey(w, rel):
    """Identity of a word for match_permutation.  A `lui` carrying an
    R_MIPS_HI16 whose symbol address is known is identified by the %hi VALUE
    it links to, register ignored: two such luis with one %hi are the same
    instruction after linking, and a reorder + paint pair that exchanges
    them (a `lui $t6,%hi(A)` / `lui $t8,%hi(B)` crossing, the paints then
    re-pointing each consumer at the other word) changes no linked value.
    Without this the un-permuted order puts such a consumer ahead of its
    `lui` and reports a producer change that cannot exist in the linked
    code (DATE/func_80140BF8, 2026-09-28).  A lui whose symbol cannot be
    placed keeps its exact identity."""
    if (w >> 26) == 15 and rel and rel[0] == 5:
        a = _name_addr(rel[1])
        if a is not None:
            return ('LUIHI', ((a + ((w & 0xFFFF) << 16) + 0x8000) >> 16) & 0xFFFF)
    return (_bkey(w), rel)


def match_permutation(R, A, rrel=None, arel=None, shape=False):
    """pi with A[pi[i]] ~ R[i] (branch displacements masked; a word's
    relocation is part of its identity, since swap_instructions carries it),
    pairing equal words in position order; None when A is not a permutation
    of R."""
    if len(R) != len(A):
        return None
    rrel, arel = rrel or {}, arel or {}
    if shape == 'tiered':
        return _match_shape(R, A, rrel, arel)
    if shape == 'ranked':
        return _match_ranked(R, A, rrel, arel)
    key = (lambda w, rel: (_shape(w), rel)) if shape else _mkey
    pos = {}
    for j, w in enumerate(A):
        pos.setdefault(key(w, arel.get(j)), []).append(j)
    pi = []
    for i, w in enumerate(R):
        L = pos.get(key(w, rrel.get(i)))
        if not L:
            return None
        pi.append(L.pop(0))
    return pi


def _exact_shape(w):
    """Register fields masked, every immediate KEPT (incl. $sp-relative
    offsets and branch displacements, which _shape masks)."""
    op = w >> 26
    if op == 0:
        return w & 0xFC0007FF
    if op in (2, 3):
        return w
    if op == 13 and ((w >> 21) & 31) == 0:
        w = (9 << 26) | (w & 0x03FFFFFF)
    return w & 0xFC00FFFF


def _match_shape(R, A, rrel, arel):
    """Register-blind pairing in two tiers (2026-10-04).  _shape masks
    $sp-relative immediates (remap_stack_slots / patch_stack rewrite them),
    so position order alone paired `addiu a0,sp,88` with `addiu v1,sp,96`
    whenever a reorder moved one sp-address above the other -- every read of
    the two pointers then looked re-pointed (main system_config_icon_mode_init /
    system_config_mouse_menu_init, a reorder custom_script that also widens the frame).
    Tier 1 pairs words whose immediates agree too; only the words left over
    fall back to the masked shape.  It is one CANDIDATE: when the reorder
    layer also remaps the stack (system_config_mouse_menu_init's script moves every slot +8)
    tier 1 pairs sp+88 with the slot that was sp+80, so _unpermute_check
    tries both pairings and keeps the one the dataflow agrees with."""
    pi = [None] * len(R)
    pos = {}
    for j, w in enumerate(A):
        pos.setdefault((_exact_shape(w), arel.get(j)), []).append(j)
    used = set()
    for i, w in enumerate(R):
        L = pos.get((_exact_shape(w), rrel.get(i)))
        if L:
            pi[i] = L.pop(0)
            used.add(pi[i])
    pos = {}
    for j, w in enumerate(A):
        if j not in used:
            pos.setdefault((_shape(w), arel.get(j)), []).append(j)
    for i, w in enumerate(R):
        if pi[i] is None:
            L = pos.get((_shape(w), rrel.get(i)))
            if not L:
                return None
            pi[i] = L.pop(0)
    return pi


def _sp_rel(w):
    op = w >> 26
    return ((w >> 21) & 31) == 29 and (op >= 0x20 or op == 9)


def _match_ranked(R, A, rrel, arel):
    """Register-blind pairing with $sp-relative words paired by OFFSET RANK
    inside their masked shape (ties in position order), everything else in
    position order.  A stack remap (remap_stack_slots / patch_stack, or a
    custom_script doing either) is a monotonic shift of slot offsets, so the
    k-th smallest offset of a shape before it is the k-th smallest after it
    -- where position order pairs a reordered sp+80 / sp+88 crosswise and
    exact-offset order (tier 1) pairs sp+88 with the shifted sp+80
    (system_config_mouse_menu_init, 2026-10-04: a script that moves every slot +8 AND
    reorders the loop).  One more candidate for _unpermute_check."""
    def key(w, rel):
        return (_shape(w), rel)

    def imm(w):
        v = w & 0xFFFF
        return v - 0x10000 if v & 0x8000 else v
    rpos, apos = {}, {}
    for i, w in enumerate(R):
        rpos.setdefault(key(w, rrel.get(i)), []).append(i)
    for j, w in enumerate(A):
        apos.setdefault(key(w, arel.get(j)), []).append(j)
    pi = [None] * len(R)
    for k, ri in rpos.items():
        aj = apos.get(k, [])
        if len(aj) != len(ri):
            return None
        if _sp_rel(R[ri[0]]):
            ri = sorted(ri, key=lambda i: (imm(R[i]), i))
            aj = sorted(aj, key=lambda j: (imm(A[j]), j))
        for i, j in zip(ri, aj):
            pi[i] = j
    return pi


def _repair_lui_groups(R, B, pi, jt, entries, rrel, arel):
    """Position-order pairing cannot see WHICH of several interchangeable
    luis (one %hi value, see _mkey) the paints made each consumer read.
    Re-pair every such group by its consumers: an R lui goes to the B lui
    that its (uniquely paired) consumers read in B.  Kept only when that is
    a bijection on the group; otherwise the position-order pairing stands."""
    groups = {}
    for i, w in enumerate(R):
        k = _mkey(w, rrel.get(i))
        if k[0] == 'LUIHI':
            groups.setdefault(k, []).append(i)
    groups = {k: v for k, v in groups.items() if len(v) > 1}
    if not groups:
        return pi
    rR, rB = reaching(R, jt, entries), reaching(B, jt, entries)
    readers = {}
    for c, w in enumerate(R):
        _, ua, _, _, _ = decode(w)
        for pos, r in enumerate(ua):
            for p in rR.get((c, r), ()):
                if isinstance(p, int):
                    readers.setdefault(p, []).append((c, pos))
    pi = list(pi)
    for members in groups.values():
        bpos = {pi[i] for i in members}
        want = {}
        for i in members:
            cand = None
            for c, pos in readers.get(i, []):
                if c in members:
                    continue
                j = pi[c]
                _, ub, _, _, _ = decode(B[j])
                if pos >= len(ub):
                    cand = False
                    break
                got = {p for p in rB.get((j, ub[pos]), ()) if p in bpos}
                if len(got) != 1 or (cand is not None and cand != got):
                    cand = False
                    break
                cand = got
            if cand:
                want[i] = next(iter(cand))
        if len(want) == len(members) and len(set(want.values())) == len(members):
            for i, j in want.items():
                pi[i] = j
    return pi


def _commutative(w):
    op, f = w >> 26, w & 0x3F
    return op in (4, 5) or (op == 0 and f in (0x21, 0x20, 0x24, 0x25, 0x26,
                                              0x27, 0x18, 0x19))


def _shape(w):
    """A word with its register fields masked (see _repair_identical_groups)."""
    op = w >> 26
    if op == 0:
        return w & 0xFC0007FF
    if op in (2, 3):
        return w
    if op == 13 and (w & 0xFFFF) < 0x8000:
        w = (9 << 26) | (w & 0x03FFFFFF)    # convert_ori_to_addiu (lui form)
    return _bkey(w) & 0xFC00FFFF


def _repair_identical_groups(R, B, pi, rR, rB, brel, exact=False):
    """Words of one SHAPE (same operation, immediate and relocation; only
    the registers differ) cannot be told apart by position once paints are
    applied, and a reorder + paint pair can exchange them: a normalizer swaps
    `sb $0,0($v0)` with `sb $0,0($t6)` and the `t6->v0` paint makes both
    `sb $0,0($v0)` again, or swaps two `addiu rX,rY,1` and the paints
    re-point each at the other's operand -- the object is the compiler's
    order after all (TT/func_801473AC, GEKO/func_80135A8C).  Paints change
    only register fields, so any shape-preserving pairing is a candidate
    isomorphism: choose the one that agrees best with the dataflow
    (exhaustive up to 6 members, pairwise hill-climb up to 40); ties keep
    the position-order pairing."""
    import itertools
    groups = {}
    for j, w in enumerate(B):
        if w:
            # exact: group by _exact_shape -- two $sp-relative words with
            # different offsets (`addiu a0,sp,88` / `addiu v1,sp,96`) are
            # then not interchangeable.  With _shape the per-group climb could
            # exchange them in step with a second, coupled group: a local
            # optimum that re-pointed both pointers (main system_config_icon_mode_init,
            # 2026-10-04).  _unpermute_check tries both groupings.
            k = _exact_shape(w) if exact else _shape(w)
            groups.setdefault((k, brel.get(j)), []).append(j)
    groups = [g for g in groups.values() if 1 < len(g) <= 40]
    if not groups:
        return pi
    pi = list(pi)
    inv = [0] * len(pi)
    for i, j in enumerate(pi):
        inv[j] = i
    readers = {}
    for c, w in enumerate(R):
        _, ua, _, _, _ = decode(w)
        for pos, r in enumerate(ua):
            for p in rR.get((c, r), ()):
                if isinstance(p, int):
                    readers.setdefault(p, []).append((c, pos))

    def mp(s, sub):
        out = set()
        for x in s:
            if isinstance(x, int):
                out.add(sub.get(x, pi[x]))
            elif isinstance(x, tuple) and x[0] == 'call':
                out.add(('call', sub.get(x[1], pi[x[1]])) + x[2:])
            else:
                out.add(x)
        return frozenset(out)

    def score(sub):
        bad = 0
        for i, j in sub.items():
            _, ua, _, _, _ = decode(R[i])
            _, ub, _, _, _ = decode(B[j])
            if len(ua) != len(ub):
                return 1 << 30
            xs = [mp(rR.get((i, r), frozenset()), sub) for r in ua]
            ys = [rB.get((j, r), frozenset()) for r in ub]

            def miss(xs, ys):
                return sum(1 for x, y in zip(xs, ys)
                           if x != y and not _setveq(B, B, rB, rB, x, y))
            m = miss(xs, ys)
            if m and len(xs) == 2 and _commutative(B[j]):
                m = min(m, miss(xs, ys[::-1]))
            bad += m
            for c, pos in readers.get(i, []):
                if c in sub:
                    continue
                _, uc, _, _, _ = decode(B[pi[c]])
                if pos < len(uc):
                    y = rB.get((pi[c], uc[pos]), frozenset())
                    if j not in y and not _setveq(B, B, rB, rB,
                                                  frozenset([j]), y):
                        bad += 1
        return bad

    for _ in range(2):
        for g in groups:
            members = [inv[j] for j in g]
            cur = {i: pi[i] for i in members}
            best, best_s = cur, score(cur)
            if best_s == 0:
                continue
            if len(g) <= 6:
                for perm in itertools.permutations(g):
                    sub = dict(zip(members, perm))
                    s = score(sub)
                    if s < best_s:
                        best, best_s = sub, s
            else:                                   # pairwise hill-climb
                improved = True
                while improved and best_s:
                    improved = False
                    for a, b in itertools.combinations(members, 2):
                        sub = dict(best)
                        sub[a], sub[b] = sub[b], sub[a]
                        s = score(sub)
                        if s < best_s:
                            best, best_s, improved = sub, s, True
            for i, j in best.items():
                pi[i] = j
                inv[j] = i
    return pi


def unpermute_check(R, A, B, jt, entries, rrel=None, arel=None, brel=None,
                    arity=None):
    _RELCTX[:] = [brel, brel] if brel is not None else [None, None]
    try:
        return _unpermute_check(R, A, B, jt, entries, rrel, arel, brel, arity)
    finally:
        _RELCTX[:] = [None, None]


def _unpermute_check(R, A, B, jt, entries, rrel, arel, brel, arity):
    """R = no rename, no reorder; A = reorder, no rename; B = full chain.

    pi pairs every R word with the B word it became (via A, which differs
    from R only by the permutation).  Every operand read of R[i] must then
    have, in B, the producers pi maps R's producers to -- the reorder and the
    rename judged TOGETHER, since either alone can be unsound where the
    composite is not (a prologue `sw $s1` swapped below the renamed `lw $s1`
    is wrong before the swap and right after it, TT/func_801437E0).
    Returns (bad, pi) with bad in B positions, or None if A is not a pure
    permutation of R."""
    best = None
    for cand in _unpermute_candidates(R, A, B, rrel, arel, brel):
        res = _unpermute_eval(R, B, jt, entries, rrel, arel, brel, arity, *cand)
        if res is not None and (best is None or len(res[0]) < len(best[0])):
            best = res
            if not res[0]:
                break
    return best


def _unpermute_candidates(R, A, B, rrel, arel, brel):
    """(pi, exact-grouping) candidates for _unpermute_eval, the original
    pairing first.  Every candidate is a shape-preserving bijection R -> B
    (same operation, same non-$sp immediates, same relocation), and the
    verdict only accepts one under which EVERY read keeps its producers, so
    taking the best candidate is as sound as the group repair's own search."""
    pi = match_permutation(R, A, rrel, arel)
    # pi pairs R with A; it pairs R with B only when B is A position for
    # position (the paint layer renames in place).  A REORDER custom_script
    # is in the paint layer (dropped from A, kept in B), so then A and B are
    # NOT aligned and pairing through A pairs each R word with whatever B
    # word sits at A's position -- a different instruction (2026-10-04:
    # ENDING/func_80135F6C and GEKO/func_80142BCC, 37 and 42 bogus flags,
    # every one an R word compared against an unrelated B word).
    if pi is not None and any(_shape(a) != _shape(b) for a, b in zip(A, B)):
        pi = None
    if pi is None:
        # The chain interleaves paints with reorders (a normalizer run AFTER
        # a paint picks different moves on unpainted registers, or a reorder
        # custom_script), so A is not B's order.  Pair R with B directly,
        # register-blind; the group repairs below settle the ambiguous shapes.
        pi = match_permutation(R, B, rrel, brel, shape=True)
    if pi is not None:
        yield pi, False
        yield pi, True
    # 2026-10-04: the two-tier pairing (immediates first) -- see _match_shape.
    pt = match_permutation(R, B, rrel, brel, shape='tiered')
    if pt is not None and pt != pi:
        yield pt, True
        yield pt, False
    pr = match_permutation(R, B, rrel, brel, shape='ranked')
    if pr is not None and pr != pi and pr != pt:
        yield pr, True
        yield pr, False


def _unpermute_eval(R, B, jt, entries, rrel, arel, brel, arity, pi, exact):
    pi = _repair_lui_groups(R, B, pi, jt, entries, rrel or {},
                            (brel if brel is not None else arel) or {})
    rR, rB = reaching(R, jt, entries), reaching(B, jt, entries)
    if brel is not None:
        pi = _repair_identical_groups(R, B, pi, rR, rB, brel, exact)

    def mp(s):
        out = set()
        for x in s:
            if isinstance(x, int):
                out.add(pi[x])
            elif isinstance(x, tuple) and x[0] == 'call':
                out.add(('call', pi[x[1]]) + x[2:])
            else:
                out.add(x)
        return frozenset(out)

    bad = []
    for i in range(len(R)):
        j = pi[i]
        _, ua, _, _, _ = decode(R[i])
        _, ub, _, _, _ = decode(B[j])
        if len(ua) != len(ub):
            continue
        sa = [rR.get((i, r), frozenset(['?'])) for r in ua]
        sb = [rB.get((j, r), frozenset(['?'])) for r in ub]
        if any('?' in x for x in sa) or any('?' in x for x in sb):
            continue
        sa = [mp(x) for x in sa]
        if sa == sb:
            continue
        op, f = B[j] >> 26, B[j] & 0x3F
        comm = op in (4, 5) or (op == 0 and f in (0x21, 0x20, 0x24, 0x25, 0x26,
                                                   0x18, 0x19))
        if comm and len(sa) == 2 and sa[0] == sb[1] and sa[1] == sb[0]:
            continue
        # value-equivalence, judged inside B (both sets are B indices now)
        if comm and len(sa) == 2:
            if ((_setveq(B, B, rB, rB, sa[0], sb[0]) and
                 _setveq(B, B, rB, rB, sa[1], sb[1])) or
                (_setveq(B, B, rB, rB, sa[0], sb[1]) and
                 _setveq(B, B, rB, rB, sa[1], sb[0]))):
                continue
        elif all(_setveq(B, B, rB, rB, x, y) for x, y in zip(sa, sb)):
            continue
        bad.append((j, ua, ub, sa, sb))
    seen = {b[0] for b in bad}
    for i, r, sa in _arg_reads(R, rR, arity):
        j = pi[i]
        if j in seen or not _passed(sa, r) or '?' in sa:
            continue
        sa = mp(sa)
        sb = rB.get((j, ('arg', r)), frozenset(['?']))
        if '?' in sb or sa == sb or _setveq(B, B, rB, rB, sa, sb):
            continue
        bad.append((j, [r], [r], [sa], [sb]))
        seen.add(j)
    bad.sort(key=lambda b: b[0])
    return bad, pi


def audit(ov, func, src_text=None):
    """Return {'nA','nB','bad','A','B'} or {'error': msg}.

    'mode' says which comparison produced 'bad': 'direct' (A = chain minus
    the rename layer, compared index by index) or 'unpermuted' (the chain
    also carries a reorder layer; see unpermute_check).  When the reorder
    layer cannot be removed cleanly the direct result stands, with
    'unpermute_error' saying why."""
    import overlay_names
    overlay_names.set_ambient(ov)      # this overlay's landed names resolve first
    wd = tempfile.mkdtemp(prefix="iso_")
    try:
        A, e1 = build(ov, func, wd, True, src_text)
        if A is None: return {"error": e1}
        B, e2 = build(ov, func, wd, False, src_text)
        if B is None: return {"error": e2}
        objA = Path(wd) / "np" / f"{func}.o"
        jt = jump_targets(objA)
        ent = func_entries(objA)
        text = src_text if src_text is not None else \
            (SRC / ov / f"{func}.c").read_text(errors="replace")
        arel = text_relocs(objA)
        bad = compare(A, B, jt, ent, call_arities(text, arel),
                      (arel, text_relocs(Path(wd) / "fp" / f"{func}.o")))
        res = {"nA": len(A), "nB": len(B), "bad": bad, "A": A, "B": B,
               "mode": "direct"}
        src = SRC / ov / f"{func}.c"
        steps = parse_postproc(Path(wd) / f"{func}.c" if src_text is not None
                               else src)
        # What the paint layer changed besides registers.  A script that only
        # PERMUTES words (a reorder custom_script) changes fields at many
        # indices but is a permutation of A: judge it as a reorder below.
        # A reorder step that runs AFTER a paint (a normalizer choosing on
        # renamed registers, a reorder custom_script) also moves words
        # between A and B; that is a permutation, not a field edit, and the
        # unpermute mode judges it.  Shapes mask branch displacements and
        # $sp offsets, which position-keyed steps legitimately rewrite.
        fc = field_changes(A, B)
        reorders = any(s.get('type') in SCRIPT_STEPS | REORDER_STEPS for s in steps)
        perm = bool(fc) and reorders and len(A) == len(B) and \
            sorted(map(_shape, A)) == sorted(map(_shape, B))
        res["field"] = [] if perm else fc
        if (bad or perm) and any(s.get('type') in REORDER_STEPS | SCRIPT_STEPS
                                 for s in steps):
            try:
                R, e3 = build(ov, func, wd, True, src_text, drop_reorder=True)
            except Exception as ex:          # noqa: BLE001
                R, e3 = None, repr(ex)
            if R is None:
                res["unpermute_error"] = "reorder-free build failed: %s" % e3
                return res
            rrel = text_relocs(Path(wd) / "nr" / f"{func}.o")
            brel = text_relocs(Path(wd) / "fp" / f"{func}.o")
            u = unpermute_check(R, A, B, jt, ent, rrel, text_relocs(objA),
                                brel, call_arities(text, rrel))
            res["rrel"], res["brel"] = rrel, brel
            if u is None:
                res["unpermute_error"] = ("the reorder layer is not a pure "
                                          "permutation of the reorder-free "
                                          "object")
                if perm:
                    res["field"] = fc     # a "reorder" script that is not one
                return res
            res.update(bad=u[0], mode="unpermuted", R=R, pi=u[1])
            res["hi_repaired"] = sum(
                1 for i, w in enumerate(R)
                if (w >> 26) == 15 and rrel.get(i) and
                rrel.get(i) != brel.get(u[1][i]))
        return res
    finally:
        shutil.rmtree(wd, ignore_errors=True)


def self_test():
    """Synthetic fixtures for unpermute_check (2026-10-04).  The reorder
    layer may live in the PAINT layer (a reorder custom_script is dropped
    from A with the renames), so A is the reorder-free order and B is not A
    position for position; pairing R with B through A then compared
    unrelated words.  Fixture 1/2 must pass (an independent swap, a swap plus
    a consistent rename); 3/4 must be caught (a use hoisted above its def;
    a rename that re-points a read at another value)."""
    LI_T0_1, LI_T1_2 = 0x24080001, 0x24090002           # li t0,1 ; li t1,2
    LI_T2_1 = 0x240A0001                                 # li t2,1
    SUBU = lambda d, s_, t: (s_ << 21) | (t << 16) | (d << 11) | 0x23
    JR, NOP = 0x03E00008, 0
    R = [LI_T0_1, LI_T1_2, SUBU(2, 8, 9), JR, NOP]       # v0 = t0 - t1
    A = list(R)                                          # script dropped
    cases = [
        ("independent swap (script reorder)",
         [LI_T1_2, LI_T0_1, SUBU(2, 8, 9), JR, NOP], False),
        ("swap + consistent rename t0->t2",
         [LI_T1_2, LI_T2_1, SUBU(2, 10, 9), JR, NOP], False),
        ("use hoisted above its def",
         [LI_T0_1, SUBU(2, 8, 9), LI_T1_2, JR, NOP], True),
        ("swap + operands re-pointed (t1 - t0)",
         [LI_T1_2, LI_T0_1, SUBU(2, 9, 8), JR, NOP], True),
    ]
    fails = 0
    for name, B, want in cases:
        u = unpermute_check(R, A, B, None, (0,), {}, {}, {}, None)
        got = bool(u and u[0]) if u is not None else True
        ok = got == want
        fails += not ok
        print(f"  {'ok ' if ok else 'FAIL'} {name}: flagged={got} want={want}")
    # 2026-10-04 (main system_config_icon_mode_init / system_config_mouse_menu_init): a reorder script that
    # also widens the frame or remaps every slot +8 while renaming v1<->a1.
    # The $sp-masked pairing paired the two sp-addresses crosswise and the
    # coupled group repair settled in a local optimum; the tiered / ranked
    # candidates (_unpermute_candidates) pair them right.  The old code
    # flags 5 and 6; 7 (a store of the pointer for the value) must flag.
    ad = lambda rt, rs, imm: (9 << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)
    sh = lambda rt, b, imm: (41 << 26) | (b << 21) | (rt << 16) | (imm & 0xFFFF)

    def lr(f, p0, p1):
        return [ad(29, 29, -f), ad(3, 0, -64), ad(4, 29, p0), ad(5, 29, p1),
                ad(4, 4, 2), ad(5, 5, 2), ad(3, 3, 16), sh(3, 4, 0),
                sh(3, 5, -2), ad(29, 29, f), JR, NOP]

    def lb(v=5):
        return [ad(29, 29, -112), ad(3, 29, 96), ad(4, 29, 88), ad(5, 0, -64),
                ad(5, 5, 16), ad(3, 3, 2), ad(4, 4, 2), sh(v, 4, 0),
                sh(5, 3, -2), ad(29, 29, 112), JR, NOP]
    for name, R2, B, want in [
            ("reorder script + frame widen + v1<->a1", lr(104, 88, 96), lb(), False),
            ("reorder script + slot remap +8 + v1<->a1", lr(96, 80, 88), lb(), False),
            ("remap+reorder storing the pointer for the value",
             lr(96, 80, 88), lb(3), True)]:
        u = unpermute_check(R2, list(R2), B, [], (), {}, {}, {}, None)
        got = bool(u and u[0]) if u is not None else True
        ok = got == want
        fails += not ok
        print(f"  {'ok ' if ok else 'FAIL'} {name}: flagged={got} want={want}")
    print("self-test:", "PASS" if not fails else f"{fails} FAILED")
    return 1 if fails else 0


def main():
    ap = argparse.ArgumentParser(
        description="Reaching-definition isomorphism audit of a "
                    "patch_insn_reg_field paint layer (ADVISORY signal — read "
                    "the module docstring's FALSE-POSITIVE MODES before "
                    "stripping anything).",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="exit 0 = no producer changes, 1 = flags present, 2 = error")
    ap.add_argument("overlay", nargs="?")
    ap.add_argument("func", nargs="?")
    ap.add_argument("--max", type=int, default=30,
                    help="max flags to print (default 30, 0 = all)")
    ap.add_argument("--all", action="store_true",
                    help="audit every byte_perfect/reloc_only overlay function whose "
                         "chain carries a paint-layer step (rename, custom_script, "
                         "asm_intercept, patch_insn_imm16); one line each")
    ap.add_argument("--jobs", type=int, default=min(16, os.cpu_count() or 1))
    ap.add_argument("--json", help="with --all: write per-function results")
    ap.add_argument("--self-test", action="store_true",
                    help="run the synthetic unpermute fixtures")
    args = ap.parse_args()

    if args.self_test:
        return self_test()

    if args.all:
        return audit_all(args.jobs, args.json, args.overlay)
    if not (args.overlay and args.func):
        ap.error("OVERLAY FUNC required (or --all)")
    r = audit(args.overlay, args.func)
    if "error" in r:
        print(f"{args.overlay}/{args.func}: ERROR {r['error']}")
        return 2
    if r.get("field"):
        print(f"{args.overlay}/{args.func}: {len(r['field'])} word(s) whose NON-register "
              f"fields the paint layer changed (opcode/immediate/displacement -- "
              f"a rename may only touch registers):")
        for i in r["field"][:args.max or None]:
            print(f"   .text 0x{i*4:x}: {r['A'][i]:08x} -> {r['B'][i]:08x}")
    bad = r["bad"]
    print(f"{args.overlay}/{args.func}: {len(bad)} instruction(s) whose operand "
          f"PRODUCERS changed" + (" (reorder layer un-permuted)"
                                  if r.get("mode") == "unpermuted" else ""))
    if r.get("hi_repaired"):
        print(f"   note: {r['hi_repaired']} lui(s) now feed a consumer of a "
              f"DIFFERENT symbol with the same %hi (value-equivalent after "
              f"link; a reloc-pairing break -- gen_reg_patches refuses these "
              f"for new paints)")
    if r.get("unpermute_error"):
        print(f"   note: reorder layer not removed -- {r['unpermute_error']}; "
              f"flags may be FP mode 1 (UPSTREAM REPAIR)")
    A, B_ = r["A"], r["B"]
    if r.get("mode") == "unpermuted":
        # show the REORDER-FREE word each flagged B word came from; producer
        # indices on both sides are B positions
        A = [0] * len(B_)
        for i, j in enumerate(r["pi"]):
            A[j] = r["R"][i]
    show = bad if args.max == 0 else bad[:args.max]
    for i, ua, ub, sa, sb in show:
        print(f"   .text 0x{i*4:x}: {'%08x' % A[i]} -> {'%08x' % B_[i]}  "
              f"uses {[RA[x] for x in ua]}->{[RA[x] for x in ub]}  "
              f"producers {[sorted(x, key=str) for x in sa]} -> "
              f"{[sorted(x, key=str) for x in sb]}")
    return 1 if (bad or r.get("field")) else 0


def _all_one(task):
    ov, fn = task
    import io
    import contextlib
    try:
        with contextlib.redirect_stdout(io.StringIO()), \
                contextlib.redirect_stderr(io.StringIO()):
            r = audit(ov, fn)
    except BaseException as ex:                      # noqa: BLE001
        return {"id": f"{ov}/{fn}", "error": repr(ex)[:160]}
    if "error" in r:
        return {"id": f"{ov}/{fn}", "error": r["error"][:160]}
    return {"id": f"{ov}/{fn}", "bad": len(r["bad"]), "field": len(r.get("field") or []),
            "mode": r.get("mode"), "unpermute_error": r.get("unpermute_error"),
            "bad_at": [b[0] for b in r["bad"]][:8], "field_at": (r.get("field") or [])[:8]}


def audit_all(jobs, json_out=None, only_ov=None):
    """Tree-wide census.  Exit 1 when any function flags (producer changes or
    non-register field changes), 2 on errors only."""
    import json
    st = json.load(open(ROOT / "config" / "overlay_match_status.json"))
    tasks = []
    for ov in sorted(st):
        if only_ov and ov != only_ov:
            continue
        for fn, info in sorted(st[ov].items()):
            if not isinstance(info, dict) or info.get("status") not in ("byte_perfect",
                                                                       "reloc_only"):
                continue
            src = SRC / ov / f"{fn}.c"
            if not src.exists():
                continue
            try:
                steps = parse_postproc(src)
            except BaseException:                    # noqa: BLE001
                continue
            if any(s.get("type") in PAINT_LAYER_STEPS for s in steps):
                tasks.append((ov, fn))
    import multiprocessing as mp
    with mp.get_context("fork").Pool(max(1, jobs)) as pool:
        res = list(pool.imap(_all_one, tasks, chunksize=2))
    flagged = [r for r in res if r.get("bad") or r.get("field")]
    errs = [r for r in res if "error" in r]
    for r in flagged:
        print(f"FLAG  {r['id']:28s} producer_changes={r['bad']} field_changes={r['field']}"
              f" mode={r['mode']}" + (f" ({r['unpermute_error']})" if r.get("unpermute_error") else "")
              + f" at={r['bad_at'] or r['field_at']}")
    for r in errs:
        print(f"ERROR {r['id']:28s} {r['error']}")
    print(f"\naudited {len(res)} painted overlay function(s): {len(flagged)} flagged, "
          f"{len(errs)} error(s)")
    if json_out:
        Path(json_out).write_text(json.dumps(res, indent=0) + "\n")
    return 1 if flagged else (2 if errs else 0)


if __name__ == "__main__":
    sys.exit(main())
