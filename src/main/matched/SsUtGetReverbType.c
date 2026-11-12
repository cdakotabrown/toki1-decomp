/* Address: 0x8008FAC4 */
/* Size: 16 bytes (4 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s16 libsnd_reverb_type;

s32 SsUtGetReverbType(void) {
    return libsnd_reverb_type;
}
