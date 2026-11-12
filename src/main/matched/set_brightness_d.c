/* Address: 0x8004932C */
/* Size: 12 bytes (3 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 bri_step;

void set_brightness_d(u8 val) {
    bri_step = val;
}
