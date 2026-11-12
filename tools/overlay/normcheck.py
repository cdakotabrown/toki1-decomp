#!/usr/bin/env python3
"""normcheck.py -- audit a `scheduler_normalizer` PostProc directive for
SEMANTIC corruption of the compiled object.

WHY
    scheduler_normalizer rewrites a compiled object's instruction order to
    approximate IDO's scheduling.  It is a REORDERING tool: the only change it
    may legitimately make is a permutation of instructions plus the
    branch-displacement repairs that permutation forces.  Batches 18 and 22 found
    objects where it did more than that -- def-use clobbers, stores dragged
    across the branch that guarded them, reloc'd same-opcode loads permuted
    into each other's slots (reading the WRONG symbol at 100% masked), and
    branch words rotated with stale displacements (a `b -6` infinite loop where
    the target has `b +5`).  The masked overlay compare and semcheck are blind
    to every one of these.

HOW
    The function is compiled once and the PostProc chain is then applied three
    ways, giving three objects:

        B   chain up to (not including) the normalizer  -- the COMPILER's own
            output, i.e. what the C source actually means
        A   B + the normalizer step alone               -- isolates the
            normalizer even when a later patch_insn_reg_field depends on it
        F   the whole chain                             -- what actually SHIPS

    THE VERDICT IS DECIDED ON F, NOT ON A.  This matters: a normalizer is
    almost always followed by a gen_reg_patches paint chain generated against
    the normalized layout, and those paints routinely repair the normalizer's
    intermediate state.  Judging A alone reports ~15% of functions broken that
    ship perfectly good bytes.  F is scored with ABSOLUTE semantic metrics
    (see S: below) computed on B and on F; any metric that gets WORSE is
    corruption that PostProc introduced.  The metrics need no instruction
    matching, so they survive register painting and reordering.

    The comparative A-vs-B checks (C1..C6) still run, and are reported as
    INFO: they say WHAT the normalizer did, which is what you need once a
    shipped object is known bad.

    Note the natural corollary: a function whose resolved verdict is
    BYTE-PERFECT cannot be semantically broken -- its bytes ARE the game's.
    Corruption can only hide in semis, which is exactly where this tool
    found it.

VERDICTS
    INERT   A == B (bytes and relocations).  The directive is a NO-OP and can
            never affect the shipped object -> delete it as dead weight.
    BROKEN  the shipped object is semantically WORSE than the compiler's own
            output.  Drop the normalizer (and the paints generated against
            it) and let the score fall honestly -- a lower honest number beats
            a misleading higher one -- or replace it with explicit
            swap_instructions, which carry their relocations.
    LEGIT   the normalizer really reordered and the shipped object is no worse
            than the compiler's output.
    WARN    only soft findings; review by hand.

SHIPPED-OBJECT METRICS (a worse count on F than on B => BROKEN)
    S:undef_reads     a register that IS defined earlier in the function is
                      read on a path where that definition does not reach --
                      a clobbered value / garbage address
    S:hidden_param_reads
                      a register nothing in the function defines is read: a
                      hidden register parameter threaded in by the caller.
                      Reported separately because this codebase is full of
                      them and the TARGET does the same read -- treat a rise
                      here as "review", not as proof of corruption
    S:hilo_mismatch   the address a %lo() instruction actually computes is not
                      the address of the symbol it names.  (A %hi/%lo pair
                      naming DIFFERENT symbols is NORMAL -- IDO CSEs one lui
                      across every global in the same 64K page -- so this is
                      checked by real address arithmetic, not by name.)
    S:branch_oob      a branch targets outside the body
    S:branch_in_slot  a branch/jump sits in another branch's delay slot
    S:self_loop       a branch to itself or its own delay slot
    S:hi_no_lo        a %hi with no %lo after it anywhere in the reloc stream

NORMALIZER-ATTRIBUTION CHECKS (reported as INFO)
    C1 MULTISET  every pre-normalizer instruction has exactly one counterpart
                 in the normalized object, keyed by (word, relocation), with
                 branch/jump displacements canonicalised away.  Catches
                 created / destroyed / MUTATED instructions.
    C2 BRANCH    every branch still targets the SAME LOGICAL INSTRUCTION: the
                 pre-normalizer target index, mapped through the permutation,
                 must equal the post-normalizer target index.  Catches a moved
                 branch whose displacement was never repaired, and
                 out-of-range targets.
    C3 DEFUSE    RAW / WAR / WAW register dependences and may-alias memory
                 dependences inside a basic block must keep their order.
                 Aliasing is decided with relocation symbols plus a trace of
                 which named global each base register points into, so the
                 struct-copy loops IDO emits everywhere do not drown the
                 output in false positives.
    C4 GUARD     no memory op may be moved across a control-transfer
                 instruction.  A STORE hoisted above its guard writes on a
                 path that excluded it; a LOAD hoisted above its guard
                 clobbers its destination on the other path (weighted by
                 backward liveness on the normalized object).
    C5 RELOC     relocations must travel WITH their instruction: same type,
                 same symbol and addend.
    C6 DELAY     no branch/jump in a branch delay slot, and no branch left as
                 the last word of the body.
                 (NOTE: a delay slot that DEFINES a register the branch reads
                 is the legitimate IDO switch-cascade idiom -- the branch
                 evaluates its condition before the slot executes -- so it is
                 deliberately NOT flagged.  If the normalizer created it by
                 sinking the def, C3 catches it as an inverted RAW.)

MODELLING NOTES (each of these was a real false-positive source; do not
"simplify" them away)
    * a jal's delay slot executes BEFORE control transfers, so the
      caller-saved clobber belongs to the DELAY SLOT, not to the jal;
    * `b LBL` is `beq $zero,$zero,LBL` and is always taken -- the word after
      its delay slot is NOT a fallthrough successor;
    * the analysis window is exactly the compare window; decoding the
      alignment pad past the body invents instructions.

USAGE
    python3 tools/overlay/normcheck.py EVENT func_80133040 [-v]
    python3 tools/overlay/normcheck.py EVENT --all [--json out.json]
"""
import argparse
import json
import os
import re
import struct
import subprocess
import sys
import tempfile
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent
# NB: `config/overlay` under a tool directory would also match, so the repo root
# must be identified by BOTH markers or the walk stops one level too low.
while ROOT != ROOT.parent and not ((ROOT / "config" / "overlay").is_dir()
                                   and (ROOT / "src" / "overlay").is_dir()):
    ROOT = ROOT.parent
