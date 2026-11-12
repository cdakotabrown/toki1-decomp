#!/usr/bin/env python3
"""Hook 77: IDO52_AS1_REPL_TGT_KEEP -- per-occurrence veto of as1's
"Repl_reg_tgt" peephole (delete a copy whose destination is dead at the end
of the block, renaming the destination's remaining uses to the source).

Idempotent (probes for its own marker); independent of every other applier
(its anchors are func_4142d8's own function head and the body of recomp
label L414438, which no other applier touches).  Run it after the as1.c
regen, with the other as1 appliers.  Part of the post-`make clean` chain
(CLAUDE.md Known Gotchas / UOPT-HOOKS-REFERENCE Hook 77).

    python3 tools/ido-static-recomp/apply_as1_repltgt_hooks.py [--as1 PATH] [--rebuild]

WHY
---
func_4142d8(rec, kind, block) is the peephole `-Wa,-peepdbg` reports as
`>>Repl_reg_tgt (INST n) changed to NOP`.  For a copy record `rD <- rS`
(record +28 = rD, +29 = rS) it

  * gives up if rD is live out of the block (block +48/+52 masks);
  * walks the later records of the block: gives up at a record it may not
    look through (+34 clear) or one that redefines rS; stops at the first
    record that redefines rD (or at the block end);
  * otherwise deletes the copy (f_change_bb(rec, 0, 5, 0)) and renames rD to
    rS in the records it walked (func_4139ec).

When the uses of rD were already renamed by pass I (func_414bf8, "Peepreg"),
the copy is simply deleted.  Retail objects keep such a copy, e.g.
NAME_ENT/func_80140804: `move a2,a3` (the generic arm's `tmp2 = tmp`) whose
spill store retail reads from a3, with the copy itself surviving and pulled
into the preceding `bne` delay slot.  Our as1 deletes it twice: f_redundant
("Rednt", Hook 52 IDO52_AS1_KEEP_DEAD_RELOAD vetoes that one) and then here.

ENV
---
IDO52_AS1_REPL_TGT_KEEP=all|N,M,..|occ:N,..   keep every / the listed
                     0-based candidates.  A candidate is a call that reached
                     the apply point (all of func_4142d8's own give-up tests
                     passed).  One counter per as1 run (= per TU).
IDO52_AS1_REPL_TGT_KEEP_TRACE=1
                     `[AS1-RRT] #N rec=<idx> word=<insn> dst=<rD> src=<rS>
                      -> APPLIED|KEPT` per candidate.
Default OFF: with both variables unset the helper returns 0 and the original
code runs unchanged.

SOUNDNESS
---------
A veto declines an optimisation: the copy stays and the later records keep
reading rD, which still holds rS's value (func_4142d8 only gets here when no
record in between redefines rS or rD).  No instruction is created, deleted
or changed by the hook; it returns func_4142d8's own "not applied" result
(v0 = 0, label L414560), the same exit its give-up tests take.  The kept
copy writes a register that is dead at the block end, so even where the
renamed reads were already done by pass I the program computes exactly the
same values.

Re-offers: a record that was KEPT and is offered again (pass II iterates) is
kept again without a new number, so numbering does not depend on the keep
set.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
AS1_C = os.path.join(HERE, "build", "5.2", "as1.c")

MARKER = "as1_rrt_keep"

HELPERS = r"""/* --- IDO52_AS1_REPL_TGT_KEEP (Hook 77, 2026-10-02): per-occurrence veto
 * of func_4142d8's "Repl_reg_tgt" copy deletion.  See
 * apply_as1_repltgt_hooks.py.  Default OFF. --- */
static int as1_rrt_inited, as1_rrt_mode, as1_rrt_trace, as1_rrt_seq;
static int as1_rrt_list[256], as1_rrt_nlist;
static uint32_t as1_rrt_key[4096][2];
static int as1_rrt_nkey;
static void as1_rrt_init(void) {
    const char *e;
    char *pp;
    if (as1_rrt_inited) return;
    as1_rrt_inited = 1;
    e = getenv("IDO52_AS1_REPL_TGT_KEEP_TRACE");
    as1_rrt_trace = (e != NULL && *e && strcmp(e, "0") != 0);
    e = getenv("IDO52_AS1_REPL_TGT_KEEP");
    if (e == NULL || e[0] == '\0') return;
    if (strcmp(e, "all") == 0) { as1_rrt_mode = 1; return; }
    pp = (char *)e;
    if (strncmp(pp, "occ:", 4) == 0) pp += 4;
    as1_rrt_mode = 2;
    while (*pp != '\0' && as1_rrt_nlist < 256) {
        char *q = pp;
        as1_rrt_list[as1_rrt_nlist++] = (int)strtol(pp, &pp, 0);
        if (pp == q) { as1_rrt_nlist--; break; }
        while (*pp == ',' || *pp == ' ') pp++;
    }
}
/* rec: the copy's record index into the 0x1002f2d0 table (40-byte records);
 * dst/src: record +28 / +29.  Returns 1 to keep the copy. */
