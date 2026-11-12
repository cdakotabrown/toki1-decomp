/* @SymbolStart: 0x80135CA4 */
/* @SymbolSize: 616 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=2 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x170 --offset2 0x178 */
/* @PostProc: swap_instructions --offset1 0x174 --offset2 0x17c */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x18c */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x18c */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x18c */
/* @PostProc: patch_insn_reg_field 0x8c:rt:t6:t7 0xcc:rt:v0:t2 0xdc:rd:v0:t2 0xdc:rs:v0:t2 0xe0:rs:v0:t2 0xe0:rt:v0:t2 0xe4:rt:t4:t5 0xe8:rs:v0:t2 0xe8:rt:t2:t3 0xec:rs:t2:t3 0xf0:rt:t5:t6 0xf4:rt:t3:t4 0x100:rt:t3:t4 0x108:rt:t4:t5 0x114:rt:t5:t6 0x118:rs:t6:t7 0x118:rt:t6:t7 0x11c:rt:v0:t9 0x120:rd:t7:t8 0x120:rt:t6:t7 0x124:rd:t7:t8 0x124:rs:t7:t8 0x124:rt:t6:t7 0x128:rd:t7:t8 0x128:rt:t7:t8 0x12c:rd:v0:t9 0x12c:rs:v0:t9 0x12c:rt:t7:t8 0x130:rs:v0:t9 0x130:rt:v0:t9 0x134:rt:t9:t1 0x138:rs:v0:t9 0x138:rt:t8:t0 0x13c:rs:t8:t0 0x140:rt:t0:t2 0x154:rt:t9:t1 0x15c:rt:t0:t2 0x160:rt:a2:t4 0x164:rs:a2:t4 0x164:rt:a2:t4 0x168:rt:a1:v1 0x16c:rs:a2:t4 0x16c:rt:a1:v1 0x170:rt:t3:t6 0x174:rs:t3:t6 0x174:rt:t3:t6 0x184:rd:t1:t3 0x188:rd:t1:t3 0x188:rs:t1:t3 0x18c:rs:t1:t3 0x194:rd:t2:t5 0x198:rd:t4:t7 0x198:rs:t2:t5 0x198:rt:t3:t6 0x19c:rd:t5:t8 0x19c:rs:t4:t7 0x19c:rt:t1:t3 0x1a0:rs:a2:t4 0x1a0:rt:a1:v1 0x1a4:rs:t5:t8 0x1a4:rt:t6:t9 0x1ac:rt:t6:t9 0x1b4:rd:t7:t0 0x1b8:rd:t8:t1 0x1b8:rs:t7:t0 0x1b8:rt:t3:t6 0x1bc:rd:t9:t2 0x1bc:rs:t8:t1 0x1bc:rt:t1:t3 0x1c0:rs:a2:t4 0x1c0:rt:a1:v1 0x1c4:rs:t9:t2 0x1c4:rt:t0:t3 0x1c8:rt:t7:t1 0x1cc:rs:t7:t1 0x1cc:rt:t7:t1 0x1d0:rt:t0:t3 0x1d8:rd:t1:t5 0x1dc:rd:t2:t7 0x1dc:rs:t1:t5 0x1dc:rt:t3:t6 0x1e0:rd:t4:t8 0x1e0:rs:t2:t7 0x1e4:rs:a2:t4 0x1e4:rt:a1:v1 0x1e8:rs:t4:t8 0x1e8:rt:t5:t9 0x1f0:rt:t5:t9 0x1f8:rd:t6:t0 0x1fc:rd:t8:t2 0x1fc:rs:t6:t0 0x1fc:rt:t7:t1 0x200:rd:t9:t3 0x200:rs:t8:t2 0x204:rs:a2:t4 0x204:rt:a1:v1 0x208:rs:t9:t3 0x208:rt:t0:t5 0x210:rt:t0:t5 0x218:rd:t1:t6 0x21c:rd:t3:t7 0x21c:rs:t1:t6 0x21c:rt:t7:t1 0x220:rd:t2:t8 0x220:rs:t3:t7 0x224:rs:a2:t4 0x224:rt:a1:v1 0x228:rs:t2:t8 0x228:rt:t4:t9 0x230:rt:t4:t9 0x238:rd:t5:t0 0x23c:rd:t6:t2 0x23c:rs:t5:t0 0x23c:rt:t7:t1 0x240:rd:t8:t3 0x240:rs:t6:t2 0x244:rs:t8:t3 0x244:rt:t9:t5 0x24c:rt:t9:t5 */
/* Function: date_aquarium_step */
/* Overlay: DATE */
/* Address: 0x80135CA4 (cross-prologue head) / 0x80135CC8 (body) */
/* Size: 616 bytes (154 instructions; ends at the jr ra at 0x80135F04. */
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
extern u8 date_aquarium_step_str0[];
extern u8 date_aquarium_step_str1[];

extern u8 now_month;
extern volatile u8 now_girl;
extern u8 girl0_spot27_cnt[];
extern u8 girl0_spot28_cnt[];
extern u8 date_serifu_stride;
extern u8 date_aquarium_flag;
extern u8 date_pattern;
extern s32 now_date_spot;

extern u8 now_date_spot_b;

extern u8 *date_impkao_tbl_15_imp;
extern u8 *date_impkao_tbl_15_kao;

extern s16 kao[];
extern s16 imp[];
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

s32 date_aquarium_step(void)
{
    s32 season;
    u8 rec;
    u8 *p1;
    u8 *p2;
    s32 c3;
    u8 gv;
    s32 v0;

    ss_serifu = date_serifu_tbl[17];
    ss_serifu_kind = date_serifu_kind_tbl[17];
    sss_sound = date_sound_tbl[17];

    strcpy((s32) ba_string, (s32) date_aquarium_step_str0);
    strcpy((s32) ba_string2, (s32) date_aquarium_step_str1);

    v0 = now_month;
    date_serifu_stride = 3;
    if (v0 == 1 || v0 == 3 || v0 == 6 || v0 == 11) {
        season = 1;
    } else {
        season = 0;
    }
    date_aquarium_flag = (u8) season;
    if (season != 0) {
        rec = girl0_spot28_cnt[now_girl * 56];
        if ((rec & 1) == 0) {
            date_pattern = 2;
        } else {
            date_pattern = 3;
        }
        now_date_spot = 28;
    } else {
        rec = girl0_spot27_cnt[now_girl * 56];
        if ((rec & 1) == 0) {
            date_pattern = 0;
        } else {
            date_pattern = 1;
        }
        now_date_spot = 27;
    }

    gv = now_girl;
    p1 = date_impkao_tbl_15_imp;
    c3 = (date_pattern * 3) | 0;
    imp[0] = (p1 + gv * 12U)[c3 + 0];
    imp[1] = (p1 + (u32)gv * 12U)[c3 + 1];
    p2 = date_impkao_tbl_15_kao;
    imp[2] = (p1 + (gv | 0U) * 12U)[c3 + 2];
    kao[0] = (p2 + (gv ^ 0U) * 12U)[c3 + 0];
    kao[1] = (p2 + (gv * 1U) * 12U)[c3 + 1];
    kao[2] = (p2 + (gv >> 0) * 12U)[c3 + 2];

    dec_bg_cache_step(17912);
    event_log_add(1, now_date_spot_b);
    return sub_sub_step_up();
}
