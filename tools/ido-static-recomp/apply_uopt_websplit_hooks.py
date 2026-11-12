#!/usr/bin/env python3
"""apply_uopt_websplit_hooks.py -- Hook 34: IDO52_FORM_SPLIT, the
site-keyed constant-descriptor WEB SPLIT (the "address-constant
def-placement" primitive raised by RPG_BAT/func_80141060).

Run AFTER apply_uopt_hooks.py, apply_uopt_defplace_hooks.py and
apply_uopt_precolor_hooks.py (it grafts onto Hook 30's statics) on the
generated build/5.2/uopt.c, then rebuild uopt:

    python3 apply_uopt_websplit_hooks.py [build/5.2/uopt.c]
    gcc -c -std=c11 -O0 -ggdb3 -fno-strict-aliasing -I. \
        -o build/5.2/uopt.o build/5.2/uopt.c
    gcc -std=c11 -O0 -ggdb3 -fno-strict-aliasing -I. -o build/5.2/out/uopt \
        build/5.2/uopt.o build/5.2/version_info.o build/5.2/libc_impl_52.o -lm

MECHANISM

uopt's makelivranges walker (func_4623c4) caches a constant's web bit in
its cfe CONSTANT DESCRIPTOR (`MEM_U16(desc + 2)`; 0xffff = none).  Every
use of the same literal shares one descriptor, so all uses join ONE web,
and the web's def is then emitted at a block dominating every use -- the
cross-region "dominator hoist" that FORM_SITE denial provably cannot
undo (web membership is decided by the cache, not by the query result).

The hook clears that cached bit at a chosen (value, statement) formation
site: f_newbit then allocates a FRESH web, and the matched use plus every
LATER use of the descriptor joins the new web while earlier uses keep the
old one.  Each region's web is then live only in its own region, so the
def is rematerialised per region by uopt's ordinary emitters -- no output
byte is ever patched.

One invariant must be preserved: globalcolor resolves a use's covering
range by reading `desc+2`, taking that bit's liverange, and walking the
split-chain at `lr+52` until `f_bvectin` hits the use's bb (the same
chain uopt's own f_split maintains).  Orphaning the old range therefore
SEGFAULTS globalcolor.  The hook links `newlr+52 = oldlr` as soon as the
new range exists (right after the const site's f_formlivbb), which keeps
the old range reachable from the new chain head.

ENV
  IDO52_FORM_SPLIT=const:VAL:STMT[-STMTTO][,...]
      At a const formation query for value VAL (decimal or 0x hex) whose
      statement ordinal (Hook 30's ido52_pform_stmt, from the
      IDO52_FORM_TRACE=1 [FORMQ const] lines) falls in the window and
      whose candidacy result is positive, clear the descriptor's cached
      web bit before the walker consults it.  The split point is the
      FIRST matched query; uses before it keep the old web.
  IDO52_FORM_SPLIT=lda:OFF:STMT[-STMTTO][,...]     (2026-08-19)
      Same split for LDA (address) descriptors at the walker's lda
      formation site (func_4623c4 L462524 -> f_ldainreg).  OFF is the
      lda offset word exactly as IDO52_FORM_TRACE=1 prints it in the
      [FORMQ lda] lines (off=0x..., e.g. 0xffffffa8), stmt keys as for
      const.  const: and lda: entries may be mixed in one spec.  The
      new liverange is chained to the old via lr+52 at the lda case's
      own f_formlivbb join (flbb_site 3), mirroring the const case.
  IDO52_FORM_TRACE=1   (Hook 30) traces the splits:
      [FORM-SPLIT] stmt=.. val=.. desc=.. oldbit=.. -> fresh web
      [FORM-SPLIT] chained newlr=.. (bit ..) -> oldlr=..

Default OFF: with IDO52_FORM_SPLIT unset ido52_fsplit_match() returns 0
before touching any state and the two shims are no-ops -- output is
bit-identical to the unhooked build (verified on a 10-function
strict-clean sample, see UOPT-HOOKS-REFERENCE.md Hook 34).
"""
import sys
import shutil

path = sys.argv[1] if len(sys.argv) > 1 else "build/5.2/uopt.c"
src = open(path).read()

if "IDO52_FORM_SPLIT" in src:
    print("Hook 34 (FORM_SPLIT) already present; nothing to do.")
    sys.exit(0)

if "ido52_pform_stmt" not in src:
    print("ERROR: Hook 30 (apply_uopt_precolor_hooks.py) must be applied first.")
    sys.exit(1)

shutil.copyfile(path, path + ".pre-websplit")

