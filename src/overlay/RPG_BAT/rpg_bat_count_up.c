/* @SymbolSize: 80 */
/* rpg_bat_count_up - RPG_BAT overlay, Address: 0x8014EB58, Size: 80 bytes (20 insns) */
#include "types.h"

s32 rpg_bat_bukatu_no = 0;
s32 rpg_bat_bukatu_flag = 0;
s32 rpg_bat_bukatu_ready = 0;
s32 rpg_bat_target_no[100] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};
s32 rpg_bat_se_wait[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};
s32 rpg_bat_talk_work[12] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1
};
s32 rpg_bat_unit0[11] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0
};
s32 rpg_bat_unit1[11] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0
};
s32 rpg_bat_unit2[11] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0
};
s32 rpg_bat_unit3[11] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0
};
s32 rpg_bat_msg_ctl[3] = {
    0, 1, 2
};
s32 rpg_bat_menu_ctl[3] = {
    0, 1, 2
};
s32 rpg_bat_slot0_ctl[3] = {
    0, 1, 2
};
s32 rpg_bat_slot1_ctl[3] = {
    0, 1, 2
};
s32 rpg_bat_slot2_ctl[3] = {
    0, 1, 2
};
s32 rpg_bat_slot3_ctl[3] = {
    0, 1, 2
};

void rpg_bat_count_up(s32 arg0)
{
    switch (arg0) {
    case 0:
        rpg_bat_msg_ctl[0]++;
        rpg_bat_msg_ctl[2] = 0;
        break;
    case 1:
        rpg_bat_menu_ctl[0]++;
        rpg_bat_menu_ctl[2] = 0;
        break;
    }
}
