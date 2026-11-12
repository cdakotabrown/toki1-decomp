/* dec_bg_cache4144_step -- GEKO overlay, Address: 0x801409E0, Size: 40 bytes (10 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache4144_step(void)
{
    dec_bg_cache_step(16708);
    sub_sub_step_up();
}
