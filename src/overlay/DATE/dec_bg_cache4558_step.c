/* Function: dec_bg_cache4558_step */
/* Overlay: DATE */
/* Address: 0x80153AFC */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void dec_bg_cache_step();
extern s32 sub_sub_step_up();

void dec_bg_cache4558_step(void) {
    dec_bg_cache_step(17752);
    sub_sub_step_up();
}
