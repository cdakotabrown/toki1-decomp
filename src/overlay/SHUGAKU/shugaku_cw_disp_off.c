/* @PostProc: scheduler_normalizer */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8  cal_cw_disp;
extern u8  cw97_disp;
extern s16 cw98_anim;
extern s16 cw98_frame;
extern CLASS_W class_w[];

s32 shugaku_cw_disp_off(void)
{
    cal_cw_disp &= ~0x80;
    cw97_disp &= ~0x80;
    class_w[98].anim_flag = 5;
    cw98_anim = 4;
    cw98_frame = 0;
    return ~0x80;
}
