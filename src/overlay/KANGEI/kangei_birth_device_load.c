/* kangei_birth_device_load - KANGEI overlay, Address: 0x80138B30, Size: 104 bytes (26 insns) */
#include "types.h"

extern s32 device_load_poll(void);
extern void sub_sub_step_up(void);
extern void device_load_start(s32, u32, u32, u32, u32, u32);

extern u8 cdq_footer_chk;

void kangei_birth_device_load(void)
{
    cdq_footer_chk = 0;
    device_load_start(1, 0xBFA6, 0xBF87, 0xCAA3, 0xCA5D, 0xCA4C);
    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
