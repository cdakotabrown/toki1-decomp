/* @SymbolStart: 0x8014DF60 */
/* @SymbolSize: 580 */
/* @RodataBase: 0x8015CF48 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern void kagami_target_set(void);
extern void rpg_bat_act_next(void);
extern void rpg_bat_slide_out(s32, s32);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_se_set(s32, s32, s32);
extern void rpg_bat_flag_act(s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern void rpg_bat_cam_orbit(void);
extern void kagami_toriko_talk(void);
extern void kagami_miss_talk(void);
extern s32  get_CD_State(void);
extern void rpg_bat_act_gate(s32);
extern void rpg_bat_act_wait(s32);
extern void rpg_bat_slide_in(s32, s32);
extern void rpg_bat_reset(void);

extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit2;

void rpg_bat_sub_step2(void)
{
    switch (rpg_bat_unit2.step) {
    case 0:
        kagami_target_set();
        rpg_bat_act_next();
        break;
    case 1:
        rpg_bat_slide_out(1, 0);
        break;
    case 2:
        class_w[126].prio = 4;
        rpg_bat_anim_set(29, 5, 5, 1);
        rpg_bat_act_next();
        break;
    case 3:
        if (class_w[125].frame == 5)
            rpg_bat_se_set(1294, 0, 4);
        rpg_bat_flag_act(29);
        break;
    case 4:
        rpg_bat_sprite_pos_set(30, 224, 128);
        rpg_bat_anim_set2(30, 7, 5, 1);
        rpg_bat_act_next();
        break;
    case 5:
        rpg_bat_flag_act(30);
        break;
    case 6:
        rpg_bat_anim_set2(30, 8, 1, 1);
        rpg_bat_act_next();
        break;
    case 7:
        rpg_bat_cam_orbit();
        break;
    case 8:
        rpg_bat_anim_set2(30, 9, 5, 1);
        rpg_bat_se_set(1296, 1, 2);
        rpg_bat_act_next();
        break;
    case 9:
        rpg_bat_flag_act(30);
        break;
    case 10:
        rpg_bat_anim_set2(30, 255, 1, 0);
        if (rpg_bat_unit2.flag == 1)
            kagami_toriko_talk();
        else
            kagami_miss_talk();
        break;
    case 11:
        if (get_CD_State() == 2) {
            rpg_bat_anim_set(29, 0, 1, 1);
        }
        rpg_bat_act_gate(1);
        break;
    case 12:
        rpg_bat_act_wait(20);
        break;
    case 13:
        rpg_bat_slide_in(3, 0);
        break;
    case 14:
        rpg_bat_act_wait(20);
        break;
    case 15:
        rpg_bat_reset();
        break;
    }
}
