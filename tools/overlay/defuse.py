#!/usr/bin/env python3
"""defuse.py -- destructive-register-paint detector for overlay PostProc.

WHY THIS EXISTS
---------------
`patch_insn_reg_field` (and the `gen_reg_patches.py` output that emits it)
pairs COMPILED word N against TARGET word N.  When the compiled instruction
stream is displaced or permuted relative to the target (an extra/missing
instruction, a different schedule, a hoisted load), compiled word N is a
DIFFERENT instruction than target word N -- so the paint renames a register
on an unrelated instruction.  The masked compare then scores the object
HIGHER while the object computes garbage: a use renamed without its def, an
`addu` reading a never-defined register, a relocated `lw %lo(D_x)` repainted
into a stack `lw`.  `--semcheck` catches none of this.

WHAT IT DOES
------------
Compiles the function, runs the real PostProc chain keeping every
intermediate object, and recovers the paint sites FROM THE BYTES (diffing the
object before each `patch_insn_reg_field` step against the object after it),
so it does not depend on how the directive's offsets were translated.  Then:

 (0) SITE SANITY (per step, exact)
     PAINT_ON_NOP      paint landed on a compiled nop -- a nop is
                       `sll $zero,$zero,0`, so painting it fabricates an
                       instruction (banned by CLAUDE.md).
     OPCODE_CHANGED    a "register field" paint changed the operation.
     REGIMM_RT_FLIP    rt of an op==1 branch is the CONDITION selector
                       (bgez<->bltz), not a register: polarity fabrication.

 (1) TARGET PAIRING (why the paints lie)
     MISALIGNED_SITE   the compiled word the paint landed on is not the same
                       instruction as the target word at that index (opcode,
                       funct, shift amount or non-reloc immediate differ).
                       Every paint on a misaligned site is a rename across
                       unrelated instructions.
     FIELD_DISAGREES_TARGET
                       the painted register is not even the register the
                       target has in that field.

 (2) DEF-USE (block-local, so it makes no claim it cannot prove)
     Basic blocks are recovered from branch displacements (delay slots
     included).  Claims are only raised for straight-line, in-block
     evidence, plus one whole-function claim that needs no CFG:
     NO_DEF_ANYWHERE   a painted USE reads a register that is never defined
                       anywhere in the function and is not live on entry.
     CLOBBERED_ACROSS_CALL
                       painted USE whose in-block def is separated from it by
                       a call, and the register is caller-saved.
     ORPHANED_USE      painted DEF renamed $old->$new while a later in-block
                       instruction still reads $old before any redefinition
                       and was NOT repainted -- it now reads a stale value.
     CLOBBER_LIVE      painted DEF overwrites a register that is still live
                       in-block.
     CROSS_BLOCK_RENAME
                       the old register had an in-block def, the new one does
                       not: the rename left its value chain (review).

 (3) RELOCATION INTEGRITY
     HILO_PAIR_BROKEN  a %lo(SYM) load/store's base register no longer equals
                       the rt of the `lui %hi(SYM)` that reaches it -- this is
                       the confirmed "relocated `lw %lo(D_x)` repainted into a
                       stack `lw`" failure.  Reported only when the pairing
                       was intact BEFORE the paints.

 (4) convert_ori_to_addiu AUDIT
     ORI_ADDIU_WRONG_ADDR
                       on a raw-address `lui;ori` pair the directive rewrites
                       ori->addiu, which REINTERPRETS imm16 as SIGNED.  Unless
                       IDO's lui already carried hi+1, the linked address is
                       0x10000 low (confirmed: 0x8013C290 -> 0x8012C290).
                       Recomputes both forms and compares to the target's.

USAGE
    python3 wip-scratch/defuse.py OVERLAY FUNC [-v]
    python3 wip-scratch/defuse.py --list FILE      ("OV FUNC" per line)
Writes wip-scratch/defuse_report.json in --list mode.
Exit status 1 if any HIGH-severity finding.
"""
import json
import os
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(ROOT / "tools" / "build"))
sys.path.insert(0, str(ROOT / "tools" / "overlay"))

