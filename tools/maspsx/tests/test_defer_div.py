import unittest

from maspsx import MaspsxProcessor, defer_div_macros

from .util import strip_comments


def _check(tag, rs, rt, signed=True):
    res = [
        ".set\tnoat",
        f"bnez\t{rt},.L_NOT_DIV_BY_ZERO_{tag}",
        "nop",
        "break\t0x7",
        f".L_NOT_DIV_BY_ZERO_{tag}:",
    ]
    if signed:
        res += [
            "addiu\t$at,$zero,-1",
            f"bne\t{rt},$at,.L_DIV_BY_POSITIVE_SIGN_{tag}",
            "lui\t$at,0x8000",
            f"bne\t{rs},$at,.L_DIV_BY_POSITIVE_SIGN_{tag}",
            "nop",
            "break\t0x6",
            f".L_DIV_BY_POSITIVE_SIGN_{tag}:",
        ]
    return res + [".set\tat"]


def _run(lines, **kw):
    mp = MaspsxProcessor(lines, defer_div=True, **kw)
    return [x for x in strip_comments(mp.process_lines()) if x]


class TestDeferDiv(unittest.TestCase):
    def test_check_deferred_to_operand_overwrite(self):
        # the check waits until rs ($3) is overwritten; the mflo until $2
        # is read
        lines = [
            "\tdiv\t$2,$3,$4",
            "\taddu\t$5,$6,$7",
            "\tlw\t$3,0($8)",
            "\taddu\t$9,$2,$0",
        ]
        expected = (
            ["div\t$zero,$3,$4", "addu\t$5,$6,$7"]
            + _check(2, "$3", "$4")
            + ["lw\t$3,0($8)", "mflo\t$2", "addu\t$9,$2,$0"]
        )
        self.assertEqual(expected, _run(lines))

    def test_check_deferred_to_at_use(self):
        # a symbolic store is a `lui $at ; sw` macro: the check (which
        # clobbers $at) is flushed in front of it
        lines = [
            "\tdiv\t$2,$3,$4",
            "\taddu\t$5,$6,$7",
            "\tsw\t$5,D_80131740",
            "\taddu\t$9,$2,$0",
        ]
        expected = (
            ["div\t$zero,$3,$4", "addu\t$5,$6,$7"]
            + _check(2, "$3", "$4")
            + ["sw\t$5,D_80131740", "mflo\t$2", "addu\t$9,$2,$0"]
        )
        self.assertEqual(expected, _run(lines))

    def test_check_and_mflo_deferred_to_block_end(self):
        # a branch ends the block: check first, then the mflo, both before
        # the branch -- never in its delay slot
        lines = [
            "\tdivu\t$2,$3,$4",
            "\taddu\t$5,$6,$7",
            "\tbne\t$5,$0,$L3",
            "\taddu\t$9,$2,$0",
        ]
        expected = (
            ["divu\t$zero,$3,$4", "addu\t$5,$6,$7"]
            + _check(2, "$3", "$4", signed=False)
            + ["mflo\t$2", "bne\t$5,$0,$L3", "nop", "addu\t$9,$2,$0"]
        )
        self.assertEqual(expected, _run(lines))

    def test_label_ends_block(self):
        lines = [
            "\tdivu\t$2,$3,$4",
            "\taddu\t$5,$6,$7",
            "$L5:",
            "\taddu\t$9,$2,$0",
        ]
        expected = (
            ["divu\t$zero,$3,$4", "addu\t$5,$6,$7"]
            + _check(2, "$3", "$4", signed=False)
            + ["mflo\t$2", "$L5:", "addu\t$9,$2,$0"]
        )
        self.assertEqual(expected, _run(lines))

    def test_mfhi_deferred_to_first_reader(self):
        # rem: the mfhi stays pending across unrelated words and lands right
        # before the first reader of rd, after the (earlier) check
        lines = [
            "\trem\t$2,$3,$4",
            "\tlw\t$4,0($8)",
            "\taddu\t$6,$7,$8",
            "\tsll\t$9,$2,2",
        ]
        expected = (
            ["div\t$zero,$3,$4"]
            + _check(1, "$3", "$4")
            + ["lw\t$4,0($8)", "addu\t$6,$7,$8", "mfhi\t$2", "sll\t$9,$2,2"]
        )
        self.assertEqual(expected, _run(lines))

    def test_mflo_before_mult_keeps_hazard_nops(self):
        # the relocated mflo is still an mflo to the hazard handling: a
        # mult may not start within two instructions of it
        lines = [
            "\tdiv\t$3,$3,$4",
            "\tmult\t$3,$5",
        ]
        expected = (
            ["div\t$zero,$3,$4"]
            + _check(1, "$3", "$4")
            + ["mflo\t$3", "nop", "nop", "mult\t$3,$5"]
        )
        self.assertEqual(expected, _run(lines))

    def test_mflo_aliasing_operand_flushes_check_first(self):
        # rd == rs: writing the quotient would corrupt the check's operand
        lines = [
            "\tdiv\t$3,$3,$4",
            "\taddu\t$5,$3,$0",
        ]
        expected = (
            ["div\t$zero,$3,$4"]
            + _check(1, "$3", "$4")
            + ["mflo\t$3", "addu\t$5,$3,$0"]
        )
        self.assertEqual(expected, _run(lines))

    def test_noreorder_region_puts_mfhi_first(self):
        # retail GsSortBg 0x8009A26C: `div ; mfhi v0 ; <check> ; bnez v0`
        lines = [
            "\trem\t$2,$16,$12",
            ".set\tnoreorder",
            ".set\tnomacro",
            "\tbne\t$2,$0,$L43",
            "\tandi\t$2,$18,0xffff",
            ".set\tmacro",
            ".set\treorder",
        ]
        expected = (
            ["div\t$zero,$16,$12", "mfhi\t$2"]
            + _check(2, "$16", "$12")
            + ["bne\t$2,$0,$L43", "andi\t$2,$18,0xffff"]
        )
        self.assertEqual(expected, _run(lines))

    def test_flag_off_unchanged(self):
        lines = [
            "\tdiv\t$2,$3,$4",
            "\taddu\t$5,$6,$7",
            "\tlw\t$3,0($8)",
            "\taddu\t$9,$2,$0",
        ]
        default = MaspsxProcessor(list(lines)).process_lines()
        off = MaspsxProcessor(list(lines), defer_div=False).process_lines()
        self.assertEqual(default, off)
        self.assertEqual(
            ["div\t$zero,$3,$4", "mflo\t$2"], strip_comments(off)[:2]
        )
        default = MaspsxProcessor(list(lines), expand_div=True).process_lines()
        off = MaspsxProcessor(
            list(lines), expand_div=True, defer_div=False
        ).process_lines()
        self.assertEqual(default, off)

    def test_divide_by_zero_register_left_alone(self):
        lines = ["\tdiv\t$zero,$3,$4", "\taddu\t$5,$6,$7"]
        self.assertEqual(
            ["div\t$zero,$3,$4", "addu\t$5,$6,$7"], _run(lines)
        )


