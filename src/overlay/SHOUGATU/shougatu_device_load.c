/* Address: 0x801325CC */
/* Size: 104 bytes (26 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);

void shougatu_device_load(void)
{
    s32 ret;

    device_load_start(1, 0xc4f3, 0xc4d5, 0xd2a2, 0xd272, 0xd26c);
    ret = device_load_poll();
    if (ret == 1) {
        device_cmd_send(512);
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
