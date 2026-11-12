/* @SymbolStart: 0x80148F70 */
/* @SymbolSize: 1644 */
/* @RodataBase: 0x8015CBC0 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

typedef struct {
    u16 val;
    u16 unk2;
} Ent;

extern Ent bukatu_rec[];
extern s32 rpg_bat_bukatu_no;
extern s32 rpg_bat_mode;
extern s32 rpg_bat_target_no;
extern s32 rpg_bat_dmg[];
extern s32 rpg_bat_hit_cnt;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void rpg_bat_reward_set()
{
    u32 val;
    s32 flag;
    s32 base;

    val = bukatu_rec[rpg_bat_bukatu_no].val;
    if (val < 50) {
        base = 50;
    } else if (val > 550) {
        base = 550;
    } else {
        base = val;
    }
    base = base * 10 - 500;
    if (rpg_bat_mode & 0xF0000) {
        rpg_bat_reward_roll();
    }
    if (rpg_bat_mode & 0x80000000) {
        rpg_bat_reward_roll();
    }
    flag = rpg_bat_unit1.flag & 0x100000;
    if (flag || (rpg_bat_unit1.flag & 0x600000) || (rpg_bat_unit1.flag & 0x8000000)) {
        if (rpg_bat_unit0.flag & 2) {
            switch (rpg_bat_bukatu_no) {
            case 2:
                if (flag) {
                    rpg_bat_dmg[0] = get_rpg_bat_roll2(2500, 2500) + 2500;
                    rpg_bat_dmg[1] = get_rpg_bat_roll2(2500, 2500) + 2500;
                    rpg_bat_dmg[2] = get_rpg_bat_roll2(2500, 2500) + 2500;
                    if (rpg_bat_target_no == 0) {
                        rpg_bat_dmg[0] = 9999;
                    }
                    if (rpg_bat_target_no == 1) {
                        rpg_bat_dmg[1] = 9999;
                    }
                    if (rpg_bat_target_no == 2) {
                        rpg_bat_dmg[2] = 9999;
                    }
                } else {
                    rpg_bat_dmg[0] = 9999;
                }
                break;
            case 5:
                if (flag) {
                    rpg_bat_dmg[0] = get_rpg_bat_roll2(2500, 1000) + 2000;
                    rpg_bat_dmg[1] = get_rpg_bat_roll2(2500, 1000) + 2000;
                    rpg_bat_dmg[2] = get_rpg_bat_roll2(2500, 1000) + 2000;
                    if (rpg_bat_target_no == 0) {
                        rpg_bat_dmg[0] += 2000;
                    }
                    if (rpg_bat_target_no == 1) {
                        rpg_bat_dmg[1] += 2000;
                    }
                    if (rpg_bat_target_no == 2) {
                        rpg_bat_dmg[2] += 2000;
                    }
                } else {
                    rpg_bat_dmg[0] = get_rpg_bat_roll2(2500, 1000) + 4000;
                }
                break;
            case 6:
                if (flag) {
                    rpg_bat_dmg[0] = get_rand_mod(50) * 100 + 2000;
                    rpg_bat_dmg[1] = get_rand_mod(50) * 100 + 1000;
                    rpg_bat_dmg[2] = get_rand_mod(50) * 100 + 1000;
                } else {
                    rpg_bat_dmg[0] = get_rand_mod(50) * 100 + 3000;
                }
                break;
            case 1:
                if (flag) {
                    rpg_bat_dmg[0] = get_rpg_bat_roll2(6000, base);
                    rpg_bat_dmg[1] = get_rpg_bat_roll2(6000, base);
                    rpg_bat_dmg[2] = get_rpg_bat_roll2(6000, base);
                } else {
                    rpg_bat_dmg[0] = get_rpg_bat_roll2(6000, base);
                }
                break;
            case 4:
                if (flag) {
                    if (get_rand_mod(100) < 20) {
                        rpg_bat_dmg[0] = 1000;
                    } else {
                        rpg_bat_dmg[0] = get_rpg_bat_roll2(base / 2, 3500) + 2500;
                    }
                } else {
                    rpg_bat_dmg[0] = get_rpg_bat_roll2(base / 2, 3500) + 2500;
                }
                break;
            case 0:
                rpg_bat_dmg[0] = get_rand_mod(base / 1000 + 1) * 1000 + 5000;
                break;
            case 9:
                rpg_bat_dmg[0] = get_rpg_bat_roll2(base / 2, base / 2) + 6000;
                break;
            case 3:
                rpg_bat_dmg[0] = get_rpg_bat_roll2(1500, base) + 3000;
                break;
            case 7:
                rpg_bat_dmg[0] = get_rpg_bat_roll2(base, base + 2000) + 1000;
                break;
            case 10:
                rpg_bat_dmg[0] = get_rpg_bat_roll2(base, base + 1000) + 2000;
                break;
            case 8:
                rpg_bat_dmg[0] = get_rpg_bat_roll2((base + 2000) / 2,
                                           (base + 2000) / 2) + 4000;
                break;
            case 11:
                switch (rpg_bat_hit_cnt) {
                case 0:
                case 1:
                    rpg_bat_hit_cnt = 0;
                    rpg_bat_dmg[0] = get_rpg_bat_roll2(500, 500);
                    break;
                case 2:
                case 3:
                    rpg_bat_hit_cnt = 1;
                    rpg_bat_dmg[0] = get_rpg_bat_roll2(1000, 1000);
                    break;
                case 4:
                case 5:
                    rpg_bat_hit_cnt = 2;
                    rpg_bat_dmg[0] = get_rpg_bat_roll2(1500, 1500);
                    break;
                case 6:
                case 7:
                    rpg_bat_hit_cnt = 3;
                    rpg_bat_dmg[0] = get_rpg_bat_roll2(2000, 2000);
                    break;
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                    rpg_bat_hit_cnt = 4;
                    rpg_bat_dmg[0] = get_rpg_bat_roll2(2500, 2500);
                    break;
                default:
                    rpg_bat_hit_cnt = 5;
                    if (flag) {
                        rpg_bat_dmg[0] = 9999;
                    } else {
                        rpg_bat_unit1.hp -= 30000;
                        rpg_bat_dmg[0] = 9999;
                    }
                    break;
                }
                break;
            }
        } else {
            rpg_bat_reward_roll();
        }
    }
    if (rpg_bat_dmg[0] >= 10000) {
        rpg_bat_dmg[0] = 9999;
    }
    if (rpg_bat_dmg[1] >= 10000) {
        rpg_bat_dmg[1] = 9999;
    }
    if (rpg_bat_dmg[2] >= 10000) {
        rpg_bat_dmg[2] = 9999;
    }
}
