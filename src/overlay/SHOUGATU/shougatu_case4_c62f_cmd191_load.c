/* Address: 0x80140A60 */
/* Size: 104 bytes (26 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern s32 device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);

void shougatu_case4_c62f_cmd191_load(void)
{
    s32 ret;

    device_cmd_send(191);
    device_load_start(1, 0xc62f, 0xc621, 0xd949, 0xd8fe, 0xd8de);
    ret = device_load_poll();
    if (ret == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
