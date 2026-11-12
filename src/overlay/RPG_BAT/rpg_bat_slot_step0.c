/* @SymbolStart: 0x8014C4FC */
/* @SymbolSize: 776 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void rpg_bat_slot_anim_set(s32, s32, s32);

extern s32 rpg_bat_slot0_ctl_mode;
extern s32 rpg_bat_slot0_ctl_cnt;
extern s16 rpg_bat_slot_move_tbl[];
extern CLASS_W class_w[];


void rpg_bat_slot_step0(void)
{
    s32 flag;
    s32 group;
    s32 idx;
    s16 *rec;

    flag = rpg_bat_slot0_ctl_mode;
    if (flag != 1)
        return;

    group = rpg_bat_slot0_ctl_cnt / 2;

    switch (group) {
    case 0:
        rpg_bat_slot_anim_set(0, 255, 1);
        rpg_bat_slot_anim_set(4, 255, 1);
        rpg_bat_slot_anim_set(8, 255, 1);
        break;
    case 1:
        rpg_bat_slot_anim_set(1, 255, 1);
        rpg_bat_slot_anim_set(5, 255, 1);
        rpg_bat_slot_anim_set(9, 255, 1);
        break;
    case 2:
        rpg_bat_slot_anim_set(2, 255, 1);
        rpg_bat_slot_anim_set(6, 255, 1);
        rpg_bat_slot_anim_set(10, 255, 1);
        break;
    case 3:
        rpg_bat_slot_anim_set(3, 255, 1);
        rpg_bat_slot_anim_set(7, 255, 1);
        rpg_bat_slot_anim_set(11, 255, 1);
        break;
    default:
        break;
    }

    if (group >= 16)
        group = 15;

    if ((rpg_bat_slot0_ctl_cnt & 1) == 0) {
        rec = (s16 *)((u8 *)rpg_bat_slot_move_tbl + group * 24);
        class_w[64].y.half[1] += rec[0];
        class_w[68].y.half[1] += rec[1];
        class_w[72].y.half[1] += rec[2];
        class_w[65].y.half[1] += rec[3];
        class_w[69].y.half[1] += rec[4];
        class_w[73].y.half[1] += rec[5];
        class_w[66].y.half[1] += rec[6];
        class_w[70].y.half[1] += rec[7];
        class_w[74].y.half[1] += rec[8];
        class_w[67].y.half[1] += rec[9];
        class_w[71].y.half[1] += rec[10];
        class_w[75].y.half[1] += rec[11];
    }
    rpg_bat_slot0_ctl_cnt++;

    group = rpg_bat_slot0_ctl_cnt / 2;
    if (group >= 26) {
        idx = 0;
        do {
            rpg_bat_slot_anim_set(idx, 255, 0);
            idx += 1;
        } while (idx != 12);
        rpg_bat_slot0_ctl_mode = 0;
    }
}
