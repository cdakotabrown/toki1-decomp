"""--fill-sp-adjust (tools/ido/fill_gcc_delay_slots.py): with
--fill-load-delay, ASPSX also uses `addiu sp,sp,N` as a load-delay filler.
Retail _mk_normsp: prologue `lh v1,%lo(v1) ; addiu sp,sp,-64 ; andi`,
epilogue `lbu v0,28(sp) ; addiu sp,sp,64 ; jr ra ; sb v0,33(a3)`."""
import os
import sys
import unittest

sys.path.insert(0, os.path.normpath(os.path.join(
    os.path.dirname(os.path.abspath(__file__)), "..", "..", "ido")))
import fill_gcc_delay_slots as F  # noqa: E402

SP, V0, V1, A0, A2, A3, RA = 29, 2, 3, 4, 6, 7, 31
NOP = 0


def i_type(op, rs, rt, imm):
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


def addiu(rt, rs, imm):
    return i_type(0x09, rs, rt, imm)


def andi(rt, rs, imm):
    return i_type(0x0C, rs, rt, imm)


def lbu(rt, off, base):
    return i_type(0x24, base, rt, off)


def lh(rt, off, base):
    return i_type(0x21, base, rt, off)


def sb(rt, off, base):
    return i_type(0x28, base, rt, off)


def sw(rt, off, base):
    return i_type(0x2B, base, rt, off)


def lui(rt, imm):
    return i_type(0x0F, 0, rt, imm)


def addu(rd, rs, rt):
    return (rs << 21) | (rt << 16) | (rd << 11) | 0x21


JR_RA = (RA << 21) | 0x08


def run(words, **kw):
    new, _ = F.process_text(list(words), fill_load_delay=True, **kw)
    return new


class TestFillSpAdjust(unittest.TestCase):

    def test_epilogue_restore_fills_sp_based_load_delay(self):
        words = [lbu(V0, 28, SP), NOP, addiu(SP, SP, 64), JR_RA, sb(V0, 33, A3)]
        self.assertEqual(run(words, fill_sp_adjust=True),
                         [lbu(V0, 28, SP), addiu(SP, SP, 64), JR_RA, sb(V0, 33, A3)])

    def test_off_is_unchanged(self):
        words = [lbu(V0, 28, SP), NOP, addiu(SP, SP, 64), JR_RA, sb(V0, 33, A3)]
        self.assertEqual(run(words), words)

    def test_prologue_allocation_sinks_into_load_delay(self):
        # every nearer word depends on the load chain; the frame allocation
        # is the nearest legal filler from above
        words = [addiu(SP, SP, -64), lui(V1, 0x8013),
                 lh(V1, 0x179A, V1), NOP, addu(A0, A0, V1), JR_RA, NOP]
        self.assertEqual(run(words, fill_sp_adjust=True)[:4],
                         [lui(V1, 0x8013), lh(V1, 0x179A, V1),
                          addiu(SP, SP, -64), addu(A0, A0, V1)])

    def test_nearer_filler_wins(self):
        # nearest-first: an independent word closer to the nop beats the
        # allocation (`move a2,a1` is not pinned by the load chain)
        words = [addiu(SP, SP, -64), addu(A2, 5, 0), lui(V1, 0x8013),
                 lh(V1, 0x179A, V1), NOP, addu(A0, A0, V1), JR_RA, NOP]
        self.assertEqual(run(words, fill_sp_adjust=True)[:4],
                         [addiu(SP, SP, -64), lui(V1, 0x8013),
                          lh(V1, 0x179A, V1), addu(A2, 5, 0)])

    def test_allocation_never_passes_an_sp_based_load(self):
        # from ABOVE the load itself reads $sp: moving the allocation below
        # it would change the address
        words = [addiu(SP, SP, -8), lbu(V0, 4, SP), NOP, addu(A0, A0, V0), JR_RA, NOP]
        self.assertEqual(run(words, fill_sp_adjust=True), words)

    def test_allocation_never_crosses_a_stack_access(self):
        words = [addiu(SP, SP, -24), sw(RA, 16, SP), lui(V1, 0x8013),
                 lh(V1, 0x179A, V1), NOP, addu(A0, A0, V1), JR_RA, NOP]
        self.assertEqual(run(words, fill_sp_adjust=True), words)

    def test_other_sp_writers_stay_pinned(self):
        move_sp = addu(SP, 30, 0)          # move sp,fp
        words = [lbu(V0, 28, A3), NOP, move_sp, JR_RA, sb(V0, 33, A3)]
        self.assertEqual(run(words, fill_sp_adjust=True), words)

    def test_symbol_start_stays_on_first_word(self):
        # the FUNC symbol sat on the allocation; after the sink it must label
        # the word that now leads the function (strip --count auto keys on it)
        words = [addiu(SP, SP, -64), lui(V1, 0x8013),
                 lh(V1, 0x179A, V1), NOP, addu(A0, A0, V1), JR_RA, NOP]
        F.LABEL_OVERRIDES.clear()
        new, omap = F.process_text(list(words), fill_load_delay=True,
                                   fill_sp_adjust=True)
        self.assertEqual(F.label_pos(omap, 0), 0)
        self.assertEqual(new[0], lui(V1, 0x8013))


if __name__ == "__main__":
    unittest.main()
