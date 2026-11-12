#!/usr/bin/env python3
"""Hook 78: IDO52_CFE_SWITCH_TABLE_MIN -- the smallest case cluster cfe
lowers to a jump table, per switch statement.

Idempotent (probes for its own marker).  Requires Hook 67
(apply_cfe_switchchain_hooks.py) first: it anchors on the two cluster tests
Hook 67 already rewrote and reuses Hook 67's per-TU switch counter
(`cfsw_idx`).  Part of the post-`make clean` chain, right after Hook 67.

    python3 tools/ido-static-recomp/apply_cfe_switchmin_hooks.py [--cfe PATH] [--rebuild]

THE DECISION
------------
func_4648f8 (switch codegen) groups the sorted case values into clusters (a
value within 25 of the running cluster's top extends it).  A cluster that
closes with FEWER THAN 5 values is split into single-value compare records
(`at = a1 < 0x5`, sites L464c48 / L464e88); a surviving cluster becomes an
XJP jump table.  So IDO 5.2 lowers every 4-label switch to a compare ladder.
The retail LIBRARY band was not all compiled with that threshold: ResetGraph
(0x8009FB74 dispatch, table at 0x800B9650) uses a 4-entry jump table for a
4-label `switch (arg0 & 3)` -- the C used to add an unreachable `case 4:` to
get a table, which made its .rodata one entry longer than retail's.

ENV
---
IDO52_CFE_SWITCH_TABLE_MIN=N | IDX:N[,IDX:N..]
    N replaces the 5 for every switch of the TU; IDX:N only for the IDX-th
    switch statement (0-based, source order, the same numbering as
    IDO52_CFE_SWITCH_CHAIN / IDO52_CFE_SWITCH_TRACE).  N in 1..64.

SOUNDNESS: a jump table and a compare ladder dispatch every value to the
same case label; the hook only chooses between cfe's own two lowerings.
Default OFF: with the env unset the threshold is the stock 5 and every
object is unchanged.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
CFE_C = os.path.join(HERE, "build", "5.2", "cfe.c")

MARKER = "cfsw_tmin"
OLD = "at = (a1 < 0x5) || cfsw_sel;\n"
NEW = "at = (a1 < cfsw_tmin()) || cfsw_sel;\n"
HELPER_ANCHOR = "static void cfsw_enter(uint8_t *mem, uint32_t stmt) {\n"

HELPERS = r"""/* --- IDO52_CFE_SWITCH_TABLE_MIN (Hook 78, 2026-10-03): minimum cluster
 * size for a jump table, per switch (stock 5).  Default OFF. --- */
static int cfsw_tmin(void) {
    static int inited;
    static const char *spec;
    const char *p;
    int best = 5;
    if (!inited) {
        inited = 1;
        spec = getenv("IDO52_CFE_SWITCH_TABLE_MIN");
    }
    if (spec == NULL || *spec == '\0') return 5;
    for (p = spec; *p; ) {
        char *e;
        long a = strtol(p, &e, 10), n;
        if (e == p) break;
        if (*e == ':') {
            n = strtol(e + 1, &e, 10);
            if (a == cfsw_idx && n >= 1 && n <= 64) best = (int)n;
        } else if (a >= 1 && a <= 64) {
            best = (int)a;
        }
        p = e;
        if (*p == ',') p++;
    }
    return best;
}
"""


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print(f"[cfe switchmin hooks] Hook 78 already applied to {path}")
        return True
    if s.count(OLD) != 2:
        print(f"[cfe switchmin hooks] ERROR: expected the 2 Hook 67 cluster tests, "
              f"found {s.count(OLD)} -- run apply_cfe_switchchain_hooks.py first")
        return False
    if s.count(HELPER_ANCHOR) != 1:
        print("[cfe switchmin hooks] ERROR: cfsw_enter definition anchor not unique")
        return False
    s = s.replace(OLD, NEW)
    s = s.replace(HELPER_ANCHOR, HELPERS + HELPER_ANCHOR, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[cfe switchmin hooks] Hook 78 applied to {path}")
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
        print("[cfe switchmin hooks] cfe rebuilt")


if __name__ == "__main__":
    main()
