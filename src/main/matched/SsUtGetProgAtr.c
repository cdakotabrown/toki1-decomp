/* Function: SsUtGetProgAtr */
/* Address: 0x80094AE4 (recorded; the prologue) */
/* Size: 216 bytes (54 instructions) */
/* @SymbolStart: 0x80094AD0 */
/* @SymbolSize: 236 */
/* Region: Library (0x80086000+) */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_UGEN_TEMP_REG=19:t0,24:t1,27:t0 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    u8 b4;
    u8 pad5;
    s16 h6;
    u8 pad8[8];
} Myu;

extern u8 _svm_vab_used[];
extern Myu *_svm_pg;

extern void SpuVmVSetUp(void);

s32 SsUtGetProgAtr(s16 id, s16 idx, Myu *out) {
    if (_svm_vab_used[id] == 1) {
        SpuVmVSetUp();
        out->b0 = _svm_pg[idx].b0;
        out->b1 = _svm_pg[(u32)idx].b1;
        out->b2 = ((Myu *)((u8 *)_svm_pg + (idx << 4)))->b2;
        out->b3 = ((Myu *)((u8 *)_svm_pg + ((u32)idx << 4)))->b3;
        out->b4 = ((Myu *)((u8 *)_svm_pg + ((idx * 4) << 2)))->b4;
        out->h6 = ((Myu *)((u8 *)_svm_pg + ((idx << 2) * 4)))->h6;
        return 0;
    }
    return -1;
}
