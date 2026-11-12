/* Function: dec_bg_cache488a_step */
/* Overlay: DATE */
/* Address: 0x80155CF0 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void dec_bg_cache_step();
extern s32 sub_sub_step_up();

void dec_bg_cache488a_step(void) {
    dec_bg_cache_step(18570);
    sub_sub_step_up();
}
