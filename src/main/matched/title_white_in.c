/* Address: 0x80062330 */
/* Size: 112 bytes (28 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 step_counter;
extern s32 pad_trig;
extern void white_light(s32);
extern void sub_step_up(void);

void title_white_in(void) {
    white_light(255 - step_counter);
    if ((u32)step_counter >= 255) {
        sub_step_up();
    } else if (pad_trig & 0x860) {
        sub_step_up();
    }
}
