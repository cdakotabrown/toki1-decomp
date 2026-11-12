/* Function: SsUtSetDetVVol */
/* Address: 0x8008F69C */
/* Size: 112 bytes (28 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x8008F69C */
/* @SymbolSize: 112 */
/* @CompileEnv: IDO52_AS1_TIE_REJECT=4,5 IDO52_AS1_NO_SHIFT_COMBINE=all IDO52_UGEN_TEMP_REG=1:t9,2:t0,3:t1 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=1 IDO52_FORCE_WEB_COLOR=6:a3,14:v1,15:a1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern u8 _svm_sreg_dirty[24];
extern s16 _svm_sreg_buf[];

s32 SsUtSetDetVVol(s16 vc, s16 v0, s16 v1)
{
    s32 idx;

    if (vc < 24) {
        if (vc >= 0) {
            _svm_sreg_dirty[vc] |= 3;
            idx = vc * 8;
            _svm_sreg_buf[idx] = v0;
            _svm_sreg_buf[idx + 1] = v1;
            return 0;
        }
    }
    return -1;
}
