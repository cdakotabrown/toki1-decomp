#!/usr/bin/env python3
"""Hook 1b: IDO52_NO_CSE_SITE -- per-site veto of the f_isearchloop CSE REUSE.

Run AFTER apply_uopt_hooks.py and apply_uopt_readphase_hooks.py (it needs
_opc_names and the Hook 1 site in f_isearchloop; the early init of Hook 46 is
not required -- this hook parses its own env lazily).  Idempotent.

    python3 tools/ido-static-recomp/apply_uopt_nocse_site_hooks.py [build/5.2/uopt.c] [--rebuild]

WHAT IT DOES
------------
Hook 1 (IDO52_NO_CSE / IDO52_NO_CSE_OPCODES) empties the hash bucket at the
top of f_isearchloop for EVERY expression of a type / opcode, so it cannot
deny one CSE while keeping another of the same opcode.  This hook sits at the
search's RESULT (L447124, the same point as IDO52_CSE_MAX_REUSE): when the
search found an existing entry (t0 != 0, s0 = the entry) it counts a HIT for
that expression's opcode (MEM_U8(expr + 32); types 4 binop, 5 unop, 6 ixa),
per procedure, 1-based, in f_isearchloop call order.  A keyed hit is turned
into a miss (t0 = s0 = 0), and the function's own miss path
(f_appendichain) creates a fresh entry -- exactly what Hook 1 does for every
occurrence, restricted to one.

    IDO52_NO_CSE_SITE=OPC:OCC[-OCCTO][,...]
        OPC  = opcode name from Hook 1's table (mpy, add, ixa, ...), a number,
               or `*` (any opcode; OCC then counts all hits of types 4/5/6)
        OCC  = 1-based hit ordinal for that OPC in the procedure
    IDO52_NO_CSE_SITE_TRACE=1
        [CSE-SITE] hit#N opc=O(name) occ=K type=T bb=B entry=0x.. [VETO]
        for every hit (whether or not keyed).  Run it first: entries with
        the same `entry=` are the same expression, `bb` is the current
        graph node (MEM_U16(curgraphnode + 8)) when the query ran.

The counters reset at the top of f_oneproc (per procedure); the key applies
in every procedure of the TU.  Unset: the shim is one test of a static int.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UOPT_C = os.path.join(HERE, "build", "5.2", "uopt.c")
MARKER = "ido52_nocse_site_q"

HELPER_ANCHOR = "static void ido52_init_hooks(void) {"

HELPER = r"""/* ---- Hook 1b: IDO52_NO_CSE_SITE -- per-site veto of an f_isearchloop hit ---- */
#define IDO52_NCS_MAX 32
static int ido52_ncs_inited, ido52_ncs_n, ido52_ncs_trace;
static int ido52_ncs_opc[IDO52_NCS_MAX];      /* -1 = any */
static int ido52_ncs_lo[IDO52_NCS_MAX], ido52_ncs_hi[IDO52_NCS_MAX];
static int ido52_ncs_cnt[257];                /* per-opcode hit count; [256] = any */
static int ido52_ncs_total;
static void ido52_ncs_init(void) {
    char *env;
    if (ido52_ncs_inited) return;
    ido52_ncs_inited = 1;
    env = getenv("IDO52_NO_CSE_SITE_TRACE");
    if (env && *env) ido52_ncs_trace = 1;
    env = getenv("IDO52_NO_CSE_SITE");
    if (env && *env) {
        char buf[512];
        char *tok;
        strncpy(buf, env, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        for (tok = strtok(buf, ","); tok && ido52_ncs_n < IDO52_NCS_MAX; tok = strtok(NULL, ",")) {
            char *colon = strchr(tok, ':');
            char *dash;
            int i, opc = -2;
            if (!colon) { fprintf(stderr, "[CSE-SITE] bad token %s (want OPC:OCC)\n", tok); continue; }
            *colon = '\0';
            if (strcmp(tok, "*") == 0) opc = -1;
            for (i = 0; opc == -2 && _opc_names[i].name; i++)
                if (strcmp(tok, _opc_names[i].name) == 0) opc = _opc_names[i].value;
            if (opc == -2) opc = (int)strtol(tok, NULL, 0);
            ido52_ncs_opc[ido52_ncs_n] = opc;
            ido52_ncs_lo[ido52_ncs_n] = atoi(colon + 1);
            dash = strchr(colon + 1, '-');
            ido52_ncs_hi[ido52_ncs_n] = dash ? atoi(dash + 1) : ido52_ncs_lo[ido52_ncs_n];
            ido52_ncs_n++;
        }
        fprintf(stderr, "[CSE-SITE] %d key(s)\n", ido52_ncs_n);
    }
}
static void ido52_ncs_reset(void) {
    ido52_ncs_init();
    if (!ido52_ncs_n && !ido52_ncs_trace) return;
    memset(ido52_ncs_cnt, 0, sizeof(ido52_ncs_cnt));
    ido52_ncs_total = 0;
}
static const char *ido52_ncs_name(int opc) {
    int i;
    for (i = 0; _opc_names[i].name; i++) if (_opc_names[i].value == opc) return _opc_names[i].name;
    return "?";
}
/* called at L447124 on a HIT (t0 != 0); returns 1 to turn it into a miss */
static int ido52_nocse_site_q(uint8_t *mem, uint32_t expr, uint32_t entry) {
    int type, opc, occ, any, i, veto = 0;
    uint32_t node;
    if (!ido52_ncs_inited) ido52_ncs_init();
    if (!ido52_ncs_n && !ido52_ncs_trace) return 0;
    type = MEM_U8(expr + 0);
    if (type != 4 && type != 5 && type != 6) return 0;
    opc = MEM_U8(expr + 32);
    occ = ++ido52_ncs_cnt[opc];
    any = ++ido52_ncs_cnt[256];
    ido52_ncs_total++;
    for (i = 0; i < ido52_ncs_n; i++) {
        int k = ido52_ncs_opc[i] == -1 ? any : (ido52_ncs_opc[i] == opc ? occ : 0);
        if (k && k >= ido52_ncs_lo[i] && k <= ido52_ncs_hi[i]) { veto = 1; break; }
    }
    if (ido52_ncs_trace) {
        node = MEM_U32(0x1001c8a0);
        fprintf(stderr, "[CSE-SITE] hit#%d opc=%d(%s) occ=%d any=%d type=%d bb=%d entry=0x%x%s\n",
                ido52_ncs_total, opc, ido52_ncs_name(opc), occ, any, type,
                node ? (int)MEM_U16(node + 8) : -1, entry, veto ? " VETO" : "");
    }
    return veto;
}

"""

SITE_ANCHOR = "L447124:\n/* IDO52_CSE_MAX_REUSE hook: limit expression reuse count */\n"
SITE_CODE = SITE_ANCHOR + (
    "if ((int32_t)t0 != 0 && s0 != 0 && ido52_nocse_site_q(mem, s1, s0)) { t0 = 0; s0 = 0; }  /* Hook 1b */\n")

RESET_ANCHOR = "L45a150:\n//oneproc:\n"
RESET_CODE = RESET_ANCHOR + "ido52_ncs_reset();  /* Hook 1b */\n"

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
        print(f"[nocse-site hook] Already applied to {path}")
        return True
    for needle in (HELPER_ANCHOR, SITE_ANCHOR, "_opc_names[] = {"):
        if s.count(needle) != 1:
            print(f"[nocse-site hook] ERROR: anchor not unique/found: {needle[:50]!r} (count {s.count(needle)})")
            return False
    # the oneproc label appears once as a label (plus possibly in comments)
    if s.count(RESET_ANCHOR) < 1:
        print("[nocse-site hook] ERROR: f_oneproc entry anchor not found")
        return False
    s = s.replace(HELPER_ANCHOR, HELPER + HELPER_ANCHOR, 1)
    s = s.replace(SITE_ANCHOR, SITE_CODE, 1)
    i = s.index("static void f_oneproc(uint8_t *mem, uint32_t sp) {")
    j = s.index(RESET_ANCHOR, i)
    s = s[:j] + RESET_CODE + s[j + len(RESET_ANCHOR):]
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[nocse-site hook] Applied to {path}: veto at f_isearchloop L447124, reset at f_oneproc")
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
        # copy-then-rename: another process may be compiling right now
        os.replace(os.path.join(HERE, "build/5.2/out/uopt.new"),
                   os.path.join(HERE, "build/5.2/out/uopt"))
        print("[nocse-site hook] uopt rebuilt and installed (atomic rename)")


if __name__ == "__main__":
    main()
