import os
import struct
import re
import sys

from typing import List

branch_mnemonics = {
    "beq",
    "bgez",
    "bgtz",
    "blez",
    "bltz",
    "bne",
}
jump_mnemonics = {
    "j",
    "jal",
}
load_mnemonics = {
    "lb",
    "lbu",
    "lh",
    "lhu",
    "lw",
    "lwl",
    "lwr",
}
store_mnemonics = {
    "sb",
    "sh",
    "sw",
    "swl",
    "swr",
    "swc2",
}

single_reg_loads = {
    "mult",
    "multu",
    "div",
    "divu",
    "rem",
    "remu",
    "move",
    "negu",
    "nor",
}
double_reg_loads = {
    "and",
    "andi",
    "or",
    "ori",
    "xor",
    "xori",
    "addu",
    "subu",
    "sll",
    "slr",
    "srl",
    "sra",
    "slt",
    "slti",
    "sltu",
}


# Instructions the ASPSX store-macro split is allowed to cross.
#
# Every confirmed retail sink crosses a `la rX,SYM` -- a two-word address
# macro (getintr words 113/143/160/179/198, CD_ready words 79 and
# 100; seven sites, two functions).  The generalisation to "any plain ALU
# word" that the file headers describe is REFUTED by retail itself:
# ivlc_dct_dc_size words 64-66 are `lui at ; sw a1,2848(at) ; addu a0,a0,v0`,
# i.e. a store macro followed by a plain ALU word that retail leaves
# ADJACENT.  Crossing plain words costs that function a word and gains
# nothing anywhere, so the crossable set is the address macro only.
#
# (The other two sites the headers cite, ivlc_dct_dc_size words 24-26 and 29-31,
# are not sinks at all: 24-26 needs the `lui $at` to move UP past a `sllv`
# that writes the stored register, and 29-31 needs the store to land in a
# branch delay slot that our cc1 fills itself.  Neither is derivable from the
# available evidence -- see docs/reference/ASPSX-STORE-MACRO-SPLIT.md.)
store_split_crossable = {
    "la",
}

# Emitted-word cost of a crossable instruction, for the split's distance cap.
store_split_word_cost = {
    "la": 2,
}


# --reuse-at-same-hi: instructions a live `lui $at,%hi(SYM)` survives.
# Straight-line words GNU as assembles without touching $at: register-form
# ALU ops (an immediate operand is checked for range separately, since an
# out-of-range one becomes a macro through $at), HI/LO moves, and the
# address macros GNU as expands through their own DESTINATION register
# (`li`, `la rd,SYM`, a symbol LOAD `lX rd,SYM` -- rd, never $at).
reuse_at_alu_ops = {
    "addu", "subu", "and", "or", "xor", "nor", "slt", "sltu",
    "sll", "srl", "sra", "sllv", "srlv", "srav",
    "addiu", "andi", "ori", "xori", "slti", "sltiu", "lui",
    "negu", "neg", "not", "move", "mflo", "mfhi", "mtlo", "mthi",
    "mult", "multu",
}
REUSE_AT_SYMADDR_RE = re.compile(r"_([0-9A-Fa-f]{8})$")


_LANDED_MAIN = None


def reuse_at_sym_addr(sym):
    """address of SYM for --reuse-at-same-hi: from an address-shaped name
    (`D_XXXXXXXX`), else from the main-binary names landed in the repo's
    ledger (config/symbol_names.json, found from this file's location).  A
    rename must not change the build (2026-11-10: renaming D_800E2DE0 /
    D_800E2DF4 cost cd_init_retry and CD_readm their $at reuse)."""
    global _LANDED_MAIN
    m = REUSE_AT_SYMADDR_RE.search(sym)
    if m:
        return int(m.group(1), 16)
    if _LANDED_MAIN is None:
        _LANDED_MAIN = {}
        led = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                           "..", "..", "..", "config", "symbol_names.json")
        try:
            import json
            with open(led) as fh:
                for key, e in json.load(fh)["symbols"].items():
                    if (e.get("landed") and e.get("scope", "main") == "main"
                            and e.get("name") and ":" not in key):
                        _LANDED_MAIN[e["name"]] = int(key, 16)
                        for v in e.get("views") or ():
                            _LANDED_MAIN[v["name"]] = int(key, 16) + int(v.get("offset", 0))
        except (OSError, ValueError, KeyError):
            pass
    return _LANDED_MAIN.get(sym)
# ...and only for a symbol inside the retail EXE's FILE IMAGE (initialised
# text+data: header t_addr 0x80041000, t_size 0xA8000 -> 0x800E9000).  Above
# that is bss, and there retail re-emits the lui for same-symbol stores:
# valiable_init (D_8012DCD0, 12 pairs), GsSetRefView (D_80131CB0) and
# GsGetObjMatrix (D_80131DA0) are byte-perfect gcc260 functions whose C
# stores to one bss symbol at several offsets, each with its own `lui at`.
# All three retail reuses are 0x800E2Dxx (initialised data) -- the ASPSX
# analogue of as1 sharing `lui $at` only for initialised definitions.
REUSE_AT_DATA_LO = 0x80041000
REUSE_AT_DATA_HI = 0x800E9000


def reuse_at_split_operand(operand: str):
    """`SYM`, `SYM+N` or `SYM-N` -> (SYM, N); None for anything else."""
    m = re.match(r"^([A-Za-z_.$][A-Za-z0-9_.$]*)\s*(?:([+-])\s*(0x[0-9A-Fa-f]+|\d+))?$",
                 operand.strip())
    if not m:
        return None
    off = 0
    if m.group(2):
        off = int(m.group(3), 0)
        if m.group(2) == "-":
            off = -off
    return m.group(1), off


def reuse_at_hi(addr: int) -> int:
    """%hi() as the linker computes it (carry from a negative %lo)."""
    return ((addr + 0x8000) >> 16) & 0xFFFF


def strip_comments(line) -> str:
    if line.count("#") > 0:
        line = line.split("#")[0]
    return line.strip()


def line_loads_from_reg(line, r_source) -> bool:
    """
    NOTE: Returns True even if line might use $at expansion
    """
    line = strip_comments(line)

    # escape dollar
    r_source = r_source.replace("$", r"\$")

    if match := re.match(r"^([A-z][A-z0-9]*)\s+(.*)$", line):
        op, rest = match.group(1, 2)
    else:
        return False

    if op in load_mnemonics:
        # lwl	$9,7($2)
        if re.match(rf"^.*\(\s*{r_source}\s*\)$", rest):
            return True

    elif op in store_mnemonics:
        if re.match(rf"^.*\(\s*{r_source}\s*\)$", rest):
            return True
        # "line_loads_from_reg" is a bit of a lie
        if re.match(rf"^{r_source},.*$", rest):
            return True

    elif op == "lwc2":
        # lwc2 $5, 4( $4
        if re.match(rf"^.*\(\s*{r_source}\s*\)$", rest):
            return True

    elif op == "jal":
        if re.match(rf"^.*,\s*{r_source}$", rest):
            return True

    elif op == "j":
        if re.match(rf"^{r_source}$", rest):
            return True

    elif op in ("ctc2", "mtc0", "mtc2"):
        if re.match(rf"^{r_source},.*$", rest):
            return True

    elif op in branch_mnemonics:
        if re.match(rf"^{r_source},.*$", rest):
            return True
        if re.match(rf"^.*,\s*{r_source},.*$", rest):
            return True

    elif op in single_reg_loads:
        if re.match(rf"^.*,\s*{r_source}$", rest):
            return True
        if op.startswith("mult"):
            if re.match(rf"^{r_source},.*$", rest):
                return True
        if op.startswith("div") or op.startswith("rem"):
            # e.g. div	$3,$3,$7
            if re.match(rf"^.*,{r_source}.*$", rest):
                return True

    elif op in double_reg_loads:
        if re.match(rf"^.*,\s*{r_source},.*$", rest):
            return True
        if re.match(rf"^.*,.*,\s*{r_source}$", rest):
            return True

    return False


def is_number(value: str) -> bool:
    if re.match(r"^-?\d+$", value) or re.match(r"^-?0x[A-Fa-f0-9]+$", value):
        return True
    return False


def uses_at(line: str) -> bool:
    line = strip_comments(line)

    # sw	$2,%lo(s_attr)($3)
    if match := re.match(r"^s[wbh]\s+(\$[a-z0-9]+),\s*%lo\(([^(]+)\)\(([^)]+)\)", line):
        return False

    # sw	$2,D_801813A4
    # sw	$3,g_CurrentRoom+40
    # sw	$2,D_us_8017863C.4
    if match := re.match(r"^s[wbh]\s+(\$[a-z0-9]+),\s*(-?[A-z0-9_.+]+)$", line):
        operand = match.group(2)
        if not is_number(operand):
            return True

    # sb	$2,g_InputSaveName($3)
    # sw	$2,-26($16)
    elif match := re.match(r"^s[wbh]\s+(\$[a-z0-9]+),\s*([^(]+)\(([^)]+)\)", line):
        operand = match.group(2)

    # lw	$2,-1000($16)
    elif match := re.match(r"l[a-z]+\s+(\$[a-z0-9]+),\s*([^(]+)\(([^)]+)\)", line):
        operand = match.group(2)

    else:
        return False

    if is_number(operand):
        num = int(operand, 0)
        if -32769 < num < 32768:
            return False

    return True


def parse_load_or_store(rest):
    if match := re.match(r"(\$[a-z0-9]+),\s*%lo\(([^(]+)\)\(([^(]+)\)", rest):
        r_dest, operand, r_source = match.group(1, 2, 3)
        needs_expanding = False
    elif match := re.match(r"(\$[a-z0-9]+),\s*([^(]+)\(([^)]+)\)", rest):
        r_dest, operand, r_source = match.group(1, 2, 3)
        needs_expanding = True
    elif match := re.match(r"(\$[a-z0-9]+),\s*([^(]+)", rest):
        r_dest, operand = match.group(1, 2)
        r_source = None
        needs_expanding = True
    else:
        raise Exception(f"Unable to parse load/store instruction: {rest}")

    if re.match(r"^-?\d+$", operand) or re.match(r"^-?0x[A-Fa-f0-9]+$", operand):
        is_addend = False
    else:
        is_addend = True

    return (r_source, r_dest, operand, is_addend, needs_expanding)


def div_needs_expanding(line: str) -> bool:
    inst, *rest = line.split()
    if not (inst.startswith("div") or inst.startswith("rem")):
        return False

    r_dest, *_ = rest[0].split(",")
    return r_dest not in ("$zero", "$0")


def expand_load_immediate(line: str) -> List[str]:
    res = []

    match = re.match(r"li\s+(\$[0-9A-z]+),\s?(-?[x0-9a-fA-F]+)", line)
    assert match is not None, "li regex failed"

    r_dest = match.group(1)
    operand = int(match.group(2), 0)

    if 0 < operand < 0x10000:
        res.append(f"ori\t{r_dest},$zero,{operand}")
    elif operand >= 0x10000:
        res.append(f"lui\t{r_dest},({operand} >> 16) & 0xFFFF")
        if operand & 0xFFFF:
            res.append(f"ori\t{r_dest},{r_dest},{operand} & 0xFFFF")
    elif 0 > operand > -0x8000:
        res.append(f"addiu\t{r_dest},$zero,{operand}")
    elif operand == -0x8000:
        res.append(f"addiu\t{r_dest},$zero,{operand} & 0xFFFF")
    elif operand < -0x8000:
        res.append(f"lui\t{r_dest},({operand} >> 16) & 0xFFFF")
        if operand & 0xFFFF:
            res.append(f"ori\t{r_dest},{r_dest},{operand} & 0xFFFF")
    else:
        # TODO: raise an exception here instead?
        # ori is actually addiu on ASPSX 2.56+
        res.append(f"ori\t{r_dest},0")

    return res


def expand_move(line: str):
    line = strip_comments(line)
    op, *rest = line.split()
    if op == "move":
        args = " ".join(rest)
        r_dest, r_source = args.split(",")
        return f"addu\t{r_dest},{r_source},$zero"
    return line


def is_label(line: str):
    return re.match(r"\$L(b|e)?\d+:$", line)


def is_instruction(line: str, ignore_nop=False, ignore_set=False, ignore_label=False):
    if len(line) == 0:
        return False

    if ignore_nop and line == "#nop":
        return False
    if ignore_set and line in (
        ".set\treorder",
        ".set\tnoreorder",
        ".set\tvolatile",
        ".set\tnovolatile",
    ):
        return False
    if ignore_label and is_label(line):
        return False

    if line.startswith(".stab"):
        return False
    if line.startswith(".def") or line.startswith(".bend") or line.startswith(".begin"):
        return False
    if line.startswith(".loc"):
        return False

    if line.startswith("L") and line[1] not in "0123456789" and line.endswith(":"):
        return False
    if line in (".set\tmacro", ".set\tnomacro"):
        return False
    if line in ("#.set\tvolatile", "#.set\tnovolatile"):
        return False
    if line in ("#APP", "#NO_APP"):
        return False

    return True