from parse_postproc import (parse_compile_flags, parse_compile_env,
                            parse_compiler, parse_symbol_start_text,
                            parse_symbol_size_text)
from overlay_postproc import (parse_postproc, apply_postproc,
                              get_func_text_offset)
from ido_flags import base_flags, pic_flags

SRC = ROOT / "src" / "overlay"
CFG = ROOT / "config" / "overlay"
ASSETS = ROOT / "assets" / "overlays"
INC = ROOT / "include"
IDO_CC = ROOT / "tools" / "ido-static-recomp" / "build" / "5.2" / "out" / "cc"
GCCWRAP = ROOT / "tools" / "gcc" / "gccwrap.py"
IDO_FLAGS = base_flags([INC, f"{INC}/psyq"])
GCCWRAP_FLAGS = {"--fill-jalr", "--fill-epilogue", "--fill-all",
                 "--no-delay-slots", "--no-cross-jumping"}
BASE = 0x80132000

REGN = ["zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
        "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
        "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
        "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"]
CALLER_SAVED = set([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25])
ENTRY_LIVE = set([0, 4, 5, 6, 7, 28, 29, 30, 31])   # zero, a0-a3, gp, sp, fp, ra


# ---------------------------------------------------------------- decoder
def decode(w):
    """(mnemonic, defs, uses, fieldrole) for a MIPS-I word.

    fieldrole maps 'rs'/'rt'/'rd' -> 'use' | 'def', describing what the field
    means for THIS opcode -- rt is a def for `lw`/`addiu` but a use for
    `sw`/`beq`.  That is what makes the def-use check field-accurate.
    """
    op = (w >> 26) & 0x3F
    rs = (w >> 21) & 0x1F
    rt = (w >> 16) & 0x1F
    rd = (w >> 11) & 0x1F
    fn = w & 0x3F
    D, U, role = set(), set(), {}

    def fin(m):
        return (m, D, U, role)

    if w == 0:
        return fin("nop")
    if op == 0:
        if fn in (0, 2, 3):
            D.add(rd); U.add(rt); role.update(rt="use", rd="def")
            return fin({0: "sll", 2: "srl", 3: "sra"}[fn])
        if fn in (4, 6, 7):
            D.add(rd); U.update((rs, rt))
            role.update(rs="use", rt="use", rd="def")
            return fin({4: "sllv", 6: "srlv", 7: "srav"}[fn])
        if fn == 8:
            U.add(rs); role.update(rs="use")
            return fin("jr")
        if fn == 9:
            # A call DEFINES the return registers -- without this the def-use
            # scan reports `jal f; ...; use $v0` as a use of an undefined /
            # call-clobbered register.
            U.add(rs); D.update((rd, 2, 3)); role.update(rs="use", rd="def")
            return fin("jalr")
        if fn in (12, 13):
            return fin("syscall")
        if fn in (16, 18):
            D.add(rd); role.update(rd="def")
            return fin("mfhi" if fn == 16 else "mflo")
        if fn in (17, 19):
            U.add(rs); role.update(rs="use")
            return fin("mthi" if fn == 17 else "mtlo")
        if fn in (24, 25, 26, 27):
            U.update((rs, rt)); role.update(rs="use", rt="use")
            return fin(["mult", "multu", "div", "divu"][fn - 24])
        if 32 <= fn <= 43:
            D.add(rd); U.update((rs, rt))
            role.update(rs="use", rt="use", rd="def")
            return fin({32: "add", 33: "addu", 34: "sub", 35: "subu",
                        36: "and", 37: "or", 38: "xor", 39: "nor",
                        42: "slt", 43: "sltu"}.get(fn, "r%d" % fn))
        return fin("special%d" % fn)
    if op == 1:
        U.add(rs); role.update(rs="use")
        if rt in (16, 17):
            D.add(31)
        return fin("regimm")
    if op == 2:
        return fin("j")
    if op == 3:
        D.update((31, 2, 3))              # $ra plus the return registers
        return fin("jal")
    if op in (4, 5):
        U.update((rs, rt)); role.update(rs="use", rt="use")
        return fin("beq" if op == 4 else "bne")
    if op in (6, 7):
        U.add(rs); role.update(rs="use")
        return fin("blez" if op == 6 else "bgtz")
    if 8 <= op <= 14:
        D.add(rt); U.add(rs); role.update(rs="use", rt="def")
        return fin({8: "addi", 9: "addiu", 10: "slti", 11: "sltiu",
                    12: "andi", 13: "ori", 14: "xori"}[op])
    if op == 15:
        D.add(rt); role.update(rt="def")
        return fin("lui")
    if op in (16, 17, 18, 19):
        if rs in (0, 2):
            D.add(rt); role.update(rt="def")
            return fin("mfc%d" % (op - 16))
        if rs in (4, 6):
            U.add(rt); role.update(rt="use")
            return fin("mtc%d" % (op - 16))
        return fin("cop%d" % (op - 16))
    if op in (32, 33, 34, 35, 36, 37, 38):
        D.add(rt); U.add(rs); role.update(rs="use", rt="def")
        return fin({32: "lb", 33: "lh", 34: "lwl", 35: "lw", 36: "lbu",
                    37: "lhu", 38: "lwr"}[op])
    if op in (40, 41, 42, 43, 46):
        U.update((rs, rt)); role.update(rs="use", rt="use")
        return fin({40: "sb", 41: "sh", 42: "swl", 43: "sw", 46: "swr"}[op])
    if op in (49, 50, 57, 58):
        U.add(rs); role.update(rs="use")
        return fin("lwc/swc")
    return fin("op%d" % op)


