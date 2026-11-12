/* @SymbolSize: 40 */
/* dec_bg_cache4122_step - VALEN overlay, Address: 0x801326EC, Size: 60 bytes (15 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32);

void dec_bg_cache4122_step(void)
{
    dec_bg_cache_step(16674);
    sub_sub_step_up();
}
