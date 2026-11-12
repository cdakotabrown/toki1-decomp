/* @SymbolStart: 0x80145180 */
/* @SymbolSize: 320 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 get_rand_mod();

extern s32 rpg_bat_act_wt;
extern s32 rpg_bat_act_wt_1;
extern s32 rpg_bat_act_wt_2[];
extern RPG_BAT_UNIT rpg_bat_unit1;

s32 get_rpg_bat_banchou_power_act(void)
{
    s32 v0;
    s32 a0;

    if (rpg_bat_unit1.hp < rpg_bat_unit1.hp_max / 3) {
        rpg_bat_act_wt_2[0] = 15;
        rpg_bat_act_wt_1 = 5;
        rpg_bat_act_wt = 0;
        return 3;
    }
    a0 = rpg_bat_act_wt_2[0] + rpg_bat_act_wt + rpg_bat_act_wt_1;
    if (a0 < 11) {
        rpg_bat_act_wt = 20;
        rpg_bat_act_wt_2[0] = 20;
        rpg_bat_act_wt_1 = 15;
        a0 = 55;
    }
    v0 = get_rand_mod(a0);
    if (v0 < rpg_bat_act_wt) {
        rpg_bat_act_wt = rpg_bat_act_wt - 5;
        return 0;
    }
    rpg_bat_act_wt = rpg_bat_act_wt;
    if (v0 < rpg_bat_act_wt_1 + rpg_bat_act_wt) {
        rpg_bat_act_wt_1 = rpg_bat_act_wt_1 - 5;
        return 1;
    }
    rpg_bat_act_wt_2[0] = rpg_bat_act_wt_2[0] - 5;
    rpg_bat_act_wt_2[12] = rpg_bat_act_wt_2[12] + 2;
    return 2;
}
