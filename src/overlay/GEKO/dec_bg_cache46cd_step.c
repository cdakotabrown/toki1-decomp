/* dec_bg_cache46cd_step -- GEKO overlay, Address: 0x8013DA2C, Size: 40 bytes (10 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache46cd_step(void)
{
    dec_bg_cache_step(18125);
    sub_sub_step_up();
}
