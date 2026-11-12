/* device_cmd_send200_step -- GEKO overlay, Address: 0x80132598, Size: 40 bytes (10 insns) */
#include "types.h"

extern void device_cmd_send(s32 a0);
extern void sub_sub_step_up(void);

void device_cmd_send200_step(void)
{
    device_cmd_send(512);
    sub_sub_step_up();
}
