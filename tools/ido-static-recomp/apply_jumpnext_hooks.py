#!/usr/bin/env python3
"""Hook 61: IDO52_KEEP_JUMP_NEXT / IDO52_AS1_KEEP_JUMP_NEXT -- keep a jump
whose target is the very next label (retail's `b next ; nop`); Hook 61b:
IDO52_AS1_NO_JUMP_CHAIN -- keep a branch off as1's jump-chain retarget.

Idempotent (probes for its own markers); independent of every other applier
(its helpers anchor on func_4351b8's / func_41b5e8's own definitions).  Part
of the post-`make clean` chain (CLAUDE.md Known Gotchas /
UOPT-HOOKS-REFERENCE).

    python3 tools/ido-static-recomp/apply_jumpnext_hooks.py [--ugen PATH] [--as1 PATH] [--rebuild]

A jump to the immediately-following label is deleted TWICE in the IDO 5.2
pipeline, once per pass, and a surviving `b next ; nop` needs BOTH vetoes:

1. ugen, f_labelopt -> func_4351b8 (recomp label L435424).  f_labelopt (-O1
   and up) calls func_4351b8 twice per procedure (before and after the
   region move func_435fb0, Hook 56).  For every jump statement -- tree
   opcode 0x88 (UJP) and the conditional jumps 0x7f / 0x26 -- it follows
   jump chains, then scans forward from the jump over label-like nodes (the
   0x10005264 opcode bitset; a label whose +34 half is nonzero stops the
   scan) and, when it reaches the jump's own target label (s0 == s1), calls
   f_delete_statement on the jump.  The veto takes the not-deleted path
   (L4355bc: stamp the target label, advance) -- what happens whenever the
   next label is not the target.

2. as1, func_41b5e8 (the -O2 CFG pass run before pass I/II; recomp label
   L41b92c).  For a block ending in a branch whose target block
   (func_41b444) is the block's own fall-through successor (s2+4), it
   zeroes the branch's delay-slot count (s2+62), turns the branch into a
   no-op with f_change_bb(.., 5, ..) and clears its target (s5+20).  Only
   the O2 path reaches it (flag 0x1002f2c8 = the xbb flag; as1 -O1 keeps the
   `b`).  The veto takes the path for a branch whose target chain is not
   followed (L41bbf8: free the scratch list, add the CFG edge to the target,
   no fall-through edge for an unconditional branch) -- exactly the state of
   any ordinary unconditional branch.

Both are subtractive and sound by construction: a `b` to the next word is a
no-op jump; nothing is created, no operand or opcode is rewritten.

THE HOOK
--------
IDO52_KEEP_JUMP_NEXT=all | N[,N...] | occ:N[,N...] | lab:L[,L...]   (ugen)
    Veto deletion candidate N (0-based, arrival order over the whole TU and
    over BOTH func_4351b8 calls -- a BARE NUMBER IS AN INDEX, not `all`) or
    every candidate whose target is ugen's INTERNAL label L (tree +36, as
    the trace prints it; not the `$N` of `cc -S`).  Prefer `lab:` -- a kept
    jump is re-offered by the second func_4351b8 call, so indices shift.
IDO52_KEEP_JUMP_NEXT_TRACE=1
    `[UGEN-JNEXT] #N op=0xOP label=L [-> KEPT]`.
IDO52_AS1_KEEP_JUMP_NEXT=all | N[,N...] | occ:N[,N...]              (as1)
    Veto as1 candidate N (0-based, arrival order over the TU).
IDO52_AS1_KEEP_JUMP_NEXT_TRACE=1
    `[AS1-JNEXT] #N word=0x........ [-> KEPT]` (word = the branch's
    instruction word as as1 holds it at that point).
IDO52_AS1_NO_JUMP_CHAIN=all | N[,N...] | occ:N[,N...]               (as1, 61b)
    Same site, the OTHER arm: a block-ending branch whose target is NOT the
    fall-through goes on to L41b99c, which follows the target's jump chain
    (func_41aa60, up to 6 hops) and retargets the branch -- when the chain
    block is `insn ; b far` the insn is copied into the slot and the branch
    goes straight to `far` (`b join` -> `b end ; move v0,..`).  Veto
    candidate N (0-based, arrival order over the TU; EVERY branch-ending
    block that reaches the test is a candidate, chain or not -- vetoing a
    non-chain branch is a no-op).  The veto takes the same L41bbf8 exit, so
    the branch keeps its original target and slot.
IDO52_AS1_NO_JUMP_CHAIN_TRACE=1
    `[AS1-JCHAIN] #N word=0x........ [-> VETOED]`.

Default OFF; with no env var set each helper returns before counting.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UGEN_C = os.path.join(HERE, "build", "5.2", "ugen.c")
AS1_C = os.path.join(HERE, "build", "5.2", "as1.c")

PARSER = r"""
#define JNEXT_MAX 64
struct jnext_cfg {
    int inited, mode, trace, seq;          /* mode: 0 off, 1 all, 2 list */
    int occ[JNEXT_MAX], nocc, lab[JNEXT_MAX], nlab;
};
static void jnext_parse(struct jnext_cfg *c, const char *var, const char *tvar) {
    const char *env;
    char buf[512];
    char *tok;
    int in_lab = 0;
    if (c->inited) return;
    c->inited = 1;
    env = getenv(tvar);
    c->trace = (env && *env) ? 1 : 0;
    env = getenv(var);
    if (!env || !*env) return;
    if (strcmp(env, "all") == 0) { c->mode = 1; return; }
    c->mode = 2;
    strncpy(buf, env, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    for (tok = strtok(buf, ","); tok; tok = strtok(NULL, ",")) {
        if (strncmp(tok, "occ:", 4) == 0) { in_lab = 0; tok += 4; }
        else if (strncmp(tok, "lab:", 4) == 0) { in_lab = 1; tok += 4; }
        if (in_lab) { if (c->nlab < JNEXT_MAX) c->lab[c->nlab++] = atoi(tok); }
        else { if (c->nocc < JNEXT_MAX) c->occ[c->nocc++] = atoi(tok); }
    }
}
/* returns -1 when off (no counting), else 1 keep / 0 delete; *idx = index */
static int jnext_decide(struct jnext_cfg *c, int lab, int *idx) {
    int i, hit = 0;
    if (c->mode == 0 && !c->trace) return -1;
    *idx = c->seq++;
    if (c->mode == 1) hit = 1;
    for (i = 0; i < c->nocc; i++) if (c->occ[i] == *idx) hit = 1;
    for (i = 0; i < c->nlab; i++) if (c->lab[i] == lab) hit = 1;
    return hit;
}
"""

UGEN_MARKER = "ugen_jnext_veto"
UGEN_HELPERS = (r"""/* --- IDO52_KEEP_JUMP_NEXT (Hook 61, 2026-09-25).  Veto ugen
 * f_labelopt's jump-to-next deletion (func_4351b8 L435424); see
 * apply_jumpnext_hooks.py.  Default OFF. --- */""" + PARSER + r"""
static struct jnext_cfg ugen_jnext_cfg;
/* jmp = the jump statement about to be deleted (opcode +32); its +4 is the
 * target label node, whose +36 is the label number.  Returns 1 to keep it. */
static int ugen_jnext_veto(uint8_t *mem, uint32_t jmp) {
    int idx = 0, hit, lab, op;
    jnext_parse(&ugen_jnext_cfg, "IDO52_KEEP_JUMP_NEXT", "IDO52_KEEP_JUMP_NEXT_TRACE");
    if (ugen_jnext_cfg.mode == 0 && !ugen_jnext_cfg.trace) return 0;
    op = (int)MEM_U8(jmp + 32);
    lab = (int)MEM_U32(MEM_U32(jmp + 4) + 36);
    hit = jnext_decide(&ugen_jnext_cfg, lab, &idx);
    if (ugen_jnext_cfg.trace)
        fprintf(stderr, "[UGEN-JNEXT] #%d op=0x%x label=%d%s\n", idx, op, lab,
                hit == 1 ? " -> KEPT" : "");
    return hit == 1;
}

""")
UGEN_HELPER_ANCHOR = "static void func_4351b8(uint8_t *mem, uint32_t sp, uint32_t v0, uint32_t a0) {\nconst"
UGEN_SITE_ANCHOR = ("L435424:\nif ((int32_t)s0 != (int32_t)s1) {\n//nop;\ngoto L435448;}\n"
                    "//nop;\n//nop;\na0 = s2;\n")
UGEN_SITE_CODE = ("L435424:\nif ((int32_t)s0 != (int32_t)s1) {\n//nop;\ngoto L435448;}\n"
                  "/* Hook 61: IDO52_KEEP_JUMP_NEXT */\n"
                  "if (ugen_jnext_veto(mem, s2)) goto L4355bc;\n"
                  "//nop;\n//nop;\na0 = s2;\n")

AS1_MARKER = "as1_jnext_veto"
AS1_HELPERS = (r"""/* --- IDO52_AS1_KEEP_JUMP_NEXT (Hook 61, 2026-09-25).  Veto as1
 * func_41b5e8's branch-to-fall-through deletion (L41b92c); see
 * apply_jumpnext_hooks.py.  Default OFF. --- */""" + PARSER + r"""
static struct jnext_cfg as1_jnext_cfg, as1_jchain_cfg;
/* ins = the block's terminating branch record (s5); +0 its word. */
static int as1_jnext_veto(uint8_t *mem, uint32_t ins) {
    int idx = 0, hit;
    jnext_parse(&as1_jnext_cfg, "IDO52_AS1_KEEP_JUMP_NEXT", "IDO52_AS1_KEEP_JUMP_NEXT_TRACE");
    if (as1_jnext_cfg.mode == 0 && !as1_jnext_cfg.trace) return 0;
    hit = jnext_decide(&as1_jnext_cfg, -1, &idx);
    if (as1_jnext_cfg.trace)
        fprintf(stderr, "[AS1-JNEXT] #%d word=0x%08x%s\n", idx,
                (unsigned)MEM_U32(ins + 0), hit == 1 ? " -> KEPT" : "");
    return hit == 1;
}
/* Same block, target is NOT the fall-through: as1 goes on (L41b99c) to
 * follow the target's jump chain (func_41aa60, up to 6 hops) and retarget
 * the branch -- copying the chain block's leading word into the slot when
 * that block is `insn ; b far`.  Returns 1 to leave the branch alone. */
static int as1_jchain_veto(uint8_t *mem, uint32_t ins) {
    int idx = 0, hit;
    jnext_parse(&as1_jchain_cfg, "IDO52_AS1_NO_JUMP_CHAIN", "IDO52_AS1_NO_JUMP_CHAIN_TRACE");
    if (as1_jchain_cfg.mode == 0 && !as1_jchain_cfg.trace) return 0;
    hit = jnext_decide(&as1_jchain_cfg, -1, &idx);
    if (as1_jchain_cfg.trace)
        fprintf(stderr, "[AS1-JCHAIN] #%d word=0x%08x%s\n", idx,
                (unsigned)MEM_U32(ins + 0), hit == 1 ? " -> VETOED" : "");
    return hit == 1;
}

""")
AS1_HELPER_ANCHOR = "static void func_41b5e8(uint8_t *mem, uint32_t sp) {\nconst"
AS1_SITE_ANCHOR = ("if ((int32_t)t8 != (int32_t)t9) {\na0 = s0;\ngoto L41b99c;}\na0 = s0;\n"
                   "//nop;\nMEM_U16(s2 + 62) = (uint16_t)zero;\n")
AS1_SITE_CODE = ("if ((int32_t)t8 != (int32_t)t9) {\na0 = s0;\n"
                 "/* Hook 61b: IDO52_AS1_NO_JUMP_CHAIN */\n"
                 "if (as1_jchain_veto(mem, s5)) goto L41bbf8;\n"
                 "goto L41b99c;}\na0 = s0;\n"
                 "/* Hook 61: IDO52_AS1_KEEP_JUMP_NEXT */\n"
                 "if (as1_jnext_veto(mem, s5)) goto L41bbf8;\n"
                 "//nop;\nMEM_U16(s2 + 62) = (uint16_t)zero;\n")


def rebuild_cmds(name):
    return [
        ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
         "-o", f"build/5.2/{name}.o", f"build/5.2/{name}.c"],
        ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
         "-o", f"build/5.2/out/{name}", f"build/5.2/{name}.o", "build/5.2/version_info.o",
         "build/5.2/libc_impl_52.o", "-lm"],
    ]


def apply(path, marker, helpers, hanchor, sanchor, scode, tag):
    s = open(path, "rb").read().decode("latin-1")
    if marker in s:
        print(f"[jumpnext hooks] {tag}: already applied to {path}")
        return True
    for needle in (hanchor, sanchor):
        if s.count(needle) != 1:
            print(f"[jumpnext hooks] {tag}: ERROR: anchor not unique/found: "
                  f"{needle[:40]!r} (count {s.count(needle)})")
            return False
    s = s.replace(hanchor, helpers + hanchor, 1)
    s = s.replace(sanchor, scode, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[jumpnext hooks] {tag}: Hook 61 applied to {path}")
    return True


def main():
    argv = sys.argv[1:]
    ugen = UGEN_C
    as1 = AS1_C
    if "--ugen" in argv:
        ugen = argv[argv.index("--ugen") + 1]
    if "--as1" in argv:
        as1 = argv[argv.index("--as1") + 1]
    ok = apply(ugen, UGEN_MARKER, UGEN_HELPERS, UGEN_HELPER_ANCHOR,
               UGEN_SITE_ANCHOR, UGEN_SITE_CODE, "ugen")
    ok = apply(as1, AS1_MARKER, AS1_HELPERS, AS1_HELPER_ANCHOR,
               AS1_SITE_ANCHOR, AS1_SITE_CODE, "as1") and ok
    if not ok:
        sys.exit(1)
    if "--rebuild" in argv:
        for name in ("ugen", "as1"):
            for cmd in rebuild_cmds(name):
                r = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
                if r.returncode != 0:
                    print(r.stderr[-2000:])
                    sys.exit(1)
        print("[jumpnext hooks] ugen + as1 rebuilt")


if __name__ == "__main__":
    main()
