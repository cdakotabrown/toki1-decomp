#!/usr/bin/env python3
"""
fill_gcc_delay_slots.py - Fill delay slots in GCC-compiled MIPS object files.

Post-processes a MIPS ELF object file to fill delay slots where GCC 2.6.0/2.7.2
left them unfilled. This is needed for matching Sony's LIBSPU library functions.

Patterns handled:
  1. lui $at, X; sw $reg, Y($at); j target; nop
     -> lui $at, X; j target; sw $reg, Y($at)

  2. lui $reg, X; addiu $reg, $reg, Y; jal target; nop
     -> lui $reg, X; jal target; addiu $reg, $reg, Y

  3. lui $reg, X; addiu $reg, $reg, Y; jalr $func; nop
     -> lui $reg, X; jalr $func; addiu $reg, $reg, Y
     (only when $reg != $func, so delay slot is safe)

  4. addiu sp, sp, N; jr ra; nop
     -> jr ra; addiu sp, sp, N

  6. lui $at, X; sw $reg, Y($at); jal target; nop        (--fill-jal-store)
     -> lui $at, X; jal target; sw $reg, Y($at)
     The `jal` form of pattern 1. Opt-in because sinking a store past a
     `jal` is only safe when the stored register is not $ra (jal writes
     $ra at the jump, and the delay-slot instruction sees the NEW value).

  10. addiu rX,rX,K; mem(rX)...; load; nop                (--fill-ptr-bump-down)
     -> mem(rX)+K...; load; addiu rX,rX,K
     ASPSX sinks an EARLIER pointer bump into a load-delay nop, rewriting
     the displacements of the rX-based words it hops (retail CdSearchFile
     `lb v0,1(s0) ; lbu v1,1(s0) ; addiu s0,s0,1`).  Runs before every other
     pass; see _ptr_bump_down for the legality rules.  Opt-in only.

The tool also adjusts:
  - All PC-relative branch offsets
  - ELF relocation offsets
  - j/jal addends for .text-relative R_MIPS_26 relocations

Usage:
    python3 fill_gcc_delay_slots.py input.o output.o
"""
import struct
import sys
import os
import shutil


def read_u32(data, offset):
    return struct.unpack('<I', data[offset:offset+4])[0]


def get_opcode(instr):
    return (instr >> 26) & 0x3F


def get_rs(instr):
    return (instr >> 21) & 0x1F


def get_rt(instr):
    return (instr >> 16) & 0x1F


def is_branch(instr):
    """BEQ, BNE, BLEZ, BGTZ, REGIMM (BLTZ, BGEZ, etc.)"""
    op = get_opcode(instr)
    return op in (4, 5, 6, 7, 1)


def is_j(instr):
    return get_opcode(instr) == 2


def is_jal(instr):
    return get_opcode(instr) == 3


def is_j_type(instr):
    """j or jal (not jalr/jr which are R-type)."""
    op = get_opcode(instr)
    return op == 2 or op == 3


def is_nop(instr):
    return instr == 0


def is_lui(instr):
    return get_opcode(instr) == 0x0F


def is_sw(instr):
    return get_opcode(instr) == 0x2B


def is_addiu(instr):
    return get_opcode(instr) == 0x09


def get_rd(instr):
    return (instr >> 11) & 0x1F


def get_func(instr):
    return instr & 0x3F


def is_jalr(instr):
    """JALR: opcode=0, func=9."""
    return get_opcode(instr) == 0 and get_func(instr) == 9


def is_jr(instr):
    """JR: opcode=0, func=8."""
    return get_opcode(instr) == 0 and get_func(instr) == 8


def get_branch_offset(instr):
    """Get signed branch offset (in instructions)."""
    imm = instr & 0xFFFF
    if imm & 0x8000:
        imm -= 0x10000
    return imm


def set_branch_offset(instr, offset):
    """Set signed branch offset (in instructions)."""
    if offset < 0:
        offset = offset & 0xFFFF
    return (instr & 0xFFFF0000) | (offset & 0xFFFF)


def get_j_target(instr):
    """Get j/jal target (26-bit word address)."""
    return instr & 0x03FFFFFF


def set_j_target(instr, target):
    """Set j/jal target (26-bit word address)."""
    return (instr & 0xFC000000) | (target & 0x03FFFFFF)


def is_store(instr):
    return get_opcode(instr) in (0x28, 0x29, 0x2b)   # sb, sh, sw


def is_cond_branch(instr):
    op = get_opcode(instr)
    return op in (0x04, 0x05, 0x06, 0x07) or (op == 0x01 and get_rt(instr) in (0, 1))


def _dest_reg(instr):
    """Register written by a simple ALU/load/lui word, or None."""
    op = get_opcode(instr)
    if op == 0:                      # SPECIAL: rd (mult/div/jr/jalr write none we care about)
        f = get_func(instr)
        if f in (0x08, 0x09, 0x18, 0x19, 0x1a, 0x1b):
            return None
        return get_rd(instr)
    if op in (0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x20, 0x21, 0x23, 0x24, 0x25):
        return get_rt(instr)
    return None


def _branch_srcs(instr):
    op = get_opcode(instr)
    if op in (0x04, 0x05):                 # beq/bne
        return {get_rs(instr), get_rt(instr)}
    if op in (0x01, 0x06, 0x07):           # bltz/bgez, blez, bgtz
        return {get_rs(instr)}
    return set()


# ---------------------------------------------------------------------------
# Pattern 9 support: a small register read/write model for the "earliest
# legal instruction sinks into the empty delay slot" rule (--fill-earliest).
# Unknown / control / hazard-bearing instructions decode to None and are
# neither movable nor crossable.
# ---------------------------------------------------------------------------
HI, LO = 32, 33

# old word index -> new word index for LABEL/TARGET semantics, where that
# differs from the instruction's own new position (pattern 9 moves the first
# word of a block into the slot; branches into the block must land on the new
# first word, while the moved word's relocation follows the word).
LABEL_OVERRIDES = {}


def label_pos(offset_map, old):
    if old in LABEL_OVERRIDES:
        return LABEL_OVERRIDES[old]
    return offset_map.get(old)


def insn_rw(w):
    """Return (writes:set, reads:set, kind) or None.  kind in alu/load/store."""
    if is_nop(w):
        return None
    op = get_opcode(w)
    rs, rt, rd = get_rs(w), get_rt(w), get_rd(w)
    if op == 0:
        f = get_func(w)
        if f in (0x00, 0x02, 0x03):
            return ({rd} - {0}, {rt}, 'alu')
        if f in (0x04, 0x06, 0x07):
            return ({rd} - {0}, {rs, rt}, 'alu')
        if f == 0x10:
            return ({rd} - {0}, {HI}, 'alu')
        if f == 0x12:
            return ({rd} - {0}, {LO}, 'alu')
        if f in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x2a, 0x2b):
            return ({rd} - {0}, {rs, rt}, 'alu')
        # mult/multu/div/divu write HI and LO and read rs,rt; mthi/mtlo write
        # HI/LO from rs.  These used to fall through to the blanket `return
        # None` below, which ABORTS _fill_load_delay_nops' scan at the first
        # multiply -- silently capping --fill-load-delay, the highest-yield
        # flag in the set, on any body with an early `mult`.  Classifying them
        # only makes the dependency analysis correct; the filler's own
        # candidate rules still decide what may move.  (2026-09-17)
        if f in (0x18, 0x19, 0x1a, 0x1b):
            return ({HI, LO}, {rs, rt}, 'alu')
        if f == 0x11:
            return ({HI}, {rs}, 'alu')
        if f == 0x13:
            return ({LO}, {rs}, 'alu')
        return None                      # jr/jalr/syscall/break
    if op in (0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e):
        return ({rt} - {0}, {rs}, 'alu')
    if op == 0x0f:
        return ({rt} - {0}, set(), 'alu')
    if op in (0x20, 0x21, 0x23, 0x24, 0x25):
        return ({rt} - {0}, {rs}, 'load')
    if op in (0x22, 0x26):
        # lwl/lwr merge into rt, so rt is both read and written; the
        # one-cycle load-use hazard on rt applies exactly as for lw.
        return ({rt} - {0}, {rs, rt}, 'load')
    if op in (0x28, 0x29, 0x2b):
        return (set(), {rs, rt}, 'store')
    if op in (0x2a, 0x2e):
        return (set(), {rs, rt}, 'store')      # swl/swr
    return None


def _mem_width(w):
    op = get_opcode(w)
    return {0x20: 1, 0x24: 1, 0x28: 1, 0x21: 2, 0x25: 2, 0x29: 2, 0x23: 4, 0x2b: 4}.get(op, 0)


def _disjoint_sp_slots(a, b):
    """Two memory words that both address $sp-relative slots which cannot
    overlap (the assembler can prove distinct stack slots, so a store may be
    scheduled across them -- retail v_wait sinks the prologue `sw ra,24(sp)`
    into a beqz slot past `sw a1,16(sp)`)."""
    if get_rs(a) != 29 or get_rs(b) != 29:
        return False
    wa, wb = _mem_width(a), _mem_width(b)
    if not wa or not wb:
        return False
    oa = a & 0xffff; ob = b & 0xffff
    oa = oa - 0x10000 if oa >= 0x8000 else oa
    ob = ob - 0x10000 if ob >= 0x8000 else ob
    return oa + wa <= ob or ob + wb <= oa


def _lui_absolute(words, q):
    """True when the memory word at q addresses a symbol through a register
    that the immediately preceding word loaded with `lui` -- an absolute
    (non-stack) address.  ASPSX reorders stack-slot stores across these
    (retail v_wait: `sw ra,24(sp)` past `lui v0 ; lw v0,%lo(v0)`;
    PadInit: `sw s0,16(sp)` past `lui at ; sw v0,%lo(at)`) but NOT
    across pointer-based accesses (`lw v0,0(a1)`, GsSetLightMatrix)."""
    base = get_rs(words[q])
    return q > 0 and is_lui(words[q - 1]) and get_rt(words[q - 1]) == base


def _mem_independent(words, p, q):
    a, b = words[p], words[q]
    if _disjoint_sp_slots(a, b):
        return True
    if not (_mem_width(a) and _mem_width(b)):
        return False
    if get_rs(a) == 29 and get_rs(b) != 29 and _lui_absolute(words, q):
        return True
    if get_rs(b) == 29 and get_rs(a) != 29 and _lui_absolute(words, p):
        return True
    return False


