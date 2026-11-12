/* @SymbolStart: 0x8014CB0C */
/* @SymbolSize: 776 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void rpg_bat_slot_anim_set(s32, s32, s32);

extern s32 rpg_bat_slot2_ctl_mode;
extern s32 rpg_bat_slot2_ctl_cnt;
extern s16 rpg_bat_slot_move_tbl[];

extern s16 cal_cw_y;
extern s16 cw97_y;
extern s16 cw94_y;
extern s16 cw98_y;
extern CLASS_W class_w[];

void rpg_bat_slot_step2(void)
{
    s32 flag;
    s32 group;
    s32 idx;
    s16 *rec;

    flag = rpg_bat_slot2_ctl_mode;
    if (flag != 1)
        return;

    group = rpg_bat_slot2_ctl_cnt / 2;

    switch (group) {
    case 0:
        rpg_bat_slot_anim_set(24, 255, 1);
        rpg_bat_slot_anim_set(28, 255, 1);
        rpg_bat_slot_anim_set(32, 255, 1);
        break;
    case 1:
        rpg_bat_slot_anim_set(25, 255, 1);
        rpg_bat_slot_anim_set(29, 255, 1);
        rpg_bat_slot_anim_set(33, 255, 1);
        break;
    case 2:
        rpg_bat_slot_anim_set(26, 255, 1);
        rpg_bat_slot_anim_set(30, 255, 1);
        rpg_bat_slot_anim_set(34, 255, 1);
        break;
    case 3:
        rpg_bat_slot_anim_set(27, 255, 1);
        rpg_bat_slot_anim_set(31, 255, 1);
        rpg_bat_slot_anim_set(35, 255, 1);
        break;
    default:
        break;
    }

    if (group >= 16)
        group = 15;

    if ((rpg_bat_slot2_ctl_cnt & 1) == 0) {
        rec = (s16 *)((u8 *)rpg_bat_slot_move_tbl + group * 24);
        class_w[88].y.half[1] += rec[0];
        class_w[92].y.half[1] += rec[1];
        cal_cw_y += rec[2];
        class_w[89].y.half[1] += rec[3];
        class_w[93].y.half[1] += rec[4];
        cw97_y += rec[5];
        class_w[90].y.half[1] += rec[6];
        cw94_y += rec[7];
        cw98_y += rec[8];
        class_w[91].y.half[1] += rec[9];
        class_w[95].y.half[1] += rec[10];
        class_w[99].y.half[1] += rec[11];
    }
    rpg_bat_slot2_ctl_cnt++;

    group = rpg_bat_slot2_ctl_cnt / 2;
    if (group >= 26) {
        idx = 0;
        do {
            rpg_bat_slot_anim_set(idx + 24, 255, 0);
            idx += 1;
        } while (idx != 12);
        rpg_bat_slot2_ctl_mode = 0;
    }
}
