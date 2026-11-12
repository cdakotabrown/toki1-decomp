"""--konami-lib-sched: list scheduling of reorder-mode basic blocks.

Retail's gcc-compiled library code (libgs/libcd/libpress/libapi...) is NOT in
the order cc1 + ASPSX emit it: every reorder-mode basic block was reordered
after the address macros had been split into machine words (the `lui` of an
`la`/symbol load or store moves independently of its `%lo` half).  The
reordering is IDO's own assembler reorganizer (IDO 5.2 `as1`), run on the
gcc text with a handful of measured differences.  This module transcribes
it (docs/reference/MATCHING-TRICKS.md, "--konami-lib-sched"):

  * the dependence DAG is built the way as1's reorganize_bb builds it:
    nodes are visited from the LAST instruction backwards; each instruction
    class runs as1's def / use / memory handlers in as1's order (func_42cc14);
    register defs and uses carry use stages (a load reaches an ALU use after
    2 cycles with a 2-word hazard, a mult/div its mflo after 12 / 35);
    memory is a table of locations treated as pseudo registers (same base +
    class with disjoint offsets do not conflict; classes are a symbol, $sp,
    or "unknown", which conflicts with everything); a pointer bump
    `addiu rX,rX,K` lets the accesses based on rX float across it with their
    displacement rebased (func_42bc70 / func_42c5b4);
  * every edge goes into its predecessor's successor list sorted by
    aftercycles(successor) + latency, a new edge in FRONT of equal keys
    (func_42ae44); aftercycles walks that list with
    `v = max(v + size(succ), ac(succ) + latency)`, then the maxhazard and
    branch rules (a block-ending branch has 1, a node at exactly 1 gets 2);
  * pick: earliest start, highest aftercycles, longest successor latency,
    lowest hazard address, then the head of the ready list (newly ready
    successors are pushed on the head in successor-list order); a branch is
    held until one node is left, which becomes its delay slot;
  * a three-operand div/rem is expanded the way as1 expands it: the divide,
    its trap check as ONE node of 9 (signed) / 3 (unsigned) words, and the
    mflo/mfhi; every node before a check precedes it (as1's -2 edges).

Validated against as1 itself: on `as1 -R` traces of 300 IDO game TUs the DAG
reproduces 3,611 of 3,676 blocks' successor lists edge-for-edge and in as1's
order (3,618 without the retail differences below), and the scheduler
repeats 3,666 blocks' pick sequences; the rest are `.noalias` facts IDO's
ugen passes and gcc text lacks.

What differs from IDO 5.2 as1 in the retail library (each measured on the
124 swap-carrying gcc functions):
  * no load -> later `$sp` adjust edge;
  * the trap check waits for the quotient (34 cycles behind its divide,
    i.e. it lands next to the mflo);
  * a mult/div held behind the mflo/mfhi of an earlier DIVIDE's result is
    timed from that divide (issue + 1), so its own move-from and trap check
    are not late against ready work (GsSetRefView/2: `mult ; mflo a1 ;
    lw` and `div ; negu ; <check> ; mflo ; sll ; sra`; IDO as1 itself
    times the mult from its real issue, _sprite_set_light_effect1);
  * the mflo/mfhi -> next HI/LO writer hazard is left to the assembler (no
    scheduler nops); it delays a mult 3 cycles behind an mflo, a divide or
    anything behind an mfhi only one;
  * gcc's `.set noreorder` branch groups (cc1's own delay-slot fills) are
    kept verbatim as the block's last node: every node precedes them; gcc's
    text epilogue (`.set noreorder ... j $31 ; slot`) is ordinary code;
  * the "longest successor latency" tie-break does not see into such a
    group: an edge into it counts only when the node defines a register a
    group instruction uses as its memory base (func_800B4430's jal slot
    `sw $0,0(v0)`), never for a branch operand, a call argument or store
    data (GsMapModelingData `ori` before the floated `lw a3` that feeds
    `blez a3`; cd_read_retry `lui a0` before the printf-argument loads).
    Aftercycles and hazards still use the real edge.

Only ORDER changes: every output word is an input word or one word of an
input macro's expansion (`la`, `lw/sw rX,SYM[(rY)]`, 32-bit `li`, the
division macro), plus a bumped access's displacement; a `lui $at` whose value
is already live for the next store of the same initialised-data symbol and
%hi is reused, as ASPSX does (never for bss).
Hazard nops (load -> consumer) are placed at the hazard address, a branch and
its slot being indivisible.  Deterministic; off unless the flag is given.
KSCHED_TRACE=1 prints every candidate set and pick to stderr; REC / FORCE /
DECISIONS are research hooks (block records, forced orders, logged picks).
"""
import os
import re
from typing import Dict, List, Optional, Tuple

REGNUM = {f"${i}": i for i in range(32)}
REGNUM.update({
    "$zero": 0, "$at": 1, "$v0": 2, "$v1": 3, "$a0": 4, "$a1": 5, "$a2": 6,
    "$a3": 7, "$t0": 8, "$t1": 9, "$t2": 10, "$t3": 11, "$t4": 12, "$t5": 13,
    "$t6": 14, "$t7": 15, "$s0": 16, "$s1": 17, "$s2": 18, "$s3": 19,
    "$s4": 20, "$s5": 21, "$s6": 22, "$s7": 23, "$t8": 24, "$t9": 25,
    "$k0": 26, "$k1": 27, "$gp": 28, "$sp": 29, "$fp": 30, "$s8": 30,
    "$ra": 31,
})
HI, LO = "HI", "LO"

