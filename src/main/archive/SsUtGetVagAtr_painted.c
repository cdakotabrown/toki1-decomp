/* Function: SsUtGetVagAtr */
/* Address: 0x80094E04 */
/* Size: 532 bytes (133 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: custom_script custom_80094E04.py */
#include "types.h"

extern s8 libsnd_cur_fake_program;
extern u8 * volatile _svm_tn;
extern void SpuVmVSetUp(void);

s32 SsUtGetVagAtr(s32 a0, s32 a1, s32 a2, u8 *a3) {
    s16 scaled;

    a1 = (s16)a1;
    a2 = (s16)a2;
    if (a0 != 1) {
        goto fail;
    }
    SpuVmVSetUp();
    scaled = (s16)((s32)(libsnd_cur_fake_program << 4) + a2);
    a3[0] = _svm_tn[scaled * 32 + 0];
    a3[1] = _svm_tn[scaled * 32 + 1];
    a3[2] = _svm_tn[scaled * 32 + 2];
    a3[3] = _svm_tn[scaled * 32 + 3];
    a3[4] = _svm_tn[scaled * 32 + 4];
    a3[5] = _svm_tn[scaled * 32 + 5];
    a3[7] = _svm_tn[scaled * 32 + 7];
    a3[6] = _svm_tn[scaled * 32 + 6];
    a3[8] = _svm_tn[scaled * 32 + 8];
    a3[9] = _svm_tn[scaled * 32 + 9];
    a3[10] = _svm_tn[scaled * 32 + 10];
    a3[11] = _svm_tn[scaled * 32 + 11];
    a3[12] = _svm_tn[scaled * 32 + 12];
    a3[13] = _svm_tn[scaled * 32 + 13];
    *(u16 *)&a3[16] = *(u16 *)&_svm_tn[scaled * 32 + 16];
    *(u16 *)&a3[18] = *(u16 *)&_svm_tn[scaled * 32 + 18];
    *(s16 *)&a3[20] = *(s16 *)&_svm_tn[scaled * 32 + 20];
    *(s16 *)&a3[22] = *(s16 *)&_svm_tn[scaled * 32 + 22];
    return 0;
fail:
    return -1;
}