class TestDeferDivGas(unittest.TestCase):
    """The ido52-gas spelling (tools/gcc/gccwrap.py --defer-div)."""

    CHECK = [
        "\t.set noreorder",
        "\t.set noat",
        "\tbnez\t$5, .+12",
        "\tnop",
        "\tbreak\t7",
        "\t.set at",
        "\t.set reorder",
    ]

    def test_immediate_branch_macro_reads_mfhi_first(self):
        # retail _spu_setVoiceAttr 0x80096B24:
        #   divu zero,t1,v0 ; mfhi t9 ; bnez v0,+2 ; nop ; break 7 ; beqz t9
        lines = [
            "\tremu\t$13, $10, $5",
            "\tbeq\t$13, 0, $43",
            "\taddu\t$10, $10, $5",
            "$43:",
        ]
        expected = (
            ["\tdivu\t$0, $10, $5", "\tmfhi\t$13"]
            + self.CHECK
            + ["\tbeq\t$13, 0, $43", "\taddu\t$10, $10, $5", "$43:"]
        )
        self.assertEqual(expected, defer_div_macros(lines, gas=True))

    def test_register_branch_puts_check_first(self):
        lines = [
            "\tremu\t$13, $10, $5",
            "\tbgez\t$13, $43",
            "$43:",
        ]
        expected = (
            ["\tdivu\t$0, $10, $5"]
            + self.CHECK
            + ["\tmfhi\t$13", "\tbgez\t$13, $43", "$43:"]
        )
        self.assertEqual(expected, defer_div_macros(lines, gas=True))
