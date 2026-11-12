#!/usr/bin/env python3
"""Translation validation: PROVE the shipped object S of a function is
semantically equivalent to its PostProc-free compile P, for all inputs.

    P = the C compiled with every @PostProc line stripped (same @CompileFlags,
        @CompileEnv, PIC mode) -- what the C means.
    S = the shipped object (the build's own chain), sliced to the window.

Verdicts: PROVEN (a simulation relation between P and S was found and
checked end to end), REFUTED (S has a MIPS-I hazard), UNPROVEN (anything
the prover cannot discharge, with the reason; never assumed).  The model,
the soundness argument and every assumption are in
docs/reference/EQUIVALENCE-PROOF.md.

Method (one function):
  1. decode P and S (MIPS I little-endian + COP0/COP2), relocation-aware
     (a relocated immediate is symbolic, or the linked value when the
     symbol's address is known: %hi carries the paired %lo as GNU ld does);
  2. hazards: a MIPS-I load-delay use, a load in a call's delay slot that
     differs between P and S, or mfhi/mflo -> mult/div within two words;
     in S they REFUTE, in P they are noted (P means its sequential form);
  3. CFGs with delay-slot semantics, paired by a simultaneous traversal
     from the entry: the two CFGs must have the same shape;
  4. a register/frame-cell relation at every block entry (greatest
     fixpoint; pairs, constants, linear differences, derived terms,
     branch-implied equalities), and symbolic execution of each block pair
     from it; terms are hash-consed, so equality is identity;
  5. per block pair: branch conditions, call targets, the callee's INPUT
     registers (C arity for a function entry, read off the retail code for
     any other target), stack arguments, syscall/break/COP2 sequences, the
     multiset of stores, every non-frame load, the order of every pair that
     may alias, the relation the successors assume, and at a return $v0
     (non-void), $sp, $ra, $s0-$s8, $gp;
  6. the frame: mode A (no address taken) keeps stack slots as pseudo-
     registers related like registers, so any slot remap or frame resize is
     a renaming; mode C maps each ESCAPED local (from the compiler's own
     -g3 debug info) to its S position by one translation and keeps it as
     memory; mode B (no debug info) treats the frame as shared memory with
     identical layout;
  7. fallbacks, each a complete re-proof: PATH MODE (every path between cut
     points executed as one straight line), and the same with uninitialised
     local stack bytes taken as one indeterminate value on both sides.

Usage:
    python3 tools/analysis/prove_equivalence.py --self-test
    python3 tools/analysis/prove_equivalence.py --only DATE/func_80137B98 -v
    python3 tools/analysis/prove_equivalence.py --only step34_load -v
    python3 tools/analysis/prove_equivalence.py                 # everything
    python3 tools/analysis/prove_equivalence.py --cache DIR --json out.json

Exit status: 0 every measured function PROVEN, 1 otherwise, 2 self-test /
usage failure.
"""
import argparse
import collections
import contextlib
import io
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
for _d in ("tools/overlay", "tools/build", "tools/analysis"):
    _p = str(ROOT / _d)
    if _p not in sys.path:
        sys.path.insert(0, _p)

REGISTRY = ROOT / "config" / "level3_functions.json"
OVL_STATUS = ROOT / "config" / "overlay_match_status.json"

R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 2, 4, 5, 6
HI, LO = 32, 33
NREG = 34
SP, RA, V0, V1 = 29, 31, 2, 3
# `break 7` (divide by zero) and `break 6` (signed divide overflow), the only
# break codes IDO emits; both guard C undefined behaviour
UB_TRAPS = (0x0007000D, 0x0006000D)
FULL_PATH_MODE = True       # False only in the self-test of local path mode
CALLEE_SAVED = (16, 17, 18, 19, 20, 21, 22, 23, 28, 30)
MAX_STACK_ARGS = 24      # outgoing argument WORDS compared beyond $a0-$a3 (struct args are wide)
CALLER_CLOBBER = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25, 31, HI, LO]
RNAME = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
         't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
         's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
         't8', 't9', 'k0', 'k1', 'gp', 'sp', 's8', 'ra', 'hi', 'lo']

# failure classes reported as REFUTED: a definite structural difference the
# prover found with every other part of the block equal, not an imprecision
REFUTE_CLASSES = ('hazard',)  # a definite property of S; everything else is UNPROVEN


class Unproven(Exception):
    def __init__(self, cls, msg):
        super().__init__(msg)
        self.cls = cls
        self.msg = msg


def _sx16(x):
    x &= 0xFFFF
    return x - 0x10000 if x & 0x8000 else x


def u32(x):
    return x & 0xFFFFFFFF


# ---------------------------------------------------------------------------
# Decoder
# ---------------------------------------------------------------------------

class Insn:
    __slots__ = ('i', 'w', 'op', 'kind', 'rs', 'rt', 'rd', 'sa', 'fn', 'imm',
                 'rel', 'defs', 'uses', 'name')

    def __repr__(self):
        return '<%d %08x %s>' % (self.i, self.w, self.name)


_LOADS = {32: ('lb', 1, True), 33: ('lh', 2, True), 35: ('lw', 4, True),
          36: ('lbu', 1, False), 37: ('lhu', 2, False),
          34: ('lwl', 4, None), 38: ('lwr', 4, None)}
_STORES = {40: ('sb', 1), 41: ('sh', 2), 43: ('sw', 4), 42: ('swl', 4), 46: ('swr', 4)}
_ALU_I = {8: 'addi', 9: 'addiu', 10: 'slti', 11: 'sltiu', 12: 'andi', 13: 'ori', 14: 'xori'}
_ALU_R = {0x20: 'add', 0x21: 'addu', 0x22: 'sub', 0x23: 'subu', 0x24: 'and', 0x25: 'or',
          0x26: 'xor', 0x27: 'nor', 0x2a: 'slt', 0x2b: 'sltu',
          0x04: 'sllv', 0x06: 'srlv', 0x07: 'srav'}
_SHIFT = {0x00: 'sll', 0x02: 'srl', 0x03: 'sra'}


def decode(i, w, rel=None):
    d = Insn()
    d.i, d.w, d.rel = i, w, rel
    op = w >> 26
    d.op = op
    d.rs, d.rt, d.rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
    d.sa, d.fn, d.imm = (w >> 6) & 31, w & 63, w & 0xFFFF
    d.defs, d.uses = [], []
    rs, rt, rd = d.rs, d.rt, d.rd
    k = None
    if op == 0:
        fn = d.fn
        if fn in _SHIFT:
            k, d.name, d.defs, d.uses = 'shift', _SHIFT[fn], [rd], [rt]
        elif fn in _ALU_R:
            k, d.name, d.defs, d.uses = 'alur', _ALU_R[fn], [rd], [rs, rt]
        elif fn == 0x08:
            k, d.name, d.uses = 'jr', 'jr', [rs]
        elif fn == 0x09:
            k, d.name, d.defs, d.uses = 'jalr', 'jalr', [rd], [rs]
        elif fn == 0x0c:
            k, d.name = 'syscall', 'syscall'
        elif fn == 0x0d:
            k, d.name = 'break', 'break'
        elif fn in (0x10, 0x12):
            k, d.name, d.defs, d.uses = 'mfhl', ('mfhi' if fn == 0x10 else 'mflo'), \
                [rd], [HI if fn == 0x10 else LO]
        elif fn in (0x11, 0x13):
            k, d.name, d.defs, d.uses = 'mthl', ('mthi' if fn == 0x11 else 'mtlo'), \
                [HI if fn == 0x11 else LO], [rs]
        elif fn in (0x18, 0x19, 0x1a, 0x1b):
            k, d.name = 'muldiv', {0x18: 'mult', 0x19: 'multu', 0x1a: 'div', 0x1b: 'divu'}[fn]
            d.defs, d.uses = [HI, LO], [rs, rt]
    elif op == 1:
        if rt in (0, 1):
            k, d.name, d.uses = 'br', ('bltz' if rt == 0 else 'bgez'), [rs]
        elif rt in (16, 17):
            k, d.name, d.uses, d.defs = 'bal', ('bltzal' if rt == 16 else 'bgezal'), [rs], [RA]
    elif op == 2:
        k, d.name = 'j', 'j'
    elif op == 3:
        k, d.name, d.defs = 'jal', 'jal', [RA]
    elif op in (4, 5):
        k, d.name, d.uses = 'br', ('beq' if op == 4 else 'bne'), [rs, rt]
    elif op in (6, 7):
        if rt == 0:
            k, d.name, d.uses = 'br', ('blez' if op == 6 else 'bgtz'), [rs]
    elif op in _ALU_I:
        k, d.name, d.defs, d.uses = 'alui', _ALU_I[op], [rt], [rs]
    elif op == 15:
        k, d.name, d.defs = 'lui', 'lui', [rt]
    elif op in _LOADS:
        k, d.name = 'load', _LOADS[op][0]
        d.defs, d.uses = [rt], ([rs, rt] if op in (34, 38) else [rs])
    elif op in _STORES:
        k, d.name, d.uses = 'store', _STORES[op][0], [rs, rt]
    elif op == 16:                       # COP0
        if rs == 0:
            k, d.name, d.defs = 'cop0', 'mfc0', [rt]
        elif rs == 4:
            k, d.name, d.uses = 'cop0', 'mtc0', [rt]
        elif rs == 16 and d.fn == 16:
            k, d.name = 'cop0', 'rfe'
    elif op == 18:                       # COP2 (GTE)
        if rs in (0, 2):
            k, d.name, d.defs = 'cop2', ('mfc2' if rs == 0 else 'cfc2'), [rt]
        elif rs in (4, 6):
            k, d.name, d.uses = 'cop2', ('mtc2' if rs == 4 else 'ctc2'), [rt]
        elif rs >= 16:
            k, d.name = 'cop2', 'cop2'
    elif op == 50:
        k, d.name, d.uses = 'lwc2', 'lwc2', [rs]
    elif op == 58:
        k, d.name, d.uses = 'swc2', 'swc2', [rs]
    if k is None:
        k, d.name = 'unknown', 'op%d' % op
    d.kind = k
    d.defs = [r for r in d.defs if r != 0]
    return d


def branch_target(d):
    return d.i + 1 + _sx16(d.imm)


def is_xfer(d):
    return d.kind in ('br', 'j', 'jr', 'jal', 'jalr', 'bal')


def is_uncond_br(d):
    return d.kind == 'br' and ((d.name == 'beq' and d.rs == 0 and d.rt == 0) or
                               (d.name == 'bgez' and d.rs == 0))


def disasm(d):
    r = RNAME
    if d.w == 0:
        return 'nop'
    k = d.kind
    if k == 'shift':
        return '%s %s,%s,%d' % (d.name, r[d.rd], r[d.rt], d.sa)
    if k == 'alur':
        return '%s %s,%s,%s' % (d.name, r[d.rd], r[d.rs], r[d.rt])
    if k == 'alui':
        return '%s %s,%s,%d' % (d.name, r[d.rt], r[d.rs], _sx16(d.imm))
    if k == 'lui':
        return 'lui %s,0x%x' % (r[d.rt], d.imm)
    if k in ('load', 'store'):
        return '%s %s,%d(%s)' % (d.name, r[d.rt], _sx16(d.imm), r[d.rs])
    if k == 'br':
        return '%s %s,%s,->%d' % (d.name, r[d.rs], r[d.rt], branch_target(d))
    if k in ('jr', 'jalr'):
        return '%s %s' % (d.name, r[d.rs])
    if k == 'muldiv':
        return '%s %s,%s' % (d.name, r[d.rs], r[d.rt])
    if k in ('mfhl', 'mthl'):
        return '%s %s' % (d.name, r[d.rd] if k == 'mfhl' else r[d.rs])
    return d.name + (' ' + str(d.rel) if d.rel else '')


# ---------------------------------------------------------------------------
# Terms (hash-consed)
# ---------------------------------------------------------------------------

class T:
    """An interned term.  Identity == structural equality.  `h` is a hash
    that IGNORES frame sides and offsets (so equal-modulo-sigma terms of P
    and S sort the same way inside commutative nodes)."""
    __slots__ = ('op', 'a', 'h', 'side', '__weakref__')

    def __repr__(self):
        return fmt(self)


class TermBank:
    def __init__(self):
        self.tab = {}

    def mk(self, op, *a):
        key = (op,) + tuple(id(x) if isinstance(x, T) else x for x in a)
        t = self.tab.get(key)
        if t is not None:
            return t
        t = T()
        t.op, t.a = op, a
        t.h = hash((op,) + tuple(x.h if isinstance(x, T) else x for x in a))
        sd = None
        if op == 'fr':
            sd = a[0]
        elif op == 'junk':
            sd = a[0]
        else:
            for x in a:
                if isinstance(x, T) and x.side is not None:
                    sd = x.side
                    break
        t.side = sd
        # keep a reference to the children through `key` (ids must stay valid)
        self.tab[key] = t
        self.tab[(id(t), 'keep')] = a
        return t


def fmt(t, depth=0):
    if not isinstance(t, T):
        return repr(t)
    if depth > 6:
        return '...'
    if t.op == 'c':
        v = t.a[0]
        return hex(v) if v > 9 else str(v)
    if t.op == 'in':
        return 'in%s.%s' % (t.a[0], RNAME[t.a[1]] if isinstance(t.a[1], int) and t.a[1] < NREG
                            else t.a[1])
    if t.op == 'fr':
        return 'FR%s[%d]' % (t.a[0] or '', t.a[1])
    return '%s(%s)' % (t.op, ','.join(fmt(x, depth + 1) for x in t.a))


COMMUTATIVE = ('and', 'or', 'xor', 'nor', 'eq', 'ne')


def _key(t):
    """Total order for the operands of a commutative node.  Equal terms are
    the SAME object (hash-consing), so ordering by (h, id) gives P and S the
    same operand order for the same multiset of terms."""
    return (t.h, id(t))


class Algebra:
    """Normalising constructors.  Sums are ('sum', terms..., const) with the
    terms sorted by h; frame addresses fold their constant: fr(side, k)."""

    def __init__(self, bank):
        self.b = bank
        self.ZERO = self.c(0)

    def c(self, v):
        return self.b.mk('c', u32(v))

    def isc(self, t):
        return t.op == 'c'

    def _parts(self, t):
        if t.op == 'sum':
            return list(t.a[:-1]), t.a[-1]
        if t.op == 'c':
            return [], t.a[0]
        return [t], 0

    def _mksum(self, terms, k):
        k = u32(k)
        # cancel t + neg(t), and frame-address differences of one side
        changed = True
        while changed:
            changed = False
            for x in terms:
                if x.op == 'neg':
                    y = x.a[0]
                    if y in terms:
                        terms.remove(x)
                        terms.remove(y)
                        changed = True
                        break
                    if y.op == 'fr':
                        for z in terms:
                            if z.op == 'fr' and z.a[0] == y.a[0]:
                                terms.remove(x)
                                terms.remove(z)
                                k = u32(k + z.a[1] - y.a[1])
                                changed = True
                                break
                        if changed:
                            break
        frs = [x for x in terms if x.op == 'fr']
        if len(frs) == 1:
            f = frs[0]
            terms.remove(f)
            ks = k - (1 << 32) if k & 0x80000000 else k
            terms.append(self.b.mk('fr', f.a[0], f.a[1] + ks))
            k = 0
        if not terms:
            return self.c(k)
        if len(terms) == 1 and k == 0:
            return terms[0]
        terms.sort(key=_key)
        return self.b.mk('sum', *terms, k)

    def add(self, x, y):
        tx, kx = self._parts(x)
        ty, ky = self._parts(y)
        return self._mksum(tx + ty, kx + ky)

    def neg(self, x):
        if x.op == 'c':
            return self.c(-x.a[0])
        if x.op == 'neg':
            return x.a[0]
        if x.op == 'sum':
            terms, k = self._parts(x)
            return self._mksum([self.neg(t) for t in terms], -k)
        return self.b.mk('neg', x)

    def sub(self, x, y):
        return self.add(x, self.neg(y))

    def fr(self, side, k):
        return self.b.mk('fr', side, k)

    def logic(self, op, x, y):
        if x.op == 'c' and y.op == 'c':
            a, b = x.a[0], y.a[0]
            return self.c({'and': a & b, 'or': a | b, 'xor': a ^ b, 'nor': ~(a | b)}[op])
        if y.op == 'c' and x.op != 'c':
            x, y = y, x
        if x.op == 'c':
            v = x.a[0]
            if op == 'and' and v == 0:
                return self.ZERO
            if op == 'and' and v == 0xFFFFFFFF:
                return y
            if op in ('or', 'xor') and v == 0:
                return y
        if x is y:
            if op in ('and', 'or'):
                return x
            if op == 'xor':
                return self.ZERO
        if _key(x) > _key(y):
            x, y = y, x
        return self.b.mk(op, x, y)

    def shift(self, op, x, sa):
        if sa == 0:
            return x
        if x.op == 'c':
            v = x.a[0]
            if op == 'sll':
                return self.c(v << sa)
            if op == 'srl':
                return self.c(v >> sa)
            sv = v - (1 << 32) if v & 0x80000000 else v
            return self.c(sv >> sa)
        if op == 'sll' and x.op == 'sll' and x.a[1] + sa < 32:
            return self.b.mk('sll', x.a[0], x.a[1] + sa)
        return self.b.mk(op, x, sa)

    def vshift(self, op, x, s):
        if s.op == 'c':
            return self.shift(op[:-1], x, s.a[0] & 31)
        return self.b.mk(op, x, s)

    def slt(self, op, x, y):
        if x.op == 'c' and y.op == 'c':
            a, b = x.a[0], y.a[0]
            if op == 'slt':
                a = a - (1 << 32) if a & 0x80000000 else a
                b = b - (1 << 32) if b & 0x80000000 else b
            return self.c(1 if a < b else 0)
        return self.b.mk(op, x, y)

    def ext(self, size, signed, v):
        """The register value of a `size`-byte load of a word whose low
        bytes are v (store forwarding)."""
        if size == 4:
            return v
        if v.op == 'c':
            x = v.a[0] & ((1 << (8 * size)) - 1)
            if signed and x & (1 << (8 * size - 1)):
                x -= 1 << (8 * size)
            return self.c(x)
        if v.op in ('sx', 'zx') and v.a[0] == size:
            v = v.a[2]
        return self.b.mk('sx' if signed else 'zx', size, 0, v)


# ---------------------------------------------------------------------------
# Function model: words + relocations + jump tables -> CFG
# ---------------------------------------------------------------------------

class Func:
    """One side of one function."""

    def __init__(self, side, words, rel, jt, base=0):
        self.side = side
        self.words = list(words)
        self.rel = dict(rel or {})
        self.jt = sorted(set(jt or []))
        self.base = base                   # function's word index in its .text
        self.ins = [decode(i, w, self.rel.get(i)) for i, w in enumerate(self.words)]
        self.n = len(self.words)

    def local_jump_target(self, d):
        """Index of a `j` (R_MIPS_26 against .text / the function itself)."""
        field = d.w & 0x3FFFFFF
        return field - self.base

    # --- CFG -----------------------------------------------------------
    def build_cfg(self):
        ins, n = self.ins, self.n
        leaders = {0}
        for d in ins:
            if d.kind == 'br':
                leaders.add(branch_target(d))
                leaders.add(d.i + 2)
            elif d.kind == 'j':
                leaders.add(self.j_target(d))
                leaders.add(d.i + 2)
            elif d.kind == 'jr':
                leaders.add(d.i + 2)
        for x in self.jt:
            leaders.add(x)
        leaders = sorted(x for x in leaders if 0 <= x < n)
        for d in ins:
            if d.kind == 'br':
                t = branch_target(d)
                if not 0 <= t < n:
                    raise Unproven('cfg', '%s: branch at %d targets %d outside the window'
                                   % (self.side, d.i, t))
            if is_xfer(d) and d.i + 1 < n and is_xfer(ins[d.i + 1]):
                raise Unproven('cfg', '%s: transfer in a delay slot at %d' % (self.side, d.i + 1))
            if d.kind == 'bal':
                raise Unproven('unsupported', '%s: conditional call %s at %d'
                               % (self.side, d.name, d.i))
        lset = set(leaders)
        self.leaders = leaders
        self.blk_at = {}                   # leader word -> block id
        self.term_blk = {}                 # transfer word -> block id
        self.blocks = []                   # [start, end_exclusive, term]
        # A block runs from its leader to the first transfer (plus the
        # transfer's delay slot) or to the next leader.  A delay slot that is
        # itself a branch target starts a block of its own as well: blocks
        # may overlap by that one word (it executes on both paths).
        for s in leaders:
            j = s
            term = None
            if ins[s].w in UB_TRAPS and s > 1 and ins[s - 2].kind == 'br' and \
                    not is_uncond_br(ins[s - 2]) and ins[s - 1].w == 0:
                # IDO's divide check `bne rt,$0,1f ; nop ; break 7 ; 1:` (and
                # the INT_MIN/-1 `break 6` check): reached only when the C
                # divides by zero / overflows -- undefined behaviour in C.
                # The path ENDS here (see check_block: no load obligations).
                self.blk_at[s] = len(self.blocks)
                self.term_blk[s] = len(self.blocks)
                self.blocks.append([s, s + 1, ('trap', s)])
                continue
            while j < n:
                if j > s and j in lset:
                    term = ('fall', j)
                    break
                d = ins[j]
                if d.kind in ('jal', 'jalr'):
                    j += 2                 # the call's delay slot belongs to it
                    continue
                if d.kind in ('br', 'j', 'jr'):
                    if j + 1 >= n:
                        raise Unproven('cfg', '%s: transfer at %d has no delay slot'
                                       % (self.side, j))
                    if d.kind == 'br':
                        t = branch_target(d)
                        term = ('goto', t, j) if is_uncond_br(d) else ('cond', j, t, j + 2)
                    elif d.kind == 'j':
                        term = ('goto', self.j_target(d), j)
                    elif d.rs == RA:
                        term = ('ret', j)
                    elif self.jt:
                        term = ('jt', j)
                    else:
                        raise Unproven('cfg', '%s: indirect jump at %d with no jump table'
                                       % (self.side, j))
                    j += 2
                    break
                j += 1
            if term is None:
                term = ('fall', j) if j < n else ('end',)
                if j > n:
                    raise Unproven('cfg', '%s: a call at the end of the window' % self.side)
            if term[0] not in ('fall', 'end'):
                self.term_blk[term[-1] if term[0] != 'cond' else term[1]] = len(self.blocks)
            self.blk_at[s] = len(self.blocks)
            self.blocks.append([s, min(j, n), term])
        self.blk_of = self.blk_at

    def blk_of_word(self, i):
        for k, (s_, e_, _) in enumerate(self.blocks):
            if s_ <= i < e_:
                return k
        return None

    def j_target(self, d):
        r = d.rel
        if r is None or r[0] != R_MIPS_26:
            return (d.w & 0x3FFFFFF) - self.base
        return self.local_jump_target(d)

    def succs(self, k):
        t = self.blocks[k][2]
        if t[0] == 'fall':
            return [('fall', self.blk_at[t[1]])]
        if t[0] == 'goto':
            if not 0 <= t[1] < self.n:
                raise Unproven('cfg', '%s: jump outside the window' % self.side)
            return [('goto', self.blk_of[t[1]])]
        if t[0] == 'cond':
            out = [('taken', self.blk_of[t[2]])]
            if t[3] < self.n:
                out.append(('fall', self.blk_of[t[3]]))
            else:
                out.append(('fall', None))
            return out
        if t[0] == 'jt':
            return [('jt%d' % q, self.blk_of[x]) for q, x in enumerate(self.jt)]
        return []


# ---------------------------------------------------------------------------
# Hazards (MIPS I)
# ---------------------------------------------------------------------------

def _reads(d, r):
    return r in d.uses


def hazards(f):
    """[(index, text)] for every load-delay or HI/LO hazard."""
    out = []
    ins, n = f.ins, f.n
    blk_succ_first = {}
    for k, b in enumerate(f.blocks):
        try:
            blk_succ_first[k] = [f.blocks[s][0] for _, s in f.succs(k) if s is not None]
        except Unproven:
            blk_succ_first[k] = []

    def nexts(i):
        """Indices that may execute right after i."""
        if i > 0 and is_xfer(ins[i - 1]):
            p = ins[i - 1]
            if p.kind in ('jal', 'jalr', 'bal'):
                return ['callee']
            k = f.term_blk.get(i - 1)
            return blk_succ_first.get(k, [])
        if i + 1 >= n:
            return []
        return [i + 1]

    for d in ins:
        if d.kind == 'load' or (d.kind in ('cop0', 'cop2') and d.name in ('mfc0', 'mfc2', 'cfc2')):
            r = d.rt
            if r == 0:
                continue
            for j in nexts(d.i):
                if j == 'callee':
                    continue                   # checked per call (Executor.run)
                e = ins[j]
                if _reads(e, r):
                    if d.name in ('lwl', 'lwr') and e.name in ('lwl', 'lwr') and e.rt == r \
                            and e.rs != r:
                        continue           # lwl/lwr pair merges through the delay
                    out.append((d.i, 'load-delay: %s at %d read by %s at %d' % (
                        disasm(d), d.i, disasm(e), j)))
        if d.kind == 'mfhl':
            frontier = [(d.i, 0)]
            seen = set()
            while frontier:
                i, dist = frontier.pop()
                for j in nexts(i):
                    if j == 'callee' or (j, dist + 1) in seen or dist + 1 > 2:
                        continue
                    seen.add((j, dist + 1))
                    e = ins[j]
                    if e.kind in ('muldiv', 'mthl'):
                        out.append((d.i, 'hi/lo: %s at %d then %s at %d' % (
                            disasm(d), d.i, disasm(e), j)))
                    frontier.append((j, dist + 1))
    return out


# ---------------------------------------------------------------------------
# Memory events and alias analysis
# ---------------------------------------------------------------------------

class Ev:
    __slots__ = ('kind', 'pos', 'addr', 'cls', 'size', 'val', 'signed', 'term', 'name',
                 'extra', 'sp', 'seq', 'regs_at')

    def __repr__(self):
        return '<Ev %s @%d %s>' % (self.kind, self.pos, fmt(self.term) if self.term else '')


PROV = None          # set per proof: callable term -> object address or None


def aclass(addr):
    """Alias class of an address term."""
    c = _aclass(addr)
    if c[0] in ('A', 'P') and PROV is not None:
        return c + (PROV(addr),)
    return c


def _aclass(addr):
    if addr.op == 'fr':
        return ('F', addr.a[0], addr.a[1])
    if addr.op == 'c':
        return ('A', addr.a[0])
    terms, k = (list(addr.a[:-1]), addr.a[-1]) if addr.op == 'sum' else ([addr], 0)
    ks = k - (1 << 32) if k & 0x80000000 else k
    if any(x.op == 'fr' for x in terms):
        return ('FX', [x for x in terms if x.op == 'fr'][0].a[0])
    objs = [x for x in terms if x.op == 'obj']
    if len(objs) == 1:
        if len(terms) == 1:
            return ('O', objs[0].a[0], ks)
        return ('OX', objs[0].a[0])
    syms = [x for x in terms if x.op == 'sym']
    if len(syms) == 1 and len(terms) == 1:
        return ('G', syms[0].a[0], ks)
    if len(syms) == 1:
        return ('GX', syms[0].a[0])
    return ('P', addr if addr.op != 'sum' else
            (terms[0] if len(terms) == 1 else tuple(terms)), ks)


def _ov(a, sa, b, sb):
    return a < b + sb and b < a + sa


_NAME_ADDR = {}
_CUR_OV = [None]        # overlay of the task being proven: its landed names resolve first


def name_addr(sym):
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


def _span(e):
    """(start offset, size) of an access for overlap purposes; swl/swr/lwl/
    lwr touch the aligned word around their address."""
    if e.name in ('lwl', 'lwr', 'swl', 'swr'):
        return -3, 7
    return 0, e.size


def canon_events(evs, escaped):
    """The memory state a load reads, as a CANONICAL event sequence: two
    adjacent events that cannot alias each other (stores to disjoint
    locations; never a call or a zone clobber) commute, so the same memory
    state gives the same order on both sides.  A topological sort that keeps
    every ordered pair (a call/clobber against anything, two stores that
    may alias) and otherwise picks the least term first."""
    n = len(evs)
    if n < 2:
        return evs
    pred = [0] * n
    succ = [[] for _ in range(n)]
    for j in range(n):
        y = evs[j]
        for i in range(j):
            x = evs[i]
            if x.kind != 'st' or y.kind != 'st' or may_alias(x, y, escaped):
                pred[j] += 1
                succ[i].append(j)
    ready = [i for i in range(n) if not pred[i]]
    out = []
    while ready:
        ready.sort(key=lambda i: _key(evs[i].term))
        i = ready.pop(0)
        out.append(evs[i])
        for j in succ[i]:
            pred[j] -= 1
            if not pred[j]:
                ready.append(j)
    return out


RANGES = None        # set per proof: {(P block, register): (lo, hi)} at block entry

_S32 = (-(1 << 31), (1 << 31) - 1)


def _clip(lo, hi):
    return (lo, hi) if _S32[0] <= lo <= hi <= _S32[1] else None


def reg_intervals(f):
    """Interval analysis of ONE side's code (P): for each block, a signed
    range [lo, hi] for each register at block entry, or None (unknown).
    Sequential semantics; a branch refines the registers its condition
    reads (as the branch reads them, before its delay slot) on each edge;
    calls clobber the caller-saved registers; anything unmodelled is
    unknown; joins take the hull; a block that keeps changing is widened to
    unknown.  Used only to bound the value of a block-entry variable."""
    nb = len(f.blocks)

    def k32(v):
        v &= 0xFFFFFFFF
        return v - (1 << 32) if v & 0x80000000 else v

    def step(st, cmp, d):
        defs = d.defs
        if d.w == 0 or not defs:
            return
        r = defs[0] if defs else None
        v = None
        rel = d.rel is not None
        k = d.kind
        I = lambda q: (0, 0) if q == 0 else st[q]
        if k == 'alui' and not rel:
            a = I(d.rs)
            imm = _sx16(d.imm)
            if d.name in ('addiu', 'addi') and a:
                v = _clip(a[0] + imm, a[1] + imm)
            elif d.name in ('ori', 'xori') and d.rs == 0:
                v = (d.imm, d.imm)
            elif d.name == 'andi':
                v = (0, min(d.imm, a[1]) if a and a[0] >= 0 else d.imm)
            elif d.name in ('slti', 'sltiu'):
                v = (0, 1)
        elif k == 'lui' and not rel:
            c = k32(d.imm << 16)
            v = (c, c)
        elif k == 'shift':
            a = I(d.rt)
            sa = d.sa
            if d.name == 'sll':
                if sa == 0:
                    v = a
                elif a and a[0] >= 0 and (a[1] << sa) <= _S32[1]:
                    v = (a[0] << sa, a[1] << sa)
            elif d.name == 'sra':
                v = (a[0] >> sa, a[1] >> sa) if a else \
                    (-(1 << (31 - sa)), (1 << (31 - sa)) - 1)
            elif d.name == 'srl':
                if sa == 0:
                    v = a
                elif a and a[0] >= 0:
                    v = (a[0] >> sa, a[1] >> sa)
                else:
                    v = (0, (1 << (32 - sa)) - 1)
        elif k == 'alur':
            a, b = I(d.rs), I(d.rt)
            if d.name in ('addu', 'or', 'add') and (d.rs == 0 or d.rt == 0):
                v = b if d.rs == 0 else a
            elif d.name in ('addu', 'add') and a and b:
                v = _clip(a[0] + b[0], a[1] + b[1])
            elif d.name in ('subu', 'sub') and a and b:
                v = _clip(a[0] - b[1], a[1] - b[0])
            elif d.name == 'and':
                nn = [x for x in (a, b) if x and x[0] >= 0]
                if nn:
                    v = (0, min(x[1] for x in nn))
            elif d.name in ('slt', 'sltu'):
                v = (0, 1)
        elif k == 'load':
            v = {'lb': (-128, 127), 'lbu': (0, 255), 'lh': (-32768, 32767),
                 'lhu': (0, 65535)}.get(d.name)
        for q in defs:
            if q < 32:
                st[q] = None
            for c in list(cmp):
                if c == q:
                    del cmp[c]
                elif q in cmp[c][1]:
                    srcs = tuple(x for x in cmp[c][1] if x != q)
                    if srcs:
                        cmp[c] = (cmp[c][0], srcs, cmp[c][2])
                    else:
                        del cmp[c]
        if r is not None and r < 32:
            st[r] = v
            if k == 'alui' and d.name in ('slti', 'sltiu') and not rel and d.rs != 0 and \
                    d.rs != r:
                cmp[r] = (d.name, (d.rs,), _sx16(d.imm))
            elif k == 'alur' and d.name in ('addu', 'or', 'add') and \
                    (d.rs == 0) != (d.rt == 0):
                # a register copy: it holds the compared value too, so a
                # later branch on the comparison bounds both
                src = d.rt if d.rs == 0 else d.rs
                for c in list(cmp):
                    if src in cmp[c][1] and r not in cmp[c][1] and c != r:
                        cmp[c] = (cmp[c][0], cmp[c][1] + (r,), cmp[c][2])

    def meet(a, lo, hi):
        a = a or _S32
        lo, hi = max(a[0], lo), min(a[1], hi)
        return (lo, hi) if lo <= hi else False

    def refine(st, cmp, d, taken, killed):
        """The state on one edge of the conditional branch d; False when
        the edge is infeasible."""
        st = list(st)

        def put(q, lo, hi):
            if q == 0 or q in killed:
                return True
            m = meet(st[q], lo, hi)
            if m is False:
                return False
            st[q] = m
            return True
        nm = d.name
        if nm in ('beq', 'bne') and (d.rs == 0) != (d.rt == 0):
            x = d.rt if d.rs == 0 else d.rs
            zero = (nm == 'beq') == taken          # the edge on which x == 0
            if zero:
                if not put(x, 0, 0):
                    return False
            c = cmp.get(x)
            for src in (c[1] if c else ()):
                if src in killed:
                    continue
                op, K = c[0], c[2]
                if op == 'slti':
                    ok = put(src, K, _S32[1]) if zero else put(src, _S32[0], K - 1)
                elif not zero and 0 < (K & 0xFFFFFFFF) <= _S32[1]:
                    ok = put(src, 0, (K & 0xFFFFFFFF) - 1)   # sltiu true
                else:
                    ok = True
                if not ok:
                    return False
            return st
        if nm in ('bltz', 'bgez', 'blez', 'bgtz'):
            x = d.rs
            neg = {'bltz': (_S32[0], -1), 'bgez': (0, _S32[1]),
                   'blez': (_S32[0], 0), 'bgtz': (1, _S32[1])}[nm]
            pos = {'bltz': (0, _S32[1]), 'bgez': (_S32[0], -1),
                   'blez': (1, _S32[1]), 'bgtz': (_S32[0], 0)}[nm]
            lo, hi = neg if taken else pos
            return st if put(x, lo, hi) else False
        return st

    entry = {0: [None] * 32}
    entry[0][0] = (0, 0)
    count = collections.Counter()
    work = [0]
    while work:
        b = work.pop()
        st = list(entry[b])
        cmp = {}
        s0, e0, term = f.blocks[b]
        at_br = None
        i = s0
        while i < e0:
            d = f.ins[i]
            if d.kind in ('jal', 'jalr'):
                step(st, cmp, d)
                if i + 1 < e0:
                    step(st, cmp, f.ins[i + 1])   # the slot runs before the callee
                for q in CALLER_CLOBBER:
                    if q < 32:
                        st[q] = None
                cmp.clear()
                i += 2
                continue
            if is_xfer(d):
                at_br = (list(st), dict(cmp), d, i)
                step(st, cmp, d)
                if i + 1 < e0:
                    step(st, cmp, f.ins[i + 1])
                i += 2
                continue
            step(st, cmp, d)
            i += 1
        outs = []
        for kind, sb in f.succs(b):
            if sb is None:
                continue
            o = st
            if term[0] == 'cond' and at_br is not None and kind in ('taken', 'fall'):
                abst, acmp, d, j = at_br
                killed = set(f.ins[j + 1].defs) if j + 1 < e0 else set()
                # the condition reads the BRANCH-TIME values; registers the
                # delay slot writes take their post-slot value instead
                o2 = refine(abst, acmp, d, kind == 'taken', set())
                if o2 is False:
                    continue
                o = [st[q] if q in killed else o2[q] for q in range(32)]
            outs.append((sb, o))
        for sb, o in outs:
            old = entry.get(sb)
            if old is None:
                new = list(o)
            else:
                new = [None if (x is None or y is None) else (min(x[0], y[0]), max(x[1], y[1]))
                       for x, y in zip(old, o)]
            if new != old:
                count[sb] += 1
                if count[sb] > 48:
                    new = [x if old is not None and x == old[q] else None
                           for q, x in enumerate(new)]
                    new[0] = (0, 0)
                if new != old:
                    entry[sb] = new
                    work.append(sb)
    return entry


