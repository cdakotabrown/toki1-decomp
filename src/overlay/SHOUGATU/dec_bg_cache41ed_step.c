/* @SymbolSize: 40 */
/* /* dec_bg_cache41ed_step - SHOUGATU overlay, Address: 0x80136404, Size: 48 bytes (12 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void dec_bg_cache_step();

void dec_bg_cache41ed_step(void)
{
    dec_bg_cache_step(16877);
    sub_sub_step_up();
}
