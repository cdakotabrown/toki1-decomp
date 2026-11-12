/* Address: 0x80088E10  Size: 184 bytes (46 insns; two alignment nops to 0x80088ED0) -- the old 88-byte window ended at */
/* @SymbolSize: 184 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 StRingAddr;
extern s32 StRingSize;
extern s32 StRingIdx3;

s32 StFreeRing(s32 addr)
{
    s32 base;
    s32 delta;
    s32 idx;
    s16 *entry;
    s32 n;
    s32 i;

    base = StRingSize * 32 + StRingAddr;
    delta = addr - base;
    idx = (delta >> 2) / 504;
    entry = (s16 *)(StRingAddr + idx * 32);
    n = ((u16 *)entry)[3];
    if (entry[0] != 4) {
        return 1;
    }
    for (i = 0; i < (s16)n; i++) {
        *(s16 *)(StRingAddr + (i + idx) * 32) = 0;
    }
    StRingIdx3 = i + idx;
    return 0;
}
