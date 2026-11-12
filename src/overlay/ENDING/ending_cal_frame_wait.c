/* @SymbolStart: 0x80134C00 */
/* @SymbolSize: 68 */
/* ending_cal_frame_wait - ENDING overlay, Address: 0x80134C00, Size: 68 bytes (17 instructions) */
#include "types.h"

extern s16 cal_cw_frame;
extern s16 cal_cw_tick;
extern void sub_sub_step_up();

void ending_cal_frame_wait(void)
{
    s16 step = cal_cw_frame;
    s16 counter;

    if (step != 2) {
        return;
    }
    counter = cal_cw_tick;
    if (counter != 47) {
        return;
    }
    sub_sub_step_up();
}
