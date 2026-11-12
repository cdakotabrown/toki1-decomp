/* Function: SD_VABTransfer */
/* Address: 0x80088E68 */
/* Size: 96 bytes (24 instructions) */
/* Region: Library (0x80086000+) */
/* Status: BLOCKED - ASM-only (confirmed unmatchable from C) */
/* CONFIRMED UNMATCHABLE FROM C - GCC INVESTIGATION (2026-04-01): */
/* ASM-ONLY MATCH: This function must be matched via handwritten assembly. */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: convert_or_to_addu */
/* @Measured: 2026-09-06 1/24 exact (4.2%), compiled 23 vs target 24 words, Fixup 0 -- SEMI */
#include "types.h"

extern s32 StRingAddr;
extern s32 StRingIdx3;

s32 SD_VABTransfer(s16 count, s32 start) {
    s32 i;
    s32 a;
    a = (s32)count;
    i = 0;
    if (a > 0) {
        do {
            *(s16 *)(StRingAddr + ((i + start) << 5)) = 0;
            i++;
        } while (i < a);
    }
    StRingIdx3 = i + start;
    return 0;
}
