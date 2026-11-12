/* Address: 0x80137938 */
/* Size: 152 bytes (38 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern void device_cmd_send(s32);
extern void device_state_init(void);
extern s32 device_load_poll(void);
extern void CDQ_init(void);
extern s32 step_counter;
extern u8 now_sub_sub_step;

void shougatu_device_wait_cmd35_36_46(void)
{
    s32 ret;
    s32 counter;
    s32 check;
    u8 param;

    ret = device_load_poll();
    if (ret == 1) {
        device_cmd_send(35);
        device_cmd_send(36);
        device_cmd_send(46);
        sub_sub_step_up();
    }
    counter = step_counter;
    check = (u32)counter >= 1024;
    step_counter = counter + 1;
    if (check != 0) {
        CDQ_init();
        device_state_init();
        param = now_sub_sub_step;
        goto_sub_sub_step((u8)(param - 1));
    }
}
