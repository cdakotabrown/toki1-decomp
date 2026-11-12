/* Address: 0x80135BD8 (true start 0x80135BBC, .sym late by 7 insns) */
/* @SymbolStart: 0x80135BBC */
/* @SymbolSize: 72 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void sub_sub_step_up(void);

extern s16 cal_cw_anim;
extern s16 cw97_anim;
extern s16 cw98_frame;
extern CLASS_W class_w[];

void shugaku_cw_anim_set3(void)
{
    cal_cw_anim = 2;
    cw97_anim = 0;
    class_w[98].anim_flag = 0x40;
    cw98_frame = 0x14;
    sub_sub_step_up();
}
