/* geko_girl9_step_skip3 -- GEKO overlay, Address: 0x8013E570, Size: 76 bytes */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_h_yuukou(s32 a0);

void geko_girl9_step_skip3(void)
{
    s32 result;

    result = get_h_yuukou(9);
    if ((u32)result >= 80U) {
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
