#!/usr/bin/env python3
"""
gccwrap.py - Wraps the full PSY-Q GCC compilation pipeline into a single command.

Pipeline:
  1. Preprocess    (mipsel-linux-gnu-cpp)
  2. Compile       (psyq cc1 for the selected GCC version)
  3. maspsx        (maspsx.py post-processing)
  (--codegen ido52 replaces 1-2 with IDO 5.2 `cc -S`: the hybrid pipeline
   for retail's library-region IDO code, which ASPSX assembled -- `move`
   becomes addu, wide immediates go through $at unnarrowed, and delay
   slots are filled by the assembler's reorder rules, not as1's XBB.
   --assembler gas skips maspsx and lets GNU as's reorder mode do it.)
  4. Assemble      (mipsel-linux-gnu-as)
  5. Delay slots   (fill_gcc_delay_slots.py)

Usage:
  python3 tools/gcc/gccwrap.py source.c -o output.o [options]

Options:
  --gcc-version 260|263|263p|272|281   Select GCC version (default: 260)
  --include-dir DIR           Extra include directory (repeatable)
  --extra-flags FLAGS         Extra cc1 flags (space-separated string)
  --aspsx-version VER         maspsx aspsx version (default: 2.67)
  --no-delay-slots            Skip fill_gcc_delay_slots step
  -v, --verbose               Print each pipeline step
  -o OUTPUT                   Output .o file (required)
"""

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile

# Auto-detect repository root (this script lives at tools/gcc/gccwrap.py)
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.normpath(os.path.join(SCRIPT_DIR, "..", ".."))

# GCC cc1 paths relative to ROOT_DIR
GCC_CC1_PATHS = {
    "260": "tools/psyq-gcc-260/cc1",
    "263": "tools/psyq-gcc-260/cc1-psx-26",  # GNU C 2.6.3 (the Psy-Q "cc1-psx-26" build)
    # GNU C 2.6.3 rebuilt from source (x86_64) with gated decision flags
    # (-fstack-args-first, ...); byte-identical to cc1-psx-26 with them off.
    # Recipe: tools/psyq-gcc-260/README.md.
    "263p": "tools/psyq-gcc-260/cc1-psx-26-patched",
    "272": "tools/psyq-gcc-272/cc1",
    "281": "tools/psyq-gcc/cc1",
}

# Modified cc1 with -fno-cross-jumping support (64-bit rebuild from patched source)
GCC_CC1_NOCJ_PATHS = {
    "260": "tools/psyq-gcc-260/cc1-nocj",
    "272": "tools/psyq-gcc-272/cc1-nocj",
}

# Default cc1 flags
DEFAULT_CC1_FLAGS = ["-quiet", "-O2", "-G0", "-mips1", "-mcpu=3000", "-mgas", "-fgnu-linker"]

# maspsx path relative to ROOT_DIR
MASPSX_PATH = "tools/maspsx/maspsx.py"

# IDO 5.2 cc (the --codegen ido52 hybrid: IDO codegen, PSY-Q-style assembly)
IDO_CC_PATH = "tools/ido-static-recomp/build/5.2/out/cc"
IDO_BASE_FLAGS = ["-O2"]
IDO_TAIL_FLAGS = ["-G0", "-mips1", "-EL", "-non_shared"]

# Directives in IDO's `-S` output that the PSY-Q-style assemblers do not
# understand (or that carry no code).  `.livereg` is as1-only liveness data;
# `.loc`/`.file` refer to IDO's own file-number table; `.option O2` and
# `.verstamp` are as1 knobs.  Everything else (.ent/.end/.frame/.mask/.globl,
# the section and data directives) is ordinary MIPS assembly.
IDO_S_DROP_DIRECTIVES = (".loc", ".livereg", ".option", ".verstamp", ".file",
                         ".alias", ".noalias")

# fill_gcc_delay_slots path relative to ROOT_DIR
FILL_DELAY_SLOTS_PATH = "tools/ido/fill_gcc_delay_slots.py"


def resolve(rel_path):
    """Resolve a path relative to ROOT_DIR."""
    return os.path.join(ROOT_DIR, rel_path)


def build_cpp_cmd(include_flags, source):
    """Build the preprocess command.

    The pipeline prefers the cross preprocessor, but it is used ONLY as a
    plain text preprocessor: `-undef` suppresses every target predefine and
    all headers come from explicit `-I` flags, so no MIPS-specific behaviour
    is involved.  Many checkouts have the mipsel *binutils* installed without
    the mipsel *gcc* cross package, in which case `mipsel-linux-gnu-cpp` does
    not exist and every `@Compiler: gcc*` file fails to compile.  Fall back to
    the host cpp in traditional mode (the command documented in CLAUDE.md:
    `cpp -traditional -nostdinc -I include`), which is equivalent here.
    """
    cross = shutil.which("mipsel-linux-gnu-cpp")
    if cross:
        return [cross] + include_flags + ["-lang-c", "-undef", source]

    host = shutil.which("cpp")
    if not host:
        raise SystemExit(
            "gccwrap: no preprocessor found (tried mipsel-linux-gnu-cpp and cpp).\n"
            "  Install gcc-mipsel-linux-gnu, or make a plain `cpp` available."
        )
    # `-lang-c` is a GCC-2.x-only spelling that modern cpp rejects; traditional
    # mode plus -nostdinc reproduces the old preprocessor's behaviour.
    return [host, "-traditional-cpp", "-nostdinc"] + include_flags + ["-undef", source]


