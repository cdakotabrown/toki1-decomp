#!/usr/bin/env python3
"""Hook 69: IDO52_AS1_NO_ADDIU_COMBINE -- veto as1's pass-II merge of two
in-place immediate adds on the same register
(`addiu rX,rX,K1 ; ... ; addiu rX,rX,K2` -> `addiu rX,rX,K1+K2`).

Idempotent (probes for its own marker); independent of every other applier
(its anchors are func_4171dc's own recomp labels L417364 / L417610 and no
other applier touches that routine).  Part of the post-`make clean` chain
(CLAUDE.md Known Gotchas / UOPT-HOOKS-REFERENCE).

    python3 tools/ido-static-recomp/apply_as1_addiucombine_hooks.py [--as1 PATH] [--rebuild]

WHY
---
ugen emits a counter bumped twice as two separate `addu $6,$6,K` (cc -S).
as1's pass-II dispatcher func_419268 hands every `addiu` (opcode 9) to
func_4171dc, which walks BACK from an in-place `addiu rX,rX,K2` (dest ==
src) to the previous in-place `addiu rX,rX,K1` and, unless

  * some record in between READS rX other than as a memory base,
  * the earlier record's +34 "optimisable" byte is clear, or
  * K1+K2 (or any intervening `off(rX)` rebased by K1) leaves the s16 range,

deletes the earlier add (f_change_bb(idx, rX, 3, 0)), rebases every
intervening `off(rX)` access by K1, and rewrites the later add's imm16 to
K1+K2.  Retail's ASPSX-assembled library code has no such peephole:
InitSoundSep keeps `addiu a2,a2,2 ; addiu a2,a2,3` and `addiu a2,2 ;
addiu a2,4` separate and native as1 leaves the body two words short.

ENV
---
IDO52_AS1_NO_ADDIU_COMBINE=all|N,M,..|occ:N,..   veto every / the listed
                         0-based merge sites (counted only where
                         func_4171dc has PROVEN the merge and is about to
                         perform it -- before any record is mutated; one
                         counter per as1 run == per TU)
IDO52_AS1_ADDIU_COMBINE_TRACE=1   `[AS1-ADDIU] #N rX=<reg> k1=<..> k2=<..>
                         -> MERGED|VETOED` per site
Default OFF.  A veto returns 0, func_4171dc's own "no change" exit, so the
two words are exactly what ugen emitted (same sum, performed in two steps;
semantics unchanged).
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
AS1_C = os.path.join(HERE, "build", "5.2", "as1.c")

MARKER = "as1_addc_veto"

HELPERS = r"""/* --- IDO52_AS1_NO_ADDIU_COMBINE (Hook 69, 2026-09-25): veto func_4171dc's
 * `addiu rX,rX,K1 ; .. ; addiu rX,rX,K2` -> `addiu rX,rX,K1+K2` merge.  See
 * apply_as1_addiucombine_hooks.py.  Default OFF. --- */
static int as1_addc_inited, as1_addc_mode, as1_addc_trace, as1_addc_seq;
static int as1_addc_list[128], as1_addc_nlist;
static void as1_addc_init(void) {
    const char *e;
    char *pp;
    if (as1_addc_inited) return;
    as1_addc_inited = 1;
    e = getenv("IDO52_AS1_ADDIU_COMBINE_TRACE");
    as1_addc_trace = (e != NULL && *e && strcmp(e, "0") != 0);
    e = getenv("IDO52_AS1_NO_ADDIU_COMBINE");
    if (e == NULL || e[0] == '\0') return;
    if (strcmp(e, "all") == 0) { as1_addc_mode = 1; return; }
    pp = (char *)e;
    if (strncmp(pp, "occ:", 4) == 0) pp += 4;
    as1_addc_mode = 2;
    while (*pp != '\0' && as1_addc_nlist < 128) {
        char *q = pp;
        as1_addc_list[as1_addc_nlist++] = (int)strtol(pp, &pp, 0);
        if (pp == q) { as1_addc_nlist--; break; }
        while (*pp == ',' || *pp == ' ') pp++;
    }
}
/* Sticky: as1 pass II re-offers a vetoed pair (func_419268 re-walks the
 * block after any change), so a vetoed pair is remembered by (record array
 * of the block, i1, i2, w1, w2) and every re-offer is vetoed WITHOUT taking
 * a new occurrence number -- the numbering is independent of the veto set. */
static uint32_t as1_addc_key[4096][5];
static int as1_addc_nkey;
/* reg: rX; w1: the earlier addiu's word (to be deleted); w2: the later
 * addiu's word (to receive K1+K2); sum: func_4171dc's proven K1+K2;
 * i1/i2: the two records' indices in the block's record array bb.
 * Returns 1 to veto (func_4171dc then reports no change). */
