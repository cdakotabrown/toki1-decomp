/* Function: ending_step_check */
/* Address: 0x800795E0 */
/* Size: 36 bytes (9 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 now_step;

s32 ending_step_check(void)
{
    if (now_step == 144) {
        return 1;
    }
    return 0;
}
