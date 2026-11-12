/* Address: 0x8013A6D8 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_send503_step(void)
{
    device_cmd_send(1283);
    sub_sub_step_up();
}
