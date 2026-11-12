/* @SymbolStart: 0x8014F080 */
/* @SymbolSize: 60 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit1;


extern void rpg_bat_seq_step_next(void);

void rpg_bat_seq_wait(s32 limit)
{
    rpg_bat_unit1.wait += 1;
    if (rpg_bat_unit1.wait >= limit) {
        rpg_bat_seq_step_next();
    }
}