LOADS = {"lb": 1, "lbu": 1, "lh": 2, "lhu": 2, "lw": 4, "lwl": 4, "lwr": 4}
STORES = {"sb": 1, "sh": 2, "sw": 4, "swl": 4, "swr": 4}
ALU3 = {"addu", "subu", "and", "or", "xor", "nor", "slt", "sltu", "add", "sub",
        "sllv", "srlv", "srav"}
ALUI = {"addiu", "addi", "andi", "ori", "xori", "slti", "sltiu"}
SHIFTS = {"sll", "srl", "sra"}
BRANCHES = {"beq", "bne", "beqz", "bnez", "bgez", "bltz", "blez", "bgtz",
            "b", "j", "bal", "bgezal", "bltzal"}
CALLS = {"jal", "jalr"}
CALL_ARGS = {4, 5, 6, 7}
RET_VALS = {2, 3}
SP = 29

G = 2                              # branch "aftercycles"; nodes at G-1 bump to G

TRACE = bool(os.environ.get("KSCHED_TRACE"))   # stderr diagnostics only
REC = None         # research hook: list -> per-block records
FORCE = None       # research hook: per-block forced orders (decision replay)
DECISIONS = None   # research hook: candidate sets of forced picks
LAST_OUT = None    # research hook: the last block's pick order
BR_LINE = [0]      # source line of a branch (as1 trace replay only)


def _reg(tok: str) -> Optional[int]:
    return REGNUM.get(tok.strip())


def _is_imm(tok: str) -> bool:
    return bool(re.match(r"^-?(0x[0-9a-fA-F]+|\d+)$", tok.strip()))


def _imm(tok: str) -> int:
    return int(tok.strip(), 0)


def _split_ops(rest: str) -> List[str]:
    rest = rest.split("#", 1)[0].strip()
    if not rest:
        return []
    out, depth, cur = [], 0, ""
    for ch in rest:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out

class Node:
    __slots__ = ("text", "op", "defs", "uses", "mem", "kind", "line")

    def __repr__(self):
        return f"<{self.text}>"


def _symoff(s: str):
    s = s.strip()
    m = re.match(r"^([A-Za-z_.$][\w.$]*)\s*([+-]\s*(?:0x[0-9a-fA-F]+|\d+))?$", s)
    if not m:
        return s, 0
    off = int(m.group(2).replace(" ", ""), 0) if m.group(2) else 0
    return m.group(1), off


def _memref(opnd: str):
    """-> (base register or None, symbol or None, offset)"""
    m = re.match(r"^%lo\(([^)]+)\)\((\$\w+)\)$", opnd)
    if m:
        sym, off = _symoff(m.group(1))
        return _reg(m.group(2)), sym, off
    m = re.match(r"^(.*)\((\$\w+)\)$", opnd)
    if m:
        disp = m.group(1).strip()
        base = _reg(m.group(2))
        if disp == "" or _is_imm(disp):
            return base, None, _imm(disp) if disp else 0
        sym, off = _symoff(disp)
        return base, sym, off
    if _is_imm(opnd):
        return None, None, _imm(opnd)
    sym, off = _symoff(opnd)
    return None, sym, off


def _fits16(v: int) -> bool:
    return -0x8000 <= v <= 0x7fff


def _symtext(sym: str, off: int) -> str:
    return sym + (f"+{off}" if off > 0 else (f"{off}" if off < 0 else ""))


