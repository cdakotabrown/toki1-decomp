/* Address: 0x80132D88 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_send603_step(void)
{
    device_cmd_send(1539);
    sub_sub_step_up();
}
