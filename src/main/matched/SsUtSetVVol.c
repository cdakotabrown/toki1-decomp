/* Address: 0x8008F790 */
/* Size: 156 bytes (39 instructions) */
/* Region: Library (0x80086000+), compiler IDO 5.2 (t6-t9 temps, no `j`). */
/* @SymbolSize: 156 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=1:t3,9:t1,3:t5,2:t4 IDO52_AS1_TIE_REJECT=4,5 IDO52_AS1_NO_SHIFT_COMBINE=all IDO52_CONST_NOINREG_VALUES=127,16383 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_FALLTHRU_FILL=1 IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=6:a3,14:v1,15:a1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern u8 _svm_sreg_dirty[24];
extern s16 _svm_sreg_buf[];

s32 SsUtSetVVol(s16 vc, s16 volL, s16 volR)
{
    s32 idx;

    if (vc < 24) {
        if (vc >= 0) {
            _svm_sreg_dirty[vc] |= 3;
            idx = vc * 8;
            _svm_sreg_buf[idx] = (s16)((volL * 16383) / 127);
            _svm_sreg_buf[idx + 1] = (s16)((volR * 16383) / 127);
            return 0;
        }
    }
    return -1;
}
