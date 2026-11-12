/* @SymbolSize: 64 */
/* Address: 0x8013AE90 */
/* Size: 56 bytes (14 instructions) + 2 words of window padding */
#include "types.h"

extern s32 get_g_zyotai_h(s32);
extern void bustup_timer_clamp(s32);
extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache4156_step(void)
{
    s32 result;
    result = get_g_zyotai_h(1);
    bustup_timer_clamp(result);
    dec_bg_cache_step(16726);
    sub_sub_step_up();
}
