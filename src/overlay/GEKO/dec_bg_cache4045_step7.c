/* dec_bg_cache4045_step7 -- GEKO overlay, Address: 0x8013F268, Size: 40 bytes (10 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache4045_step7(void)
{
    dec_bg_cache_step(16453);
    sub_sub_step_up();
}
