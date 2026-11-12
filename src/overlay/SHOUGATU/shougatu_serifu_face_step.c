/* @PostProc: rename_register_chain --chain t7:t6 */
/* @PostProc: rename_register_chain --chain t6:v0 */
/* @PostProc: patch_insn_reg_field 0x40:rt:v0:t6 */
/* @PostProc: patch_insn_reg_field 0x44:rs:v0:t6 */
/* @PostProc: patch_insn_reg_field 0x44:rt:v0:t6 */
/* @PostProc: patch_insn_reg_field 0x4C:rs:v0:t6 */
#include "types.h"

extern void bustup_face_change(s32 arg);
extern void shougatu_date_serifu_run(void);

extern s16 shougatu_date_now_serifu_in;
extern s32 step_counter;

void shougatu_serifu_face_step(void)
{
    s32 v0;
    s32 v1;

    shougatu_date_serifu_run();
    v1 = step_counter;
    v0 = (u32)(v1 ^ 1) < 1U;
    step_counter = v1 + 1;
    if (v0 == 0) return;
    if ((s32)shougatu_date_now_serifu_in != 3) return;
    bustup_face_change(3);
}