# --fill-load-delay-no-div (2026-09-26): a divide-family word -- div/divu
# (the head of an ASPSX div/rem macro) or mfhi/mflo (its deferred result
# read) -- never fills a load-delay nop.  ASPSX schedules the macro as a
# unit at macro level; our object-level pass sees the expanded words and
# would move them.  Retail GsSortBg 0x80099F68 `div ; lh t7,6(s5) ;
# nop ; sw` keeps the nop, where --fill-load-delay alone sinks the div (or,
# with div excluded, the mfhi) into it.  Opt-in, never in --fill-all.
NO_DIV_FILL = [False]


def _is_div_family(words, p):
    """div/divu, or an mfhi/mflo whose HI/LO producer (the nearest earlier
    mult/multu/div/divu/mthi/mtlo) is a div/divu.  An mflo reading a MULT
    stays a legal filler: retail gte_scale_matrix 0x8009E264 `mult v0,a1 ;
    lh v0,6(t0) ; mflo a0` (census 2026-09-26)."""
    if not NO_DIV_FILL[0]:
        return False
    w = words[p]
    if get_opcode(w) != 0:
        return False
    f = get_func(w)
    if f in (0x1a, 0x1b):
        return True
    if f not in (0x10, 0x12):
        return False
    for r in range(p - 1, -1, -1):
        x = words[r]
        if get_opcode(x) == 0 and get_func(x) in (0x18, 0x19, 0x1a, 0x1b, 0x11, 0x13):
            return get_func(x) in (0x1a, 0x1b)
    return False


def _unfillable_nop(words, q, k, b=0):
    """A load-delay nop at q (after a load at q-1) that no later word of the
    block could be hoisted into: every word in (q, k) reads the loaded
    register or depends on one that does.  ASPSX leaves such a nop in place
    (retail v_wait `lw v0 ; nop ; slt v0,v0,a0 ; beqz`), and a word
    from above may then be scheduled across it into the branch slot.  When
    a later independent word exists ASPSX hoists it into the nop instead
    (SetRCnt `lw a0 ; li v0,1 ; addu v1,v1,a0`)."""
    if q == 0:
        return False
    ld = insn_rw(words[q - 1])
    if ld is None or ld[2] != 'load' or not ld[0]:
        return False
    tainted = set(ld[0])
    for r in range(q + 1, k):
        y = insn_rw(words[r])
        if y is None:
            return False
        yw, yr, yk = y
        if not (yr & tainted):
            return False                 # independent word: ASPSX would hoist it
        tainted |= yw
    # an ALU word above the load that could legally sink into the nop also
    # fills it (retail SetRCnt: `li v0,1 ; lui a0 ; lw a0 ; [nop]` ->
    # `lui ; lw ; li`); stores never do (v_wait keeps `lw v0 ; nop ;
    # slt` although `sw a1,16(sp)` sits just above).
    for p in range(q - 2, b - 1, -1):
        x = insn_rw(words[p])
        if x is None or x[2] != 'alu':
            continue
        if _is_div_family(words, p):
            continue
        xw, xr, _ = x
        if 29 in xw or (xr & ld[0]) or (xw & ld[0]) or (xw & ld[1]):
            continue
        ok = True
        for r in range(p + 1, q - 1):
            y = insn_rw(words[r])
            if y is None:
                ok = False
                break
            yw, yr, _ = y
            if (yr & xw) or (yw & xw) or (yw & xr):
                ok = False
                break
        if ok:
            return False                 # fillable from above
    return True


def _lwlr_pair(a, b):
    """True for an lwl/lwr (or lwr/lwl) pair merging into the SAME rt.  lwr
    reads the rt that lwl just wrote, which the generic load-use test flags,
    but the MIPS I architecture defines back-to-back lwl/lwr on one register
    as hazard-free (the merge is bypassed in hardware) and ASPSX/gcc emit the
    pair adjacent.  Treating it as a hazard refused legal fills on CD_cachefile
    (2026-09-24)."""
    oa, ob = a >> 26, b >> 26
    return (oa in (0x22, 0x26) and ob in (0x22, 0x26)
            and ((a >> 16) & 31) == ((b >> 16) & 31))


def _hazard_free_move(words, q, p):
    """True when moving words[p] into the load-delay nop at words[q] leaves no
    NEW load-use hazard.

    The candidate rules below only check that the moved word may CROSS the
    words between it and the slot.  They never ask what happens at the place it
    LEAVES: when the word is taken from below and was itself covering the
    load-delay of the instruction above it, removing it closes that gap and the
    object reads a register one cycle after its load -- a real R3000 hazard, not
    a scheduling nit.  getintr shipped exactly that
    (`lbu v0,0(v0) ; sb v0,0(v1)`, 2026-09-17) and it read as count-exact
    because the vanished nop cancelled a surplus word elsewhere.  Simulate the
    move and refuse it if any load is left adjacent to a reader of its
    destination."""
    cur = list(words)
    w = cur[p]
    del cur[p]
    qq = q - 1 if p < q else q
    cur[qq] = w
    lo = max(0, min(p, qq) - 2)
    hi = min(len(cur) - 1, max(p, qq) + 2)
    for i in range(lo, hi):
        x = insn_rw(cur[i])
        if x is None or x[2] != 'load' or not x[0]:
            continue
        y = insn_rw(cur[i + 1])
        if y is not None and (x[0] & y[1]):
            if _lwlr_pair(cur[i], cur[i + 1]):
                continue
            return False
    return True


def _hazard_free_swap(words, q, p):
    """True when EXCHANGING the load-delay nop at q with the word at p (q < p)
    leaves no load-use hazard anywhere near either end.

    A plain move deletes the word at p, so when that word was the only thing
    covering the load-delay of the instruction above it the object ships a
    real R3000 hazard and one word short (see _hazard_free_move).  ASPSX does
    not give up in that situation: it fills the EARLIEST slot and lets the
    assembler put a nop back at the later one, which is a SWAP -- same word
    count, hazard still covered.  Retail getintr words 28-33
    (`lui ; lw ; addiu a0,a0,1 ; lbu ; nop ; sb`) is exactly that shape;
    ours had `lui ; lw ; nop ; lbu ; addiu ; sb`."""
    cur = list(words)
    cur[q], cur[p] = cur[p], cur[q]
    lo = max(0, q - 2)
    hi = min(len(cur) - 1, p + 2)
    for i in range(lo, hi):
        x = insn_rw(cur[i])
        if x is None or x[2] != 'load' or not x[0]:
            continue
        y = insn_rw(cur[i + 1])
        if y is not None and (x[0] & y[1]):
            if _lwlr_pair(cur[i], cur[i + 1]):
                continue
            return False
    return True


def _is_sp_adjust(w):
    """`addiu $sp,$sp,imm` -- the frame allocation or the stack restore."""
    return is_addiu(w) and get_rs(w) == 29 and get_rt(w) == 29


def _fill_load_delay_nops(words, targets, fill_sp_adjust=False):
    """--fill-load-delay: ASPSX fills a load-delay hazard nop with the NEAREST
    plain ALU word of the same basic block that can legally move there --
    from above (retail SetRCnt: `li v0,1 ; lui a0 ; lw a0 ; [nop] ;
    addu` -> `lui ; lw ; li ; addu`; func_8009E0A0: `move s0,a0` into the
    `lw v0,8(s2)` delay) or below (mul_rdmat: `lw v0,4(s1) ; [nop]` takes
    the later `lui a3,0x100`).  Stores/loads never fill it (v_wait
    keeps `lw v0 ; nop ; slt` although `sw a1,16(sp)` sits just above).

    fill_sp_adjust (--fill-sp-adjust, 2026-09-24): the `addiu sp,sp,N`
    frame allocation / stack restore is an ordinary ALU filler for ASPSX.
    Retail _mk_normsp sinks its prologue `addiu sp,sp,-64` 14 words down
    into the `lh v1,%lo(v1)` delay (every nearer word is pinned by a
    dependency) and hoists its epilogue `addiu sp,sp,64` into the
    `lbu v0,28(sp)` delay (`lbu ; addiu sp ; jr ra ; sb v0`).  Legality is
    the generic rule with $sp as the moved word's source and destination:
    no crossed word may read or write $sp (so it never passes a stack
    access or a register save), and from ABOVE the load itself must not
    use $sp as its base (the move would change the load's address -- the
    generic `xw & load-reads` test).  From BELOW an $sp-based load is fine:
    its address is formed before the adjust executes.  Only the exact
    `addiu sp,sp,imm` form is released; any other $sp writer stays pinned.
    Returns (nop_index, word_index) moves."""
    moves = []
    n = len(words)
    for q in range(1, n - 1):
        if not is_nop(words[q]):
            continue
        ld = insn_rw(words[q - 1])
        if ld is None or ld[2] != 'load' or not ld[0]:
            continue
        if q >= 2 and is_control(words[q - 2]):
            continue                         # the load sits in a branch slot; q is not a hazard nop
        b = q - 1
        while b > 0 and not is_control(words[b - 1]) and not (b >= 2 and is_control(words[b - 2])) and b not in targets:
            b -= 1
        e = q + 1
        while e < n and not is_control(words[e]) and e not in targets:
            e += 1
        best = None
        kind = 'move'
        for p in range(q - 2, b - 1, -1):                 # above, nearest first
            x = insn_rw(words[p])
            if x is None or x[2] != 'alu':
                continue
            if _is_div_family(words, p):
                continue
            xw, xr, _ = x
            if 29 in xw and not (fill_sp_adjust and _is_sp_adjust(words[p])):
                continue
            if (xr & ld[0]) or (xw & ld[0]) or (xw & ld[1]):
                continue
            ok = True
            for r in range(p + 1, q - 1):
                y = insn_rw(words[r])
                if y is None:
                    # a nop reads and writes nothing, so it is always
                    # crossable; treating it as opaque capped the search at
                    # the first hazard nop and cost getintr its
                    # `lui at` hoist (2026-09-17).
                    if is_nop(words[r]):
                        continue
                    ok = False; break
                yw, yr, _ = y
                if (yr & xw) or (yw & xw) or (yw & xr):
                    ok = False; break
            if ok and _hazard_free_move(words, q, p):
                best = p; break
        if best is None:
            for p in range(q + 1, e):                     # below, nearest first
                x = insn_rw(words[p])
                if x is None or x[2] != 'alu':
                    continue
                if _is_div_family(words, p):
                    continue
                xw, xr, _ = x
                if 29 in xw and not (fill_sp_adjust and _is_sp_adjust(words[p])):
                    continue
                if xr & ld[0]:
                    continue
                ok = True
                for r in range(q + 1, p):
                    y = insn_rw(words[r])
                    if y is None:
                        if is_nop(words[r]):
                            continue
                        ok = False; break
                    yw, yr, _ = y
                    if (yw & xr) or (yr & xw) or (yw & xw):
                        ok = False; break
                if not ok:
                    continue
                if _hazard_free_move(words, q, p):
                    best = p; break
                if q not in targets and p not in targets and \
                        _hazard_free_swap(words, q, p):
                    best = p; kind = 'swap'; break
        if best is not None:
            moves.append((q, best, kind))
    return moves


