/* @SymbolStart: 0x8014A660 */
/* @SymbolSize: 1316 */
/* @RodataBase: 0x8015CD64 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=42:v1 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 rpg_bat_target_no;
extern s32 rpg_bat_seq_work[];
extern u8 rpg_bat_msg_buf[];
extern u8 rpg_bat_msg_tbl[];
extern u8 rpg_bat_bukatu9_attack_str0[];
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

extern void strcpy(u8 *, u8 *);
extern void rpg_bat_row_disp(void);
extern void rpg_bat_slide1(void);
extern void rpg_bat_bukatu_exit(void);
extern void rpg_bat_effect_rec_set(s32);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_pos1_set(s32);
extern void rpg_bat_target_set(void);
extern void rpg_bat_reward_set(void);
extern void rpg_bat_icon_set(s32, s32, s32);
extern void rpg_bat_icon_disp(void);
extern void rpg_bat_icon_float(s32, s32);
extern void rpg_bat_msg_set(u8 *, s32, s32);
extern void rpg_bat_step_next(void);
extern void rpg_bat_clear(void);
extern void rpg_bat_timer_step(s32);

void rpg_bat_bukatu9_attack(void)
{
    switch ((u32)rpg_bat_unit0.step) {
    case 0:
        strcpy(rpg_bat_msg_buf, rpg_bat_bukatu9_attack_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl, 1, 90);
        rpg_bat_target_set();
        rpg_bat_pos1_set(rpg_bat_target_no);
        rpg_bat_reward_set();
        rpg_bat_effect_rec_set(3);
        rpg_bat_seq_work[0] = 0;
        rpg_bat_seq_work[1] = 0;
        rpg_bat_seq_work[2] = 0;
        rpg_bat_seq_work[3] = 0;
        rpg_bat_seq_work[30] = 0;
        rpg_bat_seq_work[31] = 0;
        rpg_bat_seq_work[32] = 0;
        rpg_bat_seq_work[33] = 0;
        rpg_bat_step_next();
        rpg_bat_step_next();
        break;
    case 2:
        rpg_bat_slide1();
        break;
    case 3:
        rpg_bat_anim_set(52, 3, 1, 1);
        rpg_bat_sprite_pos_set(51, 208, 112);
        rpg_bat_anim_set2(51, 5, 1, 1);
        rpg_bat_step_next();
        break;
    case 4:
        rpg_bat_timer_step(108);
        break;
    case 5:
        rpg_bat_icon_set(0, 208, 112);
        rpg_bat_anim_set2(51, 5, 1, 0);
        rpg_bat_anim_set(52, 4, 1, 1);
        switch ((u32)rpg_bat_target_no) {
        case 0:
            class_w[157].prio = 4;
            break;
        case 1:
            class_w[155].prio = 4;
            class_w[157].prio = 4;
            break;
        }
        rpg_bat_step_next();
        break;
    case 6:
        rpg_bat_unit0.cnt++;
        switch ((u32)rpg_bat_target_no) {
        case 0:
            switch (rpg_bat_unit0.cnt) {
            case 9:
                rpg_bat_icon_set(1, 179, 120);
                break;
            case 18:
                rpg_bat_step_next();
                break;
            }
            break;
        case 1:
            switch (rpg_bat_unit0.cnt) {
            case 9:
                rpg_bat_icon_set(1, 180, 113);
                break;
            case 18:
                rpg_bat_icon_set(2, 152, 114);
                break;
            case 27:
                rpg_bat_icon_set(3, 124, 115);
                break;
            case 36:
                rpg_bat_step_next();
                break;
            }
            break;
        case 2:
            switch (rpg_bat_unit0.cnt) {
            case 9:
                rpg_bat_icon_set(1, 179, 122);
                break;
            case 18:
                rpg_bat_icon_set(2, 150, 132);
                break;
            case 27:
                rpg_bat_icon_set(3, 121, 141);
                break;
            case 36:
                rpg_bat_step_next();
                break;
            }
            break;
        default:
            switch (rpg_bat_unit0.cnt) {
            case 9:
                rpg_bat_icon_set(1, 180, 122);
                break;
            case 18:
                rpg_bat_icon_set(2, 152, 132);
                break;
            case 27:
                rpg_bat_icon_set(3, 124, 141);
                break;
            case 36:
                rpg_bat_step_next();
                break;
            }
            break;
        }
        rpg_bat_icon_disp();
        break;
    case 7:
        rpg_bat_icon_disp();
        switch ((u32)rpg_bat_target_no) {
        case 0:
            rpg_bat_icon_float(120, 130);
            break;
        case 1:
            rpg_bat_icon_float(64, 114);
            break;
        case 2:
            rpg_bat_icon_float(60, 162);
            break;
        default:
            rpg_bat_icon_float(64, 162);
            break;
        }
        break;
    case 8:
        rpg_bat_step_next();
        break;
    case 9:
        rpg_bat_effect_rec_set(255);
        rpg_bat_row_disp();
        rpg_bat_step_next();
        break;
    case 10:
        rpg_bat_timer_step(20);
        break;
    case 11:
        rpg_bat_bukatu_exit();
        break;
    case 12:
        rpg_bat_clear();
        break;
    }
}
