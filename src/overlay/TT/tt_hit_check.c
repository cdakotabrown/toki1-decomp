/* @SymbolSize: 840 */
/* @PostProc: swap_instructions --offset1 0x2a4 --offset2 0x2a8 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=5:s5 */
/* @PostProc: remap_stack_slots --map '91:95,108:112,112:120' */
/* @PostProc: patch_insn_reg_field 0x290:rt:s2:s3 0x294:rs:s2:s3 0x294:rt:s2:s3 0x2a4:rt:s3:s4 0x2a8:rt:s4:s2 0x2d8:rs:s2:s3 0x2fc:rt:s3:s4 0x31c:rt:s4:s2 */
/* Address: 0x80142798, Size: 840 bytes (210 instructions) */
/* sp_00NN slot names) with `@PostProc: pad_trailing_nop --count 158` on a */
/* - ⭐ `@CompileEnv: IDO52_FORCE_WEB_COLOR=5:s5` (Hook 37).  Our globalcolor */
#include "types.h"
#include "tokimeki/tt_box.h"

extern s32 tt_shot_aimed_make();
extern s32 tt_shot_spread_make();
extern s32 tt_shot_fan4_make();
extern s32 tt_widget_pair_init(void);
extern s32 tt_box_overlap_check();

extern u8 *tt_step_wk;
extern u8 *tt_scroll_wk;
extern u8 *tt_player_wk;
extern u8 *tt_player_shot_buf;
extern u8 *tt_boss_wk;
extern u8  tt_hit_box_tbl[];

s32 tt_hit_check(void)
{
    u8 *p;
    u8 *q;
    u8 *r;
    u8 *e;
    u8 *f;
    s32 i;
    s32 j;
    s32 k;
    s32 v;
    u8 ret;

    q = tt_step_wk;
    p = tt_boss_wk;
    r = tt_player_shot_buf;
    e = tt_scroll_wk;
    f = tt_player_wk;
    ret = 0;
    for (i = 0; i < 12; i++) {
        if (r[0] != 0) {
            if (*(u16 *)(r + 4) == 64) {
                for (j = 0; j < 4; j++) {
                    if (((s32 *)(p + 536))[j] != 0) {
                        if (tt_box_overlap_check(r + 20, ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[1].x), p + 20,
                                          ((s32 *)(p + 536))[j]) != 0) {
                            q[37] = q[37] + 1;
                            ret = 1;
                            if (q[36] != 0) {
                                v = q[37];
                                if ((v & 3) == 0) {
                                    if (e[11] < 2U) {
                                        if ((v & 7) == 0) {
                                            tt_shot_fan4_make(*(s16 *)(r + 20),
                                                          *(s16 *)(r + 22),
                                                          256, 32, 160);
                                        } else {
                                            tt_shot_spread_make(*(s16 *)(r + 20),
                                                          *(s16 *)(r + 22),
                                                          256, 32, 160);
                                        }
                                    } else {
                                        tt_shot_aimed_make(*(s16 *)(r + 20),
                                                      *(s16 *)(r + 22),
                                                      32, 160);
                                    }
                                }
                            }
                            for (k = 0; k != 24; k++) {
                                *(s8 *)(p + k * 20 + 66) = 1;
                            }
                            if (i >= 6) {
                                *(s16 *)(p + 780) = *(s16 *)(p + 780) - 1;
                            } else if (*(u16 *)(r + 2) == 9) {
                                *(s16 *)(p + 780) = *(s16 *)(p + 780) - 4;
                            } else {
                                *(s16 *)(p + 780) = *(s16 *)(p + 780) - 2;
                            }
                            *(u16 *)(r + 4) = 128;
                            *(s16 *)(r + 10) = 0;
                            *(s16 *)(r + 8) = 0;
                            *(s16 *)(r + 6) = 0;
                            r[15] = 0;
                            r[14] = 0;
                            r[13] = 0;
                        }
                    }
                }
                for (j = 4; j < 8; j++) {
                    if (((s32 *)(p + 536))[j] != 0) {
                        if (tt_box_overlap_check(r + 20, ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[1].x), p + 20,
                                          ((s32 *)(p + 536))[j]) != 0) {
                            *(u16 *)(r + 4) = 128;
                            *(s16 *)(r + 10) = 0;
                            *(s16 *)(r + 8) = 0;
                            *(s16 *)(r + 6) = 0;
                            r[15] = 0;
                            r[14] = 0;
                            r[13] = 0;
                        }
                    }
                }
            }
        }
        r += 84;
    }
    for (i = 4; i < 12; i++) {
        if (((s32 *)(p + 536))[i] != 0) {
            if (f[82] == 0) {
                if (f[0] != 0) {
                    if (tt_box_overlap_check(f + 20, tt_hit_box_tbl, p + 20,
                                      ((s32 *)(p + 536))[i]) != 0) {
                        tt_widget_pair_init();
                        *(u16 *)(f + 4) = 96;
                        *(s16 *)(f + 10) = 0;
                        *(s16 *)(f + 8) = 0;
                        *(s16 *)(f + 6) = 0;
                        f[15] = 0;
                        f[14] = 0;
                        f[13] = 0;
                    }
                }
            }
        }
    }
    return ret;
}
