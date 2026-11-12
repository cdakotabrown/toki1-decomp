/* dec_bg_cache4285_step - SHUGAKU overlay, Address: 0x801347B8, Size: 40 bytes (10 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache4285_step(void)
{
    dec_bg_cache_step(0x4285);
    sub_sub_step_up();
}