def expand(line: str):
    """One reorder-mode instruction line -> [(machine-word text, dataflow)].
    None: a form the scheduler does not model (the block is left alone)."""
    code = line.split("#", 1)[0].strip()
    if not code:
        return []
    parts = code.split(None, 1)
    op = parts[0]
    ops = _split_ops(parts[1]) if len(parts) > 1 else []

    def mk(text, defs=(), uses=(), mem=None, kind="alu"):
        return (text, dict(defs=set(d for d in defs if d), uses=set(u for u in uses if u),
                           mem=mem, kind=kind))

    try:
        if op == "nop":
            return []
        if op in ("move", "negu", "neg", "not") and len(ops) == 2:
            return [mk(line, [_reg(ops[0])], [_reg(ops[1])])]
        if op in ALU3 and len(ops) == 3:
            rd, rs = _reg(ops[0]), _reg(ops[1])
            if rd is None or rs is None:
                return None
            if _reg(ops[2]) is not None:
                return [mk(line, [rd], [rs, _reg(ops[2])])]
            if _is_imm(ops[2]):
                v = _imm(ops[2])
                if op in ("addu", "add", "slt", "sltu") and _fits16(v):
                    return [mk(line, [rd], [rs])]
                if op in ("and", "or", "xor") and 0 <= v <= 0xffff:
                    return [mk(line, [rd], [rs])]
                if op == "subu" and _fits16(-v):
                    return [mk(line, [rd], [rs])]
            return None
        if op in ALUI and len(ops) == 3:
            return [mk(line, [_reg(ops[0])], [_reg(ops[1])])]
        if op in SHIFTS and len(ops) == 3:
            if _reg(ops[2]) is not None:
                return [mk(line, [_reg(ops[0])], [_reg(ops[1]), _reg(ops[2])])]
            return [mk(line, [_reg(ops[0])], [_reg(ops[1])])]
        if op == "lui" and len(ops) == 2:
            return [mk(line, [_reg(ops[0])], [])]
        if op == "li" and len(ops) == 2 and _is_imm(ops[1]):
            rd = _reg(ops[0])
            v = _imm(ops[1]) & 0xffffffff
            sv = v - (1 << 32) if v & 0x80000000 else v
            if _fits16(sv) or v <= 0xffff:
                return [mk(line, [rd], [])]
            out = [mk(f"lui\t{ops[0]},0x{(v >> 16) & 0xffff:x}", [rd], [])]
            if v & 0xffff:
                out.append(mk(f"ori\t{ops[0]},{ops[0]},0x{v & 0xffff:x}", [rd], [rd]))
            return out
        if op == "la" and len(ops) == 2:
            rd = _reg(ops[0])
            base, sym, off = _memref(ops[1])
            if base is not None or sym is None:
                return None
            s = _symtext(sym, off)
            return [mk(f"lui\t{ops[0]},%hi({s})", [rd], []),
                    mk(f"addiu\t{ops[0]},{ops[0]},%lo({s})", [rd], [rd])]
        if op in LOADS and len(ops) == 2:
            rd = _reg(ops[0])
            base, sym, off = _memref(ops[1])
            sz = LOADS[op]
            if sym is None and base is not None and _fits16(off):
                uses = [base] + ([rd] if op in ("lwl", "lwr") else [])
                return [mk(line, [rd], uses, mem=("L", base, None, off, sz), kind="load")]
            if sym is None:
                return None
            s = _symtext(sym, off)
            if base is None:
                if rd in (0, None):
                    return None
                return [mk(f"lui\t{ops[0]},%hi({s})", [rd], []),
                        mk(f"{op}\t{ops[0]},%lo({s})({ops[0]})", [rd], [rd],
                           mem=("L", None, sym, off, sz), kind="load")]
            # reg+symbol load: ASPSX builds the address in the load's own
            # destination, in $at only when that is the index register
            if rd != base and rd not in (0, None):
                return [mk(f"lui\t{ops[0]},%hi({s})", [rd], []),
                        mk(f"addu\t{ops[0]},{ops[0]},${base}", [rd], [rd, base]),
                        mk(f"{op}\t{ops[0]},%lo({s})({ops[0]})", [rd], [rd],
                           mem=("L", None, sym, None, sz), kind="load")]
            return [mk(f"lui\t$at,%hi({s})", [1], []),
                    mk(f"addu\t$at,$at,${base}", [1], [1, base]),
                    mk(f"{op}\t{ops[0]},%lo({s})($at)", [rd], [1],
                       mem=("L", None, sym, None, sz), kind="load")]
        if op in STORES and len(ops) == 2:
            rt = _reg(ops[0])
            base, sym, off = _memref(ops[1])
            sz = STORES[op]
            if sym is None and base is not None and _fits16(off):
                return [mk(line, [], [rt, base], mem=("S", base, None, off, sz), kind="store")]
            if sym is None:
                return None
            s = _symtext(sym, off)
            if base is None:
                return [mk(f"lui\t$at,%hi({s})", [1], []),
                        mk(f"{op}\t{ops[0]},%lo({s})($at)", [], [rt, 1],
                           mem=("S", None, sym, off, sz), kind="store")]
            return [mk(f"lui\t$at,%hi({s})", [1], []),
                    mk(f"addu\t$at,$at,${base}", [1], [1, base]),
                    mk(f"{op}\t{ops[0]},%lo({s})($at)", [], [rt, 1],
                       mem=("S", None, sym, None, sz), kind="store")]
        if op in ("mult", "multu", "div", "divu") and len(ops) == 2:
            return [mk(line, [HI, LO], [_reg(ops[0]), _reg(ops[1])], kind="muldiv")]
        if op in ("div", "divu", "rem", "remu") and len(ops) == 3:
            rd, rs, rt = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
            if rt is None or rs is None or rd is None:
                return None
            signed = op in ("div", "rem")
            base = "div" if signed else "divu"
            if rd == 0:
                return [mk(line, [HI, LO], [rs, rt], kind="muldiv")]
            # as1's own expansion: the divide, its trap check (one node of 9
            # / 3 words) and the move from LO/HI
            chk = "divchk" if signed else "divuchk"
            mf = "mfhi" if op.startswith("rem") else "mflo"
            return [mk(f"{base}\t$zero,{ops[1]},{ops[2]}", [HI, LO], [rs, rt], kind="muldiv"),
                    mk(f"{chk}\t{ops[1]},{ops[2]}", [1] if signed else [],
                       [rs, rt] if signed else [rt], kind="divchk"),
                    mk(f"{mf}\t{ops[0]}", [rd], [HI if mf == "mfhi" else LO], kind="mf")]
        if op in ("mflo", "mfhi") and len(ops) == 1:
            return [mk(line, [_reg(ops[0])], [LO if op == "mflo" else HI], kind="mf")]
        if op in ("mtlo", "mthi") and len(ops) == 1:
            return [mk(line, [LO if op == "mtlo" else HI], [_reg(ops[0])])]
    except (KeyError, ValueError, AttributeError):
        return None
    return None


def _branch_info(line: str):
    """-> (kind, registers the branch reads), kind in call/ret/branch."""
    code = line.split("#", 1)[0].strip()
    parts = code.split(None, 1)
    op = parts[0]
    ops = _split_ops(parts[1]) if len(parts) > 1 else []
    uses = set()
    if op in CALLS:
        kind = "call"
        if op == "jalr":
            uses |= {r for r in (_reg(o) for o in ops) if r is not None}
    elif op in ("j", "jr"):
        r = _reg(ops[0]) if ops else None
        kind = "ret" if r == 31 else "branch"
        if r is not None:
            uses.add(r)
    elif op in BRANCHES:
        kind = "branch"
        for o in ops[:-1]:
            r = _reg(o)
            if r is None:
                return None          # branch macro with an immediate: barrier
            uses.add(r)
    else:
        return None
    uses.discard(0)
    return kind, uses


