
/* Hook 38b: override the adjsave quotient stored by f_compute_save. */
static void ido52_force_adjsave(uint8_t *mem, uint32_t lr) {
    int i, bp;
    if (ido52_force_web_adjsave_count == 0) return;
    if (lr == 0 || !ido52_mem_readable(mem, lr, 0x34)) return;
    bp = ido52_gc_bitpos_of_lr(mem, lr);
    for (i = 0; i < ido52_force_web_adjsave_count; i++) {
        if (ido52_force_web_adjsave[i].bitpos == bp) {
            union { uint32_t u; float f; } nv, ov;
            ov.u = MEM_U32(lr + 48);
            nv.f = ido52_force_web_adjsave[i].value;
            MEM_U32(lr + 48) = nv.u;
            /* keep the "worth colouring" flag consistent with the new cost */
            MEM_U8(lr + 35) = (uint8_t)(nv.f > 0.0f ? 1 : 0);
            if (ido52_web_home_trace) {
                fprintf(stderr, "[WEB-HOME] adjsave bitpos=%d %.6f -> %.6f\n",
                        bp, (double)ov.f, (double)nv.f);
            }
        }
    }
}

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
static void ido52_apply_force_web_home(uint8_t *mem, int stage) {
    uint32_t bittab_ptr, numwebs, i;
    if ((ido52_web_home_stage & stage) == 0) return;
    if (ido52_force_web_home_count == 0 && ido52_web_home_trace == 0) return;
    bittab_ptr = MEM_U32(0x1001cbd0);
    numwebs = MEM_U32(0x1001cad8);
    if (bittab_ptr == 0) return;
    if (numwebs > 100000u) numwebs = 100000u;

    if (ido52_web_home_trace) {
        fprintf(stderr, "[WEB-HOME] stage=%d: %u webs\n", stage, numwebs);
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
        fprintf(stderr, "[WEB-HOME] stage=%d bitpos %d bb=%d mask=%d applied to %d"
                        " LiveUnit(s)\n", stage, bp, wantbb, mask, hits);
        if (ido52_web_home_trace) {
            ido52_web_home_dump_one(mem, bp, lr, " AFTER");
        }
    }
}
