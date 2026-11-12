/* Function: SsSeqPause  (was filed as func_8009085C, the PROLOGUE address; */
/* Address: 0x80090850 */
/* Size: 148 bytes (37 instructions) */
/* Region: Library (0x80086000+) */
/* @SymbolStart: 0x80090850 */
/* @SymbolSize: 148 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0xc 0x1c */
/* @PostProc: swap_instructions 0x10 0x1c */
/* @PostProc: swap_instructions 0x14 0x1c */
/* @PostProc: swap_instructions 0x18 0x20 */
#include "types.h"

typedef struct {
    u8 pad1[118];
    u8 field118;
    u8 pad2;
    u8 field120;
    u8 pad3[23];
    u32 flags;
} SoundStruct;

extern SoundStruct *_ss_score[];
extern void SpuVmGetSeqVol(s32, u8 *, u8 *);

void SsSeqPause(s16 index) {
    SoundStruct *ptr;

    ptr = _ss_score[index];
    SpuVmGetSeqVol(index, &ptr->field118, &ptr->field120);
    ptr = _ss_score[index];
    ptr->flags &= ~1;
    ptr = _ss_score[index];
    ptr->flags &= ~8;
    ptr = _ss_score[index];
    ptr->flags |= 2;
}
