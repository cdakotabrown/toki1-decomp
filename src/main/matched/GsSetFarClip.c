/* Address: 0x8009CBC0, Size: 12 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 GsCLIP3far;

void GsSetFarClip(s32 a) {
    GsCLIP3far = a;
}
