/* @PostProc: swap_registers v1:v0 */
/* Address: 0x80145F94 */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern void back_clear_switch();
extern void white_light();
extern s32 step_counter;
extern u8 step_mode;

void etc_white_light_step(void)
{
    s32 val;

    back_clear_switch(1);

    if (step_mode == 0) {
        return;
    }

    val = step_counter;
    if ((u32)val >= 255) {
        return;
    }

    white_light(255 - val);
}
