#include "types.h"

extern void device_load_start(s32 a0, u32 a1, u32 a2, u32 a3, u32 stack0, u32 stack1);
extern void sub_sub_step_up(void);

extern s8 cdq_footer_chk;

void geko_girl10_ev7_device_load(void)
{
    cdq_footer_chk = 0;
    device_load_start(1, 0xC66D, 0xC667, 0xD9EF, 0xD9A1, 0xD99C);
    sub_sub_step_up();
}
