/* @SymbolSize: 40 */
/* dec_bg_cache4045_step6 - GEKO overlay, Address: 0x8013DA54, Size: 48 bytes (12 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache4045_step6(void)
{
    dec_bg_cache_step(16453);
    sub_sub_step_up();
}
