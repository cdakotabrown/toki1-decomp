#!/usr/bin/env python3
"""apply_uopt_blocksplit_hooks.py -- Hook 59: IDO52_NO_BLOCK_SPLIT /
IDO52_BLOCK_SPLIT_LIMIT, control of uopt's basic-block CAPACITY split
(2026-09-24).

Run on build/5.2/uopt.c (independent of the other appliers; any order):

    python3 apply_uopt_blocksplit_hooks.py [build/5.2/uopt.c] [--rebuild]

MECHANISM

uopt reads ucode one basic block at a time (func_459944, the per-block
reader called from f_oneproc): it loops f_readnxtinst / f_getop until the
end-of-block flag 0x100135d4 is set, then runs f_constarith and
f_codeimage over the block.  Besides the natural block enders (labels,
branches, calls -- the op bitsets tested at L43aa2c / L459dc0), f_getop
ends the block on CAPACITY at L43ab50:

    if (!pending_expr (0x1001e628) && stack_empty (0x1001c444 == 0x1001c448))
        endflag = !(varref_count (0x1001e668) < varref_limit (0x1001e674));

varref_count is bumped by f_readnxtinst for each variable reference read
(L43e8c4 / L43eaa0; a `p->c = code` store is two, a `p->uv = hdr->uv0.word`
copy is two), reset to 0 at every block start (L459d08), and the limit is
copied there from 0x1001e670 -- which is uopt's ORIGINAL `-varref N`
option (f_processargs L43726c; default 0x14 = 20 at L437280).  So a
straight-line region with more than ~20 variable references is cut into
two uopt blocks at the first statement boundary past the limit, and the
second block's register save/reload lists are computed without knowing
the first one's.

  NOTE: the TU-global form of this knob already exists natively --
  `@CompileFlags: -Wo,-varref,N` (IDO's own option; ranks above any
  hook on the preference ladder).  IDO52_BLOCK_SPLIT_LIMIT is the same
  thing, env-gated, for sweeping; IDO52_NO_BLOCK_SPLIT is the KEYED form
  (one block, not the whole TU) and has no native equivalent.

ENV
  IDO52_NO_BLOCK_SPLIT=BB[,BB...]
      Veto the capacity split for the uopt block numbered BB (the number
      of the CURRENT graph node, MEM_U16(curgraphnode + 8) -- the number
      the `-Wo,-l` flow-graph listing prints).  The block keeps growing
      until a natural block end.  Blocks after it renumber (one fewer).
      Applies in every procedure of the TU.
  IDO52_BLOCK_SPLIT_LIMIT=N
      Replace the limit (for every block) with N; same as -Wo,-varref,N.
  IDO52_BLOCK_SPLIT_TRACE=1
      [BSPLIT] bb=BB cnt=C lim=L SPLIT|VETO  for every capacity split
      that fires (or would have fired and was vetoed).

Default OFF: with none of the three variables set the shim returns the
original flag unchanged after one cached getenv, so the output is
bit-identical to the unhooked build.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UOPT_C = os.path.join(HERE, "build", "5.2", "uopt.c")
MARKER = "IDO52_NO_BLOCK_SPLIT"

HELPER_ANCHOR = "static int ido52_hooks_initialized;\n"

HELPER = r'''/* --- Hook 59: IDO52_NO_BLOCK_SPLIT / IDO52_BLOCK_SPLIT_LIMIT (2026-09-24)
 * Control of uopt's basic-block capacity split (f_getop L43ab50).  See
 * apply_uopt_blocksplit_hooks.py. */
#define IDO52_BSPLIT_MAX 64
static int ido52_bsplit_init_done;
static int ido52_bsplit_active;
static int ido52_bsplit_trace;
static int ido52_bsplit_limit = -1;
static int ido52_bsplit_bb[IDO52_BSPLIT_MAX];
static int ido52_bsplit_nbb;
static void ido52_bsplit_init(void) {
    const char *s;
    ido52_bsplit_init_done = 1;
    s = getenv("IDO52_BLOCK_SPLIT_TRACE");
    ido52_bsplit_trace = (s != NULL && *s != '\0' && *s != '0');
    s = getenv("IDO52_BLOCK_SPLIT_LIMIT");
    if (s != NULL && *s != '\0') {
        ido52_bsplit_limit = (int)strtol(s, NULL, 0);
        fprintf(stderr, "[BSPLIT] limit forced to %d\n", ido52_bsplit_limit);
    }
    s = getenv("IDO52_NO_BLOCK_SPLIT");
    if (s != NULL) {
        while (*s != '\0' && ido52_bsplit_nbb < IDO52_BSPLIT_MAX) {
            char *e;
            long v = strtol(s, &e, 0);
            if (e == s) { s++; continue; }
            ido52_bsplit_bb[ido52_bsplit_nbb++] = (int)v;
            s = e;
            while (*s == ',' || *s == ' ') s++;
        }
        if (ido52_bsplit_nbb > 0)
            fprintf(stderr, "[BSPLIT] %d block veto(s) configured\n", ido52_bsplit_nbb);
    }
    ido52_bsplit_active = ido52_bsplit_trace || ido52_bsplit_limit >= 0 ||
                          ido52_bsplit_nbb > 0;
}
/* flag = the original end-of-block decision (1 = split here). */
static uint32_t ido52_bsplit_q(uint8_t *mem, uint32_t flag, uint32_t cnt, uint32_t lim) {
    uint32_t node, bb;
    int i, veto = 0;
    if (!ido52_bsplit_init_done) ido52_bsplit_init();
    if (!ido52_bsplit_active) return flag;
    /* Only inside a procedure's block reader: the limit is copied into
     * 0x1001e674 at each block start (L459d08); before the first block it
     * is 0 and there is no current graph node. */
    node = MEM_U32(0x1001c8a0);
    if (lim == 0 || node == 0) return flag;
    if (ido52_bsplit_limit >= 0) {
        lim = (uint32_t)ido52_bsplit_limit;
        flag = !((int32_t)cnt < (int32_t)lim);
    }
    if (!flag) return 0;
    bb = MEM_U16(node + 8);
    for (i = 0; i < ido52_bsplit_nbb; i++)
        if ((uint32_t)ido52_bsplit_bb[i] == bb) { veto = 1; break; }
    if (ido52_bsplit_trace)
        fprintf(stderr, "[BSPLIT] bb=%u cnt=%u lim=%u %s\n", bb, cnt, lim,
                veto ? "VETO" : "SPLIT");
    return veto ? 0 : flag;
}
'''

SITE_ANCHOR = """t5 = 0x1001e668;
t4 = 0x1001e674;
t5 = MEM_U32(t5 + 0);
t4 = MEM_U32(t4 + 0);
at = 0x100135d4;
t7 = (int)t5 < (int)t4;
t7 = t7 ^ 0x1;
MEM_U8(at + 0) = (uint8_t)t7;
L43aba4:
"""
SITE_CODE = SITE_ANCHOR.replace(
    "t7 = t7 ^ 0x1;\n",
    "t7 = t7 ^ 0x1;\nt7 = ido52_bsplit_q(mem, t7, t5, t4);  /* Hook 59 */\n")

REBUILD = [
    ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-MMD", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/uopt.o", "build/5.2/uopt.c"],
    ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/out/uopt", "build/5.2/uopt.o", "build/5.2/version_info.o",
     "build/5.2/libc_impl_52.o", "-lm"],
]


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print(f"[blocksplit hooks] Already applied to {path}")
        return True
    for needle in (HELPER_ANCHOR, SITE_ANCHOR):
        if s.count(needle) != 1:
            print(f"[blocksplit hooks] ERROR: anchor not unique/found: "
                  f"{needle[:40]!r} (count {s.count(needle)})")
            return False
    s = s.replace(HELPER_ANCHOR, HELPER + "\n" + HELPER_ANCHOR, 1)
    s = s.replace(SITE_ANCHOR, SITE_CODE, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[blocksplit hooks] Hook 59 applied to {path}")
    return True


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    path = args[0] if args else UOPT_C
    if not apply(path):
        sys.exit(1)
    if "--rebuild" in sys.argv:
        for cmd in REBUILD:
            r = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
            if r.returncode != 0:
                print(r.stderr[-2000:])
                sys.exit(1)
        print("[blocksplit hooks] uopt rebuilt")


if __name__ == "__main__":
    main()