def is_instr_line(line: str) -> bool:
    s = line.strip()
    return bool(s) and not s.startswith("#") and not s.startswith(".") and not s.endswith(":")


# ------------------------------------------------------------------ as1 DAG
# A transcription of IDO 5.2 as1's reorganizer DAG (reorganize_bb ->
# func_42f018 / func_42cc14 and their def/use/memory helpers in the recomp,
# tools/ido-static-recomp/build/5.2/as1.c).  Validated against `as1 -R`
# traces of 300 game TUs: 3,618 of 3,676 blocks give as1's successor lists
# edge-for-edge AND in as1's order, 3,664 the same aftercycles on every node,
# and the scheduler below then repeats 3,666 blocks' pick sequences; the
# rest are `.noalias` / `.mtag` facts IDO's ugen passes and gcc text lacks.
_LO, _HI = "LO", "HI"           # as1 pseudo registers 0x4e / 0x4f
_ALU_LAT = 1                    # an ALU def reaches a use a cycle later
_LD_LAT, _LD_HAZ = 3, 3         # load def: latency/hazard before the use stage
_MUL_LAT, _DIV_LAT = 13, 36     # mult/div HI/LO def (use stage 1): 12 / 35
_USE_STAGE, _MEM_STAGE = 1, 2
_CHK_AFTER_DIV = 34             # Konami: the trap check waits for the quotient
_SIZE = {"divchk": 9, "divuchk": 3}


class _E:
    __slots__ = ("node", "lat", "haz")

    def __init__(self, node, lat, haz):
        self.node, self.lat, self.haz = node, lat, haz


def _mem_from_text(text: str):
    """machine-word load/store text -> (base reg, symbol or None, offset)"""
    code = text.split("#", 1)[0].strip()
    ops = _split_ops(code.split(None, 1)[1])
    m = re.match(r"^(.*)\((\$\w+)\)$", ops[1])
    base = _reg(m.group(2))
    disp = m.group(1).strip()
    m2 = re.match(r"^%lo\(([^)]+)\)$", disp)
    if m2:
        sym, off = _symoff(m2.group(1))
        return base, sym, off
    return base, None, (_imm(disp) if disp else 0)


def _ops(nd: Node) -> List[str]:
    parts = nd.text.split("#", 1)[0].strip().split(None, 1)
    return _split_ops(parts[1]) if len(parts) > 1 else []


def _as1_actions(nd: Node):
    """as1's handler sequence for one node (func_42cc14), and its maxhazard."""
    op, ops, R = nd.op, _ops(nd), _reg
    reloc = "%hi(" in nd.text or "%lo(" in nd.text
    acts = []
    mh = 0
    if nd.kind == "load":
        base, sym, off = _mem_from_text(nd.text)
        mh = 2
        acts.append(("DEF", R(ops[0]), _LD_LAT, _LD_HAZ))
        if op in ("lwl", "lwr"):
            acts.append(("USE", R(ops[0])))
        acts.append(("MEML", base, off, nd.mem[4], sym))
        acts.append(("BASE", base, off, reloc))
    elif nd.kind == "store":
        base, sym, off = _mem_from_text(nd.text)
        acts.append(("MEMS", base, off, nd.mem[4], sym))
        acts.append(("USE", R(ops[0])))
        acts.append(("BASE", base, off, reloc))
    elif nd.kind == "muldiv":
        if len(ops) == 3:                       # div $zero,rs,rt (as1 macro)
            lat, rs, rt, isdiv = _DIV_LAT, ops[1], ops[2], True
        else:
            isdiv = op.startswith("div")
            lat, rs, rt = (_DIV_LAT if isdiv else _MUL_LAT), ops[0], ops[1]
        acts += [("DEF", _HI, lat, 0), ("DEF", _LO, lat, 0),
                 ("USE", R(rt)), ("USE", R(rs)), ("POSTDIV",) if isdiv else ("POSTMUL",)]
    elif nd.kind == "divchk":
        if op == "divchk":
            acts.append(("DEF", 1, _ALU_LAT, 0))
            acts.append(("USE", R(ops[0])))
        acts.append(("USE", R(ops[1])))
    elif nd.kind == "mf":
        mh = 3
        acts += [("DEF", R(ops[0]), _ALU_LAT, 0),
                 ("USEH", _LO if op == "mflo" else _HI), ("POSTMF",)]
    elif op in ("mtlo", "mthi"):
        acts += [("DEF", _LO, _ALU_LAT, 0), ("DEF", _HI, _ALU_LAT, 0), ("USE", R(ops[0]))]
    elif op in ("lui", "li"):
        acts.append(("DEF", R(ops[0]), _ALU_LAT, 0))
    elif op in ("move", "negu", "neg", "not"):
        acts += [("DEF", R(ops[0]), _ALU_LAT, 0), ("USE", R(ops[1]))]
    elif op in ("sllv", "srlv", "srav") and len(ops) == 3:
        acts += [("VSH",), ("DEF", R(ops[0]), _ALU_LAT, 0), ("USE", R(ops[1])), ("USE", R(ops[2]))]
    elif op in SHIFTS and len(ops) == 3:
        if R(ops[2]) is not None:
            acts.append(("VSH",))
        acts += [("DEF", R(ops[0]), _ALU_LAT, 0), ("USE", R(ops[1]))]
        if R(ops[2]) is not None:
            acts.append(("USE", R(ops[2])))
    elif (op in ALUI or op in ALU3) and len(ops) == 3:
        rd, rs, third = R(ops[0]), R(ops[1]), R(ops[2])
        if third is not None:
            acts += [("DEF", rd, _ALU_LAT, 0), ("USE", third), ("USE", rs)]
        else:
            k = None
            if not reloc and _is_imm(ops[2]):
                v = _imm(ops[2])
                if op in ("addiu", "addi", "addu", "add"):
                    k = v
                elif op in ("subu", "sub"):
                    k = -v
            if k is not None and rd == rs and rd != SP and _fits16(k):
                acts.append(("BUMP", rd, k))    # a pointer bump (func_42bc70)
            else:
                acts.append(("DEF", rd, _ALU_LAT, 0))
            acts.append(("USE", rs))
    else:
        for r in sorted(x for x in nd.defs if isinstance(x, int)):
            acts.append(("DEF", r, _ALU_LAT, 0))
        for r in sorted(x for x in nd.uses if isinstance(x, int)):
            acts.append(("USE", r))
    return acts, mh


