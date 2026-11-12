/* @SymbolStart: 0x801368B0 */
/* @RodataBase: 0x8015B038 */
/* @SymbolSize: 492 */
/* rpg_bat_step3 - RPG_BAT overlay, Address: 0x801368B8, Size: 484 bytes (121 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern s32 strcpy();
extern s32 rpgbat_slide_step();
extern s32 rpg_bat_shake_step();
extern s32 rpgbat_blink_wait();
extern s32 rpg_bat_row_disp();
extern s32 rpg_bat_slide1();
extern s32 rpg_bat_bukatu_exit();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_pos1_set();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_target_set();
extern s32 rpg_bat_reward_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_clear();
extern s32 rpg_bat_timer_step();
extern s32 rpg_bat_rec_step();

extern char rpg_bat_msg_buf[];
extern char rpg_bat_msg_tbl[];
extern char rpg_bat_step3_str0[];
extern s32 rpg_bat_target_no;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_step3(void)
{
    switch ((u32)rpg_bat_unit0.step) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf, (s32)rpg_bat_step3_str0);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl, 1, 90);
        rpg_bat_target_set();
        rpg_bat_pos1_set(rpg_bat_target_no);
        rpg_bat_reward_set();
        rpg_bat_step_next();
        rpg_bat_step_next();
        break;
    case 1:
        break;
    case 2:
        rpg_bat_slide1();
        break;
    case 3:
        rpg_bat_anim_set(52, 3, 5, 1);
        rpg_bat_step_next();
        break;
    case 4:
        rpg_bat_rec_step(52);
        if (class_w[148].frame == 1)
            rpg_bat_se_set2(1280, 0, 5);
        if (class_w[148].frame == 9)
            rpg_bat_se_set2(1281, 0, 1);
        if (class_w[148].frame == 11)
            rpg_bat_se_set2(1282, 0, 2);
        break;
    case 5:
        rpg_bat_anim_set(52, 2, 1, 1);
        rpg_bat_step_next();
        break;
    case 6:
        rpg_bat_timer_step(60);
        break;
    case 7:
        rpgbat_slide_step();
        break;
    case 8:
        rpg_bat_shake_step();
        break;
    case 9:
        rpgbat_blink_wait();
        break;
    case 10:
        rpg_bat_row_disp();
        rpg_bat_step_next();
        break;
    case 11:
        rpg_bat_timer_step(20);
        break;
    case 12:
        rpg_bat_bukatu_exit();
        break;
    case 13:
        rpg_bat_clear();
        break;
    }
}
