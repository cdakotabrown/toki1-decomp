import unittest

from maspsx import MaspsxProcessor

from .util import strip_comments


class TestLoadDestTemp(unittest.TestCase):
    """--load-dest-temp: ASPSX builds a macro load's address in the load's
    own destination register, using $at only when rd is the index."""

    def _run(self, lines, **kw):
        mp = MaspsxProcessor(lines, load_dest_temp=True, **kw)
        return strip_comments(mp.process_lines())

    def test_symbol_dest_temp(self):
        res = self._run(["lw\t$2,sym($5)", "addu\t$3,$3,$3"])
        self.assertEqual(
            ["lui\t$2,%hi(sym)", "addu\t$2,$2,$5", "lw\t$2,%lo(sym)($2)",
             "addu\t$3,$3,$3"], res)

    def test_numeric_dest_temp(self):
        """retail dma_execute: lui v0,0x1f80 ; addu v0,v0,a1 ; lw v0,4232(v0)"""
        res = self._run(["lw\t$2,528486536($5)", "addu\t$3,$3,$3"])
        self.assertEqual(
            ["lui\t$2,%hi(528486536)", "addu\t$2,$2,$5",
             "lw\t$2,%lo(528486536)($2)", "addu\t$3,$3,$3"], res)

    def test_numeric_dest_is_index_uses_at_first(self):
        """retail dma_execute words 49-51: lui at ; addu at,at,a1 ; lw a1,4232(at)"""
        res = self._run(["lw\t$5,528486536($5)", "addu\t$3,$3,$3"])
        self.assertEqual(
            [".set\tnoat", "lui\t$at,%hi(528486536)", "addu\t$at,$at,$5",
             "lw\t$5,%lo(528486536)($at)", ".set\tat", "addu\t$3,$3,$3"], res)

    def test_numeric_in_range_untouched(self):
        res = self._run(["lw\t$2,32767($5)", "addu\t$3,$3,$3"])
        self.assertEqual(["lw\t$2,32767($5)", "addu\t$3,$3,$3"], res)

    def test_numeric_without_flag_keeps_at_form(self):
        mp = MaspsxProcessor(["lw\t$2,528486536($5)", "addu\t$3,$3,$3"])
        res = strip_comments(mp.process_lines())
        self.assertEqual(
            [".set\tnoat", "lui\t$at,%hi(528486536)", "addu\t$at,$5,$at",
             "lw\t$2,%lo(528486536)($at)", ".set\tat", "addu\t$3,$3,$3"], res)


if __name__ == "__main__":
    unittest.main()