# --sink-sp-adjust bookkeeping: how many sp-adjust words the last
# process_text() call moved.  A pure sink leaves the word count unchanged,
# and main() used to treat "same length" as "nothing happened"; with the
# flag it also consults this counter (without the flag the old length test
# is the whole decision, so the default output is byte-identical).
SINK_MOVED = [0]


def _sink_touches_sp(w):
    x = insn_rw(w)
    return x is None or 29 in x[0] or 29 in x[1]


def _sink_sp_adjust_moves(words, targets):
    """--sink-sp-adjust (2026-09-24): ASPSX places the frame allocation
    `addiu sp,sp,-N` just after the LAST LOAD of the entry block and the
    stack restore `addiu sp,sp,+N` just after the last load above it, even
    when there is no hazard nop to fill (the no-hazard generalisation of
    --fill-sp-adjust).  Retail:
      StCdInterrupt  `lui v0 ; lw v0,D_80130B64 ; addiu sp,-56 ; li a0,1 ;
                      beq v0,a0 ; sw ra` (gcc: `addiu sp` first, no nop)
      GsSwapDispBuff  `... lhu v1,%lo ; addiu sp,-24 ; lui a0 ; addiu a0 ;
                      sw ra`
      ResetCallback  `lui v0 ; lw v0 ; addiu sp,-24 ; sw ra`
      _mk_normsp  after `lh v1,%lo`, 14 words down (a hazard nop there),
                      epilogue `lbu v0,28(sp) ; addiu sp,64 ; jr ra`.
    Legality, from the examples: the moved word crosses nothing that reads
    or writes $sp (so never a `sw ra`, a stack store or an sp-based load,
    and never the other adjust); it stays inside its basic block (no
    control word, no branch target, not a delay slot); an unknown word
    stops the scan.  Prologue: the destination is after the last load
    reached before the first $sp user / block end; epilogue: after the
    nearest load above.  A nop directly after that load is the load's
    hazard nop and the adjust REPLACES it (the --fill-sp-adjust case);
    otherwise the adjust is inserted and the count is unchanged.  Removing
    the word must not expose a load-use hazard between its neighbours.
    Returns [(src, load_index, replace_nop)]."""
    n = len(words)
    moves = []

    def ctl(k):
        return 0 <= k < n and is_control(words[k])

    def removal_ok(s):
        a, b = s - 1, s + 1
        if a < 0 or b >= n:
            return True
        x = insn_rw(words[a])
        y = insn_rw(words[b])
        if x is None or y is None or x[2] != 'load' or not x[0]:
            return True
        return not (x[0] & y[1]) or _lwlr_pair(words[a], words[b])

    for s in range(n):
        w = words[s]
        if not _is_sp_adjust(w) or s in targets or ctl(s - 1):
            continue
        neg = (w & 0xffff) >= 0x8000
        L = None
        if neg:
            r = s + 1
            while r < n and not ctl(r) and r not in targets:
                if is_nop(words[r]):
                    r += 1
                    continue
                if _sink_touches_sp(words[r]):
                    break
                if insn_rw(words[r])[2] == 'load':
                    L = r
                r += 1
        else:
            r = s - 1
            while r >= 0 and not ctl(r) and not ctl(r - 1):
                if is_nop(words[r]):
                    if r in targets:
                        break
                    r -= 1
                    continue
                x = insn_rw(words[r])
                if x is None:
                    break
                if x[2] == 'load':
                    L = r
                    break
                if 29 in x[0] or 29 in x[1] or r in targets:
                    break
                r -= 1
            if L is not None and L + 1 == s:
                L = None                     # already just after it
        if L is None or not removal_ok(s):
            continue
        replace = (L + 1 < n and L + 1 != s and is_nop(words[L + 1])
                   and (L + 1) not in targets)
        moves.append((s, L, replace))
    return moves


def _apply_sink_moves(words, moves):
    """Apply _sink_sp_adjust_moves.  Returns (words, orig->new map with None
    for a replaced nop, label overrides {orig_index: new_index})."""
    idx = list(range(len(words)))
    cur = list(words)
    heads = {}
    for s, L, replace in moves:
        p = idx.index(s)
        w = cur.pop(p)
        wi = idx.pop(p)
        if p < len(idx) and p <= idx.index(L):
            # the prologue adjust leaves its position (and the FUNC symbol
            # that starts there) to the word that followed it -- see the
            # same rule in _apply_load_delay_moves.
            heads[s] = idx[p]
        ins = idx.index(L) + 1
        if replace and ins < len(cur) and is_nop(cur[ins]):
            cur[ins] = w
            idx[ins] = wi
        else:
            cur.insert(ins, w)
            idx.insert(ins, wi)
    omap = {o: None for o in range(len(words))}
    for i, o in enumerate(idx):
        omap[o] = i
    labels = {o: omap[h] for o, h in heads.items() if omap.get(h) is not None}
    return cur, omap, labels


def _apply_load_delay_moves(words, moves, targets):
    """Apply (nop_index, word_index) moves one at a time (each shortens the
    stream by one word).  Returns (new_words, original->new index map with
    None for the deleted nop, label overrides for a moved block head)."""
    idx = list(range(len(words)))
    cur = list(words)
    labels = {}
    # A filler word keeps its ORIGINAL index in `idx` after it moves, so a
    # second move naming the same source would pick it up again from its new
    # home and delete a second nop (getintr lost a word that way once
    # both candidate slots for one `lui at` became legal, 2026-09-17).  Each
    # word fills at most one slot, and each slot is filled at most once.
    used_src = set()
    used_dst = set()
    for entry in moves:
        q0, p0 = entry[0], entry[1]
        mkind = entry[2] if len(entry) > 2 else 'move'
        if q0 not in idx or p0 not in idx:
            continue
        if p0 in used_src or q0 in used_dst:
            continue
        used_src.add(p0); used_dst.add(q0)
        q = idx.index(q0); p = idx.index(p0)
        if not is_nop(cur[q]):
            continue
        if mkind == 'swap':
            # count-preserving exchange: the ALU word takes the earlier slot
            # and the nop moves down to keep the later hazard covered.
            cur[q], cur[p] = cur[p], cur[q]
            idx[q], idx[p] = idx[p], idx[q]    # relocations follow the word
            continue
        w = cur[p]; wi = idx[p]
        del cur[p]; del idx[p]
        if p < q:
            q -= 1
            # A label on the moved word (branch target, or -- for the
            # --fill-sp-adjust frame allocation -- the FUNC symbol start)
            # stays at the position and lands on the word that follows.
            # Without this the function symbol followed `addiu sp,sp,-N`
            # 14 words down and `strip_leading_instructions --count auto`
            # stripped the whole cross-prologue head (_mk_normsp).
            # sp-adjust fillers only move under --fill-sp-adjust, so the
            # default output is unchanged.
            if (p0 in targets or _is_sp_adjust(w)) and p < len(idx):
                labels[p0] = idx[p]
        cur[q] = w; idx[q] = wi
    omap = {orig: None for orig in range(len(words))}
    for i, orig in enumerate(idx):
        omap[orig] = i
    fixed = {}
    for orig_head, new_head_orig in labels.items():
        if omap.get(new_head_orig) is not None:
            fixed[orig_head] = omap[new_head_orig]
    return cur, omap, fixed


def is_control(w):
    return (is_cond_branch(w) or is_j(w) or is_jal(w) or is_jr(w) or is_jalr(w)
            or (get_opcode(w) == 0 and get_func(w) in (0x0c, 0x0d)))


def _control_reads_writes(c):
    """Registers a control instruction reads / writes (for slot legality)."""
    if is_cond_branch(c):
        return _branch_srcs(c), set()
    if is_jal(c):
        return set(), {31}
    if is_jalr(c):
        return {get_rs(c)}, {get_rd(c) or 31}
    if is_jr(c):
        return {get_rs(c)}, set()
    return set(), set()


def _block_start(words, k, targets):
    """First index of the basic block that ends with the control word at k."""
    if k in targets:
        # The control word is itself a branch target: its block is just
        # [k], so nothing above it may sink into its slot.  Without this
        # guard pattern 9 pulled `move v0,zero` into a `jr ra` that a
        # `bne ...; li v0,1` jumps to (StGetNext, 2026-09-07) -- the
        # slot executes on the branch path too and clobbers the return.
        return k
    p = k - 1
    while p >= 0:
        if is_control(words[p]):
            return p + 2
        if p > 0 and is_control(words[p - 1]):
            return p + 1
        if p in targets:
            return p
        p -= 1
    return 0


def _earliest_slot_filler(words, k, targets):
    """ASPSX reorder mode: for `C; nop` at k, return the index p of the
    EARLIEST instruction of C's basic block that can legally execute in C's
    delay slot instead (retail CD_init: the `addiu a0,lo` of a `la` four
    words above the jal; PadInit: the prologue `sw s0` past two other
    stores; mul_dmat: `move v0,a2` past the whole epilogue restore
    sequence), or None.  Legal means: X is a plain ALU/store word; nothing
    between X and C reads or writes X's destination or writes X's sources;
    a store crosses no other memory op; C reads nothing X writes; a jal/jalr
    slot never touches $ra; and removing X does not expose a load-delay
    hazard between its neighbours."""
    c = words[k]
    c_reads, c_writes = _control_reads_writes(c)
    b = _block_start(words, k, targets)
    for p in range(b, k):
        x = insn_rw(words[p])
        if x is None or x[2] == 'load':
            continue
        xw, xr, xk = x
        # the frame-allocating `addiu sp,sp,-N` never moves (retail keeps it
        # first even when a branch follows immediately); the epilogue's
        # positive adjust does move (`jr ra ; addiu sp,sp,N`).
        if 29 in xw and not (is_addiu(words[p]) and get_rs(words[p]) == 29
                             and (words[p] & 0xffff) < 0x8000):
            continue
        ok = True
        for q in range(p + 1, k):
            if is_nop(words[q]):
                if _unfillable_nop(words, q, k, b):
                    continue                 # a permanent hazard nop is transparent
                ok = False                   # ASPSX would fill it (5780: `li v0,1` hoisted into it)
                break
            y = insn_rw(words[q])
            if y is None:
                ok = False
                break
            yw, yr, yk = y
            if (yr & xw) or (yw & xw) or (yw & xr):
                ok = False
                break
            if xk == 'store' and yk in ('load', 'store') and not _mem_independent(words, p, q):
                ok = False
                break
        if not ok:
            continue
        if (xw & c_reads) or (xw & c_writes) or (xr & c_writes):
            continue
        # removing X: load at p-1 whose consumer was at p+1 would become adjacent
        if p - 1 >= b:
            prev = insn_rw(words[p - 1])
            if prev is not None and prev[2] == 'load' and prev[0]:
                nxt = words[p + 1] if p + 1 < k else c
                nr = insn_rw(nxt)
                nreads = nr[1] if nr is not None else c_reads
                if prev[0] & nreads:
                    continue
        return p
    return None


