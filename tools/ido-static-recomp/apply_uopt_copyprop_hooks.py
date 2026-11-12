#!/usr/bin/env python3
"""apply_uopt_copyprop_hooks.py -- Hook 32: site-keyed f_copypropagate veto
(IDO52_COPYPROP_TRACE / IDO52_COPYPROP_SITE).

Run AFTER apply_uopt_hooks.py, apply_uopt_defplace_hooks.py and
apply_uopt_precolor_hooks.py on the generated build/5.2/uopt.c, then
rebuild uopt:

    python3 apply_uopt_copyprop_hooks.py [build/5.2/uopt.c]
    gcc -c -std=c11 -O0 -ggdb3 -fno-strict-aliasing -I. \
        -o build/5.2/uopt.o build/5.2/uopt.c
    gcc -std=c11 -O0 -ggdb3 -fno-strict-aliasing -I. -o build/5.2/out/uopt \
        build/5.2/uopt.o build/5.2/version_info.o build/5.2/libc_impl_52.o -lm

MECHANISM (the DEAD-STORE COPY-PROP BLOCKER's target, made surgical):
uopt's VALUE-BASED copy propagation (f_copypropagate, pass entry L416734)
walks the itab statement list once per proc (per-bb head L416a48, per-
statement head L416a58, advance s2 = MEM_U32(s2 + 8)).  For every
statement it rebuilds the expression "image" via the memoized recursive
builder func_414394; variable-leaf entries (ihash kinds 1/2/5/8, case
L414420) SUBSTITUTE their recorded value node (entry+20) into the image
iff the entry's validity stamp (entry+24) equals the global copy stamp
(word 0x1001cacc).  That stamp compare IS the "no operand altered in
between" test: the source-level dead-store blocker works by bumping the
stamp.  This hook (a) traces every leaf substitution decision keyed by
the copypropagate statement ordinal, and (b) can DENY the substitution
at a chosen statement -- the veto takes the exact same goto L4164f4
path a natural stamp mismatch takes, so vetoed compiles are behaviorally
identical to the proven dead-store source cure, without the source edit.

Statement ordinals come from the fixed itab walk (Hook 30 precedent:
they do not feedback-renumber when sites are forced).  They are LOCAL TO
THIS PASS -- do not reuse Hook 30 FORM_SITE ordinals.  Re-derive with
IDO52_COPYPROP_TRACE=1 after any source edit.

ENV (default off; with both unset every shim is a read-only no-op and
output is bit-identical):
  IDO52_COPYPROP_TRACE=1|2   trace substitution decisions
      1: [CPQ leaf] every variable-leaf lookup (HIT/MISS/DENY) +
         [CPS store] store-image record + [CPS entry] hash entry +
         [CPS pair] two-operand statement images + [CPM copyrep]
         image-node commit (the tree mutation)
      2: + [CPSTMT] one line per walked statement, [CPS gen] generic-
         statement images
  IDO52_COPYPROP_SITE=spec[,spec...]   two site families:
      spec := [stmt:]stmt[:deny]  f_copypropagate-walk statement veto
            | lod:q[:deny]        bb-local isearch LOD-substitution veto
      The family prefix is STICKY across the comma list: it applies to its
      own token and to every following BARE token until the other prefix
      appears.  The list starts in the stmt family.  So `35,40` = two stmt
      sites (unchanged), `lod:1,lod:2` = two lod sites (unchanged), and
      `lod:23,15` = two LOD sites (this used to make 15 a stmt ordinal --
      the "multi-entry list silently collapses" trap).  Write `stmt:` to
      switch back: `lod:23,stmt:96`.
      stmt := statement ordinal from the trace's stmt= field, or '*'
              (diagnostic: deny every statement of the walk)
      q    := the [CPQ lod] trace's q= ordinal (global across the whole
              uopt run, NOT per-proc; ticks only while TRACE/SITE are
              set, and ticks on BOTH verdicts, so a denial does not
              renumber later sites), or '*'
      deny is the default and only action.
      * stmt family (f_copypropagate walk): make the statement's image
        build come out empty, exactly as if an operand had been altered:
        STORE path (L416ae8) image zeroed -> no value-number record;
        two-operand path (L416e48) both images zeroed; variable-leaf
        memo reuse (L414420) forced MISS.
      * lod family (f_readnxtinst, THE bb-local value-numbering gate at
        L43e8f0): a load of a scalar variable whose last in-bb store
        recorded a still-valid value tree normally REUSES that tree
        (the use references the RHS item -- the copy fold).  deny takes
        the natural not-reusable path (goto L43e9f0): a fresh var-load
        item is appended and the store's value-was-read mark is set,
        exactly as if f_treekilled had said the value was clobbered.
        This is the same-bb copy-collapse mechanism (write-back respell
        chains, const-store folds); cross-bb folds (uses after a call)
        never reach this gate -- see the Hook 32 doc section.
"""
import sys
import shutil

