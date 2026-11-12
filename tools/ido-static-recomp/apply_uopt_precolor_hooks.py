#!/usr/bin/env python3
"""apply_uopt_precolor_hooks.py -- Hook 30: pre-colouring web-formation
trace + site-keyed formation override (IDO52_FORM_TRACE / IDO52_FORM_SITE).

Run AFTER apply_uopt_hooks.py and apply_uopt_defplace_hooks.py on the
generated build/5.2/uopt.c, then rebuild uopt:

    python3 apply_uopt_precolor_hooks.py [build/5.2/uopt.c]
    gcc -c -std=c11 -O0 -ggdb3 -fno-strict-aliasing -I. \
        -o build/5.2/uopt.o build/5.2/uopt.c
    gcc -std=c11 -O0 -ggdb3 -fno-strict-aliasing -I. -o build/5.2/out/uopt \
        build/5.2/uopt.o build/5.2/version_info.o build/5.2/libc_impl_52.o -lm

WHAT IT PATCHES (all env-gated, default-off, bit-identical when unset):

1. A statement tracker in f_makelivranges (reset at entry L465ec0,
   increment at the per-statement loop head L4669b8).  Statement ordinals
   are derived from the fixed itab walk, so they DO NOT feedback-renumber
   when queries are forced (unlike f_constinreg occurrence windows).
2. Trace + override shims on the two FORMATION-phase register-candidacy
   queries inside the walker func_4623c4:
     - const site (L462438 -> f_constinreg)
     - LDA   site (L462524 -> f_ldainreg)
   and trace-only shims on the two func_462260 const sites.
3. Trace-only [NEWBIT] lines after every f_newbit call site (tagged with
   the containing recompiled function), so a web bit's creation site and
   creation-time statement can be identified.
4. Trace-only [FORMLIV] line at f_formlivbb entry (trace level >= 2).

ENV:
  IDO52_FORM_TRACE=1|2      trace formation queries/newbit (2: +FORMLIV)
  IDO52_FORM_SITE=spec[,spec...]   site-keyed formation override
      spec := kind:val:stmt[-stmtTo]:res
      kind := const | lda      (lda: val matches the frame offset word
                                printed by the trace, e.g. 0xfffffefc)
      stmt := statement ordinal from the trace ('*' = any statement)
      res  := 0 (deny web formation) | 1 (force web formation)
  IDO52_EMIT_SITE=spec[,spec...]   site-keyed EMISSION-phase override
      spec := const:val:bb:occ:res
      bb   := the reemission bb number (the Q1 trace's bb= field; '*' any)
      occ  := 1-based occurrence of that (val,bb) pair per proc, 0 = all
      res  := 0 (force loadnum) | 1 (force register-candidacy: f_inreg is
              consulted, emitting REGOP when the web is bound in that bb)
      A formation-phase force (FORM_SITE ... :1) usually needs its
      emission mirror forced too -- Hook 24 section 2 documents the split.
"""
import re
import sys
import shutil

path = sys.argv[1] if len(sys.argv) > 1 else "build/5.2/uopt.c"
src = open(path).read()

if "IDO52_FORM_TRACE" in src:
    print("Hook 30 (FORM_TRACE/FORM_SITE) already present; nothing to do.")
    sys.exit(0)

shutil.copyfile(path, path + ".pre-precolor")

