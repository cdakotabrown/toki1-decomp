/* Address: 0x80044A3C, Size: 16 bytes (4 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 device_init_flag_get_tbl0;

u32 device_init_flag_get(void) {
    return device_init_flag_get_tbl0;
}
