#!/usr/bin/env python3
"""Hook 80: IDO52_UGEN_CMPBR_RIGHT_FIRST -- per-occurrence right-operand-first
evaluation of a register compare-and-branch.

Idempotent (probes for its own marker); independent of every other applier
(its helper anchors on f_jump's own definition, its site on the f_jump label
L422fc8, which no other applier touches).  Part of the post-`make clean`
chain (CLAUDE.md Known Gotchas / UOPT-HOOKS-REFERENCE.md Hook 80).

    python3 tools/ido-static-recomp/apply_ugen_cmpbr_order_hooks.py [--ugen PATH] [--rebuild]

THE TRANSFORM
-------------
ugen's f_jump lowers a conditional jump on an integer relation between two
non-constant operands (`fjp (les a b)` -> `bge a,b,L`, ...) at L422fc8 as

    f_eval(left) ; f_eval(right) ; f_reg(left) ; f_reg(right) ;
    f_emit_branch_rrll(op, rL, rR, L)

-- always LEFT first, whatever the operands cost (f_eval_2ops, the general
binary-operator path, evaluates the operand needing MORE registers first and
the left one on a tie).  uopt hands relations over in a canonical operand
order of its own (a signed `x / 2^k` lands on the left of `rem`: `60000000 %
t > t / 2` reaches ugen as `fjp (geq (div t 2) (rem K t))`, whatever the
source spelling), so the instruction stream is `div t7,t,2 ; rem t6,K,t ;
bge t7,t6`.  The retail library band has the operands in the other order:
the `rem` (its `div zero,t2,v0`, `mfhi t6` and divide checks) and only then
the signed pow2 `div` expansion (`bgez ; addu at ; addiu at ; sra t7`) and
`slt at,t7,t6 ; beqz at` -- retail InitSoundSeq 0x800912D4..0x80091374 and
its sister InitSoundSep 0x800916C4.. (the right operand evaluated first).

The hook evaluates the RIGHT operand first, then the left; the f_reg reads
and the branch emission are untouched, so the branch still compares
(left, right) in the same registers.  Every instruction is ugen's own -- only
the order in which the two operand trees are emitted changes (the order
f_eval_2ops itself picks when the right operand is the costlier one).

SOUNDNESS
---------
The reordering is applied only when BOTH operand trees are order-insensitive
(ugen_cmprf_pure): constants (Uldc), direct loads (Ulod) that are NOT
volatile (lexlev bit 0), and the pure integer operators abs add and cvt cvtl
dec div inc ior lnot mod mpy neg not rem shl shr sub xor -- no store, no
call, no indirect load -- and at most ONE operation that can trap (a
div/rem/mod whose divisor is not a constant other than 0 / -1).  Everything
else in the two trees has no observable effect, so emitting it after instead
of before that one operation changes no observable order.  An impure
occurrence keeps the stock order (TRACE says IMPURE).

ENV
---
IDO52_UGEN_CMPBR_RIGHT_FIRST=all | occ:N[,N...] | N[,N...]
    Occurrences count, from 0, the arrivals at L422fc8 in the TU (ugen
    emission order).  A bare number is an index list.
IDO52_UGEN_CMPBR_RIGHT_FIRST_TRACE=1
    `[UGEN-CMPRF] occ=<n> op=<rel> L=<op> R=<op> pure|IMPURE -> RIGHT FIRST|stock`
Default OFF: with the env unset the helper returns at once and f_jump takes
its stock path.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UGEN_C = os.path.join(HERE, "build", "5.2", "ugen.c")

MARKER = "ugen_cmprf_pure"

HELPER_ANCHOR = ("static void f_jump(uint8_t *mem, uint32_t sp, uint32_t a0, "
                 "uint32_t a1, uint32_t a2) {\nconst")

HELPERS = r"""/* --- IDO52_UGEN_CMPBR_RIGHT_FIRST (Hook 80, 2026-10-04): per-occurrence
 * right-operand-first evaluation of a register compare-and-branch in
 * f_jump (L422fc8).  See apply_ugen_cmpbr_order_hooks.py.  Default OFF. --- */
static int ugen_cmprf_inited, ugen_cmprf_all, ugen_cmprf_trace, ugen_cmprf_occ;
static int ugen_cmprf_list[256], ugen_cmprf_n;

/* 1 if the tree is side-effect free and order-insensitive: constants,
 * non-volatile direct loads (Ulod), and pure integer arithmetic. */
static int ugen_cmprf_ndiv;
static int ugen_cmprf_pure(uint8_t *mem, uint32_t n, int depth) {
    unsigned op;
    if (n == 0) return 1;
    if (depth > 32) return 0;
    op = MEM_U8(n + 32);
    if (op == 0x1d || op == 0x57 || op == 0x69) {   /* div mod rem */
        /* can trap unless the divisor is a constant other than 0 / -1 */
        uint32_t d = MEM_U32(n + 4);
        if (d == 0 || MEM_U8(d + 32) != 0x49 ||
            MEM_U32(d + 48) == 0 || MEM_U32(d + 48) == 0xffffffffu)
            ugen_cmprf_ndiv++;
    }
    switch (op) {
    case 0x49:                                   /* Uldc */
        return 1;
    case 0x52:                                   /* Ulod */
        return (MEM_U16(n + 34) & 1) == 0;       /* lexlev bit 0: volatile */
    case 0x00: case 0x01: case 0x04: case 0x18: case 0x19: case 0x1a:
    case 0x1d: case 0x37: case 0x3c: case 0x50: case 0x57: case 0x5b:
    case 0x5e: case 0x61: case 0x69: case 0x73: case 0x74: case 0x7d:
    case 0x8d:
        /* abs add and cvt cvtl dec div inc ior lnot mod mpy neg not rem
         * shl shr sub xor */
        return ugen_cmprf_pure(mem, MEM_U32(n + 0), depth + 1) &&
               ugen_cmprf_pure(mem, MEM_U32(n + 4), depth + 1);
    }
    return 0;
}

