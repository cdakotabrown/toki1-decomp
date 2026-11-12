/* Address: 0x80139788 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache4045_step2(void)
{
    dec_bg_cache_step(16453);
    sub_sub_step_up();
}
