/* date2_device_cmd_step2 - DATE2 overlay, Address: 0x80132A48, Size: 88 bytes (22 insns) */
#include "types.h"

extern s32 step_counter;
extern s16 date2_now_s_in;

void date2_serifu_run1(void);
void device_cmd_send(s32 arg0);

void date2_device_cmd_step2(void)
{
    s32 v1;
    s32 cond;

    date2_serifu_run1();
    v1 = step_counter;
    cond = (u32)v1 < 1U;
    step_counter = v1 + 1;
    if (cond != 0) {
        if (date2_now_s_in == 1) {
            device_cmd_send(1547);
        }
    }
}
