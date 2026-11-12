"""Hook 75: IDO52_AS1_PASS1_VOLATILE_OK -- let as1's pass-I copy
substitution process an instruction assembled under `.set volatile`.

Idempotent (probes for its own marker); independent of every other applier
(its only anchor is recomp label L419910 inside f_do_passI_opt, which no
other applier touches; Hook 44 patches a different statement in the same
function).  Run it after the as1.c regen, with the other as1 appliers.

    python3 tools/ido-static-recomp/apply_as1_pass1vol_hooks.py [--as1 PATH] [--rebuild]

WHY
---
f_do_passI_opt (func_419268's pass-I driver) walks each basic block's
records and, for every record with byte +34 set (the reorder flag) and byte
+33 clear (the `.set volatile` flag, global 0x1002f8f8), calls func_414bf8,
which substitutes a copy's SOURCE register into later uses of its
destination.  A record with +33 set is never offered: as1 calls
func_412b40 instead, which flushes the copy table.

The class this closes: ugen emits

      andi  t7, t6, 0xff
      move  v0, t7          # the variable's copy
      sb    v0, D(at)       # D is a volatile u8

Retail's assembler substituted the copy source into the volatile store
(`sb t7, D(at)`) and kept the copy because v0 stays live.  Native as1 never
offers the `sb` (volatile) and stores v0.  Seen on GYOZI/func_8013E478 and
EVENT/func_80153540, both `sb` to a volatile byte.

ENV
---
IDO52_AS1_PASS1_VOLATILE_OK=all|N,M,..|occ:N,..
                     offer every / the listed 0-based volatile records to
                     func_414bf8 as if +33 were clear.  Occurrences count
                     only records that reach the volatile test with +34
                     set; one counter per as1 run (= per TU).
IDO52_AS1_PASS1_VOLATILE_OK_TRACE=1
                     `[AS1-P1V] #N rec=<ptr> word=<insn> -> OFFERED|SKIPPED`
                     per arrival.

Default OFF: with the variable unset the hook returns 0 and the original
control flow runs.

SOUNDNESS.  func_414bf8 only renames source registers of the record to a
register that provably holds the same value (the copy table is per BB and
is killed at every redefinition, which func_414bf8 itself performs for the
record's destination).  The volatile flag constrains MEMORY ordering
(as1's reorder passes); the record's memory access is unchanged -- same
opcode, same address, same width, same value.  The flush that the
volatile path performs is skipped for an offered record; the table stays
valid because func_414bf8 kills any entry the record redefines.

Re-offers: f_do_passI_opt walks a block once per call, but func_419268 can
call it again after a change.  An offered record is remembered by
(record pointer, word) and its re-offers are offered again WITHOUT a new
occurrence number, so numbering does not depend on the offer set.
"""
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
AS1_C = os.path.join(HERE, "build", "5.2", "as1.c")

MARKER = "as1_p1v_offer"

HELPERS = r"""/* --- IDO52_AS1_PASS1_VOLATILE_OK (Hook 75, 2026-10-01): offer a
 * `.set volatile` record to pass-I copy substitution.  See
 * apply_as1_pass1vol_hooks.py.  Default OFF. --- */
static int as1_p1v_inited, as1_p1v_mode, as1_p1v_trace, as1_p1v_seq;
static int as1_p1v_list[256], as1_p1v_nlist;
static uint32_t as1_p1v_key[4096][2];
static int as1_p1v_nkey;
static void as1_p1v_init(void) {
    const char *e;
    char *pp;
    if (as1_p1v_inited) return;
    as1_p1v_inited = 1;
    e = getenv("IDO52_AS1_PASS1_VOLATILE_OK_TRACE");
    as1_p1v_trace = (e != NULL && *e && strcmp(e, "0") != 0);
    e = getenv("IDO52_AS1_PASS1_VOLATILE_OK");
    if (e == NULL || e[0] == '\0') return;
    if (strcmp(e, "all") == 0) { as1_p1v_mode = 1; return; }
    pp = (char *)e;
    if (strncmp(pp, "occ:", 4) == 0) pp += 4;
    as1_p1v_mode = 2;
    while (*pp != '\0' && as1_p1v_nlist < 256) {
        char *q = pp;
        as1_p1v_list[as1_p1v_nlist++] = (int)strtol(pp, &pp, 0);
        if (pp == q) { as1_p1v_nlist--; break; }
        while (*pp == ',' || *pp == ' ') pp++;
    }
}
/* rec: the record pointer; w: its instruction word.  Returns 1 to offer the
 * record to func_414bf8 as if it were not volatile. */
static int as1_p1v_offer(uint32_t rec, uint32_t w) {
    int i, n, hit = 0;
    as1_p1v_init();
    if (!as1_p1v_mode && !as1_p1v_trace) return 0;
    for (i = 0; i < as1_p1v_nkey; i++)
        if (as1_p1v_key[i][0] == rec && as1_p1v_key[i][1] == w) {
            if (as1_p1v_trace)
                fprintf(stderr, "[AS1-P1V] re-offer rec=%08x word=%08x -> OFFERED\n",
                        rec, w);
            return 1;
        }
    n = as1_p1v_seq++;
    if (as1_p1v_mode == 1) hit = 1;
    else if (as1_p1v_mode == 2)
        for (i = 0; i < as1_p1v_nlist; i++)
            if (as1_p1v_list[i] == n) { hit = 1; break; }
    if (hit && as1_p1v_nkey < 4096) {
        as1_p1v_key[as1_p1v_nkey][0] = rec;
        as1_p1v_key[as1_p1v_nkey][1] = w;
        as1_p1v_nkey++;
    }
    if (as1_p1v_trace)
        fprintf(stderr, "[AS1-P1V] #%d rec=%08x word=%08x -> %s\n",
                n, rec, w, hit ? "OFFERED" : "SKIPPED");
    return hit;
}
"""

