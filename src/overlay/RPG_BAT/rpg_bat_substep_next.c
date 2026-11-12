/* @SymbolSize: 32 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit1;


void rpg_bat_substep_next(void)
{
    rpg_bat_unit1.sub = rpg_bat_unit1.sub + 1;
    rpg_bat_unit1.wait = 0;
}
