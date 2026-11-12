/* Address: 0x80099360 */
/* Size: 16 bytes (4 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s16 PSDIDX;

s32 GsGetActiveBuff(void) {
    return (s32)PSDIDX;
}
