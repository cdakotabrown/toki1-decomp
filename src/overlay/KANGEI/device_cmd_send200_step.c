/* device_cmd_send200_step - KANGEI overlay, Address: 0x801324B0, Size: 40 bytes (10 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void
device_cmd_send200_step(void)
{
    device_cmd_send(512);
    sub_sub_step_up();
}
