/* Function: DecDCToutCallback */
/* Address: 0x80086B24 */
/* Size: 36 bytes (9 instructions) */
/* Region: Library (0x80086000+) */
/* Likely Symbol: MDEC_reset (from o.bin, offset +120 bytes from 0x80086B9C) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count 19 */
#include "types.h"

extern void DMACallback(s32 arg0, s32 arg1);

void DecDCToutCallback(s32 arg0) {
    DMACallback(1, arg0);
}