SITE_OLD = """L419910:
t4 = MEM_U8(v0 + 34);
//nop;
if ((int32_t)t4 == (int32_t)0) {
//nop;
goto L419930;}
//nop;
t5 = MEM_U8(v0 + 33);
//nop;
"""
SITE_NEW = """L419910:
t4 = MEM_U8(v0 + 34);
//nop;
if ((int32_t)t4 == (int32_t)0) {
//nop;
goto L419930;}
//nop;
t5 = MEM_U8(v0 + 33);
/* === IDO52_AS1_PASS1_VOLATILE_OK (Hook 75): offer a volatile record to
 * pass-I substitution as if +33 were clear (v0 = the record). */
if (t5 != 0 && as1_p1v_offer(v0, MEM_U32(v0 + 0))) {
    t5 = 0;
}
//nop;
"""

FUNC_DECL = "static uint32_t f_do_passI_opt(uint8_t *mem, uint32_t sp, uint32_t a0) {\n"


def rebuild_cmds():
    return [
        ["gcc", "-c", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
         "-o", "build/5.2/as1.o", "build/5.2/as1.c"],
        ["gcc", "-std=c11", "-O0", "-ggdb3", "-fno-strict-aliasing", "-I.",
         "-o", "build/5.2/out/as1.new", "build/5.2/as1.o", "build/5.2/version_info.o",
         "build/5.2/libc_impl_52.o", "-lm"],
    ]


def main():
    path = AS1_C
    rebuild = False
    args = sys.argv[1:]
    i = 0
    while i < len(args):
        if args[i] == "--as1":
            path = args[i + 1]
            i += 2
            continue
        if args[i] == "--rebuild":
            rebuild = True
        i += 1
    with open(path, "rb") as f:
        src = f.read().decode("latin-1")
    if MARKER in src:
        print("Hook 75 already applied to", path)
    else:
        if src.count(SITE_OLD) != 1:
            sys.exit("Hook 75: anchor L419910 not found exactly once")
        if src.count(FUNC_DECL) < 1:
            sys.exit("Hook 75: f_do_passI_opt definition not found")
        # helpers go right before the function definition (after its
        # prototype block, so getenv/fprintf/strtol are declared)
        idx = src.index(FUNC_DECL)
        # the first occurrence may be the definition; make sure it is the
        # one followed by a body
        while not src.startswith(FUNC_DECL, idx):
            idx = src.index(FUNC_DECL, idx + 1)
        src = src[:idx] + HELPERS + "\n" + src[idx:]
        src = src.replace(SITE_OLD, SITE_NEW, 1)
        with open(path, "wb") as f:
            f.write(src.encode("latin-1"))
        print("Hook 75 applied to", path)
    if rebuild:
        for cmd in rebuild_cmds():
            subprocess.check_call(cmd, cwd=HERE)
        out = os.path.join(HERE, "build", "5.2", "out")
        os.replace(os.path.join(out, "as1.new"), os.path.join(out, "as1"))
        print("as1 rebuilt")


if __name__ == "__main__":
    main()
