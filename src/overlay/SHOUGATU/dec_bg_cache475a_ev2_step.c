/* Address: 0x8014259C */
/* Size: 52 bytes (13 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();
extern void event_log_add();

void dec_bg_cache475a_ev2_step(void)
{
    dec_bg_cache_step(18266);
    event_log_add(2, 6);
    sub_sub_step_up();
}
