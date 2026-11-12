#!/usr/bin/env python3
"""Hook 73: IDO52_NO_ENTRY_HOME_STORE -- veto uopt's entry home store of a
register-passed parameter (per occurrence, never a default).

Order-independent among the uopt appliers: its two anchors are f_prolog's
own labels (L4201d4 entry, L421140 the single-word home `rstr`), which no
other applier touches.  Must run after the uopt.c regen.  Idempotent: probes
for its own marker.  Part of the post-`make clean` chain (CLAUDE.md Known
Gotchas / UOPT-HOOKS-REFERENCE Hook 73).

    python3 tools/ido-static-recomp/apply_uopt_entryhome_hooks.py [build/5.2/uopt.c] [--rebuild]

WHERE THE STORE COMES FROM (measured 2026-09-26)
------------------------------------------------------------
For `unsigned f(unsigned s){ s = (s + 8) & ~7; g(&s); return s; }` our
5.2 emits `sw a0,24(sp)` right after the prologue although that value is
dead on every path.  A uwrite trace of uopt's OUTPUT U-code shows the store
is an `rstr` (opcode 112, lexlev = machine register 4, memory = the
parameter's home) written by uopt's f_prolog -- the procedure-entry code
uopt generates AFTER every optimisation pass, when it writes the procedure
back out.  f_prolog walks the formals and, for each register-passed formal
whose home must hold its value (the parameter-table flag byte +3: the
formal's memory is referenced -- here because its address escapes -- or it
got no register at all), emits `rstr $aN -> home` at L421140 (single-word
formal) or a split sequence at L42115c (multi-word formal).  Because it is
emitted at write-out time, no uopt pass ever sees it, so no dead-store pass
can delete it: the "dead-store route" and the "emission veto" are the same
site.  ugen translates the `rstr` 1:1 into `sw $aN,off(sp)` (its own
parameter homing, f_home_parameters, is not involved: it only fires for
varargs / registers in the 0x100194a0 set).

THE HOOK
--------
IDO52_NO_ENTRY_HOME_STORE=all | occ:N[,M...]  (also bare N[,M...])
    Suppress the single-word entry-home `rstr` at f_prolog L421140.
    N is the 1-based ordinal of that `rstr` in the translation unit, in
    emission order (procedure order, then formal order); the TRACE prints
    it.  Multi-word formals (the L42115c path) are never touched.
IDO52_NO_ENTRY_HOME_STORE_TRACE=1
    [ENTRY-HOME] line per candidate: occ, procedure ordinal, register,
    home offset/size, KEEP or VETO.

SOUNDNESS CONDITION -- the hook does not check it; the user asserts it
by naming the occurrence:
    every path from procedure entry to any READ of the parameter's home
    (a direct reload, or any call/indirect access that can see it through
    its escaped address) passes a WRITE of the home first.
Under that condition the entry store is dead and removing it is ordinary
dead-store elimination.  If the incoming value itself is needed in memory
on some path (e.g. `g(&s)` before any assignment of `s`), the veto
MISCOMPILES.  Deleting the store never adds or rewrites a word; it is the
compiler decision retail's compiler made (SpuSetTransferStartAddr: retail's first
store to the home is the rounded value in each arm).
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UOPT_C = os.path.join(HERE, "build", "5.2", "uopt.c")
MARKER = "ido52_h73_entryhome"

HELPERS = r"""/* ---- Hook 73: IDO52_NO_ENTRY_HOME_STORE (f_prolog entry-home rstr veto) ---- */
static int ido52_h73_entryhome_inited;
static int ido52_h73_all;
static int ido52_h73_trace;
static int ido52_h73_n;
static unsigned ido52_h73_occs[64];
static unsigned ido52_h73_occ;
static unsigned ido52_h73_proc;
static void ido52_h73_init(void) {
    char *env;
    if (ido52_h73_entryhome_inited) return;
    ido52_h73_entryhome_inited = 1;
    env = getenv("IDO52_NO_ENTRY_HOME_STORE_TRACE");
    if (env && *env && strcmp(env, "0") != 0) ido52_h73_trace = 1;
    env = getenv("IDO52_NO_ENTRY_HOME_STORE");
    if (env && *env) {
        char buf[256];
        char *tok;
        strncpy(buf, env, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (strcmp(buf, "all") == 0) {
            ido52_h73_all = 1;
        } else {
            tok = strtok(buf, ",");
            while (tok && ido52_h73_n < 64) {
                if (strncmp(tok, "occ:", 4) == 0) tok += 4;
                if (atoi(tok) > 0) ido52_h73_occs[ido52_h73_n++] = (unsigned)atoi(tok);
                tok = strtok(NULL, ",");
            }
        }
        fprintf(stderr, "[ENTRY-HOME] IDO52_NO_ENTRY_HOME_STORE: %s (%d occurrence(s))\n",
                ido52_h73_all ? "all" : "listed", ido52_h73_n);
    }
}
/* returns 1 to veto this entry-home rstr; rec = the U-code record (0x10011f90) */
static int ido52_h73_veto(uint8_t *mem, uint32_t rec) {
    int i, veto = 0;
    ido52_h73_init();
    ido52_h73_occ++;
    if (ido52_h73_all) veto = 1;
    for (i = 0; i < ido52_h73_n; i++) if (ido52_h73_occs[i] == ido52_h73_occ) veto = 1;
    if (ido52_h73_trace)
        fprintf(stderr, "[ENTRY-HOME] occ=%u proc=%u rstr $%u -> blk=%u off=%d size=%u : %s\n",
                ido52_h73_occ, ido52_h73_proc, (unsigned)MEM_U16(rec + 2),
                (unsigned)MEM_U32(rec + 4), (int)MEM_U32(rec + 12), (unsigned)MEM_U32(rec + 8),
                veto ? "VETO" : "KEEP");
    return veto;
}
"""

HELPER_ANCHOR = "static void f_prolog(uint8_t *mem, uint32_t sp) {\n"

PROLOG_ANCHOR = "L4201d4:\n//prolog:\n"
PROLOG_CODE = PROLOG_ANCHOR + "ido52_h73_proc++;   /* Hook 73: procedure ordinal for the trace */\n"

SITE_ANCHOR = ("L421140:\n//nop;\n//nop;\n//nop;\n"
               "f_uwrite(mem, sp, a0, a1, a2, a3);\ngoto L421150;\n")
SITE_CODE = ("L421140:\n//nop;\n//nop;\n//nop;\n"
             "/* Hook 73: IDO52_NO_ENTRY_HOME_STORE -- veto the entry-home rstr (a0 = record) */\n"
             "if (!ido52_h73_veto(mem, a0))\n"
             "f_uwrite(mem, sp, a0, a1, a2, a3);\ngoto L421150;\n")

REBUILD = [
    ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/uopt.o", "build/5.2/uopt.c"],
    ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/out/uopt.new", "build/5.2/uopt.o", "build/5.2/version_info.o",
     "build/5.2/libc_impl_52.o", "-lm"],
]


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print(f"[entryhome hooks] Already applied to {path}")
        return True
    # the forward declaration line ends in ');', the definition in ') {'
    for needle in (HELPER_ANCHOR, PROLOG_ANCHOR, SITE_ANCHOR):
        if s.count(needle) != 1:
            print(f"[entryhome hooks] ERROR: anchor not unique/found: "
                  f"{needle[:50]!r} (count {s.count(needle)})")
            return False
    s = s.replace(HELPER_ANCHOR, HELPERS + HELPER_ANCHOR, 1)
    s = s.replace(PROLOG_ANCHOR, PROLOG_CODE, 1)
    s = s.replace(SITE_ANCHOR, SITE_CODE, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[entryhome hooks] Hook 73 applied to {path}")
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
        # install by copy-then-rename so a concurrent compile never sees a
        # half-written binary
        os.replace(os.path.join(HERE, "build/5.2/out/uopt.new"),
                   os.path.join(HERE, "build/5.2/out/uopt"))
        print("[entryhome hooks] uopt rebuilt")


if __name__ == "__main__":
    main()
