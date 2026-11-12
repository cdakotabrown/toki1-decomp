/* Address: 0x8013B9CC */
/* Size: 76 bytes (19 instructions) */
/* @SymbolSize: 52 */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();
extern void event_log_add();

void dec_bg_cache4045_ev7_step(void)
{
    event_log_add(7, 0);
    dec_bg_cache_step(16453);
    sub_sub_step_up();
}