sys.path.insert(0, str(ROOT / "tools" / "build"))
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

from parse_postproc import (parse_compile_flags, parse_compile_env,
                            parse_symbol_start_text, parse_symbol_size_text)
from overlay_postproc import parse_postproc, apply_postproc
from ido_flags import base_flags, pic_flags

INCLUDE_DIR = ROOT / "include"
IDO_CC = ROOT / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
IDO_FLAGS = base_flags([INCLUDE_DIR, f"{INCLUDE_DIR}/psyq"])
OBJDUMP = "mipsel-linux-gnu-objdump"
OBJCOPY = "mipsel-linux-gnu-objcopy"


# ───────────────────────────── MIPS I decode ─────────────────────────────

OP_SPECIAL, OP_REGIMM, OP_COP1 = 0x00, 0x01, 0x11
BRANCH_OPS = {0x04, 0x05, 0x06, 0x07, 0x14, 0x15, 0x16, 0x17}
JUMP_OPS = {0x02, 0x03}                       # j / jal
LOAD_OPS = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x30, 0x31, 0x35}
STORE_OPS = {0x28, 0x29, 0x2A, 0x2B, 0x2E, 0x38, 0x39, 0x3D}
ITYPE_WRT = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}
REGIMM_BRANCH_RT = {0x00, 0x01, 0x02, 0x03, 0x10, 0x11, 0x12, 0x13}
MEM_WIDTH = {0x20: 1, 0x24: 1, 0x21: 2, 0x25: 2, 0x23: 4, 0x22: 4, 0x26: 4,
             0x30: 4, 0x31: 4, 0x35: 8, 0x28: 1, 0x29: 2, 0x2B: 4, 0x2A: 4,
             0x2E: 4, 0x38: 4, 0x39: 4, 0x3D: 8}

REGNAMES = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
            't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
            's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
            't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra']
R_SP, R_FP, R_RA = 29, 30, 31
HI, LO, FP_BASE = 32, 33, 64
CALLER_SAVED = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25,
                HI, LO}
EXIT_LIVE = {2, 3, 16, 17, 18, 19, 20, 21, 22, 23, 28, 29, 30, 31}


def sign16(v):
    return v - 0x10000 if v & 0x8000 else v


class Insn(object):
    __slots__ = ('word', 'idx', 'op', 'rs', 'rt', 'rd', 'funct', 'imm',
                 'is_branch', 'is_jump', 'is_jr', 'is_call', 'is_load',
                 'is_store', 'target', 'defs', 'uses', 'base', 'off', 'width')

    def __init__(self, word, idx):
        self.word, self.idx = word, idx
        self.op = (word >> 26) & 0x3F
        self.rs = (word >> 21) & 0x1F
        self.rt = (word >> 16) & 0x1F
        self.rd = (word >> 11) & 0x1F
        self.funct = word & 0x3F
        self.imm = word & 0xFFFF
        self.is_branch = self.is_jump = self.is_jr = self.is_call = False
        self.is_load = self.is_store = False
        self.target = self.base = self.off = None
        self.width = 0
        d, u = set(), set()
        op = self.op
        if op == OP_SPECIAL:
            f = self.funct
            if f in (0x08, 0x09):                       # jr / jalr
                self.is_jr = True
                u.add(self.rs)
                if f == 0x09:
                    self.is_call = True
                    d.add(self.rd)
            elif f in (0x00, 0x02, 0x03):                # sll srl sra
                d.add(self.rd); u.add(self.rt)
            elif f in (0x04, 0x06, 0x07):                # sllv srlv srav
                d.add(self.rd); u.add(self.rt); u.add(self.rs)
            elif f in (0x10, 0x12):                      # mfhi / mflo
                d.add(self.rd); u.add(HI if f == 0x10 else LO)
            elif f in (0x11, 0x13):                      # mthi / mtlo
                d.add(HI if f == 0x11 else LO); u.add(self.rs)
            elif f in (0x18, 0x19, 0x1A, 0x1B):          # mult multu div divu
                d.add(HI); d.add(LO); u.add(self.rs); u.add(self.rt)
            elif f in (0x0C, 0x0D):                      # syscall / break
                pass
            else:
                d.add(self.rd); u.add(self.rs); u.add(self.rt)
        elif op == OP_REGIMM:
            if self.rt in REGIMM_BRANCH_RT:
                self.is_branch = True
                u.add(self.rs)
                if self.rt in (0x10, 0x11, 0x12, 0x13):
                    d.add(R_RA)
        elif op in JUMP_OPS:
            self.is_jump = True
            if op == 0x03:                               # jal
                self.is_call = True
                d.add(R_RA)
        elif op in BRANCH_OPS:
            self.is_branch = True
            u.add(self.rs)
            if op in (0x04, 0x05, 0x14, 0x15):
                u.add(self.rt)
        elif op == OP_COP1:
            fmt = self.rs
            if fmt == 0x00:                              # mfc1
                d.add(self.rt); u.add(FP_BASE + self.rd)
            elif fmt == 0x04:                            # mtc1
                u.add(self.rt); d.add(FP_BASE + self.rd)
            elif fmt == 0x08:                            # bc1t / bc1f
                self.is_branch = True
            else:
                d.add(FP_BASE + ((word >> 6) & 0x1F))
                u.add(FP_BASE + self.rd); u.add(FP_BASE + self.rt)
        elif op in LOAD_OPS:
            self.is_load = True
            u.add(self.rs)
            self.base, self.off = self.rs, sign16(self.imm)
            self.width = MEM_WIDTH.get(op, 4)
            if op in (0x31, 0x35):                       # lwc1 / ldc1
                d.add(FP_BASE + self.rt)
            else:
                d.add(self.rt)
            if op in (0x22, 0x26):                       # lwl / lwr
                u.add(self.rt)
        elif op in STORE_OPS:
            self.is_store = True
            u.add(self.rs); u.add(self.rt)
            self.base, self.off = self.rs, sign16(self.imm)
            self.width = MEM_WIDTH.get(op, 4)
            if op in (0x39, 0x3D):                       # swc1 / sdc1
                u.discard(self.rt); u.add(FP_BASE + self.rt)
        elif op == 0x0F:                                 # lui
            d.add(self.rt)
        elif op in ITYPE_WRT:
            d.add(self.rt); u.add(self.rs)
        self.defs, self.uses = d - {0}, u - {0}
        if self.is_branch:
            self.target = idx + 1 + sign16(self.imm)

    def is_ctrl(self):
        return self.is_branch or self.is_jump or self.is_jr

    def text(self):
        return disasm_word(self.word, self.idx)


