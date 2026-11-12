/* @SymbolStart: 0x80138A3C */
/* @SymbolSize: 68 */
/* dec_bg_cache40c6_40d0_step - SHOUGATU overlay, Size: 60 bytes (15 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32);
extern u8 shougatu_girl2_basyo3;

void dec_bg_cache40c6_40d0_step(void)
{
    if (shougatu_girl2_basyo3 != 0) {
        dec_bg_cache_step(16592);
    } else {
        dec_bg_cache_step(16582);
    }
    sub_sub_step_up();
}
