#!/usr/bin/env python3
"""Hook 46: make the READ-PHASE uopt hooks reachable, plus IDO52_NO_SHARE_OPCODES.

Run AFTER apply_uopt_hooks.py (it needs ido52_init_hooks / _opc_names /
ido52_no_cse_mode to exist).  Idempotent: probes for its own marker.

    python3 tools/ido-static-recomp/apply_uopt_readphase_hooks.py [build/5.2/uopt.c] [--rebuild]

WHAT IT FIXES (measured 2026-08-30)
-----------------------------------
apply_uopt_hooks.py calls ido52_init_hooks() inside `oneproc`, AFTER the
procedure's U-code has been read (readnxtinst / extendstat / checkstatoccur
all run before that point).  Every hook whose site is in the read phase
therefore saw an EMPTY configuration -- Hook 1 (IDO52_NO_CSE /
IDO52_NO_CSE_OPCODES, site f_isearchloop) and IDO52_CSE_DEBUG never fired at
all.  Textbook proof: `A=x*y; if(X) B=x*y+2; C=x*y+3;` gave one multu with
IDO52_NO_CSE_OPCODES=mpy before this patch and three after.  Consequence:
every "IDO52_NO_CSE ... inert" refutation written before 2026-08-30 tested
nothing, and one file (TAIIKU/func_80148458) shipped an inert NO_CSE=1 that
became live and had to be retired.

This applier inserts a second, guarded ido52_init_hooks() call at the very
top of uopt's `main` (the function has a static once-guard, so the later
oneproc call is a no-op).  Env-off output is unchanged: verified on all 668
DATE functions via resolve_overlay_relocs (0 changes) and on every shipped
NO_CSE / NO_STRENGTH_REDUCE / CSE_MAX_REUSE user.

IDO52_NO_SHARE_OPCODES=op[,op...]
---------------------------------
A new primitive at the ichain hash-consing search (`func_43c2e0`, the search
that readnxtinst runs for every expression as the DAG is BUILT -- Hook 1's
site is the loop-invariant "i" table used by codemotion/copyprop, a
different table).  For the listed U-code opcodes it forces a FRESH ichain
instead of re-using the hash hit, so identical expression trees stay
separate objects.  Pair it with IDO52_NO_CSE_OPCODES to also stop codemotion
merging them later.  ⚠️ A multiply of a NAMED-global load by a constant never
reaches this hash (uopt strength-reduces `LOD*const` on a special path at
read time; only `ILOD*const` -- a pointer / raw-address read -- becomes a
real MPY node), so `mpy` here is inert for that shape.  IDO52_NO_SHARE_DEBUG=1
prints each hit; =2 prints every opcode reaching the search.
"""
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UOPT_C = os.path.join(HERE, "build", "5.2", "uopt.c")
MARKER = "ido52_noshare_opcode_list"

STATICS = """static uint8_t ido52_noshare_opcode_list[64];   /* IDO52_NO_SHARE_OPCODES (Hook 46) */
static int ido52_noshare_opcode_count;
static int ido52_noshare_debug;
"""

INIT_FN = """static int ido52_noshare_inited;
static void ido52_noshare_init(void) {
    char *env;
    if (ido52_noshare_inited) return;
    ido52_noshare_inited = 1;
    env = getenv("IDO52_NO_SHARE_OPCODES");
    if (env && *env) {
        char buf[256];
        int i;
        char *tok;
        strncpy(buf, env, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\\0';
        tok = strtok(buf, ",");
        while (tok && ido52_noshare_opcode_count < 64) {
            int found = 0;
            for (i = 0; _opc_names[i].name; i++) {
                if (strcmp(tok, _opc_names[i].name) == 0) {
                    ido52_noshare_opcode_list[ido52_noshare_opcode_count++] = (uint8_t)_opc_names[i].value;
                    found = 1;
                    break;
                }
            }
            if (!found) ido52_noshare_opcode_list[ido52_noshare_opcode_count++] = (uint8_t)atoi(tok);
            tok = strtok(NULL, ",");
        }
        fprintf(stderr, "[NO-SHARE] %d opcodes\\n", ido52_noshare_opcode_count);
    }
    env = getenv("IDO52_NO_SHARE_DEBUG");
    if (env && *env) ido52_noshare_debug = atoi(env);
}
"""

HOOK_SITE_ANCHOR = """L43c3ac:
//nop;
if ((int32_t)t9 != (int32_t)0) {
//nop;
goto L43cfe4;}
"""

HOOK_SITE = """/* Hook 46: IDO52_NO_SHARE_OPCODES -- force a fresh ichain for listed opcodes */
ido52_noshare_init();
if (ido52_noshare_opcode_count) {
    uint8_t _op = MEM_U8(s1 + 0);
    int _i;
    if (ido52_noshare_debug > 1) fprintf(stderr, "[NO-SHARE] see opc=%u mtype=%u bucket=%u\\n", _op, MEM_U8(s1 + 1) & 0x1f, s0);
    for (_i = 0; _i < ido52_noshare_opcode_count; _i++) {
        if (_op == ido52_noshare_opcode_list[_i]) {
            if (ido52_noshare_debug) fprintf(stderr, "[NO-SHARE] opc=%u bucket=%u -> fresh\\n", _op, s0);
            s0 = 0;
            break;
        }
    }
}
"""

MAIN_ANCHOR = "L45ba98:\n//main:\n"
MAIN_INIT = "ido52_init_hooks();   /* Hook 46: early init -- read-phase hooks (Hook 1 etc.) run before oneproc */\n"

REBUILD = [
    ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/uopt.o", "build/5.2/uopt.c"],
    ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/out/uopt", "build/5.2/uopt.o", "build/5.2/version_info.o",
     "build/5.2/libc_impl_52.o", "-lm"],
]


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print(f"[readphase hooks] Already applied to {path}")
        return True
    for needle in ("static int ido52_no_cse_mode;", "static void ido52_init_hooks(void) {",
                   HOOK_SITE_ANCHOR, MAIN_ANCHOR, "_opc_names[] = {"):
        if s.count(needle) != 1:
            print(f"[readphase hooks] ERROR: anchor not unique/found: {needle[:40]!r} (count {s.count(needle)})")
            return False
    s = s.replace("static int ido52_no_cse_mode;", "static int ido52_no_cse_mode;\n" + STATICS, 1)
    s = s.replace("static void ido52_init_hooks(void) {", INIT_FN + "static void ido52_init_hooks(void) {", 1)
    s = s.replace(HOOK_SITE_ANCHOR, HOOK_SITE_ANCHOR + HOOK_SITE, 1)
    s = s.replace(MAIN_ANCHOR, MAIN_ANCHOR + MAIN_INIT, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[readphase hooks] Applied to {path}: early init at main, NO_SHARE hook at func_43c2e0")
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
        print("[readphase hooks] uopt rebuilt")


if __name__ == "__main__":
    main()
