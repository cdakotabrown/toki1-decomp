"""--mflo-noreorder-nops and --mflo-consumer-fill.

Retail GsSetFlatLight 0x8009CC74..0x8009CC94:
    lw v0,8(s0) ; mflo a0 ; addu v1,v1,a0 ; nop ; mult v0,v0 ;
    mflo a0 ; nop ; nop ; jal SquareRoot0 ; addu a0,v1,a0
for gcc's
    mflo $4 ; lw $2,8($16) ; mult $2,$2 ; addu $3,$3,$4 ; mflo $4 ;
    .set noreorder ; jal ; addu $4,$3,$4 ; .set reorder"""
import unittest

from maspsx import MaspsxProcessor

from .util import strip_comments

JAL = [".set\tnoreorder", ".set\tnomacro", "jal\tfunc_800A4E60",
       "addu\t$4,$3,$4", ".set\tmacro", ".set\treorder"]

SRC = ["mult\t$2,$2", "mflo\t$4", "lw\t$2,8($16)", "mult\t$2,$2",
       "addu\t$3,$3,$4", "mflo\t$4"] + JAL


def insns(lines):
    return [x for x in strip_comments(lines) if x and not x.startswith(".")]


def run(lines, **kw):
    return insns(MaspsxProcessor(list(lines), **kw).process_lines())


class TestMfloNoreorderNops(unittest.TestCase):

    def test_off_is_unchanged(self):
        res = run(["mflo\t$4"] + JAL)
        self.assertEqual(res, ["mflo\t$4", "jal\tfunc_800A4E60", "addu\t$4,$3,$4"])

    def test_two_nops_before_a_filled_jal(self):
        res = run(["mflo\t$4"] + JAL, mflo_noreorder_nops=True)
        self.assertEqual(res, ["mflo\t$4", "nop", "nop", "jal\tfunc_800A4E60",
                               "addu\t$4,$3,$4"])

    def test_one_word_between_takes_one_nop(self):
        # GsSetRefView2 0x8009A6A8 `mflo a0 ; addu a0,a1,a0 ; nop ; jal ; negu`
        res = run(["mflo\t$4", "addu\t$4,$5,$4", ".set\tnoreorder",
                   "jal\tf", "negu\t$16,$16", ".set\treorder"],
                  mflo_noreorder_nops=True)
        self.assertEqual(res, ["mflo\t$4", "addu\t$4,$5,$4", "nop", "jal\tf",
                               "negu\t$16,$16"])

    def test_two_words_between_take_none(self):
        # gte_scale_matrix 0x8009E2DC `mflo v0 ; sra ; sh ; jr ra ; move`
        src = ["mflo\t$2", "sra\t$2,$2,12", "sh\t$2,16($8)", ".set\tnoreorder",
               "j\t$31", "move\t$2,$9", ".set\treorder"]
        self.assertEqual(run(src, mflo_noreorder_nops=True), run(src))

    def test_reorder_mode_branch_takes_none(self):
        # GsSortBg 0x80099EF8 `mfhi t2 ; bgez t2 ; nop`
        src = ["mfhi\t$10", "bgez\t$10,$L5", "addu\t$10,$10,$15", "$L5:"]
        self.assertEqual(run(src, mflo_noreorder_nops=True), run(src))

    def test_label_stops_the_lookback(self):
        src = ["mflo\t$4", "$L9:", ".set\tnoreorder", "jal\tf",
               "addu\t$4,$3,$4", ".set\treorder"]
        self.assertEqual(run(src, mflo_noreorder_nops=True), run(src))


class TestMfloConsumerFill(unittest.TestCase):

    def test_retail_shape(self):
        res = run(SRC, mflo_nops_first=True, mflo_noreorder_nops=True,
                  mflo_consumer_fill=True)
        i = res.index("lw\t$2,8($16)")
        self.assertEqual(res[i:], [
            "lw\t$2,8($16)", "mflo\t$4", "addu\t$3,$3,$4", "nop",
            "mult\t$2,$2", "mflo\t$4", "nop", "nop", "jal\tfunc_800A4E60",
            "addu\t$4,$3,$4"])

    def test_off_keeps_nops_first(self):
        res = run(SRC, mflo_nops_first=True)
        i = res.index("lw\t$2,8($16)")
        self.assertEqual(res[i:i + 6], [
            "lw\t$2,8($16)", "mflo\t$4", "nop", "nop", "mult\t$2,$2",
            "addu\t$3,$3,$4"])

    def test_follower_not_reading_the_register_stays(self):
        # RotMatrixX_C 0x800A3CAC `mflo v1 ; nop ; nop ; mult s0,v0 ;
        # addu a1,a1,a3` (byte-perfect)
        src = ["mult\t$2,$2", "mflo\t$4", "lw\t$2,8($16)", "mult\t$2,$2",
               "addu\t$5,$5,$7", "mflo\t$6"]
        self.assertEqual(run(src, mflo_nops_first=True, mflo_consumer_fill=True),
                         run(src, mflo_nops_first=True))

    def test_follower_writing_a_mult_operand_stays(self):
        src = ["mult\t$2,$2", "mflo\t$4", "lw\t$2,8($16)", "mult\t$2,$3",
               "addu\t$3,$3,$4", "mflo\t$6"]
        self.assertEqual(run(src, mflo_nops_first=True, mflo_consumer_fill=True),
                         run(src, mflo_nops_first=True))


if __name__ == "__main__":
    unittest.main()
