/* device_cmd_send515_step - KANGEI overlay, Address: 0x801361F8, Size: 64 bytes (16 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void device_cmd_send515_step(void)
{
    device_cmd_send(35);
    device_cmd_send(36);
    device_cmd_send(46);
    device_cmd_send(515);
    sub_sub_step_up();
}
