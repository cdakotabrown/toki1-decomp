/* dec_bg_cache4068_step2 -- GEKO overlay, Address: 0x8013ED1C, Size: 40 bytes (10 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache4068_step2(void)
{
    dec_bg_cache_step(16488);
    sub_sub_step_up();
}
