#include "types.h"

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

extern u8 now_girl;

void
kangei_girl_cmd_send_step(void)
{
    u8 idx;

    device_cmd_send(35);
    device_cmd_send(36);
    device_cmd_send(46);
    idx = now_girl;
    if ((u32)idx < 4) {
        device_cmd_send(514);
    } else {
        device_cmd_send(513);
    }
    sub_sub_step_up();
}
