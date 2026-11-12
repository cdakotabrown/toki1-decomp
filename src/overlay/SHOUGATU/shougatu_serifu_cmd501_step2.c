/* @PostProc: rename_register_chain --chain t7:v0 */
/* Address: 0x80139D18 */
#include "types.h"

extern void device_cmd_send(s32);
extern void shougatu_date_serifu_run(void);
extern s32 step_counter;
extern s16 shougatu_date_now_serifu_in;

void
shougatu_serifu_cmd501_step2(void)
{
    s32 cnt;
    s32 v0;

    shougatu_date_serifu_run();
    if (shougatu_date_now_serifu_in != 2) {
        return;
    }
    cnt = step_counter;
    v0 = ((cnt ^ 1) < 1U);
    step_counter = cnt + 1;
    if (v0 == 0) {
        return;
    }
    device_cmd_send(1281);
}
