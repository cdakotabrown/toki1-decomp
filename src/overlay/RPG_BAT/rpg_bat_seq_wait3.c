/* @SymbolStart: 0x8014F5DC */
/* @SymbolSize: 60 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit3;


extern void rpg_bat_seq_next(void);

void rpg_bat_seq_wait3(s32 limit)
{
    rpg_bat_unit3.wait += 1;
    if (rpg_bat_unit3.wait >= limit) {
        rpg_bat_seq_next();
    }
}
