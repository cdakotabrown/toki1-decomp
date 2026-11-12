#!/usr/bin/env python3
"""Hook 65: IDO52_AS1_FTF_FORCE -- let as1's fall-through delay-slot fill
commit an `$at`-only writer whose branch TARGET block as1 holds `$at` live
into, when the target's first `$at` access is provably a DEFINITION.

Idempotent (probes for its own marker).  REQUIRES Hook 27
(apply_as1_delay_hooks.py) to be applied first: it reuses that hook's
fall-through-fill occurrence counter (as1_ftf_seq), so `FTF_FORCE=occ:N`
indexes exactly the `[AS1-FTF] #N` candidates IDO52_AS1_FTF_TRACE prints and
IDO52_AS1_NO_FALLTHRU_FILL vetoes.  Part of the post-`make clean` chain
(CLAUDE.md Known Gotchas / UOPT-HOOKS-REFERENCE Hook 65).

    python3 tools/ido-static-recomp/apply_as1_ftfforce_hooks.py [--as1 PATH] [--rebuild]

THE DECISION
------------
func_41d7b4 (per-block emit) tries to drop the NEXT block's first word into
the previous block's `branch ; nop` slot.  After f_eligible and
f_cannot_trap it asks func_41d22c(word, bb):

  * f_defuse(word) -> def mask (sp+52/56/60) and use mask (sp+40..);
    a def outside the GPR word (sp+56, sp+60) refuses;
  * if the global optimisation flag 0x1002f8b0 is CLEAR: accept only when
    the def mask is exactly 0x40000000 (== `$at` alone) -- as1's own
    "an $at-only writer may always go in the slot" rule;
  * if it is SET (our -O2 compiles): refuse a load (f_loads_from_memory),
    find the branch's TARGET block (f_find_branch_target) and refuse when
    def_mask & target->live_in (bb+36) != 0.

The last test is the one this hook lifts, and only when ALL of:
  (1) the occurrence is selected (all | N,M | occ:N,M);
  (2) the def mask is exactly 0x40000000 and the other def words are zero
      (func_41d22c has already refused otherwise), AND the word decodes as
      a register-writing ALU op (SPECIAL shift/arith/logic/slt with rd==1,
      or addi/addiu/slti/sltiu/andi/ori/xori/lui with rt==1);
  (3) a scan of the target block's instruction records, in order, finds
      an instruction whose f_defuse def mask contains `$at` BEFORE any
      whose use mask does (DEF-FIRST).  USE-FIRST or NONE-IN-BLOCK refuses.
Loads, hazards, trap checks, f_eligible and the pc-contiguity test are
untouched.  Because (3) is checked by the hook, a forced fill is sound: the
slot's `$at` value is dead on the taken path (redefined before any read),
and on the fall-through path the word executes exactly where it did.

ENV
---
IDO52_AS1_FTF_FORCE=all|N,M,..|occ:N,..   the Hook 27 occurrence indices
IDO52_AS1_FTF_FORCE_TRACE=1   `[AS1-FTF] #N cand=<word> target=<bb>@<pc>
    live_at=<0|1> at_only=<0|1> first_at=<word>[ (after break)]
    DEF-FIRST|USE-FIRST|UNDECIDED -> FORCED|REFUSED`
    for every candidate func_41d22c refuses on the live-at-target test
    (selected or not), so the index space is enumerable.
Default OFF: with the env unset the hook returns before touching anything.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
AS1_C = os.path.join(HERE, "build", "5.2", "as1.c")

MARKER = "as1_ftff_force"

HELPERS = r"""/* --- IDO52_AS1_FTF_FORCE (Hook 65, 2026-09-25): skip func_41d22c's
 * live-at-TARGET test for an $at-only writer whose target block defines
 * $at before reading it.  See apply_as1_ftfforce_hooks.py.  Default OFF. --- */
static struct as1_df_sel as1_ftff_sel;
static int as1_ftff_inited, as1_ftff_trace;

