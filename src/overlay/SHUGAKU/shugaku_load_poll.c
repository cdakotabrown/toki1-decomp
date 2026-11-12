#include "types.h"

extern u8 cdq_footer_chk;

extern s32  device_load_start(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern s32  device_load_poll(void);
extern void sub_sub_step_up(void);

void shugaku_load_poll(void)
{
    s32 ret;

    cdq_footer_chk = 0;
    device_load_start(1, 0xC520, 0xC4F5, 0xD565, 0xD516, 0xD4FF);
    ret = device_load_poll();
    if (ret == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
