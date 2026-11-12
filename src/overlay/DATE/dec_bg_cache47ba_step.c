/* Function: dec_bg_cache47ba_step */
/* Overlay: DATE */
/* Address: 0x8014F8E0 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void dec_bg_cache_step();
extern s32 sub_sub_step_up();

void dec_bg_cache47ba_step(void) {
    dec_bg_cache_step(18362);
    sub_sub_step_up();
}
