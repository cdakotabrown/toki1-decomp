/* dec_bg_cache4699_step -- GEKO overlay, Address: 0x80140968, Size: 40 bytes (10 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache4699_step(void)
{
    dec_bg_cache_step(18073);
    sub_sub_step_up();
}
