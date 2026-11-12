/* device_cmd_send200_step3 -- GEKO overlay, Address: 0x80142B68, Size: 40 bytes (10 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void device_cmd_send200_step3(void)
{
    device_cmd_send(512);
    sub_sub_step_up();
}
