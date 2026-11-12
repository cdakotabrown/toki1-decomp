/* Address: 0x80136158 */
/* Size: 140 bytes (35 instructions) */
/* @PostProc: patch_insn_reg_field 0x30:rt:t7:t8 0x34:rs:t7:t8 0x3c:rt:t8:t9 0x40:rs:t8:t9 0x40:rt:t8:t9 0x48:rs:t8:t9 0x68:rt:t9:t0 0x70:rt:t9:t0 */
/* @SymbolSize: 140 */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern void shougatu_serifu_run(void);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern s32 step_counter;
extern s16 shougatu_meet_now_serifu;
extern u8 shougatu_alone_flag;

void shougatu_girl5_serifu_check2(void)
{
    s32 t8;
    s32 v1;
    s32 v0;

    t8 = get_g_zyotai_s(5) & 0x7F;
    if (now_girl == 5 && (u32)t8 < 2 && shougatu_alone_flag == 0) {
        v1 = step_counter;
        v0 = (u32)v1 < 1;
        step_counter = v1 + 1;
        if (v0) {
            shougatu_meet_now_serifu = 42;
        }
        shougatu_serifu_run();
    } else {
        sub_sub_step_up();
    }
}
