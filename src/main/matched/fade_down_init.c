/* Address: 0x8007E74C, Size: 44 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8  bg_change_tbl2;
extern s32 step_counter;
extern s32 fade_flag;
void fade_down_init(void) {
    bg_change_tbl2 = 0x80;
    fade_flag = 0;
    step_counter = step_counter + 1;
}
