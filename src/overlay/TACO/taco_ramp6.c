/* @CompileEnv: IDO52_AS1_PASS1_VOLATILE_OK=2,3 */
#include "types.h"

extern volatile u8 step_mode;
extern s32 taco_drop_bright;

extern s32 sub_sub_step_up(void);
extern void taco_drop(s32, s32);

s32 taco_ramp6(void)
{
    s32 val;

    taco_drop(1, 0);
    val = step_mode;
    taco_drop_bright = 128 - val * 3;
    step_mode = val = (val + 1) & 0xff;
    if ((u32)(val * 3) >= 129U) {
        sub_sub_step_up();
    }
}
