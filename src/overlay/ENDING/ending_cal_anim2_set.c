/* @SymbolStart: 0x80134AB8 */
/* @SymbolSize: 112 */
/* ending_cal_anim2_set - ENDING overlay, Address: 0x80134AB8, Size: 112 bytes (28 instructions) */
#include "types.h"

extern u8 cw97_mode;
extern s16 cal_cw_frame;
extern s16 cal_cw_tick;
extern s16 cal_cw_anim;
extern u8 cal_cw_mode;
extern void sub_sub_step_up();

void ending_cal_anim2_set(void)
{
    u8 guard = cw97_mode;

    if (guard != 0) {
        return;
    }
    if (cal_cw_frame != 1) {
        return;
    }
    if (cal_cw_tick != 20) {
        return;
    }
    cal_cw_anim = 2;
    cal_cw_frame = 0;
    cal_cw_tick = 0;
    cal_cw_mode = 5;
    sub_sub_step_up();
}
