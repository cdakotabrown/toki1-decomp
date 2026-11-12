/* @PostProc: patch_insn_reg_field 0x44:rt:t6:v0 0x4C:rs:t6:v0 0x74:rt:t7:t6 0x7C:rs:t7:t6 */
/* kangei_girl_device_wait - KANGEI overlay, Address: 0x80132430, Size: 128 bytes (32 insns) */
#include "types.h"

extern s32 device_load_poll(void);
extern void sub_sub_step_up(void);
extern s32 CDQ_init(void);
extern void device_state_init(void);
extern void goto_sub_sub_step(s32);

extern s32 step_counter;
extern u8 now_sub_sub_step;

void kangei_girl_device_wait(void)
{
    s32 v1;
    s32 a0;
    s32 cond;

    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    v1 = step_counter;
    cond = ((u32)v1 < 1024U);
    cond = cond ^ 1;
    step_counter = v1 + 1;
    if (cond == 0) return;
    CDQ_init();
    device_state_init();
    a0 = (u8)now_sub_sub_step - 1;
    goto_sub_sub_step(a0 & 0xFF);
}