def get_next_register(reg: str):
    lut = {
        # li $fx
        "$f0": "$f1",
        "$f2": "$f3",
        "$f4": "$f5",
        "$f6": "$f7",
        "$f12": "$f13",
        "$f14": "$f15",
        # names
        "$v0": "$v1",
        "$a0": "$a1",
        "$a2": "$a3",
        "$t0": "$t1",
        "$t2": "$t3",
        "$s0": "$s1",
        "$s2": "$s3",
        # nums
        "$2": "$3",  # $v0
        "$4": "$5",  # $a0
        "$6": "$7",  # $a2
        "$8": "$9",  # $t0
        "$10": "$11",  # t2
        "$18": "$19",  # s2
    }
    next_reg = lut.get(reg)
    assert next_reg is not None, f"Unknown mapping for {reg}"
    return next_reg


def expand_macro(line: str):
    res = []
    for l in strip_comments(line).split(";"):
        l = l.strip()
        if len(l) == 0:
            continue
        op, *rest = l.split()
        res.append(
            (op, " ".join(rest)),
        )
    return res


def load_immediate_single(line: str):
    res = []
    r1, value = line[5:].split(",")
    (num,) = struct.unpack(">i", struct.pack(">f", float(value)))
    upper = (num & 0xFFFF_0000) >> 16
    lower = (num & 0x0000_FFFF) >> 0

    res.append(f"lui\t{r1},0x{upper:X}")
    # we don't always need the lower part
    if lower:
        res.append(f"ori\t{r1},0x{lower:X}")
    return res


def load_immediate_double(line: str):
    res = []
    r1, value = line[5:].split(",")
    r2 = get_next_register(r1)
    (num,) = struct.unpack(">q", struct.pack(">d", float(value)))

    r1_upper = (num & 0x0000_0000_FFFF_0000) >> 16
    r1_lower = (num & 0x0000_0000_0000_FFFF) >> 0
    r2_upper = (num & 0xFFFF_0000_0000_0000) >> 48
    r2_lower = (num & 0x0000_FFFF_0000_0000) >> 32

    if r1_upper or r1_lower:
        res.append(f"lui\t{r1},0x{r1_upper:X}")
        if r1_lower:
            res.append(f"ori\t{r1},0x{r1_lower:X}")
    else:
        res.append(f"li\t{r1},0x0")

    res.append(f"lui\t{r2},0x{r2_upper:X}")
    if r2_lower:
        res.append(f"ori\t{r2},0x{r2_lower:X}")

    return res


# ---------------------------------------------------------------------------
# --defer-div: the ASPSX division-macro schedule
# ---------------------------------------------------------------------------
#
# Measured from retail library bytes (valiable_init, the nine divisions of
# GsSortBg, and the `divu ; mfhi ; bnez ; nop ; break 7` shape of
# _spu_setVoiceAttr): ASPSX does not emit a `div/divu/rem/remu rd,rs,rt` macro as
# one contiguous block the way --expand-div (and GNU as) do.  It
#   1. emits `div $zero,rs,rt` IN PLACE;
#   2. DEFERS the divide-by-zero check (`bnez rt,1f ; nop ; break 7 ; 1:`,
#      plus the -1 / 0x80000000 overflow block for a signed divide) until the
#      first later instruction that OVERWRITES rs or rt, USES $at, or ENDS the
#      basic block (label, branch, jump, call, another divide, a
#      `.set noreorder` region, the end of the function);
#   3. DEFERS the `mflo rd` / `mfhi rd` until the first later instruction
#      that READS or WRITES rd, TOUCHES HI/LO (mult*/div*/mf*/mt*), or ends
#      the basic block.
# When both are due at the same instruction the check goes first (it reads
# rs/rt, which rd may alias) -- except at a `.set noreorder` directive, where
# retail has the mflo/mfhi first (see the comment at that case).  Emitting the mflo/mfhi counts as a write of rd,
# so a check whose rs or rt is rd is flushed before it.
#
# Everything is keyed on the compiled instruction stream: the pass only
# decides WHERE the words the macro always produces are placed.  It never
# moves a check across a branch or into a delay slot (a branch ends the block
# and flushes the check in front of it), and the relocated mflo/mfhi is an
# ordinary instruction to the later hazard handling, so the MIPS I rule that
# no mult/div may start within two instructions of an mflo/mfhi is still
# enforced where it lands (maspsx's _handle_mflo_mfhi, or GNU as's reorder
# mode on the ido52-gas path).

_DD_REGNAMES = {
    "zero": "0", "at": "1", "v0": "2", "v1": "3",
    "a0": "4", "a1": "5", "a2": "6", "a3": "7",
    "t0": "8", "t1": "9", "t2": "10", "t3": "11",
    "t4": "12", "t5": "13", "t6": "14", "t7": "15",
    "s0": "16", "s1": "17", "s2": "18", "s3": "19",
    "s4": "20", "s5": "21", "s6": "22", "s7": "23",
    "t8": "24", "t9": "25", "k0": "26", "k1": "27",
    "gp": "28", "sp": "29", "fp": "30", "s8": "30", "ra": "31",
}

_DD_DIV_RE = re.compile(
    r"^(div|divu|rem|remu)\s+(\$\w+)\s*,\s*(\$\w+)\s*,\s*(\$\w+)\s*$"
)

# ops whose first register operand is NOT written
_DD_NO_DEST = {
    "sb", "sh", "sw", "swl", "swr", "swc1", "swc2", "sdc1", "usw", "ush",
    "mult", "multu", "div", "divu", "mthi", "mtlo", "break", "syscall",
    "nop", "jr", "j", "jal", "b", "bal", "beq", "bne", "beqz", "bnez",
    "bgez", "bgtz", "blez", "bltz", "bgezal", "bltzal", "beql", "bnel",
    "bge", "bgt", "ble", "blt", "bgeu", "bgtu", "bleu", "bltu",
    "teq", "tne", "tge", "tlt", "tgeu", "tltu", "ctc2", "mtc0", "mtc1",
    "mtc2", "lwc2",
}

_DD_HILO = {"mult", "multu", "div", "divu", "rem", "remu", "mfhi", "mflo",
            "mthi", "mtlo", "mul", "mulo", "mulou", "dmul"}

_DD_BLOCK_END = {
    "j", "jal", "jr", "jalr", "b", "bal", "beq", "bne", "beqz", "bnez",
    "bgez", "bgtz", "blez", "bltz", "bgezal", "bltzal", "beql", "bnel",
    "bge", "bgt", "ble", "blt", "bgeu", "bgtu", "bleu", "bltu",
    "bc1t", "bc1f", "bc2t", "bc2f", "syscall", "break", "eret", "rfe",
}

# directives that neither emit words nor end a block
_DD_QUIET_DIRECTIVES = (
    ".loc", ".stab", ".def", ".begin", ".bend", ".set\tvolatile",
    ".set\tnovolatile", ".set\tmacro", ".set\tnomacro", ".mask", ".fmask",
    ".frame",
)


# --mflo-consumer-fill: words that may move into an mflo hazard slot.  Only
# forms process_line passes through unchanged (or `move`, which it expands),
# so emitting one early loses no processing.
_MFLO_FILL_OPS = {"addu", "subu", "and", "or", "xor", "nor", "negu", "move",
                  "sll", "srl", "sra", "sllv", "srlv", "srav", "slt"}


def _dd_norm(reg: str) -> str:
    r = reg.lstrip("$")
    return _DD_REGNAMES.get(r, r)


def _dd_is_label(text: str) -> bool:
    return re.match(r"^[$.\w]+:$", text) is not None


def _dd_classify(text: str):
    """(op, writes, reads, uses_at) for one stripped instruction."""
    parts = text.split(None, 1)
    op = parts[0]
    ops = parts[1].strip() if len(parts) > 1 else ""
    regs = [_dd_norm(r) for r in re.findall(r"\$\w+", ops)]
    writes, reads = set(), set(regs)
    if regs and op not in _DD_NO_DEST:
        first = re.findall(r"\$\w+", ops.split(",")[0])
        if first:
            dest = _dd_norm(first[0])
            writes.add(dest)
            rest = [_dd_norm(r) for r in re.findall(r"\$\w+", ops.split(",", 1)[1])] if "," in ops else []
            reads = set(rest)
    at = "1" in reads or "1" in writes
    if not at and op in store_mnemonics:
        # a symbolic / out-of-range store is a `lui $at ; sw` macro
        at = uses_at(f"{op}\t{ops}")
    if not at and op in load_mnemonics:
        # a symbol load with an index register (`lw $2,sym($3)`) or an
        # out-of-range displacement goes through $at
        m = re.match(r"^\$\w+\s*,\s*([^(]+)\(([^)]+)\)$", ops)
        if m and "%lo" not in m.group(1):
            disp = m.group(1).strip()
            if not is_number(disp) or not (-32769 < int(disp, 0) < 32768):
                at = True
    return op, writes, reads, at


def _dd_branch_has_imm(text: str) -> bool:
    """`beq rs, 0, L` / `bne rs, 5, L`: a conditional branch whose comparand
    is an immediate rather than a register (an assembler branch macro)."""
    parts = text.split(None, 1)
    if len(parts) < 2 or parts[0] not in ("beq", "bne", "bge", "bgt", "ble",
                                          "blt", "bgeu", "bgtu", "bleu", "bltu"):
        return False
    ops = [o.strip() for o in parts[1].split(",")]
    return len(ops) == 3 and not ops[1].startswith("$")


def _dd_noreorder_slot_reads(lines: List[str], idx: int, rd: str) -> bool:
    """Does the delay-slot word of the noreorder branch that follows
    ``lines[idx]`` (a ``.set noreorder``) read register ``rd``?  The slot is
    the SECOND instruction after the directive (branch, then slot);
    directives, labels and blank/comment lines are skipped."""
    ins = []
    for raw in lines[idx + 1:]:
        t = raw.split("#", 1)[0].strip()
        if not t or t.startswith("."):
            continue
        if _dd_is_label(t):
            return False
        ins.append(t)
        if len(ins) == 2:
            break
    if len(ins) != 2:
        return False
    _op, _writes, reads, _at = _dd_classify(ins[1])
    return _dd_norm(rd) in reads


