/* Address: 0x8014B404, Size: 144 bytes (36 insns) */
#include "types.h"

extern void tt_shot_aimed_make(s16 x, s16 y, s32 width, s32 height);
extern u8 *tt_scroll_wk;

void tt_obj_timer(u8 *obj)
{
    u8 *base;

    base = tt_scroll_wk;
    if (base[11] != 0) {
        if ((u32)*(u16 *)(obj + 94) >= 25) {
            if (obj[96] == 0) {
                *(u16 *)(obj + 94) = 0;
                obj[96] = obj[96] + 1;
                tt_shot_aimed_make(*(s16 *)(obj + 20), *(s16 *)(obj + 22), 32, 160);
            }
        }
    }

    if (obj[81] != 0) {
        *(u16 *)(obj + 94) = *(u16 *)(obj + 94) + 1;
    }
}