_CS = None


def disasm_word(word, idx=0):
    global _CS
    if _CS is None:
        try:
            import capstone
            _CS = capstone.Cs(capstone.CS_ARCH_MIPS,
                              capstone.CS_MODE_MIPS32 |
                              capstone.CS_MODE_LITTLE_ENDIAN)
        except Exception:
            _CS = False
    if _CS:
        for i in _CS.disasm(struct.pack('<I', word), idx * 4):
            return ("%s %s" % (i.mnemonic, i.op_str)).strip()
    return "0x%08x" % word


def regname(r):
    if r == HI:
        return 'hi'
    if r == LO:
        return 'lo'
    if r >= FP_BASE:
        return 'f%d' % (r - FP_BASE)
    return REGNAMES[r] if r < 32 else '?%d' % r


# ───────────────────────────── object plumbing ─────────────────────────────

def sh(cmd):
    return subprocess.run(cmd, capture_output=True, text=True)


def parse_sym_file(overlay):
    out = {}
    for line in (ROOT / "config" / "overlay" /
                 ("%s.sym" % overlay)).read_text().splitlines():
        line = line.split('#')[0].strip()
        parts = line.split(':')
        if len(parts) < 3:
            continue
        try:
            out[parts[2].strip()] = (int(parts[0], 16), int(parts[1], 0))
        except ValueError:
            pass
    return out


def sym_window(obj, func):
    for line in sh([OBJDUMP, "-t", str(obj)]).stdout.splitlines():
        parts = line.split()
        if len(parts) >= 6 and parts[-1] == func and '.text' in line:
            try:
                return int(parts[0], 16), int(parts[-2], 16)
            except ValueError:
                pass
    return None, None


def text_bytes(obj):
    return subprocess.run([OBJCOPY, "-O", "binary", "-j", ".text",
                           str(obj), "/dev/stdout"],
                          capture_output=True).stdout


def relocs(obj):
    """{text_offset: (type, 'symbol+addend')} for .text relocations."""
    out, in_text = {}, False
    for line in sh([OBJDUMP, "-r", str(obj)]).stdout.splitlines():
        if line.startswith("RELOCATION RECORDS FOR"):
            in_text = ".text" in line
            continue
        if not in_text:
            continue
        m = re.match(r'([0-9a-f]+)\s+(\S+)\s+(.+?)\s*$', line)
        if m:
            out[int(m.group(1), 16)] = (m.group(2), m.group(3).strip())
    return out


def compile_obj(src, out_obj, tmpdir):
    env = os.environ.copy()
    env["TMPDIR"] = str(tmpdir)
    cenv = parse_compile_env(src)
    if cenv:
        env.update(cenv)
    _cflags = list(parse_compile_flags(src))
    cmd = ([str(IDO_CC)] + IDO_FLAGS
           + pic_flags(src, compile_flags=_cflags) + _cflags +
           ["-c", str(src), "-o", str(out_obj)])
    r = subprocess.run(cmd, capture_output=True, text=True, timeout=300,
                       env=env, cwd=str(tmpdir))
    if r.returncode != 0:
        raise RuntimeError("compile failed: %s" % r.stderr[-300:])


# ───────────────────────────── analysis ─────────────────────────────

def basic_blocks(insns):
    n = len(insns)
    leaders = {0}
    for i, ins in enumerate(insns):
        if ins.is_branch:
            if ins.target is not None and 0 <= ins.target < n:
                leaders.add(ins.target)
            if i + 2 < n:
                leaders.add(i + 2)
        elif ins.is_jump or ins.is_jr:
            if i + 2 < n:
                leaders.add(i + 2)
    bid, cur = [0] * n, -1
    for i in range(n):
        if i in leaders:
            cur += 1
        bid[i] = cur
    return bid


def successors(insns, j):
    """Instruction-level CFG successors (delay slots modelled explicitly)."""
    n = len(insns)
    ins = insns[j]
    if ins.is_ctrl():
        return [j + 1] if j + 1 < n else []
    if j > 0 and insns[j - 1].is_ctrl():          # j is a delay slot
        b = insns[j - 1]
        if b.is_branch:
            s = []
            # `b LBL` is `beq $zero,$zero,LBL` -- always taken, so the word
            # after its delay slot is NOT a fallthrough successor.  Treating
            # it as one invents a path along which registers look undefined.
            uncond = (b.op == 0x04 and b.rs == b.rt)
            if j + 1 < n and not uncond:
                s.append(j + 1)
            if b.target is not None and 0 <= b.target < n:
                s.append(b.target)
            return s
        if b.is_call or b.op == 0x03:             # jal/jalr: returns
            return [j + 1] if j + 1 < n else []
        return []                                 # jr / j : leaves the body
    return [j + 1] if j + 1 < n else []


def clobbers_here(insns, j):
    """True if the call-clobber of caller-saved registers takes effect AFTER
    instruction j.  A jal's delay slot executes BEFORE control transfers, so
    the clobber belongs to the delay slot, not to the jal itself."""
    return j > 0 and insns[j - 1].is_call


