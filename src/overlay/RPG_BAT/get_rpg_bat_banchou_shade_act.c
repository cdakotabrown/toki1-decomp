/* @SymbolStart: 0x801452C0 */
/* @SymbolSize: 244 */
#include "types.h"

extern s32 rpg_bat_act_wt[];
extern s32 get_rand_mod(s32);

s32 get_rpg_bat_banchou_shade_act(void)
{
    s32 a0;
    s32 v0;

    a0 = rpg_bat_act_wt[0] + rpg_bat_act_wt[1] + rpg_bat_act_wt[2];

    if (a0 < 11) {
        rpg_bat_act_wt[2] = 15;
        rpg_bat_act_wt[0] = 25;
        rpg_bat_act_wt[1] = 15;
        a0 = 55;
    }

    v0 = get_rand_mod(a0);
    if (v0 < rpg_bat_act_wt[0]) {
        rpg_bat_act_wt[0] = rpg_bat_act_wt[0] - 5;
        return 0;
    }
    rpg_bat_act_wt[0] = rpg_bat_act_wt[0];
    if (v0 < rpg_bat_act_wt[0] + rpg_bat_act_wt[1]) {
        rpg_bat_act_wt[1] = rpg_bat_act_wt[1] - 5;
        return 1;
    }

    rpg_bat_act_wt[2] = rpg_bat_act_wt[2] - 5;
    rpg_bat_act_wt[14] = rpg_bat_act_wt[14] + 2;
    return 4;
}
