#!/usr/bin/env python3
"""Hook 76: IDO52_AS1_PEEPREG_KEEP -- keep a copy-table entry that as1's
pass-I "Peepreg" would kill when a fall-through block inherits the table
from the block before it.

Idempotent (probes for its own marker); independent of every other applier
(its anchors are func_412cc4's own function head and the body of recomp
label L412d4c, which no other applier touches).  Run it after the as1.c
regen, with the other as1 appliers.  Part of the post-`make clean` chain
(CLAUDE.md Known Gotchas / UOPT-HOOKS-REFERENCE Hook 76).

    python3 tools/ido-static-recomp/apply_as1_peepreg_hooks.py [--as1 PATH] [--rebuild]

WHY
---
as1's pass-I driver f_do_passI_opt walks the basic blocks in layout order.
For every record it calls func_414bf8, the per-block register peephole that
`-Wb,-peepdbg` reports as `>>Peepreg (INST n) changed rs A => B`: it keeps a
register-contents table (0x10022240, 8 bytes per register: +0 kind, +6
source register; kind 3 = "this register is a copy of register +6") and
renames a use of a copy to the copy's source (func_413038).

At the start of each block, func_412cc4(block, previous block) decides
whether the table survives:

  * the block's ONLY predecessor is the previous block (pure fall-through)
    and the previous block has records -> the table is INHERITED, except
    that every entry whose source register is live-in at the previous
    block's BRANCH TARGET (f_find_branch_target, live-in mask +36) is killed;
  * otherwise the table is flushed (func_412b40).

So in a compare ladder

      lbu   $3, sel
      move  $2, $3
      beq   $2, 0, $L0
      beq   $2, 1, $L1         # $L1 reads $3
      beq   $2, 2, $L2         # <- not renamed: entry killed at this block

the renames stop at the first compare block after a branch whose target has
$3 live-in.  Retail objects show compares past that point renamed (and the
dead `move` surviving), e.g. main/byouki_kega.

ENV
---
IDO52_AS1_PEEPREG_KEEP=all|N,M,..|occ:N,..   keep every / the listed
                     0-based kill candidates.  A candidate is a NON-EMPTY
                     general-register entry (regs 1..31; kind != 0) that
                     func_412cc4 is about to kill because its source is
                     live-in at the previous block's branch target.  One
                     counter per as1 run (= per TU).
IDO52_AS1_PEEPREG_KEEP_TRACE=1
                     `[AS1-PRK] #N blk=<ord>@<ptr> reg=<r> kind=<k> src=<s>
                      target=<blk> livein=<mask> -> KILLED|KEPT` per
                     candidate (mask bit 31-r = register r); <ord> is the ordinal of
                     the block (func_412cc4 call) in the as1 run.
Default OFF: with the variable unset the helper returns 0 and the original
kill runs.  Floating-point entries (32..63, the second loop) are never
touched.

SOUNDNESS
---------
The entry being kept records a fact established in the previous block: "at
the end of block P, register D holds a copy of register S".  func_412cc4
only inherits the table into a block F whose single predecessor is P, so on
every path into F that fact still holds; whether S is ALSO live-in at P's
taken target says nothing about the value of S or D on the fall-through
edge.  as1 already keeps entries whose source is not live at the target,
so a kept entry produces exactly the transformation as1 performs for any
other inherited entry: func_414bf8 renames a source operand of a later
instruction to a register that holds the identical value.  No instruction
is created, deleted or changed in opcode by the hook; liveness is
recomputed after pass I (func_41ca4c / func_41ce84 run again whenever pass
I reports a change), so the reorganiser and pass II see the renamed code.
Entries are still killed by redefinitions inside F (func_414bf8's own
bookkeeping), exactly as for an inherited entry.

Re-offers: if the same (block record, register) is offered again (pass I
re-run), a kept candidate is kept again WITHOUT a new number, so numbering
does not depend on the keep set.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
AS1_C = os.path.join(HERE, "build", "5.2", "as1.c")

MARKER = "as1_prk_keep"

HELPERS = r"""/* --- IDO52_AS1_PEEPREG_KEEP (Hook 76, 2026-10-02): keep a pass-I copy
 * table entry that func_412cc4 would kill on inheriting the table into a
 * fall-through block.  See apply_as1_peepreg_hooks.py.  Default OFF. --- */