def is_load(w):
    return 32 <= ((w >> 26) & 0x3F) <= 38


def is_store(w):
    return ((w >> 26) & 0x3F) in (40, 41, 42, 43, 46)


def is_call(w):
    op = (w >> 26) & 0x3F
    return op == 3 or (op == 0 and (w & 0x3F) == 9)


def is_branch(w):
    op = (w >> 26) & 0x3F
    return op in (1, 4, 5, 6, 7)


def is_jump(w):
    op = (w >> 26) & 0x3F
    return op == 2 or (op == 0 and (w & 0x3F) == 8)


def branch_target(stream_idx, w):
    imm = w & 0xFFFF
    if imm >= 0x8000:
        imm -= 0x10000
    return stream_idx + 1 + imm


def blocks_of(words, lo, hi):
    """Map word index -> basic-block id, from intra-function branches.

    Leaders: function entry, every branch target, and the instruction after
    a branch's delay slot / after a jump.  Block-local claims made against
    this partition are straight-line facts.
    """
    leaders = set([lo])
    for i in range(lo, hi):
        w = words[i]
        if is_branch(w):
            t = branch_target(i, w)
            if lo <= t < hi:
                leaders.add(t)
            if i + 2 < hi:
                leaders.add(i + 2)
        elif is_jump(w):
            if i + 2 < hi:
                leaders.add(i + 2)
    bid, out = -1, {}
    for i in range(lo, hi):
        if i in leaders:
            bid += 1
        out[i] = bid
    return out


# ---------------------------------------------------------------- objects
def words_of_text(obj):
    raw = subprocess.run(["mipsel-linux-gnu-objcopy", "-O", "binary",
                          "-j", ".text", str(obj), "/dev/stdout"],
                         capture_output=True).stdout
    return [struct.unpack("<I", raw[i:i + 4])[0]
            for i in range(0, len(raw) - len(raw) % 4, 4)]


def sym_window(obj, func):
    out = subprocess.run(["mipsel-linux-gnu-objdump", "-t", str(obj)],
                         capture_output=True, text=True).stdout
    for line in out.splitlines():
        p = line.split()
        if len(p) >= 6 and p[-1] == func and ".text" in line:
            try:
                return int(p[0], 16), int(p[-2], 16)
            except ValueError:
                pass
    return get_func_text_offset(obj), None


def reloc_map(obj):
    """text byte offset -> (type, symbol)."""
    out = subprocess.run(["mipsel-linux-gnu-objdump", "-r", str(obj)],
                         capture_output=True, text=True).stdout
    res, insec = {}, False
    for line in out.splitlines():
        if line.startswith("RELOCATION RECORDS FOR"):
            insec = "[.text]" in line or ".text" in line
            continue
        if not insec:
            continue
        p = line.split()
        if len(p) >= 3 and p[1].startswith("R_MIPS"):
            try:
                res[int(p[0], 16)] = (p[1], p[2])
            except ValueError:
                pass
    return res


