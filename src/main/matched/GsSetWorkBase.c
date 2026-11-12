/* Address: 0x800995D0 */
/* Size: 12 bytes (3 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 GsOUT_PACKET_P;

void GsSetWorkBase(s32 a0) {
    GsOUT_PACKET_P = a0;
}
