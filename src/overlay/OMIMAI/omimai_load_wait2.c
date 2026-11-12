/* @PostProc: swap_registers t6:v0 */
/* @PostProc: swap_registers t7:t6 */
/* omimai_load_wait2 - OMIMAI overlay, Address: 0x8013364C, Size: 128 bytes (32 insns) */
#include "types.h"

extern s32 step_counter;
extern u8 now_sub_sub_step;
extern s32 device_load_poll(void);
extern void sub_sub_step_up(void);
extern void CDQ_init(void);
extern void device_state_init(void);
extern void goto_sub_sub_step(s32);

void omimai_load_wait2(void)
{
    s32 v1;
    s32 cond;

    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    v1 = step_counter;
    cond = (u32)v1 < 1024u;
    cond ^= 1;
    step_counter = v1 + 1;
    if (cond == 0) {
        return;
    }
    CDQ_init();
    device_state_init();
    goto_sub_sub_step((u8)(now_sub_sub_step - 1));
}
