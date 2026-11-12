#!/usr/bin/env python3
"""Hook 67: IDO52_CFE_SWITCH_CHAIN -- lower a selected `switch` the way the
retail LIBRARY-region compiler does: no jump table, one complete binary
compare tree with three-way nodes, emitted AFTER the switch body.

Idempotent (probes for its own marker).  cfe is not touched by any other
applier.  Part of the post-`make clean` chain (CLAUDE.md Known Gotchas /
UOPT-HOOKS-REFERENCE Hook 67).

    python3 tools/ido-static-recomp/apply_cfe_switchchain_hooks.py [--cfe PATH] [--rebuild]

THE DECISION (cfe, not ugen -- the ucode already carries XJP or a compare
chain; at -O1 there is no uopt and ugen lowers what it is given)
----------------------------------------------------------------------------
func_4648f8 (switch statement codegen) walks the sorted case list and builds
16-byte cluster records {value hi, value lo, count, case node} at sp+0xa8:
a value within 25 of the running cluster's top extends it; a cluster that
closes with fewer than 5 values (`at = a1 < 0x5`, the two sites at
L464c48 / L464e88) is split into single-value records.  Any surviving
multi-value cluster becomes an XJP jump table (func_463aac).  The records go
to func_4640e4, which emits the dispatch right there, BEFORE the body:
runs of < 8 singles as linear `equ ; tjp` chains, else a two-way split
`leq mid ; tjp L ; <right> ; L: <left>`.  The caller (func_465444, L466a50)
then emits the body and the break label.

Retail (0x800B5444, 0x80096784, 0x8009325C) does none of that: `key -> $sN ;
b dispatch ; <bodies> ; dispatch: <tree> ; break:`, the tree a COMPLETE
binary search tree (last level filled from the left) of three-way nodes
`beq key,K,case ; bgt key,K,R ; <left> ; R: <right>`, a single-value leaf
`beq key,K,case ; b default`, and an empty right subtree `bgt -> default`.

For a selected switch the hook (1) forces every cluster to split (both
`a1 < 0x5` tests read true), so there is no table; (2) at L4653e8, instead
of calling func_4640e4, pre-assigns every case label, emits `ujp Ldisp`
and parks the records; (3) at L466a6c (after the body, before the break
label) emits `ujp Lbreak ; lab Ldisp ; <complete-tree>`, then frees the
key temp.  Unselected switches are untouched.

ENV
---
IDO52_CFE_SWITCH_CHAIN=all | N[,N..]   0-based index of the switch statement
    in the translation unit (every switch counts, in source order).
IDO52_CFE_SWITCH_TRACE=1   `[CFE-SWITCH] #N cases=<c> range=<r> -> TABLE|
    MIXED|CHAIN[ (forced, retail tree)]` per switch.
Default OFF: with the env unset nothing is selected and every object is
unchanged.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
CFE_C = os.path.join(HERE, "build", "5.2", "cfe.c")

MARKER = "cfsw_enter"

HELPER_ANCHOR = ("static void func_4648f8(uint8_t *mem, uint32_t sp, uint32_t a0) {\n"
                 "const")

HELPERS = r"""/* --- IDO52_CFE_SWITCH_CHAIN (Hook 67, 2026-09-25): retail library-region
 * switch lowering (no table, complete three-way compare tree after the
 * body).  See apply_cfe_switchchain_hooks.py.  Default OFF. --- */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
static int cfsw_inited, cfsw_all, cfsw_trace, cfsw_idx = -1, cfsw_sel;
static char *cfsw_list;
static int cfsw_cases, cfsw_range;
static uint32_t cfsw_stmt;
struct cfsw_pend { uint32_t stmt, n, temp, tfree, deflab, ldisp, dt, sz; uint32_t *rec; };
static struct cfsw_pend cfsw_p[64];
static int cfsw_np;

