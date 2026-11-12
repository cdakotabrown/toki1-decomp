/* Address: 0x800882B0 */
/* Size: 40 bytes (10 instructions) */
/* Region: IDO (Library, >= 0x80086000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_ra_offset 20 16 */
/* @PostProc: swap_instructions 0x4 0x8 */
/* Compiler: IDO 5.2 */
#include "types.h"

extern s32 StRingAddr;
extern s32 StRingSize;

extern s32 StClearRing(void);

s32 StSetRing(s32 arg0, s32 arg1) {
    StRingAddr = arg0;
    StRingSize = arg1;
    return StClearRing();
}
