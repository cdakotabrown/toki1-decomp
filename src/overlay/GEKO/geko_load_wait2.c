/* @PostProc: swap_registers t6:v0 */
/* @PostProc: swap_registers t7:t6 */
/* geko_load_wait2 -- GEKO overlay, Address: 0x80138D1C, Size: 128 bytes */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 goto_sub_sub_step(s32 a0);
extern s32 device_state_init(void);
extern s32 device_load_poll(void);
extern void CDQ_init(void);

extern s32 step_counter;
extern u8 now_sub_sub_step;

void geko_load_wait2(void)
{
    s32 val;
    s32 cond;

    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    val = step_counter;
    cond = (u32)val < 1024;
    step_counter = val + 1;
    cond ^= 1;
    if (cond != 0) {
        CDQ_init();
        device_state_init();
        goto_sub_sub_step((u8)(now_sub_sub_step - 1));
    }
}