path = sys.argv[1] if len(sys.argv) > 1 else "build/5.2/uopt.c"
src = open(path).read()

if "IDO52_COPYPROP_TRACE" in src:
    print("Hook 32 (COPYPROP_TRACE/COPYPROP_SITE) already present; nothing to do.")
    sys.exit(0)

shutil.copyfile(path, path + ".pre-copyprop")

# ------------------------------------------------------------------ statics
STATICS = r'''
/* --- Hook 32: IDO52_COPYPROP_TRACE / IDO52_COPYPROP_SITE -------------
 * Site-keyed veto of value-based copy propagation.  f_copypropagate
 * substitutes a variable leaf's recorded value node (entry+20) into a
 * statement's expression image when the entry's stamp (entry+24)
 * matches the global copy stamp (0x1001cacc) -- func_414394 case
 * L414420.  The veto forces that lookup to MISS for all variable
 * leaves of a chosen statement (ordinal from the copypropagate walk),
 * taking the same path a natural stamp mismatch takes.  Default off;
 * with the env vars unset every shim is a no-op and output is
 * bit-identical. */
#define IDO52_CPROP_MAX 32
static int ido52_cprop_init_done;
static int ido52_cprop_trace;
static int ido52_cprop_stmt;          /* statement ordinal, current proc */
static unsigned ido52_cprop_stmt_op;  /* that statement's ucode opcode  */
static int ido52_cprop_deny[IDO52_CPROP_MAX];
static int ido52_cprop_deny_count;
static int ido52_cprop_lod_deny[IDO52_CPROP_MAX];
static int ido52_cprop_lod_deny_count;
static int ido52_cprop_lodq_n;        /* global [CPQ lod] arrival ordinal */

/* Parse IDO52_COPYPROP_SITE.
 *
 * The family prefix is STICKY across the comma list: `lod:` (or `stmt:`)
 * selects the family for that token AND for every following bare token,
 * until the other prefix appears.  The list starts in the stmt family, so
 * an all-bare list (`35,40,42`) is unchanged, and `lod:A,lod:B` is
 * unchanged.  What changes is `lod:A,B` -- B used to silently fall back to
 * the stmt family (a statement ordinal), which on a function whose stmt
 * family is output-inert is indistinguishable from `lod:A` alone.  That is
 * the documented "multi-entry occ lists silently collapse" trap.
 *
 * Also: the loop no longer stops on the stmt array being full (that ended
 * a mixed list early), unparseable tokens are reported instead of being
 * silently read as ordinal 0, and array overflow is reported. */
static int ido52_cprop_init(void) {
    const char *e;
    char buf[512];
    char *p, *tok;
    int is_lod = 0;             /* sticky family: 0 = stmt, 1 = lod */
    if (ido52_cprop_init_done) return ido52_cprop_trace;
    ido52_cprop_init_done = 1;
    e = getenv("IDO52_COPYPROP_TRACE");
    ido52_cprop_trace = e ? atoi(e) : 0;
    e = getenv("IDO52_COPYPROP_SITE");
    if (e && strlen(e) < sizeof(buf)) {
        strcpy(buf, e);
        p = buf;
        while ((tok = strtok(p, ",")) != NULL) {
            char *c1, *endp;
            int val;
            p = NULL;
            if (strncmp(tok, "lod:", 4) == 0) {
                is_lod = 1;
                tok += 4;
            } else if (strncmp(tok, "stmt:", 5) == 0) {
                is_lod = 0;
                tok += 5;
            }
            c1 = strchr(tok, ':');
            if (c1) {
                *c1++ = 0;
                if (strcmp(c1, "deny") != 0) {
                    fprintf(stderr,
                            "[CPROP-SITE] unknown action '%s' (only deny)\n",
                            c1);
                    continue;
                }
            }
            if (tok[0] == 0) continue;
            if (tok[0] == 42) {
                val = -1;
            } else {
                val = (int)strtol(tok, &endp, 0);
                if (endp == tok || *endp != 0) {
                    fprintf(stderr,
                            "[CPROP-SITE] unparseable site '%s' -- ignored\n",
                            tok);
                    continue;
                }
            }
            if (is_lod) {
                if (ido52_cprop_lod_deny_count < IDO52_CPROP_MAX)
                    ido52_cprop_lod_deny[ido52_cprop_lod_deny_count++] = val;
                else
                    fprintf(stderr, "[CPROP-SITE] too many lod sites "
                            "(max %d) -- '%s' ignored\n",
                            IDO52_CPROP_MAX, tok);
            } else {
                if (ido52_cprop_deny_count < IDO52_CPROP_MAX)
                    ido52_cprop_deny[ido52_cprop_deny_count++] = val;
                else
                    fprintf(stderr, "[CPROP-SITE] too many stmt sites "
                            "(max %d) -- '%s' ignored\n",
                            IDO52_CPROP_MAX, tok);
            }
        }
        fprintf(stderr, "[CPROP-SITE] %d stmt + %d lod deny site(s)\n",
                ido52_cprop_deny_count, ido52_cprop_lod_deny_count);
    }
    return ido52_cprop_trace;
}

/* 1 iff the current statement is denied ('*' spec = -1 denies all).
 * Read-only when env unset. */
static int ido52_cprop_denied(void) {
    int i;
    ido52_cprop_init();
    for (i = 0; i < ido52_cprop_deny_count; i++)
        if (ido52_cprop_deny[i] == ido52_cprop_stmt ||
            ido52_cprop_deny[i] == -1) return 1;
    return 0;
}

/* Called at func_414394's variable-leaf case L414420 with the entry and
 * the two stamp words already loaded.  Returns 1 to DENY (caller takes
 * the natural-MISS goto L4164f4).  Read-only when env unset. */
static int ido52_cprop_leafq(uint8_t *mem, uint32_t ent, uint32_t cur,
                             uint32_t stamp) {
    int deny, hit = (cur == stamp);
    ido52_cprop_init();
    if (!ido52_cprop_trace && !ido52_cprop_deny_count) return 0;
    deny = ido52_cprop_denied();
    if (ido52_cprop_trace)
        fprintf(stderr, "[CPQ leaf] stmt=%d op=0x%02x ent=0x%x kind=%u "
                "bit=%u node=0x%x stamp=%u cur=%u res=%s\n",
                ido52_cprop_stmt, ido52_cprop_stmt_op, ent, MEM_U8(ent + 0),
                MEM_U16(ent + 2), MEM_U32(ent + 20), stamp, cur,
                deny && hit ? "DENY" : hit ? "HIT" : "MISS");
    return deny && hit;
}
'''

