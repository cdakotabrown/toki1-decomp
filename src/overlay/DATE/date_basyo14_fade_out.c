/* @SymbolStart: 0x80138488 */
/* @SymbolSize: 40 */
/* Address: 0x80138488 (cross-prologue head) / 0x80138490 (body) */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern u8 now_sub_sub_step;

extern s32 fade_out_step();

s32 date_basyo14_fade_out(s32 a0, s32 a1, s32 a2, s32 a3)
{
    s32 pad;
    volatile s32 keep;

    keep = now_sub_sub_step;
    return fade_out_step(a0, a1, a2, a3);
}
