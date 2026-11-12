/* Address: 0x80138354 */
#include "types.h"

extern void device_cmd_send(s32);
extern void shougatu_date_serifu_run(void);
extern s32 step_counter;
extern s16 shougatu_date_now_serifu_in;

void
shougatu_serifu_cmd500_step(void)
{
    s32 cnt;
    s32 v0;

    shougatu_date_serifu_run();
    cnt = step_counter;
    v0 = (cnt < 1U);
    step_counter = cnt + 1;
    if (v0 == 0) {
        return;
    }
    if (shougatu_date_now_serifu_in != 3) {
        return;
    }
    device_cmd_send(1280);
}
