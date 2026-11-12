
/* --- IDO52_FORCE_WEB_HOME (Hook 38, 2026-08-14) ---------------------------
 * Positive per-web WEB-HOME-STORE force.  Hook 37 chooses a web's colour;
 * this decides whether ugen writes the value INTO that colour at the def
 * (the `or $rHome,$rTemp,$zero` copy) when uopt's own cost model priced the
 * copy out.
 *
 * Mechanism (measured 2026-08-14, GYOZI/func_8013306C + the uopt listing):
 *   f_compute_save (L46c530..) accumulates a per-liverange benefit into
 *   f22 and divides it by an occurrence count at lr+28, storing the result
 *   as the float `adjsave` at lr+48; `hasstore` is the u8 at lr+34 and the
 *   "worth colouring" flag is lr+35.  f_globalcolor consults adjsave three
 *   times: L46eb48 (pick the highest-adjsave uncoloured web), L46ef64
 *   (`adjsave * lr[28] <= threshold` -> range-split), L46fa20
 *   (`threshold < adjsave * lr[28]` -> allocate).  The per-LiveUnit
 *   decisions that survive into the u-code re-emission are the four u8
 *   flags printed by f_printregs as
 *       firstisstr(lu+20)  deadout(lu+23)  needreglod(lu+21)  needregsave(lu+22)
 *   and the emission pass f_reemit (called right after globalcolor) is the
 *   consumer.  needregsave == the home store.
 *
 * IDO52_FORCE_WEB_HOME=BITPOS[:BB][=MASK][,...]
 *   BITPOS  bittab index (the SAME key space as Hooks 28 and 37)
 *   BB      restrict to the LiveUnit(s) whose block number (node+8, u16)
 *           equals BB; omitted = every LiveUnit of the web
 *   MASK    bit 1  set needregsave (lu+22) = 1    [default]
 *           bit 2  set needreglod  (lu+21) = 1
 *           bit 4  set hasstore    (lr+34) = 1
 *           bit 8  clear deadout   (lu+23) = 0
 *           bit 16 clear firstisstr(lu+20) = 0
 *           0      probe only (trace, no write)
 *
 * IDO52_WEB_HOME_TRACE=1  dump every web's adjsave / hasstore / assigned_reg
 *                         and each LiveUnit's bb + four flags, post-globalcolor.
 *                         (=2 also dumps before the force is applied.)
 *
 * SOUNDNESS: a forced home store is an EXTRA copy into a register the
 * allocator already owns for that web -- it never changes which register
 * any reader reads, and it never removes an instruction.  It is exactly the
 * copy uopt itself emits when adjsave clears its own gate.  The hook
 * refuses to act on a web with no assigned colour (assigned_reg 0 or 0xff),
 * because a home store into "no register" is meaningless.
 * ------------------------------------------------------------------- */
#define FORCE_WEB_HOME_MAX 24
static struct { int bitpos; int bb; int mask; } ido52_force_web_home[FORCE_WEB_HOME_MAX];
static int ido52_force_web_home_count;
static int ido52_web_home_trace;

static void ido52_web_home_dump_one(uint8_t *mem, int bp, uint32_t lr,
                                    const char *tag) {
    uint32_t lu;
    union { uint32_t u; float f; } as;
    if (lr == 0 || !ido52_mem_readable(mem, lr, 0x34)) return;
    as.u = MEM_U32(lr + 48);
    fprintf(stderr, "[WEB-HOME]%s bitpos=%d lr=0x%08x reg=%u hasstore=%u"
                    " adjsave=%.6f occ=%u worth=%u\n",
            tag, bp, lr, MEM_U8(lr + 0x20), MEM_U8(lr + 34), (double)as.f,
            MEM_U32(lr + 28), MEM_U8(lr + 35));
    for (lu = MEM_U32(lr + 8); lu != 0; lu = MEM_U32(lu + 4)) {
        uint32_t nd = MEM_U32(lu + 0);
        int bb = -1;
        if (!ido52_mem_readable(mem, lu, 0x18)) break;
        if (nd != 0 && ido52_mem_readable(mem, nd, 0x0a)) bb = (int)MEM_U16(nd + 8);
        fprintf(stderr, "[WEB-HOME]%s   lu=0x%08x bb=%d reg=%u firstisstr=%u"
                        " deadout=%u needreglod=%u needregsave=%u\n",
                tag, lu, bb, MEM_U8(lu + 0x13), MEM_U8(lu + 20),
                MEM_U8(lu + 23), MEM_U8(lu + 21), MEM_U8(lu + 22));
    }
}

