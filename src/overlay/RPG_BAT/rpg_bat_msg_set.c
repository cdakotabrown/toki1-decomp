/* rpg_bat_msg_set - RPG_BAT overlay, Address: 0x8014B738, Size: 100 bytes (25 insns) */
#include "types.h"

extern s32 rpg_bat_msg_tbl_ptr;
extern s32 rpg_bat_msg_time;
extern s32 rpg_bat_msg_num;
extern s32 rpg_bat_msg_ctl;
extern s32 rpg_bat_msg_ctl_mode;
extern s32 rpg_bat_msg_ctl_cnt;

extern void k_sub_reset();

void rpg_bat_msg_set(s32 a0, s32 a1, s32 a2)
{
    k_sub_reset();
    rpg_bat_msg_tbl_ptr = a0;
    rpg_bat_msg_time = a2;
    rpg_bat_msg_num = a1;
    rpg_bat_msg_ctl = 0;
    rpg_bat_msg_ctl_cnt = 0;
    rpg_bat_msg_ctl_mode = 1;
}
