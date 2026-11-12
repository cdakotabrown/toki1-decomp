/* @SymbolSize: 40 */
/* Address: 0x8013D3D8 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache403b_step6(void)
{
    dec_bg_cache_step(16443);
    sub_sub_step_up();
}
