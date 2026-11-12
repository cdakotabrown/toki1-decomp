/* Function: SsSeqStop */
/* Address: 0x8009012C  (TRUE start 0x80090120 — cross-prologue hoist) */
/* Size: 148 bytes (37 instructions) over the recovered window */
/* Region: Library (0x80086000+) */
/* @SymbolStart: 0x80090120 */
/* @SymbolSize: 148 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_TIE_TAKE=13,23 */
#include "types.h"

extern s32 *_ss_score[];
extern void Snd_stop(s32 arg0, s32 arg1);

void SsSeqStop(s16 arg0) {
    s32 *ptr;
    ptr = _ss_score[arg0];
    ptr[36] = ptr[36] & ~1;
    ptr = _ss_score[arg0];
    ptr[36] = ptr[36] & ~2;
    ptr = _ss_score[arg0];
    ptr[36] = ptr[36] & ~8;
    ptr = _ss_score[arg0];
    ptr[36] = ptr[36] | 4;
    Snd_stop(arg0, 0);
}