# ------------------------------------------------------------------ statics
STATICS = r'''
/* --- Hook 30: IDO52_FORM_TRACE / IDO52_FORM_SITE ---------------------
 * Pre-colouring web-formation trace and site-keyed override.
 * Formation-phase register-candidacy queries (f_constinreg/f_ldainreg
 * inside the makelivranges walker func_4623c4) decide whether a value
 * becomes a live-range web at all.  This hook (a) traces them with a
 * STATEMENT-ordinal site key that does not feedback-renumber, and
 * (b) can force/deny individual sites.  Default off; with the env vars
 * unset every shim is a no-op and output is bit-identical. */
#define IDO52_PFORM_MAX 32
static int ido52_pform_init_done;
static int ido52_pform_trace;
static int ido52_pform_stmt;          /* statement ordinal, current proc */
static unsigned ido52_pform_stmt_op;  /* that statement's ucode opcode  */
static int ido52_pform_nq_const, ido52_pform_nq_lda;
static int ido52_pform_flbb_site;     /* caller-site id for FORMLIV trace */
static struct {
    int kind;            /* 0 = const, 1 = lda */
    uint32_t val;
    int stmt_from, stmt_to;   /* -1 = any */
    int res;
} ido52_pform_site[IDO52_PFORM_MAX];
static int ido52_pform_site_count;
static struct {
    uint32_t val;
    int bb;              /* -1 = any */
    int occ;             /* 0 = all occurrences of (val,bb) */
    int res;
    int seen;            /* per-proc match counter */
} ido52_pemit_site[IDO52_PFORM_MAX];
static int ido52_pemit_site_count;

static int ido52_pform_init(void) {
    const char *e;
    char buf[512];
    char *p, *tok;
    if (ido52_pform_init_done) return ido52_pform_trace;
    ido52_pform_init_done = 1;
    e = getenv("IDO52_FORM_TRACE");
    ido52_pform_trace = e ? atoi(e) : 0;
    e = getenv("IDO52_FORM_SITE");
    if (e && strlen(e) < sizeof(buf)) {
        strcpy(buf, e);
        p = buf;
        while ((tok = strtok(p, ",")) != NULL && ido52_pform_site_count < IDO52_PFORM_MAX) {
            char *c1, *c2, *c3, *dash;
            p = NULL;
            c1 = strchr(tok, ':');
            if (!c1) continue;
            *c1++ = 0;
            c2 = strchr(c1, ':');
            if (!c2) continue;
            *c2++ = 0;
            c3 = strchr(c2, ':');
            if (!c3) continue;
            *c3++ = 0;
            {
                int k = -1;
                if (strcmp(tok, "const") == 0) k = 0;
                else if (strcmp(tok, "lda") == 0) k = 1;
                if (k < 0) continue;
                ido52_pform_site[ido52_pform_site_count].kind = k;
            }
            ido52_pform_site[ido52_pform_site_count].val =
                (uint32_t)strtoul(c1, NULL, 0);
            if (c2[0] == '*') {
                ido52_pform_site[ido52_pform_site_count].stmt_from = -1;
                ido52_pform_site[ido52_pform_site_count].stmt_to = -1;
            } else {
                dash = strchr(c2, '-');
                ido52_pform_site[ido52_pform_site_count].stmt_from =
                    (int)strtol(c2, NULL, 0);
                ido52_pform_site[ido52_pform_site_count].stmt_to =
                    dash ? (int)strtol(dash + 1, NULL, 0)
                         : ido52_pform_site[ido52_pform_site_count].stmt_from;
            }
            ido52_pform_site[ido52_pform_site_count].res = atoi(c3);
            ido52_pform_site_count++;
        }
        fprintf(stderr, "[FORM-SITE] %d site override(s)\n",
                ido52_pform_site_count);
    }
    e = getenv("IDO52_EMIT_SITE");
    if (e && strlen(e) < sizeof(buf)) {
        strcpy(buf, e);
        p = buf;
        while ((tok = strtok(p, ",")) != NULL && ido52_pemit_site_count < IDO52_PFORM_MAX) {
            char *f[5];
            int nf = 0;
            char *q = tok;
            p = NULL;
            while (nf < 5 && q) {
                f[nf++] = q;
                q = strchr(q, ':');
                if (q) *q++ = 0;
            }
            if (nf != 5 || strcmp(f[0], "const") != 0) continue;
            ido52_pemit_site[ido52_pemit_site_count].val =
                (uint32_t)strtoul(f[1], NULL, 0);
            ido52_pemit_site[ido52_pemit_site_count].bb =
                f[2][0] == '*' ? -1 : (int)strtol(f[2], NULL, 0);
            ido52_pemit_site[ido52_pemit_site_count].occ =
                (int)strtol(f[3], NULL, 0);
            ido52_pemit_site[ido52_pemit_site_count].res = atoi(f[4]);
            ido52_pemit_site_count++;
        }
        fprintf(stderr, "[EMIT-SITE] %d site override(s)\n",
                ido52_pemit_site_count);
    }
    return ido52_pform_trace;
}

static uint32_t ido52_pemit_query(uint32_t res, uint32_t val, uint32_t bb) {
    int i;
    uint32_t newres = res;
    const char *why = "";
    ido52_pform_init();
    /* Advance EVERY matching entry's occurrence counter before deciding,
     * so multiple windows over the same (val,bb) stay in one index space. */
    for (i = 0; i < ido52_pemit_site_count; i++) {
        if (ido52_pemit_site[i].val != val) continue;
        if (ido52_pemit_site[i].bb >= 0 &&
            (uint32_t)ido52_pemit_site[i].bb != bb) continue;
        ido52_pemit_site[i].seen++;
        if (ido52_pemit_site[i].occ != 0 &&
            ido52_pemit_site[i].seen != ido52_pemit_site[i].occ) continue;
        newres = (uint32_t)ido52_pemit_site[i].res;
        why = newres != res ? " OVERRIDE" : " (match, same)";
    }
    if (ido52_pform_trace && (ido52_pemit_site_count || ido52_pform_trace > 1))
        fprintf(stderr, "[EMITQ const] val=0x%x bb=%u res=%u%s\n",
                val, bb, newres, why);
    return newres;
}

static uint32_t ido52_pform_query(int kind, uint32_t res, uint32_t v1,
                                  uint32_t v2, uint32_t ctx, uint32_t desc,
                                  uint32_t descbit) {
    /* kind 0: v1=value, v2=type byte.  kind 1: v1=offset word, v2=class. */
    int i, n;
    uint32_t newres = res;
    const char *why = "";
    ido52_pform_init();
    if (kind == 0) n = ++ido52_pform_nq_const; else n = ++ido52_pform_nq_lda;
    for (i = 0; i < ido52_pform_site_count; i++) {
        if (ido52_pform_site[i].kind != kind) continue;
        if (ido52_pform_site[i].val != v1) continue;
        if (ido52_pform_site[i].stmt_from >= 0 &&
            (ido52_pform_stmt < ido52_pform_site[i].stmt_from ||
             ido52_pform_stmt > ido52_pform_site[i].stmt_to)) continue;
        newres = (uint32_t)ido52_pform_site[i].res;
        why = newres != res ? " OVERRIDE" : " (match, same)";
        break;
    }
    if (ido52_pform_trace) {
        if (kind == 0)
            fprintf(stderr, "[FORMQ const] q=%d stmt=%d op=0x%02x val=0x%x "
                    "type=%u ctx=%u desc=0x%x bit=%u res=%u%s\n",
                    n, ido52_pform_stmt, ido52_pform_stmt_op, v1, v2, ctx,
                    desc, descbit, newres, why);
        else
            fprintf(stderr, "[FORMQ lda] q=%d stmt=%d op=0x%02x off=0x%x "
                    "class=%u ctx=%u desc=0x%x bit=%u res=%u%s\n",
                    n, ido52_pform_stmt, ido52_pform_stmt_op, v1, v2, ctx,
                    desc, descbit, newres, why);
    }
    return newres;
}
'''

