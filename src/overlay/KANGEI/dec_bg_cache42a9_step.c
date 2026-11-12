/* dec_bg_cache42a9_step - KANGEI overlay, Address: 0x80138E44, Size: 40 bytes (10 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache42a9_step(void)
{
    dec_bg_cache_step(17065);
    sub_sub_step_up();
}
