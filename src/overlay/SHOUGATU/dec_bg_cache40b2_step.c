/* Address: 0x8013A218 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache40b2_step(void)
{
    dec_bg_cache_step(16562);
    sub_sub_step_up();
}
