/* Address: 0x80095040 */
/* Address: recorded 0x80095058 -- TRUE ENTRY 0x80095040 (jal-verified from */
/* Size: 416 bytes (incl. trailing alignment pad; body ends 0x800951D0) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x80095040 */
/* @SymbolSize: 416 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=168 IDO52_NO_CSE_SITE=mpy:2,mpy:4,mpy:6,mpy:8,mpy:10,mpy:12 IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 *_ss_score[];
extern void SpuVmSetSeqVol(s16, s32, s32, s32);

void Snd_nextseq(s16 a0, s16 a1)
{
    u8 *entry;

    entry = *(u8 **)((s32)_ss_score + a0 * 4) + a1 * 168;
    *(s16 *)(entry + 68) = 1;
    *(s16 *)(entry + 70) = 0;

    SpuVmSetSeqVol(a0, 127, 127, a1);

    *(s32 *)(*(u8 **)((s32)_ss_score + a0 * 4) + a1 * 168 + 144) &= ~0x100;
    goto L1; L1:
    *(s32 *)(*(u8 **)((s32)_ss_score + a0 * 4) + a1 * 168 + 144) &= ~0x8;
    goto L2; L2:
    *(s32 *)(*(u8 **)((s32)_ss_score + a0 * 4) + a1 * 168 + 144) &= ~0x2;
    goto L3; L3:
    *(s32 *)(*(u8 **)((s32)_ss_score + a0 * 4) + a1 * 168 + 144) &= ~0x4;
    goto L4; L4:
    *(s32 *)(*(u8 **)((s32)_ss_score + a0 * 4) + a1 * 168 + 144) &= ~0x200;
    goto L5; L5:
    entry[43] = 1;
    *(s32 *)(*(u8 **)((s32)_ss_score + a0 * 4) + a1 * 168 + 144) |= 0x1;
}
