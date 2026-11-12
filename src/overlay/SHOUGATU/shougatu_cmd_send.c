/* Address: 0x801432EC */
/* Size: 32 bytes (8 instructions) */
#include "types.h"

extern void device_cmd_send();

void shougatu_cmd_send(void)
{
    device_cmd_send(1284);
}
