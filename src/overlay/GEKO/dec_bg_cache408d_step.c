/* dec_bg_cache408d_step -- GEKO overlay, Address: 0x80139840, Size: 40 bytes (10 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache408d_step(void)
{
    dec_bg_cache_step(16525);
    sub_sub_step_up();
}
