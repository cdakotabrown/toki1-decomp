/* @PostProc: scheduler_normalizer */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8  cal_cw_disp;
extern u8  cw97_disp;
extern s16 cal_cw_anim;
extern s16 cw97_anim;
extern s16 cw98_frame;
extern CLASS_W class_w[];

void shugaku_cw_anim_set2(void)
{
    cal_cw_disp |= 0x80;
    cal_cw_anim = 3;
    cw97_disp |= 0x80;
    cw97_anim = 1;
    class_w[98].anim_flag = 0x40;
    cw98_frame = 22;
}
