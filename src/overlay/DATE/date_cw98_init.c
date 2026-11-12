/* @SymbolStart: 0x801510DC */
/* @SymbolSize: 100 */
/* @PostProc: scheduler_normalizer --block-aware */
/* Address: 0x801510DC (absorbed cross-prologue head func_801510DC) */
/* Size: 100 bytes (25 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern volatile u8 cw98_disp;
extern s16 cw98_anim;
extern s16 cw98_frame;
extern CLASS_W class_w[];
extern s32 sub_sub_step_up(void);

s32 date_cw98_init(void)
{
    class_w[98].bright = 128;
    cw98_disp |= 0x80;
    class_w[98].tick = 0;
    cw98_anim = 4;
    cw98_frame = 0;
    class_w[98].anim_flag = 1;
    return sub_sub_step_up();
}
