/* Address: 0x80066C70 */
/* Size: 200 bytes (50 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

s32 get_vacation(s32 kind, s32 value) {
    if (kind == 8) {
        return 1;
    }
    if (kind == 7 && value >= 24) {
        return 1;
    }
    if (kind == 12 && value >= 25) {
        return 2;
    }
    if (kind == 13 && value < 8) {
        return 2;
    }
    if (kind == 1 && value < 8) {
        return 2;
    }
    if (kind == 3 && value >= 24) {
        return 3;
    }
    if (kind == 4 && value < 4) {
        return 3;
    }
    return 0;
}
