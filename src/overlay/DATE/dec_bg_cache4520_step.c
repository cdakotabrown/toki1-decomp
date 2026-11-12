/* Function: dec_bg_cache4520_step */
/* Overlay: DATE */
/* Address: 0x80150B90 */
/* Size: 48 bytes (12 instructions) */
#include "types.h"

extern s32 device_cmd_send();
extern void dec_bg_cache_step();
extern s32 sub_sub_step_up();

void dec_bg_cache4520_step(void) {
    device_cmd_send(515);
    dec_bg_cache_step(17696);
    sub_sub_step_up();
}
