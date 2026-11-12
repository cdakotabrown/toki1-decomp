/* device_cmd_send500_step3 -- GEKO overlay, Address: 0x80141888, Size: 48 bytes (12 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void device_cmd_send500_step3(void)
{
    device_cmd_send(0x24);
    device_cmd_send(0x500);
    sub_sub_step_up();
}
