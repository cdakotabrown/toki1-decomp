/* Function: dec_bg_cache4544_step */
/* Overlay: DATE */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern s32 sub_sub_step_up();
extern void dec_bg_cache_step();

s32 dec_bg_cache4544_step(void)
{
    dec_bg_cache_step(17732);
    return sub_sub_step_up();
}
