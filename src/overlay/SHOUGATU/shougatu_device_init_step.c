/* Address: 0x801379D0 */
/* Size: 44 bytes (11 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void device_port_init();

void shougatu_device_init_step(void)
{
    device_port_init(1, 512);
    sub_sub_step_up();
}
