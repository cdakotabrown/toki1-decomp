/* kangei_device_load2 - KANGEI overlay, Address: 0x80136F98, Size: 76 bytes (19 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void device_load_start(s32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5);
extern void sub_sub_step_up(void);

void
kangei_device_load2(void)
{
    device_cmd_send(191);
    device_load_start(1, 0xBFA6, 0xBF87, 0xC990, 0xC943, 0xC929);
    sub_sub_step_up();
}
