/* Address: 0x80141588 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_send505_step(void)
{
    device_cmd_send(1285);
    sub_sub_step_up();
}
