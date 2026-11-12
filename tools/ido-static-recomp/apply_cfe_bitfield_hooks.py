#!/usr/bin/env python3
"""Hook 68: IDO52_CFE_BITFIELD_WORD -- keep a bitfield STORE's read-modify-
write in the field's word instead of the smallest byte/halfword holding it
(the retail library-region compiler's form).

Idempotent (probes for its own marker); independent of Hook 67 (both patch
cfe, at unrelated functions).  Part of the post-`make clean` chain
(CLAUDE.md Known Gotchas / UOPT-HOOKS-REFERENCE Hook 68).

    python3 tools/ido-static-recomp/apply_cfe_bitfield_hooks.py [--cfe PATH] [--rebuild]

THE DECISION (cfe, func_47a970 -- the bitfield-assignment codegen; its only
caller is func_487850, the store of an assignment, so READS never reach it
and stay word-wide `lw ; sll ; srl`)
----------------------------------------------------------------------------
func_47a970 picks the access unit s2 (a type) for the RMW from the field's
width w (sp+108) and bit offset off (sp+124).  First the exact fits (w ==
8/16/32/.. at a matching boundary -> that type).  Otherwise, at L47ad64:

    if (off % 8  + w <= 8)  unit = (u)char    -> lbu ; andi ; ori ; sb
    elif (off % 16 + w <= 16) unit = (u)short -> lhu ; andi ; ori ; sh
    elif (off % 32 + w <= 32) unit = (u)int   (L47ae7c)
    ...

The hook sends a selected store from L47ad64 straight to L47ae7c (the word
test), so the unit is the 32-bit word: `lw ; li at,~mask ; and ; ori ; sw`.
The exact-fit branches (a whole aligned byte/half/word field) are not
touched: no retail evidence covers them.  Byte order is irrelevant to
soundness -- the word holds the same bits the byte did; the load and store
simply cover the whole aligned word.

ENV
---
IDO52_CFE_BITFIELD_WORD=all | occ:N[,N..]   N = 0-based index over the
    TU's bitfield stores that reach L47ad64 (the non-exact-fit ones), in
    codegen order.  A bare number is REFUSED (warning, stock) -- it is not
    an on/off switch.
IDO52_CFE_BITFIELD_TRACE=1   `[CFE-BFSTORE] #N size=<bits> off=<bit>
    unit=<byte|half|word|wider> -> KEPT WORD|STOCK` per such store.
Default OFF.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
CFE_C = os.path.join(HERE, "build", "5.2", "cfe.c")

MARKER = "cfbf_word"

HELPER_ANCHOR = ("static void f_LDC_integer(uint8_t *mem, uint32_t sp, uint32_t a0, "
                 "uint32_t a1, uint32_t a2, uint32_t a3) {\nconst")
FUNC_ANCHOR = ("static void func_47a970(uint8_t *mem, uint32_t sp, uint32_t a0, "
               "uint32_t a1, uint32_t a2, uint32_t a3) {\nconst")

HELPERS = r"""/* --- IDO52_CFE_BITFIELD_WORD (Hook 68, 2026-09-25): keep a bitfield
 * store's RMW unit at the word (func_47a970, L47ad64 -> L47ae7c).  See
 * apply_cfe_bitfield_hooks.py.  Default OFF. --- */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
static int cfbf_inited, cfbf_all, cfbf_trace, cfbf_idx = -1;
static const char *cfbf_list;
/* ucode cursor right after the last f_LDC_integer record, and whether the
 * current (selected) store's value is that constant: then the store takes
 * 5.2's own and/or insert path (L47b374) at the word, as retail does. */
static uint32_t cfbf_ldc_end = 0xffffffff;
static int cfbf_andor;

