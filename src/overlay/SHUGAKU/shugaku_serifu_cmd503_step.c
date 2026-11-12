/* @PostProc: patch_insn_reg_field 0x40:rt:t7:v0 0x48:rs:t7:v0 */
#include "types.h"

extern s32 step_counter;
extern s16 move_in;

extern void func_801343A0(void);
extern void device_cmd_send(s32 a0);

void shugaku_serifu_cmd503_step(void)
{
    s32 v1;
    s32 eq;

    func_801343A0();
    if (move_in != 1) {
        return;
    }
    v1 = step_counter;
    eq = (u32)(v1 ^ 1) < 1U;
    step_counter = v1 + 1;
    if (eq) {
        device_cmd_send(1283);
    }
}