def valued_taint(f, sentinels, callrets, voidcallees, nonvoid):
    """P-only forward dataflow: for each block, at entry and on each
    successor edge, which registers MAY hold the value of a `return
    <expr>;` -- a sentinel constant of C'' (or, when C has call returns, a
    call result).  Over-approximate: an operation on a tainted operand is
    tainted, a frame/memory load is tainted once anything tainted is stored,
    an unmodelled word taints what it defines.  Entry registers are clean (a
    literal or call return is no argument).  -> {(block, succ index):
    [taint per register]} for the edges."""
    NR = 34                                # 32 GPRs + HI + LO
    sent = {x & 0xFFFFFFFF for x in sentinels}
    nb = len(f.blocks)

    def run_block(b, st, cst, mem):
        st, cst = list(st), list(cst)
        s0, e0, _ = f.blocks[b]
        stored = False
        i = s0
        order = []
        while i < e0:
            d = f.ins[i]
            if is_xfer(d) and i + 1 < e0:
                order += [d, f.ins[i + 1]]
                i += 2
            else:
                order.append(d)
                i += 1
        for d in order:
            k = d.kind
            if d.w == 0:
                continue
            if k == 'store':
                if st[d.rt]:
                    stored = True
                continue
            if k in ('jal', 'jalr'):
                for q in CALLER_CLOBBER:
                    if q < NR:
                        st[q], cst[q] = False, None
                nm = d.rel[1] if (k == 'jal' and d.rel) else None
                if callrets and not (nm and (nm in voidcallees or nonvoid.get(nm) is False)):
                    st[V0] = True
                st[RA] = False
                continue
            if not d.defs:
                continue
            ops = [q for q in d.uses if q != 0]
            taint = any(st[q] for q in ops if q < NR)
            cv = None
            if d.rel is None:
                if k == 'alui':
                    a = 0 if d.rs == 0 else cst[d.rs]
                    if a is not None:
                        imm = _sx16(d.imm)
                        cv = {'addiu': a + imm, 'addi': a + imm, 'ori': a | d.imm,
                              'xori': a ^ d.imm, 'andi': a & d.imm}.get(d.name)
                elif k == 'lui':
                    cv = d.imm << 16
                elif k == 'alur' and d.name in ('addu', 'or', 'add', 'subu', 'xor', 'and'):
                    a = 0 if d.rs == 0 else cst[d.rs]
                    bb = 0 if d.rt == 0 else cst[d.rt]
                    if a is not None and bb is not None:
                        cv = {'addu': a + bb, 'add': a + bb, 'or': a | bb, 'subu': a - bb,
                              'xor': a ^ bb, 'and': a & bb}[d.name]
            if k == 'load':
                taint = mem
            elif k not in ('alui', 'lui', 'alur', 'shift', 'muldiv', 'mfhl', 'mthl'):
                taint = True                   # unmodelled: conservative
            if cv is not None:
                cv &= 0xFFFFFFFF
                taint = cv in sent
            for q in d.defs:
                if q < NR:
                    st[q], cst[q] = taint, cv
        return st, cst, stored

    entry = {0: ([False] * NR, [0] + [None] * (NR - 1))}
    mem = False
    for _round in range(4):
        edges = {}
        work = [0]
        seen_entry = dict(entry)
        cnt = collections.Counter()
        anystore = False
        while work:
            b = work.pop()
            st, cst = seen_entry[b]
            st2, cst2, stored = run_block(b, st, cst, mem)
            anystore |= stored
            for q, (_, sb) in enumerate(f.succs(b)):
                edges[(b, q)] = st2
                if sb is None:
                    continue
                old = seen_entry.get(sb)
                if old is None:
                    new = (list(st2), list(cst2))
                else:
                    new = ([x or y for x, y in zip(old[0], st2)],
                           [x if x == y else None for x, y in zip(old[1], cst2)])
                if new != old:
                    cnt[sb] += 1
                    if cnt[sb] > 200:
                        new = ([True] * NR, [None] * NR)
                    seen_entry[sb] = new
                    work.append(sb)
        if anystore == mem or mem:
            break
        mem = anystore
    return edges


FREE = None          # set per mode-B proof: {P block: [frame-free? per register]}


def reg_frame_free(f):
    """Must-analysis over ONE side's code (P): which registers hold, at each
    block entry, a value NOT derived from a frame address of this function.
    At entry every register is free but $sp (a value the caller holds cannot
    legitimately point below its own $sp, where this frame is about to be
    allocated); $sp-derived values, word loads (memory may hold an escaped
    frame address), call results and clobbers and anything unmodelled are
    not; arithmetic on free values is free; narrow loads are free."""
    n = NREG
    top = [True] * n
    top[SP] = False

    def step(st, d):
        if d.w == 0:
            return
        k = d.kind
        if k in ('jal', 'jalr'):
            return
        if k == 'load':
            v = d.name in ('lb', 'lbu', 'lh', 'lhu')
        elif k in ('alui', 'alur', 'shift', 'lui', 'mfhl', 'mthl', 'muldiv'):
            v = all(st[u] for u in d.uses if u != 0)
        else:
            v = False
        for q in d.defs:
            st[q] = v

    entry = {0: list(top)}
    work = [0]
    while work:
        b = work.pop()
        st = list(entry[b])
        s0, e0, _ = f.blocks[b]
        i = s0
        while i < e0:
            d = f.ins[i]
            if d.kind in ('jal', 'jalr'):
                if i + 1 < e0:
                    step(st, f.ins[i + 1])
                for q in CALLER_CLOBBER:
                    st[q] = False
                st[RA] = True                      # a code address
                i += 2
                continue
            step(st, d)
            i += 1
        for _, sb in f.succs(b):
            if sb is None:
                continue
            old = entry.get(sb)
            new = list(st) if old is None else [x and y for x, y in zip(old, st)]
            if new != old:
                entry[sb] = new
                work.append(sb)
    return entry


def frame_free(t, depth=0):
    """Is the value of term t provably not derived from this function's
    frame?  Constants, symbols, narrow values and arithmetic on free values;
    a block-entry variable when P's analysis says its register is free."""
    if depth > 12:
        return False
    op = t.op
    if op in ('c', 'sym'):
        return True
    if op in ('sx', 'zx'):
        return True
    if op == 'ld':
        return t.a[1] in (1, 2)
    if op == 'in':
        key = t.a[1]
        if FREE is None or not (isinstance(key, tuple) and key[0] == 0 and
                                isinstance(key[1], int)):
            return False
        st = FREE.get(t.a[0])
        return bool(st and st[key[1]])
    if op in ('sum', 'sll', 'srl', 'sra', 'and', 'or', 'xor', 'nor', 'neg', 'slt', 'sltu',
              'lo', 'hi', 'mult', 'multu', 'div', 'divu', 'sllv', 'srlv', 'srav'):
        return all(frame_free(x, depth + 1) for x in t.a if isinstance(x, T))
    return False


def _rng(t, depth=0):
    """(lo, hi) bounds of a term's value as a mathematical integer (its
    32-bit pattern read signed), or None when unknown.  Only exact cases:
    constants, narrow loads and extensions, a mask, a shift and a sum of
    bounded terms."""
    if depth > 8:
        return None
    op = t.op
    if op == 'in':
        # a block-entry variable whose class holds P register p: P's own
        # interval analysis bounds that register at that block's entry
        key = t.a[1]
        if RANGES is not None and isinstance(key, tuple) and key[0] == 0 and \
                isinstance(key[1], int) and key[1] < 32:
            st = RANGES.get(t.a[0])
            return st[key[1]] if st else None
        return None
    if op == 'c':
        v = t.a[0] & 0xFFFFFFFF
        v = v - (1 << 32) if v & 0x80000000 else v
        return v, v
    if op in ('ld', 'sx', 'zx'):
        size, signed = (t.a[1], t.a[2]) if op == 'ld' else (t.a[0], op == 'sx')
        if size in (1, 2):
            b = 8 * size
            return (-(1 << (b - 1)), (1 << (b - 1)) - 1) if signed else (0, (1 << b) - 1)
        return None
    if op == 'and':
        best = None
        for x in t.a:
            if isinstance(x, T) and x.op == 'c' and 0 <= (x.a[0] & 0xFFFFFFFF) < (1 << 31):
                best = (0, x.a[0] & 0xFFFFFFFF)
        return best
    if op == 'sll' and isinstance(t.a[1], int):
        r = _rng(t.a[0], depth + 1)
        if r and r[0] >= 0 and (r[1] << t.a[1]) < (1 << 31):
            return r[0] << t.a[1], r[1] << t.a[1]
        return None
    if op == 'sum':
        lo = hi = t.a[-1] - (1 << 32) if t.a[-1] & 0x80000000 else t.a[-1]
        for x in t.a[:-1]:
            r = _rng(x, depth + 1)
            if r is None:
                return None
            lo, hi = lo + r[0], hi + r[1]
        if -(1 << 31) <= lo and hi < (1 << 31):
            return lo, hi
        return None
    return None


def _lin(t):
    """An address term as const + sum(coeff * atom), coefficients mod 2^32:
    sums, constant shifts left and negation distribute, and a low-bit mask
    `and(x, 2^k-1)` whose operand provably lies in [0, 2^k-1] is x itself;
    anything else is an atom (any value).  Exact: every MIPS address
    computation is mod 2^32."""
    atoms = {}
    const = 0

    def go(x, m):
        nonlocal const
        if x.op == 'c':
            const += m * x.a[0]
        elif x.op == 'sum':
            for y in x.a[:-1]:
                go(y, m)
            const += m * x.a[-1]
        elif x.op == 'sll' and isinstance(x.a[1], int):
            go(x.a[0], m << x.a[1])
        elif x.op == 'neg':
            go(x.a[0], -m)
        elif x.op == 'lo' and len(x.a) == 1 and isinstance(x.a[0], T) and \
                x.a[0].op in ('mult', 'multu') and \
                sum(1 for y in x.a[0].a if isinstance(y, T) and y.op == 'c') == 1 and \
                len(x.a[0].a) == 2:
            # LO of a product by a constant: the low 32 bits of x*c, which is
            # c*x mod 2^32 for mult and multu alike
            cy = [y for y in x.a[0].a if y.op == 'c'][0]
            oy = [y for y in x.a[0].a if y is not cy][0]
            go(oy, m * cy.a[0])
        else:
            if x.op == 'and' and len(x.a) == 2:
                cs = [y for y in x.a if isinstance(y, T) and y.op == 'c']
                ot = [y for y in x.a if not (isinstance(y, T) and y.op == 'c')]
                if len(cs) == 1 and len(ot) == 1:
                    mask = cs[0].a[0] & 0xFFFFFFFF
                    r = _rng(ot[0])
                    if mask & (mask + 1) == 0 and r and r[0] >= 0 and r[1] <= mask:
                        go(ot[0], m)
                        return
            k = id(x)
            x0, c0 = atoms.get(k, (x, 0))
            atoms[k] = (x0, c0 + m)
    go(t, 1)
    return const & 0xFFFFFFFF, {k: (x, c & 0xFFFFFFFF) for k, (x, c) in atoms.items()}


def modular_disjoint(e1, e2):
    """True when the two accesses can NEVER overlap.  Their address
    difference is const + sum(k_i * atom_i) mod 2^32.  (1) Modulo g, the
    largest power of two dividing every k_i (g divides 2^32), it is the
    constant: if no offset at which the byte ranges meet is congruent to it,
    they are disjoint for every value of every atom.  (2) With ONE atom of
    known range [lo, hi], k*a = R (mod 2^32) is solved exactly for each
    meeting offset; no solution in range means disjoint."""
    a1, a2 = getattr(e1, 'addr', None), getattr(e2, 'addr', None)
    if a1 is None or a2 is None:
        return False
    o1, s1 = _span(e1)
    o2, s2 = _span(e2)
    c1, m1 = _lin(a1)
    c2, m2 = _lin(a2)
    ks = []
    for k in set(m1) | set(m2):
        v = ((m1[k][1] if k in m1 else 0) - (m2[k][1] if k in m2 else 0)) & 0xFFFFFFFF
        if v:
            ks.append((v, (m1.get(k) or m2.get(k))[0]))
    g = 1 << 32
    for v, _ in ks:
        g = min(g, v & -v)
    c = (c1 + o1 - c2 - o2) & 0xFFFFFFFF
    # overlap iff (A1+o1) - (A2+o2) lies in (-s1, s2)
    window = range(-s1 + 1, s2)
    if not any((w - c) % g == 0 for w in window):
        return True
    if len(ks) == 1:
        k, atom = ks[0]
        r = _rng(atom)
        if r is None:
            return False
        return not _affine_meets(k, c, r[0], r[1], window)
    return False


