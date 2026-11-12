/* Address: 0x80138A80 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache40c6_step(void)
{
    dec_bg_cache_step(16582);
    sub_sub_step_up();
}