# ---------------------------------------------------------------- helpers
def fld(w, f):
    return (w >> {"rs": 21, "rt": 16, "rd": 11}[f]) & 0x1F


def changed_fields(a, b):
    return [(f, fld(a, f), fld(b, f))
            for f in ("rs", "rt", "rd") if fld(a, f) != fld(b, f)]


def site_alignment(c, t, relocd):
    """Is the compiled word the SAME instruction as the target word the paint
    tool paired it with?  Register fields ignored (that is what a paint is
    allowed to change).  Returns None when aligned, else (severity, reason).
    """
    if t is None:
        return ("MED", "no target word at this index (compile overruns the "
                       "symbol window)")
    if (c >> 26) != (t >> 26):
        return ("HIGH", "opcode %d vs target opcode %d"
                % (c >> 26, t >> 26))
    op = c >> 26
    if op == 0:
        if (c & 0x3F) != (t & 0x3F):
            return ("HIGH", "funct %d vs target funct %d"
                    % (c & 0x3F, t & 0x3F))
        if ((c >> 6) & 0x1F) != ((t >> 6) & 0x1F):
            return ("HIGH", "shift amount %d vs target %d"
                    % ((c >> 6) & 0x1F, (t >> 6) & 0x1F))
        return None
    if op in (2, 3):
        if relocd or (c & 0x3FFFFFF) == (t & 0x3FFFFFF):
            return None
        return ("MED", "jump target differs")
    if relocd or op == 1:
        return None
    if (c & 0xFFFF) != (t & 0xFFFF):
        br = op in (4, 5, 6, 7)
        return ("MED" if br else "HIGH",
                "%s 0x%04x vs target 0x%04x"
                % ("branch displacement" if br else "immediate",
                   c & 0xFFFF, t & 0xFFFF))
    return None


def in_block_last_def(words, blk, idx, reg, lo):
    for j in range(idx - 1, lo - 1, -1):
        if blk.get(j) != blk.get(idx):
            return None
        if reg in decode(words[j])[1]:
            return j
    return None


def def_anywhere(words, lo, hi, reg):
    return any(reg in decode(words[j])[1] for j in range(lo, hi))


def in_block_next_ref(words, blk, idx, reg, hi):
    for j in range(idx + 1, hi):
        if blk.get(j) != blk.get(idx):
            return (None, None)
        _, D, U, _ = decode(words[j])
        if reg in U:
            return ("use", j)
        if reg in D:
            return ("def", j)
    return (None, None)


# ---------------------------------------------------------------- build
def compile_and_chain(ov, func, workdir, verbose=False):
    """Compile + run the PostProc chain, keeping every intermediate object.

    Returns (base_obj, [(step_idx, step, pre_obj, post_obj)], info).
    """
    src = SRC / ov / f"{func}.c"
    text = src.read_text(errors="replace")
    steps = parse_postproc(src)
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
    if ss is not None:
        size = ss
    if st is not None:
        addr = st

    obj = workdir / f"{func}.o"
    if compiler.startswith("gcc"):
        cmd = [sys.executable, str(GCCWRAP), str(src), "-o", str(obj),
               "--gcc-version", compiler[3:], "-I", str(INC),
               "-I", str(INC / "psyq")]
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
    r = subprocess.run(cmd, capture_output=True, text=True, timeout=300,
                       env=env, cwd=str(workdir))
    if r.returncode != 0:
        return None, None, {"error": "compile failed: " + r.stderr[:200]}

    base = workdir / f"{func}.base.o"
    shutil.copy(obj, base)
    asm = ROOT / "asm" / "overlay" / ov / f"{func}.s"
    ok = apply_postproc(obj, steps, addr, ov, verbose=verbose,
                        target_asm=asm if asm.exists() else None)

    pairs, cur = [], base
    for i, s in enumerate(steps):
        pp = workdir / f"{func}.pp{i}.o"
        if not pp.exists():
            continue                        # step skipped or chain aborted
        pairs.append((i, s, cur, pp))
        cur = pp
    return base, pairs, {"addr": addr, "size": size, "chain_ok": ok,
                         "final": cur, "nsteps": len(steps)}


