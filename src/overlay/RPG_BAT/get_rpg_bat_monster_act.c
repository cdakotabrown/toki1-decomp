/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit1;


s32 get_rpg_bat_monster_act()
{
    if (++rpg_bat_unit1.aux14 < 3) {
        return 1;
    }
    return 2;
}
