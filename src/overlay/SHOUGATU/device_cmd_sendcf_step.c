/* Address: 0x80137890 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_sendcf_step(void)
{
    device_cmd_send(207);
    sub_sub_step_up();
}
