#!/usr/bin/env python3
"""Per-paint BAN screen for `patch_insn_reg_field` layers.

    python3 tools/overlay/audit_paint_bans.py DATE func_80134CF4

Audits every `patch_insn_reg_field` entry against the object produced by the
REAL PostProc chain with the paint steps removed, so offsets line up after any
count-changing (allowed) step such as `cse_lui_at`.  Reports:

  * BANNED-CLASS violations  — provably unsound, strip on sight
  * stale/mismatched paints  — the stated OLD register is not what the
    compiler emitted there (a silent no-op, or an offset authored against a
    different object)
  * legal branch-operand paints — informational; see the CORRECTED rule below

=============================================================================
THE BANNED CLASSES (PROVABLE — safe to refuse automatically)
=============================================================================
1. PAINT ON A COMPILED NOP.  A nop IS `sll $zero,$zero,0`; writing a register
   field into it FABRICATES an instruction the compiler never emitted.  This
   is the CLAUDE.md fabrication ban, and it fakes a ✅.
2. rt-FIELD PAINT ON REGIMM (op 1: bltz/bgez/bltzal/bgezal).  There the rt
   field SELECTS the condition — painting it is a branch POLARITY FLIP, i.e.
   a change of operation, i.e. banned painting.
3. rt-FIELD PAINT ON blez/bgtz (op 6/7).  Those encodings require rt == 0;
   any rt paint corrupts the instruction.
4. zero <-> register PAINT ON ANY WORD.  `$zero` is the constant 0, not an
   allocation: repointing a read off `$zero` asserts the target register holds
   0 there.  On a branch it changes the comparison; on a store it changes the
   value stored.  Both are banned.
   (`beq $x,$0` vs `beq $x,$y`), not just its register allocation.
5. OFFSET OUTSIDE THE BODY.  Overlay PostProc offsets are `.text`-relative
   and the body starts at +0x10 (two types.h `jr ra; nop` thunks).  A layer
   authored BODY-relative is uniformly 0x10 low and dies at the first paint
   ("field rt is 29, expected 14" = the prologue's $sp).

⭐ CORRECTED BRANCH-rt RULE (2026-08-09).  The rt ban applies ONLY to
REGIMM (op 1) and blez/bgtz (op 6/7).  **For beq/bne (op 4/5) the rt field is
an ORDINARY REGISTER OPERAND and painting it is legal** — it is a normal
register-allocation paint.  The earlier "no rt paint on any branch word"
screen was over-broad and produced false bans; those are now reported in the
informational "legal branch-operand paints" section instead.
Painting `rs` on a REGIMM/blez/bgtz word is likewise LEGAL — that field is a
real register operand.

=============================================================================
THE $at RULE  (ADVISORY — direction matters; `--check-at` to report it)
=============================================================================
`$at` is the assembler temp: it is created by macro expansion (la/li/symbolic
load-store), never by this C.

  * `X -> at` on a READ is garbage on sight: the read is repointed at a
    register no instruction in this function defines.  Treat as unsound.
  * `at -> X` (the MIRROR direction) can be legitimate when a def+use pair of
    an assembler-expanded macro is renamed TOGETHER and the result is
    byte-identical to the target (RENSYU/func_801335E0's lui/sh pairs;
    SHUGAKU/func_80136060's verified upstream repair).

Because of that asymmetry the $at screen is NOT part of the default ban set
here — but `gen_reg_patches.py` refuses to EMIT any $at paint, since a
generator has no way to establish the mirror-direction argument.

=============================================================================
OTHER FALSE-POSITIVE / BLIND-SPOT MODES
=============================================================================
  * A "stale" verdict is often a CompileEnv coupling, not a bad paint:
    `cse_lui_at` offsets are CompileEnv-coupled and the step silently no-ops
    without the env, shifting every later offset by the 2 words it should
    have removed.  Check `@CompileEnv` before rewriting offsets.
  * This screen sees only the paint's own word.  It cannot see producer
    changes (`audit_paint_isomorphism.py`), wrong-symbol reloc re-pairing
    (`gen_reg_patches.py --audit-relocs`), `swap_registers` live-range
    collapses, or instruction-count fabrication (`audit_raw_count.py`).
"""
import argparse
import os
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "overlay"))
sys.path.insert(0, str(ROOT / "tools" / "build"))
import resolve_overlay_relocs as R
from parse_postproc import parse_compile_flags, parse_compile_env, parse_compiler
from overlay_postproc import parse_postproc, apply_postproc

REGS = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
        't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
        's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
        't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra']
