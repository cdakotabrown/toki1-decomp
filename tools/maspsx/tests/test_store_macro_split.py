import unittest

from maspsx import MaspsxProcessor

from .util import strip_comments


class TestStoreMacroSplit(unittest.TestCase):
    """The ASPSX store-macro split (--store-macro-split).

    `sw rt,SYM` expands to `lui $at,%hi(SYM) ; sw rt,%lo(SYM)($at)`.  Retail
    emits the `lui $at`, then the whole `la` macro that follows, and only then
    the store.  Derived from getintr words 113-116, confirmed on
    CD_ready words 79-82 and 100-103.
    """

    def test_off_by_default(self):
        lines = [
            "sw	$2,D_800E2DC8",
            "la	$3,D_800EB708",
        ]
        mp = MaspsxProcessor(lines)
        self.assertEqual(
            ["sw	$2,D_800E2DC8", "la	$3,D_800EB708"],
            strip_comments(mp.process_lines()),
        )

    def test_sinks_past_a_la_macro(self):
        """getintr words 113-116."""
        lines = [
            "sw	$2,D_800E2DC8",
            "la	$3,D_800EB708",
            "beq	$3,$0,$L81",
            "addu	$5,$sp,16",
        ]
        mp = MaspsxProcessor(lines, store_macro_split=True)
        self.assertEqual(
            [
                ".set	noat",
                "lui	$at,%hi(D_800E2DC8)",
                ".set	at",
                "la	$3,D_800EB708",
                ".set	noat",
                "sw	$2,%lo(D_800E2DC8)($at)",
                ".set	at",
                "beq	$3,$0,$L81",
                "nop",
                "addu	$5,$sp,16",
            ],
            strip_comments(mp.process_lines()),
        )

    def test_refuses_when_the_next_macro_writes_the_stored_register(self):
        """getintr words 215/216 -- retail does NOT split this one."""
        lines = [
            "sw	$2,D_800E2DD0",
            "lw	$2,D_800E2DD0",
        ]
        mp = MaspsxProcessor(lines, store_macro_split=True)
        res = strip_comments(mp.process_lines())
        self.assertEqual("sw	$2,D_800E2DD0", res[0])

    def test_refuses_to_cross_a_plain_alu_word(self):
        """ivlc_dct_dc_size words 64-66: retail leaves `lui at ; sw a1,2848(at)`
        adjacent even though a plain `addu` follows."""
        lines = [
            "sw	$5,D_80130B20",
            "addu	$4,$4,$2",
            "andi	$2,$4,0x0010",
        ]
        mp = MaspsxProcessor(lines, store_macro_split=True)
        res = strip_comments(mp.process_lines())
        self.assertEqual("sw	$5,D_80130B20", res[0])

    def test_refuses_to_cross_a_branch(self):
        lines = [
            "sw	$5,D_80130B10",
            "beq	$2,$0,$L6",
            "nop",
        ]
        mp = MaspsxProcessor(lines, store_macro_split=True)
        res = strip_comments(mp.process_lines())
        self.assertEqual("sw	$5,D_80130B10", res[0])

    def test_refuses_to_cross_a_label(self):
        lines = [
            "sw	$2,D_800E2DC8",
            "$L81:",
            "la	$3,D_800EB708",
        ]
        mp = MaspsxProcessor(lines, store_macro_split=True)
        res = strip_comments(mp.process_lines())
        self.assertEqual("sw	$2,D_800E2DC8", res[0])

    def test_refuses_when_the_la_writes_the_stored_register(self):
        lines = [
            "sw	$3,D_800E2DC8",
            "la	$3,D_800EB708",
        ]
        mp = MaspsxProcessor(lines, store_macro_split=True)
        res = strip_comments(mp.process_lines())
        self.assertEqual("sw	$3,D_800E2DC8", res[0])

    def test_window_stops_a_second_la(self):
        """Only one two-word macro fits in the three-word window."""
        lines = [
            "sw	$2,D_800E2DC8",
            "la	$3,D_800EB708",
            "la	$4,D_800EB710",
            "beq	$3,$0,$L81",
            "nop",
        ]
        mp = MaspsxProcessor(lines, store_macro_split=True)
        res = strip_comments(mp.process_lines())
        self.assertEqual(
            [
                ".set	noat",
                "lui	$at,%hi(D_800E2DC8)",
                ".set	at",
                "la	$3,D_800EB708",
                ".set	noat",
                "sw	$2,%lo(D_800E2DC8)($at)",
                ".set	at",
                "la	$4,D_800EB710",
                "beq	$3,$0,$L81",
                "nop",
                "nop",
            ],
            res,
        )

    def test_gp_relative_la_is_not_crossed(self):
        """A `la` that collapses to one gp-relative word has an unknown cost."""
        lines = [
            ".sdata",
            "D_sdata_sym:",
            ".word	0",
            ".text",
            "sw	$2,D_800E2DC8",
            "la	$3,D_sdata_sym",
        ]
        mp = MaspsxProcessor(lines, sdata_limit=8, store_macro_split=True)
        res = strip_comments(mp.process_lines())
        self.assertIn("sw	$2,D_800E2DC8", res)


if __name__ == "__main__":
    unittest.main()
