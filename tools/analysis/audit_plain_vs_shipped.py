#!/usr/bin/env python3
"""Whole-chain dataflow gate: does the SHIPPED object do what the C says?

Every other gate in this tree compares the shipped (post-PostProc) object with
RETAIL.  That comparison is satisfied by construction whenever a PostProc step
writes retail's words into the object -- `fill_delay_slots` copying a retail
instruction onto a compiled nop, `swap_spill_with_move` turning a `sw` into an
`or`, `invert_branch` flipping `beq`/`bne`, `nop_at_offset` deleting a real
instruction.  None of those moves the instruction count, so the count test
cannot see them either.  This gate asks the honest question instead:

    Is the shipped object a register-renaming plus a rescheduling of what the
    C compiles to with NO PostProc at all?

Method (promoted from the 2026-09-27 hardening audit, `ha/ha_dfa.py`; see
docs/reference/PS1-HARDENING-AUDIT.md section 3):

  * compile the function RAW (every @PostProc line stripped; @CompileFlags,
    @CompileEnv hooks and the ORIGINAL header's PIC mode kept) and FINAL (the
    build's own chain, sliced to the window);
  * reaching definitions over the CFG (a call's caller-saved clobber applied
    AFTER its delay slot), then a Weisfeiler-Lehman hash of every instruction
    as (normalised operation, producer hashes per operand);
  * compare the multisets.  Register-blind and schedule-blind; it sees a
    changed producer, a changed operation, an inserted word and a deleted word.

2026-10-03 hardening (red-team findings; see the "Instruction model" block
for the detail).  Every one of these let a dishonest object measure SAME:
  * entry values are register-tagged (IN:a0 != IN:a1; callee-saved share
    IN:sv), clobbers are register-tagged, `jal`/`jalr` read their argument
    registers (callee arity from the C, 5th+ arguments from their outgoing
    stack slots) and a `jr ra` reads $sp and the callee-saved registers
    after its delay slot;
  * load/store offsets and arithmetic immediates are in the key; only $sp
    frame offsets (direct, or through a base computed from $sp) and the
    frame size are dropped -- what patch_stack_complete / remap_stack_slots
    rewrite -- and $sp slots are memory locations whose reaching store is a
    producer, so a remap that moves ONE store of a pair is a change;
  * `ori` and `addiu` differ unless provably equal (li by value; K < 0x8000
    on a lui); relocation symbols are part of a word's identity (%hi by its
    linked value);
  * control dependence replaces "the branch target": every store, call and
    branch carries the (branch, edge) set it is control-dependent on, so a
    changed displacement that makes a store conditional/unconditional, or a
    word moved into a delay slot, is a change wherever in the chain it
    happened (adjust_branch_offset, a custom_script, a normalizer);
  * memory: a non-$sp load reads the reaching may-alias stores and calls
    (different symbols, or disjoint ranges of one base, do not alias;
    pointer provenance follows %hi/%lo, addiu and base+index);
  * the verdict is the 3-round multiset (r3): one round identifies a
    producer only by its own operation, so two identical-key producers were
    interchangeable (swapped `dy>>16` / `dx>>16` call arguments).

Dead argument-home stores (the one sanctioned `nop_at_offset` use)
--------------------------------------------------------------------
A RAW word `sw $aN, frame+4N($sp)` whose value is the incoming argument, in a
function that never loads that slot and never takes `$sp`'s address, is a dead
store to the caller's home area.  Deleting it changes nothing observable, so
it is ignored on the RAW side (never on the FINAL side: a store that appears
only after PostProc is still flagged).  `--no-arg-home-exemption` restores the
audit's exact normalisation.

scheduler_normalizer branch-displacement check
----------------------------------------------
`tools/postproc/scheduler_normalizer.py` (the "Fix branch offsets using target
binary (authoritative)" loop) copies every branch immediate from RETAIL when
opcode and registers agree, and only falls back to its own permutation when
they do not.  For every function carrying the step, this gate rebuilds the
object immediately before and after the step and checks that each branch still
lands on the same instruction it targeted before the permutation (multiset of
(branch, target word) pairs).  A branch that now lands elsewhere took retail's
displacement, not the permutation's.

The gate
--------
`config/plain_vs_shipped_baseline.json` lists every function that differs
today (the remediation worklist) plus the unmeasurable ones and the current
scheduler_normalizer offenders.  The gate FAILS only on:
  * a function that differs and is not in the baseline (NEW),
  * a baseline function whose raw-only or final-only count GREW,
  * a function that newly cannot be measured,
  * a new scheduler_normalizer branch-copy offender, or a count that grew.
A baseline function that now measures `same` is reported as FIXED (prune it
with --write-baseline on a full run).

Usage:
    python3 tools/analysis/audit_plain_vs_shipped.py                  # full gate
    python3 tools/analysis/audit_plain_vs_shipped.py --overlay DATE
    python3 tools/analysis/audit_plain_vs_shipped.py --only DATE/func_80137B98
    python3 tools/analysis/audit_plain_vs_shipped.py --only step34_load --show
    python3 tools/analysis/audit_plain_vs_shipped.py --main-only --json out.json
    python3 tools/analysis/audit_plain_vs_shipped.py --self-test
    python3 tools/analysis/audit_plain_vs_shipped.py --write-baseline # full run only

Exit status: 0 gate passes, 1 gate fails, 2 self-test / usage failure.
"""
import argparse
import collections
import contextlib
import hashlib
import io
import json
import os
import re
import shutil
import struct
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
for _d in ("tools/overlay", "tools/build", "tools/analysis"):
    _p = str(ROOT / _d)
    if _p not in sys.path:
        sys.path.insert(0, _p)

BASELINE = ROOT / "config" / "plain_vs_shipped_baseline.json"
REGISTRY = ROOT / "config" / "level3_functions.json"
OVL_STATUS = ROOT / "config" / "overlay_match_status.json"

# ---------------------------------------------------------------------------
# Instruction model.
#
# 2026-10-03 HARDENING (red-team findings; every one of these let a
# dishonest object measure `same`):
#   * entry values are REGISTER-TAGGED (`IN:a0`, `IN:a1`, `IN:sp`, `IN:ra`;
#     callee-saved registers share `IN:sv`, since their entry value is only
#     saved and restored and an s-register permutation re-pairs the saves);
#     an untagged 'IN' made "reads $a0" equal "reads $a1" (red-team M1: a
#     custom_script swapping two stores' a0/a1 fields measured SAME);
#   * caller-saved registers clobbered by a call are tagged by register
#     (`CLOB:t6` != `CLOB:t7`);
#   * `jal`/`jalr` READ their argument registers (after the delay slot): up
#     to the callee's arity as the C declares/calls it (relocation name ->
#     prototype, the overlay iso audit's call_arities), else every $aN with
#     an in-function definition; 5th+ arguments are read from their
#     outgoing stack slots.  Swapped call arguments were invisible;
#   * load/store offsets and arithmetic immediates are part of the key;
#     only $sp-relative offsets and the frame size are dropped -- those are
#     what patch_stack_complete / remap_stack_slots legitimately rewrite --
#     and $sp slots are modelled as memory locations instead: a $sp load's
#     producer is the reaching $sp store to the same offset (or the incoming
#     stack argument at off-frame), so a remap that moves ONE store of a
#     pair (remap_stack_slots --at) changes the dataflow, while a consistent
#     remap or frame resize is a renaming;
#   * `ori` and `addiu` are different operations, except where they provably
#     compute the same value: from $zero they are both `li` (compared by the
#     VALUE they load), and `ori rX,rY,K` with K < 0x8000 on a `lui` is
#     `addiu` (the low half of a lui is zero);
#   * relocations are part of a word's identity (symbol + type): a `jal` to
#     a different function, or a %lo of a different global, differs;
#   * a branch's TARGET is an operand: the WL hash of the basic block it
#     lands on, so a changed displacement (adjust_branch_offset copying a
#     retail displacement, a custom_script, a normalizer) is a change
#     wherever it happens in the chain;
#   * memory: every non-$sp load also reads the reaching may-alias stores
#     and calls (a store to the exact same location kills the older one);
#     two accesses do not alias when they name different symbols (%lo
#     relocations, or a base register formed from one) or the same base
#     with disjoint byte ranges.  A load moved across an aliasing store, or
#     two aliasing stores exchanged, is a change.
# ---------------------------------------------------------------------------

_RNAME = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
          't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
          's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
          't8', 't9', 'k0', 'k1', 'gp', 'sp', 's8', 'ra']
_CALLEE_SAVED = {16, 17, 18, 19, 20, 21, 22, 23, 28, 30}
_ARGS = (4, 5, 6, 7)
_R_MIPS_26, _R_MIPS_HI16, _R_MIPS_LO16 = 4, 5, 6
_WIDTH = {32: 1, 36: 1, 40: 1, 33: 2, 37: 2, 41: 2, 35: 4, 43: 4, 34: 4, 38: 4,
          42: 4, 46: 4, 48: 4, 49: 4, 56: 4, 57: 4, 58: 4, 59: 4}


def _sx16(x):
    x &= 0xFFFF
    return x - 0x10000 if x & 0x8000 else x


def analyze(w):
    """(kind, defs, uses, key) for one MIPS I word."""
    op = w >> 26
    rs = (w >> 21) & 31
    rt = (w >> 16) & 31
    rd = (w >> 11) & 31
    sa = (w >> 6) & 31
    fn = w & 63
    imm = w & 0xFFFF
    if op == 0:
        if fn in (0x00, 0x02, 0x03):
            return ('r', [rd], [rt], ('R', fn, sa))
        if fn in (0x04, 0x06, 0x07):
            return ('r', [rd], [rt, rs], ('R', fn, 0))
        if fn in (0x0c, 0x0d):          # syscall/break: a code, not registers
            return ('r', [], [], ('R', fn, (w >> 6) & 0xFFFFF))
        if fn == 0x08:
            return ('jr', [], [rs], ('JR',))
        if fn == 0x09:
            return ('jalr', [rd], [rs], ('JALR',))
        if fn in (0x18, 0x19, 0x1a, 0x1b):
            return ('r', [], [rs, rt], ('R', fn, 0))
        if fn in (0x10, 0x12):
            return ('r', [rd], [], ('R', fn, 0))
        if fn in (0x11, 0x13):          # mthi/mtlo
            return ('r', [], [rs], ('R', fn, 0))
        return ('r', [rd], [rs, rt], ('R', fn, 0))
    if op == 2:
        return ('j', [], [], ('J',))
    if op == 3:
        return ('jal', [], [], ('JAL',))
    if op in (4, 5):
        if op == 4 and rs == 0 and rt == 0:
            return ('bu', [], [], ('B', op, imm))
        return ('br', [], [rs, rt], ('B', op, imm))
    if op in (1, 6, 7):
        return ('br', [], [rs], ('B', op, rt, imm))
    if op == 15:
        return ('lui', [rt], [], ('LUI', imm))
    if op in (8, 9, 10, 11, 12, 13, 14, 24, 25):
        return ('i', [rt], [rs], ('I', op, imm))
    if op in (32, 33, 34, 35, 36, 37, 38, 39, 48, 49):
        # lwl/lwr come as a full-word PAIR on one register: not a read of rt
        return ('load', [rt], [rs], ('L', op, imm))
    if op in (40, 41, 42, 43, 46, 56, 57, 58, 59):
        return ('store', [], [rs, rt], ('S', op, imm))
    return ('?', [], [], ('?', op, w))


