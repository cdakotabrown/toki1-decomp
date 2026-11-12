/* Address: 0x8008FA10   (the recorded 0x8008FA18 is the PROLOGUE; the two */
/* Size: 180 bytes (45 instructions).  TRUE SIZE IS 180, not the 188/196 */
/* Region: Library (0x80086000+) */
/* @SymbolStart: 0x8008FA10 */
/* @SymbolSize: 180 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_COPY_PROP=2 IDO52_AS1_XBB_SKIP_CAND=1,2 IDO52_AS1_NO_FALLTHRU_FILL=0,1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern s32 _svm_rattr;
extern s32 libsnd_reverb_type;
extern void SpuSetReverb(s32);
extern void SpuSetReverbModeParam(s32 *);

s32 SsUtSetReverbType(s16 arg0)
{
    s32 flag;
    s16 pad_;
    s16 val;

    flag = 0;
    val = arg0;
    if (arg0 < 0) {
        flag = 1;
        val = -arg0;
    }

    if (val < 0) {
        goto err;
    }
    if (val >= 10) {
        goto err;
    }

    _svm_rattr = 1;
    if (flag != 0) {
        libsnd_reverb_type = val | 0x100;
    } else {
        libsnd_reverb_type = val;
    }

    if (val == 0) {
        SpuSetReverb(0);
    }

    SpuSetReverbModeParam(&_svm_rattr);
    return val;
err:
    return -1;
}
