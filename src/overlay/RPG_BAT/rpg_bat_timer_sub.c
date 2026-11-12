/* @SymbolStart: 0x8014ED80 */
/* @SymbolSize: 60 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit0;


extern void rpg_bat_state_next(void);

void rpg_bat_timer_sub(s32 limit)
{
    rpg_bat_unit0.wait += 1;
    if (rpg_bat_unit0.wait >= limit) {
        rpg_bat_state_next();
    }
}
