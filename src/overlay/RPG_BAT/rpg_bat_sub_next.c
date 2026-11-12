/* @SymbolSize: 28 */
#include "types.h"

s32 rpg_bat_unit3_step[4] = {0};

void rpg_bat_sub_next(void)
{
    rpg_bat_unit3_step[1] = rpg_bat_unit3_step[1] + 1;
    rpg_bat_unit3_step[3] = 0;
}
