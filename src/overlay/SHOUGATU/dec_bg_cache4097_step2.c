/* @SymbolSize: 40 */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache4097_step2(void)
{
    dec_bg_cache_step(16535);
    sub_sub_step_up();
}
