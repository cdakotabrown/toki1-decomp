#!/usr/bin/env python3
"""Unit checks for the paint-audit screens (no compiler, no I/O).

    python3 tools/overlay/test_paint_refusals.py

Covers:
  * audit_paint_bans.screen_paint — the PROVABLE ban classes and the
    CORRECTED branch-rt rule (REGIMM / blez / bgtz only; beq/bne rt is an
    ordinary operand and must stay legal)
  * gen_reg_patches.screen_provable — the same rules at emit time, plus $at
  * gen_reg_patches.pair_symbols / screen_reloc_pairing — the WRONG-GLOBAL
    class: a paint that re-points a %lo consumer at a different lui %hi
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from audit_paint_bans import screen_paint
import gen_reg_patches as G

FAILS = []


def check(name, cond, extra=""):
    print("%-4s %s%s" % ("ok" if cond else "FAIL", name,
                         ("  -- " + extra) if extra and not cond else ""))
    if not cond:
        FAILS.append(name)


def enc_r(funct, rs, rt, rd, sa=0):
    return (rs << 21) | (rt << 16) | (rd << 11) | (sa << 6) | funct


def enc_i(op, rs, rt, imm):
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


R = {n: i for i, n in enumerate(G.REGS)}

# --------------------------------------------------------------- ban classes
verdicts = lambda *a, **k: [v for v, _ in screen_paint(*a, **k)]

check("compiled nop is BANNED",
      verdicts(0x00000000, 'rd', 'zero', 't0') == ['BANNED'])

# bltz $t0, x  (REGIMM, rt = 0 selects bltz)
regimm = enc_i(1, R['t0'], 0, 4)
check("REGIMM rt paint is BANNED", verdicts(regimm, 'rt', 'zero', 't1') == ['BANNED'])
check("REGIMM rs paint is LEGAL", 'BANNED' not in verdicts(regimm, 'rs', 't0', 't1'))

blez = enc_i(6, R['t0'], 0, 4)
check("blez rt paint is BANNED", verdicts(blez, 'rt', 'zero', 't1') == ['BANNED'])
check("blez rs paint is LEGAL", 'BANNED' not in verdicts(blez, 'rs', 't0', 't1'))

# ⭐ the CORRECTED rule: beq/bne rt is an ordinary register operand
bne = enc_i(5, R['t0'], R['t1'], 4)
check("bne rt paint is LEGAL (corrected rule)",
      'BANNED' not in verdicts(bne, 'rt', 't1', 't2'))
check("bne rt paint is reported as a NOTE",
      'NOTE' in verdicts(bne, 'rt', 't1', 't2'))
check("bne zero<->reg is BANNED",
      verdicts(enc_i(5, R['t0'], 0, 4), 'rt', 'zero', 't2') == ['BANNED'])

# stale detection
addu = enc_r(0x21, R['t0'], R['t1'], R['t2'])
check("stale paint detected", 'STALE' in verdicts(addu, 'rs', 't5', 't6'))
check("matching paint is clean", verdicts(addu, 'rs', 't0', 't6') == [])

# --------------------------------------------------------- emit-time screens
words = {0x20: 0x00000000, 0x24: regimm, 0x28: bne, 0x2c: addu}
keep, refused = G.screen_provable(
    ['0x20:rd:zero:t0',      # nop            -> refuse
     '0x24:rt:zero:t1',      # REGIMM rt      -> refuse
     '0x28:rt:t1:t2',        # bne rt         -> keep
     '0x2c:rs:t0:at',        # $at            -> refuse
     '0x2c:rs:t0:t3'],       # ordinary       -> keep
    words)
check("screen_provable keeps exactly the legal pair",
      keep == ['0x28:rt:t1:t2', '0x2c:rs:t0:t3'], repr(keep))
check("screen_provable refuses nop/REGIMM/$at",
      sorted(k for _, k, _ in refused) == ['at', 'nop', 'regimm'],
      repr([(p, k) for p, k, _ in refused]))

# ------------------------------------------------------ %hi/%lo re-pairing
#   0: lui   $t0, %hi(A)
#   1: lui   $t1, %hi(B)
#   2: sw    $v0, %lo(A)($t0)
#   3: sw    $v1, %lo(B)($t1)
lui = 0x0f
words_l = [enc_i(lui, 0, R['t0'], 0),
           enc_i(lui, 0, R['t1'], 0),
           enc_i(0x2b, R['t0'], R['v0'], 0),
           enc_i(0x2b, R['t1'], R['v1'], 0)]
rel = {0x0: ('R_MIPS_HI16', 'D_A'), 0x4: ('R_MIPS_HI16', 'D_B'),
       0x8: ('R_MIPS_LO16', 'D_A'), 0xc: ('R_MIPS_LO16', 'D_B')}
pairs = G.pair_symbols(words_l, rel)
check("pair_symbols pairs each %lo with its own %hi",
      pairs[0x8][1] == 'D_A' and pairs[0xc][1] == 'D_B', repr(pairs))

# the wrong-global paint: aim the D_A store at D_B's lui
crossed = G.apply_patches_inmem(words_l, ['0x8:rs:t0:t1'])
p2 = G.pair_symbols(crossed, rel)
check("crossing a %lo onto another symbol's %hi is visible",
      p2[0x8][1] == 'D_B' and p2[0x8][2] == 'D_A', repr(p2))

# a WHOLE-pair rename (both lui and consumer) must NOT be flagged
renamed = G.apply_patches_inmem(words_l, ['0x0:rt:t0:t7', '0x8:rs:t0:t7'])
p3 = G.pair_symbols(renamed, rel)
check("consistent whole-pair rename keeps the symbol",
      p3[0x8][1] == 'D_A', repr(p3))

print()
if FAILS:
    print("FAIL: %d check(s) failed: %s" % (len(FAILS), ", ".join(FAILS)))
    sys.exit(1)
print("PASS")
