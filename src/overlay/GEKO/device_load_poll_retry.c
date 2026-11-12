/* @PostProc: patch_insn_reg_field 0x44:rt:t6:v0 0x4c:rs:t6:v0 0x74:rt:t7:t6 0x7c:rs:t7:t6 */
/* device_load_poll_retry -- GEKO overlay, Address: 0x80134614, Size: 128 bytes */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern s32 device_state_init(void);
extern s32 device_load_poll(void);
extern void CDQ_init(void);

extern s32 step_counter;
extern u8 now_sub_sub_step;

void device_load_poll_retry(void)
{
    s32 count;
    s32 cond;

    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }

    count = step_counter;
    cond = (u32)count < 1024U;
    cond ^= 1;
    step_counter = count + 1;
    if (cond) {
        CDQ_init();
        device_state_init();
        goto_sub_sub_step((u8)(now_sub_sub_step - 1));
    }
}
