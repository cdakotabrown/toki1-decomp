/* geko_girl11_ev4_device_load -- GEKO overlay, Address: 0x801418F0, Size: 96 bytes */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);

void geko_girl11_ev4_device_load(void)
{
    device_load_start(1, 0xC051, 0xC02B, 0xCF0B, 0xCEC2, 0xCEBF);
    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
