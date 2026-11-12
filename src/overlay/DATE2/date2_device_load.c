/* date2_device_load - DATE2 overlay, Address: 0x80132AA0, Size: 104 bytes (26 insns) */
#include "types.h"

extern u8 cdq_footer_chk;
void device_load_start(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
s32 device_load_poll(void);
void sub_sub_step_up(void);

void date2_device_load(void)
{
    s32 v0;

    cdq_footer_chk = 0;
    device_load_start(1, 0xC665, 0xC631, 0xDAAD, 0xDA5D, 0xDA4B);
    v0 = device_load_poll();
    if (v0 == 1)
    {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
