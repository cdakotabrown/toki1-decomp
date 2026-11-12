/* @SymbolSize: 40  * Status: BYTE-PERFECT 🎉 */
/* Address: 0x801397B0 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache46f0_step(void)
{
    dec_bg_cache_step(18160);
    sub_sub_step_up();
}
