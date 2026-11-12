/* Address: 0x801435A4 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache42e2_step(void)
{
    dec_bg_cache_step(17122);
    sub_sub_step_up();
}
