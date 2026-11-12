#!/usr/bin/env python3
"""Hook 70: IDO52_CFE_FRAME_TOP_BASE -- hand the base of a subscripted LOCAL
array to uopt as `LDA(M, offset 0, offset2 = array) ; LDC <array offset> ;
ADD` (the retail library-region front end's form) instead of 5.2's
`LDA(M, <array offset>)`.

Idempotent (probes for its own marker); independent of Hooks 67/68 (all three
patch cfe, at unrelated functions; any order).  Part of the post-`make clean`
chain (CLAUDE.md Known Gotchas / UOPT-HOOKS-REFERENCE Hook 70).

    python3 tools/ido-static-recomp/apply_cfe_frametop_hooks.py [--cfe PATH] [--rebuild]

THE SITE (cfe)
--------------
f_expr_cg's IXA case (L481668 ff.) evaluates the subscript's base operand
(node+24) with a recursive f_expr_cg -- first (L481734) or, for the swapped
form, second (after L481708) -- then the index, then emits `IXA`.  A local
array base comes out of that recursion through func_486a64 -> f_load_addr,
whose plain path (L48a974) writes the 24-byte LDA record: +1 mtype<<5|dtype,
+4 block, +8 length, +12 offset, +16 offset2 (offset == offset2 == the
array's byte offset from the frame top, negative for an auto).

The hook ARMS at the base evaluation (records the ucode cursor and the
f_UWRITE flush count) and DISARMS when it returns.  In f_load_addr, a record
is a candidate only when it is the FIRST record written since the arm (same
cursor, no flush in between), the variable is an auto (mtype 1) of ARRAY
type, and its offset is negative.  Each candidate takes the next 0-based
site number.  For a selected site the LDA's offset field (+12) is written 0
(offset2 keeps the array), and after the record is committed the hook emits,
through cfe's own emitters, `LDC <array offset>` (f_LDC_integer, dtype J,
4 bytes -- the call cfe itself makes for a struct member offset) and `ADD`
(opcode 0x01, dtype Adt = 0, exactly as cfe's struct-member ADD).  uopt then folds the constant into the
ILOD/ISTR displacement exactly as it does for `s.b[i]`, the frame-top LDAs
are one leaf, and ugen emits `addiu tN,sp,<framesize> ; l* -K(tN)`.

ENV
---
IDO52_CFE_FRAME_TOP_BASE=all | N[,N..]   N = 0-based candidate site in the TU
    (source/codegen order).
IDO52_CFE_FRAME_TOP_TRACE=1   `[CFE-FTB] #N array=<name> off=<frame off> ->
    STOCK|FRAME-TOP` per candidate.
Default OFF: with the env unset no record is touched.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
CFE_C = os.path.join(HERE, "build", "5.2", "cfe.c")

MARKER = "cfftb_arm"

HELPER_ANCHOR = ("static void f_expr_cg(uint8_t *mem, uint32_t sp, uint32_t a0, "
                 "uint32_t a1, uint32_t a2, uint32_t a3) {\nconst")

HELPERS = r"""/* --- IDO52_CFE_FRAME_TOP_BASE (Hook 70, 2026-09-25): a subscripted
 * local array's base as LDA(M,0 ; offset2 = array) + LDC off + ADD.  See
 * apply_cfe_frametop_hooks.py.  Default OFF. --- */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
static int cfftb_inited, cfftb_all, cfftb_trace, cfftb_idx = -1;
static const char *cfftb_list;
extern uint32_t cfftb_flushes;          /* f_UWRITE calls so far */
static int cfftb_armed;
static uint32_t cfftb_arm_cur, cfftb_arm_fl;
static int cfftb_cand;                  /* f_load_addr: this LDA is the base */
static int cfftb_emit;                  /* f_load_addr: emit LDC + ADD at exit */
static int32_t cfftb_off;

static void cfftb_init(void) {
    const char *e, *t;
    if (cfftb_inited) return;
    cfftb_inited = 1;
    e = getenv("IDO52_CFE_FRAME_TOP_BASE");
    t = getenv("IDO52_CFE_FRAME_TOP_TRACE");
    if (e != NULL && *e) {
        if (strcmp(e, "all") == 0) cfftb_all = 1;
        else cfftb_list = e;
    }
    cfftb_trace = (t != NULL && *t && strcmp(t, "0") != 0);
}