def target_words(ov, addr, size):
    with open(ASSETS / f"{ov}.EXN", "rb") as f:
        f.seek(addr - BASE)
        raw = f.read(size)
    return [struct.unpack("<I", raw[i:i + 4])[0]
            for i in range(0, len(raw) - len(raw) % 4, 4)]


# ---------------------------------------------------------------- checks
def check_sites(paint_steps, tw, lo, rel_pre):
    """Site sanity + target pairing, per paint step (exact, no dataflow)."""
    out, paints = [], {}
    for stepno, pre_w, post_w in paint_steps:
        for k in range(min(len(pre_w), len(post_w))):
            if pre_w[k] == post_w[k]:
                continue
            ti = k - lo
            tword = tw[ti] if 0 <= ti < len(tw) else None
            mn, _, _, role = decode(pre_w[k])
            ctx = {"step": stepno, "word": k, "off": hex(k * 4),
                   "pre": "%08x" % pre_w[k], "post": "%08x" % post_w[k],
                   "mnem": mn,
                   "target": ("%08x" % tword) if tword is not None else None}
            if pre_w[k] == 0:
                out.append(dict(ctx, sev="HIGH", kind="PAINT_ON_NOP",
                                msg="paint landed on a compiled nop (a nop IS "
                                    "sll $zero,$zero,0 -- painting it "
                                    "fabricates an instruction)"))
                continue
            if (pre_w[k] >> 26) != (post_w[k] >> 26) or \
               ((pre_w[k] >> 26) == 0 and
                    (pre_w[k] & 0x3F) != (post_w[k] & 0x3F)):
                out.append(dict(ctx, sev="HIGH", kind="OPCODE_CHANGED",
                                msg="%s -> %s" % (mn, decode(post_w[k])[0])))
                continue
            fields = changed_fields(pre_w[k], post_w[k])
            if (pre_w[k] >> 26) == 1 and any(f == "rt" for f, _, _ in fields):
                out.append(dict(ctx, sev="HIGH", kind="REGIMM_RT_FLIP",
                                msg="rt of an op==1 branch is the condition "
                                    "selector (bgez<->bltz), not a register"))
                continue
            mis = site_alignment(pre_w[k], tword, k * 4 in rel_pre)
            if mis:
                out.append(dict(ctx, sev=mis[0], kind="MISALIGNED_SITE",
                                msg="compiled word %d is NOT the target's "
                                    "word %d (%s) -- a paint here renames a "
                                    "register across unrelated instructions"
                                    % (k, ti, mis[1])))
            else:
                for f, old, new in fields:
                    tv = fld(tword, f)
                    if tv != new:
                        out.append(dict(ctx, sev="HIGH",
                                        kind="FIELD_DISAGREES_TARGET",
                                        msg="%s field %s painted $%s -> $%s "
                                            "but the target has $%s there"
                                            % (mn, f, REGN[old], REGN[new],
                                               REGN[tv])))
            for f, old, new in fields:
                tv = fld(tword, f) if (tword is not None and not mis) else None
                paints.setdefault(k, []).append((f, old, new, role.get(f),
                                                 mn, mis is not None, tv))
    return out, paints


