/* @SymbolStart: 0x8014F138 */
/* @SymbolSize: 64 */
/* rpg_bat_flag_substep - RPG_BAT overlay, Address: 0x8014F138 (prologue 0x8014F150), */
/* Size: 64 bytes (16 insns).  Calls rpg_bat_substep_next() unless the flag byte */
#include "types.h"

extern u8 cal_cw_mode[];
extern s32 rpg_bat_substep_next();

void rpg_bat_flag_substep(s32 idx)
{
    if ((cal_cw_mode[idx * 68] & 1) == 0) {
        rpg_bat_substep_next();
    }
}