static int ugen_cmprf(uint8_t *mem, uint32_t node) {
    int occ, v = 0, i, pure;
    if (!ugen_cmprf_inited) {
        const char *e = getenv("IDO52_UGEN_CMPBR_RIGHT_FIRST");
        const char *t = getenv("IDO52_UGEN_CMPBR_RIGHT_FIRST_TRACE");
        ugen_cmprf_inited = 1;
        if (e != NULL && *e) {
            if (strcmp(e, "all") == 0) {
                ugen_cmprf_all = 1;
            } else {
                const char *p = e;
                if (strncmp(p, "occ:", 4) == 0) p += 4;
                while (*p && ugen_cmprf_n < 256) {
                    char *end;
                    long n = strtol(p, &end, 10);
                    if (end == p) {
                        fprintf(stderr, "[UGEN-CMPRF] bad list %s\n", e);
                        break;
                    }
                    ugen_cmprf_list[ugen_cmprf_n++] = (int)n;
                    p = end;
                    if (*p == ',') p++;
                }
            }
        }
        ugen_cmprf_trace = (t != NULL && *t && strcmp(t, "0") != 0);
    }
    if (!ugen_cmprf_all && ugen_cmprf_n == 0 && !ugen_cmprf_trace) return 0;
    occ = ugen_cmprf_occ++;
    if (ugen_cmprf_all) v = 1;
    for (i = 0; !v && i < ugen_cmprf_n; i++)
        if (ugen_cmprf_list[i] == occ) v = 1;
    /* at most ONE operation that can trap (div/rem/mod) across both
     * operands: everything else is unobservable, so moving it across that
     * one operation changes no observable order */
    ugen_cmprf_ndiv = 0;
    pure = ugen_cmprf_pure(mem, MEM_U32(node + 0), 0) &&
           ugen_cmprf_pure(mem, MEM_U32(node + 4), 0) && ugen_cmprf_ndiv <= 1;
    if (ugen_cmprf_trace)
        fprintf(stderr, "[UGEN-CMPRF] occ=%d op=%02x L=%02x R=%02x %s -> %s\n", occ,
                MEM_U8(node + 32), MEM_U8(MEM_U32(node + 0) + 32),
                MEM_U8(MEM_U32(node + 4) + 32), pure ? "pure" : "IMPURE",
                (v && pure) ? "RIGHT FIRST" : "stock");
    return v && pure;
}

"""

SITE_OLD = ("L422fc8:\n//nop;\na0 = MEM_U32(s0 + 0);\na1 = 0x48;\n"
            "f_eval(mem, sp, a0, a1);\n")
SITE_NEW = ("L422fc8:\n/* === IDO52_UGEN_CMPBR_RIGHT_FIRST (Hook 80): right operand first;\n"
            " * the f_reg reads and the branch below are unchanged */\n"
            "if (ugen_cmprf(mem, s0)) {\n"
            "f_eval(mem, sp, MEM_U32(s0 + 4), 0x48);\n"
            "f_eval(mem, sp, MEM_U32(s0 + 0), 0x48);\n"
            "goto L422ff0;}\n"
            "//nop;\na0 = MEM_U32(s0 + 0);\na1 = 0x48;\n"
            "f_eval(mem, sp, a0, a1);\n")

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
        print(f"[ugen cmpbr-order hooks] Hook 80 already applied to {path}")
        return True
    for needle in (HELPER_ANCHOR, SITE_OLD):
        if s.count(needle) != 1:
            print(f"[ugen cmpbr-order hooks] ERROR: anchor not unique/found: "
                  f"{needle[:48]!r} (count {s.count(needle)})")
            return False
    start = s.index(HELPER_ANCHOR)
    nxt = s.find("\nstatic ", start + 1)
    site = s.index(SITE_OLD)
    if not (start < site < (nxt if nxt != -1 else len(s))):
        print("[ugen cmpbr-order hooks] ERROR: site L422fc8 is not inside f_jump")
        return False
    if "L422ff0:\n" not in s[site:nxt]:
        print("[ugen cmpbr-order hooks] ERROR: L422ff0 not found after the site")
        return False
    s = s.replace(SITE_OLD, SITE_NEW, 1)
    s = s.replace(HELPER_ANCHOR, HELPERS + HELPER_ANCHOR, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[ugen cmpbr-order hooks] Hook 80 applied to {path}")
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
        print("[ugen cmpbr-order hooks] ugen rebuilt")


if __name__ == "__main__":
    main()
