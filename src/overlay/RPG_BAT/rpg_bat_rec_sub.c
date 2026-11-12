/* @SymbolStart: 0x8014EDFC */
/* @SymbolSize: 64 */
/* rpg_bat_rec_sub - RPG_BAT overlay, Address: 0x8014EDFC (prologue 0x8014EE14), */
/* Size: 64 bytes (16 insns).  Calls rpg_bat_state_next() unless the flag byte */
#include "types.h"

extern u8 cal_cw_mode[];
extern s32 rpg_bat_state_next();

void rpg_bat_rec_sub(s32 idx)
{
    if ((cal_cw_mode[idx * 68] & 1) == 0) {
        rpg_bat_state_next();
    }
}
