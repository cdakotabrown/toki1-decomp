/* @SymbolSize: 40 */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache403b_step(void)
{
    dec_bg_cache_step(16443);
    sub_sub_step_up();
}
