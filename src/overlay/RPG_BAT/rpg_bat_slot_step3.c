/* @SymbolStart: 0x8014CE14 */
/* @SymbolSize: 776 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void rpg_bat_slot_anim_set(s32, s32, s32);

extern s32 rpg_bat_slot3_ctl_mode;
extern s32 rpg_bat_slot3_ctl_cnt;
extern s16 rpg_bat_slot_move_tbl[];
extern CLASS_W class_w[];


void rpg_bat_slot_step3(void)
{
    s32 flag;
    s32 group;
    s32 idx;
    s16 *rec;

    flag = rpg_bat_slot3_ctl_mode;
    if (flag != 1)
        return;

    group = rpg_bat_slot3_ctl_cnt / 2;

    switch (group) {
    case 0:
        rpg_bat_slot_anim_set(36, 255, 1);
        rpg_bat_slot_anim_set(40, 255, 1);
        rpg_bat_slot_anim_set(44, 255, 1);
        break;
    case 1:
        rpg_bat_slot_anim_set(37, 255, 1);
        rpg_bat_slot_anim_set(41, 255, 1);
        rpg_bat_slot_anim_set(45, 255, 1);
        break;
    case 2:
        rpg_bat_slot_anim_set(38, 255, 1);
        rpg_bat_slot_anim_set(42, 255, 1);
        rpg_bat_slot_anim_set(46, 255, 1);
        break;
    case 3:
        rpg_bat_slot_anim_set(39, 255, 1);
        rpg_bat_slot_anim_set(43, 255, 1);
        rpg_bat_slot_anim_set(47, 255, 1);
        break;
    default:
        break;
    }

    if (group >= 16)
        group = 15;

    if ((rpg_bat_slot3_ctl_cnt & 1) == 0) {
        rec = (s16 *)((u8 *)rpg_bat_slot_move_tbl + group * 24);
        class_w[100].y.half[1] += rec[0];
        class_w[104].y.half[1] += rec[1];
        class_w[108].y.half[1] += rec[2];
        class_w[101].y.half[1] += rec[3];
        class_w[105].y.half[1] += rec[4];
        class_w[109].y.half[1] += rec[5];
        class_w[102].y.half[1] += rec[6];
        class_w[106].y.half[1] += rec[7];
        class_w[110].y.half[1] += rec[8];
        class_w[103].y.half[1] += rec[9];
        class_w[107].y.half[1] += rec[10];
        class_w[111].y.half[1] += rec[11];
    }
    rpg_bat_slot3_ctl_cnt++;

    group = rpg_bat_slot3_ctl_cnt / 2;
    if (group >= 26) {
        idx = 0;
        do {
            rpg_bat_slot_anim_set(idx + 36, 255, 0);
            idx += 1;
        } while (idx != 12);
        rpg_bat_slot3_ctl_mode = 0;
    }
}