STATICS += r'''
/* Called at f_readnxtinst's bb-local LOD-substitution gate (L43e8f0):
 * the load's variable entry ent has a recorded value tree (ent+52) from
 * its last in-bb store (ent+56).  Natural flow reuses the tree unless
 * f_bigtree/f_treekilled object.  Returns 1 to DENY (caller takes the
 * natural goto L43e9f0 fresh-load path).  Ticks the global q ordinal on
 * every arrival while enabled, on BOTH verdicts, so denial does not
 * renumber later sites.  Read-only when env unset. */
static int ido52_cprop_lodq(uint8_t *mem, uint32_t ent) {
    int i, deny = 0, q;
    uint32_t val;
    ido52_cprop_init();
    if (!ido52_cprop_trace && !ido52_cprop_lod_deny_count) return 0;
    q = ++ido52_cprop_lodq_n;
    for (i = 0; i < ido52_cprop_lod_deny_count; i++)
        if (ido52_cprop_lod_deny[i] == q ||
            ido52_cprop_lod_deny[i] == -1) { deny = 1; break; }
    val = MEM_U32(ent + 52);
    if (ido52_cprop_trace)
        fprintf(stderr, "[CPQ lod] q=%d ent=0x%x kind=%u off=0x%x mt=0x%x "
                "val=0x%x vkind=%u vkilled=%u stmt=0x%x%s\n",
                q, ent, MEM_U8(ent + 0), MEM_U32(ent + 40), MEM_U32(ent + 44),
                val, val ? MEM_U8(val + 0) : 0, val ? MEM_U8(val + 2) : 0,
                MEM_U32(ent + 56), deny ? " DENY" : "");
    return deny;
}
'''

