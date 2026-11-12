/* Address: 0x800429FC */
/* Size: 44 bytes (11 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_instructions 0x20 0x28 */
#include "types.h"

extern u8 now_sub_sub_step;
extern s32 step_counter;
extern u8 step_mode;

s32 sub_sub_step_up(void) {
    now_sub_sub_step++;
    step_counter = 0;
    step_mode = 0;
    return 0;
}