# ---------------------------------------------------------------------------
# --fill-ptr-bump-down: ASPSX sinks an EARLIER pointer bump into a load-delay
# nop, rewriting the displacements of the same-base memory words it hops.
# ---------------------------------------------------------------------------
PTR_BUMP_MOVED = [0]
FILL_TRACE = [bool(os.environ.get('FILL_TRACE'))]

# lb lh lw lbu lhu / sb sh sw -- the aligned forms only (lwl/lwr/swl/swr
# pair displacements are never rewritten)
_PB_MEM_OPS = (0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2b)


def _is_ptr_bump(w):
    """`addiu rX,rX,K` with K != 0 and rX not $zero/$at/$sp/$ra."""
    return (get_opcode(w) == 0x09 and get_rs(w) == get_rt(w)
            and get_rs(w) not in (0, 1, 29, 31) and (w & 0xffff) != 0)


def _s16(v):
    v &= 0xffff
    return v - 0x10000 if v >= 0x8000 else v


def _pb_hazards(ws, ids, lo, hi):
    """Load-use adjacencies (load at i, reader of its destination at i+1)
    in ws[lo:hi+1], as (orig_id_load, orig_id_reader) pairs."""
    out = set()
    for i in range(max(0, lo), min(len(ws) - 1, hi)):
        x = insn_rw(ws[i])
        if x is None or x[2] != 'load' or not x[0]:
            continue
        y = insn_rw(ws[i + 1])
        if y is not None and (x[0] & y[1]) and not _lwlr_pair(ws[i], ws[i + 1]):
            out.add((ids[i], ids[i + 1]))
    return out


def _ptr_bump_down_one(cur, idx, targets, relocs):
    """Find and build ONE legal move on the current stream, or None.

    Returns (new_cur, new_idx, trace_line)."""
    n = len(cur)
    tpos = {i for i, o in enumerate(idx) if o in targets}
    rpos = {i for i, o in enumerate(idx) if o in relocs}
    for q in range(2, n - 1):
        if not is_nop(cur[q]) or q in tpos:
            continue
        ld = insn_rw(cur[q - 1])
        if ld is None or ld[2] != 'load' or not ld[0]:
            continue
        if is_control(cur[q - 2]):
            continue                      # the load is itself a delay-slot word
        # basic block [b, q): stop at a branch target, a control word and
        # its delay slot (the same boundary rule as --fill-load-delay)
        b = q - 1
        while (b > 0 and not is_control(cur[b - 1])
               and not (b >= 2 and is_control(cur[b - 2])) and b not in tpos):
            b -= 1
        # nearest bump above the load
        for p in range(q - 2, b - 1, -1):
            if not _is_ptr_bump(cur[p]) or p in rpos:
                continue
            rx = get_rs(cur[p])

            def legal(r):
                """'bump', 'mem', 'cross' or None for word r w.r.t. rx."""
                w = cur[r]
                if _is_ptr_bump(w) and get_rs(w) == rx:
                    return 'bump' if r not in rpos else None
                if get_opcode(w) in _PB_MEM_OPS and get_rs(w) == rx:
                    if get_rt(w) == rx or r in rpos:
                        return None       # rX in a non-base role / reloc'd
                    return 'mem'
                x = insn_rw(w)
                if x is None:
                    return None           # nop, control, unknown: a wall
                xw, xr, _ = x
                if rx in xw or rx in xr:
                    return None
                return 'cross'

            kinds = {}
            ok = True
            for r in range(p + 1, q):
                k = legal(r)
                if k is None:
                    ok = False
                    break
                kinds[r] = k
            if not ok:
                continue
            kinds[p] = 'bump'
            # extend the chain upward: every earlier bump of rX that can
            # sink with it goes too (retail dma_execute: both `addiu v1,4`
            # land below the stores at 0/4/8(v1))
            top = p
            for r in range(p - 1, b - 1, -1):
                k = legal(r)
                if k is None:
                    break
                kinds[r] = k
                if k == 'bump':
                    top = r
            if any(r in tpos for r in range(top, q + 1)):
                continue                  # a label inside the hop
            # rebuild [top, q]: the kept words (displacements += every
            # bump above them), then the bumps, which take the nop's place
            kept, kept_ids, bumps, bump_ids = [], [], [], []
            acc = 0
            fits = True
            nmem = 0
            for r in range(top, q):
                w = cur[r]
                if kinds[r] == 'bump':
                    bumps.append(w)
                    bump_ids.append(idx[r])
                    acc += _s16(w)
                    continue
                if kinds[r] == 'mem' and acc:
                    d = _s16(w) + acc
                    if not -0x8000 <= d <= 0x7fff:
                        fits = False
                        break
                    w = (w & 0xffff0000) | (d & 0xffff)
                    nmem += 1
                kept.append(w)
                kept_ids.append(idx[r])
            if not fits:
                continue
            new = cur[:top] + kept + bumps + cur[q + 1:]
            nid = idx[:top] + kept_ids + bump_ids + idx[q + 1:]
            # where the words LEAVE: no new load-use adjacency anywhere in
            # the rewritten span (plus one word either side)
            before = _pb_hazards(cur, idx, top - 1, q + 1)
            after = _pb_hazards(new, nid, top - 1, q)
            if after - before:
                continue
            line = ('fill-ptr-bump-down: %d x addiu $%d (sum %+d) from word %d '
                    '-> load-delay nop at word %d (load word %d); %d displacement(s) '
                    'rewritten' % (len(bumps), rx, acc, top, q, q - 1, nmem))
            return new, nid, line
    return None


def _ptr_bump_down(words, targets, relocs):
    """--fill-ptr-bump-down (ASPSX reorder mode, gcc path).

    Retail's assembler fills a load-delay nop with an EARLIER pointer bump
    `addiu rX,rX,K` of the same basic block, sinking it past the rX-based
    loads/stores between and rewriting their displacements by +K (the
    memory words move ABOVE the bump, so each now adds the K it no longer
    sees in rX).  CdSearchFile (0x80089044):
        cc1:    sb v1,0(s1) ; addiu s0,s0,1 ; lb v0,0(s0) ; lbu v1,0(s0) ; nop ; bne
        retail: sb v1,0(s1) ; lb v0,1(s0) ; lbu v1,1(s0) ; addiu s0,s0,1 ; bne
    Every earlier bump of the same register that can sink with it goes too,
    in order (dma_execute 0x80088DC8: stores at 0/4/8(v1) with both
    `addiu v1,v1,4` below them).  The inverse direction is the IDO path's
    --ido-fill-ptr-bump (a LATER bump hoisted).

    Legality, all required:
      * the nop directly follows a load that is not itself in a delay slot,
        and neither the nop nor any word from the topmost moved bump down
        to it is a branch target / symbol start;
      * every word hopped is either an aligned load/store with base rX,
        no relocation and rX not its data register (its displacement is
        rewritten), another bump of rX (it joins the chain), or a plain
        ALU/memory word that neither reads nor writes rX -- a nop, a
        control word or any unmodelled word is a wall;
      * rX is not $zero/$at/$sp/$ra and no bump carries a relocation;
      * every rewritten displacement stays in the signed 16-bit range;
      * the rewritten span contains no load-use adjacency that was not
        already there (the vacated positions must not expose a hazard).
    The nop is consumed (-1 word per move).  Returns (words, orig->new map
    with None for the consumed nops, number of moves)."""
    cur = list(words)
    idx = list(range(len(words)))
    nmoves = 0
    while True:
        m = _ptr_bump_down_one(cur, idx, targets, relocs)
        if m is None:
            break
        cur, idx, line = m
        nmoves += 1
        if FILL_TRACE[0]:
            print(line, file=sys.stderr)
    omap = {o: None for o in range(len(words))}
    for i, o in enumerate(idx):
        omap[o] = i
    return cur, omap, nmoves


# ---------------------------------------------------------------------------
# --fill-load-delay-macro (2026-09-26): ASPSX's load-delay fill at MACRO level
# ---------------------------------------------------------------------------
# Unlike every other pattern in this file this one runs on the COMPILER TEXT
# (cc1's .s, before maspsx), because ASPSX makes the decision before it
# expands macros: a div/rem macro is ONE word to it, and the words it
# crosses are the source words, not the expanded check branches.
#
# Rule: in a reorder region, a load followed by cc1's `#nop` load-delay
# marker takes the NEAREST legal LATER word of the same basic block into the
# delay slot -- an ALU word or another LOAD.  The search
#   - skips `#nop` markers;
#   - stops at a label, a directive, a branch/jump, or an independent
#     symbol-load macro (`lw $r,SYM`: maspsx's own lui fill takes that slot,
#     retail GsSortBg 0x80099EB8);
#   - may cross div/rem/mult macros and stores (never picks them);
#   - rejects a candidate that reads or writes the loaded register, or that
#     conflicts (RAW/WAR/WAW, HI/LO included) with any crossed word;
#   - a LOAD candidate may cross a store only when both are $sp-relative at
#     different offsets (retail 0x80099EEC `lh t7,8 ; lhu v1,136(sp) ;
#     sw t7,104(sp)`);
#   - rejects a candidate whose removal would leave a load directly followed
#     by a reader of its destination (no new hazard where the word LEAVES).
# Retail evidence: GsSortBg 0x8009A040 (cc1 `lw $15,112 ; #nop ; rem ;
# div ; lw $15,120 ; rem ; lw $15,152 ; div ; mult ; lw $5,20($21)` ->
# `lw t7,112 ; lw a1,20(s5) ; div ...`) and 0x80099EEC; negative controls
# that keep their nop: 0x80099F64, 0x80099F08, 0x80099F18.
# Moves existing source lines only; never creates, deletes or rewrites one.

_MLD_LOADS = ('lw', 'lh', 'lhu', 'lb', 'lbu', 'lwl', 'lwr')
_MLD_STORES = ('sw', 'sh', 'sb', 'swl', 'swr')


def _mld_parse(line):
    t = line.split('#', 1)[0].strip()
    if not t or t.startswith('.') or t.endswith(':'):
        return None
    parts = t.split(None, 1)
    ops = [x.strip() for x in parts[1].split(',')] if len(parts) > 1 else []
    return parts[0], ops


