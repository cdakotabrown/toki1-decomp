/* @CompileEnv: IDO52_UGEN_TEMP_REG=2:v0 IDO52_UGEN_TEMP_REG_INPLACE=1 */
/* geko_serifuev_in3_step -- GEKO overlay, Address: 0x80140E2C, Size: 88 bytes */
#include "types.h"

extern void device_cmd_send(s32 arg);
extern void geko_serifuev_run(void);

extern u16 now_serifuev_in;
extern s32 step_counter;

s32 geko_serifuev_in3_step(void)
{
    s32 val;
    s32 cond;

    geko_serifuev_run();
    if (now_serifuev_in == 3) {
        val = step_counter;
        cond = (u32)(val ^ 1) < 1U;
        val = val + 1;
        step_counter = val;
        if (cond != 0) {
            device_cmd_send(1281);
        }
    }
}
