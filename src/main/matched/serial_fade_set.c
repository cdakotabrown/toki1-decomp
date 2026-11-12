/* Address: 0x8007ABB4 */
/* Size: 188 bytes (47 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 gSDSerialInfo;
extern s16 serial_fade_acc;
extern s16 serial_fade_lim;
extern s16 serial_fade_step;

void serial_fade_set(u16 code) {
    u8 c;

    if ((gSDSerialInfo & 0x100) == 0) {
        serial_fade_acc = 0;
        gSDSerialInfo |= 0x100;
    }

    c = (u8)code;
    if (c == 180) {
        goto c180;
    }
    if (c == 196) {
        goto c196;
    }
    if (c == 212) {
        goto c212;
    }
    serial_fade_lim = 256;
    serial_fade_step = 10;
    return;
c180:
    serial_fade_lim = 256;
    serial_fade_step = 10;
    return;
c196:
    serial_fade_lim = 256;
    serial_fade_step = 1;
    return;
c212:
    serial_fade_lim = 128;
    serial_fade_step = 1;
}