# ------------------------------------------------------------- helper block
HELPER = r'''
/* --- Hook 34: IDO52_FORM_SPLIT ---------------------------------------
 * Site-keyed web split for constant descriptors.  At a matching
 * (value, statement) formation site with a positive candidacy result,
 * clears the descriptor's cached web bit (desc+2) so f_newbit allocates
 * a FRESH web: the matched use and every later use of that descriptor
 * join the new web instead of the one formed upstream.  The new
 * liverange is chained to the old one via lr+52 (the split chain
 * globalcolor walks) as soon as it exists.  Default off. */
static int ido52_fsplit_init_done;
static struct { int kind; uint32_t val; int stmt_from, stmt_to; } ido52_fsplit[IDO52_PFORM_MAX];
static int ido52_fsplit_count;
static uint32_t ido52_fsplit_pend_desc;   /* descriptor awaiting chain link */
static uint32_t ido52_fsplit_pend_oldlr;  /* its pre-split liverange */
static void ido52_fsplit_init(void) {
    const char *e;
    char buf[512];
    char *p, *tok;
    if (ido52_fsplit_init_done) return;
    ido52_fsplit_init_done = 1;
    e = getenv("IDO52_FORM_SPLIT");
    if (e && strlen(e) < sizeof(buf)) {
        strcpy(buf, e);
        p = buf;
        while ((tok = strtok(p, ",")) != NULL && ido52_fsplit_count < IDO52_PFORM_MAX) {
            char *c1, *c2, *dash;
            int kind;
            p = NULL;
            if (strncmp(tok, "const:", 6) == 0) { kind = 0; c1 = tok + 6; }
            else if (strncmp(tok, "lda:", 4) == 0) { kind = 1; c1 = tok + 4; }
            else continue;
            c2 = strchr(c1, ':');
            if (!c2) continue;
            *c2++ = 0;
            ido52_fsplit[ido52_fsplit_count].kind = kind;
            ido52_fsplit[ido52_fsplit_count].val = (uint32_t)strtoul(c1, NULL, 0);
            dash = strchr(c2, '-');
            ido52_fsplit[ido52_fsplit_count].stmt_from = (int)strtol(c2, NULL, 0);
            ido52_fsplit[ido52_fsplit_count].stmt_to =
                dash ? (int)strtol(dash + 1, NULL, 0)
                     : ido52_fsplit[ido52_fsplit_count].stmt_from;
            ido52_fsplit_count++;
        }
        fprintf(stderr, "[FORM-SPLIT] %d split site(s)\n", ido52_fsplit_count);
    }
}
static int ido52_fsplit_match(int kind, uint32_t val) {
    int i;
    ido52_fsplit_init();
    for (i = 0; i < ido52_fsplit_count; i++) {
        if (ido52_fsplit[i].kind != kind) continue;
        if (ido52_fsplit[i].val != val) continue;
        if (ido52_pform_stmt < ido52_fsplit[i].stmt_from ||
            ido52_pform_stmt > ido52_fsplit[i].stmt_to) continue;
        return 1;
    }
    return 0;
}
'''

anchor = "static uint32_t ido52_pemit_query(uint32_t res, uint32_t val, uint32_t bb) {"
assert anchor in src, "Hook 30 pemit_query anchor missing"
src = src.replace(anchor, HELPER + "\n" + anchor, 1)

# ------------------------------------------- split at the walker const site
old = """v0 = ido52_pform_query(0, v0, a2, a0, MEM_U8(sp + 103), MEM_U32(s0 + 20),
    MEM_U32(s0 + 20) ? MEM_U16(MEM_U32(s0 + 20) + 2) : 0xffffffffu);
goto L46245c;"""
assert old in src, "walker const-site anchor missing"
new = """v0 = ido52_pform_query(0, v0, a2, a0, MEM_U8(sp + 103), MEM_U32(s0 + 20),
    MEM_U32(s0 + 20) ? MEM_U16(MEM_U32(s0 + 20) + 2) : 0xffffffffu);
if (v0 != 0 && MEM_U32(s0 + 20) != 0 && ido52_fsplit_match(0, a2)) {
    if (ido52_pform_trace)
        fprintf(stderr, "[FORM-SPLIT] stmt=%d const val=0x%x desc=0x%x oldbit=%u -> fresh web\\n",
                ido52_pform_stmt, a2, MEM_U32(s0 + 20), MEM_U16(MEM_U32(s0 + 20) + 2));
    {
        uint32_t oldbit = MEM_U16(MEM_U32(s0 + 20) + 2);
        uint32_t bittab = MEM_U32(0x1001cbd0);
        ido52_fsplit_pend_desc = MEM_U32(s0 + 20);
        ido52_fsplit_pend_oldlr =
            (oldbit != 0xffffu && bittab) ? MEM_U32(bittab + oldbit * 8 + 4) : 0;
    }
    MEM_U16(MEM_U32(s0 + 20) + 2) = 0xffff;
}
goto L46245c;"""
src = src.replace(old, new, 1)