/*
 * ido52_apply_force_web_home: after globalcolor(), before f_reemit().
 */
static void ido52_apply_force_web_home(uint8_t *mem) {
    uint32_t bittab_ptr, numwebs, i;
    if (ido52_force_web_home_count == 0 && ido52_web_home_trace == 0) return;
    bittab_ptr = MEM_U32(0x1001cbd0);
    numwebs = MEM_U32(0x1001cad8);
    if (bittab_ptr == 0) return;
    if (numwebs > 100000u) numwebs = 100000u;

    if (ido52_web_home_trace) {
        fprintf(stderr, "[WEB-HOME] %u webs after globalcolor\n", numwebs);
        for (i = 0; i < numwebs; i++) {
            uint32_t ent = bittab_ptr + i * 8;
            if (!ido52_mem_readable(mem, ent, 8)) break;
            ido52_web_home_dump_one(mem, (int)i, MEM_U32(ent + 4), "");
        }
    }

    for (i = 0; i < (uint32_t)ido52_force_web_home_count; i++) {
        int bp = ido52_force_web_home[i].bitpos;
        int wantbb = ido52_force_web_home[i].bb;
        int mask = ido52_force_web_home[i].mask;
        uint32_t ent, lr, lu;
        int hits = 0;
        if (bp < 0 || (uint32_t)bp >= numwebs) {
            fprintf(stderr, "[WEB-HOME] bitpos %d out of range (numwebs %u)"
                            " -- SKIPPED\n", bp, numwebs);
            continue;
        }
        ent = bittab_ptr + (uint32_t)bp * 8;
        if (!ido52_mem_readable(mem, ent, 8)) continue;
        lr = MEM_U32(ent + 4);
        if (lr == 0 || !ido52_mem_readable(mem, lr, 0x34)) {
            fprintf(stderr, "[WEB-HOME] bitpos %d: no liverange -- SKIPPED\n", bp);
            continue;
        }
        if (MEM_U8(lr + 0x20) == 0 || MEM_U8(lr + 0x20) == 0xff) {
            fprintf(stderr, "[WEB-HOME] bitpos %d: web is uncoloured"
                            " (assigned_reg=%u) -- SKIPPED\n",
                    bp, MEM_U8(lr + 0x20));
            continue;
        }
        if (mask & 4) MEM_U8(lr + 34) = 1;
        for (lu = MEM_U32(lr + 8); lu != 0; lu = MEM_U32(lu + 4)) {
            uint32_t nd = MEM_U32(lu + 0);
            int bb = -1;
            if (!ido52_mem_readable(mem, lu, 0x18)) break;
            if (nd != 0 && ido52_mem_readable(mem, nd, 0x0a)) bb = (int)MEM_U16(nd + 8);
            if (wantbb >= 0 && bb != wantbb) continue;
            if (mask & 1) MEM_U8(lu + 22) = 1;
            if (mask & 2) MEM_U8(lu + 21) = 1;
            if (mask & 8) MEM_U8(lu + 23) = 0;
            if (mask & 16) MEM_U8(lu + 20) = 0;
            hits++;
        }
        fprintf(stderr, "[WEB-HOME] bitpos %d bb=%d mask=%d applied to %d"
                        " LiveUnit(s)\n", bp, wantbb, mask, hits);
        if (ido52_web_home_trace) {
            ido52_web_home_dump_one(mem, bp, lr, " AFTER");
        }
    }
}
