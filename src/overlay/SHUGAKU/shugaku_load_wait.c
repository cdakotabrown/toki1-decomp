#include "types.h"

extern s32 step_counter;
extern u8  now_sub_sub_step;

extern s32  device_load_poll(void);
extern void sub_sub_step_up(void);
extern void CDQ_init(void);
extern void device_state_init(void);
extern void goto_sub_sub_step(s32 a0);

void shugaku_load_wait(void)
{
    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    if ((u32)step_counter++ >= 1024U) {
        CDQ_init();
        device_state_init();
        goto_sub_sub_step((s32)(u8)(now_sub_sub_step - 1));
    }
}
