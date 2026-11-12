/* @SymbolSize: 40 */
#include "types.h"

extern void sub_sub_step_up();
extern u8 cal_cw_mode;

void shougatu_cal_cw_mode9_step(void)
{
    cal_cw_mode = 9;
    sub_sub_step_up();
}