def liveness(insns):
    """Backward liveness. live[j] = regs live BEFORE insn j."""
    n = len(insns)
    live = [set() for _ in range(n)]
    succ = [successors(insns, j) for j in range(n)]
    for _ in range(200):
        changed = False
        for j in range(n - 1, -1, -1):
            ins = insns[j]
            out = set()
            if not succ[j]:
                out = set(EXIT_LIVE)
            for s in succ[j]:
                out |= live[s]
            defs = set(ins.defs)
            if clobbers_here(insns, j):
                defs |= CALLER_SAVED
            new = (out - defs) | ins.uses
            if new != live[j]:
                live[j] = new
                changed = True
        if not changed:
            break
    return live


def symbase(r):
    return r[1].split('+')[0] if r else None


def trace_base_objects(insns, rel):
    """{mem-op index: base object symbol or None}.

    Linear forward propagation of "which named global does this register point
    into", following lui %hi / addiu %lo / addiu reg,K / addu chains.  It lets
    the alias test tell a walking pointer into one global from a walking
    pointer into another (the struct-copy loops IDO emits everywhere)."""
    cur, out = {}, {}
    for i, ins in enumerate(insns):
        if ins.is_load or ins.is_store:
            out[i] = cur.get(ins.base)
        r = rel.get(i)
        if clobbers_here(insns, i):
            for x in list(cur):
                if x in CALLER_SAVED:
                    del cur[x]
        if ins.op == 0x0F:                               # lui
            cur[ins.rt] = symbase(r) if r and r[0] == 'R_MIPS_HI16' else None
        elif ins.op == 0x09:                             # addiu
            cur[ins.rt] = (symbase(r) if r and r[0] == 'R_MIPS_LO16'
                           else cur.get(ins.rs))
        elif ins.op == OP_SPECIAL and ins.funct in (0x20, 0x21):   # add/addu
            cur[ins.rd] = cur.get(ins.rs) or cur.get(ins.rt)
        else:
            for d in ins.defs:
                cur.pop(d, None)
    return out


def addr_class(ins, rel, obj):
    """Symbolic address of a memory op.
    ('sym', 'D_80120A74+0x4')  exact %lo access to a named global
    ('sp',  offset)            stack slot
    ('obj', 'D_80120A74', off) walking pointer into a traced global
    ('reg', base, offset)      unknown
    """
    if rel and rel[0] in ('R_MIPS_LO16', 'R_MIPS_GPREL16'):
        return ('sym', rel[1])
    if ins.base in (R_SP, R_FP):
        return ('sp', ins.off)
    if obj:
        return ('obj', obj, ins.off)
    return ('reg', ins.base, ins.off)


def alias_verdict(a, b, wa, wb):
    """'yes' (may alias -> hard), 'maybe' (different traced globals -> review),
    'no' (provably disjoint)."""
    ka, kb = a[0], b[0]
    if ka == 'sp' and kb == 'sp':
        return 'no' if (a[1] + wa <= b[1] or b[1] + wb <= a[1]) else 'yes'
    if 'sp' in (ka, kb) and {ka, kb} & {'sym', 'obj'}:
        return 'no'                       # a stack slot and a global
    if ka == 'sym' and kb == 'sym':
        return 'yes' if a[1] == b[1] else 'maybe'
    if ka in ('sym', 'obj') and kb in ('sym', 'obj'):
        na = a[1].split('+')[0]
        nb = b[1].split('+')[0]
        return 'yes' if na == nb else 'maybe'
    if ka == 'reg' and kb == 'reg' and a[1] == b[1]:
        return 'no' if (a[2] + wa <= b[2] or b[2] + wb <= a[2]) else 'yes'
    return 'yes'


def canon(ins):
    w = ins.word
    if ins.is_branch:
        w &= 0xFFFF0000
    if ins.is_jump:
        w &= 0xFC000000
    return w


def build_permutation(B, A, rel_b, rel_a, F):
    """pi[i in B] = j in A.  Two-tier: pair on (word, reloc) first, then on
    word alone so a moved-but-mis-relocated word is reported as C5 rather than
    vanishing into C1."""
    buckets = defaultdict(list)
    for j, ins in enumerate(A):
        buckets[(canon(ins), rel_a.get(j))].append(j)
    cursor, pi, left = defaultdict(int), {}, []
    for i, ins in enumerate(B):
        k = (canon(ins), rel_b.get(i))
        lst, c = buckets.get(k, []), cursor[(canon(ins), rel_b.get(i))]
        if c < len(lst):
            pi[i] = lst[c]
            cursor[k] = c + 1
        else:
            left.append(i)
    used = set(pi.values())
    free = defaultdict(list)
    for j, ins in enumerate(A):
        if j not in used:
            free[canon(ins)].append(j)
    still = []
    for i in left:
        lst = free.get(canon(B[i]))
        if lst:
            j = lst.pop(0)
            pi[i] = j
            rb, ra = rel_b.get(i), rel_a.get(j)
            if rb and not ra:
                F.append(dict(code='C5', sev='BROKEN', idx=i,
                              msg='relocation %s %s on insn #%d `%s` was LOST '
                                  'when it moved to #%d -- that word will link '
                                  'against nothing'
                                  % (rb[0], rb[1], i, B[i].text(), j)))
            elif ra and not rb:
                F.append(dict(code='C5', sev='BROKEN', idx=i,
                              msg='insn #%d `%s` GAINED relocation %s %s at #%d'
                                  % (i, B[i].text(), ra[0], ra[1], j)))
            else:
                F.append(dict(code='C5', sev='BROKEN', idx=i,
                              msg='insn #%d `%s` carried relocation %s %s but '
                                  'at its new position #%d the word is '
                                  'relocated %s %s -- it now references the '
                                  'WRONG SYMBOL (invisible to the masked '
                                  'compare)'
                                  % (i, B[i].text(), rb[0], rb[1], j,
                                     ra[0], ra[1])))
        else:
            still.append(i)
    surplus = [j for j in range(len(A)) if j not in set(pi.values())]
    return pi, still, surplus


