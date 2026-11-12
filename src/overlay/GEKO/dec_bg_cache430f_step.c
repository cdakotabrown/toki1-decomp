/* dec_bg_cache430f_step -- GEKO overlay, Address: 0x801415F8, Size: 40 bytes (10 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache430f_step(void)
{
    dec_bg_cache_step(17167);
    sub_sub_step_up();
}
