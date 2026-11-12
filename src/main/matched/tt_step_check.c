/* Address: 0x800795B0, Size: 48 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8 now_step;
s32 tt_step_check(void) {
    u8 v = now_step;
    if (v == 0xD0 || v == 0xD1) {
        return 1;
    }
    return 0;
}