def check(B, A, rel_b, rel_a):
    F = []
    nb, na = len(B), len(A)
    if nb != na:
        F.append(dict(code='C1', sev='BROKEN',
                      msg='instruction COUNT changed %d -> %d; the normalizer '
                          'must be a pure permutation' % (nb, na)))
    pi, missing, surplus = build_permutation(B, A, rel_b, rel_a, F)

    for i in missing[:10]:
        F.append(dict(code='C1', sev='BROKEN', idx=i,
                      msg='pre-normalizer insn #%d `%s` has NO counterpart in '
                          'the normalized object -- an instruction was '
                          'created, destroyed or MUTATED'
                          % (i, B[i].text())))
    for j in surplus[:10]:
        F.append(dict(code='C1', sev='BROKEN', idx=j,
                      msg='normalized insn #%d `%s` appeared from nowhere'
                          % (j, A[j].text())))

    # ── C2 branch targets ────────────────────────────────────────────
    for i, ins in enumerate(B):
        if not ins.is_branch or i not in pi:
            continue
        j, aj = pi[i], A[pi[i]]
        if not aj.is_branch:
            F.append(dict(code='C2', sev='BROKEN', idx=i,
                          msg='branch #%d became non-branch at #%d' % (i, j)))
            continue
        if aj.target is None or not (0 <= aj.target < na):
            F.append(dict(code='C2', sev='BROKEN', idx=i,
                          msg='branch #%d -> #%d `%s` now targets word %d, OUT '
                              'OF RANGE (body is 0..%d)'
                              % (i, j, aj.text(), aj.target, na - 1)))
            continue
        if ins.target is None or not (0 <= ins.target < nb):
            continue
        want = pi.get(ins.target)
        if want is not None and want != aj.target:
            F.append(dict(code='C2', sev='BROKEN', idx=i,
                          msg='branch moved #%d -> #%d but its displacement '
                              'was NOT repaired: it targeted word %d (`%s`), '
                              'which is now word %d, yet the branch points at '
                              'word %d (`%s`) -- displacement %+d, should be '
                              '%+d'
                              % (i, j, ins.target, B[ins.target].text(), want,
                                 aj.target, A[aj.target].text(),
                                 aj.target - (j + 1), want - (j + 1))))

    bbB = basic_blocks(B)

    def keeps_order(i, j):
        return (i not in pi) or (j not in pi) or pi[i] < pi[j]

    # ── C3 register dependences inside a basic block ────────────────
    lastdef, lastuse, cur = {}, {}, -1
    for j, ins in enumerate(B):
        if bbB[j] != cur:
            cur, lastdef, lastuse = bbB[j], {}, {}
        for r in ins.uses:
            i = lastdef.get(r)
            if i is not None and not keeps_order(i, j):
                F.append(dict(code='C3', sev='BROKEN', idx=j,
                              msg='RAW INVERTED: #%d `%s` defines $%s and #%d '
                                  '`%s` reads it; normalized they sit at #%d / '
                                  '#%d -- the use now PRECEDES its def'
                                  % (i, B[i].text(), regname(r), j, ins.text(),
                                     pi[i], pi[j])))
        for r in ins.defs:
            i = lastuse.get(r)
            if i is not None and not keeps_order(i, j):
                F.append(dict(code='C3', sev='BROKEN', idx=j,
                              msg='WAR INVERTED: #%d `%s` reads $%s which #%d '
                                  '`%s` overwrites; normalized at #%d / #%d -- '
                                  'the read now sees the NEW value'
                                  % (i, B[i].text(), regname(r), j, ins.text(),
                                     pi[i], pi[j])))
            i = lastdef.get(r)
            if i is not None and not keeps_order(i, j):
                F.append(dict(code='C3', sev='BROKEN', idx=j,
                              msg='WAW INVERTED: #%d `%s` and #%d `%s` both '
                                  'define $%s; normalized at #%d / #%d -- the '
                                  'surviving value is the WRONG one'
                                  % (i, B[i].text(), j, ins.text(), regname(r),
                                     pi[i], pi[j])))
        for r in ins.uses:
            lastuse[r] = j
        for r in ins.defs:
            lastdef[r] = j

    # ── C3 memory dependences inside a basic block ──────────────────
    objs = trace_base_objects(B, rel_b)
    mem = [j for j in range(nb) if B[j].is_load or B[j].is_store]
    for x in range(len(mem)):
        ja = mem[x]
        ca = addr_class(B[ja], rel_b.get(ja), objs.get(ja))
        for y in range(x + 1, len(mem)):
            jb = mem[y]
            if bbB[jb] != bbB[ja]:
                break
            if not (B[ja].is_store or B[jb].is_store):
                continue
            cb = addr_class(B[jb], rel_b.get(jb), objs.get(jb))
            v = alias_verdict(ca, cb, B[ja].width, B[jb].width)
            if v == 'no' or keeps_order(ja, jb):
                continue
            F.append(dict(code='C3', sev='BROKEN' if v == 'yes' else 'WARN',
                          idx=jb,
                          msg='MEMORY dependence INVERTED: #%d `%s` [%s] and '
                              '#%d `%s` [%s] %s but are now at #%d / #%d'
                              % (ja, B[ja].text(), ca, jb, B[jb].text(), cb,
                                 'MAY ALIAS' if v == 'yes' else
                                 'point into DIFFERENT globals (review: '
                                 'adjacent objects can still overlap under a '
                                 'walking pointer)', pi[ja], pi[jb])))

    # ── C4 memory ops moved across a control transfer ───────────────
    liveA = liveness(A)
    ctrl = [i for i in range(nb) if B[i].is_ctrl()]
    crossed_other = 0
    for i, ins in enumerate(B):
        if i not in pi:
            continue
        j = pi[i]
        crossings = []
        for c in ctrl:
            if c not in pi:
                continue
            jc = pi[c]
            # ignore motion into/out of the branch's own delay slot
            before_b = i < c
            after_b = i > c + 1
            nbefore = j < jc
            nafter = j > jc + 1
            if before_b and nafter:
                crossings.append((c, 'sank below'))
            elif after_b and nbefore:
                crossings.append((c, 'hoisted above'))
        if not crossings:
            continue
        if not (ins.is_load or ins.is_store):
            crossed_other += 1
            continue
        c, how = crossings[0]
        if ins.is_store:
            F.append(dict(code='C4', sev='BROKEN', idx=i,
                          msg='STORE #%d `%s` was %s the control transfer #%d '
                              '`%s` (now #%d vs #%d) -- it writes memory on a '
                              'path the guard excluded'
                              % (i, ins.text(), how, c, B[c].text(), pi[i],
                                 pi[c])))
        else:
            dst = sorted(ins.defs)
            r = dst[0] if dst else None
            lv = r is not None and r in liveA[min(pi[c] + 2, na - 1)]
            F.append(dict(code='C4', sev='BROKEN' if lv else 'WARN', idx=i,
                          msg='LOAD #%d `%s` was %s the control transfer #%d '
                              '`%s` (now #%d vs #%d); its destination $%s is '
                              '%s on the other path'
                              % (i, ins.text(), how, c, B[c].text(), pi[i],
                                 pi[c], regname(r) if r is not None else '?',
                                 'LIVE -- the clobber is observable' if lv
                                 else 'dead there (speculation is harmless)')))
    if crossed_other:
        F.append(dict(code='C4', sev='INFO',
                      msg='%d non-memory instructions crossed a control '
                          'transfer (IDO itself hoists constants across '
                          'blocks; not an error by itself)' % crossed_other))

    # ── C5 relocation integrity ─────────────────────────────────────
    for i, rb in rel_b.items():
        j = pi.get(i)
        if j is None:
            continue
        ra = rel_a.get(j)
        if ra is None:
            F.append(dict(code='C5', sev='BROKEN', idx=i,
                          msg='relocation %s %s on insn #%d was LOST at its '
                              'new position #%d' % (rb[0], rb[1], i, j)))
        elif ra != rb:
            F.append(dict(code='C5', sev='BROKEN', idx=i,
                          msg='insn #%d relocation changed %s %s -> %s %s at '
                              '#%d: the word references the WRONG SYMBOL'
                              % (i, rb[0], rb[1], ra[0], ra[1], j)))
    # A HI16 with no LO16 for the same symbol anywhere after it is a hard
    # link error ("R_MIPS_HI16 without matching R_MIPS_LO16") -- the linker
    # needs the following LO16's addend to compute the %hi carry.  The
    # converse (a LO16 with no preceding HI16) is LEGAL: IDO reuses an
    # already-loaded %hi register.  A genuinely broken pair shows up as a C3
    # RAW inversion on the base register, so it is not double-reported here.
    lo_after = defaultdict(list)
    for j in sorted(rel_a):
        if rel_a[j][0] == 'R_MIPS_LO16':
            lo_after[rel_a[j][1]].append(j)
    for j in sorted(rel_a):
        typ, sym = rel_a[j]
        if typ == 'R_MIPS_HI16' and not any(k > j for k in lo_after.get(sym, [])):
            F.append(dict(code='C5', sev='BROKEN', idx=j,
                          msg='HI16 for %s at #%d has NO LO16 after it -- the '
                              'normalizer sank the %%lo above its %%hi; ld '
                              'cannot compute the carry' % (sym, j)))

    # ── C6 delay-slot structure ─────────────────────────────────────
    for j, ins in enumerate(A):
        if not ins.is_ctrl():
            continue
        if j + 1 >= na:
            F.append(dict(code='C6', sev='BROKEN', idx=j,
                          msg='control transfer #%d `%s` is the LAST word of '
                              'the body: no delay slot' % (j, ins.text())))
            continue
        d = A[j + 1]
        if d.is_ctrl():
            F.append(dict(code='C6', sev='BROKEN', idx=j,
                          msg='#%d `%s` has a BRANCH in its delay slot (#%d '
                              '`%s`)' % (j, ins.text(), j + 1, d.text())))

    hard = [f for f in F if f['sev'] == 'BROKEN']
    soft = [f for f in F if f['sev'] == 'WARN']
    return ('BROKEN' if hard else ('WARN' if soft else 'LEGIT')), F


