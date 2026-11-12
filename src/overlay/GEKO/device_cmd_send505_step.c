/* device_cmd_send505_step -- GEKO overlay, Address: 0x80139B70, Size: 48 bytes (12 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void device_cmd_send505_step(void)
{
    device_cmd_send(0x505);
    device_cmd_send(0x506);
    sub_sub_step_up();
}
