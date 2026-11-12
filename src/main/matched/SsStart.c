/* Likely Symbol: SsStart (from o.bin, offset +120 bytes from 0x8008BD54) */
/* Address: 0x8008BDCC - 0x8008BDEC (32 bytes) */
/* Region: IDO (>= 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern void _SsStart(s32 arg0);

void SsStart(void) {
    _SsStart(1);
}
