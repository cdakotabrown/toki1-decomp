/* @SymbolStart: 0x800908E4 */
/* @SymbolSize: 244 */
/* Address: 0x800908F8 */
/* Size: 224 bytes (56 instructions) */
/* Region: Library (0x80086000+), compiler IDO 5.2 (t6-t9 temp allocation, */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: convert_or_to_addu */
/* @Measured: 2026-09-19 1/61 exact (1.8%), compiled 53 vs target 61 words, Fixup 0 -- SEMI */
#include "types.h"

extern s8 *_ss_score[];
extern void SpuVmGetSeqVol(s16 a0, u16 *a1, u16 *a2);

void func_800908F8(s16 arg0, s16 arg1)
{
    s8 **pp;

    pp = &_ss_score[arg0];
    SpuVmGetSeqVol(arg0, (u16 *)(*pp + arg1 * 168 + 118),
                  (u16 *)(*pp + arg1 * 168 + 120));
    *(s32 *)(*pp + arg1 * 168 + 144) &= ~1;
    *(s32 *)(*pp + arg1 * 168 + 144) &= ~8;
    *(s32 *)(*pp + arg1 * 168 + 144) |= 2;
}
