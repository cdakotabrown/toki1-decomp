/* device_cmd_send200_step - DATE2 overlay, Address: 0x80132B88, Size: 40 bytes (10 insns) */
#include "types.h"

void device_cmd_send(s32 arg0);
void sub_sub_step_up(void);

void device_cmd_send200_step(void)
{
    device_cmd_send(512);
    sub_sub_step_up();
}
