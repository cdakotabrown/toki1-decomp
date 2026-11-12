"""--div-mf-with-check (defer_div_macros) and --mflo-div-writer-slot
(carve-out from --mflo-nops-first).  Retail GsSetFlatLight 0x8009CDAC:
`div zero,a0,a1 ; <check> ; mflo a0 ; sll a1,s2,12 ; nop ; mult a1,a3`."""
import unittest

from maspsx import MaspsxProcessor, defer_div_macros

from .util import strip_comments

SRC = ["div\t$4,$4,$5", "sll\t$5,$18,12", "mult\t$5,$7", "mfhi\t$2"]


def insns(lines):
    return [x for x in strip_comments(lines) if x and not x.startswith(".")]


def run(lines, **kw):
    mp = MaspsxProcessor(list(lines), expand_div=True, defer_div=True,
                         mflo_nops_first=True, **kw)
    return insns(mp.process_lines())


class TestDivMfWithCheck(unittest.TestCase):

    def test_default_defers_mflo_to_the_mult(self):
        self.assertEqual(defer_div_macros(SRC), [
            "div\t$zero,$4,$5", "divchk\t$4,$5", "sll\t$5,$18,12",
            "mflo\t$4", "mult\t$5,$7", "mfhi\t$2"])

    def test_overwrite_flush_takes_mflo_when_rd_is_rs(self):
        self.assertEqual(defer_div_macros(SRC, mf_with_check=True), [
            "div\t$zero,$4,$5", "divchk\t$4,$5", "mflo\t$4",
            "sll\t$5,$18,12", "mult\t$5,$7", "mfhi\t$2"])

    def test_rd_not_rs_rt_stays_deferred(self):
        # GsSortBg 0x8009A044: rt overwritten, rd is neither operand
        src = ["div\t$3,$4,$5", "sll\t$5,$18,12", "mult\t$5,$7"]
        self.assertEqual(defer_div_macros(src, mf_with_check=True),
                         defer_div_macros(src))

    def test_at_flush_does_not_take_mflo(self):
        # valiable_init 0x8009903C: rd == rs but the check is flushed by $at
        src = ["div\t$4,$4,$5", "lui\t$at,%hi(sym)", "addu\t$6,$6,$4"]
        out = defer_div_macros(src, mf_with_check=True)
        self.assertEqual(out, defer_div_macros(src))
        self.assertLess(out.index("divchk\t$4,$5"), out.index("mflo\t$4"))
        self.assertEqual(out.index("mflo\t$4"), out.index("addu\t$6,$6,$4") - 1)

    def test_gas_form(self):
        out = defer_div_macros(SRC, gas=True, mf_with_check=True)
        self.assertLess(out.index("mflo\t$4"), out.index("sll\t$5,$18,12"))
        self.assertLess(out.index(".set reorder"), out.index("mflo\t$4"))


class TestMfloDivWriterSlot(unittest.TestCase):

    def test_retail_shape(self):
        res = run(SRC, div_mf_with_check=True, mflo_div_writer_slot=True)
        i = res.index("mflo\t$4")
        self.assertEqual(res[i:i + 5],
                         ["mflo\t$4", "sll\t$5,$18,12", "nop", "mult\t$5,$7", "mfhi\t$2"])

    def test_off_keeps_nops_first(self):
        res = run(SRC, div_mf_with_check=True)
        i = res.index("mflo\t$4")
        self.assertEqual(res[i:i + 4], ["mflo\t$4", "nop", "nop", "sll\t$5,$18,12"])

    def test_plain_mult_mflo_keeps_nops_first(self):
        # RotMatrix_C 0x800A3844: `mflo t0 ; nop ; nop ; negu a2,s0 ;
        # mult s2,a2` -- not a division macro, the carve-out must not fire
        src = ["mult\t$2,$19", "mflo\t$8", "negu\t$6,$16", "mult\t$18,$6"]
        mp = MaspsxProcessor(src, mflo_nops_first=True, mflo_div_writer_slot=True)
        self.assertEqual(insns(mp.process_lines()),
                         ["mult\t$2,$19", "mflo\t$8", "nop", "nop",
                          "negu\t$6,$16", "mult\t$18,$6"])

    def test_non_writer_keeps_nops_first(self):
        src = ["div\t$4,$4,$5", "sll\t$5,$18,12", "mult\t$9,$7"]
        res = run(src, div_mf_with_check=True, mflo_div_writer_slot=True)
        i = res.index("mflo\t$4")
        self.assertEqual(res[i:i + 3], ["mflo\t$4", "nop", "nop"])


if __name__ == "__main__":
    unittest.main()
