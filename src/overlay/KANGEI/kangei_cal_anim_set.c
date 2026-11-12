/* @SymbolStart: 0x80137448 */
/* @SymbolSize: 68 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern s16 cal_cw_tick;
extern u8 cal_cw_mode;

void kangei_cal_anim_set(void)
{
    cal_cw_anim = 2;
    cal_cw_frame = 0;
    cal_cw_tick = 0;
    cal_cw_mode = 5;
    sub_sub_step_up();
}
