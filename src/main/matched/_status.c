/* Address: 0x800A14EC */
/* Size: 24 bytes (6 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 *GPU_status;

s32 _status(void) {
    return *GPU_status;
}
