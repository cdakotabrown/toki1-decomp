/* @SymbolStart: 0x8013944C */
/* @SymbolSize: 472 */
/* @PostProc: patch_insn_reg_field 0x94:rt:t4:t5 0x98:rs:t4:t5 0x98:rt:a1:t5 0x9c:rt:a0:v1 0xa0:rs:a1:t5 0xa0:rt:a0:v1 0xb8:rd:t5:t4 0xbc:rd:t5:t4 0xbc:rs:t5:t4 0xcc:rs:t5:t4 0xd8:rt:t5:t4 0xdc:rs:a1:t5 0xdc:rt:a0:v1 0xf8:rt:t5:t4 0xfc:rs:a1:t5 0xfc:rt:a0:v1 0x100:rt:t5:t4 0x104:rt:t1:t2 0x108:rs:t1:t2 0x108:rt:t1:t2 0x10c:rt:t5:t4 0x120:rs:a1:t5 0x120:rt:a0:v1 0x134:rd:t4:t1 0x138:rd:t2:t3 0x138:rs:t4:t1 0x138:rt:t1:t2 0x13c:rd:t3:t4 0x13c:rs:t2:t3 0x140:rs:a1:t5 0x140:rt:a0:v1 0x144:rs:t3:t4 0x144:rt:t5:t6 0x14c:rt:t5:t6 0x154:rd:t6:t7 0x158:rd:t7:t8 0x158:rs:t6:t7 0x158:rt:t1:t2 0x15c:rd:t8:t9 0x15c:rs:t7:t8 0x160:rs:a1:t5 0x160:rt:a0:v1 0x164:rs:t8:t9 0x164:rt:t9:t0 0x16c:rt:t9:t0 0x174:rd:t0:t1 0x178:rd:t4:t3 0x178:rs:t0:t1 0x178:rt:t1:t2 0x17c:rd:t2:t4 0x17c:rs:t4:t3 0x180:rs:t2:t4 0x180:rt:t3:t6 0x188:rt:t3:t6 0x18c:rt:t6:t8 0x190:rs:t6:t8 0x190:rt:t6:t8 0x194:rt:t5:t7 0x198:rs:t5:t7 0x198:rt:t5:t7 0x19c:rd:t7:t9 0x19c:rt:t6:t8 0x1a4:rt:t9:t5 0x1a8:rd:t8:t0 0x1a8:rs:t5:t7 0x1a8:rt:t7:t9 0x1ac:rs:t9:t5 0x1ac:rt:t9:t5 0x1b0:rt:t8:t0 0x1b8:rs:t9:t5 0x1bc:rs:t6:t8 0x1c4:rs:t8:t0 0x1c4:rt:t4:t3 0x1cc:rt:t4:t3 */
/* Address: 0x80139454, Size: 464 bytes (116 insns) */
#include "types.h"

extern s32 serifu_no;
extern u8  screen_w[];
extern u8  now_girl;
extern u8  date_pattern;
extern u8  ba_string2[];
extern u8  date_basyo9_scene_set_str0[];
extern u8  date_basyo9_scene_set_str1[];
extern s32 now_date_spot;
extern u8 *date_impkao_tbl_5_imp;
extern u8 *date_impkao_tbl_5_kao;
extern s16 imp[];
extern s16 kao[];
extern s16 now_serifu;

extern s32 sub_sub_step_up();
extern s32 strcpy(u8 *, u8 *);

void date_basyo9_scene_set(void)
{
    u8 *p1;
    u8 *p2;
    s32 c3;
    u8 gv;

    if (serifu_no != 0) {
        if (!(screen_w[now_girl * 56 + 481] & 1)) {
            date_pattern = 2;
            strcpy(ba_string2, date_basyo9_scene_set_str0);
        } else {
            date_pattern = 3;
            strcpy(ba_string2, date_basyo9_scene_set_str1);
        }
        gv = now_girl;
        p1 = date_impkao_tbl_5_imp;
        now_date_spot = 17;
        c3 = date_pattern * 3;
        imp[0] = (p1 + gv * 12U)[c3 + 0];
        imp[1] = (p1 + (u32)gv * 12U)[c3 + 1];
        imp[2] = (p1 + (gv | 0U) * 12U)[c3 + 2];
        p2 = date_impkao_tbl_5_kao;
        kao[0] = (p2 + (gv ^ 0U) * 12U)[c3 + 0];
        kao[1] = (p2 + (gv * 1U) * 12U)[c3 + 1];
        kao[2] = (p2 + (gv >> 0) * 12U)[c3 + 2];
    }
    now_serifu = (s16)(now_serifu + date_pattern * 2);
    if (now_girl == 7 && (u32)date_pattern >= 2U) {
        now_serifu = (s16)(now_serifu + 3);
    }
    sub_sub_step_up();
}
