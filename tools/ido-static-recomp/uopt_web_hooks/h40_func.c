
static void f_resetbit(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1);

/*
 * ido52_apply_veto_web: right after f_makelivranges(), before f_regdataflow().
 */
static void ido52_apply_veto_web(uint8_t *mem, uint32_t sp, int stage) {
    uint32_t bittab_ptr, numwebs;
    int i;
    if (ido52_veto_web_count == 0) return;
    if ((ido52_veto_web_stage & stage) == 0) return;
    bittab_ptr = MEM_U32(0x1001cbd0);
    numwebs = MEM_U32(0x1001cad8);
    if (bittab_ptr == 0) return;
    if (numwebs > 100000u) numwebs = 100000u;
    for (i = 0; i < ido52_veto_web_count; i++) {
        int bp = ido52_veto_web[i];
        uint32_t ent, lr;
        if (bp < 0 || (uint32_t)bp >= numwebs) {
            fprintf(stderr, "[VETO-WEB] bitpos %d out of range (numwebs %u)"
                            " -- SKIPPED\n", bp, numwebs);
            continue;
        }
        ent = bittab_ptr + (uint32_t)bp * 8;
        if (!ido52_mem_readable(mem, ent, 8)) continue;
        lr = MEM_U32(ent + 4);
        if (lr == 0) {
            fprintf(stderr, "[VETO-WEB] bitpos %d: no liverange -- SKIPPED\n", bp);
            continue;
        }
        /* drop it from both colouring worklists first */
        f_resetbit(mem, sp, 0x100223a8, (uint32_t)bp);
        f_resetbit(mem, sp, 0x100223a0, (uint32_t)bp);
        f_resetbit(mem, sp, 0x1001cb98, (uint32_t)bp);
        if (ido52_veto_web_droplr) {
            MEM_U32(ent + 4) = 0;
        }
        fprintf(stderr, "[VETO-WEB] stage=%d bitpos %d lr=0x%08x removed from"
                        " the allocator (droplr=%d)\n", stage, bp, lr,
                ido52_veto_web_droplr);
    }
}

/* Hook 40b: wrapper around f_formlivbb -- see IDO52_VETO_FORMLIV above. */
static void f_formlivbb_orig(uint8_t *mem, uint32_t sp, uint32_t a0,
                             uint32_t a1, uint32_t a2);
static void f_formlivbb(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1,
                        uint32_t a2) {
    int occ = ido52_formliv_occ++;
    int i;
    if (ido52_formliv_trace) {
        fprintf(stderr, "[FORMLIV] occ=%d site=%d a0=0x%08x a1=0x%08x\n",
                occ, ido52_pform_flbb_site, a0, a1);
    }
    for (i = 0; i < ido52_veto_formliv_count; i++) {
        if (ido52_veto_formliv[i].occ == occ
            && (ido52_veto_formliv[i].site < 0
                || ido52_veto_formliv[i].site == ido52_pform_flbb_site)) {
            fprintf(stderr, "[FORMLIV] occ=%d site=%d VETOED\n",
                    occ, ido52_pform_flbb_site);
            return;
        }
    }
    f_formlivbb_orig(mem, sp, a0, a1, a2);
}

static void ido52_formliv_reset(void) { ido52_formliv_occ = 0; }
