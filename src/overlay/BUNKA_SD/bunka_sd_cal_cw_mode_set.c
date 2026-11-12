/* Address: 0x801369AC */
/* Size: 32 bytes */
/* @SymbolSize: 32 */
#include "types.h"

extern s32 step_counter;
extern u8 cal_cw_mode;

void bunka_sd_cal_cw_mode_set(void)
{
    if (step_counter == 0) {
        cal_cw_mode = 5;
    }
}
