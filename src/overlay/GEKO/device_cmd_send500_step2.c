/* device_cmd_send500_step2 -- GEKO overlay, Address: 0x8013FA74, Size: 40 bytes (10 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void device_cmd_send500_step2(void)
{
    device_cmd_send(1280);
    sub_sub_step_up();
}
