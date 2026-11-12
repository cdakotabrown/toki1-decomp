/* @SymbolSize: 40 */
/* dec_bg_cache403b_step7 -- GEKO overlay, Address: 0x8013DE5C */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache403b_step7(void)
{
    dec_bg_cache_step(16443);
    sub_sub_step_up();
}
