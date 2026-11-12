/* @PostProc: patch_insn_reg_field 0x4c:rt:t6:v0 0x54:rs:t6:v0 0x7c:rt:t7:t6 0x84:rs:t7:t6 */
/* date_device_load_wait512 -- DATE overlay, Address: 0x8014C634, Size: 136 bytes (34 insns) */
#include "types.h"

extern s32 device_load_poll(void);
extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);
extern void CDQ_init(void);
extern void device_state_init(void);
extern void goto_sub_sub_step(s32);

extern s32 step_counter;
extern u8 now_sub_sub_step;

void date_device_load_wait512(void)
{
    s32 v1;
    u32 cmp;

    if (device_load_poll() == 1) {
        device_cmd_send(512);
        sub_sub_step_up();
    }
    v1 = step_counter;
    cmp = ((u32)v1 < 1024U) ^ 1;
    v1++;
    step_counter = v1;
    if (cmp != 0) {
        CDQ_init();
        device_state_init();
        goto_sub_sub_step((u8)(now_sub_sub_step - 1));
    }
}