static int as1_prk_inited, as1_prk_mode, as1_prk_trace, as1_prk_seq;
static int as1_prk_list[256], as1_prk_nlist;
static uint32_t as1_prk_lastblk;
static int as1_prk_bbord = -1;
static uint32_t as1_prk_key[4096][2];
static int as1_prk_nkey;
static void as1_prk_init(void) {
    const char *e;
    char *pp;
    if (as1_prk_inited) return;
    as1_prk_inited = 1;
    e = getenv("IDO52_AS1_PEEPREG_KEEP_TRACE");
    as1_prk_trace = (e != NULL && *e && strcmp(e, "0") != 0);
    e = getenv("IDO52_AS1_PEEPREG_KEEP");
    if (e == NULL || e[0] == '\0') return;
    if (strcmp(e, "all") == 0) { as1_prk_mode = 1; return; }
    pp = (char *)e;
    if (strncmp(pp, "occ:", 4) == 0) pp += 4;
    as1_prk_mode = 2;
    while (*pp != '\0' && as1_prk_nlist < 256) {
        char *q = pp;
        as1_prk_list[as1_prk_nlist++] = (int)strtol(pp, &pp, 0);
        if (pp == q) { as1_prk_nlist--; break; }
        while (*pp == ',' || *pp == ' ') pp++;
    }
}
/* Called once per func_412cc4 invocation so block ordinals count blocks. */
static void as1_prk_block(uint32_t blk) {
    as1_prk_init();
    if (blk != as1_prk_lastblk) { as1_prk_lastblk = blk; as1_prk_bbord++; }
}
/* blk: the block inheriting the table; reg: the entry's register (1..31);
 * kind: entry +0; src: entry +6 (live-in at the previous block's branch
 * target); tgt/livein: that target block and its live-in mask (bit 31-r =
 * register r).  Returns 1 to keep the entry. */
static int as1_prk_keep(uint32_t blk, uint32_t reg, uint32_t kind, uint32_t src,
                        uint32_t tgt, uint32_t livein) {
    int i, n, hit = 0;
    if (!as1_prk_mode && !as1_prk_trace) return 0;
    if (kind == 0) return 0;
    for (i = 0; i < as1_prk_nkey; i++)
        if (as1_prk_key[i][0] == blk && as1_prk_key[i][1] == reg) {
            if (as1_prk_trace)
                fprintf(stderr, "[AS1-PRK] re-offer blk=%d@%08x reg=%u -> KEPT\n",
                        as1_prk_bbord, blk, reg);
            return 1;
        }
    n = as1_prk_seq++;
    if (as1_prk_mode == 1) hit = 1;
    else if (as1_prk_mode == 2)
        for (i = 0; i < as1_prk_nlist; i++)
            if (as1_prk_list[i] == n) { hit = 1; break; }
    if (hit && as1_prk_nkey < 4096) {
        as1_prk_key[as1_prk_nkey][0] = blk;
        as1_prk_key[as1_prk_nkey][1] = reg;
        as1_prk_nkey++;
    }
    if (as1_prk_trace)
        fprintf(stderr, "[AS1-PRK] #%d blk=%d@%08x reg=%u kind=%u src=%u "
                "target=%08x livein=%08x -> %s\n",
                n, as1_prk_bbord, blk, reg, kind, src, tgt, livein,
                hit ? "KEPT" : "KILLED");
    return hit;
}
"""

FUNC_HEAD = ("static uint32_t func_412cc4(uint8_t *mem, uint32_t sp, "
             "uint32_t a0, uint32_t a1) {\n")
DECL_OLD = "uint32_t a3 = 0;\n"
DECL_NEW = ("uint32_t a3 = 0;\n"
            "uint32_t as1_prk_blk = a0; /* Hook 76: the inheriting block */\n")
ENTRY_OLD = "L412cc4:\n"
ENTRY_NEW = "L412cc4:\nas1_prk_block(as1_prk_blk); /* Hook 76 */\n"
SITE_OLD = """t5 = t2 & t4;
if ((int32_t)t5 == (int32_t)0) {
//nop;
goto L412d94;}
//nop;
MEM_U32(a0 + 0) = zero;
"""
SITE_NEW = """t5 = t2 & t4;
if ((int32_t)t5 == (int32_t)0) {
//nop;
goto L412d94;}
//nop;
/* === IDO52_AS1_PEEPREG_KEEP (Hook 76): entry a0 (register v1/8) has its
 * source t3 live-in at the previous block's branch target; a keep skips
 * the kill (the fall-through fact still holds). */
if (as1_prk_keep(as1_prk_blk, v1 >> 3, MEM_U32(a0 + 0), t3, v0, t2)) {
    goto L412d94;
}
MEM_U32(a0 + 0) = zero;
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
        print("[peepreg hooks] Hook 76 already applied")
        return True
    if s.count(FUNC_HEAD) != 1:
        print(f"[peepreg hooks] ERROR: anchor count {s.count(FUNC_HEAD)}: "
              f"{FUNC_HEAD[:50]!r}")
        return False
    start = s.index(FUNC_HEAD)
    end = s.index("\n}\n", start)
    body = s[start:end]
    for old in (DECL_OLD, ENTRY_OLD, SITE_OLD):
        if body.count(old) != 1:
            print(f"[peepreg hooks] ERROR: {old.splitlines()[0]!r} not unique in "
                  f"func_412cc4 (count {body.count(old)})")
            return False
    body = body.replace(DECL_OLD, DECL_NEW, 1)
    body = body.replace(ENTRY_OLD, ENTRY_NEW, 1)
    body = body.replace(SITE_OLD, SITE_NEW, 1)
    s = s[:start] + HELPERS + "\n" + body + s[end:]
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[peepreg hooks] Hook 76 applied to {path}")
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
        print("[peepreg hooks] as1 rebuilt")


if __name__ == "__main__":
    main()
