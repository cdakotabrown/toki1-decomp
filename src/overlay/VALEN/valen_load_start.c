/* valen_load_start - VALEN overlay, Address: 0x8013244C, Size: 104 bytes (26 insns) */
#include "types.h"

extern s32 device_load_poll(void);
extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);
extern void device_load_start(s32, u32, u32, u32, u32, u32);

void valen_load_start(void)
{
    device_load_start(1, 0xBFA6, 0xBF87, 0xCAA3, 0xCA5D, 0xCA4C);
    if (device_load_poll() == 1) {
        device_cmd_send(513);
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
