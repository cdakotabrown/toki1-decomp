/* shugaku_girl5_cmd_send - SHUGAKU overlay, Address: 0x801345D8, Size: 56 bytes (14 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void shugaku_girl5_cmd_send(void)
{
    device_cmd_send(0x24);
    device_cmd_send(0x206);
    device_cmd_send(0x504);
    sub_sub_step_up();
}
