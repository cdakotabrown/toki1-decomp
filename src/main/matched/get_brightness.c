/* Address: 0x80049294, Size: 16 bytes (4 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 screen_bri;

u32 get_brightness(void) {
    return screen_bri;
}
