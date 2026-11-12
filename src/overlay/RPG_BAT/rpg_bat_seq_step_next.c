/* @SymbolSize: 48 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit1;


void rpg_bat_seq_step_next(void)
{
    rpg_bat_unit1.step = rpg_bat_unit1.step + 1;
    rpg_bat_unit1.cnt = 0;
    rpg_bat_unit1.sub = 0;
    rpg_bat_unit1.wait = 0;
}
