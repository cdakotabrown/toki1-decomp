/* Address: 0x8008CDE0 */
/* Size: 180 bytes (45 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileFlags: -cckr -Wc,-pic0 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_SHIFT_COMBINE=all IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=12:a1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    u8 pad0[6];
    u8 min;
    u8 max;
    u8 pad8[14];
    s16 vag;
    u8 pad24[8];
} VagAtr;

extern s8 _svm_cur;
extern s8 D_801315F2;
extern s8 libsnd_cur_fake_program;
extern VagAtr *_svm_tn;

u8 SpuVmSelectToneAndVag(vag_nums, tone_nums)
u8 *vag_nums;
u8 *tone_nums;
{
    u8 n;
    s8 i;

    n = 0;
    for (i = 0; i < _svm_cur; i++) {
        if (D_801315F2 >= (_svm_tn + libsnd_cur_fake_program * 16)[i].min
            && D_801315F2 <= (_svm_tn + libsnd_cur_fake_program * 16)[i].max) {
            tone_nums[n] = (_svm_tn + libsnd_cur_fake_program * 16)[i].vag;
            vag_nums[n] = i;
            n++;
        }
    }
    return n;
}
