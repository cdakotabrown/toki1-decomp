/* @SymbolStart: 0x801421E0 */
/* @SymbolSize: 868 */
/* @RodataBase: 0x8015C318 */
/* rpg_bat_state - RPG_BAT overlay, Address: 0x801421E8, Size: 860 bytes (215 insns). */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern s32 strcpy();
extern s32 rpg_bat_blink_step();
extern s32 rpg_bat_blink_step2();
extern s32 rpg_bat_rows_disp();
extern s32 rpg_bat_slide1();
extern s32 rpg_bat_bukatu_exit();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_pos2_set();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_zoom();
extern s32 rpg_bat_bukatu2_drop_step();
extern s32 rpg_bat_step0d();
extern s32 rpg_bat_step10();
extern s32 rpg_bat_step14();
extern s32 rpg_bat_step17();
extern s32 rpg_bat_target_set();
extern s32 rpg_bat_reward_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_clear();
extern s32 rpg_bat_timer_step();
extern s32 rpg_bat_rec_step();
extern s32 rpg_bat_step_gate();
extern s32 rpg_bat_pos_reset();

extern u8 rpg_bat_msg_buf[];
extern u8 rpg_bat_msg_tbl[];
extern u8 rpg_bat_state_str0[];
extern u8 rpg_bat_state_str1[];
extern u8 rpg_bat_state_str2[];
extern u8 rpg_bat_state_str3[];
extern s32 rpg_bat_target_no;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_state(void)
{
    s32 v0;

    switch ((u32)rpg_bat_unit0.step) {
    case 0:
        strcpy(rpg_bat_msg_buf, rpg_bat_state_str0);
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
        rpg_bat_step_next();
        break;
    case 4:
        if ((class_w[148].anim_flag & 1) != 0) {
            return;
        }
        rpg_bat_anim_set(52, 3, 5, 1);
        v0 = rpg_bat_unit0.cnt + 1;
        rpg_bat_unit0.cnt = v0;
        if (v0 < 5) {
            break;
        }
        rpg_bat_step_next();
        break;
    case 5:
        rpg_bat_anim_set(52, 2, 1, 1);
        rpg_bat_anim_set(43, 17, 5, 1);
        rpg_bat_se_set2(1280, 1, 0);
        rpg_bat_step_next();
        break;
    case 6:
        rpg_bat_rec_step(43);
        break;
    case 7:
        rpg_bat_zoom();
        break;
    case 8:
        rpg_bat_blink_step();
        break;
    case 9:
        rpg_bat_timer_step(30);
        break;
    case 10:
        rpg_bat_bukatu2_drop_step();
        break;
    case 11:
        strcpy(rpg_bat_msg_buf, rpg_bat_state_str1);
        rpg_bat_msg_set(rpg_bat_msg_tbl, 1, 60);
        rpg_bat_step_next();
        break;
    case 12:
        rpg_bat_step_gate(1);
        break;
    case 13:
        rpg_bat_step0d();
        break;
    case 14:
        strcpy(rpg_bat_msg_buf, rpg_bat_state_str2);
        rpg_bat_msg_set(rpg_bat_msg_tbl, 1, 60);
        rpg_bat_step_next();
        break;
    case 15:
        rpg_bat_step_gate(1);
        break;
    case 16:
        rpg_bat_step10();
        break;
    case 17:
        rpg_bat_timer_step(30);
        break;
    case 18:
        strcpy(rpg_bat_msg_buf, rpg_bat_state_str3);
        rpg_bat_msg_set(rpg_bat_msg_tbl, 1, 60);
        rpg_bat_step_next();
        break;
    case 19:
        rpg_bat_step_gate(0);
        break;
    case 20:
        rpg_bat_step14();
        break;
    case 21:
        rpg_bat_rows_disp();
        rpg_bat_step_next();
        break;
    case 22:
        rpg_bat_timer_step(40);
        break;
    case 23:
        rpg_bat_step17();
        break;
    case 24:
        rpg_bat_timer_step(30);
        break;
    case 25:
        rpg_bat_blink_step2();
        break;
    case 26:
        rpg_bat_timer_step(20);
        break;
    case 27:
        rpg_bat_bukatu_exit();
        break;
    case 28:
        rpg_bat_clear();
        break;
    }
}
