import unittest
from collections import Counter

from maspsx import MaspsxProcessor
from maspsx.ksched import schedule_lines, expand, build_dag, Node

from .util import strip_comments


def _instrs(lines):
    out = []
    for x in strip_comments(lines):
        if not x or x.startswith(".") or x.endswith(":"):
            continue
        out.append(x)
    return out


class TestKonamiLibSched(unittest.TestCase):
    """--konami-lib-sched: IDO-as1-style list scheduling of each reorder-mode
    basic block, after the address macros are split into their machine words.
    It only REORDERS: every output word is an input word or one word of an
    input macro's expansion."""

    # GsSortClear (0x800A2BD0's caller), cc1 2.6.0 -O2 text: the prologue
    # sinks to the call, the `lui $at` of a store macro rises above the
    # index computation (retail 0x80099...).
    BLOCK = [
        "lh\t$2,PSDIDX",
        "subu\t$sp,$sp,24",
        "sw\t$31,16($sp)",
        "sll\t$2,$2,4",
        "sb\t$4,D_800EBD40+4($2)",
        "lw\t$4,16($7)",
        ".set\tnoreorder",
        ".set\tnomacro",
        "jal\tAddPrim",
        "addu\t$5,$5,$2",
        ".set\tmacro",
        ".set\treorder",
    ]

    def test_macro_expansion_is_word_exact(self):
        ex = expand("la\t$5,D_80131CF0")
        self.assertEqual(["lui\t$5,%hi(D_80131CF0)", "addiu\t$5,$5,%lo(D_80131CF0)"],
                         [t for t, _ in ex])
        ex = expand("sw\t$3,D_800E2DF4+4")
        self.assertEqual(["lui\t$at,%hi(D_800E2DF4+4)", "sw\t$3,%lo(D_800E2DF4+4)($at)"],
                         [t for t, _ in ex])
        ex = expand("lw\t$2,D_800EA0CC($2)")   # rd is the index: $at
        self.assertEqual("lui\t$at,%hi(D_800EA0CC)", ex[0][0])
        ex = expand("lhu\t$2,D_80131790($4)")  # dest-temp
        self.assertEqual(["lui\t$2,%hi(D_80131790)", "addu\t$2,$2,$4",
                          "lhu\t$2,%lo(D_80131790)($2)"], [t for t, _ in ex])

    def test_only_reorders(self):
        out = schedule_lines(list(self.BLOCK))
        want = []
        for ln in self.BLOCK:
            if ln.startswith("."):
                continue
            ex = expand(ln)
            want += [t for t, _ in ex] if ex else [ln]
        got = [x for x in out if not x.startswith(".") and x != "nop"]
        self.assertEqual(Counter(want), Counter(got))

    def test_fixed_group_untouched(self):
        out = schedule_lines(list(self.BLOCK))
        i = out.index("jal\tAddPrim")
        self.assertEqual(".set\tnoreorder", out[i - 2])
        self.assertEqual("addu\t$5,$5,$2", out[i + 1])

    def test_deterministic(self):
        a = schedule_lines(list(self.BLOCK))
        b = schedule_lines(list(self.BLOCK))
        self.assertEqual(a, b)

    def test_flag_off_is_identity(self):
        lines = ["lw\t$2,0($4)", "addu\t$3,$3,1", "sw\t$3,4($4)"]
        off = MaspsxProcessor(list(lines)).process_lines()
        self.assertEqual(off, MaspsxProcessor(list(lines), konami_lib_sched=False).process_lines())

    def test_store_lui_rises_above_index(self):
        out = _instrs(MaspsxProcessor(list(self.BLOCK), konami_lib_sched=True,
                                      load_dest_temp=True).process_lines())
        ilui = out.index("lui\t$at,%hi(D_800EBD40+4)")
        isll = out.index("sll\t$2,$2,4")
        self.assertLess(ilui, isll)

    def test_epilogue_restores(self):
        # a reorder-mode text epilogue is one block; the restores stay in front
        # of the stack release and `j $31` takes the last word as its slot
        lines = ["lw\t$31,16($sp)", "sltu\t$2,$2,1", "addu\t$sp,$sp,24", "j\t$31"]
        out = [x for x in schedule_lines(list(lines)) if not x.startswith(".")]
        self.assertEqual("lw\t$31,16($sp)", out[0])
        self.assertEqual("j\t$31", out[-2])

    # ---- the as1 reorganizer model (2026-10-04 v2) ----------------------

    @staticmethod
    def _body(lines):
        return [x for x in schedule_lines(list(lines)) if not x.startswith(".")]

    def test_pointer_bump_rebases_a_floated_load(self):
        # as1 func_42bc70: an access based on a bumped register may float
        # above the bump with its displacement rebased (CdSearchFile retail
        # `lb v0,1(s0)` for cc1's `addiu s0,s0,1 ; lb v0,0(s0)`)
        out = self._body(["addu\t$16,$16,1", "lbu\t$2,0($16)", "sltu\t$3,$2,10",
                          "bne\t$3,$0,$L1"])
        self.assertEqual(["lbu\t$2,1($16)", "addu\t$16,$16,1"], out[:2])
        # an access already in front of the bump keeps its displacement
        out = self._body(["lbu\t$2,0($16)", "addu\t$16,$16,1", "sll\t$2,$2,2",
                          "sw\t$2,0($17)"])
        self.assertIn("lbu\t$2,0($16)", out)

    def test_division_is_expanded_as1_style(self):
        # the divide, ONE trap-check node and the mflo; independent work goes
        # in front of the check, which lands next to the quotient
        # (GsSetRefView 0x8009ECBC: div ; lw ; lw ; nop ; subu ; lw ; <check> ;
        # mflo)
        out = self._body(["sll\t$16,$17,12", "div\t$16,$16,$18", "lw\t$2,12($19)",
                          "lw\t$3,0($19)", "subu\t$2,$2,$3", "lw\t$3,8($19)",
                          "mult\t$2,$2", "mflo\t$5", "addu\t$4,$5,$16", "jal\tf"])
        i = out.index("div\t$zero,$16,$18")
        self.assertEqual(["lw\t$2,12($19)", "lw\t$3,0($19)", "nop", "subu\t$2,$2,$3",
                          "lw\t$3,8($19)", "divchk\t$16,$18", "mflo\t$16"],
                         out[i + 1:i + 8])

    # GsSetRefView2 0x8009A604 (cc1 2.6.0 -O2 text): the first divide, then
    # a sum of squares whose first mult must wait for the quotient's mflo
    QUOT_MULT = ["lw\t$3,20($sp)", "lw\t$2,32($sp)", "subu\t$17,$3,$2",
                 "sll\t$16,$17,12", "div\t$16,$16,$18", "lw\t$2,28($sp)",
                 "lw\t$3,16($sp)", "subu\t$2,$2,$3", "mult\t$2,$2",
                 "lw\t$2,36($sp)", "lw\t$3,24($sp)", "mflo\t$5",
                 "subu\t$2,$2,$3", "mult\t$2,$2", "mflo\t$4", "addu\t$4,$5,$4",
                 ".set\tnoreorder", ".set\tnomacro", "jal\tSquareRoot0",
                 "subu\t$16,$0,$16", ".set\tmacro", ".set\treorder"]

    def test_mult_behind_a_quotient_is_timed_from_the_divide(self):
        # retail 0x8009A690: `mflo s0 ; mult v0,v0 ; mflo a1 ; lw v0,0x24` --
        # the mult held behind the divide's mflo counts its latency from the
        # divide, so its own mflo is not "late" against the ready lw
        out = self._body(self.QUOT_MULT)
        i = out.index("mflo\t$16")
        self.assertEqual(["mflo\t$16", "mult\t$2,$2", "mflo\t$5", "lw\t$2,36($sp)"],
                         out[i:i + 4])

    def test_second_divide_check_and_quotient_before_the_shifts(self):
        # retail 0x8009A754: `div a2 ; negu a1 ; <check> ; mflo a2 ; sll a1 ;
        # sra a1` -- the second divide waits for the first quotient, so its
        # check and mflo are timed from the first divide
        out = self._body(["lw\t$3,28($sp)", "lw\t$2,16($sp)", "subu\t$17,$3,$2",
                          "sll\t$5,$17,12", "div\t$5,$5,$18", "lw\t$3,36($sp)",
                          "lw\t$2,24($sp)", "subu\t$17,$3,$2", "sll\t$6,$17,12",
                          "div\t$6,$6,$18", "move\t$4,$19", "li\t$7,0x00000079",
                          "subu\t$5,$0,$5", "sll\t$5,$5,16", "sra\t$5,$5,16",
                          "sll\t$6,$6,16", ".set\tnoreorder", ".set\tnomacro",
                          "jal\tfunc_8009E860", "sra\t$6,$6,16", ".set\tmacro",
                          ".set\treorder"])
        i = out.index("div\t$zero,$6,$18")
        self.assertEqual(["div\t$zero,$6,$18", "subu\t$5,$0,$5", "divchk\t$6,$18",
                          "mflo\t$6", "sll\t$5,$5,16", "sra\t$5,$5,16"], out[i:i + 6])

    def test_at_reuse_only_for_initialised_data(self):
        # bss (D_80131CB0.t[], GsSetRefView 0x8009EF3C): one lui per store;
        # initialised data (0x800E2Dxx): the live lui is reused
        bss = self._body(["lw\t$2,0($4)", "lw\t$3,4($4)",
                          "sw\t$2,D_80131CB0+24", "sw\t$3,D_80131CB0+28"])
        self.assertEqual(2, sum(1 for x in bss if x.startswith("lui\t$at")))
        data = self._body(["lw\t$2,0($4)", "lw\t$3,4($4)",
                           "sw\t$2,D_800E2DF4", "sw\t$3,D_800E2DF4+4"])
        self.assertEqual(1, sum(1 for x in data if x.startswith("lui\t$at")))

    def test_block_copy_groups_reverse(self):
        # newly ready successors are pushed on the ready list's head in as1's
        # successor order, so a register-ready group comes out reversed
        # (GsSetRefView2 0x8009A584: lw a0,0x14 | lw v1,0x10 | lw v0,0xc)
        out = self._body(["la\t$5,D_80131720",
                          "lw\t$2,0($5)", "lw\t$3,4($5)", "lw\t$4,8($5)",
                          "sw\t$2,0($20)", "sw\t$3,4($20)", "sw\t$4,8($20)",
                          "lw\t$2,12($5)", "lw\t$3,16($5)", "lw\t$4,20($5)",
                          "sw\t$2,12($20)", "sw\t$3,16($20)", "sw\t$4,20($20)"])
        self.assertEqual(["lw\t$4,20($5)", "lw\t$3,16($5)", "lw\t$2,12($5)",
                          "sw\t$4,20($20)", "sw\t$3,16($20)", "sw\t$2,12($20)"], out[-6:])

    def test_latency_tie_break_does_not_see_into_a_noreorder_group(self):
        # GsMapModelingData 0x8009D234: `ori` and the floated `lw a3` tie on
        # aftercycles; the load's only long edge reads into cc1's filled
        # blez group (a branch operand), which the tie-break ignores
        out = self._body(["ori\t$2,$3,0x0001", "sw\t$2,0($4)", "addu\t$4,$4,4",
                          "move\t$6,$0", "lw\t$7,0($4)",
                          ".set\tnoreorder", ".set\tnomacro",
                          "blez\t$7,$L1", "addu\t$4,$4,4",
                          ".set\tmacro", ".set\treorder"])
        self.assertEqual(["ori\t$2,$3,0x0001", "lw\t$7,4($4)", "sw\t$2,0($4)",
                          "addu\t$4,$4,4", "move\t$6,$0", "blez\t$7,$L1"], out[:6])
        # ... but a group instruction's memory BASE keeps the load latency
        # (func_800B4430: `lw v0` feeds the jal slot's `sw $0,0(v0)`)
        out = self._body(["lw\t$2,D_800E8D34", "la\t$5,D_800B44D0", "li\t$4,3",
                          ".set\tnoreorder", ".set\tnomacro",
                          "jal\tInterruptCallback", "sw\t$0,0($2)",
                          ".set\tmacro", ".set\treorder"])
        self.assertEqual(["lui\t$2,%hi(D_800E8D34)", "lw\t$2,%lo(D_800E8D34)($2)",
                          "lui\t$5,%hi(D_800B44D0)"], out[:3])

    def test_successor_lists_are_sorted_like_as1(self):
        # as1 func_42ae44: successors sorted by aftercycles + latency, a new
        # edge in front of equal keys (insertion follows as1's handler order)
        nodes = []
        for t in ["lw\t$2,0($4)", "addu\t$3,$2,1", "sw\t$3,0($5)", "sll\t$6,$2,2"]:
            (txt, d), = expand(t)
            nd = Node()
            nd.text, nd.op = txt, txt.split()[0]
            nd.defs, nd.uses, nd.mem, nd.kind = d["defs"], d["uses"], d["mem"], d["kind"]
            nodes.append(nd)
        succ, ac, size, bumps, reloff = build_dag(nodes, None, False)
        keys = [ac[e.node] + e.lat for e in succ[0]]
        self.assertEqual(sorted(keys), keys)
        self.assertEqual({1, 2, 3}, {e.node for e in succ[0]})


if __name__ == "__main__":
    unittest.main()
