/* Address: 0x80140E00 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache41dc_step(void)
{
    dec_bg_cache_step(16860);
    sub_sub_step_up();
}