def check_defuse(pre_w, post_w, paints, lo, hi, tw):
    """Block-local def-use integrity of the painted stream."""
    out = []
    blk_post = blocks_of(post_w, lo, hi)
    blk_pre = blocks_of(pre_w, lo, hi)
    for k in sorted(paints):
        if not (lo <= k < hi):
            continue
        for f, old, new, role, mn, mis, tv in paints[k]:
            ctx = {"word": k, "off": hex(k * 4), "mnem": mn,
                   "misaligned": mis}
            if role == "use":
                if new not in ENTRY_LIVE and \
                        not def_anywhere(post_w, lo, hi, new) and tv == new:
                    # The paint reproduces the TARGET's own register at an
                    # aligned site: the value arrives from the caller.  This
                    # is the sanctioned hidden-parameter convention (selector
                    # in $v0/$v1 modelled as the a0 parameter in C), not a
                    # broken chain.
                    out.append(dict(ctx, sev="INFO" if new in (2, 3)
                                    else "MED", kind="HIDDEN_PARAM_USE",
                                    msg="%s field %s: $%s -> $%s has no def "
                                        "in the function, but the target "
                                        "reads $%s here too (incoming hidden "
                                        "parameter)"
                                        % (mn, f, REGN[old], REGN[new],
                                           REGN[new])))
                elif new not in ENTRY_LIVE and \
                        not def_anywhere(post_w, lo, hi, new):
                    out.append(dict(ctx, sev="HIGH", kind="NO_DEF_ANYWHERE",
                                    msg="%s field %s: $%s -> $%s, but $%s is "
                                        "never defined anywhere in the "
                                        "function and is not live on entry "
                                        "-- this reads garbage"
                                        % (mn, f, REGN[old], REGN[new],
                                           REGN[new])))
                    continue
                dnew = in_block_last_def(post_w, blk_post, k, new, lo)
                dold = in_block_last_def(pre_w, blk_pre, k, old, lo)
                if dnew is not None and new in CALLER_SAVED and \
                        any(is_call(post_w[j]) for j in range(dnew + 1, k)):
                    out.append(dict(ctx, sev="HIGH",
                                    kind="CLOBBERED_ACROSS_CALL",
                                    msg="%s field %s: $%s -> $%s, whose only "
                                        "in-block def (word %d) is separated "
                                        "from this use by a call; $%s is "
                                        "caller-saved and holds garbage here"
                                        % (mn, f, REGN[old], REGN[new], dnew,
                                           REGN[new])))
                elif dold is not None and dnew is None:
                    out.append(dict(ctx, sev="MED", kind="CROSS_BLOCK_RENAME",
                                    msg="%s field %s: $%s (defined in-block "
                                        "at word %d) -> $%s (no in-block def) "
                                        "-- the rename left its value chain"
                                        % (mn, f, REGN[old], dold, REGN[new])))
            elif role == "def":
                kind_, j = in_block_next_ref(post_w, blk_post, k, old, hi)
                if kind_ == "use" and j not in paints:
                    out.append(dict(ctx, sev="HIGH", kind="ORPHANED_USE",
                                    msg="def renamed $%s -> $%s but word %d "
                                        "(off %s) in the SAME block still "
                                        "reads $%s before any redefinition "
                                        "and was not repainted -- it now "
                                        "reads a stale value"
                                        % (REGN[old], REGN[new], j,
                                           hex(j * 4), REGN[old])))
                pd = in_block_last_def(post_w, blk_post, k, new, lo)
                if pd is not None:
                    nk, nj = in_block_next_ref(post_w, blk_post, k, new, hi)
                    if nk == "use" and nj not in paints:
                        out.append(dict(ctx, sev="MED", kind="CLOBBER_LIVE",
                                        msg="new dest $%s was defined in-block "
                                            "at word %d and is read at word "
                                            "%d; this paint clobbers a live "
                                            "value" % (REGN[new], pd, nj)))
    return out


def hilo_pairs(words, rel, lo, hi):
    """{lo16 word idx: (sym, base_reg, hi_word_idx, hi_rt)} for %hi/%lo."""
    res = {}
    for off, (rt_, sym) in rel.items():
        k = off // 4
        if rt_ != "R_MIPS_LO16" or not (lo <= k < hi):
            continue
        base = fld(words[k], "rs")
        hidx = hirt = None
        for j in range(k - 1, lo - 1, -1):
            e = rel.get(j * 4)
            if e and e[0] == "R_MIPS_HI16" and e[1] == sym:
                hidx, hirt = j, fld(words[j], "rt")
                break
        res[k] = (sym, base, hidx, hirt)
    return res


