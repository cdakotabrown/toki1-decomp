/* @SymbolSize: 300 */
/* tt_shot_heading_step - TT overlay, Address: 0x801349B0, Size: 300 bytes (75 insns) */
#include "types.h"

extern s32 rsin();
extern s32 rcos();
extern s32 get_angle();

extern s32 *tt_player_wk;

void tt_shot_heading_step(u8 *w)
{
    s32 *other;
    s32 ang;
    s32 v1;

    other = tt_player_wk;
    v1 = *(u16 *)(w + 96);
    if (*(u16 *)(w + 88) >= 256U) {
        return;
    }
    *(s16 *)(w + 88) = *(u16 *)(w + 88) + 1;
    if (0) {
    }
    ang = get_angle((other[9] - *(s32 *)(w + 36)) >> 16,
                        (other[8] - *(s32 *)(w + 32)) >> 16);

    if (v1 < 2048) {
        if (v1 < ((ang + 2048) & 0xFFF) && ((ang + 2048) & 0xFFF) < v1 + 2048) {
            v1 += 24;
        } else {
            v1 -= 24;
        }
    }
    if (v1 >= 2048) {
        if (((ang + 2048) & 0xFFF) < v1 && v1 - 2048 < ((ang + 2048) & 0xFFF)) {
            v1 -= 24;
        } else {
            v1 += 24;
        }
    }
    if (v1 < 0) {
        v1 += 4096;
    }
    v1 &= 0xFFF;
    *(s32 *)(w + 40) = rcos(v1 - 2048) * 24;
    *(s32 *)(w + 44) = rsin(v1 - 2048) * 24;
    *(s16 *)(w + 96) = v1;
}
