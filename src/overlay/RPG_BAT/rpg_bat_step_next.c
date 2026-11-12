/* rpg_bat_step_next - RPG_BAT overlay, Address: 0x8014EBA8, Size: 40 bytes. */
/* @CompileFlags: -cckr */
#include "types.h"

s32 rpg_bat_unit0_step = 6;
s32 rpg_bat_unit0_sub[3] = {7, 8, 9};

void rpg_bat_step_next()
{
    rpg_bat_unit0_step++;
    rpg_bat_unit0_sub[1] = 0;
    rpg_bat_unit0_sub[0] = 0;
    rpg_bat_unit0_sub[2] = 0;
}
