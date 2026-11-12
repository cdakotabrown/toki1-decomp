/* Function: dec_bg_cache482a_step */
/* Overlay: DATE */
/* Address: 0x80153A38 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void dec_bg_cache_step();
extern s32 sub_sub_step_up();

void dec_bg_cache482a_step(void) {
    dec_bg_cache_step(18474);
    sub_sub_step_up();
}
