/* Address: 0x80042AF0, 8 insns. */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: scheduler_normalizer */
#include "types.h"
extern u8  now_sub_sub_step;
extern s32 step_counter;
extern u8  step_mode;
s32 goto_sub_sub_step(s32 a0) {
    now_sub_sub_step = (u8)a0;
    step_counter = 0;
    step_mode = 0;
    return 0;
}
