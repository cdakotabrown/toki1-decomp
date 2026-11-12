/* @SymbolStart: 0x801447E0 */
/* @SymbolSize: 924 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_COPYPROP_VAR=-4 IDO52_COPYPROP_SITE=lod:9:deny,lod:10:deny,lod:11:deny,lod:12:deny,lod:13:deny */
/* @PostProc: swap_instructions --offset1 0x164 --offset2 0x16c */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rpg_bat_mode;
extern s32 rpg_bat_dmg;
extern s32 rpg_bat_dmg_rate;
extern s16 para_kon;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void rpg_bat_damage_set()
{
    s32 v;
    s32 lvl;

    if (para_kon >= 151) {
        lvl = 150;
    } else {
        lvl = para_kon;
    }
    if (rpg_bat_mode & 0x80000000) {
        switch (rpg_bat_unit1.flag & 0xFFFF) {
        case 1:
        default:
            v = get_rpg_bat_roll2(rpg_bat_unit0.hp_max / 10, rpg_bat_unit0.hp_max * 5 / 100) + 100;
            break;
        case 2:
            v = get_rpg_bat_roll2(rpg_bat_unit0.hp_max / 10, rpg_bat_unit0.hp_max / 10) + 150;
            break;
        }
        v *= 225 - lvl;
        v /= 150;
        v = rpg_bat_dmg_rate * v / 1000;
        rpg_bat_dmg = v;
        if (rpg_bat_unit0.flag & 0x100) {
            rpg_bat_dmg_rate += 100;
            rpg_bat_dmg = v / 2;
        }
    }
    if ((rpg_bat_mode << 11) < 0) {
        switch (rpg_bat_unit1.flag & 0xFFFF) {
        case 2:
        default:
            v = get_rand_mod(rpg_bat_unit0.hp_max * 25 / 100 - 500) + 500;
            if (rpg_bat_unit0.flag & 0x100) {
                v /= 3;
            }
            break;
        case 4:
            v = get_rand_mod(rpg_bat_unit0.hp_max * 50 / 100 - 1000) + 1000;
            if (rpg_bat_unit0.flag & 0x100) {
                v /= 10;
            }
            break;
        }
        v *= 225 - lvl;
        v /= 150;
        v = rpg_bat_dmg_rate * v;
        rpg_bat_dmg = v / 1000;
    }
    if (rpg_bat_mode & 0xF0000) {
        if (rpg_bat_unit0.flag & 0x100) {
            v = rpg_bat_unit0.hp / 100;
            rpg_bat_dmg = v;
        } else {
            v = rpg_bat_unit0.hp / 10;
            rpg_bat_dmg = v;
        }
    }
    if ((rpg_bat_mode << 4) < 0) {
        if (rpg_bat_unit0.flag & 0x1000) {
            v = rpg_bat_unit0.hp / 2;
            rpg_bat_dmg = v;
        } else {
            if (rpg_bat_unit0.flag & 0x100) {
                v = rpg_bat_unit0.hp / 50;
            rpg_bat_dmg = v;
            } else {
                v = rpg_bat_unit0.hp / 5;
            rpg_bat_dmg = v;
            }
            if (rpg_bat_unit0.hp < 20) {
                rpg_bat_dmg = 0;
            }
        }
    }
}
