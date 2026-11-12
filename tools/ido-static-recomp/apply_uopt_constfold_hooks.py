#!/usr/bin/env python3
"""Hook 47: IDO52_NO_BINFOLD_OPCODES -- veto uopt's binary constant fold.

*** NOT PART OF THE POST-`make clean` APPLIER CHAIN. ***  uopt.c is pristine
without it; apply it deliberately when you want the probe.  It was built to
serve main/pad_type_check's stated "wanted primitive" (a uopt veto for the
and-with-mask simplification) and it REFUTES that request: declining the fold
yields `li v1,255`, not retail's `andi v1,a1,0xff`, because ugen then folds
`AND(const,const)` itself.  See docs/reference/UOPT-HOOKS-REFERENCE.md Hook 47
and the header of src/main/semi-matched/pad_type_check.c before re-deriving any
of this.

Run AFTER apply_uopt_hooks.py (needs ``_opc_names`` and ``getenv``) and after
apply_uopt_readphase_hooks.py (which installs the early ``ido52_init_hooks()``
call at ``main``).  Idempotent: probes for its own marker.

    python3 tools/ido-static-recomp/apply_uopt_constfold_hooks.py [build/5.2/uopt.c] [--rebuild]

WHAT IT DOES
------------
uopt's expression-simplification walk calls ``f_binaryfold`` whenever BOTH
operands of a binary U-code node have resolved to literal constants, replaces
the node with the computed constant, and the later CONST_INREG pass then
serves that constant out of whatever register already holds it.  The visible
result for ``x & 0xFF`` where copy/constant propagation has proved ``x == 255``
is a bare ``move`` -- the mask disappears entirely.

Retail Konami objects contain the UNFOLDED form (``andi $v1,$a1,0xff`` with
``$a1`` the held-constant 255 register), i.e. the original build did not fold
that node.  This hook reproduces that by DENYING the fold for selected U-code
opcodes at the one call site that performs it.

    IDO52_NO_BINFOLD_OPCODES=OPC[:FROM[:TO]][,...]

``OPC`` is a ``_opc_names`` mnemonic (``and``, ``ior``, ``xor``, ``add``,
``shl``, ...) or a raw U-code opcode number.  ``FROM``/``TO`` are a 1-based
occurrence window over the *eligible* folds of that opcode in this uopt run
(default ``1..INT_MAX`` = all of them), so a single site can be targeted in a
function that folds the same opcode more than once.  Use one function per
translation unit when windowing -- the counter is global to the uopt run.

``IDO52_NO_BINFOLD_DEBUG=1`` prints every denied fold; ``=2`` prints every
constant-constant fold the site sees (opcode + both operand values), which is
how you read off the occurrence index to window on.

SOUNDNESS
---------
The hook only ever DECLINES an optimisation.  On the deny path it takes the
function's own pre-existing bail-out (the branch already used when both
operands are constants but their machine types disagree): the expression node
is left untouched and the constant operands stay as they were, so ugen emits
the real instruction instead of the folded constant.  It cannot delete, move
or invent code, and it changes nothing when the env var is unset.

Default-off verification: with ``IDO52_NO_BINFOLD_OPCODES`` unset the inserted
code is a single ``if (ido52_nobinfold_count)`` test that is false, so output
is bit-identical to the unhooked compiler.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UOPT_C = os.path.join(HERE, "build", "5.2", "uopt.c")
MARKER = "ido52_nobinfold_opcodes"

STATICS = """static uint8_t ido52_nobinfold_opcodes[32];   /* IDO52_NO_BINFOLD_OPCODES (Hook 47) */
static int ido52_nobinfold_from[32];
static int ido52_nobinfold_to[32];
static int ido52_nobinfold_seen[32];
static int ido52_nobinfold_count;
static int ido52_nobinfold_debug;
static int ido52_nobinfold_inited;
"""

INIT_FN = """static void ido52_nobinfold_init(void) {
    char *env;
    if (ido52_nobinfold_inited) return;
    ido52_nobinfold_inited = 1;
    env = getenv("IDO52_NO_BINFOLD_DEBUG");
    if (env && *env) ido52_nobinfold_debug = atoi(env);
    env = getenv("IDO52_NO_BINFOLD_OPCODES");
    if (env && *env) {
        char buf[256];
        char *tok;
        strncpy(buf, env, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\\0';
        tok = strtok(buf, ",");
        while (tok && ido52_nobinfold_count < 32) {
            int idx = ido52_nobinfold_count;
            int i, found = 0;
            char *c1 = strchr(tok, ':');
            ido52_nobinfold_from[idx] = 1;
            ido52_nobinfold_to[idx] = 0x7fffffff;
            ido52_nobinfold_seen[idx] = 0;
            if (c1) {
                char *c2;
                *c1 = '\\0';
                ido52_nobinfold_from[idx] = (int)strtol(c1 + 1, NULL, 0);
                c2 = strchr(c1 + 1, ':');
                if (c2) ido52_nobinfold_to[idx] = (int)strtol(c2 + 1, NULL, 0);
                else ido52_nobinfold_to[idx] = ido52_nobinfold_from[idx];
            }
            for (i = 0; _opc_names[i].name; i++) {
                if (strcmp(tok, _opc_names[i].name) == 0) {
                    ido52_nobinfold_opcodes[idx] = (uint8_t)_opc_names[i].value;
                    found = 1;
                    break;
                }
            }
            if (!found) ido52_nobinfold_opcodes[idx] = (uint8_t)atoi(tok);
            ido52_nobinfold_count++;
            tok = strtok(NULL, ",");
        }
        fprintf(stderr, "[NO-BINFOLD] %d opcode rule(s)\\n", ido52_nobinfold_count);
    }
}

/* Returns 1 to DECLINE the constant fold of this binary node. */
static int ido52_nobinfold_denied(uint32_t opc, uint32_t v1val, uint32_t v2val) {
    int i;
    if (ido52_nobinfold_debug > 1)
        fprintf(stderr, "[NO-BINFOLD] see opc=%u a=0x%x b=0x%x\\n",
                (unsigned)opc, (unsigned)v1val, (unsigned)v2val);
    for (i = 0; i < ido52_nobinfold_count; i++) {
        if ((uint32_t)ido52_nobinfold_opcodes[i] != opc) continue;
        ido52_nobinfold_seen[i]++;
        if (ido52_nobinfold_seen[i] < ido52_nobinfold_from[i]) continue;
        if (ido52_nobinfold_seen[i] > ido52_nobinfold_to[i]) continue;
        if (ido52_nobinfold_debug)
            fprintf(stderr, "[NO-BINFOLD] DENY opc=%u occ=%d a=0x%x b=0x%x\\n",
                    (unsigned)opc, ido52_nobinfold_seen[i],
                    (unsigned)v1val, (unsigned)v2val);
        return 1;
    }
    return 0;
}
"""

SITE_ANCHOR = ("L4151e8:\n"
               "//nop;\n"
               "a1 = MEM_U8(v1 + 1);\n"
               "MEM_U32(sp + 16) = v1;\n")

SITE_HOOK = """/* Hook 47: IDO52_NO_BINFOLD_OPCODES -- decline this constant fold.
   Guarded on both operands being literal-constant leaves of the same node
   class (type tag 1 or 2 -- 2 is what an integer literal carries here), which
   is the only shape this hook is for.  The deny path is the function's own
   "leave the node alone" bail-out -- the branch already taken at L4150f4 when
   two constant operands have differing machine types. */
ido52_nobinfold_init();
if ((ido52_nobinfold_count || ido52_nobinfold_debug > 1)
    && MEM_U8(a2 + 0) == MEM_U8(a3 + 0) && MEM_U8(a2 + 0) <= 2) {
    if (ido52_nobinfold_denied(a0, MEM_U32(a2 + 32), MEM_U32(a3 + 32))) {
        s2 = MEM_U32(sp + 192);
        goto L415214;
    }
}
"""


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print("[constfold hooks] Already applied to %s" % path)
        return True
    for needle in ("static const struct { const char *name; int value; } _opc_names[] = {",
                   "static void ido52_init_hooks(void) {",
                   SITE_ANCHOR, "L415214:"):
        if s.count(needle) != 1:
            print("[constfold hooks] ERROR: anchor not unique/found: %r (count %d)"
                  % (needle[:48], s.count(needle)))
            return False
    s = s.replace("static void ido52_init_hooks(void) {",
                  STATICS + INIT_FN + "static void ido52_init_hooks(void) {", 1)
    s = s.replace(SITE_ANCHOR, SITE_ANCHOR + SITE_HOOK, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print("[constfold hooks] Applied to %s: Hook 47 at the f_binaryfold call site" % path)
    return True


REBUILD = [
    ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/uopt.o", "build/5.2/uopt.c"],
    ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/out/uopt", "build/5.2/uopt.o", "build/5.2/version_info.o",
     "build/5.2/libc_impl_52.o", "-lm"],
]


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    path = args[0] if args else UOPT_C
    if not apply(path):
        sys.exit(1)
    if "--rebuild" in sys.argv:
        for cmd in REBUILD:
            r = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
            if r.returncode != 0:
                print(r.stderr[-3000:])
                sys.exit(1)
        print("[constfold hooks] uopt rebuilt")


if __name__ == "__main__":
    main()
