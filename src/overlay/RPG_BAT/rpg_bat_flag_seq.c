/* @SymbolStart: 0x8014F0F8 */
/* @SymbolSize: 64 */
#include "types.h"

extern u8 cal_cw_mode[];
extern void rpg_bat_seq_step_next(void);

void rpg_bat_flag_seq(s32 idx)
{
    if ((cal_cw_mode[idx * 68] & 1) == 0) {
        rpg_bat_seq_step_next();
    }
}