_COMMUTATIVE_FN = (0x20, 0x21, 0x24, 0x25, 0x26, 0x27, 0x18, 0x19)
_SIDE_EFFECT = ('store', 'jal', 'jalr', 'br', 'jr', 'j')
_RETURN_USES = (29, 16, 17, 18, 19, 20, 21, 22, 23, 30)
_CLOBBER = list(range(1, 16)) + [24, 25]


def _frame_size(words):
    for w in words:
        if (w >> 26) == 9 and ((w >> 21) & 31) == 29 and ((w >> 16) & 31) == 29 \
                and _sx16(w) < 0:
            return -_sx16(w)
    return 0


def _sp_escapes(words):
    """True when $sp is used other than as a load/store base or by the frame
    adjust: its address is taken, so a call or pointer store may write a
    slot."""
    for w in words:
        if w == 0:
            continue
        k, defs, uses, key = analyze(w)
        if 29 not in uses and 29 not in defs:
            continue
        op = w >> 26
        rs, rt = (w >> 21) & 31, (w >> 16) & 31
        if k in ('load', 'store') and rs == 29 and rt != 29:
            continue
        if op == 9 and rs == 29 and rt == 29:
            continue
        if k == 'jr' or (k == 'load' and rt == 29):
            continue
        return True
    return False


class Model:
    """Per-object dataflow facts: keys[i] = (normalised key, operand producer
    sets), tgt[i] = branch target index, blocks = [(start, end)]."""
    __slots__ = ('keys', 'tgt', 'blocks', 'blk_at', 'arity_used', 'cd')


def _merge_state(dst, src):
    for r, v in src.items():
        o = dst.get(r)
        dst[r] = v if o is None else (o | v)


def build(words, rel=None, arity=None, jt=None):
    """Reaching-definition model of one function body.

    rel   = {word index: (reloc type, symbol)} (None: no relocations known)
    arity = {call word index: register-argument count from the C} (None or a
            missing index: inferred from the dataflow)
    jt    = jump-table case-body word indices (None: blocks with no
            predecessor inherit the state of every register-jump block)."""
    rel = rel or {}
    arity = arity or {}
    n = len(words)
    an = [analyze(w) for w in words]
    frame = _frame_size(words)
    escapes = _sp_escapes(words)
    leaders = {0}
    for i, w in enumerate(words):
        k = an[i][0]
        if k in ('br', 'bu'):
            t = i + 1 + _sx16(w)
            if 0 <= t < n:
                leaders.add(t)
            if i + 2 < n:
                leaders.add(i + 2)
        if k in ('j', 'jr') and i + 2 < n:
            leaders.add(i + 2)
    for t in (jt or ()):
        if 0 <= t < n:
            leaders.add(t)
    leaders = sorted(leaders)
    blocks = []
    blkof = {}
    for bi, s in enumerate(leaders):
        e = leaders[bi + 1] if bi + 1 < len(leaders) else n
        blocks.append((s, e))
        for i in range(s, e):
            blkof[i] = bi
    preds = {b: set() for b in range(len(leaders))}
    leader_set = set(leaders)
    dslot_of = {}
    for i in range(n - 1):
        if an[i][0] in ('br', 'bu', 'j', 'jr') and (i + 1) in leader_set:
            dslot_of[i] = i + 1
    dslot_of = {v: k for k, v in dslot_of.items()}   # delay index -> branch
    tgt = {}
    for i, w in enumerate(words):
        k = an[i][0]
        b = blkof[i]
        if k in ('bu', 'br'):
            t = i + 1 + _sx16(w)
            tgt[i] = t
            if t in blkof:
                preds[blkof[t]].add(b)
            if k == 'br' and i + 2 in blkof and blkof[i + 2] != b:
                preds[blkof[i + 2]].add(b)
        elif k in ('j', 'jr', 'jal', 'jalr'):
            pass
        elif i + 1 in blkof and blkof[i + 1] != b:
            prevk = an[i - 1][0] if i > 0 else None
            if prevk not in ('bu', 'j', 'jr') or (i - 1) in dslot_of:
                preds[blkof[i + 1]].add(b)
    # Jump-table case bodies: entered from the dispatch `jr rX` (rX != ra).
    jr_blocks = [blkof[i] for i, w in enumerate(words)
                 if an[i][0] == 'jr' and ((w >> 21) & 31) != 31]
    unk = set()
    if jt:
        for t in jt:
            if t in blkof and t > 0:
                preds[blkof[t]] |= set(jr_blocks)
    for b in range(1, len(leaders)):
        if not preds[b]:
            # with the object's own jump table known, only the blocks it
            # names inherit the dispatch state (dead words after an
            # unconditional branch must not: EN_NICHI/func_80133B70); without
            # one, every predecessor-less block is taken to be a case body
            if jr_blocks and not jt:
                preds[b] = set(jr_blocks)
            else:
                unk.add(b)                 # unreachable: make no claim

    # memory-location descriptor of a load/store, given its base producers
    def _loc(i, base_set):
        w = words[i]
        off = _sx16(w)
        width = _WIDTH.get(w >> 26, 4)
        r = rel.get(i)
        if r and r[0] == _R_MIPS_LO16:
            return ('SYM', r[1], off, width)
        if len(base_set) == 1:
            p = next(iter(base_set))
            if isinstance(p, int):
                pr = rel.get(p)
                pw = words[p]
                if pr and pr[0] == _R_MIPS_LO16 and (pw >> 26) in (9, 13):
                    return ('SYM', pr[1], _sx16(pw) + off, width)
        return ('BASE', base_set, off, width)

    # Pointer PROVENANCE: the symbol (or $sp) a base register's value is
    # derived from through `lui %hi` / `addiu %lo` / constant `addiu` steps,
    # across loop back edges (a struct copy's walking src/dst pointers).
    # Two accesses whose bases derive from different roots cannot alias.
    prov_memo = {}
    cur_ops = {}

    def _prov(p, depth=0):
        if not isinstance(p, int):
            if isinstance(p, tuple) and p[0] == 'IN' and p[1] == 29:
                return frozenset(['SP'])
            return None
        if p in prov_memo:
            return prov_memo[p]
        prov_memo[p] = frozenset()          # cycle guard: contributes nothing
        w = words[p]
        op = w >> 26
        rr = rel.get(p)
        out = None
        if rr and rr[0] in (_R_MIPS_HI16, _R_MIPS_LO16) and op in (15, 9, 13):
            out = frozenset([('SYM', rr[1])])
        elif op == 9 and depth < 40:
            if ((w >> 21) & 31) == 29:
                out = frozenset(['SP'])
            else:
                srcs = cur_ops.get(p)
                if srcs:
                    acc = set()
                    for q in srcs[0]:
                        pq = _prov(q, depth + 1)
                        if pq is None:
                            acc = None
                            break
                        acc |= pq
                    out = frozenset(acc) if acc is not None else None
        elif op == 0 and (w & 63) in (0x21, 0x20) and depth < 40 and \
                ((w >> 21) & 31) != 0 and ((w >> 16) & 31) != 0:
            # base + index: a pointer derived from whichever operand is one
            srcs = cur_ops.get(p)
            if srcs and len(srcs) == 2:
                pv = []
                for o in srcs:
                    acc = set()
                    for q in o:
                        pq = _prov(q, depth + 1)
                        if pq is None:
                            acc = None
                            break
                        acc |= pq
                    pv.append(frozenset(acc) if acc is not None else None)
                known = [x for x in pv if x]
                if len(known) == 1:
                    out = known[0]
        elif op == 0 and (w & 63) in (0x21, 0x25) and depth < 40 and \
                (((w >> 21) & 31) == 0 or ((w >> 16) & 31) == 0):
            srcs = cur_ops.get(p)
            if srcs:
                acc = set()
                for o in srcs:
                    for q in o:
                        if q == ('ZERO',):
                            continue
                        pq = _prov(q, depth + 1)
                        if pq is None:
                            acc = None
                            break
                        acc |= pq
                    if acc is None:
                        break
                out = frozenset(acc) if acc is not None else None
        prov_memo[p] = out
        return out

    def _set_prov(base_set):
        acc = set()
        for q in base_set:
            pq = _prov(q)
            if pq is None:
                return None
            acc |= pq
        return frozenset(acc) or None

    def _alias(a, b):
        if a is None or b is None:
            return True
        if a[0] == b[0] == 'SYM':
            return a[1] == b[1] and a[2] < b[2] + b[3] and b[2] < a[2] + a[3]
        if a[0] == b[0] == 'BASE' and a[1] == b[1]:
            return a[2] < b[2] + b[3] and b[2] < a[2] + a[3]
        pa = frozenset([('SYM', a[1])]) if a[0] == 'SYM' else _set_prov(a[1])
        pb = frozenset([('SYM', b[1])]) if b[0] == 'SYM' else _set_prov(b[1])
        if pa and pb and not (pa & pb):
            return False
        return True

    keys = [None] * n
    locs = {}
    arity_used = {}
    outs = {b: {} for b in range(len(leaders))}
    visited = set()
    entry_state = {r: frozenset([('IN', r)]) for r in range(1, 32)}
    ins = {b: None for b in range(len(leaders))}
    dextra = {}

    def default(r, b):
        if r == 0:
            return ('ZERO',)
        if b in unk:
            return ('UNK',)
        return ('IN', r)

    def opnd(cur, r, b):
        if r == 0:
            return frozenset([('ZERO',)])
        return cur.get(r) or frozenset([default(r, b)])

    def call_args(cur, i, b):
        """Argument operands a call at i receives (state after its delay slot)."""
        na = arity.get(i)
        if na is None:
            # unknown callee (a function pointer, a variadic, no relocation):
            # no argument claim -- a dataflow guess differs between two
            # legitimately renamed objects (a temp painted into $a1)
            na = 0
        arity_used[i] = na
        out = [opnd(cur, r, b) for r in _ARGS[:min(na, 4)]]
        for k in range(4, na):
            off = 16 + 4 * (k - 4)
            out.append(cur.get(('S', off)) or frozenset([('SLOTU',)]))
        return out

    for _ in range(64):
        changed = False
        prov_memo.clear()
        for bi, (s, e) in enumerate(blocks):
            # Every path carries an explicit value for every register (the
            # entry state names them all), so a join where ONE path still
            # holds the incoming value keeps it: a branch retargeted past the
            # epilogue's `lw ra` used to read only the restore (A5b control
            # TT/func_80137A80).  A $sp slot one visited path never stored
            # contributes its default too.
            st = dict(entry_state) if bi == 0 else {}
            vis = [p for p in preds[bi] if p in visited]
            for p in vis:
                _merge_state(st, outs[p])
            for key_ in [k_ for k_ in st if isinstance(k_, tuple) and k_[0] == 'S']:
                if any(key_ not in outs[p] for p in vis) or (bi == 0 and vis):
                    off_ = key_[1]
                    st[key_] = st[key_] | frozenset([('SLOTIN', off_ - frame)
                                                     if off_ >= frame else ('SLOTU',)])
            if st != ins[bi]:
                ins[bi] = st
                changed = True
            cur = dict(st)
            pending = None
            ret_pending = None
            for i in range(s, e):
                k, defs, uses, key = an[i]
                w = words[i]
                op = w >> 26
                ex = dextra.get(i) if i == s else None
                src = []
                for j, r in enumerate(uses):
                    v = opnd(cur, r, bi)
                    if ex and ex[j] is not None:
                        v = v | ex[j]
                    src.append(v)
                if k in ('load', 'store'):
                    rs = (w >> 21) & 31
                    if rs == 29:
                        off = _sx16(w)
                        if k == 'load':
                            sv = cur.get(('S', off))
                            if not sv:
                                sv = frozenset([('SLOTIN', off - frame)
                                                if off >= frame else ('SLOTU',)])
                            src.append(sv)
                            if escapes:
                                # only stores that may point into the frame
                                L = ('BASE', frozenset([('IN', 29)]), off,
                                     _WIDTH.get(op, 4))
                                m = frozenset(x for x in cur.get('MEM', ())
                                              if _alias(locs.get(x) if isinstance(x, int)
                                                        else None, L))
                                src.append(m or frozenset([('MEMIN',)]))
                    else:
                        L = _loc(i, src[0])
                        locs[i] = L
                        if k == 'load':
                            m = frozenset(x for x in cur.get('MEM', ())
                                          if _alias(locs.get(x) if isinstance(x, int)
                                                    else None, L))
                            src.append(m or frozenset([('MEMIN',)]))
                spb = k in ('load', 'store') and ((w >> 21) & 31) != 29 and \
                    _set_prov(src[0]) == frozenset(['SP'])
                nk = _nkey(i, w, key, src, words, rel, spb)
                keys[i] = [nk, src]
                cur_ops[i] = src
                if k == 'store':
                    rs = (w >> 21) & 31
                    if rs == 29:
                        cur[('S', _sx16(w))] = frozenset([i])
                    else:
                        L = locs[i]
                        old = cur.get('MEM', frozenset())
                        cur['MEM'] = frozenset(
                            x for x in old
                            if not (isinstance(x, int) and locs.get(x) == L)) | {i}
                for r in defs:
                    if r != 0:
                        cur[r] = frozenset([i])
                if ret_pending is not None:
                    keys[ret_pending][1] = keys[ret_pending][1] + \
                        [opnd(cur, r_, bi) for r_ in _RETURN_USES]
                    ret_pending = None
                if pending is not None:
                    keys[pending][1] = keys[pending][1] + call_args(cur, pending, bi)
                    for r in _CLOBBER:
                        cur[r] = frozenset([('CALL', pending, r)])
                    cur['MEM'] = frozenset(cur.get('MEM', frozenset())) | {('C', pending)}
                    if escapes:
                        for key_ in [x for x in cur if isinstance(x, tuple) and x[0] == 'S']:
                            cur[key_] = frozenset([('C', pending)])
                    pending = None
                if k in ('jal', 'jalr'):
                    pending = i
                if k == 'jr' and ((w >> 21) & 31) == 31:
                    # a RETURN hands the caller $sp and the callee-saved
                    # registers, read after the delay slot (the epilogue's
                    # `addiu sp,sp,N` usually sits there): a retargeted
                    # branch that skips it, or an s-register restore,
                    # changes them (red-team A5b control TT/func_80137A80)
                    ret_pending = i
            if ret_pending is not None:
                cur2 = dict(cur)
                if ret_pending + 1 < n:
                    for r in an[ret_pending + 1][1]:
                        if r:
                            cur2[r] = frozenset([ret_pending + 1])
                    w2 = words[ret_pending + 1]
                    if (w2 >> 26) == 9 and ((w2 >> 21) & 31) == 29 and ((w2 >> 16) & 31) == 29:
                        cur2[29] = frozenset([ret_pending + 1])
                keys[ret_pending][1] = keys[ret_pending][1] + \
                    [opnd(cur2, r_, bi) for r_ in _RETURN_USES]
            if pending is not None:
                # delay slot starts another block: read the args here, with
                # the delay slot's own definitions applied
                cur2 = dict(cur)
                if pending + 1 < n:
                    for r in an[pending + 1][1]:
                        if r:
                            cur2[r] = frozenset([pending + 1])
                keys[pending][1] = keys[pending][1] + call_args(cur2, pending, bi)
            d = e
            if d < n and dslot_of.get(d) == e - 1:
                k2, defs2, uses2, key2 = an[d]
                ops = tuple(frozenset(cur.get(r) or [default(r, bi)]) if r != 0 else None
                            for r in uses2)
                if dextra.get(d) != ops:
                    dextra[d] = ops
                    changed = True
                for r in defs2:
                    if r != 0:
                        cur[r] = frozenset([d])
            visited.add(bi)
            if cur != outs[bi]:
                outs[bi] = cur
                changed = True
        if not changed:
            break
    m = Model()
    m.cd = _control_dependence(words, an, blocks, blkof, preds, tgt, jt, jr_blocks)
    m.keys = [None if x is None else (x[0], tuple(x[1])) for x in keys]
    m.tgt = tgt
    m.blocks = blocks
    m.blk_at = {s: (s, e) for s, e in blocks}
    m.arity_used = arity_used
    return m


