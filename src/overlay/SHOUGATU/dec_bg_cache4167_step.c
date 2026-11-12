/* Address: 0x80132864 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache4167_step(void)
{
    dec_bg_cache_step(16743);
    sub_sub_step_up();
}
