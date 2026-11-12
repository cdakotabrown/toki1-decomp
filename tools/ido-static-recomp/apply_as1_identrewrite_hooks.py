#!/usr/bin/env python3
"""Hook 72: IDO52_AS1_KEEP_IDENTICAL_REWRITE -- veto as1's pass-I deletion
of an instruction that rewrites a register with the identical word it
already received earlier in the same basic block.

Idempotent (probes for its own marker); independent of every other applier
(its anchors are func_418fb8's own function head and recomp label L419180,
which no other applier touches).  Run it after the as1.c regen, with the
other as1 appliers.  Part of the post-`make clean` chain (CLAUDE.md Known
Gotchas / UOPT-HOOKS-REFERENCE Hook 72).

    python3 tools/ido-static-recomp/apply_as1_identrewrite_hooks.py [--as1 PATH] [--rebuild]

WHY
---
as1's pass-I driver f_do_passI_opt (func_419268) calls func_418fb8 for every
record of a basic block.  func_418fb8 keeps a per-BB table (0x10022248[reg]
= record index of the register's last write; cleared by func_418f7c at each
block start) and DELETES the current record (f_change_bb(idx, 0, 5, 0)) when
the destination's previous write in the block

  * is the identical instruction word,
  * neither record carries a relocation (+4 == 0),
  * the word is not a load (f_loads_from_memory), and
  * none of its source registers has been rewritten since,

i.e. `sll t8,v0,2 ; ... ; sll t8,v0,2` -> the second is dropped.  ugen emits
both (they are in `cc -S`); retail's library-region assembler (ASPSX) kept
them: _addque2 (libgpu _addque2) recomputes `sll t8,v0,2` in unrolled
copies 2-4 and native as1 leaves the body three words short.

ENV
---
IDO52_AS1_KEEP_IDENTICAL_REWRITE=all|N,M,..|occ:N,..   keep every / the
                     listed 0-based deletion candidates (counted only where
                     func_418fb8 has PROVEN the deletion and is about to call
                     f_change_bb; one counter per as1 run == per TU)
IDO52_AS1_KEEP_IDENTICAL_REWRITE_TRACE=1
                     `[AS1-KIR] #N blk=<ord>@<ptr> rec=<i> prev=<j> reg=<r>
                      word=<insn> -> DELETED|KEPT` per candidate; <ord> is
                     the ordinal of the distinct block records that produced
                     a candidate (not a uopt/as1 bb number), <ptr> the record
Default OFF.  A veto takes func_418fb8's own "not redundant" path (L4191dc),
which records the kept word as the register's latest def -- exactly what as1
does for any non-deletable write.  Nothing is created: the kept word is the
one ugen emitted.

Re-offers: func_419268 re-walks a block after any change, so a kept record
is offered again.  Every kept record is remembered by (block, record index,
word) and its re-offers are kept WITHOUT taking a new occurrence number, so
numbering does not depend on the veto set.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
AS1_C = os.path.join(HERE, "build", "5.2", "as1.c")

MARKER = "as1_kir_veto"

HELPERS = r"""/* --- IDO52_AS1_KEEP_IDENTICAL_REWRITE (Hook 72, 2026-09-26): veto
 * func_418fb8's deletion of a same-BB identical rewrite of a register.  See
 * apply_as1_identrewrite_hooks.py.  Default OFF. --- */
static int as1_kir_inited, as1_kir_mode, as1_kir_trace, as1_kir_seq;
static int as1_kir_list[256], as1_kir_nlist;
static uint32_t as1_kir_lastblk;
static int as1_kir_bbord = -1;
static void as1_kir_init(void) {
    const char *e;
    char *pp;
    if (as1_kir_inited) return;
    as1_kir_inited = 1;
    e = getenv("IDO52_AS1_KEEP_IDENTICAL_REWRITE_TRACE");
    as1_kir_trace = (e != NULL && *e && strcmp(e, "0") != 0);
    e = getenv("IDO52_AS1_KEEP_IDENTICAL_REWRITE");
    if (e == NULL || e[0] == '\0') return;
    if (strcmp(e, "all") == 0) { as1_kir_mode = 1; return; }
    pp = (char *)e;
    if (strncmp(pp, "occ:", 4) == 0) pp += 4;
    as1_kir_mode = 2;
    while (*pp != '\0' && as1_kir_nlist < 256) {
        char *q = pp;
        as1_kir_list[as1_kir_nlist++] = (int)strtol(pp, &pp, 0);
        if (pp == q) { as1_kir_nlist--; break; }
        while (*pp == ',' || *pp == ' ') pp++;
    }
}
static uint32_t as1_kir_key[4096][3];
static int as1_kir_nkey;
/* blk: the block record func_418fb8 was handed (a0); idx: the record about
 * to be deleted; prev: the register's previous def in the block; reg: its
 * destination; w: the (identical) word.  Returns 1 to keep the record. */
