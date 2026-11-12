/* Function: scene_date_check */
/* Address: 0x80079604 */
/* Size: 36 bytes (9 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 now_step;

s32 scene_date_check(void) {
    if (now_step == 0x21) {
        return 1;
    }
    return 0;
}
