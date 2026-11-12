/* @SymbolStart: 0x80133F30 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @SymbolSize: 972 */
/* Function: date_movie_effect_step (absorbs cross-prologue head func_80133F30) */
/* Overlay: DATE */
/* Address: 0x80133F30 (head) / 0x80133F5C (labelled entry) */
/* Size: 972 bytes (243 instructions: 11-insn hoisted head + 232-insn body) */
/* ANY "@CompileEnv: NAME=VALUE" text in this file, header prose included. */
/* @PostProc: remap_stack_slots --from 47 --to 43 */
/* @PostProc: swap_instructions --offset1 0x29c --offset2 0x2a4 */
/* @PostProc: swap_instructions --offset1 0x2a0 --offset2 0x2a8 */
/* @PostProc: swap_instructions --offset1 0x2ac --offset2 0x2b8 */
/* @PostProc: swap_instructions --offset1 0x2b0 --offset2 0x2b8 */
/* @PostProc: swap_instructions --offset1 0x2b4 --offset2 0x2b8 */
/* @PostProc: swap_instructions --offset1 0x2b8 --offset2 0x2bc */
/* @PostProc: swap_instructions --offset1 0x2bc --offset2 0x2c0 */
/* @PostProc: swap_instructions --offset1 0x2c0 --offset2 0x2c4 */
/* @PostProc: swap_instructions --offset1 0x2dc --offset2 0x324 */
/* @PostProc: swap_instructions --offset1 0x2e8 --offset2 0x2ec */
/* @PostProc: swap_instructions --offset1 0x2ec --offset2 0x2f0 */
/* @PostProc: swap_instructions --offset1 0x2f0 --offset2 0x2f4 */
/* @PostProc: swap_instructions --offset1 0x2f4 --offset2 0x2f8 */
/* @PostProc: swap_instructions --offset1 0x2f8 --offset2 0x2fc */
/* @PostProc: swap_instructions --offset1 0x2fc --offset2 0x300 */
/* @PostProc: swap_instructions --offset1 0x300 --offset2 0x304 */
/* @PostProc: swap_instructions --offset1 0x304 --offset2 0x308 */
/* @PostProc: swap_instructions --offset1 0x308 --offset2 0x30c */
/* @PostProc: swap_instructions --offset1 0x30c --offset2 0x310 */
/* @PostProc: swap_instructions --offset1 0x310 --offset2 0x314 */
/* @PostProc: swap_instructions --offset1 0x314 --offset2 0x318 */
/* @PostProc: swap_instructions --offset1 0x318 --offset2 0x31c */
/* @PostProc: swap_instructions --offset1 0x31c --offset2 0x320 */
/* @PostProc: swap_instructions --offset1 0x320 --offset2 0x344 */
/* @PostProc: swap_instructions --offset1 0x324 --offset2 0x328 */
/* @PostProc: swap_instructions --offset1 0x328 --offset2 0x32c */
/* @PostProc: swap_instructions --offset1 0x32c --offset2 0x330 */
/* @PostProc: swap_instructions --offset1 0x330 --offset2 0x334 */
/* @PostProc: swap_instructions --offset1 0x334 --offset2 0x338 */
/* @PostProc: swap_instructions --offset1 0x338 --offset2 0x33c */
/* @PostProc: swap_instructions --offset1 0x33c --offset2 0x340 */
/* @PostProc: swap_instructions --offset1 0x340 --offset2 0x364 */
/* @PostProc: swap_instructions --offset1 0x344 --offset2 0x348 */
/* @PostProc: swap_instructions --offset1 0x348 --offset2 0x34c */
/* @PostProc: swap_instructions --offset1 0x34c --offset2 0x350 */
/* @PostProc: swap_instructions --offset1 0x350 --offset2 0x354 */
/* @PostProc: swap_instructions --offset1 0x354 --offset2 0x358 */
/* @PostProc: swap_instructions --offset1 0x358 --offset2 0x35c */
/* @PostProc: swap_instructions --offset1 0x35c --offset2 0x360 */
/* @PostProc: swap_instructions --offset1 0x360 --offset2 0x364 */
/* @PostProc: swap_instructions --offset1 0x364 --offset2 0x368 */
/* @PostProc: patch_insn_reg_field 0xa8:rt:a1:v1 0xa0:rt:a3:a0 0xa4:rs:a3:a0 0xa4:rt:a3:a0 0xb0:rs:a3:a0 0xc8:rt:a2:a1 0xcc:rs:a3:a1 0xcc:rt:a2:a0 0xd0:rt:v1:t6 0xdc:rt:a2:a1 0x110:rs:a2:a1 0x118:rs:a1:v1 0x154:rs:v1:t6 0x154:rt:a0:t6 0x158:rt:t7:t8 0x15c:rd:t6:t7 0x15c:rt:a0:t6 0x160:rd:t6:t7 0x160:rs:t6:t7 0x160:rt:a0:t6 0x164:rd:t6:t7 0x164:rt:t6:t7 0x168:rd:t7:t8 0x168:rs:t7:t8 0x168:rt:t6:t7 0x16c:rs:t7:t8 0x16c:rt:t7:t8 0x170:rt:a3:a0 0x174:rs:t7:t8 0x174:rt:t8:t9 0x178:rs:t8:t9 0x17c:rt:t1:t2 0x180:rt:a3:a0 0x184:rs:a3:a0 0x184:rt:a3:a0 0x18c:rs:a3:a0 0x194:rs:a3:a0 0x19c:rt:t9:t0 0x1a0:rs:t9:t0 0x1a0:rt:t9:t0 0x1a8:rs:t9:t0 0x1b0:rs:a1:v1 0x1c0:rs:a0:t6 0x1c4:rt:t0:t1 0x1d4:rt:t0:t1 0x1f8:rs:t1:t2 0x1f8:rt:t1:t2 0x1fc:rs:a3:a0 0x1fc:rt:a3:a0 0x200:rs:t1:t2 0x204:rt:t2:t3 0x208:rs:t2:t3 0x208:rt:t2:t3 0x20c:rs:a3:a0 0x210:rs:t2:t3 0x21c:rs:a3:a0 0x224:rt:t3:t4 0x228:rs:t3:t4 0x228:rt:t3:t4 0x22c:rt:t4:t5 0x230:rs:t3:t4 0x234:rt:t5:t7 0x238:rs:t4:t5 0x238:rt:t4:t5 0x23c:rs:a3:a0 0x240:rs:a2:a1 0x240:rt:t4:t5 0x24c:rs:a3:a0 0x260:rs:a1:v1 0x27c:rt:t5:t7 0x28c:rt:a1:t0 0x290:rs:a1:t0 0x290:rt:a1:t0 0x294:rt:a0:v1 0x298:rs:a1:t0 0x298:rt:a0:v1 0x29c:rt:t9:t1 0x2a0:rs:t9:t1 0x2a0:rt:t9:t1 0x2ac:rt:t6:t8 0x2b0:rd:t7:t9 0x2b4:rd:t7:t9 0x2b4:rs:t7:t9 0x2bc:rt:t6:t8 0x2c4:rs:t7:t9 0x2c8:rd:t8:t6 0x2cc:rd:t0:t2 0x2cc:rs:t8:t6 0x2cc:rt:t9:t1 0x2d0:rd:t1:t3 0x2d0:rs:t0:t2 0x2d0:rt:t7:t9 0x2d4:rs:a1:t0 0x2d4:rt:a0:v1 0x2d8:rs:t1:t3 0x2d8:rt:t2:t4 0x2e0:rt:t2:t4 0x2e8:rd:t3:t5 0x2ec:rd:t4:t7 0x2ec:rs:t3:t5 0x2ec:rt:t9:t1 0x2f0:rd:t5:t8 0x2f0:rs:t4:t7 0x2f0:rt:t7:t9 0x2f4:rs:a1:t0 0x2f4:rt:a0:v1 0x2f8:rs:t5:t8 0x2f8:rt:t6:t9 0x2fc:rt:t3:t7 0x300:rs:t3:t7 0x300:rt:t3:t7 0x304:rt:t6:t9 0x30c:rd:t7:t6 0x310:rd:t8:t2 0x310:rs:t7:t6 0x310:rt:t9:t1 0x314:rd:t0:t3 0x314:rs:t8:t2 0x318:rs:a1:t0 0x318:rt:a0:v1 0x31c:rs:t0:t3 0x31c:rt:t1:t4 0x324:rt:t1:t4 0x32c:rd:t2:t5 0x330:rd:t4:t8 0x330:rs:t2:t5 0x330:rt:t3:t7 0x334:rd:t5:t9 0x334:rs:t4:t8 0x338:rs:a1:t0 0x338:rt:a0:v1 0x33c:rs:t5:t9 0x34c:rd:t7:t1 0x350:rd:t9:t2 0x350:rs:t7:t1 0x350:rt:t3:t7 0x354:rd:t8:t3 0x354:rs:t9:t2 0x358:rs:a1:t0 0x358:rt:a0:v1 0x35c:rs:t8:t3 0x35c:rt:t0:t4 0x360:rt:a3:t1 0x364:rt:t0:t4 0x368:rs:a3:t1 0x368:rt:a3:t1 0x370:rd:t1:t5 0x374:rd:t2:t8 0x374:rs:t1:t5 0x374:rt:t3:t7 0x378:rd:t4:t9 0x378:rs:t2:t8 0x37c:rs:t4:t9 0x37c:rt:t5:t6 0x384:rt:t5:t6 0x388:rs:a3:t1 0x390:rs:a3:t1 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32  get_g_zyotai_s(s32);
extern s32  get_g_zyotai_h(s32);
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
extern u8  date_pattern;
extern volatile u8 now_month;
extern u8  now_day;
extern u8  girl0_flag2[];
extern u8 ev_flag03;
extern volatile u8 ev_flag1c;
extern volatile u8 ev_flag46;
extern volatile u8 now_girl;
extern s32 bustup_on;
extern s32 now_date_spot;

extern u8  now_date_spot_b;
extern u8  date_movie_effect_step_str0;
extern u8  date_movie_effect_step_str1;
extern u8  date_movie_effect_step_str2;
extern u8  date_movie_effect_step_str3;
extern u8  date_movie_effect_step_str4;
extern u8  date_serifu_stride;
extern u8 *date_impkao_tbl_19_imp;
extern u8 *date_impkao_tbl_19_kao;
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

void date_movie_effect_step(void)
{
    u8  r1;
    s32 r2;
    s32 e;
    u8  g;
    u8  gv;
    u8 *p1;
    u8 *p2;
    s32 c3;

    ss_serifu = date_serifu_tbl[21];
    ss_serifu_kind = date_serifu_kind_tbl[21];
    sss_sound = date_sound_tbl[21];

    bustup_on = 0;
    strcpy(&ba_string, &date_movie_effect_step_str0);
    strcpy(&ba_string2, &date_movie_effect_step_str1);
    date_serifu_stride = 3;
    r1 = get_g_zyotai_s(now_girl);
    r2 = get_g_zyotai_h(now_girl);

    e = now_month;
    if ((e == 3 && now_day >= 15U) || e == 4U) {
        if ((ev_flag03 == 0 && now_girl == 0) ||
            (ev_flag1c == 0 && now_girl == 4U && r1 == 128 && r2 == 0)) {
            date_pattern = 0;
        } else {
            date_pattern = 2;
            strcpy(&ba_string2, &date_movie_effect_step_str2);
        }
    } else {
        g = now_girl;
        if ((girl0_flag2[g * 56] & 1) == 0) {
            e = now_month;
            if (e >= 6U && e < 9U && ev_flag46 == 0 && r1 == 128 &&
                r2 == 0 && (g | 0) == 10) {
                date_pattern = 1;
                strcpy(&ba_string2, &date_movie_effect_step_str3);
            } else {
                date_pattern = 0;
            }
        } else {
            e = now_month;
            if ((ev_flag03 == 0 && now_girl == 0 && e >= 3U && e < 6U) ||
                (ev_flag1c == 0 && now_girl == 4U && e >= 3U && e < 6U &&
                 r2 == 0 && r1 == 128)) {
                date_pattern = 0;
            } else {
                date_pattern = 1;
                strcpy(&ba_string2, &date_movie_effect_step_str4);
            }
        }
    }

    gv = now_girl;
    p1 = date_impkao_tbl_19_imp;
    now_date_spot = 1;
    c3 = (date_pattern * 3) | 0;
    imp[0] = (p1 + gv * 9U)[c3 + 0];
    imp[1] = (p1 + (u32)gv * 9U)[c3 + 1];
    p2 = date_impkao_tbl_19_kao;
    imp[2] = (p1 + (gv | 0U) * 9U)[c3 + 2];
    kao[0] = (p2 + (gv ^ 0U) * 9U)[c3 + 0];
    kao[1] = (p2 + (gv * 1U) * 9U)[c3 + 1];
    kao[2] = (p2 + (gv >> 0) * 9U)[c3 + 2];

    e = now_month;
    if (e >= 3U && e < 9U) {
        dec_bg_cache_step(17131);
    } else {
        dec_bg_cache_step(17140);
    }
    event_log_add(1, now_date_spot_b);
    sub_sub_step_up();
}