def _mld_regs(text):
    import re
    return set(re.findall(r'\$(?:[a-z]+\d*|\d+)', text))


def _mld_is_num(x):
    try:
        int(x, 0)
        return True
    except ValueError:
        return False


def _mld_rw(op, a):
    """(writes, reads, kind) of one source line, or None if opaque."""
    import re
    if op in _MLD_LOADS:
        if len(a) < 2:
            return None
        m = re.match(r'(.*)\((\$\w+)\)$', a[1])
        if not m or not _mld_is_num(m.group(1).strip() or '0'):
            return {a[0]}, (_mld_regs(a[1])), 'symload'
        return {a[0]}, {m.group(2)}, 'load'
    if op in _MLD_STORES:
        if len(a) < 2:
            return None
        return set(), {a[0]} | _mld_regs(a[1]), 'store'
    if op in ('rem', 'div', 'remu', 'divu', 'mul'):
        if not a:
            return None
        if len(a) == 3:
            return {a[0], 'HI', 'LO'}, _mld_regs(','.join(a[1:])), 'md'
        return {'HI', 'LO'}, _mld_regs(','.join(a)), 'md'
    if op in ('mult', 'multu'):
        return {'HI', 'LO'}, _mld_regs(','.join(a)), 'md'
    if op == 'mflo':
        return {a[0]}, {'LO'}, 'mf'
    if op == 'mfhi':
        return {a[0]}, {'HI'}, 'mf'
    if op in ('mtlo', 'mthi'):
        return {op[2:].upper()}, {a[0]}, 'mf'
    if op[0] == 'b' or op in ('j', 'jal', 'jr', 'jalr', 'break', 'syscall'):
        return None
    if op in ('la', 'li', 'lui'):
        return {a[0]}, set(), 'alu'
    if not a:
        return None
    return {a[0]}, _mld_regs(','.join(a[1:])), 'alu'


def _mld_sp_slot(operand):
    import re
    m = re.match(r'(-?\d+)\((\$sp)\)$', operand.strip())
    return int(m.group(1)) if m else None


def fill_load_delay_macro(lines, trace=False):
    """Apply the --fill-load-delay-macro rule to cc1 text lines.
    Returns (new_lines, number_of_fills)."""
    out = list(lines)
    reorder = True
    n = 0
    i = 0
    while i < len(out):
        s = out[i].strip()
        if s.startswith('.set'):
            w = s.split()[-1]
            if w == 'noreorder':
                reorder = False
            elif w == 'reorder':
                reorder = True
        pi = _mld_parse(out[i]) if reorder else None
        if (pi and pi[0] in _MLD_LOADS and i + 1 < len(out)
                and out[i + 1].strip() == '#nop'):
            ld = _mld_rw(*pi)
            dest = ld[0] if ld else set()
            crossed = []          # (writes, reads, kind, line index, mem operand)
            j = i + 2
            best = None
            while ld is not None and j < len(out):
                t = out[j].strip()
                if not t or t == '#nop' or t.startswith('#'):
                    j += 1
                    continue
                if t.startswith('.') or t.endswith(':'):
                    break
                q = _mld_parse(out[j])
                if q is None:
                    break
                x = _mld_rw(*q)
                if x is None:
                    break
                w, r, k = x
                if k == 'symload' and not (r & dest) and not (w & dest):
                    break
                ok = k in ('alu', 'load') and not (r & dest) and not (w & dest)
                if ok:
                    for cw, cr, ck, _cj, cmem in crossed:
                        if (cw & r) or (cr & w) or (cw & w):
                            ok = False
                            break
                        if k == 'load' and ck == 'store':
                            a_slot = _mld_sp_slot(q[1][1]) if len(q[1]) > 1 else None
                            b_slot = _mld_sp_slot(cmem)
                            if a_slot is None or b_slot is None or a_slot == b_slot:
                                ok = False
                                break
                if ok and crossed:
                    # no new load-use hazard where the word leaves: the
                    # word above it (last crossed) must not be a load whose
                    # destination the next instruction below it reads.
                    pw, _pr, pk, _pj, _pm = crossed[-1]
                    if pk in ('load', 'symload'):
                        nxt = None
                        for jj in range(j + 1, len(out)):
                            tt = out[jj].strip()
                            if not tt or tt.startswith('#'):
                                continue
                            if tt.startswith('.') or tt.endswith(':'):
                                break
                            nq = _mld_parse(out[jj])
                            nxt = _mld_rw(*nq) if nq else None
                            break
                        if nxt is None or (nxt[1] & pw):
                            ok = False
                if ok:
                    best = j
                    break
                crossed.append((w, r, k, j, q[1][1] if len(q[1]) > 1 else ''))
                j += 1
            if best is not None:
                mv = out.pop(best)
                out[i + 1] = mv
                n += 1
                if trace:
                    sys.stderr.write('fill-load-delay-macro: %s <- %s\n'
                                     % (out[i].strip(), mv.strip()))
        i += 1
    return out, n


def _branch_targets(words):
    t = set()
    for i, w in enumerate(words):
        if is_cond_branch(w):
            t.add(i + 1 + get_branch_offset(w))
        elif is_j(w):
            tg = get_j_target(w)
            if 0 < tg < len(words):
                t.add(tg)
    return t


def process_text(words, fill_jalr=False, fill_epilogue=False, fill_j=False,
                 fill_jal_store=False, fill_branch_store=False, fill_prev=False,
                 fill_earliest=False, fill_load_delay=False, fill_sp_adjust=False,
                 sink_sp_adjust=False, fill_ptr_bump_down=False,
                 reloc_words=(), pinned_words=()):
    """Process instruction words to fill delay slots.

    fill_ptr_bump_down (--fill-ptr-bump-down) runs FIRST, as its own pass
    (see _ptr_bump_down); the remaining passes then see its output with the
    branch-target set mapped through its move map, and the final
    original->new map is the composition.  reloc_words / pinned_words
    (relocated word indices / symbol starts) are only consulted by that
    pass.  With the flag off this is exactly _process_text_core.
    """
    kw = dict(fill_jalr=fill_jalr, fill_epilogue=fill_epilogue, fill_j=fill_j,
              fill_jal_store=fill_jal_store, fill_branch_store=fill_branch_store,
              fill_prev=fill_prev, fill_earliest=fill_earliest,
              fill_load_delay=fill_load_delay, fill_sp_adjust=fill_sp_adjust,
              sink_sp_adjust=sink_sp_adjust)
    PTR_BUMP_MOVED[0] = 0
    if not fill_ptr_bump_down:
        return _process_text_core(words, **kw)
    targets0 = _branch_targets(words)
    words1, map0, nmoves = _ptr_bump_down(words, targets0 | set(pinned_words),
                                          set(reloc_words))
    if not nmoves:
        return _process_text_core(words, **kw)
    PTR_BUMP_MOVED[0] = nmoves
    # a moved bump or a deleted nop is never a target (the pass refuses it),
    # so every target survives the map
    t1 = {map0[t] for t in targets0 if map0.get(t) is not None}
    new_words, map1 = _process_text_core(words1, base_targets=t1, **kw)
    labels1 = dict(LABEL_OVERRIDES)
    final = {o: (None if m is None else map1.get(m)) for o, m in map0.items()}
    LABEL_OVERRIDES.clear()
    inv = {m: o for o, m in map0.items() if m is not None}
    for m, nw in labels1.items():
        if m in inv and nw is not None:
            LABEL_OVERRIDES[inv[m]] = nw
    return new_words, final


