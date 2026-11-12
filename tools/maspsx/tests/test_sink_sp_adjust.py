"""--sink-sp-adjust (tools/ido/fill_gcc_delay_slots.py): the no-hazard
generalisation of --fill-sp-adjust.  ASPSX places the prologue
`addiu sp,sp,-N` just after the last load of the entry block, and the
epilogue restore just after the last load above it, whether or not a hazard
nop sits there.  Retail StCdInterrupt:
    lui v0 ; lw v0,D_80130B64 ; addiu sp,-56 ; li a0,1 ; beq v0,a0 ; sw ra
(gcc emits the allocation first and there is no nop to fill)."""
import os
import sys
import unittest

sys.path.insert(0, os.path.normpath(os.path.join(
    os.path.dirname(os.path.abspath(__file__)), "..", "..", "ido")))
import fill_gcc_delay_slots as F  # noqa: E402
from tests.test_fill_sp_adjust import (  # noqa: E402
    SP, V0, V1, A0, A3, RA, NOP, JR_RA, i_type, addiu, lbu, lh, sb, sw,
    lui, addu)

T1 = 9


def lw(rt, off, base):
    return i_type(0x23, base, rt, off)


def beq(rs, rt, off):
    return i_type(0x04, rs, rt, off)


def run(words, **kw):
    F.LABEL_OVERRIDES.clear()
    return F.process_text(list(words), **kw)


class TestSinkSpAdjust(unittest.TestCase):

    PROLOGUE = [addiu(SP, SP, -56), lui(V0, 0x8013), lw(V0, 0xB64, V0),
                addiu(A0, 0, 1), beq(V0, A0, 1), sw(RA, 48, SP),
                addiu(V0, 0, 5), JR_RA, NOP]

    def test_prologue_sinks_past_the_last_load_without_a_nop(self):
        new, omap = run(self.PROLOGUE, sink_sp_adjust=True)
        self.assertEqual(new[:6], [lui(V0, 0x8013), lw(V0, 0xB64, V0),
                                   addiu(SP, SP, -56), addiu(A0, 0, 1),
                                   beq(V0, A0, 1), sw(RA, 48, SP)])
        self.assertEqual(len(new), len(self.PROLOGUE))
        self.assertEqual(F.SINK_MOVED[0], 1)
        # the FUNC symbol stays on the first word
        self.assertEqual(F.label_pos(omap, 0), 0)
        self.assertEqual(omap[0], 2)

    def test_off_is_unchanged(self):
        new, _ = run(self.PROLOGUE)
        self.assertEqual(new, self.PROLOGUE)
        self.assertEqual(F.SINK_MOVED[0], 0)

    def test_branch_target_survives(self):
        new, omap = run(self.PROLOGUE, sink_sp_adjust=True)
        F.adjust_branches(new, omap)
        k = new.index(beq(V0, A0, 1))
        self.assertEqual(new[k + 1 + 1], addiu(V0, 0, 5))

    def test_never_crosses_a_register_save(self):
        words = [addiu(SP, SP, -24), sw(RA, 16, SP), lui(V1, 0x8013),
                 lw(V1, 0x2DAC, V1), JR_RA, NOP]
        self.assertEqual(run(words, sink_sp_adjust=True)[0], words)

    def test_stops_at_the_first_sp_user(self):
        # func_80089900-like, but with a load before the save: the allocation
        # lands after the last load that precedes the `sw ra`
        words = [addiu(SP, SP, -24), lui(V1, 0x8013), lw(V1, 0x10, V1),
                 sw(RA, 16, SP), lw(V0, 0, V1), JR_RA, NOP]
        self.assertEqual(run(words, sink_sp_adjust=True)[0],
                         [lui(V1, 0x8013), lw(V1, 0x10, V1), addiu(SP, SP, -24),
                          sw(RA, 16, SP), lw(V0, 0, V1), JR_RA, NOP])

    def test_never_passes_an_sp_based_load(self):
        # retail TmdG4 keeps `addiu sp` first above `lw t1,56(sp)`
        words = [addiu(SP, SP, -40), lw(T1, 0x38, SP), lui(V0, 0x8013),
                 lw(V0, 0x1D7C, V0), JR_RA, NOP]
        self.assertEqual(run(words, sink_sp_adjust=True)[0], words)

    def test_no_load_no_move(self):
        words = [addiu(SP, SP, -24), lui(V1, 0x8013), addu(A0, A0, V1),
                 JR_RA, NOP]
        self.assertEqual(run(words, sink_sp_adjust=True)[0], words)

    def test_prologue_replaces_the_hazard_nop(self):
        # the --fill-sp-adjust case (_mk_normsp): the load's nop goes
        words = [addiu(SP, SP, -64), lui(V1, 0x8013), lh(V1, 0x179A, V1),
                 NOP, addu(A0, A0, V1), JR_RA, NOP]
        new, omap = run(words, sink_sp_adjust=True)
        self.assertEqual(new, [lui(V1, 0x8013), lh(V1, 0x179A, V1),
                               addiu(SP, SP, -64), addu(A0, A0, V1), JR_RA, NOP])
        self.assertIsNone(omap[3])

    def test_stays_in_the_entry_block(self):
        # the load after the branch is in another block
        words = [addiu(SP, SP, -24), beq(A0, 0, 1), NOP,
                 lw(V0, 0, A0), JR_RA, NOP]
        self.assertEqual(run(words, sink_sp_adjust=True)[0], words)

    def test_epilogue_rises_over_a_non_sp_word(self):
        words = [lw(RA, 16, SP), addu(V0, A0, A3), addiu(SP, SP, 24),
                 JR_RA, NOP]
        self.assertEqual(run(words, sink_sp_adjust=True)[0],
                         [lw(RA, 16, SP), addiu(SP, SP, 24), addu(V0, A0, A3),
                          JR_RA, NOP])

    def test_epilogue_fills_the_restore_load_delay(self):
        # retail _mk_normsp epilogue: `lbu v0,28(sp) ; addiu sp,64 ;
        # jr ra ; sb v0,33(a3)` (the jr slot is filled later)
        words = [lbu(V0, 28, SP), NOP, sb(V0, 33, A3), addiu(SP, SP, 64),
                 JR_RA, NOP]
        self.assertEqual(run(words, sink_sp_adjust=True)[0],
                         [lbu(V0, 28, SP), addiu(SP, SP, 64), sb(V0, 33, A3),
                          JR_RA, NOP])

    def test_epilogue_never_crosses_a_stack_access(self):
        words = [lw(V0, 0, A0), sw(V0, 20, SP), addiu(SP, SP, 24), JR_RA, NOP]
        self.assertEqual(run(words, sink_sp_adjust=True)[0], words)

    def test_composes_with_fill_load_delay(self):
        # sink first, then the load-delay pass fills the other nop; the
        # FUNC symbol still labels the first word
        words = [addiu(SP, SP, -24), addu(A3, A0, 0), lh(V0, 18, 6), NOP,
                 addu(V1, V1, V0), lui(V1, 0x8013), lw(V1, 0x10, V1),
                 addiu(A0, 0, 1), JR_RA, NOP]
        new, omap = run(words, sink_sp_adjust=True, fill_load_delay=True)
        self.assertEqual(new[:3], [lh(V0, 18, 6), addu(A3, A0, 0),
                                   addu(V1, V1, V0)])
        self.assertEqual(new[5], addiu(SP, SP, -24))
        self.assertEqual(F.label_pos(omap, 0), 0)


if __name__ == "__main__":
    unittest.main()
