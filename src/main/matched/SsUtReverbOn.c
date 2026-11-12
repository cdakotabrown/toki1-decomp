/* Address: 0x8008F9D0 - 0x8008F9F0 (32 bytes) */
/* Region: IDO (>= 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern void SpuSetReverb(s32 arg0);

void SsUtReverbOn(void) {
    SpuSetReverb(1);
}
