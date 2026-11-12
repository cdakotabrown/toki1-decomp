/* @SymbolStart: 0x8014F3C8 */
/* @SymbolSize: 64 */
#include "types.h"

extern u8 cal_cw_mode[];
extern void rpg_bat_act_next(void);

void rpg_bat_flag_act(s32 idx)
{
    if ((cal_cw_mode[idx * 68] & 1) == 0) {
        rpg_bat_act_next();
    }
}
