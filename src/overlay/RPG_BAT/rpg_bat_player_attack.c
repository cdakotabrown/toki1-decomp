/* @SymbolStart: 0x8013AF1C */
/* @SymbolSize: 980 */
/* @RodataBase: 0x8015BB68 */
/* rpg_bat_player_attack - RPG_BAT overlay, Address: 0x8013AF24, Size: 972 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern void strcpy(u8 *, u8 *);
extern void strcat(u8 *, u8 *);
extern void rpg_bat_target_set(void);
extern void rpg_bat_reward_set(void);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_slot_adjust(s32, s32, s32, s32);
extern void rpg_bat_pos2_set(s32);
extern void rpg_bat_hit_step(void);
extern void rpg_bat_miss_step(void);
extern void rpg_bat_msg_set(u8 *, s32, s32);
extern void suuzi_str(u8 *, u8 *, u8 *, s32, u8 *);
extern void rpg_bat_step_next(void);
extern void rpg_bat_clear(void);
extern void rpg_bat_timer_step(s32);
extern void rpg_bat_step_gate(s32);

extern u8  p_sei[];
extern u8  rpg_bat_player_attack_str0[];
extern u8  rpg_bat_player_attack_str1[];
extern u8  rpg_bat_player_attack_str2[];
extern u8  rpg_bat_player_attack_str3[];
extern u8  rpg_bat_msg_buf[];
extern u8  rpg_bat_msg_buf_1[];
extern u8  rpg_bat_msg_tbl[];
extern s32 rpg_bat_target_no;
extern s32 rpg_bat_dmg;
extern s32 rpg_bat_target_x;
extern s32 rpg_bat_target_y;
extern s32 rpg_bat_seq_work;
extern s32 rpg_bat_msg_ctl_mode;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_player_attack(void)
{
    s32 v;

    switch (rpg_bat_unit0.step) {
    case 0:
        rpg_bat_target_set();
        rpg_bat_pos2_set(rpg_bat_target_no);
        rpg_bat_reward_set();
        rpg_bat_anim_set(48, 1, 1, 1);
        rpg_bat_step_next();
        break;
    case 1:
        rpg_bat_sprite_pos_set(48, -rpg_bat_unit0.cnt, 0);
        rpg_bat_unit0.cnt = rpg_bat_unit0.cnt + 3;
        if (rpg_bat_unit0.cnt < 33) {
            break;
        }
        rpg_bat_sprite_pos_set(48, -32, 0);
        rpg_bat_anim_set(48, 0, 1, 1);
        rpg_bat_step_next();
        break;
    case 2:
        strcpy(rpg_bat_msg_buf, p_sei);
        strcat(rpg_bat_msg_buf, rpg_bat_player_attack_str0);
        if (rpg_bat_dmg == 0) {
            strcpy(rpg_bat_msg_buf_1, rpg_bat_msg_buf);
            strcat(rpg_bat_msg_buf_1, rpg_bat_player_attack_str1);
        } else {
            suuzi_str(rpg_bat_msg_buf_1, p_sei, rpg_bat_player_attack_str2, rpg_bat_dmg,
                          rpg_bat_player_attack_str3);
        }
        rpg_bat_msg_set(rpg_bat_msg_tbl, 2, 60);
        rpg_bat_msg_ctl_mode = 3;
        rpg_bat_sprite_pos_set(49, rpg_bat_target_x, rpg_bat_target_y);
        rpg_bat_step_next();
        break;
    case 3:
        rpg_bat_timer_step(25);
        break;
    case 4:
        v = rpg_bat_dmg;
        if (v < 200) {
            rpg_bat_seq_work = 1;
        } else if (v < 500) {
            rpg_bat_seq_work = 2;
        } else if (v < 800) {
            rpg_bat_seq_work = 3;
        } else if (v < 1100) {
            rpg_bat_seq_work = 4;
        } else if (v < 1300) {
            rpg_bat_seq_work = 5;
        } else if (v == 9999) {
            rpg_bat_seq_work = 10;
        } else {
            rpg_bat_seq_work = 6;
        }
        class_w[144].rate = 130;
        class_w[145].rate = 130;
        rpg_bat_step_next();
        break;
    case 5:
        if (rpg_bat_dmg == 0) {
            rpg_bat_miss_step();
        } else {
            rpg_bat_hit_step();
        }
        break;
    case 6:
        rpg_bat_msg_ctl_mode = 1;
        class_w[144].rate = 1;
        class_w[145].rate = 1;
        rpg_bat_step_gate(1);
        break;
    case 7:
        rpg_bat_slot_adjust(0, rpg_bat_target_no, rpg_bat_dmg, 1);
        rpg_bat_anim_set(48, 2, 1, 1);
        rpg_bat_step_next();
        break;
    case 8:
        rpg_bat_sprite_pos_set(48, rpg_bat_unit0.cnt - 32, 0);
        rpg_bat_unit0.cnt = rpg_bat_unit0.cnt + 3;
        if (rpg_bat_unit0.cnt < 33) {
            break;
        }
        rpg_bat_sprite_pos_set(48, 0, 0);
        rpg_bat_anim_set(48, 0, 1, 1);
        rpg_bat_step_next();
        break;
    case 9:
        rpg_bat_step_gate(0);
        break;
    case 10:
        rpg_bat_timer_step(20);
        break;
    case 11:
        rpg_bat_clear();
        break;
    }
}
