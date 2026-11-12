/* rpg_bat_cw_anim_set - RPG_BAT overlay, Address: 0x8013E864, Size: 84 bytes */
#include "types.h"

extern u8 class_w[];

void rpg_bat_cw_anim_set(s32 idx, s32 a1, s32 a2)
{
    u8 *e = class_w + idx * 68;

    *(u16 *)(e + 8) = 0;
    e[2] = 1;
    if (a2 == 1)
        e[3] = 128;
    else
        e[3] = 0;
    *(u16 *)(e + 24) = 0;
    if (a1 != 255)
        *(u16 *)(e + 22) = a1;
}