def _process_text_core(words, fill_jalr=False, fill_epilogue=False, fill_j=False,
                       fill_jal_store=False, fill_branch_store=False, fill_prev=False,
                       fill_earliest=False, fill_load_delay=False, fill_sp_adjust=False,
                       sink_sp_adjust=False, base_targets=None):
    """Process instruction words to fill delay slots.

    Args:
        words: List of instruction words
        fill_jalr: Enable pattern 3 (jalr delay slot filling)
        fill_epilogue: Enable pattern 4 (jr ra epilogue delay slot)
        fill_j: Enable pattern 5 (j delay slot with lui+addiu)
        fill_jal_store: Enable pattern 6 (jal delay slot with lui $at + sw)
        fill_branch_store: Enable pattern 7 (ASPSX reorder mode: the store
            immediately before a conditional branch or `j` moves into its
            delay slot -- `sh a3,X(at); beqz v0,L; nop` -> `beqz v0,L; sh`)

    Returns:
        new_words: List of instruction words with delay slots filled
        offset_map: Dict mapping old word index to new word index
    """
    n = len(words)
    transforms = []
    LABEL_OVERRIDES.clear()
    pre_map = None
    pre_labels = {}
    # base_targets: the caller already moved words (--fill-ptr-bump-down),
    # so the branch fields in `words` are stale relative to their new
    # positions and must not be re-decoded; use the mapped set instead.
    orig_targets = _branch_targets(words) if base_targets is None else set(base_targets)
    SINK_MOVED[0] = 0
    sink_map = None
    sink_labels = {}
    ld_targets = orig_targets
    if sink_sp_adjust:
        # Runs FIRST: it never moves a control word, so the branch-target
        # set only needs re-indexing through its map (a replaced nop that
        # was itself a target cannot happen -- the pass refuses that).
        smoves = _sink_sp_adjust_moves(words, orig_targets)
        if smoves:
            words, sink_map, sink_labels = _apply_sink_moves(words, smoves)
            n = len(words)
            SINK_MOVED[0] = len(smoves)
            ld_targets = {sink_map[t] for t in orig_targets
                          if sink_map.get(t) is not None}
            pre_map = dict(sink_map)
            pre_labels = dict(sink_labels)
    if fill_load_delay:
        moves = _fill_load_delay_nops(words, ld_targets, fill_sp_adjust=fill_sp_adjust)
        if moves:
            # A word the sink left holding the FUNC symbol keeps it even
            # when this pass moves that word down (_mk_normsp: `move
            # a3,a0` becomes the first word, then fills the `lh v0,18`
            # delay).  Label keeping only -- the fill search above saw the
            # plain target set, so which words move is unchanged.
            keep = set(ld_targets) | set(sink_labels.values())
            words, ld_map, ld_labels = _apply_load_delay_moves(words, moves, keep)
            n = len(words)
            if sink_map is None:
                pre_map, pre_labels = ld_map, ld_labels
            else:
                # compose original -> sunk -> filled
                pre_map = {o: (None if m is None else ld_map.get(m))
                           for o, m in sink_map.items()}
                pre_labels = {}
                inv = {m: o for o, m in sink_map.items() if m is not None}
                for m, nw in ld_labels.items():
                    if m in inv:
                        pre_labels[inv[m]] = nw
                for o, m in sink_labels.items():
                    pre_labels[o] = ld_labels[m] if m in ld_labels else ld_map.get(m)
    # Always known: a j/jal/jalr that is itself a branch/jump TARGET must keep
    # an empty slot -- a word sunk into it from above would execute on the
    # path that arrives at the jump directly (CdSearchFile: pattern 2 sank
    # `addiu a0,%lo($LC5)` into a cross-jumped `jal` and corrupted $a0).
    if pre_map is None:
        targets = set(orig_targets)
    else:
        # The load-delay pass shortened the stream but the j/jal fields
        # (absolute section words) are only rewritten at the very end, so
        # re-decoding them here read STALE targets and left a cross-jumped
        # `jal` unprotected (GsSwapDispBuff / SetRCnt, 2026-09-07).
        # Map the original targets through the move map instead.
        targets = set()
        for t in orig_targets:
            nt = pre_map.get(t)
            if nt is None:
                # the target word itself was the deleted nop: land on the
                # next surviving word
                u = t + 1
                while u < len(pre_map) and pre_map.get(u) is None:
                    u += 1
                nt = pre_map.get(u)
            if nt is not None:
                targets.add(nt)
        for nt in pre_labels.values():
            if nt is not None:
                targets.add(nt)

    def _div_guard(k):
        """A divide-by-zero check (`bcond ; nop ; break 7`) keeps its nop:
        ASPSX never fills that slot (retail GsSortBg, all nine checks),
        and filling it also hid a 4-word body deficit behind a count that
        read close (2026-09-24)."""
        return (0 <= k and k + 2 < n and is_cond_branch(words[k]) and is_nop(words[k+1])
                and get_opcode(words[k+2]) == 0 and get_func(words[k+2]) == 0x0d)
    i = 0
    while i < n:
        if _div_guard(i) or _div_guard(i + 1):
            i += 1
            continue
        if i + 3 < n:
            w0, w1, w2, w3 = words[i], words[i+1], words[i+2], words[i+3]
            jump_is_target = (i + 2) in targets

            # Pattern 1: lui $at, X; sw $reg, Y($at); j target; nop
            if (not jump_is_target and
                is_lui(w0) and get_rt(w0) == 1 and
                is_sw(w1) and get_rs(w1) == 1 and
                is_j(w2) and is_nop(w3)):
                transforms.append((i, 4, [w0, w2, w1]))
                i += 4
                continue

            # Pattern 6: lui $at, X; sw $reg, Y($at); jal target; nop
            # Same shape as pattern 1 but with jal. The store still runs
            # before the callee (delay slot executes first), so memory
            # ordering is preserved. Guard: never sink a store of $ra --
            # the delay slot of a jal already sees the NEW $ra.
            if fill_jal_store:
                if (not jump_is_target and
                    is_lui(w0) and get_rt(w0) == 1 and
                    is_sw(w1) and get_rs(w1) == 1 and
                    get_rt(w1) != 31 and
                    is_jal(w2) and is_nop(w3)):
                    transforms.append((i, 4, [w0, w2, w1]))
                    i += 4
                    continue

            # Pattern 2: lui $reg, X; addiu $reg, $reg, Y; jal target; nop
            if (not jump_is_target and
                is_lui(w0) and get_rt(w0) != 1 and
                is_addiu(w1) and is_jal(w2) and is_nop(w3)):
                lui_reg = get_rt(w0)
                if get_rs(w1) == lui_reg and get_rt(w1) == lui_reg:
                    transforms.append((i, 4, [w0, w2, w1]))
                    i += 4
                    continue

            # Pattern 5: lui $reg, X; addiu $reg, $reg, Y; j target; nop
            if fill_j:
                if (not jump_is_target and
                    is_lui(w0) and get_rt(w0) != 1 and
                    is_addiu(w1) and is_j(w2) and is_nop(w3)):
                    lui_reg = get_rt(w0)
                    if get_rs(w1) == lui_reg and get_rt(w1) == lui_reg:
                        transforms.append((i, 4, [w0, w2, w1]))
                        i += 4
                        continue

            # Pattern 3: lui $reg, X; addiu $reg, $reg, Y; jalr $func; nop
            # Only safe when $reg != $func (the register being called)
            if fill_jalr:
                if (not jump_is_target and
                    is_lui(w0) and get_rt(w0) != 1 and
                    is_addiu(w1) and is_jalr(w2) and is_nop(w3)):
                    lui_reg = get_rt(w0)
                    jalr_func_reg = get_rs(w2)
                    if (get_rs(w1) == lui_reg and get_rt(w1) == lui_reg and
                        lui_reg != jalr_func_reg):
                        transforms.append((i, 4, [w0, w2, w1]))
                        i += 4
                        continue

        # Pattern 8: X; j/jal/branch; nop  ->  j/jal/branch; X
        # ASPSX reorder mode's basic move: the instruction right before an
        # unfilled branch or jump sinks into the slot.  Legal when X is not
        # itself a branch/jump/store-of-ra and the branch reads no register X
        # writes (the slot executes AFTER the branch decision).  Loads are
        # fine: the slot runs before the target either way.  Gated.
        if fill_prev:
            if i + 2 < n:
                w0, w1, w2 = words[i], words[i+1], words[i+2]
                # REFUSE when w0 is already the DELAY SLOT of a preceding
                # control word: sinking it would leave w1 in that slot --
                # illegal MIPS and a real miscompile (CD_cw
                # `bnez v0 ; li v0,-1 ; jal ; nop` became `bnez ; jal ; li`,
                # 2026-09-25).  Also refuse when w1 is a branch TARGET: w0
                # would then execute only on the fall-through path.
                if (is_nop(w2) and (is_j(w1) or is_jal(w1) or is_cond_branch(w1))
                        and not (is_j(w0) or is_jal(w0) or is_cond_branch(w0)
                                 or is_jr(w0) or is_jalr(w0) or is_nop(w0))
                        and not (is_sw(w0) and get_rt(w0) == 31 and is_jal(w1))
                        and not (i > 0 and is_control(words[i - 1]))
                        and (i + 1) not in targets):
                    d = _dest_reg(w0)
                    if d is None or d not in _branch_srcs(w1):
                        if not (is_jal(w1) and d == 31):
                            transforms.append((i, 3, [w1, w0]))
                            i += 3
                            continue

        # Pattern 9 (--fill-earliest): C; nop  ->  C; X  where X is the
        # EARLIEST legal instruction of C's basic block (ASPSX reorder mode;
        # see _earliest_slot_filler).  Checked at the control word itself so
        # every adjacency pattern above gets first refusal.
        if fill_earliest and i + 1 < n and is_control(words[i]) and is_nop(words[i + 1]) \
                and not (get_opcode(words[i]) == 0 and get_func(words[i]) in (0x0c, 0x0d)):
            p = _earliest_slot_filler(words, i, targets)
            if p is not None:
                seg = words[p + 1:i + 1] + [words[p]]
                # mapping old rel -> new rel: X goes last, the rest shift up one, nop dies
                mapping = {0: i - p}
                for r in range(1, i - p + 1):
                    mapping[r] = r - 1
                mapping[i - p + 1] = None
                transforms.append((p, i - p + 2, seg, mapping, 'label-first' if p in targets else None))
                i += 2
                continue

        # Pattern 7: store; beq/bne/blez/bgtz/bltz/bgez/j; nop
        # ASPSX (reorder mode) sinks the preceding store into the slot.  A
        # store writes no register, the branch reads only registers, and
        # the slot executes on both paths, so the transform is exact.
        # Retail libsnd (SsSetSerialVol 0x800994A0) has both shapes.
        if fill_branch_store:
            if i + 2 < n:
                w0, w1, w2 = words[i], words[i+1], words[i+2]
                # REFUSE when w0 is itself already the DELAY SLOT of a preceding
                # control word: moving it down would leave the branch w1 sitting
                # in that jump's delay slot -- illegal MIPS and a real
                # miscompile (observed on CdSearchFile's
                # `jal CD_cachefile; sb $0,0($17)` pair, 2026-09-05).
                # Also refuse when w1 is a branch TARGET: the store would then
                # execute only on the fall-through path.
                if (is_store(w0) and (is_cond_branch(w1) or is_j(w1)) and
                    is_nop(w2) and not (i > 0 and is_control(words[i - 1])) and
                    i not in targets and (i + 1) not in targets):
                    transforms.append((i, 3, [w1, w0]))
                    i += 3
                    continue

        # Pattern 4: addiu sp, sp, N; jr ra; nop (epilogue delay slot)
        if fill_epilogue:
            if i + 2 < n:
                w0, w1, w2 = words[i], words[i+1], words[i+2]
                if (is_addiu(w0) and get_rs(w0) == 29 and get_rt(w0) == 29 and
                    is_jr(w1) and get_rs(w1) == 31 and is_nop(w2)):
                    transforms.append((i, 3, [w1, w0]))
                    i += 3
                    continue

        i += 1

    if not transforms:
        if pre_map is None:
            return words, {i: i for i in range(n)}
        # The load-delay pass already moved words: the caller still needs
        # the original->new map so relocations, symbols and branch
        # displacements follow the moved words (without this a `%lo` reloc
        # stayed on the old byte offset and landed on the `li` that had
        # been sunk into the slot -- SetRCnt, 2026-09-07; the byte
        # compare hid it because the harness copies reloc immediates).
        LABEL_OVERRIDES.clear()
        for old, new_i in pre_labels.items():
            if new_i is not None:
                LABEL_OVERRIDES[old] = new_i
        return words, dict(pre_map)

    new_words = []
    offset_map = {}

    old_i = 0
    transform_idx = 0

    while old_i < n:
        if transform_idx < len(transforms):
            t_start, t_count, t_new = transforms[transform_idx][:3]
            t_map = transforms[transform_idx][3] if len(transforms[transform_idx]) > 3 else None
            if old_i == t_start:
                new_start = len(new_words)
                if t_map is not None:
                    for r, nr in t_map.items():
                        offset_map[old_i + r] = None if nr is None else new_start + nr
                    if len(transforms[transform_idx]) > 4 and transforms[transform_idx][4] == 'label-first':
                        LABEL_OVERRIDES[old_i] = new_start   # branches into the block land on its new first word
                elif t_count == 4:
                    # [lui, sw/addiu, j/jal/jalr, nop] -> [lui, j/jal/jalr, sw/addiu]
                    offset_map[old_i] = new_start          # lui stays at position 0
                    offset_map[old_i + 1] = new_start + 2  # sw/addiu moves to position 2
                    offset_map[old_i + 2] = new_start + 1  # j/jal/jalr moves to position 1
                    offset_map[old_i + 3] = None            # nop removed
                elif t_count == 3:
                    # [addiu sp, jr ra, nop] -> [jr ra, addiu sp]
                    # [store, branch, nop]   -> [branch, store]
                    offset_map[old_i] = new_start + 1      # addiu moves to position 1
                    offset_map[old_i + 1] = new_start      # jr moves to position 0
                    offset_map[old_i + 2] = None            # nop removed
                    # A branch INTO the block (the epilogue label sits on the
                    # `addiu sp`) must land on the new first word, whose delay
                    # slot now executes the moved word.  Until 2026-10-03 it
                    # landed on new_start + 1 -- one word PAST the `jr ra` --
                    # so every early exit fell through the stack restore into
                    # the next function; the shipped users compensated with
                    # adjust_branch_offset (GsMapModelingData, IsIdMatrix...).
                    LABEL_OVERRIDES[old_i] = new_start
                new_words.extend(t_new)
                old_i += t_count
                transform_idx += 1
                continue

        offset_map[old_i] = len(new_words)
        new_words.append(words[old_i])
        old_i += 1

    if pre_map is not None:
        mid_labels = dict(LABEL_OVERRIDES)
        final = {}
        LABEL_OVERRIDES.clear()
        for old, mid in pre_map.items():
            if mid is None:
                final[old] = None
                continue
            final[old] = offset_map.get(mid)
            if mid in mid_labels:
                LABEL_OVERRIDES[old] = mid_labels[mid]
            elif old in pre_labels:
                LABEL_OVERRIDES[old] = offset_map.get(pre_labels[old])
        offset_map = final
    return new_words, offset_map