def build_dag(nodes: List[Node], br, fixed: bool, group_defs=frozenset()):
    """-> (succ lists [[_E]] in as1's order, aftercycles, node sizes,
    pointer-bump links {mem node: [(bump node, K)]}, rebased offsets).
    The branch / fixed group, if any, is node n = len(nodes)."""
    n = len(nodes)
    has_br = br is not None
    N = n + (1 if has_br else 0)
    succ: List[List[_E]] = [[] for _ in range(N)]
    acv = [0] * N
    size = [_SIZE.get(nodes[k].op, 1) if k < n else 1 for k in range(N)]
    nextdef: Dict = {}
    uses: Dict = {}            # reg -> [[node, stage], ...], earliest first
    after_bump: Dict = {}      # 0x10022cc8: the def that followed a bump
    memtab: List[list] = []    # [base, size, offset, class, pseudo register]
    vsh = [None]               # 0x100232c4: the next variable shift
    bumps: Dict[int, list] = {}
    reloff: Dict[int, int] = {}

    def add(i, j, lat, haz=0):
        """func_42ae44: keep i's successors sorted by ac(succ) + latency, a
        new edge in front of equal keys; a stronger edge is re-inserted."""
        if i == j:
            return
        lst = succ[i]
        found = None
        a2 = -1
        k = 0
        while k < len(lst):
            e = lst[k]
            if e.node == j:
                found = e
                e.haz = max(e.haz, haz)
                if e.lat < lat:
                    lst.pop(k)
                    continue
                return
            if e.lat < 0 or acv[e.node] + e.lat < acv[j] + lat:
                a2 = k
            k += 1
        if found is None:
            found = _E(j, lat, haz)
        found.lat = lat
        lst.insert(a2 + 1, found)

    def use_entry(r, cur, stage):
        lst = uses.setdefault(r, [])
        if lst and lst[0][0] == cur:
            lst[0][1] = max(lst[0][1], stage)
            return
        lst.insert(0, [cur, stage])

    def DEF(cur, r, L, H):                      # func_42ba20 / func_42bf70
        if r is None or r == 0:
            return
        nd_ = nextdef.get(r)
        if nd_ is not None:
            add(cur, nd_, 4 if size[cur] == 9 else 1, 0)
        nextdef[r] = cur
        for u, stage in uses.get(r, []):
            add(cur, u, L - stage if stage < L else 1,
                (H - stage if stage < H else 1) if H else 0)
        uses[r] = []

    def USE(cur, r, warlat=1):                  # func_42c2c0
        if r is None or r == 0:
            return
        nd_ = nextdef.get(r)
        if nd_ is not None and nd_ != cur:
            add(cur, nd_, warlat, 0)
        use_entry(r, cur, _USE_STAGE)

    def cls_of(base, sym):
        if sym is not None:
            return ("S", sym)
        return -1 if base == SP else 0

    def may_alias(c1, c2):
        return c1 == 0 or c2 == 0 or c1 == c2

    def MEML(cur, base, off, sz, sym):          # func_42b5a4
        c = cls_of(base, sym)
        key = None
        for eb, esz, eoff, ec, ek in memtab:
            w = nextdef.get(ek)
            if eb == base and ec == c:
                if off < eoff + esz and eoff < off + sz:
                    if w is not None and w != cur:
                        add(cur, w, 1, 0)
                    if off == eoff and sz == esz:
                        key = ek
            elif w is not None and w != cur and may_alias(c, ec):
                add(cur, w, 1, 0)
        if key is None:
            key = ("M", len(memtab))
            memtab.append([base, sz, off, c, key])
        use_entry(key, cur, _MEM_STAGE)

    def MEMS(cur, base, off, sz, sym):          # func_42b094
        c = cls_of(base, sym)
        key = None
        for eb, esz, eoff, ec, ek in memtab:
            same = eb == base and ec == c
            if same and not (off < eoff + esz and eoff < off + sz):
                continue
            if not same and not may_alias(c, ec):
                continue
            w = nextdef.get(ek)
            if w is not None:
                add(cur, w, 1, 0)
            for u, _ in uses.get(ek, []):
                add(cur, u, 1, 0)
            if same and off == eoff and sz == esz:
                key = ek
                uses[ek] = []
        if key is None:
            key = ("M", len(memtab))
            memtab.append([base, sz, off, c, key])
        nextdef[key] = cur

    def BUMP(cur, r, k):                        # func_42bc70
        after_bump[r] = nextdef.get(r)
        if nextdef.get(r) is not None:
            add(cur, nextdef[r], 1, 0)
        nextdef[r] = cur
        lst = uses.get(r, [])
        uses[r] = []
        if lst:
            for ent in memtab:
                if ent[0] == r:
                    ent[2] += k
        keep = []
        for u, stage in lst:
            un = nodes[u] if u < n else None
            ok = False
            if un is not None and un.mem is not None and "%" not in un.text:
                b, s, o = _mem_from_text(un.text)
                if b == r and s is None and (un.kind == "load" or _reg(_ops(un)[0]) != r):
                    ok = _fits16(reloff.get(u, o) + k)
            if ok:
                reloff[u] = reloff.get(u, _mem_from_text(un.text)[2]) + k
                bumps.setdefault(u, []).append((cur, k))
                succ[cur].insert(0, _E(u, -1, 0))   # the access may float above
                keep.append([u, stage])
            else:
                add(cur, u, 1, 0)
        uses[r] = keep

    def BASE(cur, r, off, reloc):               # func_42c5b4
        if r is None or r == 0:
            return
        nd_ = nextdef.get(r)
        if nd_ is not None and nd_ != cur:
            bk = None
            if nd_ < n and r != SP and not reloc and "%" not in nodes[nd_].text:
                bops = _ops(nodes[nd_])
                if len(bops) == 3 and _reg(bops[0]) == r and _reg(bops[1]) == r \
                        and _is_imm(bops[2]):
                    if nodes[nd_].op in ("addiu", "addi", "addu", "add"):
                        bk = _imm(bops[2])
                    elif nodes[nd_].op in ("subu", "sub"):
                        bk = -_imm(bops[2])
            if bk is not None and _fits16(bk) and _fits16(off - bk):
                succ[nd_].insert(0, _E(cur, -1, 0))  # the bump may rise above
                bumps.setdefault(cur, []).append((nd_, bk))
                if after_bump.get(r) is not None:
                    add(cur, after_bump[r], 1, 0)
            else:
                add(cur, nd_, 1, 0)
        use_entry(r, cur, _USE_STAGE)

    # the branch first: it is the last instruction
    brn = n if has_br else None
    if has_br:
        kind, buses = br
        if fixed:
            for r in sorted(x for x in group_defs if isinstance(x, int)):
                DEF(brn, r, _ALU_LAT, 0)
        elif kind == "call":
            DEF(brn, 31, _ALU_LAT, 0)
        for r in sorted(buses):
            USE(brn, r)
        acv[brn] = G - 1

    lastchk = None
    for cur in range(n - 1, -1, -1):
        nd = nodes[cur]
        acts, mh = _as1_actions(nd)
        isgrp = nd.kind == "divchk"
        if isgrp:
            if lastchk is not None:
                add(cur, lastchk, -2, 0)
            lastchk = cur
        for a in acts:
            t = a[0]
            if t == "DEF":
                DEF(cur, a[1], a[2], a[3])
            elif t == "USE":
                USE(cur, a[1])
            elif t == "USEH":
                # Konami: the move-from's hazard on the next HI/LO writer is
                # left to the assembler; it delays a mult 3 cycles behind an
                # mflo, a divide or anything behind an mfhi only one
                w = nextdef.get(a[1])
                lat = 3
                if nd.op == "mfhi" or (w is not None and w < n and nodes[w].op.startswith("div")):
                    lat = 1
                USE(cur, a[1], lat)
            elif t == "MEML":
                MEML(cur, *a[1:])
            elif t == "MEMS":
                MEMS(cur, *a[1:])
            elif t == "BASE":
                BASE(cur, *a[1:])
            elif t == "BUMP":
                BUMP(cur, a[1], a[2])
            elif t == "VSH":
                vsh[0] = cur
            elif t in ("POSTMF", "POSTMUL", "POSTDIV"):
                if t == "POSTDIV" and brn is not None:
                    add(cur, brn, 1, 0)
                if vsh[0] is not None:
                    add(cur, vsh[0], 1, {"POSTMF": 0, "POSTMUL": 12, "POSTDIV": 2}[t])
                    vsh[0] = None
        if not isgrp and lastchk is not None:
            divide = nd.kind == "muldiv" and len(_ops(nd)) == 3
            add(cur, lastchk, _CHK_AFTER_DIV if divide else -2, 0)
        if brn is not None and (fixed or mh >= 2 or isgrp):
            add(cur, brn, 1, 0)
        s0 = 0
        for e in succ[cur]:
            if e.lat > 0:
                s0 = max(s0 + size[e.node], acv[e.node] + e.lat)
        if mh >= 2:
            s0 = max(s0, mh - 1)
        if brn is not None and s0 + 1 == G:
            s0 = G
        acv[cur] = s0
    return succ, acv, size, bumps, reloff


