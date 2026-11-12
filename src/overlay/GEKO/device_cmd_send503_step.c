/* device_cmd_send503_step -- GEKO overlay, Address: 0x8013BED0, Size: 48 bytes (12 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void device_cmd_send503_step(void)
{
    device_cmd_send(0x24);
    device_cmd_send(0x503);
    sub_sub_step_up();
}
