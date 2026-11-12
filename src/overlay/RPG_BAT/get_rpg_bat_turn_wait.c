/* THE FRAME.  The file was pinned `@CompileFlags: -Wc,-pic0` under the */
/* @SymbolSize: 88 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit1;

extern s32 get_rpg_bat_roll2();

s32 get_rpg_bat_turn_wait(s32 arg0)
{
    if (arg0 == 0) {
        return 120;
    }
    if ((rpg_bat_unit1.flag & 0x80000000) != 0) {
        return get_rpg_bat_roll2(30, 30) + 30;
    }
    return get_rpg_bat_roll2(40, 30) + 60;
}
