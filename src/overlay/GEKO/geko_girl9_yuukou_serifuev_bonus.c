/* geko_girl9_yuukou_serifuev_bonus -- GEKO overlay, Address: 0x8013E4E8, Size: 68 bytes */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 get_h_yuukou(s32 arg);

extern u16 now_serifuev;

void geko_girl9_yuukou_serifuev_bonus(void)
{
    s32 result;

    result = get_h_yuukou(9);
    if ((u32)result < 80) {
        now_serifuev = now_serifuev + 2;
    }
    sub_sub_step_up();
}
