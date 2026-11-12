/* Address: 0x8008F9F0 - 0x8008FA10 (32 bytes) */
/* Region: IDO (>= 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern void SpuSetReverb(s32 arg0);

void SsUtReverbOff(void) {
    SpuSetReverb(0);
}
