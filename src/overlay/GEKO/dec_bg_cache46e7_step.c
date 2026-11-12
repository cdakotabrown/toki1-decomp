/* dec_bg_cache46e7_step -- GEKO overlay, Address: 0x8013D3B0, Size: 40 bytes (10 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache46e7_step(void)
{
    dec_bg_cache_step(18151);
    sub_sub_step_up();
}
