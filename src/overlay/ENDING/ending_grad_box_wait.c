/* @SymbolSize: 124 */
/* @PostProc: patch_insn_reg_field 0x64:rt:t9:v0 0x6C:rs:t9:v0 */
/* ending_grad_box_wait - ENDING overlay, Address: 0x80137518, Size: 124 bytes (31 insns) */
#include "types.h"

extern void grad_box_disp();
extern void dtd_on();
extern void sub_sub_step_up();

extern s32 step_counter;

void ending_grad_box_wait(void)
{
    s32 v1;
    s32 cond;

    grad_box_disp(-160, -120, 320, 240, 14, (s32)0x00C0C0FFU, (s32)0x004080FFU);
    dtd_on(14);

    v1 = step_counter;
    cond = (u32)v1 < 129U;
    cond = cond ^ 1;
    v1 += 1;
    step_counter = v1;
    if (cond != 0) {
        sub_sub_step_up();
    }
}