anchor = "static int ido52_hooks_initialized;"
assert anchor in src
src = src.replace(anchor, STATICS + "\n" + anchor, 1)

# ------------------------------------------- makelivranges entry: reset+init
old = "L465ec0:\n//makelivranges:\n"
assert old in src
src = src.replace(old, old +
    "ido52_pform_init();\n"
    "ido52_pform_stmt = 0;\n"
    "ido52_pform_stmt_op = 0;\n"
    "{ int _pe; for (_pe = 0; _pe < ido52_pemit_site_count; _pe++) "
    "ido52_pemit_site[_pe].seen = 0; }\n", 1)

# ------------------------------------------- statement loop head: tracker
old = "L4669b8:\nv0 = MEM_U8(fp + 0);\n"
assert old in src
src = src.replace(old,
    "L4669b8:\n"
    "ido52_pform_stmt++;\n"
    "ido52_pform_stmt_op = MEM_U8(fp + 0);\n"
    "v0 = MEM_U8(fp + 0);\n", 1)

# ------------------------- walker const formation site (func_4623c4 L462438)
old = ("v0 = f_constinreg(mem, sp, a0, a1, a2, a3);\ngoto L46245c;")
assert old in src
src = src.replace(old,
    "v0 = f_constinreg(mem, sp, a0, a1, a2, a3);\n"
    "v0 = ido52_pform_query(0, v0, a2, a0, MEM_U8(sp + 103), MEM_U32(s0 + 20),\n"
    "    MEM_U32(s0 + 20) ? MEM_U16(MEM_U32(s0 + 20) + 2) : 0xffffffffu);\n"
    "goto L46245c;", 1)

# --------------------------- walker LDA formation site (func_4623c4 L462524)
old = ("v0 = f_ldainreg(mem, sp, a0, a1, a2);\ngoto L46253c;")
assert old in src
src = src.replace(old,
    "v0 = f_ldainreg(mem, sp, a0, a1, a2);\n"
    "v0 = ido52_pform_query(1, v0, a1, a0, a2, MEM_U32(s0 + 20),\n"
    "    MEM_U32(s0 + 20) ? MEM_U16(MEM_U32(s0 + 20) + 2) : 0xffffffffu);\n"
    "goto L46253c;", 1)

