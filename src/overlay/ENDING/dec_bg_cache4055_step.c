/* @SymbolSize: 40 */
/* dec_bg_cache4055_step - ENDING overlay, Address: 0x80133BF0, Size: 40 bytes (10 instructions) */
#include "types.h"

extern void dec_bg_cache_step();
extern void sub_sub_step_up();

void dec_bg_cache4055_step(void)
{
    dec_bg_cache_step(16469);
    sub_sub_step_up();
}
