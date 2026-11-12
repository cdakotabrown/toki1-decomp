/* Address: 0x800492A4, Size: 24 bytes (6 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 screen_bri;
extern u8 bri_step;

void clear_brightness(void) {
    screen_bri = 0x80;
    bri_step = 0;
}
