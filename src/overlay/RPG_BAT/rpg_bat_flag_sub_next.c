/* @SymbolStart: 0x8014F694 */
/* @SymbolSize: 64 */
/* rpg_bat_flag_sub_next - RPG_BAT overlay, Address: 0x8014F694 (prologue 0x8014F6AC), */
/* Size: 64 bytes (16 insns).  Calls rpg_bat_sub_next() unless the flag byte */
#include "types.h"

extern u8 cal_cw_mode[];
extern void rpg_bat_sub_next();

void rpg_bat_flag_sub_next(s32 idx)
{
    if ((cal_cw_mode[idx * 68] & 1) == 0) {
        rpg_bat_sub_next();
    }
}
