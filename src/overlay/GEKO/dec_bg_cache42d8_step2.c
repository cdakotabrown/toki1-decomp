/* @SymbolSize: 40 */
/* /* dec_bg_cache42d8_step2 -- GEKO overlay, Address: 0x8013B8A8, Size: 40 bytes (10 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache42d8_step2(void)
{
    dec_bg_cache_step(17112);
    sub_sub_step_up();
}