def defer_div_macros(lines: List[str], gas: bool = False,
                     mf_with_check: bool = False,
                     check_first_on_slot_read: bool = False) -> List[str]:
    """Re-schedule the register-operand div/divu/rem/remu macros of a
    reorder-mode stream the way ASPSX does (see the block comment above).

    ``lines`` are instruction/directive lines (whitespace tolerant).  With
    ``gas=False`` (maspsx) each deferred check is written as one ``divchk`` /
    ``divuchk`` pseudo-instruction that MaspsxProcessor.process_line expands;
    with ``gas=True`` it is written out as an explicit ``.set noreorder``
    block for GNU as.  Divides inside a ``.set noreorder`` region, and
    divides by an immediate, are left untouched.

    ``mf_with_check`` (--div-mf-with-check, 2026-09-24): when the pending
    check is flushed because an instruction OVERWRITES rs or rt, and the
    pending mflo/mfhi's rd is itself rs or rt, the mflo/mfhi is emitted
    together with the check (check first), ahead of the overwriting word.
    Retail GsSetFlatLight 0x8009CDAC / 0x8009CED0 / 0x8009CFF0:
    `div zero,a0,a1 ; <check> ; mflo a0 ; sll a1,s2,12 ; nop ; mult a1,a3`
    (the default leaves the mflo until the `mult`).  Negative controls
    that keep the default: GsSortBg 0x8009A044 (rt overwritten but rd
    is neither rs nor rt: mfhi stays deferred) and valiable_init 0x8009903C
    (rd == rs but the check is flushed by an $at use, not an overwrite).

    ``check_first_on_slot_read`` (--defer-div-check-first-on-slot-read,
    2026-09-26): at a compiler ``.set noreorder`` region that flushes a
    pending division, the default emits the mflo/mfhi BEFORE the check.
    With this flag, when the delay-slot word of the noreorder branch (the
    second instruction after the ``.set noreorder``) READS the mflo/mfhi
    destination, the check is emitted first and the mflo/mfhi after it.
    Retail GsSortBg has both shapes in one function:
    0x80099F44 `<check> ; mfhi t6 ; nop ; nop ; bgez t6 ; move s6,t6`
    (slot reads t6 -> check first) and 0x8009A26C `mfhi v0 ; <check> ;
    bnez v0 ; andi v0,s2,..` (slot writes v0 -> mf first, the default).
    """
    out: List[str] = []
    pend_chk = None  # (signed, rs, rt, indent)
    pend_mf = None   # (mf-op, rd, indent)
    reorder = True

    def emit_check():
        nonlocal pend_chk
        if pend_chk is None:
            return
        signed, rs, rt, ind = pend_chk
        pend_chk = None
        if not gas:
            out.append(f"{ind}{'divchk' if signed else 'divuchk'}\t{rs},{rt}")
            return
        # `.+N` targets, not labels: a label would attach to the next
        # instruction and stop GNU as swapping it into a following branch's
        # delay slot, which ASPSX does not do.
        # Directives are spelled `.set noreorder` (one space) like the rest
        # of gccwrap's ido52-gas text, whose slot passes key on that form.
        blk = [".set noreorder", ".set noat", f"bnez\t{rt}, .+12", "nop",
               "break\t7"]
        if signed:
            blk += ["addiu\t$at, $zero, -1", f"bne\t{rt}, $at, .+20",
                    "lui\t$at, 0x8000", f"bne\t{rs}, $at, .+12", "nop",
                    "break\t6"]
        blk += [".set at", ".set reorder"]
        out.extend(ind + b for b in blk)

    def emit_mf():
        nonlocal pend_mf
        if pend_mf is None:
            return
        mf, rd, ind = pend_mf
        pend_mf = None
        if pend_chk is not None and _dd_norm(rd) in (_dd_norm(pend_chk[1]), _dd_norm(pend_chk[2])):
            emit_check()
        out.append(f"{ind}{mf}\t{rd}")

    def flush_all():
        emit_check()
        emit_mf()

    for li, raw in enumerate(lines):
        text = raw.split("#", 1)[0].strip()
        ind = raw[: len(raw) - len(raw.lstrip())]

        if not text:
            out.append(raw)
            continue

        if text.startswith("."):
            if text.startswith(".set"):
                word = text.split()[-1]
                if word == "noreorder":
                    # A compiler .set noreorder region (gcc's filled
                    # branches) ends the block with the mflo/mfhi FIRST:
                    # retail GsSortBg 0x8009A26C `div zero,s1,t3 ;
                    # mfhi v0 ; <check> ; bnez v0` for gcc's `rem $2,$16,$12
                    # ; .set noreorder ; bne $2,$0`.  (A reorder-mode branch
                    # puts the check first: 0x80099F2C `<check> ; mfhi t6 ;
                    # ... bgez t6`.)
                    if (check_first_on_slot_read and pend_mf is not None
                            and pend_chk is not None
                            and _dd_noreorder_slot_reads(lines, li, pend_mf[1])):
                        emit_check()
                        emit_mf()
                    else:
                        emit_mf()
                        emit_check()
                    reorder = False
                elif word == "reorder":
                    reorder = True
            if not text.startswith(_DD_QUIET_DIRECTIVES) and not text.startswith(".set"):
                flush_all()
            out.append(raw)
            continue

        if _dd_is_label(text) or ";" in text:
            flush_all()
            out.append(raw)
            continue

        m = _DD_DIV_RE.match(text)
        if m and reorder and _dd_norm(m.group(2)) != "0":
            flush_all()
            op, rd, rs, rt = m.groups()
            unsigned = op.endswith("u")
            base = "divu" if unsigned else "div"
            zero = "$0" if gas else "$zero"
            sep = ", " if gas else ","
            out.append(f"{ind}{base}\t{zero}{sep}{rs}{sep}{rt}")
            pend_chk = (not unsigned, rs, rt, ind)
            pend_mf = ("mfhi" if op.startswith("rem") else "mflo", rd, ind)
            continue

        if pend_chk is None and pend_mf is None:
            out.append(raw)
            continue

        op, writes, reads, at = _dd_classify(text)
        if op in _DD_BLOCK_END:
            if (
                pend_mf is not None
                and _dd_norm(pend_mf[1]) in reads
                and _dd_branch_has_imm(text)
            ):
                # A compare-with-IMMEDIATE branch is an assembler macro
                # (IDO's `beq $13, 0, $43`): its operands are read -- and
                # the mfhi flushed -- before the block-end flush of the
                # check.  Retail _spu_setVoiceAttr 0x80096B24 `divu zero,t1,v0 ;
                # mfhi t9 ; bnez v0 ; nop ; break 7 ; beqz t9`.  A native
                # register branch keeps the check first (GsSortBg
                # 0x80099F2C, gcc's `bgez $14`).
                emit_mf()
            flush_all()
        else:
            if pend_chk is not None:
                _srcs = {_dd_norm(pend_chk[1]), _dd_norm(pend_chk[2])}
                if (writes & _srcs) or at:
                    emit_check()
                    if (mf_with_check and (writes & _srcs) and pend_mf is not None
                            and _dd_norm(pend_mf[1]) in _srcs):
                        emit_mf()
            if pend_mf is not None:
                rd = _dd_norm(pend_mf[1])
                if rd in reads or rd in writes or op in _DD_HILO:
                    emit_mf()
        out.append(raw)

    flush_all()
    return out