# ------------------- chain the new liverange to the old one (post-formlivbb)
old = """ido52_pform_flbb_site = 2;
f_formlivbb(mem, sp, a0, a1, a2);
goto L4624bc;
a1 = s1;
L4624bc:"""
assert old in src, "formlivbb site-2 anchor missing"
new = """ido52_pform_flbb_site = 2;
f_formlivbb(mem, sp, a0, a1, a2);
goto L4624bc;
a1 = s1;
L4624bc:
if (ido52_fsplit_pend_desc != 0 && MEM_U32(s0 + 20) == ido52_fsplit_pend_desc) {
    uint32_t nb = MEM_U16(ido52_fsplit_pend_desc + 2);
    uint32_t bittab = MEM_U32(0x1001cbd0);
    uint32_t newlr = (nb != 0xffffu && bittab) ? MEM_U32(bittab + nb * 8 + 4) : 0;
    if (newlr != 0 && ido52_fsplit_pend_oldlr != 0 && newlr != ido52_fsplit_pend_oldlr) {
        if (ido52_pform_trace)
            fprintf(stderr, "[FORM-SPLIT] chained newlr=0x%x (bit %u, +52 was 0x%x) -> oldlr=0x%x\\n",
                    newlr, nb, MEM_U32(newlr + 52), ido52_fsplit_pend_oldlr);
        MEM_U32(newlr + 52) = ido52_fsplit_pend_oldlr;
        ido52_fsplit_pend_desc = 0;
        ido52_fsplit_pend_oldlr = 0;
    }
}"""
src = src.replace(old, new, 1)


# --------------------------------------------- split at the walker LDA site
old = """v0 = ido52_pform_query(1, v0, a1, a0, a2, MEM_U32(s0 + 20),
    MEM_U32(s0 + 20) ? MEM_U16(MEM_U32(s0 + 20) + 2) : 0xffffffffu);
goto L46253c;"""
assert old in src, "walker lda-site anchor missing"
new = """v0 = ido52_pform_query(1, v0, a1, a0, a2, MEM_U32(s0 + 20),
    MEM_U32(s0 + 20) ? MEM_U16(MEM_U32(s0 + 20) + 2) : 0xffffffffu);
if (v0 != 0 && MEM_U32(s0 + 20) != 0 && ido52_fsplit_match(1, a1)) {
    if (ido52_pform_trace)
        fprintf(stderr, "[FORM-SPLIT] stmt=%d lda off=0x%x desc=0x%x oldbit=%u -> fresh web\\n",
                ido52_pform_stmt, a1, MEM_U32(s0 + 20), MEM_U16(MEM_U32(s0 + 20) + 2));
    {
        uint32_t oldbit = MEM_U16(MEM_U32(s0 + 20) + 2);
        uint32_t bittab = MEM_U32(0x1001cbd0);
        ido52_fsplit_pend_desc = MEM_U32(s0 + 20);
        ido52_fsplit_pend_oldlr =
            (oldbit != 0xffffu && bittab) ? MEM_U32(bittab + oldbit * 8 + 4) : 0;
    }
    MEM_U16(MEM_U32(s0 + 20) + 2) = 0xffff;
}
goto L46253c;"""
src = src.replace(old, new, 1)

# ----------------- chain the new LDA liverange to the old one (post-formlivbb)
old = """ido52_pform_flbb_site = 3;
f_formlivbb(mem, sp, a0, a1, a2);
goto L46259c;
a1 = s1;
L46259c:"""
assert old in src, "formlivbb site-3 (lda) anchor missing"
new = """ido52_pform_flbb_site = 3;
f_formlivbb(mem, sp, a0, a1, a2);
goto L46259c;
a1 = s1;
L46259c:
if (ido52_fsplit_pend_desc != 0 && MEM_U32(s0 + 20) == ido52_fsplit_pend_desc) {
    uint32_t nb = MEM_U16(ido52_fsplit_pend_desc + 2);
    uint32_t bittab = MEM_U32(0x1001cbd0);
    uint32_t newlr = (nb != 0xffffu && bittab) ? MEM_U32(bittab + nb * 8 + 4) : 0;
    if (newlr != 0 && ido52_fsplit_pend_oldlr != 0 && newlr != ido52_fsplit_pend_oldlr) {
        if (ido52_pform_trace)
            fprintf(stderr, "[FORM-SPLIT] chained (lda) newlr=0x%x (bit %u, +52 was 0x%x) -> oldlr=0x%x\\n",
                    newlr, nb, MEM_U32(newlr + 52), ido52_fsplit_pend_oldlr);
        MEM_U32(newlr + 52) = ido52_fsplit_pend_oldlr;
        ido52_fsplit_pend_desc = 0;
        ido52_fsplit_pend_oldlr = 0;
    }
}"""
src = src.replace(old, new, 1)


open(path, "w").write(src)
print("Hook 34 (IDO52_FORM_SPLIT) applied to", path)
print("Rebuild uopt as described in the docstring.")
