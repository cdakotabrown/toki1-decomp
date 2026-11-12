/* @SymbolStart: 0x8014C804 */
/* @SymbolSize: 776 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void rpg_bat_slot_anim_set(s32, s32, s32);

extern s32 rpg_bat_slot1_ctl_mode;
extern s32 rpg_bat_slot1_ctl_cnt;
extern s16 rpg_bat_slot_move_tbl[];

extern s16 cw78_y;
extern CLASS_W class_w[];

void rpg_bat_slot_step1(void)
{
    s32 flag;
    s32 group;
    s32 idx;
    s16 *rec;

    flag = rpg_bat_slot1_ctl_mode;
    if (flag != 1)
        return;

    group = rpg_bat_slot1_ctl_cnt / 2;

    switch (group) {
    case 0:
        rpg_bat_slot_anim_set(12, 255, 1);
        rpg_bat_slot_anim_set(16, 255, 1);
        rpg_bat_slot_anim_set(20, 255, 1);
        break;
    case 1:
        rpg_bat_slot_anim_set(13, 255, 1);
        rpg_bat_slot_anim_set(17, 255, 1);
        rpg_bat_slot_anim_set(21, 255, 1);
        break;
    case 2:
        rpg_bat_slot_anim_set(14, 255, 1);
        rpg_bat_slot_anim_set(18, 255, 1);
        rpg_bat_slot_anim_set(22, 255, 1);
        break;
    case 3:
        rpg_bat_slot_anim_set(15, 255, 1);
        rpg_bat_slot_anim_set(19, 255, 1);
        rpg_bat_slot_anim_set(23, 255, 1);
        break;
    default:
        break;
    }

    if (group >= 16)
        group = 15;

    if ((rpg_bat_slot1_ctl_cnt & 1) == 0) {
        rec = (s16 *)((u8 *)rpg_bat_slot_move_tbl + group * 24);
        class_w[76].y.half[1] += rec[0];
        class_w[80].y.half[1] += rec[1];
        class_w[84].y.half[1] += rec[2];
        class_w[77].y.half[1] += rec[3];
        class_w[81].y.half[1] += rec[4];
        class_w[85].y.half[1] += rec[5];
        cw78_y += rec[6];
        class_w[82].y.half[1] += rec[7];
        class_w[86].y.half[1] += rec[8];
        class_w[79].y.half[1] += rec[9];
        class_w[83].y.half[1] += rec[10];
        class_w[87].y.half[1] += rec[11];
    }
    rpg_bat_slot1_ctl_cnt++;

    group = rpg_bat_slot1_ctl_cnt / 2;
    if (group >= 26) {
        idx = 0;
        do {
            rpg_bat_slot_anim_set(idx + 12, 255, 0);
            idx += 1;
        } while (idx != 12);
        rpg_bat_slot1_ctl_mode = 0;
    }
}
