/* tt_sway_move - TT overlay, Address: 0x8014B594, Size: 216 bytes (54 insns) */
#include "types.h"

extern s32 rsin(s32 angle);
extern void tt_shot_aimed_make(s16 a0, s16 a1, s32 a2, s32 a3);

void tt_sway_move(u8 *obj)
{
    s32 v;

    if (obj[99] != 0) {
        v = rsin(*(s16 *)(obj + 22) << 5);
        *(s32 *)(obj + 32) = -v * 1536;
    } else {
        v = rsin(*(s16 *)(obj + 22) << 5);
        *(s32 *)(obj + 32) = v * 1536;
    }

    if ((u32)*(u16 *)(obj + 94) >= 17U && obj[96] == 0) {
        *(s16 *)(obj + 94) = 0;
        obj[96]++;
        tt_shot_aimed_make(*(s16 *)(obj + 20), *(s16 *)(obj + 22), 32, 160);
    }
    if (obj[81] != 0) {
        *(s16 *)(obj + 94) = *(u16 *)(obj + 94) + 1;
    }
}
