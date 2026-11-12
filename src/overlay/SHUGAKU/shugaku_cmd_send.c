#include "types.h"

extern u8 shugaku_place;

extern void device_cmd_send(s32 a0);
extern void sub_sub_step_up(void);

void shugaku_cmd_send(void)
{
    u8 flag;

    device_cmd_send(35);
    device_cmd_send(36);
    device_cmd_send(46);

    flag = shugaku_place;
    switch (flag) {
    case 0:
        device_cmd_send(512);
        break;
    case 1:
        device_cmd_send(513);
        break;
    default:
        device_cmd_send(514);
        break;
    }
    sub_sub_step_up();
}
