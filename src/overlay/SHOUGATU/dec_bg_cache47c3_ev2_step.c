/* @SymbolSize: 52 */
/* Address: 0x801435CC */
/* Size: 52 bytes (13 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();
extern void event_log_add();

void dec_bg_cache47c3_ev2_step(void)
{
    dec_bg_cache_step(18371);
    event_log_add(2, 66);
    sub_sub_step_up();
}
