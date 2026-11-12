#!/usr/bin/env python3
"""Hook 71: IDO52_UGEN_NO_MOVE_NOALIAS -- per-occurrence veto of the
`.noalias src,dst` hint ugen's block-move expansion emits around a struct
copy between two FIXED addresses in different areas.

Idempotent (probes for its own marker); independent of the other ugen
appliers (its helper anchors on f_eval_mov's own definition, its site on the
recomp label L428df4 inside it; Hook 66 patches L428bb8 in the same function
and the two commute).  Part of the post-`make clean` chain (CLAUDE.md Known
Gotchas / UOPT-HOOKS-REFERENCE Hook 71).

    python3 tools/ido-static-recomp/apply_ugen_movenoalias_hooks.py [--ugen PATH] [--rebuild]

THE DECISION (ugen f_eval_mov, recomp labels L428df4..L428e58)
--------------------------------------------------------------
Before the unrolled lw/sw tail, f_eval_mov tests both address trees of the
move: if both are LDA nodes (opcode 0x47) and their area/block words
(+36) differ, it sets a flag at sp+242 and calls

    f_emit_alias(0x30 /* .noalias */, dst_reg, src_reg);

and at L429310, after the copy, `f_emit_alias(0x31 /* .alias */, ...)`
re-arms the dependence when the flag is set.  With the hint as1 is free to
hoist the source loads over the destination stores (`lw t2,4 ; lw at,8 ;
sw t2,4 ; sw at,8`); without it as1 keeps the memory order and fills the
load delays with nops (`lw t2,4 ; nop ; sw t2,4 ; lw at,8 ; nop ; sw at,8`).

The hook replaces the `src_blk == dst_blk` skip test with
`src_blk == dst_blk || veto(occ)`: a vetoed occurrence takes EXACTLY the
path the stock compiler takes for a same-area move -- neither the
.noalias nor the closing .alias is emitted (the flag stays 0).  Nothing
else changes; every instruction is still ugen's and as1's own.  Soundness:
removing a no-alias hint only adds a dependence edge to as1's scheduler.

ENV
---
IDO52_UGEN_NO_MOVE_NOALIAS=all | occ:N[,N...] | N[,N...]
    Occurrences count, from 0, the block moves in the TU that WOULD emit the
    hint (both addresses LDA, different areas), in ugen emission order.
IDO52_UGEN_NO_MOVE_NOALIAS_TRACE=1
    `[UGEN-MOVENOALIAS] occ=<n> size=<bytes> dst=r<d> src=r<s> -> VETO|KEEP`
Default OFF: with the env unset the stock hint is emitted everywhere.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UGEN_C = os.path.join(HERE, "build", "5.2", "ugen.c")

MARKER = "ugen_mnoalias_veto"

HELPER_ANCHOR = ("static void f_eval_mov(uint8_t *mem, uint32_t sp, "
                 "uint32_t a0) {\nconst")

HELPERS = r"""/* --- IDO52_UGEN_NO_MOVE_NOALIAS (Hook 71, 2026-09-26): per-occurrence
 * veto of the `.noalias` hint f_eval_mov (L428df4) emits around a block
 * move between two LDA addresses of different areas.  See
 * apply_ugen_movenoalias_hooks.py.  Default OFF. --- */
static int ugen_mnoalias_inited, ugen_mnoalias_all, ugen_mnoalias_trace;
static int ugen_mnoalias_occ;
static int ugen_mnoalias_list[256], ugen_mnoalias_n;

