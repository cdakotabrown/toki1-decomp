/* Address: 0x8007F048 */
/* Size: 68 bytes (17 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 step_counter;
extern void sub_sub_step_up(void);

void step_wait(s16 arg0) {
    s32 counter;

    counter = step_counter + 1;
    step_counter = counter;
    if ((u32)counter >= (u32)(s16)arg0) {
        sub_sub_step_up();
    }
}
