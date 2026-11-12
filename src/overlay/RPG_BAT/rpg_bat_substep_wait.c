/* @SymbolStart: 0x8014F0BC */
/* @SymbolSize: 60 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit1;


extern void rpg_bat_substep_next(void);

void rpg_bat_substep_wait(s32 limit)
{
    rpg_bat_unit1.wait += 1;
    if (rpg_bat_unit1.wait >= limit) {
        rpg_bat_substep_next();
    }
}
