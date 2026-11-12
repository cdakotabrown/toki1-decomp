#!/usr/bin/env python3
"""Hook 64: IDO52_AS1_KEEP_WIDE_AND -- veto as1's narrowing of a wide-
immediate `and` (`li at,-N ; and rd,rs,at`) into `andi rd,rs,0xNNNN`.

Idempotent (probes for its own marker); independent of every other applier
(its anchor is func_416ac4's own recomp label L416c00).  Part of the
post-`make clean` chain (CLAUDE.md Known Gotchas / UOPT-HOOKS-REFERENCE).

    python3 tools/ido-static-recomp/apply_as1_wideimm_hooks.py [--as1 PATH] [--rebuild]

WHY
---
ugen emits `x & ~0x20` as the macro `and rd,rs,-33` (cc -S).  as1 expands
it to `li at,-33 ; and rd,rs,at`, and then its pass-I peephole func_416ac4
(dispatched from f_do_passI_opt for SPECIAL funct 0x24 == `and` ONLY; or/xor
have no such routine) rewrites the pair into ONE `andi rd,rs,0xffdf` when

  * the DEMANDED-bits count of rd (the 0x100221b0 per-register table: how
    many low bits rd's consumers read -- 8 for an `sb`) is <= 16, and
  * one source of the `and` is defined by a constant load (func_416304 finds
    the def, func_416964 extracts the constant),

i.e. the upper bits of the result provably do not matter.  The word then
arrives at the andi simplifier func_416ca8 already as an andi, which is why
Hook 29 (IDO52_AS1_KEEP_ANDI, the inverse direction) cannot reach it.

Retail evidence (game region 0x80041000-0x80086000 of SLPS_000.65): 91
`andi rd,rs,0x8000..0xfffe` words feed an `sb` (the narrowed shape), and
only 4 `li at,-N ; and` pairs feed an `sb` -- all four inside
sprite_slot_disp.  Every overlay narrows (~180 sb-fed andi, 0 sb-fed wide).
So retail's as1 normally narrows: this is a PER-OCCURRENCE veto, never a
blanket rule.

ENV
---
IDO52_AS1_KEEP_WIDE_AND=all|N,M,..|occ:N,..   veto every / the listed
                         0-based narrowing sites (counted only where
                         func_416ac4 has proven the rewrite and is about to
                         perform it; one counter per as1 run == per TU)
IDO52_AS1_KEEP_WIDE_AND_TRACE=1   `[AS1-WIDEAND] #N word=.. rd rs imm=..
                         -> andi 0x....[ -> VETOED]` per site
Default OFF.  Returning 0 is func_416ac4's own "no change" exit, so a vetoed
site is exactly the code as1 emits when the proof fails (the and is the same
and; semantics unchanged).
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
AS1_C = os.path.join(HERE, "build", "5.2", "as1.c")

MARKER = "as1_wand_veto"

HELPERS = r"""/* --- IDO52_AS1_KEEP_WIDE_AND (Hook 64, 2026-09-25): veto func_416ac4's
 * `li at,K ; and rd,rs,at` -> `andi rd,rs,K&0xffff` narrowing.  See
 * apply_as1_wideimm_hooks.py.  Default OFF. --- */
static int as1_wand_inited, as1_wand_mode, as1_wand_trace, as1_wand_seq;
static int as1_wand_list[128], as1_wand_nlist;
static void as1_wand_init(void) {
    const char *e;
    char *pp;
    if (as1_wand_inited) return;
    as1_wand_inited = 1;
    e = getenv("IDO52_AS1_KEEP_WIDE_AND_TRACE");
    as1_wand_trace = (e != NULL && *e && strcmp(e, "0") != 0);
    e = getenv("IDO52_AS1_KEEP_WIDE_AND");
    if (e == NULL || e[0] == '\0') return;
    if (strcmp(e, "all") == 0) { as1_wand_mode = 1; return; }
    pp = (char *)e;
    if (strncmp(pp, "occ:", 4) == 0) pp += 4;
    as1_wand_mode = 2;
    while (*pp != '\0' && as1_wand_nlist < 128) {
        char *q = pp;
        as1_wand_list[as1_wand_nlist++] = (int)strtol(pp, &pp, 0);
        if (pp == q) { as1_wand_nlist--; break; }
        while (*pp == ',' || *pp == ' ') pp++;
    }
}
/* word: the `and` being rewritten; rd: its destination; rs: the surviving
 * (non-constant) source; imm: the constant the other source holds.
 * Returns 1 to veto (func_416ac4 then reports no change). */
static int as1_wand_veto(uint32_t word, uint32_t rd, uint32_t rs, uint32_t imm) {
    int i, idx, hit = 0;
    as1_wand_init();
    if (!as1_wand_mode && !as1_wand_trace) return 0;
    idx = as1_wand_seq++;
    if (as1_wand_mode == 1) hit = 1;
    else if (as1_wand_mode == 2)
        for (i = 0; i < as1_wand_nlist; i++)
            if (as1_wand_list[i] == idx) { hit = 1; break; }
    if (as1_wand_trace)
        fprintf(stderr, "[AS1-WIDEAND] #%d word=%08x rd=%u rs=%u imm=%08x (%d)"
                        " -> andi 0x%04x%s\n", idx, word, rd, rs, imm, (int)imm,
                imm & 0xffffu, hit ? " -> VETOED" : "");
    return hit;
}
"""

FUNC_HEAD = "static uint32_t func_416ac4(uint8_t *mem, uint32_t sp, uint32_t a0) {\n"

SITE_OLD = "L416c00:\n"
SITE_NEW = """L416c00:
/* === IDO52_AS1_KEEP_WIDE_AND (Hook 64): the narrowing is proven here
 * (v1 = the `and` record, a2 = surviving source, sp+44 = the constant,
 * sp+48 = rd).  Returning 0 is func_416ac4's own "no change" exit. */
if (as1_wand_veto(MEM_U32(v1 + 0), MEM_U32(sp + 48), a2, MEM_U32(sp + 44))) {
    return 0;
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
        print("[wideimm hooks] Hook 64 already applied")
        return True
    # The function DEFINITION (the prototype line ends in ';').
    for anchor in (FUNC_HEAD,):
        if s.count(anchor) != 1:
            print(f"[wideimm hooks] ERROR: anchor count {s.count(anchor)}: {anchor[:50]!r}")
            return False
    start = s.index(FUNC_HEAD)
    end = s.index("\n}\n", start)
    body = s[start:end]
    if body.count(SITE_OLD) != 1:
        print(f"[wideimm hooks] ERROR: L416c00 not unique in func_416ac4 "
              f"(count {body.count(SITE_OLD)})")
        return False
    body = body.replace(SITE_OLD, SITE_NEW, 1)
    s = s[:start] + HELPERS + "\n" + body + s[end:]
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[wideimm hooks] Hook 64 applied to {path}")
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
        print("[wideimm hooks] as1 rebuilt")


if __name__ == "__main__":
    main()