static int cfbf_word(uint8_t *mem, uint32_t w, uint32_t off) {
    int sel = 0;
    const char *unit;
    if (!cfbf_inited) {
        const char *e = getenv("IDO52_CFE_BITFIELD_WORD");
        const char *t = getenv("IDO52_CFE_BITFIELD_TRACE");
        cfbf_inited = 1;
        if (e != NULL && *e) {
            if (strcmp(e, "all") == 0) cfbf_all = 1;
            else if (strncmp(e, "occ:", 4) == 0) cfbf_list = e + 4;
            else fprintf(stderr, "[CFE-BFSTORE] IDO52_CFE_BITFIELD_WORD=%s refused "
                                 "(need all | occ:N[,N..]); stock\n", e);
        }
        cfbf_trace = (t != NULL && *t && strcmp(t, "0") != 0);
    }
    cfbf_idx++;
    if (cfbf_all) sel = 1;
    else if (cfbf_list != NULL) {
        const char *p = cfbf_list;
        while (*p) {
            char *end;
            long v = strtol(p, &end, 10);
            if (end == p) { p++; continue; }
            if (v == cfbf_idx) { sel = 1; break; }
            p = end;
        }
    }
    cfbf_andor = sel && MEM_U32(0x1002f620) == cfbf_ldc_end;
    if (cfbf_trace) {
        if ((int)(off % 8) + (int)w <= 8) unit = "byte";
        else if ((int)(off % 16) + (int)w <= 16) unit = "half";
        else if ((int)(off % 32) + (int)w <= 32) unit = "word";
        else unit = "wider";
        fprintf(stderr, "[CFE-BFSTORE] #%d size=%u off=%u unit=%s value=%s -> %s\n",
                cfbf_idx, w, off, unit,
                MEM_U32(0x1002f620) == cfbf_ldc_end ? "const" : "expr",
                !sel ? "STOCK" : cfbf_andor ? "KEPT WORD (and/or)" : "KEPT WORD (xor)");
    }
    return sel;
}

"""

SITE_OLD = "L47ad64:\nlo = (int)a3 / (int)a1; hi = (int)a3 % (int)a1;\n"
SITE_NEW = ("L47ad64:\n"
            "/* === IDO52_CFE_BITFIELD_WORD (Hook 68): a3 = bit offset, sp+108 = width */\n"
            "if (cfbf_word(mem, MEM_U32(sp + 108), a3)) goto L47ae7c;\n"
            "lo = (int)a3 / (int)a1; hi = (int)a3 % (int)a1;\n")

REBUILD = [
    ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/cfe.o", "build/5.2/cfe.c"],
    ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/out/cfe.new", "build/5.2/cfe.o", "build/5.2/version_info.o",
     "build/5.2/libc_impl_52.o", "-lm"],
]


# (old, new, count) inside func_47a970 / f_LDC_integer
FUNC_EDITS = [
    ("func_47a970", "at = t2 < 0x11;\n",
     "at = (t2 < 0x11) || cfbf_andor;   /* Hook 68: word unit, constant value */\n", 1),
    ("func_47a970", "t3 = a1 ^ 0xffff;\n",
     "t3 = a1 ^ (cfbf_andor ? 0xffffffff : 0xffff);   /* Hook 68 */\n", 1),
    ("func_47a970", "t1 = a0 ^ 0xffff;\n",
     "t1 = a0 ^ (cfbf_andor ? 0xffffffff : 0xffff);   /* Hook 68 */\n", 1),
    ("f_LDC_integer", "return;\n",
     "cfbf_ldc_end = MEM_U32(0x1002f620);   /* Hook 68 */\nreturn;\n", 1),
]


def func_span(s, name):
    hdr = "static void %s(uint8_t *mem" % name
    start = s.index(hdr)
    while s[s.index("\n", start) - 1] != "{":     # skip the prototype
        start = s.index(hdr, start + 1)
    return start, s.index("\nstatic ", start + 1)


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print(f"[cfe bitfield hooks] Hook 68 already applied to {path}")
        return True
    for needle in (HELPER_ANCHOR, FUNC_ANCHOR, SITE_OLD, "L47ae7c:\n"):
        if s.count(needle) != 1:
            print(f"[cfe bitfield hooks] ERROR: anchor not unique/found: "
                  f"{needle[:48]!r} (count {s.count(needle)})")
            return False
    start = s.index(FUNC_ANCHOR)
    end = s.index("\nstatic ", start + 1)
    if not (start < s.index(SITE_OLD) < end and start < s.index("L47ae7c:\n") < end):
        print("[cfe bitfield hooks] ERROR: L47ad64/L47ae7c not inside func_47a970")
        return False
    for fn, old, new, n in FUNC_EDITS:
        a, b = func_span(s, fn)
        if s[a:b].count(old) != n:
            print(f"[cfe bitfield hooks] ERROR: {old!r} in {fn}: {s[a:b].count(old)} (want {n})")
            return False
    for fn, old, new, n in FUNC_EDITS:
        a, b = func_span(s, fn)
        s = s[:a] + s[a:b].replace(old, new) + s[b:]
    s = s.replace(SITE_OLD, SITE_NEW, 1)
    s = s.replace(HELPER_ANCHOR, HELPERS + HELPER_ANCHOR, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[cfe bitfield hooks] Hook 68 applied to {path}")
    return True


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
        print("[cfe bitfield hooks] cfe rebuilt")


if __name__ == "__main__":
    main()