def _is_divide(nodes: List[Node], k: int) -> bool:
    return k < len(nodes) and nodes[k].kind == "muldiv" and nodes[k].op.startswith("div")


def schedule_block(nodes: List[Node], br, fixed: bool = False, group_defs=frozenset(),
                   group_bases=frozenset()):
    """-> (emission order before the branch, -1 = hazard nop; slot node or
    None; {node: rewritten text} for pointer-bumped displacements)"""
    n = len(nodes)
    succ, acv, size, bumps, reloff = build_dag(nodes, br, fixed, group_defs)
    has_br = br is not None
    N = n + (1 if has_br else 0)
    preds: List[List[_E]] = [[] for _ in range(N)]
    for i in range(N):
        for e in succ[i]:
            preds[e.node].append(_E(i, e.lat, e.haz))
    # Konami: the "longest successor latency" tie-break does not see into a
    # cc1 noreorder group -- an edge into the group counts only when the
    # node defines a register a group instruction uses as its memory BASE
    # (GsMapModelingData, cd_read_retry, GsSortClear; not branch operands,
    # call arguments or store data)
    def _lat(i):
        v = 0
        for e in succ[i]:
            if e.lat <= 0:
                continue
            if fixed and has_br and e.node == n and i < n \
                    and not (nodes[i].defs & group_bases):
                continue
            v = max(v, e.lat)
        return v
    lat = [_lat(i) for i in range(N)]
    lines = [getattr(nodes[i], "line", 0) if i < n else BR_LINE[0] for i in range(N)]
    # a pointer-bump link (-1) never holds a node back; everything else does
    remaining = [sum(1 for p in preds[j] if p.lat != -1) for j in range(N)]
    ready = [i for i in range(N) if remaining[i] == 0]
    issued: List[Optional[int]] = [None] * N
    off48 = [0] * N                        # earliest hazard-free address
    out: List[int] = []
    nops_before: Dict[int, int] = {}
    addr = 0
    cyc = 0
    forced = FORCE.pop(0) if FORCE else None

    def start(i):
        return max([issued[p.node] + abs(p.lat) for p in preds[i]
                    if issued[p.node] is not None] + [cyc])

    while len(out) < N:
        left = N - len(out)
        best = None
        for k, i in enumerate(ready):
            if has_br and i == n and left > (1 if fixed else 2):
                continue
            t = start(i)
            key = (t, -acv[i], -lat[i], off48[i], lines[i])
            if best is None or key < best[0]:
                best = (key, k, i, t)
        if best is None:
            best = (None, 0, ready[0], start(ready[0]))
        if forced is not None:
            want = forced[len(out)] if len(out) < len(forced) else None
            if want is None or want not in ready:
                forced = None
            else:
                if DECISIONS is not None and len(ready) > 1:
                    DECISIONS.append(dict(model=best[2], want=want, cyc=cyc, addr=addr, cands=[
                        dict(i=i2, pos=k2, t=start(i2), ac=acv[i2], lat=lat[i2], off=off48[i2],
                             text=nodes[i2].text if i2 < n else "<branch>",
                             kind=nodes[i2].kind if i2 < n else "branch")
                        for k2, i2 in enumerate(ready)]))
                best = (None, ready.index(want), want, start(want))
        _, k, i, t = best
        if TRACE:
            import sys
            for i2 in ready:
                nm = nodes[i2].text if i2 < n else "<branch>"
                sys.stderr.write(f"   cand t={start(i2)} ac={acv[i2]} lat={lat[i2]} haz={off48[i2]} {nm}\n")
            sys.stderr.write(f"PICK at {t}: {nodes[i].text if i < n else '<branch>'}\n")
        issued[i] = t
        if i < n and nodes[i].kind == "muldiv":
            # Konami: a mult/div held behind the move-from of an earlier
            # DIVIDE's result is timed from that divide, not from its own
            # issue: its mflo/mfhi (and a divide's trap check) count their
            # latency from the earlier divide's issue + 1.
            dv = [pp.node for q in preds[i]
                  if q.node < n and nodes[q.node].kind == "mf"
                  for pp in preds[q.node] if _is_divide(nodes, pp.node)]
            if dv:
                issued[i] = max(issued[d] for d in dv) + 1
        cyc = t + 1
        if off48[i] > addr:
            nops_before[i] = (off48[i] - addr) // 4
            addr = off48[i]
        out.append(i)
        ready.pop(k)
        for e in succ[i]:
            j = e.node
            if e.haz:
                off48[j] = max(off48[j], addr + 4 * e.haz)
            if e.lat == -1:
                continue
            remaining[j] -= 1
            if remaining[j] == 0:
                ready.insert(0, j)
        addr += 4 * size[i]
    # displacements of accesses linked to a pointer bump: as1 rebased them
    # to the value before the bump; one emitted after the bump gets it back
    pos = {x: k for k, x in enumerate(out)}
    texts = {}
    for m, lst in bumps.items():
        off = reloff.get(m, _mem_from_text(nodes[m].text)[2])
        for a, k in lst:
            if pos[a] < pos[m]:
                off -= k
        if off != _mem_from_text(nodes[m].text)[2]:
            o = _ops(nodes[m])
            texts[m] = f"{nodes[m].op}\t{o[0]},{off}{o[1][o[1].index('('):]}"

    def with_nops(seq):
        res = []
        for x in seq:
            res.extend([-1] * nops_before.get(x, 0))
            res.append(x)
        return res

    global LAST_OUT
    LAST_OUT = list(out)
    if not has_br:
        return with_nops(out), None, texts
    bi = out.index(n)
    body, after = out[:bi], out[bi + 1:]
    slot = after[0] if after else None
    if slot is not None and nops_before.get(slot):
        # a branch and its delay slot are indivisible for hazards
        nops_before[n] = nops_before.get(n, 0) + nops_before.pop(slot)
    return with_nops(body) + [-1] * nops_before.get(n, 0), slot, texts


