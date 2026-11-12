import unittest

from maspsx import MaspsxProcessor

from .util import strip_comments


class TestMacroLuiFillSplit(unittest.TestCase):
    """--macro-lui-fill-split: ASPSX splits a symbol-load macro across two
    load-delay slots -- the `%lo` half sinks into the nearest LATER load
    delay (and, with --macro-lui-fill*, the lui half may fill a PLAIN
    load's delay).  Retail CD_cachefile 0x8008974C, GsSortFastSprite 0x80099BE0,
    _mk_normsp 0x8009E600."""

    def run_mp(self, lines, **kw):
        return strip_comments(MaspsxProcessor(lines, **kw).process_lines())

    def test_off_by_default(self):
        lines = ["lhu	$3,D_80131798", "lhu	$4,D_8013179A", "addu	$5,$5,$4"]
        self.assertEqual(
            ["lhu	$3,D_80131798", "lhu	$4,D_8013179A", "nop", "addu	$5,$5,$4"],
            self.run_mp(lines))

    def test_lo_half_fills_next_macro_delay(self):
        """GsSortFastSprite 0x80099BE0: lui v1 | lui a0 | lhu a0 | lhu v1 | addu."""
        lines = ["lhu	$3,D_80131798", "lhu	$4,D_8013179A",
                 "addu	$5,$5,$4", "addu	$2,$2,$3"]
        self.assertEqual(
            ["lui	$3,%hi(D_80131798)",
             "lhu	$4,D_8013179A",
             "lhu	$3,%lo(D_80131798)($3)",
             "addu	$5,$5,$4",
             "addu	$2,$2,$3"],
            self.run_mp(lines, macro_lui_fill_split=True))

    def test_lo_half_sinks_past_alu_and_loads(self):
        """_mk_normsp words 3-10: the %lo half crosses independent ALU
        words and a load, and lands in the `lh a0,12` delay."""
        lines = ["lh	$5,D_80131798", "addu	$7,$4,$0", "addu	$3,$3,$2",
                 "lh	$2,20($6)", "lh	$4,12($6)", "addu	$4,$4,$2",
                 "addu	$9,$3,$5"]
        self.assertEqual(
            ["lui	$5,%hi(D_80131798)", "addu	$7,$4,$0", "addu	$3,$3,$2",
             "lh	$2,20($6)", "lh	$4,12($6)",
             "lh	$5,%lo(D_80131798)($5)",
             "addu	$4,$4,$2", "addu	$9,$3,$5"],
            self.run_mp(lines, macro_lui_fill_split=True))

    def test_lui_half_fills_plain_load_delay_with_macro_lui_fill(self):
        lines = ["lh	$2,18($6)", "addu	$3,$3,$2", "lh	$5,D_80131798",
                 "lh	$4,12($6)", "addu	$4,$4,$2", "addu	$9,$3,$5"]
        self.assertEqual(
            ["lh	$2,18($6)", "lui	$5,%hi(D_80131798)", "addu	$3,$3,$2",
             "lh	$4,12($6)", "lh	$5,%lo(D_80131798)($5)",
             "addu	$4,$4,$2", "addu	$9,$3,$5"],
            self.run_mp(lines, macro_lui_fill=True, macro_lui_fill_split=True))

    def test_plain_load_delay_needs_the_split_flag(self):
        lines = ["lh	$2,18($6)", "addu	$3,$3,$2", "lh	$5,D_80131798"]
        self.assertEqual(
            ["lh	$2,18($6)", "nop", "addu	$3,$3,$2", "lh	$5,D_80131798"],
            self.run_mp(lines, macro_lui_fill=True))

    def test_no_sink_without_a_later_hazard(self):
        lines = ["lh	$5,D_80131798", "lh	$4,12($6)", "addu	$9,$3,$5"]
        self.assertEqual(lines, self.run_mp(lines, macro_lui_fill_split=True))

    def test_refuses_to_cross_a_store(self):
        lines = ["lh	$5,D_80131798", "sw	$2,0($3)", "lh	$4,12($6)",
                 "addu	$4,$4,$4"]
        res = self.run_mp(lines, macro_lui_fill_split=True)
        self.assertEqual("lh	$5,D_80131798", res[0])

    def test_refuses_to_cross_a_label(self):
        lines = ["lh	$5,D_80131798", "$L4:", "lh	$4,12($6)",
                 "addu	$4,$4,$4"]
        res = self.run_mp(lines, macro_lui_fill_split=True)
        self.assertEqual("lh	$5,D_80131798", res[0])

    def test_refuses_to_cross_a_reader_of_rd(self):
        lines = ["lh	$5,D_80131798", "addu	$9,$3,$5", "lh	$4,12($6)",
                 "addu	$4,$4,$4"]
        res = self.run_mp(lines, macro_lui_fill_split=True)
        self.assertEqual("lh	$5,D_80131798", res[0])

    def test_refuses_slot_whose_consumer_reads_rd(self):
        """The %lo load would itself need a nop before that consumer."""
        lines = ["lh	$5,D_80131798", "lh	$4,12($6)", "addu	$4,$4,$5"]
        res = self.run_mp(lines, macro_lui_fill_split=True)
        self.assertEqual("lh	$5,D_80131798", res[0])

    def test_refuses_to_cross_a_division(self):
        lines = ["lw	$19,D_80131CAC", "div	$0,$2,$15", "lh	$15,8($21)",
                 "sw	$15,104($sp)"]
        res = self.run_mp(lines, macro_lui_fill_split=True)
        self.assertEqual("lw	$19,D_80131CAC", res[0])


if __name__ == "__main__":
    unittest.main()
