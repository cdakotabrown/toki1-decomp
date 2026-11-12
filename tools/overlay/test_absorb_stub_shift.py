#!/usr/bin/env python3
"""Unit tests for absorb_stub.shift_ambiguous_offsets (TODO-NEXT D5).

When a fake cross-prologue head is absorbed, the compare window grows by the
stub size (`delta`, classically 8 = a 2-word head) at the FRONT, so every
in-window PostProc offset moves. shift_postproc_offsets already handled the
unambiguous forms; this covers the three that need per-directive unit
knowledge:

  --offsets          word indices (convert_ori_to_addiu: +delta/4) vs byte
                     offsets (invert_branch, cse_lui_at, ...: +delta)
  --exclude-offsets  byte offsets (swap_registers / convert_or_to_addu): +delta
  --address          scheduler window START: -delta (symbol starts earlier)

Stack/frame flags (--old/--new/--count/--sp-offset/--base-address) and prose
that merely quotes a directive must be left untouched.

Run:  python3 tools/overlay/test_absorb_stub_shift.py
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import absorb_stub as a

DELTA = 8

CASES = [
    # convert_ori_to_addiu --offsets are WORD indices -> +delta/4
    ('/* @PostProc: convert_ori_to_addiu --offsets 13 */',
     '/* @PostProc: convert_ori_to_addiu --offsets 15 */'),
    ('/* @PostProc: convert_ori_to_addiu --offsets 27,28,46,47 */',
     '/* @PostProc: convert_ori_to_addiu --offsets 29,30,48,49 */'),
    # invert_branch --offsets are BYTE offsets (space-separated) -> +delta
    ('/* @PostProc: invert_branch --offsets 0x10 0x28 */',
     '/* @PostProc: invert_branch --offsets 0x18 0x30 */'),
    # cse_lui_at --offsets BYTE (comma-separated) -> +delta
    ('/* @PostProc: cse_lui_at --offsets 0x70,0x84 */',
     '/* @PostProc: cse_lui_at --offsets 0x78,0x8c */'),
    # trailing --sp-offset (a stack slot) must be left alone
    ('/* @PostProc: strip_lw_ra_from_branch_delays --offsets 0x10,0x20 '
     '--sp-offset 0x18 */',
     '/* @PostProc: strip_lw_ra_from_branch_delays --offsets 0x18,0x28 '
     '--sp-offset 0x18 */'),
    # --exclude-offsets BYTE -> +delta; the swapped registers are untouched
    ('/* @PostProc: swap_registers a0:t6 --unidirectional '
     '--exclude-offsets 0x20,0x3c */',
     '/* @PostProc: swap_registers a0:t6 --unidirectional '
     '--exclude-offsets 0x28,0x44 */'),
    # --address -> -delta, --base-address and --target-bin path untouched
    ('/* @PostProc: scheduler_normalizer --target-bin assets/overlays/DATE.EXN '
     '--address 0x8013521c --base-address 0x80132000 */',
     '/* @PostProc: scheduler_normalizer --target-bin assets/overlays/DATE.EXN '
     '--address 0x80135214 --base-address 0x80132000 */'),
    # frame directives never shift
    ('/* @PostProc: patch_stack_complete --old 176 --new 192 */',
     '/* @PostProc: patch_stack_complete --old 176 --new 192 */'),
    # prose that quotes a directive mid-sentence is NOT a directive line
    (' * fabricated by `@PostProc: invert_branch --offsets 0x10` in prose',
     ' * fabricated by `@PostProc: invert_branch --offsets 0x10` in prose'),
]


def run():
    fails = []
    for inp, exp in CASES:
        got, _notes = a.shift_ambiguous_offsets(inp, DELTA)
        if got == exp:
            print("  ok:", inp.strip()[:70])
        else:
            fails.append((inp, exp, got))
            print("  FAIL:", inp.strip())
            print("     got:", got.strip())
            print("     exp:", exp.strip())

    # An unrecognised --offsets directive is assumed byte and WARNS.
    _g, notes = a.shift_ambiguous_offsets(
        '/* @PostProc: some_future_tool --offsets 0x10 */', DELTA)
    if any('unrecognised' in n for n in notes):
        print("  ok: unknown --offsets directive warns")
    else:
        fails.append(('unknown-warn', 'a WARNING note', notes))
        print("  FAIL: unknown --offsets directive did not warn")

    print()
    if fails:
        print("FAILED (%d)" % len(fails))
        return 1
    print("ALL PASSED")
    return 0


if __name__ == "__main__":
    sys.exit(run())
