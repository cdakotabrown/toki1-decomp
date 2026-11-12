/* Function: title_main3 */
/* Address: 0x800629A8 */
/* Size: 100 bytes (25 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern u32 step_counter;
extern u8 now_sub_sub_step;
extern void title_fade_out(void);
extern void title_exit(void);

void title_main3(void) {
    step_counter++;
    {
        s32 mode = now_sub_sub_step;
        switch (mode) {
        case 0:
            title_fade_out();
            break;
        case 1:
            title_exit();
            break;
        }
    }
}
