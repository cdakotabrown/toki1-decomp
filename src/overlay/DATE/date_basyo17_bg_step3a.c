/* Function: date_basyo17_bg_step3a */
/* Overlay: DATE */
/* Address: 0x80138800 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern s32 sub_sub_step_up();
extern void dec_bg_cache_step();

s32 date_basyo17_bg_step3a(void)
{
    dec_bg_cache_step(17827);
    return sub_sub_step_up();
}
