"""gccwrap.py (ido52-gas): ASPSX's shift/add expansion of `mul rd,rs,IMM`.

Every expected sequence is a retail site (addresses in the comments); see
``_mul_const_shift_add`` in tools/gcc/gccwrap.py.
"""
import importlib.util
import os
import unittest

_GW = os.path.join(os.path.dirname(__file__), "..", "..", "gcc", "gccwrap.py")
_spec = importlib.util.spec_from_file_location("gccwrap_for_tests", _GW)
gccwrap = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(gccwrap)


def _expand(line):
    out = gccwrap.ido_expand_pow2_macros(line + "\n")
    return [x.strip() for x in out.split("\n") if x.strip()]


class TestIdoMulConst(unittest.TestCase):
    def test_pow2_unchanged(self):
        self.assertEqual(["sll $15, $14, 3"], _expand("\tmul\t$15, $14, 8"))

    def test_2k_minus_1_aliased_goes_through_at(self):
        # vmNoiseOn 0x8008D0AC: move at,a1 ; sll a1,at,14 ; subu a1,a1,at
        self.assertEqual(
            [".set noat", "addu $at, $5, $0", "sll $5, $at, 14",
             "subu $5, $5, $at", ".set at"],
            _expand("\tmul\t$5, $5, 16383"))

    def test_2k_minus_1_distinct_uses_rs(self):
        # the aliasing copy is only needed when rd == rs
        self.assertEqual(["sll $12, $5, 14", "subu $12, $12, $5"],
                         _expand("\tmul\t$12, $5, 16383"))

    def test_3_is_4_minus_1_then_shift(self):
        # vmNoiseOn 0x8008D31C: x*24 = sll t7,a0,2 ; subu t7,t7,a0 ;
        # (scheduled words) ; sll t7,t7,3
        self.assertEqual(["sll $15, $4, 2", "subu $15, $15, $4",
                          "sll $15, $15, 3"],
                         _expand("\tmul\t$15, $4, 24"))

    def test_60_aliased(self):
        # GetMetaEvent 0x800947F0: move at,v0 ; sll v0,at,4 ; subu v0,v0,at ;
        # sll v0,v0,2
        self.assertEqual(
            [".set noat", "addu $at, $2, $0", "sll $2, $at, 4",
             "subu $2, $2, $at", ".set at", "sll $2, $2, 2"],
            _expand("\tmul\t$2, $2, 60"))

    def test_10_is_5_shifted(self):
        # GetMetaEvent 0x80094708: sll t5,t4,2 ; addu t5,t5,t4 ; sll t5,t5,1
        self.assertEqual(["sll $13, $12, 2", "addu $13, $13, $12",
                          "sll $13, $13, 1"],
                         _expand("\tmul\t$13, $12, 10"))

    def test_168_three_digits(self):
        # GetMetaEvent 0x800948C0: sll 2 ; addu ; sll 2 ; addu ; sll 3
        self.assertEqual(["sll $15, $9, 2", "addu $15, $15, $9",
                          "sll $15, $15, 2", "addu $15, $15, $9",
                          "sll $15, $15, 3"],
                         _expand("\tmul\t$15, $9, 168"))

    def test_600_mixed_signs(self):
        # GetMetaEvent 0x80094748: 75 = 64+16-4-1
        self.assertEqual(["sll $24, $14, 2", "addu $24, $24, $14",
                          "sll $24, $24, 2", "subu $24, $24, $14",
                          "sll $24, $24, 2", "subu $24, $24, $14",
                          "sll $24, $24, 3"],
                         _expand("\tmul\t$24, $14, 600"))

    def test_register_and_negative_operands_left_alone(self):
        self.assertEqual(["mul\t$5, $8, $5"], _expand("\tmul\t$5, $8, $5"))
        self.assertEqual(["mul\t$5, $8, -3"], _expand("\tmul\t$5, $8, -3"))

    def test_naf_evaluates_to_the_constant(self):
        for v in range(1, 4096):
            acc = 0
            for bit, d in gccwrap._naf_digits(v):
                acc += d << bit
            self.assertEqual(v, acc)


if __name__ == "__main__":
    unittest.main()
