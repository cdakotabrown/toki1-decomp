/* @SymbolSize: 36 */
#include "types.h"

s32 rpg_bat_unit3_step[4] = {0};

void rpg_bat_seq_next(void)
{
    rpg_bat_unit3_step[0] = rpg_bat_unit3_step[0] + 1;
    rpg_bat_unit3_step[2] = 0;
    rpg_bat_unit3_step[1] = 0;
    rpg_bat_unit3_step[3] = 0;
}
