
/*
 * ido52_apply_force_web_spill: before f_globalcolor().  Marks each named
 * web "uncolourable" (lr+35 = 2) so the allocator's own L46ea34 epilogue
 * spills it.
 */
static void ido52_apply_force_web_spill(uint8_t *mem) {
    uint32_t bittab_ptr, numwebs;
    int i;
    if (ido52_force_web_spill_count == 0
        && ido52_force_web_nospill_count == 0) return;
    bittab_ptr = MEM_U32(0x1001cbd0);
    numwebs = MEM_U32(0x1001cad8);
    if (bittab_ptr == 0) return;
    if (numwebs > 100000u) numwebs = 100000u;
    for (i = 0; i < ido52_force_web_spill_count; i++) {
        int bp = ido52_force_web_spill[i];
        uint32_t ent, lr;
        if (bp < 0 || (uint32_t)bp >= numwebs) {
            fprintf(stderr, "[WEB-SPILL] bitpos %d out of range (numwebs %u)"
                            " -- SKIPPED\n", bp, numwebs);
            continue;
        }
        ent = bittab_ptr + (uint32_t)bp * 8;
        if (!ido52_mem_readable(mem, ent, 8)) continue;
        lr = MEM_U32(ent + 4);
        if (lr == 0 || !ido52_mem_readable(mem, lr, 0x34)) {
            fprintf(stderr, "[WEB-SPILL] bitpos %d: no liverange -- SKIPPED\n", bp);
            continue;
        }
        MEM_U8(lr + 35) = 2;
        fprintf(stderr, "[WEB-SPILL] bitpos %d lr=0x%08x marked uncolourable\n",
                bp, lr);
    }
    for (i = 0; i < ido52_force_web_nospill_count; i++) {
        int bp = ido52_force_web_nospill[i];
        uint32_t ent, lr;
        if (bp < 0 || (uint32_t)bp >= numwebs) {
            fprintf(stderr, "[WEB-SPILL] nospill bitpos %d out of range"
                            " (numwebs %u) -- SKIPPED\n", bp, numwebs);
            continue;
        }
        ent = bittab_ptr + (uint32_t)bp * 8;
        if (!ido52_mem_readable(mem, ent, 8)) continue;
        lr = MEM_U32(ent + 4);
        if (lr == 0 || !ido52_mem_readable(mem, lr, 0x34)) {
            fprintf(stderr, "[WEB-SPILL] nospill bitpos %d: no liverange"
                            " -- SKIPPED\n", bp);
            continue;
        }
        MEM_U8(lr + 35) = 1;
        fprintf(stderr, "[WEB-SPILL] bitpos %d lr=0x%08x forced COLOURABLE\n",
                bp, lr);
    }
}
