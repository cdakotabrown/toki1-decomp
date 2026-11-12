/* kagami_miss_talk - RPG_BAT overlay, Address: 0x8014E28C, Size: 116 bytes (29 insns) */
#include "types.h"

extern u8 rpg_bat_msg_buf3[];
extern u8 kagami_miss_talk_str0[];
extern u8 rpg_bat_msg_tbl3[];

extern s32 strcpy();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_koe_set();
extern void rpg_bat_msg_set();
extern s32 rpg_bat_act_next();

void kagami_miss_talk(void)
{
    strcpy(rpg_bat_msg_buf3, kagami_miss_talk_str0);
    rpg_bat_msg_set(rpg_bat_msg_tbl3, 1, 100);
    rpg_bat_koe_set(0x1F000069, 0x1300006D, 0);
    rpg_bat_anim_set(29, 6, 1, 1);
    rpg_bat_act_next();
}
