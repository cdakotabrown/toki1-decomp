/* @SymbolSize: 180 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: patch_insn_reg_field 0x30:rt:t6:t7 0x34:rs:t6:t7 0x34:rt:t6:t7 0x38:rt:t7:t6 0x40:rd:t9:t8 0x44:rt:t8:t0 0x48:rs:t8:t0 0x48:rt:t8:t0 0x4c:rd:t0:t9 0x4c:rt:t9:t8 0x50:rs:t8:t9 0x54:rt:t1:t2 0x58:rs:t1:t2 0x60:rt:t6:t7 0x64:rt:t2:t3 0x68:rs:t2:t3 0x68:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t3:t4 0x74:rt:t3:t4 0x78:rt:t4:t5 0x7c:rs:t4:t5 0x7c:rt:t4:t5 0x84:rs:t4:t5 0x84:rt:t5:t6 0x8c:rt:t5:t6 0x90:rs:t6:t7 0x90:rt:t6:t7 0x98:rs:t6:t7 0x98:rt:t7:t8 0xa0:rt:t7:t8 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_g_zyotai_s(s32);
extern u8 now_month;
extern u8 now_day;
extern s32 p_birthday;
extern u8 day_flag;
extern u8 now_girl;
extern u8 now_sub_sub_step;

void shougatu_birth_day_check2(void)
{
    s32 r;
    s32 v1;

    r = get_g_zyotai_s(now_girl);
    v1 = p_birthday;
    if ((v1 & 0xF) == now_month && ((u32)(v1 << 23) >> 27) == now_day) {
        if ((u32)(r & 0x7F) < 2) {
            day_flag |= 4;
            now_sub_sub_step += 5;
        } else {
            day_flag |= 8;
            sub_sub_step_up();
        }
    } else {
        sub_sub_step_up();
    }
}
