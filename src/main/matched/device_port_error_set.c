/* Address: 0x80044960 */
/* Size: 32 bytes (8 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8 device_load_poll_tbl1;
extern u8 device_err[];
void device_port_error_set(s32 a0) {
    device_err[device_load_poll_tbl1] = 1;
}
