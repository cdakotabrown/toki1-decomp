/* @PostProc: patch_insn_reg_field 0x40:rt:t7:v0 */
/* @PostProc: patch_insn_reg_field 0x48:rs:t7:v0 */
#include "types.h"

extern void device_cmd_send(s32 arg);
extern void shougatu_date_serifu_run(void);

extern s16 shougatu_date_now_serifu_in;
extern s32 step_counter;

void shougatu_serifu_cmd501_step(void)
{
    s32 v0;
    s32 v1;

    shougatu_date_serifu_run();
    if ((s32)shougatu_date_now_serifu_in != 2) return;
    v1 = step_counter;
    v0 = (u32)(v1 ^ 1) < 1U;
    step_counter = v1 + 1;
    if (v0 == 0) return;
    device_cmd_send(36);
    device_cmd_send(1281);
}
