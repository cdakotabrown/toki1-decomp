/* kagami_toriko_talk - RPG_BAT overlay, Address: 0x8014E1A4, Size: 232 bytes (58 insns) */
#include "types.h"

extern s32 strcpy();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_koe_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_act_next();

extern u8 kagami_toriko_talk_str0;
extern u8 rpg_bat_msg_buf3;
extern u8 rpg_bat_msg_tbl3;
extern s32 rpg_bat_target_no;
extern s32 rpg_bat_toriko_flag[];
extern u8 class_w[];

void kagami_toriko_talk(void)
{
    strcpy(&rpg_bat_msg_buf3, &kagami_toriko_talk_str0);
    rpg_bat_msg_set(&rpg_bat_msg_tbl3, 1, 140);
    rpg_bat_koe_set(0x0300001D, 0x04000025, 0);
    switch (rpg_bat_target_no) {
    case 0:
        rpg_bat_toriko_flag[0] = 1;
        break;
    case 1:
        rpg_bat_toriko_flag[1] = 1;
        break;
    case 2:
        rpg_bat_toriko_flag[2] = 1;
        break;
    }
    class_w[rpg_bat_target_no * 68 + 10543] |= 1;
    rpg_bat_anim_set(29, 6, 1, 1);
    rpg_bat_act_next();
}