# ────────────── absolute semantic metrics (one object) ──────────────
#
# The comparative B-vs-A checks above attribute blame to the normalizer, but
# what SHIPS is the object after the WHOLE chain (the register paints that
# follow a normalizer routinely repair its intermediate state).  These metrics
# are computed on a single object with no cross-object matching, so they
# survive register painting, and are compared BETWEEN the compiler's own
# output and the final object: any metric that gets WORSE is corruption
# introduced by PostProc.

ABI_ENTRY_DEFINED = {R_SP, 28, R_RA, R_FP, 4, 5, 6, 7,
                     16, 17, 18, 19, 20, 21, 22, 23}


def defined_analysis(insns):
    """Forward must-be-defined dataflow.  Returns list of (idx, reg) reads of
    a register with no definition on some path -- garbage reads."""
    n = len(insns)
    preds = defaultdict(list)
    for j in range(n):
        for s in successors(insns, j):
            preds[s].append(j)
    ALL = set(range(34)) | {FP_BASE + k for k in range(32)}
    din = [set(ALL) for _ in range(n)]
    din[0] = set(ABI_ENTRY_DEFINED)
    for _ in range(300):
        changed = False
        for j in range(n):
            if j == 0:
                new = set(ABI_ENTRY_DEFINED)
            elif not preds[j]:
                new = set(ABI_ENTRY_DEFINED)      # unreachable / alt entry
            else:
                new = None
                for p in preds[j]:
                    dp = din[p] | insns[p].defs
                    if clobbers_here(insns, p):
                        dp = (dp - CALLER_SAVED) | {2, 3}
                    new = dp if new is None else (new & dp)
            if new != din[j]:
                din[j] = new
                changed = True
        if not changed:
            break
    # A read of a register that NOTHING earlier defines is a hidden register
    # parameter -- this codebase is full of functions the caller threads
    # values to in t-regs, and the target does the same read.  Those are
    # reported separately from a read whose register IS defined earlier but
    # not on every path, which is a genuine clobber.
    firstdef = {}
    for j, ins in enumerate(insns):
        for r in ins.defs:
            firstdef.setdefault(r, j)
    bad, livein = [], []
    for j, ins in enumerate(insns):
        for r in sorted(ins.uses):
            if r in din[j]:
                continue
            (livein if firstdef.get(r, len(insns)) >= j else bad).append((j, r))
    return bad, livein


