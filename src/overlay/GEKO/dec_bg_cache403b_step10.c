/* dec_bg_cache403b_step10 -- GEKO overlay, Address: 0x80143A14, Size: 40 bytes (10 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache403b_step10(void)
{
    dec_bg_cache_step(16443);
    sub_sub_step_up();
}
