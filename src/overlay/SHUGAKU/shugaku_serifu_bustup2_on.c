/* @PostProc: patch_insn_reg_field 0x30:rt:t6:v0 */
/* @PostProc: patch_insn_reg_field 0x38:rs:t6:v0 */
/* @PostProc: patch_insn_reg_field 0x40:rt:t7:t6 */
/* @PostProc: patch_insn_reg_field 0x44:rs:t7:t6 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t7:t6 */
/* @PostProc: patch_insn_reg_field 0x4c:rs:t7:t6 */
#include "types.h"

extern s32 step_counter;
extern s16 move_in;
extern s32 wink2_stop;

extern void func_801343A0(void);
extern void bustup2_flag_on(void);

void shugaku_serifu_bustup2_on(void)
{
    s32 v1;
    u32 cond;

    func_801343A0();
    v1 = step_counter;
    cond = (u32)(v1 ^ 1) < 1U;
    step_counter = v1 + 1;
    if (cond == 0) {
        return;
    }
    if (move_in != 2) {
        return;
    }
    wink2_stop = 0;
    bustup2_flag_on();
}
