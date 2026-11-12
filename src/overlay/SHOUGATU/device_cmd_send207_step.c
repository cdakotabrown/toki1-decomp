/* Address: 0x80142674 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_send207_step(void)
{
    device_cmd_send(519);
    sub_sub_step_up();
}