anchor = "static int ido52_hooks_initialized;"
assert anchor in src
src = src.replace(anchor, STATICS + "\n" + anchor, 1)

# ---------------------------------------- copypropagate entry: reset+init
old = "L416734:\n//copypropagate:\n"
assert old in src
src = src.replace(old, old +
    "ido52_cprop_init();\n"
    "ido52_cprop_stmt = 0;\n"
    "ido52_cprop_stmt_op = 0;\n", 1)

# ---------------------------------------- statement loop head: tracker
old = "L416a58:\ns0 = MEM_U8(s2 + 0);\n"
assert old in src
src = src.replace(old,
    "L416a58:\n"
    "ido52_cprop_stmt++;\n"
    "ido52_cprop_stmt_op = MEM_U8(s2 + 0);\n"
    "if (ido52_cprop_trace > 1) fprintf(stderr, \"[CPSTMT] stmt=%d "
    "op=0x%02x s=0x%x\\n\", ido52_cprop_stmt, ido52_cprop_stmt_op, s2);\n"
    "s0 = MEM_U8(s2 + 0);\n", 1)

# ------------------- variable-leaf lookup in func_414394 (THE veto site)
old = ("L414420:\ns7 = 0x1001cacc;\nt9 = MEM_U32(a3 + 24);\n"
       "t8 = MEM_U32(s7 + 0);\nt1 = MEM_U32(sp + 196);\n"
       "if ((int32_t)t8 != (int32_t)t9) {\n")
assert old in src
src = src.replace(old,
    "L414420:\n"
    "s7 = 0x1001cacc;\n"
    "t9 = MEM_U32(a3 + 24);\n"
    "t8 = MEM_U32(s7 + 0);\n"
    "t1 = MEM_U32(sp + 196);\n"
    "if (ido52_cprop_leafq(mem, a3, t8, t9)) {\n"
    "goto L4164f4;}\n"
    "if ((int32_t)t8 != (int32_t)t9) {\n", 1)

# ---------- bb-local LOD-substitution gate veto (f_readnxtinst L43e8f0)
old = ("L43e8f0:\n//nop;\na0 = MEM_U32(s5 + 52);\na1 = 0x14;\n"
       "v0 = f_bigtree(mem, sp, a0, a1);\n")
assert old in src
src = src.replace(old,
    "L43e8f0:\n"
    "if (ido52_cprop_lodq(mem, s5)) {\n"
    "goto L43e9f0;}\n"
    "//nop;\n"
    "a0 = MEM_U32(s5 + 52);\n"
    "a1 = 0x14;\n"
    "v0 = f_bigtree(mem, sp, a0, a1);\n", 1)

