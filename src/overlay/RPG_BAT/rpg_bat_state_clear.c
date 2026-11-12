/* @SymbolSize: 452 */
/* rpg_bat_state_clear - RPG_BAT overlay, Address: 0x8014E994, Size: 452 bytes (113 insns) */
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

void rpg_bat_state_clear(void)
{
    s32 i;

    for (i = 0; i < 100; i++) rpg_bat_target_no[i] = 0;
    for (i = 0; i < 10; i++) rpg_bat_se_wait[i] = 0;
    for (i = 0; i < 12; i++) rpg_bat_talk_work[i] = 0;
    rpg_bat_msg_ctl[0] = 0;
    rpg_bat_msg_ctl[1] = 0;
    rpg_bat_msg_ctl[2] = 0;
    rpg_bat_menu_ctl[0] = 0;
    rpg_bat_menu_ctl[1] = 0;
    rpg_bat_menu_ctl[2] = 0;
    rpg_bat_slot0_ctl[0] = 0;
    rpg_bat_slot0_ctl[1] = 0;
    rpg_bat_slot0_ctl[2] = 0;
    rpg_bat_slot1_ctl[0] = 0;
    rpg_bat_slot1_ctl[1] = 0;
    rpg_bat_slot1_ctl[2] = 0;
    rpg_bat_slot2_ctl[0] = 0;
    rpg_bat_slot2_ctl[1] = 0;
    rpg_bat_slot2_ctl[2] = 0;
    rpg_bat_slot3_ctl[0] = 0;
    rpg_bat_slot3_ctl[1] = 0;
    rpg_bat_slot3_ctl[2] = 0;
    rpg_bat_unit0[0] = 0;
    rpg_bat_unit0[1] = 0;
    rpg_bat_unit0[2] = 0;
    rpg_bat_unit0[3] = 0;
    rpg_bat_unit0[4] = 0;
    rpg_bat_unit2[0] = 0;
    rpg_bat_unit2[1] = 0;
    rpg_bat_unit2[2] = 0;
    rpg_bat_unit2[3] = 0;
    rpg_bat_unit2[4] = 0;
    rpg_bat_unit1[0] = 0;
    rpg_bat_unit1[1] = 0;
    rpg_bat_unit1[2] = 0;
    rpg_bat_unit1[3] = 0;
    rpg_bat_unit1[4] = 0;
    rpg_bat_unit3[0] = 0;
    rpg_bat_unit3[1] = 0;
    rpg_bat_unit3[2] = 0;
    rpg_bat_unit3[3] = 0;
    rpg_bat_unit3[4] = 0;
    rpg_bat_unit0[5] = 0;
    rpg_bat_unit0[6] = 0;
    rpg_bat_unit0[8] = 0;
    rpg_bat_unit0[9] = 0;
    rpg_bat_unit2[5] = 0;
    rpg_bat_unit2[6] = 0;
    rpg_bat_unit2[8] = 0;
    rpg_bat_unit2[9] = 0;
    rpg_bat_unit1[5] = 0;
    rpg_bat_unit1[6] = 0;
    rpg_bat_unit1[8] = 0;
    rpg_bat_unit1[9] = 0;
    rpg_bat_unit3[5] = 0;
    rpg_bat_unit3[6] = 0;
    rpg_bat_unit3[8] = 0;
    rpg_bat_unit3[9] = 0;
    rpg_bat_unit0[7] = 0;
    rpg_bat_unit2[7] = 0;
    rpg_bat_unit1[7] = 0;
    rpg_bat_unit3[7] = 0;
}
