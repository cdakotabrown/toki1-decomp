/* @PostProc: rename_register_chain --chain t7:t8:t9:t0:t1:t2 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x60 */
#include "types.h"

extern s32 get_g_zyotai_s(s32 a0);
extern void goto_sub_step(s32 a0);
extern void sub_sub_step_up(void);

extern u8  now_girl;
extern u8  now_sub_step;
extern u8  ret_sub_step;
extern u8  now_sub_sub_step;
extern u8  ret_sub_sub_step;
extern u8  girl_no;

void shugaku_girl4_event_judge(void)
{
    s32 v0;

    v0 = get_g_zyotai_s(4);
    if (now_girl != 4 || (u32)(v0 & 0x7F) >= 2U) {
        sub_sub_step_up();
        return;
    }
    ret_sub_step = now_sub_step;
    ret_sub_sub_step = now_sub_sub_step + 2;
    girl_no = 4;
    goto_sub_step(10);
}
