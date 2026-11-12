/* Address: 0x800651E8 */
/* Size: 44 bytes (11 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 icon_can_use[];

void icon_can_use_set(s32 idx, s32 on) {
    if (on != 0) {
        icon_can_use[idx] = 1;
    } else {
        icon_can_use[idx] = 0;
    }
}
