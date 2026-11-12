/* rpg_bat_anim_set - RPG_BAT overlay, Address: 0x8013E7C0, Size: 80 bytes */
#include "types.h"

extern u8 class_w[];

void rpg_bat_anim_set(s32 idx, s32 a1, s32 a2, s32 a3)
{
    u8 *e = class_w + idx * 68;

    *(u16 *)(e + 6536) = 0;
    e[6530] = a2;
    if (a3 == 1)
        e[6531] = 128;
    else
        e[6531] = 0;
    *(u16 *)(e + 6552) = 0;
    if (a1 != 255)
        *(u16 *)(e + 6550) = a1;
}
