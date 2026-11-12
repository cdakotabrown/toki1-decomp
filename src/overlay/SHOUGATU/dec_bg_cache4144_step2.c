/* @SymbolSize: 40 */
/* /* dec_bg_cache4144_step2 - SHOUGATU overlay, Address: 0x80133738, Size: 48 bytes (12 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache4144_step2(void)
{
    dec_bg_cache_step(16708);
    sub_sub_step_up();
}
