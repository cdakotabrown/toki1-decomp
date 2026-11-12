/* Address: 0x80151160 */
/* Size: 32 bytes (8 instructions) */
#include "types.h"

extern void device_cmd_send(s32);

void date_cmd1283_send(void)
{
    device_cmd_send(1283);
}