def build_reverse_map(offset_map):
    """Build new_idx -> old_idx reverse mapping."""
    rev = {}
    for old, new in offset_map.items():
        if new is not None:
            rev[new] = old
    return rev


def adjust_branches(words, offset_map):
    """Adjust PC-relative branch offsets after delay slot filling."""
    rev_map = build_reverse_map(offset_map)

    for new_idx in range(len(words)):
        instr = words[new_idx]
        if not is_branch(instr):
            continue

        old_idx = rev_map.get(new_idx)
        if old_idx is None:
            continue

        old_offset = get_branch_offset(instr)
        old_target = old_idx + 1 + old_offset

        # Find new position of old target
        if label_pos(offset_map, old_target) is not None:
            new_target = label_pos(offset_map, old_target)
        else:
            # Find nearest valid target at or after old_target
            new_target = None
            for k in sorted(offset_map.keys()):
                if k >= old_target and offset_map[k] is not None:
                    new_target = offset_map[k]
                    break
            if new_target is None:
                continue

        new_offset = new_target - (new_idx + 1)
        words[new_idx] = set_branch_offset(instr, new_offset)


def adjust_j_addends(words, offset_map, text_relocs, elf_data):
    """Adjust j/jal addends for .text-relative R_MIPS_26 relocations.

    For j/jal instructions with R_MIPS_26 relocations targeting .text,
    the 26-bit field encodes the intra-section target address / 4.
    After delay slot filling, these targets may have moved.
    """
    for reloc in text_relocs:
        r_offset = reloc['offset']  # byte offset in .text
        r_type = reloc['type']
        r_sym_is_text = reloc['sym_is_text']

        if r_type != 4:  # R_MIPS_26 = 4
            continue
        if not r_sym_is_text:
            continue  # Only fix .text-relative jumps

        new_word_idx = r_offset // 4
        if new_word_idx >= len(words):
            continue

        instr = words[new_word_idx]
        if not is_j_type(instr):
            continue

        # The 26-bit field is the old target word address within .text
        old_target_word = get_j_target(instr)

        # Find new position
        if label_pos(offset_map, old_target_word) is not None:
            new_target_word = label_pos(offset_map, old_target_word)
            words[new_word_idx] = set_j_target(instr, new_target_word)


