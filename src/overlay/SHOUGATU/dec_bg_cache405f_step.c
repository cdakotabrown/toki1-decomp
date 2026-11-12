/* Address: 0x801389EC */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache405f_step(void)
{
    dec_bg_cache_step(16479);
    sub_sub_step_up();
}
