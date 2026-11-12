/* Function: dec_bg_cache45bf_step */
/* Overlay: DATE */
/* Address: 0x80158F9C */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void dec_bg_cache_step();
extern s32 sub_sub_step_up();

void dec_bg_cache45bf_step(void) {
    dec_bg_cache_step(17855);
    sub_sub_step_up();
}
