/* ending_device_load_start - ENDING overlay, Address: 0x80132E20, Size: 96 bytes (24 insns) */
#include "types.h"

extern s32 device_load_start();
extern s32 device_load_poll();
extern void sub_sub_step_up();

void ending_device_load_start(void)
{
        s32 ret;
    s32 sp16;
    s32 sp20;

    sp16 = 0xCE2C;
    sp20 = 0xCE41;
    device_load_start(1, 0xBFA6, 0xBF87, 0xCE7B, 0xCE41, 0xCE2C);
    {
        ret = device_load_poll();
        if (ret == 1) {
            sub_sub_step_up();
        }
        sub_sub_step_up();
    }
}
