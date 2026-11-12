#include "types.h"

extern void device_load_start(s32 a0, u32 a1, u32 a2, u32 a3, u32 stack0, u32 stack1);
extern void sub_sub_step_up(void);

extern s8 cdq_footer_chk;

void geko_girl0_ev8_device_load(void)
{
    cdq_footer_chk = 0;
    device_load_start(1, 0xC015, 0xC005, 0xCBB1, 0xCB61, 0xCB5C);
    sub_sub_step_up();
}