static int as1_rrt_keep(uint8_t *mem, uint32_t rec, uint32_t dst, uint32_t src) {
    int i, n, hit = 0;
    uint32_t addr, word;
    as1_rrt_init();
    if (!as1_rrt_mode && !as1_rrt_trace) return 0;
    addr = MEM_U32(0x1002f2d0) + rec * 40u;
    word = MEM_U32(addr);
    for (i = 0; i < as1_rrt_nkey; i++)
        if (as1_rrt_key[i][0] == addr && as1_rrt_key[i][1] == word) {
            if (as1_rrt_trace)
                fprintf(stderr, "[AS1-RRT] re-offer rec=%u word=%08x -> KEPT\n",
                        rec, word);
            return 1;
        }
    n = as1_rrt_seq++;
    if (as1_rrt_mode == 1) hit = 1;
    else if (as1_rrt_mode == 2)
        for (i = 0; i < as1_rrt_nlist; i++)
            if (as1_rrt_list[i] == n) { hit = 1; break; }
    if (hit && as1_rrt_nkey < 4096) {
        as1_rrt_key[as1_rrt_nkey][0] = addr;
        as1_rrt_key[as1_rrt_nkey][1] = word;
        as1_rrt_nkey++;
    }
    if (as1_rrt_trace)
        fprintf(stderr, "[AS1-RRT] #%d rec=%u word=%08x dst=%u src=%u -> %s\n",
                n, rec, word, dst, src, hit ? "KEPT" : "APPLIED");
    return hit;
}
"""

FUNC_HEAD = ("static uint32_t func_4142d8(uint8_t *mem, uint32_t sp, "
             "uint32_t a0, uint32_t a1, uint32_t a2) {\n")
SITE_OLD = """L414438:
if ((int32_t)at == (int32_t)0) {
a1 = zero;
goto L414474;}
"""
SITE_NEW = """L414438:
/* === IDO52_AS1_REPL_TGT_KEEP (Hook 77): every give-up test passed; a keep
 * takes func_4142d8's own "not applied" exit (s0 = rD, t0 = rS). */
if (as1_rrt_keep(mem, MEM_U32(sp + 56), s0, t0)) {
    v0 = zero;
    goto L414560;
}
if ((int32_t)at == (int32_t)0) {
a1 = zero;
goto L414474;}
"""


def rebuild_cmds():
    return [
        ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
         "-o", "build/5.2/as1.o", "build/5.2/as1.c"],
        ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
         "-o", "build/5.2/out/as1.new", "build/5.2/as1.o", "build/5.2/version_info.o",
         "build/5.2/libc_impl_52.o", "-lm"],
    ]


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print("[repltgt hooks] Hook 77 already applied")
        return True
    if s.count(FUNC_HEAD) != 1:
        print(f"[repltgt hooks] ERROR: anchor count {s.count(FUNC_HEAD)}: "
              f"{FUNC_HEAD[:50]!r}")
        return False
    start = s.index(FUNC_HEAD)
    end = s.index("\n}\n", start)
    body = s[start:end]
    if body.count(SITE_OLD) != 1:
        print(f"[repltgt hooks] ERROR: L414438 site not unique in func_4142d8 "
              f"(count {body.count(SITE_OLD)})")
        return False
    body = body.replace(SITE_OLD, SITE_NEW, 1)
    s = s[:start] + HELPERS + "\n" + body + s[end:]
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[repltgt hooks] Hook 77 applied to {path}")
    return True


def main():
    argv = sys.argv[1:]
    path = AS1_C
    if "--as1" in argv:
        path = argv[argv.index("--as1") + 1]
    if not apply(path):
        sys.exit(1)
    if "--rebuild" in argv:
        for cmd in rebuild_cmds():
            r = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
            if r.returncode != 0:
                print(r.stderr[-2000:])
                sys.exit(1)
        os.replace(os.path.join(HERE, "build/5.2/out/as1.new"),
                   os.path.join(HERE, "build/5.2/out/as1"))
        print("[repltgt hooks] as1 rebuilt")


if __name__ == "__main__":
    main()
