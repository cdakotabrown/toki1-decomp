/* @SymbolStart: 0x8014EDBC */
/* @SymbolSize: 64 */
#include "types.h"

extern u8 cal_cw_mode[];
extern void rpg_bat_step_next(void);

void rpg_bat_rec_step(s32 idx)
{
    if ((cal_cw_mode[idx * 68] & 1) == 0) {
        rpg_bat_step_next();
    }
}
