/* Address: 0x801432C4 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_send205_step(void)
{
    device_cmd_send(517);
    sub_sub_step_up();
}
