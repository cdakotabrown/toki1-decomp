/* Address: 0x80134F10 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_send201_step(void)
{
    device_cmd_send(513);
    sub_sub_step_up();
}
