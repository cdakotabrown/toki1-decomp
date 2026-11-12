import unittest

from maspsx import MaspsxProcessor

from .util import strip_comments


class TestLocLoadNop(unittest.TestCase):
    """--loc-load-nop: ASPSX flushes a pending load delay at a SOURCE-LINE
    boundary (the `.loc` cc1 -g emits between statements), as it does at a
    label (retail EigenMatrix 0x800A41C0: six `lh rX,off(sp) ; nop` in a row
    in each switch arm, none in the text epilogue)."""

    LINES = [
        "lh\t$2,16($sp)",
        ".loc\t3 123",
        "LM55:",
        "lh\t$3,18($sp)",
        "lw\t$31,20($sp)",
        "lw\t$16,16($sp)",
        "addu\t$sp,$sp,24",
    ]

    def _run(self, **kw):
        return strip_comments(MaspsxProcessor(list(self.LINES), **kw).process_lines())

    def test_flag_pads_across_loc(self):
        res = [x for x in self._run(loc_load_nop=True) if not x.startswith((".loc", "LM"))]
        self.assertEqual(
            ["lh\t$2,16($sp)", "nop", "lh\t$3,18($sp)",
             "lw\t$31,20($sp)", "lw\t$16,16($sp)", "addu\t$sp,$sp,24"], res)

    def test_default_looks_through_loc(self):
        res = [x for x in self._run() if not x.startswith((".loc", "LM"))]
        self.assertNotIn("nop", res)


if __name__ == "__main__":
    unittest.main()
