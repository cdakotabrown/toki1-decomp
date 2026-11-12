/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x2c */
/* @SymbolStart: 0x80132048 */
/* @SymbolSize: 80 */
/* Address: 0x80132050 */
/* Size: 72 bytes (18 instructions) */
#include "types.h"

extern void goto_step();
extern u32 pad_trig;
extern s32 frame_counter;

void goto_step_on_down_or_timer(s32 a0)
{
    u32 t7_in;

    a0 = a0 & 0xff;
    t7_in = pad_trig;
    if ((t7_in & 0x0040) == 0) {
        if ((frame_counter & 0x02FF) != 767) {
            return;
        }
    }

    goto_step(a0);
}
