/* Function: week_day_exit */
/* Address: 0x80073994 */
/* Size: 64 bytes (16 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers t6:t7 --only 0x0c,0x10 */
/* @PostProc: swap_registers v0 t6 */
/* @PostProc: swap_registers v1 t8 */
#include "types.h"

extern void week_day_exit0(void);
extern volatile s32 step_counter;
extern volatile u8 now_sub_sub_step;

void week_day_exit(void) {
    s32 val;
    s32 result;
    u8 flag;

    val = step_counter;
    result = val + 1;
    step_counter = result;
    flag = now_sub_sub_step;
    if (flag == 0) {
        week_day_exit0();
    }
}
