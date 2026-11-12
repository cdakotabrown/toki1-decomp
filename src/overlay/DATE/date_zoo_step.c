/* @SymbolStart: 0x801363D0 */
/* @SymbolSize: 868 */
/* Function: date_zoo_step (absorbs cross-prologue head func_801363D0) */
/* Overlay: DATE */
/* Address: 0x801363D0 (head) / 0x80136410 (labelled entry) */
/* Size: 868 bytes (217 instructions: 16-insn hoisted head + 201-insn body) */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: patch_insn_reg_field 0x25c:rt:t4:t5 0x260:rs:t4:t5 0x260:rt:a2:t5 0x264:rt:a1:v1 0x268:rs:a2:t5 0x268:rt:a1:v1 0x280:rd:t5:t4 0x284:rd:t5:t4 0x284:rs:t5:t4 0x288:rs:t5:t4 0x298:rt:t5:t4 0x29c:rs:a2:t5 0x29c:rt:a1:v1 0x2b8:rt:t5:t4 0x2bc:rs:a2:t5 0x2bc:rt:a1:v1 0x2c0:rt:t5:t4 0x2c4:rt:t1:t2 0x2c8:rs:t1:t2 0x2c8:rt:t1:t2 0x2cc:rt:t5:t4 0x2e0:rs:a2:t5 0x2e0:rt:a1:v1 0x2f4:rd:t4:t1 0x2f8:rd:t2:t3 0x2f8:rs:t4:t1 0x2f8:rt:t1:t2 0x2fc:rd:t3:t4 0x2fc:rs:t2:t3 0x300:rs:a2:t5 0x300:rt:a1:v1 0x304:rs:t3:t4 0x304:rt:t5:t6 0x30c:rt:t5:t6 0x314:rd:t6:t7 0x318:rd:t7:t8 0x318:rs:t6:t7 0x318:rt:t1:t2 0x31c:rd:t8:t9 0x31c:rs:t7:t8 0x320:rs:a2:t5 0x320:rt:a1:v1 0x324:rs:t8:t9 0x324:rt:t9:t0 0x32c:rt:t9:t0 0x334:rd:t0:t1 0x338:rd:t4:t3 0x338:rs:t0:t1 0x338:rt:t1:t2 0x33c:rd:t2:t4 0x33c:rs:t4:t3 0x340:rs:t2:t4 0x340:rt:t3:t6 0x348:rt:t3:t6 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern void strcpy(void *, void *);

extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s16 kao[];
extern s16 imp[];
extern u8  ba_string;
extern u8  ba_string2;
extern u8  now_year;
extern u8  now_month;
extern u8  now_girl;
extern s32 now_date_spot;

extern u8  now_date_spot_b;
extern u8  date_zoo_step_str0;
extern u8  date_zoo_step_str1;
extern u8  date_serifu_stride;
extern u8  date_zoo_season;
extern u8 *date_impkao_tbl_2_imp;
extern u8 *date_impkao_tbl_2_kao;
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

void date_zoo_step(void)
{
    s32 v;
    s32 w;
    u8  gv;
    u8 *p1;
    u8 *p2;
    s32 c3;

    ss_serifu = date_serifu_tbl[2];
    ss_serifu_kind = date_serifu_kind_tbl[2];
    sss_sound = date_sound_tbl[2];
    date_serifu_stride = 0;

    v = now_year;
    if (v == 95) {
        w = now_month;
        if (w >= 4U && w < 6U) {
            date_zoo_season = 0;
        } else if (w >= 6U && w < 9U) {
            date_zoo_season = 1;
        } else if (w >= 9U && w < 12U) {
            date_zoo_season = 2;
        } else {
            date_zoo_season = 3;
        }
    } else if (v == 96) {
        w = now_month;
        if (w != 0 && w < 3U) {
            date_zoo_season = 3;
        } else if (w >= 3U && w < 6U) {
            date_zoo_season = 4;
        } else if (w >= 6U && w < 9U) {
            date_zoo_season = 5;
        } else if (w >= 9U && w < 12U) {
            date_zoo_season = 6;
        } else {
            date_zoo_season = 7;
        }
    } else if (v == 97) {
        w = now_month;
        if (w != 0 && w < 3U) {
            date_zoo_season = 7;
        } else if (w >= 3U && w < 6U) {
            date_zoo_season = 8;
        } else if (w >= 6U && w < 9U) {
            date_zoo_season = 9;
        } else if (w >= 9U && w < 12U) {
            date_zoo_season = 10;
        } else {
            date_zoo_season = 11;
        }
    } else {
        date_zoo_season = 11;
    }

    now_date_spot = 36;
    strcpy(&ba_string, &date_zoo_step_str0);
    strcpy(&ba_string2, &date_zoo_step_str1);

    gv = now_girl;
    p1 = date_impkao_tbl_2_imp;
    c3 = (date_zoo_season * 3) | 0;
    imp[0] = (p1 + gv * 36U)[c3 + 0];
    imp[1] = (p1 + (u32)gv * 36U)[c3 + 1];
    p2 = date_impkao_tbl_2_kao;
    imp[2] = (p1 + (gv | 0U) * 36U)[c3 + 2];
    kao[0] = (p2 + (gv ^ 0U) * 36U)[c3 + 0];
    kao[1] = (p2 + (gv * 1U) * 36U)[c3 + 1];
    kao[2] = (p2 + (gv >> 0) * 36U)[c3 + 2];

    dec_bg_cache_step(17510);
    event_log_add(1, now_date_spot_b);
    sub_sub_step_up();
}
