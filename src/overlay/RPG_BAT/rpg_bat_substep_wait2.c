/* @SymbolStart: 0x8014F38C */
/* @SymbolSize: 60 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit2;


extern void rpg_bat_act_sub_next(void);

void rpg_bat_substep_wait2(s32 limit)
{
    rpg_bat_unit2.wait += 1;
    if (rpg_bat_unit2.wait >= limit) {
        rpg_bat_act_sub_next();
    }
}
