/* Function: DecDCTinCallback */
/* Address: 0x80086B00 */
/* Size: 36 bytes (9 instructions) */
/* Region: Library (0x80086000+) */
/* Likely Symbol: MDEC_reset (from o.bin, offset +156 bytes from 0x80086B9C) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count 19 */
#include "types.h"

extern void DMACallback(s32 arg0, s32 arg1);

void DecDCTinCallback(s32 arg0) {
    DMACallback(0, arg0);
}