static int as1_addc_veto(uint32_t reg, uint32_t w1, uint32_t w2, uint32_t sum,
                         uint32_t i1, uint32_t i2, uint32_t bb) {
    int i, idx, hit = 0;
    as1_addc_init();
    if (!as1_addc_mode && !as1_addc_trace) return 0;
    for (i = 0; i < as1_addc_nkey; i++)
        if (as1_addc_key[i][0] == bb && as1_addc_key[i][1] == i1 &&
            as1_addc_key[i][2] == i2 && as1_addc_key[i][3] == w1 &&
            as1_addc_key[i][4] == w2) {
            if (as1_addc_trace)
                fprintf(stderr, "[AS1-ADDIU] re-offer rX=%u rec %u+%u -> VETOED\n",
                        reg, i1, i2);
            return 1;
        }
    idx = as1_addc_seq++;
    if (as1_addc_mode == 1) hit = 1;
    else if (as1_addc_mode == 2)
        for (i = 0; i < as1_addc_nlist; i++)
            if (as1_addc_list[i] == idx) { hit = 1; break; }
    if (hit && as1_addc_nkey < 4096) {
        as1_addc_key[as1_addc_nkey][0] = bb;
        as1_addc_key[as1_addc_nkey][1] = i1;
        as1_addc_key[as1_addc_nkey][2] = i2;
        as1_addc_key[as1_addc_nkey][3] = w1;
        as1_addc_key[as1_addc_nkey][4] = w2;
        as1_addc_nkey++;
    }
    if (as1_addc_trace)
        fprintf(stderr, "[AS1-ADDIU] #%d rX=%u k1=%d k2=%d (sum %d) rec %u+%u -> %s\n",
                idx, reg, (int)(int16_t)(w1 & 0xffffu),
                (int)(int16_t)(w2 & 0xffffu), (int)(int16_t)(sum & 0xffffu), i1, i2,
                hit ? "VETOED" : "MERGED");
    return hit;
}
"""

FUNC_HEAD = ("static uint32_t func_4171dc(uint8_t *mem, uint32_t sp, "
             "uint32_t a0, uint32_t a1) {\n")

# Site A: no intervening `off(rX)` access (a0 == 0) -- straight to the merge.
SITE_A_OLD = "L417364:\n"
SITE_A_NEW = """L417364:
/* === IDO52_AS1_NO_ADDIU_COMBINE (Hook 69) site A: merge proven, nothing
 * between to rebase (ra = later addiu record, t2 = earlier, s0 = rX,
 * sp+40 = K1+K2).  Return 0 == func_4171dc's own "no change" exit. */
if ((int32_t)a0 == (int32_t)0 &&
    as1_addc_veto(s0, MEM_U32(t2 + 0), MEM_U32(ra + 0), MEM_U32(sp + 40),
                  MEM_U32(sp + 60), s1, MEM_U32(0x1002f2d0))) {
    v0 = zero;
    goto L417904;
}
"""
# Site B: every intervening `off(rX)` checked in range; about to rebase them.
SITE_B_OLD = "L417610:\n"
SITE_B_NEW = """L417610:
/* === IDO52_AS1_NO_ADDIU_COMBINE (Hook 69) site B: merge proven, the
 * intervening `off(rX)` records are about to be rebased by K1 -- veto before
 * any record is mutated. */
if (as1_addc_veto(s0, MEM_U32(t2 + 0), MEM_U32(ra + 0), MEM_U32(sp + 40),
                  MEM_U32(sp + 60), s1, MEM_U32(0x1002f2d0))) {
    v0 = zero;
    goto L417904;
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
        print("[addiucombine hooks] Hook 69 already applied")
        return True
    if s.count(FUNC_HEAD) != 1:
        print(f"[addiucombine hooks] ERROR: anchor count {s.count(FUNC_HEAD)}: "
              f"{FUNC_HEAD[:50]!r}")
        return False
    start = s.index(FUNC_HEAD)
    end = s.index("\n}\n", start)
    body = s[start:end]
    for old in (SITE_A_OLD, SITE_B_OLD):
        if body.count(old) != 1:
            print(f"[addiucombine hooks] ERROR: {old.strip()} not unique in "
                  f"func_4171dc (count {body.count(old)})")
            return False
    body = body.replace(SITE_A_OLD, SITE_A_NEW, 1)
    body = body.replace(SITE_B_OLD, SITE_B_NEW, 1)
    s = s[:start] + HELPERS + "\n" + body + s[end:]
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[addiucombine hooks] Hook 69 applied to {path}")
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
        print("[addiucombine hooks] as1 rebuilt")


if __name__ == "__main__":
    main()
