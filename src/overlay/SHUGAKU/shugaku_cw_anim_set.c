/* Address: 0x80135AE4 (true start 0x80135A9C, .sym late by 18 insns) */
/* @SymbolStart: 0x80135A9C */
/* @SymbolSize: 116 */
/* @PostProc: scheduler_normalizer */
#include "types.h"
#include "tokimeki/class_w.h"

extern void sub_sub_step_up(void);

extern u8 cal_cw_disp;
extern u8 cw97_disp;
extern s16 cal_cw_anim;
extern s16 cw97_anim;
extern s16 cw98_frame;
extern CLASS_W class_w[];

void shugaku_cw_anim_set(void)
{
    cal_cw_disp |= 0x80;
    cal_cw_anim = 8;
    cw97_disp |= 0x80;
    cw97_anim = 7;
    class_w[98].anim_flag = 0x40;
    cw98_frame = 0x14;
    sub_sub_step_up();
}
