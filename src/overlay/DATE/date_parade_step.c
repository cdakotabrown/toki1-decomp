/* @SymbolStart: 0x80135F0C */
/* @SymbolSize: 836 */
/* Function: date_parade_step (absorbs cross-prologue head func_80135F0C) */
/* Overlay: DATE */
/* Address: 0x80135F0C (head) / 0x80135F4C (labelled entry) */
/* Size: 836 bytes (209 instructions: 16-insn hoisted head + 193-insn body) */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: patch_insn_reg_field 0x23c:rt:t3:t4 0x260:rd:t4:t3 0x264:rd:t4:t3 0x264:rs:t4:t3 0x268:rs:t4:t3 0x278:rt:t4:t3 0x298:rt:t4:t3 0x2a0:rt:t4:t3 0x2a4:rt:t0:t1 0x2a8:rs:t0:t1 0x2a8:rt:t0:t1 0x2ac:rt:t4:t3 0x2d4:rd:t3:t0 0x2d8:rd:t1:t2 0x2d8:rs:t3:t0 0x2d8:rt:t0:t1 0x2dc:rd:t2:t3 0x2dc:rs:t1:t2 0x2e4:rs:t2:t3 0x2e4:rt:t4:t5 0x2ec:rt:t4:t5 0x2f4:rd:t5:t6 0x2f8:rd:t6:t7 0x2f8:rs:t5:t6 0x2f8:rt:t0:t1 0x2fc:rd:t7:t8 0x2fc:rs:t6:t7 0x304:rs:t7:t8 0x304:rt:t8:t9 0x30c:rt:t8:t9 0x314:rd:t9:t0 0x318:rd:t3:t2 0x318:rs:t9:t0 0x318:rt:t0:t1 0x31c:rd:t1:t3 0x31c:rs:t3:t2 0x320:rs:t1:t3 0x320:rt:t2:t5 0x328:rt:t2:t5 0x240:rs:t3:t4 0x240:rt:a2:t4 0x244:rt:a1:v1 0x248:rs:a2:t4 0x248:rt:a1:v1 0x27c:rs:a2:t4 0x27c:rt:a1:v1 0x29c:rs:a2:t4 0x29c:rt:a1:v1 0x2c0:rs:a2:t4 0x2c0:rt:a1:v1 0x2e0:rs:a2:t4 0x2e0:rt:a1:v1 0x300:rs:a2:t4 0x300:rt:a1:v1 */
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
extern u8  date_parade_step_str0;
extern u8  date_parade_step_str1;
extern u8  date_serifu_stride;
extern u8  date_parade_season;
extern u8 *date_impkao_tbl_7_imp;
extern u8 *date_impkao_tbl_7_kao;
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

void date_parade_step(void)
{
    s32 v;
    s32 w;
    u8  gv;
    u8 *p1;
    u8 *p2;
    s32 c3;

    ss_serifu = date_serifu_tbl[8];
    ss_serifu_kind = date_serifu_kind_tbl[8];
    sss_sound = date_sound_tbl[8];
    date_serifu_stride = 0;

    v = now_year;
    if (v == 95) {
        w = now_month;
        if (w >= 6U && w < 9U) {
            date_parade_season = 0;
        } else if (w >= 9U && w < 12U) {
            date_parade_season = 1;
        } else {
            date_parade_season = 2;
        }
    } else if (v == 96) {
        w = now_month;
        if (w != 0 && w < 3U) {
            date_parade_season = 2;
        } else if (w >= 3U && w < 6U) {
            date_parade_season = 3;
        } else if (w >= 6U && w < 9U) {
            date_parade_season = 4;
        } else if (w >= 9U && w < 12U) {
            date_parade_season = 5;
        } else {
            date_parade_season = 6;
        }
    } else if (v == 97) {
        w = now_month;
        if (w != 0 && w < 3U) {
            date_parade_season = 6;
        } else if (w >= 3U && w < 6U) {
            date_parade_season = 7;
        } else if (w >= 6U && w < 9U) {
            date_parade_season = 8;
        } else if (w >= 9U && w < 12U) {
            date_parade_season = 9;
        } else {
            date_parade_season = 10;
        }
    } else {
        date_parade_season = 10;
    }

    now_date_spot = 35;
    strcpy(&ba_string, &date_parade_step_str0);
    strcpy(&ba_string2, &date_parade_step_str1);

    gv = now_girl;
    p1 = date_impkao_tbl_7_imp;
    c3 = (date_parade_season * 3) | 0;
    imp[0] = (p1 + gv * 33U)[c3 + 0];
    imp[1] = (p1 + (u32)gv * 33U)[c3 + 1];
    p2 = date_impkao_tbl_7_kao;
    imp[2] = (p1 + (gv | 0U) * 33U)[c3 + 2];
    kao[0] = (p2 + (gv ^ 0U) * 33U)[c3 + 0];
    kao[1] = (p2 + (gv * 1U) * 33U)[c3 + 1];
    kao[2] = (p2 + (gv >> 0) * 33U)[c3 + 2];

    dec_bg_cache_step(17392);
    event_log_add(1, now_date_spot_b);
    sub_sub_step_up();
}
