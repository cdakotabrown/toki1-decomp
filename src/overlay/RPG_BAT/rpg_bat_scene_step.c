/* @SymbolStart: 0x801498C0 */
/* @RodataBase: 0x8015CC98 */
/* @SymbolSize: 488 */
/* rpg_bat_scene_step - RPG_BAT overlay, Address: 0x801498C8, Size: 480 bytes (120 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern s32 strcpy();
extern s32 rpg_bat_row_disp();
extern s32 rpg_bat_slide1();
extern s32 rpg_bat_bukatu_exit();
extern s32 rpg_bat_pos2_set();
extern s32 rpg_bat_target_set();
extern s32 rpg_bat_reward_set();
extern s32 rpg_bat_effect_step6();
extern s32 rpg_bat_bukatu11_fly_step();
extern s32 rpg_bat_bukatu11_lineup_step();
extern s32 rpg_bat_bukatu11_count_step();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_clear();
extern s32 rpg_bat_pos_reset();

extern char rpg_bat_msg_buf[];
extern char rpg_bat_msg_tbl[];
extern char rpg_bat_scene_step_str0[];
extern s32 rpg_bat_target_no;
extern s32 rpg_bat_hit_cnt;
extern s32 rpg_bat_seq_work;
extern volatile u8 cw155_prio;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_scene_step(void)
{
    s32 t0;

    switch ((u32)rpg_bat_unit0.step) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf, (s32)rpg_bat_scene_step_str0);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl, 1, 90);
        rpg_bat_pos_reset();
        rpg_bat_target_set();
        rpg_bat_pos2_set(rpg_bat_target_no);
        rpg_bat_reward_set();
        if (rpg_bat_target_no == 1) {
            cw155_prio = 4;
            class_w[157].prio = 4;
        }
        t0 = rpg_bat_hit_cnt;
        switch ((u32)t0) {
        default:
        case 0:
            rpg_bat_seq_work = 5;
            break;
        case 1:
            rpg_bat_seq_work = 6;
            break;
        case 2:
            rpg_bat_seq_work = 7;
            break;
        case 3:
            rpg_bat_seq_work = 8;
            break;
        case 4:
            rpg_bat_seq_work = 9;
            break;
        case 5:
            rpg_bat_seq_work = 10;
            break;
        }
        rpg_bat_step_next();
        rpg_bat_step_next();
        break;
    case 1:
        break;
    case 2:
        rpg_bat_slide1();
        break;
    case 3:
        rpg_bat_bukatu11_count_step();
        break;
    case 4:
        rpg_bat_bukatu11_lineup_step();
        break;
    case 5:
        rpg_bat_bukatu11_fly_step();
        break;
    case 6:
        rpg_bat_effect_step6();
        break;
    case 7:
        rpg_bat_row_disp();
        rpg_bat_step_next();
        break;
    case 8:
        rpg_bat_bukatu_exit();
        break;
    case 9:
        rpg_bat_clear();
        rpg_bat_hit_cnt = 0;
        break;
    }
}
