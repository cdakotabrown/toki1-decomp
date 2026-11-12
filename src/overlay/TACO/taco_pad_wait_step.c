/* @SymbolStart: 0x80135230 */
/* @SymbolSize: 88 */
/* Address: 0x80135238 */
/* Size: 80 bytes (20 insns) */
#include "types.h"

extern s32 step_counter;

extern s32 pad_trig;

extern void sub_sub_step_up();
extern void goto_sub_sub_step();

void taco_pad_wait_step(void)
{
    s32 arg0 = step_counter;

    if ((u32)arg0 >= 209U) {
        sub_sub_step_up();
        return;
    }

    if (pad_trig & 0x860) {
        goto_sub_sub_step(5);
    }
}