def adjust_data_text_addends(elf_data, sections, offset_map, text_sym_idx):
    """Re-point .text-relative R_MIPS_32 words in DATA sections (jump tables).

    A `switch` jump table lives in .rodata as R_MIPS_32 words against the
    .text section symbol whose in-place addend is the case label's byte
    offset in .text.  Removing nops from .text moves those labels, exactly
    like the branch and j targets adjust_branches / adjust_j_addends fix --
    until 2026-10-03 nothing re-pointed the tables, so every filled object
    with a jump table carried entries 4-8 bytes off (Gslight_*,
    GsLinkObject4, GsSortObject4, getintr): an object that would jump into
    the wrong instruction if linked.  The build only ever shipped retail's
    table, which hid it until the build began checking tables.
    Returns the number of entries rewritten.
    """
    if text_sym_idx is None:
        return 0
    text_idx = sections['.text']['index']
    by_index = {sec['index']: sec for sec in sections.values()}
    n = 0
    for name, sec in sections.items():
        if sec['type'] != 9 or sec['info'] == text_idx:   # SHT_REL, not .rel.text
            continue
        target = by_index.get(sec['info'])
        if target is None or target['type'] == 8:          # NOBITS
            continue
        entsize = sec['entsize'] or 8
        for j in range(sec['size'] // entsize):
            ent_off = sec['offset'] + j * entsize
            r_offset, r_info = struct.unpack_from('<II', elf_data, ent_off)
            if (r_info & 0xff) != 2 or (r_info >> 8) != text_sym_idx:
                continue
            woff = target['offset'] + r_offset
            addend = struct.unpack_from('<I', elf_data, woff)[0]
            if addend % 4:
                continue
            new = label_pos(offset_map, addend // 4)
            if new is None:          # label on a removed word: next survivor
                new = next((offset_map[k] for k in sorted(offset_map)
                            if k >= addend // 4 and offset_map[k] is not None),
                           None)
            if new is not None and new * 4 != addend:
                struct.pack_into('<I', elf_data, woff, new * 4)
                n += 1
    return n


def find_sections(elf_data):
    """Parse ELF to find section information."""
    if elf_data[:4] != b'\x7fELF':
        raise ValueError("Not an ELF file")

    e_shoff = struct.unpack('<I', elf_data[32:36])[0]
    e_shentsize = struct.unpack('<H', elf_data[46:48])[0]
    e_shnum = struct.unpack('<H', elf_data[48:50])[0]
    e_shstrndx = struct.unpack('<H', elf_data[50:52])[0]

    shstrtab_off = e_shoff + e_shstrndx * e_shentsize
    shstrtab_offset = struct.unpack('<I', elf_data[shstrtab_off + 16:shstrtab_off + 20])[0]
    shstrtab_size = struct.unpack('<I', elf_data[shstrtab_off + 20:shstrtab_off + 24])[0]
    shstrtab = elf_data[shstrtab_offset:shstrtab_offset + shstrtab_size]

    sections = {}
    for i in range(e_shnum):
        sh_off = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack('<I', elf_data[sh_off:sh_off + 4])[0]
        sh_type = struct.unpack('<I', elf_data[sh_off + 4:sh_off + 8])[0]
        sh_offset = struct.unpack('<I', elf_data[sh_off + 16:sh_off + 20])[0]
        sh_size = struct.unpack('<I', elf_data[sh_off + 20:sh_off + 24])[0]
        sh_link = struct.unpack('<I', elf_data[sh_off + 24:sh_off + 28])[0]
        sh_info = struct.unpack('<I', elf_data[sh_off + 28:sh_off + 32])[0]
        sh_entsize = struct.unpack('<I', elf_data[sh_off + 36:sh_off + 40])[0]

        name_end = shstrtab.index(b'\x00', sh_name_idx)
        name = shstrtab[sh_name_idx:name_end].decode('ascii')

        sections[name] = {
            'index': i,
            'sh_off': sh_off,
            'type': sh_type,
            'offset': sh_offset,
            'size': sh_size,
            'link': sh_link,
            'info': sh_info,
            'entsize': sh_entsize,
        }

    return sections


def adjust_symbols(elf_data, sections, offset_map, new_word_count):
    """Re-point .text symbols after words were removed from .text.

    process_text() DELETES the nops it folds into delay slots, so every
    symbol that lives past a fold moves earlier in the section and every
    FUNC symbol that contained a fold gets shorter. The section size,
    relocations and branches were already being adjusted; the symbol
    table was not, which left every function symbol after the first fold
    pointing at a stale offset with a stale size. Downstream tools that
    key off st_value (notably strip_leading_instructions' --from-function
    pad detection) then read the wrong function start.

    st_value/st_size are byte quantities within .text; offset_map is
    old_word_index -> new_word_index (or None when the word was removed).
    """
    symtab = sections.get('.symtab')
    if not symtab:
        return 0
    text_idx = sections['.text']['index']
    entsize = symtab['entsize'] or 16
    num_syms = symtab['size'] // entsize
    n_old = len(offset_map)

    def map_word(idx):
        """New index for old word idx, rounding forward over removed words."""
        if idx >= n_old:
            return new_word_count
        for j in range(idx, n_old):
            v = label_pos(offset_map, j)
            if v is not None:
                return v
        return new_word_count

    changed = 0
    for i in range(num_syms):
        sym_off = symtab['offset'] + i * entsize
        st_shndx = struct.unpack('<H', elf_data[sym_off + 14:sym_off + 16])[0]
        if st_shndx != text_idx:
            continue
        st_info = elf_data[sym_off + 12]
        if (st_info & 0xF) == 3:      # the .text section symbol stays at 0
            continue
        st_value = struct.unpack('<I', elf_data[sym_off + 4:sym_off + 8])[0]
        st_size = struct.unpack('<I', elf_data[sym_off + 8:sym_off + 12])[0]
        new_value = map_word(st_value // 4) * 4
        new_size = st_size
        if st_size:
            end = map_word((st_value + st_size) // 4) * 4
            new_size = max(0, end - new_value)
        if new_value != st_value or new_size != st_size:
            struct.pack_into('<I', elf_data, sym_off + 4, new_value)
            struct.pack_into('<I', elf_data, sym_off + 8, new_size)
            changed += 1
    return changed


def _text_symbol_words(elf_data, sections):
    """Word indices at which a non-section .text symbol starts."""
    out = set()
    symtab = sections.get('.symtab')
    if not symtab:
        return out
    text_idx = sections['.text']['index']
    entsize = symtab['entsize'] or 16
    for i in range(symtab['size'] // entsize):
        o = symtab['offset'] + i * entsize
        if struct.unpack('<H', elf_data[o + 14:o + 16])[0] != text_idx:
            continue
        if (elf_data[o + 12] & 0xF) == 3:
            continue
        out.add(struct.unpack('<I', elf_data[o + 4:o + 8])[0] // 4)
    return out


def get_text_sym_idx(elf_data, sections):
    """Find the symbol table index of the .text section symbol."""
    symtab = sections.get('.symtab')
    if not symtab:
        return None

    text_idx = sections['.text']['index']
    entsize = symtab['entsize'] or 16
    num_syms = symtab['size'] // entsize

    for i in range(num_syms):
        sym_off = symtab['offset'] + i * entsize
        st_shndx = struct.unpack('<H', elf_data[sym_off + 14:sym_off + 16])[0]
        st_info = elf_data[sym_off + 12]
        st_type = st_info & 0xF
        # Section symbol (type 3) for .text
        if st_type == 3 and st_shndx == text_idx:
            return i

    return None


def parse_text_relocations(elf_data, sections, text_sym_idx):
    """Parse relocation entries for .text section.

    Returns list of dicts with offset, type, sym_is_text.
    """
    text_idx = sections['.text']['index']
    relocs = []

    for name, sec in sections.items():
        if sec['type'] not in (9, 4):  # SHT_REL, SHT_RELA
            continue
        if sec['info'] != text_idx:
            continue

        entsize = sec['entsize'] or 8
        num_entries = sec['size'] // entsize

        for j in range(num_entries):
            ent_off = sec['offset'] + j * entsize
            r_offset = struct.unpack('<I', elf_data[ent_off:ent_off + 4])[0]
            r_info = struct.unpack('<I', elf_data[ent_off + 4:ent_off + 8])[0]

            r_type = r_info & 0xFF
            r_sym = r_info >> 8

            relocs.append({
                'ent_off': ent_off,
                'offset': r_offset,
                'type': r_type,
                'sym': r_sym,
                'sym_is_text': (r_sym == text_sym_idx) if text_sym_idx is not None else False,
                'section_name': name,
            })

    return relocs


def adjust_relocations(elf_data, relocs, offset_map):
    """Adjust relocation r_offset values after code movement."""
    for reloc in relocs:
        old_byte_offset = reloc['offset']
        old_word_idx = old_byte_offset // 4

        if old_word_idx in offset_map and offset_map[old_word_idx] is not None:
            new_word_idx = offset_map[old_word_idx]
            new_byte_offset = new_word_idx * 4
            struct.pack_into('<I', elf_data, reloc['ent_off'], new_byte_offset)
            reloc['offset'] = new_byte_offset  # Update for later use


def main():
    import argparse
    parser = argparse.ArgumentParser(description='Fill delay slots in GCC-compiled MIPS object files')
    parser.add_argument('input', help='Input .o file')
    parser.add_argument('output', help='Output .o file')
    parser.add_argument('--fill-jalr', action='store_true',
                        help='Fill jalr delay slots (pattern 3)')
    parser.add_argument('--fill-epilogue', action='store_true',
                        help='Fill jr ra epilogue delay slots (pattern 4)')
    parser.add_argument('--fill-j', action='store_true',
                        help='Fill j delay slots with lui+addiu (pattern 5)')
    parser.add_argument('--fill-jal-store', action='store_true',
                        help='Fill jal delay slots with lui $at + sw (pattern 6)')
    parser.add_argument('--fill-prev', action='store_true',
                        help='Sink the instruction before an unfilled j/jal/branch into its delay slot (pattern 8, ASPSX reorder mode)')
    parser.add_argument('--fill-load-delay', action='store_true',
                        help='ASPSX: the nearest legal ALU word of the block (above first, then below) fills a load-delay hazard nop')
    parser.add_argument('--fill-sp-adjust', action='store_true',
                        help='With --fill-load-delay: `addiu sp,sp,N` (frame allocation / stack restore) may also fill a load-delay nop (ASPSX, retail _mk_normsp prologue and epilogue)')
    parser.add_argument('--sink-sp-adjust', action='store_true',
                        help='ASPSX: the prologue `addiu sp,sp,-N` sinks to just after the last load of the entry block, and the epilogue restore rises to just after the last load above it, with or without a hazard nop there (retail StCdInterrupt, _mk_normsp, GsSwapDispBuff, ResetCallback)')
    parser.add_argument('--fill-earliest', action='store_true',
                        help='ASPSX reorder mode: the EARLIEST legal instruction of the basic block sinks into an empty j/jal/jalr/jr/branch delay slot (pattern 9)')
    parser.add_argument('--fill-branch-store', action='store_true',
                        help='Move the store before a conditional branch / j into its delay slot (pattern 7, ASPSX reorder mode)')
    parser.add_argument('--fill-ptr-bump-down', action='store_true',
                        help='ASPSX: an EARLIER `addiu rX,rX,K` sinks into a load-delay nop past the rX-based loads/stores of its block, their displacements rewritten by +K (retail CdSearchFile, dma_execute); the inverse of --ido-fill-ptr-bump')
    parser.add_argument('--fill-load-delay-no-div', action='store_true',
                        help='With --fill-load-delay / --fill-earliest: a div/divu/mfhi/mflo word never fills a load-delay nop (ASPSX schedules the div/rem macro as a unit; retail GsSortBg 0x80099F68)')
    parser.add_argument('--asm-macro-fill', action='store_true',
                        help='Treat INPUT/OUTPUT as compiler .s text and apply only the --fill-load-delay-macro rule (fill_load_delay_macro; gccwrap calls it in-process)')
    parser.add_argument('--fill-trace', action='store_true',
                        help='Print one line per --fill-ptr-bump-down move to stderr (also FILL_TRACE=1)')
    parser.add_argument('--fill-all', action='store_true',
                        help='Enable all delay slot patterns')
    args = parser.parse_args()

    input_path = args.input
    output_path = args.output
    fill_jalr = args.fill_jalr or args.fill_all
    fill_epilogue = args.fill_epilogue or args.fill_all
    fill_j = args.fill_j or args.fill_all
    # NOTE: deliberately NOT folded into --fill-all, so the handful of
    # already-matched files that pass --fill-all keep byte-identical output.
    # Opt in explicitly with --fill-jal-store.
    fill_jal_store = args.fill_jal_store
    fill_branch_store = args.fill_branch_store   # opt-in only, like --fill-jal-store
    fill_prev = args.fill_prev
    fill_earliest = args.fill_earliest
    fill_load_delay = args.fill_load_delay
    fill_sp_adjust = args.fill_sp_adjust     # modifier of --fill-load-delay only
    sink_sp_adjust = args.sink_sp_adjust
    fill_ptr_bump_down = args.fill_ptr_bump_down   # opt-in only, never in --fill-all
    if args.fill_trace:
        FILL_TRACE[0] = True
    if args.fill_load_delay_no_div:
        NO_DIV_FILL[0] = True
    if args.asm_macro_fill:
        with open(args.input) as f:
            _lines = f.read().split('\n')
        _lines, _n = fill_load_delay_macro(_lines, trace=FILL_TRACE[0] or bool(os.environ.get('FILL_TRACE')))
        with open(args.output, 'w') as f:
            f.write('\n'.join(_lines))
        print(f"Macro-level load-delay fills: {_n}")
        return

    with open(input_path, 'rb') as f:
        elf_data = bytearray(f.read())

    sections = find_sections(elf_data)

    if '.text' not in sections:
        print("No .text section found")
        shutil.copy2(input_path, output_path)
        return

    text_sec = sections['.text']
    text_offset = text_sec['offset']
    text_size = text_sec['size']

    # Read instruction words
    old_words = []
    for i in range(0, text_size, 4):
        old_words.append(read_u32(elf_data, text_offset + i))

    # Relocated words and symbol starts (--fill-ptr-bump-down never moves or
    # rewrites a relocated word and never moves a word across a symbol start)
    reloc_words = set()
    pinned_words = set()
    if fill_ptr_bump_down:
        _tsi = get_text_sym_idx(elf_data, sections)
        reloc_words = {r['offset'] // 4 for r in parse_text_relocations(elf_data, sections, _tsi)}
        pinned_words = _text_symbol_words(elf_data, sections)

    # Fill delay slots
    new_words, offset_map = process_text(old_words, fill_jalr=fill_jalr,
                                         fill_epilogue=fill_epilogue,
                                         fill_j=fill_j,
                                         fill_jal_store=fill_jal_store, fill_branch_store=fill_branch_store, fill_prev=fill_prev,
                                         fill_earliest=fill_earliest, fill_load_delay=fill_load_delay,
                                         fill_sp_adjust=fill_sp_adjust,
                                         sink_sp_adjust=sink_sp_adjust,
                                         fill_ptr_bump_down=fill_ptr_bump_down,
                                         reloc_words=reloc_words,
                                         pinned_words=pinned_words)

    if len(new_words) == len(old_words) and not SINK_MOVED[0]:
        print("No delay slots to fill")
        shutil.copy2(input_path, output_path)
        return

    saved = len(old_words) - len(new_words)
    if PTR_BUMP_MOVED[0]:
        print(f"Sank {PTR_BUMP_MOVED[0]} pointer bump(s) into load-delay nops")
    if SINK_MOVED[0]:
        print(f"Sank {SINK_MOVED[0]} sp adjustment(s)")
    print(f"Filled {saved} delay slot(s), removed {saved} nop(s)")
    print(f"Old: {len(old_words)} words ({text_size} bytes), "
          f"New: {len(new_words)} words ({len(new_words) * 4} bytes)")

    # Adjust PC-relative branches
    adjust_branches(new_words, offset_map)

    # Parse and adjust ELF relocations
    text_sym_idx = get_text_sym_idx(elf_data, sections)
    relocs = parse_text_relocations(elf_data, sections, text_sym_idx)
    adjust_relocations(elf_data, relocs, offset_map)

    # Adjust j/jal addends for .text-relative jumps
    adjust_j_addends(new_words, offset_map, relocs, elf_data)

    # Re-point jump-table entries (.text-relative R_MIPS_32 in data sections)
    n_tab = adjust_data_text_addends(elf_data, sections, offset_map, text_sym_idx)
    if n_tab:
        print(f"Re-pointed {n_tab} jump-table entr{'y' if n_tab == 1 else 'ies'}")

    # Write new .text section (pad with nops)
    new_text_bytes = b''.join(struct.pack('<I', w) for w in new_words)
    new_text_padded = new_text_bytes + b'\x00' * (text_size - len(new_text_bytes))
    elf_data[text_offset:text_offset + text_size] = new_text_padded

    # Update section size
    struct.pack_into('<I', elf_data, text_sec['sh_off'] + 20, len(new_text_bytes))

    # Re-point .text symbols (values + sizes) over the removed words
    n_syms_fixed = adjust_symbols(elf_data, sections, offset_map, len(new_words))
    if n_syms_fixed:
        print(f"Adjusted {n_syms_fixed} .text symbol(s) for the removed words")

    with open(output_path, 'wb') as f:
        f.write(elf_data)

    print(f"Written to {output_path}")


if __name__ == '__main__':
    main()