BRANCH_OPS = {1, 4, 5, 6, 7}          # regimm, beq, bne, blez, bgtz
BRANCH_NAME = {1: 'regimm', 4: 'beq', 5: 'bne', 6: 'blez', 7: 'bgtz'}
FIELD_SHIFT = {'rs': 21, 'rt': 16, 'rd': 11}
IDO_CC = ROOT / "tools/ido-static-recomp/build/5.2/out/cc"
FLAGS = ["-O2", "-G0", "-mips1", "-EL", "-non_shared",
         f"-I{ROOT/'include'}", f"-I{ROOT/'include'/'psyq'}"]


def screen_paint(word, field, old, new, check_at=False):
    """Screen ONE paint against the word it lands on.

    Returns a list of (verdict, reason) pairs — a paint can be both a NOTE
    and a STALE.  Verdicts:
      'BANNED' — provably unsound (see the module docstring); terminal
      'STALE'  — the compiled word does not hold `old` in `field`
      'NOTE'   — legal, but worth reporting (beq/bne rt operand paint, $at)
    An empty list means an ordinary, clean register-allocation paint.

    `word` must come from the object built with the paint steps REMOVED but
    every other PostProc step applied.
    """
    out = []
    op = (word >> 26) & 0x3F
    if word == 0:
        return [('BANNED', "paint on compiled NOP (fabrication)")]
    if op in BRANCH_OPS:
        # REGIMM (op=1): rt SELECTS the condition (bltz/bgez/bltzal/bgezal)
        # -> an rt paint there IS a polarity flip.  blez/bgtz (6/7) require
        # rt==0, so any rt paint corrupts the encoding.  For beq/bne (4/5)
        # rt is a genuine register OPERAND and painting it is an ordinary
        # register-allocation paint.
        if field == 'rt' and op == 1:
            return [('BANNED', "REGIMM rt paint = POLARITY FLIP")]
        if field == 'rt' and op in (6, 7):
            return [('BANNED', "rt paint on blez/bgtz (rt must be 0)")]
        if 'zero' in (old, new):
            return [('BANNED', "zero<->reg paint on BRANCH word")]
        out.append(('NOTE', "register-operand paint on %s (legal class)"
                    % BRANCH_NAME[op]))
    if field not in FIELD_SHIFT:
        return [('BANNED', "unknown field %s" % field)]
    # 4b. zero <-> register paint on ANY word, not just a branch.  `$zero` is
    # not a register allocation -- it is the constant 0.  Repointing a READ off
    # `$zero` onto $rX asserts that $rX holds 0 there, and repointing a read
    # ONTO `$zero` asserts the opposite; neither is something a
    # register-allocation paint can establish.  Caught in the wild on
    # 2026-08-23: a gen_reg_patches run offered
    # `0x48:rt:zero:a0` / `0x4c:rt:zero:a0` on TT/func_8014C5E8, turning two
    # `sb $zero` stores into `sb $a0` -- and $a0 there still held the incoming
    # `u8 *src` parameter, because the `move $a0,$zero` retail emits is a word
    # our compile does not have at all.  It scored +2 and was byte-nonsense.
    # (`audit_paint_isomorphism` does NOT catch it: $zero has no producer and
    # $a0's is the function-entry value, which it classes as provable.)
    # A WRITE (rd, or rt on a load) is a different matter -- retargeting a
    # destination to/from $zero changes whether the result is discarded -- so
    # it is banned too.
    if 'zero' in (old, new):
        return [('BANNED', "zero<->reg paint (%s is the constant 0, not an "
                           "allocation)" % ('old' if old == 'zero' else 'new'))]
    cur = REGS[(word >> FIELD_SHIFT[field]) & 0x1F]
    if cur != old:
        out.append(('STALE', "compiled has $%s, paint claims $%s" % (cur, old)))
    if check_at and 'at' in (old, new):
        out.append(('NOTE', "read repointed AT $at (unsound)" if new == 'at'
                    else "paint off $at (mirror direction — argue it)"))
    return out


def collect_paints(content):
    """Parse every `off:field:old:new` triple out of the paint directives."""
    paints = []
    for m in re.finditer(
            r'patch_insn_reg_field\s+([^*\n]+(?:\n\s*\*\s*[0-9a-fx][^*\n]*)*)',
            content):
        for tok in m.group(1).split():
            if tok.count(':') == 3:
                off, fld, old, new = tok.split(':')
                try:
                    paints.append((int(off, 16), fld, old, new))
                except ValueError:
                    pass
    return paints


