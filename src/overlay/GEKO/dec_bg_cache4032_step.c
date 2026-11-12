/* dec_bg_cache4032_step -- GEKO overlay, Address: 0x8013313C, Size: 40 bytes (10 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32 a0);
extern void sub_sub_step_up(void);

void dec_bg_cache4032_step(void)
{
    dec_bg_cache_step(16434);
    sub_sub_step_up();
}
