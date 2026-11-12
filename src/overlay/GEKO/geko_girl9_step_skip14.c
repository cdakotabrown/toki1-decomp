/* geko_girl9_step_skip14 -- GEKO overlay, Address: 0x8013E52C, Size: 68 bytes */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 get_h_yuukou(s32 arg);

extern u8 now_sub_sub_step;

void geko_girl9_step_skip14(void)
{
    s32 result;

    result = get_h_yuukou(9);
    if ((u32)result < 80) {
        now_sub_sub_step = now_sub_sub_step + 14;
    }
    sub_sub_step_up();
}
