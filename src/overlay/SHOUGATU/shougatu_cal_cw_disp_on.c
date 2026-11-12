/* @SymbolStart: 0x8013DFB4 */
/* @SymbolSize: 48 */
/* Address: 0x8013DFB4 (head) / 0x8013DFBC (body) */
/* Size: 48 bytes (12 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern u8 cal_cw_disp;

void shougatu_cal_cw_disp_on(void)
{
    u8 t6 = cal_cw_disp;

    cal_cw_disp = t6 | 0x80;
    sub_sub_step_up();
}