static int as1_kir_veto(uint32_t blk, uint32_t idx, uint32_t prev, uint32_t reg,
                        uint32_t w) {
    int i, n, hit = 0;
    as1_kir_init();
    if (!as1_kir_mode && !as1_kir_trace) return 0;
    if (blk != as1_kir_lastblk) { as1_kir_lastblk = blk; as1_kir_bbord++; }
    for (i = 0; i < as1_kir_nkey; i++)
        if (as1_kir_key[i][0] == blk && as1_kir_key[i][1] == idx &&
            as1_kir_key[i][2] == w) {
            if (as1_kir_trace)
                fprintf(stderr, "[AS1-KIR] re-offer blk=%d@%08x rec=%u word=%08x -> KEPT\n",
                        as1_kir_bbord, blk, idx, w);
            return 1;
        }
    n = as1_kir_seq++;
    if (as1_kir_mode == 1) hit = 1;
    else if (as1_kir_mode == 2)
        for (i = 0; i < as1_kir_nlist; i++)
            if (as1_kir_list[i] == n) { hit = 1; break; }
    if (hit && as1_kir_nkey < 4096) {
        as1_kir_key[as1_kir_nkey][0] = blk;
        as1_kir_key[as1_kir_nkey][1] = idx;
        as1_kir_key[as1_kir_nkey][2] = w;
        as1_kir_nkey++;
    }
    if (as1_kir_trace)
        fprintf(stderr, "[AS1-KIR] #%d blk=%d@%08x rec=%u prev=%u reg=%u word=%08x -> %s\n",
                n, as1_kir_bbord, blk, idx, prev, reg, w, hit ? "KEPT" : "DELETED");
    return hit;
}
"""

FUNC_HEAD = ("static uint32_t func_418fb8(uint8_t *mem, uint32_t sp, "
             "uint32_t a0, uint32_t a1) {\n")
DECL_OLD = "uint32_t a3 = 0;\n"
DECL_NEW = ("uint32_t a3 = 0;\n"
            "uint32_t as1_kir_blk = a0; /* Hook 72: the block record */\n")
SITE_OLD = "L419180:\n"
SITE_NEW = """L419180:
/* === IDO52_AS1_KEEP_IDENTICAL_REWRITE (Hook 72): deletion proven
 * (sp+92 = record, a2 = the register's previous def in this BB, v1 = the
 * record, sp+76 = its word).  A veto takes the "not redundant" path. */
if (as1_kir_veto(as1_kir_blk, MEM_U32(sp + 92), a2, MEM_U8(v1 + 28),
                 MEM_U32(sp + 76))) {
    goto L4191dc;
}
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
        print("[identrewrite hooks] Hook 72 already applied")
        return True
    if s.count(FUNC_HEAD) != 1:
        print(f"[identrewrite hooks] ERROR: anchor count {s.count(FUNC_HEAD)}: "
              f"{FUNC_HEAD[:50]!r}")
        return False
    start = s.index(FUNC_HEAD)
    end = s.index("\n}\n", start)
    body = s[start:end]
    for old in (DECL_OLD, SITE_OLD):
        if body.count(old) != 1:
            print(f"[identrewrite hooks] ERROR: {old.strip()} not unique in "
                  f"func_418fb8 (count {body.count(old)})")
            return False
    body = body.replace(DECL_OLD, DECL_NEW, 1)
    body = body.replace(SITE_OLD, SITE_NEW, 1)
    s = s[:start] + HELPERS + "\n" + body + s[end:]
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[identrewrite hooks] Hook 72 applied to {path}")
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
        print("[identrewrite hooks] as1 rebuilt")


if __name__ == "__main__":
    main()
