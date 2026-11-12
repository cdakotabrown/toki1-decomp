/* Address: 0x80141C68 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_cmd_send();

void device_cmd_send208_step(void)
{
    device_cmd_send(520);
    sub_sub_step_up();
}
