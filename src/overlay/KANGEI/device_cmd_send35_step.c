/* device_cmd_send35_step - KANGEI overlay, Address: 0x801357E0, Size: 56 bytes (14 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void device_cmd_send35_step(void)
{
    device_cmd_send(35);
    device_cmd_send(36);
    device_cmd_send(46);
    sub_sub_step_up();
}
