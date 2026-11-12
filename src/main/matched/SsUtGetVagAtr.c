/* @SymbolStart: 0x80094DF0 */
/* @SymbolSize: 556 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_NO_CSE_SITE=mpy:1-16 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
#include "types.h"

extern s8 libsnd_cur_fake_program;
typedef struct {
    u8  b0, b1, b2, b3, b4, b5, b6, b7;
    u8  b8, b9, b10, b11, b12, b13;
    u8  pad0[2];
    u16 h16;
    u16 h18;
    s16 h20;
    s16 h22;
    u8  pad1[8];
} Rec;

extern Rec *_svm_tn;
extern u8 _svm_vab_used[];
extern void SpuVmVSetUp();

s32 SsUtGetVagAtr(a0, a1, a2, a3)
short a0;
short a1;
short a2;
u8 *a3;
{
    short scaled;

    if (_svm_vab_used[a0] != 1) {
        goto fail;
    }
    SpuVmVSetUp(a0, a1);
    scaled = (libsnd_cur_fake_program << 4) + a2;
    a3[0] = _svm_tn[scaled].b0;
    goto L1; L1:
    a3[1] = _svm_tn[scaled].b1;
    goto L2; L2:
    a3[2] = _svm_tn[scaled].b2;
    goto L3; L3:
    a3[3] = _svm_tn[scaled].b3;
    goto L4; L4:
    a3[4] = _svm_tn[scaled].b4;
    goto L5; L5:
    a3[5] = _svm_tn[scaled].b5;
    goto L6; L6:
    a3[7] = _svm_tn[scaled].b7;
    goto L7; L7:
    a3[6] = _svm_tn[scaled].b6;
    goto L8; L8:
    a3[8] = _svm_tn[scaled].b8;
    goto L9; L9:
    a3[9] = _svm_tn[scaled].b9;
    goto L10; L10:
    a3[10] = _svm_tn[scaled].b10;
    goto L11; L11:
    a3[11] = _svm_tn[scaled].b11;
    goto L12; L12:
    a3[12] = _svm_tn[scaled].b12;
    goto L13; L13:
    a3[13] = _svm_tn[scaled].b13;
    goto L14; L14:
    *(u16 *)&a3[16] = _svm_tn[scaled].h16;
    goto L15; L15:
    *(u16 *)&a3[18] = _svm_tn[scaled].h18;
    goto L16; L16:
    *(s16 *)&a3[20] = _svm_tn[scaled].h20;
    goto L17; L17:
    *(s16 *)&a3[22] = _svm_tn[scaled].h22;
    return 0;
fail:
    return -1;
}
