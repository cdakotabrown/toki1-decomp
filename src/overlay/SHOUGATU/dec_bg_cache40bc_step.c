/* Address: 0x8013A1A0 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache40bc_step(void)
{
    dec_bg_cache_step(16572);
    sub_sub_step_up();
}