def _affine_meets(k, c, lo, hi, window):
    """Is there an integer a in [lo, hi] and w in window with
    k*a + c = w (mod 2^32)?  Exact (k != 0 mod 2^32)."""
    k &= 0xFFFFFFFF
    g = k & -k
    mod = (1 << 32) // g
    kk = (k // g) % mod
    inv = pow(kk, -1, mod) if mod > 1 else 0
    for w in window:
        rr = (w - c) & 0xFFFFFFFF
        if rr % g:
            continue
        a0 = ((rr // g) * inv) % mod if mod > 1 else 0
        j = -((a0 - lo) // mod)            # least a = a0 + j*mod >= lo
        if a0 + j * mod <= hi:
            return True
    return False


def may_alias(e1, e2, escaped):
    c1, c2 = e1.cls, e2.cls
    if SAVE_BYTES and (is_save(e1) and c2[0] != 'F' or is_save(e2) and c1[0] != 'F'):
        return False                      # the save area is in no C object
    if c1[0] in ('A', 'P', 'G', 'GX') and c2[0] in ('A', 'P', 'G', 'GX') and \
            modular_disjoint(e1, e2):
        return False                      # never overlap, for any value
    o1, s1 = _span(e1)
    o2, s2 = _span(e2)
    t1, t2 = c1[0], c2[0]
    if t1 in ('O', 'OX') or t2 in ('O', 'OX'):
        if t1 in ('O', 'OX') and t2 in ('O', 'OX'):
            if c1[1] != c2[1]:
                return False
            if t1 == 'O' and t2 == 'O':
                return _ov(c1[2] + o1, s1, c2[2] + o2, s2)
            return True
        other = c2 if t1 in ('O', 'OX') else c1
        return other[0] not in ('G', 'GX', 'A', 'F')
    if t1 > t2:
        c1, c2, t1, t2, o1, s1, o2, s2 = c2, c1, t2, t1, o2, s2, o1, s1
    if t1 == 'F' and t2 == 'F':
        return _ov(c1[2] + o1, s1, c2[2] + o2, s2)
    if 'F' in (t1, t2) or 'FX' in (t1, t2):
        other = c2 if t1 in ('F', 'FX') else c1
        fc = c1 if t1 in ('F', 'FX') else c2
        if other[0] in ('F', 'FX'):
            return True                   # FX against frame
        if other[0] in ('G', 'GX', 'A'):
            return False                  # the stack is not a global
        # pointer against frame: only an escaped frame (or the caller-owned
        # incoming area, conservatively) can be pointed to
        if fc[0] == 'F' and fc[2] >= 0:
            return True
        if escaped and fc[0] == 'F' and FREE is not None:
            oe = e1 if e1.cls is other else e2
            if getattr(oe, 'addr', None) is not None and frame_free(oe.addr):
                return False              # not derived from this frame
        return escaped
    if t1 == 'A' and t2 == 'A':
        return _ov(c1[1] + o1, s1, c2[1] + o2, s2)
    # provenance (C semantics: a pointer formed from one named object only
    # reaches that object; distinct global symbols are distinct objects)
    if t1 in ('A', 'P') and t2 in ('A', 'P') and len(c1) > 3 - (t1 == 'A') and \
            len(c2) > 3 - (t2 == 'A'):
        p1, p2 = c1[-1], c2[-1]
        if p1 is not None and p2 is not None and p1 != p2:
            if not (t1 == 'P' and t2 == 'P' and (c1[1] is c2[1] or c1[1] == c2[1])):
                return False
    if t1 == 'A' and t2 == 'G':
        g = name_addr(c2[1])
        return g is None or _ov(c1[1] + o1, s1, g + c2[2] + o2, s2)
    if t1 == 'G' and t2 == 'G':
        if c1[1] == c2[1]:
            return _ov(c1[2] + o1, s1, c2[2] + o2, s2)
        g1, g2 = name_addr(c1[1]), name_addr(c2[1])
        if g1 is None or g2 is None:
            return True
        return _ov(g1 + c1[2] + o1, s1, g2 + c2[2] + o2, s2)
    if t1 == 'P' and t2 == 'P':
        if c1[1] is c2[1] or c1[1] == c2[1]:
            return _ov(c1[2] + o1, s1, c2[2] + o2, s2)
        return True
    return True


# ---------------------------------------------------------------------------
# Symbolic execution of one block on one side
# ---------------------------------------------------------------------------

class Ctx:
    """Per-proof shared state.  Equality is IDENTITY of hash-consed terms:
    there is no frame map to unify through (mode A keeps frame cells as
    pseudo-registers related like registers; mode B forbids any remap)."""

    def __init__(self, arity, volatile, void, ret64, mode):
        self.bank = TermBank()
        self.al = Algebra(self.bank)
        self.arity = arity or {}
        self.volatile = volatile or set()
        self.void = void
        self.ret64 = ret64
        self.mode = mode                   # 'A' cells, 'B' shared frame memory
        self.escaped = mode in ('B', 'C')
        self.needs = set()                 # mode B: (block pair, byte)
        self.nonvoid = {}                  # callee -> True when it returns a value
        self.volatile_addrs = [a for a in (name_addr(v) for v in self.volatile
                                           if v.startswith(('D_', 'func_')) or True)
                               if a is not None] if self.volatile else []

    def junk(self, side, *tag):
        return self.bank.mk('junk', side, *tag)

    # --- provenance --------------------------------------------------------
    def set_symbols(self, names):
        import bisect
        self.symaddrs = sorted({a for a in (name_addr(n) for n in names) if a is not None
                                and 0x80000000 <= a < 0x80200000})
        self._bis = bisect
        self.prov_var = {}

    def obj_of(self, v):
        sa = getattr(self, 'symaddrs', None)
        if not sa or not (0x80000000 <= v < 0x80200000):
            return None
        j = self._bis.bisect_right(sa, v) - 1
        return sa[j] if j >= 0 else None

    def prov(self, t, depth=0):
        if t.op == 'c':
            return self.obj_of(t.a[0])
        if t.op in ('in', 'junk'):
            return self.prov_var.get(id(t))
        if t.op == 'sum' and depth < 3:
            cands = set()
            k = t.a[-1]
            if 0x80000000 <= k < 0x80200000:
                o = self.obj_of(k)
                if o is not None:
                    cands.add(o)
            for x in t.a[:-1]:
                if x.op in ('in', 'junk', 'sum'):
                    o = self.prov(x, depth + 1)
                    if o is not None:
                        cands.add(o)
            return cands.pop() if len(cands) == 1 else None
        return None


def eq(p, s):
    return p is s


# --- static frame facts ---------------------------------------------------

def sp_offsets(f):
    """{word index: $sp offset (from the entry $sp) BEFORE the word}."""
    at = {}
    spin = {0: 0}
    work = [0]
    seen = set()
    while work:
        k = work.pop()
        if k in seen:
            continue
        seen.add(k)
        cur = spin[k]
        s, e, _ = f.blocks[k]
        for i in range(s, e):
            at[i] = cur
            d = f.ins[i]
            if SP in d.defs:
                if d.kind == 'alui' and d.name == 'addiu' and d.rs == SP and d.rel is None:
                    cur += _sx16(d.imm)
                else:
                    raise Unproven('frame', '%s: $sp written by %s at %d' % (
                        f.side, disasm(d), i))
        for _, sb in f.succs(k):
            if sb is None:
                continue
            o = spin.get(sb)
            if o is None:
                spin[sb] = cur
                work.append(sb)
            elif o != cur:
                raise Unproven('frame', '%s: $sp offset differs at a join (%d vs %d)'
                               % (f.side, o, cur))
    return at, spin


def frame_escapes(f):
    """Words that use $sp other than as a load/store base or the frame
    adjust (its address is taken)."""
    out = []
    for d in f.ins:
        if SP not in d.uses:
            continue
        if d.kind == 'load' and d.rs == SP:
            continue
        if d.kind == 'store' and d.rs == SP and d.rt != SP:
            continue
        if d.kind == 'alui' and d.name == 'addiu' and d.rs == SP and d.rt == SP:
            continue
        out.append(d.i)
    return out


SAVE_BYTES = None    # set per mode-B proof: frame bytes that only hold callee-saved registers

CALLEE_SAVED = (16, 17, 18, 19, 20, 21, 22, 23, 30, RA)


def save_slots(f):
    """Mode B: frame bytes (entry-$sp relative) of the callee-saved
    register SAVE AREA -- words written exactly once, in the entry block,
    with the entry value of $s0-$s8/$ra (no earlier entry-block word defines
    that register), and otherwise only reloaded into the same register.
    No address is formed inside them.  A correct compiler never lets a C
    object overlap such a word (a legal store through the object would
    corrupt the caller's register), so no escaped-object access reaches it.
    Returns a frozenset of bytes (empty when there are none)."""
    at = f.sp_at[0]
    s0, e0, _ = f.blocks[0]
    acc = collections.defaultdict(list)    # word offset -> [(kind, reg, i)]
    bad = set()
    for d in f.ins:
        if d.i not in at:
            continue
        if d.kind in ('load', 'store') and d.rs == SP:
            if d.rel is not None:
                return frozenset()
            k = at[d.i] + _sx16(d.imm)
            size = _STORES[d.op][1] if d.kind == 'store' else _LOADS[d.op][1]
            if size != 4 or (k & 3) or d.name not in ('lw', 'sw'):
                bad.update(range((k & ~3), k + size))
                continue
            acc[k].append((d.kind, d.rt, d.i))
    try:
        forms = formations(f)
    except Unproven:
        return frozenset()
    for _, k, _ in forms:
        if k is None:
            return frozenset()
        bad.update(range(k - 3, k + 4))
    out = set()
    for k, lst in acc.items():
        if k >= 0 or any(b in bad for b in range(k, k + 4)):
            continue
        regs = {r for _, r, _ in lst}
        if len(regs) != 1:
            continue
        r = regs.pop()
        if r not in CALLEE_SAVED:
            continue
        sts = [i for kind, _, i in lst if kind == 'store']
        if len(sts) != 1 or not s0 <= sts[0] < e0:
            continue
        if any(r in f.ins[j].defs for j in range(s0, sts[0])):
            continue
        out.update(range(k, k + 4))
    return frozenset(out)


def is_save(e):
    """An access lying wholly in the mode-B register save area."""
    if not SAVE_BYTES or e.cls[0] != 'F':
        return False
    o, s_ = _span(e)
    return all(b in SAVE_BYTES for b in range(e.cls[2] + o, e.cls[2] + o + s_))


def obj_at(objects, k, size, side, i):
    """(object index, offset) when [k, k+size) lies in an escaped frame
    object; None when disjoint from all of them; Unproven on a straddle."""
    for j, (b, sz) in enumerate(objects):
        if b <= k and k + size <= b + sz:
            return j, k - b
        if k < b + sz and b < k + size:
            raise Unproven('escape-layout', '%s: frame access [%d,+%d) at %d straddles the '
                           'escaped object [%d,+%d)' % (side, k, size, i, b, sz))
    return None


def formations(f):
    """[(word, frame offset, kind)] of every word that forms a frame
    ADDRESS ($sp used other than as a load/store base or the frame
    adjust)."""
    at = f.sp_at[0]
    out = []
    for i in frame_escapes(f):
        d = f.ins[i]
        if i not in at:
            continue                       # unreachable word
        if d.kind == 'alui' and d.name == 'addiu' and d.rs == SP and d.rel is None:
            out.append((i, at[i] + _sx16(d.imm), 'addiu'))
        elif d.kind == 'alur' and d.name in ('addu', 'or') and SP in (d.rs, d.rt):
            # sp + index: the object is named by the first access through
            # the result (its displacement), if there is one nearby
            k = None
            r = d.rd
            for j in range(i + 1, min(i + 12, f.n)):
                e = f.ins[j]
                if e.kind in ('load', 'store') and e.rs == r:
                    k = at[i] + _sx16(e.imm)
                    break
                if r in e.defs or is_xfer(e):
                    break
            out.append((i, k, d.name))
        else:
            raise Unproven('escape-unsupported', '%s: %s at %d uses $sp' % (
                f.side, disasm(d), i))
    return out


class Cells:
    """Mode A: the frame as pseudo-registers.  Overlapping direct accesses
    share one cell; a cell's content is a term whose low bytes are the
    cell's bytes."""

    def __init__(self, f, at, objects=None):
        acc = []
        self.acc = {}
        self.obj_acc = {}                  # mode C: word -> (object, offset, size)
        self.uacc = {}                     # lwl/lwr/swl/swr: word -> address offset
        for d in f.ins:
            if d.kind in ('load', 'store') and d.rs == SP and d.i in at:
                if d.rel is not None:
                    raise Unproven('reloc', '%s: relocation on a $sp access at %d' % (
                        f.side, d.i))
                imm = _sx16(d.imm)
                if imm < 0:
                    raise Unproven('frame-bounds', '%s: %s at %d accesses below $sp' % (
                        f.side, disasm(d), d.i))
                size = (_LOADS[d.op][1] if d.kind == 'load' else _STORES[d.op][1])
                k = at[d.i] + imm
                rk, rs_ = k, size
                if d.name in ('lwl', 'swl'):       # bytes (k & ~3) .. k
                    rk, rs_ = k & ~3, k - (k & ~3) + 1
                elif d.name in ('lwr', 'swr'):     # bytes k .. (k | 3)
                    rs_ = 4 - (k & 3)
                hit = obj_at(objects, rk, rs_, f.side, d.i) if objects else None
                if hit is not None:
                    self.obj_acc[d.i] = (hit[0], k - (rk - hit[1]), size)
                    continue
                if d.name in ('lwl', 'lwr', 'swl', 'swr'):
                    acc.append((rk, rs_))
                    self.acc[d.i] = (rk, rs_)
                    self.uacc[d.i] = k
                    continue
                acc.append((k, size))
                self.acc[d.i] = (k, size)
        acc.sort()
        cells = []
        for k, sz in acc:
            if cells and k < cells[-1][0] + cells[-1][1]:
                c0, s0 = cells[-1]
                cells[-1] = (c0, max(s0, k + sz - c0))
            else:
                cells.append((k, sz))
        self.cells = cells
        self.starts = [c[0] for c in cells]

    def find(self, k, size):
        import bisect
        j = bisect.bisect_right(self.starts, k) - 1
        if j < 0:
            return None
        c0, s0 = self.cells[j]
        if k >= c0 and k + size <= c0 + s0:
            return j, k - c0
        return None

    def overlapping(self, k, size):
        return [j for j, (c0, s0) in enumerate(self.cells) if c0 < k + size and k < c0 + s0]


# --- block execution ------------------------------------------------------

class BlockRun:
    __slots__ = ('regs', 'cells', 'events', 'cond', 'jt_target', 'ret_ra', 'calls',
                 'sp_out', 'zones', 'old', 'fr_values', 'cell_stores', 'ncop')


class Executor:
    def __init__(self, ctx, f, cells=None, objs=None):
        self.objs = objs
        self.cx = ctx
        self.al = ctx.al
        self.f = f
        self.side = f.side
        self.C = cells

    def reloc_imm(self, d, base_term):
        al, mk = self.al, self.cx.bank.mk
        r = d.rel
        imm = _sx16(d.imm)
        if r is None:
            return al.add(base_term, al.c(imm))
        typ, name = r[0], r[1]
        if typ != R_MIPS_LO16:
            raise Unproven('reloc', '%s: relocation type %d on %s at %d' % (
                self.side, typ, d.name, d.i))
        a_ = name_addr(name)
        if a_ is not None:
            # the linked low half: low 16 bits of (S + addend), sign-extended
            return al.add(base_term, al.c(_sx16((a_ + imm) & 0xFFFF)))
        terms, k = (list(base_term.a[:-1]), base_term.a[-1]) if base_term.op == 'sum' \
            else ([base_term], 0)
        his = [x for x in terms if x.op == 'hi16' and x.a[0] == name]
        if len(his) == 1:
            h = his[0]
            terms.remove(h)
            terms.append(mk('sym', name))
            return al._mksum(terms, k + (h.a[1] << 16) + imm)
        return al.add(base_term, mk('lo16', name, imm))

    # cells (mode A) ------------------------------------------------------
    def cell_read(self, st, k, size, signed):
        al, mk = self.al, self.cx.bank.mk
        hit = self.C.find(k, size)
        if hit is None:
            return None
        j, off = hit
        c = st.cells[j]
        while True:
            if c.op == 'ins':
                base, o2, s2, v = c.a
                if (o2, s2) == (off, size):
                    return al.ext(size, signed, v)
                if o2 >= off + size or off >= o2 + s2:
                    c = base
                    continue
            break
        if off == 0:
            return al.ext(size, signed, c)
        return mk('extb', c, off, size, signed)

    def cell_write(self, st, k, size, v):
        mk = self.cx.bank.mk
        if size == 4:
            st.cell_stores.add(k)
        j, off = self.C.find(k, size)
        c0, s0 = self.C.cells[j]
        if off == 0 and size == s0:
            st.cells[j] = v
            return
        cur = st.cells[j]
        if cur.op == 'ins' and cur.a[1:3] == (off, size):
            cur = cur.a[0]
        st.cells[j] = mk('ins', cur, off, size, v)

    # memory (mode B frame, and all non-frame memory) ---------------------
    def load_value(self, st, addr, size, signed, name, pos, blk, upto=None):
        cx, al = self.cx, self.al
        e = Ev()
        e.kind, e.addr, e.cls, e.size, e.name, e.pos = 'ld', addr, aclass(addr), size, name, pos
        ctx = []
        fwd = None
        evs = st.events
        for q in range((len(evs) if upto is None else upto) - 1, -1, -1):
            x = evs[q]
            if x.kind not in ('st', 'call', 'clob'):
                continue
            if x.kind == 'call':
                if e.cls[0] == 'F' and e.cls[2] < 0 and (not cx.escaped or is_save(e)):
                    continue
                ctx.append(x)
                continue
            if not may_alias(e, x, cx.escaped):
                continue
            if x.kind == 'clob' and e.cls[0] != 'F':
                ctx.append(x)
                continue
            if x.kind == 'clob':
                prior = self.load_value(st, addr, size, signed, name, pos, blk, upto=q)
                z = cx.bank.mk('zoneld', x.term, e.cls[2] - x.sp, size, signed, prior)
                if not ctx:
                    return z
                ctx.reverse()
                return cx.bank.mk('ldz', addr, size, signed, z, *[y.term for y in ctx])
            if not ctx and x.addr is addr and x.size == size and \
                    x.name not in ('swl', 'swr') and name not in ('lwl', 'lwr'):
                fwd = x
                break
            ctx.append(x)
        if fwd is not None:
            return al.ext(size, signed, fwd.val)
        ctx.reverse()
        ctx = canon_events(ctx, cx.escaped)
        if name in ('lwl', 'lwr'):
            old = st.old
            mate = 'lwr' if name == 'lwl' else 'lwl'
            lo = al.add(addr, al.c(-3)) if name == 'lwl' else addr
            if old is not None and old.op == 'lwx' and old.a[0] == mate and \
                    old.a[1] is (al.add(lo, al.c(3)) if mate == 'lwl' else lo):
                # an lwl/lwr PAIR on one register: all four bytes come from
                # memory and the old register value is dead -- an unaligned
                # word load of [lo, lo+4)
                both = list(old.a[4:]) + [x.term for x in ctx]
                both = sorted(set(both), key=_key)
                t = cx.bank.mk('ulw', lo, blk, *both)
            else:
                t = cx.bank.mk('lwx', name, addr, old, blk, *[x.term for x in ctx])
        else:
            t = cx.bank.mk('ld', addr, size, signed, blk, *[x.term for x in ctx])
        if e.cls[0] in ('F', 'FX') and cx.mode == 'B':
            if e.cls[0] == 'FX':
                # a frame load at a DYNAMIC address may read any frame byte:
                # every byte must be valid (equal on both sides) here -- no
                # call zone clobbered before it in this block, and none
                # invalid at the block's entry (checked in check_mode_b)
                if any(x.kind in ('call', 'clob') for x in ctx):
                    raise Unproven('frame-uninit', '%s: dynamic frame load at %d after a call '
                                   'zone was clobbered' % (self.side, pos))
                cx.needs.add((blk, 'any'))
                return t
            k = e.cls[2]
            covered = set()
            for x in ctx:
                if x.kind == 'st' and x.cls[0] == 'F' and x.name in ('sb', 'sh', 'sw'):
                    covered.update(range(x.cls[2], x.cls[2] + x.size))
            o, s_ = _span(e)
            for b in range(k + o, k + o + s_):
                if b not in covered:
                    cx.needs.add((blk, b))
        return t

    def run(self, bidx, blk, entry_regs, entry_cells, spoff, st=None):
        """Execute block `bidx`; with `st`, CONTINUE that state (path mode:
        a path of blocks executed as one straight line)."""
        f, cx, al, mk = self.f, self.cx, self.al, self.cx.bank.mk
        s, e, term = f.blocks[bidx]
        if st is not None:
            regs = st.regs
            st.cond = st.jt_target = st.ret_ra = None
            ncop = st.ncop
        else:
            st = BlockRun()
            regs = list(entry_regs)
            regs[0] = al.ZERO
            regs[SP] = al.fr(None, spoff)
            st.regs = regs
            st.cells = list(entry_cells) if entry_cells is not None else None
            st.events = []
            st.cond = st.jt_target = st.ret_ra = None
            st.calls = []
            st.zones = []
            st.old = None
            st.fr_values = []
            st.cell_stores = set()
            ncop = [0]
            st.ncop = ncop
        i = s
        while i < e:
            d = f.ins[i]
            k = d.kind
            if k in ('br', 'j', 'jr'):
                if k == 'br' and not is_uncond_br(d):
                    a = regs[d.rs]
                    if d.name in ('beq', 'bne'):
                        st.cond = mk('eq' if d.name == 'beq' else 'ne',
                                     *sorted((a, regs[d.rt]), key=_key))
                    else:
                        st.cond = mk(d.name, a)
                elif k == 'jr':
                    if d.rs == RA:
                        st.ret_ra = regs[RA]
                    else:
                        st.jt_target = regs[d.rs]
                if i + 1 < e:
                    self.step(st, f.ins[i + 1], blk, ncop)
                i += 2
                continue
            if k in ('jal', 'jalr'):
                if k == 'jal':
                    r = d.rel
                    if r is not None and r[0] == R_MIPS_26:
                        tgt = mk('fn', r[1], d.w & 0x3FFFFFF)
                    elif r is None:
                        tgt = mk('fnabs', d.w & 0x3FFFFFF)
                    else:
                        raise Unproven('reloc', '%s: jal relocation %r' % (self.side, r))
                    name = r[1] if r else None
                else:
                    if d.rd != RA:
                        raise Unproven('unsupported', '%s: jalr links to $%s' % (
                            self.side, RNAME[d.rd]))
                    tgt = regs[d.rs]
                    name = None
                if i + 1 >= e:
                    raise Unproven('cfg', '%s: call at %d without a delay slot in its block'
                                   % (self.side, i))
                slot = f.ins[i + 1]
                if RA in slot.uses or RA in slot.defs:
                    raise Unproven('unsupported', '%s: call delay slot touches $ra (%d)'
                                   % (self.side, i + 1))
                # a load in a call's delay slot: the callee's first word sees
                # the OLD value of its destination (MIPS I load delay)
                slotload = None
                if slot.kind == 'load' or (slot.kind in ('cop0', 'cop2') and
                                           slot.name in ('mfc0', 'mfc2', 'cfc2')):
                    if slot.rt:
                        slotload = (slot.rt, regs[slot.rt])
                self.step(st, slot, blk, ncop)
                self.call(st, d, tgt, name, blk)
                st.calls[-1].extra = st.calls[-1].extra + (slotload,)
                i += 2
                continue
            self.step(st, d, blk, ncop)
            i += 1
        st.sp_out = regs[SP]
        if cx.mode == 'C':
            for r in range(1, NREG):
                if r != SP:
                    regs[r] = self.tryobj(regs[r])
            if st.cells is not None:
                st.cells = [self.tryobj(c) for c in st.cells]
            if st.cond is not None:
                st.cond = self.tryobj(st.cond, deep=True)
        return st

    def tryobj(self, v, deep=False):
        """Mode C: a frame-address value as (object, offset) where it is
        one; otherwise unchanged (a raw frame address compares only to the
        SAME raw address, which is value equality)."""
        if self.cx.mode != 'C' or not _has_fr(v):
            return v
        try:
            return self.objify(v, None)
        except Unproven:
            if deep and v.op in ('eq', 'ne'):
                return self.cx.bank.mk(v.op, *sorted(
                    (self.tryobj(x) for x in v.a), key=_key))
            return v

    def call(self, st, d, tgt, name, blk):
        cx, al, mk = self.cx, self.al, self.cx.bank.mk
        regs = st.regs
        n = len(st.calls)
        ar_raw = cx.arity.get(name) if name else cx.arity.get('<jalr>')
        ar = None if ar_raw == 'agg?' else ar_raw     # 'agg?' is refused at the check
        sp = regs[SP]
        spk = sp.a[1]
        if ar is not None:
            nst = max(0, ar - 4)
        else:
            # unknown arity: the stack arguments actually passed -- words
            # stored at sp+16, sp+20, ... in this block before the call (C:
            # a callee cannot have more parameters than the call passes)
            nst = 0
            while nst < 8 and self.stored_in_block(st, spk + 16 + 4 * nst):
                nst += 1
        stack_vals = []
        for q in range(MAX_STACK_ARGS):    # argument words 5.. from the outgoing area
            kq = spk + 16 + 4 * q
            if cx.mode in ('A', 'C'):
                v = self.cell_read(st, kq, 4, True)
                if v is None and cx.mode == 'C' and self.objs:
                    # mode C: an argument word inside an escaped object (the
                    # outgoing area built through a pointer) is memory
                    try:
                        hit = obj_at(self.objs, kq, 4, self.side, d.i)
                    except Unproven:
                        hit = None
                    if hit is not None:
                        v = self.load_value(st, al.add(mk('obj', hit[0]), al.c(hit[1])), 4,
                                            True, 'lw', d.i, blk)
                stack_vals.append(v if v is not None else cx.junk(self.side, 'nostack', blk, n, q))
            elif q < nst:
                # mode B: only the words the callee reads (the ones compared)
                # are frame loads; reading the rest would demand validity of
                # bytes nothing observes
                stack_vals.append(self.load_value(st, al.fr(None, kq), 4, True, 'lw', d.i, blk))
            else:
                stack_vals.append(cx.junk(self.side, 'nostack', blk, n, q))
        ct = mk('call', tgt, blk, n)
        ev = Ev()
        ev.kind, ev.pos, ev.term, ev.name = 'call', d.i, ct, name
        ev.extra = (tgt, [self.tryobj(regs[q]) for q in (4, 5, 6, 7)],
                    [self.tryobj(x) for x in stack_vals], ar)
        ev.regs_at = [self.tryobj(r) for r in regs]   # every register at the call
        ev.addr = ev.cls = None
        ev.size = 0
        ev.sp = spk
        self.emit(st, ev)
        st.calls.append(ev)
        ev.extra = ev.extra[:3] + (ar_raw, nst)
        zsize = 16 + 4 * nst
        st.zones.append((spk, zsize))
        # the callee's effects are a function of its inputs (checked equal:
        # target, arguments, memory) and of what it leaves untouched -- the
        # PRIOR contents -- so each clobbered location carries its prior
        if cx.mode in ('A', 'C'):
            for j in self.C.overlapping(spk, zsize):
                c0, s0 = self.C.cells[j]
                st.cells[j] = mk('zone', blk, n, c0 - spk, s0, st.cells[j])
        else:
            z = Ev()
            z.kind, z.pos, z.term, z.name = 'clob', d.i, mk('zone', blk, n), 'clob'
            z.sp = spk
            z.addr = al.fr(None, spk)
            z.cls = ('F', None, spk)
            z.size = zsize
            z.val = None
            self.emit(st, z)
        prior = list(regs)
        for r in CALLER_CLOBBER:
            regs[r] = mk('clob', blk, n, r, prior[r])
        # $v0: a callee known to return a value writes it; otherwise what it
        # leaves there may be the prior value
        nvk = cx.nonvoid.get(name) if name else (
            cx.nonvoid.get('<jalr>') if d.kind == 'jalr' else None)
        if nvk:
            regs[V0] = mk('ret', blk, n, 0)
        elif name and name in getattr(cx, 'voidcallees', ()):
            # a callee DECLARED void: same meaning as 'ret' with a prior; the
            # distinct op records that what it leaves in $v0 is no C value
            # (used by the indeterminate-return rule)
            regs[V0] = mk('retv', blk, n, 0, prior[V0])
        else:
            regs[V0] = mk('ret', blk, n, 0, prior[V0])
        regs[V1] = mk('ret', blk, n, 1, prior[V1])
        regs[RA] = mk('retaddr', d.i + 2)   # the code address after the call's slot

    def stored_in_block(self, st, k):
        if self.cx.mode in ('A', 'C'):
            return k in st.cell_stores
        return any(x.kind == 'st' and x.cls[0] == 'F' and x.cls[2] == k and x.size == 4
                   for x in st.events)

    def step(self, st, d, blk, ncop):
        cx, al, mk = self.cx, self.al, self.cx.bank.mk
        regs = st.regs
        k = d.kind
        val = None
        dst = None
        if d.w == 0:
            return
        if k == 'shift':
            val, dst = al.shift(d.name, regs[d.rt], d.sa), d.rd
        elif k == 'alur':
            a, b = regs[d.rs], regs[d.rt]
            nm = d.name
            if nm == 'addu':
                val = al.add(a, b)
            elif nm == 'subu':
                val = al.sub(a, b)
            elif nm in ('and', 'or', 'xor', 'nor'):
                val = al.logic(nm, a, b)
            elif nm in ('slt', 'sltu'):
                val = al.slt(nm, a, b)
            elif nm in ('sllv', 'srlv', 'srav'):
                val = al.vshift(nm, b, a)
            elif nm in ('add', 'sub'):
                val = mk('trap' + nm, *((a, b) if nm == 'sub' else sorted((a, b), key=_key)))
                self.barrier(st, d, mk('trapev', val))
            dst = d.rd
        elif k == 'alui':
            a = regs[d.rs]
            nm = d.name
            imm = d.imm
            if nm == 'addiu':
                val = self.reloc_imm(d, a)
            elif d.rel is not None:
                raise Unproven('reloc', '%s: relocation on %s at %d' % (self.side, nm, d.i))
            elif nm == 'addi':
                val = mk('trapadd', *sorted((a, al.c(_sx16(imm))), key=_key))
                self.barrier(st, d, mk('trapev', val))
            elif nm == 'slti':
                val = al.slt('slt', a, al.c(_sx16(imm)))
            elif nm == 'sltiu':
                val = al.slt('sltu', a, al.c(_sx16(imm)))
            elif nm == 'andi':
                val = al.logic('and', a, al.c(imm))
            elif nm == 'ori':
                val = al.logic('or', a, al.c(imm))
            elif nm == 'xori':
                val = al.logic('xor', a, al.c(imm))
            dst = d.rt
        elif k == 'lui':
            r = d.rel
            if r is None:
                val = al.c(d.imm << 16)
            elif r[0] == R_MIPS_HI16:
                a_ = name_addr(r[1])
                if a_ is not None and len(r) > 2 and r[2] is not None:
                    # the linked value (GNU ld: carry from the paired LO16)
                    val = al.c(((a_ + (d.imm << 16) + r[2] + 0x8000) >> 16) << 16)
                else:
                    val = mk('hi16', r[1], d.imm)
            else:
                raise Unproven('reloc', '%s: lui relocation %r at %d' % (self.side, r, d.i))
            dst = d.rt
        elif k == 'load':
            size = _LOADS[d.op][1]
            signed = _LOADS[d.op][2]
            dst = d.rt
            if cx.mode in ('A', 'C') and d.rs == SP and d.i in self.C.acc:
                kk, sz2 = self.C.acc[d.i]
                if d.name in ('lwl', 'lwr'):
                    # merge the bytes [kk, kk+sz2) into the old register value
                    # by the same (deterministic) rule on both sides
                    byt = self.cell_read(st, kk, sz2, False)
                    if sz2 == 4:           # aligned: all four bytes, old value dead
                        val = byt
                    else:
                        old = regs[d.rt]
                        mate = 'lwr' if d.name == 'lwl' else 'lwl'
                        if old.op == mate and len(old.a) == 5 and old.a[4] + sz2 == 4 and (
                                old.a[3] + old.a[4] == kk or kk + sz2 == old.a[3]):
                            # an lwl/lwr pair: four bytes from the frame
                            parts = sorted([(old.a[3], old.a[2]), (kk, byt)], key=lambda x: x[0])
                            val = mk('ulwc', parts[0][1], parts[1][1])
                        else:
                            val = mk(d.name, old, byt, kk, sz2)
                else:
                    val = self.cell_read(st, kk, size, signed)
            else:
                addr = self.address(st, d)
                st.old = regs[d.rt]
                val = self.load_value(st, addr, size, signed, d.name, d.i, blk)
                e = Ev()
                e.kind, e.pos, e.addr, e.cls, e.size, e.name = \
                    'ld', d.i, addr, aclass(addr), size, d.name
                e.term, e.val, e.signed = val, val, signed
                if d.name in ('lwl', 'lwr'):
                    # the memory access, not the merged register value
                    e.term = mk('lwev', d.name, addr, blk)
                self.emit(st, e)
        elif k == 'store':
            size = _STORES[d.op][1]
            v = regs[d.rt]
            if cx.mode in ('A', 'C') and d.rs == SP and d.i in self.C.acc:
                kk, sz2 = self.C.acc[d.i]
                if d.name in ('swl', 'swr'):
                    # the bytes written are a fixed function of (reg, k & 3)
                    self.cell_write(st, kk, sz2, v if sz2 == 4 else
                                    mk(d.name, v, self.C.uacc[d.i] & 3))
                else:
                    self.cell_write(st, kk, size, v)
            else:
                addr = self.address(st, d)
                v = self.tryobj(v)
                e = Ev()
                e.kind, e.pos, e.addr, e.cls, e.size, e.name, e.val = \
                    'st', d.i, addr, aclass(addr), size, d.name, v
                e.term = mk('st', addr, size, v, d.name)
                self.emit(st, e)
        elif k == 'mfhl':
            val, dst = regs[d.uses[0]], d.rd
        elif k == 'mthl':
            regs[d.defs[0]] = regs[d.rs]
        elif k == 'muldiv':
            a, b = regs[d.rs], regs[d.rt]
            if d.name in ('mult', 'multu'):
                a, b = sorted((a, b), key=_key)
            m = mk(d.name, a, b)
            regs[HI] = mk('hi', m)
            regs[LO] = mk('lo', m)
        elif k in ('syscall', 'break'):
            self.barrier(st, d, mk(k, (d.w >> 6) & 0xFFFFF))
        elif k in ('cop0', 'cop2', 'lwc2', 'swc2'):
            n = ncop[0]
            ncop[0] += 1
            nm = d.name
            if nm in ('mfc0', 'mfc2', 'cfc2'):
                self.barrier(st, d, mk(nm, d.rd))
                val, dst = mk('copval', nm, blk, n, d.rd), d.rt
            elif nm in ('mtc0', 'mtc2', 'ctc2'):
                self.barrier(st, d, mk(nm, d.rd, regs[d.rt]))
            elif nm in ('cop2', 'rfe'):
                self.barrier(st, d, mk(nm, d.w & 0x1FFFFFF))
            elif d.rs == SP:
                raise Unproven('unsupported', '%s: %s on the frame' % (self.side, nm))
            elif nm == 'lwc2':
                addr = self.reloc_imm(d, regs[d.rs])
                v = self.load_value(st, addr, 4, True, 'lw', d.i, blk)
                e = Ev()
                e.kind, e.pos, e.addr, e.cls, e.size, e.name = \
                    'ld', d.i, addr, aclass(addr), 4, 'lw'
                e.term, e.val, e.signed = v, v, True
                self.emit(st, e)
                self.barrier(st, d, mk('lwc2', d.rt, v))
            elif nm == 'swc2':
                addr = self.reloc_imm(d, regs[d.rs])
                v = mk('copval', 'swc2', blk, n, d.rt)
                self.barrier(st, d, mk('swc2', d.rt))
                e = Ev()
                e.kind, e.pos, e.addr, e.cls, e.size, e.name, e.val = \
                    'st', d.i, addr, aclass(addr), 4, 'sw', v
                e.term = mk('st', addr, 4, v, 'sw')
                self.emit(st, e)
        else:
            raise Unproven('unsupported', '%s: cannot model %s (%08x) at %d' % (
                self.side, d.name, d.w, d.i))
        if dst:
            if dst != SP and val is not None and cx.mode != 'C' and _fr_leaves(val):
                st.fr_values.append((d.i, val))
            regs[dst] = val

    def address(self, st, d):
        if self.cx.mode == 'C' and d.rs == SP:
            j, off, _ = self.C.obj_acc[d.i]
            return self.al.add(self.cx.bank.mk('obj', j), self.al.c(off))
        addr = self.reloc_imm(d, st.regs[d.rs])
        if self.cx.mode == 'C' and _has_fr(addr):
            addr = self.objify(addr, d)
        self.check_frame_access(st, d, addr)
        return addr

    def objify(self, v, d):
        """Mode C: a frame address value -> (escaped object, offset)."""
        al, mk = self.al, self.cx.bank.mk
        terms, k = (list(v.a[:-1]), v.a[-1]) if v.op == 'sum' else ([v], 0)
        frs = [x for x in terms if x.op == 'fr']
        if len(frs) != 1 or any(_has_fr(x) for x in terms if x.op != 'fr'):
            raise Unproven('escape-unsupported', '%s: frame address in %s at %s' % (
                self.side, fmt(v), d.i if d else '?'))
        f = frs[0]
        kk = f.a[1]
        objs = self.objs
        for past in (False, True):         # one-past-the-end is a valid C pointer
            for j, (b, sz) in enumerate(objs):
                if b <= kk < b + sz or (past and kk == b + sz):
                    terms.remove(f)
                    return al._mksum(terms + [mk('obj', j)], k + (kk - b))
        if getattr(self.cx, 'area_obj', False) and len(objs) == 1 and len(terms) > 1:
            # the merged candidate-local object (setup _setup_area_object):
            # a frame address with a DYNAMIC part whose $sp-relative
            # constant lies outside it still points into it (C)
            b, _ = objs[0]
            terms.remove(f)
            return al._mksum(terms + [mk('obj', 0)], k + (kk - b))
        raise Unproven('escape-unknown', '%s: frame address %d at %s is in no escaped object'
                       % (self.side, kk, d.i if d else '?'))

    def check_frame_access(self, st, d, addr):
        if addr.op == 'fr':
            sp = st.regs[SP]
            if addr.a[1] < sp.a[1]:
                raise Unproven('frame-bounds', '%s: %s at %d accesses below $sp' % (
                    self.side, d.name, d.i))

    @staticmethod
    def emit(st, e):
        e.seq = len(st.events)             # EXECUTION order (delay slots resolved)
        st.events.append(e)

    def barrier(self, st, d, t):
        e = Ev()
        e.kind, e.pos, e.term, e.name = 'bar', d.i, t, d.name
        e.addr = e.cls = None
        e.size = 0
        self.emit(st, e)


def _is_const(t):
    if t.op in ('c', 'hi16'):
        return True
    if t.op == 'sum':
        return all(x.op in ('sym', 'hi16', 'lo16') for x in t.a[:-1])
    return False


def _has_fr(t, depth=0):
    """Does term t mention the frame anywhere (unbounded; a superset of
    _fr_leaves -- it also looks inside loads)?"""
    return _mentions_op(t, ('fr',))


# ---------------------------------------------------------------------------
# Liveness (P side) and the prover
# ---------------------------------------------------------------------------

def _exec_items(f, k, cells=None):
    """[(uses, defs)] of block k in EXECUTION order (delay slots resolved).
    Mode A: frame cells are nodes ('c', j) too."""
    s, e, term = f.blocks[k]
    out = []

    def item(d):
        uses, defs = list(d.uses), list(d.defs)
        if cells is not None and d.kind in ('load', 'store') and d.rs == SP and d.i in cells.acc:
            kk, sz = cells.acc[d.i]
            j, off = cells.find(kk, sz)
            c0, s0 = cells.cells[j]
            if d.kind == 'load':
                uses.append(('c', j))
            else:
                if not (off == 0 and sz == s0):
                    uses.append(('c', j))
                defs.append(('c', j))
        return uses, defs
    i = s
    while i < e:
        d = f.ins[i]
        if d.kind in ('br', 'j', 'jr'):
            out.append((list(d.uses), []))
            if i + 1 < e:
                out.append(item(f.ins[i + 1]))
            if d.kind == 'jr' and d.rs == RA:
                out.append(([V0, V1, SP, RA] + list(CALLEE_SAVED), []))
            i += 2
            continue
        if d.kind in ('jal', 'jalr'):
            if i + 1 < e:
                out.append(item(f.ins[i + 1]))
            # the callee's outputs carry the PRIOR value of every register it
            # may clobber (Executor.call), so those are read by the call
            uses = [4, 5, 6, 7, SP] + list(d.uses) + [r for r in CALLER_CLOBBER if r != RA]
            if cells is not None:
                at = f.sp_at[0]
                spk = at.get(d.i, 0)
                uses += [("c", j) for j in cells.overlapping(spk + 16, 48)]
            out.append((uses, list(CALLER_CLOBBER)))
            i += 2
            continue
        out.append(item(d))
        i += 1
    return out


def liveness(f, cells=None):
    nb = len(f.blocks)
    items = [_exec_items(f, k, cells) for k in range(nb)]
    live_in = [set() for _ in range(nb)]
    succ = [[sb for _, sb in f.succs(k) if sb is not None] for k in range(nb)]
    changed = True
    while changed:
        changed = False
        for k in range(nb - 1, -1, -1):
            live = set()
            for sb in succ[k]:
                live |= live_in[sb]
            for uses, defs in reversed(items[k]):
                live -= set(defs)
                live |= set(uses)
            live.discard(0)
            live.discard(SP)
            if live != live_in[k]:
                live_in[k] = live
                changed = True
    return live_in


def _norm_label(lbl):
    return 'next' if lbl in ('fall', 'goto') else lbl


class Prover:
    def __init__(self, P, S, arity=None, volatile=None, void=False, ret64=False,
                 rodata_equal=True, objects=None, nonvoid=None, home='main', noval=None,
                 valueless=False, voidcallees=(), flow=None, addrtaken=()):
        self.P, self.S = P, S
        self.addrtaken = set(addrtaken)
        self.pending_ret = set()
        self.flow = flow
        self.voidcallees = set(voidcallees)
        self.noval = noval
        self.valueless = valueless
        self.home = home
        self.nonvoid = nonvoid or {}
        self.arity, self.volatile, self.void, self.ret64 = arity, volatile, void, ret64
        self.rodata_equal = rodata_equal
        self.objects = objects
        self.mode = None
        self.notes = set()

    def prove(self):
        P, S = self.P, self.S
        P.build_cfg()
        S.build_cfg()
        hs = hazards(S)
        hp = hazards(P)
        if hs:
            raise Unproven('hazard', 'shipped object: ' + hs[0][1] +
                           (' (also in the plain compile)' if hp else ''))
        if hp:
            # the plain compile relies on the assembler to schedule around a
            # hazard; its MEANING is the sequential one the compiler intended,
            # which is what P is evaluated with.  S (what ships) has none.
            self.notes.add('plain-compile-hazard')
        if (P.jt or S.jt) and (len(P.jt) != len(S.jt) or not self.rodata_equal):
            raise Unproven('jump-table', 'jump tables differ between P and S')
        self.pair_blocks()
        P.sp_at = sp_offsets(P)
        S.sp_at = sp_offsets(S)
        escP, escS = frame_escapes(P), frame_escapes(S)
        self.mode = 'B' if (escP or escS) else 'A'
        self.objP = self.objS = None
        if self.mode == 'B' and isinstance(self.objects, list):
            # the S object bases are a GUESS (verified by the proof): try
            # the position-paired guess, then the global-vote guess
            first = None
            for strat in ('pos', 'global', 'area'):
                self.strategy = strat
                try:
                    self.setup_objects()
                    self.mode = 'C'
                    return self._prove_rest(P, S)
                except Unproven as u:
                    first = first or u
                    self.mode = 'B'
                    self.notes = set()
            # mode B needs no debug info at all (the frame is shared memory
            # with an identical layout, checked byte by byte): a complete
            # proof of its own when P and S keep the same frame
            self.objP = self.objS = None
            saved, self.objects = self.objects, None
            try:
                return self._prove_rest(P, S)
            except Unproven:
                raise first
            finally:
                self.objects = saved
        return self._prove_rest(P, S)

    def _prove_rest(self, P, S):
        if self.mode == 'B' and isinstance(self.objects, list):
            pass
        elif self.mode == 'B' and self.objects is not None:
            self.notes.add('no-frame-objects: ' + str(self.objects))
        self.cx = Ctx(self.arity, self.volatile, self.void, self.ret64, self.mode)
        self.cx.nonvoid = self.nonvoid
        self.cx.voidcallees = self.voidcallees
        self.cx.area_obj = self.mode == 'C' and getattr(self, 'area_mode', False)
        global PROV, SAVE_BYTES, RANGES, FREE
        SAVE_BYTES = None
        RANGES = reg_intervals(P)
        FREE = reg_frame_free(P) if self.mode == 'B' else None
        if self.mode == 'B':
            # the register save area, when both sides have the SAME one
            sv = save_slots(P)
            if sv and sv == save_slots(S):
                SAVE_BYTES = sv
                self.notes.add('save-area')
        self.cx.set_symbols({v[1] for f in (P, S) for v in f.rel.values()})
        PROV = self.cx.prov if os.environ.get("PEQ_PROV") == "1" else None
        if self.mode in ('A', 'C'):
            self.cellsP = Cells(P, P.sp_at[0], self.objP)
            self.cellsS = Cells(S, S.sp_at[0], self.objS)
            self.live = liveness(P, self.cellsP)
        else:
            self.cellsP = self.cellsS = None
            self.live = liveness(P)
        # Attempts, each a complete proof checked end to end:
        #  1 block mode;  2 PATH MODE (every path between cut points -- entry
        #  and loop headers -- executed as one straight line, so code moved
        #  across a branch is compared on each path as a whole);  3/4 the
        #  same with the UNINITIALISED frame bytes at function entry taken
        #  as one indeterminate value on both sides (P's junk at one address,
        #  S's at another; only cells read before written are affected).
        self.extra_entry = set()
        self.path_mode = False
        first = None
        extra = None
        for relaxed, pm in ((False, False), (False, True), (True, False), (True, True)):
            if relaxed:
                if extra is None:
                    break
                self.extra_entry = extra
            if self.mode == 'B' and (pm or relaxed):
                break
            if pm and not FULL_PATH_MODE:
                continue                       # (self-test of local path mode)
            self.path_mode = pm
            try:
                self.fixpoint()
                self.final()
                if pm:
                    self.notes.add('path-mode')
                if relaxed:
                    self.notes.add('indeterminate-stack')
                first = None
                break
            except Unproven as u:
                if os.environ.get('PEQ_DEBUG'):
                    print('  [attempt relaxed=%s path=%s] %s: %s' % (
                        relaxed, pm, u.cls, u.msg[:400]), file=sys.stderr)
                if first is None:
                    first = u
                if u.cls in ('hazard',):
                    break
                if not relaxed and not pm and self.mode != 'B':
                    try:
                        extra = self.uninit_cell_pairs() or None
                    except Unproven:
                        extra = None
        if first is not None and self.mode != 'B' and first.cls != 'hazard' and \
                getattr(self, '_fail_bp', None) is not None:
            # LOCAL PATH MODE: path mode over a cut set that is every block
            # except a small region after the block where block mode failed.
            # Path mode is sound for ANY cut set that holds the entry and a
            # block of every cycle (loop headers are always kept), so this is
            # the same proof with longer straight lines only where code was
            # moved across a branch -- it avoids the whole-function path
            # explosion.
            fb = self._fail_bp
            loops = self.cut_points()
            nb = len(self.P.blocks)
            succ_ = {b_: [s_ for _, s_ in self.P.succs(b_) if s_ is not None]
                     for b_ in range(nb)}
            pred_ = collections.defaultdict(list)
            for b_, ss_ in succ_.items():
                for s_ in ss_:
                    pred_[s_].append(b_)

            def reach(start, nxt, depth):
                seen, front = set(), [start]
                for _ in range(depth):
                    front = [y for x in front for y in nxt(x) if y not in seen]
                    seen.update(front)
                return seen
            # un-cut regions to try: forward from the failing block or from a
            # predecessor, then symmetric neighbourhoods (code moved INTO the
            # failing block from a few blocks back)
            fwd = lambda x: succ_.get(x, [])       # noqa: E731
            bwd = lambda x: pred_.get(x, [])       # noqa: E731

            def hood(b_, depth):
                return reach(b_, fwd, depth) | reach(b_, bwd, depth) | {b_}
            regions = []
            for st_ in [fb] + pred_[fb]:
                for depth in (2, 4, 8):
                    r_ = reach(st_, fwd, depth) | ({fb} if st_ != fb else set())
                    r_.discard(st_)
                    regions.append(r_)
            for depth in (1, 2, 3, 4):
                regions.append(hood(fb, depth))
            seen_r = set()
            done = False
            for relaxed in ((False, True) if extra is not None else (False,)):
                self.extra_entry = extra if relaxed else set()
                queue = list(regions)
                grown = 0
                while queue:
                    region = queue.pop(0)
                    for _once in (0,):
                        region = frozenset(region - loops)
                        if (region, relaxed) in seen_r:
                            continue
                        seen_r.add((region, relaxed))
                        st_, depth = fb, len(region)
                        cuts = set(self.pairs) - region
                        self.cut_override = cuts | loops | {0}
                        self.path_mode = True
                        try:
                            self.fixpoint()
                            self.final()
                            self.notes.add('local-path-mode')
                            if relaxed:
                                self.notes.add('indeterminate-stack')
                            done = True
                            break
                        except Unproven as u:
                            if os.environ.get('PEQ_DEBUG'):
                                print('  [local path mode start=%d depth=%d] %s: %s' % (
                                    st_, depth, u.cls, u.msg[:300]), file=sys.stderr)
                            # a second motion site further on: grow this
                            # region by that block's neighbourhood and retry
                            lb = getattr(self, '_last_bp', None)
                            if lb is not None and lb not in region and grown < 8 and \
                                    u.cls != 'limit':
                                grown += 1
                                for d_ in (1, 2, 3):
                                    queue.insert(d_ - 1, set(region) | hood(lb, d_))
                        finally:
                            self.cut_override = None
                    if done:
                        break
                if done:
                    break
            if done:
                first = None
        if first is not None:
            raise first
        if self.mode == 'B':
            self.check_mode_b()
        if self.mode == 'C':
            self.check_mode_c()
        return True

    def uninit_cell_pairs(self):
        """(P cell, S cell) pairs the relation establishes somewhere in the
        function, for cells NOT already paired at the entry."""
        cnt = collections.Counter()
        for bp in list(self.R):
            try:
                a, b = self.run_pair(bp, self.R[bp])
            except Unproven:
                continue
            for k_, (_, sp_) in enumerate(self.P.succs(bp)):
                if sp_ is None:
                    continue
                for x in self.exit_pairs(*self.edge(a, b, bp, k_), sp_):
                    if len(x) == 2 and not isinstance(x[0], int) and \
                            not isinstance(x[1], int):
                        cnt[x] += 1
        # weaker evidence: the same word index accesses a cell on both sides
        pos = collections.Counter()
        for i, (kp, szp) in self.cellsP.acc.items():
            if i in self.cellsS.acc:
                ks, szs = self.cellsS.acc[i]
                hp, hs = self.cellsP.find(kp, szp), self.cellsS.find(ks, szs)
                if hp and hs:
                    pos[(('c', hp[0]), ('c', hs[0]))] += 1
        # weaker still (a GUESS like the rest -- the proof re-checks it): the
        # k-th frame LOAD of a paired block pair, when both blocks load the
        # frame equally often (swaps move words, so indices differ)
        for bp, bs in self.pairs.items():
            sp_, ep_ = self.P.blocks[bp][:2]
            ss_, es_ = self.S.blocks[bs][:2]
            lp = [i for i in range(sp_, ep_) if i in self.cellsP.acc and
                  self.P.ins[i].kind == 'load']
            ls = [i for i in range(ss_, es_) if i in self.cellsS.acc and
                  self.S.ins[i].kind == 'load']
            if len(lp) != len(ls):
                continue
            for ip, is_ in zip(lp, ls):
                if self.P.ins[ip].name != self.S.ins[is_].name:
                    continue
                hp = self.cellsP.find(*self.cellsP.acc[ip])
                hs = self.cellsS.find(*self.cellsS.acc[is_])
                if hp and hs and (('c', hp[0]), ('c', hs[0])) not in pos:
                    cnt[(('c', hp[0]), ('c', hs[0]))] += 0
        for x, n_ in pos.items():
            cnt[x] += 0                    # keep exit-pair evidence first
        # only LOCAL cells (below the entry $sp) are indeterminate; the
        # incoming area (arguments) keeps its physical pairing
        usedp = {('c', j) for j, c in enumerate(self.cellsP.cells) if c[0] >= 0}
        useds = {('c', j) for j, c in enumerate(self.cellsS.cells) if c[0] >= 0}
        out = set()
        order = sorted(set(cnt) | set(pos), key=lambda x: (-pos.get(x, 0), -cnt.get(x, 0)))
        for (p, s_) in order:
            if p in usedp or s_ in useds:
                continue
            usedp.add(p)
            useds.add(s_)
            out.add((p, s_))
        return out

    # --- mode C: escaped frame objects (from the compiler's debug info) --
    def setup_objects(self):
        """P's escaped objects come from the -g3 compile's local symbols;
        S's base of each is paired from the address-forming words (same
        block pair, same order).  The final proof checks everything under
        this map; nothing here is trusted."""
        self.area_mode = False
        if getattr(self, 'strategy', 'pos') == 'area':
            return self._setup_area_object()
        P, S = self.P, self.S
        locs = sorted((v, sz, nm) for nm, v, sz, _ in self.objects if v is not None)
        starts = sorted(v for v, _, _ in locs)
        # locals the debug info gives no address: those that can be in
        # memory (aggregates, unknown types, address-taken scalars)
        unloc = [(nm, sz) for nm, v, sz, kind in self.objects
                 if v is None and (kind != 'scalar' or nm in self.addrtaken)]
        lowP = min(P.sp_at[0].values())
        lowS = min(S.sp_at[0].values())
        outP = self.outgoing_words(P)
        self.outarg = None

        def extent(v, sz):
            if sz:
                return sz
            above = [x for x in starts if x > v] + ([0] if v < 0 else [])
            return (min(above) - v) if above else None

        fp, fs = formations(P), formations(S)
        objs = []                          # [(P start, size)]
        idx_of = {}

        def obj_for(k, i):
            for past in (False, True):     # one-past-the-end is a valid C pointer
              for v, sz, nm in locs:
                e = extent(v, sz)
                if e and (v <= k < v + e or (past and k == v + e)):
                    if v not in idx_of:
                        idx_of[v] = len(objs)
                        objs.append((v, e))
                    return idx_of[v], k - v
            if outP is not None and lowP <= k < lowP + outP:
                # the OUTGOING ARGUMENT AREA (a by-value aggregate argument
                # built through a pointer): one object at the frame bottom
                # on both sides, its S base fixed at S's frame bottom
                if self.outarg is None:
                    self.outarg = len(objs)
                    objs.append((lowP, outP))
                return self.outarg, k - lowP
            ua = unloc_area(k)
            if ua is not None:
                v, e = ua
                if v not in idx_of:
                    idx_of[v] = len(objs)
                    objs.append((v, e))
                return idx_of[v], k - v
            raise Unproven('escape-unknown', 'P: frame address %d formed at %d is in no '
                           'local of the debug info' % (k, i))

        def unloc_area(k):
            """A sound OVER-approximation of where the debug-unlocated
            locals that may hold k can be: every placement of their total
            size Z that covers all unexplained formation offsets, clipped to
            the local area (above the outgoing area, below the save area and
            0) and to the gap between located objects."""
            if not unloc or any(sz is None for _, sz in unloc):
                return None
            Z = sum(sz for _, sz in unloc)
            un = [k2 for _, k2, _ in fp if k2 is not None and
                  not any(v <= k2 <= v + (extent(v, sz) or 0) for v, sz, _ in locs) and
                  not (outP is not None and lowP <= k2 < lowP + outP)]
            if k not in un:
                un.append(k)
            lo, hi = max(un) - Z, min(un) + Z
            lo = max(lo, lowP + (outP or 0))
            sv = save_slots(P)
            top = min([0] + [b for b in sv if b > max(un)])
            hi = min(hi, top)
            for v, sz, _ in locs:
                e = extent(v, sz) or 0
                if v + e <= min(un):
                    lo = max(lo, v + e)
                elif v > max(un):
                    hi = min(hi, v)
                else:
                    return None
            if not lo <= min(un) or not max(un) <= hi or hi - lo < 1:
                return None
            self.notes.add('unlocated-local-area')
            return lo, hi - lo
        byblkP = collections.defaultdict(list)
        byblkS = collections.defaultdict(list)
        for i, k, kind in fp:
            byblkP[P.blk_of_word(i)].append((i, k, kind))
        for i, k, kind in fs:
            byblkS[S.blk_of_word(i)].append((i, k, kind))
        base_s = {}
        inv = {v: k for k, v in self.pairs.items()}
        # P formations as (object, offset), S as offsets -- over the whole
        # function (a formation may be hoisted across blocks)
        pk, sk = [], collections.Counter()
        for bp, lst in byblkP.items():
            if bp in self.pairs:
                pk += [obj_for(k, i) for i, k, _ in lst if k is not None]
        for bs, lst in byblkS.items():
            if bs in inv:
                sk.update(k for _, k, _ in lst if k is not None)
        blocks = [(pk, sk)]
        # each object's S base: prefer the translation (S - P) that
        # explains the most formations overall, then per object (a vote; the
        # final proof checks the result)
        gl = collections.Counter()
        for pk, sk in blocks:
            for (jj, off) in pk:
                for k2 in sk:
                    gl[k2 - (objs[jj][0] + off)] += 1
        # strongest evidence: a formation at the SAME word index on both
        # sides (PostProc keeps the count; most formations stay in place)
        at_s = {i: k for i, k, _ in fs if k is not None}
        pos = collections.defaultdict(collections.Counter)
        for i, k, _ in fp:
            if k is not None and i in at_s:
                jj, off = obj_for(k, i)
                pos[jj][at_s[i] - (objs[jj][0] + off)] += 1
        for j in range(len(objs)):
            if j == self.outarg:
                base_s[j] = lowS
                continue
            cands = set()
            for pk, sk in blocks:
                for (jj, off) in pk:
                    if jj == j:
                        cands |= {k2 - (objs[j][0] + off) for k2 in sk}
            if not cands:
                raise Unproven('escape-layout', 'no S formation for escaped object at P %d'
                               % objs[j][0])
            if getattr(self, 'strategy', 'pos') == 'pos':
                d = max(cands, key=lambda x: (pos[j][x], gl[x], x == 0))
            else:
                d = max(cands, key=lambda x: (gl[x], pos[j][x], x == 0))
            base_s[j] = objs[j][0] + d
        self.objP = objs
        self.objS = [(base_s[j], sz) for j, (v, sz) in enumerate(objs)]
        for side, lst in (('P', self.objP), ('S', self.objS)):
            for a in range(len(lst)):
                for b in range(a + 1, len(lst)):
                    if lst[a][0] < lst[b][0] + lst[b][1] and lst[b][0] < lst[a][0] + lst[a][1]:
                        raise Unproven('escape-layout', '%s: escaped objects overlap' % side)
        if any(b != v for (v, _), (b, _) in zip(self.objP, self.objS)):
            self.notes.add('moved-escaped-object')

    def _setup_area_object(self):
        """Mode C fallback for a DYNAMIC frame address no single local
        explains (IDO folds a constant of the index into the displacement:
        `addu t5,sp,t7 ; lbu t9,-1064(t5)` for `((u8 *)&obj + i)[j - 1140]`).
        In C a frame address can only come from the address of a local, and
        pointer arithmetic stays inside that local, so such an access lies
        in SOME candidate local: an aggregate, a local of unknown type, or a
        scalar whose address the C takes.  All of them are merged into ONE
        object spanning their hull [lo, hi): every direct $sp access inside
        it becomes ordered memory (stricter than cells), accesses at
        distinct offsets of the one object are compared by exact address
        arithmetic, and no "distinct objects never alias" rule is used at
        all.  Refused when a candidate has no frame address in the debug
        info (its position is unknown), when the hull holds a callee-saved
        save slot, or when a STATIC address formation falls outside it."""
        P, S = self.P, self.S
        self.outarg = None
        if any(v is None and (kind != 'scalar' or nm in self.addrtaken)
               for nm, v, sz, kind in self.objects):
            raise Unproven('escape-unknown', 'area: a candidate local has no frame address')
        starts = sorted(v for _, v, _, _ in self.objects if v is not None)

        def extent(v, sz):
            if sz:
                return sz
            above = [x for x in starts if x > v] + ([0] if v < 0 else [])
            return (min(above) - v) if above else None

        cand = []
        for nm, v, sz, kind in self.objects:
            if v is None or not (kind != 'scalar' or nm in self.addrtaken):
                continue
            e = extent(v, sz)
            if not e:
                raise Unproven('escape-unknown', 'area: no extent for local %s' % nm)
            cand.append((v, e))
        if not cand:
            raise Unproven('escape-unknown', 'area: no candidate local')
        lo = min(v for v, _ in cand)
        hi = max(v + e for v, e in cand)
        fp, fs = formations(P), formations(S)
        dyn = False
        for i, k, kind in fp:
            if kind in ('addu', 'or'):
                dyn = True
            elif k is None or not lo <= k <= hi:
                raise Unproven('escape-unknown', 'area: static frame address %s formed at %d '
                               'is outside the candidate locals [%d,%d)' % (k, i, lo, hi))
        if not dyn:
            raise Unproven('escape-unknown', 'area: no dynamic frame address to explain')
        # S's base: the translation the formations at the same word index
        # agree on (the final proof checks it)
        at_s = {i: k for i, k, _ in fs if k is not None}
        votes = collections.Counter(at_s[i] - k for i, k, _ in fp
                                    if k is not None and i in at_s)
        if not votes:
            raise Unproven('escape-layout', 'area: no S formation pairs a P formation')
        d = max(votes, key=lambda x: (votes[x], x == 0))
        for f, b in ((P, lo), (S, lo + d)):
            sv = save_slots(f)
            if any(b <= x < b + (hi - lo) for x in sv):
                raise Unproven('escape-layout', '%s: the candidate-local hull [%d,%d) holds '
                               'a register save slot' % (f.side, b, b + hi - lo))
        self.objP = [(lo, hi - lo)]
        self.objS = [(lo + d, hi - lo)]
        self.area_mode = True
        self.notes.add('local-area-object')
        if d:
            self.notes.add('moved-escaped-object')

    def outgoing_words(self, f):
        """Bytes of the outgoing argument area f's calls use (16 home bytes
        plus 4 per stack argument word), from the callees' argument words;
        None when a callee's are unknown."""
        mx = 0
        for v in f.rel.values():
            if v[0] == R_MIPS_26:
                w = self.arity.get(v[1])
                if not isinstance(w, int):
                    return None
                mx = max(mx, w)
        if any(d.kind == 'jalr' for d in f.ins):
            w = self.arity.get('<jalr>')
            if not isinstance(w, int):
                return None
            mx = max(mx, w)
        has_call = any(v[0] == R_MIPS_26 for v in f.rel.values()) or \
            any(d.kind == 'jalr' for d in f.ins)
        return (16 + 4 * max(0, mx - 4)) if has_call else None

    def check_mode_c(self):
        """Escaped objects must lie inside each side's frame and clear of
        every call's home/argument zone (a callee owns those bytes)."""
        lowP = min(self.P.sp_at[0].values())
        lowS = min(self.S.sp_at[0].values())
        for (v, sz), (b, _) in zip(self.objP, self.objS):
            if v < lowP or b < lowS:
                raise Unproven('escape-layout', 'escaped object outside the frame')
        for a, b in self.runs.values():
            for run, objs in ((a, self.objP), (b, self.objS)):
                for k, z in run.zones:
                    for jj, (v, sz) in enumerate(objs):
                        if jj == self.outarg:
                            # the outgoing area itself: every zone must lie
                            # inside it or clear of it (a call reads its
                            # stack arguments from it, and clobbers it)
                            if not (v <= k and k + z <= v + sz) and \
                                    v < k + z and k < v + sz:
                                raise Unproven('escape-zone', 'a call zone [%d,+%d) '
                                               'straddles the outgoing area' % (k, z))
                            continue
                        if v < k + z and k < v + sz:
                            raise Unproven('escape-zone', 'escaped object [%d,+%d) overlaps a '
                                           'call zone [%d,+%d)' % (v, sz, k, z))

    # --- CFG correspondence ------------------------------------------
    def pair_blocks(self):
        P, S = self.P, self.S
        pairs, inv = {}, {}
        work = [(0, 0)]
        while work:
            bp, bs = work.pop()
            if bp in pairs or bs in inv:
                if pairs.get(bp) != bs or inv.get(bs) != bp:
                    raise Unproven('cfg-shape', 'P block %d / S block %d paired inconsistently'
                                   % (bp, bs))
                continue
            pairs[bp], inv[bs] = bs, bp
            tp, ts = P.blocks[bp][2], S.blocks[bs][2]
            kp, ks = _norm_label(tp[0]), _norm_label(ts[0])
            if kp != ks:
                raise Unproven('branch-kind', 'P block @%d ends %s, S block @%d ends %s' % (
                    P.blocks[bp][0], tp[0], S.blocks[bs][0], ts[0]))
            if kp == 'end':
                raise Unproven('cfg', 'control falls off the end of the window (P @%d)'
                               % P.blocks[bp][0])
            if kp == 'cond' and P.ins[tp[1]].name != S.ins[ts[1]].name:
                raise Unproven('branch-kind', 'P %s @%d vs S %s @%d' % (
                    P.ins[tp[1]].name, tp[1], S.ins[ts[1]].name, ts[1]))
            sp_, ss_ = P.succs(bp), S.succs(bs)
            if [_norm_label(x[0]) for x in sp_] != [_norm_label(x[0]) for x in ss_]:
                raise Unproven('cfg-shape', 'successor kinds differ at P @%d'
                               % P.blocks[bp][0])
            for (_, a), (_, b) in zip(sp_, ss_):
                if a is None or b is None:
                    raise Unproven('cfg', 'a conditional falls off the window (P @%d)'
                                   % P.blocks[bp][0])
                work.append((a, b))
        self.pairs = pairs

    # --- nodes: registers (int) and, in mode A, frame cells ('c', j) ----
    def nodes(self, side):
        out = [r for r in range(1, NREG) if r != SP]
        C = self.cellsP if side == 'P' else self.cellsS
        if C is not None:
            out += [('c', j) for j in range(len(C.cells))]
        return out

    def entry_relation(self):
        extra = set(getattr(self, 'extra_entry', ()))
        rel = {(r, r) for r in range(1, NREG) if r != SP} | extra
        if self.mode in ('A', 'C'):
            # function entry: the memory below the entry $sp is the SAME
            # physical memory on both sides, so cells at the same place hold
            # the same bytes (incoming arguments, and junk alike) -- unless
            # the relaxed attempt re-paired an uninitialised local cell
            usedp = {x[0] for x in extra}
            useds = {x[1] for x in extra}
            idx = {c: j for j, c in enumerate(self.cellsS.cells)}
            for j, c in enumerate(self.cellsP.cells):
                if c in idx and ('c', j) not in usedp and ('c', idx[c]) not in useds:
                    rel.add((('c', j), ('c', idx[c])))
        return rel

    def entry_state(self, bp, rel):
        cx = self.cx
        mk = cx.bank.mk
        parent = {}

        def find(x):
            while parent.get(x, x) != x:
                x = parent[x]
            return x
        konst = {}
        derived = {}
        pairs = []
        diffs = []
        provs = []
        xos = []
        for x in rel:
            if len(x) == 6:
                xos.append(x)              # ('XO', p, s, K, 0, 0): P_p == S_s + K
            elif len(x) == 5:
                provs.append(x)
            elif len(x) == 4:
                diffs.append(x)
            elif len(x) == 3:
                if x[0] in ('KP', 'KS'):
                    konst[(x[0][1], x[1])] = x[2]
                else:
                    derived[(x[0][1], x[1])] = x[2]
            else:
                pairs.append(x)
        rel = pairs
        for p, s in rel:
            a, b = find(('P', p)), find(('S', s))
            if a != b:
                parent[a] = b
        rep = {}
        for p, s in rel:
            rt = find(('P', p))
            key = (0, p) if isinstance(p, int) else (1, p[1])
            if rt not in rep or key < rep[rt]:
                rep[rt] = key
        inP = {p for p, _ in rel}
        inS = {s for _, s in rel}

        def val(side, node, members):
            k = konst.get((side, node))
            if k is not None:
                return k
            if node in members:
                return mk('in', bp, rep[find((side, node))])
            return cx.junk(side, 'entry', bp, node if isinstance(node, int) else node[1],
                           isinstance(node, int))
        rp = [val('P', r, inP) if r not in (0, SP) else cx.al.ZERO for r in range(NREG)]
        rs = [val('S', r, inS) if r not in (0, SP) else cx.al.ZERO for r in range(NREG)]
        cp = cs = None
        if self.mode in ('A', 'C'):
            cp = [val('P', ('c', j), inP) for j in range(len(self.cellsP.cells))]
            cs = [val('S', ('c', j), inS) for j in range(len(self.cellsS.cells))]
        # derived facts: node == template over other nodes' entry values
        # (instantiated over the values BEFORE any derived fact is applied)
        base = {'P': (list(rp), list(cp) if cp is not None else None),
                'S': (list(rs), list(cs) if cs is not None else None)}
        al = cx.al
        for _, (p, s_), (m, s2), K in diffs:
            for side, node, ref in (('P', p, m), ('S', s_, s2)):
                regs, cells = (rp, cp) if side == 'P' else (rs, cs)
                b0 = base[side]
                rv = b0[0][ref] if isinstance(ref, int) else b0[1][ref[1]]
                v = al.add(rv, K)
                if isinstance(node, int):
                    regs[node] = v
                else:
                    cells[node[1]] = v
        for (side, node), tpl in derived.items():
            regs, cells = (rp, cp) if side == 'P' else (rs, cs)
            b0 = base[side]

            def get(n_, b0=b0):
                return b0[0][n_] if isinstance(n_, int) else b0[1][n_[1]]
            v = self._instantiate(tpl, get)
            if isinstance(node, int):
                regs[node] = v
            else:
                cells[node[1]] = v
        # cross-side offsets: a P register that is no pair's member and not a
        # known constant holds the related S register's value plus K
        for _, p, s, K, _z, _z2 in xos:
            if isinstance(p, int) and isinstance(s, int) and p not in inP and \
                    ('P', p) not in konst and ('P', p) not in derived:
                rp[p] = al.add(rs[s], K)
        for v in rp + rs + (cp or []) + (cs or []):
            cx.prov_var.pop(id(v), None)   # stale facts from an earlier iteration
        for _, side, node, o, _z in provs:
            regs, cells = (rp, cp) if side == 'P' else (rs, cs)
            v = regs[node] if isinstance(node, int) else cells[node[1]]
            if v.op in ('in', 'junk'):
                cx.prov_var[id(v)] = o
        return rp, cp, rs, cs

    def _instantiate(self, t, get):
        if t.op == 'ref':
            n_ = t.a[0]
            return get(n_ if isinstance(n_, int) else ('c', n_[1]))
        if not any(isinstance(x, T) for x in t.a):
            return t
        b = self.cx.bank
        args = [self._instantiate(x, get) if isinstance(x, T) else x for x in t.a]
        return self._rebuild(t.op, args)

    def _rebuild(self, op, args):
        al, mk = self.cx.al, self.cx.bank.mk
        if op == 'sum':
            return al._mksum(list(args[:-1]), args[-1])
        if op in ('and', 'or', 'xor', 'nor'):
            return al.logic(op, args[0], args[1])
        if op in ('sll', 'srl', 'sra'):
            return al.shift(op, args[0], args[1])
        if op == 'neg':
            return al.neg(args[0])
        if op in ('sx', 'zx'):
            return al.ext(args[0], op == 'sx', args[2])
        if op in ('slt', 'sltu'):
            return al.slt(op, args[0], args[1])
        return mk(op, *args)

    def _template(self, v, idx, avoid, depth=0):
        """v as a term over ('ref', node) leaves + constants, or None."""
        q = idx.get(id(v))
        if q is not None and q != avoid:
            return self.cx.bank.mk('ref', q if isinstance(q, int) else ('c', q[1]))
        if v.op == 'c':
            return v
        if depth > 5 or v.op in ('in', 'junk', 'ld', 'lwx', 'ret', 'retv', 'clob', 'zone',
                                 'zoneld', 'copval', 'fr', 'obj'):
            return None
        args = []
        for x in v.a:
            if isinstance(x, T):
                y = self._template(x, idx, avoid, depth + 1)
                if y is None:
                    return None
                args.append(y)
            else:
                args.append(x)
        return self.cx.bank.mk(v.op, *args)

    def derived_facts(self, run, side, nodes, pairs_nodes):
        idx = {}
        for n_ in nodes:
            v = self._get(run, n_)
            if v.op in ('c',):
                continue
            k = id(v)
            if k not in idx or (n_ in pairs_nodes and idx[k] not in pairs_nodes):
                idx[k] = n_
        out = set()
        for n_ in nodes:
            if n_ in pairs_nodes:
                continue
            v = self._get(run, n_)
            if _is_const(v) or v.op in ('in', 'junk'):
                continue
            t = self._template(v, idx, n_)
            if t is not None and t.op != 'c':
                out.add(('D' + side, n_, t))
        return out

    def run_pair(self, bp, rel):
        bs = self.pairs[bp]
        rp, cp, rs, cs = self.entry_state(bp, rel)
        xp = Executor(self.cx, self.P, self.cellsP, self.objP)
        xs = Executor(self.cx, self.S, self.cellsS, self.objS)
        a = xp.run(bp, bp, rp, cp, self.P.sp_at[1][bp])
        b = xs.run(bs, bp, rs, cs, self.S.sp_at[1][bs])
        return a, b

    @staticmethod
    def _get(run, node):
        if isinstance(node, int):
            return run.regs[node]
        return run.cells[node[1]]

    def edge(self, a, b, bp, k):
        """The exit states of block pair bp on its k-th successor edge,
        with what the edge's branch implies: on the edge where `beq x, c` /
        `bne x, c` (c a constant) says x == c, every location holding x
        holds c."""
        return (self._edge_view(a, self.P, bp, k),
                self._edge_view(b, self.S, self.pairs[bp], k))

    def infeasible(self, run, bidx, k):
        """True when P block bidx's branch condition is a CONSTANT that rules
        out its k-th successor edge (0 = taken, 1 = fall-through) -- e.g.
        IDO's divide-by-zero check on a divisor the relation knows is 84.  No
        execution takes such an edge, so it carries no obligation."""
        term = self.P.blocks[bidx][2]
        if term[0] != 'cond' or run is None or run.cond is None:
            return False
        c = run.cond
        if any(x.op != 'c' for x in c.a):
            return False
        sv = lambda v: v - (1 << 32) if v & 0x80000000 else v      # noqa: E731
        vals = [x.a[0] for x in c.a]
        if c.op == 'eq':
            v = vals[0] == vals[1]
        elif c.op == 'ne':
            v = vals[0] != vals[1]
        elif c.op == 'bltz':
            v = sv(vals[0]) < 0
        elif c.op == 'bgez':
            v = sv(vals[0]) >= 0
        elif c.op == 'blez':
            v = sv(vals[0]) <= 0
        elif c.op == 'bgtz':
            v = sv(vals[0]) > 0
        else:
            return False
        return (k == 0) != v

    def _edge_view(self, run, f, bidx, k):
        term = f.blocks[bidx][2]
        if term[0] != 'cond' or run.cond is None:
            return run
        name = f.ins[term[1]].name
        if not ((name == 'bne' and k == 1) or (name == 'beq' and k == 0)):
            return run
        x, y = run.cond.a
        if _is_const(y) and not _is_const(x):
            t, c = x, y
        elif _is_const(x) and not _is_const(y):
            t, c = y, x
        else:
            return run
        v = BlockRun()
        for sl in BlockRun.__slots__:
            if hasattr(run, sl):
                setattr(v, sl, getattr(run, sl))
        al = self.cx.al

        def sub(r):
            if r is t:
                return c
            if r.op == 'sum' and any(x is t for x in r.a[:-1]):
                terms = [x for x in r.a[:-1] if x is not t]
                n = sum(1 for x in r.a[:-1] if x is t)
                acc = al._mksum(terms, r.a[-1])
                for _ in range(n):
                    acc = al.add(acc, c)
                return acc
            return r
        v.regs = [sub(r) for r in run.regs]
        if run.cells is not None:
            v.cells = [sub(r) for r in run.cells]
        return v

    def exit_pairs(self, a, b, succ_p):
        by = collections.defaultdict(list)
        for s in self.nodes('S'):
            by[id(self._get(b, s))].append(s)
        out = set()
        for p in self.live[succ_p]:
            if p == 0 or p == SP:
                continue
            v = self._get(a, p)
            if _is_const(v):
                out.add(('KP', p, v))      # P-side constant at the entry
            for s in by.get(id(v), ()):
                out.add((p, s))
        for s in self.nodes('S'):
            v = self._get(b, s)
            if _is_const(v):
                out.add(('KS', s, v))
        pn = {x[0] for x in out if len(x) == 2}
        sn = {x[1] for x in out if len(x) == 2}
        livep = [p for p in self.live[succ_p] if p not in (0, SP)]
        out |= self.derived_facts(a, 'P', [p for p in self.nodes('P')], pn) - \
            {x for x in () if False}
        out = {x for x in out if not (len(x) == 3 and x[0] == 'DP' and x[1] not in livep)}
        out |= self.derived_facts(b, 'S', self.nodes('S'), sn)
        out |= self.difference_facts(a, b, [x for x in out if len(x) == 2])
        out |= self.offset_facts(a, b, succ_p, pn, out)
        # provenance facts (per side): the node holds a pointer into object O
        for side, run, nodes in [] if PROV is None else (('P', a, [p for p in self.live[succ_p] if p not in (0, SP)]),
                                 ('S', b, self.nodes('S'))):
            for n_ in nodes:
                v = self._get(run, n_)
                o = self.cx.prov(v)
                if o is not None:
                    out.add(('PV', side, n_, o, 0))
        return out

    def offset_facts(self, a, b, succ_p, pn, out):
        """('XO', p, s, K, 0, 0): live P register p (in no pair, not a
        constant) holds S register s's value plus a constant K != 0 -- code
        that bumps a counter on the other side of a join keeps the two
        sides one step apart at the join."""
        al = self.cx.al
        kp = {x[1] for x in out if len(x) == 3 and x[0] in ('KP', 'DP')}
        bys = collections.defaultdict(list)
        for s in range(1, 32):
            if s == SP:
                continue
            v = b.regs[s]
            if not _is_const(v):
                bys[tuple(id(x) for x in self._base(v))].append(s)
        res = set()
        for p in self.live[succ_p]:
            if not isinstance(p, int) or p in (0, SP) or p in pn or p in kp:
                continue
            v = a.regs[p]
            if _is_const(v):
                continue
            for s in bys.get(tuple(id(x) for x in self._base(v)), ()):
                d = al.sub(v, b.regs[s])
                if d.op == 'c' and d.a[0] != 0:
                    res.add(('XO', p, s, d, 0, 0))
        return res

    @staticmethod
    def _base(v):
        if v.op == 'sum':
            return v.a[:-1]
        if v.op == 'c':
            return ()
        return (v,)

    def difference_facts(self, a, b, pairs):
        """('DC', x, y, K): for related pairs x=(p,s), y=(m,s2), P_p - P_m
        == S_s - S_s2 == K (a constant) -- relative pointer facts, so two
        pointers walking in step keep their distance across a loop."""
        al = self.cx.al
        groups = collections.defaultdict(list)
        for p, s in pairs:
            v = self._get(a, p)
            if v.op not in ('sum', 'c', 'in'):
                continue
            groups[tuple(id(x) for x in self._base(v))].append((p, s))
        out = set()
        for key, g in groups.items():
            if len(g) < 2:
                continue
            g.sort(key=lambda x: (str(type(x[0])), str(x[0]), str(x[1])))
            if key == ():
                # constants: relate every pair of RAM addresses (pointers
                # that then walk in step keep their distance)
                ram = [x for x in g if 0x80000000 <= self._get(a, x[0]).a[0] < 0x80200000]
                cand = [(x, y) for i_, x in enumerate(ram) for y in ram[i_ + 1:]][:200]
            else:
                cand = [(x, g[0]) for x in g[1:]]
            for x, y in cand:
                kx = lambda z: (str(type(z[0])), str(z[0]), str(z[1]))
                if kx(x) < kx(y):
                    x, y = y, x            # one orientation, so facts intersect
                kp = al.sub(self._get(a, x[0]), self._get(a, y[0]))
                ks = al.sub(self._get(b, x[1]), self._get(b, y[1]))
                if kp.op == 'c' and kp is ks:
                    out.add(('DC', x, y, kp))
        return out

    def fixpoint(self):
        if self.path_mode:
            return self.fixpoint_paths()
        P = self.P
        self.R = {0: self.entry_relation()}
        work = [0]
        it = 0
        while work:
            it += 1
            if it > 50000:
                raise Unproven('limit', 'relation fixpoint did not converge')
            bp = work.pop()
            a, b = self.run_pair(bp, self.R[bp])
            for k_, ((_, sp_), (_, ss_)) in enumerate(zip(P.succs(bp), self.S.succs(self.pairs[bp]))):
                if self.infeasible(a, bp, k_):
                    continue
                new = self.exit_pairs(*self.edge(a, b, bp, k_), sp_)
                if sp_ == 0:
                    new &= self.R[0] | new
                old = self.R.get(sp_)
                if old is None:
                    self.R[sp_] = new
                    work.append(sp_)
                else:
                    inter = old & new
                    if inter != old:
                        self.R[sp_] = inter
                        work.append(sp_)
        # The transfer is not monotone in the REPRESENTATION of a value (a
        # constant / derived fact seen first can drop a pair that later
        # holds), so re-derive every entry set from its predecessors' exits
        # under the current sets until nothing changes.  Soundness does not
        # depend on this: final() checks every edge against the sets used.
        for _ in range(8):
            preds = collections.defaultdict(list)
            exits = {}
            for bp in self.R:
                a, b = self.run_pair(bp, self.R[bp])
                for k_, ((_, sp_), _x) in enumerate(zip(P.succs(bp), self.S.succs(self.pairs[bp]))):
                    if self.infeasible(a, bp, k_):
                        continue
                    preds[sp_].append(self.exit_pairs(*self.edge(a, b, bp, k_), sp_))
            newR = {}
            for bp in self.R:
                sets = preds.get(bp, [])
                acc = None
                for x in sets:
                    acc = set(x) if acc is None else (acc & x)
                if bp == 0:
                    e0 = self.entry_relation()
                    acc = e0 if acc is None else (acc & e0)
                newR[bp] = acc if acc is not None else self.R[bp]
            if newR == self.R:
                break
            self.R = newR
        # finally a monotone pass (only intersections) to a consistent point
        for _ in range(200):
            changed = False
            for bp in sorted(self.R):
                a, b = self.run_pair(bp, self.R[bp])
                for k_, ((_, sp_), _x) in enumerate(zip(P.succs(bp), self.S.succs(self.pairs[bp]))):
                    if sp_ not in self.R or self.infeasible(a, bp, k_):
                        continue
                    inter = self.R[sp_] & self.exit_pairs(*self.edge(a, b, bp, k_), sp_)
                    if inter != self.R[sp_]:
                        self.R[sp_] = inter
                        changed = True
            if not changed:
                break

    # --- final verification --------------------------------------------
    # --- path mode ---------------------------------------------------------
    def cut_points(self):
        if getattr(self, 'cut_override', None) is not None:
            return set(self.cut_override)
        P = self.P
        cuts = {0}
        state = {}
        stack = [(0, iter([sb for _, sb in P.succs(0)]))]
        state[0] = 1
        while stack:
            bp, it = stack[-1]
            nxt = next(it, None)
            if nxt is None:
                state[bp] = 2
                stack.pop()
                continue
            if nxt is None or nxt not in self.pairs:
                continue
            if state.get(nxt) == 1:
                cuts.add(nxt)              # back edge: a loop header
            elif nxt not in state:
                state[nxt] = 1
                stack.append((nxt, iter([sb for _, sb in P.succs(nxt)])))
        return cuts

    def paths_from(self, c):
        """[(blocks, k)]: block sequences from cut point c to the next cut
        point (taking successor index k of the last block) or a return (k
        None)."""
        P = self.P
        out = []
        work = [[c]]
        while work:
            path = work.pop()
            bp = path[-1]
            succ = P.succs(bp)
            if not succ:
                out.append((path, None))
            for k, (_, nb) in enumerate(succ):
                if nb in self.cuts:
                    out.append((path, k))
                else:
                    if nb in path or len(path) > 400:
                        raise Unproven('limit', 'path enumeration: cycle without a cut')
                    work.append(path + [nb])
            if len(out) > 3000:
                raise Unproven('limit', 'too many paths for path mode')
        return out

    def run_path(self, c, rel, path):
        P, S = self.P, self.S
        rp, cp, rs, cs = self.entry_state(c, rel)
        xp = Executor(self.cx, P, self.cellsP, self.objP)
        xs = Executor(self.cx, S, self.cellsS, self.objS)
        a = xp.run(c, c, rp, cp, P.sp_at[1][c])
        b = xs.run(self.pairs[c], c, rs, cs, S.sp_at[1][self.pairs[c]])
        for prev, bp in zip(path, path[1:]):
            self.check_direction(prev, a, b)
            ks = [k_ for k_, (_, s_) in enumerate(P.succs(prev)) if s_ == bp]
            if ks and all(self.infeasible(a, prev, k_) for k_ in ks):
                return None, None          # no execution follows this path
            a = xp.run(bp, c, None, None, None, st=a)
            b = xs.run(self.pairs[bp], c, None, None, None, st=b)
        a.sp_out, b.sp_out = a.regs[SP], b.regs[SP]
        return a, b

    def check_direction(self, bp, a, b):
        term = self.P.blocks[bp][2][0]
        where = self._where(bp)
        if term == 'cond' and not eq(a.cond, b.cond):
            raise Unproven('branch-cond', '%s: condition %s vs %s' % (
                where, fmt(a.cond), fmt(b.cond)))
        if term == 'jt' and not eq(a.jt_target, b.jt_target):
            raise Unproven('branch-cond', '%s: jump-table index differs' % where)

    def fixpoint_paths(self):
        P = self.P
        self.cuts = self.cut_points()
        self.paths = {}
        self.R = {0: self.entry_relation()}
        work = [0]
        it = 0
        while work:
            it += 1
            if it > 5000:
                raise Unproven('limit', 'path fixpoint did not converge')
            c = work.pop()
            if c not in self.paths:
                self.paths[c] = self.paths_from(c)
            for path, k in self.paths[c]:
                if k is None:
                    continue
                a, b = self.run_path(c, self.R[c], path)
                if a is None or self.infeasible(a, path[-1], k):
                    continue
                nxt = P.succs(path[-1])[k][1]
                new = self.exit_pairs(*self.edge(a, b, path[-1], k), nxt)
                old = self.R.get(nxt)
                if old is None:
                    self.R[nxt] = new
                    work.append(nxt)
                elif (old & new) != old:
                    self.R[nxt] = old & new
                    work.append(nxt)
        for _ in range(200):
            changed = False
            for c in sorted(self.R):
                if c not in self.paths:
                    self.paths[c] = self.paths_from(c)
                for path, k in self.paths[c]:
                    if k is None:
                        continue
                    a, b = self.run_path(c, self.R[c], path)
                    if a is None or self.infeasible(a, path[-1], k):
                        continue
                    nxt = P.succs(path[-1])[k][1]
                    if nxt not in self.R:
                        continue
                    inter = self.R[nxt] & self.exit_pairs(*self.edge(a, b, path[-1], k), nxt)
                    if inter != self.R[nxt]:
                        self.R[nxt] = inter
                        changed = True
            if not changed:
                break

    def final_paths(self):
        self.cx.needs = set()
        self.runs = {}
        for c in sorted(self.R):
            for q, (path, k) in enumerate(self.paths[c]):
                a, b = self.run_path(c, self.R[c], path)
                if a is None or (k is not None and self.infeasible(a, path[-1], k)):
                    continue                   # an infeasible path
                self.runs[(c, q)] = (a, b)
                self.check_block(path[-1], a, b, only_succ=k)

    def final(self):
        self.pending_ret = set()
        if self.path_mode:
            return self.final_paths()
        self.cx.needs = set()
        self.runs = {}
        for bp in sorted(self.R):
            a, b = self.run_pair(bp, self.R[bp])
            self.runs[bp] = (a, b)
            self.check_block(bp, a, b)
        self.check_pending_returns()

    def _where(self, bp):
        return 'P block @%d' % self.P.blocks[bp][0]

    def check_block(self, bp, a, b, only_succ=-1):
        P, S = self.P, self.S
        self._last_bp = bp
        if not self.path_mode:
            self._fail_bp = bp                 # where block mode stops, if it does
        self._last_runs = (a, b)
        where = self._where(bp)
        ba = [e for e in a.events if e.kind in ('call', 'bar')]
        bb = [e for e in b.events if e.kind in ('call', 'bar')]
        if len(ba) != len(bb) or any(x.kind != y.kind for x, y in zip(ba, bb)):
            raise Unproven('barrier-sequence', '%s: call/barrier sequence differs' % where)
        for x, y in zip(ba, bb):
            if x.kind == 'bar':
                if not eq(x.term, y.term):
                    raise Unproven('barrier-sequence', '%s: %s @%d vs %s @%d differ' % (
                        where, x.name, x.pos, y.name, y.pos))
                continue
            tx, argx, stx, ar, nstx, slx = x.extra
            ty, argy, sty, _, nsty, sly = y.extra
            if (slx is None) != (sly is None) or (slx and (slx[0] != sly[0] or
                                                         not eq(slx[1], sly[1]))):
                raise Unproven('hazard', '%s: call @%d: a load in the call delay slot on '
                               'one side only (the callee sees the old value)' % (where, x.pos))
            if not eq(tx, ty):
                raise Unproven('call-target', '%s: call @%d targets %s, S @%d targets %s' % (
                    where, x.pos, fmt(tx), y.pos, fmt(ty)))
            # the callee's INPUT registers, read off its retail code; the C
            # arity is the fallback for an unresolvable target (jalr)
            # A call to a proper C function entry (a function of this tree
            # whose window STARTS at the target) follows the ABI: it reads
            # its declared arguments.  Any other target (mid-function entry,
            # assembly, BIOS) has its input registers read off the retail
            # code, conservatively (unfollowable inner calls read $a0-$a3).
            ins_ = None
            if tx.op == 'fn':
                ta = name_addr(tx.a[0])
                if ta is not None and not is_c_function_start(ta, self.home) and \
                        _image_for(ta, self.home)[1] is not None:
                    ins_ = callee_inputs(ta, self.home,
                                         unknown_reads=frozenset({4, 5, 6, 7}))
                    if ins_ is None:
                        raise Unproven('call-args', '%s: call @%d to %s: not a C function '
                                       'entry and its inputs cannot be derived' % (
                                           where, x.pos, tx.a[0]))
            if ins_ is not None:
                for r in sorted(ins_):
                    if not eq(x.regs_at[r], y.regs_at[r]):
                        raise Unproven('call-args', '%s: call @%d to %s (not a function entry): '
                                       'its input $%s %s vs %s' % (
                                           where, x.pos, tx.a[0], RNAME[r], fmt(x.regs_at[r]),
                                           fmt(y.regs_at[r])))
                self.notes.add('callee-inputs')
                nreg = 0
            else:
                if ar == 'agg?':
                    raise Unproven('call-args', '%s: call @%d to %s passes an aggregate by '
                                   'value whose size could not be measured' % (
                                       where, x.pos, tx.a[0] if tx.op == 'fn' else 'jalr'))
                nreg = 4 if ar is None else min(ar, 4)
                if ar is not None:
                    self.notes.add('c-arity')
                if tx.op == 'fn' and name_addr(tx.a[0]) is not None and \
                        _image_for(name_addr(tx.a[0]), self.home)[1] is None:
                    self.notes.add('callee-outside-images')
            for q in range(nreg):
                if not eq(argx[q], argy[q]):
                    raise Unproven('call-args', '%s: call @%d ($a%d) %s vs %s%s' % (
                        where, x.pos, q, fmt(argx[q]), fmt(argy[q]),
                        '' if ar is not None else ' [callee arity unknown]'))
            nst = max(0, ar - 4) if ar is not None else nstx
            if ar is None and nstx != nsty:
                raise Unproven('call-args', '%s: call @%d passes %d stack words vs %d' % (
                    where, x.pos, nstx, nsty))
            if nst > MAX_STACK_ARGS:
                raise Unproven('call-args', '%s: call @%d with %d stack arguments' % (
                    where, x.pos, nst))
            for q in range(nst):
                if not eq(stx[q], sty[q]):
                    raise Unproven('call-args', '%s: call @%d stack argument %d %s vs %s' % (
                        where, x.pos, q + 5, fmt(stx[q]), fmt(sty[q])))
        sa = [e for e in a.events if e.kind == 'st']
        sb = [e for e in b.events if e.kind == 'st']
        if len(sa) != len(sb):
            raise Unproven('store-multiset', '%s: %d stores vs %d' % (where, len(sa), len(sb)))
        mst = self._match(sa, sb)
        if mst is None:
            raise Unproven('store-value', '%s: stores do not match one-to-one (%s)' % (
                where, self._first_unmatched(sa, sb)))
        la = [e for e in a.events if e.kind == 'ld' and e.cls[0] not in ('F', 'FX')]
        lb = [e for e in b.events if e.kind == 'ld' and e.cls[0] not in ('F', 'FX')]
        if P.blocks[bp][2][0] == 'trap' and (len(la) != len(lb) or
                                             self._match(la, lb) is None):
            # this execution divides by zero (or INT_MIN by -1): undefined
            # behaviour in C, so the abstract machine requires nothing of it.
            # Conservatively its stores and calls up to the trap are still
            # matched and ordered; only the LOADS (a value nobody can use any
            # more -- the trap is the last thing the path does) are released.
            # Both sides end this path in the SAME trap (same terminator,
            # same branch conditions).  A load from hardware (I/O,
            # scratchpad, BIOS) has an effect and stays matched.
            la = [e for e in la if self._hardware(e)]
            lb = [e for e in lb if self._hardware(e)]
            self.notes.add('ub-trap-path')
        if len(la) != len(lb):
            raise Unproven('load-multiset', '%s: %d non-frame loads vs %d' % (
                where, len(la), len(lb)))
        mld = self._match(la, lb)
        if mld is None:
            raise Unproven('load-value', '%s: non-frame loads do not match (%s)' % (
                where, self._first_unmatched(la, lb)))
        m = list(zip(ba, bb)) + mst + mld
        m.sort(key=lambda xy: xy[0].seq)
        for i in range(len(m)):
            x, xs = m[i]
            for j in range(i + 1, len(m)):
                y, ys = m[j]
                if (xs.seq < ys.seq) != (x.seq < y.seq):
                    why = self.conflict(x, y) or self.conflict(xs, ys)
                    if why:
                        raise Unproven('memory-order', '%s: %s @%d and %s @%d are reordered '
                                       'in S (%s): %s / %s' % (
                                           where, x.name, x.pos, y.name, y.pos, why,
                                           x.cls, y.cls))
        term = P.blocks[bp][2][0]
        if term == 'cond':
            if not eq(a.cond, b.cond):
                raise Unproven('branch-cond', '%s: condition %s vs %s' % (
                    where, fmt(a.cond), fmt(b.cond)))
        elif term == 'jt':
            if not eq(a.jt_target, b.jt_target):
                raise Unproven('branch-cond', '%s: jump-table index %s vs %s' % (
                    where, fmt(a.jt_target), fmt(b.jt_target)))
        elif term == 'ret':
            if not eq(a.ret_ra, b.ret_ra):
                raise Unproven('return', '%s: returns to %s vs %s' % (
                    where, fmt(a.ret_ra), fmt(b.ret_ra)))
            if a.sp_out.a[1] != 0 or b.sp_out.a[1] != 0:
                raise Unproven('frame', '%s: $sp not restored (%d / %d)' % (
                    where, a.sp_out.a[1], b.sp_out.a[1]))
            regs = list(CALLEE_SAVED)
            if not self.void:
                regs.append(V0)
            if self.ret64:
                regs.append(V1)
            for r in regs:
                if r == V0 and self.noval and not eq(a.regs[r], b.regs[r]) and \
                        _mentions_sym(a.regs[r], self.noval):
                    # this path leaves P' through a value-less exit of C
                    self.notes.add('valueless-exit')
                    continue
                if r == V0 and self.flow is not None and not eq(a.regs[r], b.regs[r]) and \
                        not self.path_mode and self._v0_untouched(bp) and \
                        bp not in self.pending_ret:
                    # block mode: $v0 at this return is the block's entry
                    # value; decided per INCOMING EDGE after every block ran
                    self.pending_ret.add(bp)
                    continue
                if r == V0 and self.flow is not None and not eq(a.regs[r], b.regs[r]):
                    smap, callrets = self.flow
                    tv = a.regs[r]
                    if tv.op == 'c' and tv.a[0] in smap:
                        # a `return K;` path of C'': S must return K
                        if eq(b.regs[r], self.cx.al.c(smap[tv.a[0]])):
                            self.notes.add('sentinel-return')
                            continue
                    elif not self._maybe_valued(tv, callrets):
                        self.notes.add('valueless-flow')
                        continue
                if r == V0 and self.valueless and not eq(a.regs[r], b.regs[r]) and \
                        self._valueless_def(a.regs[r]):
                    # nothing on this path wrote $v0 after the last call to a
                    # void-declared callee (or after entry): it cannot be a
                    # `return <expr>;` path, whose value IDO must write into
                    # $v0 after the last call
                    self.notes.add('valueless-path')
                    continue
                if not eq(a.regs[r], b.regs[r]):
                    raise Unproven('return', '%s: $%s at return %s vs %s' % (
                        where, RNAME[r], fmt(a.regs[r]), fmt(b.regs[r])))
        a0, b0 = a, b
        for q_, ((_, sp_), (_, ss_)) in enumerate(zip(P.succs(bp), S.succs(self.pairs[bp]))):
            if only_succ != -1 and q_ != only_succ:
                continue
            if self.infeasible(a0, bp, q_):
                continue
            a, b = self.edge(a0, b0, bp, q_)
            for x in self.R.get(sp_, ()):
                if len(x) == 6:
                    _, p, s_, K, _z, _z2 = x
                    if not eq(self._get(a, p), self.cx.al.add(self._get(b, s_), K)):
                        raise Unproven('relation', '%s: cross-side offset fact broken' % where)
                    continue
                if len(x) == 5:
                    run = a if x[1] == 'P' else b
                    if self.cx.prov(self._get(run, x[2])) != x[3]:
                        raise Unproven('relation', '%s: provenance fact broken' % where)
                    continue
                if len(x) == 4:
                    _, (p, s_), (m, s2), K = x
                    al = self.cx.al
                    if not (eq(al.sub(self._get(a, p), self._get(a, m)), K) and
                            eq(al.sub(self._get(b, s_), self._get(b, s2)), K)):
                        raise Unproven('relation', '%s: difference fact broken' % where)
                    continue
                if len(x) == 3:
                    run = a if x[0] in ('KP', 'DP') else b
                    want = x[2]
                    if x[0] in ('DP', 'DS'):
                        want = self._instantiate(want, lambda n_: self._get(run, n_))
                    if not eq(self._get(run, x[1]), want):
                        raise Unproven('relation', '%s: constant/derived fact broken' % where)
                    continue
                p, s = x
                if not eq(self._get(a, p), self._get(b, s)):
                    raise Unproven('relation', '%s: %s/%s relation broken on an edge' % (
                        where, p, s))

    def _v0_untouched(self, bp):
        """Neither side's return block writes $v0 or calls."""
        for f, b_ in ((self.P, bp), (self.S, self.pairs[bp])):
            s0, e0, _ = f.blocks[b_]
            for i in range(s0, e0):
                d = f.ins[i]
                if V0 in d.defs or d.kind in ('jal', 'jalr'):
                    return False
        return True

    def check_pending_returns(self):
        """$v0 at a return block that does not write it, edge by edge: on an
        edge where P's $v0 is a sentinel S must hold that literal; an edge
        on which P's taint analysis says no path can carry a return value
        is a value-less exit; anything else is unproven."""
        if not self.pending_ret:
            return
        smap, callrets = self.flow
        taint = valued_taint(self.P, smap.keys(), callrets, self.voidcallees, self.nonvoid)
        for x in sorted(self.pending_ret):
            for bp in sorted(self.runs):
                for k_, (_, sp_) in enumerate(self.P.succs(bp)):
                    if sp_ != x or self.infeasible(self.runs[bp][0], bp, k_):
                        continue
                    a, b = self.edge(*self.runs[bp], bp, k_)
                    tv, ts = a.regs[V0], b.regs[V0]
                    if eq(tv, ts):
                        continue
                    if tv.op == 'c' and tv.a[0] in smap:
                        if eq(ts, self.cx.al.c(smap[tv.a[0]])):
                            self.notes.add('sentinel-return')
                            continue
                        raise Unproven('return', '%s: $v0 on the edge from P @%d: '
                                       'sentinel for %d, S has %s' % (
                                           self._where(x), self.P.blocks[bp][0],
                                           smap[tv.a[0]], fmt(ts)))
                    tn = taint.get((bp, k_))
                    if tn is not None and not tn[V0]:
                        self.notes.add('valueless-edge')
                        continue
                    raise Unproven('return', '%s: $v0 on the edge from P @%d %s vs %s' % (
                        self._where(x), self.P.blocks[bp][0], fmt(tv), fmt(ts)))

    def _maybe_valued(self, t, callrets):
        """Could P's $v0 term t be the value of a `return <literal>;` (a
        constant: in C'' its sentinel) or of a `return f(...);` (data flow
        from a call result)?  Conservatively yes for any constant, any value
        of unknown provenance (a block-entry variable other than the
        function entry's, junk, a frame load or call zone), and -- when C
        has call returns -- any call result.  A non-frame load is memory, an
        opaque value that neither is constant nor flows from a call."""
        if _is_const(t):
            return True
        entry_ok = not any(s_ == 0 for b_ in range(len(self.P.blocks))
                           for _, s_ in self.P.succs(b_))
        seen, stack = set(), [t]
        while stack:
            x = stack.pop()
            if id(x) in seen:
                continue
            seen.add(id(x))
            op = x.op
            if op == 'c' or op in ('sym', 'hi16', 'lo16', 'fn', 'fnabs', 'retaddr'):
                continue
            if op == 'in':
                if x.a[0] == 0 and entry_ok:
                    continue
                return True
            if op in ('junk', 'zone', 'zoneld', 'ldz', 'fr', 'obj', 'copval', 'ref'):
                return True
            if op in ('ulw', 'ulwc'):
                return True
            if op in ('ld', 'lwx'):
                if _mentions_op(x.a[0] if op == 'ld' else x.a[1], ('fr', 'obj')):
                    return True              # a frame slot: may hold anything
                if op == 'lwx':
                    stack.append(x.a[2])     # lwl/lwr merge the old register
                continue                     # non-frame memory: opaque
            if op == 'ret':
                if callrets:
                    return True
                stack.extend(y for y in x.a[3:] if isinstance(y, T))
                continue
            if op == 'retv':
                continue                     # written by a void callee: no C value
            stack.extend(y for y in x.a if isinstance(y, T))
        return False

    def _valueless_def(self, t):
        """P's $v0 at a return was last defined by a call to a callee the C
        declares void, or is the function-entry value (block 0 has no
        predecessor, so its 'in' terms are the caller's registers)."""
        if t.op == 'retv':
            return True
        if t.op == 'in' and t.a[0] == 0 and t.a[1] == (0, V0):
            return not any(s_ == 0 for b_ in range(len(self.P.blocks))
                           for _, s_ in self.P.succs(b_))
        return False

    @staticmethod
    def _hardware(e):
        """An access the machine model orders: I/O, scratchpad, BIOS (any
        segment), or through a pointer that may hold such an address."""
        c = e.cls
        if c is None:
            return False
        if c[0] == 'A':
            return (c[1] & 0x1FFFFFFF) >= 0x1F000000
        return False

    def _c_volatile(self, e):
        """The C declares this location volatile (a RAM global, or any
        pointer access when the file forms a pointer to volatile)."""
        c = e.cls
        if c is None:
            return False
        if c[0] == 'A':
            return any(v <= c[1] < v + 4 for v in self.cx.volatile_addrs)
        if c[0] in ('G', 'GX') and c[1] in self.cx.volatile:
            return True
        if c[0] in ('P', 'GX') and '*ptr' in self.cx.volatile:
            return True
        return False

    def conflict(self, x, y):
        """Why the order of x and y matters ('' when it does not)."""
        kx, ky = x.kind, y.kind
        if kx in ('call', 'bar') or ky in ('call', 'bar'):
            o = y if kx in ('call', 'bar') else x
            if o.kind in ('call', 'bar'):
                return 'barrier'
            if o.cls[0] in ('F', 'FX'):
                return 'barrier' if (o.kind == 'st' and self.cx.escaped and
                                     not is_save(o)) else ''
            return 'barrier'
        if self._hardware(x) and self._hardware(y):
            return 'hardware'
        # a pointer access may reach hardware: keep it ordered against a
        # hardware access
        if (self._hardware(x) and y.cls[0] in ('P', 'GX')) or \
                (self._hardware(y) and x.cls[0] in ('P', 'GX')):
            return 'hardware'
        if kx == 'ld' and ky == 'ld':
            if self._c_volatile(x) and self._c_volatile(y):
                self.notes.add('c-volatile-order')
            return ''
        if may_alias(x, y, self.cx.escaped):
            return 'may-alias'
        if self._c_volatile(x) and self._c_volatile(y):
            self.notes.add('c-volatile-order')
        return ''

    def _match(self, xa, xb):
        """One-to-one matching by term identity, nearest rank first (equal
        terms are interchangeable, so greedy is complete)."""
        n = len(xa)
        used = [False] * n
        out = []
        by = collections.defaultdict(list)
        for j, y in enumerate(xb):
            by[id(y.term)].append(j)
        for i, x in enumerate(xa):
            cand = [j for j in by.get(id(x.term), ()) if not used[j]]
            if not cand:
                return None
            j = min(cand, key=lambda j: abs(j - i))
            used[j] = True
            out.append((x, xb[j]))
        return out

    def _first_unmatched(self, xa, xb):
        ids = collections.Counter(id(y.term) for y in xb)
        for x in xa:
            if ids[id(x.term)] == 0:
                return '%s @%d %s%s' % (x.name, x.pos, fmt(x.term), self._why_reordered())
            ids[id(x.term)] -= 1
        return '?'

    def _why_reordered(self):
        """Diagnosis only: the first may-alias memory pair whose order
        differs between P and S when accesses are matched by ADDRESS alone
        (values ignored)."""
        a, b = getattr(self, '_last_runs', (None, None))
        if a is None:
            return ''
        ma = [e for e in a.events if e.kind in ('st', 'ld', 'call') and
              (e.kind == 'call' or e.cls[0] not in ('F', 'FX'))]
        mb = [e for e in b.events if e.kind in ('st', 'ld', 'call') and
              (e.kind == 'call' or e.cls[0] not in ('F', 'FX'))]
        key = lambda e: (e.kind, e.name, id(e.addr) if e.addr is not None else id(e.term))
        pool = collections.defaultdict(list)
        for e in mb:
            pool[key(e)].append(e)
        pairs = []
        for e in ma:
            if pool[key(e)]:
                pairs.append((e, pool[key(e)].pop(0)))
        for i in range(len(pairs)):
            for j in range(i + 1, len(pairs)):
                (x, xs), (y, ys) = pairs[i], pairs[j]
                if x.kind == 'ld' and y.kind == 'ld':
                    continue
                if (x.seq < y.seq) != (xs.seq < ys.seq):
                    why = self.conflict(x, y) or self.conflict(xs, ys)
                    if why:
                        return ' [S moves %s @%d across %s @%d (%s)]' % (
                            y.name, ys.pos, x.name, xs.pos, why)
        return ''

    # --- mode B: shared frame memory ------------------------------------
    def check_mode_b(self):
        cx = self.cx
        tops = []
        for a, b in self.runs.values():
            tops += [k + s for k, s in a.zones] + [k + s for k, s in b.zones]
        top = max(tops) if tops else None
        for a, b in self.runs.values():
            for run, side in ((a, 'P'), (b, 'S')):
                for i, v in run.fr_values:
                    for leaf in _fr_leaves(v):
                        if leaf < 0 and top is not None and leaf < top:
                            raise Unproven('escape-zone', '%s: frame address %d formed at %d '
                                           'lies in a call home/argument zone (top %d)'
                                           % (side, leaf, i, top))
        if not cx.needs:
            return
        vin = {0: None}                    # None = every byte valid
        work = [0]
        it = 0
        while work:
            it += 1
            if it > 50000:
                raise Unproven('limit', 'validity dataflow did not converge')
            bp = work.pop()
            a, b = self.runs[bp]
            out = self._invalid_out(vin[bp], a, b)
            for _, sp_ in self.P.succs(bp):
                if sp_ is None or sp_ not in self.runs:
                    continue
                if sp_ not in vin:
                    vin[sp_] = out
                    work.append(sp_)
                else:
                    old = vin[sp_]
                    new = (old or set()) | (out or set())
                    if new != (old or set()):
                        vin[sp_] = new
                        work.append(sp_)
        for bp, byte in sorted(cx.needs, key=repr):
            bad = vin.get(bp)
            if bad and byte == 'any':
                raise Unproven('frame-uninit', '%s makes a dynamic frame load while frame '
                               'bytes a call may have clobbered differently are live' %
                               self._where(bp))
            if bad and byte in bad:
                raise Unproven('frame-uninit', '%s reads frame byte %d, which a call may '
                               'have clobbered differently on the two sides' % (
                                   self._where(bp), byte))

    @staticmethod
    def _invalid_out(inv, a, b):
        """Mode B validity as the set of INVALID frame bytes: a call's
        home/argument zone is invalidated (on either side's own $sp), a
        store makes its bytes valid only if it is the last writer on both
        sides (stores are matched one-to-one at identical addresses)."""
        bad = set(inv or ())
        for run in (a, b):
            for e in run.events:
                if e.kind == 'clob':
                    bad |= set(range(e.cls[2], e.cls[2] + e.size))
        last = {}
        for side, run in (('P', a), ('S', b)):
            for e in run.events:
                if e.kind == 'clob':
                    for x in range(e.cls[2], e.cls[2] + e.size):
                        last[(side, x)] = 'clob'
                elif e.kind == 'st' and e.cls[0] == 'F' and e.name in ('sb', 'sh', 'sw'):
                    for x in range(e.cls[2], e.cls[2] + e.size):
                        last[(side, x)] = 'st'
        for (side, x), v in list(last.items()):
            if side == 'P' and v == 'st' and last.get(('S', x)) == 'st':
                bad.discard(x)
        return bad


def _mentions_op(t, ops):
    """Does term t contain a node whose op is in `ops` (unbounded depth)?"""
    seen, stack = set(), [t]
    while stack:
        x = stack.pop()
        if not isinstance(x, T) or id(x) in seen:
            continue
        seen.add(id(x))
        if x.op in ops:
            return True
        stack.extend(x.a)
    return False


def _mentions_sym(t, name):
    """Does term t contain a relocation term (sym/hi16/lo16) on `name`?"""
    seen, stack = set(), [t]
    while stack:
        x = stack.pop()
        if id(x) in seen:
            continue
        seen.add(id(x))
        if x.op in ('sym', 'hi16', 'lo16') and x.a and x.a[0] == name:
            return True
        stack.extend(y for y in x.a if isinstance(y, T))
    return False


_LOADED = ('ld', 'ldz', 'zoneld', 'ulw', 'ulwc', 'lwc2', 'copval')


def _fr_leaves(t, depth=0):
    """Frame offsets of the frame ADDRESSES that term t's VALUE is derived
    from (unbounded depth).  A loaded value is memory content, not an
    address computed from $sp, so a load's operands are not descended into
    (a frame address that reaches memory was recorded where it was formed,
    before the store); `lwx` (lwl/lwr) merges into its old register value,
    which is."""
    out, seen, stack = [], set(), [t]
    while stack:
        x = stack.pop()
        if not isinstance(x, T) or id(x) in seen:
            continue
        seen.add(id(x))
        if x.op == 'fr':
            out.append(x.a[1])
        elif x.op in _LOADED:
            continue
        elif x.op == 'lwx':
            stack.append(x.a[2])
        else:
            stack.extend(x.a)
    return out


def prove_pair(P, S, **kw):
    """-> (verdict, class, message, mode)."""
    pr = Prover(P, S, **kw)
    try:
        pr.prove()
        return 'PROVEN', None, ','.join(sorted(pr.notes)), pr.mode
    except Unproven as u:
        v = 'REFUTED' if u.cls in REFUTE_CLASSES else 'UNPROVEN'
        return v, u.cls, u.msg, pr.mode
    except RecursionError:
        return 'UNPROVEN', 'limit', 'term too deep (recursion limit)', pr.mode


# ---------------------------------------------------------------------------
# Whole-program side lemma: does any caller read $v0 after the call?
# ---------------------------------------------------------------------------

_IMAGES = None
MAIN_TEXT = 0x80041000


def retail_images():
    """[(name, base address, words)] of the retail main binary and every
    overlay EXN (each overlay loads at 0x80132000)."""
    global _IMAGES
    if _IMAGES is not None:
        return _IMAGES
    out = []
    exe = ROOT / 'disks' / 'SLPS_000.65'
    if exe.exists():
        raw = exe.read_bytes()
        tsize = struct.unpack_from('<I', raw, 0x1C)[0]
        d = raw[0x800:0x800 + tsize]
        out.append(('main', MAIN_TEXT, struct.unpack('<%dI' % (len(d) // 4), d[:len(d) // 4 * 4])))
    for exn in sorted((ROOT / 'assets' / 'overlays').glob('*.EXN')):
        d = exn.read_bytes()
        out.append((exn.stem, 0x80132000, struct.unpack('<%dI' % (len(d) // 4),
                                                         d[:len(d) // 4 * 4])))
    _IMAGES = out
    return out


_SYMS = {}
_LINKSHIFT = {}


def overlay_link_shift(home):
    """Runtime address minus .sym address for overlay `home` (0 for every
    overlay that loads at 0x80132000).  MEASURED from the EXN's own `jal`s:
    the shift under which (nearly) every jal into the overlay's own range
    lands on a .sym entry.  GYOZI was linked at 0x80134000 (+0x2000: 153 of
    153 internal jals hit an entry) and EVENT at 0x800F6000 (-0x3C000:
    208 of 216); at shift 0 neither hits any.  Both overlays are also linked
    against a different MAIN layout (main_image_matches) and their LBAs are
    never referenced by the retail loader (GAME-ARCHITECTURE.md): they are
    leftovers of another build.  The shift is used only to read the callee
    code a jal really reaches; a shift is accepted only when it explains
    >= 90% of the internal jals and shift 0 explains <= 10%."""
    if home == 'main':
        return 0
    if home in _LINKSHIFT:
        return _LINKSHIFT[home]
    _LINKSHIFT[home] = 0
    try:
        containing_function(home, 0)
    except Exception:                                # noqa: BLE001
        return 0
    starts = sorted({a0 for a0, _sz, _nm, _c in _SYMS.get(home, [])})
    imgs = [(b, W) for n, b, W in retail_images() if n == home]
    if not starts or not imgs:
        return 0
    b, W = imgs[0]
    lo_, hi_ = b - 0x40000, b + 4 * len(W) + 0x40000
    tg = [0x80000000 | ((w & 0x3FFFFFF) << 2) for w in W if w >> 26 == 3]
    tg = [t for t in tg if lo_ <= t < hi_]
    if len(tg) < 8:
        return 0
    import bisect
    cnt = collections.Counter()
    for t in tg:
        j = bisect.bisect_left(starts, t - 0x40000)
        while j < len(starts) and starts[j] <= t + 0x40000:
            cnt[t - starts[j]] += 1            # runtime - .sym
            j += 1
    if not cnt:
        return 0
    sh, n = cnt.most_common(1)[0]
    if sh != 0 and n >= 0.9 * len(tg) and cnt.get(0, 0) <= 0.1 * len(tg):
        _LINKSHIFT[home] = sh
    return _LINKSHIFT[home]


def containing_function(home, a):
    """(start, name, C source path or None) of the function containing
    address `a` in image `home`."""
    if home not in _SYMS:
        tab = []
        if home == 'main':
            for e in json.load(open(REGISTRY))['functions']:
                tab.append((int(e['address'], 16), e['size'], e['name'],
                            ROOT / e['source'] if e.get('source', '').endswith('.c') else None))
        else:
            for ln in (ROOT / 'config/overlay' / f'{home}.sym').read_text().splitlines():
                p_ = ln.strip().split(':')
                if len(p_) >= 3 and not ln.startswith('#'):
                    try:
                        a0, sz = int(p_[0], 16), int(p_[1], 0)
                    except ValueError:
                        continue
                    c = ROOT / 'src/overlay' / home / (p_[2].strip() + '.c')
                    tab.append((a0, sz, p_[2].strip(), c if c.exists() else None))
        _SYMS[home] = sorted(tab)
    best = None
    for a0, sz, nm, c in _SYMS[home]:
        if a0 <= a < a0 + sz:
            best = (a0, nm, c)
    return best


_AUDIT = None


def is_c_function_start(addr, home):
    """True when a FUNCTION (C or assembly) starts at `addr`: the main
    audit's matched_c / asm_only entries, or an overlay .sym symbol.  A
    'covered' address (inside another function's body) is not one."""
    global _AUDIT
    if _AUDIT is None:
        try:
            _AUDIT = json.load(open(ROOT / 'config' / 'main_function_audit_by_addr.json'))[
                'addresses']
        except Exception:                            # noqa: BLE001
            _AUDIT = {}
    e = _AUDIT.get('0x%08X' % addr)
    if e is not None:
        return e.get('kind') in ('matched_c', 'asm_only') or \
            e.get('enclosed_by') == '0x%08X' % addr
    for img in ([home] if home != 'main' else []) + ['main']:
        a = addr - overlay_link_shift(img) if img != 'main' else addr
        try:
            cf = containing_function(img, a)
        except Exception:                            # noqa: BLE001
            cf = None
        if cf is not None and cf[0] == a:
            return True
    return False


def v0_unused_by_callers(addr, home, depth=0):
    """True when the function at `addr` (in image `home`: 'main' or an
    overlay) has its address taken nowhere and no direct caller can read
    $v0 after the call returns -- then $v0 at its return is unobservable.
    Images searched: main + `home` (an overlay address is only meaningful
    while that overlay is loaded), or main + every overlay for a main
    function.  Conservative: anything not followed to a definite write of
    $v0 counts as a use."""
    imgs = [im for im in retail_images() if im[0] == 'main' or home == 'main' or im[0] == home]
    if not imgs:
        return False
    sh = overlay_link_shift(home)
    if sh:
        # callers reach the function at its RUNTIME address (.sym + shift);
        # main is not this overlay's main (main_image_matches), so main
        # callers are not considered for it -- conservatively refuse
        return False
    jal = (3 << 26) | ((addr >> 2) & 0x3FFFFFF)
    hi, lo = (addr + 0x8000) >> 16, addr & 0xFFFF
    for name, base, W in imgs:
        n = len(W)
        for i, w in enumerate(W):
            if w == addr:
                return False                       # address stored as data
            op = w >> 26
            if op == 15 and (w & 0xFFFF) == hi:    # lui r, %hi(addr)
                r = (w >> 16) & 31
                for j in range(i + 1, min(i + 16, n)):
                    x = W[j]
                    if (x >> 26) in (9, 13) and (x >> 21) & 31 == r and (x & 0xFFFF) == lo:
                        return False               # address formed in code
            if w == jal:
                r = _v0_dead_after(W, i)
                if r is True:
                    continue
                if r == 'returned' and depth < 3:
                    # the caller returns our $v0 as its own: fine if the
                    # caller is void in its C, or nobody reads ITS $v0
                    cf = containing_function(name, base + 4 * i)
                    if cf is not None:
                        if cf[2] is not None and return_kind(
                                Path(cf[2]).read_text(errors='replace'), cf[1])[0]:
                            continue
                        if v0_unused_by_callers(cf[0], name, depth + 1):
                            continue
                return False
    return True


def _v0_dead_after(W, j):
    """True: every path writes $v0 before reading it.  'returned': the only
    reads are the caller's own return.  False otherwise."""
    n = len(W)
    returned = False
    work = [(j + 1, 0, True)]                      # (pc, steps, pc is a call slot)
    seen = set()
    while work:
        pc, steps, slot = work.pop()
        if (pc, slot) in seen:
            continue
        seen.add((pc, slot))
        if steps > 64 or pc >= n:
            return False
        d = decode(pc, W[pc])
        if d.kind == 'unknown':
            return False
        if V0 in d.uses:
            return False
        if slot:                                   # the call's delay slot
            if V0 in d.defs:
                continue
            work.append((pc + 1, steps + 1, False))
            continue
        if d.kind in ('br', 'j'):
            if pc + 1 >= n:
                return False
            sl = decode(pc + 1, W[pc + 1])
            if V0 in sl.uses:
                return False
            if V0 in sl.defs:
                continue
            if d.kind == 'br':
                work.append((branch_target(d), steps + 2, False))
                if not is_uncond_br(d):
                    work.append((pc + 2, steps + 2, False))
            else:
                return False                       # `j`: not followed (conservative)
            continue
        if d.kind == 'jr' and d.rs == RA:
            if pc + 1 >= n:
                return False
            sl = decode(pc + 1, W[pc + 1])
            if V0 in sl.uses:
                return False
            if V0 not in sl.defs:
                returned = True
            continue
        if d.kind in ('jr', 'jalr', 'jal', 'bal', 'syscall', 'break'):
            if d.kind in ('jal', 'jalr'):
                sl = decode(pc + 1, W[pc + 1]) if pc + 1 < n else None
                if sl is None or V0 in sl.uses:
                    return False
                continue                           # the callee overwrites $v0
            return False                           # returns it / unknown flow
        if V0 in d.defs:
            continue
        work.append((pc + 1, steps + 1, False))
    return 'returned' if returned else True


# ---------------------------------------------------------------------------
# Callee input registers, from the retail binary (replaces the C arity)
# ---------------------------------------------------------------------------

_CALLEE_IN = {}


# the retail main image holds DATA here (docs: the verified table-data range);
# whatever code runs there at run time is not in any image
_MAIN_DATA = (0x800BB3C0, 0x800C0000)


_MAIN_OK = {}


def main_image_matches(home):
    """Does the disc's main binary match what overlay `home` was linked
    against?  Measured: the share of its `jal`s into main that land on a
    function entry.  EVENT and GYOZI land ~0% (they call into the middle of
    main functions everywhere -- they were linked against a different main
    layout), every other overlay ~100%."""
    if home == 'main':
        return True
    if home not in _MAIN_OK:
        imgs = retail_images()
        mb = [(b, W) for n, b, W in imgs if n == 'main']
        ov = [W for n, b, W in imgs if n == home]
        ok = True
        if mb and ov:
            b, W = mb[0]
            st = mid = 0
            for w in ov[0]:
                if w >> 26 == 3:
                    t = 0x80000000 | ((w & 0x3FFFFFF) << 2)
                    if b <= t < b + 4 * len(W):
                        if is_c_function_start(t, home):
                            st += 1
                        else:
                            mid += 1
            ok = mid <= 0.1 * max(1, st + mid)
        _MAIN_OK[home] = ok
    return _MAIN_OK[home]


def _image_for(addr, home):
    if _MAIN_DATA[0] <= addr < _MAIN_DATA[1]:
        return None, None
    for name, base, W in retail_images():
        if home != 'main' and name == home:
            base = base + overlay_link_shift(home)       # runtime coordinates
            if base <= addr < base + 4 * len(W):
                return base, W
    if not main_image_matches(home):
        return None, None
    for name, base, W in retail_images():
        if name == 'main' and base <= addr < base + 4 * len(W):
            return base, W
    return None, None


def _items(W, i, n):
    """(uses, defs) of word i; a store of a callee-saved register or $ra
    to the stack is a SAVE, not a use."""
    d = decode(i, W[i])
    uses, defs = set(d.uses), set(d.defs)
    if d.kind == 'store' and d.rs == SP and (d.rt in CALLEE_SAVED or d.rt == RA):
        uses.discard(d.rt)
    return d, uses, defs


_BRT = {}


def _branch_targets(W):
    """Every word index some branch in image W can reach."""
    k = id(W)
    if k not in _BRT or _BRT[k][0] is not W:     # (keeps W alive: ids stay unique)
        out = set()
        for i, w in enumerate(W):
            op = w >> 26
            if op in (4, 5, 6, 7) or (op == 1 and ((w >> 16) & 31) in (0, 1, 16, 17)):
                out.add(i + 1 + _sx16(w & 0xFFFF))
        _BRT[k] = (W, out)
    return _BRT[k][1]


def _jtable_targets(W, base, i):
    """Word indices a `jr` at W[i] through an IDO jump table can reach, or
    None.  Only the exact IDO switch shape is accepted, all inside ONE
    straight-line run that nothing branches into:
        sltiu rC, rI, N   ...  beq rC,$0 (or bnez/beqz guard on rC) + slot
        sll   rQ, rI, 2   ; lui rP, %hi(T) ; addu rB, rP, rQ (either order)
        lw    rX, %lo(T)(rB) ; jr rX
    with rI unmodified between the sltiu and the sll.  The table T must lie
    in the same image and hold N in-image code addresses; those are the
    successors."""
    d = decode(i, W[i])
    rx = d.rs
    # the straight-line run ending at the jr: back to the previous
    # transfer's delay slot
    j = i - 1
    while j >= 0 and j > i - 24:
        dj = decode(j, W[j])
        if is_xfer(dj):
            break
        j -= 1
    if j < 0 or j <= i - 24:
        return None
    guard = decode(j, W[j])                 # must be the bound check's branch
    run = range(j + 2, i)                   # words after its slot, before jr
    if guard.kind != 'br':
        return None
    bt = _branch_targets(W)
    if any(k in bt for k in range(j + 2, i + 1)):
        return None                         # entered from elsewhere
    defs = {}
    for k in list(range(max(0, j - 12), j + 2)) + list(run):
        dk = decode(k, W[k])
        for r in dk.defs:
            defs.setdefault(r, []).append(k)

    def last_def(r, before):
        ks = [k for k in defs.get(r, []) if k < before]
        return ks[-1] if ks else None
    kl = last_def(rx, i)
    if kl is None or kl not in run:
        return None
    ld = decode(kl, W[kl])
    if ld.name != 'lw' or ld.rt != rx:
        return None
    ka = last_def(ld.rs, kl)
    if ka is None or ka not in run:
        return None
    ad = decode(ka, W[ka])
    if ad.name != 'addu':
        return None
    hi = sl = None
    for r in (ad.rs, ad.rt):
        kr = last_def(r, ka)
        if kr is None or not j + 1 <= kr < i:      # the guard's slot counts
            return None
        dr = decode(kr, W[kr])
        if dr.kind == 'lui':
            hi = dr.imm
        elif dr.name == 'sll' and dr.sa == 2:
            sl = (kr, dr.rt)
    if hi is None or sl is None:
        return None
    ksl, ri = sl
    # the bound check: guard = beq rC,$0 (bound fails -> skip) where
    # rC = sltiu rI, N, and rI is not redefined between it and the sll
    if not (guard.name == 'beq' and guard.rt == 0) and \
            not (guard.name == 'beq' and guard.rs == 0):
        return None
    rc = guard.rs if guard.rt == 0 else guard.rt
    kc = last_def(rc, j)
    if kc is None:
        return None
    sc = decode(kc, W[kc])
    if sc.name != 'sltiu' or sc.rt != rc:
        return None
    # the sll's index must hold the value sltiu tested: follow plain
    # register copies (or/addu x,y,$0 ; addiu x,y,0) back past the sltiu
    r, pos = ri, ksl
    while True:
        k = last_def(r, pos)
        if k is None or k < kc:
            break
        dk = decode(k, W[k])
        if dk.name in ('or', 'addu') and dk.rt == 0:
            r, pos = dk.rs, k
        elif dk.name in ('or', 'addu') and dk.rs == 0:
            r, pos = dk.rt, k
        elif dk.name == 'addiu' and dk.imm == 0:
            r, pos = dk.rs, k
        else:
            return None
    if r != sc.rs:
        return None
    if any(k in bt for k in range(kc + 1, j + 1)) or \
            any(is_xfer(decode(k, W[k])) for k in range(kc - 1, j)):
        return None                         # sltiu .. guard is one straight run
    n = sc.imm
    tab = ((hi << 16) + _sx16(ld.imm)) & 0xFFFFFFFF
    t0 = (tab - base) // 4
    if tab & 3 or n == 0 or n > 512 or not 0 <= t0 or t0 + n > len(W):
        return None
    out = []
    for q in range(n):
        e = W[t0 + q]
        if e & 3 or not base <= e < base + 4 * len(W):
            return None
        out.append((e - base) // 4)
    return out


def callee_inputs(addr, home, depth=0, unknown_reads=frozenset()):
    """Registers the retail code at `addr` can read before writing them.
    Liveness over every word reachable from `addr` (branches followed,
    calls stepped over, returns end a path).  An inner call reads its own
    inputs (recursively, else $a0-$a3) and clobbers the caller-saved set.
    None when the code cannot be followed."""
    key = (addr, home, unknown_reads)
    if key in _CALLEE_IN:
        return _CALLEE_IN[key]
    _CALLEE_IN[key] = None
    base, W = _image_for(addr, home)
    if W is None:
        return None
    n = len(W)
    start = (addr - base) // 4
    # a BIOS call thunk (`li t2,0xA0/0xB0/0xC0 ; jr t2 ; li t1,N`): the
    # BIOS function reads its arguments $a0-$a3
    if start + 2 < n:
        d0, d1, d2 = (decode(start + q, W[start + q]) for q in range(3))
        if d0.kind == 'alui' and d0.rt == 10 and d0.rs == 0 and d0.imm in (0xA0, 0xB0, 0xC0) \
                and d1.kind == 'jr' and d1.rs == 10 and d2.kind == 'alui' and d2.rt == 9:
            _CALLEE_IN[key] = frozenset({4, 5, 6, 7})
            return _CALLEE_IN[key]
    succ = {}
    work = [start]
    while work:
        i = work.pop()
        if i in succ:
            continue
        if not 0 <= i < n or len(succ) > 6000:
            return None
        d = decode(i, W[i])
        if d.kind in ('unknown', 'bal'):
            return None
        if d.kind == 'br':
            nx = [i + 1]                       # the slot, then the targets
            succ[i] = nx
            succ.setdefault(i + 1, None)
            tg = [branch_target(d)] + ([] if is_uncond_br(d) else [i + 2])
            succ[i + 1] = ('slot', tg)
            work.extend(tg)
            continue
        if d.kind == 'j':
            return None
        if d.kind == 'jr':
            succ[i] = [i + 1]
            if d.rs == RA:
                succ[i + 1] = ('slot', [])
                continue
            tg = _jtable_targets(W, base, i)
            if tg is None:
                return None
            succ[i + 1] = ('slot', tg)
            work.extend(tg)
            continue
        if d.kind in ('jal', 'jalr'):
            succ[i] = [i + 1]
            succ[i + 1] = ('callslot', [i + 2], d)
            work.append(i + 2)
            continue
        succ[i] = [i + 1]
        work.append(i + 1)
    live = {i: set() for i in succ}
    order = sorted(succ, reverse=True)
    changed = True
    rounds = 0
    while changed:
        changed = False
        rounds += 1
        if rounds > 200:
            return None
        for i in order:
            s_ = succ[i]
            if isinstance(s_, tuple):
                nexts = s_[1] or []
            else:
                nexts = s_
            out = set()
            for j in nexts:
                out |= live.get(j, set())
            if isinstance(s_, tuple) and s_[0] == 'callslot':
                cd = s_[2]
                inner = None
                if cd.kind == 'jal' and depth < 3:
                    tgt = (base & 0xF0000000) | ((cd.w & 0x3FFFFFF) << 2)
                    inner = callee_inputs(tgt, home, depth + 1, unknown_reads)
                ins_ = set(inner) if inner is not None else set(unknown_reads)
                out = (out - set(CALLER_CLOBBER)) | ins_
                if cd.kind == 'jalr':
                    out |= {cd.rs}
            d, uses, defs = _items(W, i, n)
            new = (out - defs) | uses
            if isinstance(s_, tuple) and s_[0] == 'slot':
                br = decode(i - 1, W[i - 1])
                new = (new - set(br.defs)) | set(br.uses)
            new.discard(0)
            if new != live[i]:
                live[i] = new
                changed = True
    # the branch word's own uses are folded into its slot entry above
    first = live[start]
    d0 = decode(start, W[start])
    if d0.kind in ('br', 'jr'):
        first = live[start + 1] | set(d0.uses)
    res = frozenset(r for r in first if r not in (SP, RA))
    _CALLEE_IN[key] = res
    return res


# ---------------------------------------------------------------------------
# Facts from the C source
# ---------------------------------------------------------------------------

def _strip_comments(text):
    return re.sub(r'/\*.*?\*/', ' ', text, flags=re.S)


def volatile_names(text):
    """Identifiers named in any declaration that says `volatile` (an
    over-approximation: more volatile names only add ordering constraints),
    plus '*ptr' when a pointer to volatile is formed (then every pointer
    access is treated as volatile)."""
    src = _strip_comments(text)
    out = set()
    for m in re.finditer(r'[^;{}]*\bvolatile\b[^;{}]*', src):
        stmt = m.group(0)
        out.update(re.findall(r'\b[A-Za-z_]\w*\b', stmt))
        if re.search(r'volatile[^;{}()]*\*', stmt) or re.search(r'\*\s*\)', stmt):
            out.add('*ptr')
    if re.search(r'\bPTR(8|16|32)\s*\(', src):
        out.add('*ptr')
    return out


def falls_off_end(text, name):
    """True when the C definition of `name` can reach its closing brace
    without a `return` (its last statement is not a return): the return
    value on such a path is indeterminate in C."""
    src = _strip_comments(text)
    m = re.search(r'\b' + re.escape(name) + r'\s*\([^;{}]*\)\s*(?:[^;{}()]*;\s*)*\{', src)
    if not m:
        return False
    i, depth = m.end(), 1
    while i < len(src) and depth:
        if src[i] == '{':
            depth += 1
        elif src[i] == '}':
            depth -= 1
        i += 1
    body = src[m.end():i - 1].rstrip()
    return not re.search(r'\breturn\b[^;]*;\s*$', body)


NOVAL_SYM = '__peq_noval'


def mask_c(text):
    """`text` with every comment and string/char literal body replaced by
    spaces (newlines kept), so indices and line numbers stay aligned."""
    out = list(text)
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith('/*', i):
            j = text.find('*/', i + 2)
            j = n if j < 0 else j + 2
            for q in range(i, j):
                if out[q] != '\n':
                    out[q] = ' '
            i = j
        elif c in '"\'':
            j = i + 1
            while j < n and text[j] != c and text[j] != '\n':
                j += 2 if text[j] == '\\' else 1
            for q in range(i + 1, min(j, n)):
                if out[q] != '\n':
                    out[q] = ' '
            i = j + 1
        else:
            i += 1
    return ''.join(out)


_DEF_PAT = r'(^|[;}\n])([^;{}()#]*?)\b%s\s*\(([^;{}]*?)\)\s*((?:[^;{}()]*;\s*)*)\{'


def function_body(text, name):
    """(masked text, return-type text, index of the body's `{`, index of its
    closing `}`) for the C definition of `name` -- or of the file's ONLY
    definition when no function has that name (as return_kind) -- else
    None."""
    src = mask_c(text)
    src_np = re.sub(r'(?m)^\s*#.*$', lambda m: ' ' * len(m.group(0)), src)
    if not re.search(_DEF_PAT % re.escape(name), src_np):
        defs = {m.group(3) for m in re.finditer(_DEF_PAT % r'([A-Za-z_]\w*)', src_np)}
        defs = {d for d in defs if d not in ('if', 'while', 'for', 'switch', 'return')}
        if len(defs) != 1:
            return None
        name = defs.pop()
    ms = list(re.finditer(_DEF_PAT % re.escape(name), src_np))
    if len(ms) != 1:
        return None
    m = ms[0]
    lb = m.end() - 1
    i, depth = m.end(), 1
    while i < len(src_np) and depth:
        if src_np[i] == '{':
            depth += 1
        elif src_np[i] == '}':
            depth -= 1
        i += 1
    if depth:
        return None
    rt = ' '.join(x for x in m.group(2).split() if x not in ('static', 'extern', 'inline'))
    return src_np, rt or 'int', lb, i - 1


def return_sites(text, name):
    """(valueless, valued) for the C definition of `name`: valueless counts
    the bare `return;` statements plus 1 when the body can reach its closing
    brace (its last statement is not a return); valued counts `return <expr>;`.
    None when the definition cannot be located, or when the file defines a
    macro that contains `return` (a macro could hide a return either way)."""
    fb = function_body(text, name)
    if fb is None:
        return None
    src, rt, lb, rb = fb
    if re.search(r'(?m)^\s*#\s*define\b.*\breturn\b', mask_c(text)):
        return None
    body = src[lb + 1:rb]
    bare = len(re.findall(r'\breturn\s*;', body))
    valued = len(re.findall(r'\breturn\b\s*[^\s;]', body))
    falls = not re.search(r'\breturn\b[^;]*;\s*$', body.rstrip())
    return bare + int(falls), valued


_RT_RANGE = {('s8',): (8, True), ('u8',): (8, False), ('signed', 'char'): (8, True),
             ('unsigned', 'char'): (8, False), ('s16',): (16, True), ('short',): (16, True),
             ('signed', 'short'): (16, True), ('u16',): (16, False),
             ('unsigned', 'short'): (16, False)}
_RT_32 = {'s32', 'u32', 'int', 'long', 'unsigned', 'signed'}


def _rt_width(rt):
    """(bits, signed) of a return type the C spells out, else None."""
    toks = tuple(x for x in rt.replace('*', ' * ').split() if x not in ('const', 'volatile'))
    if '*' in toks:
        return 32, False
    if toks in _RT_RANGE:
        return _RT_RANGE[toks]
    if toks and all(x in _RT_32 for x in toks):
        return 32, 'unsigned' not in toks and 'u32' not in toks
    return None


def _as_return(v, bits, signed):
    """The 32-bit $v0 image of integer v converted to the return type."""
    v &= (1 << bits) - 1
    if signed and v >> (bits - 1):
        v -= 1 << bits
    return v & 0xFFFFFFFF


_LIT = r'\(?\s*([-+]?\s*(?:0[xX][0-9a-fA-F]+|\d+))[uUlL]*\s*\)?'


def valued_return_kinds(text, name):
    """The `return <expr>;` statements of `name` when every one is either an
    integer literal or a direct call `f(...)`: (literals [(start, end, value)
    of the literal text], has_call_returns, (bits, signed)).  None otherwise."""
    fb = function_body(text, name)
    if fb is None:
        return None
    src, rt, lb, rb = fb
    w = _rt_width(rt)
    if w is None:
        return None
    lits, calls = [], False
    for m in re.finditer(r'\breturn\b\s*([^;]*);', src[lb + 1:rb]):
        e = m.group(1).strip()
        if not e:
            continue
        ml = re.fullmatch(_LIT, e)
        if ml:
            s0 = lb + 1 + m.start(1) + e.find(ml.group(1))
            lits.append((s0, s0 + len(ml.group(1)), int(ml.group(1).replace(' ', ''), 0)))
            continue
        mc = re.fullmatch(r'[A-Za-z_]\w*\s*\((.*)\)', e, re.S)
        if mc and mc.group(1).count('(') == mc.group(1).count(')'):
            calls = True
            continue
        return None
    return lits, calls, w


def instrument_sentinels(text, name, avoid_imms=()):
    """C'' = the C with each literal `return K;` of `name` replaced by its own
    sentinel literal: a value no other literal of the file (nor any 16-bit
    immediate of P, `avoid_imms`) has, in the return type's range, and in
    the same one-instruction class as K (signed 16 bits), so the compile
    keeps its shape.  -> (text, {sentinel $v0 image: K's $v0 image}) or None.
    Nothing can equal a sentinel by knowledge of a comparison, so on a
    `return K;` path P'' holds exactly that constant."""
    vk = valued_return_kinds(text, name)
    if vk is None or not vk[0]:
        return None
    lits, _, (bits, signed) = vk
    used = {int(x.replace(' ', ''), 0) & 0xFFFFFFFF
            for x in re.findall(r'\b(0[xX][0-9a-fA-F]+|\d+)', mask_c(text))}
    used |= {x & 0xFFFF for x in avoid_imms} | {x for x in avoid_imms}
    used |= {_as_return(x, bits, signed) for x in list(used)}
    used |= {_as_return(x, 16, True) for x in list(used)}
    lo, hi = (-128, 127) if (bits == 8 and signed) else (0, 255) if bits == 8 else (1000, 30000)
    cands = (v for v in range(hi, lo, -7) if v not in (0, 1) and (v & 0xFFFFFFFF) not in used
             and (v & 0xFFFF) not in used and _as_return(v, bits, signed) == (v & 0xFFFFFFFF))
    out, smap = text, {}
    for s0, e0, k in sorted(lits, reverse=True):
        if not -32768 <= _as_return(k, bits, signed) - (1 << 32 if _as_return(k, bits, signed) >> 31 else 0) <= 32767:
            return None
        sv = next(cands, None)
        if sv is None:
            return None
        smap[sv & 0xFFFFFFFF] = _as_return(k, bits, signed)
        out = out[:s0] + str(sv) + out[e0:]
    return out, smap


def needs_noval(text, name):
    """The function is non-void and has both value-less exits and valued
    returns: the return-value obligation then needs C' (instrument_valueless)."""
    if return_kind(text, name)[0]:
        return False
    rs = return_sites(text, name)
    return rs is None or (rs[0] > 0 and rs[1] > 0)


def instrument_valueless(text, name):
    """C' = the C with every value-less exit of `name` (each bare `return;`
    and the fall-off at the closing brace) replaced by `return (RT)(long)
    &__peq_noval;`, an address no other code in the file can form.  C' is a
    REFINEMENT of C: it fixes the indeterminate return value and changes
    nothing else.  Line numbers are preserved (no newline is added).  None
    when there is nothing to instrument or the definition is not found."""
    fb = function_body(text, name)
    if fb is None:
        return None
    src, rt, lb, rb = fb
    if NOVAL_SYM in text:
        return None
    ret = ' return (%s)(long)&%s; ' % (rt, NOVAL_SYM)
    body = src[lb + 1:rb]
    edits = []                                      # (start, end, replacement)
    for m in re.finditer(r'\breturn\s*;', body):
        edits.append((lb + 1 + m.start(), lb + 1 + m.end(), ret.strip()))
    if not re.search(r'\breturn\b[^;]*;\s*$', body.rstrip()):
        edits.append((rb, rb, ret))
    if not edits:
        return None
    edits.append((lb + 1, lb + 1, ' extern char %s; ' % NOVAL_SYM))
    out = text
    for s, e, r in sorted(edits, reverse=True):
        out = out[:s] + r + out[e:]
    return out


def return_kind(text, name):
    """(void, ret64) from the function's definition in its C file.  When
    the file defines no function of that name (a registry name that differs
    from the C name, e.g. after a head absorption), its ONLY definition."""
    src = _strip_comments(text)
    pat = r'(^|[;}\n])([^;{}()#]*?)\b%s\s*\(([^;{}]*?)\)\s*((?:[^;{}()]*;\s*)*)\{'
    if not re.search(pat % re.escape(name), src):
        defs = {m.group(3) for m in re.finditer(
            pat % r'([A-Za-z_]\w*)', src)}
        defs = {d for d in defs if d not in ('if', 'while', 'for', 'switch', 'return')}
        if len(defs) == 1:
            name = defs.pop()
    for m in re.finditer(pat % re.escape(name), src):
        pre = m.group(2).strip()
        toks = pre.replace('*', ' * ').split()
        toks = [t for t in toks if t not in ('static', 'extern', 'inline')]
        if '*' in toks:
            return False, False
        if toks == ['void']:
            return True, False
        if 'long' in toks and toks.count('long') >= 2:
            return False, True
        return False, False
    return False, False


_HDR = None


def header_prototypes():
    """{name: parameter count} from every ANSI prototype in include/ (None
    when prototypes disagree, are variadic, or K&R `f()`)."""
    global _HDR
    if _HDR is not None:
        return _HDR
    import audit_paint_isomorphism as iso
    tab = {}
    for h in sorted((ROOT / 'include').rglob('*.h')):
        src = _strip_comments(h.read_text(errors='replace'))
        src = re.sub(r'^\s*#.*$', ' ', src, flags=re.M)
        # the leading anchor is a LOOKBEHIND: a consuming `[;}]` was eaten by
        # the previous match's closing `;`, so every other prototype of a
        # run of declarations was skipped and the census flipped parity when
        # a line was added or removed (2026-11-10)
        for m in re.finditer(r'(?:^|(?<=[;}]))\s*(?:extern\s+|static\s+)?[A-Za-z_][\w\s\*]*?'
                             r'\b([A-Za-z_]\w*)\s*\(([^()]*(?:\([^()]*\)[^()]*)*)\)\s*;', src):
            name, body = m.group(1), m.group(2).strip()
            pre = m.group(0)[:m.group(0).rfind(name)]
            toks = pre.replace('*', ' * ').replace(';', ' ').replace('}', ' ').split()
            toks = [t for t in toks if t not in ('extern', 'static', 'inline', 'const',
                                                 'volatile')]
            rv = None if not toks else (toks != ['void'])
            if (name, 'ret') in tab and tab[(name, 'ret')] != rv:
                tab[(name, 'ret')] = None
            else:
                tab[(name, 'ret')] = rv
            if '...' in body or not body:
                n = None
            elif body == 'void':
                n = 0
            else:
                n = iso._count_args(body)
            if name in tab and tab[name] != n:
                tab[name] = None
            else:
                tab[name] = n
            pl = None if n is None else ([] if n == 0 else _split_top(body))
            if (name, 'params') in tab and tab[(name, 'params')] != pl:
                tab[(name, 'params')] = None
            else:
                tab[(name, 'params')] = pl
    _HDR = tab
    return tab


def funcptr_arity(text):
    """Arity of an indirect call (jalr): the parameter count of the
    function-pointer types the C file declares, when they all agree (the C
    type the call is compiled against); else None (all four $aN compared)."""
    import audit_paint_isomorphism as iso
    src = _strip_comments(text)
    ns = set()
    # `(*f)(..)`, `(*tab[N])(..)` and `(**)(..)` (a pointer to a function
    # pointer, dereferenced at the call): the function type's parameters
    for m in re.finditer(r'\(\s*\*+\s*\w*\s*(?:\[[^\]]*\])*\s*\)\s*\(([^()]*)\)', src):
        body = m.group(1).strip()
        if '...' in body or not body:
            return None
        ns.add(0 if body == 'void' else iso._count_args(body))
    return ns.pop() if len(ns) == 1 else None


def funcptr_has_aggregate(text):
    """Does any function-pointer type in the file take a parameter that may
    be an aggregate by value?"""
    src = _strip_comments(text)
    tdefs = _typedef_table(text)
    for m in re.finditer(r'\(\s*\*+\s*\w*\s*(?:\[[^\]]*\])*\s*\)\s*\(([^()]*)\)', src):
        body = m.group(1).strip()
        if body and body != 'void' and '...' not in body and \
                any(_param_kind(x, tdefs)[0] == 'agg' for x in _split_top(body)):
            return True
    return False


def funcptr_nonvoid(text):
    """Whether an indirect call (jalr) returns a value: True/False when every
    function-pointer type the C file declares agrees, else None."""
    src = _strip_comments(text)
    rs = set()
    for m in re.finditer(r'(\w+)\s*(\**)\s*\(\s*\*+\s*\w*\s*(?:\[[^\]]*\])*\s*\)\s*\([^()]*\)',
                         src):
        rs.add(not (m.group(1) == 'void' and not m.group(2)))
    return rs.pop() if len(rs) == 1 else None


def callee_nonvoid(text, names):
    """{callee: True if it returns a value, False if void} where the C file
    or the headers say so (absent when unknown)."""
    src = _strip_comments(text)
    hdr = header_prototypes()
    out = {}
    for n in names:
        rv = None
        m = re.search(r'(?:^|[;{}\n])\s*((?:extern\s+|static\s+)?[A-Za-z_][\w\s\*]*?)\b'
                      + re.escape(n) + r'\s*\(', src)
        if m:
            toks = m.group(1).replace('*', ' * ').split()
            toks = [t for t in toks if t not in ('extern', 'static', 'inline', 'const',
                                                 'volatile')]
            if toks and not any(t in ('return', 'if', 'while', 'else', 'case', 'do')
                                for t in toks):
                rv = toks != ['void']
        if rv is None:
            rv = hdr.get((n, 'ret'))
        if rv is not None:
            out[n] = rv
    return out


def file_void_callees(text, names):
    """Callees the C file ITSELF declares (or defines) `void` -- never from
    the include/ census, which may hold a prototype the file does not see
    (an implicitly declared callee is `int` to the compiler).  Every
    declaration of the name in the file must say void."""
    src = mask_c(text)
    out = set()
    for n in names:
        decls = re.findall(r'(?:^|[;{}\n])\s*((?:extern\s+|static\s+)?[A-Za-z_][\w\s\*]*?)\b'
                           + re.escape(n) + r'\s*\(', src)
        kinds = set()
        for g in decls:
            toks = g.replace('*', ' * ').split()
            toks = [x for x in toks if x not in ('extern', 'static', 'inline', 'const')]
            if not toks or any(x in ('return', 'if', 'while', 'else', 'case', 'do', 'goto')
                               for x in toks):
                continue                         # a call statement, not a declaration
            kinds.add(toks == ['void'])
        if kinds == {True}:
            out.add(n)
    return out


_SCALAR_KW = {'u8', 's8', 'u16', 's16', 'u32', 's32', 'int', 'char', 'short', 'long',
              'unsigned', 'signed', 'float', 'const', 'volatile', 'register', 'void',
              'vu8', 'vs8', 'vu16', 'vs16', 'vu32', 'vs32', 'u_char', 'u_short', 'u_int',
              'u_long', 'uint', 'ushort', 'uchar', 'size_t'}
_WIDE_KW = {'s64', 'u64', 'f64', 'double'}
_TYPEDEFS = None


def _typedef_table(text):
    """{name: definition text} of every typedef in include/ and in `text`
    (the file's own win).  Only used to recognise SCALAR typedefs; anything
    else is measured by the compiler (aggregate_arg_words)."""
    global _TYPEDEFS
    if _TYPEDEFS is None:
        tab = {}
        for h in sorted((ROOT / 'include').rglob('*.h')):
            tab.update(_typedefs_in(h.read_text(errors='replace')))
        _TYPEDEFS = tab
    out = dict(_TYPEDEFS)
    out.update(_typedefs_in(text))
    return out


def _typedefs_in(src):
    src = re.sub(r'(?m)^\s*#.*$', ' ', _strip_comments(src))
    out = {}
    for m in re.finditer(r'\btypedef\b', src):
        i, depth = m.end(), 0
        while i < len(src):
            c = src[i]
            if c == '{':
                depth += 1
            elif c == '}':
                depth -= 1
            elif c == ';' and depth == 0:
                break
            i += 1
        body = src[m.end():i]
        fp = re.search(r'\(\s*\*\s*([A-Za-z_]\w*)\s*\)\s*\(', body)
        if fp:
            out[fp.group(1)] = '<funcptr>'
            continue
        names = re.findall(r'([A-Za-z_]\w*)\s*(?:\[[^\]]*\])*\s*(?:,|$)',
                           body[body.rfind('}') + 1:] if '}' in body else body)
        for n in names[-1:]:
            out[n] = body.strip()
    return out


def _param_kind(p, tdefs, depth=0):
    """('w', words) for a scalar / pointer / array / function-pointer
    parameter, ('agg', type text) for anything that may be an aggregate."""
    p = p.strip()
    if '(' in p:
        return ('w', 1)                      # a function pointer
    if '*' in p or '[' in p:
        return ('w', 1)
    toks = p.split()
    if not toks:
        return ('agg', p)
    if toks[0] in ('struct', 'union', 'enum'):
        return ('w', 1) if toks[0] == 'enum' else ('agg', ' '.join(toks[:2]))
    typ = [x for x in toks if x not in ('const', 'volatile', 'register')]
    if len(typ) > 1 and typ[-1] not in _SCALAR_KW | _WIDE_KW and \
            (typ[-2] in _SCALAR_KW | _WIDE_KW or typ[-2] in tdefs):
        typ = typ[:-1]                       # drop the parameter name
    if all(x in _SCALAR_KW for x in typ):
        return ('w', 2) if typ.count('long') >= 2 else ('w', 1)
    if len(typ) == 1 and typ[0] in _WIDE_KW:
        return ('w', 2)
    if len(typ) == 1 and typ[0] in tdefs and depth < 8:
        d = tdefs[typ[0]]
        if d == '<funcptr>':
            return ('w', 1)
        if '{' not in d and not d.startswith(('struct', 'union')):
            return _param_kind(d.rsplit(None, 1)[0] if len(d.split()) > 1 else d, tdefs,
                               depth + 1)
    return ('agg', ' '.join(typ))


def prototype_params(text, name):
    """The parameter list of `name`'s prototype in the C file, else in the
    headers; None for K&R / variadic / unknown."""
    src = _strip_comments(text)
    decl_re = re.compile(r'(?:^|[;{}\n])\s*(?:extern\s+|static\s+)?[A-Za-z_][\w\s\*]*$')
    stmt_re = re.compile(r'\b(return|if|while|switch|else|do|case)\s*$')
    for m in re.finditer(r'\b' + re.escape(name) + r'\s*\(', src):
        k, depth = m.end(), 1
        while k < len(src) and depth:
            depth += {'(': 1, ')': -1}.get(src[k], 0)
            k += 1
        before = src[max(0, m.start() - 80):m.start()]
        if decl_re.search(before) and not stmt_re.search(before):
            body = src[m.end():k - 1].strip()
            if '...' in body or not body:
                return None
            return [] if body == 'void' else _split_top(body)
    hp = header_prototypes().get((name, 'params'))
    return hp


def _split_top(body):
    out, depth, cur = [], 0, ''
    for c in body:
        if c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        if c == ',' and depth == 0:
            out.append(cur)
            cur = ''
        else:
            cur += c
    out.append(cur)
    return [x.strip() for x in out]


def aggregate_params(text, names):
    """{callee: [param kinds]} for callees whose prototype passes something
    that may be an aggregate BY VALUE (its words are not its count)."""
    tdefs = _typedef_table(text)
    out = {}
    for n in names:
        ps = prototype_params(text, n)
        if not ps:
            continue
        kinds = [_param_kind(x, tdefs) for x in ps]
        if any(k[0] == 'agg' for k in kinds):
            out[n] = kinds
    return out


def aggregate_arg_words(text, names, cflags=()):
    """{callee: argument WORDS (O32: an aggregate takes ceil(size/4) words,
    aligned to an even word when its alignment is 8; a 64-bit scalar takes
    an aligned pair)} for every callee with a by-value aggregate parameter,
    the sizes and alignments measured by IDO itself: the file plus a probe
    function declaring one object of each type (and `struct {char c; T x;}`,
    whose size - sizeof(T) is T's alignment), compiled -g, read from .mdebug.
    A callee whose layout cannot be measured maps to None."""
    agg = aggregate_params(text, names)
    if not agg:
        return {}
    import resolve_overlay_relocs as R
    types = sorted({k[1] for ks in agg.values() for k in ks if k[0] == 'agg'})
    decl = ''.join('typedef struct { char c; %s x; } __peq_A%d;\n' % (ty, i)
                   for i, ty in enumerate(types))
    body = ' '.join('%s __peq_s%d; __peq_A%d __peq_a%d;' % (ty, i, i, i)
                    for i, ty in enumerate(types))
    td = tempfile.mkdtemp(prefix='peq_agg_')
    sizes = {}
    try:
        src = os.path.join(td, 'probe.c')
        Path(src).write_text(text + '\n' + decl + 'void __peq_probe(void) { ' + body + ' }\n')
        fl = [x for x in cflags if not x.startswith(('-O', '-g'))]
        r = subprocess.run([str(R.IDO_CC)] + R.IDO_FLAGS + fl + ['-g', '-c', 'probe.c', '-o',
                                                                 'probe.o'],
                           cwd=td, capture_output=True, env=dict(os.environ, TMPDIR=td))
        if r.returncode == 0:
            objs = MDebug(os.path.join(td, 'probe.o')).frame_objects('__peq_probe') or []
            got = {nm: sz for nm, _, sz, _ in objs}
            for i, ty in enumerate(types):
                s, a = got.get('__peq_s%d' % i), got.get('__peq_a%d' % i)
                if s and a and a > s:
                    al = a - s
                    if al in (1, 2, 4, 8) or (al > 8 and al % 4 == 0):
                        sizes[ty] = (s, 8 if al == 8 else 4)
    except Exception:                                # noqa: BLE001
        pass
    finally:
        shutil.rmtree(td, ignore_errors=True)
    out = {}
    for n, ks in agg.items():
        w = 0
        for k in ks:
            if k[0] == 'w':
                if k[1] == 2:
                    w += w & 1
                w += k[1]
                continue
            sz = sizes.get(k[1])
            if sz is None:
                w = None
                break
            if sz[1] == 8:
                w += w & 1
            w += (sz[0] + 3) // 4
        out[n] = w
    return out


def arities(text, names):
    """Callee arity: the C file's own prototype / call sites (the plain-vs-
    shipped gate's rule), else the headers' ANSI prototype, else None."""
    import audit_plain_vs_shipped as pvs
    hdr = header_prototypes()
    out = {}
    for n in names:
        try:
            a = pvs.callee_arity(text, n)
        except Exception:                            # noqa: BLE001
            a = None
        if a is None:
            a = hdr.get(n)
        out[n] = a
    return out


# ---------------------------------------------------------------------------
# Frame objects from the compiler's own debug info (IDO -g3: same code)
# ---------------------------------------------------------------------------

_BT_SIZE = {2: 1, 3: 1, 4: 2, 5: 2, 6: 4, 7: 4, 8: 4, 9: 4, 10: 4, 11: 8, 14: 4}


class MDebug:
    """Minimal reader for IDO's .mdebug (big-endian ECOFF symbolic data):
    local / parameter symbols of a procedure with their frame offsets
    (relative to the virtual frame pointer = the entry $sp) and sizes."""

    def __init__(self, obj):
        from elftools.elf.elffile import ELFFile
        data = open(obj, 'rb').read()
        e = ELFFile(open(obj, 'rb'))
        sec = e.get_section_by_name('.mdebug')
        if sec is None:
            raise ValueError('no .mdebug')
        off = sec['sh_offset']
        h = struct.unpack_from('>hh23i', data, off)
        if h[0] != 0x7009:
            raise ValueError('bad .mdebug magic')
        names = ('magic vstamp ilineMax cbLine cbLineOffset idnMax cbDnOffset ipdMax '
                 'cbPdOffset isymMax cbSymOffset ioptMax cbOptOffset iauxMax cbAuxOffset '
                 'issMax cbSsOffset issExtMax cbSsExtOffset ifdMax cbFdOffset crfd '
                 'cbRfdOffset iextMax cbExtOffset').split()
        self.H = dict(zip(names, h))
        self.data = data
        self.fds = [struct.unpack_from('>11i2h7i', data, self.H['cbFdOffset'] + 72 * i)
                    for i in range(self.H['ifdMax'])]

    def sym(self, fd, i):
        F = self.fds[fd]
        iss, val, bits = struct.unpack_from('>iiI', self.data,
                                            self.H['cbSymOffset'] + 12 * (F[4] + i))
        name = self.data[self.H['cbSsOffset'] + F[2] + iss:].split(b'\0')[0].decode(
            'latin-1')
        return name, val, bits >> 26, (bits >> 21) & 31, bits & 0xFFFFF

    def aux(self, fd, i):
        return struct.unpack_from('>I', self.data,
                                  self.H['cbAuxOffset'] + 4 * (self.fds[fd][12] + i))[0]

    def _rndx(self, fd, i):
        w = self.aux(fd, i)
        rfd, idx = w >> 20, w & 0xFFFFF
        if rfd == 0xFFF:
            rfd = self.aux(fd, i + 1)
            return rfd, idx, 2
        return rfd, idx, 1

    def _fd_of(self, fd, rfd):
        if self.H['crfd'] == 0 or self.fds[fd][16] == 0:
            return rfd
        return struct.unpack_from('>i', self.data, self.H['cbRfdOffset'] +
                                  4 * (self.fds[fd][15] + rfd))[0]

    def type_size(self, fd, ai, depth=0):
        if depth > 8:
            return None
        tir = self.aux(fd, ai)
        if tir & 0x40000000:               # continued TIR: not handled
            return None
        bt = (tir >> 24) & 0x3F
        tqs = [(tir >> 12) & 15, (tir >> 8) & 15, (tir >> 4) & 15, tir & 15,
               (tir >> 20) & 15, (tir >> 16) & 15]
        k = ai + 1
        ref = None
        if bt in (12, 13, 14, 15):
            rfd, idx, used = self._rndx(fd, k)
            ref = (self._fd_of(fd, rfd), idx)
            k += used
        # type qualifiers run from the base type outward (tq0 is applied
        # first: `Poly *obj[4]` is tq0 = pointer, tq1 = array), so the
        # OUTERMOST one decides the object's size.  Each array qualifier owns
        # an aux group (index-type RNDX, low, high, element width in bits),
        # in qualifier order.
        qs = [q for q in tqs if q != 0]
        if any(q not in (1, 3) for q in qs):
            return None
        last_arr = None
        for q in qs:
            if q == 3:                     # array: index type, low, high, width
                rfd, idx, used = self._rndx(fd, k)
                k += used
                lo, hi, width = (struct.unpack('>i', struct.pack('>I', self.aux(fd, k + j)))[0]
                                 for j in range(3))
                k += 3
                last_arr = (lo, hi, width)
        if qs and qs[-1] == 1:             # outermost is a pointer
            return 4
        if qs and qs[-1] == 3:
            lo, hi, width = last_arr
            if width <= 0 or hi < lo:
                return None
            return (hi - lo + 1) * width // 8
        if bt in _BT_SIZE:
            return _BT_SIZE[bt]
        if ref is None or ref[1] == 0xFFFFF:
            return None
        name, val, st, sc, idx2 = self.sym(*ref)
        if bt in (12, 13) and st in (7, 26, 27):   # stBlock/stStruct/stUnion: size
            return val
        if bt == 15 and idx2 != 0xFFFFF:   # typedef: its own type
            return self.type_size(ref[0], idx2, depth + 1)
        return None

    def is_aggregate(self, fd, ai, depth=0):
        """True for an array / struct / union type, False for a scalar or
        pointer, None when the type record is not understood."""
        if depth > 8:
            return None
        tir = self.aux(fd, ai)
        if tir & 0x40000000:
            return None
        bt = (tir >> 24) & 0x3F
        tqs = [(tir >> 12) & 15, (tir >> 8) & 15, (tir >> 4) & 15, tir & 15,
               (tir >> 20) & 15, (tir >> 16) & 15]
        qs = [q for q in tqs if q != 0]
        if qs:
            return {1: False, 3: True}.get(qs[-1])
        if bt in (12, 13):
            return True
        if bt in _BT_SIZE:
            return False
        if bt == 15:
            rfd, idx, used = self._rndx(fd, ai + 1)
            ref = (self._fd_of(fd, rfd), idx)
            if idx == 0xFFFFF:
                return None
            name, val, st, sc, idx2 = self.sym(*ref)
            if idx2 == 0xFFFFF:
                return None
            return self.is_aggregate(ref[0], idx2, depth + 1)
        return None

    def frame_objects(self, fn):
        """[(name, offset from the entry $sp, size or None, is_param)] and
        the save-area offset of procedure `fn`."""
        for fd, F in enumerate(self.fds):
            n = F[5]
            i = 0
            while i < n:
                name, val, st, sc, idx = self.sym(fd, i)
                if st in (6, 14) and name == fn:     # stProc / stStaticProc
                    out = []
                    j = i + 1
                    depth = 0
                    while j < n:
                        nm, v, st2, sc2, ix = self.sym(fd, j)
                        if st2 in (7, 26, 27, 28):   # stBlock/stStruct/stUnion/stEnum
                            depth += 1
                        elif st2 == 8:
                            if depth == 0:
                                break
                            depth -= 1
                        elif st2 in (3, 4) and sc2 == 5:     # stParam/stLocal, scAbs
                            sz = self.type_size(fd, ix) if ix != 0xFFFFF else None
                            out.append((nm, v, sz, st2 == 3))
                        elif st2 == 4:
                            # a local the debug info gives no frame address
                            # (IDO -O2 -g3 records many as scRegister, even
                            # arrays): (name, None, size, kind)
                            sz = self.type_size(fd, ix) if ix != 0xFFFFF else None
                            ag = self.is_aggregate(fd, ix) if ix != 0xFFFFF else None
                            out.append((nm, None, sz,
                                        'agg' if ag else 'scalar' if ag is False else '?'))
                        j += 1
                    return out
                i += 1
        return None


# ---------------------------------------------------------------------------
# Building P and S
# ---------------------------------------------------------------------------

def _elf_text_and_sym(obj, fn=None):
    from elftools.elf.elffile import ELFFile
    with open(obj, 'rb') as f:
        e = ELFFile(f)
        d = e.get_section_by_name('.text').data()
        words = [struct.unpack_from('<I', d, i)[0] for i in range(0, len(d) // 4 * 4, 4)]
        st = None
        if fn is not None:
            for s in e.get_section_by_name('.symtab').iter_symbols():
                if s.name == fn and s['st_info']['type'] == 'STT_FUNC':
                    st = (s['st_value'] // 4, s['st_size'] // 4)
    return words, st


def _rodata_sig(obj):
    """Bytes + relocations of every data section (jump tables live there)."""
    from elftools.elf.elffile import ELFFile
    from elftools.elf.relocation import RelocationSection
    sig = []
    with open(obj, 'rb') as f:
        e = ELFFile(f)
        symtab = e.get_section_by_name('.symtab')
        for s in e.iter_sections():
            if s.name.startswith(('.rodata', '.rdata', '.data', '.sdata')) and \
                    s['sh_type'] != 'SHT_NOBITS':
                sig.append((s.name, s.data()))
            if isinstance(s, RelocationSection):
                tgt = e.get_section(s['sh_info']).name
                if tgt.startswith(('.rodata', '.rdata', '.data', '.sdata')):
                    for r in s.iter_relocations():
                        sym = symtab.get_symbol(r['r_info_sym'])
                        sig.append((tgt, r['r_offset'], r['r_info_type'], sym.name,
                                    sym['st_shndx'], sym['st_value']))
    return sig


def text_relocs_paired(obj):
    """{word index: (type, symbol[, paired LO16 addend])} for .text.  A
    HI16's value depends on the LO16 GNU ld pairs it with (the next LO16
    against the same symbol in relocation order) through the carry, so the
    pair's low addend is recorded with the HI16."""
    from elftools.elf.elffile import ELFFile
    from elftools.elf.relocation import RelocationSection
    ents = []
    with open(obj, 'rb') as f:
        e = ELFFile(f)
        symtab = e.get_section_by_name('.symtab')
        text = e.get_section_by_name('.text').data()
        for sec in e.iter_sections():
            if not isinstance(sec, RelocationSection):
                continue
            if e.get_section(sec['sh_info']).name != '.text':
                continue
            for r in sec.iter_relocations():
                sym = symtab.get_symbol(r['r_info_sym'])
                name = sym.name
                if not name and sym['st_shndx'] not in ('SHN_UNDEF', 'SHN_ABS'):
                    name = e.get_section(sym['st_shndx']).name
                ents.append((r['r_offset'], r['r_info_type'], name))
    out = {}
    for q, (off, typ, name) in enumerate(ents):
        if typ == R_MIPS_HI16:
            lo = None
            for off2, typ2, name2 in ents[q + 1:]:
                if typ2 == R_MIPS_LO16 and name2 == name:
                    lo = _sx16(struct.unpack_from('<I', text, off2)[0])
                    break
            out[off // 4] = (typ, name, lo)
        else:
            out[off // 4] = (typ, name)
    return out


def _facts(obj, fn, limit, jt_override=None):
    import audit_paint_isomorphism as iso
    words, st = _elf_text_and_sym(obj, fn)
    base = 0
    if fn is not None:
        if st is None or st[1] == 0:
            return None
        base, n = st
        if limit is not None:
            n = min(n, limit)
        words = words[base:base + n]
    rel = {k - base: v for k, v in text_relocs_paired(obj).items()
           if base <= k < base + len(words)}
    if jt_override is not None:
        jt = jt_override
    else:
        jt = [x - base for x in (iso.jump_targets(obj) or []) if 0 <= x - base < len(words)]
    return dict(words=words, rel=rel, jt=jt, base=base)


def _escapes_words(words):
    f = Func('P', words, {}, [])
    return bool(frame_escapes(f))


def _debug_objects(gobj, fn, pwords, mode):
    """Frame objects of `fn` from a -g3 compile, or a string saying why
    not.  The -g3 object's frame layout must be P's (see spw)."""
    if gobj is None or not os.path.exists(str(gobj)):
        return 'g3 compile failed'
    try:
        if mode == 'whole':
            gw, _ = _elf_text_and_sym(gobj, None)
        else:
            gf = _facts(gobj, fn, None)
            gw = gf['words'] if gf else None
        if gw is None:
            return 'g3 code differs from P'
        # -g3 can reschedule and reallocate; the frame LAYOUT (what the
        # debug info describes) is the same when every $sp-relative word has
        # the same (operation, offset): the frame adjust, each slot access
        # and each address formation (as sets: -g3 may repeat a reload)
        def spw(ws):
            out = []
            for w in ws:
                op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
                if rs == SP and op in (9, 32, 33, 34, 35, 36, 37, 38, 40, 41, 42, 43, 46,
                                       50, 58):
                    out.append((op, w & 0xFFFF, rt == SP))
                elif op == 0 and SP in (rs, rt) and (w & 63) in (0x21, 0x25, 0x23):
                    out.append(('r', w & 63))
            return sorted(set(out), key=repr)
        if spw(gw) != spw(pwords):
            return 'g3 frame layout differs from P'
        objs = MDebug(str(gobj)).frame_objects(fn)
        if objs is None:
            return 'g3: procedure not in .mdebug'
        return objs
    except Exception as ex:                          # noqa: BLE001
        return 'g3: ' + repr(ex)[:100]


def collect_overlay(ov, fn, keep_dir=None):
    import ido_flags
    import resolve_overlay_relocs as R
    import audit_raw_count as A
    from parse_postproc import parse_compiler
    from parse_postproc import parse_symbol_start_text, parse_symbol_size_text
    from overlay_postproc import parse_postproc
    src = ROOT / 'src/overlay' / ov / f'{fn}.c'
    content = src.read_text(errors='replace')
    sym_a = sym_s = None
    for ln in (ROOT / 'config/overlay' / f'{ov}.sym').read_text().splitlines():
        p = ln.strip().split(':')
        if len(p) >= 3 and p[2].strip() == fn:
            sym_a, sym_s = int(p[0], 16), int(p[1], 0)
    addr = parse_symbol_start_text(content) or sym_a
    eff = parse_symbol_size_text(content) or sym_s
    with contextlib.redirect_stdout(io.StringIO()):
        steps = parse_postproc(src)
    td = Path(tempfile.mkdtemp(prefix='peq_'))
    try:
        fin = R.compile_object(ov, fn, src, addr, td)
        if fin is None:
            return dict(err='final compile/chain failed')
        rd = td / 'raw'
        rd.mkdir()
        s2 = rd / f'{fn}.c'
        s2.write_text(A.strip_postproc(content))
        extra = []
        if not parse_compiler(src).startswith('gcc'):
            # the PIC mode of the SHIPPED compile (single source of truth:
            # ido_flags.stripped_pic_extra)
            extra = ido_flags.stripped_pic_extra(content, R.parse_compile_flags(src))
        raw = R.compile_object(ov, fn, s2, addr, rd, extra_cflags=extra)
        if raw is None:
            return dict(err='raw compile failed')
        fp = _facts(raw, fn, None)
        fs = _facts(fin, fn, eff // 4 if eff else None)
        if fp is None or fs is None:
            return dict(err='symbol missing')
        rod = _rodata_sig(raw) == _rodata_sig(fin)
        callees = sorted({v[1] for f_ in (fp, fs) for v in f_['rel'].values()
                          if v[0] == R_MIPS_26})
        argwords = aggregate_arg_words(content, callees, R.parse_compile_flags(src))
        pnv = None
        if needs_noval(content, fn):
            # C' (value-less exits marked): the meaning the return-value
            # obligation is checked against, path by path
            ic = instrument_valueless(A.strip_postproc(content), fn)
            if ic is not None:
                nd = td / 'nv'
                nd.mkdir()
                s4 = nd / f'{fn}.c'
                s4.write_text(ic)
                nobj = R.compile_object(ov, fn, s4, addr, nd, extra_cflags=extra)
                if nobj is not None:
                    pnv = _facts(nobj, fn, None)
                    if pnv is not None and _rodata_sig(nobj) != _rodata_sig(raw):
                        pnv = None
        psent = None
        if needs_noval(content, fn):
            isn = instrument_sentinels(A.strip_postproc(content), fn,
                                       [w & 0xFFFF for w in fp['words']])
            if isn is not None:
                sd = td / 'sent'
                sd.mkdir()
                s5 = sd / f'{fn}.c'
                s5.write_text(isn[0])
                sobj = R.compile_object(ov, fn, s5, addr, sd, extra_cflags=extra)
                if sobj is not None and _rodata_sig(sobj) == _rodata_sig(raw):
                    psent = _facts(sobj, fn, None)
                    if psent is not None:
                        psent['smap'] = {str(k): v for k, v in isn[1].items()}
        objects = None
        if not parse_compiler(src).startswith('gcc') and _escapes_words(fp['words']):
            gd = td / 'g3'
            gd.mkdir()
            s3 = gd / f'{fn}.c'
            s3.write_text(A.strip_postproc(content))
            gobj = R.compile_object(ov, fn, s3, addr, gd, extra_cflags=extra + ['-g3'])
            objects = _debug_objects(gobj, fn, fp['words'], None)
        if keep_dir:
            os.makedirs(keep_dir, exist_ok=True)
            shutil.copy(raw, os.path.join(keep_dir, 'raw.o'))
            shutil.copy(fin, os.path.join(keep_dir, 'fin.o'))
    finally:
        shutil.rmtree(td, ignore_errors=True)
    return dict(P=fp, S=fs, rodata_equal=rod, text=content, name=fn,
                steps=[x.get('type') for x in steps], objects=objects, addr=addr, home=ov,
                Pnv=pnv, Psent=psent, argwords=argwords)


def trim_zero_tail(words, rel, n):
    """P past an n-word window: drop an all-zero tail (alignment pad), but
    a transfer in the last window word keeps its delay slot -- the compiled
    nop after it is part of P (S may have filled the slot by a reorder)."""
    if len(words) > n and not any(words[n:]):
        m = n + 1 if is_xfer(decode(n - 1, words[n - 1])) else n
        return words[:m], {k: v for k, v in rel.items() if k < m}
    return words, rel


def collect_main(e, keep_dir=None):
    import audit_plain_vs_shipped as pvs
    import audit_paint_isomorphism as iso
    from parse_postproc import (parse_postproc_header, parse_compile_flags,
                                parse_compiler, parse_compile_env)
    src = str(ROOT / e['source'])
    with contextlib.redirect_stdout(io.StringIO()):
        pp = parse_postproc_header(src) or []
    parsed = (parse_compile_flags(src), parse_compiler(src), parse_compile_env(src))
    text = Path(src).read_text(errors='replace')
    td = tempfile.mkdtemp(prefix='peqm_')
    try:
        fo, ro, fo2 = (os.path.join(td, x) for x in ('final.o', 'raw.o', 'full.o'))
        pvs._main_compile(e, pp, parsed, keep=fo)
        strip = [s for s in pp if s.get('type') == 'strip_leading_instructions']
        pvs._main_compile(e, strip, parsed, keep=ro)
        full = pvs._main_compile(e, [], parsed, keep=fo2)
        fp = _facts(ro, None, None)
        fs = _facts(fo, None, None)
        shift = len(full) - len(fp['words'])
        F = 0
        from elftools.elf.elffile import ELFFile
        with open(fo2, 'rb') as f_:
            for sy in ELFFile(f_).get_section_by_name('.symtab').iter_symbols():
                if sy.name == e['name'] and sy['st_info']['type'] == 'STT_FUNC':
                    F = sy['st_value'] // 4
        rjt = None
        if iso.jump_targets(fo2):
            # the .rodata entries are .text offsets of the object BEFORE the
            # strip; reuse the audit's inference (same list for both sides)
            rw = fp['words']
            rjt = sorted(set(x - shift for x in (iso.jump_targets(fo2) or [])
                             if 0 <= x - shift < len(rw)))

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
        n = e['size'] // 4
        fs['words'] = fs['words'][:n]
        fs['rel'] = {k: v for k, v in fs['rel'].items() if k < n}
        fp['words'], fp['rel'] = trim_zero_tail(fp['words'], fp['rel'], n)
        fp['jt'] = [x for x in (rjt or []) if x < len(fp['words'])]
        fs['jt'] = [x for x in (rjt or []) if x < len(fs['words'])]
        fp['base'] = fs['base'] = shift
        rod = _rodata_sig(ro) == _rodata_sig(fo)
        callees = sorted({v[1] for f_ in (fp, fs) for v in f_['rel'].values()
                          if v[0] == R_MIPS_26})
        argwords = aggregate_arg_words(text, callees, list(parsed[0] or []))
        pnv = None
        if needs_noval(text, e['name']) and not rjt:
            ic = instrument_valueless(text, e['name'])
            if ic is not None:
                nsrc = os.path.join(td, 'nv_' + os.path.basename(src))
                Path(nsrc).write_text(ic)
                no = os.path.join(td, 'nv.o')
                try:
                    pvs._main_compile(dict(e, source=nsrc), strip, parsed, keep=no)
                    pnv = _facts(no, None, None)
                except Exception:                    # noqa: BLE001
                    pnv = None
                if pnv is not None:
                    w_ = pnv['words']
                    jr_ = [q for q, x in enumerate(w_) if x == 0x03E00008]
                    if jr_ and not any(w_[jr_[-1] + 2:]):
                        m_ = jr_[-1] + 2
                        pnv['words'] = w_[:m_]
                        pnv['rel'] = {k: v for k, v in pnv['rel'].items() if k < m_}
                    pnv['jt'] = []
                    pnv['base'] = shift
                    if _rodata_sig(no) != _rodata_sig(ro):
                        pnv = None
        psent = None
        if needs_noval(text, e['name']) and not rjt:
            isn = instrument_sentinels(text, e['name'], [w & 0xFFFF for w in fp['words']])
            if isn is not None:
                ssrc = os.path.join(td, 'sent_' + os.path.basename(src))
                Path(ssrc).write_text(isn[0])
                so = os.path.join(td, 'sent.o')
                try:
                    pvs._main_compile(dict(e, source=ssrc), strip, parsed, keep=so)
                    psent = _facts(so, None, None)
                except Exception:                    # noqa: BLE001
                    psent = None
                if psent is not None and _rodata_sig(so) == _rodata_sig(ro):
                    w_ = psent['words']
                    jr_ = [q for q, x in enumerate(w_) if x == 0x03E00008]
                    if jr_ and not any(w_[jr_[-1] + 2:]):
                        m_ = jr_[-1] + 2
                        psent['words'] = w_[:m_]
                        psent['rel'] = {k: v for k, v in psent['rel'].items() if k < m_}
                    psent['jt'] = []
                    psent['base'] = shift
                    psent['smap'] = {str(k): v for k, v in isn[1].items()}
                else:
                    psent = None
        objects = None
        if not (parsed[1] or '').startswith('gcc') and _escapes_words(fp['words']):
            go = os.path.join(td, 'g3.o')
            pvs._main_compile(e, strip, (list(parsed[0] or []) + ['-g3'], parsed[1], parsed[2]),
                              keep=go)
            objects = _debug_objects(go, e['name'], fp['words'], 'whole')
        if keep_dir:
            os.makedirs(keep_dir, exist_ok=True)
            shutil.copy(ro, os.path.join(keep_dir, 'raw.o'))
            shutil.copy(fo, os.path.join(keep_dir, 'fin.o'))
    finally:
        shutil.rmtree(td, ignore_errors=True)
    return dict(P=fp, S=fs, rodata_equal=rod, text=text, name=e['name'],
                steps=[x.get('type') for x in pp], objects=objects,
                addr=int(e['address'], 16), home='main', Pnv=pnv, Psent=psent,
                argwords=argwords)


def prove_record(rec):
    """rec from collect_*: run the prover."""
    P = Func('P', rec['P']['words'], {int(k): tuple(v) for k, v in rec['P']['rel'].items()},
             rec['P']['jt'], rec['P']['base'])
    S = Func('S', rec['S']['words'], {int(k): tuple(v) for k, v in rec['S']['rel'].items()},
             rec['S']['jt'], rec['S']['base'])
    text = rec['text']
    names = {v[1] for v in P.rel.values() if v[0] == R_MIPS_26} | \
        {v[1] for v in S.rel.values() if v[0] == R_MIPS_26}
    ar = arities(text, sorted(names))
    ar['<jalr>'] = funcptr_arity(text)
    # a by-value aggregate parameter: the ARGUMENT WORDS (measured by IDO at
    # collection) replace the parameter count
    for n_, w_ in (rec.get('argwords') or {}).items():
        ar[n_] = w_ if w_ is not None else 'agg?'
    if funcptr_has_aggregate(text):
        ar['<jalr>'] = 'agg?'
    vol = volatile_names(text)
    void, r64 = return_kind(text, rec['name'])
    identical = P.words == S.words and P.rel == S.rel
    nv = callee_nonvoid(text, sorted(names))
    if funcptr_nonvoid(text):
        nv['<jalr>'] = True        # (never False: indirect targets are not trusted void)
    rs = None if void else return_sites(text, rec['name'])
    kw = dict(arity=ar, volatile=vol, void=void, ret64=r64,
              rodata_equal=rec['rodata_equal'], objects=rec.get('objects'), nonvoid=nv,
              home=rec.get('home', 'main'), valueless=bool(rs and rs[0] > 0))
    kw['voidcallees'] = file_void_callees(text, sorted(names))
    kw['addrtaken'] = set(re.findall(r'&\s*\(?\s*([A-Za-z_]\w*)', mask_c(text)))
    v, cls, msg, mode = prove_pair(P, S, **kw)
    if v == 'PROVEN' and 'valueless-path' in (msg or '').split(','):
        msg = msg + ',indeterminate-return'
    if v != 'PROVEN' and cls == 'return' and '$v0' in msg and not void:
        # INDETERMINATE RETURN, path-precise.  C defines the return value
        # only on paths that end in `return <expr>;`; on a value-less path
        # (fall-off, bare `return;`) it is indeterminate.
        if rs is not None and rs[0] > 0 and rs[1] == 0:
            # no valued return anywhere: every path is value-less
            kw2 = dict(kw)
            kw2['void'] = True
            v2, cls2, msg2, mode2 = prove_pair(Func('P', P.words, P.rel, P.jt, P.base),
                                               Func('S', S.words, S.rel, S.jt, S.base), **kw2)
            if v2 == 'PROVEN':
                v, cls, msg, mode = v2, None, ','.join(
                    x for x in (msg2, 'indeterminate-return') if x), mode2
        elif rec.get('Pnv'):
            # mixed: prove S against C' (each value-less exit returns the
            # address of a symbol nothing else can form), and exempt $v0 at a
            # return exactly when P's $v0 there is built from that symbol --
            # i.e. on the paths that leave through a value-less exit.  Every
            # other obligation is checked against C' as usual.
            q = rec['Pnv']
            Pn = Func('P', q['words'], {int(k): tuple(x) for k, x in q['rel'].items()},
                      q['jt'], q['base'])
            kw2 = dict(kw)
            kw2['noval'] = NOVAL_SYM
            v2, cls2, msg2, mode2 = prove_pair(Pn, Func('S', S.words, S.rel, S.jt, S.base),
                                               **kw2)
            if v2 == 'PROVEN':
                v, cls, msg, mode = v2, None, ','.join(
                    x for x in (msg2, 'indeterminate-return') if x), mode2
        vk = valued_return_kinds(text, rec['name']) if rs and rs[0] > 0 else None
        if v != 'PROVEN' and vk is not None and (rec.get('Psent') or not vk[0]):
            # VALUE FLOW: every `return <expr>;` is a literal or a direct
            # call.  Against C'' (each literal replaced by a sentinel) a
            # `return K;` path ends with $v0 == its sentinel, and a `return
            # f();` path with $v0 flowing from a call result; a path whose
            # $v0 is neither (and flows from no unknown value) is value-less.
            q = rec.get('Psent')
            if q:
                Pq = Func('P', q['words'], {int(k): tuple(x) for k, x in q['rel'].items()},
                          q['jt'], q['base'])
                smap = {int(k): x for k, x in q['smap'].items()}
            else:
                Pq, smap = Func('P', P.words, P.rel, P.jt, P.base), {}
            kw2 = dict(kw)
            kw2['flow'] = (smap, vk[1])
            v2, cls2, msg2, mode2 = prove_pair(Pq, Func('S', S.words, S.rel, S.jt, S.base),
                                               **kw2)
            if v2 == 'PROVEN':
                v, cls, msg, mode = v2, None, ','.join(
                    x for x in (msg2, 'indeterminate-return') if x), mode2
    if v != 'PROVEN' and cls == 'return' and '$v0' in msg and rec.get('addr'):
        # $v0 differs at a return: unobservable if no caller ever reads it
        # (and the address is never taken).  Re-prove without the $v0 check.
        if v0_unused_by_callers(rec['addr'], rec.get('home', 'main')):
            P2 = Func('P', P.words, P.rel, P.jt, P.base)
            S2 = Func('S', S.words, S.rel, S.jt, S.base)
            kw['void'] = True
            v2, cls2, msg2, mode2 = prove_pair(P2, S2, **kw)
            if v2 == 'PROVEN':
                v, cls, msg, mode = v2, None, ','.join(
                    x for x in (msg2, 'v0-unread-by-callers') if x), mode2
    prover = v
    if v != 'PROVEN' and identical and rec['rodata_equal']:
        # P and S are the same object (words, relocations, data): equal by
        # definition.  The prover's own verdict is kept as a sanity measure.
        v, cls, msg = 'PROVEN', None, 'identical (prover: %s %s)' % (v, cls)
    return dict(verdict=v, cls=cls, msg=msg, identical=identical, mode=mode, prover=prover,
                nP=len(P.words), nS=len(S.words))


# ---------------------------------------------------------------------------
# Enumeration, worker, CLI
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


def task_id(kind, payload):
    if kind == 'ov':
        return '%s/%s' % payload
    return 'main/%s' % payload['name']


CACHE = None
_TOOLCHAIN = None

# Everything a cached P/S record is a function of, beyond the function's own
# inputs: the prover and every module the collection imports, the PostProc
# tools, the compilers and assemblers, and the headers.  Hashed by CONTENT once
# per process (the parent, before the worker pool forks).
_TOOL_TREES = ('tools/analysis', 'tools/overlay', 'tools/build', 'tools/postproc',
               'tools/decomp', 'tools/maspsx', 'include')
_TOOL_BINS = ('tools/ido-static-recomp/build/5.2/out', 'tools/psyq-gcc-260',
              'tools/psyq-gcc-272', 'tools/psyq-gcc')


def toolchain_digest():
    """sha1 over the content of every tool/compiler/header file that can
    change a collected record (the tool-version part of the cache key)."""
    global _TOOLCHAIN
    if _TOOLCHAIN is None:
        import hashlib
        h = hashlib.sha1(b'peq-cache-v2')
        for tree in _TOOL_TREES + _TOOL_BINS:
            base = ROOT / tree
            if not base.exists():
                continue
            for f in sorted(base.rglob('*')):
                if not f.is_file() or '__pycache__' in f.parts:
                    continue
                if tree in _TOOL_TREES and f.suffix not in (
                        '.py', '.h', '.inc', '.json', '.txt') and tree != 'include':
                    continue
                h.update(str(f.relative_to(ROOT)).encode() + b'\0')
                h.update(f.read_bytes())
                h.update(b'\0')
        _TOOLCHAIN = h.hexdigest()
    return _TOOLCHAIN


def cache_key(kind, payload):
    """Content hash of one record's inputs: the C source (which carries the
    @CompileFlags/@CompileEnv/@PostProc directives), the function's window
    (.sym map / registry entry), the compile-relevant environment, and the
    toolchain digest.  Any edit to any of these is a different key, so a
    cached record can never be stale."""
    import hashlib
    h = hashlib.sha1(toolchain_digest().encode())
    if kind == 'ov':
        ov, fn = payload[:2]
        h.update(('ov:%s/%s\0' % (ov, fn)).encode())
        for f in (ROOT / 'src/overlay' / ov / f'{fn}.c', ROOT / 'config/overlay' / f'{ov}.sym'):
            h.update(f.read_bytes() if f.exists() else b'<missing>')
            h.update(b'\0')
    else:
        h.update(('main:%s\0' % json.dumps(payload, sort_keys=True)).encode())
        f = ROOT / payload['source']
        h.update(f.read_bytes() if f.exists() else b'<missing>')
        for f in (ROOT / 'config/main_symbol_boundaries.json',):
            h.update(f.read_bytes() if f.exists() else b'<missing>')
    env = sorted((k, v) for k, v in os.environ.items()
                 if k.startswith(('IDO52_', 'L3_', 'MASPSX')))
    h.update(json.dumps(env).encode())
    return h.hexdigest()


def _collect(kind, payload):
    tid = task_id(kind, payload)
    cf = None
    if CACHE:
        cf = Path(CACHE) / (tid.replace('/', '__') + '.' + cache_key(kind, payload)[:20] + '.json')
        if cf.exists():
            return json.loads(cf.read_text())
    for attempt in range(2):
        try:
            with contextlib.redirect_stderr(io.StringIO()), \
                    contextlib.redirect_stdout(io.StringIO()):
                rec = collect_overlay(*payload) if kind == 'ov' else collect_main(payload)
        except BaseException as ex:                  # noqa: BLE001
            rec = dict(err=repr(ex)[:300])
        if 'err' not in rec:
            break
    if cf and 'err' not in rec:
        rec2 = dict(rec)
        for side in ('P', 'S'):
            rec2[side] = dict(rec[side])
            rec2[side]['rel'] = {str(k): list(v) for k, v in rec[side]['rel'].items()}
        cf.parent.mkdir(parents=True, exist_ok=True)
        cf.write_text(json.dumps(rec2))
    return rec


def work(task):
    kind, payload = task
    _CUR_OV[0] = payload[0] if kind == 'ov' else None
    try:
        return _work(task)
    finally:
        _CUR_OV[0] = None


def _work(task):
    kind, payload = task
    tid = task_id(kind, payload)
    rec = _collect(kind, payload)
    out = dict(id=tid, region='overlay' if kind == 'ov' else 'main')
    if 'err' in rec:
        out.update(verdict='UNMEASURABLE', cls='collect', msg=rec['err'], steps=[])
        return out
    try:
        r = prove_record(rec)
    except Exception as ex:                          # noqa: BLE001
        import traceback
        r = dict(verdict='UNPROVEN', cls='prover-crash',
                 msg=repr(ex)[:200] + ' ' + traceback.format_exc().splitlines()[-3][:200])
    out.update(r)
    out['steps'] = rec.get('steps') or []
    out['postproc'] = bool([s for s in out['steps'] if s != 'strip_leading_instructions'])
    return out


def run(tasks, jobs):
    if jobs <= 1 or len(tasks) <= 1:
        return [work(t) for t in tasks]
    import multiprocessing as mp
    with mp.get_context('fork').Pool(jobs) as pool:
        return list(pool.imap_unordered(work, tasks, chunksize=2))


def main():
    global CACHE
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0],
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--only', action='append', default=[])
    ap.add_argument('--overlay', action='append', default=[])
    ap.add_argument('--main-only', action='store_true')
    ap.add_argument('--overlays-only', action='store_true')
    ap.add_argument('--postproc-only', action='store_true',
                    help='report only functions whose chain has a PostProc step')
    ap.add_argument('--jobs', type=int, default=min(16, os.cpu_count() or 1))
    ap.add_argument('--cache', help='directory to keep/reuse compiled P/S facts (keyed by a '
                    'content hash of source, window, environment and toolchain)')
    ap.add_argument('--json', help='write per-function results here')
    ap.add_argument('-v', '--verbose', action='store_true')
    ap.add_argument('--self-test', action='store_true')
    a = ap.parse_args()
    os.chdir(ROOT)
    sys.setrecursionlimit(20000)
    CACHE = a.cache
    if CACHE:
        toolchain_digest()                 # once, before the pool forks
    if a.self_test:
        ok = self_test()
        print('SELF-TEST ' + ('PASSED' if ok else 'FAILED'))
        return 0 if ok else 2
    tasks = []
    mains = main_entries()
    if a.only:
        byname = {e['name']: e for e in mains}
        byaddr = {int(e['address'], 16): e for e in mains}
        ovl = overlay_entries()
        for q in a.only:
            if q.startswith('main/'):
                q = q[5:]
            if '/' in q:
                import overlay_names as _ON
                ov_, fn_ = q.split('/', 1)
                tasks.append(('ov', (ov_, _ON.current_name(ov_, fn_))))   # follow a landed rename
                continue
            hit = False
            if q.lower().startswith('0x') and int(q, 16) in byaddr:
                tasks.append(('main', byaddr[int(q, 16)]))
                hit = True
            if q in byname:
                tasks.append(('main', byname[q]))
                hit = True
            import overlay_names as _ON
            for ov, fn in ovl:
                if fn == q or _ON.current_name(ov, q) == fn:
                    tasks.append(('ov', (ov, fn)))
                    hit = True
            if not hit:
                print('no such function: ' + q, file=sys.stderr)
                return 2
    else:
        if not a.overlays_only and not a.overlay:
            tasks += [('main', e) for e in mains]
        if not a.main_only:
            tasks += [('ov', x) for x in overlay_entries(set(a.overlay) or None)]
    results = run(tasks, a.jobs)
    results.sort(key=lambda r: r['id'])
    cnt = collections.Counter()
    for r in results:
        grp = 'postproc' if r.get('postproc') else 'plain'
        cnt[(r['region'], grp, r['verdict'])] += 1
        if r['verdict'] != 'PROVEN' or a.verbose:
            if a.postproc_only and not r.get('postproc') and r['verdict'] == 'PROVEN':
                continue
            print('%-9s %-40s %-16s %s' % (r['verdict'], r['id'], r.get('cls') or '',
                                          r.get('msg', '')))
    print()
    for region in ('overlay', 'main'):
        for grp in ('postproc', 'plain'):
            tot = sum(v for k, v in cnt.items() if k[:2] == (region, grp))
            if not tot:
                continue
            print('%-8s %-9s %5d: PROVEN %d  UNPROVEN %d  REFUTED %d  UNMEASURABLE %d' % (
                region, grp, tot, cnt[(region, grp, 'PROVEN')], cnt[(region, grp, 'UNPROVEN')],
                cnt[(region, grp, 'REFUTED')], cnt[(region, grp, 'UNMEASURABLE')]))
    plain = [r for r in results if not r.get('postproc') and r['verdict'] != 'UNMEASURABLE']
    print('zero-PostProc sanity: %d functions, %d with P == S word for word; the prover '
          'alone (without the identity shortcut) proves %d' % (
              len(plain), sum(1 for r in plain if r.get('identical')),
              sum(1 for r in plain if r.get('prover') == 'PROVEN')))
    notes = collections.Counter(n for r in results if r['verdict'] == 'PROVEN'
                                for n in (r.get('msg') or '').split(',') if n and '(' not in n)
    print('notes on PROVEN results: ' + ', '.join('%s %d' % kv for kv in notes.most_common()))
    plain_bad = [r for r in results if not r.get('postproc') and not r.get('identical')
                 and r['verdict'] != 'UNMEASURABLE']
    if plain_bad:
        print('NOTE: %d zero-PostProc function(s) have P != S words: %s' % (
            len(plain_bad), ', '.join(r['id'] for r in plain_bad[:10])))
    if a.json:
        Path(a.json).write_text(json.dumps(results, indent=0) + '\n')
    proven = sum(1 for r in results if r['verdict'] == 'PROVEN')
    print('RESULT: %d / %d PROVEN%s' % (proven, len(results),
                                       '' if proven == len(results) else ' -- FAILED'))
    return 0 if proven == len(results) else 1


# ---------------------------------------------------------------------------
# Self-test
# ---------------------------------------------------------------------------

_RIDX = {n: i for i, n in enumerate(RNAME[:32])}
_RIDX['fp'] = 30


def asm(lines):
    """Tiny assembler for fixtures -> (words, rel).  A trailing `@sym` on a
    jal / lui / addiu / load / store attaches a relocation (jal: R_MIPS_26,
    lui: HI16, others: LO16)."""
    words, rel = [], {}
    labels = {}
    toks = []
    for ln in lines:
        ln = ln.split('#')[0].strip()
        if not ln:
            continue
        if ln.endswith(':'):
            labels[ln[:-1]] = len(toks)
            continue
        toks.append(ln)
    R = lambda x: _RIDX[x.strip().lstrip('$')]
    for i, ln in enumerate(toks):
        sym = None
        if '@' in ln:
            ln, sym = ln.split('@')
            ln, sym = ln.strip(), sym.strip()
        op, _, rest = ln.partition(' ')
        a = [x.strip() for x in rest.split(',')] if rest else []

        def imm(x):
            return int(x, 0) & 0xFFFF

        def mem(x):
            off, base = x.rstrip(')').split('(')
            return int(off, 0) & 0xFFFF, R(base)
        rt_ = {'addu': 0x21, 'subu': 0x23, 'and': 0x24, 'or': 0x25, 'xor': 0x26,
               'slt': 0x2a, 'sltu': 0x2b}
        it_ = {'addiu': 9, 'slti': 10, 'sltiu': 11, 'andi': 12, 'ori': 13, 'xori': 14}
        ld_ = {'lb': 32, 'lh': 33, 'lw': 35, 'lbu': 36, 'lhu': 37,
               'sb': 40, 'sh': 41, 'sw': 43}
        if op == 'nop':
            w = 0
        elif op == 'move':
            w = (R(a[1]) << 21) | (R(a[0]) << 11) | 0x25
        elif op in rt_:
            w = (R(a[1]) << 21) | (R(a[2]) << 16) | (R(a[0]) << 11) | rt_[op]
        elif op in ('sll', 'srl', 'sra'):
            w = (R(a[1]) << 16) | (R(a[0]) << 11) | (int(a[2]) << 6) | \
                {'sll': 0, 'srl': 2, 'sra': 3}[op]
        elif op in it_:
            w = (it_[op] << 26) | (R(a[1]) << 21) | (R(a[0]) << 16) | imm(a[2])
        elif op == 'li':
            w = (9 << 26) | (R(a[0]) << 16) | imm(a[1])
        elif op == 'lui':
            w = (15 << 26) | (R(a[0]) << 16) | imm(a[1])
        elif op in ld_:
            off, base = mem(a[1])
            w = (ld_[op] << 26) | (base << 21) | (R(a[0]) << 16) | off
        elif op in ('beq', 'bne'):
            t = labels[a[2]]
            w = ((4 if op == 'beq' else 5) << 26) | (R(a[0]) << 21) | (R(a[1]) << 16) | \
                ((t - i - 1) & 0xFFFF)
        elif op in ('blez', 'bgtz'):
            t = labels[a[1]]
            w = ((6 if op == 'blez' else 7) << 26) | (R(a[0]) << 21) | ((t - i - 1) & 0xFFFF)
        elif op == 'b':
            t = labels[a[0]]
            w = (4 << 26) | ((t - i - 1) & 0xFFFF)
        elif op == 'jal':
            w = 3 << 26
        elif op == 'jr':
            w = (R(a[0]) << 21) | 8
        elif op == 'jalr':
            w = (R(a[0]) << 21) | (31 << 11) | 9
        elif op in ('divu', 'div', 'multu', 'mult'):
            w = (R(a[0]) << 21) | (R(a[1]) << 16) | \
                {'mult': 0x18, 'multu': 0x19, 'div': 0x1a, 'divu': 0x1b}[op]
        elif op in ('mflo', 'mfhi'):
            w = (R(a[0]) << 11) | (0x12 if op == 'mflo' else 0x10)
        elif op == 'break':
            w = (int(a[0]) << 16) | 0xD
        else:
            raise ValueError(op)
        words.append(w)
        if sym:
            typ = {'jal': R_MIPS_26, 'lui': R_MIPS_HI16}.get(op, R_MIPS_LO16)
            rel[i] = (typ, sym)
    return words, rel


_BASE = [
    'addiu sp,sp,-24', 'sw ra,20(sp)', 'sw s0,16(sp)', 'move s0,a0',
    'lw t6,0(a0)', 'lw t8,12(a0)', 'addiu t7,t6,1', 'sw t7,4(a0)',
    'move a1,t8', 'jal @foo', 'move a0,t7',
    'beq v0,zero,L1', 'nop',
    'sw v0,8(s0)',
    'L1:', 'lw ra,20(sp)', 'lw s0,16(sp)', 'jr ra', 'addiu sp,sp,24']


def _sub(lines, old, new):
    out = list(lines)
    i = out.index(old)
    out[i] = new
    return out


def _swap(lines, x, y):
    out = list(lines)
    i, j = out.index(x), out.index(y)
    out[i], out[j] = out[j], out[i]
    return out


def _rename(lines, a, b):
    return [re.sub(r'\b%s\b' % a, b, ln) for ln in lines]


SELF_TEST_TEXT = 'extern int foo(int a, int b);\nextern int bar(int a, int b);\n' \
    'extern int qux(int a, int b, int c, int d, int e);\n' \
    'int f(int *p) { return 0; }\n'


def _fixtures():
    B = _BASE
    loop = ['li v0,0', 'li t6,10', 'L:', 'lw t7,0(a0)', 'addiu t6,t6,-1', 'addiu a0,a0,4',
            'addu v0,v0,t7', 'bne t6,zero,L', 'nop', 'jr ra', 'nop']
    frame = ['addiu sp,sp,-32', 'sw ra,28(sp)', 'sw a0,16(sp)', 'jal @foo', 'li a1,3',
             'lw t6,16(sp)', 'nop', 'sw v0,0(t6)', 'lw ra,28(sp)', 'nop', 'jr ra',
             'addiu sp,sp,32']
    esc = ['addiu sp,sp,-32', 'sw ra,28(sp)', 'sw zero,20(sp)', 'addiu a0,sp,20',
           'jal @foo', 'li a1,3', 'lw v0,20(sp)', 'lw ra,28(sp)', 'nop', 'jr ra',
           'addiu sp,sp,32']
    # mode B (an escaped local) with a callee-saved register save area
    save = ['addiu sp,sp,-32', 'sw ra,28(sp)', 'sw s0,24(sp)', 'move s0,a0',
            'addiu a0,sp,16', 'jal @foo', 'li a1,3', 'lw v0,0(s0)', 'lw ra,28(sp)',
            'lw s0,24(sp)', 'jr ra', 'addiu sp,sp,32']
    # ... a "save" word an escaped address is formed in, or that is
    # reloaded into another register, is NOT a save slot
    save_obj = ['addiu sp,sp,-32', 'sw ra,28(sp)', 'sw s0,24(sp)', 'addiu a0,sp,24',
                'jal @foo', 'li a1,3', 'sw zero,0(v0)', 'lw s0,24(sp)', 'lw ra,28(sp)',
                'nop', 'jr ra', 'addiu sp,sp,32']
    save_oth = ['addiu sp,sp,-32', 'sw ra,28(sp)', 'sw s0,24(sp)', 'addiu a0,sp,16',
                'jal @foo', 'li a1,3', 'sw zero,0(v0)', 'lw t0,24(sp)', 'lw ra,28(sp)',
                'move v0,t0', 'jr ra', 'addiu sp,sp,32']
    # mode B: a call's unread stack-argument words need no validity
    stk = ['addiu sp,sp,-40', 'sw ra,32(sp)', 'addiu a0,sp,28', 'li t6,5', 'sw t6,16(sp)',
           'jal @baz', 'li a1,1', 'beq v0,zero,L', 'nop', 'jal @foo', 'move a0,v0', 'L:',
           'lw ra,32(sp)', 'nop', 'jr ra', 'addiu sp,sp,40']
    # 16*a0+5 can never meet 0x80131690 (mod 16); 16*a0+0 can
    strd = ['sll t0,a0,4', 'lui t1,0x8013', 'sb a1,5(t0)', 'lh v0,5776(t1)', 'jr ra', 'nop']
    # a in [0,255]: 16a never equals a+1 (15a = 1 has no such root), but
    # 16a equals a+15 at a = 1
    bnd = ['andi t0,a0,0xff', 'sll t1,t0,4', 'addiu t2,t0,1', 'sb a1,0(t1)',
           'lbu v0,0(t2)', 'jr ra', 'nop']
    # 0 <= a0 < 24 (branch guards): 16*a0 never meets a0+376 (only a0 = 25)
    grd = ['slti at,a0,24', 'beq at,zero,L', 'move v0,zero', 'slti at,a0,0', 'bne at,zero,L',
           'nop',
           'sll t0,a0,4', 'sh a1,0(t0)', 'lbu v0,376(a0)', 'L:', 'jr ra', 'nop']
    # mode B: a pointer the caller passed cannot reach this frame; one
    # loaded from memory or returned by a callee can
    ffr = ['addiu sp,sp,-40', 'sw ra,32(sp)', 'sw s0,28(sp)', 'move s0,a0', 'addiu a0,sp,16',
           'jal @foo', 'li a1,1', 'li t0,7', 'sw t0,20(sp)', 'lw v0,4(s0)', 'lw ra,32(sp)',
           'lw s0,28(sp)', 'jr ra', 'addiu sp,sp,40']
    # a load after two stores: disjoint stores commute, may-alias ones do not
    comm = ['sw a1,4(a0)', 'sw a2,8(a0)', 'lw v0,0(a3)', 'jr ra', 'nop']
    comm2 = ['sw a1,4(a0)', 'sw a2,8(a3)', 'lw v0,0(a3)', 'jr ra', 'nop']
    join_p = ['sw a1,0(a2)', 'beq a0,zero,J', 'nop', 'addiu v1,v1,1', 'J:', 'jr ra', 'move v0,v1']
    join_s = ['beq a0,zero,J', 'nop', 'addiu v1,v1,1', 'J:', 'sw a1,0(a2)', 'jr ra',
              'move v0,v1']
    # IDO's divide check: P loads before it, S after it (the trap path is
    # C undefined behaviour; the load is released there, nothing else is)
    dv_p = ['divu a0,a2', 'lw t7,0(a1)', 'bne a2,zero,K', 'nop', 'break 7', 'K:',
            'mflo t6', 'addu v0,t6,t7', 'jr ra', 'nop']
    dv_s = ['divu a0,a2', 'bne a2,zero,K', 'nop', 'break 7', 'K:', 'lw t7,0(a1)',
            'mflo t6', 'addu v0,t6,t7', 'jr ra', 'nop']
    yes = [
        ('identity', B, B),
        ('load-sunk-past-divide-check (UB trap path)', dv_p, dv_s),
        ('store-past-check-of-a-constant-divisor (infeasible trap)',
         ['li a2,84', 'divu a0,a2', 'sw a3,0(a1)', 'bne a2,zero,K', 'nop', 'break 7', 'K:',
          'mfhi v0', 'jr ra', 'nop'],
         ['li a2,84', 'divu a0,a2', 'bne a2,zero,K', 'nop', 'break 7', 'K:', 'sw a3,0(a1)',
          'mfhi v0', 'jr ra', 'nop']),
        ('uninit-read-remapped-and-moved (indeterminate stack)',
         ['addiu sp,sp,-32', 'sw ra,28(sp)', 'lw a1,16(sp)', 'li a0,1', 'jal @foo', 'nop',
          'lw ra,28(sp)', 'nop', 'jr ra', 'addiu sp,sp,32'],
         ['addiu sp,sp,-32', 'sw ra,28(sp)', 'li a0,1', 'lw a1,20(sp)', 'jal @foo', 'nop',
          'lw ra,28(sp)', 'nop', 'jr ra', 'addiu sp,sp,32']),
        ('rename-temp', B, _rename(B, 't6', 't9')),
        ('rename-callee-saved', B, _rename(B, 's0', 's1')),
        ('swap-independent', B, _swap(B, 'lw t8,12(a0)', 'move s0,a0')),
        ('or-to-addu', B, _sub(B, 'move s0,a0', 'addu s0,a0,zero')),
        ('fill-call-delay-slot', B, _swap(B, 'move a1,t8', 'jal @foo')[:8] +
         ['move a0,t7', 'jal @foo', 'move a1,t8'] + B[11:]),
        ('slot-remap', frame, _sub(_sub(frame, 'sw a0,16(sp)', 'sw a0,20(sp)'),
                                   'lw t6,16(sp)', 'lw t6,20(sp)')),
        ('frame-resize', frame, ['addiu sp,sp,-40', 'sw ra,36(sp)', 'sw a0,24(sp)'] +
         frame[3:5] + ['lw t6,24(sp)', 'nop', 'sw v0,0(t6)', 'lw ra,36(sp)', 'nop', 'jr ra',
                       'addiu sp,sp,40']),
        ('saved-reg-slot-swap', B, _sub(_sub(_sub(_sub(B, 'sw ra,20(sp)', 'sw ra,16(sp)'),
                                                  'sw s0,16(sp)', 'sw s0,20(sp)'),
                                             'lw ra,20(sp)', 'lw ra,16(sp)'),
                                        'lw s0,16(sp)', 'lw s0,20(sp)')),
        ('loop-rename', loop, _rename(_rename(loop, 't6', 't8'), 't7', 't9')),
        ('loop-swap', loop, _swap(loop, 'addiu a0,a0,4', 'addiu t6,t6,-1')),
        ('escape-identity', esc, esc),
        ('store-sunk-to-join (path mode)', join_p, join_s),
        ('escape-save-area-swap', save, _swap(save, 'sw ra,28(sp)', 'sw s0,24(sp)')),
        ('modeB-2arg-call-after-5arg-call', stk, _rename(stk, 't6', 't7')),
        ('stride-congruence-disjoint', strd, _swap(strd, 'sb a1,5(t0)', 'lh v0,5776(t1)')),
        ('bounded-index-disjoint', bnd, _swap(bnd, 'sb a1,0(t1)', 'lbu v0,0(t2)')),
        ('guarded-index-disjoint', grd, _swap(grd, 'sh a1,0(t0)', 'lbu v0,376(a0)')),
        ('modeB-arg-pointer-vs-frame-slot', ffr, _swap(ffr, 'sw t0,20(sp)', 'lw v0,4(s0)')),
        ('disjoint-stores-swapped-before-load', comm,
         _swap(comm, 'sw a1,4(a0)', 'sw a2,8(a0)')),
    ]
    no = [
        ('swapped-dependent-stores', B + [],
         _swap(B, 'sw t7,4(a0)', 'sw v0,8(s0)') if False else
         _swap(_sub(B, 'sw v0,8(s0)', 'sw v0,4(s0)'), 'lw t6,0(a0)', 'lw t6,0(a0)')),
        ('changed-constant', B, _sub(B, 'addiu t7,t6,1', 'addiu t7,t6,2')),
        ('load-across-aliasing-store',
         _sub(B, 'lw t8,12(a0)', 'lw t8,4(a0)'),
         None),
        ('changed-branch', B, _sub(B, 'beq v0,zero,L1', 'bne v0,zero,L1')),
        ('changed-branch-target', B, B[:11] + ['beq v0,zero,L2', 'nop', 'L2:'] + B[13:]),
        ('swapped-call-args', B, _sub(_sub(B, 'move a1,t8', 'move a1,t7'),
                                      'move a0,t7', 'move a0,t8')),
        ('dropped-store', B, _sub(B, 'sw t7,4(a0)', 'nop')),
        ('load-delay-hazard', B, _swap(B, 'lw t8,12(a0)', 'lw t6,0(a0)')),
        ('dataflow-paint', B, _sub(B, 'addiu t7,t6,1', 'addiu t7,s0,1')),
        ('decrement-into-delay-slot', loop, loop[:4] + ['addiu a0,a0,4', 'addu v0,v0,t7',
                                                      'bne t6,zero,L', 'addiu t6,t6,-1']
         + loop[9:]),
        ('uninit-slot-read', frame, _sub(frame, 'sw a0,16(sp)', 'sw a0,20(sp)')),
        ('escape-uninit-after-call', esc, _sub(esc, 'lw v0,20(sp)', 'lw v0,0(sp)')),
        ('call-target', B, _sub(B, 'jal @foo', 'jal @bar')),
        ('slot-remap-inconsistent', frame, _sub(frame, 'lw t6,16(sp)', 'lw t6,20(sp)')),
        ('escape-remap', esc, _sub(_sub(_sub(esc, 'sw zero,20(sp)', 'sw zero,24(sp)'),
                                        'addiu a0,sp,20', 'addiu a0,sp,24'),
                                   'lw v0,20(sp)', 'lw v0,24(sp)')),
        ('store-moved-into-one-path', join_p,
         ['beq a0,zero,J', 'nop', 'sw a1,0(a2)', 'addiu v1,v1,1', 'J:', 'jr ra', 'move v0,v1']),
        ('loop-paint', loop, _sub(loop, 'addu v0,v0,t7', 'addu v0,v0,t6')),
        ('store-below-sp', frame, _sub(frame, 'sw a0,16(sp)', 'sw a0,-4(sp)')),
        ('return-value', B, B[:-2] + ['jr ra', 'move v0,zero'] if False else
         _sub(B, 'lw s0,16(sp)', 'lw s1,16(sp)')),
        ('modeB-stale-stack-arg-read', _sub(stk, 'jal @foo', 'jal @qux'),
         _rename(_sub(stk, 'jal @foo', 'jal @qux'), 't6', 't7')),
        ('stride-congruence-may-alias', _sub(strd, 'sb a1,5(t0)', 'sb a1,0(t0)'),
         _swap(_sub(strd, 'sb a1,5(t0)', 'sb a1,0(t0)'), 'sb a1,0(t0)', 'lh v0,5776(t1)')),
        ('bounded-index-may-alias', _sub(bnd, 'addiu t2,t0,1', 'addiu t2,t0,15'),
         _swap(_sub(bnd, 'addiu t2,t0,1', 'addiu t2,t0,15'), 'sb a1,0(t1)', 'lbu v0,0(t2)')),
        ('guard-too-weak', _sub(grd, 'slti at,a0,24', 'slti at,a0,26'),
         _swap(_sub(grd, 'slti at,a0,24', 'slti at,a0,26'), 'sh a1,0(t0)', 'lbu v0,376(a0)')),
        ('guard-on-other-register', _sub(grd, 'slti at,a0,24', 'slti at,a1,24'),
         _swap(_sub(grd, 'slti at,a0,24', 'slti at,a1,24'), 'sh a1,0(t0)', 'lbu v0,376(a0)')),
        ('guarded-register-rewritten-in-slot', _sub(grd, 'nop', 'addiu a0,a0,2'),
         _swap(_sub(grd, 'nop', 'addiu a0,a0,2'), 'sh a1,0(t0)', 'lbu v0,376(a0)')),
        ('modeB-loaded-pointer-vs-frame-slot', _sub(ffr, 'move s0,a0', 'lw s0,0(a0)'),
         _swap(_sub(ffr, 'move s0,a0', 'lw s0,0(a0)'), 'sw t0,20(sp)', 'lw v0,4(s0)')),
        ('modeB-returned-pointer-vs-frame-slot', _sub(ffr, 'li t0,7', 'move s0,v0'),
         _swap(_sub(ffr, 'li t0,7', 'move s0,v0'), 'sw t0,20(sp)', 'lw v0,4(s0)')),
        ('may-alias-stores-swapped-before-load', comm2,
         _swap(comm2, 'sw a1,4(a0)', 'sw a2,8(a3)')),
        ('save-slot-in-escaped-object', save_obj,
         _swap(save_obj, 'sw zero,0(v0)', 'lw s0,24(sp)')),
        ('save-slot-reloaded-elsewhere', save_oth,
         _swap(save_oth, 'sw zero,0(v0)', 'lw t0,24(sp)')),
        ('store-sunk-past-divide-check', _sub(dv_p, 'lw t7,0(a1)', 'sw a0,0(a1)'),
         _sub(dv_s, 'lw t7,0(a1)', 'sw a0,0(a1)')),
        ('load-sunk-past-divide-check-and-a-store',
         dv_p[:5] + ['K:', 'sw a0,0(a3)'] + dv_p[6:],
         dv_s[:5] + ['sw a0,0(a3)'] + dv_s[5:]),
        ('store-past-check-of-a-variable-divisor',
         ['divu a0,a2', 'sw a3,0(a1)', 'bne a2,zero,K', 'nop', 'break 7', 'K:',
          'mfhi v0', 'jr ra', 'nop'],
         ['divu a0,a2', 'bne a2,zero,K', 'nop', 'break 7', 'K:', 'sw a3,0(a1)',
          'mfhi v0', 'jr ra', 'nop']),
        ('changed-constant-makes-path-feasible',
         ['li a2,84', 'divu a0,a2', 'sw a3,0(a1)', 'bne a2,zero,K', 'nop', 'break 7', 'K:',
          'mfhi v0', 'jr ra', 'nop'],
         ['li a2,0', 'divu a0,a2', 'bne a2,zero,K', 'nop', 'break 7', 'K:', 'sw a3,0(a1)',
          'mfhi v0', 'jr ra', 'nop']),
        ('load-sunk-past-a-non-ub-break', _sub(dv_p, 'break 7', 'break 0'),
         _sub(dv_s, 'break 7', 'break 0')),
    ]
    # load moved across an aliasing store: P loads 4(a0) after the store,
    # S before it
    lp = _sub(B, 'move a1,t8', 'lw a1,4(a0)')
    lp = lp[:8] + ['nop'] + lp[8:]
    ls = list(lp)
    i, j = ls.index('sw t7,4(a0)'), ls.index('lw a1,4(a0)')
    ls[i], ls[j] = ls[j], ls[i]
    no[2] = ('load-across-aliasing-store', lp, ls)
    # two stores to the same location exchanged
    sp_ = _sub(B, 'sw v0,8(s0)', 'nop')
    sp_ = sp_[:8] + ['sw t8,4(a0)'] + sp_[8:]
    ss_ = list(sp_)
    i, j = ss_.index('sw t7,4(a0)'), ss_.index('sw t8,4(a0)')
    ss_[i], ss_[j] = ss_[j], ss_[i]
    no[0] = ('swapped-dependent-stores', sp_, ss_)
    return yes, no


def _prove_lines(pl, sl, text=None, nvl=None, sent=None, argwords=None, objects=None):
    pw, pr = asm(pl)
    sw, sr = asm(sl)
    rec = dict(P=dict(words=pw, rel=pr, jt=[], base=0), S=dict(words=sw, rel=sr, jt=[], base=0),
               rodata_equal=True, text=text or SELF_TEST_TEXT, name='f', argwords=argwords,
               objects=objects)
    if nvl is not None:
        nw, nr = asm(nvl)
        rec['Pnv'] = dict(words=nw, rel=nr, jt=[], base=0)
    if sent is not None:
        sl_, smap = sent
        qw, qr = asm(sl_)
        rec['Psent'] = dict(words=qw, rel=qr, jt=[], base=0,
                            smap={str(k): v for k, v in smap.items()})
    return prove_record(rec)


def _flow_fixtures():
    """Value-flow fixtures: (name, C text, P, C'' compile + sentinel map or
    None, S, expect PROVEN)."""
    lit = 'int f(int a, int *p) { if (a) return 0; p[1] = (*p < 1); }\n'
    p = ['beq a0,zero,L', 'nop', 'b X', 'move v0,zero', 'L:', 'lw t6,0(a1)', 'nop',
         'sltiu v0,t6,1', 'sw v0,4(a1)', 'X:', 'jr ra', 'nop']
    s = _sub(_sub(p, 'sltiu v0,t6,1', 'sltiu v1,t6,1'), 'sw v0,4(a1)', 'sw v1,4(a1)')
    q = (_sub(p, 'move v0,zero', 'li v0,29993'), {29993: 0})
    calls = ('extern int h(void); extern int k(void);\n'
             'int f(int a, int *p) { if (a) return h(); *p = k(); }\n')
    pc = ['addiu sp,sp,-24', 'sw ra,20(sp)', 'sw s0,16(sp)', 'move s0,a1',
          'beq a0,zero,L', 'nop', 'jal @h', 'nop', 'b X', 'nop',
          'L:', 'jal @k', 'nop', 'sw v0,0(s0)',
          'X:', 'lw ra,20(sp)', 'lw s0,16(sp)', 'jr ra', 'addiu sp,sp,24']
    sc = _sub(pc, 'sw v0,0(s0)', 'sw v0,0(s0)')
    sc = sc[:sc.index('X:')] + ['li v0,3'] + sc[sc.index('X:'):]
    pc_ld = pc[:pc.index('X:')] + ['lw v0,8(s0)'] + pc[pc.index('X:'):]
    sc_ld = pc[:pc.index('X:')] + ['lw v1,8(s0)'] + pc[pc.index('X:'):]
    return [
        ('flow-sentinel-valueless-exempt', lit, p, q, s, True),
        ('flow-sentinel-return-changed', lit, p, q, _sub(s, 'move v0,zero', 'li v0,1'), False),
        ('flow-literal-needs-Cdoubleprime', lit, p, None, s, False),
        ('flow-call-result-maybe-valued', calls, pc, None, sc, False),
        ('flow-load-not-valued', calls, pc_ld, None, sc_ld, True),
        ('flow-valued-call-changed', calls, pc, None,
         _sub(pc, "jal @h", "jal @k"), False),
    ]


def _noval_fixtures():
    """Indeterminate-return fixtures: (name, C text, P, P' (C' compile or
    None), S, expect PROVEN)."""
    mixed = 'int f(int a, int *p) { if (a) return 5; *p = 1; }\n'
    bare = 'int f(int a, int *p) { if (a) { p[1] = 0; return; } *p = 1; return 7; }\n'
    none_ = 'int f(int a, int *p) { if (a) return; *p = 1; }\n'
    # P: the valued path sets v0 = 5; the fall-off path leaves v0 alone
    p = ['beq a0,zero,L', 'nop', 'b X', 'li v0,5', 'L:', 'li t6,1', 'sw t6,0(a1)',
         'X:', 'jr ra', 'nop']
    # C': the fall-off path returns &__peq_noval
    pn = ['beq a0,zero,L', 'nop', 'b X', 'li v0,5', 'L:', 'li t6,1', 'sw t6,0(a1)',
          'lui v0,0 @' + NOVAL_SYM, 'addiu v0,v0,0 @' + NOVAL_SYM, 'X:', 'jr ra', 'nop']
    # S uses $v0 as the store temp on the fall-off path: v0 differs there only
    s_ok = _sub(p, 'li t6,1', 'li v0,1')
    s_ok = _sub(s_ok, 'sw t6,0(a1)', 'sw v0,0(a1)')
    s_bad = _sub(s_ok, 'li v0,5', 'li v0,6')            # changed `return 5`
    # P itself uses $v0 as the temp on the fall-off path (not provably
    # value-less from P alone; C' marks it)
    pn2 = s_ok[:s_ok.index('X:')] + ['lui v0,0 @' + NOVAL_SYM,
                                      'addiu v0,v0,0 @' + NOVAL_SYM] + s_ok[s_ok.index('X:'):]
    # bare return + valued `return 7`
    pb = ['bne a0,zero,Y', 'nop', 'li t6,1', 'sw t6,0(a1)', 'b X', 'li v0,7',
          'Y:', 'sw zero,4(a1)', 'X:', 'jr ra', 'nop']
    pbn = ['bne a0,zero,Y', 'nop', 'li t6,1', 'sw t6,0(a1)', 'b X', 'li v0,7',
           'Y:', 'sw zero,4(a1)', 'lui v0,0 @' + NOVAL_SYM, 'addiu v0,v0,0 @' + NOVAL_SYM,
           'X:', 'jr ra', 'nop']
    sb_ok = ['bne a0,zero,Y', 'li v0,3', 'li t6,1', 'sw t6,0(a1)', 'b X', 'li v0,7',
             'Y:', 'sw zero,4(a1)', 'X:', 'jr ra', 'nop']
    sb_bad = _sub(sb_ok, 'li v0,7', 'li v0,8')
    vtext = 'extern void g(void);\nint f(int a) { if (a) return 5; g(); }\n'
    vp = ['addiu sp,sp,-24', 'sw ra,20(sp)', 'beq a0,zero,L', 'nop', 'b X', 'li v0,5',
          'L:', 'jal @g', 'nop', 'X:', 'lw ra,20(sp)', 'nop', 'jr ra', 'addiu sp,sp,24']
    vs = _sub(vp, 'jal @g', 'li v0,9')
    vs = vs[:vs.index('li v0,9') + 1] + ['jal @g'] + vs[vs.index('li v0,9') + 1:]
    return [
        ('noval-fall-off-path-exempt', mixed, p, pn, s_ok, True),
        ('noval-valued-path-changed', mixed, p, pn, s_bad, False),
        ('noval-valued-changed-no-Cprime', mixed, p, None, s_bad, False),
        ('entry-v0-path-exempt', mixed, p, None, s_ok, True),
        ('noval-temp-v0-exempt-by-Cprime', mixed, s_ok, pn2, p, True),
        ('noval-temp-v0-no-Cprime', mixed, s_ok, None, p, False),
        ('noval-bare-return-exempt', bare, pb, pbn, sb_ok, True),
        ('noval-bare-valued-changed', bare, pb, pbn, sb_bad, False),
        ('noval-no-valued-return', none_, p, None, s_bad, True),
        # the void-call rule (no C' needed): $v0 last defined by a call to a
        # callee the file declares void cannot be a `return <expr>;` value
        ('voidcall-path-exempt', vtext, vp, None, vs, True),
        ('voidcall-valued-changed', vtext, vp, None, _sub(vs, 'li v0,5', 'li v0,6'), False),
        ('voidcall-callee-undeclared', vtext.replace('extern void g(void);', ''), vp, None,
         vs, False),
        ('voidcall-callee-header-only', vtext.replace('extern void g(void);',
                                                      '#include "x.h"'), vp, None, vs, False),
        ('voidcall-no-valueless-exit', vtext.replace('g(); }', 'g(); return 5; }'), vp, None,
         vs, False),
    ]


def _interval_fuzz(seed=12, programs=400, trials=20):
    """reg_intervals against concrete execution: random forward-branching
    programs (compare-and-branch guards included), random inputs; every
    register value at every block entry reached must lie in its interval.
    Returns the number of violations."""
    import random
    rnd = random.Random(seed)
    R = ['at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2', 't3']

    def s32(v):
        v &= 0xFFFFFFFF
        return v - (1 << 32) if v & 0x80000000 else v

    def gen():
        lines, pend, nl = [], [], 0
        for _ in range(14):
            c = rnd.random()
            r1, r2, r3 = rnd.choice(R), rnd.choice(R + ['zero']), rnd.choice(R + ['zero'])
            if c < 0.1:
                lab = 'L%d' % nl
                nl += 1
                mid = []
                for _m in range(rnd.choice([0, 0, 1, 2, 3])):
                    # copies of the compared register, and redefinitions of
                    # the register or of its copies (the refinement must
                    # follow exactly the registers still equal to it)
                    q = rnd.choice(R[1:])
                    mid.append(rnd.choice(['move %s,%s' % (q, r1), 'move %s,%s' % (q, r1),
                                           'addiu %s,%s,3' % (q, q),
                                           'addiu %s,%s,1' % (r1, r1)]))
                lines += ['%s at,%s,%d' % (rnd.choice(['slti', 'sltiu']), r1, rnd.randrange(-6, 30))] + \
                    mid + ['%s at,zero,%s' % (rnd.choice(['beq', 'bne']), lab),
                          rnd.choice(['nop', 'addiu %s,%s,1' % (r1, r1)])]
                pend.append(lab)
            elif c < 0.18:
                lab = 'L%d' % nl
                nl += 1
                lines += ['%s %s,%s' % (rnd.choice(['blez', 'bgtz']), r2, lab), 'nop']
                pend.append(lab)
            elif c < 0.28:
                lines.append('%s %s,%s,%d' % (rnd.choice(['addiu', 'slti', 'sltiu', 'andi']), r1,
                                              r2, rnd.randrange(-40, 300)))
            elif c < 0.36:
                lines.append('%s %s,%s,%d' % (rnd.choice(['sll', 'srl', 'sra']), r1, r2,
                                              rnd.randrange(0, 32)))
            elif c < 0.5:
                lines.append('%s %s,%s,%s' % (rnd.choice(['addu', 'subu', 'and', 'or', 'slt',
                                                          'sltu', 'xor']), r1, r2, r3))
            elif c < 0.6:
                lines.append('%s %s,0(a3)' % (rnd.choice(['lb', 'lbu', 'lh', 'lhu', 'lw']), r1))
            elif c < 0.7:
                lines.append('lui %s,%d' % (r1, rnd.randrange(0, 65536)))
            else:
                lines.append('move %s,%s' % (r1, r2))
            if pend and rnd.random() < 0.4:
                lines += [x + ':' for x in pend]
                pend = []
        return lines + [x + ':' for x in pend] + ['jr ra', 'nop']

    def ex(regs, d):
        if d.w == 0 or not d.defs:
            return
        n, a, b, imm = d.name, regs[d.rs] & 0xFFFFFFFF, regs[d.rt] & 0xFFFFFFFF, _sx16(d.imm)
        if d.kind == 'alui':
            v = {'addiu': a + imm, 'slti': int(s32(a) < imm), 'andi': a & d.imm,
                 'sltiu': int(a < (imm & 0xFFFFFFFF)), 'ori': a | d.imm, 'xori': a ^ d.imm}[n]
        elif d.kind == 'lui':
            v = d.imm << 16
        elif d.kind == 'shift':
            v = {'sll': b << d.sa, 'srl': b >> d.sa, 'sra': s32(b) >> d.sa}[n]
        elif d.kind == 'alur':
            v = {'addu': a + b, 'subu': a - b, 'and': a & b, 'or': a | b, 'xor': a ^ b,
                 'slt': int(s32(a) < s32(b)), 'sltu': int(a < b)}[n]
        else:
            raw = rnd.randrange(0, 1 << 32)
            v = {'lb': s32(raw << 24) >> 24, 'lbu': raw & 255, 'lh': s32(raw << 16) >> 16,
                 'lhu': raw & 65535, 'lw': raw}[n]
        regs[d.defs[0]] = s32(v)

    bad = 0
    for _ in range(programs):
        w, r = asm(gen())
        f = Func('P', w, r, [], 0)
        f.build_cfg()
        iv = reg_intervals(f)
        for _ in range(trials):
            regs = [0] + [s32(rnd.choice([rnd.randrange(-8, 32), rnd.randrange(1 << 32)]))
                          for _ in range(33)]
            regs[0] = 0
            pc = 0
            for _ in range(200):
                if pc in f.blk_at:
                    st = iv.get(f.blk_at[pc])
                    if st is None or any(st[q] is not None and not st[q][0] <= regs[q] <= st[q][1]
                                         for q in range(32)):
                        bad += 1
                        break
                d = f.ins[pc]
                if d.kind == 'jr':
                    break
                if d.kind == 'br':
                    x, y = s32(regs[d.rs]), s32(regs[d.rt])
                    take = {'beq': x == y, 'bne': x != y, 'blez': x <= 0, 'bgtz': x > 0}[d.name]
                    ex(regs, f.ins[pc + 1])
                    pc = branch_target(d) if take else pc + 2
                    continue
                ex(regs, d)
                pc += 1
    return bad


def self_test():
    yes, no = _fixtures()
    ok = True
    nbad = _interval_fuzz()
    ok &= nbad == 0
    print('  %-4s %-30s %d violations (400 programs x 20 inputs)' % (
        'ok' if not nbad else 'FAIL', 'interval-fuzz', nbad))
    # .mdebug object sizes from a real IDO -g3 compile (skipped without IDO):
    # the OUTERMOST type qualifier sizes the object
    try:
        import resolve_overlay_relocs as _R
        td = Path(tempfile.mkdtemp(prefix='peq_md_'))
        (td / 'm.c').write_text('extern void g(void *, void *, void *, void *);\n'
                                'void f(void) { int *a[4]; short b[3]; short c[2][5]; '
                                'char *d; g(a, b, c, &d); }\n')
        r = subprocess.run([str(_R.IDO_CC)] + _R.IDO_FLAGS + ['-g3', '-c', 'm.c', '-o', 'm.o'],
                           cwd=str(td), capture_output=True, env=dict(os.environ, TMPDIR=str(td)))
        objs = MDebug(str(td / 'm.o')).frame_objects('f') if r.returncode == 0 else None
        shutil.rmtree(td, ignore_errors=True)
    except Exception:                                # noqa: BLE001
        objs = None
    if objs is None:
        print('  skip mdebug-sizes (no IDO -g3 compile)')
    else:
        got = {n: sz for n, _, sz, _ in objs}
        want = {'a': 16, 'b': 6, 'c': 20, 'd': 4}
        good = all(got.get(k) == v for k, v in want.items())
        ok &= good
        print('  %-4s %-36s expect %s got %s' % ('ok' if good else 'FAIL', 'mdebug-sizes', want,
                                                {k: got.get(k) for k in want}))
    # an indirect call whose C type returns a value writes $v0: what was in
    # $v0 before it is dead (P and S may leave different junk there)
    jr_p = ['addiu sp,sp,-24', 'sw ra,20(sp)', 'lw v0,0(a1)', 'lw t9,4(a1)', 'nop',
            'jalr t9', 'nop', 'lw ra,20(sp)', 'nop', 'jr ra', 'addiu sp,sp,24']
    jr_s = _sub(jr_p, 'lw v0,0(a1)', 'lw t8,0(a1)')
    for name, text, want in (
            ('jalr-nonvoid-v0-written', 'int f(int *p) { return 0; }\n'
             'typedef int (*F)(int); F g;\n', True),
            ('jalr-void-v0-kept', 'int f(int *p) { return 0; }\n'
             'typedef void (*F)(int); F g;\n', False)):
        r = _prove_lines(jr_p, jr_s, text)
        good = (r['verdict'] == 'PROVEN') == want
        ok &= good
        print('  %-4s %-36s expect %s got %s %s' % ('ok' if good else 'FAIL', name,
                                                   'PROVEN' if want else 'not PROVEN',
                                                   r['verdict'], r['msg'] or ''))
    for name, text, want in (('funcptr-returns-int', 's32 (*f[33])(s32, s32);', True),
                             ('funcptr-returns-void', '(void (**)(s32))', False),
                             ('funcptr-returns-pointer', 'u8 *(*g)(s32);', True),
                             ('funcptr-returns-disagree', 'void (*a)(void); s32 (*b)(void);',
                              None)):
        got = funcptr_nonvoid(text)
        good = got == want
        ok &= good
        print('  %-4s %-36s expect %s got %s' % ('ok' if good else 'FAIL', name, want, got))
    # indirect-call arity from the C function-pointer types
    for name, text, want in (
            ('funcptr-ptr-to-ptr', 'void f(void){ (*(void (**)(s32))&t[i])(64); }', 1),
            ('funcptr-plain', 'void (*cb)(s32, s32);', 2),
            ('funcptr-disagree', 'void (*a)(s32);\nvoid (**b)(s32, s32);', None)):
        got = funcptr_arity(text)
        good = got == want
        ok &= good
        print('  %-4s %-36s expect %s got %s' % ('ok' if good else 'FAIL', name, want, got))
    # overlay link base, measured from the image's own jals (synthetic images)
    global _IMAGES
    saved = (_IMAGES, dict(_SYMS), dict(_LINKSHIFT))
    try:
        ents = [0x80132000 + 0x40 * k for k in range(12)]
        jal = lambda a: (3 << 26) | ((a >> 2) & 0x3FFFFFF)
        _IMAGES = [('ZZ', 0x80132000, tuple([jal(a + 0x2000) for a in ents] + [0] * 300)),
                   ('YY', 0x80132000, tuple([jal(a) for a in ents] + [0] * 300))]
        for nm in ('ZZ', 'YY'):
            _SYMS[nm] = [(a, 0x40, 'func_%08X' % a, None) for a in ents]
            _LINKSHIFT.pop(nm, None)
        for name, got, want in (
                ('link-shift-measured', overlay_link_shift('ZZ'), 0x2000),
                ('link-shift-none', overlay_link_shift('YY'), 0),
                ('shifted-entry-is-start', is_c_function_start(ents[3] + 0x2000, 'ZZ'), True),
                ('unshifted-addr-not-start', is_c_function_start(ents[3] + 0x40 - 0x2000 + 4,
                                                                 'ZZ'), False),
                ('shifted-v0-callers-refused', v0_unused_by_callers(ents[3], 'ZZ'), False)):
            good = got == want
            ok &= good
            print('  %-4s %-36s expect %s got %s' % ('ok' if good else 'FAIL', name, want, got))
    finally:
        _IMAGES = saved[0]
        _SYMS.clear()
        _SYMS.update(saved[1])
        _LINKSHIFT.clear()
        _LINKSHIFT.update(saved[2])
    # local path mode on its own (whole-function path mode switched off)
    global FULL_PATH_MODE
    FULL_PATH_MODE = False
    try:
        jp = ['sw a1,0(a2)', 'beq a0,zero,J', 'nop', 'addiu v1,v1,1', 'J:', 'jr ra',
              'move v0,v1']
        for name, sl, want in (
                ('local-path-store-sunk-to-join', ['beq a0,zero,J', 'nop', 'addiu v1,v1,1',
                                                   'J:', 'sw a1,0(a2)', 'jr ra', 'move v0,v1'],
                 True),
                ('local-path-store-into-one-path', ['beq a0,zero,J', 'nop', 'sw a1,0(a2)',
                                                    'addiu v1,v1,1', 'J:', 'jr ra',
                                                    'move v0,v1'], False)):
            r = _prove_lines(jp, sl)
            good = (r['verdict'] == 'PROVEN') == want and \
                (not want or 'local-path-mode' in (r['msg'] or ''))
            ok &= good
            print('  %-4s %-36s expect %s got %s %s' % ('ok' if good else 'FAIL', name,
                                                       'PROVEN' if want else 'not PROVEN',
                                                       r['verdict'], r['msg'] or ''))
    finally:
        FULL_PATH_MODE = True
    # callee input liveness through an IDO jump table (retail-image reader)
    base = 0x80132000
    jt = ['sltiu at,t6,3', 'beq at,zero,D', 'sll t6,t6,2', 'lui at,0x8013',
          'addu at,at,t6', 'lw t6,0x2040(at)', 'nop', 'jr t6', 'nop',
          'nop', 'nop', 'nop', 'D:', 'nop', 'nop', 'nop', 'nop']
    tab = [base + 4 * 9, base + 4 * 10, base + 4 * 11]
    for name, lines, want in (
            ('jtable-read', jt, [9, 10, 11]),
            ('jtable-jr-reg-redefined', _sub(jt, 'nop', 'addiu t6,t6,4'), None),
            ('jtable-index-changed-after-bound',
             jt[:1] + ['addiu t6,t6,1'] + jt[1:8] + jt[9:], None),
            ('jtable-run-entered-from-elsewhere',
             jt[:4] + ['L:'] + jt[4:13] + ['b L'] + jt[14:], None)):
        W, _ = asm(lines)
        W = (W + [0] * 16)[:16] + tab
        ji = [q for q, w in enumerate(W[:16]) if w == 0x01C00008][0]   # jr t6
        got = _jtable_targets(W, base, ji)
        good = got == want
        ok &= good
        print('  %-4s %-36s expect %s got %s' % ('ok' if good else 'FAIL', name, want, got))
    # window trimming of P: a zero tail goes, a last-word transfer keeps
    # its delay slot
    jr, nop, mv = 0x03E00008, 0, 0x00601025
    for name, words, want in (('trim-pad', [mv, jr, nop, nop, nop], 3),
                              ('trim-keeps-delay-slot', [mv, jr, nop, nop], 3),
                              ('trim-nonzero-tail-kept', [mv, mv, jr, mv], 4)):
        n = 3 if name != 'trim-keeps-delay-slot' else 2
        got = len(trim_zero_tail(words, {}, n)[0])
        good = got == want
        ok &= good
        print('  %-4s %-30s expect %d words   got %d' % ('ok' if good else 'FAIL', name,
                                                       want, got))
    # LO of a product by a constant is linear mod 2^32 (mult and multu)
    bk = TermBank()
    xa = bk.mk('in', 1, (0, 17))
    for op in ('multu', 'mult'):
        lt = bk.mk('lo', bk.mk(op, bk.mk('c', 36), xa))
        c_, m_ = _lin(bk.mk('sum', lt, 0x80129AF0))
        good = c_ == 0x80129AF0 and list(m_.values()) == [(xa, 36)]
        ok &= good
        print('  %-4s %-30s %s' % ('ok' if good else 'FAIL', 'lin-lo-' + op, (c_, m_)))
    hl = _lin(bk.mk('hi', bk.mk('multu', bk.mk('c', 36), xa)))
    good = len(hl[1]) == 1 and list(hl[1].values())[0][0].op == 'hi'
    ok &= good
    print('  %-4s %-30s HI stays an atom' % ('ok' if good else 'FAIL', 'lin-hi-atom'))
    # the affine solver against brute force (seeded, both directions)
    import random
    rnd = random.Random(7)
    bad = 0
    for _ in range(3000):
        k = rnd.choice([1, 2, 3, 15, 16, 24, -1, -15, 0x10000, 0x80000000,
                        rnd.randrange(1, 1 << 32)])
        lo = rnd.randrange(-300, 300)
        hi = lo + rnd.randrange(0, 300)
        c = rnd.choice([rnd.randrange(0, 1 << 32), rnd.randrange(-2000, 2000) & 0xFFFFFFFF])
        win = range(-rnd.choice([1, 2, 4]) + 1, rnd.choice([1, 2, 4]))
        bf = any(((k * a + c - w) & 0xFFFFFFFF) == 0 for a in range(lo, hi + 1) for w in win)
        bad += _affine_meets(k, c, lo, hi, win) != bf
    ok &= bad == 0
    print('  %-4s %-30s %d mismatches vs brute force' % ('ok' if not bad else 'FAIL',
                                                          'affine-solver', bad))
    # the function's own return kind: by name, else the file's only definition
    for name, text, want in (
            ('void-by-only-definition', 'void g(a) int a; { if (a) { return; } }\n', True),
            ('two-definitions-not-guessed', 'void g() { }\nint h() { return 0; }\n', False),
            ('void-by-name', 'void zz() { }\nint h() { return 0; }\n', True)):
        got = return_kind(text, 'zz')[0]
        good = got == want
        ok &= good
        print('  %-4s %-30s expect void=%s got %s' % ('ok' if good else 'FAIL', name, want, got))
    # path-precise indeterminate return (C' marks the value-less exits)
    for name, text, pl, nvl, sl, want in _noval_fixtures():
        r = _prove_lines(pl, sl, text, nvl)
        good = (r['verdict'] == 'PROVEN') == want
        ok &= good
        print('  %-4s %-36s expect %s got %s %s' % ('ok' if good else 'FAIL', name,
                                                   'PROVEN' if want else 'not PROVEN',
                                                   r['verdict'], r['msg'] or ''))
    for name, text, pl, sent, sl, want in _flow_fixtures():
        r = _prove_lines(pl, sl, text, sent=sent)
        good = (r['verdict'] == 'PROVEN') == want
        ok &= good
        print('  %-4s %-36s expect %s got %s %s' % ('ok' if good else 'FAIL', name,
                                                   'PROVEN' if want else 'not PROVEN',
                                                   r['verdict'], r['msg'] or ''))
    # the P-only return-value taint (per edge): sentinel constants, copies,
    # memory round trips and call results (with call returns)
    tcases = (
        ('taint-sentinel-copy', ['li t9,29993', 'beq a0,zero,L', 'nop', 'move v0,t9', 'L:',
                                 'jr ra', 'nop'], False, (0, 0), True),
        ('taint-other-const-clean', ['li t9,29993', 'beq a0,zero,L', 'nop',
                                     'addiu v0,t9,1', 'L:', 'jr ra', 'nop'], False, (0, 0), False),
        ('taint-through-memory', ['li t9,29993', 'sw t9,8(a1)', 'beq a0,zero,L', 'nop',
                                  'lw v0,8(a1)', 'L:', 'jr ra', 'nop'], False, (0, 0), True),
        ('taint-load-clean', ['beq a0,zero,L', 'nop', 'lw v0,8(a1)', 'L:', 'jr ra', 'nop'],
         False, (0, 0), False),
        ('taint-call-result', ['addiu sp,sp,-24', 'sw ra,20(sp)', 'beq a0,zero,L', 'nop',
                               'jal @k', 'nop', 'L:', 'lw ra,20(sp)', 'nop', 'jr ra',
                               'addiu sp,sp,24'], True, None, True),
        ('taint-void-call-clean', ['addiu sp,sp,-24', 'sw ra,20(sp)', 'beq a0,zero,L', 'nop',
                                   'jal @g', 'nop', 'L:', 'lw ra,20(sp)', 'nop', 'jr ra',
                                   'addiu sp,sp,24'], True, None, False))
    for name, lines, cr, edge, want in tcases:
        w_, r_ = asm(lines)
        f_ = Func('P', w_, r_, [], 0)
        f_.build_cfg()
        tn = valued_taint(f_, [29993], cr, {'g'}, {})
        if edge is None:
            # the fall-through edge out of the block holding the call
            jb = [b for b in range(len(f_.blocks))
                  if any(f_.ins[i].kind == 'jal' for i in range(*f_.blocks[b][:2]))][0]
            edge = (jb, 0)
        got = tn.get(edge, [None] * 34)[V0]
        if name.startswith(('taint-sentinel', 'taint-other', 'taint-through', 'taint-load')):
            fb = [b for b in range(len(f_.blocks)) if f_.blocks[b][2][0] == 'fall'][0]
            got = tn.get((fb, 0), [None] * 34)[V0]
        good = got == want
        ok &= good
        print('  %-4s %-36s expect %s got %s' % ('ok' if good else 'FAIL', name, want, got))
    for name, text, want in (
            ('kinds-literal-and-call', 'int f(int a) { if (a) return -1; return g(a, (1)); }',
             ([-1], True)),
            ('kinds-variable-refused', 'int f(int a) { if (a) return a; return 0; }', None),
            ('kinds-u8-range', 'u8 f(int a) { if (a) return 0x1FF; return 0; }',
             ([0x1FF, 0], False))):
        r = valued_return_kinds(text, 'f')
        got = None if r is None else ([x[2] for x in r[0]], r[1])
        good = got == want
        ok &= good
        print('  %-4s %-36s expect %s got %s' % ('ok' if good else 'FAIL', name, want, got))
    isn = instrument_sentinels('u8 f(int a) { if (a) return 0x1FF; return 0; g(91); }', 'f')
    good = bool(isn) and set(isn[1].values()) == {0xFF, 0} and \
        all(0 < k < 256 and k not in (91, 255) for k in isn[1])
    ok &= good
    print('  %-4s %-36s %r' % ('ok' if good else 'FAIL', 'instrument-sentinels-u8', isn))
    # by-value aggregate arguments: the call's argument WORDS are compared
    st_text = ('typedef struct { int a, b; } V;\nextern void g(int, int, V, V);\n'
               'void f(void) { }\n')
    stp = ['addiu sp,sp,-32', 'sw ra,28(sp)', 'li t6,1', 'sw t6,16(sp)', 'li t7,2',
           'sw t7,20(sp)', 'jal @g', 'nop', 'lw ra,28(sp)', 'nop', 'jr ra', 'addiu sp,sp,32']
    sts = _sub(stp, 'li t7,2', 'li t7,3')
    for name, aw, want in (('struct-arg-word-changed', {'g': 6}, False),
                           ('struct-arg-words-equal', {'g': 6}, True),
                           ('struct-arg-unmeasured', {'g': None}, False)):
        r = _prove_lines(stp, sts if 'changed' in name or 'unmeasured' in name else stp,
                         st_text, argwords=aw)
        good = (r['verdict'] == 'PROVEN') == want
        ok &= good
        print('  %-4s %-36s expect %s got %s %s' % ('ok' if good else 'FAIL', name,
                                                   'PROVEN' if want else 'not PROVEN',
                                                   r['verdict'], r['msg'] or ''))
    # the outgoing argument area built through a pointer (mode C object)
    oa = ['addiu sp,sp,-40', 'sw ra,36(sp)', 'move t3,sp', 'li t6,1', 'sw t6,16(t3)',
          'li t7,2', 'sw t7,20(t3)', 'jal @g', 'nop', 'lw ra,36(sp)', 'nop', 'jr ra',
          'addiu sp,sp,40']
    # mode B (no debug objects): a DYNAMIC frame load needs every frame byte
    # valid -- refused after a call clobbered its zone
    fxa = ['addiu sp,sp,-32', 'sw ra,28(sp)', 'sw s0,24(sp)', 'addiu t0,sp,16',
           'addu s0,t0,a0', 'jal @g', 'nop', 'lbu v0,0(s0)', 'lw ra,28(sp)', 'lw s0,24(sp)',
           'jr ra', 'addiu sp,sp,32']
    fxb = ['addiu sp,sp,-32', 'sw ra,28(sp)', 'sw s0,24(sp)', 'addiu t0,sp,16',
           'addu s0,t0,a0', 'lbu v0,0(s0)', 'jal @g', 'nop', 'lw ra,28(sp)', 'lw s0,24(sp)',
           'jr ra', 'addiu sp,sp,32']
    fxtext = 'extern void g(void);\nint f(int i) { char b[8]; return b[i]; }\n'
    for name, pl, want in (('modeB-dynamic-load-after-call', fxa, False),
                           ('modeB-dynamic-load-before-call', fxb, True)):
        sl = [x.replace('t0', 't2') for x in pl]
        r = _prove_lines(pl, sl, fxtext)
        good = (r['verdict'] == 'PROVEN') == want
        ok &= good
        print('  %-4s %-36s expect %s got %s %s %s' % ('ok' if good else 'FAIL', name,
                                                      'PROVEN' if want else 'not PROVEN',
                                                      r['verdict'], r['cls'] or '',
                                                      r['msg'] or ''))
    # an escaped local the debug info gives no address (IDO -g3 scRegister)
    ul = ['addiu sp,sp,-24', 'addiu a0,sp,16', 'sh a1,0(a0)', 'sh a2,2(a0)', 'lh v0,16(sp)',
          'lh v1,18(sp)', 'nop', 'addu v0,v0,v1', 'jr ra', 'addiu sp,sp,24']
    uls = ['addiu sp,sp,-24', 'addiu a0,sp,12', 'sh a1,0(a0)', 'sh a2,2(a0)', 'lh v0,12(sp)',
           'lh v1,14(sp)', 'nop', 'addu v0,v0,v1', 'jr ra', 'addiu sp,sp,24']
    ultext = 'int f(int a, int b) { short buf[2]; g(buf); return buf[0] + buf[1]; }\n'
    for name, pl, sl, text, aw, objs, want in (
            ('outgoing-area-object', oa, oa, st_text, {'g': 6}, [], True),
            ('outgoing-area-word-changed', oa, _sub(oa, 'li t7,2', 'li t7,5'), st_text,
             {'g': 6}, [], False),
            ('unlocated-local-remapped', ul, uls, ultext, None,
             [('buf', None, 4, 'agg')], True),
            ('unlocated-local-wrong-slot', ul, _sub(uls, 'lh v1,14(sp)', 'lh v1,16(sp)'), ultext,
             None, [('buf', None, 4, 'agg')], False),
            ('unlocated-scalar-not-candidate', ul, uls, ultext, None,
             [('buf', None, 4, 'scalar')], False)):
        r = _prove_lines(pl, sl, text, argwords=aw, objects=objs)
        good = (r['verdict'] == 'PROVEN') == want
        ok &= good
        print('  %-4s %-36s expect %s got %s %s' % ('ok' if good else 'FAIL', name,
                                                   'PROVEN' if want else 'not PROVEN',
                                                   r['verdict'], r['msg'] or ''))
    # a DYNAMIC frame address whose folded constant lies in no local
    # (`((char *)b + i)[j - 100]` -> `addu t0,sp,a0 ; lbu v0,-76(t0)`):
    # attributed to the merged candidate-local object
    da = ['addiu sp,sp,-40', 'sw zero,24(sp)', 'sw zero,28(sp)', 'addu t0,sp,a0',
          'lbu v0,-76(t0)', 'sw a1,32(sp)', 'lw v1,32(sp)', 'nop', 'addu v0,v0,v1', 'jr ra',
          'addiu sp,sp,40']
    das = [x.replace('t0', 't2') for x in da]
    datext = 'int f(int i, int a) { char b[8]; int x; return ((char *)b + i)[-100] + x; }\n'
    dob = [('b', -16, 8, 'agg'), ('x', -8, 4, 'scalar')]
    # ... with the register save slot between two candidate locals
    dsv = ['addiu sp,sp,-40', 'sw ra,20(sp)', 'sw zero,24(sp)', 'sw zero,16(sp)',
           'addu t0,sp,a0', 'lbu v0,-76(t0)', 'sw a1,32(sp)', 'lw v1,32(sp)', 'lw ra,20(sp)',
           'addu v0,v0,v1', 'jr ra', 'addiu sp,sp,40']
    # ... a non-candidate scalar slot remapped (mode B cannot: the layouts
    # differ)
    dar = _sub(_sub(das, 'sw a1,32(sp)', 'sw a1,36(sp)'), 'lw v1,32(sp)', 'lw v1,36(sp)')
    for name, pl, sl, objs, want in (
            ('area-dynamic-frame-load', da, das, dob, True),
            ('area-store-moved-past-dynamic-load', da,
             das[:2] + das[3:5] + das[2:3] + das[5:], dob, False),
            ('area-dynamic-load-offset-changed', da, _sub(das, 'lbu v0,-76(t2)',
                                                          'lbu v0,-72(t2)'), dob, False),
            ('area-cell-remapped', da, dar, dob, True),
            ('area-no-candidate-local', da, dar, [('b', -16, 8, 'scalar'),
                                                 ('x', -8, 4, 'scalar')], False),
            ('area-save-slot-in-hull', dsv,
             [x.replace('t0', 't2').replace('32(sp)', '36(sp)') for x in dsv],
             [('b', -16, 8, 'agg'), ('c', -24, 4, 'agg')], False)):
        r = _prove_lines(pl, sl, datext, objects=objs)
        good = (r['verdict'] == 'PROVEN') == want
        ok &= good
        print('  %-4s %-36s expect %s got %s %s' % ('ok' if good else 'FAIL', name,
                                                   'PROVEN' if want else 'not PROVEN',
                                                   r['verdict'], r['msg'] or ''))
    try:
        got = aggregate_arg_words(st_text, ['g'])
    except Exception:                                # noqa: BLE001
        got = None
    if not got:
        print('  skip aggregate-arg-words (no IDO compile)')
    else:
        good = got == {'g': 6}
        ok &= good
        print('  %-4s %-36s expect %s got %s' % ('ok' if good else 'FAIL',
                                                'aggregate-arg-words', {'g': 6}, got))
    got = aggregate_arg_words('typedef struct { short a; double d; } D;\n'
                              'extern void h(s32, D);\n', ['h'])
    good = got == {'h': 6} or not got
    ok &= good
    print('  %-4s %-36s expect %s got %s' % ('ok' if good else 'FAIL',
                                            'aggregate-arg-words-align8', {'h': 6}, got))
    # the C-side analysis and the instrumentation
    for name, text, want in (
            ('sites-fall-off+valued', 'int f(int a) { if (a) return 5; g(); }', (1, 1)),
            ('sites-bare+valued', 'int f(int a) { if (a) return; return (a); }', (1, 1)),
            ('sites-no-valueless', 'int f(int a) { if (a) return 1; return 2; }', (0, 2)),
            ('sites-comment-ignored', 'int f(int a) { /* return 1; */ g(); }', (1, 0)),
            ('sites-macro-refused', '#define R(x) return x\nint f(int a) { R(a); }', None)):
        got = return_sites(text, 'f')
        good = got == want
        ok &= good
        print('  %-4s %-36s expect %s got %s' % ('ok' if good else 'FAIL', name, want, got))
    ic = instrument_valueless('int f(int a)\n{\n if (a) return;\n if (a > 1) return a;\n'
                              ' g();\n}\n', 'f') or ''
    good = (ic.count('\n') == 6 and ic.count('&' + NOVAL_SYM) == 2 and
            'return a;' in ic and 'extern char ' + NOVAL_SYM in ic)
    ok &= good
    print('  %-4s %-36s %r' % ('ok' if good else 'FAIL', 'instrument-valueless', ic))
    for name, pl, sl in yes:
        r = _prove_lines(pl, sl)
        good = r['verdict'] == 'PROVEN'
        ok &= good
        print('  %-4s %-30s expect PROVEN     got %-8s %s %s' % (
            'ok' if good else 'FAIL', name, r['verdict'], r['cls'] or '', r['msg']))
    for name, pl, sl in no:
        r = _prove_lines(pl, sl)
        good = r['verdict'] != 'PROVEN'
        ok &= good
        print('  %-4s %-30s expect not PROVEN got %-8s %s %s' % (
            'ok' if good else 'FAIL', name, r['verdict'], r['cls'] or '', r['msg']))
    return ok


if __name__ == '__main__':
    sys.exit(main())
