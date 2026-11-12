/* Address: 0x8008EF3C */
/* Size: 272 bytes (68 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: scheduler_normalizer */
/* @PostProc: custom_script custom_8008EF3C.py */
#include "types.h"

extern s32 _snd_ev_flag;
extern s16 D_8013160A;
extern s8 _svm_voice[];
extern void vmNoiseOff(s32);
extern void SpuVmKeyOffNow(s32);

s32 func_8008EF3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s8 *ch;
    s32 v1;

    v1 = (s16)arg0;
    arg1 = (s16)arg1;
    arg2 = (s16)arg2;
    arg3 = (s16)arg3;

    if (_snd_ev_flag == 1) {
        return -1;
    }

    _snd_ev_flag = 1;

    if (v1 >= 24) goto err;
    if (v1 < 0) goto err;

    ch = _svm_voice + (v1 * 4 - v1) * 8;

    if (arg1 != *(s16 *)(ch + 18)) goto err;
    if (arg2 != *(s16 *)(ch + 14)) goto err;
    if (arg3 != *(s16 *)(ch + 16)) goto err;
    if (arg4 != *(s16 *)(ch + 8)) goto err;

    if (*(s16 *)(ch + 0) == 255) {
        vmNoiseOff(v1 & 0xFF);
    } else {
        D_8013160A = (s16)v1;
        SpuVmKeyOffNow(0);
    }

    _snd_ev_flag = 0;
    return 0;

err:
    _snd_ev_flag = 0;
    return -1;
}
