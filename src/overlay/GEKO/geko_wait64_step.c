/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:v1,7:v0 IDO52_UGEN_TEMP_REG=1:v0 IDO52_UGEN_TEMP_REG_INPLACE=1 */
/* @SymbolStart: 0x8013A598 */
/* @SymbolSize: 88 */
/* geko_wait64_step -- GEKO overlay, Address: 0x8013A598, Size: 88 bytes (22 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32 arg);

extern volatile s32 step_counter;

s32 geko_wait64_step(void)
{
    s32 v1 = step_counter;
    s32 cond;

    if (v1 == 0) {
        device_cmd_send(1280);
        v1 = step_counter;
    }
    cond = (u32)(v1 ^ 64) < 1U;
    v1++;
    step_counter = v1;
    if (cond != 0) {
        sub_sub_step_up();
    }
}
