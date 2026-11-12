/* tt_camera_ease - TT overlay, Address: 0x80138240, Size: 192 bytes (48 insns) */
#include "types.h"

extern u8 *tt_boss_wk;
extern u8 *tt_player_wk;

void tt_camera_ease(void)
{
    u8 *a;
    u8 *b;
    s32 d;
    s32 ad;
    s16 v;

    a = tt_boss_wk;
    b = tt_player_wk;
    d = *(s16 *)(b + 20) - *(s16 *)(a + 20);
    if (d < 0) {
        ad = -d;
    } else {
        ad = d;
    }

    if (ad < 2) {
        *(s32 *)(a + 32) = *(s16 *)(b + 20) << 16;
    } else if (d < 0) {
        *(s32 *)(a + 32) = *(s32 *)(a + 32) - 16384;
    } else {
        *(s32 *)(a + 32) = *(s32 *)(a + 32) + 16384;
    }

    v = *(s16 *)(a + 34);
    if (v >= 81) {
        *(s32 *)(a + 32) = 0x00500000;
        v = *(s16 *)(a + 34);
    } else if (v < -80) {
        *(s32 *)(a + 32) = 0xFFB00000;
        v = *(s16 *)(a + 34);
    }
    *(s16 *)(a + 20) = v;
}
