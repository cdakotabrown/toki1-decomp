/* @SymbolStart: 0x80136900 */
/* @SymbolSize: 516 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @PostProc: swap_instructions --offset1 0xFC --offset2 0x108 */
/* @PostProc: swap_instructions --offset1 0x104 --offset2 0x10C */
/* @PostProc: swap_instructions --offset1 0x110 --offset2 0x11C */
/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x11C */
/* @PostProc: swap_instructions --offset1 0x118 --offset2 0x11C */
/* @PostProc: swap_instructions --offset1 0x11C --offset2 0x120 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x124 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x128 */
/* @PostProc: patch_insn_reg_field 0x4c:rt:a3:v1 0x5c:rs:a3:v1 0x9c:rt:v0:t3 0xac:rd:v0:t3 0xac:rs:v0:t3 0xb0:rs:v0:t3 0xb0:rt:v0:t3 0xb4:rt:a3:v1 0xb8:rs:v0:t3 0xb8:rt:t3:t4 0xbc:rs:t3:t4 0xc0:rt:a1:t0 0xd0:rt:t4:t5 0xd8:rt:t5:t6 0xdc:rs:t5:t6 0xdc:rt:t5:t6 0xe0:rt:t4:t5 0xe8:rs:t5:t6 0xec:rt:t6:t7 0xf4:rt:t6:t7 0xf8:rs:a1:t0 0xf8:rt:a1:t0 0xfc:rt:t0:t2 0x100:rs:a1:t0 0x100:rt:a3:v1 0x104:rs:t0:t2 0x104:rt:t0:t2 0x110:rt:t7:t8 0x114:rd:t8:t9 0x118:rd:t8:t9 0x118:rs:t8:t9 0x120:rt:t7:t8 0x128:rs:t8:t9 0x130:rd:t9:t1 0x134:rd:t1:t3 0x134:rs:t9:t1 0x134:rt:t0:t2 0x138:rd:t2:t4 0x138:rs:t1:t3 0x138:rt:t8:t9 0x13c:rs:a1:t0 0x13c:rt:a3:v1 0x140:rs:t2:t4 0x140:rt:t3:t5 0x148:rt:t3:t5 0x150:rd:t4:t6 0x154:rd:t5:t7 0x154:rs:t4:t6 0x154:rt:t0:t2 0x158:rd:t6:t8 0x158:rs:t5:t7 0x158:rt:t8:t9 0x15c:rs:a1:t0 0x15c:rt:a3:v1 0x160:rs:t6:t8 0x160:rt:t7:t9 0x164:rt:t4:t7 0x168:rs:t4:t7 0x168:rt:t4:t7 0x16c:rt:t7:t9 0x174:rd:t8:t1 0x178:rd:t9:t3 0x178:rs:t8:t1 0x178:rt:t0:t2 0x17c:rd:t1:t4 0x17c:rs:t9:t3 0x180:rs:a1:t0 0x180:rt:a3:v1 0x184:rs:t1:t4 0x184:rt:t2:t5 0x18c:rt:t2:t5 0x194:rd:t3:t6 0x198:rd:t5:t8 0x198:rs:t3:t6 0x198:rt:t4:t7 0x19c:rd:t6:t9 0x19c:rs:t5:t8 0x1a0:rs:a1:t0 0x1a0:rt:a3:v1 0x1a4:rs:t6:t9 0x1a4:rt:t7:t1 0x1ac:rt:t7:t1 0x1b4:rd:t8:t2 0x1b8:rd:t0:t3 0x1b8:rs:t8:t2 0x1b8:rt:t4:t7 0x1bc:rd:t9:t4 0x1bc:rs:t0:t3 0x1c0:rs:a1:t0 0x1c0:rt:a3:v1 0x1c4:rs:t9:t4 0x1c4:rt:t1:t5 0x1cc:rt:t1:t5 0x1d4:rd:t2:t6 0x1d8:rd:t3:t8 0x1d8:rs:t2:t6 0x1d8:rt:t4:t7 0x1dc:rd:t5:t9 0x1dc:rs:t3:t8 0x1e0:rs:t5:t9 0x1e0:rt:t6:t1 0x1e8:rt:t6:t1 */
/* Function: date_basyo17_init */
/* Overlay: DATE */
/* Address: 0x80136900 (cross-prologue head) / 0x80136938 (body) */
/* Size: 516 bytes (129 instructions: 14 head + 115 body) */
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
extern u8 date_basyo17_init_str0[];
extern u8 date_basyo17_init_str1[];

extern volatile u8 now_girl;
extern u8 girl0_spot30_cnt[];
extern u8 date_serifu_stride;
extern u8 date_pattern;
extern s32 now_date_spot;

extern u8 *date_impkao_tbl_10_imp;
extern u8 *date_impkao_tbl_10_kao;

extern s16 now_serifu;
extern s16 kao[];
extern s16 imp[];
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

s32 date_basyo17_init(void)
{
    u8 rec;
    u8 *p1;
    u8 *p2;
    s32 c3;
    u8 gv;

    ss_serifu = date_serifu_tbl[12];
    ss_serifu_kind = date_serifu_kind_tbl[12];
    sss_sound = date_sound_tbl[12];

    date_serifu_stride = 3;
    if (*(u8 *)&now_girl == 6U) {
        date_serifu_stride = 0;
    }
    strcpy((s32) ba_string, (s32) date_basyo17_init_str0);
    strcpy((s32) ba_string2, (s32) date_basyo17_init_str1);

    rec = girl0_spot30_cnt[now_girl * 56];
    if ((rec & 1) == 0) {
        date_pattern = 0;
    } else {
        date_pattern = 1;
        if (now_girl == 5) {
            now_serifu = 2;
        }
    }

    gv = now_girl;
    p1 = date_impkao_tbl_10_imp;
    c3 = date_pattern * 3;
    now_date_spot = 30;
    imp[0] = (p1 + gv * 6U)[c3 + 0];
    imp[1] = (p1 + (u32)gv * 6U)[c3 + 1];
    p2 = date_impkao_tbl_10_kao;
    imp[2] = (p1 + (gv | 0U) * 6U)[c3 + 2];
    kao[0] = (p2 + (gv ^ 0U) * 6U)[c3 + 0];
    kao[1] = (p2 + (gv * 1U) * 6U)[c3 + 1];
    kao[2] = (p2 + (gv >> 0) * 6U)[c3 + 2];

    dec_bg_cache_step(17827);
    event_log_add(1, (s32)*(u8 *)&now_date_spot);
    return sub_sub_step_up();
}
