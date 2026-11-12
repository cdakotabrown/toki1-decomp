/* Address: 0x8013B72C */
/* Size: 104 bytes (26 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);

void shougatu_girl12_device_load_cmd514(void)
{
    s32 ret;

    device_load_start(1, 0xbfa6, 0xbf87, 0xcaa3, 0xca5d, 0xca4c);
    ret = device_load_poll();
    if (ret == 1) {
        device_cmd_send(514);
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
