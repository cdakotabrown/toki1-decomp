/* geko_bustup_timer_step -- GEKO overlay, Address: 0x8013F6D0, Size: 144 bytes (36 insns), Byte-perfect */
#include "types.h"

extern s32 get_g_zyotai_h(s32 a0);
extern void bustup_timer_clamp(s32 a0);
extern void sub_sub_step_up(void);

extern u8 ev_flag43;

void geko_bustup_timer_step(void)
{
    s32 temp;

    temp = get_g_zyotai_h(10);
    bustup_timer_clamp(temp);
    if ((u32)ev_flag43 < (u32)2) goto label0;
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();

label0:
    sub_sub_step_up();
}
