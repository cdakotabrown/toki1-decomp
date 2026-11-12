/* Address: 0x8008FD70 */
/* Size: 272 bytes (68 instructions) */
/* Region: Library (0x80086000+) */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_FALLTHRU_FILL=1 IDO52_AS1_NO_LA_PULL=all */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s32 w0;
    s32 w4;
    s32 w8;
    u8 pad0C[6];
    u16 h12;
    u16 h14;
    u16 h16;
    u8 b18;
    u8 b19;
    u8 b1A;
    u8 b1B;
} Voice;

extern u8 _svm_vab_used[];
extern Voice *_svm_vab_vh[];
extern Voice *_svm_vh;

s32 SsUtGetVabHdr(s16 voice, Voice *dst)
{
    if (_svm_vab_used[voice] == 1) {
    _svm_vh = _svm_vab_vh[voice];
    dst->w0 = _svm_vh->w0;
    dst->w8 = _svm_vh->w8;
    dst->w4 = _svm_vh->w4;
    dst->h12 = _svm_vh->h12;
    dst->h14 = _svm_vh->h14;
    dst->h16 = _svm_vh->h16;
    dst->b18 = _svm_vh->b18;
    dst->b19 = _svm_vh->b19;
    dst->b1A = _svm_vh->b1A;
    dst->b1B = _svm_vh->b1B;
    return 0;
    }
    return -1;
}
