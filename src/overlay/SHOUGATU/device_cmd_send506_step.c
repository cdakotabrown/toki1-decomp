/* Address: 0x801410D4 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_send506_step(void)
{
    device_cmd_send(1286);
    sub_sub_step_up();
}
