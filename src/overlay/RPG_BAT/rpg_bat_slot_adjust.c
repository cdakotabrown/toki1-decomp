/* @SymbolSize: 1100 */
/* @RodataBase: 0x8015BDC0 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0x140 --offset2 0x148 */
/* @PostProc: swap_instructions --offset1 0x144 --offset2 0x148 */
/* @PostProc: swap_instructions --offset1 0x14c --offset2 0x150 */
/* rpg_bat_slot_adjust - RPG_BAT overlay, Address: 0x8013EA60, Size: 1100 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern void rpg_bat_score_disp(s32, s32, s32, s32, s32);
extern void rpg_bat_slot_set(s32, s32, s32);

extern s32 rpg_bat_enemy_hp[];
extern s32 rpg_bat_toriko_flag[];
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void rpg_bat_slot_adjust(s32 arg0, s32 sel, s32 amt, s32 arg3)
{
    switch (sel) {
    case 0:
        class_w[155].disp_flag = class_w[155].disp_flag & 0xFFFE;
        rpg_bat_toriko_flag[0] = 0;
        rpg_bat_enemy_hp[0] = rpg_bat_enemy_hp[0] - amt;
        if (rpg_bat_enemy_hp[0] < 0) {
            rpg_bat_enemy_hp[0] = 0;
        }
        if (amt == 0) {
            rpg_bat_slot_set(0, 108, 119);
        } else {
            rpg_bat_score_disp(arg0, 116, 119, amt, arg3);
        }
        break;
    case 1:
        class_w[156].disp_flag = class_w[156].disp_flag & 0xFFFE;
        rpg_bat_toriko_flag[1] = 0;
        rpg_bat_enemy_hp[1] = rpg_bat_enemy_hp[1] - amt;
        if (rpg_bat_enemy_hp[1] < 0) {
            rpg_bat_enemy_hp[1] = 0;
        }
        if (amt == 0) {
            rpg_bat_slot_set(0, 51, 103);
        } else {
            rpg_bat_score_disp(arg0, 59, 103, amt, arg3);
        }
        break;
    case 2:
        class_w[157].disp_flag = class_w[157].disp_flag & 0xFFFE;
        rpg_bat_toriko_flag[2] = 0;
        rpg_bat_enemy_hp[2] = rpg_bat_enemy_hp[2] - amt;
        if (rpg_bat_enemy_hp[2] < 0) {
            rpg_bat_enemy_hp[2] = 0;
        }
        if (amt == 0) {
            rpg_bat_slot_set(0, 48, 151);
        } else {
            rpg_bat_score_disp(arg0, 52, 151, amt, arg3);
        }
        break;
    case 3:
        rpg_bat_unit1.hp = rpg_bat_unit1.hp - amt;
        if (rpg_bat_unit1.hp < 0) {
            rpg_bat_unit1.hp = 0;
        }
        if (amt == 0) {
            rpg_bat_slot_set(0, 56, 152);
        } else {
            rpg_bat_score_disp(arg0, 64, 152, amt, arg3);
        }
        break;
    case 4:
        rpg_bat_unit1.hp = rpg_bat_unit1.hp - amt;
        if (rpg_bat_unit1.hp < 0) {
            rpg_bat_unit1.hp = 0;
        }
        if (amt == 0) {
            rpg_bat_slot_set(0, 56, 152);
        } else {
            rpg_bat_score_disp(arg0, 64, 152, amt, arg3);
        }
        break;
    case 5:
        rpg_bat_unit1.hp = rpg_bat_unit1.hp - amt;
        if (rpg_bat_unit1.hp < 0) {
            rpg_bat_unit1.hp = 0;
        }
        if (amt == 0) {
            rpg_bat_slot_set(0, 56, 149);
        } else {
            rpg_bat_score_disp(arg0, 64, 149, amt, arg3);
        }
        break;
    case 16:
        if (amt == 0) {
            rpg_bat_slot_set(0, 64, 151);
        } else {
            rpg_bat_score_disp(arg0, 72, 151, amt, arg3);
        }
        break;
    case 17:
        if (amt == 0) {
            rpg_bat_slot_set(0, 64, 133);
        } else {
            rpg_bat_score_disp(arg0, 72, 133, amt, arg3);
        }
        break;
    case 18:
        if (amt == 0) {
            rpg_bat_slot_set(0, 72, 141);
        } else {
            rpg_bat_score_disp(arg0, 80, 141, amt, arg3);
        }
        break;
    case 32:
        rpg_bat_unit0.hp = rpg_bat_unit0.hp - amt;
        if (rpg_bat_unit0.hp < 0) {
            rpg_bat_unit0.hp = 0;
        }
        rpg_bat_score_disp(arg0, 244, 150, amt, arg3);
        break;
    case 33:
        if (arg3 == 1) {
            rpg_bat_unit0.hp = rpg_bat_unit0.hp - amt;
            if (rpg_bat_unit0.hp < 0) {
                rpg_bat_unit0.hp = 0;
            }
        } else {
            rpg_bat_unit0.hp = rpg_bat_unit0.hp + amt;
            if (rpg_bat_unit0.hp_max < rpg_bat_unit0.hp) {
                rpg_bat_unit0.hp = rpg_bat_unit0.hp_max;
            }
        }
        rpg_bat_score_disp(arg0, 268, 102, amt, arg3);
        break;
    case 35:
        rpg_bat_score_disp(arg0, 276, 150, amt, arg3);
        break;
    }
}