def _hi_of(sym: str, off: int) -> Optional[int]:
    m = re.match(r"^(?:[DF]|func)_([0-9A-Fa-f]{8})$", sym)
    if not m:
        return None
    addr = (int(m.group(1), 16) + (off or 0)) & 0xffffffff
    return ((addr + 0x8000) >> 16) & 0xffff


def _in_image(sym: str) -> bool:
    """SYM lies in the EXE's initialised image (text + data, 0x80041000 to
    0x800E9000), the only place ASPSX reuses a live `lui $at`: above it is
    bss, where retail re-emits the lui even for one symbol (GsSetRefView,
    GsGetObjMatrix; maspsx REUSE_AT_DATA_HI is the same rule)."""
    m = re.match(r"^(?:[DF]|func)_([0-9A-Fa-f]{8})$", sym)
    return bool(m) and 0x80041000 <= int(m.group(1), 16) < 0x800E9000


def _reuse_at(nodes: List[Node]) -> List[Node]:
    """ASPSX keeps a live `lui $at,%hi(SYM)` for the next store with the same
    %hi (input order) when SYM is initialised data: drop the repeated lui."""
    out: List[Node] = []
    live = None
    for nd in nodes:
        m = re.match(r"^lui\t\$at,%hi\(([^)]+)\)$", nd.text)
        if m:
            sym, off = _symoff(m.group(1))
            key = (sym, _hi_of(sym, off)) if _in_image(sym) else None
            if key is not None and live == key:
                continue
            live = key
            out.append(nd)
            continue
        if 1 in nd.defs:
            live = None
        out.append(nd)
    return out


