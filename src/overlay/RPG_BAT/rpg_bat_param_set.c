/* @SymbolStart: 0x801477E0 */
/* @SymbolSize: 336 */
/* Address: 0x801477E8 (window starts at the absorbed cross-prologue head */
/* Size: 336 bytes (84 insns) including the head */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 get_rpg_bat_roll2();

extern s32 rpg_bat_girl;
extern s32 rpg_bat_dmg[];
extern s32 rpg_bat_player_dmg;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void rpg_bat_param_set(void)
{
    s32 sel, a0, t4;

    sel = rpg_bat_girl;
    switch (sel) {
    default:
        break;
    case 2:
        rpg_bat_dmg[0] = get_rpg_bat_roll2(1250, 750) + 1;
        break;
    case 5:
        rpg_bat_dmg[0] = get_rpg_bat_roll2(2000, 1000) + 1;
        break;
    case 3:
        if ((rpg_bat_unit1.flag & 0x600000) != 0) {
            rpg_bat_dmg[0] = get_rpg_bat_roll2(1000, 500) + 1;
        } else {
            rpg_bat_dmg[0] = get_rpg_bat_roll2(600, 300) + 1;
            rpg_bat_dmg[1] = get_rpg_bat_roll2(600, 300) + 1;
            rpg_bat_dmg[2] = get_rpg_bat_roll2(600, 300) + 1;
        }
        t4 = rpg_bat_unit0.hp / 4;
        a0 = t4;
        if (t4 >= 101) {
            a0 = 100;
        }
        rpg_bat_player_dmg = get_rpg_bat_roll2(a0, a0);
        break;
    case 4:
        rpg_bat_dmg[0] = get_rpg_bat_roll2(75, 125) + 1;
        break;
    }
}