def _control_dependence(words, an, blocks, blkof, preds, tgt, jt, jr_blocks):
    """{word index: frozenset((branch word index, edge))} for every word in a
    block that is control-dependent on a conditional branch (Ferrante et al.:
    X is control-dependent on edge A->B when X post-dominates B but does not
    strictly post-dominate A).  Edge 'T' = taken, 'F' = fall-through, 'C' =
    a jump-table case.  This is what a branch's TARGET means semantically:
    a changed displacement that makes a store or a call conditional (or
    unconditional), or re-guards it under another branch, changes the set;
    a legitimate hoist/sink of code out of a join block does not."""
    n = len(words)
    nb = len(blocks)
    succ = {b: [] for b in range(nb)}
    for b, (s, e) in enumerate(blocks):
        last = e - 1
        # the transfer is the last or second-to-last word (delay slot)
        tr = None
        for c in (last - 1, last):
            if s <= c < e and an[c][0] in ('br', 'bu', 'j', 'jr'):
                tr = c
        if tr is None:
            if e < n:
                succ[b].append((blkof[e], 'N'))
            continue
        k = an[tr][0]
        if k in ('br', 'bu'):
            t = tgt.get(tr)
            if t is not None and t in blkof:
                succ[b].append((blkof[t], 'T' if k == 'br' else 'N'))
            if k == 'br' and tr + 2 < n:
                succ[b].append((blkof[tr + 2], 'F'))
        elif k == 'jr' and ((words[tr] >> 21) & 31) != 31:
            for x in (jt or []):
                if x in blkof:
                    succ[b].append((blkof[x], 'C'))
        # j / jr ra: exits
    EXIT = nb
    allset = frozenset(range(nb + 1))
    pdom = {b: allset for b in range(nb)}
    pdom[EXIT] = frozenset([EXIT])
    for _ in range(200):
        ch = False
        for b in reversed(range(nb)):
            ss = [x for x, _l in succ[b]] or [EXIT]
            acc = None
            for x in ss:
                acc = pdom[x] if acc is None else (acc & pdom[x])
            new = frozenset([b]) | acc
            if new != pdom[b]:
                pdom[b] = new
                ch = True
        if not ch:
            break
    cdb = {b: set() for b in range(nb)}
    for a in range(nb):
        labels = {l for _x, l in succ[a]}
        if len(succ[a]) < 2 and 'C' not in labels:
            continue
        s, e = blocks[a]
        tr = None
        for c in (e - 2, e - 1):
            if s <= c < e and an[c][0] in ('br', 'jr'):
                tr = c
        if tr is None:
            continue
        spdom_a = pdom[a] - {a}
        for bb, lbl in succ[a]:
            for x in pdom[bb]:
                if x != EXIT and x not in spdom_a:
                    cdb[x].add((tr, lbl))
    out = {}
    for b, (s, e) in enumerate(blocks):
        if cdb[b]:
            f = frozenset(cdb[b])
            for i in range(s, e):
                out[i] = f
    return out


def _single_producer_word(src, words):
    if len(src) == 1:
        p = next(iter(src))
        if isinstance(p, int):
            return words[p]
    return None


def _nkey(i, w, key, src, words, rel, sp_derived=False):
    """Normalised operation key.  Register-blind; keeps every immediate
    except the $sp-relative ones and the frame size (see the header).
    `sp_derived`: a load/store whose base is computed from $sp (an indexed
    local array, `addu t,sp,i; lh K(t)`): patch_stack_complete rewrites K
    with the frame (DATE/func_801560C8), so it is a frame offset too."""
    if w == 0:
        return None
    op = w >> 26
    fn = w & 63
    r = rel.get(i)
    r = (r[0], r[1]) if r else None
    rs = (w >> 21) & 31
    rt = (w >> 16) & 31
    if op == 0 and fn in (0x21, 0x25, 0x20) and (rs == 0 or rt == 0):
        return ('MOVE',)
    k0 = key[0]
    if k0 == 'B':
        return ('B',) + key[1:-1]                   # target is an operand
    if k0 in ('J', 'JAL'):
        return (k0, r)
    if k0 == 'LUI':
        if r and r[0] == _R_MIPS_HI16:
            # identified by the %hi VALUE it links to when the symbol can be
            # placed: two symbols sharing a %hi are one lui after linking
            # (the iso audit's same-%hi re-pairing, value-equivalent)
            a = _name_addr(r[1])
            if a is not None:
                return ('LUI', 'HI', ((a + (key[1] << 16) + 0x8000) >> 16) & 0xFFFF)
        return ('LUI', key[1], r)
    if k0 == 'I':
        imm = w & 0xFFFF
        if rs == 29:
            if rt == 29:
                return ('FRAME', _sx16(w) < 0)      # patch_stack rewrites it
            return ('SPADDR', op)                   # &local: slot renamed
        if op in (9, 13, 8) and rs == 0 and r is None:
            return ('LI', _sx16(w) if op != 13 else imm)
        if op in (9, 13) and imm < 0x8000:
            pw = _single_producer_word(src[0], words) if src else None
            if pw is not None and (pw >> 26) == 15:
                return ('I', 9, imm, r)             # ori on a lui == addiu
        return ('I', op, imm, r)
    if k0 in ('L', 'S'):
        if rs == 29 or sp_derived:
            return (k0, op, 'SP')
        return (k0, op, w & 0xFFFF, r)
    return key


_NAME_ADDR = {}
_CUR_OV = [None]        # the overlay being measured: its landed names resolve first


def _name_addr(sym):
    key = (sym, _CUR_OV[0])
    if key not in _NAME_ADDR:
        a = None
        try:
            from resolve_overlay_relocs import name_to_addr
            a = name_to_addr(sym, _CUR_OV[0])
        except Exception:                            # noqa: BLE001
            a = None
        _NAME_ADDR[key] = a
    return _NAME_ADDR[key]


def _H(*a):
    return hashlib.md5(repr(a).encode()).hexdigest()[:12]


def _tag(x, hp):
    if isinstance(x, int):
        return hp[x]
    t = x[0]
    if t == 'IN':
        r = x[1]
        return 'IN:sv' if r in _CALLEE_SAVED else 'IN:' + _RNAME[r]
    if t == 'ZERO':
        return 'Z'
    if t == 'CALL':
        return ('RET', hp[x[1]], x[2]) if x[2] in (2, 3) else 'CLOB:' + _RNAME[x[2]]
    if t == 'C':
        return ('MC', hp[x[1]])
    if t == 'SLOTIN':
        return 'SIN:%d' % x[1]
    return t                                         # UNK / SLOTU / MEMIN


def dead_arg_home_stores(words, model):
    """RAW indices of provably dead argument-home stores (see module doc)."""
    keys = model.keys
    frame = _frame_size(words)
    slots_read = set()
    for w in words:
        if w == 0:
            continue
        k, defs, uses, key = analyze(w)
        if 29 not in uses and 29 not in defs:
            continue
        op = w >> 26
        rs = (w >> 21) & 31
        rt = (w >> 16) & 31
        if k == 'load' and rs == 29 and rt != 29:
            slots_read.add(_sx16(w))
            continue
        if k == 'store' and rs == 29 and rt != 29:
            continue
        if op == 9 and rs == 29 and rt == 29:
            continue                                  # prologue / epilogue
        if k == 'jr':
            continue
        return set()                                  # $sp escapes: exempt nothing
    out = set()
    for i, w in enumerate(words):
        if (w >> 26) != 0x2b or ((w >> 21) & 31) != 29:
            continue
        rt = (w >> 16) & 31
        if not 4 <= rt <= 7:
            continue
        off = _sx16(w)
        if off != frame + 4 * (rt - 4) or off in slots_read:
            continue
        if keys[i] is None or keys[i][1][1] != frozenset([('IN', rt)]):
            continue
        out.add(i)
    return out