# ------------------------------------------------------------------ driver
def _group_uses(group: List[str]):
    """(kind, registers read) of a fixed `.set noreorder` branch group."""
    uses, defined = set(), set()
    kind = "fixed"
    for ln in group:
        s = ln.strip()
        if not is_instr_line(s):
            continue
        bi = _branch_info(s)
        if bi is not None:
            uses |= bi[1] - defined
            if bi[0] in ("call", "ret"):
                kind = bi[0]
            continue
        ex = expand(s)
        if ex is None:
            return None
        for _, d in ex:
            uses |= d["uses"] - defined
            defined |= d["defs"]
    if kind == "call":
        uses |= CALL_ARGS - defined
    if kind == "ret":
        uses |= RET_VALS - defined
    return kind, uses


_EPILOGUE_J = re.compile(r"^j\s+\$(31|ra)$")
_DROP = {"#nop", "#.set\tvolatile", "#.set\tnovolatile"}


def _is_set(s: str, what: str) -> bool:
    return s.startswith(".set") and s.split()[-1] == what


def schedule_lines(lines: List[str]) -> List[str]:
    out: List[str] = []
    reorder = True
    block: List[str] = []

    def flush(branch_line: Optional[str] = None, group: Optional[List[str]] = None):
        nonlocal block
        nodes: List[Node] = []
        ok = True
        for ln in block:
            ex = expand(ln)
            if ex is None:
                ok = False
                break
            for text, d in ex:
                nd = Node()
                nd.text, nd.op = text, text.split()[0]
                nd.defs, nd.uses, nd.mem, nd.kind = d["defs"], d["uses"], d["mem"], d["kind"]
                nodes.append(nd)
        br, fixed = None, False
        if branch_line is not None:
            br = _branch_info(branch_line)
        elif group is not None:
            br, fixed = _group_uses(group), True
        if not ok or (branch_line is not None and br is None):
            out.extend(block)                  # unmodelled form: leave as is
            if branch_line is not None:
                out.append(branch_line)
            block = []
            return
        nodes = _reuse_at(nodes)
        gdefs, gbases = set(), set()
        if fixed:
            for g in group:
                gs = g.strip()
                if not is_instr_line(gs):
                    continue
                bi = _branch_info(gs)
                if bi is None:
                    for _, dd in (expand(gs) or []):
                        gdefs |= dd["defs"]
                    gp = gs.split(None, 1)
                    if gp[0] in LOADS or gp[0] in STORES:
                        gops = _split_ops(gp[1])
                        if len(gops) == 2:
                            gb = _memref(gops[1])[0]
                            if gb is not None:
                                gbases.add(gb)
                elif gs.split()[0] in CALLS:
                    gdefs.add(31)
        order, slot, texts = schedule_block(nodes, br, fixed, frozenset(gdefs),
                                            frozenset(gbases))
        for k, t in texts.items():
            nodes[k].text = t
        if REC is not None:
            REC.append(dict(nodes=nodes, br=br, fixed=fixed, order=order, slot=slot,
                            out_start=len(out), branch_line=branch_line, group=group))
        out.extend("nop" if i < 0 else nodes[i].text for i in order)
        if branch_line is not None:
            if slot is not None:
                out.extend([".set\tnoreorder", ".set\tnomacro", branch_line,
                            nodes[slot].text, ".set\tmacro", ".set\treorder"])
            else:
                out.append(branch_line)
        block = []

    i, n = 0, len(lines)
    while i < n:
        ln = lines[i]
        s = ln.strip()
        if _is_set(s, "noreorder"):
            j = i + 1
            grp = []
            while j < n and not _is_set(lines[j].strip(), "reorder"):
                grp.append(lines[j].strip())
                j += 1
            body = [g for g in grp if is_instr_line(g)]
            if any(_EPILOGUE_J.match(g) for g in body):
                # gcc's text epilogue is scheduled like reorder-mode code,
                # its slot word back in the block before the `j $31`
                k = next(x for x, g in enumerate(body) if _EPILOGUE_J.match(g))
                lines = lines[:i] + body[:k] + body[k + 1:] + [body[k]] + lines[j + 1:]
                n = len(lines)
                continue
            flush(group=grp)
            reorder = False
            out.append(ln)
            i += 1
            continue
        if _is_set(s, "reorder"):
            reorder = True
            out.append(ln)
            i += 1
            continue
        i += 1
        if not reorder:
            out.append(ln)
            continue
        if not s or s in _DROP:
            continue
        if s.startswith("#") or s.startswith(".") or s.endswith(":"):
            flush()
            out.append(ln)
            continue
        op = s.split()[0]
        if op in BRANCHES or op in CALLS or op == "jr":
            flush(ln)
            continue
        if op in ("break", "syscall") or op[:3] in ("mtc", "mfc", "ctc", "cfc") \
                or op in ("lwc2", "swc2") or op.startswith("c2"):
            flush()
            out.append(ln)
            continue
        block.append(ln)
    flush()
    return out
