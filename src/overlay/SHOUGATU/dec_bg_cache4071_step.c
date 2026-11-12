/* Address: 0x80138F58 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache4071_step(void)
{
    dec_bg_cache_step(16497);
    sub_sub_step_up();
}
