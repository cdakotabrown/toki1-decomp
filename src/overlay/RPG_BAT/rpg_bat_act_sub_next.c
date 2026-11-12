/* @SymbolSize: 32 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit2;


void rpg_bat_act_sub_next(void)
{
    rpg_bat_unit2.sub = rpg_bat_unit2.sub + 1;
    rpg_bat_unit2.wait = 0;
}