static int as1_ftff_writes_only_at(uint32_t w) {
    uint32_t op = w >> 26, fn = w & 0x3f;
    if (op == 0) {
        if (fn == 0x00 || fn == 0x02 || fn == 0x03 || fn == 0x04 ||
            fn == 0x06 || fn == 0x07 || (fn >= 0x20 && fn <= 0x27) ||
            fn == 0x2a || fn == 0x2b)
            return ((w >> 11) & 31) == 1;
        return 0;
    }
    if (op >= 8 && op <= 15) return ((w >> 16) & 31) == 1;
    return 0;
}

/* sp: func_41d22c's frame (its def/use scratch at sp+52 / sp+40 is dead by
 * now and is reused for f_defuse).  tbb: the branch-target block.
 * Walks EVERY path out of tbb (successor list bb+16: node+0 block, node+4
 * next) through the blocks' instruction records in order, until each path
 * either DEFINES $at (safe), USES $at (unsafe), or leaves the function
 * through `jr ra` (safe: $at is dead at a return).  Returns 1 when every
 * path is safe, -1 on the first USE (*first = that word), 0 when the walk
 * could not decide (block budget, a dead end that is not `jr ra`).
 * *first = the first defining word met, *brk = a `break` was passed
 * before the def on some path (see the Hook 65 doc: trap-visible $at). */
#define AS1_FTFF_MAXBB 256
static int as1_ftff_scan(uint8_t *mem, uint32_t sp, uint32_t tbb,
                         uint32_t *first, int *brk) {
    uint32_t stack[AS1_FTFF_MAXBB], seen[AS1_FTFF_MAXBB];
    int nstack = 0, nseen = 0, j;
    *first = 0;
    *brk = 0;
    stack[nstack++] = tbb;
    seen[nseen++] = tbb;
    while (nstack > 0) {
        uint32_t bb = stack[--nstack];
        uint32_t base = MEM_U32(bb + 12), node, last = 0;
        int n = (int)MEM_U16(bb + 60), i, done = 0, sawbrk = 0;
        (void)sawbrk;
        for (i = 1; i <= n && !done; i++) {
            uint32_t w = MEM_U32(base + (uint32_t)i * 0x28);
            uint32_t d, u;
            if (w == 0x7fffffffu) continue;
            last = w;
            if ((w >> 26) == 0 && (w & 0x3f) == 0x0d) { sawbrk = 1; *brk = 1; }
            f_defuse(mem, sp, w, sp + 0x34, sp + 0x28);
            d = MEM_U32(sp + 52);
            u = MEM_U32(sp + 40);
            if (u & 0x40000000u) { *first = w; return -1; }
            if (d & 0x40000000u) {
                if (*first == 0) *first = w;
                done = 1;
            }
        }
        if (done) continue;
        if (MEM_U32(bb + 16) == 0) {
            /* a path out of the function: only `jr ra` (+ its slot) is
             * accepted as proof that $at dies here */
            int k;
            for (k = n; k >= 1; k--) {
                uint32_t w = MEM_U32(base + (uint32_t)k * 0x28);
                if (w == 0x03e00008u) break;
            }
            if (k < 1) return 0;
            (void)last;
            continue;
        }
        for (node = MEM_U32(bb + 16); node; node = MEM_U32(node + 4)) {
            uint32_t s = MEM_U32(node);
            int dup = 0;
            for (j = 0; j < nseen; j++) if (seen[j] == s) { dup = 1; break; }
            if (dup) continue;
            if (nseen >= AS1_FTFF_MAXBB || nstack >= AS1_FTFF_MAXBB) return 0;
            seen[nseen++] = s;
            stack[nstack++] = s;
        }
    }
    return 1;
}

/* Called in func_41d22c exactly where def_mask & target_livein != 0 would
 * refuse.  Returns 1 to accept the fill instead. */
