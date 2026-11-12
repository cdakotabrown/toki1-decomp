/* Address: 0x8007A6A8 */
/* Size: 148 bytes (37 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s16 serial_vol_base;

void serial_vol_base_set(s32 a0) {
    serial_vol_base = 100;
    switch (a0) {
    case 0:
        return;
    case 6:
        serial_vol_base = 125;
        return;
    case 7:
        serial_vol_base = 95;
        return;
    case 9:
    case 11:
        serial_vol_base = 95;
        return;
    case 10:
        serial_vol_base = 90;
        return;
    case 12:
    case 13:
        serial_vol_base = 110;
        return;
    case 15:
        serial_vol_base = 105;
        return;
    }
}