static int cfftb_selected(int idx) {
    const char *p = cfftb_list;
    if (cfftb_all) return 1;
    if (p == NULL) return 0;
    while (*p) {
        char *end;
        long v = strtol(p, &end, 10);
        if (end == p) { p++; continue; }
        if (v == idx) return 1;
        p = end;
    }
    return 0;
}

/* f_expr_cg IXA case: about to evaluate the subscript's base operand */
static void cfftb_arm(uint8_t *mem) {
    cfftb_armed = 1;
    cfftb_arm_cur = MEM_U32(0x1002f620);
    cfftb_arm_fl = cfftb_flushes;
}

static void cfftb_disarm(void) {
    cfftb_armed = 0;
}

/* f_load_addr, L48a974 (plain path, before the LDA record is written) */
static void cfftb_enter(uint8_t *mem) {
    cfftb_cand = cfftb_armed && MEM_U32(0x1002f620) == cfftb_arm_cur
                 && cfftb_flushes == cfftb_arm_fl;
    cfftb_armed = 0;        /* only the first record after the arm is the base */
}

/* strip a qualified-type wrapper (kind 0x18 with qualifier bits
 * 0xd0000000 at +12), exactly as f_load_addr does */
static uint32_t cfftb_type(uint8_t *mem, uint32_t ty) {
    int n = 0;
    while (ty != 0 && MEM_U32(ty + 4) == 0x18 && (MEM_U32(ty + 12) & 0xd0000000u)
           && n++ < 16)
        ty = MEM_U32(ty + 8);
    return ty;
}

/* f_load_addr, L48aa94: record complete (cur = record, decl = the variable) */
static void cfftb_record(uint8_t *mem, uint32_t cur, uint32_t decl) {
    uint32_t mt, ty;
    int32_t off;
    int sel;
    char name[64];
    if (!cfftb_cand) return;
    cfftb_cand = 0;
    mt = MEM_U8(cur + 1) >> 5;
    off = (int32_t)MEM_U32(cur + 12);
    ty = cfftb_type(mem, MEM_U32(decl + 8));
    if (mt != 1 || off >= 0 || ty == 0 || MEM_U32(ty + 4) != CFFTB_ARRAY_KIND) return;
    if ((int32_t)MEM_U32(cur + 16) != off) return;
    cfftb_init();
    cfftb_idx++;
    sel = cfftb_selected(cfftb_idx);
    if (cfftb_trace) {
        /* decl+24 = identifier entry: length at +20, characters at +24 */
        uint32_t id = MEM_U32(decl + 24), len = id ? MEM_U32(id + 20) : 0, j;
        if (len > sizeof(name) - 1) len = sizeof(name) - 1;
        for (j = 0; j < len; j++) name[j] = (char)MEM_U8(id + 24 + j);
        name[len] = 0;
        fprintf(stderr, "[CFE-FTB] #%d array=%s off=%d -> %s\n", cfftb_idx, name,
                off, sel ? "FRAME-TOP" : "STOCK");
    }
    if (!sel) return;
    MEM_U32(cur + 12) = 0;          /* LDA(M, 0 ; offset2 = the array) */
    cfftb_emit = 1;
    cfftb_off = off;
}

/* f_load_addr exit: the LDA is committed; emit LDC off ; ADD */
static void cfftb_tail(uint8_t *mem, uint32_t sp) {
    uint32_t c, nx;
    if (!cfftb_emit) return;
    cfftb_emit = 0;
    f_LDC_integer(mem, sp, 6, 4, cfftb_off < 0 ? 0xffffffffu : 0, (uint32_t)cfftb_off);
    c = MEM_U32(0x1002f620);
    MEM_U8(c + 0) = 0x01;                                   /* ADD */
    /* dtype Adt (0), as cfe's own struct-member ADD (f_U_DT of an address).
     * NOT the LDA's dtype bits: f_load_addr never writes them (stale buffer
     * bytes), and a stale Pdt/Ldt there makes uopt's f_genloadnum abort
     * (`Internal: code 6320`) once strength reduction materialises it. */
    MEM_U8(c + 1) = (uint8_t)(MEM_U8(c + 1) & 0xffe0);
    MEM_U16(c + 2) = 0;
    nx = c + MEM_U32(0x100087d0 + (MEM_U8(c + 0) << 4) + 4);
    MEM_U32(0x1002f620) = nx;
    if (!((int)(nx - 0x1002e620) < (int)0x801)) {
        f_UWRITE(mem, sp, 0x1002e620, nx - 0x1002e620, 0, 0);
        MEM_U32(0x1002f620) = 0x1002e620;
    }
}

