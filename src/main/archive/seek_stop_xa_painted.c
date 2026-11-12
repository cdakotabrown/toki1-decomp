/* Function: seek_stop_xa */
/* Address: 0x8008F478 */
/* Size: 264 bytes (66 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: convert_or_to_addu */
/* @PostProc: custom_script custom_func_8008F478.py */
#include "types.h"

extern s8 _svm_voice[];
extern u8 _svm_sreg_dirty[];
extern s16 _svm_sreg_buf[];
extern s16 D_80131606;
extern s16 D_8013160A;
extern u8 libsnd_cur_tone_idx;

extern s32 SpuVmVSetUp(s16 a0, s16 a1);
extern s16 note2pitch2(u16 a0, u16 a1);

s32 seek_stop_xa(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, u16 arg5, u16 arg6) {
    s8 *entry;
    (void)&arg0;

    if (arg0 >= 24) {
        return -1;
    }
    if (arg0 < 0) {
        return -1;
    }

    entry = _svm_voice + arg0 * 24;

    if (arg1 != *(s16 *)(entry + 18)) {
        return -1;
    }
    if (arg2 != *(s16 *)(entry + 14)) {
        return -1;
    }
    if (arg3 != *(s16 *)(entry + 8)) {
        return -1;
    }

    SpuVmVSetUp(arg1, arg2);

    D_80131606 = 32;
    D_8013160A = arg0;
    libsnd_cur_tone_idx = (u8)*(s16 *)(entry + 16);

    _svm_sreg_buf[arg0 * 8] = note2pitch2(arg5, arg6);
    _svm_sreg_dirty[arg0] |= 4;

    return 0;
}
