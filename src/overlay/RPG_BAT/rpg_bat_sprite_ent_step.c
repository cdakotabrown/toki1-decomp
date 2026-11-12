/* @RodataBase: 0x8015B088 */
/* rpg_bat_sprite_ent_step - RPG_BAT overlay, Address: 0x80136E2C */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 rpg_bat_dmg;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit2;

extern void rpg_bat_nijino_talk_step(void);
extern void rpg_bat_sprite_enter(s32, s32);
extern void rpg_bat_entry(void);
extern void rpg_bat_effect_rec_set(s32);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_slot_adjust(s32, s32, s32, s32);
extern void rpg_bat_se_set(s32, s32, s32);
extern void rpg_bat_sprite_move(s32, s32, s32, s32, s32, s32);
extern void rpg_bat_param_set(void);
extern void rpg_bat_act_next(void);
extern void rpg_bat_reset(void);
extern void rpg_bat_act_wait(s32);
extern void rpg_bat_flag_act(s32);

void rpg_bat_sprite_ent_step(void)
{
    rpg_bat_entry();

    switch ((u32)rpg_bat_unit2.step) {
    case 0:
        rpg_bat_sprite_move(29, 0, 0, -32, -48, 100);
        return;
    case 1:
        rpg_bat_anim_set(29, 5, 5, 1);
        rpg_bat_act_next();
        return;
    case 2:
        if (class_w[125].frame == 8) {
            rpg_bat_effect_rec_set(5);
            rpg_bat_param_set();
            rpg_bat_act_next();
        }
        return;
    case 3:
        rpg_bat_nijino_talk_step();
        return;
    case 4:
        rpg_bat_flag_act(29);
        return;
    case 5:
        switch ((u32)rpg_bat_unit2.cnt) {
        case 0:
            rpg_bat_se_set(1297, 1, 0);
            rpg_bat_sprite_enter(4, 6);
            break;
        case 1:
            rpg_bat_sprite_enter(5, 7);
            break;
        case 5:
            rpg_bat_sprite_enter(6, 7);
            break;
        case 6:
            rpg_bat_sprite_enter(7, 6);
            break;
        case 9:
            rpg_bat_sprite_enter(8, 7);
            break;
        case 13:
            rpg_bat_sprite_enter(9, 7);
            break;
        case 14:
            rpg_bat_sprite_enter(10, 6);
            break;
        case 20:
            rpg_bat_sprite_enter(11, 7);
            break;
        case 22:
            rpg_bat_sprite_enter(12, 7);
            break;
        case 26:
            rpg_bat_sprite_enter(13, 6);
            break;
        case 27:
            rpg_bat_sprite_enter(14, 7);
            break;
        case 32:
            rpg_bat_sprite_enter(15, 7);
            break;
        case 40:
            rpg_bat_act_next();
            break;
        }
        rpg_bat_unit2.cnt = rpg_bat_unit2.cnt + 1;
        return;
    case 6:
        rpg_bat_slot_adjust(0, 33, rpg_bat_dmg, 0);
        rpg_bat_se_set(1282, 1, 0);
        rpg_bat_act_next();
        return;
    case 7:
        rpg_bat_act_wait(40);
        return;
    case 8:
        rpg_bat_anim_set(29, 2, 1, 1);
        rpg_bat_act_next();
        return;
    case 9:
        rpg_bat_sprite_move(29, -32, -48, 0, 0, 100);
        return;
    case 10:
        rpg_bat_anim_set(29, 0, 1, 1);
        rpg_bat_effect_rec_set(255);
        rpg_bat_act_next();
        return;
    case 11:
        rpg_bat_act_wait(20);
        return;
    case 12:
        rpg_bat_reset();
        return;
    }
}
