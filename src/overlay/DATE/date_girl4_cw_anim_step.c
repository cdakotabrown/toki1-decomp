/* @SymbolStart: 0x8015943C */
/* @SymbolSize: 52 */
/* Address: 0x8015943C (absorbed cross-prologue head func_8015943C) */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern u8 now_girl;
extern s16 cal_cw_anim;
extern s32 sub_sub_step_up(void);

s32 date_girl4_cw_anim_step(void)
{
    now_girl = 14;
    cal_cw_anim = 4;
    return sub_sub_step_up();
}