def wl(words, rounds=1, exclude=(), per_index=False, model=None, rel=None,
       arity=None, jt=None):
    m = model or build(words, rel, arity, jt)
    keys = m.keys
    n = len(words)
    h = [_H(keys[i][0] if keys[i] else None) for i in range(n)]
    live_set = {i for i in range(n) if words[i] != 0 and keys[i] is not None
                and keys[i][0] is not None}

    for _ in range(rounds):
        hp = h[:]
        nh = []
        for i in range(n):
            if keys[i] is None:
                nh.append(hp[i])
                continue
            key, src = keys[i]
            if key == ('MOVE',):
                ops = [o for o in src if o != frozenset([('ZERO',)])]
                if len(ops) == 1:
                    v = sorted(str(_tag(x, hp)) for x in ops[0])
                    nh.append(v[0] if len(v) == 1 else _H('MV', tuple(v)))
                    continue
            s = [tuple(sorted(str(_tag(x, hp)) for x in operand)) for operand in src]
            w = words[i]
            op = w >> 26
            if op == 0 and (w & 0x3f) in _COMMUTATIVE_FN:
                s = sorted(map(str, s))
            elif op in (4, 5):
                s = sorted(map(str, s))
            if i in m.cd and analyze(w)[0] in _SIDE_EFFECT:
                s = list(s) + [('CD', tuple(sorted((hp[b], l) for b, l in m.cd[i])))]
            nh.append(_H(hp[i], tuple(map(str, s))))
        h = nh
    live = [i for i in range(n) if i in live_set and i not in exclude]
    if per_index:
        return {i: h[i] for i in live}
    return collections.Counter(h[i] for i in live)


EXPLAIN = False

# The verdict criterion.  2026-10-03: r3, not r1.  After one WL round a
# producer is identified only by its OWN operation key, so two values made by
# identical-key instructions are interchangeable: call arguments `dy >> 16` /
# `dx >> 16` exchanged (red-team A2 on TT/func_801320F0), or a reload that now
# reads the OTHER of two `sw ...($sp)` stores (A3) measured `same`.  Three
# rounds see three producer levels; over the whole tree r3 flags 6 functions
# r1 does not (2026-10-03).
VERDICT = 'r3'


def _disasm(w):
    try:
        import capstone
        md = getattr(_disasm, 'md', None)
        if md is None:
            md = _disasm.md = capstone.Cs(capstone.CS_ARCH_MIPS,
                                          capstone.CS_MODE_MIPS32 + capstone.CS_MODE_LITTLE_ENDIAN)
        for x in md.disasm(struct.pack('<I', w), 0):
            return f'{x.mnemonic} {x.op_str}'
    except Exception:                                # noqa: BLE001
        pass
    return '%08x' % w


def _explain(words, m, idx, rel):
    """Human-readable lines for the differing words: disassembly, the
    normalised key and each operand's producers (round-1 tags)."""
    out = []
    hp = [_H(k[0] if k else None) for k in m.keys]

    def pt(x):
        if isinstance(x, int):
            return '@%d(%s)' % (x, _disasm(words[x]).split(' ')[0])
        t = _tag(x, hp)
        return t if isinstance(t, str) else str(t[0]) + '@%d' % x[1]
    for i in idx:
        k = m.keys[i]
        ops = ' | '.join(','.join(sorted(pt(x) for x in o)) for o in (k[1] if k else ()))
        tg = (' -> @%d' % m.tgt[i]) if i in m.tgt else ''
        if i in m.cd and analyze(words[i])[0] in _SIDE_EFFECT:
            tg += ' cd{%s}' % ','.join('@%d%s' % (b, l) for b, l in sorted(m.cd[i]))
        out.append('  %4d %08x %-28s %s%s  ops[%s]%s' % (
            i, words[i], _disasm(words[i]), k[0] if k else None,
            (' rel=%s' % (rel.get(i),)) if rel and rel.get(i) else '', ops, tg))
    return out


def compare_words(rw, fw, arg_home_exemption=True, rrel=None, frel=None,
                  rarity=None, farity=None, rjt=None, fjt=None):
    """Return dict(r1=[raw_only, final_only], r3=[...], raw_real, fin_real).

    rrel/frel: relocations of the RAW / FINAL words ({index: (type, sym)});
    rarity/farity: call arities from the C; rjt/fjt: jump-table targets.

    With the exemption, a RAW-only word that is a provably dead argument-home
    store is forgiven -- only when it is actually missing from FINAL, so an
    exemption can never create a difference or hide an inserted word."""
    rm = build(rw, rrel, rarity, rjt)
    fm = build(fw, frel, farity, fjt)
    dead = dead_arg_home_stores(rw, rm) if arg_home_exemption else set()
    out = {}
    forgiven = 0
    for r in (1, 3):
        ha = wl(rw, r, per_index=True, model=rm)
        b = wl(fw, r, model=fm)
        a = collections.Counter(ha.values())
        raw_only = a - b
        fin_only = b - a
        if dead:
            pool = collections.Counter(ha[i] for i in dead if i in ha)
            f = raw_only & pool
            raw_only = raw_only - f
            if r == 1:
                forgiven = sum(f.values())
                out['r1_no_exemption'] = [sum((a - b).values()), sum(fin_only.values())]
        out['r%d' % r] = [sum(raw_only.values()), sum(fin_only.values())]
        if r == int(VERDICT[1:]) and (raw_only or fin_only):
            hb = wl(fw, r, per_index=True, model=fm)
            out['raw_only_idx'] = sorted(i for i, x in ha.items() if x in raw_only)[:12]
            out['fin_only_idx'] = sorted(i for i, x in hb.items() if x in fin_only)[:12]
            if EXPLAIN:
                out['explain'] = (_explain(rw, rm, out['raw_only_idx'], rrel) +
                                  ['  ----'] +
                                  _explain(fw, fm, out['fin_only_idx'], frel))
    out['raw_real'] = sum(1 for w in rw if w)
    out['fin_real'] = sum(1 for w in fw if w)
    if forgiven:
        out['arg_home_exempt'] = forgiven
    else:
        out.pop('r1_no_exemption', None)
    return out


# ---------------------------------------------------------------------------
# Compiling RAW and FINAL
# ---------------------------------------------------------------------------


