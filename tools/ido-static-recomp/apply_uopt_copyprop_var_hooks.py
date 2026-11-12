#!/usr/bin/env python3
"""apply_uopt_copyprop_var_hooks.py -- Hook 58: IDO52_COPYPROP_VAR, a
VARIABLE-keyed veto of f_copypropagate's cross-block reaching-value
replacement (2026-09-24).

Run AFTER apply_uopt_copyprop_hooks.py (Hook 32; it uses Hook 32's
statement ordinal and trace level) on build/5.2/uopt.c:

    python3 apply_uopt_copyprop_var_hooks.py [build/5.2/uopt.c] [--rebuild]

MECHANISM

f_copypropagate rebuilds each statement's expression image with the
memoised builder func_414394.  Its var/ilod-leaf case (L414444) looks the
leaf's variable up in the copy-candidate hash chain (0x100135e0 indexed by
the item's hash, item+8) at L414524 and walks the chain (L414544 ..
L414d54): a candidate whose recorded copy statement is available at this
block (f_bvectin0 on the block's copy vector) and survives the
expinalter / has_ilod / is_incr / countvars screens is COMMITTED at
L414778 -- the leaf's image becomes the copy's source tree (`idx` -> `0`),
func_412d78/f_copycoderep rewrite the item, and the parent's constant
operands then fold (`0 + 1` -> `1`, f_binaryfold).  This is the
CROSS-BLOCK constant/copy propagation that Hook 32's two families cannot
see (Family 1 keys the statement image, not the leaf; Family 2 is the
bb-local LOD gate).

The hook empties the candidate chain for ONE variable leaf (s4 = 0 at
L414524), which is exactly the natural "no candidate for this variable"
path (goto L4164f0 with the result left 0): the leaf keeps its own
`LOD var` image.  It never invents a candidate and never touches any other
leaf, so it can only DECLINE one propagation.

KEY
  The variable's frame offset (item+40, the number the `-Wo,-l` listing
  prints on the `isvar` line: `{ 960|0} 103 isvar M 8 -12` -> -12), and an
  optional 1-based occurrence among that variable's leaf arrivals WITH A
  NON-EMPTY candidate chain (per procedure; reset at the pass entry
  L416734).  Only kind-3 (isvar) items are keyed.

ENV
  IDO52_COPYPROP_VAR=OFF[@BLOCK][:OCC[-OCCTO]][,...]
      OFF decimal (negative allowed) or 0x hex; BLOCK = the listing's
      block number (item+44 >> 16; locals are all in the procedure's
      block, so it is only needed for statics, several of which sit at
      offset 0); OCC omitted = every arrival of that variable.
  IDO52_COPYPROP_TRACE=1 (Hook 32's switch) also prints
      [CPV var] off=OFF@BLOCK occ=.. stmt=.. item=.. bit=.. loc=.. size=.. [DENY]
      [CPV commit] stmt=.. item=.. off=OFF@BLOCK src=.. skind=..
  -- the commit line names the arrivals that actually replaced the leaf.

Default OFF: with IDO52_COPYPROP_VAR and IDO52_COPYPROP_TRACE unset the
shim returns 0 before touching any state (the commit trace is a single
false `if`), so output is bit-identical to the unhooked build.

NOTE: a leaf left unreplaced in uopt can still be folded BELOW it -- as1
pass I knows the register's constant value across a fall-through edge and
turns `addu $3,$3,1` into `li $3,1`.  Pair with Hook 44
(IDO52_AS1_NO_PASS1_PROP=<n>) at that instruction when it does.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
UOPT_C = os.path.join(HERE, "build", "5.2", "uopt.c")
MARKER = "IDO52_COPYPROP_VAR"

HELPER_ANCHOR = "static int ido52_hooks_initialized;\n"

HELPER = r'''/* --- Hook 58: IDO52_COPYPROP_VAR (2026-09-24) -----------------------
 * Variable-keyed veto of f_copypropagate's cross-block reaching-value
 * replacement (func_414394 L414524).  See
 * apply_uopt_copyprop_var_hooks.py. */
#define IDO52_CPVAR_MAX 32
static int ido52_cpvar_init_done;
static struct { int32_t off; int blk, from, to; } ido52_cpvar[IDO52_CPVAR_MAX];
static int ido52_cpvar_count;
#define IDO52_CPVAR_SEEN 512
static struct { int32_t off; int blk, n; } ido52_cpvar_seen[IDO52_CPVAR_SEEN];
static int ido52_cpvar_nseen;
static void ido52_cpvar_init(void) {
    const char *e;
    char buf[512];
    char *p, *tok;
    if (ido52_cpvar_init_done) return;
    ido52_cpvar_init_done = 1;
    e = getenv("IDO52_COPYPROP_VAR");
    if (!e || strlen(e) >= sizeof(buf)) return;
    strcpy(buf, e);
    p = buf;
    while ((tok = strtok(p, ",")) != NULL) {
        char *c1, *endp, *dash, *at;
        p = NULL;
        if (ido52_cpvar_count >= IDO52_CPVAR_MAX) {
            fprintf(stderr, "[CPROP-VAR] too many sites (max %d) -- '%s' ignored\n",
                    IDO52_CPVAR_MAX, tok);
            continue;
        }
        c1 = strchr(tok, ':');
        if (c1) *c1++ = 0;
        at = strchr(tok, '@');
        ido52_cpvar[ido52_cpvar_count].blk = -1;
        if (at) {
            *at++ = 0;
            ido52_cpvar[ido52_cpvar_count].blk = (int)strtol(at, NULL, 0);
        }
        ido52_cpvar[ido52_cpvar_count].off = (int32_t)strtol(tok, &endp, 0);
        if (endp == tok || *endp != 0) {
            fprintf(stderr, "[CPROP-VAR] unparseable offset '%s' -- ignored\n", tok);
            continue;
        }
        ido52_cpvar[ido52_cpvar_count].from = 0;
        ido52_cpvar[ido52_cpvar_count].to = 0x7fffffff;
        if (c1 && *c1) {
            dash = strchr(c1, '-');
            ido52_cpvar[ido52_cpvar_count].from = (int)strtol(c1, NULL, 0);
            ido52_cpvar[ido52_cpvar_count].to =
                dash ? (int)strtol(dash + 1, NULL, 0)
                     : ido52_cpvar[ido52_cpvar_count].from;
        }
        ido52_cpvar_count++;
    }
    fprintf(stderr, "[CPROP-VAR] %d variable site(s)\n", ido52_cpvar_count);
}
static void ido52_cpvar_reset(void) { ido52_cpvar_nseen = 0; }
/* At func_414394 L414524, item = the var/ilod leaf, chain = its copy
 * candidate chain head.  Returns 1 to DENY (caller empties the chain =
 * the natural no-candidate path).  Read-only when env unset. */
static int ido52_cpvar_q(uint8_t *mem, uint32_t item, uint32_t chain) {
    int i, occ = 0, deny = 0, blk;
    int32_t off;
    ido52_cpvar_init();
    if (!ido52_cpvar_count && !ido52_cprop_trace) return 0;
    if (chain == 0 || MEM_U8(item + 0) != 3) return 0;
    off = (int32_t)MEM_U32(item + 40);
    blk = (int)(MEM_U32(item + 44) >> 16);
    for (i = 0; i < ido52_cpvar_nseen; i++)
        if (ido52_cpvar_seen[i].off == off && ido52_cpvar_seen[i].blk == blk) {
            occ = ++ido52_cpvar_seen[i].n; break; }
    if (i == ido52_cpvar_nseen && ido52_cpvar_nseen < IDO52_CPVAR_SEEN) {
        ido52_cpvar_seen[ido52_cpvar_nseen].off = off;
        ido52_cpvar_seen[ido52_cpvar_nseen].blk = blk;
        ido52_cpvar_seen[ido52_cpvar_nseen].n = 1;
        ido52_cpvar_nseen++;
        occ = 1;
    }
    for (i = 0; i < ido52_cpvar_count; i++)
        if (ido52_cpvar[i].off == off &&
            (ido52_cpvar[i].blk < 0 || ido52_cpvar[i].blk == blk) &&
            occ >= ido52_cpvar[i].from &&
            occ <= ido52_cpvar[i].to) { deny = 1; break; }
    if (ido52_cprop_trace)
        fprintf(stderr, "[CPV var] off=%d@%d occ=%d stmt=%d item=0x%x bit=%u loc=0x%x size=%u%s\n",
                off, blk, occ, ido52_cprop_stmt, item, MEM_U16(item + 2),
                MEM_U32(item + 44), MEM_U8(item + 32), deny ? " DENY" : "");
    return deny;
}
'''

SITE_ANCHOR = """L414524:
t7 = MEM_U16(a3 + 8);
t5 = 0x100135e0;
t0 = t7 << 2;
t6 = t0 + t5;
s4 = MEM_U32(t6 + 0);
"""
SITE_CODE = SITE_ANCHOR + "if (ido52_cpvar_q(mem, a3, s4)) s4 = 0;  /* Hook 58 */\n"

COMMIT_ANCHOR = """L414778:
a2 = MEM_U32(sp + 192);
"""
COMMIT_CODE = COMMIT_ANCHOR + (
    'if (ido52_cprop_trace && MEM_U8(a2 + 0) == 3) fprintf(stderr, "[CPV commit] '
    'stmt=%d item=0x%x off=%d@%u src=0x%x skind=%u\\n", ido52_cprop_stmt, a2, '
    '(int32_t)MEM_U32(a2 + 40), MEM_U32(a2 + 44) >> 16, a0, MEM_U8(a0 + 0));  /* Hook 58 */\n')

RESET_ANCHOR = """L416734:
//copypropagate:
ido52_cprop_init();
ido52_cprop_stmt = 0;
"""
RESET_CODE = RESET_ANCHOR + "ido52_cpvar_reset();  /* Hook 58 */\n"

REBUILD = [
    ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-MMD", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/uopt.o", "build/5.2/uopt.c"],
    ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
     "-o", "build/5.2/out/uopt", "build/5.2/uopt.o", "build/5.2/version_info.o",
     "build/5.2/libc_impl_52.o", "-lm"],
]


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print(f"[copyprop-var hooks] Already applied to {path}")
        return True
    if "ido52_cprop_leafq" not in s or "[CPM copyrep]" not in s:
        print("[copyprop-var hooks] ERROR: apply_uopt_copyprop_hooks.py (Hook 32) first")
        return False
    for needle in (HELPER_ANCHOR, SITE_ANCHOR, COMMIT_ANCHOR, RESET_ANCHOR):
        if s.count(needle) != 1:
            print(f"[copyprop-var hooks] ERROR: anchor not unique/found: "
                  f"{needle[:40]!r} (count {s.count(needle)})")
            return False
    s = s.replace(HELPER_ANCHOR, HELPER + "\n" + HELPER_ANCHOR, 1)
    s = s.replace(SITE_ANCHOR, SITE_CODE, 1)
    s = s.replace(COMMIT_ANCHOR, COMMIT_CODE, 1)
    s = s.replace(RESET_ANCHOR, RESET_CODE, 1)
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[copyprop-var hooks] Hook 58 applied to {path}")
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
        print("[copyprop-var hooks] uopt rebuilt")


if __name__ == "__main__":
    main()