def reaching_defs(insns):
    """{idx: {reg: set(def indices)}} classic reaching definitions."""
    n = len(insns)
    preds = defaultdict(list)
    for j in range(n):
        for s in successors(insns, j):
            preds[s].append(j)
    rin = [defaultdict(set) for _ in range(n)]
    for _ in range(300):
        changed = False
        for j in range(n):
            new = defaultdict(set)
            for p in preds[j]:
                out = defaultdict(set)
                for r, s in rin[p].items():
                    out[r] |= s
                for r in insns[p].defs:
                    out[r] = {p}
                if clobbers_here(insns, p):
                    for r in CALLER_SAVED:
                        out[r] = {p}
                for r, s in out.items():
                    new[r] |= s
            if {k: v for k, v in new.items() if v} != \
               {k: v for k, v in rin[j].items() if v}:
                rin[j] = new
                changed = True
        if not changed:
            break
    return rin


_ADDR_RE = re.compile(r'(?:D_|func_|jtbl_|_)?([0-9A-Fa-f]{8})$')


def sym_addr(name):
    """Address encoded in a decomp symbol name (D_800B2580 -> 0x800B2580)."""
    m = _ADDR_RE.match(name.strip())
    if not m:
        return None
    v = int(m.group(1), 16)
    return v if 0x80000000 <= v < 0x80800000 else None


def reloc_addr(rel_entry):
    """Absolute address a relocation resolves to, or None."""
    if not rel_entry:
        return None
    txt = rel_entry[1]
    name, _, add = txt.partition('+')
    a = sym_addr(name)
    if a is None:
        return None
    if add:
        try:
            a += int(add, 0)
        except ValueError:
            return None
    return a


def hilo_mismatches(insns, rel):
    """Sites where the address a %lo() instruction actually computes is NOT
    the address of the symbol it references.

    IDO legitimately CSEs one `lui` across every global in the same 64K page,
    so a %hi/%lo pair naming DIFFERENT symbols is normal and correct; what is
    never correct is the pair computing the wrong final address.  That is the
    signature of reloc'd words permuted into each other's slots -- which the
    masked compare reports as 100%% matched."""
    rin = reaching_defs(insns)
    out = []
    for j, ins in enumerate(insns):
        r = rel.get(j)
        if not r or r[0] != 'R_MIPS_LO16':
            continue
        want = reloc_addr(r)
        if want is None:
            continue
        base = ins.rs
        for d in sorted(rin[j].get(base, ())):
            rd = rel.get(d)
            if insns[d].op != 0x0F or not rd or rd[0] != 'R_MIPS_HI16':
                continue
            hbase = reloc_addr(rd)
            if hbase is None:
                continue
            hi = ((hbase + 0x8000) >> 16) & 0xFFFF
            got = ((hi << 16) + sign16(want & 0xFFFF)) & 0xFFFFFFFF
            if got != want:
                out.append((j, '%s wants 0x%08x' % (r[1], want),
                            d, '%s gives 0x%08x' % (rd[1], got)))
    return out


def object_metrics(insns, rel):
    n = len(insns)
    m = {}
    m['undef_reads'], m['hidden_param_reads'] = defined_analysis(insns)
    m['hilo_mismatch'] = hilo_mismatches(insns, rel)
    m['branch_oob'] = [j for j, i in enumerate(insns)
                       if i.is_branch and not (0 <= (i.target or -1) < n)]
    m['branch_in_slot'] = [j for j, i in enumerate(insns)
                           if i.is_ctrl() and j + 1 < n and insns[j + 1].is_ctrl()]
    m['self_loop'] = [j for j, i in enumerate(insns)
                      if i.is_branch and i.target is not None
                      and i.target in (j - 1, j)]
    # The MIPS ABI pairs each HI16 with the NEXT LO16 in the relocation
    # stream (the LO16 addend supplies the carry), whatever symbol that LO16
    # names -- so only a HI16 with no LO16 after it at all is a real defect.
    los = sorted(j for j, (t, _) in rel.items() if t == 'R_MIPS_LO16')
    m['hi_no_lo'] = [(rel[j][1], j) for j in sorted(rel)
                     if rel[j][0] == 'R_MIPS_HI16'
                     and not any(k > j for k in los)]
    return m


METRIC_TEXT = {
    'undef_reads': 'reads of a register that IS defined earlier in the '
                   'function but not on every path reaching the read '
                   '(a clobbered value / garbage address)',
    'hidden_param_reads': 'reads of a register nothing in the function ever '
                          'defines -- a hidden register parameter threaded in '
                          'by the caller, NOT corruption on its own',
    'hilo_mismatch': '%lo(S) whose base register was loaded with %hi(T) '
                     '-- the address is assembled from TWO DIFFERENT symbols',
    'branch_oob': 'branch targeting outside the function body',
    'branch_in_slot': 'branch/jump sitting in another branch delay slot',
    'self_loop': 'branch to itself or to its own delay slot (infinite loop)',
    'hi_no_lo': '%hi with no following %lo (ld cannot compute the carry)',
}


# Metrics whose rise means "look at this", not "this is corrupt".  A hidden
# register parameter is a real pattern in this codebase (the caller threads a
# value in a t-reg and the TARGET reads it the same way), so a paint chain
# that renames an ABI register to the hidden one legitimately raises it.
SOFT_METRICS = {'hidden_param_reads': 'WARN'}


def diff_metrics(mb, mf):
    """Findings for every metric that got WORSE from compiler output to the
    final object."""
    F = []
    for k, desc in METRIC_TEXT.items():
        b, f = mb.get(k, []), mf.get(k, [])
        if len(f) > len(b):
            F.append(dict(code='S:' + k, sev=SOFT_METRICS.get(k, 'BROKEN'),
                          msg='PostProc INTRODUCED %d new site(s) of "%s" '
                              '(compiler emitted %d, shipped object has %d); '
                              'first new: %s'
                              % (len(f) - len(b), desc, len(b), len(f),
                                 f[len(b)] if len(f) > len(b) else '')))
    return F


# ───────────────────────────── driver ─────────────────────────────

