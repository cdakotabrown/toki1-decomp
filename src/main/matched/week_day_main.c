/* Address: 0x80073A54 */
/* Size: 64 bytes (16 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern s32 step_counter;
extern u8 now_sub_sub_step;
extern void week_day_main0(void);

void week_day_main(void) {
    step_counter = step_counter + 1;
    if (now_sub_sub_step == 0) {
        week_day_main0();
    }
}