static int as1_ftff_force(uint8_t *mem, uint32_t sp, uint32_t word,
                          uint32_t defm, uint32_t tbb) {
    int idx, sel, scan = 2, ok = 0, brk = 0;
    uint32_t first = 0;
    if (!as1_ftff_inited) {
        const char *e = getenv("IDO52_AS1_FTF_FORCE_TRACE");
        as1_ftff_inited = 1;
        as1_df_parse(&as1_ftff_sel, getenv("IDO52_AS1_FTF_FORCE"));
        as1_ftff_trace = (e != NULL && *e && strcmp(e, "0") != 0);
    }
    if (!as1_ftff_sel.mode && !as1_ftff_trace) return 0;
    idx = as1_ftf_seq - 1;   /* Hook 27's index of this same candidate */
    sel = as1_df_hit(&as1_ftff_sel, idx);
    if (defm == 0x40000000u && as1_ftff_writes_only_at(word)) {
        scan = as1_ftff_scan(mem, sp, tbb, &first, &brk);
        ok = (scan == 1);
    }
    if (as1_ftff_trace)
        fprintf(stderr, "[AS1-FTF] #%d cand=%08x target=%08x@%04x live_at=%d "
                        "at_only=%d first_at=%08x%s %s -> %s\n",
                idx, word, tbb, MEM_U32(tbb + 84),
                (MEM_U32(tbb + 36) & 0x40000000u) != 0,
                defm == 0x40000000u && as1_ftff_writes_only_at(word), first,
                brk ? " (after break)" : "",
                scan == 1 ? "DEF-FIRST" : scan == -1 ? "USE-FIRST" :
                scan == 0 ? "UNDECIDED" : "NOT-SCANNED",
                (sel && ok) ? "FORCED" : "REFUSED");
    return sel && ok;
}
"""

FUNC_HEAD = ("static uint32_t func_41d22c(uint8_t *mem, uint32_t sp, "
             "uint32_t a0, uint32_t a1) {\n")

SITE_OLD = """t1 = MEM_U32(sp + 52);
t2 = MEM_U32(v0 + 36);
//nop;
t3 = t1 & t2;
"""
SITE_NEW = """t1 = MEM_U32(sp + 52);
t2 = MEM_U32(v0 + 36);
//nop;
t3 = t1 & t2;
/* === IDO52_AS1_FTF_FORCE (Hook 65): v0 = branch-target block, t1 = the
 * candidate's GPR def mask, sp+64 = the candidate word. */
if ((int32_t)t3 != (int32_t)0 && as1_ftff_force(mem, sp, MEM_U32(sp + 64), t1, v0)) {
    t3 = 0;
}
"""


def rebuild_cmds():
    return [
        ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
         "-o", "build/5.2/as1.o", "build/5.2/as1.c"],
        ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
         "-o", "build/5.2/out/as1.new", "build/5.2/as1.o", "build/5.2/version_info.o",
         "build/5.2/libc_impl_52.o", "-lm"],
    ]


def apply(path):
    s = open(path, "rb").read().decode("latin-1")
    if MARKER in s:
        print("[ftfforce hooks] Hook 65 already applied")
        return True
    if "static int as1_ftf_seq;" not in s or "static void as1_df_parse(" not in s:
        print("[ftfforce hooks] ERROR: Hook 27 (apply_as1_delay_hooks.py) must be "
              "applied first -- as1_ftf_seq / as1_df_parse missing")
        return False
    if s.count(FUNC_HEAD) != 1:
        print(f"[ftfforce hooks] ERROR: func_41d22c definition count {s.count(FUNC_HEAD)}")
        return False
    start = s.index(FUNC_HEAD)
    end = s.index("\n}\n", start)
    body = s[start:end]
    if body.count(SITE_OLD) != 1:
        print(f"[ftfforce hooks] ERROR: live-at-target test not unique in "
              f"func_41d22c (count {body.count(SITE_OLD)})")
        return False
    body = body.replace(SITE_OLD, SITE_NEW, 1)
    s = s[:start] + HELPERS + "\n" + body + s[end:]
    open(path, "wb").write(s.encode("latin-1"))
    print(f"[ftfforce hooks] Hook 65 applied to {path}")
    return True


def main():
    argv = sys.argv[1:]
    path = AS1_C
    if "--as1" in argv:
        path = argv[argv.index("--as1") + 1]
    if not apply(path):
        sys.exit(1)
    if "--rebuild" in argv:
        for cmd in rebuild_cmds():
            r = subprocess.run(cmd, cwd=HERE, capture_output=True, text=True)
            if r.returncode != 0:
                print(r.stderr[-2000:])
                sys.exit(1)
        os.replace(os.path.join(HERE, "build/5.2/out/as1.new"),
                   os.path.join(HERE, "build/5.2/out/as1"))
        print("[ftfforce hooks] as1 rebuilt")


if __name__ == "__main__":
    main()
