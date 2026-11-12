/* rpg_bat_rec_flag_check - RPG_BAT overlay, Address: 0x8013E90C, Size: 40 bytes */
#include "types.h"

extern u8 cal_cw_mode[];

s32 rpg_bat_rec_flag_check(s32 idx)
{
    return (cal_cw_mode[idx * 68] & 1) == 0;
}
