/* Address: 0x80044A4C */
/* Size: 16 bytes (4 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 device_load_start_tbl6;

u32 device_port_get(void) {
    return device_load_start_tbl6;
}