"""

# (function, old, new) -- each old must be unique inside the named function
EDITS = [
    ("f_UWRITE", "L46050c:\n", "L46050c:\ncfftb_flushes++;   /* Hook 70 */\n"),
    ("f_expr_cg", "L481734:\n",
     "L481734:\n/* === IDO52_CFE_FRAME_TOP_BASE (Hook 70): base operand first */\n"
     "cfftb_arm(mem);\n"),
    ("f_expr_cg", "L481748:\n", "L481748:\ncfftb_disarm();   /* Hook 70 */\n"),
    ("f_expr_cg", "L481708:\n",
     "L481708:\n/* === IDO52_CFE_FRAME_TOP_BASE (Hook 70): base operand second */\n"
     "cfftb_arm(mem);\n"),
    ("f_expr_cg", "L481724:\n", "L481724:\ncfftb_disarm();   /* Hook 70 */\n"),
    ("f_load_addr", "L48a974:\n",
     "L48a974:\n/* === IDO52_CFE_FRAME_TOP_BASE (Hook 70) */\ncfftb_enter(mem);\n"),
    ("f_load_addr", "L48aa94:\nMEM_U32(t4 + 16) = t3;\n",
     "L48aa94:\nMEM_U32(t4 + 16) = t3;\n"
     "cfftb_record(mem, t4, MEM_U32(sp + 48));   /* Hook 70 */\n"),
    ("f_load_addr", "L48aae4:\n", "L48aae4:\ncfftb_tail(mem, sp);   /* Hook 70 */\n"),
]


def func_span(s, name):
    for ret in ("void", "uint64_t", "uint32_t"):
        hdr = "static %s %s(uint8_t *mem" % (ret, name)
        start = s.find(hdr)
        while start >= 0 and s[s.index("\n", start) - 1] != "{":   # prototype
            start = s.find(hdr, start + 1)
        if start >= 0:
            return start, s.index("\nstatic ", start + 1)
    raise ValueError(name)


def apply(path, array_kind):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print(f"[cfe frametop hooks] Hook 70 already applied to {path}")
        return True
    if s.count(HELPER_ANCHOR) != 1:
        print("[cfe frametop hooks] ERROR: f_expr_cg definition anchor not unique")
        return False
    for fn, old, new in EDITS:
        a, b = func_span(s, fn)
        if s[a:b].count(old) != 1 or s.count(old) != 1:
            print(f"[cfe frametop hooks] ERROR: anchor {old[:40]!r} in {fn}: "
                  f"{s[a:b].count(old)} (want 1), file-wide {s.count(old)}")
            return False
    for fn, old, new in EDITS:
        a, b = func_span(s, fn)
        s = s[:a] + s[a:b].replace(old, new, 1) + s[b:]
    helpers = HELPERS.replace("CFFTB_ARRAY_KIND", str(array_kind))
    a, _ = func_span(s, "f_UWRITE")
    s = s[:a] + ("/* Hook 70 (IDO52_CFE_FRAME_TOP_BASE): flush counter */\n"
                 "uint32_t cfftb_flushes;\n") + s[a:]
    s = s.replace(HELPER_ANCHOR, helpers + HELPER_ANCHOR, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[cfe frametop hooks] Hook 70 applied to {path}")
    return True


REBUILD = [
    ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/cfe.o", "build/5.2/cfe.c"],
    ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/out/cfe.new", "build/5.2/cfe.o", "build/5.2/version_info.o",
     "build/5.2/libc_impl_52.o", "-lm"],
]

ARRAY_KIND = 0x13


def main():
    argv = sys.argv[1:]
    path = CFE_C
    if "--cfe" in argv:
        path = argv[argv.index("--cfe") + 1]
    if not apply(path, ARRAY_KIND):
        sys.exit(1)
    if "--rebuild" in argv:
        for cmd in REBUILD:
            r = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
            if r.returncode != 0:
                print(r.stderr[-2000:])
                sys.exit(1)
        os.replace(os.path.join(HERE, "build/5.2/out/cfe.new"),
                   os.path.join(HERE, "build/5.2/out/cfe"))
        print("[cfe frametop hooks] cfe rebuilt")


if __name__ == "__main__":
    main()