def check_hilo(pre_w, post_w, rel_pre, rel_post, lo, hi):
    out = []
    before = hilo_pairs(pre_w, rel_pre, lo, hi)
    after = hilo_pairs(post_w, rel_post, lo, hi)
    for k, (sym, base, hidx, hirt) in after.items():
        if hidx is None:
            continue
        if base == hirt:
            continue
        b = before.get(k)
        if b and b[2] is not None and b[1] == b[3]:
            out.append({"sev": "HIGH", "kind": "HILO_PAIR_BROKEN",
                        "word": k, "off": hex(k * 4),
                        "mnem": decode(post_w[k])[0],
                        "msg": "%%lo(%s) access now uses base $%s but the "
                               "reaching lui %%hi(%s) (word %d) targets $%s "
                               "-- the paints broke the address pair (before "
                               "the paints both were $%s)"
                               % (sym, REGN[base], sym, hidx, REGN[hirt],
                                  REGN[b[1]])})
    return out


def check_ori(pre_w, post_w, lo, hi, tw, stepno, final_w=None):
    """convert_ori_to_addiu: signed-imm16 reinterpretation audit.

    The verdict is taken on the FINAL object, because a later
    `patch_insn_imm16` on the paired `lui` is the sanctioned way to carry the
    hi+1 the signed reinterpretation needs.
    """
    out = []
    fin = final_w if final_w is not None else post_w
    for k in range(min(len(pre_w), len(post_w))):
        if pre_w[k] == post_w[k]:
            continue
        if ((pre_w[k] >> 26) & 0x3F) != 13 or ((post_w[k] >> 26) & 0x3F) != 9:
            continue
        if k >= len(fin) or ((fin[k] >> 26) & 0x3F) != 9:
            continue                       # a later step rewrote the site
        imm = fin[k] & 0xFFFF
        rs = fld(fin[k], "rs")
        ctx = {"step": stepno, "word": k, "off": hex(k * 4),
               "pre": "%08x" % pre_w[k], "post": "%08x" % post_w[k],
               "mnem": "ori"}
        if imm < 0x8000:
            continue                       # signed == unsigned: always safe
        hidx = None
        for j in range(k - 1, max(lo - 1, k - 60), -1):
            if ((fin[j] >> 26) & 0x3F) == 15 and fld(fin[j], "rt") == rs:
                hidx = j
                break
        if hidx is None:
            out.append(dict(ctx, sev="MED", kind="ORI_NO_LUI",
                            msg="imm16 0x%04x >= 0x8000 with no defining lui "
                                "in range -- address unprovable" % imm))
            continue
        hiv = fin[hidx] & 0xFFFF
        was = ((hiv << 16) | imm) & 0xFFFFFFFF
        now = ((hiv << 16) + imm - 0x10000) & 0xFFFFFFFF
        tgt = None
        ti, thi = k - lo, hidx - lo
        if 0 <= ti < len(tw) and 0 <= thi < len(tw):
            th, tl = tw[thi] & 0xFFFF, tw[ti] & 0xFFFF
            top = (tw[ti] >> 26) & 0x3F
            tgt = ((th << 16) + (tl - 0x10000 if top == 9 and tl >= 0x8000
                                 else tl)) & 0xFFFFFFFF
        out.append(dict(ctx, sev="HIGH" if (tgt is None or tgt != now)
                        else "INFO", kind="ORI_ADDIU_WRONG_ADDR",
                        msg="lui 0x%04x + ori 0x%04x = 0x%08X, but as addiu "
                            "(signed imm16) it links 0x%08X%s"
                            % (hiv, imm, was, now,
                               "; target links 0x%08X" % tgt
                               if tgt is not None else "")))
    return out


