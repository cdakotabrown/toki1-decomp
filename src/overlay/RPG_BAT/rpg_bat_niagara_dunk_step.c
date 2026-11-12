/* @SymbolStart: 0x801321D0 */
/* @RodataBase: 0x8015AA08 */
/* @SymbolSize: 376 */
/* rpg_bat_niagara_dunk_step - RPG_BAT overlay, Address: 0x801321D8, Size: 368 bytes (92 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 strcpy();
extern s32 rpg_bat_sweep();
extern s32 rpg_bat_pair_slide_out();
extern s32 rpg_bat_pair_cross();
extern s32 rpg_bat_pair_slide_in();
extern s32 rpg_bat_slide1();
extern s32 rpg_bat_bukatu_exit();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_pos1_set();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_target_set();
extern s32 rpg_bat_reward_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_clear();
extern s32 rpg_bat_timer_step();

extern s32 rpg_bat_target_no;
extern char rpg_bat_msg_buf[];
extern char rpg_bat_msg_tbl[];
extern char rpg_bat_niagara_dunk_step_str0[];
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_niagara_dunk_step(void)
{
    if (rpg_bat_unit0.step >= 3 && rpg_bat_unit0.step < 6) {
        if (class_w[148].frame == 0 && class_w[148].tick == 0) {
            rpg_bat_se_set2(1283, 1, 0);
            rpg_bat_unit0.step = rpg_bat_unit0.step;
        }
    }

    switch ((u32)rpg_bat_unit0.step) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf, (s32)rpg_bat_niagara_dunk_step_str0);
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
        rpg_bat_pair_slide_out();
        break;
    case 4:
        rpg_bat_pair_cross();
        break;
    case 5:
        rpg_bat_pair_slide_in();
        break;
    case 6:
        rpg_bat_sweep();
        break;
    case 7:
        rpg_bat_anim_set2(52, 2, 1, 1);
        rpg_bat_timer_step(20);
        break;
    case 8:
        rpg_bat_bukatu_exit();
        break;
    case 9:
        rpg_bat_clear();
        break;
    }
}
