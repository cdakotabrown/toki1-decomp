/* @SymbolSize: 40 */
/* dec_bg_cache4144_step - SHOUGATU overlay, Address: 0x80132DE4, Size: 40 bytes (10 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache4144_step(void)
{
    dec_bg_cache_step(16708);
    sub_sub_step_up();
}
