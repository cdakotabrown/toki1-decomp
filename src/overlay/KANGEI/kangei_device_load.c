/* kangei_device_load - KANGEI overlay, Address: 0x80133744, Size: 128 bytes (32 insns) */
#include "types.h"

extern void device_load_start(s32 a0, u32 a1, u32 a2, u32 a3, s32 stackArg0, s32 stackArg1);
extern s32 device_load_poll(void);
extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);

extern u8 cdq_footer_chk;

void kangei_device_load(void)
{
    s32 result;

    cdq_footer_chk = 0;
    device_load_start(1, 0xC07D, 0xC073, 0xCC41, 0xCBF1, 0xCBE7);
    result = device_load_poll();
    if (result == 1) {
        sub_sub_step_up();
    }
    device_cmd_send(34);
    device_cmd_send(33);
    device_cmd_send(47);
    sub_sub_step_up();
}
