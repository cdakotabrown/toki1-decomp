/* Address: 0x801383AC */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_send500_step(void)
{
    device_cmd_send(1280);
    sub_sub_step_up();
}