def run_one(overlay, func):
    src = ROOT / "src" / "overlay" / overlay / ("%s.c" % func)
    if not src.exists():
        return dict(func=func, verdict='NOSRC', findings=[])
    content = src.read_text(errors='ignore')
    if "Status: ASM-ONLY" in content or "@AsmOnly" in content:
        return dict(func=func, verdict='ASM_ONLY', findings=[])
    steps = parse_postproc(src)
    idx = next((k for k, s in enumerate(steps)
                if s.get('type') == 'scheduler_normalizer'), None)
    if idx is None:
        return dict(func=func, verdict='NO_NORM', findings=[])
    pre, norm, post = steps[:idx], steps[idx:idx + 1], steps[idx + 1:]

    syms = parse_sym_file(overlay)
    if func not in syms:
        return dict(func=func, verdict='NOSYM', findings=[])
    addr, size = syms[func]
    o = parse_symbol_start_text(content)
    if o is not None:
        addr = o
    o = parse_symbol_size_text(content)
    if o is not None:
        size = o
    asm = ROOT / "asm" / "overlay" / overlay / ("%s.s" % func)
    asm = asm if asm.exists() else None

    with tempfile.TemporaryDirectory() as td:
        td = Path(td)
        base = td / ("%s.o" % func)
        try:
            compile_obj(src, base, td)
        except Exception as e:
            return dict(func=func, verdict='COMPILEFAIL',
                        findings=[dict(code='--', sev='BROKEN', msg=str(e))])
        ob, oa, of = td / "b.o", td / "a.o", td / "f.o"
        ob.write_bytes(base.read_bytes())
        cwd = os.getcwd()
        os.chdir(ROOT)
        try:
            if pre and not apply_postproc(ob, pre, addr, overlay,
                                          verbose=False, target_asm=asm):
                return dict(func=func, verdict='PREFAIL', findings=[
                    dict(code='--', sev='BROKEN',
                         msg='the PostProc steps BEFORE the normalizer failed')])
            oa.write_bytes(ob.read_bytes())
            if not apply_postproc(oa, norm, addr, overlay, verbose=False,
                                  target_asm=asm):
                return dict(func=func, verdict='NORMFAIL', findings=[
                    dict(code='--', sev='BROKEN',
                         msg='the scheduler_normalizer step itself failed')])
            of.write_bytes(oa.read_bytes())
            final_ok = (not post) or apply_postproc(of, post, addr, overlay,
                                                    verbose=False,
                                                    target_asm=asm)
        finally:
            os.chdir(cwd)

        def load(obj):
            v, s = sym_window(obj, func)
            t = text_bytes(obj)
            if v is None:
                v, s = 0, len(t)
            # analyse exactly the compare window: decoding the alignment pad
            # past the body would invent instructions
            w = t[v:v + min(size, max(0, len(t) - v))]
            r = {(k - v) // 4: x for k, x in relocs(obj).items()
                 if v <= k < v + len(w)}
            return w, r, [Insn(struct.unpack_from('<I', w, i * 4)[0], i)
                          for i in range(len(w) // 4)]

        wb, rb, B = load(ob)
        wa, ra, A = load(oa)
        wf, rf, FO = load(of)

        inert = (wa == wb and ra == rb)
        findings = []
        if not inert:
            _, findings = check(B, A, rb, ra)
            # The C* checks attribute blame to the normalizer STEP.  What
            # ships is the object after the whole chain, and the register
            # paints that usually follow a normalizer routinely repair its
            # intermediate state -- so C* findings are DETAIL, not a verdict.
            for f in findings:
                f['sev'] = 'INFO'

        # what actually SHIPS is `of`; compare its absolute semantic health
        # with the compiler's own output
        ship = []
        if final_ok:
            ship = diff_metrics(object_metrics(B, rb), object_metrics(FO, rf))
        else:
            ship = [dict(code='S:chain', sev='WARN',
                         msg='the PostProc steps AFTER the normalizer failed, '
                             'so the shipped object could not be built here')]
        findings = ship + findings

        hard = [f for f in findings if f['sev'] == 'BROKEN']
        soft = [f for f in findings if f['sev'] == 'WARN']
        if inert and not hard and not soft:
            verdict = 'INERT'
        elif hard:
            verdict = 'BROKEN'
        elif soft:
            verdict = 'WARN'
        else:
            verdict = 'LEGIT'
        moved = sum(1 for i in range(min(len(A), len(B)))
                    if A[i].word != B[i].word)
        return dict(func=func, verdict=verdict, findings=findings,
                    inert=inert, nwords=len(B), diffwords=moved,
                    addr=addr, size=size)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('overlay')
    ap.add_argument('func', nargs='?')
    ap.add_argument('--all', action='store_true')
    ap.add_argument('--json')
    ap.add_argument('-v', '--verbose', action='store_true')
    a = ap.parse_args()

    if a.all:
        funcs = []
        for c in sorted((ROOT / "src" / "overlay" / a.overlay).glob("*.c")):
            try:
                st = parse_postproc(c)
            except Exception:
                continue
            if any(s.get('type') == 'scheduler_normalizer' for s in st):
                funcs.append(c.stem)
    else:
        funcs = [a.func]

    results = []
    for f in funcs:
        r = run_one(a.overlay, f)
        results.append(r)
        codes = sorted({x['code'] for x in r['findings']
                        if x['sev'] in ('BROKEN', 'WARN')})
        print("%-16s %-12s %s" % (f, r['verdict'], ",".join(codes)))
        sys.stdout.flush()
        if a.verbose or r['verdict'] in ('BROKEN', 'WARN'):
            for x in r['findings']:
                print("    [%s/%s] %s" % (x['code'], x['sev'], x['msg']))
    if a.json:
        Path(a.json).write_text(json.dumps(results, indent=1))
    from collections import Counter
    c = Counter(r['verdict'] for r in results)
    print("\n== %s: %d function(s) with a scheduler_normalizer ==" %
          (a.overlay, len(results)))
    for k, v in sorted(c.items()):
        print("  %-12s %d" % (k, v))


if __name__ == '__main__':
    main()
