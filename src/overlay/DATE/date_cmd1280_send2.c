/* Address: 0x8015A160 */
/* Size: 32 bytes (8 instructions) */
#include "types.h"

extern void device_cmd_send(s32);

void date_cmd1280_send2(void)
{
    device_cmd_send(1280);
}
