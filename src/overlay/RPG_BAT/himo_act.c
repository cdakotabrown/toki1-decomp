/* @SymbolStart: 0x801481C8 */
/* @RodataBase: 0x8015CAE0 */
/* @SymbolSize: 780 */
/* himo_act - RPG_BAT overlay, Address: 0x801481D0, Size: 772 bytes (193 insns). */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern s32 rpg_bat_slide_out();
extern s32 rpg_bat_slide_in();
extern s32 rpg_bat_sprite_move();
extern s32 rpg_bat_target_set();
extern s32 rpg_bat_param_set();
extern s32 rpg_bat_talk1b();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_slot_adjust();
extern s32 rpg_bat_pos2_set();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_act_next();
extern s32 rpg_bat_reset();
extern s32 rpg_bat_act_wait();
extern s32 rpg_bat_flag_act();

extern s32 rpg_bat_target_no;
extern s32 rpg_bat_dmg;
extern s32 rpg_bat_target_x;
extern s32 rpg_bat_target_y;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;

void himo_act(void)
{
    switch ((u32)rpg_bat_unit2.step) {
    case 0:
        rpg_bat_slide_out(9, 0);
        break;
    case 1:
        rpg_bat_anim_set(29, 3, 1, 1);
        rpg_bat_sprite_pos_set(30, 216, 136);
        rpg_bat_anim_set2(30, 6, 5, 1);
        rpg_bat_target_set();
        rpg_bat_pos2_set(rpg_bat_target_no);
        rpg_bat_param_set();
        rpg_bat_se_set(1294, 1, 0);
        rpg_bat_act_next();
        break;
    case 2:
        rpg_bat_flag_act(30);
        break;
    case 3:
        rpg_bat_anim_set2(30, 7, 1, 1);
        rpg_bat_act_next();
        break;
    case 4:
        rpg_bat_talk1b();
        break;
    case 5:
        rpg_bat_anim_set(29, 4, 5, 1);
        rpg_bat_act_next();
        break;
    case 6:
        rpg_bat_flag_act(29);
        break;
    case 7:
        rpg_bat_anim_set(29, 5, 1, 1);
        rpg_bat_se_set(1295, 1, 0);
        rpg_bat_act_next();
        break;
    case 8:
        if (rpg_bat_target_no == 0) {
            rpg_bat_sprite_move(30, 216, 136, rpg_bat_target_x, rpg_bat_target_y, 60);
        } else {
            rpg_bat_sprite_move(30, 216, 136, rpg_bat_target_x, rpg_bat_target_y, 40);
        }
        break;
    case 9:
        rpg_bat_act_wait(30);
        break;
    case 10:
        if ((rpg_bat_unit1.flag & 0x100000) != 0) {
            rpg_bat_sprite_pos_set(30, rpg_bat_target_x, rpg_bat_target_y - 48);
        } else {
            rpg_bat_sprite_pos_set(30, rpg_bat_target_x - 4, 94);
        }
        rpg_bat_anim_set2(30, 8, 5, 1);
        rpg_bat_se_set(1296, 1, 0);
        rpg_bat_act_next();
        break;
    case 11:
        rpg_bat_flag_act(30);
        break;
    case 12:
        rpg_bat_anim_set(30, 255, 1, 0);
        rpg_bat_slot_adjust(0, rpg_bat_target_no, rpg_bat_dmg, 1);
        rpg_bat_act_next();
        break;
    case 13:
        rpg_bat_act_wait(60);
        break;
    case 14:
        rpg_bat_slide_in(1, 0);
        break;
    case 15:
        rpg_bat_act_wait(20);
        break;
    case 16:
        rpg_bat_reset();
        break;
    }
}
