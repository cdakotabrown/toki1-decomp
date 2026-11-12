/* Address: 0x801378B8 */
/* Size: 128 bytes (32 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern u8 cdq_footer_chk;

void shougatu_device_load_cmd35_36_46(void)
{
    s32 ret;

    cdq_footer_chk = 0;
    device_load_start(0, 0xbfa6, 0xbf87, 0xd997, 0xd957, 0xd94f);
    device_cmd_send(35);
    device_cmd_send(36);
    device_cmd_send(46);
    ret = device_load_poll();
    if (ret == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
