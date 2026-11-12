#!/usr/bin/env python3
"""apply_uopt_formmerge_hooks.py -- Hook 57: IDO52_FORM_MERGE, the
site-keyed constant-descriptor WEB MERGE (inverse of Hook 34 FORM_SPLIT).
Built for the `held_constant_zero` class (2026-09-24).

Run AFTER apply_uopt_websplit_hooks.py (it grafts onto Hook 30's statics and
sits beside Hook 34's walker-site shim) on build/5.2/uopt.c:

    python3 apply_uopt_formmerge_hooks.py [build/5.2/uopt.c] [--rebuild]

MECHANISM

cfe emits one constant DESCRIPTOR per (value, data type).  A literal 0 used
as an int (Jdt, type=6) and a literal 0 stored through a u8 lvalue (Ldt,
type=8) are two descriptors, so uopt's makelivranges walker (func_4623c4,
const case L462438) caches TWO web bits (`MEM_U16(desc+2)`) and forms two
webs.  Retail's uopt put both in ONE web: the int 0 the entry block loaded
into a register served the u8 store (`sb a1`) and the argument copy
(`move a0,a1`).  Ours gives the u8 zero its own web, which ugen colours
straight into the argument register and as1 then copy-propagates to $zero
(`move a0,zero; sb zero`).

The hook, at a site-keyed (value, statement) formation query with a
positive candidacy result whose descriptor has NO web yet, re-points the
use's itab node (node+20) at an EARLIER descriptor of the same value that
already owns a web: the walker's own join path (f_formlivbb on the
existing bit) then adds the use, and every consumer -- globalcolor and
the emitter's f_inreg(ichain, bb) register map -- sees the donor ichain.
(Copying only the donor's bit into desc+2 was tried first: the web forms
but emission still rematerialises, because f_inreg compares ichains.)
No output byte is patched; the register is whatever globalcolor gives
the merged web.  Only same-VALUE descriptors can donate.

ENV
  IDO52_FORM_MERGE=const:VAL:STMT[-STMTTO][,...]
      VAL decimal or 0x hex; STMT = Hook 30's statement ordinal from the
      IDO52_FORM_TRACE=1 `[FORMQ const]` lines (the FIRST query of the
      descriptor to merge -- the one printing bit=65535).  The donor is
      the first descriptor of the same value that already owns a web.
  IDO52_FORM_TRACE=1 (Hook 30) traces it:
      [FORM-MERGE] stmt=.. val=.. desc=.. -> joins bit .. of desc ..

Default OFF: with IDO52_FORM_MERGE unset ido52_fmerge_match() returns 0
before touching any state, and the recording shim is skipped, so output
is bit-identical to the unhooked build.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UOPT_C = os.path.join(HERE, "build", "5.2", "uopt.c")
MARKER = "IDO52_FORM_MERGE"

HELPER = r'''
/* --- Hook 57: IDO52_FORM_MERGE (2026-09-24) ---------------------------
 * Site-keyed constant web MERGE: at a matching (value, statement) query
 * whose descriptor has no web yet, adopt the web bit of an earlier
 * descriptor of the same value.  See apply_uopt_formmerge_hooks.py. */
static int ido52_fmerge_init_done;
static int ido52_fmerge_on;
static struct { uint32_t val; int stmt_from, stmt_to; } ido52_fmerge[IDO52_PFORM_MAX];
static int ido52_fmerge_count;
#define IDO52_FMERGE_SEEN 256
static struct { uint32_t val, desc; } ido52_fmerge_seen[IDO52_FMERGE_SEEN];
static int ido52_fmerge_nseen;
static int ido52_fmerge_enabled(void) {
    const char *e;
    char buf[512];
    char *p, *tok;
    if (ido52_fmerge_init_done) return ido52_fmerge_on;
    ido52_fmerge_init_done = 1;
    e = getenv("IDO52_FORM_MERGE");
    if (e && strlen(e) < sizeof(buf)) {
        strcpy(buf, e);
        p = buf;
        while ((tok = strtok(p, ",")) != NULL && ido52_fmerge_count < IDO52_PFORM_MAX) {
            char *c1, *c2, *dash;
            p = NULL;
            if (strncmp(tok, "const:", 6) != 0) continue;
            c1 = tok + 6;
            c2 = strchr(c1, ':');
            if (!c2) continue;
            *c2++ = 0;
            ido52_fmerge[ido52_fmerge_count].val = (uint32_t)strtoul(c1, NULL, 0);
            dash = strchr(c2, '-');
            ido52_fmerge[ido52_fmerge_count].stmt_from = (int)strtol(c2, NULL, 0);
            ido52_fmerge[ido52_fmerge_count].stmt_to =
                dash ? (int)strtol(dash + 1, NULL, 0)
                     : ido52_fmerge[ido52_fmerge_count].stmt_from;
            ido52_fmerge_count++;
        }
        fprintf(stderr, "[FORM-MERGE] %d merge site(s)\n", ido52_fmerge_count);
    }
    ido52_fmerge_on = ido52_fmerge_count > 0;
    return ido52_fmerge_on;
}
/* Record every const descriptor the walker queries (first sighting). */
static void ido52_fmerge_note(uint32_t val, uint32_t desc) {
    int i;
    if (!desc) return;
    for (i = 0; i < ido52_fmerge_nseen; i++)
        if (ido52_fmerge_seen[i].desc == desc) return;
    if (ido52_fmerge_nseen < IDO52_FMERGE_SEEN) {
        ido52_fmerge_seen[ido52_fmerge_nseen].val = val;
        ido52_fmerge_seen[ido52_fmerge_nseen].desc = desc;
        ido52_fmerge_nseen++;
    }
}
static void ido52_fmerge_apply(uint8_t *mem, uint32_t val, uint32_t node, uint32_t desc) {
    int i, hit = 0;
    for (i = 0; i < ido52_fmerge_count; i++) {
        if (ido52_fmerge[i].val != val) continue;
        if (ido52_pform_stmt < ido52_fmerge[i].stmt_from ||
            ido52_pform_stmt > ido52_fmerge[i].stmt_to) continue;
        hit = 1;
        break;
    }
    if (!hit || !desc || MEM_U16(desc + 2) != 0xffffu) return;
    for (i = 0; i < ido52_fmerge_nseen; i++) {
        uint32_t d = ido52_fmerge_seen[i].desc;
        if (d == desc || ido52_fmerge_seen[i].val != val) continue;
        if (MEM_U16(d + 2) == 0xffffu) continue;
        if (ido52_pform_trace)
            fprintf(stderr, "[FORM-MERGE] stmt=%d val=0x%x desc=0x%x -> joins bit %u of desc 0x%x\n",
                    ido52_pform_stmt, val, desc, MEM_U16(d + 2), d);
        MEM_U32(node + 20) = d;   /* re-point the use at the donor ichain */
        return;
    }
    if (ido52_pform_trace)
        fprintf(stderr, "[FORM-MERGE] stmt=%d val=0x%x desc=0x%x: no donor web\n",
                ido52_pform_stmt, val, desc);
}
'''

HELPER_ANCHOR = "static uint32_t ido52_pemit_query(uint32_t res, uint32_t val, uint32_t bb) {"

SITE_ANCHOR = """v0 = ido52_pform_query(0, v0, a2, a0, MEM_U8(sp + 103), MEM_U32(s0 + 20),
    MEM_U32(s0 + 20) ? MEM_U16(MEM_U32(s0 + 20) + 2) : 0xffffffffu);
"""
SITE_CODE = SITE_ANCHOR + """if (ido52_fmerge_enabled() && v0 != 0 && MEM_U32(s0 + 20) != 0) {
    ido52_fmerge_note(a2, MEM_U32(s0 + 20));
    ido52_fmerge_apply(mem, a2, s0, MEM_U32(s0 + 20));
}
"""

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
        print(f"[formmerge hooks] Already applied to {path}")
        return True
    if "ido52_fsplit_match" not in s:
        print("[formmerge hooks] ERROR: apply_uopt_websplit_hooks.py (Hook 34) first")
        return False
    for needle in (HELPER_ANCHOR, SITE_ANCHOR):
        if s.count(needle) != 1:
            print(f"[formmerge hooks] ERROR: anchor not unique/found: "
                  f"{needle[:40]!r} (count {s.count(needle)})")
            return False
    s = s.replace(HELPER_ANCHOR, HELPER + "\n" + HELPER_ANCHOR, 1)
    s = s.replace(SITE_ANCHOR, SITE_CODE, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[formmerge hooks] Hook 57 applied to {path}")
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
        print("[formmerge hooks] uopt rebuilt")


if __name__ == "__main__":
    main()
