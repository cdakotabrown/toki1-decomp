/* device_cmd_send500_step -- GEKO overlay, Address: 0x8013BD24, Size: 40 bytes (10 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void device_cmd_send500_step(void)
{
    device_cmd_send(1280);
    sub_sub_step_up();
}
