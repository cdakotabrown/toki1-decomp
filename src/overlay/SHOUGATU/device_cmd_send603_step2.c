/* Address: 0x80133134 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_send603_step2(void)
{
    device_cmd_send(1539);
    sub_sub_step_up();
}
