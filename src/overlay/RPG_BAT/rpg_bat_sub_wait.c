/* @SymbolStart: 0x8014F618 */
/* @SymbolSize: 60 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit3;


extern void rpg_bat_sub_next(void);

void rpg_bat_sub_wait(s32 limit)
{
    rpg_bat_unit3.wait += 1;
    if (rpg_bat_unit3.wait >= limit) {
        rpg_bat_sub_next();
    }
}
