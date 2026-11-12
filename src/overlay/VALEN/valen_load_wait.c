/* valen_load_wait - VALEN overlay, Address: 0x801324B4, Size: 136 bytes (34 insns) */
/* @PostProc: patch_insn_reg_field 0x4c:rt:t6:v0 */
/* @PostProc: patch_insn_reg_field 0x54:rs:t6:v0 */
/* @PostProc: patch_insn_reg_field 0x7c:rt:t7:t6 */
/* @PostProc: patch_insn_reg_field 0x84:rs:t7:t6 */
#include "types.h"

extern s32 step_counter;
extern u8 now_sub_sub_step;
extern s32 device_load_poll(void);
extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);
extern void CDQ_init(void);
extern void device_state_init(void);
extern void goto_sub_sub_step(s32);

void valen_load_wait(void)
{
    u32 old;
    u32 cond;

    if (device_load_poll() == 1) {
        device_cmd_send(513);
        sub_sub_step_up();
    }
    old = (u32)step_counter;
    cond = old < 1024U;
    cond = cond ^ 1;
    step_counter = (s32)(old + 1);
    if (cond != 0) {
        CDQ_init();
        device_state_init();
        goto_sub_sub_step((u8)(now_sub_sub_step - 1));
    }
}