static int cfsw_selected(int idx) {
    const char *p = cfsw_list;
    if (cfsw_all) return 1;
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

static void cfsw_enter(uint8_t *mem, uint32_t stmt) {
    uint32_t node;
    int32_t mn = 0, mx = 0;
    if (!cfsw_inited) {
        const char *e = getenv("IDO52_CFE_SWITCH_CHAIN");
        const char *t = getenv("IDO52_CFE_SWITCH_TRACE");
        cfsw_inited = 1;
        if (e != NULL && *e) {
            if (strcmp(e, "all") == 0) cfsw_all = 1;
            else cfsw_list = (char *)e;
        }
        cfsw_trace = (t != NULL && *t && strcmp(t, "0") != 0);
    }
    cfsw_idx++;
    cfsw_stmt = stmt;
    cfsw_sel = (cfsw_all || cfsw_list != NULL) ? cfsw_selected(cfsw_idx) : 0;
    if (!cfsw_trace) return;
    cfsw_cases = 0;
    node = MEM_U32(stmt + 40);
    if (node != 0) node = MEM_U32(node + 8);
    while (node != 0) {
        int32_t v = (int32_t)MEM_U32(MEM_U32(node + 28) + 28);
        if (cfsw_cases == 0 || v < mn) mn = v;
        if (cfsw_cases == 0 || v > mx) mx = v;
        cfsw_cases++;
        node = MEM_U32(node + 8);
    }
    cfsw_range = cfsw_cases ? (int)(mx - mn + 1) : 0;
}

/* at L4650a0: s0 = index of the last cluster record (-1: none) */
static void cfsw_verdict(uint8_t *mem, uint32_t recs, uint32_t last) {
    int i, n = (int32_t)last + 1, multi = 0;
    if (!cfsw_trace) return;
    for (i = 0; i < n; i++)
        if (MEM_U32(recs + i * 16 + 8) != 1) multi++;
    fprintf(stderr, "[CFE-SWITCH] #%d cases=%d range=%d -> %s%s\n", cfsw_idx,
            cfsw_cases, cfsw_range,
            multi == 0 ? "CHAIN" : (multi == n ? "TABLE" : "MIXED"),
            cfsw_sel ? " (forced, retail tree)" : "");
}

static void cfsw_advance(uint8_t *mem, uint32_t sp) {
    uint32_t v0 = MEM_U32(0x1002f620);
    uint32_t nx = v0 + MEM_U32(0x100087d0 + (MEM_U8(v0 + 0) << 4) + 4);
    MEM_U32(0x1002f620) = nx;
    if (!((int)(nx - 0x1002e620) < (int)0x801)) {
        f_UWRITE(mem, sp, 0x1002e620, nx - 0x1002e620, 0, 0);
        MEM_U32(0x1002f620) = 0x1002e620;
    }
}

static void cfsw_op_dt(uint8_t *mem, uint32_t sp, uint32_t op, uint32_t dt) {
    uint32_t c = MEM_U32(0x1002f620);
    MEM_U8(c + 0) = (uint8_t)op;
    MEM_U8(c + 1) = (uint8_t)((MEM_U8(c + 1) & 0xffe0) | (dt & 0x1f));
    MEM_U16(c + 2) = 0;
    cfsw_advance(mem, sp);
}

static void cfsw_op_lab(uint8_t *mem, uint32_t sp, uint32_t op, uint32_t lab) {
    uint32_t c = MEM_U32(0x1002f620);
    MEM_U8(c + 0) = (uint8_t)op;
    MEM_U32(c + 4) = lab;
    if (op == 0x42) {           /* LAB */
        MEM_U16(c + 2) = 0;
        MEM_U32(c + 8) = 0;
    }
    cfsw_advance(mem, sp);
}

static uint32_t cfsw_newlab(uint8_t *mem) {
    uint32_t l = MEM_U32(0x1002f624) + 1;
    MEM_U32(0x1002f624) = l;
    return l;
}

/* compare key against record i: ldv key ; ldc K ; <op> ; tjp lab */
static void cfsw_cmp(uint8_t *mem, uint32_t sp, struct cfsw_pend *p, int i,
                     uint32_t op, uint32_t lab) {
    f_load_var(mem, sp, p->temp, 0, 0, 0);
    f_LDC_integer(mem, sp, p->dt, p->sz, p->rec[i * 3 + 0], p->rec[i * 3 + 1]);
    cfsw_op_dt(mem, sp, op, p->dt);
    cfsw_op_lab(mem, sp, 0x7f, lab);      /* TJP */
}

/* complete binary search tree: left subtree = full part + last-level share */
static void cfsw_tree(uint8_t *mem, uint32_t sp, struct cfsw_pend *p, int lo, int hi) {
    int n = hi - lo + 1, full = 1, left, m;
    uint32_t r;
    if (n <= 0) {
        cfsw_op_lab(mem, sp, 0x88, p->deflab);          /* UJP default */
        return;
    }
    if (n == 1) {
        cfsw_cmp(mem, sp, p, lo, 0x23, MEM_U32(p->rec[lo * 3 + 2] + 36));   /* EQU */
        cfsw_op_lab(mem, sp, 0x88, p->deflab);
        return;
    }
    while (full * 2 + 1 <= n) full = full * 2 + 1;
    left = (full - 1) / 2;
    left += (n - full) < (full + 1) / 2 ? (n - full) : (full + 1) / 2;
    m = lo + left;
    cfsw_cmp(mem, sp, p, m, 0x23, MEM_U32(p->rec[m * 3 + 2] + 36));          /* EQU */
    r = (m < hi) ? cfsw_newlab(mem) : p->deflab;
    cfsw_cmp(mem, sp, p, m, 0x29, r);                                         /* GRT */
    cfsw_tree(mem, sp, p, lo, m - 1);
    if (m < hi) {
        cfsw_op_lab(mem, sp, 0x42, r);
        cfsw_tree(mem, sp, p, m + 1, hi);
    }
}

/* at L4653e8 (chain path, key already stored in temp): park the records */
static void cfsw_defer(uint8_t *mem, uint32_t sp, uint32_t recs, uint32_t last,
                       uint32_t temp, uint32_t deflab, uint32_t tfree) {
    struct cfsw_pend *p;
    uint32_t ty;
    int i, n = (int32_t)last + 1;
    if (cfsw_np >= 64) { fprintf(stderr, "[CFE-SWITCH] nesting > 64\n"); exit(1); }
    p = &cfsw_p[cfsw_np++];
    p->stmt = cfsw_stmt;
    p->n = n;
    p->temp = temp;
    p->tfree = tfree;
    p->deflab = deflab;
    ty = MEM_U32(temp + 8);
    p->dt = f_U_DT(mem, sp, ty);
    p->sz = MEM_U32(ty + 24) >> 3;
    p->rec = (uint32_t *)malloc(sizeof(uint32_t) * 3 * (n > 0 ? n : 1));
    for (i = 0; i < n; i++) {
        uint32_t node = MEM_U32(recs + i * 16 + 12);
        p->rec[i * 3 + 0] = MEM_U32(recs + i * 16 + 0);
        p->rec[i * 3 + 1] = MEM_U32(recs + i * 16 + 4);
        p->rec[i * 3 + 2] = node;
        MEM_U32(node + 36) = cfsw_newlab(mem);      /* case label, now */
    }
    p->ldisp = cfsw_newlab(mem);
    cfsw_op_lab(mem, sp, 0x88, p->ldisp);           /* UJP dispatch */
}

/* at L466a6c (after the switch body, before its break label) */
static void cfsw_flush(uint8_t *mem, uint32_t sp, uint32_t stmt) {
    struct cfsw_pend *p;
    if (cfsw_np == 0 || cfsw_p[cfsw_np - 1].stmt != stmt) return;
    p = &cfsw_p[cfsw_np - 1];
    cfsw_op_lab(mem, sp, 0x88, MEM_U32(stmt + 32));  /* UJP break */
    cfsw_op_lab(mem, sp, 0x42, p->ldisp);            /* LAB dispatch */
    cfsw_tree(mem, sp, p, 0, (int)p->n - 1);
    f_free_temp(mem, sp, p->tfree);
    free(p->rec);
    cfsw_np--;
}

"""

EDITS = [
    # (old, new) -- each old must be unique inside the named function
    ("func_4648f8", "L4648f8:\n",
     "L4648f8:\n/* === IDO52_CFE_SWITCH_CHAIN (Hook 67) */\ncfsw_enter(mem, a0);\n"),
    ("func_4648f8", "at = a1 < 0x5;\n", None),   # both sites, handled below
    ("func_4648f8", "L4650a0:\n",
     "L4650a0:\ncfsw_verdict(mem, sp + 0xa8, s0);\n"),
    ("func_4648f8", "L4653e8:\n",
     "L4653e8:\n/* === IDO52_CFE_SWITCH_CHAIN (Hook 67): park, emit after the body */\n"
     "if (cfsw_sel) {\n"
     "cfsw_defer(mem, sp, sp + 0xa8, s0, s1, MEM_U32(sp + 152), MEM_U32(sp + 92));\n"
     "goto L465430;}\n"),
    ("func_465444", "L466a6c:\n",
     "L466a6c:\n/* === IDO52_CFE_SWITCH_CHAIN (Hook 67) */\ncfsw_flush(mem, sp, s2);\n"),
]


def func_span(s, name):
    hdr = "static void %s(uint8_t *mem" % name
    start = s.index(hdr)
    while s[s.index("\n", start) - 1] != "{":     # skip the prototype
        start = s.index(hdr, start + 1)
    end = s.index("\nstatic ", start + 1)
    return start, end


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print(f"[cfe switchchain hooks] Hook 67 already applied to {path}")
        return True
    if s.count(HELPER_ANCHOR) != 1:
        print("[cfe switchchain hooks] ERROR: func_4648f8 definition anchor not unique")
        return False
    for fn, old, new in EDITS:
        a, b = func_span(s, fn)
        body = s[a:b]
        want = 2 if new is None else 1
        if body.count(old) != want or (new is not None and s.count(old) != 1):
            print(f"[cfe switchchain hooks] ERROR: anchor {old[:40]!r} in {fn}: "
                  f"{body.count(old)} (want {want}), file-wide {s.count(old)}")
            return False
        if new is None:
            body = body.replace(old, "at = (a1 < 0x5) || cfsw_sel;\n")
        else:
            body = body.replace(old, new, 1)
        s = s[:a] + body + s[b:]
    s = s.replace(HELPER_ANCHOR, HELPERS + HELPER_ANCHOR, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[cfe switchchain hooks] Hook 67 applied to {path}")
    return True


REBUILD = [
    ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/cfe.o", "build/5.2/cfe.c"],
    ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/out/cfe.new", "build/5.2/cfe.o", "build/5.2/version_info.o",
     "build/5.2/libc_impl_52.o", "-lm"],
]


def main():
    argv = sys.argv[1:]
    path = CFE_C
    if "--cfe" in argv:
        path = argv[argv.index("--cfe") + 1]
    if not apply(path):
        sys.exit(1)
    if "--rebuild" in argv:
        for cmd in REBUILD:
            r = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
            if r.returncode != 0:
                print(r.stderr[-2000:])
                sys.exit(1)
        os.replace(os.path.join(HERE, "build/5.2/out/cfe.new"),
                   os.path.join(HERE, "build/5.2/out/cfe"))
        print("[cfe switchchain hooks] cfe rebuilt")


if __name__ == "__main__":
    main()
