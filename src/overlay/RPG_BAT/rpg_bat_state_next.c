/* rpg_bat_state_next - RPG_BAT overlay, Address: 0x8014EBD0, Size: 28 bytes (7 insns) */
#include "types.h"

s32 rpg_bat_unit0_sub[3] = {0};

void rpg_bat_state_next(void)
{
    rpg_bat_unit0_sub[0] = rpg_bat_unit0_sub[0] + 1;
    rpg_bat_unit0_sub[2] = 0;
}
