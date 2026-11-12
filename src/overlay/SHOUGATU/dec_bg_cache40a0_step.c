/* Address: 0x8013A964 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache40a0_step(void)
{
    dec_bg_cache_step(16544);
    sub_sub_step_up();
}
