/* @SymbolStart: 0x80144C84 */
/* @SymbolSize: 1276 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s16 para_kon;
extern s32 rpg_bat_target_no;
extern s32 rpg_bat_dmg;
typedef struct {
    s32 kind[3];
    s32 gain;
    s32 unk10;
    s32 w0;
    s32 unk18[6];
    s32 w1;
    s32 w2;
    s32 w3;
    s32 w4;
} Bat;
extern Bat rpg_bat_toriko_flag;
extern s32 rpg_bat_hit_cnt;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit2;

get_rpg_bat_act()
{
    s32 p1;
    s32 p2;
    s32 c;
    s32 r;

    if (para_kon > 150) {
        c = 150;
    } else {
        c = para_kon;
    }

    switch (get_rand_mod(3)) {
    case 0:
        rpg_bat_target_no = 0;
        break;
    case 1:
        rpg_bat_target_no = 1;
        break;
    case 2:
        rpg_bat_target_no = 2;
        break;
    }

    if (rpg_bat_flag_check()) {
        if (rpg_bat_toriko_flag.w4 + rpg_bat_toriko_flag.w1 + rpg_bat_toriko_flag.w2 +
            rpg_bat_toriko_flag.w3 < 21) {
            rpg_bat_toriko_flag.w1 = 15;
            rpg_bat_toriko_flag.w2 = 15;
            rpg_bat_toriko_flag.w3 = 5;
            rpg_bat_toriko_flag.w4 = 30;
        }
    } else {
        if (rpg_bat_toriko_flag.w4 + rpg_bat_toriko_flag.w1 + rpg_bat_toriko_flag.w2 +
            rpg_bat_toriko_flag.w3 < 21) {
            rpg_bat_toriko_flag.w1 = 10;
            rpg_bat_toriko_flag.w2 = 10;
            rpg_bat_toriko_flag.w3 = 0;
            rpg_bat_toriko_flag.w4 = 40;
        }
    }

    r = get_rand_mod(rpg_bat_toriko_flag.w0 + rpg_bat_toriko_flag.w1 + rpg_bat_toriko_flag.w2 +
                      rpg_bat_toriko_flag.w3 + rpg_bat_toriko_flag.w4);

    if (r < rpg_bat_toriko_flag.w1) {
        rpg_bat_toriko_flag.w1 -= 5;
        return 1;
    }
    if (r < rpg_bat_toriko_flag.w2 + rpg_bat_toriko_flag.w1) {
        rpg_bat_toriko_flag.w2 -= 5; return 0;
    }
    if (r < rpg_bat_toriko_flag.w0 + rpg_bat_toriko_flag.w1 + rpg_bat_toriko_flag.w2 +
            rpg_bat_toriko_flag.w3) {
        if (rpg_bat_toriko_flag.kind[rpg_bat_target_no] == 1) {
            rpg_bat_dmg = get_rpg_bat_roll2(100, 100);
            return 4;
        }
        rpg_bat_toriko_flag.w3 -= 5;
        rpg_bat_toriko_flag.w0 -= 20;
        if (rpg_bat_toriko_flag.w3 < 0) {
            rpg_bat_toriko_flag.w3 = 0;
        }
        if (rpg_bat_toriko_flag.w0 < 0) {
            rpg_bat_toriko_flag.w0 = 0;
        }
        return 3;
    }

    rpg_bat_toriko_flag.w4 -= 5;
    if (rpg_bat_flag_check() == 0) {
        rpg_bat_toriko_flag.gain += 25;
    }
    if (rpg_bat_toriko_flag.kind[rpg_bat_target_no] == 1) {
        rpg_bat_dmg = get_rpg_bat_roll2(100, 100);
        return 4;
    }
    if (rpg_bat_unit2.flag != 0x40000000 && get_rand_mod(100) < 5) {
        rpg_bat_dmg = rpg_bat_unit2.hp / 10;
        if (rpg_bat_unit0.flag & 0x100) {
            rpg_bat_dmg = rpg_bat_dmg / 10;
        }
        rpg_bat_hit_cnt += 2;
        return 6;
    }
    if (rpg_bat_unit0.flag != 256 && get_rand_mod(100) < 5) {
        rpg_bat_dmg = get_rpg_bat_roll2(10, 10);
        rpg_bat_dmg = rpg_bat_toriko_flag.gain * rpg_bat_dmg / 1000;
        rpg_bat_hit_cnt += 1;
        return 5;
    }
    rpg_bat_dmg = get_rpg_bat_roll2(125, 75) * (225 - c) / 150;
    rpg_bat_dmg = rpg_bat_toriko_flag.gain * rpg_bat_dmg / 1000;
    if (rpg_bat_unit0.flag & 0x100) {
        rpg_bat_dmg = rpg_bat_dmg / 10;
    }
    rpg_bat_hit_cnt += 1;
    return 2;
}