# ---------------------------------------------------------------- driver
def analyse(ov, func, verbose=False, workroot=None):
    findings = []
    tmp = Path(workroot or tempfile.mkdtemp(prefix="defuse_"))
    tmp.mkdir(parents=True, exist_ok=True)
    try:
        base, pairs, info = compile_and_chain(ov, func, tmp, verbose=verbose)
        if base is None:
            return [{"sev": "INFO", "kind": "COMPILE_FAIL",
                     "msg": info["error"]}], info
        addr, size = info["addr"], info["size"]
        woff, wsize = sym_window(base, func)
        lo = woff // 4
        nwords = len(words_of_text(base))
        hi = lo + ((wsize or (size or 0)) // 4 or nwords)
        tw = target_words(ov, addr, size) if (addr and size) else []
        info["align_window"] = [lo, hi]

        paint_steps, ori_steps = [], []
        for i, step, pre, post in pairs:
            if step["type"] == "patch_insn_reg_field":
                paint_steps.append((i, words_of_text(pre), words_of_text(post),
                                    pre, post))
            elif step["type"] == "convert_ori_to_addiu":
                ori_steps.append((i, words_of_text(pre), words_of_text(post)))

        final_w = words_of_text(info["final"])
        for i, pw, ow in ori_steps:
            findings += check_ori(pw, ow, lo, hi, tw, i, final_w)

        if paint_steps:
            first_pre, last_post = paint_steps[0][3], paint_steps[-1][4]
            pre_w = paint_steps[0][1]
            post_w = words_of_text(last_post)
            rel_pre, rel_post = reloc_map(first_pre), reloc_map(last_post)
            hi_eff = min(hi, len(post_w), len(pre_w))
            site_f, paints = check_sites(
                [(i, a, b) for i, a, b, _, _ in paint_steps], tw, lo, rel_pre)
            findings += site_f
            info["n_paints"] = len(paints)
            info["n_misaligned"] = sum(
                1 for k in paints if any(p[5] for p in paints[k]))
            if len(pre_w) == len(post_w):
                findings += check_defuse(pre_w, post_w, paints, lo, hi_eff, tw)
                findings += check_hilo(pre_w, post_w, rel_pre, rel_post,
                                       lo, hi_eff)
            else:
                findings.append({"sev": "INFO", "kind": "LENGTH_SHIFT",
                                 "msg": "stream length changed inside the "
                                        "chain; def-use check skipped"})
            n = min(len(pre_w) - lo, len(tw))
            if n > 0:
                same = sum(1 for k in range(n)
                           if (pre_w[lo + k] >> 26) == (tw[k] >> 26))
                info["prepaint_opcode_align"] = round(100.0 * same / n, 1)
            info["len_pre"] = len(pre_w) - lo
            info["len_target"] = len(tw)
    finally:
        if workroot is None:
            shutil.rmtree(tmp, ignore_errors=True)
    return findings, info


def run(ov, func, verbose=False):
    findings, info = analyse(ov, func, verbose=verbose)
    high = [f for f in findings if f.get("sev") == "HIGH"]
    print("%-9s %-16s %-5s align=%s%% paints=%s misaligned=%s "
          "findings=%d high=%d"
          % (ov, func, "FLAG" if high else ("note" if findings else "clean"),
             info.get("prepaint_opcode_align"), info.get("n_paints"),
             info.get("n_misaligned"), len(findings), len(high)))
    for f in findings:
        if f.get("sev") == "INFO":
            continue
        print("    [%s] %s @%s %s: %s"
              % (f.get("sev"), f.get("kind"), f.get("off", "-"),
                 f.get("mnem", ""), f.get("msg", "")))
    return findings, info


def main():
    args = [a for a in sys.argv[1:] if a != "-v"]
    verbose = "-v" in sys.argv[1:]
    os.chdir(ROOT)
    bad = 0
    if args and args[0] == "--list":
        results = {}
        for line in Path(args[1]).read_text().split("\n"):
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            ov, func = line.split()[:2]
            try:
                f, i = run(ov, func, verbose)
            except Exception as e:
                print("%-9s %-16s ERROR %s" % (ov, func, e))
                continue
            results[ov + "/" + func] = {
                "findings": f,
                "align": i.get("prepaint_opcode_align"),
                "n_paints": i.get("n_paints"),
                "n_misaligned": i.get("n_misaligned")}
            if any(x.get("sev") == "HIGH" for x in f):
                bad += 1
        Path("wip-scratch/defuse_report.json").write_text(
            json.dumps(results, indent=1, default=str))
        print("\nflagged (HIGH): %d" % bad)
    else:
        f, _ = run(args[0], args[1], verbose)
        bad = sum(1 for x in f if x.get("sev") == "HIGH")
    return 1 if bad else 0


if __name__ == "__main__":
    raise SystemExit(main())
