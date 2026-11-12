/* Address: 0x8013A354 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_send502_step(void)
{
    device_cmd_send(1282);
    sub_sub_step_up();
}
