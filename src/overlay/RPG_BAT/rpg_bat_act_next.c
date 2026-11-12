/* @SymbolSize: 40 */
/* rpg_bat_act_next - RPG_BAT overlay, Address: 0x8014F230, Size: 40 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit2;


s32 rpg_bat_unit2_step[3] = {0};

void rpg_bat_act_next(void)
{
    rpg_bat_unit2_step[0] = rpg_bat_unit2_step[0] + 1;
    rpg_bat_unit2_step[2] = 0;
    rpg_bat_unit2_step[1] = 0;
    rpg_bat_unit2.wait = 0;
}
