/* Address: 0x80065214 */
/* Size: 8 instructions */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 icon_can_use[];

s32 get_icon_can_use(s32 a0) {
    if (icon_can_use[a0] == 1) {
        return 1;
    }
    return 0;
}
