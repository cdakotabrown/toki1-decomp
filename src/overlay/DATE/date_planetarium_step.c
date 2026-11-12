/* @SymbolStart: 0x80136734 */
/* @SymbolSize: 460 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @PostProc: swap_instructions --offset1 0xCC --offset2 0xD4 */
/* @PostProc: swap_instructions --offset1 0xD0 --offset2 0xD8 */
/* @PostProc: swap_instructions --offset1 0xDC --offset2 0xE8 */
/* @PostProc: swap_instructions --offset1 0xE0 --offset2 0xE8 */
/* @PostProc: swap_instructions --offset1 0xE4 --offset2 0xE8 */
/* @PostProc: swap_instructions --offset1 0xE8 --offset2 0xEC */
/* @PostProc: swap_instructions --offset1 0xEC --offset2 0xF0 */
/* @PostProc: swap_instructions --offset1 0xF0 --offset2 0xF4 */
/* @PostProc: patch_insn_reg_field 0x84:rt:v0:t2 0x94:rd:v0:t2 0x94:rs:v0:t2 0x98:rs:v0:t2 0x98:rt:v0:t2 0x9c:rt:t3:t4 0xa0:rs:v0:t2 0xa0:rt:t2:t3 0xa4:rs:t2:t3 0xa8:rt:a2:t7 0xbc:rt:t3:t4 0xc0:rs:a2:t7 0xc0:rt:a2:t7 0xc4:rt:a1:v1 0xc8:rs:a2:t7 0xc8:rt:a1:v1 0xcc:rt:t7:t9 0xd0:rs:t7:t9 0xd0:rt:t7:t9 0xdc:rt:t4:t5 0xe0:rd:t5:t6 0xe4:rd:t5:t6 0xe4:rs:t5:t6 0xec:rt:t4:t5 0xf4:rs:t5:t6 0xf8:rd:t6:t8 0xfc:rd:t8:t0 0xfc:rs:t6:t8 0xfc:rt:t7:t9 0x100:rd:t9:t1 0x100:rs:t8:t0 0x100:rt:t5:t6 0x104:rs:a2:t7 0x104:rt:a1:v1 0x108:rs:t9:t1 0x108:rt:t0:t2 0x110:rt:t0:t2 0x118:rd:t1:t3 0x11c:rd:t2:t4 0x11c:rs:t1:t3 0x11c:rt:t7:t9 0x120:rd:t3:t5 0x120:rs:t2:t4 0x120:rt:t5:t6 0x124:rs:a2:t7 0x124:rt:a1:v1 0x128:rs:t3:t5 0x128:rt:t4:t6 0x12c:rt:t1:t4 0x130:rs:t1:t4 0x130:rt:t1:t4 0x134:rt:t4:t6 0x13c:rd:t5:t8 0x140:rd:t6:t0 0x140:rs:t5:t8 0x140:rt:t7:t9 0x144:rd:t8:t1 0x144:rs:t6:t0 0x148:rs:a2:t7 0x148:rt:a1:v1 0x14c:rs:t8:t1 0x14c:rt:t9:t2 0x154:rt:t9:t2 0x15c:rd:t0:t3 0x160:rd:t2:t5 0x160:rs:t0:t3 0x160:rt:t1:t4 0x164:rd:t3:t6 0x164:rs:t2:t5 0x168:rs:a2:t7 0x168:rt:a1:v1 0x16c:rs:t3:t6 0x16c:rt:t4:t8 0x174:rt:t4:t8 0x17c:rd:t5:t9 0x180:rd:t7:t0 0x180:rs:t5:t9 0x180:rt:t1:t4 0x184:rd:t6:t1 0x184:rs:t7:t0 0x188:rs:a2:t7 0x188:rt:a1:v1 0x18c:rs:t6:t1 0x18c:rt:t8:t2 0x194:rt:t8:t2 0x19c:rd:t9:t3 0x1a0:rd:t0:t5 0x1a0:rs:t9:t3 0x1a0:rt:t1:t4 0x1a4:rd:t2:t6 0x1a4:rs:t0:t5 0x1a8:rs:t2:t6 0x1a8:rt:t3:t8 0x1b0:rt:t3:t8 */
/* Function: date_planetarium_step */
/* Overlay: DATE */
/* Address: 0x80136734 (cross-prologue head) / 0x80136760 (body) */
/* Size: 460 bytes (115 instructions: 11 head + 104 body) */
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
extern u8 date_planetarium_step_str0[];
extern u8 date_planetarium_step_str1[];

extern volatile u8 now_girl;
extern u8 girl0_spot29_cnt[];
extern u8 date_serifu_stride;
extern u8 date_pattern;
extern s32 now_date_spot;

extern u8 *date_impkao_tbl_8_imp;
extern u8 *date_impkao_tbl_8_kao;

extern s16 kao[];
extern s16 imp[];
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

s32 date_planetarium_step(void)
{
    u8 rec;
    u8 *p1;
    u8 *p2;
    s32 c3;
    u8 gv;

    ss_serifu = date_serifu_tbl[10];
    ss_serifu_kind = date_serifu_kind_tbl[10];
    sss_sound = date_sound_tbl[10];

    date_serifu_stride = 3;
    strcpy((s32) ba_string, (s32) date_planetarium_step_str0);
    strcpy((s32) ba_string2, (s32) date_planetarium_step_str1);

    rec = girl0_spot29_cnt[now_girl * 56];
    if ((rec & 1) == 0) {
        date_pattern = 0;
    } else {
        date_pattern = 1;
    }

    gv = now_girl;
    p1 = date_impkao_tbl_8_imp;
    c3 = date_pattern * 3;
    now_date_spot = 29;
    imp[0] = (p1 + gv * 6U)[c3 + 0];
    imp[1] = (p1 + (u32)gv * 6U)[c3 + 1];
    p2 = date_impkao_tbl_8_kao;
    imp[2] = (p1 + (gv | 0U) * 6U)[c3 + 2];
    kao[0] = (p2 + (gv ^ 0U) * 6U)[c3 + 0];
    kao[1] = (p2 + (gv * 1U) * 6U)[c3 + 1];
    kao[2] = (p2 + (gv >> 0) * 6U)[c3 + 2];

    dec_bg_cache_step(17855);
    event_log_add(1, (s32)*(u8 *)&now_date_spot);
    return sub_sub_step_up();
}
