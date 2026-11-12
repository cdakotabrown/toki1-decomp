/* Function: SsUtChangeADSR */
/* Address: 0x8008F580 */
/* Size: 200 bytes (50 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x8008F580 */
/* @SymbolSize: 200 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_FALLTHRU_FILL=1 IDO52_AS1_NO_SHIFT_COMBINE=all IDO52_AS1_NO_LA_PULL=all IDO52_UGEN_TEMP_REG=13:t5,12:t8,11:t7,10:t6 IDO52_AS1_TIE_REJECT=6,7,8,39,40 IDO52_AS1_TIE_TAKE=33 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa0 */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    u8  unk0[8];
    s16 unk8;
    u8  unkA[4];
    s16 unkE;
    s16 unk10;
    s16 unk12;
    u8  unk14[4];
} SpuVoiceParam;

extern SpuVoiceParam _svm_voice[24];
extern u8 _svm_sreg_dirty[24];
extern s16 _svm_sreg_buf[];

s32 SsUtChangeADSR(s16 vc, s16 p12, s16 pE, s16 p8, u16 lo, u16 hi)
{
    SpuVoiceParam *t;
    s16 *r;
    s32 idx;

    if (vc < 24) {
        if (vc >= 0) {
            t = &_svm_voice[vc];
            if (p12 == t->unk12) {
                if (pE == t->unkE) {
                    if (p8 == t->unk8) {
                        idx = vc * 8;
                        r = _svm_sreg_buf;
                        r += idx;
                        r[4] = lo;
                        _svm_sreg_dirty[vc] |= 0x30;
                        r[5] = hi;
                        return 0;
                    }
                }
            }
        }
    }
    return -1;
}
