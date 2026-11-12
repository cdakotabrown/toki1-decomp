/* tt_cross_move - TT overlay, Address: 0x8014C1AC, Size: 316 bytes (79 insns) */
#include "types.h"

extern u8 *tt_player_wk;

extern void tt_shot_aimed_make(s16 a0, s16 a1, s32 a2, s32 a3);

void tt_cross_move(u8 *obj)
{
    u8 *ptr;

    switch (*(u16 *)(obj + 6)) {
    case 0:
        ptr = tt_player_wk;
        if (*(s32 *)(obj + 36) < *(s32 *)(ptr + 36)) {
            break;
        }
        *(s32 *)(obj + 44) = 0;
        if (obj[99] != 0) {
            *(s32 *)(obj + 40) = 0xFFFE0000;
        } else {
            *(s32 *)(obj + 40) = 0x20000;
        }
        *(s16 *)(obj + 6) = 1;
        break;
    case 1:
        if (obj[99] != 0) {
            ptr = tt_player_wk;
            if (*(s32 *)(ptr + 32) < *(s32 *)(obj + 32)) {
                break;
            }
            *(s32 *)(obj + 40) = 0;
            *(s32 *)(obj + 44) = 0xFFFE0000;
        } else {
            ptr = tt_player_wk;
            if (*(s32 *)(obj + 32) < *(s32 *)(ptr + 32)) {
                break;
            }
            *(s32 *)(obj + 40) = 0;
            *(s32 *)(obj + 44) = 0xFFFE0000;
        }
        break;
    }

    if ((u32)*(u16 *)(obj + 94) >= 25U && obj[96] == 0) {
        *(s16 *)(obj + 94) = 0;
        obj[96]++;
        tt_shot_aimed_make(*(s16 *)(obj + 20), *(s16 *)(obj + 22), 32, 160);
    }
    if (obj[81] != 0) {
        *(s16 *)(obj + 94) = *(u16 *)(obj + 94) + 1;
    }
}