def ido_codegen_to_s(source, ido_flags, tmpdir, verbose=False):
    """Run IDO 5.2 `cc -S` on `source` and return the filtered .s text.

    IDO's cc ignores -o with -S and writes <stem>.s into the cwd, so the
    compile runs inside tmpdir.  The @CompileEnv hook variables reach uopt/
    ugen through the inherited environment exactly as in the IDO pipeline.
    The result is IDO's reorder-mode stream: no nops, no delay-slot fills --
    every hazard is left to the assembler, which is the whole point of the
    hybrid (retail's library-region IDO code was assembled by ASPSX, not as1).
    """
    cc_path = resolve(IDO_CC_PATH)
    if not os.path.isfile(cc_path):
        print(f"Error: IDO cc not found at {cc_path} (make setup-ido52)", file=sys.stderr)
        sys.exit(1)
    stem = os.path.splitext(os.path.basename(source))[0]
    cmd = ([cc_path] + IDO_BASE_FLAGS + list(ido_flags) + IDO_TAIL_FLAGS
           + ["-S", "-I", resolve("include"), source])
    if verbose:
        print(f"[ido -S] {' '.join(cmd)}", file=sys.stderr)
    result = subprocess.run(cmd, cwd=tmpdir, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    s_path = os.path.join(tmpdir, stem + ".s")
    if result.returncode != 0 or not os.path.isfile(s_path):
        print("ERROR in step 'ido -S':", file=sys.stderr)
        print(result.stderr.decode("utf-8", errors="replace"), file=sys.stderr)
        sys.exit(result.returncode or 1)
    out = []
    with open(s_path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            stripped = line.lstrip()
            if stripped.startswith("#"):
                continue          # IDO's source-line echo comments
            if stripped.startswith(IDO_S_DROP_DIRECTIVES):
                continue
            # `.set volatile` / `.set novolatile` bracket every volatile
            # access in IDO's stream.  They are as1 pins, and maspsx's
            # one-line hazard lookahead reads the directive as the next
            # instruction and drops the load-delay nop.  GNU as accepts
            # them but they carry nothing, so they go unconditionally.
            if re.match(r"\.set\s+(no)?volatile\b", stripped):
                continue
            out.append(line)
    return "".join(out).encode("utf-8")



# ---------------------------------------------------------------------------
# ASPSX-style load-delay fill for IDO's reorder-mode stream (--ido-fill-load-delay)
#
# IDO's `-S` output leaves every load-delay hazard to the assembler.  GNU as
# resolves one by inserting a nop; retail's assembler instead hoisted the
# NEAREST later instruction of the same basic block that could legally move
# into the slot -- a later independent load as readily as an ALU word
# (main/_spu_getCommonAttr: `lhu t2 ; lhu t5 ; and t3,t2` and `lhu t8 ; move v0,0 ;
# and t9,t8`).  This pass does that on the text before GNU as sees it, so as
# then finds no hazard and emits no nop.  It never moves a store, a branch, a
# jump, a macro it does not understand, or anything across a label.
# ---------------------------------------------------------------------------
_LOADS = {"lw", "lh", "lhu", "lb", "lbu", "lwl", "lwr"}
_STORES = {"sw", "sh", "sb", "swl", "swr"}
_ALU = {"addu", "subu", "and", "or", "xor", "nor", "sll", "srl", "sra", "sllv",
        "srlv", "srav", "addiu", "andi", "ori", "xori", "slt", "sltu", "slti",
        "sltiu", "lui", "li", "move", "negu", "not", "add", "sub", "addi"}
_BRANCH_RE = re.compile(r"^(b|j|jal|jr|jalr|beq|bne|beqz|bnez|blez|bgtz|bltz|bgez|"
                        r"bltzal|bgezal|bc\w*|beql|bnel)$")
_REG_RE = re.compile(r"\$(\w+)")


def _parse_insn(line):
    """-> (mnemonic, [operand strings]) or None for blank/label/directive."""
    t = line.split("#", 1)[0].strip()
    if not t or t.endswith(":") or t.startswith("."):
        return None
    parts = t.split(None, 1)
    mnem = parts[0]
    ops = [o.strip() for o in parts[1].split(",")] if len(parts) > 1 else []
    return mnem, ops


def _regs_of(op):
    return set(_REG_RE.findall(op))


def _is_symbol_load(mnem, ops):
    """`lw rd,SYMBOL` -- a two-instruction lui/lw macro, never a slot filler."""
    return mnem in _LOADS and len(ops) > 1 and not re.search(r"\(\s*\$", ops[1])


_MACRO_LUI_FILL = False       # --ido-fill-macro-lui
_SYM_OPND_RE = re.compile(r"^([A-Za-z_.$][\w.$]*(?:\s*[-+]\s*(?:0x[0-9a-fA-F]+|\d+))?)"
                          r"\s*(?:\(\s*(\$\w+)\s*\))?$")


def _la_macro(mnem, ops):
    """`la rd,SYM` (no base register) -> (rd, SYM), else None."""
    if mnem != "la" or len(ops) != 2:
        return None
    m = _SYM_OPND_RE.match(ops[1].strip())
    if not m or m.group(2):
        return None
    return ops[0].strip(), m.group(1)


def _split_symbol_macro(mnem, ops):
    """Split a symbol-address macro into its `lui` half and the rest, the
    way GNU as would expand it (--ido-fill-macro-lui).

    Handles `la rd,SYM` and the loads `l{w,h,hu,b,bu} rd,SYM[(rB)]`.  The
    macro's scratch register is its destination; a load whose destination
    is also its index register would need $at and is left alone, as is any
    macro writing $0.  Returns (temp, lui_line, [rest lines], (defs, uses))
    or None.
    """
    la = _la_macro(mnem, ops)
    if la is not None:
        rd, sym = la
        if rd in ("$0", "$zero"):
            return None
        return (rd, "lui\t%s,%%hi(%s)" % (rd, sym),
                ["addiu\t%s,%s,%%lo(%s)" % (rd, rd, sym)], (_regs_of(rd), set()))
    if mnem not in ("lw", "lh", "lhu", "lb", "lbu") or len(ops) != 2:
        return None
    if not _is_symbol_load(mnem, ops):
        # `lh rd,SYM($rB)` is a macro too: an index register with a symbol
        m = _SYM_OPND_RE.match(ops[1].strip())
        if not m or not m.group(2) or re.match(r"^-?(0x[0-9a-fA-F]+|\d+)$", m.group(1)):
            return None
    else:
        m = _SYM_OPND_RE.match(ops[1].strip())
        if not m:
            return None
    rd = ops[0].strip()
    sym, base = m.group(1), m.group(2)
    if rd in ("$0", "$zero") or (base is not None and _regs_of(base) & _regs_of(rd)):
        return None
    rest = []
    if base is not None:
        rest.append("addu\t%s,%s,%s" % (rd, rd, base))
    rest.append("%s\t%s,%%lo(%s)(%s)" % (mnem, rd, sym, rd))
    return (rd, "lui\t%s,%%hi(%s)" % (rd, sym), rest,
            (_regs_of(rd), _regs_of(base) if base else set()))


def _defs_uses(mnem, ops):
    """Registers written / read by one simple instruction (None = unknown)."""
    if mnem in _LOADS:
        return _regs_of(ops[0]), _regs_of(ops[1]) if len(ops) > 1 else set()
    if mnem in _STORES:
        return set(), _regs_of(ops[0]) | (_regs_of(ops[1]) if len(ops) > 1 else set())
    if mnem in _ALU:
        d = _regs_of(ops[0]) if ops else set()
        u = set()
        for o in ops[1:]:
            u |= _regs_of(o)
        return d, u
    return None, None



_MUL_RE = re.compile(r"^(\s*)mul\s+(\$\w+)\s*,\s*(\$\w+)\s*,\s*(-?\d+)\s*$", re.M)
_DIV_RE = re.compile(r"^(\s*)div\s+(\$\w+)\s*,\s*(\$\w+)\s*,\s*(\d+)\s*$", re.M)
_REM_RE = re.compile(r"^(\s*)rem\s+(\$\w+)\s*,\s*(\$\w+)\s*,\s*(\d+)\s*$", re.M)
_ido_div_label = [0]
_DIV_RESIDUE = False          # --ido-div-macro-residue
_HOIST_LOAD_ABOVE = False     # --ido-hoist-load-above


def ido_expand_pow2_macros(text):
    """Retail's assembler expands ugen's constant-operand `mul`/`div` macros
    for a power of two the way ASPSX does: `mul rd,rs,2^k` is one `sll`, and
    the signed `div rd,rs,2^k` is
        bgez rs,1f ; addu $at,rs,$0 (slot) ; addiu $at,$at,2^k-1 ; 1: sra rd,$at,k
    (main/get_dx words 34-37 and 43-47; GNU as would emit a real
    mult/div with the constant in $at, which retail never has).  The `move`
    is written as addu so it carries retail's encoding, and the block is
    bracketed in .set noat so GNU as does not warn about the explicit $at.
    The signed `rem rd,rs,2^k` is
        bgez rs,1f ; andi rd,rs,2^k-1 (slot) ; beqz rd,1f ; nop ; addiu rd,rd,-2^k ; 1:
    (main/_drs words 132-136; see ``rem`` below).

    ``--ido-div-macro-residue`` (``_DIV_RESIDUE``) additionally models how many
    words the expansion occupies in retail, which the bare form above gets
    wrong by one or two.  Measured on main/get_dx, whose two pow2
    divides are the only ones in the corpus with a retail reference:

      * retail ALWAYS has one idle ``nop`` immediately before the ``bgez``
        (words 33 and 43).  That is the residue of the slot fill: retail's
        assembler COPIES the word above the branch into the delay slot and
        leaves it in place as a ``nop`` rather than compacting the stream --
        the same non-compacting fill ``--ido-copy-and-bump`` already models
        for an unconditional branch.  GNU as compacts, so the word has to be
        emitted for it.
      * when the divide's SOURCE was loaded by the immediately preceding
        instruction retail has a SECOND nop (word 42, after ``lh v1,0(a0)``).
        The macro is atomic for hazard purposes: the load delay is counted
        against ``addu $at,rs,$0``, the macro's real first word, not against
        the residue nop.  GNU as would see our residue nop as satisfying the
        hazard and emit nothing, so that word has to be emitted too.
    """
    def mul(m):
        ind, rd, rs, imm = m.groups()
        v = int(imm)
        if v > 0 and (v & (v - 1)) == 0:
            return "%ssll %s, %s, %d" % (ind, rd, rs, v.bit_length() - 1)
        if v > 0:
            return _mul_const_shift_add(ind, rd, rs, v)
        return m.group(0)

    def div(m):
        ind, rd, rs, imm = m.groups()
        v = int(imm)
        if v > 1 and (v & (v - 1)) == 0:
            k = v.bit_length() - 1
            _ido_div_label[0] += 1
            lab = "$Ldiv%d" % _ido_div_label[0]
            head = ""
            if _DIV_RESIDUE:
                nops = 1
                prev = _prev_insn_before(text, m.start())
                if prev is not None and prev[0] in _LOADS and prev[1] \
                        and rs.lstrip("$") in _regs_of(prev[1][0]):
                    nops = 2        # macro-atomic load delay + the residue
                head = ("%snop\n" % ind) * nops
            return (head +
                    "%s.set noat\n%saddu $at, %s, $0\n%sbgez %s, %s\n"
                    "%saddiu $at, $at, %d\n%s:\n%ssra %s, $at, %d\n%s.set at"
                    % (ind, ind, rs, ind, rs, lab, ind, v - 1, lab, ind, rd, k, ind))
        return m.group(0)

    def rem(m):
        # Signed `rem rd,rs,2^k` (main/_drs words 129-136, the
        # `n % 16` drain count of libgpu StoreImage):
        #     bgez rs,L ; andi rd,rs,2^k-1 (slot) ; beqz rd,L ; nop ;
        #     addiu rd,rd,-2^k ; L:
        # i.e. the low bits, fixed up by -2^k when rs is negative and the
        # remainder is nonzero.  The andi sits IN the bgez delay slot and may
        # overwrite rs (retail: rd == rs == a0), so the two branches are
        # emitted in an explicit .set noreorder bracket -- GNU as could not
        # move the andi into the slot itself when it writes the register the
        # branch tests.  The .set noat bracket (no $at is used) marks the
        # block as an assembler macro for the passes that key on it
        # (_feeds_noat_macro), and --ido-div-macro-residue applies its
        # residue-nop rule unchanged: retail has TWO nops before the bgez
        # (words 130-131) because the operand was loaded by the word above
        # (`lw a0,60(sp)`) -- the same shape as the pow2 div.
        ind, rd, rs, imm = m.groups()
        v = int(imm)
        if 1 < v <= 0x8000 and (v & (v - 1)) == 0:
            _ido_div_label[0] += 1
            lab = "$Lrem%d" % _ido_div_label[0]
            head = ""
            if _DIV_RESIDUE:
                nops = 1
                prev = _prev_insn_before(text, m.start())
                if prev is not None and prev[0] in _LOADS and prev[1] \
                        and rs.lstrip("$") in _regs_of(prev[1][0]):
                    nops = 2        # macro-atomic load delay + the residue
                head = ("%snop\n" % ind) * nops
            return (head +
                    "%s.set noat\n%s.set noreorder\n"
                    "%sbgez %s, %s\n%sandi %s, %s, %d\n"
                    "%sbeqz %s, %s\n%snop\n%s.set reorder\n"
                    "%saddiu %s, %s, %d\n%s:\n%s.set at"
                    % (ind, ind,
                       ind, rs, lab, ind, rd, rs, v - 1,
                       ind, rd, lab, ind, ind,
                       ind, rd, rd, -v, lab, ind))
        return m.group(0)

    text = _MUL_RE.sub(mul, text)
    text = _DIV_RE.sub(div, text)
    text = _REM_RE.sub(rem, text)
    return text


def _naf_digits(v):
    """Non-adjacent form of ``v`` > 0: [(bit, +1|-1), ...], highest bit first."""
    out = []
    bit = 0
    while v:
        if v & 1:
            d = 2 - (v & 3)          # 1 when v%4 == 1, -1 when v%4 == 3
            out.append((bit, d))
            v -= d
        v >>= 1
        bit += 1
    return out[::-1]


def _mul_const_shift_add(ind, rd, rs, v):
    """ASPSX's expansion of ugen's `mul rd,rs,IMM` for a non-power-of-two
    IMM > 0 (GNU as would emit `li $at,IMM ; mult ; mflo`, a SIGNED multiply
    retail never contains: the whole retail image has no mult/multu against
    an immediate loaded into $at).

    IMM = m << s with m odd.  m is written in non-adjacent form (signed
    binary digits, no two adjacent non-zero) and evaluated Horner-style from
    the top digit: one `sll` per gap between non-zero digits, then `addu` or
    `subu` of the operand; finally `sll rd,rd,s`.  Retail sites, all in
    ASPSX-assembled library functions:

      x*16383 = 2^14-1   vmNoiseOn 0x8008D0AC, 0x8008D0C0 (rd == rs)
                         move at,a1 ; sll a1,at,14 ; subu a1,a1,at
      x*24 = 3<<3        vmNoiseOn 0x8008D31C (rd != rs)
                         sll t7,a0,2 ; subu t7,t7,a0 ; ... ; sll t7,t7,3
                         (3 is 4-1, not 2+1: the NAF choice)
      x*60 = 15<<2       GetMetaEvent 0x800946F8, 0x800947BC (rd != rs),
                         0x800947F0 (rd == rs); InitSoundSeq 0x800914FC
                         and InitSoundSep 0x8009195C (rd == rs)
      x*10 = 5<<1        GetMetaEvent 0x80094708 (rd != rs), 0x800947CC
                         (rd == rs); 0x800A0730 (rd == rs)
      x*168 = 21<<3      GetMetaEvent 0x800948C0.. (x5, rd != rs)
                         sll ; addu ; sll 2 ; addu ; sll 3
      x*600 = 75<<3      GetMetaEvent 0x80094748 (rd != rs)
                         sll 2 ; addu ; sll 2 ; subu ; sll 2 ; subu ; sll 3
                         (75 = 64+16-4-1)

    When rd != rs the chain works in rd and re-reads rs.  When rd == rs the
    first `sll` would destroy the operand, so retail first copies it to
    $at (`move at,rs`, encoded addu like every ASPSX move) and every
    add/subtract term reads $at -- that copy is what makes the aliased form
    safe.  Only that form needs a `.set noat` bracket, which also makes it
    a block boundary for the load-delay fill passes; retail DID schedule an
    unrelated load into one such chain (0x800A0730: move at,t1 ; sll t1,at,2
    ; lbu a0,18(a1) ; addu t1,t1,at ; sll t1,t1,1), so the bracketed form
    is atomic where retail was not.  The rd != rs form is plain words the
    fill passes may interleave, which retail also did (0x80094748 multu,
    0x800948CC lw inside the chain).
    """
    s = (v & -v).bit_length() - 1
    digits = _naf_digits(v >> s)
    alias = rd == rs
    src = "$at" if alias else rs
    out = []
    if alias:
        out.append(".set noat")
        out.append("addu $at, %s, $0" % rs)
    top = digits[0][0]
    cur = src
    for bit, d in digits[1:]:
        out.append("sll %s, %s, %d" % (rd, cur, top - bit))
        out.append("%s %s, %s, %s" % ("addu" if d > 0 else "subu", rd, rd, src))
        cur = rd
        top = bit
    if alias:
        out.append(".set at")
    if s:
        out.append("sll %s, %s, %d" % (rd, rd, s))
    return "\n".join(ind + x for x in out)


def _prev_insn_before(text, pos):
    """The parsed instruction on the last non-blank line before ``pos``.

    ``None`` when that line is a label, a directive or a comment -- callers
    use it to ask "was this operand produced by the word right above me?",
    which a block boundary answers in the negative.
    """
    for line in reversed(text[:pos].split("\n")):
        if not line.strip():
            continue
        return _parse_insn(line)
    return None



_COND_BRANCH_RE = re.compile(r"^(beq|bne|beqz|bnez|blez|bgtz|bltz|bgez|b|j)$")
_ido_cb_label = [0]


def _gas_slot_empty(lines, bi, br_uses):
    """Would GNU as leave this branch's delay slot empty?  It fills the slot
    with the instruction just above the branch when that instruction is in
    the same block, is a single non-branch instruction and does not write a
    register the branch reads."""
    a = bi - 1
    while a >= 0 and not lines[a].strip():
        a -= 1
    if a < 0:
        return True
    pa = _parse_insn(lines[a])
    if pa is None:
        return True                     # label or directive above
    if _BRANCH_RE.match(pa[0]):
        return True
    d, u = _defs_uses(pa[0], pa[1])
    if d is None:
        return True                     # a macro: gas does not split one into a slot
    if pa[0] in _LOADS and len(pa[1]) > 1 and not re.search(r"\(\s*\$", pa[1][1]):
        return True                     # `lw rd,SYMBOL` symbol macro
    return bool(d & br_uses)


def _feeds_noat_macro(lines, k):
    """Does the load at ``lines[k]`` feed an assembler MACRO right below it?

    Detected by the `.set noat` bracket that ``ido_expand_pow2_macros`` and
    ``ido_expand_wide_logic`` put around every expansion (nops between the
    load and the bracket are the macro's own residue, so they are skipped).

    This is what separates the two load-delay fills in main/get_dx.
    A macro is atomic, so its operand load cannot have its own delay filled
    from inside it, and retail's assembler schedules that load EARLY instead
    of using it to fill somebody else's slot:

      case 1  `lh t7,0(a0) ; negu t8,t7 ; lh t9,4(a0) ; subu v1,t8,t9`
              -- t9 feeds an ordinary `subu`, and retail SINKS the load into
              t7's slot: `lh t7 ; lh t9 ; negu ; subu`.
      case 2  `lh t1,0(a0) ; negu t2,t1 ; lh t3,4(a0) ; div t4,t3,2`
              -- t3 feeds the div macro, and retail HOISTS the load above
              `lh t1`, whose slot is then padded:
              `lh t3 ; lh t1 ; nop ; negu`.

    IDO as1 schedules both blocks exactly the same way, which is what says
    this is a real assembler rule rather than a fit to one function.
    """
    j = k + 1
    while j < len(lines):
        t = lines[j].split("#", 1)[0].strip()
        if not t or t == "nop":
            j += 1
            continue
        return t.startswith(".set noat")
    return False


def ido_label_load_nop(text):
    """ASPSX flushes a pending load delay at a LABEL (--ido-label-load-nop).

    The assembler's hazard tracking is per basic block, so a load that is the
    last word before a label gets its delay slot padded rather than left for
    whatever falls through.  GNU as does not: it drops the tracking at the
    label and emits nothing.

    Evidence: main/get_dx words 50-53, the switch's default arm.  ugen
    emits `lh $2,0($4)` then the epilogue label `$39:` then `j $31`; retail
    has `lh v0,0(a0) ; nop ; jr ra ; nop` and GNU as gives three words.  IDO
    as1 pads it too (checked against the IRIX 5.2 as1 binary under qemu-irix),
    which is why this is a gap in the gas emulation rather than a guess.

    The nop is inserted BEFORE the label, so it cannot be pulled into the
    following jump's delay slot -- that would be a cross-block fill, which
    GNU as does not do.
    """
    lines = text.split("\n")
    out = []
    for i, line in enumerate(lines):
        out.append(line)
        p = _parse_insn(line)
        if not p or p[0] not in _LOADS:
            continue
        j = i + 1
        while j < len(lines) and not lines[j].strip():
            j += 1
        if j < len(lines) and re.match(r"^\s*\$?[\w.$]+:\s*$", lines[j]):
            out.append(re.match(r"^(\s*)", line).group(1) + "nop")
    return "\n".join(out)


_AT_NEXT_FILL = False         # --ido-fill-at-next
_STORE_SYM_RE = re.compile(r"^(sw|sh|sb)$")


def _at_only_head(line):
    """If the instruction on ``line`` begins with a word that writes ONLY the
    assembler temporary ``$at`` (and does not read it), return
    (slot_line, rest_lines), else None.

    * an explicit `lui/addiu/ori/slti/sltiu/slt/sltu $at,...` (ugen's own
      `$at` compares, and the halves ``ido_expand_branch_macros`` /
      ``ido_expand_wide_logic`` already spelled out) -- the whole word;
    * a symbol store `s{w,h,b} rX,SYM[(rB)]`, a macro that GNU as and ASPSX
      both expand as `lui $at,%hi(SYM) ; [addu $at,$at,rB ;] sX rX,%lo(SYM)($at)`
      -- split, the `lui` half goes to the slot.
    Symbol LOADS use their destination as the scratch register, not $at, so
    they never qualify.
    """
    p = _parse_insn(line)
    if p is None:
        return None
    mnem, ops = p
    ind = line[:len(line) - len(line.lstrip())]
    if mnem in ("lui", "addiu", "ori", "slti", "sltiu", "slt", "sltu") and ops:
        if _regs_of(ops[0]) != {"at"}:
            return None
        uses = set()
        for o in ops[1:]:
            uses |= _regs_of(o)
        if "at" in uses:
            return None                 # `addiu at,at,K`: the macro's own tail
        return line, []
    if _STORE_SYM_RE.match(mnem) and len(ops) == 2:
        m = _SYM_OPND_RE.match(ops[1].strip())
        if not m or re.match(r"^-?(0x[0-9a-fA-F]+|\d+)$", m.group(1)):
            return None                 # `sw rX,off(rB)`: a plain store
        sym, base = m.group(1), m.group(2)
        if "at" in _regs_of(ops[0]) or (base and "at" in _regs_of(base)):
            return None
        rest = []
        if base:
            rest.append("%saddu\t$at,$at,%s" % (ind, base))
        rest.append("%s%s\t%s,%%lo(%s)($at)" % (ind, mnem, ops[0], sym))
        return "%slui\t$at,%%hi(%s)" % (ind, sym), rest
    return None


def ido_at_next_fill(text):
    """ASPSX fills an otherwise-empty CONDITIONAL branch delay slot with the
    NEXT word when that word writes only ``$at`` (--ido-fill-at-next).

    The assembler owns ``$at`` and treats it as dead at every label, so the
    fall-through's first `$at` write may execute on the taken path too.  It
    does so even when the branch itself reads ``$at`` (operands are read
    before the slot executes -- legal MIPS I).  GNU as never fills from
    below.  Retail evidence (all ASPSX-assembled IDO):

      SsSetTickMode  `slti at ; beqz at ; lui at,0x8013 ; sw v0,..(at)` and
                     `beqz v0 ; li at,1 ; beq v0,at` (twice) -- the $at
                     word wins over a legal copy of the target's `li t3,60`;
                     `beqz at ; nop ; sll t8,..` (next word not $at: empty)
      _spu_ioctl  the sparse switch's binary search, 26 slots:
                     `beq a1,at ; sltiu at,a1,0x61 ; beqz at ; li at,0x40`,
                     and `bnez t4 ; lui at,0x800f ; b ; sw ..(at)`
      _spu_setReverbAttr  `bnez v1 ; lui at,1 ; and t8,v0,at` (x16)
      LoadTPage  `beqz v1 ; li at,1 ; beq v1,at ; li at,2 ; beq v1,at`
      _gcSPU  `blez a0 ; lui at,0x800f ; sw a1,..(at)`
      get_dx  `beq v0,at ; li at,2 ; beq v0,at`

    Runs before --ido-copy-and-bump, which then leaves these branches alone
    (the $at word has priority over a copy).  Only fires when GNU as would
    leave the slot empty, never across a label or directive, never for an
    unconditional b/j (its next word is another block), and never for a
    word that also reads $at (a macro's own tail, e.g. the div expansion's
    `addiu $at,$at,K`).  The branch and its filler are emitted as an
    explicit .set noreorder pair; a load just above the branch that feeds
    it gets its delay nop made explicit.
    """
    lines = text.split("\n")
    out = []
    n = len(lines)
    noat = False
    i = 0
    while i < n:
        ln = lines[i]
        t = ln.split("#", 1)[0].strip()
        if t.startswith(".set"):
            if t.split()[1:2] == ["noat"]:
                noat = True
            elif t.split()[1:2] == ["at"]:
                noat = False
        pi = _parse_insn(ln)
        if (noat or pi is None or not _COND_BRANCH_RE.match(pi[0])
                or pi[0] in ("b", "j") or not pi[1]):
            out.append(ln); i += 1; continue
        br_uses = set()
        for o in pi[1][:-1]:
            br_uses |= _regs_of(o)
        if not _gas_slot_empty(lines, i, br_uses):
            out.append(ln); i += 1; continue
        f = i + 1
        while f < n and not lines[f].strip():
            f += 1
        head = _at_only_head(lines[f]) if f < n else None
        if head is None:
            out.append(ln); i += 1; continue
        slot, rest = head
        ind = ln[:len(ln) - len(ln.lstrip())]
        a = i - 1
        while a >= 0 and not lines[a].strip():
            a -= 1
        if a >= 0:
            pa = _parse_insn(lines[a])
            if pa is not None and pa[0] in _LOADS and pa[1] and (_regs_of(pa[1][0]) & br_uses):
                out.append(ind + "nop")
        out.append(ind + ".set noreorder")
        out.append(ln)
        out.append(slot)
        out.append(ind + ".set reorder")
        out.extend(rest)
        lines[f] = ""
        i = f + 1
    return "\n".join(out)


_COPY_DEAD_FT = False         # --ido-copy-dead-ft
_DEFER_DIV = False            # --defer-div (ASPSX division schedule; see maspsx.defer_div_macros)
_REG_NUM = {"zero": "0", "at": "1", "v0": "2", "v1": "3", "a0": "4", "a1": "5",
            "a2": "6", "a3": "7", "t0": "8", "t1": "9", "t2": "10", "t3": "11",
            "t4": "12", "t5": "13", "t6": "14", "t7": "15", "s0": "16", "s1": "17",
            "s2": "18", "s3": "19", "s4": "20", "s5": "21", "s6": "22", "s7": "23",
            "t8": "24", "t9": "25", "k0": "26", "k1": "27", "gp": "28", "sp": "29",
            "fp": "30", "s8": "30", "ra": "31"}
_CALL_CLOBBER = {"1", "2", "3", "8", "9", "10", "11", "12", "13", "14", "15", "24", "25"}
_RET_DEAD = _CALL_CLOBBER - {"2", "3"} | {"4", "5", "6", "7"}


def _norm_regs(regs):
    return {_REG_NUM.get(r, r) for r in regs}


def _ft_liveness(lines, labels, start, regs, depth=0, seen=None):
    """Is any of ``regs`` (normalised register numbers) live on entry to
    ``lines[start]``?  -> "dead" / "live" / "unknown".  A conservative
    forward scan of IDO's reorder-mode text: a read is "live", a write
    before any read is "dead"; a conditional branch needs both paths dead;
    `j $31` kills the temporaries and argument registers (not v0/v1); a
    call kills the caller-saved temporaries but reads a0-a3; anything the
    scan does not understand (a jump table, an unknown macro) is "unknown".
    """
    if seen is None:
        seen = set()
    if depth > 8:
        return "unknown"
    regs = set(regs)
    i = start
    n = len(lines)
    while i < n and regs:
        t = lines[i].split("#", 1)[0].strip()
        if not t or t.endswith(":") or t.startswith(".set"):
            if t.endswith(":"):
                if i in seen:
                    return "dead"           # a loop back to scanned code
                seen.add(i)
            i += 1
            continue
        p = _parse_insn(lines[i])
        if p is None:
            return "unknown"                # .end / .rdata / other directive
        mnem, ops = p
        if mnem in ("j", "jr") and ops and _norm_regs(_regs_of(ops[0])) == {"31"}:
            return "dead" if regs <= _RET_DEAD else "live"
        if mnem in ("j", "b") and ops and ops[0].strip() in labels:
            i = labels[ops[0].strip()] + 1
            if i - 1 in seen:
                return "dead"
            seen.add(i - 1)
            continue
        if mnem in ("jal", "jalr"):
            if regs & {"4", "5", "6", "7"}:
                return "live"
            if mnem == "jalr" and _norm_regs(_regs_of(ops[-1])) & regs:
                return "live"
            if regs <= _CALL_CLOBBER:
                return "dead"
            return "unknown"
        if _BRANCH_RE.match(mnem) or mnem in ("beql", "bnel", "bltu", "bgeu", "blt",
                                               "bge", "bgt", "ble", "bgtu", "bleu"):
            if mnem in ("j", "jr", "b"):
                return "unknown"            # jump table / unresolved label
            uses = set()
            for o in ops[:-1]:
                uses |= _norm_regs(_regs_of(o))
            if uses & regs:
                return "live"
            lab = ops[-1].strip()
            if lab not in labels:
                return "unknown"
            a = _ft_liveness(lines, labels, labels[lab] + 1, regs, depth + 1, seen)
            if a != "dead":
                return a
            i += 1
            continue
        if mnem in ("mult", "multu", "div", "divu") and len(ops) == 2:
            d, u = set(), set()
            for o in ops:
                u |= _regs_of(o)
        elif mnem in ("mflo", "mfhi"):
            d, u = _regs_of(ops[0]), set()
        elif mnem in ("mul", "div", "divu", "rem", "remu") and len(ops) == 3:
            d, u = _regs_of(ops[0]), _regs_of(ops[1]) | _regs_of(ops[2])
        elif mnem == "la" and len(ops) == 2:
            d, u = _regs_of(ops[0]), _regs_of(ops[1])
        elif mnem == "nop":
            d, u = set(), set()
        else:
            d, u = _defs_uses(mnem, ops)
            if d is None:
                return "unknown"
        d, u = _norm_regs(d), _norm_regs(u)
        if u & regs:
            return "live"
        regs -= d
        i += 1
    return "dead" if not regs else "unknown"


def ido_copy_and_bump(text):
    """ASPSX fills a delay slot that has nothing above it by COPYING the
    branch target's first instruction into the slot and bumping the branch
    past it, leaving the original in place (docs/reference/
    IDO-CODEGEN-ASPSX-ASSEMBLY.md; _spu_setCommonAttr words 2-3: `bnez v0 ;
    move v1,zero` with the original `move v1,zero` still at the old target).
    Only a single plain ALU word is copied, never a load, store or branch.
    The copy is placed just before the branch and GNU as's own reorder mode
    moves it into the slot (see the note at the emission site)."""
    lines = text.split("\n")
    labels = {}
    for idx, ln in enumerate(lines):
        t = ln.split("#", 1)[0].strip()
        if t.endswith(":") and not t.startswith("."):
            labels[t[:-1]] = idx
    out = []
    i = 0
    n = len(lines)
    while i < n:
        ln = lines[i]
        pi = _parse_insn(ln)
        if pi is None or not _COND_BRANCH_RE.match(pi[0]) or not pi[1]:
            out.append(ln); i += 1; continue
        if _AT_NEXT_FILL and out and out[-1].strip() == ".set noreorder":
            # slot already filled by --ido-fill-at-next (it has priority)
            out.append(ln); i += 1; continue
        target = pi[1][-1].strip()
        br_uses = set()
        for o in pi[1][:-1]:
            br_uses |= _regs_of(o)
        if target not in labels or not _gas_slot_empty(lines, i, br_uses):
            out.append(ln); i += 1; continue
        ind = ln[:len(ln) - len(ln.lstrip())]

        def fallthrough_at_fill():
            # The slot stays empty otherwise; ASPSX then pulls the NEXT
            # instruction up when it only defines the assembler temp $at
            # (the first half of an expanded macro): `bnez v1 ; lui at,0x1 ;
            # and t8,v0,at`.  Placed before the branch so GNU as fills.
            f = i + 1
            while f < n and not lines[f].strip():
                f += 1
            if f < n:
                pf = _parse_insn(lines[f])
                if pf is not None and pf[0] in ("lui", "addiu", "li") and pf[1] and _regs_of(pf[1][0]) == {"at"}:
                    fd, fu = _defs_uses(pf[0], pf[1])
                    if fd is not None and not (fu & {"at"}) and pf[0] != "li":
                        # The branch may itself read $at (an expanded
                        # `beq rs,IMM` compare), so the filler cannot sit
                        # before it; put it in the slot explicitly.  The
                        # bracket resets GNU as's previous-instruction state,
                        # which is right: the next branch's would-be filler
                        # is exactly the word consumed here.
                        out.append(ind + ".set noreorder")
                        out.append(ln)
                        out.append(lines[f])
                        out.append(ind + ".set reorder")
                        lines[f] = ""
                        return True
            return False

        # first instruction at the target label
        k = labels[target] + 1
        while k < n and not lines[k].strip():
            k += 1
        if k >= n:
            out.append(ln); i += 1; continue
        pk = _parse_insn(lines[k])
        # A `li rd,IMM32` at the target is the lui/ori pair to the assembler,
        # and ASPSX copies its FIRST half (_SsInit words 10-14: `b ;
        # lui v1,0x1f80` with `lui v1 ; ... ; ori v1,v1,0x1c00` at the
        # target).  Split it here so the lui is an ordinary copyable word.
        if pk is not None and pk[0] == "li" and len(pk[1]) == 2:
            try:
                val = int(pk[1][1].strip(), 0) & 0xFFFFFFFF
            except ValueError:
                val = None
            if val is not None and not (val < 0x8000 or val >= 0xFFFF8000 or (val <= 0xFFFF)):
                lind = lines[k][:len(lines[k]) - len(lines[k].lstrip())]
                lines[k] = "%slui\t%s,0x%x" % (lind, pk[1][0], val >> 16)
                if val & 0xFFFF:
                    lines.insert(k + 1, "%sori\t%s,%s,0x%x" % (lind, pk[1][0], pk[1][0], val & 0xFFFF))
                    n += 1
                    for lab, at in labels.items():
                        if at > k:
                            labels[lab] = at + 1
                pk = _parse_insn(lines[k])
        uncond = pi[0] in ("b", "j")
        # A conditional branch copies only a plain ALU word; an unconditional
        # one may also take the target's first LOAD (get_dx words 7-8:
        # `b default ; lh v0,0(a0)`, the load left in place at the target).
        copyable = pk is not None and (pk[0] in _ALU or (uncond and pk[0] in _LOADS
                                                          and not _is_symbol_load(pk[0], pk[1])))
        if _COPY_DEAD_FT and not uncond and pk is not None and pk[0] in _LOADS \
                and not _is_symbol_load(pk[0], pk[1]) and len(pk[1]) == 2 \
                and _regs_of(pk[1][1]) & {"sp", "29"}:
            copyable = True     # a $sp load cannot fault on the other path
        if not copyable:
            if not fallthrough_at_fill():
                out.append(ln)
            i += 1; continue
        kd, ku = _defs_uses(pk[0], pk[1])
        if _COPY_DEAD_FT and not uncond and kd is not None and \
                _ft_liveness(lines, labels, i + 1, _norm_regs(kd)) != "dead":
            if not fallthrough_at_fill():
                out.append(ln)
            i += 1; continue
        if kd is None or (kd & br_uses and not _COPY_DEAD_FT):
            if not fallthrough_at_fill():
                out.append(ln)
            i += 1; continue
        # The assembler's own slot fill runs first: if the target word would
        # be taken into the slot of the branch/jump that immediately follows
        # it (_spu_setCommonAttr words 200-201: `move v0,zero ; j $31` becomes
        # `jr ra ; move v0,zero`), the label's first instruction is that
        # jump by the time the copy is considered, and no copy is made.
        k2 = k + 1
        while k2 < n and not lines[k2].strip():
            k2 += 1
        if k2 < n:
            pk2 = _parse_insn(lines[k2])
            if pk2 is not None and _BRANCH_RE.match(pk2[0]):
                nxt_uses = set()
                for o in pk2[1][:-1] if pk2[0] not in ("j", "jr", "jalr", "jal") else pk2[1]:
                    nxt_uses |= _regs_of(o)
                if not (kd & nxt_uses):
                    if not fallthrough_at_fill():
                        out.append(ln)
                    i += 1; continue
        _ido_cb_label[0] += 1
        newlab = "$Lcb%d" % _ido_cb_label[0]
        new_br = ln.replace(target, newlab) if ln.count(target) == 1 else None
        if new_br is None:
            out.append(ln); i += 1; continue
        # Emit the copy just BEFORE the branch: GNU as's reorder mode then
        # moves it into the slot itself (it is independent of the branch by
        # the check above), keeps any load-delay nop the branch's operand
        # still needs, and its previous-instruction tracking survives for
        # the next branch -- a .set noreorder bracket would reset that and
        # cost the following branch its own fill.
        # A load just above the branch that feeds it still needs its delay
        # nop; make it explicit so GNU as does not spend the copy on it.
        a = i - 1
        while a >= 0 and not lines[a].strip():
            a -= 1
        if a >= 0:
            pa = _parse_insn(lines[a])
            if pa is not None and pa[0] in _LOADS and (_regs_of(pa[1][0]) & br_uses):
                out.append(ind + "nop")
        after_slot = False
        if _AT_NEXT_FILL:
            b = len(out) - 1
            while b >= 0 and not out[b].strip():
                b -= 1
            # the word above sits in an --ido-fill-at-next delay slot, and
            # GNU as will not move the word after a slot into the next slot
            after_slot = b >= 0 and out[b].strip() == ".set reorder"
        if _DEFER_DIV and not after_slot:
            # --defer-div: the word above is the last of a deferred division
            # check, a .set noreorder bracket; GNU as will not swap the word
            # after a noreorder block into the next branch's slot.
            b = len(out) - 1
            while b >= 0 and not out[b].strip():
                b -= 1
            after_slot = b >= 0 and out[b].strip() == ".set reorder" \
                and b >= 2 and out[b - 1].strip() == ".set at" \
                and out[b - 2].strip().startswith("break")
        if _COPY_DEAD_FT and (kd & br_uses):
            after_slot = True   # it overwrites a branch operand: slot only
        if pk[0] in _LOADS or after_slot:
            # GNU as will not sink a load into a jump's slot on its own;
            # place the copied load in the slot explicitly.
            out.append(ind + ".set noreorder")
            out.append(new_br)
            out.append(lines[k])
            out.append(ind + ".set reorder")
        else:
            out.append(lines[k])
            out.append(new_br)
        # place the new label right after the original target instruction
        lines.insert(k + 1, newlab + ":")
        n += 1
        for lab, at in labels.items():
            if at > k:
                labels[lab] = at + 1
        if k < i:
            i += 1          # a backward branch: the insert shifted us too
            # The target line was already copied to `out` before this
            # branch was reached, so the label inserted into `lines` above
            # would never be emitted (SsSetTickMode: undefined $Lcb2/3/4
            # and three unresolved branch relocs that the harness hid as
            # Fixup words).  Place it in `out` after the target's first
            # instruction; a wide `li` there is split so the label lands
            # between its lui and ori halves, as it does in `lines`.
            j = None
            for b in range(len(out) - 1, -1, -1):
                if out[b].split("#", 1)[0].strip() == target + ":":
                    j = b + 1
                    break
            if j is not None:
                while j < len(out) and not out[j].strip():
                    j += 1
                if j < len(out):
                    pj = _parse_insn(out[j])
                    if pj is not None and pj[0] == "li" and len(pj[1]) == 2:
                        try:
                            v = int(pj[1][1].strip(), 0) & 0xFFFFFFFF
                        except ValueError:
                            v = None
                        if v is not None and not (v < 0x8000 or v >= 0xFFFF8000 or v <= 0xFFFF):
                            lind = out[j][:len(out[j]) - len(out[j].lstrip())]
                            rest = []
                            if v & 0xFFFF:
                                rest = ["%sori\t%s,%s,0x%x" % (lind, pj[1][0], pj[1][0], v & 0xFFFF)]
                            out[j:j + 1] = ["%slui\t%s,0x%x" % (lind, pj[1][0], v >> 16)] + rest
                    out.insert(j + 1, newlab + ":")
        i += 1
    return "\n".join(out)



_WIDE_LOGIC_RE = re.compile(r"^(\s*)(and|or|xor)\s+(\$\w+)\s*,\s*(\$\w+)\s*,\s*(-?\d+)\s*$", re.M)


def ido_expand_wide_logic(text):
    """Spell out the assembler's `$at` expansion of a logic op whose constant
    does not fit the zero-extended 16-bit immediate form, so that the halves
    are individual instructions for the slot rules below (ASPSX expands its
    macros first and fills slots with the pieces: _spu_setReverbAttr `bnez v1 ;
    lui at,0x1 ; and t8,v0,at`, SetSemiTrans `lbu v0 ; li at,-3 ; and`).
    A constant in 0..0xFFFF is left for GNU as to encode as andi/ori/xori.
    """
    def rep(m):
        ind, op, rd, rs, imm = m.groups()
        v = int(imm) & 0xFFFFFFFF
        if v <= 0xFFFF:
            return m.group(0)
        if v >= 0xFFFF8000:                       # fits addiu $at,$zero,IMM
            return "%saddiu $at, $0, %d\n%s%s %s, %s, $at" % (ind, v - 0x100000000, ind, op, rd, rs)
        hi, lo = v >> 16, v & 0xFFFF
        out = "%slui $at, 0x%x" % (ind, hi)
        if lo:
            out += "\n%sori $at, $at, 0x%x" % (ind, lo)
        return out + "\n%s%s %s, %s, $at" % (ind, op, rd, rs)
    return _WIDE_LOGIC_RE.sub(rep, text)



_BR_IMM_RE = re.compile(r"^(\s*)(beq|bne)\s+(\$\w+)\s*,\s*(-?\d+)\s*,\s*(\S+)\s*$", re.M)
_BR_CMP_RE = re.compile(r"^(\s*)(blt|ble|bgt|bge|bltu|bleu|bgtu|bgeu)\s+(\$\w+)\s*,\s*(\$\w+|-?\d+)\s*,\s*(\S+)\s*$", re.M)


def _li_at(ind, v):
    v &= 0xFFFFFFFF
    if v < 0x8000 or v >= 0xFFFF8000:
        return "%saddiu $at, $0, %d" % (ind, v if v < 0x8000 else v - 0x100000000)
    if v & 0xFFFF == 0:
        return "%slui $at, 0x%x" % (ind, v >> 16)
    return "%slui $at, 0x%x\n%sori $at, $at, 0x%x" % (ind, v >> 16, ind, v & 0xFFFF)


_BRANCH_ZERO_IMM = False      # --ido-branch-zero-imm


_B_STORE_SLOT = False         # --ido-b-store-slot


def ido_jr_store_slot(text):
    """ASPSX sinks a store into a `j $31` delay slot even when the store's
    data register was loaded by the word just above it: the jump itself
    covers the load delay (--ido-jr-store-slot; SsUtGetDetVVol words 15-17,
    `lhu t0,2(v1) ; jr ra ; sh t0,0(a2)`).  GNU as pads a nop after the load
    first and then fills the slot, one word longer.  The store is placed in
    the slot explicitly; nothing else moves, and never across a label.

    ``--ido-b-store-slot`` (``_B_STORE_SLOT``) extends the rule to an
    unconditional `b LABEL`: retail _spu_setVoiceAttr word 60-62 `lw t8,0x14(a0)
    ; b ; sh t8,6(t3)`, LoadTPage 30-32, _spu_ioctl 175-177,
    _gcSPU 133-135; no retail `load ; nop ; b ; store-of-it` exists
    in the ASPSX-assembled set."""
    lines = text.split("\n")
    out = list(lines)
    idx = [i for i, ln in enumerate(lines) if ln.strip()]
    for pos in range(2, len(idx)):
        ij, isv, il = idx[pos], idx[pos - 1], idx[pos - 2]
        pj = _parse_insn(lines[ij])
        if pj is None or not ((pj[0] in ("j", "jr") and pj[1] == ["$31"])
                              or (_B_STORE_SLOT and pj[0] == "b" and len(pj[1]) == 1)):
            continue
        ps = _parse_insn(lines[isv])
        pl = _parse_insn(lines[il])
        if ps is None or pl is None or ps[0] not in _STORES or pl[0] not in _LOADS:
            continue
        if len(ps[1]) != 2 or not re.search(r"\(\s*\$", ps[1][1]):
            continue                    # a symbol store is a macro: leave it
        if not (_regs_of(pl[1][0]) & _regs_of(ps[1][0])):
            continue                    # no load-delay hazard: GNU as fills it already
        if _regs_of(pl[1][0]) & _regs_of(ps[1][1]):
            continue                    # the base itself was just loaded
        ind = lines[ij][:len(lines[ij]) - len(lines[ij].lstrip())]
        out[isv] = ""
        out[ij] = "%s.set noreorder\n%s\n%s\n%s.set reorder" % (
            ind, lines[ij], lines[isv], ind)
    return "\n".join(out)


def ido_b_store_macro_slot(text):
    """ASPSX fills an unconditional `b LABEL` slot with the STORE HALF of a
    symbol-store macro written just above it (--ido-b-store-macro-slot).

    `s{w,h,b} rt,SYM[(rB)]` is a macro (`lui $at,%hi(SYM) ; [addu $at,$at,rB ;]
    sX rt,%lo(SYM)($at)`).  GNU as never splits a macro into a delay slot, so
    it leaves `b` a nop; ASPSX treats the expansion as ordinary words and the
    store word, being the instruction just above the branch, takes the slot:
    `li a1,K ; b join ; sw t1,%lo(D)(at)` (main/_SsStart words 35/43/51,
    with `freq = K;` written before the store).  Retail census over the whole
    library band 0x80086000-0x800B5000: 62 unconditional `beq $0,$0` slots
    hold a `s* rt,%lo(SYM)($at)` store, and no unconditional branch with a
    `lui $at ; s* ..($at)` pair right above it has a nop slot (0; the 11 with
    another word in the slot are the `sw SYM ; <word> ; b` order, where that
    word is the one just above the branch and GNU as already sinks it).

    The macro is spelled out as its two halves directly above the branch and
    GNU as's own reorder mode then moves the store word into the slot, so the
    `%hi`/`%lo` pair stays in order and GNU as keeps its hazard tracking
    (a store's data loaded by the word above the macro is covered by the
    `lui $at`).  Never fires across a label, inside `.set noreorder` /
    `.set noat`, for a store of or through `$at`, or for `j $31` (the retail
    evidence there is mixed with as1-assembled code).
    """
    lines = text.split("\n")
    noreorder = noat = False
    last = None                         # index of the last instruction line
    for i, ln in enumerate(lines):
        t = ln.split("#", 1)[0].strip()
        if not t:
            continue
        if t.startswith(".set"):
            w = t.split()[1:2]
            if w == ["noreorder"]:
                noreorder = True
            elif w == ["reorder"]:
                noreorder = False
            elif w == ["noat"]:
                noat = True
            elif w == ["at"]:
                noat = False
            last = None
            continue
        pi = _parse_insn(ln)
        if pi is None:
            last = None                 # label or directive: block boundary
            continue
        if (pi[0] == "b" and len(pi[1]) == 1 and last is not None
                and not noreorder and not noat):
            ps = _parse_insn(lines[last])
            if ps is not None and _STORE_SYM_RE.match(ps[0]) and len(ps[1]) == 2:
                head = _at_only_head(lines[last])
                if head is not None:
                    slot, rest = head
                    lines[last] = "\n".join([slot] + rest)
        last = i
    return "\n".join(lines)


def ido_expand_branch_macros(text):
    """Spell out ugen's compare-and-branch macros the way GNU as would expand
    them, so the `$at` half is an instruction of its own for the slot rules
    (retail get_dx: `li at,1 ; beq v0,at ; li at,2` -- the second
    macro's `li $at` sits in the first branch's delay slot).  Encodings are
    GNU as's own; only the text order changes."""
    def imm(m):
        ind, op, rs, v, lab = m.groups()
        v = int(v)
        if v == 0:
            return m.group(0)
        return "%s\n%s%s %s, $at, %s" % (_li_at(ind, v), ind, op, rs, lab)

    def cmp(m):
        ind, op, rs, rt, lab = m.groups()
        u = op.endswith("u")
        base = op[:-1] if u else op
        slt = "sltu" if u else "slt"
        slti = "sltiu" if u else "slti"
        if rt.startswith("$"):
            if base == "blt":
                return "%s%s $at, %s, %s\n%sbne $at, $0, %s" % (ind, slt, rs, rt, ind, lab)
            if base == "bge":
                return "%s%s $at, %s, %s\n%sbeq $at, $0, %s" % (ind, slt, rs, rt, ind, lab)
            if base == "bgt":
                return "%s%s $at, %s, %s\n%sbne $at, $0, %s" % (ind, slt, rt, rs, ind, lab)
            if base == "ble":
                return "%s%s $at, %s, %s\n%sbeq $at, $0, %s" % (ind, slt, rt, rs, ind, lab)
        else:
            v = int(rt)
            if _BRANCH_ZERO_IMM and v == 0 and not u:
                # GNU as (and ASPSX) encode a compare against a ZERO
                # immediate as the single-register branch, no $at
                # (SsUtGetDetVVol word 5: ugen `blt $4,0` is retail `bltz a0`)
                z = {"blt": "bltz", "bge": "bgez", "bgt": "bgtz", "ble": "blez"}[base]
                return "%s%s %s, %s" % (ind, z, rs, lab)
            if base == "blt" and -32768 <= v <= 32767:
                return "%s%s $at, %s, %d\n%sbne $at, $0, %s" % (ind, slti, rs, v, ind, lab)
            if base == "bge" and -32768 <= v <= 32767:
                return "%s%s $at, %s, %d\n%sbeq $at, $0, %s" % (ind, slti, rs, v, ind, lab)
            if base == "bgt" and -32768 <= v + 1 <= 32767:
                return "%s%s $at, %s, %d\n%sbeq $at, $0, %s" % (ind, slti, rs, v + 1, ind, lab)
            if base == "ble" and -32768 <= v + 1 <= 32767:
                return "%s%s $at, %s, %d\n%sbne $at, $0, %s" % (ind, slti, rs, v + 1, ind, lab)
        return m.group(0)

    text = _BR_IMM_RE.sub(imm, text)
    text = _BR_CMP_RE.sub(cmp, text)
    return text


_PTR_BUMP = False     # --ido-fill-ptr-bump
_MEM_OPND_RE = re.compile(r"^(-?\d+)?\(\s*(\$\w+)\s*\)$")


def _ptr_bump_fill(lines, j, k, base_reg, bump):
    """The ASPSX pointer-bump fill (--ido-fill-ptr-bump).

    Retail's assembler fills a load-delay slot with a later `addiu rB,rB,K`
    even when the instructions it hops over use rB as a memory BASE, by
    rewriting each of those offsets by -K: IDO's `lh t7,0(v0) ; sh t7,0(v1) ;
    addu v1,v1,2` is retail's `lh t7,0(v0) ; addiu v1,v1,2 ; sh t7,-2(v1)`
    (_SsInit words 22-35 and 47-60, six sites; the store-then-bump
    form occurs nowhere in the library region).  Only memory operands may
    mention rB in between, and only as the base.  Returns True when applied.
    """
    victims = []
    base_set = _regs_of(base_reg)
    for t in range(j, k):
        r = _parse_insn(lines[t])
        if r is None:
            if lines[t].strip():
                return False
            continue
        mnem, ops = r
        for idx, o in enumerate(ops):
            if not (base_set & _regs_of(o)):
                continue
            m = _MEM_OPND_RE.match(o)
            if not (mnem in _LOADS or mnem in _STORES) or idx != 1 or not m or m.group(2) != base_reg:
                return False
            victims.append((t, idx, int(m.group(1) or 0)))
    if not victims:
        return False
    for t, idx, off in victims:
        r = _parse_insn(lines[t])
        mnem, ops = r
        ops[idx] = "%d(%s)" % (off - bump, base_reg)
        lines[t] = "\t%s\t%s" % (mnem, ",".join(ops))
    lines.insert(j, lines.pop(k))
    return True


def ido_fill_load_delays(text):
    lines = text.split("\n")
    n = len(lines)
    i = 0
    while i < n:
        p = _parse_insn(lines[i])
        if not p or p[0] not in _LOADS or len(p[1]) < 2:
            i += 1
            continue
        ldest = _regs_of(p[1][0])
        # next instruction (skipping blank lines); a label/directive ends the block
        j = i + 1
        while j < n and not lines[j].strip():
            j += 1
        if j >= n:
            break
        q = _parse_insn(lines[j])
        if q is None:
            i += 1
            continue
        qd, qu = _defs_uses(q[0], q[1])
        if qd is None or not (qu & ldest):
            i += 1          # no hazard, or a shape we do not model
            continue
        # hazard.  ASPSX looks ABOVE first: the instruction just before the
        # load sinks below it when the two are independent (_spu_setCommonAttr
        # words 158/159: `lhu t9,426(a1) ; lhu t0,36(a0) ; andi t1,t0` became
        # `lhu t0 ; lhu t9 ; andi`).  Loads and ALU words only; never a
        # store, a branch, or anything across a label.
        a = i - 1
        while a >= 0 and not lines[a].strip():
            a -= 1
        if a >= 0:
            pa = _parse_insn(lines[a])
            if (pa is not None and (pa[0] in _ALU or pa[0] in _LOADS)
                    and not _BRANCH_RE.match(pa[0]) and not _is_symbol_load(pa[0], pa[1])):
                ad, au = _defs_uses(pa[0], pa[1])
                luses = _regs_of(p[1][1])
                if ad is not None and not (ad & luses) and not (ldest & au) and not (ad & ldest):
                    lines.insert(i, lines.pop(a))   # P now sits right after L
                    i += 1
                    continue
            # --ido-fill-macro-lui: an `la rd,SYM` macro just above the load
            # sinks below it WHOLE (lui+addiu), its lui filling the slot
            # (SpuVmGetSeqVol words 10-13: `la v1,T ; lh t6,G ; sll t7,t6`
            # became `lui t6 ; lh t6 ; lui v1 ; addiu v1 ; sll t7`).
            elif _MACRO_LUI_FILL and pa is not None:
                la = _la_macro(pa[0], pa[1])
                if la is not None and not (_regs_of(la[0]) & (_regs_of(p[1][1]) | ldest)):
                    lines.insert(i, lines.pop(a))
                    i += 1
                    continue
        # otherwise scan forward for the nearest legal filler.  A plain
        # independent word anywhere in the block wins over a pointer bump
        # that needs an offset rewrite (_SsInit words 31-35: the src
        # bump `addiu v0,8` fills the last `lh` slot, not the nearer dst
        # bump), so the bump pass runs only when the plain pass found none.
        moved = False
        hoisted = False
        for want_bump in ((False, True) if _PTR_BUMP else (False,)):
            skipped_defs, skipped_uses = set(ldest), set()
            skipped_mem = False
            k = j
            while k < n:
                r = _parse_insn(lines[k])
                if r is None:
                    if lines[k].strip():
                        break           # label or directive: block boundary
                    k += 1
                    continue
                mnem, ops = r
                if _BRANCH_RE.match(mnem):
                    break
                if _MACRO_LUI_FILL and k > j and not want_bump:
                    split = _split_symbol_macro(mnem, ops)
                    if split is not None:
                        treg = _regs_of(split[0])
                        if not (treg & (skipped_defs | skipped_uses)):
                            lead = re.match(r"^(\s*)", lines[k]).group(1) or "\t"
                            lines[k] = "\n".join(lead + w for w in split[2])
                            lines.insert(j, lead + split[1])
                            # the split macro's rest now spans several text
                            # lines joined in one list slot; re-split so
                            # the later hazard scan sees each word
                            lines[:] = "\n".join(lines).split("\n")
                            n = len(lines)
                            moved = True
                            break
                        # its lui cannot rise: the macro is an ordinary
                        # word the scan continues past (its defs/uses below)
                    if mnem == "la" and split is None:
                        break
                    if split is not None:
                        rd, ru = split[3]
                        skipped_defs |= rd
                        skipped_uses |= ru
                        k += 1
                        continue
                rd, ru = _defs_uses(mnem, ops)
                if rd is None:
                    break               # unknown macro: give up on this slot
                if k > j and not want_bump:
                    legal = (mnem in _ALU or mnem in _LOADS) and not _is_symbol_load(mnem, ops)
                    if mnem in _LOADS and skipped_mem:
                        legal = False   # a load never crosses a store
                    if ru & skipped_defs or rd & skipped_uses or rd & skipped_defs:
                        legal = False
                    if ru & ldest:
                        legal = False
                    if legal:
                        # --ido-hoist-load-above: when the word that would fill
                        # the slot is itself a LOAD, retail's assembler does not
                        # sink it into the slot -- it hoists it ABOVE the hazard
                        # load and leaves the hazard to a nop.  Both IDO as1 and
                        # retail do this on main/get_dx's second case
                        # (`lh t1,0(a0) ; negu ; lh t3,4(a0)` becomes
                        # `lh t3,4(a0) ; lh t1,0(a0) ; nop ; negu`), while the
                        # plain fill gives `lh t1 ; lh t3 ; negu`, the same four
                        # words in a different order.  Opt-in, because the
                        # _spu_getCommonAttr / _spu_setCommonAttr fills this pass was
                        # written for want the sinking behaviour.
                        if (_HOIST_LOAD_ABOVE and mnem in _LOADS
                                and _feeds_noat_macro(lines, k)):
                            # The macro's FIRST residue nop is its own
                            # load delay (--ido-div-macro-residue counts it
                            # because the load was adjacent).  Hoisting the
                            # load away pays that delay with real words, so
                            # give the nop back.
                            for d in range(k + 1, len(lines)):
                                t = lines[d].split("#", 1)[0].strip()
                                if not t:
                                    continue
                                if t != "nop":
                                    break
                                nxt = d + 1
                                while nxt < len(lines) and not lines[nxt].strip():
                                    nxt += 1
                                if (nxt < len(lines)
                                        and lines[nxt].split("#", 1)[0].strip() == "nop"):
                                    del lines[d]
                                    n -= 1
                                break
                            lines.insert(i, lines.pop(k))
                            hoisted = True
                        else:
                            lines.insert(j, lines.pop(k))
                        moved = True
                        break
                if (k > j and want_bump and mnem in ("addiu", "addu", "addi") and len(ops) == 3
                        and ops[0] == ops[1] and re.match(r"^-?\d+$", ops[2].strip())
                        and not (ru & ldest) and not (ru & (skipped_defs - ldest))
                        and rd & skipped_uses and not (rd & skipped_defs)):
                    if _ptr_bump_fill(lines, j, k, ops[0], int(ops[2])):
                        moved = True
                        break
                skipped_defs |= rd
                skipped_uses |= ru
                if mnem in _STORES or mnem in _LOADS:
                    skipped_mem = skipped_mem or mnem in _STORES
                k += 1
            if moved:
                break
        if hoisted:
            # The filler floated ABOVE this load, so this load's own slot is
            # now unfilled on purpose -- GNU as pads it, which is retail's
            # `lh t3,4(a0) ; lh t1,0(a0) ; nop ; negu t2,t1`.  Step over both
            # words: re-examining the hazard load would let the ABOVE-FIRST
            # rule sink the hoisted load straight back down again.
            i += 2
            continue
        i += 1
    return "\n".join(lines)


def ido_branch_slot_hop(text):
    """The ASPSX branch-slot store hop (--ido-fill-branch-hop).

    When the word just above a branch is a pointer bump `addiu rB,rB,K` and
    the word above THAT is a store based on rB, retail's assembler fills the
    branch delay slot with the STORE, rewriting its offset by -K, and leaves
    the bump in place -- whether or not the branch reads rB (_SsInit
    words 33-35 `addiu v1,v1,2 ; bne v0,a1 ; sh t0,-2(v1)`; 0x800A2550
    `addiu v1,v1,384 ; bne v1,v0 ; sw zero,-96(v1)`; six library sites, nine
    in the as1-assembled game region).  This pass only moves the store to
    just above the branch with the rewritten offset; GNU as's reorder mode
    then sinks it into the slot.  Never across a label, and the store may
    mention rB only as its base."""
    lines = text.split("\n")
    n = len(lines)
    for bi in range(n):
        pb = _parse_insn(lines[bi])
        if pb is None or not _BRANCH_RE.match(pb[0]) or pb[0] in ("jal", "jalr", "jr", "j"):
            continue
        br_uses = set()
        for o in pb[1]:
            br_uses |= _regs_of(o)
        a = bi - 1
        while a >= 0 and not lines[a].strip():
            a -= 1
        if a < 0:
            continue
        pa = _parse_insn(lines[a])
        if (pa is None or pa[0] not in ("addiu", "addu", "addi") or len(pa[1]) != 3
                or pa[1][0] != pa[1][1] or not re.match(r"^-?\d+$", pa[1][2].strip())):
            continue
        base_reg = pa[1][0]
        bump = int(pa[1][2])
        b = a - 1
        while b >= 0 and not lines[b].strip():
            b -= 1
        if b < 0:
            continue
        ps = _parse_insn(lines[b])
        if ps is None or ps[0] not in _STORES or len(ps[1]) != 2:
            continue
        m = _MEM_OPND_RE.match(ps[1][1])
        if not m or m.group(2) != base_reg or _regs_of(base_reg) & _regs_of(ps[1][0]):
            continue
        if _regs_of(ps[1][0]) & br_uses and False:
            continue                    # a store writes no register; nothing to clash
        off = int(m.group(1) or 0)
        lines[b] = "\t%s\t%s,%d(%s)" % (ps[0], ps[1][0], off - bump, base_reg)
        moved = lines.pop(b)                # b < a: the bump slides to a-1 ...
        lines.insert(a, moved)              # ... and the store lands between it and the branch
    return "\n".join(lines)


def run_step(name, cmd, verbose=False, stdin_data=None, capture_stdout=False):
    """Run a pipeline step. Returns stdout bytes if capture_stdout is True."""
    if verbose:
        if isinstance(cmd, list):
            display = " ".join(cmd)
        else:
            display = cmd
        print(f"[{name}] {display}", file=sys.stderr)

    try:
        result = subprocess.run(
            cmd,
            input=stdin_data,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            check=True,
        )
        if name == "maspsx" and result.stderr and os.environ.get("MASPSX_TRACE"):
            # --reuse-at-same-hi trace lines (and any other maspsx notes)
            sys.stderr.write(result.stderr.decode("utf-8", errors="replace"))
        if capture_stdout:
            return result.stdout
        return None
    except subprocess.CalledProcessError as e:
        print(f"ERROR in step '{name}':", file=sys.stderr)
        print(f"  Command: {cmd}", file=sys.stderr)
        print(f"  Return code: {e.returncode}", file=sys.stderr)
        if e.stderr:
            stderr_text = e.stderr.decode("utf-8", errors="replace").strip()
            if stderr_text:
                print(f"  stderr: {stderr_text}", file=sys.stderr)
        if e.stdout:
            stdout_text = e.stdout.decode("utf-8", errors="replace").strip()
            if stdout_text:
                print(f"  stdout: {stdout_text}", file=sys.stderr)
        sys.exit(e.returncode)


def main():
    parser = argparse.ArgumentParser(
        description="PSY-Q GCC compilation pipeline wrapper",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )
    parser.add_argument("source", help="C source file to compile")
    parser.add_argument("-o", "--output", required=True, help="Output .o file")
    parser.add_argument(
        "--gcc-version",
        choices=["260", "263", "263p", "272", "281"],
        default="260",
        help="GCC version to use (default: 260)",
    )
    parser.add_argument(
        "--codegen",
        choices=["gcc", "ido52"],
        default="gcc",
        help="Code generator: PSY-Q GCC cc1 (default) or IDO 5.2 `cc -S` "
             "(the hybrid pipeline: IDO codegen assembled PSY-Q style, "
             "which is how retail's library-region IDO code was built)",
    )
    parser.add_argument(
        "--assembler",
        choices=["maspsx", "gas"],
        default="maspsx",
        help="Assembly stage: maspsx (ASPSX emulation) then GNU as, or GNU as "
             "alone in MIPS reorder mode (fills delay slots and load-delay "
             "hazards itself; expands wide immediates through $at without "
             "narrowing).  --codegen ido52 only.",
    )
    parser.add_argument(
        "-I", "--include-dir",
        action="append",
        default=[],
        help="Extra include directory (repeatable)",
    )
    parser.add_argument(
        "--extra-flags",
        default="",
        help="Extra cc1 flags (space-separated string)",
    )
    parser.add_argument(
        "--aspsx-version",
        default="2.67",
        help="maspsx --aspsx-version (default: 2.67)",
    )
    parser.add_argument(
        "--expand-div",
        action="store_true",
        help="Pass --expand-div to maspsx (emit the ASPSX div-by-zero / "
             "overflow guard sequence around div/divu instead of a bare div)",
    )
    parser.add_argument(
        "--defer-div",
        action="store_true",
        help="ASPSX division-macro schedule: emit `div $zero,rs,rt` in place, "
             "defer its zero/overflow check to the first word that overwrites "
             "rs/rt, uses $at or ends the block, and defer the mflo/mfhi to "
             "the first word that reads rd or touches HI/LO.  maspsx path: "
             "passes --defer-div to maspsx; ido52-gas path: applied to the "
             "IDO text before GNU as (maspsx.defer_div_macros)",
    )
    parser.add_argument(
        "--div-mf-with-check",
        action="store_true",
        help="With --defer-div: a check flushed by an OVERWRITE of rs/rt also "
             "flushes the pending mflo/mfhi when its rd is rs or rt (retail "
             "GsSetFlatLight `<check> ; mflo a0 ; sll a1`).  maspsx path: passes "
             "--div-mf-with-check; ido52-gas path: defer_div_macros(mf_with_check)",
    )
    parser.add_argument(
        "--defer-div-check-first-on-slot-read",
        action="store_true",
        help="With --defer-div: at a compiler `.set noreorder` branch that "
             "flushes a pending division, emit the check BEFORE the mflo/mfhi "
             "when the branch's delay-slot word reads the mflo/mfhi "
             "destination (retail GsSortBg 0x80099F44 `<check> ; mfhi t6 "
             "; nop ; nop ; bgez t6 ; move s6,t6`); otherwise the default mf "
             "first (0x8009A26C).  maspsx path: passes the same flag; "
             "ido52-gas path: defer_div_macros(check_first_on_slot_read)",
    )
    parser.add_argument(
        "--fill-load-delay-macro",
        action="store_true",
        help="ASPSX macro-level load-delay fill on the cc1 text BEFORE maspsx "
             "(fill_gcc_delay_slots.fill_load_delay_macro): a `#nop` after a "
             "load takes the NEAREST legal later word of the block, loads "
             "included, crossing div/rem macros, stopping at a symbol-load "
             "macro (retail GsSortBg 0x8009A040, 0x80099EEC)",
    )
    parser.add_argument(
        "--mflo-div-writer-slot",
        action="store_true",
        help="Pass --mflo-div-writer-slot to maspsx (carve-out from "
             "--mflo-nops-first: a division macro's mflo/mfhi followed by a "
             "word that writes an operand of the next mult/div counts that "
             "word as a hazard slot; retail GsSetFlatLight)",
    )
    parser.add_argument(
        "--mflo-noreorder-nops",
        action="store_true",
        help="Pass --mflo-noreorder-nops to maspsx (ASPSX pads up to two words "
             "between an mflo/mfhi and a following compiler .set noreorder block "
             "-- a branch/jal whose slot gcc filled: GsSetFlatLight 0x8009CC84, "
             "GsSortBg 0x80099E00)",
    )
    parser.add_argument(
        "--mflo-consumer-fill",
        action="store_true",
        help="Pass --mflo-consumer-fill to maspsx (with --mflo-nops-first's load "
             "hoist: the first mflo->mult hazard nop takes the word after the mult "
             "when it reads the moved register; GsSetFlatLight 0x8009CC74)",
    )
    parser.add_argument(
        "--mflo-nops-first",
        action="store_true",
        help="Pass --mflo-nops-first to maspsx (ASPSX places both hazard nops "
             "right after mflo/mfhi when the next instruction does not read it)",
    )
    parser.add_argument(
        "--mflo-hoist-skip",
        default="",
        help="Comma-separated 1-based indices of mflo/mfhi load-hoist "
             "candidates NOT to hoist (occurrence window; retail ASPSX "
             "applies the hoist only at some sites)",
    )
    parser.add_argument(
        "--load-base-nop",
        action="store_true",
        help="Pass --load-base-nop to maspsx (ASPSX keeps the load-delay nop "
             "when the intervening instruction defines the consumer's base)",
    )
    parser.add_argument(
        "--delay-slot-load-nop",
        action="store_true",
        help="Pass --delay-slot-load-nop to maspsx (ASPSX emits the load-delay "
             "nop before a branch/jump whose delay slot reads the loaded register)",
    )
    parser.add_argument("--div-result-no-nop", action="store_true",
                        help="Pass --div-result-no-nop to maspsx (ASPSX emits NO load-delay nop between the div macro's closing mflo/mfhi and a consumer of its destination)")
    parser.add_argument("--jal-load-nop", action="store_true",
                        help="Pass --jal-load-nop to maspsx (ASPSX emits the load-delay nop before ANY jump/branch following a load)")
    parser.add_argument("--label-load-nop", action="store_true",
                        help="Pass --label-load-nop to maspsx (ASPSX flushes a load delay at a label)")
    parser.add_argument("--konami-lib-sched", action="store_true",
                        help="Pass --konami-lib-sched to maspsx (list-schedule "
                             "reorder-mode basic blocks after address-macro "
                             "expansion; see docs/reference/MATCHING-TRICKS.md)")
    parser.add_argument("--loc-load-nop", action="store_true",
                        help="Pass --loc-load-nop to maspsx (ASPSX flushes a load delay at a "
                             "source-line boundary: every `.loc` cc1 -g emits between "
                             "statements; use with -g in the cc1 flags; EigenMatrix)")
    parser.add_argument("--macro-lui-fill", action="store_true",
                        help="Pass --macro-lui-fill to maspsx (ASPSX fills a load delay with the next symbol-load macro's lui)")
    parser.add_argument("--load-dest-temp", action="store_true",
                        help="Pass --load-dest-temp to maspsx (ASPSX: reg+symbol loads, and reg+numeric loads whose offset is outside imm16, use the destination as the scratch register (lui rd; addu rd,rd,rs), $at only when it is the index)")
    parser.add_argument("--store-macro-split", action="store_true",
                        help="Pass --store-macro-split to maspsx (ASPSX splits a "
                             "`sw reg,SYM` macro: the `lui $at` is emitted, then the "
                             "following instructions, and only then the store)")
    parser.add_argument("--store-macro-split-window", type=int, default=3,
                        help="Maximum emitted words the deferred store may sink past "
                             "(default 3, the largest observed in retail)")
    parser.add_argument("--reuse-at-same-hi", action="store_true",
                        help="Pass --reuse-at-same-hi to maspsx (ASPSX reuses a live `lui $at,%%hi(SYM)` for a "
                             "later store to the SAME symbol in the same basic block -- no second lui; "
                             "CD_readm 0x8008ADD8/0x8008ADF0, cd_init_retry 0x8008AA0C). "
                             "MASPSX_TRACE=1 prints one line per reuse")
    parser.add_argument("--macro-lui-fill-far", action="store_true",
                        help="Pass --macro-lui-fill-far to maspsx (like --macro-lui-fill but the filling macro may be several register-independent instructions later)")
    parser.add_argument("--macro-lui-fill-cross", action="store_true",
                        help="Pass --macro-lui-fill-cross to maspsx (far fill that continues past rejected candidate macros)")
    parser.add_argument("--macro-lui-fill-split", action="store_true",
                        help="Pass --macro-lui-fill-split to maspsx (ASPSX splits a symbol-load macro across two "
                             "load-delay slots: the %%lo half sinks into the nearest later load delay; with "
                             "--macro-lui-fill* a plain load's delay may also take the lui half)")
    parser.add_argument("--ido-copy-and-bump", action="store_true",
                        help="--codegen ido52 --assembler gas only: ASPSX-style copy-and-bump -- an "
                             "otherwise-empty delay slot takes a copy of the branch target's ALU word "
                             "and the branch is retargeted past the original")
    parser.add_argument("--ido-div-macro-residue", action="store_true",
                        help="--codegen ido52 --assembler gas only: give the pow2 signed-div macro "
                             "retail's word count -- one idle nop before the bgez (the non-compacting "
                             "slot-fill residue), two when the divide's source was loaded by the word "
                             "immediately above (the macro is atomic for the load delay) (get_dx)")
    parser.add_argument("--ido-hoist-load-above", action="store_true",
                        help="With --ido-fill-load-delay: a LOAD chosen as the slot filler is hoisted "
                             "ABOVE the hazard load instead of sinking into its slot, which is what "
                             "both as1 and retail do (get_dx words 29-32)")
    parser.add_argument("--ido-label-load-nop", action="store_true",
                        help="--codegen ido52 --assembler gas only: pad the load delay of a load that "
                             "is the last word before a LABEL, which both ASPSX and as1 do and GNU as "
                             "does not (get_dx words 50-51)")
    parser.add_argument("--ido-fill-branch-hop", action="store_true",
                        help="ASPSX branch-slot fill: a store above a same-base pointer bump hops below it into the slot with its offset rewritten (_SsInit)")
    parser.add_argument("--ido-fill-ptr-bump", action="store_true",
                        help="With --ido-fill-load-delay: also fill a load delay with a later `addiu rB,rB,K`, rewriting the hopped-over rB-based memory offsets by -K (ASPSX, _SsInit)")
    parser.add_argument("--ido-fill-macro-lui", action="store_true",
                        help="With --ido-fill-load-delay: a load-delay slot may also be filled by the "
                             "`lui` half of a later symbol-address macro in the block (la / lw SYM / "
                             "lh SYM(rB)), which is split; and an `la` just above the load sinks below "
                             "it whole (ASPSX, SpuVmGetSeqPan / SpuVmGetSeqVol)")
    parser.add_argument("--ido-branch-zero-imm", action="store_true",
                        help="--codegen ido52 --assembler gas only: spell ugen's `blt/bge/bgt/ble rs,0` as "
                             "bltz/bgez/bgtz/blez, as GNU as and ASPSX encode it, not slti+bnez (SsUtGetDetVVol)")
    parser.add_argument("--ido-jr-store-slot", action="store_true",
                        help="--codegen ido52 --assembler gas only: a store whose data was loaded by the word "
                             "above it sinks into the `j $31` slot with no load-delay nop (ASPSX, SsUtGetDetVVol)")
    parser.add_argument("--ido-fill-at-next", action="store_true",
                        help="--codegen ido52 --assembler gas only: an otherwise-empty conditional branch "
                             "slot takes the NEXT word when it writes only $at (an explicit $at op or the "
                             "lui half of a symbol-store macro), even when the branch reads $at; runs "
                             "before --ido-copy-and-bump and has priority over it (ASPSX, SsSetTickMode)")
    parser.add_argument("--ido-copy-dead-ft", action="store_true",
                        help="With --ido-copy-and-bump: a CONDITIONAL branch copies the target's first "
                             "word only when its destination is dead on the fall-through path (it runs "
                             "there too), may overwrite a register the branch reads, and may copy a "
                             "$sp-relative load (it cannot fault) (ASPSX, 45 retail slots)")
    parser.add_argument("--ido-b-store-slot", action="store_true",
                        help="With --ido-jr-store-slot: the same load-covered store sink into the slot "
                             "of an unconditional `b LABEL` (ASPSX, _spu_setVoiceAttr / LoadTPage)")
    parser.add_argument("--ido-b-store-macro-slot", action="store_true",
                        help="--codegen ido52 --assembler gas only: a symbol-store macro just above an "
                             "unconditional `b` is split and its store half fills the slot, not a nop "
                             "(ASPSX, _SsStart; 62/62 unconditional retail slots)")
    parser.add_argument("--ido-fill-load-delay", action="store_true",
                        help="--codegen ido52 --assembler gas only: ASPSX-style load-delay fill on the text "
                             "before GNU as (nearest later independent word of the block, loads included)")
    parser.add_argument(
        "--no-delay-slots",
        action="store_true",
        help="Skip fill_gcc_delay_slots post-processing step",
    )
    parser.add_argument(
        "--fill-jalr",
        action="store_true",
        help="Also fill jalr delay slots (addiu into jalr delay)",
    )
    parser.add_argument(
        "--fill-epilogue",
        action="store_true",
        help="Also fill jr ra epilogue delay slots (addiu sp into jr delay)",
    )
    parser.add_argument(
        "--fill-j",
        action="store_true",
        help="Also fill j delay slots (lui+addiu into j delay)",
    )
    parser.add_argument("--fill-load-delay", action="store_true",
                        help="Pass --fill-load-delay to fill_gcc_delay_slots (ASPSX: nearest legal ALU word fills a load-delay hazard nop)")
    parser.add_argument("--fill-load-delay-no-div", action="store_true",
                        help="Pass --fill-load-delay-no-div to fill_gcc_delay_slots (a div/divu/mfhi/mflo word never fills a load-delay nop; retail GsSortBg 0x80099F68)")
    parser.add_argument("--sink-sp-adjust", action="store_true",
                        help="Pass --sink-sp-adjust to fill_gcc_delay_slots (ASPSX: the prologue `addiu sp,sp,-N` sinks to just after the last load of the entry block and the epilogue restore rises to just after the last load above it, hazard nop or not; StCdInterrupt, _mk_normsp)")
    parser.add_argument("--fill-sp-adjust", action="store_true",
                        help="Pass --fill-sp-adjust to fill_gcc_delay_slots (with --fill-load-delay: `addiu sp,sp,N` may fill a load-delay nop; ASPSX, _mk_normsp)")
    parser.add_argument("--fill-earliest", action="store_true",
                        help="Pass --fill-earliest to fill_gcc_delay_slots (ASPSX reorder: the earliest legal instruction of the block sinks into an empty delay slot)")
    parser.add_argument("--fill-prev", action="store_true",
                        help="Pass --fill-prev to fill_gcc_delay_slots (ASPSX reorder: the instruction before an unfilled j/jal/branch sinks into its slot)")
    parser.add_argument("--fill-ptr-bump-down", action="store_true",
                        help="Pass --fill-ptr-bump-down to fill_gcc_delay_slots (ASPSX reorder: an EARLIER `addiu rX,rX,K` sinks into a load-delay nop past the rX-based loads/stores, displacements rewritten +K; CdSearchFile, dma_execute)")
    parser.add_argument("--fill-branch-store", action="store_true",
                        help="Pass --fill-branch-store to fill_gcc_delay_slots (ASPSX reorder: preceding store into a branch/j slot)")
    parser.add_argument(
        "--fill-jal-store",
        action="store_true",
        help="Also fill jal delay slots with a preceding lui $at + sw pair",
    )
    parser.add_argument(
        "--fill-all",
        action="store_true",
        help="Enable all delay slot filling patterns (jalr + epilogue + j)",
    )
    parser.add_argument(
        "--no-cross-jumping",
        action="store_true",
        help="Disable GCC cross-jumping optimization (uses patched cc1-nocj binary)",
    )
    parser.add_argument(
        "-v", "--verbose",
        action="store_true",
        help="Print each pipeline step to stderr",
    )
    args = parser.parse_args()

    # Validate source file
    source = os.path.abspath(args.source)
    if not os.path.isfile(source):
        print(f"Error: source file not found: {source}", file=sys.stderr)
        sys.exit(1)

    # Validate cc1 binary (use patched nocj binary if --no-cross-jumping)
    cc1_path = None
    if args.codegen == "ido52":
        cc1_rel = None
    elif args.no_cross_jumping:
        nocj_paths = GCC_CC1_NOCJ_PATHS
        if args.gcc_version not in nocj_paths:
            print(f"Error: --no-cross-jumping not available for GCC {args.gcc_version}", file=sys.stderr)
            print(f"  Available for: {', '.join(nocj_paths.keys())}", file=sys.stderr)
            sys.exit(1)
        cc1_rel = nocj_paths[args.gcc_version]
    else:
        cc1_rel = GCC_CC1_PATHS[args.gcc_version]
    if cc1_rel is not None:
        cc1_path = resolve(cc1_rel)
        if not os.path.isfile(cc1_path):
            print(f"Error: cc1 not found at {cc1_path}", file=sys.stderr)
            print(f"  Expected GCC {args.gcc_version} cc1 at: {cc1_rel}", file=sys.stderr)
            sys.exit(1)

    # Validate maspsx
    maspsx_path = resolve(MASPSX_PATH)
    if not os.path.isfile(maspsx_path):
        print(f"Error: maspsx.py not found at {maspsx_path}", file=sys.stderr)
        sys.exit(1)

    # Validate fill_gcc_delay_slots
    delay_slots_path = resolve(FILL_DELAY_SLOTS_PATH)
    if not args.no_delay_slots and not os.path.isfile(delay_slots_path):
        print(f"Error: fill_gcc_delay_slots.py not found at {delay_slots_path}", file=sys.stderr)
        sys.exit(1)

    output = os.path.abspath(args.output)
    verbose = args.verbose

    # Build include flags: always include the project defaults
    include_flags = [
        "-I", resolve("include"),
        "-I", resolve("include/psyq"),
    ]
    for inc_dir in args.include_dir:
        include_flags.extend(["-I", os.path.abspath(inc_dir)])

    # Build cc1 flags
    cc1_flags = list(DEFAULT_CC1_FLAGS)
    if args.no_cross_jumping:
        cc1_flags.append("-fno-cross-jumping")
    if args.extra_flags:
        cc1_flags.extend(args.extra_flags.split())

    if verbose:
        print(f"--- PSY-Q GCC Pipeline (version {args.gcc_version}) ---", file=sys.stderr)
        print(f"  Source:  {source}", file=sys.stderr)
        print(f"  Output:  {output}", file=sys.stderr)
        print(f"  cc1:     {cc1_path}", file=sys.stderr)
        print(f"  cc1 flags: {' '.join(cc1_flags)}", file=sys.stderr)
        print(f"  aspsx:   {args.aspsx_version}", file=sys.stderr)
        print(f"  delay:   {'skip' if args.no_delay_slots else 'yes'}", file=sys.stderr)
        print("", file=sys.stderr)

    # Use a temporary directory for intermediate files
    with tempfile.TemporaryDirectory(prefix="gccwrap_") as tmpdir:
        pp_file = os.path.join(tmpdir, "pp.i")
        gcc_s_file = os.path.join(tmpdir, "gcc.s")
        maspsx_s_file = os.path.join(tmpdir, "maspsx.s")
        asm_o_file = os.path.join(tmpdir, "asm.o")

        if args.codegen == "ido52":
            # Steps 1-2 (hybrid): IDO 5.2 codegen.  --extra-flags are IDO cc
            # flags here (-Wc,-pic1, -Wo,... ), not cc1 flags.
            ido_flags = args.extra_flags.split() if args.extra_flags else []
            gcc_s_data = ido_codegen_to_s(source, ido_flags, tmpdir, verbose=verbose)
            if args.assembler == "maspsx":
                # IDO writes `div\t$24, $4, $15`; maspsx splits operands on
                # whitespace and then on ",", so its div/rem/mul handlers
                # crash on the space.  Drop the spaces after commas on
                # instruction lines (directives are left alone).
                gcc_s_data = re.sub(
                    rb"(?m)^(\t[a-z][a-z0-9.]*\t)(.*)$",
                    lambda m: m.group(1) + re.sub(rb",\s+", b",", m.group(2)),
                    gcc_s_data)
            with open(gcc_s_file, "wb") as f:
                f.write(gcc_s_data)
        else:
            # Step 1: Preprocess
            cpp_cmd = build_cpp_cmd(include_flags, source)
            pp_output = run_step("preprocess", cpp_cmd, verbose=verbose, capture_stdout=True)
            with open(pp_file, "wb") as f:
                f.write(pp_output)

            # Step 2: Compile with PSY-Q GCC cc1
            # cc1 reads from stdin
            cc1_cmd = [cc1_path] + cc1_flags + ["-o", gcc_s_file]
            with open(pp_file, "rb") as f:
                pp_data = f.read()
            run_step("cc1", cc1_cmd, verbose=verbose, stdin_data=pp_data)

            with open(gcc_s_file, "rb") as f:
                gcc_s_data = f.read()

        # Step 2b (--fill-load-delay-macro): ASPSX's macro-level load-delay
        # fill, on the compiler text before maspsx expands any macro.
        if args.fill_load_delay_macro:
            sys.path.insert(0, resolve(os.path.dirname(FILL_DELAY_SLOTS_PATH)))
            from fill_gcc_delay_slots import fill_load_delay_macro
            _fl_lines, _fl_n = fill_load_delay_macro(
                gcc_s_data.decode("utf-8", errors="replace").split("\n"),
                trace=verbose or bool(os.environ.get("FILL_TRACE")))
            gcc_s_data = "\n".join(_fl_lines).encode("utf-8")
            with open(gcc_s_file, "wb") as f:
                f.write(gcc_s_data)

        # Step 3: maspsx processing (skipped for --assembler gas, where GNU
        # as's own reorder mode does the delay-slot and hazard work)
        maspsx_cmd = [
            sys.executable, maspsx_path,
            "--aspsx-version=" + args.aspsx_version,
            "--force-stdin",
        ]
        if args.expand_div:
            maspsx_cmd.append("--expand-div")
        if args.defer_div:
            maspsx_cmd.append("--defer-div")
        if args.mflo_nops_first:
            maspsx_cmd.append("--mflo-nops-first")
        if args.div_mf_with_check:
            maspsx_cmd.append("--div-mf-with-check")
        if args.defer_div_check_first_on_slot_read:
            maspsx_cmd.append("--defer-div-check-first-on-slot-read")
        if args.mflo_div_writer_slot:
            maspsx_cmd.append("--mflo-div-writer-slot")
        if args.mflo_noreorder_nops:
            maspsx_cmd.append("--mflo-noreorder-nops")
        if args.mflo_consumer_fill:
            maspsx_cmd.append("--mflo-consumer-fill")
        if args.div_result_no_nop:
            maspsx_cmd.append("--div-result-no-nop")
        if args.mflo_hoist_skip:
            maspsx_cmd.append("--mflo-hoist-skip=" + args.mflo_hoist_skip)
        if args.load_base_nop:
            maspsx_cmd.append("--load-base-nop")
        if args.delay_slot_load_nop:
            maspsx_cmd.append("--delay-slot-load-nop")
        if args.label_load_nop:
            maspsx_cmd.append("--label-load-nop")
        if args.loc_load_nop:
            maspsx_cmd.append("--loc-load-nop")
        if args.konami_lib_sched:
            maspsx_cmd.append("--konami-lib-sched")
        if args.jal_load_nop:
            maspsx_cmd.append("--jal-load-nop")
        if args.macro_lui_fill:
            maspsx_cmd.append("--macro-lui-fill")
        if args.macro_lui_fill_far:
            maspsx_cmd.append("--macro-lui-fill-far")
        if args.macro_lui_fill_cross:
            maspsx_cmd.append("--macro-lui-fill-cross")
        if args.macro_lui_fill_split:
            maspsx_cmd.append("--macro-lui-fill-split")
        if args.load_dest_temp:
            maspsx_cmd.append("--load-dest-temp")
        if args.reuse_at_same_hi:
            maspsx_cmd.append("--reuse-at-same-hi")
        if args.store_macro_split:
            maspsx_cmd.append("--store-macro-split")
            maspsx_cmd.append(
                "--store-macro-split-window=" + str(args.store_macro_split_window)
            )
        if args.codegen == "ido52" and args.assembler == "gas":
            # GNU as expands the `move` macro to `or`; ASPSX (and maspsx)
            # expand it to `addu`, which is what retail's library region
            # carries.  Spell the macro out before GNU as sees it.
            maspsx_output = re.sub(
                rb"^(\s*)move\s+(\$\w+)\s*,\s*(\$\w+)\s*$",
                rb"\1addu \2, \3, $0",
                gcc_s_data, flags=re.M)
            if args.defer_div:
                global _DEFER_DIV
                _DEFER_DIV = True
                # ASPSX division schedule (see maspsx.defer_div_macros).
                # First, on the plain IDO text: the later passes then see
                # `divu $0,rs,rt`, the relocated mfhi/mflo and the check as
                # an explicit .set noreorder block.
                sys.path.insert(0, resolve(os.path.dirname(MASPSX_PATH)))
                from maspsx import defer_div_macros
                maspsx_output = "\n".join(defer_div_macros(
                    maspsx_output.decode("utf-8", errors="replace").split("\n"),
                    gas=True, mf_with_check=args.div_mf_with_check,
                    check_first_on_slot_read=args.defer_div_check_first_on_slot_read,
                )).encode("utf-8")
            if args.ido_div_macro_residue:
                global _DIV_RESIDUE
                _DIV_RESIDUE = True
            maspsx_output = ido_expand_pow2_macros(
                maspsx_output.decode("utf-8", errors="replace")).encode("utf-8")
            if args.ido_label_load_nop:
                maspsx_output = ido_label_load_nop(
                    maspsx_output.decode("utf-8", errors="replace")).encode("utf-8")
            maspsx_output = ido_expand_wide_logic(
                maspsx_output.decode("utf-8", errors="replace")).encode("utf-8")
            if args.ido_branch_zero_imm:
                global _BRANCH_ZERO_IMM
                _BRANCH_ZERO_IMM = True
            maspsx_output = ido_expand_branch_macros(
                maspsx_output.decode("utf-8", errors="replace")).encode("utf-8")
            if args.ido_fill_ptr_bump:
                global _PTR_BUMP
                _PTR_BUMP = True
            if args.ido_hoist_load_above:
                global _HOIST_LOAD_ABOVE
                _HOIST_LOAD_ABOVE = True
            if args.ido_fill_macro_lui:
                global _MACRO_LUI_FILL
                _MACRO_LUI_FILL = True
            if args.ido_fill_load_delay:
                maspsx_output = ido_fill_load_delays(
                    maspsx_output.decode("utf-8", errors="replace")).encode("utf-8")
            if args.ido_fill_branch_hop:
                maspsx_output = ido_branch_slot_hop(
                    maspsx_output.decode("utf-8", errors="replace")).encode("utf-8")
            if args.ido_b_store_slot:
                global _B_STORE_SLOT
                _B_STORE_SLOT = True
            if args.ido_jr_store_slot:
                maspsx_output = ido_jr_store_slot(
                    maspsx_output.decode("utf-8", errors="replace")).encode("utf-8")
            if args.ido_b_store_macro_slot:
                maspsx_output = ido_b_store_macro_slot(
                    maspsx_output.decode("utf-8", errors="replace")).encode("utf-8")
            if args.ido_fill_at_next:
                global _AT_NEXT_FILL
                _AT_NEXT_FILL = True
                maspsx_output = ido_at_next_fill(
                    maspsx_output.decode("utf-8", errors="replace")).encode("utf-8")
            if args.ido_copy_dead_ft:
                global _COPY_DEAD_FT
                _COPY_DEAD_FT = True
            if args.ido_copy_and_bump:
                maspsx_output = ido_copy_and_bump(
                    maspsx_output.decode("utf-8", errors="replace")).encode("utf-8")
        else:
            maspsx_output = run_step(
                "maspsx", maspsx_cmd, verbose=verbose,
                stdin_data=gcc_s_data, capture_stdout=True,
            )
        with open(maspsx_s_file, "wb") as f:
            f.write(maspsx_output)

        # Step 4: Assemble
        as_cmd = [
            "mipsel-linux-gnu-as",
            "-EL",
            "-march=r3000",
            "-mtune=r3000",
            "-no-pad-sections",
            "-G0",
            "-o", asm_o_file,
            maspsx_s_file,
        ]
        run_step("assemble", as_cmd, verbose=verbose)

        # Step 5: Fill delay slots (optional).  --no-delay-slots skips the
        # pass's default patterns, but explicitly requested selector fills
        # (e.g. --fill-epilogue) still run -- ASPSX fills the jr-ra epilogue
        # slot even in code whose other slots GCC filled itself.
        _explicit_fills = args.fill_jalr or args.fill_epilogue or args.fill_j or args.fill_jal_store or args.fill_branch_store or args.fill_prev or args.fill_earliest or args.fill_load_delay or args.sink_sp_adjust or args.fill_ptr_bump_down or args.fill_all
        if not args.no_delay_slots or _explicit_fills:
            delay_cmd = [
                sys.executable, delay_slots_path,
                asm_o_file,
                output,
            ]
            if args.fill_jalr or args.fill_all:
                delay_cmd.append("--fill-jalr")
            if args.fill_epilogue or args.fill_all:
                delay_cmd.append("--fill-epilogue")
            if args.fill_j or args.fill_all:
                delay_cmd.append("--fill-j")
            if args.fill_jal_store:
                delay_cmd.append("--fill-jal-store")
            if args.fill_branch_store:
                delay_cmd.append("--fill-branch-store")
            if args.fill_prev:
                delay_cmd.append("--fill-prev")
            if args.fill_earliest:
                delay_cmd.append("--fill-earliest")
            if args.fill_load_delay:
                delay_cmd.append("--fill-load-delay")
            if args.fill_sp_adjust:
                delay_cmd.append("--fill-sp-adjust")
            if args.fill_load_delay_no_div:
                delay_cmd.append("--fill-load-delay-no-div")
            if args.sink_sp_adjust:
                delay_cmd.append("--sink-sp-adjust")
            if args.fill_ptr_bump_down:
                delay_cmd.append("--fill-ptr-bump-down")
            if args.fill_all:
                delay_cmd.append("--fill-all")
            run_step("fill_delay_slots", delay_cmd, verbose=verbose)
        else:
            # Just copy the assembled object directly
            import shutil
            shutil.copy2(asm_o_file, output)

    if verbose:
        print(f"\nDone: {output}", file=sys.stderr)


if __name__ == "__main__":
    main()