class MaspsxProcessor:
    is_reorder = True
    skip_instructions = 0
    file_num = 1
    line_index = 0

    def __init__(
        self,
        lines: List[str],
        sdata_limit=0,
        expand_div=False,
        defer_div=False,
        div_mf_with_check=False,
        div_check_first_on_slot_read=False,
        expand_li=False,
        nop_at_expansion=False,
        nop_mflo_mfhi=True,
        mflo_nops_first=False,
        mflo_div_writer_slot=False,
        mflo_noreorder_nops=False,
        mflo_consumer_fill=False,
        div_result_no_nop=False,
        mflo_hoist_skip=(),
        load_base_nop=False,
        delay_slot_load_nop=False,
        label_load_nop=False,
        loc_load_nop=False,
        macro_lui_fill=False,
        macro_lui_fill_far=False,
        macro_lui_fill_cross=False,
        macro_lui_fill_split=False,
        load_dest_temp=False,
        jal_load_nop=False,
        store_macro_split=False,
        store_macro_split_window=3,
        reuse_at_same_hi=False,
        sltu_at=False,
        addiu_at=False,
        div_uses_tge=False,
        gp_allow_offset=False,
        gp_allow_la=False,
        use_comm_section=False,
        use_comm_for_lcomm=False,
        konami_lib_sched=False,
    ):
        self.lines = [x.strip() for x in lines]
        # --konami-lib-sched: list-schedule reorder-mode blocks first
        # (maspsx/ksched.py); off by default.
        self.konami_lib_sched = konami_lib_sched

        self.sdata_limit = sdata_limit

        self.expand_div = expand_div
        self.defer_div = defer_div
        self.div_mf_with_check = div_mf_with_check
        self.div_check_first_on_slot_read = div_check_first_on_slot_read
        self.expand_li = expand_li

        self.nop_at_expansion = nop_at_expansion
        self.nop_mflo_mfhi = nop_mflo_mfhi
        self.mflo_nops_first = mflo_nops_first
        self.mflo_div_writer_slot = mflo_div_writer_slot
        self.mflo_noreorder_nops = mflo_noreorder_nops
        self.mflo_consumer_fill = mflo_consumer_fill
        # --mflo-consumer-fill: indices of self.lines already emitted early
        # (pulled into an mflo hazard slot); process_lines skips them.
        self.moved_line_indices = set()
        self.div_result_no_nop = div_result_no_nop
        self.mflo_hoist_skip = set(mflo_hoist_skip)
        self.load_base_nop = load_base_nop
        self.delay_slot_load_nop = delay_slot_load_nop
        self.label_load_nop = label_load_nop
        self.loc_load_nop = loc_load_nop
        self.macro_lui_fill = macro_lui_fill
        self.macro_lui_fill_far = macro_lui_fill_far
        self.macro_lui_fill_cross = macro_lui_fill_cross
        # --macro-lui-fill-split: the %lo half of a symbol-load macro sinks
        # into the nearest LATER load-delay slot (see _lo_sink_target).
        self.macro_lui_fill_split = macro_lui_fill_split
        # (target line index, [lines to emit in that load's hazard slot])
        self.pending_lo_sink = None
        self.load_dest_temp = load_dest_temp
        self.jal_load_nop = jal_load_nop
        self.store_macro_split = store_macro_split
        self.store_macro_split_window = store_macro_split_window
        self.pending_lui_fill = None
        self.pending_store_split = None
        # --reuse-at-same-hi: (SYM, offset, address) whose `lui $at,%hi`
        # is live in $at, or None; see _reuse_at_store.
        self.reuse_at_same_hi = reuse_at_same_hi
        self.at_live = None
        self._at_armed_this_line = False
        self.reuse_at_count = 0
        self.mflo_hoist_index = 0

        self.sltu_at = sltu_at
        self.addiu_at = addiu_at
        self.div_uses_tge = div_uses_tge

        self.gp_allow_offset = gp_allow_offset
        self.gp_allow_la = gp_allow_la

        self.use_comm_section = use_comm_section
        self.use_comm_for_lcomm = use_comm_for_lcomm

        self.bss_entries: dict[str, int] = {}
        self.sbss_entries: dict[str, int] = {}
        self.sdata_entries: dict[str, int] = {}

        self.comm_symbols: set[str] = set()

    def preprocess_lines(self) -> None:
        in_sdata = False
        uses_size = False

        for line in self.lines:
            if line == "":
                continue

            if line.startswith(".align"):
                # TODO: worry about alignment later
                continue

            if line.startswith(".globl"):
                continue

            if line.startswith(".text"):
                in_sdata = False
                continue
            if line.startswith(".data"):
                in_sdata = False
                continue
            if line.startswith(".rdata"):
                in_sdata = False
                continue

            if line.startswith(".section") and line.endswith(".text"):
                in_sdata = False
                continue

            if line.startswith("#"):
                continue

            if line.startswith(".sdata"):
                in_sdata = True
                continue

            if line.startswith(".file"):
                in_sdata = False
                continue

            if line.startswith(".extern"):
                in_sdata = False
                continue

            if line.startswith(".comm") or line.startswith(".lcomm"):
                # e.g.	.comm	MENU_RadarScale_800AB480,4
                in_sdata = False
                _, var = line.split()
                symbol, size_str = var.split(",")
                size = int(size_str)
                if size <= self.sdata_limit:
                    self.sbss_entries[symbol] = size
                else:
                    self.bss_entries[symbol] = size

                if line.startswith(".comm"):
                    self.comm_symbols.add(symbol)
                continue

            if in_sdata:
                # NOTE: newer compilers emit .size for sdata, old ones do not...
                if match := re.match(r"\.size\s+([^,]+),([0-9]+)", line):
                    current_symbol = match.group(1)
                    size = int(match.group(2))
                    self.sdata_entries[current_symbol] = size
                    uses_size = True
                    continue

                if not uses_size:
                    if line.endswith(":"):
                        current_symbol = line.replace(":", "")
                        self.sdata_entries[current_symbol] = 0
                    else:
                        if line.startswith(".type"):
                            continue

                        if line.startswith(".space"):
                            _, size_str = line.split()
                            size = int(size_str)
                        elif line.startswith(".word"):
                            size = 4
                        elif line.startswith(".half") or line.startswith(".short"):
                            size = 2
                        elif line.startswith(".byte"):
                            size = 1
                        elif line.startswith(".ascii"):
                            # e.g. .ascii	"Map poly groups\000"
                            # NOTE: len('.ascii\t""') == 9
                            size = len(line) - 9
                        else:
                            raise Exception(
                                f"Unable to parse .sdata instruction: {line}"
                            )
                        self.sdata_entries[current_symbol] += size

    def process_lines(self):
        self.is_reorder = True
        self.skip_instructions = 0
        self.file_num = 1

        self.bss_entries = {}
        self.sbss_entries = {}
        self.sdata_entries = {}

        self.preprocess_lines()

        if self.konami_lib_sched:
            from .ksched import schedule_lines
            self.lines = [x.strip() for x in schedule_lines(self.lines)]

        if self.defer_div:
            # --defer-div: ASPSX's div/rem macro schedule (see
            # defer_div_macros).  Rewrites the input stream only; with the
            # flag off self.lines is untouched.
            self.lines = [x.strip() for x in defer_div_macros(
                self.lines, mf_with_check=self.div_mf_with_check,
                check_first_on_slot_read=self.div_check_first_on_slot_read)]

        res = []
        self._out = res   # accumulated output, visible to per-line handlers
        in_include_asm_hack = False
        for i, line in enumerate(self.lines):
            self.line_index = i

            if ".ent\t__maspsx_include_asm_hack" in line:
                in_include_asm_hack = True

            if in_include_asm_hack:
                if "# maspsx-keep" in line:
                    res += [line]
                else:
                    res += [f"# {line} # DEBUG: skipped due to include asm hack"]
                if ".end\t__maspsx_include_asm_hack" in line:
                    in_include_asm_hack = False
                continue

            if line.startswith(".end") and self.pending_store_split is not None:
                raise Exception(
                    "store-macro split: deferred store never flushed before "
                    f".end (line {i}) -- refusing to emit a function with a "
                    "dropped store"
                )

            if line.startswith(".end") and self.pending_lo_sink is not None:
                raise Exception(
                    "macro-lui-fill-split: %lo half never placed before "
                    f".end (line {i})")

            chunk_start = len(res)
            self._at_armed_this_line = False
            if is_instruction(line) and self.skip_instructions > 0:
                self.skip_instructions -= 1
                res += [f"# {line}  # DEBUG: skipped"]
                self.at_live = None
            elif i in self.moved_line_indices:
                res += [f"# {line}  # DEBUG: moved into an mflo hazard slot"]
                self.at_live = None
            else:
                res += self.process_line(line)
            if (self.reuse_at_same_hi and self.at_live is not None
                    and not self._at_armed_this_line
                    and not self._reuse_at_preserved(line, res[chunk_start:])):
                self.at_live = None

            if self.pending_lo_sink is not None and self.pending_lo_sink[0] == i:
                # the target load's hazard nop becomes the %lo half; if the
                # load unexpectedly got no nop, the half goes in front of it
                # (every word it crossed was checked independent of rd).
                lo_lines = self.pending_lo_sink[1]
                self.pending_lo_sink = None
                for k in range(chunk_start, len(res)):
                    if res[k].startswith("nop"):
                        res[k:k + 1] = lo_lines
                        break
                else:
                    res[chunk_start:chunk_start] = lo_lines

            if (
                self.pending_store_split is not None
                and self.pending_store_split[1] != i
                and is_instruction(
                    line, ignore_nop=True, ignore_set=True, ignore_label=True
                )
                and not (line.startswith(".") or line.startswith("#"))
            ):
                self.pending_store_split[0] -= 1
                if self.pending_store_split[0] <= 0:
                    res += self.pending_store_split[2]
                    self.pending_store_split = None

        if self.pending_store_split is not None:
            raise Exception(
                "store-macro split: deferred store never flushed at end of input"
            )

        for section, entries in [
            ("sbss", self.sbss_entries),
            ("bss", self.bss_entries),
        ]:
            for i, (symbol, size) in enumerate(entries.items()):
                if i == 0:
                    res.append(f".section .{section}")

                if self.use_comm_section and (
                    symbol in self.comm_symbols or self.use_comm_for_lcomm
                ):
                    # implicit alignment for COMMON
                    res.append(f"\t.comm {symbol},{size}")
                    continue

                if section == "sbss":
                    if size >= 8:
                        res.append("\t.align 3")
                    elif size >= 4:
                        res.append("\t.align 2")
                    elif size >= 2:
                        res.append("\t.align 1")

                # only mark bss symbols as global
                if section == "bss":
                    res.append(
                        f"\t.globl {symbol}",
                    )
                res.extend(
                    [
                        f"{symbol}:",
                        f"\t.space {size}",
                    ]
                )

        return res

    def get_next_instruction(
        self, skip=0, ignore_nop=False, ignore_set=False, ignore_label=False
    ):
        i = self.line_index + 1
        while i < len(self.lines):
            line = self.lines[i]
            if is_instruction(
                line,
                ignore_nop=ignore_nop,
                ignore_set=ignore_set,
                ignore_label=ignore_label,
            ):
                if skip == 0:
                    return line
                skip -= 1
            i += 1

        return ""  # warn user?

    def _uses_gp(self, line: str) -> bool:
        if self.sdata_limit == 0:
            return False

        line = strip_comments(line)
        if uses_at(line):
            op, *rest = line.split("\t")
            if op in load_mnemonics or op in store_mnemonics:
                (
                    _,
                    _,
                    operand,
                    _,
                    _,
                ) = parse_load_or_store(" ".join(rest))

                if operand.count("+") == 1:
                    symbol, _ = operand.split("+")
                    gp_allowed = self.gp_allow_offset or symbol not in self.comm_symbols
                else:
                    symbol = operand
                    gp_allowed = True

                if gp_allowed and (
                    symbol in self.sbss_entries or symbol in self.sdata_entries
                ):
                    return True

        return False

    def _handle_nop_before_next_instruction(
        self, next_instruction: str, r_dest: str
    ) -> List[str]:
        res: List[str] = []

        if line_loads_from_reg(next_instruction, r_dest):
            nop_required = False

            if not uses_at(next_instruction):
                reason = f"'{next_instruction}' does not use $at"
                nop_required = True
            if self._uses_gp(next_instruction):
                reason = f"'{next_instruction}' uses $gp"
                nop_required = True
            if uses_at(next_instruction) and self.nop_at_expansion:
                reason = (
                    f"'{next_instruction}' inject nop beween {r_dest} and $at expansion"
                )
                nop_required = True

            if nop_required:
                label = self.get_next_instruction(
                    skip=0, ignore_nop=True, ignore_set=True
                )
                if is_label(label) and not (self.delay_slot_load_nop or self.label_load_nop):
                    res.append(label)
                    self.skip_instructions = 1
                # ASPSX attaches the hazard nop to the LOAD, so with
                # --delay-slot-load-nop a label between the load and its
                # consumer stays after the nop (a jump to the label lands on
                # the consumer, as retail's cross-jumps do).
                res.append(f"nop # DEBUG: Reuse of '{r_dest}'. {reason}")
        else:
            emitted = False
            if self.load_base_nop:
                # ASPSX (retail LIBGTE epilogues): when the instruction between
                # a load and its consumer DEFINES the consumer's base register
                # (lhu v1 | move v0,s1 | sh v1,4(v0)), ASPSX does not treat it
                # as a delay-slot filler and still inserts the load-delay nop.
                nn = self.get_next_instruction(
                    skip=1, ignore_nop=True, ignore_set=True, ignore_label=True
                )
                m = re.match(r"^[a-z.]+\s+(\$?[a-z0-9]+)", strip_comments(next_instruction))
                filler_dest = m.group(1) if m else None
                if (
                    filler_dest
                    and line_loads_from_reg(nn, r_dest)
                    and re.search(r"\(\s*" + re.escape(filler_dest) + r"\s*\)", strip_comments(nn))
                ):
                    res.append(
                        f"nop # DEBUG: load-base-nop ('{next_instruction}' defines the base of '{nn}')"
                    )
                    emitted = True
            if not emitted and self.loc_load_nop and self._loc_boundary_follows():
                # --loc-load-nop: ASPSX flushes a pending load delay at a
                # SOURCE-LINE boundary (a `.loc`, which cc1 -g emits between
                # statements), exactly as it does at a label.  Gated.
                res.append(
                    f"nop # DEBUG: loc-load-nop (a .loc follows the load of {r_dest})"
                )
                emitted = True
            if not emitted and self.label_load_nop:
                # ASPSX flushes a pending load delay at a LABEL boundary: it
                # cannot see what reaches the label from other paths, so a
                # load immediately followed by a label gets its hazard nop
                # even when the instruction after the label does not read
                # the register (retail VSync word 35).  Gated.
                nxt = self.get_next_instruction(
                    skip=0, ignore_nop=True, ignore_set=True, ignore_label=False
                )
                if is_label(nxt):
                    res.append(
                        f"nop # DEBUG: label-load-nop ('{nxt}' follows the load of {r_dest})"
                    )
                    emitted = True
            if not emitted and self.jal_load_nop:
                # ASPSX attaches the load hazard nop to the load whenever the
                # next instruction is a jump/branch, whatever its delay slot
                # reads (retail mul_dmat: `lw a1 | nop | jal | move
                # a0,s0` x4).  Superset of --delay-slot-load-nop; gated.
                m = re.match(r"^([a-z]+)\s", strip_comments(next_instruction))
                op = m.group(1) if m else None
                if op in branch_mnemonics or op in jump_mnemonics or op in (
                    "beqz", "bnez", "jr", "jalr", "b"
                ):
                    res.append(
                        f"nop # DEBUG: jal-load-nop ('{next_instruction}' follows the load of {r_dest})"
                    )
                    emitted = True
            if not emitted and self.delay_slot_load_nop:
                # ASPSX (retail libsnd/libcd objects): a branch or jump and
                # its delay slot are indivisible for hazard purposes.  When
                # the instruction after a load is a branch/jump whose DELAY
                # SLOT reads the loaded register, ASPSX cannot put the
                # load-delay nop inside the slot and emits it BEFORE the
                # branch (lw v0 | nop | j L | srav v0,v0,v1).  gnu-as sees
                # the consumer two slots away and needs nothing.  Gated:
                # the retail census is 10:1 (j) / 7:3 (jal), so it is a
                # per-file flag like --load-base-nop / --mflo-nops-first.
                m = re.match(r"^([a-z]+)\s", strip_comments(next_instruction))
                op = m.group(1) if m else None
                if op in branch_mnemonics or op in jump_mnemonics or op in (
                    "beqz", "bnez", "jr", "jalr", "b"
                ):
                    # The DELAY SLOT is whatever literally follows the
                    # branch.  A `.set reorder` branch has NO slot in the
                    # input at all, and an ignore_label/ignore_nop lookup
                    # walks past the label into the NEXT block and reads its
                    # first instruction as if it were the slot -- which in
                    # rcos picked up `subu v0,zero,v0` from the arm
                    # after the `j` and invented a nop retail does not have
                    # (0x800a4190 is `lh v0,9664(at); j; nop`).  An empty
                    # slot is already a full delay, so there is no hazard.
                    slot = self._explicit_delay_slot(next_instruction)
                    if slot and line_loads_from_reg(slot, r_dest):
                        res.append(
                            f"nop # DEBUG: delay-slot-load-nop ('{slot}' in the delay slot of '{next_instruction}' reads {r_dest})"
                        )
                        emitted = True
            if not emitted:
                res.append(
                    f"#nop # DEBUG: '{next_instruction}' does not load from {r_dest}"
                )

        return res

    def _loc_boundary_follows(self) -> bool:
        """True when a `.loc` directive sits between the current line and
        the next instruction (directives, labels, `.set` and `#nop` hints
        are looked through)."""
        i = self.line_index + 1
        while i < len(self.lines):
            line = self.lines[i]
            if line.startswith(".loc"):
                return True
            if line == "#nop" or line.startswith(".set") or is_label(line):
                i += 1
                continue
            if is_instruction(line):
                return False
            i += 1
        return False

    def _explicit_delay_slot(self, branch_line: str) -> str:
        """Return the instruction literally occupying `branch_line`'s delay
        slot, or "" when the slot is empty.

        gcc emits a filled slot only inside a `.set noreorder` block, where
        the slot instruction is the very next line.  A plain `.set reorder`
        branch is followed by a label or another `.set`, and the assembler
        fills the slot itself -- so for hazard purposes the slot is empty.
        """
        i = self.line_index + 1
        while i < len(self.lines) and self.lines[i] != branch_line:
            i += 1
        if i >= len(self.lines):
            return ""
        j = i + 1
        while j < len(self.lines):
            line = self.lines[j]
            if not line or line.startswith("#") or line.startswith("."):
                if line.startswith(".set"):
                    return ""
                j += 1
                continue
            if is_label(line):
                return ""
            return line
        return ""

    def _store_split_plan(self, op: str, r_store: str, symbol: str):
        """THE ASPSX STORE-MACRO SPLIT.

        `s{b,h,w} rt,SYM` expands to `lui $at,%hi(SYM) ; s.. rt,%lo(SYM)($at)`.
        Retail does not keep the two halves adjacent: it emits the `lui $at`,
        then the whole `la` macro that follows, and only then the store.
        Derived from getintr words 113-116 (`lui at ; lui v1 ;
        addiu v1 ; sw v0,11720(at)`), confirmed unchanged on CD_ready
        words 79-82 and 100-103.  See store_split_crossable above for why the
        crossed instruction must be an address macro and not any plain word.

        Returns the number of following SOURCE INSTRUCTION LINES the store
        sinks past, or None when the split does not apply.  The store stops
        *before*:
          - a label (a branch target -- the store must stay in its own block),
          - a branch or jump, and their delay slots,
          - anything that writes $at or expands through $at (it would clobber
            the pending base register),
          - anything that writes the stored register (it would store the wrong
            value -- this is getintr words 215/216, which retail does NOT
            split because the next macro's %hi register is the one being
            stored),
          - any memory reference (the store must not be reordered against a
            load or another store), or a `#nop` load-delay marker,
          - anything whose written register cannot be determined.
        `store_macro_split_window` caps the distance in emitted words at the
        largest one actually observed in retail (3).
        """
        if not self.store_macro_split:
            return None

        # $zero is never written, so the stored-register barrier cannot fire.
        watch_store_reg = r_store not in ("$0", "$zero")
        r_store_re = re.escape(r_store) + r"(?![0-9])"

        crossed_lines = 0
        crossed_words = 0
        i = self.line_index + 1
        while i < len(self.lines):
            raw = self.lines[i]
            i += 1

            if len(raw) == 0 or raw.startswith("#APP") or raw.startswith("#NO_APP"):
                break
            if raw == "#nop":
                # an unresolved load-delay marker: the nop machinery owns this
                # slot, so do not reorder a store through it
                break
            if raw.startswith("#"):
                continue
            if is_label(raw) or raw.endswith(":"):
                break          # branch target
            if raw.startswith(".set"):
                continue       # noreorder/nomacro bookkeeping, emits nothing
            if raw.startswith("."):
                if raw.startswith(".loc") or raw.startswith(".stab"):
                    continue
                break          # .end / .ent / section change
            if ";" in raw:
                break          # a macro line -- cannot cost or clobber-check it

            line = strip_comments(raw)
            if not line:
                continue

            parts = line.split()
            cop = parts[0]
            if cop not in store_split_crossable:
                break          # branch, jump, load, store, plain ALU word, ...
            if cop == "la" and self.sdata_limit > 0:
                break          # may collapse to one gp-relative word: cost unknown
            if uses_at(line) or self._uses_gp(line):
                break
            rest = " ".join(parts[1:])
            if "(" in rest:
                break          # a base-register form: memory, refuse
            operands = [x.strip() for x in rest.split(",")]
            if not operands or not operands[0].startswith("$"):
                break
            r_written = operands[0]
            if r_written == "$at":
                break
            if watch_store_reg and re.match(r_store_re + r"$", r_written):
                break

            cost = store_split_word_cost.get(cop, 1)
            if crossed_words + cost > self.store_macro_split_window:
                break          # would sink further than retail ever does
            crossed_lines += 1
            crossed_words += cost

        if crossed_lines == 0:
            return None
        return crossed_lines

    def _reuse_at_preserved(self, line: str, chunk: List[str]) -> bool:
        """--reuse-at-same-hi: does source line `line` (which emitted
        `chunk`) leave a live `lui $at,%hi(SYM)` intact AND keep us in the
        same basic block?  Conservative whitelist: anything not provably
        $at-neutral straight-line code ends the reuse window."""
        text = line.strip()
        if text == "" or text.startswith("#"):
            return True
        if text.startswith("."):
            # debug-line bookkeeping only; .set (noreorder/nomacro blocks
            # wrap gcc's filled branches), .end/.ent, sections: stop
            return text.startswith(".loc") or text.startswith(".stab")
        if is_label(text) or text.endswith(":") or text.startswith("$L"):
            return False                       # a branch target: new block
        if ";" in text:
            return False                       # multi-word macro line
        for out in chunk:
            o = out.strip()
            if not o or o.startswith("#") or o.startswith("."):
                continue
            if re.search(r"\$(at|1)(?![0-9])", strip_comments(o)):
                return False                   # an $at expansion / explicit use
        code = strip_comments(text)
        if re.search(r"\$(at|1)(?![0-9])", code):
            return False
        parts = code.split(None, 1)
        op = parts[0]
        rest = parts[1] if len(parts) > 1 else ""
        if op in branch_mnemonics or op in jump_mnemonics or op in (
                "b", "bal", "beqz", "bnez", "jr", "jalr", "bltzal", "bgezal",
                "beql", "bnel", "break", "syscall"):
            return False                       # block end (call, branch, return)
        if op == "nop":
            return True
        if op in load_mnemonics:
            try:
                r_source, r_dest, operand, is_addend, _ = parse_load_or_store(rest)
            except Exception:
                return False
            if r_dest in ("$0", "$zero"):
                return False                   # GNU as would expand via $at
            if r_source is None:
                return is_addend               # `lX rd,SYM`: expanded via rd
            if is_addend:
                return False                   # SYM(reg): $at expansion
            return -32768 <= int(operand, 0) <= 32767
        if op in store_mnemonics:
            try:
                r_source, _, operand, is_addend, _ = parse_load_or_store(rest)
            except Exception:
                return False
            if r_source is None or is_addend:
                return False                   # a symbol store writes $at
            return -32768 <= int(operand, 0) <= 32767
        if op == "li":
            return True                        # expanded through rd
        if op == "la":
            return "(" not in rest
        if op in reuse_at_alu_ops:
            ops = [x.strip() for x in rest.split(",")]
            if not ops or not ops[0].startswith("$"):
                return False
            last = ops[-1]
            if last.startswith("$"):
                return True
            if not is_number(last):
                return False
            v = int(last, 0)
            if op in ("andi", "ori", "xori", "and", "or", "xor", "lui"):
                return 0 <= v <= 0xFFFF
            if op in ("sll", "srl", "sra"):
                return 0 <= v <= 31
            return -32768 <= v <= 32767
        return False

    def _reuse_at_store(self, op: str, r_store: str, operand: str, line: str):
        """--reuse-at-same-hi (ASPSX same-symbol $at reuse, gcc path).

        ASPSX expands `s{b,h,w} rt,SYM+N` as `lui $at,%hi(SYM+N) ;
        s.. rt,%lo(SYM+N)($at)`.  When a LATER store in the same basic
        block names the SAME symbol and nothing in between writes $at, it
        emits only `s.. rt2,%lo(SYM+M)($at)` -- the first `lui` is reused.
        Retail, gcc260 sound-driver TU: CD_readm 0x8008ADCC/ADD8
        (`lui at ; sw a1,DD8 ; li v0,16 ; sw v0,DD4(at)`) and
        0x8008ADE4/ADF0 (`lui at ; sw v1,DF4 ; jal ; sw a0,DF8(at)`),
        cd_init_retry 0x8008A9E8/AA0C.

        Returns the lines to emit for this store, or None to leave the store
        to the ordinary expansion (which then ARMS $at when the symbol's
        address is known).  Safety: the symbol is compared by NAME, and the
        reuse also requires the address derived from that name
        (`..._XXXXXXXX`) to give the SAME %hi for both displacements, so the
        shared %hi is exactly right and the new %lo is in range against it.
        A symbol without a derivable address never arms $at, and neither
        does one outside the initialised image (bss; see REUSE_AT_DATA_HI):
        retail re-emits the lui there even for one symbol."""
        if not self.reuse_at_same_hi:
            return None
        if r_store in ("$at", "$1"):
            self.at_live = None
            return None
        parsed = reuse_at_split_operand(operand)
        if parsed is None:
            return None
        sym, off = parsed
        addr = reuse_at_sym_addr(sym)
        live = self.at_live
        self._at_armed_this_line = True
        if (live is not None and addr is not None and live[0] == sym
                and reuse_at_hi(live[2] + live[1]) == reuse_at_hi(addr + off)):
            self.reuse_at_count += 1
            msg = (f"reuse-at-same-hi: '{line}' reuses the live "
                   f"`lui $at,%hi({live[0]}{live[1]:+d})` "
                   f"(%hi 0x{reuse_at_hi(addr + off):04X}); no new lui")
            if os.environ.get("MASPSX_TRACE"):
                sys.stderr.write(msg + "\n")
            if os.environ.get("MASPSX_TRACE_FILE"):
                # census hook: gccwrap/build capture maspsx's stderr
                with open(os.environ["MASPSX_TRACE_FILE"], "a") as tf:
                    tf.write(msg + "\n")
            return [
                "# EXPAND_AT START",
                ".set\tnoat",
                f"{op}\t{r_store},%lo({operand})($at) # DEBUG: {msg}",
                ".set\tat",
                "# EXPAND_AT END",
            ]
        # ordinary two-word expansion (GNU as): it now owns $at.  Only an
        # initialised-image symbol arms a reuse (bss: retail re-emits).
        if addr is not None and REUSE_AT_DATA_LO <= addr + off < REUSE_AT_DATA_HI:
            self.at_live = (sym, off, addr)
        else:
            self.at_live = None
        return None

    def _maybe_macro_lui_fill(self, extra_nops: List[str], r_dest: str) -> List[str]:
        """ASPSX reorder mode fills a load-delay slot with the first half of
        the NEXT symbol-load macro (`lw v0,SYM1 | lui a0,%hi(SYM2) |
        lw s0,0(v0) | lw a0,%lo(SYM2)(a0)`, retail VSync words 41-45)
        instead of a nop.  maspsx never reorders across macros, so under
        --macro-lui-fill we split that macro ourselves: its `lui` replaces
        the nop and its `%lo` half is emitted when the macro line arrives.
        Legal only when the lui's register is neither the loaded register
        nor read/written by the consumer, and nothing but the consumer sits
        between the load and the macro.

        --macro-lui-fill-far extends the search past intervening plain
        instructions (retail MDEC_reset words 37-48: the slot of
        `lw v0,SYM1 | nop | sw zero,0(v0)` is filled by the `lui v1` of the
        THIRD following macro, the two nearer ones being `lw v0,...` macros
        whose register the pending consumer still reads).  The lui may move
        up only across instructions that neither read nor write its
        register; the search stops at a label, branch, jump or macro
        (`;`) and never crosses another candidate it rejected.

        --macro-lui-fill-cross (implies far) additionally CONTINUES past
        macro lines it cannot split (`;`), treating them as ordinary
        instructions the lui must be independent of (retail CD_getsector
        words 25-38: the slot of `lw v1,E24` is filled by the `lui a0` of
        the SIXTH following macro).  Built 2026-09-17; no registered
        function ships it yet -- CD_getsector itself is blocked upstream
        by register allocation.  The search still stops at a label, branch
        or jump."""
        if not (self.macro_lui_fill or self.macro_lui_fill_far or self.macro_lui_fill_cross) or self.pending_lui_fill is not None:
            return extra_nops
        if self.pending_lo_sink is not None and self.pending_lo_sink[0] == self.line_index:
            return extra_nops    # this delay is reserved for a sunk %lo half
        idx = next((i for i, l in enumerate(extra_nops) if l.startswith("nop")), None)
        if idx is None:
            return extra_nops
        consumer = self.get_next_instruction(skip=0, ignore_nop=True, ignore_set=True, ignore_label=False)
        if not consumer or is_label(consumer):
            return extra_nops
        c = strip_comments(consumer)
        cop = c.split()[0]
        if cop in branch_mnemonics or cop in jump_mnemonics or cop in ("beqz", "bnez", "jr", "jalr", "b"):
            return extra_nops
        between = [c]          # instructions the lui would be hoisted across
        far = self.macro_lui_fill_far or self.macro_lui_fill_cross
        max_skip = 12 if far else 1
        for skip in range(1, max_skip + 1):
            after = self.get_next_instruction(skip=skip, ignore_nop=True, ignore_set=True, ignore_label=False)
            if not after or is_label(after):
                return extra_nops
            a = strip_comments(after)
            aop = a.split()[0]
            if aop in branch_mnemonics or aop in jump_mnemonics or aop in ("beqz", "bnez", "jr", "jalr", "b"):
                return extra_nops
            m = re.match(r"^([a-z]+)\s+(.*)$", a)
            cand = None
            if m and m.group(1) in load_mnemonics and ";" not in after:
                try:
                    r_source, rd, operand, is_addend, needs_expanding = parse_load_or_store(m.group(2))
                    if r_source is None and is_addend and needs_expanding:
                        cand = (rd, operand, m.group(1), None)
                    elif (self.load_dest_temp and r_source is not None and is_addend
                          and needs_expanding and rd != r_source and rd not in ("$0", "$zero")):
                        cand = (rd, operand, m.group(1), r_source)   # dest-temp macro: its lui can fill too
                except Exception:
                    cand = None
            if cand is not None:
                rd, operand, lop, csrc = cand
                rd_re = re.escape(rd) + r"(?![0-9])"
                if rd != r_dest and not any(re.search(rd_re, b) for b in between):
                    extra_nops = list(extra_nops)
                    extra_nops[idx] = f"lui\t{rd},%hi({operand}) # DEBUG: macro-lui-fill (first half of '{after}' fills the load delay of {r_dest})"
                    self.pending_lui_fill = (a, lop, rd, operand, csrc)
                    return extra_nops
                if not far:
                    return extra_nops
            elif not far:
                return extra_nops
            if ";" in after and not self.macro_lui_fill_cross:
                return extra_nops
            between.append(a)
        return extra_nops

    def _lo_sink_target(self, rd: str):
        """--macro-lui-fill-split: where the `%lo` half of the symbol-load
        macro `lX rd,SYM` on the current line goes.

        ASPSX does not keep a symbol-load macro's two words together.  Its
        `lui rd,%hi(SYM)` stays where the macro is (or, with
        --macro-lui-fill*, fills an EARLIER load delay), and its
        `lX rd,%lo(SYM)(rd)` sinks DOWN into the nearest LATER load-delay
        slot, where it replaces the hazard nop.  Retail, all gcc260/272:

          CD_cachefile 0x8008974C   lui v0 | lui v1 | lb v1,E3E(v1) |
                                 lh v0,E3C(v0) | sb v1,34(s5)
          GsSortFastSprite 0x80099BE0 lui v1 | lui a0 | lhu a0,179A(a0) |
                                 lhu v1,1798(v1) | addu a1,a1,a0
          _mk_normsp 0x8009E600  lh v0,18(a2) | lui a1 | move a3,a0 |
                                 addu v1,v1,v0 | lh v0,20(a2) | lh a0,12(a2) |
                                 lh a1,1798(a1) | addu a0,a0,v0 | addu t1,v1,a1

        In each the first macro's `%lo` load sits in the delay slot of a
        LATER load (a symbol load or a plain one) whose consumer follows it
        directly.  The sink is legal only across straight-line words that
        neither read nor write rd; it stops at a label, a branch or jump, a
        `.set` block, a macro line (`;`), a store (the %lo load must not
        pass a possibly-aliasing write) and after 12 words.  The target load
        must write a register other than rd, be followed directly by a
        reader of that register (which is what makes the nop) that does not
        use $at or rd.  Returns the target line index or None (no sink: the
        macro is emitted whole, exactly as without the flag).

        Built 2026-09-24; no file ships it.  Swept over the 38 gcc files
        that mention --fill-load-delay: it fires at exactly one site, the
        `lh v1,D_8013179A` macro of _mk_normsp (141 -> 140/153), where
        retail keeps that macro whole below the `lhu v0,22` -- the retail
        sites above need a gcc emission order (the first macro's symbol
        loaded FIRST) that the current C files do not produce."""
        if not self.macro_lui_fill_split or self.pending_lo_sink is not None:
            return None
        rd_re = re.escape(rd) + r"(?![0-9A-Za-z_])"
        stops = ("beqz", "bnez", "jr", "jalr", "b", "bal", "break", "syscall",
                 "bltzal", "bgezal", "beql", "bnel",
                 # division macros carry a zero-check branch (and, under
                 # --defer-div, a deferred check pseudo-op): a block boundary
                 "div", "divu", "rem", "remu", "divchk", "divuchk")
        i = self.line_index + 1
        crossed = 0
        while i < len(self.lines) and crossed < 12:
            raw = self.lines[i]
            if (not is_instruction(raw, ignore_nop=True, ignore_set=False, ignore_label=False)
                    or raw.startswith("#")):
                i += 1
                continue
            if is_label(raw) or raw.startswith(".") or ";" in raw:
                return None
            a = strip_comments(raw)
            if not a:
                i += 1
                continue
            op = a.split()[0]
            if (op in branch_mnemonics or op in jump_mnemonics or op in stops
                    or op in store_mnemonics):
                return None
            if re.search(rd_re, a):
                return None
            if op in load_mnemonics:
                try:
                    _, r_k, _, _, _ = parse_load_or_store(" ".join(a.split()[1:]))
                except Exception:
                    r_k = None
                j = i + 1
                nxt = ""
                while j < len(self.lines):
                    cand = self.lines[j]
                    if is_instruction(cand, ignore_nop=True, ignore_set=True, ignore_label=False) \
                            and not cand.startswith("#"):
                        nxt = cand
                        break
                    j += 1
                if (r_k and r_k not in ("$0", "$zero") and r_k != rd and nxt
                        and not is_label(nxt) and ";" not in nxt
                        and line_loads_from_reg(nxt, r_k) and not uses_at(nxt)
                        and not re.search(rd_re, strip_comments(nxt))):
                    return i
            crossed += 1
            i += 1
        return None

    def _prev_is_div_macro(self) -> bool:
        """The mflo/mfhi being processed is the tail of a division macro:
        the previous instruction line is the deferred check (`divchk` /
        `divuchk`) or the `div $zero,rs,rt` itself."""
        i = self.line_index - 1
        while i >= 0:
            prev = strip_comments(self.lines[i]).strip()
            if not prev or prev == "#nop" or prev.startswith("."):
                i -= 1
                continue
            return (prev.startswith(("divchk", "divuchk"))
                    or re.match(r"^divu?\s+\$(zero|0)\s*,", prev) is not None)
        return False

    def _div_macro_writer_slot(self, nxt, mflo_dest) -> bool:
        """--mflo-div-writer-slot: carve-out from --mflo-nops-first.

        When a DIVISION MACRO's mflo/mfhi is followed by a word that writes
        an operand of the mult/div right after it (and does not read the
        moved register), ASPSX counts that word as one of the two hazard
        slots: retail GsSetFlatLight 0x8009CDAC / 0x8009CED0 / 0x8009CFF0
        `mflo a0 ; sll a1,s2,12 ; nop ; mult a1,a3`, where nops-first gives
        `mflo ; nop ; nop ; sll ; mult`.  Restricted to the macro's mflo on
        purpose: a PLAIN mult's mflo keeps nops-first in exactly this shape
        in two byte-perfect LIBGTE files -- RotMatrix_C 0x800A3844
        `mflo t0 ; nop ; nop ; negu a2,s0 ; mult s2,a2` and RotMatrixY_C
        0x800A3D60 `mflo a0 ; nop ; nop ; negu v0,v0 ; mult v0,v1`."""
        if not self._prev_is_div_macro():
            return False
        md = strip_comments(nxt[1]).strip()
        if not md.startswith(("mult\t", "multu\t", "div\t", "divu\t")):
            return False
        w0 = strip_comments(nxt[0]).strip()
        if not w0:
            return False
        _, writes, reads, _ = _dd_classify(w0)
        _, _, md_reads, _ = _dd_classify(md)
        if _dd_norm(mflo_dest) in reads:
            return False
        return bool(writes & (md_reads - {"0"}))

    def _mflo_noreorder_pad(self) -> List[str]:
        """--mflo-noreorder-nops: a compiler `.set noreorder` block (a
        branch/jump whose delay slot gcc filled itself) never starts within
        two instructions of an mflo/mfhi -- ASPSX cannot schedule inside the
        block, so it pads up to two words in front of it.  Retail:
          GsSetFlatLight 0x8009CC84  mflo a0 ; nop ; nop ; jal ; addu a0,v1,a0
          GsSortBg 0x80099E00  mflo t7 ; nop ; nop ; bnez v0 ; sw t7
          GsSortBg 0x80099F44  mfhi t6 ; nop ; nop ; bgez t6 ; move s6,t6
          GsSetRefView2 0x8009A614  mflo a0 ; nop ; nop ; jal ; addu a0,a1,a0
          GsSetRefView2 0x8009A6A8  mflo a0 ; addu a0,a1,a0 ; nop ; jal ; negu
          GsSetRefView2 0x8009A700  mflo a2 ; sll a2,a2,16 ; nop ; jal ; sra
        (and the same three shapes at 0x8009EC8C / 0x8009ED20 / 0x8009ED74).
        A reorder-mode branch (slot left for the assembler) gets none:
        0x80099EF8 `mfhi t2 ; bgez t2 ; nop`, 0x8009A278 `mfhi v0 ; bnez t3
        ; nop`.  Two words already between (0x8009E2DC `mflo v0 ; sra ; sh ;
        jr ra ; move`) need none.  A label in between stops the look-back."""
        out = getattr(self, "_out", [])
        seen = 0
        for prev in reversed(out):
            t = strip_comments(prev).strip()
            if not t or t.startswith(".") or t.startswith("#"):
                continue
            if t.endswith(":"):
                return []
            op = t.split()[0]
            if op in ("mflo", "mfhi"):
                return ["nop  # DEBUG: mflo/mfhi before a noreorder block"] * (2 - seen)
            seen += 1
            if seen >= 2:
                return []
        return []

    def _mflo_consumer_fill(self, md_index: int, mflo_dest: str):
        """--mflo-consumer-fill: of the two hazard nops between an
        mflo/mfhi and the mult after it, ASPSX fills the first with the word
        that FOLLOWS the mult when that word reads the moved register and is
        independent of the multiply: retail GsSetFlatLight 0x8009CC74
        `mflo a0 ; addu v1,v1,a0 ; nop ; mult v0,v0` for gcc's `mflo $4 ;
        lw $2,8($16) ; mult $2,$2 ; addu $3,$3,$4` (also GsSetRefView2
        0x8009A604 and 0x8009EC7C).  A follower that does NOT read the
        moved register stays below the mult (byte-perfect LIBGTE
        RotMatrixX_C 0x800A3CAC `mflo v1 ; nop ; nop ; mult s0,v0 ; addu
        a1,a1,a3`), and a division macro keeps both nops (mul_rdmat
        0x8009F3A8 `mflo a1 ; nop ; nop ; div ; sll a1,a1,16`), so only
        `mult`/`multu` qualify.  Returns the follower's line index or
        None."""
        if md_index is None or md_index + 1 >= len(self.lines):
            return None
        md = strip_comments(self.lines[md_index]).strip()
        if not md.startswith(("mult\t", "multu\t")):
            return None
        j = md_index + 1
        while j < len(self.lines) and self.lines[j] == "#nop":
            j += 1
        if j >= len(self.lines):
            return None
        text = strip_comments(self.lines[j]).strip()
        if not text or text.startswith((".", "#", "$")) or text.endswith(":") or ";" in text:
            return None
        op, writes, reads, at = _dd_classify(text)
        if op is None or at or op in _DD_HILO or op in _DD_BLOCK_END:
            return None
        if op not in _MFLO_FILL_OPS:
            return None                  # plain register ALU words only
        _, _, md_reads, _ = _dd_classify(md)
        if _dd_norm(mflo_dest) not in reads:
            return None
        if writes & (md_reads - {"0"}):
            return None
        return j

    def _handle_mflo_mfhi(self, r_source=None, mflo_dest=None) -> List[str]:
        # we cannot use a div/mult within 2 instructions of mflo/mfhi
        res: List[str] = []

        if not self.nop_mflo_mfhi:
            return res

        if self.mflo_nops_first and mflo_dest:
            # ASPSX rule observed in retail LIBGTE (RotMatrix family): when a
            # mult/div follows within the next three instructions and the very
            # next instruction does not read the moved register, BOTH hazard
            # nops are placed immediately after the mflo/mfhi.  When the next
            # instruction does read it (or is itself the mult), the standard
            # handling below applies.
            _nxt = [
                self.get_next_instruction(
                    skip=k, ignore_nop=True, ignore_set=True, ignore_label=True
                )
                for k in range(3)
            ]
            _is_md = lambda x: any(
                x.startswith(y)
                for y in ["mult\t", "multu\t", "div\t", "divu\t", "rem\t", "remu\t"]
            )
            if (
                not _is_md(_nxt[0])
                and any(_is_md(x) for x in _nxt[1:])
                and not line_loads_from_reg(_nxt[0], mflo_dest)
                and not (self.mflo_div_writer_slot
                         and self._div_macro_writer_slot(_nxt, mflo_dest))
            ):
                return [
                    "nop  # DEBUG: mflo/mfhi nops-first",
                    "nop  # DEBUG: mflo/mfhi nops-first",
                ]

        next_instruction = self.get_next_instruction(
            skip=0, ignore_nop=True, ignore_set=True, ignore_label=True
        )
        next_next_instruction = self.get_next_instruction(
            skip=1, ignore_nop=True, ignore_set=True, ignore_label=True
        )

        if any(
            next_instruction.startswith(x)
            for x in ["mult\t", "multu\t", "div\t", "divu\t", "rem\t", "remu\t"]
        ):
            # #nop
            # #nop
            # mult...
            skip = 0
            while True:
                inst = self.get_next_instruction(skip=skip)
                skip += 1
                if inst == next_instruction:
                    res.append("nop")
                    res.append("nop")
                    if div_needs_expanding(inst):
                        res.append("# DEBUG: div needs expanding")
                        skip -= 1
                    else:
                        res.append(expand_move(inst))
                    break
                if not inst.startswith("#"):
                    res.append(expand_move(inst))
            self.skip_instructions = skip

        elif any(
            next_next_instruction.startswith(x)
            for x in ["mult\t", "multu\t", "div\t", "divu\t", "rem\t", "remu\t"]
        ):
            # #nop
            # #nop
            # addu or lh ...
            # mult ...
            skip = 0
            no_reorder = False
            while True:
                inst = self.get_next_instruction(skip=skip)
                if inst.startswith(".set") and inst.endswith("noreorder"):
                    no_reorder = True
                    skip += 1
                    continue

                skip += 1
                if inst == next_instruction:
                    op, *_ = inst.strip().split()
                    if op in load_mnemonics:
                        # allow for $at handling later in the script
                        skip = 0
                        break

                    if op in ("mflo", "mfhi"):
                        # allow for mflo/mfhi handling later on
                        skip = 0
                        break

                    if op == "li":
                        expanded = expand_load_immediate(inst)

                        if self.expand_li:
                            res += expanded
                        else:
                            res.append(inst)

                        if len(expanded) == 2:
                            res.append(
                                "#nop  # DEBUG: mflo/mfhi with mult/div/rem and li expands to 2 ops"
                            )
                        else:
                            res.append(
                                "nop  # DEBUG: mflo/mfhi with mult/div/rem and li expands to 1 op"
                            )
                    else:
                        if no_reorder:
                            res.append(
                                "nop  # DEBUG: mflo/mfhi with mult/div/rem and 1 instruction (noreorder)"
                            )
                            res.append(".set\tnoreorder")
                            res.append(expand_move(inst))
                        else:
                            if r_source and line_loads_from_reg(inst, r_source):
                                # NOTE: only relevant when div has been expanded (i.e. -0 flag)
                                res.extend(
                                    [
                                        f"nop  # DEBUG: mflo/mfhi with mult/div/rem and 1 instruction which loads from {r_source}",
                                        expand_move(inst),
                                    ]
                                )
                            else:
                                maybe_label = self.get_next_instruction(skip=skip)
                                if is_label(maybe_label):
                                    res.extend(
                                        [
                                            expand_move(inst),
                                            maybe_label,
                                            "nop  # DEBUG: mflo/mfhi with mult/div/rem and 1 instruction (label)",
                                        ]
                                    )
                                    skip += 1
                                else:
                                    res.extend(
                                        [
                                            expand_move(inst),
                                            "nop  # DEBUG: mflo/mfhi with mult/div/rem and 1 instruction",
                                        ]
                                    )

                elif inst == next_next_instruction:
                    # reached mult/div/rem
                    if div_needs_expanding(inst):
                        res.append("# DEBUG: div needs expanding")
                        skip -= 1
                    else:
                        res.append(inst)
                    break
                elif not inst.startswith("#"):
                    res.append(expand_move(inst))
            self.skip_instructions = skip

        else:
            # do nothing
            pass

        return res

    def process_line(self, line):
        res = []

        if len(line) == 0:
            return [line]

        if line.startswith("#"):
            return []

        if line.startswith("."):
            if (
                line.startswith(".def\t")
                or line.startswith(".begin\t")
                or line.startswith(".bend\t")
            ):
                # skip these coff directives - gnu as does not like them
                pass

            elif line.startswith(".set\t"):
                if line.endswith("\tnoreorder"):
                    if self.mflo_noreorder_nops and self.is_reorder:
                        res += self._mflo_noreorder_pad()
                    self.is_reorder = False
                elif line.endswith("\treorder"):
                    self.is_reorder = True

            elif line.startswith(".file\t"):
                # fix same-numbered files
                _, num, filename = line.split()
                res.append(f".file\t{self.file_num} {filename}")
                self.file_num += 1

            elif line.startswith(".ent\t"):
                # enforce noreorder for each function
                res.append(line)
                res.append(".set\tnoreorder")

            elif line.startswith(".comm") or line.startswith(".lcomm"):
                # already handled via preprocess_lines
                pass

            elif line.startswith(".data"):
                res.append(".section .data")
            elif line.startswith(".sdata"):
                res.append(".section .sdata")
            elif line.startswith(".rdata"):
                res.append(".section .rodata")

            else:
                res.append(line)

            return res

        if line.startswith("$L"):
            return [line]

        actual_r_dest = None
        is_macro = ";" in line
        if is_macro:
            expanded = expand_macro(line)
            if len(expanded) > 0:
                actual_op, *actual_rest = expanded[-1]
                if actual_op in load_mnemonics:
                    _, actual_r_dest, _, _, _ = parse_load_or_store(
                        " ".join(actual_rest)
                    )

        op, *rest = line.split()

        if op in load_mnemonics:
            rest = " ".join(rest)
            r_source, r_dest, operand, is_addend, needs_expanding = parse_load_or_store(
                rest
            )

            next_instruction = self.get_next_instruction(
                skip=0, ignore_nop=True, ignore_set=True, ignore_label=True
            )
            # Naively handle scenario where *next* line is a macro
            if ";" in next_instruction:
                next_instruction = next_instruction.split(";")[0]

            if self.pending_lui_fill and strip_comments(line) == self.pending_lui_fill[0]:
                # second half of a macro whose lui already filled a load delay
                _, pop, prd, psym, psrc = self.pending_lui_fill
                self.pending_lui_fill = None
                tgt = self._lo_sink_target(prd) if psrc is None else None
                if tgt is not None:
                    # --macro-lui-fill-split: the lui already filled an
                    # earlier load delay; the %lo half fills a later one.
                    self.pending_lo_sink = (tgt, [
                        f"{pop}\t{prd},%lo({psym})({prd}) # DEBUG: macro-lui-fill-split, %lo half of '{line}' sunk into a later load delay"])
                    return res
                if psrc is not None:
                    res.append(f"addu\t{prd},{prd},{psrc} # DEBUG: macro-lui-fill second half (dest-temp index add)")
                res.append(f"{pop}\t{prd},%lo({psym})({prd}) # DEBUG: macro-lui-fill second half")
                res.extend(self._handle_nop_before_next_instruction(next_instruction, r_dest))
                return res

            if not needs_expanding:
                # newer GCCs can emit %hi() and %lo() separately...
                res.append(f"{line} # DEBUG: leaving for assembler to expand")
                extra_nops = self._handle_nop_before_next_instruction(
                    next_instruction, r_dest
                )
                res.extend(extra_nops)

            elif is_addend and r_source is None:
                # e.g. lb	$s0,D_800E52E0
                if operand.count("+") == 1:
                    symbol, offset = operand.split("+")
                    gp_rel = f"%gp_rel({symbol}+{offset})($gp)"
                    gp_allowed = self.gp_allow_offset or symbol not in self.comm_symbols
                else:
                    symbol = operand
                    gp_rel = f"%gp_rel({symbol})($gp)"
                    gp_allowed = True

                tgt = None
                if gp_allowed and (
                    symbol in self.sdata_entries or symbol in self.sbss_entries
                ):
                    res.append(f"{op}\t{r_dest},{gp_rel}")
                else:
                    tgt = self._lo_sink_target(r_dest)
                    if tgt is not None:
                        # --macro-lui-fill-split: lui here, %lo half later
                        res.append(f"lui\t{r_dest},%hi({operand}) # DEBUG: macro-lui-fill-split, first half of '{line}'")
                        self.pending_lo_sink = (tgt, [
                            f"{op}\t{r_dest},%lo({operand})({r_dest}) # DEBUG: macro-lui-fill-split, %lo half of '{line}' sunk into a later load delay"])
                    else:
                        res.append(line)

                if tgt is None:
                    extra_nops = self._handle_nop_before_next_instruction(
                        next_instruction, r_dest
                    )
                    extra_nops = self._maybe_macro_lui_fill(extra_nops, r_dest)
                    res.extend(extra_nops)

            elif is_addend and r_source:
                # e.g. lw	$2,test_sym($4)
                if self.load_dest_temp and r_dest != r_source and r_dest != "$0" and r_dest != "$zero":
                    # ASPSX: a reg+symbol LOAD uses its own destination as the
                    # scratch register (`lui rd ; addu rd,rd,rs ; lw rd,%lo(rd)`)
                    # and falls back to $at only when rd is the index register.
                    # Retail library region: 35 dest-temp / 28 $at(rd==rs) / 0
                    # counter-examples (2026-09-05).
                    res.extend(
                        [
                            f"lui\t{r_dest},%hi({operand})",
                            f"addu\t{r_dest},{r_dest},{r_source}",
                            f"{op}\t{r_dest},%lo({operand})({r_dest})",
                        ]
                    )
                elif self.addiu_at:
                    res.extend(
                        [
                            "# EXPAND_AT START",
                            ".set\tnoat",
                            f"lui\t$at,%hi({operand})",
                            f"addiu\t$at,$at,%lo({operand})",
                            f"addu\t$at,$at,{r_source}",
                            f"{op}\t{r_dest},0x0($at)",
                            ".set\tat",
                            "# EXPAND_AT END",
                        ]
                    )
                else:
                    res.extend(
                        [
                            "# EXPAND_AT START",
                            ".set\tnoat",
                            f"lui\t$at,%hi({operand})",
                            f"addu\t$at,$at,{r_source}",
                            f"{op}\t{r_dest},%lo({operand})($at)",
                            ".set\tat",
                            "# EXPAND_AT END",
                        ]
                    )

                extra_nops = self._handle_nop_before_next_instruction(
                    next_instruction, r_dest
                )
                res.extend(extra_nops)

            else:
                if (self.load_dest_temp and r_source
                        and (int(operand, 0) > 32767 or int(operand, 0) < -32768)):
                    # ASPSX, NUMERIC displacement outside imm16 with an index
                    # register (e.g. `lw $2,528486536($5)`): the same rule as
                    # the reg+symbol case above.  The address is built in the
                    # LOAD's own destination (`lui rd,%hi(N) ; addu rd,rd,rs ;
                    # lw rd,%lo(N)(rd)`); $at is used only when rd is the
                    # index register (or $zero), and then with $at FIRST in
                    # the addu (`lui at ; addu at,at,rs`).  Retail
                    # dma_execute words 7/9/14, 27-29 (dest-temp) and
                    # 49-51 (rd == rs), 2026-09-24.
                    if r_dest != r_source and r_dest not in ("$0", "$zero"):
                        res.extend(
                            [
                                f"lui\t{r_dest},%hi({operand})",
                                f"addu\t{r_dest},{r_dest},{r_source}",
                                f"{op}\t{r_dest},%lo({operand})({r_dest})",
                            ]
                        )
                    else:
                        res.extend(
                            [
                                "# EXPAND_AT START",
                                ".set\tnoat",
                                f"lui\t$at,%hi({operand})",
                                f"addu\t$at,$at,{r_source}",
                                f"{op}\t{r_dest},%lo({operand})($at)",
                                ".set\tat",
                                "# EXPAND_AT END",
                            ]
                        )
                elif r_source and (int(operand) > 32767 or int(operand) < -32768):
                    # e.g. lhu	$2,49344($2)
                    res.extend(
                        [
                            "# EXPAND_AT START",
                            ".set\tnoat",
                            f"lui\t$at,%hi({operand})",
                            f"addu\t$at,{r_source},$at",
                            f"{op}\t{r_dest},%lo({operand})($at)",
                            ".set\tat",
                            "# EXPAND_AT END",
                        ]
                    )
                else:
                    # e.g. lhu	$2,528482304
                    res.append(line)

                # Naively handle scenario where *current* line is a macro
                if actual_r_dest is not None:
                    r_dest = actual_r_dest

                extra_nops = self._handle_nop_before_next_instruction(
                    next_instruction, r_dest
                )
                if self.macro_lui_fill_split and not is_macro and res and res[-1] == line:
                    # --macro-lui-fill-split with --macro-lui-fill*: a PLAIN
                    # `lX rt,N(rs)` delay may also take the next symbol-load
                    # macro's lui (retail _mk_normsp word 3).  Without the
                    # split flag only symbol-load delays are offered.
                    extra_nops = self._maybe_macro_lui_fill(extra_nops, r_dest)
                res.extend(extra_nops)

        elif op in store_mnemonics or (op == "la" and self.sdata_limit > 0):
            rest = " ".join(rest)
            r_source, r_dest, operand, is_addend, _ = parse_load_or_store(rest)

            if is_addend and r_source is None:
                # e.g. sw	$v0,D_800E52E0
                if operand.count("+") == 1:
                    symbol, offset = operand.split("+")
                    gp_rel = f"%gp_rel({symbol}+{offset})($gp)"
                    gp_allowed = self.gp_allow_offset or symbol not in self.comm_symbols
                else:
                    symbol = operand
                    gp_rel = f"%gp_rel({symbol})($gp)"
                    gp_allowed = True

                if op == "la" and not self.gp_allow_la:
                    gp_allowed = False

                if gp_allowed and (
                    symbol in self.sdata_entries or symbol in self.sbss_entries
                ):
                    res.append(f"{op}\t{r_dest},{gp_rel}")
                else:
                    plan = None
                    if op != "la" and self.pending_store_split is None:
                        plan = self._store_split_plan(op, r_dest, operand)
                    if plan is not None:
                        # ASPSX store-macro split: the `lui $at` now, the store
                        # itself after `plan` more source instructions.
                        res.extend(
                            [
                                "# EXPAND_AT START",
                                ".set\tnoat",
                                f"lui\t$at,%hi({operand}) # DEBUG: store-macro split, first half of '{line}'",
                                ".set\tat",
                                "# EXPAND_AT END",
                            ]
                        )
                        self.pending_store_split = [
                            plan,
                            self.line_index,
                            [
                                "# EXPAND_AT START",
                                ".set\tnoat",
                                f"{op}\t{r_dest},%lo({operand})($at) # DEBUG: store-macro split, deferred half of '{line}'",
                                ".set\tat",
                                "# EXPAND_AT END",
                            ],
                        ]
                    else:
                        reuse = None
                        if op != "la":
                            reuse = self._reuse_at_store(op, r_dest, operand, line)
                        if reuse is not None:
                            res.extend(reuse)
                        else:
                            res.append(line)
            elif is_addend and r_source:
                # e.g. sw	$a0,ctlbuf($v0)
                if self.addiu_at and op != "la":
                    res.extend(
                        [
                            "# EXPAND_AT START",
                            ".set\tnoat",
                            f"lui\t$at,%hi({operand})",
                            f"addiu\t$at,$at,%lo({operand})",
                            f"addu\t$at,$at,{r_source}",
                            f"{op}\t{r_dest},0x0($at)",
                            ".set\tat",
                            "# EXPAND_AT END",
                        ]
                    )
                else:
                    res.append(line)
            elif r_source and (int(operand) > 32767 or int(operand) < -32768):
                # e.g. sw	$2,56200($4)
                res.extend(
                    [
                        "# EXPAND_AT START",
                        ".set\tnoat",
                        f"lui\t$at,%hi({operand})",
                        f"addu\t$at,{r_source},$at",
                        f"{op}\t{r_dest},%lo({operand})($at)",
                        ".set\tat",
                        "# EXPAND_AT END",
                    ]
                )
            else:
                res.append(line)

        elif op in branch_mnemonics or op in jump_mnemonics:
            res.append(line)
            if self.is_reorder:
                res.append("nop  # DEBUG: branch/jump")

        elif op == "move":
            # expand move $2,$16 to addu $2,$16,$zero
            res.append(expand_move(line))

        elif op in ("addu", "subu", "sra", "srl", "srr", "sll", "or"):
            # no extra processing required
            res.append(line)
            # TODO: check if this line is a macro and insert a nop if required...

        elif op == "li":
            # TODO: handle non-soft floats?
            if self.expand_li:
                res += expand_load_immediate(line)
            else:
                res.append(line)

        elif op == "li.s":
            res += load_immediate_single(line)

        elif op == "li.d":
            res += load_immediate_double(line)

        elif op in ("mflo", "mfhi"):
            _mflo_dest = strip_comments(line).split()[1] if len(strip_comments(line).split()) > 1 else None
            _hoisted = False
            if self.mflo_nops_first and _mflo_dest and self.line_index + 1 < len(self.lines):
                # ASPSX (retail LIBGTE RotMatrixX/Y/Z): a load that immediately
                # follows mflo/mfhi and neither reads nor writes the moved
                # register is emitted ABOVE the mflo, then the two hazard nops.
                _j = self.line_index + 1
                while _j < len(self.lines) and self.lines[_j] == "#nop":
                    _j += 1
                _raw = self.lines[_j] if _j < len(self.lines) else ""
                _consumed = _j - self.line_index   # #nop markers + the load itself
                _m = re.match(r"^([a-z]+)\s+(\$?[a-z0-9]+),(.*)$", strip_comments(_raw))
                if _m and _m.group(1) in load_mnemonics:
                    _ld_dest, _ld_rest = _m.group(2), _m.group(3)
                    _nxt = [
                        self.get_next_instruction(skip=k, ignore_nop=True, ignore_set=True, ignore_label=True)
                        for k in range(1, 3)
                    ]
                    _is_md = lambda x: any(x.startswith(y) for y in ["mult\t", "multu\t", "div\t", "divu\t", "rem\t", "remu\t"])
                    _eligible = (
                        _ld_dest != _mflo_dest
                        and not re.search(r"\(\s*" + re.escape(_mflo_dest) + r"\s*\)", _ld_rest)
                        and any(_is_md(x) for x in _nxt)
                    )
                    if _eligible:
                        self.mflo_hoist_index += 1
                    if _eligible and self.mflo_hoist_index in self.mflo_hoist_skip:
                        _eligible = False
                    if _eligible:
                        # If the preceding mult/div sits behind a load-delay nop
                        # and the hoisted load is independent of it, ASPSX puts
                        # the load INTO that nop slot (retail: lh v1 | lh a0 |
                        # mult s0,v1 | mflo v1 | nop | nop).
                        _placed = False
                        _o = getattr(self, "_out", [])
                        if len(_o) >= 2 and _is_md(_o[-1]) and _o[-2].startswith("nop"):
                            _md_ops = re.findall(r"\$?[a-z0-9]+", strip_comments(_o[-1]).split(None, 1)[1])
                            _ld_regs = [_ld_dest] + re.findall(r"\(\s*(\$?[a-z0-9]+)\s*\)", _ld_rest)
                            if _ld_dest not in _md_ops and not any(r in _md_ops for r in _ld_regs[1:]) and _ld_dest not in _ld_regs[1:]:
                                _o[-2] = _raw + "  # DEBUG: load hoisted into load-delay slot before mult (nops-first)"
                                _placed = True
                        if not _placed:
                            res.append(_raw + "  # DEBUG: load hoisted above mflo/mfhi (nops-first)")
                        res.append(line)
                        _fill = None
                        if self.mflo_consumer_fill:
                            _k = _j + 1
                            while _k < len(self.lines) and not is_instruction(
                                    self.lines[_k], ignore_nop=True, ignore_set=True, ignore_label=False):
                                _k += 1
                            if _k < len(self.lines) and not is_label(self.lines[_k]):
                                _fill = self._mflo_consumer_fill(_k, _mflo_dest)
                        if _fill is not None:
                            res.append(expand_move(self.lines[_fill]) + "  # DEBUG: mflo hazard slot filled by the mult's follower")
                            self.moved_line_indices.add(_fill)
                        else:
                            res.append("nop  # DEBUG: mflo/mfhi nops-first (after hoisted load)")
                        res.append("nop  # DEBUG: mflo/mfhi nops-first (after hoisted load)")
                        self.skip_instructions = _consumed
                        _hoisted = True
            if not _hoisted:
                res.append(line)
                res += self._handle_mflo_mfhi(mflo_dest=_mflo_dest)

        elif op == "break":
            # turn 'break 7' into 'break 0x0,0x7'
            num = int(rest[0], 0)
            line = f"break\t0x{num >> 10:X},0x{num & 0x3FF:X}"
            res.append(line)

        elif op in ("divchk", "divuchk"):
            # --defer-div: the divide-by-zero (and, signed, overflow) check
            # of an earlier `div $zero,rs,rt`, at the point ASPSX flushes it.
            # Same words and labels as the --expand-div block.
            r_source, r_operand = rest[0].split(",")
            tag = f"{self.line_index}"
            res.extend(
                [
                    "# DEFER_DIV CHECK START",
                    ".set\tnoat",
                    f"bnez\t{r_operand},.L_NOT_DIV_BY_ZERO_{tag}",
                    "nop",
                    "break\t0x7",
                    f".L_NOT_DIV_BY_ZERO_{tag}:",
                ]
            )
            if op == "divchk":
                res.extend(
                    [
                        "addiu\t$at,$zero,-1",
                        f"bne\t{r_operand},$at,.L_DIV_BY_POSITIVE_SIGN_{tag}",
                        "lui\t$at,0x8000",
                        f"bne\t{r_source},$at,.L_DIV_BY_POSITIVE_SIGN_{tag}",
                        "nop",
                        "tge\t$zero,$zero,93" if self.div_uses_tge else "break\t0x6",
                        f".L_DIV_BY_POSITIVE_SIGN_{tag}:",
                    ]
                )
            res.extend([".set\tat", "# DEFER_DIV CHECK END"])

        elif op in ("div", "rem"):
            r_dest, r_source, r_operand = rest[0].split(",")
            if r_dest in ("$zero", "$0"):
                # e.g. div $zero, $v0, $a0
                return [line]

            move_from = "mfhi" if op == "rem" else "mflo"
            if self.expand_div:
                res.extend(
                    [
                        "# EXPAND_DIV START",
                        ".set\tnoat",
                        f"div\t$zero,{r_source},{r_operand}",
                        f"bnez\t{r_operand},.L_NOT_DIV_BY_ZERO_{self.line_index}",
                        "nop",
                        "break\t0x7",
                        f".L_NOT_DIV_BY_ZERO_{self.line_index}:",
                        "addiu\t$at,$zero,-1",
                        f"bne\t{r_operand},$at,.L_DIV_BY_POSITIVE_SIGN_{self.line_index}",
                        "lui\t$at,0x8000",
                        f"bne\t{r_source},$at,.L_DIV_BY_POSITIVE_SIGN_{self.line_index}",
                        "nop",
                        "tge\t$zero,$zero,93" if self.div_uses_tge else "break\t0x6",
                        f".L_DIV_BY_POSITIVE_SIGN_{self.line_index}:",
                        f"{move_from}\t{r_dest}",
                        ".set\tat",
                        "# EXPAND_DIV END",
                    ]
                )
            else:
                res.extend(
                    [
                        "# EXPAND_ZERO_DIV START",
                        f"div\t$zero,{r_source},{r_operand}",
                        f"{move_from}\t{r_dest}",
                        "# EXPAND_ZERO_DIV END",
                    ]
                )

            extra_nops = self._handle_mflo_mfhi(r_source=r_dest)
            if len(extra_nops) > 0:
                res += extra_nops
            elif not self.div_result_no_nop:
                # --div-result-no-nop: retail ASPSX puts NO load-delay nop
                # between the div macro's closing mflo/mfhi and a consumer of
                # its destination (measured: 0x8009cd2c `mflo v0; sh v0,16(sp)`
                # and 0x8009f418 `mflo a3; sll a3,a3,0x10`).  mflo is not a
                # load; only the mult/div hazard above needs padding.
                next_instruction = self.get_next_instruction(
                    skip=0, ignore_set=True, ignore_label=True
                )
                extra_nops = self._handle_nop_before_next_instruction(
                    next_instruction, r_dest
                )
                res.extend(extra_nops)

        elif op in ("divu", "remu"):
            r_dest, r_source, r_operand = rest[0].split(",")
            if r_dest in ("$zero", "$0"):
                # e.g. divu $zero, $v1, $a2
                return [line]

            move_from = "mfhi" if op == "remu" else "mflo"
            if self.expand_div:
                res.extend(
                    [
                        "# EXPAND_DIVU START",
                        ".set\tnoat",
                        f"divu\t$zero,{r_source},{r_operand}",
                        f"bnez\t{r_operand},.L_NOT_DIV_BY_ZERO_{self.line_index}",
                        "nop",
                        "break\t0x7",
                        f".L_NOT_DIV_BY_ZERO_{self.line_index}:",
                        f"{move_from}\t{r_dest}",
                        ".set\tat",
                        "# EXPAND_DIVU END",
                    ]
                )
            else:
                res.extend(
                    [
                        "# EXPAND_ZERO_DIVU START",
                        f"divu\t$zero,{r_source},{r_operand}",
                        f"{move_from}\t{r_dest}",
                        "# EXPAND_ZERO_DIVU END",
                    ]
                )

            extra_nops = self._handle_mflo_mfhi(r_source=r_dest)
            if len(extra_nops) > 0:
                res += extra_nops
            elif not self.div_result_no_nop:
                # --div-result-no-nop: retail ASPSX puts NO load-delay nop
                # between the div macro's closing mflo/mfhi and a consumer of
                # its destination (measured: 0x8009cd2c `mflo v0; sh v0,16(sp)`
                # and 0x8009f418 `mflo a3; sll a3,a3,0x10`).  mflo is not a
                # load; only the mult/div hazard above needs padding.
                next_instruction = self.get_next_instruction(
                    skip=0, ignore_set=True, ignore_label=True
                )
                extra_nops = self._handle_nop_before_next_instruction(
                    next_instruction, r_dest
                )
                res.extend(extra_nops)

        elif op == "sltu":
            r_dest, r_source, r_operand = rest[0].split(",")
            if re.match(r"^-?\d+$", r_operand) or re.match(
                r"^-?0x[A-Fa-f0-9]+$", r_operand
            ):
                value = int(r_operand)
                if self.sltu_at and value < 0:
                    res.append(f"li\t$at,{r_operand}")
                    res.append(f"{op}\t{r_dest},{r_source},$at")
                else:
                    # TODO: do we want to expand sltu into sltiu?
                    res.append(line)
            else:
                res.append(line)

        else:
            res.append(line)

        return res
