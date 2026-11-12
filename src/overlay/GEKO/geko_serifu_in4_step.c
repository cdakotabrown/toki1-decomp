/* @CompileEnv: IDO52_UGEN_TEMP_REG=1:v0 IDO52_UGEN_TEMP_REG_INPLACE=1 */
/* geko_serifu_in4_step -- GEKO overlay, Address: 0x80135A28, Size: 92 bytes */
#include "types.h"

extern void device_cmd_send(s32 arg);
extern void geko_serifu_run2(void);

extern s32 step_counter;
extern s16 geko_now_serifu2_in;

s32 geko_serifu_in4_step(void)
{
    s32 val;
    s32 cond;

    geko_serifu_run2();
    val = step_counter;
    cond = (u32)(val ^ 1) < 1U;
    val = val + 1;
    step_counter = val;
    if (cond != 0) {
        if (geko_now_serifu2_in == 4) {
            device_cmd_send(1548);
        }
    }
}