def _elf_words(obj, fn=None, limit=None):
    from elftools.elf.elffile import ELFFile
    with open(obj, 'rb') as f:
        e = ELFFile(f)
        d = e.get_section_by_name('.text').data()
        if fn is None:
            return [struct.unpack_from('<I', d, i)[0] for i in range(0, len(d) // 4 * 4, 4)]
        st = None
        for s in e.get_section_by_name('.symtab').iter_symbols():
            if s.name == fn and s['st_info']['type'] == 'STT_FUNC':
                st = (s['st_value'], s['st_size'])
    if st is None or st[1] == 0:
        return None
    v, sz = st
    if limit is not None:
        sz = min(sz, limit)
    return [struct.unpack_from('<I', d, v + i)[0] for i in range(0, sz, 4) if v + i + 4 <= len(d)]



def _obj_facts(obj, fn=None, limit=None, src_text=None, main=False):
    """(words, relocations, jump-table targets, call arities) of one function
    in an object, all as indices into the returned words.  Relocations,
    jump tables and arities are best effort: {} / None when unreadable."""
    import audit_paint_isomorphism as iso
    words = _elf_words(obj, fn, limit)
    if words is None:
        return None, {}, None, {}
    base = 0
    if fn is not None:
        from elftools.elf.elffile import ELFFile
        with open(obj, 'rb') as f:
            for s in ELFFile(f).get_section_by_name('.symtab').iter_symbols():
                if s.name == fn and s['st_info']['type'] == 'STT_FUNC':
                    base = s['st_value'] // 4
    try:
        rel = {k - base: v for k, v in iso.text_relocs(obj).items()
               if base <= k < base + len(words)}
    except Exception:                                # noqa: BLE001
        rel = {}
    jt = None
    try:
        raw_jt = [x - base for x in (iso.jump_targets(obj) or [])]
        if main and raw_jt:
            import audit_main_paint_isomorphism as miso
            jt = miso._case_starts(words, raw_jt) or None
        elif raw_jt:
            jt = sorted(x for x in raw_jt if 0 <= x < len(words)) or None
    except Exception:                                # noqa: BLE001
        jt = None
    arity = {}
    if src_text:
        try:
            arity = call_arities(src_text, rel)
        except Exception:                            # noqa: BLE001
            arity = {}
    return words, rel, jt, arity


def callee_arity(text, name):
    """Register+stack arguments every call of `name` in the C passes: its
    ANSI prototype's parameter count, else (K&R `f();` or no prototype) the
    SMALLEST argument count over its call sites -- the iso audit's
    first-call-site rule over-claims when a K&R callee is called with
    different counts (ETC/func_80146D40 calls func_800B4850 with 5 and 2).
    None for variadic or unknown."""
    import audit_paint_isomorphism as iso
    src = iso._strip_comments(text)
    decl_re = re.compile(r'(?:^|[;{}\n])\s*(?:extern\s+|static\s+)?[A-Za-z_][\w\s\*]*$')
    stmt_re = re.compile(r'\b(return|if|while|switch|else|do|case)\s*$')
    proto = None
    calls = []
    for m in re.finditer(r'\b' + re.escape(name) + r'\s*\(', src):
        k, depth = m.end(), 1
        while k < len(src) and depth:
            if src[k] == '(':
                depth += 1
            elif src[k] == ')':
                depth -= 1
            k += 1
        body = src[m.end():k - 1]
        before = src[max(0, m.start() - 80):m.start()]
        if '...' in body:
            return None
        is_decl = bool(decl_re.search(before)) and not stmt_re.search(before)
        if is_decl:
            b = body.strip()
            if b == 'void':
                proto = 0
            elif b:
                proto = iso._count_args(b)
        else:
            calls.append(iso._count_args(body))
    if proto is not None:
        return proto
    return min(calls) if calls else None


def call_arities(text, rel):
    out, cache = {}, {}
    for i, (typ, name) in rel.items():
        if typ != _R_MIPS_26 or not name:
            continue
        if name not in cache:
            cache[name] = callee_arity(text, name)
        out[i] = cache[name]
    return out


def _compare_objects(raw, fin, fn, eff, src_text, arg_home, main=False):
    rw, rrel, rjt, rar = _obj_facts(raw, fn, None, src_text, main)
    fw, frel, fjt, far = _obj_facts(fin, fn, eff, src_text, main)
    if rw is None or fw is None:
        return None, None, None
    return compare_words(rw, fw, arg_home, rrel, frel, rar, far, rjt, fjt), rw, fw


def _is_sched(step):
    return step.get('type') == 'scheduler_normalizer'


def _mask_branch(w):
    return w & 0xFFFF0000 if (w >> 26) in (1, 4, 5, 6, 7) else w


def _branch_targets(words):
    """{masked branch word: [absolute target index, ...] in position order}."""
    out = collections.defaultdict(list)
    for q, w in enumerate(words):
        if (w >> 26) in (1, 4, 5, 6, 7):
            out[_mask_branch(w)].append(q + 1 + _sx16(w))
    return out


def sched_branch_check(pre, post):
    """Branches whose ABSOLUTE target changed across the step.

    The normalizer only swaps words, and a swap moves no label, so every
    branch must still land on the same index afterwards (branches pair up
    pre/post in position order).  A changed target is the displacement the
    step wrote -- in practice the one it copied from retail (its 'Fix branch
    offsets using target binary (authoritative)' loop)."""
    if len(pre) != len(post) or \
            collections.Counter(map(_mask_branch, pre)) != collections.Counter(map(_mask_branch, post)):
        return {'not_a_permutation': True, 'bad': None}
    a = _branch_targets(pre)
    b = _branch_targets(post)
    bad = sum(1 for k in a for x, y in zip(a[k], b[k]) if x != y)
    return {'bad': bad}


def _overlay_measure(ov, fn, arg_home, sched):
    _CUR_OV[0] = ov
    try:
        return _overlay_measure_scoped(ov, fn, arg_home, sched)
    finally:
        _CUR_OV[0] = None


def _overlay_measure_scoped(ov, fn, arg_home, sched):
    import ido_flags
    import resolve_overlay_relocs as R
    import audit_raw_count as A
    from overlay_postproc import parse_postproc, apply_postproc
    from parse_postproc import parse_compiler
    from parse_postproc import parse_symbol_start_text, parse_symbol_size_text
    src = ROOT / 'src/overlay' / ov / f'{fn}.c'
    content = src.read_text(errors='replace')
    # Window from DIRECTIVE lines only.  The audit's regex scrape read header
    # prose (`... ABSORBED (\`@SymbolStart: 0x80137724\`)`) as the window and
    # could not measure ENDING/func_801375BC; it called that a flake.
    sym_a = sym_s = None
    for ln in (ROOT / 'config/overlay' / f'{ov}.sym').read_text().splitlines():
        p = ln.strip().split(':')
        if len(p) >= 3 and p[2].strip() == fn:
            sym_a, sym_s = int(p[0], 16), int(p[1], 0)
    addr = parse_symbol_start_text(content) or sym_a
    eff = parse_symbol_size_text(content) or sym_s
    td = Path(tempfile.mkdtemp(prefix='pvs_'))
    res = {}
    try:
        fin = R.compile_object(ov, fn, src, addr, td)
        if fin is None:
            return dict(err='final compile/chain failed')
        fin_text = _elf_words(fin)
        rd = td / 'raw'
        rd.mkdir()
        s2 = rd / f'{fn}.c'
        s2.write_text(A.strip_postproc(content))
        extra = []
        if not parse_compiler(src).startswith('gcc'):
            # the PIC mode of the SHIPPED compile.  (Was `pic_flags(...) or
            # [-Wc,-pic0]`, which appended pic0 LAST to files stamped
            # `@CompileFlags: -Wc,-pic1` and compiled them pic0.)
            extra = ido_flags.stripped_pic_extra(content, R.parse_compile_flags(src))
        raw = R.compile_object(ov, fn, s2, addr, rd, extra_cflags=extra)
        if raw is None:
            return dict(err='raw compile failed')
        res, rw, fw = _compare_objects(raw, fin, fn, eff, content, arg_home)
        if res is None:
            return dict(err='symbol missing')
        if sched:
            steps = parse_postproc(src)
            idx = [i for i, s in enumerate(steps) if _is_sched(s)]
            if idx:
                res['sched'] = _overlay_sched(ov, fn, raw, steps, idx, addr, td, fin_text,
                                              apply_postproc)
    finally:
        shutil.rmtree(td, ignore_errors=True)
    return res


def _overlay_sched(ov, fn, raw, steps, idx, addr, td, fin_text, apply_postproc):
    asm = ROOT / 'asm' / 'overlay' / ov / f'{fn}.s'
    asm = asm if asm.exists() else None
    # The object's STEM is the function identity apply_postproc hands to the
    # banned/restricted-step check, so it must be the function's own name.
    (td / 'sched').mkdir()
    cur = td / 'sched' / f'{fn}.o'
    shutil.copy(raw, cur)
    done = 0
    bad = 0
    detail = []
    for k in idx:
        if not apply_postproc(cur, steps[done:k], addr, ov, verbose=False, target_asm=asm):
            return {'err': 'prefix chain failed'}
        pre = _elf_words(cur)
        if not apply_postproc(cur, steps[k:k + 1], addr, ov, verbose=False, target_asm=asm):
            return {'err': 'normalizer step failed'}
        post = _elf_words(cur)
        done = k + 1
        r = sched_branch_check(pre, post)
        if r.get('not_a_permutation'):
            detail.append('not-a-permutation')
            bad += 1
        else:
            bad += r['bad']
    if not apply_postproc(cur, steps[done:], addr, ov, verbose=False, target_asm=asm):
        return {'err': 'suffix chain failed'}
    out = {'bad': bad}
    if _elf_words(cur) != fin_text:
        out['nonrepro'] = True        # split chain != build chain: check void
    if detail:
        out['detail'] = detail
    return out


def _main_compile(e, steps, parsed, keep=None):
    """Compile one registered main function through `steps`; return the
    whole .text as words (and copy the object to `keep` when given)."""
    import build_level3_c as b
    fl, cp, env = parsed
    td = tempfile.mkdtemp(prefix='pvs_')
    o = os.path.join(td, 'k.o')
    saved = dict(os.environ)
    try:
        os.environ.update(env or {})
        os.environ['L3_KEEP_OBJ'] = o
        with contextlib.redirect_stdout(io.StringIO()), \
                contextlib.redirect_stderr(io.StringIO()):
            b.compile_c_function(str(ROOT / e['source']), e['name'], int(e['address'], 16),
                                 postproc_steps=steps, extra_flags=fl, compiler=cp)
        if keep:
            shutil.copy(o, keep)
        return _elf_words(o)
    finally:
        os.environ.clear()
        os.environ.update(saved)
        shutil.rmtree(td, ignore_errors=True)


def _main_measure(e, arg_home, sched):
    from parse_postproc import (parse_postproc_header, parse_compile_flags,
                                parse_compiler, parse_compile_env)
    src = str(ROOT / e['source'])
    with contextlib.redirect_stdout(io.StringIO()):
        pp = parse_postproc_header(src) or []
    parsed = (parse_compile_flags(src), parse_compiler(src), parse_compile_env(src))
    text = Path(src).read_text(errors='replace')
    td = tempfile.mkdtemp(prefix='pvsm_')
    try:
        fo, ro = os.path.join(td, 'final.o'), os.path.join(td, 'raw.o')
        _main_compile(e, pp, parsed, keep=fo)
        _main_compile(e, [s for s in pp if s.get('type') == 'strip_leading_instructions'],
                      parsed, keep=ro)
        rw, rrel, rjt, rar = _obj_facts(ro, None, None, text)
        fw, frel, fjt, far = _obj_facts(fo, None, None, text)
        # strip_leading_instructions leaves the .rodata jump table pointing
        # N words late; the shift is a property of the strip, the same for
        # both objects -- infer it ONCE, on the raw words (the inference
        # reads the words, which the PostProc chain permutes)
        import audit_paint_isomorphism as iso
        # Jump tables: the .rodata entries are .text offsets of the object
        # BEFORE strip_leading_instructions, and no PostProc step rewrites
        # them (a swap moves words, never labels).  Read them from a compile
        # with no step at all, relative to the function symbol there, and use
        # the same case-entry indices for both objects.
        rjt = fjt = None
        if iso.jump_targets(ro):
            fo2 = os.path.join(td, 'full.o')
            _main_compile(e, [], parsed, keep=fo2)
            from elftools.elf.elffile import ELFFile
            F = 0
            with open(fo2, 'rb') as f_:
                for sy in ELFFile(f_).get_section_by_name('.symtab').iter_symbols():
                    if sy.name == e['name'] and sy['st_info']['type'] == 'STT_FUNC':
                        F = sy['st_value'] // 4
            rjt = sorted(set(x - F for x in (iso.jump_targets(fo2) or [])
                             if 0 <= x - F < len(rw))) or None
            # gcc objects can leave the entries a few words off the case
            # bodies (GsSortObject4 by 1, getintr by 4 -- the stripped
            # thunks): a case body starts right after an unconditional
            # transfer's delay slot, so pick the smallest shift that lands
            # the most entries there
            if rjt:
                def _unc(w):
                    op_ = w >> 26
                    return op_ == 2 or (op_ == 4 and not (w >> 16) & 0x3FF) or \
                        (op_ == 0 and (w & 63) == 8)
                best = (sum(1 for x in rjt if x >= 2 and _unc(rw[x - 2])), 0)
                for k_ in sorted(range(-4, 33), key=abs):
                    sc = sum(1 for x in rjt if 0 <= x - k_ - 2 and x - k_ < len(rw)
                             and _unc(rw[x - k_ - 2]))
                    if sc > best[0]:
                        best = (sc, k_)
                rjt = sorted(set(x - best[1] for x in rjt if 0 <= x - best[1] < len(rw)))
            fjt = [x for x in (rjt or []) if x < len(fw)] or None
        n = e['size'] // 4
        fw = fw[:n]
        if len(rw) > n and not any(rw[n:]):
            # trailing alignment padding: judge both objects over one window
            rw = rw[:n]
            rrel = {k: v for k, v in rrel.items() if k < n}
            rjt = [x for x in (rjt or []) if x < n] or None
        frel = {k: v for k, v in frel.items() if k < n}
        fjt = [x for x in (fjt or []) if x < n] or None
        res = compare_words(rw, fw, arg_home, rrel, frel, rar, far, rjt, fjt)
    finally:
        shutil.rmtree(td, ignore_errors=True)
    idx = [i for i, s in enumerate(pp) if _is_sched(s)]
    if sched and idx:
        bad = 0
        for k in idx:
            pre = _main_compile(e, pp[:k], parsed)
            post = _main_compile(e, pp[:k + 1], parsed)
            r = sched_branch_check(pre, post)
            bad += 1 if r.get('not_a_permutation') else r['bad']
        res['sched'] = {'bad': bad}
    return res


# ---------------------------------------------------------------------------
# Enumeration, identity, worker
# ---------------------------------------------------------------------------


def main_entries():
    reg = json.load(open(REGISTRY))['functions']
    return [e for e in reg if e.get('source', '').endswith('.c')]


def overlay_entries(ovs=None):
    d = json.load(open(OVL_STATUS))
    out = []
    for ov in sorted(d):
        if ovs and ov not in ovs:
            continue
        for fn in sorted(d[ov]):
            if d[ov][fn].get('status') in ('byte_perfect', 'reloc_only') and \
                    (ROOT / 'src/overlay' / ov / f'{fn}.c').exists():
                out.append((ov, fn))
    return out


def main_id(e):
    return 'main/0x%08X' % int(e['address'], 16)


def _work(task):
    kind, payload, arg_home, sched = task
    for attempt in range(2):                     # one retry: compile flakes
        try:
            with contextlib.redirect_stderr(io.StringIO()), \
                    contextlib.redirect_stdout(io.StringIO()):
                if kind == 'ov':
                    ov, fn = payload
                    r = _overlay_measure(ov, fn, arg_home, sched)
                    r.update(id=f'{ov}/{fn}', name=fn, region='overlay')
                else:
                    r = _main_measure(payload, arg_home, sched)
                    r.update(id=main_id(payload), name=payload['name'], region='main')
        except BaseException as ex:              # noqa: BLE001 -- incl. SystemExit from a
            # refusing PostProc chain: a dead pool worker hangs the whole run
            r = dict(err=repr(ex)[:200])
            if kind == 'ov':
                r.update(id='%s/%s' % payload, name=payload[1], region='overlay')
            else:
                r.update(id=main_id(payload), name=payload['name'], region='main')
        if 'err' not in r:
            break
    if 'err' in r:
        r['verdict'] = 'unmeasurable'
    else:
        r['verdict'] = 'same' if r[VERDICT] == [0, 0] else 'differ'
    return r


def run(tasks, jobs):
    if jobs <= 1 or len(tasks) <= 1:
        return [_work(t) for t in tasks]
    import multiprocessing as mp
    # No maxtasksperchild: a replacement worker is forked from a parent whose
    # pool threads are running, can inherit a held lock, and hangs (seen).
    with mp.get_context('fork').Pool(jobs) as pool:
        return list(pool.imap(_work, tasks, chunksize=4))


# ---------------------------------------------------------------------------
# Baseline gate
# ---------------------------------------------------------------------------


def load_baseline():
    if not BASELINE.exists():
        return {'differ': {}, 'unmeasurable': {}, 'sched_branch_copy': {}}
    return json.load(open(BASELINE))


def gate(results, base):
    fails, fixed = [], []
    bd = base.get('differ', {})
    bu = base.get('unmeasurable', {})
    bs = base.get('sched_branch_copy', {})
    for r in results:
        i = r['id']
        v = r['verdict']
        if v == 'unmeasurable':
            if i not in bu and i not in bd:
                fails.append((i, 'NEW-UNMEASURABLE', r.get('err')))
        elif v == 'differ':
            if i not in bd:
                fails.append((i, 'NEW-DIFFER', r[VERDICT]))
            else:
                old = bd[i].get(VERDICT, bd[i]['r1'])
                if r[VERDICT][0] > old[0] or r[VERDICT][1] > old[1]:
                    fails.append((i, 'GREW', f"{old} -> {r[VERDICT]}"))
        elif i in bd or i in bu:
            fixed.append(i)
        s = r.get('sched')
        if s and s.get('bad'):
            if s.get('nonrepro'):
                pass
            elif i not in bs:
                fails.append((i, 'NEW-SCHED-BRANCH-COPY', s['bad']))
            elif s['bad'] > bs[i]['bad']:
                fails.append((i, 'SCHED-BRANCH-COPY-GREW', f"{bs[i]['bad']} -> {s['bad']}"))
    return fails, fixed


def write_baseline(results, arg_home):
    import datetime
    differ = {r['id']: {'name': r['name'], 'region': r['region'], 'r1': r['r1'],
                        'r3': r['r3'], 'raw_real': r['raw_real'], 'fin_real': r['fin_real']}
              for r in results if r['verdict'] == 'differ'}
    unm = {r['id']: {'name': r['name'], 'region': r['region'], 'err': r.get('err')}
           for r in results if r['verdict'] == 'unmeasurable'}
    sch = {r['id']: {'name': r['name'], 'region': r['region'], 'bad': r['sched']['bad']}
           for r in results if r.get('sched') and r['sched'].get('bad')
           and not r['sched'].get('nonrepro')}
    doc = {
        'description': (
            'Remediation worklist for tools/analysis/audit_plain_vs_shipped.py. '
            '"differ": functions whose shipped (post-PostProc) object is not a '
            'register-renaming + reschedule of their PostProc-free compile. '
            '"sched_branch_copy": functions whose scheduler_normalizer step took '
            "a branch displacement from retail instead of from its own permutation. "
            'The gate fails only on NEW entries or growth; remove an entry by '
            'fixing the C (or demoting to asm_only) and re-running --write-baseline.'),
        'generated': datetime.date.today().isoformat(),
        'criterion': '%s (WL multiset after %s rounds) != [0,0]' % (VERDICT, VERDICT[1:]),
        'arg_home_exemption': arg_home,
        'counts': {'measured': len(results), 'same': sum(r['verdict'] == 'same' for r in results),
                   'differ': len(differ), 'unmeasurable': len(unm),
                   'sched_branch_copy': len(sch)},
        'differ': dict(sorted(differ.items())),
        'unmeasurable': dict(sorted(unm.items())),
        'sched_branch_copy': dict(sorted(sch.items())),
    }
    BASELINE.write_text(json.dumps(doc, indent=1) + '\n')


# ---------------------------------------------------------------------------
# Self-test: synthetic fixtures + compiled negative controls
# ---------------------------------------------------------------------------

def _enc_r(rs, rt, rd, fn, sa=0):
    return (rs << 21) | (rt << 16) | (rd << 11) | (sa << 6) | fn


def _enc_i(op, rs, rt, imm):
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


def _synthetic():
    """(name, raw, final, expect_same, arg_home_exemption[, extra kwargs]).

    extra kwargs (dict) go to compare_words: rrel/frel relocations,
    rarity/farity call arities."""
    SP, A0, A1, A2, V0, V1, T6, T7, S0, S1, RA, AT = 29, 4, 5, 6, 2, 3, 14, 15, 16, 17, 31, 1
    jr_ra = _enc_r(RA, 0, 0, 0x08)
    jal = 3 << 26
    base = [
        _enc_i(9, SP, SP, -24),                 # addiu sp,sp,-24
        _enc_i(0x23, A0, T6, 4),                # lw t6,4(a0)
        _enc_i(0x23, A0, T7, 8),                # lw t7,8(a0)
        _enc_r(T6, T7, V0, 0x21),               # addu v0,t6,t7
        _enc_i(0x2b, A1, V0, 0),                # sw v0,0(a1)
        _enc_i(9, SP, SP, 24),                  # addiu sp,sp,24
        jr_ra, 0,
    ]
    rename = list(base)
    rename[1] = _enc_i(0x23, A0, V1, 4)
    rename[3] = _enc_r(V1, T7, V0, 0x21)
    swapped = list(base)
    swapped[1], swapped[2] = base[2], base[1]
    commuted = list(base)
    commuted[3] = _enc_r(T7, T6, V0, 0x21)
    spill_to_move = list(base)
    spill_to_move[4] = _enc_r(V0, 0, V1, 0x25)          # sw v0 -> or v1,v0,zero
    deleted = list(base)
    deleted[2] = 0                                      # nop over a real load
    rebased = list(base)
    rebased[2] = _enc_i(0x23, T6, T7, 8)                # base repointed at $t6
    zero_paint = list(base)
    zero_paint[4] = _enc_i(0x2b, A1, 0, 0)              # sw v0 -> sw zero
    br = [_enc_i(4, A0, 0, 2), 0, _enc_i(9, 0, V0, 1), jr_ra, 0]
    br_inv = list(br)
    br_inv[0] = _enc_i(5, A0, 0, 2)                     # beq -> bne
    li = [_enc_i(9, 0, V0, 3), _enc_i(0x2b, A1, V0, 0), jr_ra, 0]
    li_from_reg = [_enc_i(9, A0, V0, 1), _enc_i(0x2b, A1, V0, 0), jr_ra, 0]
    delay_fill = list(base)
    delay_fill[7] = _enc_r(A0, 0, V1, 0x25)             # nop -> move v1,a0 (target word)
    home = [_enc_i(9, SP, SP, -24), _enc_i(0x2b, SP, A0, 24),   # sw a0,24(sp): dead home
            _enc_i(0x23, A0, V0, 0), _enc_i(9, SP, SP, 24), jr_ra, 0]
    home_del = list(home)
    home_del[1] = 0
    home_live = home + [_enc_i(0x23, SP, T6, 24)]       # the slot IS read back
    home_live_del = list(home_live)
    home_live_del[1] = 0

    # --- 2026-10-03 red-team attacks ------------------------------------
    # A1b: a custom_script swaps the rs field a0<->a1 on two loads
    # (redteam/regfix_rs.py on TT/func_801320F0): entry values were untagged.
    a1b = list(base)
    a1b[1] = _enc_i(0x23, A1, T6, 4)
    a1b[2] = _enc_i(0x23, A1, T7, 8)
    a1b[4] = _enc_i(0x2b, A0, V0, 0)
    # A1d: a custom_script adds to an immediate (redteam/immfix.py on
    # DATE2/func_80133088's rect literals) -- li value and a field offset.
    a1d_li = [_enc_i(9, 0, T6, 320), _enc_i(0x29, SP, T6, 16), jr_ra, 0]
    a1d_li_f = [_enc_i(9, 0, T6, 384), _enc_i(0x29, SP, T6, 16), jr_ra, 0]
    a1d_off = list(base)
    a1d_off[2] = _enc_i(0x23, A0, T7, 12)               # lw t7,8(a0) -> 12(a0)
    # A2: call arguments exchanged (move a0,s0 / move a1,s1 swapped into the
    # other register) -- `jal` used to read no registers.
    call = [_enc_i(9, SP, SP, -24), _enc_i(0x2b, SP, RA, 20),
            _enc_i(0x23, A2, T6, 12), _enc_i(0x23, A2, T7, 16),
            _enc_r(T6, 0, A0, 0x25), _enc_r(T7, 0, A1, 0x25), jal, 0,
            _enc_i(0x23, SP, RA, 20), _enc_i(9, SP, SP, 24), jr_ra, 0]
    call_sw = list(call)
    call_sw[4] = _enc_r(T6, 0, A1, 0x25)
    call_sw[5] = _enc_r(T7, 0, A0, 0x25)
    call_rel = {6: (4, 'func_80123456')}
    ar = {6: 2}
    # A2b: the delay-slot argument moved into ANOTHER call's slot (the
    # normalizer class found in RPG_BAT/func_8014FDDC).
    two = [_enc_i(9, SP, SP, -24), _enc_i(0x2b, SP, RA, 20),
           jal, _enc_i(9, 0, A0, 0x3c), jal, 0,
           _enc_i(0x23, SP, RA, 20), _enc_i(9, SP, SP, 24), jr_ra, 0]
    two_mv = list(two)
    two_mv[3], two_mv[5] = 0, _enc_i(9, 0, A0, 0x3c)
    two_rel = {2: (4, 'func_80120000'), 4: (4, 'func_80120100')}
    two_ar = {2: 1, 4: 1}
    # A3: remap_stack_slots --at moves ONE store of a pair: the reload now
    # reads the other value.  A consistent remap (every access) is a rename.
    slots = [_enc_i(9, SP, SP, -32), _enc_i(0x2b, SP, A0, 16), _enc_i(0x2b, SP, A1, 20),
             _enc_i(0x23, SP, V0, 16), _enc_i(9, SP, SP, 32), jr_ra, 0]
    slots_at = list(slots)
    slots_at[1] = _enc_i(0x2b, SP, A0, 20)
    slots_at[2] = _enc_i(0x2b, SP, A1, 16)
    slots_ok = list(slots)
    slots_ok[1] = _enc_i(0x2b, SP, A0, 24)
    slots_ok[3] = _enc_i(0x23, SP, V0, 24)
    frame = list(slots)                                # patch_stack: frame +16
    frame[0] = _enc_i(9, SP, SP, -48)
    frame[4] = _enc_i(9, SP, SP, 48)
    # A4: convert_ori_to_addiu on K >= 0x8000 (main icon_disp_switch):
    # 0x800EE760 shipped as 0x800DE760.  K < 0x8000 on a lui is equivalent.
    ori = [_enc_i(15, 0, V0, 0x800e), _enc_i(13, V0, V0, 0xe760),
           _enc_i(0x24, V0, T6, 0xf6d), _enc_i(0x2b, A1, T6, 0), jr_ra, 0]
    ori_bad = list(ori)
    ori_bad[1] = _enc_i(9, V0, V0, 0xe760)
    ori_ok = list(ori)
    ori_ok[1] = _enc_i(13, V0, V0, 0x1234)
    ori_ok_f = list(ori)
    ori_ok_f[1] = _enc_i(9, V0, V0, 0x1234)
    # A5b: adjust_branch_offset retargets a branch so a guarded store
    # becomes unconditional (GsMapModelingData / GEKO/func_80135A28 shape).
    guard = [_enc_i(4, A0, 0, 2), 0, _enc_i(0x2b, A1, A2, 0), jr_ra, 0]
    guard_rt = list(guard)
    guard_rt[0] = _enc_i(4, A0, 0, 1)                  # lands ON the store
    guard_ds = [_enc_i(4, A0, 0, 1), _enc_i(0x2b, A1, A2, 0), 0, jr_ra, 0]   # store into the delay slot
    # A8: a load moved above a store that may alias it (different bases);
    # moving it above a store to a disjoint offset of the same base is fine.
    mem = [_enc_i(0x2b, A0, A1, 0), _enc_i(0x23, A2, V0, 0), jr_ra, 0]
    mem_sw = [_enc_i(0x23, A2, V0, 0), _enc_i(0x2b, A0, A1, 0), jr_ra, 0]
    mem2 = [_enc_i(0x2b, A0, A1, 0), _enc_i(0x23, A0, V0, 4), jr_ra, 0]
    mem2_sw = [_enc_i(0x23, A0, V0, 4), _enc_i(0x2b, A0, A1, 0), jr_ra, 0]
    # M1: main draw2d3d -- a custom_script paints the two stores' value
    # registers a0<->a1, so each global receives the other argument.
    m1 = [_enc_i(15, 0, AT, 0), _enc_i(0x28, AT, A0, 0), _enc_i(15, 0, AT, 0),
          jr_ra, _enc_i(0x28, AT, A1, 0)]
    m1_f = list(m1)
    m1_f[1] = _enc_i(0x28, AT, A1, 0)
    m1_f[4] = _enc_i(0x28, AT, A0, 0)
    m1_rel = {0: (5, 'D_800EF874'), 1: (6, 'D_800EF874'), 2: (5, 'D_800EF873'),
              4: (6, 'D_800EF873')}
    # a jal retargeted to another function (relocation symbol is identity)
    jt_rel = {6: (4, 'func_80120000')}
    jt_rel2 = {6: (4, 'func_80120100')}
    # clobbered registers are tagged: reading t6 after a call != reading t7
    clob = [jal, 0, _enc_r(T6, 0, V0, 0x25), jr_ra, 0]
    clob_f = [jal, 0, _enc_r(T7, 0, V0, 0x25), jr_ra, 0]
    return [
        ('identity', base, base, True, True),
        ('register rename', base, rename, True, True),
        ('independent swap', base, swapped, True, True),
        ('commutative operand swap', base, commuted, True, True),
        ('sw -> or (swap_spill_with_move)', base, spill_to_move, False, True),
        ('real load nop-ed (nop_at_offset)', base, deleted, False, True),
        ('base register repointed', base, rebased, False, True),
        ('zero<->reg store paint', base, zero_paint, False, True),
        ('beq -> bne (invert_branch)', br, br_inv, False, True),
        ('literal -> reg+K (selector+1 paint)', li, li_from_reg, False, True),
        ('retail word into delay nop (fill_delay_slots)', base, delay_fill, False, True),
        ('dead arg-home store deleted (exempt)', home, home_del, True, True),
        ('dead arg-home store kept on both sides', home, home, True, True),
        ('dead arg-home store deleted (no exemption)', home, home_del, False, False),
        ('LIVE arg-home store deleted', home_live, home_live_del, False, True),
        ('arg-home store INSERTED is never exempt', home_del, home, False, True),
        ('A1b custom_script a0<->a1 base swap (entry tags)', base, a1b, False, True),
        ('A1d custom_script li immediate edit', a1d_li, a1d_li_f, False, True),
        ('A1d custom_script load offset edit', base, a1d_off, False, True),
        ('A2 swapped call arguments', call, call_sw, False, True,
         dict(rrel=call_rel, frel=call_rel, rarity=ar, farity=ar)),
        ('A2 call arguments, register-renamed only', call, call, True, True,
         dict(rrel=call_rel, frel=call_rel, rarity=ar, farity=ar)),
        ('A2b argument moved to another call\'s delay slot', two, two_mv, False, True,
         dict(rrel=two_rel, frel=two_rel, rarity=two_ar, farity=two_ar)),
        ('A3 remap_stack_slots --at moves one store', slots, slots_at, False, True),
        ('A3 consistent slot remap is a rename', slots, slots_ok, True, True),
        ('A3 frame resize (patch_stack_complete) is a rename', slots, frame, True, True),
        ('A4 convert_ori_to_addiu K>=0x8000 (value changes)', ori, ori_bad, False, True),
        ('A4 ori->addiu K<0x8000 on a lui (same value)', ori_ok, ori_ok_f, True, True),
        ('A5b branch retargeted onto a guarded store', guard, guard_rt, False, True),
        ('A5b guarded store moved into the delay slot', guard, guard_ds, False, True),
        ('A8 load hoisted over a may-alias store', mem, mem_sw, False, True),
        ('A8 load hoisted over a disjoint same-base store', mem2, mem2_sw, True, True),
        ('M1 main store value registers a0<->a1 swapped', m1, m1_f, False, True,
         dict(rrel=m1_rel, frel=m1_rel)),
        ('jal retargeted to another symbol', call, call, False, True,
         dict(rrel=jt_rel, frel=jt_rel2)),
        ('clobbered t6 read vs clobbered t7 read', clob, clob_f, False, True),
    ]


# Negative-control functions: overlay ones from the 2026-09-27 audit; main
# ones by address (names are being renamed concurrently).
_NEG_OVERLAY = ['EVENT/func_8013FD30', 'TACO/func_8015ACDC', 'SHUGAKU/func_80138A7C',
                'ENDING/func_80133478', 'GYOZI/func_8013EA30', 'TAIIKU/func_8013FD80',
                'RPG_BAT/func_8014F180', 'TT/func_80137A80', 'TACO/func_8014DF38',
                'DATE/func_80145C84', 'TAIIKU/func_80141BC8', 'BUNKAKEN/func_80146748',
                'TT/func_801320F0', 'DATE2/func_80133088', 'RPG_BAT/func_8014EEF4',
                'EN_NICHI/func_80134900']
_NEG_MAIN = [0x80041000, 0x80042990, 0x80045804, 0x8004C750, 0x80053174, 0x8005A508,
             0x80062F5C, 0x8006D1D4, 0x80079DB8, 0x8007BF94, 0x80085890, 0x800A2C98,
             0x80043AE8, 0x80045410]


def _corrupt(fw, rel=None, arity=None):
    """Repoint one base register at $s7 (never defined).  Only a base whose
    reaching definition is an in-function instruction is chosen: repointing an
    incoming-argument base at another undefined register is invisible to a
    register-blind check by design, so it would be a void control."""
    keys = build(fw, rel, arity).keys
    fw = list(fw)
    for i, w in enumerate(fw):
        op = w >> 26
        if op in (0x23, 0x2b, 0x09) and ((w >> 21) & 31) not in (0, 29, 23) \
                and keys[i] is not None \
                and all(isinstance(x, int) for x in keys[i][1][0]):
            fw[i] = (w & ~(31 << 21)) | (23 << 21)
            return fw, rel
    return None


# Compiled attack controls: each applies one red-team attack, in place, to a
# CLEAN compiled final object -- exactly what a custom_script / paint /
# position-keyed step could do -- and must be flagged.

def _atk_entry_tag(fw, rel, arity):
    """A1b / M1: a word that reads an incoming argument register is made to
    read another argument register that is also live-in."""
    m = build(fw, rel, arity)
    reads = {}
    for i, k in enumerate(m.keys):
        if not k:
            continue
        an = analyze(fw[i])
        for j, r in enumerate(an[2]):
            if 4 <= r <= 7 and j < len(k[1]) and k[1][j] == frozenset([('IN', r)]):
                reads.setdefault(r, []).append((i, j))
    regs = sorted(reads)
    if not regs:
        return None
    r0 = regs[0]
    r1 = regs[1] if len(regs) > 1 else (5 if r0 != 5 else 4)
    i, j = reads[r0][0]
    w = fw[i]
    shift = 21 if ((w >> 21) & 31) == r0 and analyze(w)[2][j] == ((w >> 21) & 31) else 16
    fw = list(fw)
    fw[i] = (w & ~(31 << shift)) | (r1 << shift)
    return fw, rel


def _atk_offset(fw, rel, arity):
    """A1d: a non-$sp load/store offset edited (unrelocated word)."""
    for i, w in enumerate(fw):
        op = w >> 26
        if op in (0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2b) and \
                ((w >> 21) & 31) not in (0, 29) and i not in (rel or {}):
            fw = list(fw)
            fw[i] = (w & 0xFFFF0000) | (((w & 0xFFFF) + 4) & 0xFFFF)
            return fw, rel
    return None


def _atk_imm(fw, rel, arity):
    """A1d: an arithmetic immediate edited (li / addiu / slti / andi ...)."""
    for i, w in enumerate(fw):
        op = w >> 26
        if op in (9, 10, 11, 12, 13, 14) and ((w >> 21) & 31) != 29 and \
                ((w >> 16) & 31) != 29 and i not in (rel or {}):
            fw = list(fw)
            fw[i] = (w & 0xFFFF0000) | (((w & 0xFFFF) + 1) & 0xFFFF)
            return fw, rel
    return None


def _atk_call_args(fw, rel, arity):
    """A2: the two in-function definitions feeding a call's $a0 and $a1 swap
    destination registers."""
    m = build(fw, rel, arity)
    for i, w in enumerate(fw):
        if (w >> 26) != 3 or (arity or {}).get(i, 0) < 2 or not m.keys[i]:
            continue
        ops = m.keys[i][1]
        if len(ops) < 2:
            continue
        p0, p1 = ops[0], ops[1]
        if len(p0) != 1 or len(p1) != 1:
            continue
        d0, d1 = next(iter(p0)), next(iter(p1))
        if not (isinstance(d0, int) and isinstance(d1, int)) or d0 == d1:
            continue

        def redest(wd, new):
            op_ = wd >> 26
            if op_ == 0:
                return (wd & ~(31 << 11)) | (new << 11)
            return (wd & ~(31 << 16)) | (new << 16)
        fw = list(fw)
        fw[d0], fw[d1] = redest(fw[d0], 5), redest(fw[d1], 4)
        return fw, rel
    return None


def _atk_branch(fw, rel, arity):
    """A5b: a conditional branch displacement changed by one word."""
    n = len(fw)
    for i, w in enumerate(fw):
        op = w >> 26
        if op in (1, 4, 5, 6, 7) and not (op == 4 and not (w >> 16) & 0x3FF):
            t = i + 1 + _sx16(w)
            for d in (1, -1):
                if 0 <= t + d < n and fw[t + d] and t + d not in (i, i + 1):
                    fw = list(fw)
                    fw[i] = (w & 0xFFFF0000) | ((_sx16(w) + d) & 0xFFFF)
                    return fw, rel
    return None


def _atk_slot(fw, rel, arity):
    """A3: one $sp store moved to another slot (remap_stack_slots --at)."""
    loads = {(_sx16(w)) for w in fw if (w >> 26) == 0x23 and ((w >> 21) & 31) == 29}
    for i, w in enumerate(fw):
        if (w >> 26) == 0x2b and ((w >> 21) & 31) == 29 and _sx16(w) in loads \
                and ((w >> 16) & 31) != 31:
            fw = list(fw)
            fw[i] = (w & 0xFFFF0000) | ((_sx16(w) + 4) & 0xFFFF)
            return fw, rel
    return None


def _atk_mem_order(fw, rel, arity):
    """A8: an adjacent (store; load) pair whose load may read the store is
    exchanged -- the load now runs first."""
    m = build(fw, rel, arity)
    for i in range(len(fw) - 1):
        k = m.keys[i + 1]
        if analyze(fw[i])[0] == 'store' and analyze(fw[i + 1])[0] == 'load' and k \
                and ((fw[i] >> 21) & 31) != 29 and any(i in o for o in k[1]):
            fw = list(fw)
            fw[i], fw[i + 1] = fw[i + 1], fw[i]
            rel = dict(rel or {})
            a, b = rel.pop(i, None), rel.pop(i + 1, None)
            if a:
                rel[i + 1] = a
            if b:
                rel[i] = b
            return fw, rel
    return None


def _atk_ori(fw, rel, arity):
    """A4: ori -> addiu (convert_ori_to_addiu) where it changes the value."""
    for i, w in enumerate(fw):
        if (w >> 26) == 13 and (w & 0xFFFF) >= 0x8000 and ((w >> 21) & 31) != 0:
            fw = list(fw)
            fw[i] = (9 << 26) | (w & 0x03FFFFFF)
            return fw, rel
    return None


_ATTACKS = [('base->s7 (legacy)', lambda f, r, a: _corrupt(f, r, a)),
            ('A1b/M1 entry-register swap', _atk_entry_tag),
            ('A1d offset edit', _atk_offset),
            ('A1d immediate edit', _atk_imm),
            ('A2 call-argument swap', _atk_call_args),
            ('A3 one-slot remap', _atk_slot),
            ('A4 ori->addiu value change', _atk_ori),
            ('A5b branch retarget', _atk_branch),
            ('A8 load over aliasing store', _atk_mem_order)]


def self_test(jobs):
    global compare_words
    ok = True
    for row in _synthetic():
        name, rw, fw, expect_same, ah = row[:5]
        kw = row[5] if len(row) > 5 else {}
        r = compare_words(rw, fw, ah, **kw)
        same = r[VERDICT] == [0, 0]
        good = same == expect_same
        ok &= good
        print(f"  {'ok  ' if good else 'FAIL'} synthetic: {name}: {VERDICT}={r[VERDICT]} "
              f"(expected {'same' if expect_same else 'differ'})")
    # compiled controls: the uncorrupted function must measure SAME, every
    # applicable attack on it must be flagged.
    captured = {}
    real = compare_words

    def capture(rw, fw, ah=True, *rest):
        captured['rw'], captured['fw'], captured['rest'] = rw, fw, rest
        return real(rw, fw, ah, *rest)
    compare_words = capture
    per = {name: [0, 0] for name, _f in _ATTACKS}
    try:
        reg = {int(e['address'], 16): e for e in main_entries()}
        import overlay_names as _ON
        controls = [('ov', (x.split('/')[0], _ON.current_name(*x.split('/')))) for x in _NEG_OVERLAY] + \
                   [('main', reg[a]) for a in _NEG_MAIN if a in reg]
        clean = 0
        for kind, p in controls:
            captured.clear()
            label = '/'.join(p) if kind == 'ov' else p['name']
            try:
                with contextlib.redirect_stderr(io.StringIO()), \
                        contextlib.redirect_stdout(io.StringIO()):
                    r = _overlay_measure(*p, True, False) if kind == 'ov' else \
                        _main_measure(p, True, False)
            except Exception as ex:          # noqa: BLE001 -- e.g. a file mid-rename
                r = {'err': repr(ex)[:120]}
            if 'err' in r or not captured:
                print(f"  skip compiled control {label}: {r.get('err')}")
                continue
            if r[VERDICT] != [0, 0]:
                print(f"  skip compiled control {label}: not clean ({r[VERDICT]})")
                continue
            clean += 1
            rest = list(captured['rest'])          # rrel frel rarity farity rjt fjt
            frel, farity = rest[1], rest[3]
            hits = []
            for name, atk in _ATTACKS:
                got = atk(captured['fw'], frel, farity)
                if got is None:
                    continue
                bad, brel = got
                rest2 = list(rest)
                rest2[1] = brel
                c = real(captured['rw'], bad, True, *rest2)
                hit = c[VERDICT] != [0, 0]
                per[name][0] += hit
                per[name][1] += 1
                hits.append(('' if hit else 'MISSED ') + name.split(' ')[0])
                ok &= hit
            print(f"  {'ok  ' if all(not h.startswith('MISSED') for h in hits) else 'FAIL'} "
                  f"control {label}: clean; attacks {', '.join(hits)}")
        need = 16
        total = sum(t for _f, t in per.values())
        flagged = sum(f for f, _t in per.values())
        for name, (f, t) in per.items():
            print(f"  compiled attack {name:32s}: {f}/{t} flagged")
            if t == 0:
                ok = False                         # an attack nothing exercised
        if clean < need or flagged != total:
            ok = False
        print(f"  compiled negative controls: {clean} clean functions, {flagged}/{total} "
              f"corruptions flagged (need all, and at least {need} functions)")
    finally:
        compare_words = real
    return ok


# ---------------------------------------------------------------------------


def _show(r, detail=None):
    tag = {'same': 'SAME  ', 'differ': 'DIFFER', 'unmeasurable': 'UNMEAS'}[r['verdict']]
    extra = ''
    if r['verdict'] != 'unmeasurable':
        extra = f" r1={r['r1']} r3={r['r3']} words raw={r['raw_real']} final={r['fin_real']}"
        if r.get('arg_home_exempt'):
            extra += f" arg_home_exempt={r['arg_home_exempt']}"
    else:
        extra = f" {r.get('err')}"
    if r.get('sched'):
        extra += f" sched={r['sched']}"
    print(f"{tag} {r['id']} ({r['name']}){extra}")
    for ln in r.get('explain', []):
        print(ln)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0],
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--only', action='append', default=[],
                    help='OV/func, func (any region), main name, or main address 0x...')
    ap.add_argument('--overlay', action='append', default=[], help='restrict to overlay(s)')
    ap.add_argument('--main-only', action='store_true')
    ap.add_argument('--overlays-only', action='store_true')
    ap.add_argument('--jobs', type=int, default=min(16, os.cpu_count() or 1))
    ap.add_argument('--json', help='write per-function results here')
    ap.add_argument('--no-arg-home-exemption', action='store_true',
                    help="the audit's exact normalisation (flags deleted dead arg-home stores)")
    ap.add_argument('--no-sched-check', action='store_true',
                    help='skip the scheduler_normalizer branch-displacement check')
    ap.add_argument('--show', action='store_true', help='print every result, not just failures')
    ap.add_argument('--self-test', action='store_true')
    ap.add_argument('--explain', action='store_true',
                    help='print the differing words with their keys and operand producers')
    ap.add_argument('--write-baseline', action='store_true',
                    help='rewrite config/plain_vs_shipped_baseline.json (full run only)')
    a = ap.parse_args()
    os.chdir(ROOT)
    global EXPLAIN
    EXPLAIN = a.explain

    if a.self_test:
        ok = self_test(a.jobs)
        print('SELF-TEST ' + ('PASSED' if ok else 'FAILED'))
        return 0 if ok else 2

    arg_home = not a.no_arg_home_exemption
    sched = not a.no_sched_check
    tasks = []
    partial = bool(a.only or a.overlay or a.main_only or a.overlays_only)
    mains = main_entries()
    if a.only:
        byname = {e['name']: e for e in mains}
        byaddr = {int(e['address'], 16): e for e in mains}
        ovl = overlay_entries()
        for q in a.only:
            hit = False
            if q.startswith('main/'):
                q = q[5:]
            if '/' in q:
                ov, fn = q.split('/', 1)
                import overlay_names as _ON
                fn = _ON.current_name(ov, fn)       # follow a landed rename
                tasks.append(('ov', (ov, fn), arg_home, sched))
                hit = True
            else:
                if q.lower().startswith('0x') and int(q, 16) in byaddr:
                    tasks.append(('main', byaddr[int(q, 16)], arg_home, sched))
                    hit = True
                if q in byname:
                    tasks.append(('main', byname[q], arg_home, sched))
                    hit = True
                import overlay_names as _ON
                for ov, fn in ovl:
                    if fn == q or _ON.current_name(ov, q) == fn:
                        tasks.append(('ov', (ov, fn), arg_home, sched))
                        hit = True
            if not hit:
                print(f'no such function: {q}', file=sys.stderr)
                return 2
    else:
        if not a.overlays_only and not a.overlay:
            tasks += [('main', e, arg_home, sched) for e in mains]
        if not a.main_only:
            tasks += [('ov', x, arg_home, sched) for x in overlay_entries(set(a.overlay) or None)]

    results = run(tasks, a.jobs)
    base = load_baseline()
    fails, fixed = gate(results, base)

    n = collections.Counter(r['verdict'] for r in results)
    reg_n = collections.Counter((r['region'], r['verdict']) for r in results)
    sched_users = [r for r in results if r.get('sched')]
    sched_bad = [r for r in sched_users if r['sched'].get('bad')]
    sched_nonrepro = [r for r in sched_users if r['sched'].get('nonrepro')]
    failed = {f[0] for f in fails}
    for r in results:
        flagged = r['verdict'] != 'same' or (r.get('sched') or {}).get('bad')
        if a.show or flagged and (partial or r['id'] in failed):
            _show(r)
    print()
    print(f"measured {len(results)}: {n['same']} same / {n['differ']} differ / "
          f"{n['unmeasurable']} unmeasurable   "
          f"(main {reg_n[('main', 'same')]}/{reg_n[('main', 'differ')]}/"
          f"{reg_n[('main', 'unmeasurable')]}, overlay {reg_n[('overlay', 'same')]}/"
          f"{reg_n[('overlay', 'differ')]}/{reg_n[('overlay', 'unmeasurable')]})")
    ah = sum(1 for r in results if r.get('arg_home_exempt'))
    print(f"arg-home exemption {'ON' if arg_home else 'OFF'}: applied in {ah} function(s)")
    if sched:
        print(f"scheduler_normalizer users checked: {len(sched_users)}; "
              f"branch displacement copied from retail in {len(sched_bad)} "
              f"({sum(r['sched']['bad'] for r in sched_bad)} branches); "
              f"split-chain not reproducible (check void) in {len(sched_nonrepro)}")
    print(f"baseline {BASELINE.relative_to(ROOT)}: {len(base.get('differ', {}))} differ, "
          f"{len(base.get('unmeasurable', {}))} unmeasurable, "
          f"{len(base.get('sched_branch_copy', {}))} sched offenders")
    if fixed:
        print(f"FIXED since baseline ({len(fixed)}): " + ', '.join(sorted(fixed)[:20]) +
              (' ...' if len(fixed) > 20 else ''))
    if a.json:
        Path(a.json).write_text(json.dumps(results, indent=0) + '\n')
    if a.write_baseline:
        if partial:
            print('--write-baseline needs a full run (no --only/--overlay/--main-only)',
                  file=sys.stderr)
            return 2
        write_baseline(results, arg_home)
        print(f'wrote {BASELINE.relative_to(ROOT)}')
        return 0
    if fails:
        print(f'\nGATE FAILED: {len(fails)} new difference(s)')
        for f in fails:
            print('  %-40s %-24s %s' % f)
        return 1
    print('\nGATE PASSED' + (' (partial run)' if partial else ''))
    return 0


if __name__ == '__main__':
    sys.exit(main())
