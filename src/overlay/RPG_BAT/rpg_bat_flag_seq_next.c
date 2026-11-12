/* @SymbolStart: 0x8014F654 */
/* @SymbolSize: 64 */
#include "types.h"

extern u8 cal_cw_mode[];
extern void rpg_bat_seq_next(void);

void rpg_bat_flag_seq_next(s32 idx)
{
    if ((cal_cw_mode[idx * 68] & 1) == 0) {
        rpg_bat_seq_next();
    }
}