# ------------------------- emission const query sites (bb-keyed override)
# Site E1: func_422b30 (f_gen_outparcode helper, the CONSTQ_TRACE "Q1" site)
old = "v0 = f_constinreg(mem, sp, a0, a1, a2, a3);\ngoto L422c88;"
assert old in src
src = src.replace(old,
    "v0 = f_constinreg(mem, sp, a0, a1, a2, a3);\n"
    "v0 = ido52_pemit_query(v0, a2,\n"
    "    MEM_U16(MEM_U32(MEM_U32(sp + 124) + -12) + 8));\n"
    "goto L422c88;", 1)

# Site E2: func_426d34 (second emission const query)
old = "v0 = f_constinreg(mem, sp, a0, a1, a2, a3);\ngoto L426e6c;"
assert old in src
src = src.replace(old,
    "v0 = f_constinreg(mem, sp, a0, a1, a2, a3);\n"
    "v0 = ido52_pemit_query(v0, a2,\n"
    "    MEM_U16(MEM_U32(MEM_U32(sp + 124) + -12) + 8));\n"
    "goto L426e6c;", 1)

# ------------------------------ func_462260 const sites (trace-only, rare)
for lbl in ("L4622e0", "L462314"):
    old = "v0 = f_constinreg(mem, sp, a0, a1, a2, a3);\ngoto %s;" % lbl
    assert old in src, lbl
    src = src.replace(old,
        "v0 = f_constinreg(mem, sp, a0, a1, a2, a3);\n"
        "if (ido52_pform_trace) fprintf(stderr, \"[FORMQ c462260/%s] "
        "stmt=%%d val=0x%%x res=%%u\\n\", ido52_pform_stmt, a2, v0);\n"
        "goto %s;" % (lbl, lbl), 1)

# ------------------------------------------------- NEWBIT trace at all sites
# Tag by containing recompiled function, in file order.
NEWBIT_TAGS = [
    "isearch1", "isearch2", "trep_image", "searchstore",
    "walk462260", "walk_const", "walk_lda", "walk_3",
    "f463d04", "f465580", "split",
]
call = "v0 = f_newbit(mem, sp, a0, a1);"
parts = src.split(call)
assert len(parts) == len(NEWBIT_TAGS) + 1, \
    "expected %d f_newbit sites, found %d" % (len(NEWBIT_TAGS), len(parts) - 1)
out = parts[0]
for i, tag in enumerate(NEWBIT_TAGS):
    out += (call +
        "\nif (ido52_pform_init()) fprintf(stderr, \"[NEWBIT %s] bit=%%u "
        "lr=0x%%x a1=0x%%x stmt=%%d\\n\", v0, a0, a1, ido52_pform_stmt);"
        % tag) + parts[i + 1]
src = out

# ---------------------------------- f_formlivbb caller-site id tagging
# Every call site gets a sequential id stored in ido52_pform_flbb_site just
# before the call, so the entry trace can attribute marks to their caller.
call_re = re.compile(r"^(f_formlivbb\(mem, sp, a0, a1, a2\);)$", re.M)
flbb_n = [0]
def _tag(m):
    flbb_n[0] += 1
    return "ido52_pform_flbb_site = %d;\n%s" % (flbb_n[0], m.group(1))
src = call_re.sub(_tag, src)
print("tagged %d f_formlivbb call sites" % flbb_n[0])

# --------------------------------------- f_formlivbb entry trace (level 2)
old = "MEM_U32(sp + 92) = a1;\nv0 = MEM_U16(a0 + 2);\nt7 = 0x1001cbd0;"
assert old in src
src = src.replace(old,
    "MEM_U32(sp + 92) = a1;\n"
    "if (ido52_pform_trace > 1) fprintf(stderr, \"[FORMLIV s%d] bit=%u "
    "desc=0x%x bb=0x%x stmt=%d\\n\", ido52_pform_flbb_site, "
    "MEM_U16(a0 + 2), a0, a1, ido52_pform_stmt);\n"
    "v0 = MEM_U16(a0 + 2);\nt7 = 0x1001cbd0;", 1)

open(path, "w").write(src)
print("Hook 30 (IDO52_FORM_TRACE / IDO52_FORM_SITE) applied to %s" % path)
