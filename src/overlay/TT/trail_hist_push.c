/* trail_hist_push - TT overlay, Address: 0x80147F04, Size: 188 bytes (47 insns) */
#include "types.h"

extern u8 *tt_trail_buf;
extern u8 *tt_player_wk;

void trail_hist_push(u8 *p)
{
    u8 *head;
    u8 *src;
    s32 i;

    head = tt_trail_buf;
    src = tt_player_wk;
    if (p == head) {
        *(s16 *)(p + 98) = *(s16 *)(src + 20);
        *(s16 *)(p + 130) = *(s16 *)(src + 22);
    } else {
        *(s16 *)(p + 98) = *(s16 *)(p - 84);
        *(s16 *)(p + 130) = *(s16 *)(p - 52);
    }
    for (i = 1; i < 10; i++) {
        *(s16 *)(p + 78 + i * 2) = *(s16 *)(p + 80 + i * 2);
        *(s16 *)(p + 110 + i * 2) = *(s16 *)(p + 112 + i * 2);
    }
    *(s16 *)(p + 20) = *(s16 *)(p + 80);
    *(s16 *)(p + 22) = *(s16 *)(p + 112);
}
