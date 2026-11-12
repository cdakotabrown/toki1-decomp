/* Function: dec_bg_cache45f8_step2 */
/* Overlay: DATE */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern s32 sub_sub_step_up();
extern void dec_bg_cache_step();

s32 dec_bg_cache45f8_step2(void)
{
    dec_bg_cache_step(17912);
    return sub_sub_step_up();
}
