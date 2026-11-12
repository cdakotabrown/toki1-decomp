/* @SymbolStart: 0x80147930 */
/* @SymbolSize: 1504 */
/* @RodataBase: 0x8015CA00 */
/* @PostProc: swap_registers v1:a1 --bidirectional */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x318 --offset2 0x324 */
/* @PostProc: swap_instructions --offset1 0x384 --offset2 0x390 */
/* @PostProc: swap_instructions --offset1 0x404 --offset2 0x410 */
/* @PostProc: swap_instructions --offset1 0x544 --offset2 0x54c */
/* @PostProc: swap_instructions --offset1 0x548 --offset2 0x54c */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* rpg_bat_judge - RPG_BAT overlay, Address: 0x80147940, Size: 1484 bytes. */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 get_rand_mod(s32);

extern s32 rpg_bat_girl;
extern volatile s32 rpg_bat_judge_wt;
extern volatile s32 rpg_bat_judge_wt_1;
extern s32 rpg_bat_toriko_flag;
extern s32 rpg_bat_toriko_flag_1;
extern s32 rpg_bat_toriko_flag_2;
extern s32 rpg_bat_dmg_rate;
extern s32 rpg_bat_act_delay;
extern s32 rpg_bat_next_delay;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;

s32 rpg_bat_judge(void)
{
    s32 v1;
    s32 a1;
    s32 v0;
    s32 t4;

    v1 = *(s32 *)&rpg_bat_judge_wt;
    a1 = *(s32 *)&rpg_bat_judge_wt_1;

    if (a1 + v1 < 20) {
        switch (rpg_bat_girl) {
        case 0:  v1 = 50;  a1 = 50;  break;
        case 2:  v1 = 50;  a1 = 50;  break;
        case 3:  v1 = 50;  a1 = 50;  break;
        case 4:  v1 = 50;  a1 = 50;  break;
        case 5:  v1 = 50;  a1 = 50;  break;
        case 6:  v1 = 50;  a1 = 50;  break;
        case 10: v1 = 50;  a1 = 50;  break;
        case 8:  v1 = 50;  a1 = 50;  break;
        case 1:  v1 = 100; a1 = 100; break;
        case 7:  v1 = 50;  a1 = 50;  break;
        case 9:  v1 = 50;  a1 = 50;  break;
        case 13: v1 = 100; a1 = 100; break;
        case 14: v1 = 100; a1 = 100; break;
        }
    }

    rpg_bat_judge_wt = v1;
    rpg_bat_judge_wt_1 = a1;

    switch (rpg_bat_girl) {
    case 0:
        rpg_bat_judge_wt = v1;
        rpg_bat_judge_wt_1 = a1;
        v0 = get_rand_mod(a1 + v1);
        v1 = rpg_bat_judge_wt;
        if (v0 < v1) {
            v1 = v1 - 5;
            rpg_bat_judge_wt = v1;
            return 0;
        }
        a1 = rpg_bat_judge_wt_1;
        rpg_bat_unit1.aux08 = 0;
        a1 = a1 - 5;
        rpg_bat_judge_wt_1 = a1;
        return 1;
    case 2:
        rpg_bat_judge_wt = v1;
        rpg_bat_judge_wt_1 = a1;
        v0 = get_rand_mod(a1 + v1);
        v1 = rpg_bat_judge_wt;
        if (v0 < v1) {
            v1 = v1 - 5;
            rpg_bat_judge_wt = v1;
            return 0;
        }
        a1 = rpg_bat_judge_wt_1;
        a1 = a1 - 5;
        rpg_bat_judge_wt_1 = a1;
        return 1;
    case 3:
        rpg_bat_judge_wt = v1;
        rpg_bat_judge_wt_1 = a1;
        v0 = get_rand_mod(a1 + v1);
        v1 = rpg_bat_judge_wt;
        if (v0 < v1) {
            v1 = v1 - 5;
            rpg_bat_judge_wt = v1;
            return 0;
        }
        a1 = rpg_bat_judge_wt_1;
        a1 = a1 - 5;
        rpg_bat_judge_wt_1 = a1;
        return 1;
    case 4:
        rpg_bat_judge_wt = v1;
        rpg_bat_judge_wt_1 = a1;
        v0 = get_rand_mod(a1 + v1);
        v1 = rpg_bat_judge_wt;
        if (v0 < v1) {
            v1 = v1 - 5;
            rpg_bat_judge_wt = v1;
            return 0;
        }
        a1 = rpg_bat_judge_wt_1;
        a1 = a1 - 5;
        rpg_bat_judge_wt_1 = a1;
        return 1;
    case 5:
        rpg_bat_judge_wt = v1;
        rpg_bat_judge_wt_1 = a1;
        v0 = get_rand_mod(a1 + v1);
        v1 = rpg_bat_judge_wt;
        if (v0 < v1) {
            v1 = v1 - 5;
            rpg_bat_judge_wt = v1;
            return 0;
        }
        a1 = rpg_bat_judge_wt_1;
        a1 = a1 - 5;
        rpg_bat_judge_wt_1 = a1;
        return 1;
    case 6:
        rpg_bat_judge_wt = v1;
        rpg_bat_judge_wt_1 = a1;
        v0 = get_rand_mod(a1 + v1);
        v1 = rpg_bat_judge_wt;
        if (v0 < v1) {
            v1 = v1 - 5;
            rpg_bat_judge_wt = v1;
            return 0;
        }
        rpg_bat_dmg_rate = rpg_bat_dmg_rate + 10;
        a1 = rpg_bat_judge_wt_1;
        a1 = a1 - 5;
        rpg_bat_judge_wt_1 = a1;
        return 1;
    case 10:
        rpg_bat_judge_wt = v1;
        rpg_bat_judge_wt_1 = a1;
        v0 = get_rand_mod(a1 + v1);
        v1 = rpg_bat_judge_wt;
        if (v0 < v1) {
            v1 = v1 - 5;
            rpg_bat_judge_wt = v1;
            return 0;
        }
        rpg_bat_next_delay = 50;
        a1 = rpg_bat_judge_wt_1;
        a1 = a1 - 5;
        rpg_bat_judge_wt_1 = a1;
        return 1;
    case 8:
        rpg_bat_judge_wt = v1;
        rpg_bat_judge_wt_1 = a1;
        v0 = get_rand_mod(a1 + v1);
        if (rpg_bat_act_delay >= 101) {
            return 0;
        }
        v1 = rpg_bat_judge_wt;
        if (v0 < v1) {
            v1 = v1 - 5;
            rpg_bat_judge_wt = v1;
            return 0;
        }
        rpg_bat_act_delay = 360;
        a1 = rpg_bat_judge_wt_1;
        a1 = a1 - 5;
        rpg_bat_judge_wt_1 = a1;
        return 1;
    case 1:
        return 0x80000000;
    case 7:
        rpg_bat_judge_wt = v1;
        rpg_bat_judge_wt_1 = a1;
        v0 = get_rand_mod(a1 + v1);
        v1 = rpg_bat_judge_wt;
        if (v0 < v1) {
            v1 = v1 - 5;
            rpg_bat_judge_wt = v1;
            return 0;
        }
        a1 = rpg_bat_judge_wt_1;
        t4 = rpg_bat_unit1.flag;
        a1 = a1 - 5;
        rpg_bat_judge_wt_1 = a1;
        if ((t4 << 11) < 0) {
            if (rpg_bat_toriko_flag_2 + rpg_bat_toriko_flag + rpg_bat_toriko_flag_1 == 3) {
                return 0;
            }
            if (get_rand_mod(100) < 50) {
                return 1;
            }
            return 2;
        }
        return 2;
    case 9:
        rpg_bat_judge_wt = v1;
        rpg_bat_judge_wt_1 = a1;
        v0 = get_rand_mod(a1 + v1);
        v1 = rpg_bat_judge_wt;
        if (v0 < v1) {
            v0 = rpg_bat_unit2.flag;
            v1 = v1 - 5;
            rpg_bat_judge_wt = v1;
            return v0;
        }
        a1 = rpg_bat_judge_wt_1;
        a1 = a1 - 5;
        rpg_bat_judge_wt_1 = a1;
        if (rpg_bat_unit2.flag != 0x40000000) {
            return 1;
        }
        if (get_rand_mod(5) == 0) {
            return 2;
        }
        return 3;
    case 13:
        if (rpg_bat_unit2.hp < 8) {
            return 3;
        }
        if (get_rand_mod(2) == 0) {
            return 2;
        }
        return 1;
    case 14:
        break;
    }

}
