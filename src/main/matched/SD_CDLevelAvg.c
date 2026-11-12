/* Address: 0x8007C4A4 */
/* Size: 264 bytes (66 instructions)  [the old 332-byte window spanned into */
/* Region: Game (IDO 5.2) */
/* 1. `@CompileEnv: IDO52_UGEN_TEMP_REG=19:t4,18:t6` -- a greedy Hook 36 */
/* @SymbolStart: 0x8007C4A4 */
/* @SymbolSize: 264 */
/* @CompileEnv: IDO52_AS1_TIE_TAKE=47,52 IDO52_AS1_TIE_REJECT=53 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s16 gSDCDLevelTbl[];

void SD_CDLevelAvg(out1, out2, start, count)
    u16 *out1;
    u16 *out2;
    u16 start;
    u16 count;
{
    s32 sumA;
    s32 sumB;
    u16 i;
    u16 step;
    s16 a;
    s16 b;

    sumA = 0;
    sumB = 0;
    i = 0;
    if (count != 0) {
        step = 256 / count;
        do {
            a = gSDCDLevelTbl[start];
            b = gSDCDLevelTbl[start + 512];
            if (a & 0x8000) {
                a = ~(a - 1);
            }
            sumA += a;
            if (b & 0x8000) {
                b = ~(b - 1);
            }
            sumB += b;
            start += step;
        } while (++i < count);
    }

    *out1 = (s16)(u16)(sumA * 2 / count);
    *out2 = (s16)(u16)(sumB * 2 / count);
}
