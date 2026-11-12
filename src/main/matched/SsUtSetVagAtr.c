/* @SymbolStart: 0x80094BC0 */
/* @SymbolSize: 556 */
/* Function: SsUtSetVagAtr */
/* Address: 0x80094BD4 */
/* Size: 532 bytes (133 instructions) */
/* Region: Library (0x80086000+) */
/* carried `@PostProc: convert_or_to_addu` alone, which disables the region */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_NO_CSE_SITE=mpy:1-16 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
#include "types.h"

typedef struct {
    u8 b[14];
    u8 pad[2];
    u16 h16;
    u16 h18;
    s16 h20;
    s16 h22;
    u8 pad2[8];
} DctEnt;

extern s8 libsnd_cur_fake_program;
extern u8 *_svm_tn;
extern u8 _svm_vab_used[];
extern void SpuVmVSetUp();

s32 SsUtSetVagAtr(a0, a1, a2, a3)
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
    _svm_tn[scaled * 32 + 0] = a3[0];
    goto L1; L1:
    _svm_tn[scaled * 32 + 1] = a3[1];
    goto L2; L2:
    _svm_tn[scaled * 32 + 2] = a3[2];
    goto L3; L3:
    _svm_tn[scaled * 32 + 3] = a3[3];
    goto L4; L4:
    _svm_tn[scaled * 32 + 4] = a3[4];
    goto L5; L5:
    _svm_tn[scaled * 32 + 5] = a3[5];
    goto L6; L6:
    _svm_tn[scaled * 32 + 7] = a3[7];
    goto L7; L7:
    _svm_tn[scaled * 32 + 6] = a3[6];
    goto L8; L8:
    _svm_tn[scaled * 32 + 8] = a3[8];
    goto L9; L9:
    _svm_tn[scaled * 32 + 9] = a3[9];
    goto L10; L10:
    _svm_tn[scaled * 32 + 10] = a3[10];
    goto L11; L11:
    _svm_tn[scaled * 32 + 11] = a3[11];
    goto L12; L12:
    _svm_tn[scaled * 32 + 12] = a3[12];
    goto L13; L13:
    _svm_tn[scaled * 32 + 13] = a3[13];
    goto L14; L14:
    *(u16 *)&_svm_tn[scaled * 32 + 16] = *(u16 *)&a3[16];
    goto L15; L15:
    *(u16 *)&_svm_tn[scaled * 32 + 18] = *(u16 *)&a3[18];
    goto L16; L16:
    *(s16 *)&_svm_tn[scaled * 32 + 20] = *(s16 *)&a3[20];
    goto L17; L17:
    *(s16 *)&_svm_tn[scaled * 32 + 22] = *(s16 *)&a3[22];
    return 0;
fail:
    return -1;
}