def build_unpainted_body(ov, fn, content, addr, size):
    """Compile with the paint steps commented out, run the rest of the chain,
    link name-resolved, and return the body words (or None)."""
    kept = []
    for ln in content.splitlines():
        if 'patch_insn_reg_field' in ln:
            op = ln.lstrip().startswith("/*")
            cl = ln.rstrip().endswith("*/")
            if op and cl:      continue
            elif op:           kept.append("/*")
            elif cl:           kept.append(" */")
            else:              kept.append(" *")
        else:
            kept.append(ln)
    nopaint = "\n".join(kept) + "\n"
    tmpdir = Path(tempfile.mkdtemp())
    np_src = tmpdir / f"{fn}.c"
    np_src.write_text(nopaint)

    steps = parse_postproc(np_src)
    env = os.environ.copy()
    env.update(parse_compile_env(np_src) or {})
    extra = parse_compile_flags(np_src)

    with tempfile.TemporaryDirectory() as td:
        td = Path(td)
        env["TMPDIR"] = str(td)
        obj = td / f"{fn}.o"   # stem = identity for the restricted-PostProc gate
        r = subprocess.run([str(IDO_CC)] + FLAGS + extra +
                           ["-c", str(np_src), "-o", str(obj)],
                           capture_output=True, text=True, env=env, cwd=str(td))
        if r.returncode != 0:
            print("COMPILE FAILED", r.stderr[:300])
            return None
        if steps:
            asm_file = ROOT / "asm/overlay" / ov / f"{fn}.s"
            ok = apply_postproc(obj, steps, addr, ov, verbose=False,
                                target_asm=asm_file if asm_file.exists() else None)
            if not ok:
                print("!! PostProc chain (minus paints) FAILED")
        linked = R.link_resolved(obj, addr, size, td, rodata_base=0, func_name=fn,
                                 overlay_name=ov)
        if linked is None:
            print("LINK FAILED")
            return None
        pre = linked[0]
    return [struct.unpack('<I', pre[i * 4:i * 4 + 4])[0]
            for i in range(len(pre) // 4)]


def main():
    ap = argparse.ArgumentParser(
        description="Per-paint ban screen for a patch_insn_reg_field layer "
                    "(PROVABLE classes only — see the module docstring).",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="exit 0 = clean, 1 = banned/stale present, 2 = build error")
    ap.add_argument("overlay")
    ap.add_argument("func")
    ap.add_argument("--check-at", action="store_true",
                    help="also report $at paints (ADVISORY: the mirror "
                         "direction can be legitimate — see docstring)")
    args = ap.parse_args()
    ov, fn = args.overlay, args.func

    src = ROOT / "src/overlay" / ov / f"{fn}.c"
    content = src.read_text()

    addr = size = None
    for ln in (ROOT / "config/overlay" / f"{ov}.sym").read_text().splitlines():
        p = ln.strip().split(":")
        if len(p) >= 3 and p[2].strip() == fn:
            addr, size = int(p[0], 16), int(p[1], 0)
    m = re.search(r"@SymbolSize:\s*(\d+)", content)
    if m:
        size = int(m.group(1))

    paints = collect_paints(content)
    words = build_unpainted_body(ov, fn, content, addr, size)
    if words is None:
        return 2

    # Paint offsets are relative to the object the paint step actually sees.
    # If the chain strips the leading types.h thunks BEFORE the paints, that
    # object already starts at the body and the offsets are body-relative;
    # subtracting the thunk prefix again indexes past the end and every paint
    # reports "offset OUTSIDE body".  Only bias by the prefix when nothing
    # stripped it.  (Found 2026-09-17: this reported a false BANNED CLASS
    # PRESENT on every file carrying strip_leading_instructions.)
    body_base = 0 if re.search(r"@PostProc:\s*strip_leading_instructions",
                               content) else 0x10

    print(f"{ov}/{fn}: {len(paints)} paints, pre-paint body {len(words)} words "
          f"(target {size//4})")

    bad, stale, note = [], [], []
    for off, fld, old, new in paints:
        idx = (off - body_base) // 4
        if idx < 0 or idx >= len(words):
            bad.append((off, fld, old, new, "offset OUTSIDE body"))
            continue
        for verdict, reason in screen_paint(words[idx], fld, old, new,
                                            check_at=args.check_at):
            if verdict == 'BANNED':
                bad.append((off, fld, old, new, reason))
            elif verdict == 'STALE':
                stale.append((off, fld, old, new, reason))
            elif verdict == 'NOTE':
                note.append((off, fld, old, new, reason))

    print(f"  BANNED-CLASS violations: {len(bad)}")
    for b in bad:
        print(f"    0x{b[0]:x}:{b[1]}:{b[2]}:{b[3]}  <-- {b[4]}")
    print(f"  stale/mismatched paints: {len(stale)}")
    for s in stale[:12]:
        print(f"    0x{s[0]:x}:{s[1]}:{s[2]}:{s[3]}  ({s[4]})")
    print(f"  legal branch-operand paints (beq/bne rt): {len(note)}")
    for n in note[:12]:
        print(f"    0x{n[0]:x}:{n[1]}:{n[2]}:{n[3]}  ({n[4]})")
    print("  VERDICT:", "CLEAN (all paints provable-class)" if not bad and not stale
          else ("BANNED CLASS PRESENT" if bad else "stale paints present"))
    return 1 if (bad or stale) else 0


if __name__ == "__main__":
    sys.exit(main())
