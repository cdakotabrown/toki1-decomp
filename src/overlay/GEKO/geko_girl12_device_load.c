/* geko_girl12_device_load -- GEKO overlay, Address: 0x8013BA9C, Size: 112 bytes */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32 a0);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);

extern u8 cdq_footer_chk;

void geko_girl12_device_load(void)
{
    device_cmd_send(207);
    cdq_footer_chk = 0;
    device_load_start(0, 0, 0, 0xD997, 0xD957, 0xD94F);
    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
