/* geko_serifuev_in2_step -- GEKO overlay, Address: 0x8013F9C0, Size: 84 bytes (21 insns) */
#include "types.h"

extern void geko_serifuev_run(void);
extern void device_cmd_send(s32 arg);

extern u16 now_serifuev_in;
extern s32 step_counter;

s32 geko_serifuev_in2_step(void)
{
    s32 val;
    s32 cond;

    geko_serifuev_run();
    if (now_serifuev_in == 2) {
        val = step_counter;
        cond = (u32)val < 1U;
        val = val + 1;
        step_counter = val;
        if (cond != 0) {
            device_cmd_send(1539);
        }
    }
}
