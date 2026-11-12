/* rpg_bat_slot_anim_set - RPG_BAT overlay, Address: 0x8013E8B8, Size: 84 bytes */
#include "types.h"

extern u8 class_w[];

void rpg_bat_slot_anim_set(s32 idx, s32 a1, s32 a2)
{
    u8 *e = class_w + idx * 68;

    *(u16 *)(e + 4360) = 0;
    e[4354] = 1;
    if (a2 == 1)
        e[4355] = 128;
    else
        e[4355] = 0;
    *(u16 *)(e + 4376) = 0;
    if (a1 != 255)
        *(u16 *)(e + 4374) = a1;
}
