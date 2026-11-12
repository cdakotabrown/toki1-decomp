/* dec_bg_cache42d8_step -- GEKO overlay, Address: 0x80134010, Size: 40 bytes (10 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32 a0);
extern void sub_sub_step_up(void);

void dec_bg_cache42d8_step(void)
{
    dec_bg_cache_step(17112);
    sub_sub_step_up();
}
