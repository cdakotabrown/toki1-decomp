/* Address: 0x80088ED0 */
/* Size: 180 bytes (45 instructions) -- window ends at `jr ra ; nop`; the 3 words to 0x80088F90 are alignment padding */
/* @SymbolSize: 180 */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 StRingIdx3;
extern u8 *StRingAddr;
extern s32 StRingSize;

s32 StGetNext(s32 *outA, s32 *outB)
{
    volatile u16 *entry;

    entry = (volatile u16 *)(StRingAddr + (StRingIdx3 << 5));
    if (entry[0] == 1) {
        StRingIdx3 = 0;
        entry[0] = 0;
        entry = (volatile u16 *)(StRingAddr + (StRingIdx3 << 5));
    }
    if (entry[0] != 2) {
        return 1;
    }
    entry[0] = 4;
    *outA = (s32)(StRingAddr + (StRingSize << 5) + ((StRingIdx3 * 63) << 5));
    *outB = (s32)entry;
    return 0;
}
