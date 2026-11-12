/* @SymbolStart: 0x801453B4 */
/* @SymbolSize: 308 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd4 */
#include "types.h"

extern s32 get_rand_mod(s32);

extern s32 rpg_bat_act_wt;
extern s32 rpg_bat_act_wt_1;
extern s32 rpg_bat_act_wt_2;
extern s32 rpg_bat_act_wt_3;

s32 get_rpg_bat_utyuuzin_act(void)
{
    s32 arg0;
    s32 v0;

    arg0 = rpg_bat_act_wt_3 + rpg_bat_act_wt + rpg_bat_act_wt_1 + rpg_bat_act_wt_2;

    if (arg0 < 6) {
        rpg_bat_act_wt_3 = 10;
        rpg_bat_act_wt_1 = 5;
        rpg_bat_act_wt_2 = 10;
        rpg_bat_act_wt = 15;
        arg0 = 40;
    }
    v0 = get_rand_mod(arg0);

    if (v0 < rpg_bat_act_wt) {
        rpg_bat_act_wt = rpg_bat_act_wt - 5;
        return 0;
    }
    if (v0 < rpg_bat_act_wt_1 + rpg_bat_act_wt) {
        rpg_bat_act_wt = rpg_bat_act_wt;
        rpg_bat_act_wt_1 = rpg_bat_act_wt_1 - 5;
        return 4;
    }
    rpg_bat_act_wt_1 = rpg_bat_act_wt_1;
    rpg_bat_act_wt = rpg_bat_act_wt;
    if (v0 < rpg_bat_act_wt_2 + rpg_bat_act_wt + rpg_bat_act_wt_1) {
        rpg_bat_act_wt_2 = rpg_bat_act_wt_2 - 5;
        return 1;
    }
    rpg_bat_act_wt_3 = rpg_bat_act_wt_3 - 5;
    return 2;
}
