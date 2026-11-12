/* @CompileEnv: IDO52_UGEN_TEMP_REG=1:v0 IDO52_UGEN_TEMP_REG_INPLACE=1 */
/* geko_serifuev_in9_step -- GEKO overlay, Address: 0x8013E48C, Size: 92 bytes */
#include "types.h"

extern void bustup_face_change(s32 arg);
extern void geko_serifuev_run(void);

extern u16 now_serifuev_in;
extern s32 step_counter;

s32 geko_serifuev_in9_step(void)
{
    s32 val;
    s32 cond;
    s32 newval;

    geko_serifuev_run();
    val = step_counter;
    cond = (u32)(val ^ 1) < 1U;
    newval = val + 1;
    step_counter = newval;
    if (cond != 0) {
        if (now_serifuev_in == 9) {
            bustup_face_change(0);
        }
    }
}
