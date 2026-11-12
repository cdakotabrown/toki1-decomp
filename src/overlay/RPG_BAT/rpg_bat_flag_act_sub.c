/* @SymbolStart: 0x8014F408 */
/* @SymbolSize: 64 */
/* rpg_bat_flag_act_sub - RPG_BAT overlay, Address: 0x8014F408 (prologue 0x8014F420), */
/* Size: 64 bytes (16 insns).  Calls rpg_bat_act_sub_next() unless the flag byte */
#include "types.h"

extern u8 cal_cw_mode[];
extern s32 rpg_bat_act_sub_next();

void rpg_bat_flag_act_sub(s32 idx)
{
    if ((cal_cw_mode[idx * 68] & 1) == 0) {
        rpg_bat_act_sub_next();
    }
}