# ------------- store-path image result: trace + THE veto ([CPS store])
# Zeroing the image takes the natural s7==0 -> L416d88 path: the store
# gets no value-number record, later uses cannot unify with the RHS.
old = ("L416ae8:\n// bdead c0000003 gp = MEM_U32(sp + 60);\n"
       "s7 = MEM_U32(sp + 184);\n")
assert old in src
src = src.replace(old, old +
    "if (s7 != 0 && ido52_cprop_denied()) {\n"
    "if (ido52_cprop_trace) fprintf(stderr, \"[CPS store] stmt=%d "
    "image=0x%x res=DENY\\n\", ido52_cprop_stmt, s7);\n"
    "s7 = 0;\n"
    "MEM_U32(sp + 184) = 0;\n"
    "} else if (ido52_cprop_trace) fprintf(stderr, \"[CPS store] stmt=%d "
    "image=0x%x\\n\", ido52_cprop_stmt, s7);\n", 1)

# --------------------------- store-path hash entry trace ([CPS entry])
old = "L416b88:\nMEM_U32(s4 + 32) = s2;\n"
assert old in src
src = src.replace(old,
    "L416b88:\n"
    "if (ido52_cprop_trace) fprintf(stderr, \"[CPS entry] stmt=%d "
    "ent=0x%x bit=%u\\n\", ido52_cprop_stmt, s4, MEM_U16(s4 + 2));\n"
    "MEM_U32(s4 + 32) = s2;\n", 1)

# ------------- two-operand statement images: trace + veto ([CPS pair])
old = ("L416e48:\n// bdead c0040003 gp = MEM_U32(sp + 60);\n"
       "s3 = MEM_U32(sp + 188);\nt1 = 0x1001eb24;\ns7 = MEM_U32(sp + 184);\n")
assert old in src
src = src.replace(old, old +
    "if ((s3 != 0 || s7 != 0) && ido52_cprop_denied()) {\n"
    "if (ido52_cprop_trace) fprintf(stderr, \"[CPS pair] stmt=%d "
    "img4=0x%x img20=0x%x res=DENY\\n\", ido52_cprop_stmt, s3, s7);\n"
    "s3 = 0;\n"
    "s7 = 0;\n"
    "MEM_U32(sp + 188) = 0;\n"
    "MEM_U32(sp + 184) = 0;\n"
    "} else if (ido52_cprop_trace) fprintf(stderr, \"[CPS pair] stmt=%d "
    "img4=0x%x img20=0x%x\\n\", ido52_cprop_stmt, s3, s7);\n", 1)

# ----------------- image-node commit trace ([CPM copyrep])
old = ("L414cf0:\n// bdead 40020003 gp = MEM_U32(sp + 60);\n"
       "a0 = MEM_U32(sp + 192);\n//nop;\na1 = s0;\n//nop;\n"
       "f_copycoderep(mem, sp, a0, a1);\n")
assert old in src
src = src.replace(old,
    "L414cf0:\n"
    "// bdead 40020003 gp = MEM_U32(sp + 60);\n"
    "a0 = MEM_U32(sp + 192);\n"
    "//nop;\n"
    "a1 = s0;\n"
    "//nop;\n"
    "if (ido52_cprop_trace) fprintf(stderr, \"[CPM copyrep] stmt=%d "
    "dst=0x%x src=0x%x\\n\", ido52_cprop_stmt, a0, a1);\n"
    "f_copycoderep(mem, sp, a0, a1);\n", 1)

# ----------------- generic-statement image trace ([CPS gen], level 2)
old = ("L417e70:\n// bdead 40000003 gp = MEM_U32(sp + 60);\n"
       "s2 = MEM_U32(sp + 172);\n")
assert old in src
src = src.replace(old, old +
    "if (ido52_cprop_trace > 1) fprintf(stderr, \"[CPS gen] stmt=%d "
    "img=0x%x\\n\", ido52_cprop_stmt, MEM_U32(sp + 176));\n", 1)

open(path, "w").write(src)
print("Hook 32 (IDO52_COPYPROP_TRACE / IDO52_COPYPROP_SITE) applied to %s" % path)
