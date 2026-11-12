/* dec_bg_cache4708_step -- GEKO overlay, Address: 0x80141590, Size: 40 bytes (10 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache4708_step(void)
{
    dec_bg_cache_step(18184);
    sub_sub_step_up();
}
