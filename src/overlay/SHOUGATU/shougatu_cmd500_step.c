/* Address: 0x80137C9C */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void shougatu_cmd500_step(void)
{
    device_cmd_send(35);
    device_cmd_send(36);
    device_cmd_send(1280);
    sub_sub_step_up();
}
