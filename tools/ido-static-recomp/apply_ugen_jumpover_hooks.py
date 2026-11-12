#!/usr/bin/env python3
"""Hook 79: IDO52_UGEN_KEEP_JUMP_OVER -- keep a conditional jump over an
unconditional jump (retail's `bcc L1 ; b END ; L1:` for an EMPTY then-arm)
instead of ugen's inversion into one `!bcc END`.

Idempotent (probes for its own marker); independent of every other applier
(its helper anchors on func_4351b8's own definition, its site on the
func_4351b8 label L435510, which no other applier touches -- Hook 61 patches
L435424 of the same function).  Part of the post-`make clean` chain
(CLAUDE.md Known Gotchas / UOPT-HOOKS-REFERENCE.md Hook 79).

    python3 tools/ido-static-recomp/apply_ugen_jumpover_hooks.py [--ugen PATH] [--rebuild]

THE TRANSFORM
-------------
cfe lowers `if (c) {} else { X }` (and an `if/else if` ladder whose first
arm is empty) as

    fjp/tjp c, L1 ; ujp END ; L1: X ; END:

-O0 ugen emits exactly that (`bge $2,281,$32 ; b $35 ; $32:`).  From -O1 up,
f_labelopt -> func_4351b8 walks every jump statement; for a conditional jump
s2 (tree opcode 0x7f FJP / 0x26 TJP) whose next statement s1 (past label
nodes) is a UJP (0x88), it scans the label nodes after s1 and, when the
conditional's own target label s0 is among them (recomp label L435510,
`s0 == v0`), flips the conditional (0x7f <-> 0x26), retargets it to the
UJP's target (s1 + 4) and f_delete_statement's the UJP.  The jump-over-jump
becomes one inverted branch (`blt $2,281,$END`), which is why no source
spelling of an empty arm (`;`, `{}`, `if (1) {}`, dead stores, a goto) keeps
the `b` at -O2: uopt has already folded the arm to nothing and ugen does the
rest.  The library-region compiler (libgpu PutDispEnv) keeps the `b END`.

THE HOOK
--------
The veto sits at L435510 after the `s0 == v0` test and before the opcode
flip; it jumps to L4355bc, the path func_4351b8 takes for every jump that
is not a jump-over-jump (stamp the target label, advance).  Subtractive and
sound by construction: the conditional keeps its own target and the UJP
stays; nothing is created, no operand or opcode is rewritten (both forms
reach the same code).  func_4350c4(s1), run on the UJP just before the
scan, only resolves the UJP's own jump chain, which happens anyway when the
walk reaches the UJP.

IDO52_UGEN_KEEP_JUMP_OVER=all | N[,N...] | occ:N[,N...] | lab:L[,L...]
    Keep candidate N (0-based arrival order over the whole TU, and over BOTH
    func_4351b8 calls -- a BARE NUMBER IS AN INDEX, not `all`) or every
    candidate whose CONDITIONAL's target is ugen's INTERNAL label L (tree
    +36, as the trace prints it; not the `$N` of `cc -S`).  Prefer `lab:`:
    a kept candidate is re-offered by the second func_4351b8 call.
IDO52_UGEN_KEEP_JUMP_OVER_TRACE=1
    `[UGEN-JOVER] #N op=0xOP label=L ujp_label=U [-> KEPT]`.

Default OFF; with no env var set the helper returns before counting.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UGEN_C = os.path.join(HERE, "build", "5.2", "ugen.c")

MARKER = "ugen_jover_veto"
HELPERS = r"""/* --- IDO52_UGEN_KEEP_JUMP_OVER (Hook 79, 2026-10-03).  Veto ugen
 * f_labelopt's jump-over-jump inversion (func_4351b8 L435510); see
 * apply_ugen_jumpover_hooks.py.  Default OFF. --- */
