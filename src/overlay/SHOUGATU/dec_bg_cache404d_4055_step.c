/* @SymbolStart: 0x8013AE50 */
/* @SymbolSize: 84 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32 arg);
extern u8 now_month;

void dec_bg_cache404d_4055_step(void)
{
    if ((u32)now_month < 6U) goto lbl_AE84;
    if ((u32)now_month >= 10U) goto lbl_AE84;
    dec_bg_cache_step(16461);
    goto lbl_AE8C;
lbl_AE84:
    dec_bg_cache_step(16469);
lbl_AE8C:
    sub_sub_step_up();
}
