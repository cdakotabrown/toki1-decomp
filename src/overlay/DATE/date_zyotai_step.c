/* @SymbolSize: 124 */
/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp */
/* @PostProc: patch_insn_reg_field 0x2c:rs:t6:t7 0x2c:rt:t6:t7 0x34:rs:t6:t7 0x38:rt:v1:t6 0x3c:rt:t6:t8 0x40:rs:t7:t8 0x40:rt:t7:t8 0x44:rs:v1:t6 0x44:rt:t8:t9 0x48:rs:t8:t9 0x4c:rs:v1:t6 0x50:rt:t2:t1 0x54:rs:t7:t8 0x58:rd:t3:t2 0x58:rt:t2:t1 0x64:rt:t3:t2 */
/* Function: date_zyotai_step */
/* Overlay: DATE */
/* Address: 0x80139340 */
/* Size: 140 bytes (35 instructions; 31 body + trailing foreign words) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 get_g_zyotai_s(s32);
extern s32 date_score_apply(void);

extern volatile u8 now_girl;
extern volatile u8 date_pattern;
extern volatile s16 now_serifu;
extern volatile s32 serifu_no;

void date_zyotai_step(void)
{
    s32 v;
    s32 m;

    v = get_g_zyotai_s(now_girl);
    m = v & 0x7f;
    if (date_pattern == 5) {
        now_serifu = now_serifu +
                     (((u32)m < 2u) ^ 1) +
                     (((u32)m < 3u) ^ 1);
        date_score_apply();
        serifu_no = 0;
    } else {
        sub_sub_step_up();
    }
}
