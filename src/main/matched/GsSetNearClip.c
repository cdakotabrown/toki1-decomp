/* Address: 0x8009CBB0, Size: 12 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 GsCLIP3near;

void GsSetNearClip(s32 a) {
    GsCLIP3near = a;
}