#define JOVER_MAX 64
static struct {
    int inited, mode, trace, seq;          /* mode: 0 off, 1 all, 2 list */
    int occ[JOVER_MAX], nocc, lab[JOVER_MAX], nlab;
} ugen_jover_cfg;
static void ugen_jover_parse(void) {
    const char *env;
    char buf[512];
    char *tok;
    int in_lab = 0;
    if (ugen_jover_cfg.inited) return;
    ugen_jover_cfg.inited = 1;
    env = getenv("IDO52_UGEN_KEEP_JUMP_OVER_TRACE");
    ugen_jover_cfg.trace = (env && *env) ? 1 : 0;
    env = getenv("IDO52_UGEN_KEEP_JUMP_OVER");
    if (!env || !*env) return;
    if (strcmp(env, "all") == 0) { ugen_jover_cfg.mode = 1; return; }
    ugen_jover_cfg.mode = 2;
    strncpy(buf, env, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    for (tok = strtok(buf, ","); tok; tok = strtok(NULL, ",")) {
        if (strncmp(tok, "occ:", 4) == 0) { in_lab = 0; tok += 4; }
        else if (strncmp(tok, "lab:", 4) == 0) { in_lab = 1; tok += 4; }
        if (in_lab) {
            if (ugen_jover_cfg.nlab < JOVER_MAX)
                ugen_jover_cfg.lab[ugen_jover_cfg.nlab++] = atoi(tok);
        } else {
            if (ugen_jover_cfg.nocc < JOVER_MAX)
                ugen_jover_cfg.occ[ugen_jover_cfg.nocc++] = atoi(tok);
        }
    }
}
/* cj = the conditional jump (opcode +32, target label node +4); uj = the
 * unconditional jump it would absorb.  Returns 1 to keep both. */
static int ugen_jover_veto(uint8_t *mem, uint32_t cj, uint32_t uj) {
    int i, idx, hit = 0, lab, ulab;
    ugen_jover_parse();
    if (ugen_jover_cfg.mode == 0 && !ugen_jover_cfg.trace) return 0;
    idx = ugen_jover_cfg.seq++;
    lab = (int)MEM_U32(MEM_U32(cj + 4) + 36);
    ulab = (int)MEM_U32(MEM_U32(uj + 4) + 36);
    if (ugen_jover_cfg.mode == 1) hit = 1;
    for (i = 0; i < ugen_jover_cfg.nocc; i++) if (ugen_jover_cfg.occ[i] == idx) hit = 1;
    for (i = 0; i < ugen_jover_cfg.nlab; i++) if (ugen_jover_cfg.lab[i] == lab) hit = 1;
    if (ugen_jover_cfg.trace)
        fprintf(stderr, "[UGEN-JOVER] #%d op=0x%x label=%d ujp_label=%d%s\n", idx,
                (int)MEM_U8(cj + 32), lab, ulab, hit ? " -> KEPT" : "");
    return hit;
}

"""
HELPER_ANCHOR = "static void func_4351b8(uint8_t *mem, uint32_t sp, uint32_t v0, uint32_t a0) {\nconst"
SITE_ANCHOR = ("L435510:\nif ((int32_t)s0 != (int32_t)v0) {\n//nop;\ngoto L435578;}\n"
               "//nop;\nt9 = MEM_U8(s2 + 32);\n")
SITE_CODE = ("L435510:\nif ((int32_t)s0 != (int32_t)v0) {\n//nop;\ngoto L435578;}\n"
             "/* Hook 79: IDO52_UGEN_KEEP_JUMP_OVER */\n"
             "if (ugen_jover_veto(mem, s2, s1)) goto L4355bc;\n"
             "//nop;\nt9 = MEM_U8(s2 + 32);\n")


def rebuild(path):
    """Compile the patched ugen.c and atomically replace out/ugen (a
    concurrent compile never sees a half-written binary)."""
    out_dir = os.path.join(os.path.dirname(os.path.abspath(path)), "out")
    obj = os.path.splitext(path)[0] + ".o"
    tmp = os.path.join(out_dir, "ugen.new")
    cmds = [
        ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I" + HERE,
         "-o", obj, path],
        ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I" + HERE,
         "-o", tmp, obj, os.path.join(HERE, "build/5.2/version_info.o"),
         os.path.join(HERE, "build/5.2/libc_impl_52.o"), "-lm"],
    ]
    for cmd in cmds:
        r = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
        if r.returncode != 0:
            print(r.stderr[-2000:])
            sys.exit(1)
    os.replace(tmp, os.path.join(out_dir, "ugen"))
    print(f"[ugen jumpover hooks] ugen rebuilt -> {os.path.join(out_dir, 'ugen')}")


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print(f"[ugen jumpover hooks] already applied to {path}")
        return True
    for needle in (HELPER_ANCHOR, SITE_ANCHOR):
        if s.count(needle) != 1:
            print(f"[ugen jumpover hooks] ERROR: anchor not unique/found: "
                  f"{needle[:40]!r} (count {s.count(needle)})")
            return False
    s = s.replace(HELPER_ANCHOR, HELPERS + HELPER_ANCHOR, 1)
    s = s.replace(SITE_ANCHOR, SITE_CODE, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[ugen jumpover hooks] Hook 79 applied to {path}")
    return True


def main():
    argv = sys.argv[1:]
    path = UGEN_C
    if "--ugen" in argv:
        path = argv[argv.index("--ugen") + 1]
    if not apply(path):
        sys.exit(1)
    if "--rebuild" in argv:
        rebuild(path)


if __name__ == "__main__":
    main()
