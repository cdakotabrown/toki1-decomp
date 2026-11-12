/* @SymbolStart: 0x80135894 */
/* @SymbolSize: 560 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7C */
/* @PostProc: swap_instructions --offset1 0xF0 --offset2 0xF8 */
/* @PostProc: swap_instructions --offset1 0xF4 --offset2 0xFC */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x10C */
/* @PostProc: swap_instructions --offset1 0x10C --offset2 0x110 */
/* @PostProc: swap_instructions --offset1 0x110 --offset2 0x114 */
/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x118 */
/* @PostProc: swap_instructions --offset1 0x104 --offset2 0x108 */
/* @PostProc: swap_instructions --offset1 0x100 --offset2 0x104 */
/* @PostProc: patch_insn_reg_field 0x5c:rt:t9:t0 0x60:rs:t9:t0 0x60:rt:t9:t0 0x64:rt:v0:t2 0x68:rd:t0:t1 0x68:rt:t9:t0 0x6c:rd:t0:t1 0x6c:rs:t0:t1 0x6c:rt:t9:t0 0x70:rd:t0:t1 0x70:rt:t0:t1 0x74:rt:t1:t9 0x7c:rd:v0:t2 0x7c:rs:v0:t2 0x7c:rt:t0:t1 0x80:rs:v0:t2 0x80:rt:v0:t2 0x84:rt:t1:t9 0x8c:rs:v0:t2 0x9c:rt:t3:t4 0xa8:rd:t2:t3 0xac:rs:t2:t3 0xdc:rt:t3:t4 0xe0:rt:a2:t7 0xe4:rs:a2:t7 0xe4:rt:a2:t7 0xe8:rt:a1:v1 0xec:rs:a2:t7 0xec:rt:a1:v1 0xf0:rt:t7:t9 0xf4:rs:t7:t9 0xf4:rt:t7:t9 0x100:rt:t4:t5 0x104:rd:t5:t6 0x108:rd:t5:t6 0x108:rs:t5:t6 0x110:rt:t4:t5 0x118:rs:t5:t6 0x11c:rd:t6:t8 0x120:rd:t8:t0 0x120:rs:t6:t8 0x120:rt:t7:t9 0x124:rd:t9:t1 0x124:rs:t8:t0 0x124:rt:t5:t6 0x128:rs:a2:t7 0x128:rt:a1:v1 0x12c:rs:t9:t1 0x12c:rt:t0:t2 0x134:rt:t0:t2 0x13c:rd:t1:t3 0x140:rd:t2:t4 0x140:rs:t1:t3 0x140:rt:t7:t9 0x144:rd:t3:t5 0x144:rs:t2:t4 0x144:rt:t5:t6 0x148:rs:a2:t7 0x148:rt:a1:v1 0x14c:rs:t3:t5 0x14c:rt:t4:t6 0x150:rt:t1:t4 0x154:rs:t1:t4 0x154:rt:t1:t4 0x158:rt:t4:t6 0x160:rd:t5:t8 0x164:rd:t6:t0 0x164:rs:t5:t8 0x164:rt:t7:t9 0x168:rd:t8:t1 0x168:rs:t6:t0 0x16c:rs:a2:t7 0x16c:rt:a1:v1 0x170:rs:t8:t1 0x170:rt:t9:t2 0x178:rt:t9:t2 0x180:rd:t0:t3 0x184:rd:t2:t5 0x184:rs:t0:t3 0x184:rt:t1:t4 0x188:rd:t3:t6 0x188:rs:t2:t5 0x18c:rs:a2:t7 0x18c:rt:a1:v1 0x190:rs:t3:t6 0x190:rt:t4:t8 0x198:rt:t4:t8 0x1a0:rd:t5:t9 0x1a4:rd:t7:t0 0x1a4:rs:t5:t9 0x1a4:rt:t1:t4 0x1a8:rd:t6:t1 0x1a8:rs:t7:t0 0x1ac:rs:a2:t7 0x1ac:rt:a1:v1 0x1b0:rs:t6:t1 0x1b0:rt:t8:t2 0x1b8:rt:t8:t2 0x1c0:rd:t9:t3 0x1c4:rd:t0:t5 0x1c4:rs:t9:t3 0x1c4:rt:t1:t4 0x1c8:rd:t2:t6 0x1c8:rs:t0:t5 0x1cc:rs:t2:t6 0x1cc:rt:t3:t8 0x1d4:rt:t3:t8 0x1e8:rt:t4:t9 0x1ec:rs:t4:t9 0x1ec:rt:t4:t9 0x1f0:rt:v0:t1 0x1f4:rd:t5:t0 0x1f4:rt:t4:t9 0x1f8:rd:t5:t0 0x1f8:rs:t5:t0 0x1f8:rt:t4:t9 0x1fc:rd:t5:t0 0x1fc:rt:t5:t0 0x200:rd:v0:t1 0x200:rs:v0:t1 0x200:rt:t5:t0 0x204:rs:v0:t1 0x204:rt:v0:t1 0x20c:rs:v0:t1 0x214:rt:t6:t7 0x218:rd:t7:t2 0x21c:rs:t7:t2 0x224:rt:t6:t7 */
/* Function: date_talk_step */
/* Overlay: DATE */
/* Address: 0x80135894 (cross-prologue head) / 0x801358B8 (body) */
/* Size: 560 bytes (140 instructions; ends at the jr ra at 0x80135ABC. */
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
extern u8 date_talk_step_str0[];
extern u8 date_talk_step_str1[];
extern u8 date_talk_step_str2[];

extern volatile u8 now_girl;
extern u8 girl0_spot19_cnt[];
extern u8 date_serifu_stride;
extern u8 date_pattern;
extern s32 now_date_spot;

extern u8 now_date_spot_b;

extern u8 *date_impkao_tbl_6_imp;
extern u8 *date_impkao_tbl_6_kao;

extern s16 kao[];
extern s16 imp[];
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

s32 date_talk_step(void)
{
    u8 rec;
    s32 mode;
    u8 *p1;
    u8 *p2;
    s32 c3;
    u8 gv;

    ss_serifu = date_serifu_tbl[6];
    ss_serifu_kind = date_serifu_kind_tbl[6];
    sss_sound = date_sound_tbl[6];

    strcpy((s32) ba_string, (s32) date_talk_step_str0);
    rec = girl0_spot19_cnt[now_girl * 56];
    date_serifu_stride = 3;
    if ((rec % 3U) == 0U) {
        date_pattern = 0;
        strcpy((s32) ba_string2, (s32) date_talk_step_str1);
    } else {
        date_pattern = 1;
        strcpy((s32) ba_string2, (s32) date_talk_step_str2);
    }

    gv = now_girl;
    p1 = date_impkao_tbl_6_imp;
    c3 = date_pattern * 3;
    now_date_spot = 19;
    imp[0] = (p1 + gv * 6U)[c3 + 0];
    imp[1] = (p1 + (u32)gv * 6U)[c3 + 1];
    p2 = date_impkao_tbl_6_kao;
    imp[2] = (p1 + (gv | 0U) * 6U)[c3 + 2];
    kao[0] = (p2 + (gv ^ 0U) * 6U)[c3 + 0];
    kao[1] = (p2 + (gv * 1U) * 6U)[c3 + 1];
    kao[2] = (p2 + (gv >> 0) * 6U)[c3 + 2];

    dec_bg_cache_step(17939);
    event_log_add(1, now_date_spot_b);

    rec = girl0_spot19_cnt[now_girl * 56];
    if ((rec % 3U) == 2U) {
        date_pattern = 2;
    }
    return sub_sub_step_up();
}