static int ugen_mnoalias_veto(uint32_t size, uint32_t dreg, uint32_t sreg) {
    int occ, v = 0, i;
    if (!ugen_mnoalias_inited) {
        const char *e = getenv("IDO52_UGEN_NO_MOVE_NOALIAS");
        const char *t = getenv("IDO52_UGEN_NO_MOVE_NOALIAS_TRACE");
        ugen_mnoalias_inited = 1;
        if (e != NULL && *e) {
            if (strcmp(e, "all") == 0) {
                ugen_mnoalias_all = 1;
            } else {
                const char *p = e;
                if (strncmp(p, "occ:", 4) == 0) p += 4;
                while (*p && ugen_mnoalias_n < 256) {
                    char *end;
                    long n = strtol(p, &end, 10);
                    if (end == p) {
                        fprintf(stderr, "[UGEN-MOVENOALIAS] bad list %s\n", e);
                        break;
                    }
                    ugen_mnoalias_list[ugen_mnoalias_n++] = (int)n;
                    p = end;
                    if (*p == ',') p++;
                }
            }
        }
        ugen_mnoalias_trace = (t != NULL && *t && strcmp(t, "0") != 0);
    }
    occ = ugen_mnoalias_occ++;
    if (ugen_mnoalias_all) v = 1;
    for (i = 0; !v && i < ugen_mnoalias_n; i++)
        if (ugen_mnoalias_list[i] == occ) v = 1;
    if (ugen_mnoalias_trace)
        fprintf(stderr, "[UGEN-MOVENOALIAS] occ=%d size=%u dst=r%u src=r%u -> %s\n",
                occ, size, dreg, sreg, v ? "VETO" : "KEEP");
    return v;
}

"""

SITE_OLD = ("t1 = MEM_U32(t0 + 36);\n"
            "t2 = MEM_U32(t6 + 36);\n"
            "t5 = 0x1;\n"
            "if ((int32_t)t1 == (int32_t)t2) {\n"
            "a0 = 0x30;\n"
            "goto L428e58;}\n")
SITE_NEW = ("t1 = MEM_U32(t0 + 36);\n"
            "t2 = MEM_U32(t6 + 36);\n"
            "t5 = 0x1;\n"
            "/* === IDO52_UGEN_NO_MOVE_NOALIAS (Hook 71): a veto takes the\n"
            " * same-area path -- no .noalias, flag sp+242 stays 0 so no .alias */\n"
            "if ((int32_t)t1 == (int32_t)t2 ||\n"
            "    ugen_mnoalias_veto(s1, MEM_U8(sp + 252), MEM_U8(sp + 253))) {\n"
            "a0 = 0x30;\n"
            "goto L428e58;}\n")

REBUILD = [
    ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/ugen.o", "build/5.2/ugen.c"],
    ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/out/ugen.new", "build/5.2/ugen.o", "build/5.2/version_info.o",
     "build/5.2/libc_impl_52.o", "-lm"],
]


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print(f"[ugen movenoalias hooks] Hook 71 already applied to {path}")
        return True
    for needle in (HELPER_ANCHOR, SITE_OLD):
        if s.count(needle) != 1:
            print(f"[ugen movenoalias hooks] ERROR: anchor not unique/found: "
                  f"{needle[:48]!r} (count {s.count(needle)})")
            return False
    start = s.index(HELPER_ANCHOR)
    nxt = s.find("\nstatic void f_", start + 1)
    end = nxt if nxt != -1 else len(s)
    site = s.index(SITE_OLD)
    if not (start < site < end) or s.find("L428df4:", start) > site:
        print("[ugen movenoalias hooks] ERROR: site is not after L428df4 "
              "inside f_eval_mov")
        return False
    s = s.replace(SITE_OLD, SITE_NEW, 1)
    s = s.replace(HELPER_ANCHOR, HELPERS + HELPER_ANCHOR, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[ugen movenoalias hooks] Hook 71 applied to {path}")
    return True


def main():
    argv = sys.argv[1:]
    path = UGEN_C
    if "--ugen" in argv:
        path = argv[argv.index("--ugen") + 1]
    if not apply(path):
        sys.exit(1)
    if "--rebuild" in argv:
        for cmd in REBUILD:
            r = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
            if r.returncode != 0:
                print(r.stderr[-2000:])
                sys.exit(1)
        os.replace(os.path.join(HERE, "build/5.2/out/ugen.new"),
                   os.path.join(HERE, "build/5.2/out/ugen"))
        print("[ugen movenoalias hooks] ugen rebuilt")


if __name__ == "__main__":
    main()
