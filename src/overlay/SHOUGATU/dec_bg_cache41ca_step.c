/* Address: 0x8013DDCC */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache41ca_step(void)
{
    dec_bg_cache_step(16842);
    sub_sub_step_up();
}
