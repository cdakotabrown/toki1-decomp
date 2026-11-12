/* @SymbolStart: 0x8013A650 */
/* @SymbolSize: 828 */
/* @RodataBase: 0x8015BAA8 */
/* rpg_bat_cyber_fang_step - RPG_BAT overlay, Address: 0x8013A658, Size: 820 bytes (205 insns). */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern s32 strcpy();
extern s32 rpg_bat_banner0d();
extern s32 rpg_bat_banner0f();
extern s32 rpg_bat_row_disp();
extern s32 rpg_bat_slide1();
extern s32 rpg_bat_bukatu_exit();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_sprite_bri_set();
extern s32 rpg_bat_pos2_set();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_target_set();
extern s32 rpg_bat_reward_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_clear();
extern s32 rpg_bat_timer_step();
extern s32 rpg_bat_rec_step();
extern s32 rpg_bat_pos_reset();

extern u8 rpg_bat_msg_buf[];
extern u8 rpg_bat_msg_tbl[];
extern u8 rpg_bat_cyber_fang_step_str0[];
extern s32 rpg_bat_target_no;
extern s32 rpg_bat_target_x;
extern s32 rpg_bat_target_y;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_cyber_fang_step(void)
{
    s32 v0;

    switch ((u32)rpg_bat_unit0.step) {
    case 0:
        strcpy(rpg_bat_msg_buf, rpg_bat_cyber_fang_step_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl, 1, 90);
        rpg_bat_pos_reset();
        rpg_bat_target_set();
        rpg_bat_pos2_set(rpg_bat_target_no);
        rpg_bat_reward_set();
        rpg_bat_step_next();
        rpg_bat_step_next();
        break;
    case 2:
        rpg_bat_slide1();
        break;
    case 3:
        rpg_bat_anim_set(52, 3, 5, 1);
        rpg_bat_se_set2(1285, 1, 0);
        rpg_bat_step_next();
        break;
    case 4:
        rpg_bat_rec_step(52);
        break;
    case 5:
        rpg_bat_anim_set(52, 3, 5, 1);
        rpg_bat_step_next();
        break;
    case 6:
        rpg_bat_rec_step(52);
        break;
    case 7:
        rpg_bat_anim_set(52, 3, 5, 1);
        rpg_bat_step_next();
        break;
    case 8:
        rpg_bat_rec_step(52);
        break;
    case 9:
        rpg_bat_anim_set(52, 4, 1, 1);
        rpg_bat_anim_set(47, 5, 1, 1);
        (*(s32 *)&class_w[140].attribute) = 0x71000000;
        (*(s32 *)&class_w[141].attribute) = 0x71000000;
        rpg_bat_anim_set(44, 6, 1, 1);
        rpg_bat_se_set2(1293, 0, 4);
        rpg_bat_step_next();
        break;
    case 10:
        rpg_bat_timer_step(6);
        break;
    case 11:
        rpg_bat_anim_set(45, 7, 1, 1);
        rpg_bat_step_next();
        break;
    case 12:
        rpg_bat_timer_step(56);
        break;
    case 13:
        rpg_bat_banner0d();
        break;
    case 14:
        rpg_bat_timer_step(40);
        break;
    case 15:
        rpg_bat_banner0f();
        break;
    case 16:
        rpg_bat_sprite_pos_set(46, rpg_bat_target_x, rpg_bat_target_y);
        rpg_bat_anim_set2(46, 11, 5, 1);
        rpg_bat_se_set2(1288, 0, 3);
        rpg_bat_step_next();
        break;
    case 17:
        rpg_bat_sprite_bri_set(255);
        v0 = rpg_bat_unit0.cnt + 1;
        rpg_bat_unit0.cnt = v0;
        if (v0 < 5) {
            break;
        }
        rpg_bat_sprite_bri_set(128);
        rpg_bat_step_next();
        break;
    case 18:
        rpg_bat_rec_step(46);
        break;
    case 19:
        rpg_bat_row_disp();
        rpg_bat_anim_set2(46, 255, 1, 0);
        rpg_bat_step_next();
        break;
    case 20:
        rpg_bat_timer_step(20);
        break;
    case 21:
        rpg_bat_bukatu_exit();
        break;
    case 22:
        rpg_bat_clear();
        break;
    }
}
