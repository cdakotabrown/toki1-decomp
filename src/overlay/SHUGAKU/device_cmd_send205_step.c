/* device_cmd_send205_step - SHUGAKU overlay, Address: 0x80134E30, Size: 40 bytes (10 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void device_cmd_send205_step(void)
{
    device_cmd_send(0x205);
    sub_sub_step_up();
}
