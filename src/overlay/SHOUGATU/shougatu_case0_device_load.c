/* Address: 0x8013D7A4 */
/* Size: 96 bytes (24 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);

void shougatu_case0_device_load(void)
{
    s32 ret;

    device_load_start(1, 0xc62f, 0xc621, 0xd949, 0xd8fe, 0xd8de);
    ret = device_load_poll();
    if (ret == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
