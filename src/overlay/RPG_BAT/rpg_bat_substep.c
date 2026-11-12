/* @SymbolStart: 0x8014FDBC */
/* @RodataBase: 0x8015D0B0 */
/* Address: 0x8014FDDC */
/* Size: 836 bytes (209 insns) */
/* @SymbolSize: 868 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 sub_step_up();
extern s32 device_port_init();
extern s32 strcpy();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_slot_adjust();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_act_next();
extern s32 rpg_bat_act_wait();
extern s32 rpg_bat_flag_act();
extern s32 rpg_bat_act_gate();
extern s32 himo_robo_lose_talk();
extern s32 rpg_bat_slide_disp();

extern char rpg_bat_substep_str0[];
extern char rpg_bat_msg_buf4[];
extern char rpg_bat_msg_tbl4[];
extern s32 rpg_bat_result;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;

void rpg_bat_substep(void)
{
    s32 idx;

    class_w[125].prio = 3;
    class_w[126].prio = 3;
    idx = rpg_bat_unit2.step;

switch (idx) {
    case 0:
        rpg_bat_sprite_pos_set(29, 0, 0);
        rpg_bat_anim_set(29, 24, 1, 1);
        rpg_bat_act_next();
        break;
    case 1:
        rpg_bat_act_wait(60);
        break;
    case 2:
        rpg_bat_anim_set(7, 16, 5, 1);
        rpg_bat_act_next();
        break;
    case 3:
        rpg_bat_flag_act(7);
        break;
    case 4:
        rpg_bat_anim_set(7, 17, 1, 1);
        rpg_bat_act_next();
        break;
    case 5:
        rpg_bat_anim_set(6, 18, 1, 1);
        rpg_bat_se_set(1295, 1, 0);
        rpg_bat_act_next();
        break;
    case 6:
        rpg_bat_act_wait(60);
        break;
    case 7:
        rpg_bat_anim_set(6, 19, 1, 1);
        rpg_bat_se_set(1295, 1, 0);
        rpg_bat_act_next();
        break;
    case 8:
        rpg_bat_act_wait(60);
        break;
    case 9:
        rpg_bat_anim_set(6, 20, 1, 1);
        rpg_bat_act_next();
        break;
    case 10:
        rpg_bat_se_set(1294, 15, 1);
        rpg_bat_act_wait(180);
        break;
    case 11:
        rpg_bat_anim_set(6, 255, 1, 0);
        rpg_bat_anim_set(7, 255, 1, 0);
        rpg_bat_anim_set(8, 15, 1, 1);
        rpg_bat_slot_adjust(0, 5, 9999, 1);
        rpg_bat_unit1.hp = 0;
        rpg_bat_act_next();
        break;
    case 12:
        rpg_bat_act_wait(60);
        break;
    case 13:
        rpg_bat_slide_disp();
        break;
    case 14:
        rpg_bat_act_wait(60);
        break;
    case 15:
        strcpy(rpg_bat_msg_buf4, rpg_bat_substep_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl4, 1, 60);
        rpg_bat_anim_set(48, 9, 1, 1);
        device_port_init(1, 514);
        rpg_bat_act_next();
        break;
    case 16:
        rpg_bat_act_gate(0);
        break;
    case 17:
        rpg_bat_act_wait(120);
        break;
    case 18:
        rpg_bat_anim_set(48, 0, 1, 1);
        rpg_bat_anim_set(8, 21, 5, 1);
        rpg_bat_se_set(1296, 1, 0);
        rpg_bat_act_next();
        break;
    case 19:
        rpg_bat_flag_act(8);
        break;
    case 20:
        rpg_bat_act_wait(30);
        break;
    case 21:
        himo_robo_lose_talk();
        break;
    case 22:
        rpg_bat_act_wait(60);
        break;
    case 23:
        rpg_bat_result = 0;
        sub_step_up();
        break;
    default:
        break;
    }
}
