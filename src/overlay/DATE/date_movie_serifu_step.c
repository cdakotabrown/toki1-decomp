/* @SymbolStart: 0x801356B0 */
/* @SymbolSize: 484 */
/* @PostProc: swap_instructions --offset1 0xE4 --offset2 0xEC */
/* @PostProc: swap_instructions --offset1 0xE8 --offset2 0xF0 */
/* @PostProc: swap_instructions --offset1 0xFC --offset2 0x100 */
/* @PostProc: swap_instructions --offset1 0x100 --offset2 0x104 */
/* @PostProc: swap_instructions --offset1 0x104 --offset2 0x108 */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x10C */
/* @PostProc: swap_instructions --offset1 0xF8 --offset2 0xFC */
/* @PostProc: swap_instructions --offset1 0xF4 --offset2 0xF8 */
/* @PostProc: patch_insn_reg_field 0x5c:rt:t9:t0 0x60:rs:t9:t0 0x60:rt:t9:t0 0x64:rt:v0:t2 0x68:rd:t0:t1 0x68:rt:t9:t0 0x6c:rd:t0:t1 0x6c:rs:t0:t1 0x6c:rt:t9:t0 0x70:rd:t0:t1 0x70:rt:t0:t1 0x74:rd:v0:t2 0x74:rs:v0:t2 0x74:rt:t0:t1 0x78:rs:v0:t2 0x78:rt:v0:t2 0x7c:rt:t1:t9 0x84:rs:v0:t2 0x84:rt:t2:t3 0x88:rs:t2:t3 0x8c:rt:t1:t9 0xb4:rt:t3:t4 0xc4:rt:t3:t4 0xd4:rt:a2:t7 0xd8:rs:a2:t7 0xd8:rt:a2:t7 0xdc:rt:a1:v1 0xe0:rs:a2:t7 0xe0:rt:a1:v1 0xe4:rt:t7:t9 0xe8:rs:t7:t9 0xe8:rt:t7:t9 0xf4:rt:t4:t5 0xf8:rd:t5:t6 0xfc:rd:t5:t6 0xfc:rs:t5:t6 0x104:rt:t4:t5 0x10c:rs:t5:t6 0x110:rd:t6:t8 0x114:rd:t8:t0 0x114:rs:t6:t8 0x114:rt:t7:t9 0x118:rd:t9:t1 0x118:rs:t8:t0 0x118:rt:t5:t6 0x11c:rs:a2:t7 0x11c:rt:a1:v1 0x120:rs:t9:t1 0x120:rt:t0:t2 0x128:rt:t0:t2 0x130:rd:t1:t3 0x134:rd:t2:t4 0x134:rs:t1:t3 0x134:rt:t7:t9 0x138:rd:t3:t5 0x138:rs:t2:t4 0x138:rt:t5:t6 0x13c:rs:a2:t7 0x13c:rt:a1:v1 0x140:rs:t3:t5 0x140:rt:t4:t6 0x144:rt:t1:t4 0x148:rs:t1:t4 0x148:rt:t1:t4 0x14c:rt:t4:t6 0x154:rd:t5:t8 0x158:rd:t6:t0 0x158:rs:t5:t8 0x158:rt:t7:t9 0x15c:rd:t8:t1 0x15c:rs:t6:t0 0x160:rs:a2:t7 0x160:rt:a1:v1 0x164:rs:t8:t1 0x164:rt:t9:t2 0x16c:rt:t9:t2 0x174:rd:t0:t3 0x178:rd:t2:t5 0x178:rs:t0:t3 0x178:rt:t1:t4 0x17c:rd:t3:t6 0x17c:rs:t2:t5 0x180:rs:a2:t7 0x180:rt:a1:v1 0x184:rs:t3:t6 0x184:rt:t4:t8 0x18c:rt:t4:t8 0x194:rd:t5:t9 0x198:rd:t7:t0 0x198:rs:t5:t9 0x198:rt:t1:t4 0x19c:rd:t6:t1 0x19c:rs:t7:t0 0x1a0:rs:a2:t7 0x1a0:rt:a1:v1 0x1a4:rs:t6:t1 0x1a4:rt:t8:t2 0x1ac:rt:t8:t2 0x1b4:rd:t9:t3 0x1b8:rd:t0:t5 0x1b8:rs:t9:t3 0x1b8:rt:t1:t4 0x1bc:rd:t2:t6 0x1bc:rs:t0:t5 0x1c0:rs:t2:t6 0x1c0:rt:t3:t8 0x1c8:rt:t3:t8 */
/* Function: date_movie_serifu_step */
/* Overlay: DATE */
/* Address: 0x801356B0 (cross-prologue head) / 0x801356D4 (body) */
/* Size: 484 bytes (121 instructions) */
#include "types.h"

extern void strcpy(s32, s32);
extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern s32 sub_sub_step_up(void);

extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;

extern u8 ba_string[];
extern u8 ba_string2[];
extern u8 date_movie_serifu_step_str0[];
extern u8 date_movie_serifu_step_str1[];
extern u8 date_movie_serifu_step_str2[];

extern volatile u8 now_girl;
extern u8 girl0_spot18_cnt[];
extern u8 date_serifu_stride;
extern u8 date_pattern;
extern s32 now_date_spot;

extern u8 now_date_spot_b;

extern u8 *date_impkao_tbl_1_imp;
extern u8 *date_impkao_tbl_1_kao;

extern s16 kao[];
extern s16 imp[];
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

s32 date_movie_serifu_step(void)
{
    u8 rec;
    u8 *p1;
    u8 *p2;
    s32 c3;
    u8 gv;

    ss_serifu = date_serifu_tbl[1];
    ss_serifu_kind = date_serifu_kind_tbl[1];
    sss_sound = date_sound_tbl[1];

    strcpy((s32) ba_string, (s32) date_movie_serifu_step_str0);
    rec = girl0_spot18_cnt[now_girl * 56];
    date_serifu_stride = 3;
    if ((rec & 1) == 0) {
        date_pattern = 0;
        strcpy((s32) ba_string2, (s32) date_movie_serifu_step_str1);
    } else {
        date_pattern = 1;
        strcpy((s32) ba_string2, (s32) date_movie_serifu_step_str2);
    }

    gv = now_girl;
    p1 = date_impkao_tbl_1_imp;
    now_date_spot = 18;
    c3 = date_pattern * 3;
    imp[0] = (p1 + gv * 6U)[c3 + 0];
    imp[1] = (p1 + (u32)gv * 6U)[c3 + 1];
    p2 = date_impkao_tbl_1_kao;
    imp[2] = (p1 + (gv | 0U) * 6U)[c3 + 2];
    kao[0] = (p2 + (gv ^ 0U) * 6U)[c3 + 0];
    kao[1] = (p2 + (gv * 1U) * 6U)[c3 + 1];
    kao[2] = (p2 + (gv >> 0) * 6U)[c3 + 2];

    dec_bg_cache_step(17969);
    event_log_add(1, now_date_spot_b);
    return sub_sub_step_up();
}
